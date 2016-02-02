<?php


function uwwtd_page_stats_querys()
{
	$title = 'Querys for national stats';
	drupal_set_title($title);  //todo
	$options = array('year'=> 2012);
	$data = uwwtd_stat_sewage_network_data($options);
	return 'fin';
}

/**
 * Récupère en base les données pour Graph 1 de la ligne 1.
 */
function uwwtd_stat_collect_data($yearSelected, $yearsPossibles){
	switch ($yearSelected) {
		case ALL_YEARS :
			$conditions .= " and f_annee.field_anneedata_value in " . getInSql($yearsPossibles);
			break;
		case '' :
			break;
		default :
			$conditions .= " and  f_annee.field_anneedata_value = '" . $yearSelected . "'";
			break;
	}

	if (variable_get('display_only_active', '1') == 1) {
        $conditions .= "  AND f_status.field_status_value = '1' ";
    }   
	$params = array(
		':typenode' => 'agglomeration',
		':agggenerated' => variable_get('min_aggenerated_display', '2000'), 
	);  

	$result = db_query("
		SELECT
			f_annee.field_anneedata_value as year,
			SUM (g.field_agggenerated_value) AS \"Generated load\",
			SUM ((g.field_agggenerated_value*c1.field_aggc1_value)/100) AS cs,
			SUM ((g.field_agggenerated_value*c2.field_aggc2_value)/100) AS ias,
			SUM ((g.field_agggenerated_value*wot.field_aggpercwithouttreatment_value)/100) as wot
		FROM {node n
			LEFT JOIN {field_data_field_anneedata} f_annee ON n.nid = f_annee.entity_id
			LEFT JOIN {field_data_field_status} f_status ON n.nid = f_status.entity_id 
			LEFT JOIN {field_data_field_agggenerated} g ON n.vid = g.revision_id
			LEFT JOIN {field_data_field_aggc1} c1  ON n.vid = c1.revision_id
			LEFT JOIN {field_data_field_aggc2} c2 ON n.vid = c2.revision_id
			LEFT JOIN {field_data_field_aggpercwithouttreatment} wot ON n.vid = wot.revision_id
		WHERE n.type = :typenode
			AND n.status = 1
			AND g.field_agggenerated_value > :agggenerated". 
			$conditions
	 ." GROUP BY f_annee.field_anneedata_value
		ORDER BY f_annee.field_anneedata_value", 
	$params);
	
	$out = array();
	if ($dataByYears = $result->fetchAllAssoc('year')) {
		// Pour chaque ligne retournée :
		foreach ($dataByYears as $year => $dataByYear) {
			// Pour chaque colonne de la ligne :
			foreach ($dataByYear as $key => $data) {
				if ($key != 'year') {
					$out[$year][] = array(
						"value" => (float)$data,
						"label" => $GLOBALS['ms_level']['connection'][$key]['label'],
						"valueformat" => uwwtd_format_number(round($data, 0)).' pe',
						"color" => $GLOBALS['ms_level']['connection'][$key]['color'],
					);
				}
			}
		}
	}
	
	return $out;
}

/**
 * Récupère en base les données pour Graph 2 de la ligne 1.
 */
function uwwtd_stat_sewage_network_data($yearSelected, $yearsPossibles){
	switch ($yearSelected) {
		case ALL_YEARS :
			$conditions .= " and f_annee.field_anneedata_value in " . getInSql($yearsPossibles);
			break;
		case '' :
			break;
		default :
			$conditions .= " and  f_annee.field_anneedata_value = '" . $yearSelected . "'";
			break;
	}
	if (variable_get('display_only_active', '1') == 1) {
		$conditions .= "  AND f_status.field_status_value = '1' ";
	}
	$params = array(
		':typenode' => 'agglomeration',
		':agggenerated' => variable_get('min_aggenerated_display', '2000'),
	);
	try {
		$result = db_query("
			SELECT f_annee.field_anneedata_value as year,
				s.field_aggsewagenetwork_value AS type, 
				SUM(g.field_agggenerated_value) AS sn_sum,  
				COUNT (*) AS sn_count
			FROM {node} n
				LEFT JOIN {field_data_field_anneedata} f_annee ON n.nid = f_annee.entity_id
				LEFT JOIN {field_data_field_agggenerated} g ON n.vid = g.revision_id
				LEFT JOIN {field_data_field_status} f_status ON n.nid = f_status.entity_id
				LEFT JOIN {field_data_field_aggsewagenetwork} s ON n.nid = s.entity_id
			WHERE n.type = :typenode
				AND n.status = 1
				AND g.field_agggenerated_value > :agggenerated ".
				$conditions."
		GROUP BY f_annee.field_anneedata_value, s.field_aggsewagenetwork_value
		ORDER BY f_annee.field_anneedata_value"
				, $params);

		$out = array();
		while ($row = $result->fetchAssoc()) {
			if ($row['type'] == '') {
				$row['type'] = 'NI';
			}
			$networkTypeName = $GLOBALS['ms_level']['sewage_network'][$row['type']]['label'];
			$out[$networkTypeName]['data'][$row['year']] = array(
				'equValue' => $row['sn_sum'],
				'equFormated' => uwwtd_format_number((integer)$row['sn_sum']). ' pe',
				'aggValue' => $row['sn_count'],
				'aggFormated' => uwwtd_format_number((integer)$row['sn_count']),
				'color' => $GLOBALS['ms_level']['sewage_network'][$row['type']]['color'],
			);
		}
	} catch(Exception $e) {
		dsm($e->getMessage());
	}
	
	return $out;
}

/**
 * Récupère en base les données pour Graph 1 et 2 de la ligne 2.
 */ 
function uwwtd_stat_national_compliance($yearSelected, $yearsPossibles) {
	$data = array();
	$param = array();
	$conditions = '';
	switch ($yearSelected) {
		case ALL_YEARS :
			$conditions .= " and f_annee.field_anneedata_value in " . getInSql($yearsPossibles);
			break;
		case '' :
			break;
		default :
			$conditions .= " and  f_annee.field_anneedata_value = '" . $yearSelected . "'";
			break;
	}

	if (variable_get('display_only_active', '1') == 1) {
		$conditions .= "  AND f_status.field_status_value = '1' ";
	}

	$query = 'select 
		 f_annee.field_anneedata_value as year,
		 f_comp.field_aggcompliance_value,
		 COUNT(n.title) AS  TOTAL_COUNT,
		 SUM(f_aggc2.field_aggc2_value) AS AGGC2_SUM,
         SUM(f_aggc1.field_aggc1_value) AS AGGC1_SUM,
         SUM(f_aggpercwithouttreatment.field_aggpercwithouttreatment_value) AS AGGPCWT_SUM,
         SUM(f_agggenerated.field_agggenerated_value) AS GENERATED_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        left join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        left join
            {field_data_field_aggart3compliance} f_art3
                ON n.nid = f_art3.entity_id
        left join
            {field_data_field_aggart4compliance} f_art4
                ON n.nid = f_art4.entity_id
        left join
            {field_data_field_aggart5compliance} f_art5
                ON n.nid = f_art5.entity_id
        left join
            {field_data_field_aggart6compliance} f_art6
                ON n.nid = f_art6.entity_id
        left join
            {field_data_field_aggcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_regionnuts} f_nuts
                ON n.nid = f_nuts.entity_id
        left join
            {field_data_field_aggc2} f_aggc2
                ON n.nid = f_aggc2.entity_id
        left join
            {field_data_field_aggc1} f_aggc1
                ON n.nid = f_aggc1.entity_id
        left join
            {field_data_field_aggpercwithouttreatment} f_aggpercwithouttreatment
                ON n.nid = f_aggpercwithouttreatment.entity_id
        left join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		where n.type = :typenode
        and f_agggenerated.field_agggenerated_value > :agggenerated '.
    	$conditions .
    	' group BY f_annee.field_anneedata_value, f_comp.field_aggcompliance_value
    	order by f_annee.field_anneedata_value';

    $param = array(
    	':typenode' => 'agglomeration',
    	':agggenerated' => variable_get('min_aggenerated_display', '2000'),
    );
    try {
    	$result = db_query($query,$param);
    	while($row = $result->fetchAssoc()) {
    		$complianceState = $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggcompliance_value']];
    		$data[$complianceState]['data']['Compliance state'] = $complianceState;
    		if (is_null($row['generated_sum']) || is_null($row['total_count'])) {
    										// (pe, pe formatée, nombre d'agglo, nombre d'agglo formatée)
    			$data[$complianceState]['data'][(string)$row['year']] = array('pe'=>0, 'pe_format'=>'0', 'nb'=>0, 'nb_format'=>'0', 'color'=>$GLOBALS['uwwtd']['compliance_color'][$row['field_aggcompliance_value']]);
    		} else {
    			$data[$complianceState]['data'][(string)$row['year']] = array(
                    'pe'=>$row['generated_sum'], 
    				'pe_format'=>uwwtd_format_number($row['generated_sum'], 0) .' pe', 
    				'nb'=>$row['total_count'],
    				'nb_format'=>uwwtd_format_number($row['total_count'], 0),
                    'color'=>$GLOBALS['uwwtd']['compliance_color'][$row['field_aggcompliance_value']]
    			);
    		}
    	}
    } catch(Exception $e) {
    	dsm($e->getMessage());
    }
    if(!empty($data)){
        $keys_order = $GLOBALS['uwwtd']['all_compliance'];
        $odata;
        foreach($keys_order as $k=>$v){
            if(isset($data[$v])){
                $odata[$v] = $data[$v];
            }
        }
        //$data = array_replace(array_flip($keys_order), $data);
        return $odata;
    }
    
    return $data;
}

/**
 * Récupère en base les données pour Graph 1 et 2 de la ligne 3 et 4.
 */
//====FIX ME nd@oieau.fr : In Ireland for year = 2010, the dataset contain all uwwtp (agggeneratedload > 2000 )
// We need to exclude all uwwtp link to agglomeration < 2000 pe but not with double count
function uwwtd_stat_national_uwwtp($yearSelected, $yearsPossibles, $groupBy) {
	$conditions = '';
	switch($yearSelected){
		case ALL_YEARS :
			$conditions .= " and f_annee.field_anneedata_value in " . getInSql($yearsPossibles);
			break;
		case '' :
			break;
		default :
			$conditions .= " and f_annee.field_anneedata_value = '" . $yearSelected . "'";
			break;
	}
	if (variable_get('display_only_active', '1') == 1) {
		$conditions .= "  AND f_status.field_status_value = '1' ";
	}
	switch ($groupBy){
		case 'compliance' :
			$group_by = 'f_comp.field_uwwcompliance_value';
			break;
		case 'treatmenttype' :
			$group_by = 'f_uwwtreatmenttype.field_uwwtreatmenttype_value';
			break;
		default:
			$group_by = 'f_comp.field_uwwcompliance_value';
	}
	$data = array();
    
    //Filter uwwtp with "min_aggenerated_display" parameter
    
    $with = 'WITH uwwtp_min_agg AS(
        SELECT n.nid, n.type, n.title 
        FROM {node} n
            LEFT join
              {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
            left join
              {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
            LEFT join
              {field_data_field_agguwwliste} f_uwwtp_agglo
                ON f_uwwtp_agglo.field_agguwwliste_nid = n.nid
            LEFT join
            {field_data_field_agggenerated} f_agggenerated
                ON f_uwwtp_agglo.entity_id = f_agggenerated.entity_id
        WHERE n.type = :typenode
            AND f_agggenerated.field_agggenerated_value >= :min_agg
            '.$conditions.'
        GROUP BY n.nid
        )
    ';
    
	$query = $with. 'select f_annee.field_anneedata_value as year, ' . $group_by . ',';
	$query .= ' COUNT(n.title) AS  TOTAL_COUNT,
			SUM(f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value) AS LOADENTERING_SUM
    from {node} n
        join uwwtp_min_agg
            ON n.nid = uwwtp_min_agg.nid 
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        left join
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        left join
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        left join
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        left join
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        left join
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        left join
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        left join
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        left join
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        left join
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        left join
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        left join
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        left join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        left join
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        left join
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        left join
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        left join
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        left join
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        left join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		where n.type = :typenode

    ' .
    $conditions . '
		group by f_annee.field_anneedata_value,' . $group_by;

    $param = array(
        ':typenode' => 'uwwtp',
        ':min_agg' => variable_get('min_aggenerated_display', '2000'),
    );
    try {
//     	dsm($query);
        $result = db_query($query,$param);
    	while ($row = $result->fetchAssoc()) {
    		switch ($groupBy){
    			case 'compliance' :
    				$complianceState = $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_uwwcompliance_value']];
                    $color = $GLOBALS['uwwtd']['compliance_color'][$row['field_uwwcompliance_value']];
    				break;
    			case 'treatmenttype' :
    				$complianceState = $GLOBALS['uwwtd']['ui']['treatment_type_in_place'][$row['field_uwwtreatmenttype_value']];
                    $color = $GLOBALS['uwwtd']['treatment_type_color'][$row['field_uwwtreatmenttype_value']];
    				break;
    			default:
    				$complianceState = $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_uwwcompliance_value']];
    		}
    		$data[$complianceState]['data']['Compliance state'] = $complianceState;
    		if (is_null($row['loadentering_sum']) || is_null($row['total_count'])) {
    			// (pe, pe formatée, nombre d'agglo, nombre d'agglo formatée)
    			$data[$complianceState]['data'][(string)$row['year']] = array('pe'=>0, 'pe_format'=>'0', 'nb'=>0, 'nb_format'=>'0', 'color'=>$color);
    		} else {
    			$data[$complianceState]['data'][(string)$row['year']] = array(
                    'pe'=>$row['loadentering_sum'], 
    				'pe_format'=>uwwtd_format_number($row['loadentering_sum'], 0) .' pe', 
    				'nb'=>$row['total_count'],
    				'nb_format'=>uwwtd_format_number($row['total_count'], 0),
                    'color'=>$color
    			);
    		}
    	}
    }catch(Exception $e) {
    	dsm($e->getMessage());
    }
    /*
    if(!empty($data)){
        switch ($groupBy){
            case 'compliance' :
                $keys_order = $GLOBALS['uwwtd']['all_compliance'];
              break;
            case 'treatmenttype' :
                $keys_order = array_reverse($GLOBALS['uwwtd']['ui']['treatment_type_in_place']);
              break;
        }      
        $data = array_replace(array_flip($keys_order), $data);
    }*/
    return $data;
}

/**
 * Récupère en base les données pour Graph 1 et 2 de la ligne 5.
 */
function uwwtd_stat_national_generated_by_agglomeration($yearSelected, $yearsPossibles) {
	$conditions = '';
	switch ($yearSelected) {
		case ALL_YEARS :
			$conditions .= " and f_annee.field_anneedata_value in " . getInSql($yearsPossibles);
			break;
		case '' :
			break;
		default :
			$conditions .= " and f_annee.field_anneedata_value = '" . $yearSelected . "'";
			break;
	}
	if (variable_get('display_only_active', '1') == 1) {
		$conditions .= " and f_status.field_status_value = '1' ";
	}
	$data = array();
	$param = array();
	$aConditions = array(
		'< 10000 pe' => ' and (f_agggenerated.field_agggenerated_value > 2000 and f_agggenerated.field_agggenerated_value <= 10000) ',
		'10000 to 100000 pe' => ' and (f_agggenerated.field_agggenerated_value > 10000 and f_agggenerated.field_agggenerated_value <= 100000) ',
		'> 100000 pe' => ' and (f_agggenerated.field_agggenerated_value > 100000) ',
	);
	$query = 'select 
		f_annee.field_anneedata_value as year,
		COUNT(f_agggenerated.field_agggenerated_value) AS number,
    	SUM(f_agggenerated.field_agggenerated_value) AS generated_sum
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        left join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        left join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
		where n.type = :typenode
         and f_agggenerated.field_agggenerated_value > :agggenerated 
    ' . $conditions;
	$param = array(
		':typenode' => 'agglomeration',
		':agggenerated' => variable_get('min_aggenerated_display', '2000'),
	);
	try {
		foreach ($aConditions as $libelle => $condition) {
			$data[$libelle]['data']['Size'] = $libelle;
			// Récupération des résultats en base :
			$result = db_query($query . $condition . ' group by f_annee.field_anneedata_value order by f_annee.field_anneedata_value', 
					$param);
			while ($row = $result->fetchAssoc()) {
				if (is_null($row['number']) || is_null($row['generated_sum'])) {
												// (pe, pe formatée, nombre d'agglo, nombre d'agglo formatée)
					$data[$libelle]['data'][(string)$row['year']] = array('pe'=>0, 'pe_format'=>'0', 'nb'=>0, 'nb_format'=>'0', 'color'=>"#aaa");
				} else {
					$data[$libelle]['data'][(string)$row['year']] = array(
                        'pe'=>$row['generated_sum'],
						'pe_format'=>uwwtd_format_number($row['generated_sum'], 0) .' pe',
						'nb'=>$row['number'],
						'nb_format'=>uwwtd_format_number($row['number'], 0),
                        'color'=>$GLOBALS['uwwtd']['uwwtp_size_color'][$libelle]
					);
				}
			}
		}
	} catch(Exception $e) {
		dsm($e->getMessage());
	}
	return $data;
}

/**
 * Récupère en base les données pour le graph 1 et 2 de la ligne 6.
 */
function uwwtd_stat_national_treatmentplant($yearSelected, $yearsPossibles) {
	$conditions = '';
	switch($yearSelected){
		case ALL_YEARS :
			$conditions .= " and f_annee.field_anneedata_value in " . getInSql($yearsPossibles);
			break;
		case '' :
			break;
		default :
			$conditions .= " and  f_annee.field_anneedata_value = '" . $yearSelected . "'";
			break;
	}
	if (variable_get('display_only_active', '1') == 1) {
		$conditions .= "  AND f_status.field_status_value = '1' ";
	}
	$data = array();
	$param = array();
	$query = 'select
		f_annee.field_anneedata_value as year,
		SUM(f_uwwbodincoming.field_uwwbodincoming_value) AS bodin_sum,
		SUM(f_uwwboddischarge.field_uwwboddischarge_value) AS bodout_sum,
		SUM(f_uwwcodincoming.field_uwwcodincoming_value) AS codin_sum,
		SUM(f_uwwcoddischarge.field_uwwcoddischarge_value) AS codout_sum,
		SUM(f_uwwnincoming.field_uwwnincoming_value) AS nin_sum,
		SUM(f_uwwndischarge.field_uwwndischarge_value) AS nout_sum,
		SUM(f_uwwpincoming.field_uwwpincoming_value) AS pin_sum,
		SUM(f_uwwpdischarge.field_uwwpdischarge_value) AS pout_sum,
		SUM(f_uwwwastewatertreated.field_uwwwastewatertreated_value) AS uwwt_sum
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        left join
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        left join
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        left join
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        left join
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        left join
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        left join
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        left join
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        left join
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        left join
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        left join
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        left join
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        left join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        left join
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        left join
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        left join
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        left join
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        left join
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		 left join
            {field_data_field_uwwbodincoming} f_uwwbodincoming
                ON n.nid = f_uwwbodincoming.entity_id
		 left join
            {field_data_field_uwwcodincoming} f_uwwcodincoming
                ON n.nid = f_uwwcodincoming.entity_id
		 left join
            {field_data_field_uwwnincoming} f_uwwnincoming
                ON n.nid = f_uwwnincoming.entity_id
		 left join
            {field_data_field_uwwpincoming} f_uwwpincoming
                ON n.nid = f_uwwpincoming.entity_id
		left join
            {field_data_field_uwwwastewatertreated} f_uwwwastewatertreated
                ON n.nid = f_uwwwastewatertreated.entity_id
        left join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
    where n.type = :typenode ' . 
    $conditions .
	' group by f_annee.field_anneedata_value order by f_annee.field_anneedata_value';
	$param = array(
        ':typenode' => 'uwwtp',
	);
	try {
		$data['BOD']['data']['Parameter'] = 'BOD';
		$data['COD']['data']['Parameter'] = 'COD';
		$data['N']['data']['Parameter'] = 'N';
		$data['P']['data']['Parameter'] = 'P';
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			// Récupération de la somme :
			$sum = is_null($row['uwwt_sum']) ? 1 : $row['uwwt_sum'];
			
			// Récupération du BOD :
			$in = is_null($row['bodin_sum']) ? 0 : array('q'=>$row['bodin_sum'], 'c'=>$row['bodin_sum']/$sum);
			$out = is_null($row['bodout_sum']) ? 0 : array('q'=>$row['bodout_sum'], 'c'=>$row['bodout_sum']/$sum);
			$data['BOD']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => $sum);
			
			// Récupération du COD :
			$in = is_null($row['codin_sum']) ? 0 : array('q'=>$row['codin_sum'], 'c'=>$row['codin_sum']/$sum);
			$out = is_null($row['codout_sum']) ? 0 : array('q'=>$row['codout_sum'], 'c'=>$row['codout_sum']/$sum);
			$data['COD']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => $sum);
			
			// Récupération du N :
			$in = is_null($row['nin_sum']) ? 0 : array('q'=>$row['nin_sum'], 'c'=>$row['nin_sum']/$sum );
			$out = is_null($row['nout_sum']) ? 0 : array('q'=>$row['nout_sum'], 'c'=>$row['nout_sum']/$sum);
			$data['N']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => $sum);
			
			// Récupération du P :
			$in = is_null($row['pin_sum']) ? 0 : array('q'=>$row['pin_sum'], 'c'=>$row['pin_sum']/$sum);
			$out = is_null($row['pout_sum']) ? 0 : array('q'=>$row['pout_sum'], 'c'=>$row['pout_sum']/$sum);
			$data['P']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => $sum);
		}
	} catch(Exception $e) {
		dsm($e->getMessage());
	}
	return $data;
}

/**
 * Récupère en base les données pour le graph 1 de la ligne 7.
 */
function uwwtd_stat_national_ms_level_data($yearSelected, $yearsPossibles) {
	$data = array();
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')
		->entityCondition('bundle', 'ms_level')
		->propertyCondition('status', 1);
	if (isset($yearSelected)){
		switch($yearSelected){
			case ALL_YEARS :
				$query->fieldCondition('field_anneedata', 'value', (array)$yearsPossibles, 'IN');
				break;
			default :
				$query->fieldCondition('field_anneedata', 'value', $yearSelected, '=');
		}
		
	}
	$query->fieldOrderBy('field_anneedata', 'value', 'ASC');
	if ($result = $query->execute()) {
		foreach($result['node'] as $id => $obj) {
			if ($node = node_load($obj->nid)) {
				$year = $node->field_anneedata['und'][0]['value'];
				$data[$year] = array(
					"sludge"=>array(
						'production'=>(float)$node->field_mslsludgeproduction['und'][0]['value'],
						'destination'=>array(
							'dcp'=>(float)$node->field_msldischargepipelines['und'][0]['value'],
							'dcs'=>(float)$node->field_msldischargeships['und'][0]['value'],
							'dco'=>(float)$node->field_msldischargeothers['und'][0]['value'],
							'rua'=>(float)$node->field_mslreusesoilagriculture['und'][0]['value'],
							'ruo'=>(float)$node->field_mslreuseothers['und'][0]['value'],
							'dil'=>(float)$node->field_msldisposallandfill['und'][0]['value'],
							'dii'=>(float)$node->field_msldisposalincineration['und'][0]['value'],
							'dio'=>(float)$node->	field_msldisposalothers['und'][0]['value'],
						),
						'remark'=>$node->field_mslremarks['und'][0]['value'],
					),
					"waste_water"=>array(
						"Re-used"=>(float)$node->field_mslwwreuseexplain['und'][0]['value'],
						"rate of re-used"=>(float)$node->field_mslwwreuseperc['und'][0]['value'],
						"repartition"=>array(
							'agriculture'=>(float)$node->field_mslwwreuseagri['und'][0]['value'],
							'industry'=>(float)$node->field_mslwwreuseind['und'][0]['value'],
							'others'=>(float)$node->field_mslwwreuseother['und'][0]['value'],
						),
					),
				);
				if ($data[$year]['sludge']['production'] > 0) {
					$sum = array_sum($data[$year]['sludge']['destination']);
					$delta = $data[$year]['sludge']['production'] - $sum;
					if($delta !=0){
						if($delta>0){
							$data[$year]['sludge']['destination']['missing']=$delta;
						}
						drupal_set_message("There are probably an error in sludge destination (delta = $delta t DS/y)","error");
					}
				}
			}
		}
		return $data;
	}
	return false;
}

/**
 * Récupère les données pour le graph 1 de la ligne 8.
 */
function uwwtd_stat_national_distance_to_compliance($yearSelected, $yearsPossibles) {
	// Récupération du nid du node MS_LEVEL de l'année demandée :
	$query = db_select('node', 'n');
	$query->leftJoin('field_data_field_anneedata', 'a', 'n.nid = a.entity_id AND a.entity_type = \'node\'');
	$query->leftJoin('field_data_field_comparison_of_compliance_p', 'cp', 'n.nid = cp.entity_id AND cp.entity_type = \'node\'');
	$query->leftJoin('field_data_field_comparison_of_compliance_a', 'ca', 'n.nid = ca.entity_id AND ca.entity_type = \'node\'');
	$query->fields('n');
	$query->fields('a', array('field_anneedata_value'));
	$query->fields('cp', array('field_comparison_of_compliance_p_value'));
	$query->fields('ca', array('field_comparison_of_compliance_a_value'));
	switch ($yearSelected) {
		case ALL_YEARS :
			$query->condition('a.field_anneedata_value', $yearsPossibles, 'IN');
			break;
		default :
			$query->condition('a.field_anneedata_value', $yearSelected, '=');
			break;
	}
	$query->condition('n.type', 'ms_level', '=');
	$query->condition('n.status', 1, '=');
	$nodes = $query->execute()->fetchAllAssoc('nid');
	
	// Pour chaque node :
	$comparisonsCompliancesByPe = array();
	$comparisonsCompliancesByAgg = array();
	foreach ($nodes as $node) {
		if (!empty($node->field_anneedata_value)) {
			// Récupération des tableaux "Comparison of Compliance PE" et "Comparison of Compliance Agg" :
			if (!empty($node->field_comparison_of_compliance_p_value)) {
				$comparisonsCompliancesByPe[$node->field_anneedata_value] = json_decode($node->field_comparison_of_compliance_p_value);
			}
			if (!empty($node->field_comparison_of_compliance_a_value)) {
				$comparisonsCompliancesByAgg[$node->field_anneedata_value] = json_decode($node->field_comparison_of_compliance_a_value);
			}
		}
	}
	
	// Tri selon les clés (année) :
	ksort($comparisonsCompliancesByPe);
	ksort($comparisonsCompliancesByAgg);
	
	return array('comparisonsCompliancesByPe' => $comparisonsCompliancesByPe, 
				 'comparisonsCompliancesByAgg' => $comparisonsCompliancesByAgg);
}

function uwwtd_stat_national_ms_level_preprocess_sludge_chart($data, $year){
    $chart_data =array();
    if(isset($data[$year]['sludge']['destination'])){
        foreach($data[$year]['sludge']['destination'] as $k=>$v){
            if($v>0){
                $chart_data[]=array(
                    'label'=>$GLOBALS['ms_level']['sludge'][$k]['label'],
                    'value'=>(float)$v,
                    'valueformat'=>uwwtd_format_number($v).' T DS/year',
                    'color'=>$GLOBALS['ms_level']['sludge'][$k]['color'], 
                );
            }
        }
        
    }
    return $chart_data;
}
function uwwtd_stat_national_ms_level_preprocess_ww_reuse_chart($data, $year){
    $chart_data =array();
    if(isset($data[$year]['waste_water']['repartition'])){
        foreach($data[$year]['waste_water']['repartition'] as $k=>$v){
            if($v>0){
                $chart_data[]=array(
                    'label'=>$GLOBALS['ms_level']['waste_water'][$k]['label'],
                    'value'=>$v,
                    'valueformat'=>$v.' T DS/year',
                    'color'=>$GLOBALS['ms_level']['waste_water'][$k]['color'], 
                );
            }
        }
        
    }
    return $chart_data;
}

function uwwtd_stat_national_uwwtp_agglo($options)
{

	$conditions = '';

	switch($options['year']){
		case 'all' :
			break;
		case '' :
			break;
		default :
			$conditions .= " and  f_annee.field_anneedata_value = '" . $options['year'] . "'";
			break;
	}
    
      if (variable_get('display_only_active', '1') == 1) {
          $conditions .= "  AND f_status.field_status_value = '1' ";
      }     


	$data = array();

	$query = 'select
			SUM(f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value) AS LOADENTERING_SUM,
			SUM(f_physicalcapacityinput.field_physicalcapacityinput_value) AS CAPACITYPHY_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        left join
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        left join
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        left join
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        left join
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        left join
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        left join
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        left join
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        left join
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        left join
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        left join
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        left join
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        left join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        left join
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        left join
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        left join
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        left join
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        left join
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        left join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id                       
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		left join
            {field_data_field_physicalcapacityinput} f_physicalcapacityinput
                ON n.nid = f_physicalcapacityinput.entity_id    
		where n.type = :typenode
            
    ' . $conditions;

	$param = array(
			':typenode' => 'uwwtp',
            
	);
//      dsm($query);
//      dsm($param);
	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {

			$data1[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}
    
    
    $conditions = '';
      if (variable_get('display_only_active', '1') == 1) {
          $conditions .= "  AND f_status.field_status_value = '1' ";
      }     

	$query = 'select SUM(f_agggenerated.field_agggenerated_value) AS GENERATED_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        left join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        left join
            {field_data_field_aggart3compliance} f_art3
                ON n.nid = f_art3.entity_id
        left join
            {field_data_field_aggart4compliance} f_art4
                ON n.nid = f_art4.entity_id
        left join
            {field_data_field_aggart5compliance} f_art5
                ON n.nid = f_art5.entity_id
        left join
            {field_data_field_aggart6compliance} f_art6
                ON n.nid = f_art6.entity_id
        left join
            {field_data_field_aggcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_regionnuts} f_nuts
                ON n.nid = f_nuts.entity_id
        left join
            {field_data_field_aggc2} f_aggc2
                ON n.nid = f_aggc2.entity_id
        left join
            {field_data_field_aggc1} f_aggc1
                ON n.nid = f_aggc1.entity_id
        left join
            {field_data_field_aggpercwithouttreatment} f_aggpercwithouttreatment
                ON n.nid = f_aggpercwithouttreatment.entity_id
        left join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id                       
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		where n.type = :typenode    
              and f_agggenerated.field_agggenerated_value > :agggenerated
    '
    . $conditions;
//     dsm($query);
//     dsm($param);
	$param = array(
			':typenode' => 'agglomeration',
            ':agggenerated' => variable_get('min_aggenerated_display', '2000'),
            
	);

	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {

			$data2[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}
	$data[0] = array_merge($data1[0],$data2[0]);

	return $data;
}

/**
 * Retourne la chaine "(test1, test2, test3, -1)" créée à partir du tableau de string donné.
 */
function getInSql($strings) {
	// Construction de la string pour le IN sql :
	$inSql = '(';
	if (!empty($strings)) {
		foreach ($strings as $string) {
			$inSql .= "'". $string ."', ";
		}
	}
	$inSql .= "'-1')";
	return $inSql;
}

?>