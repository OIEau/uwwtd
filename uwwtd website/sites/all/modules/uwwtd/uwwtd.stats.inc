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
    $conditions ='';
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
		':agggenerated' => variable_get('min_aggenerated_display', '1999'),
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
				if ($key != 'year' && $key!='Generated load') {
					$out[$year][] = array(
						"value" => (float)$data,
						"label" => $GLOBALS['ms_level']['connection'][$key]['label'],
						"valueformat" => uwwtd_format_number(round($data, 0)).' '.t('pe'),
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
	$conditions ='';
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
		':agggenerated' => variable_get('min_aggenerated_display', '1999'),
	);
	try {
		$result = db_query("
			SELECT f_annee.field_anneedata_value as year,
				upper(s.field_aggsewagenetwork_value) AS type, 
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
        
        $years = array();
		$out = array();
		while ($row = $result->fetchAssoc()) {
			if ($row['type'] == '') {
				$row['type'] = 'NI';
			}
			
			$out[$row['type']]['data'][$row['year']] = array(
				'equValue' => $row['sn_sum'],
				'equFormated' => uwwtd_format_number((integer)$row['sn_sum']).' '.t('pe'),
				'aggValue' => $row['sn_count'],
				'aggFormated' => uwwtd_format_number((integer)$row['sn_count']) . ' na',
				'color' => $GLOBALS['ms_level']['sewage_network'][$row['type']]['color'],
			);
            if(!in_array($row['year'], $years)) {
                $years[] =$row['year'];
            }
		}
        if(!empty($out) && !empty($years) ){
            sort($years);
            foreach($out as $nt=>$values){
                $networkTypeName = $GLOBALS['ms_level']['sewage_network'][$nt]['label'];
                foreach($years as $year){
                    if(empty($values['data'][$year])){
                        $out[$nt]['data'][$year] = array(
                            'equValue' => null,
                            'equFormated' =>'',
                            'aggValue'=>null,
                            'aggFormated' =>'',
                            'color' =>$GLOBALS['ms_level']['sewage_network'][$nt]['color'], 
                        );
                    }  
                }
                ksort($out[$nt]['data']);
                $out[$networkTypeName] = $out[$nt];
                unset($out[$nt]);
            }
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

	$query = 'SELECT 
		 f_annee.field_anneedata_value as year,
		 f_comp.field_aggcompliance_value,
		 COUNT(n.title) AS  TOTAL_COUNT,
		 SUM(f_aggc2.field_aggc2_value) AS AGGC2_SUM,
         SUM(f_aggc1.field_aggc1_value) AS AGGC1_SUM,
         SUM(f_aggpercwithouttreatment.field_aggpercwithouttreatment_value) AS AGGPCWT_SUM,
         SUM(f_agggenerated.field_agggenerated_value) AS GENERATED_SUM
    FROM {node} n
        LEFT JOIN
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT JOIN
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        LEFT JOIN
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        LEFT JOIN
            {field_data_field_aggart3compliance} f_art3
                ON n.nid = f_art3.entity_id
        LEFT JOIN
            {field_data_field_aggart4compliance} f_art4
                ON n.nid = f_art4.entity_id
        LEFT JOIN
            {field_data_field_aggart5compliance} f_art5
                ON n.nid = f_art5.entity_id
        LEFT JOIN
            {field_data_field_aggart6compliance} f_art6
                ON n.nid = f_art6.entity_id
        LEFT JOIN
            {field_data_field_aggcompliance} f_comp
                ON n.nid = f_comp.entity_id
        LEFT JOIN
            {field_data_field_regionnuts} f_nuts
                ON n.nid = f_nuts.entity_id
        LEFT JOIN
            {field_data_field_aggc2} f_aggc2
                ON n.nid = f_aggc2.entity_id
        LEFT JOIN
            {field_data_field_aggc1} f_aggc1
                ON n.nid = f_aggc1.entity_id
        LEFT JOIN
            {field_data_field_aggpercwithouttreatment} f_aggpercwithouttreatment
                ON n.nid = f_aggpercwithouttreatment.entity_id
        LEFT JOIN
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
        LEFT JOIN
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		WHERE n.type = :typenode
        and f_agggenerated.field_agggenerated_value > :agggenerated '.
    	$conditions .
    	' group BY f_annee.field_anneedata_value, f_comp.field_aggcompliance_value
    	order by f_annee.field_anneedata_value';

    $param = array(
    	':typenode' => 'agglomeration',
    	':agggenerated' => variable_get('min_aggenerated_display', '1999'),
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
    				'pe_format'=>uwwtd_format_number($row['generated_sum'], 0) .' '.t('pe'), 
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
            $table_key_label = t("Compliance state");
			break;
		case 'treatmenttype' :
			$group_by = 'f_uwwtreatmenttype.field_uwwtreatmenttype_value';
            $table_key_label = t("Treatment type");
			break;
		default:
            $table_key_label = t("Compliance state");
			$group_by = 'f_comp.field_uwwcompliance_value';
	}
	$data = array();
    
    //Filter uwwtp with "min_aggenerated_display" parameter
    //In this request we need all uwwtp linked with a relevant agg (> 2000 ep)
    $with = 'WITH uwwtp_min_agg AS(
        SELECT n.nid, n.type, n.title 
        FROM {node} n
            JOIN
              {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
            JOIN
              {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
            JOIN {field_data_field_uwwcollectingsystem}  con
                ON n.nid = con.entity_id AND con.field_uwwcollectingsystem_value = \'ISCON\'
            JOIN
              {field_data_field_agguwwliste} f_uwwtp_agglo
                ON f_uwwtp_agglo.field_agguwwliste_nid = n.nid
            JOIN
            {field_data_field_agggenerated} f_agggenerated
                ON f_uwwtp_agglo.entity_id = f_agggenerated.entity_id
        WHERE n.type = :typenode
            AND f_agggenerated.field_agggenerated_value >= :min_agg
            '.$conditions.'
        GROUP BY n.nid
        )
    ';
	$query = $with. 'SELECT f_annee.field_anneedata_value as year, ' . $group_by . ',';
    //warning this request count the number of agglomeration and not just the number of uwwtp
	$query .= ' COUNT(n.nid) AS  total_count, SUM(f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value) AS loadentering_sum
    FROM {node} n
        JOIN uwwtp_min_agg
            ON n.nid = uwwtp_min_agg.nid 
        JOIN
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        JOIN {field_data_field_uwwcollectingsystem}  con
                ON n.nid = con.entity_id AND con.field_uwwcollectingsystem_value = \'ISCON\'
        LEFT JOIN
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        LEFT JOIN
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        LEFT JOIN
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        LEFT JOIN
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        LEFT JOIN
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
		WHERE n.type = :typenode
    ' .
    $conditions . '
		GROUP BY f_annee.field_anneedata_value,' . $group_by;

    $param = array(
        ':typenode' => 'uwwtp',
        ':min_agg' => variable_get('min_aggenerated_display', '2000'),
    );
    try {
        $result = db_query($query,$param);
    	while ($row = $result->fetchAssoc()) {
    		switch ($groupBy){
    			case 'compliance' :
                    if($row['field_uwwcompliance_value']==''){
                        $row['field_uwwcompliance_value']='NI';
                    }
    				$complianceState = $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_uwwcompliance_value']];
                    $color = $GLOBALS['uwwtd']['compliance_color'][$row['field_uwwcompliance_value']];
    				break;
    			case 'treatmenttype' :
                    if($row['field_uwwtreatmenttype_value']==''){
                        $row['field_uwwtreatmenttype_value']='NI';
                    }
    				$complianceState = $GLOBALS['uwwtd']['ui']['treatment_type_in_place'][$row['field_uwwtreatmenttype_value']];
                    $color = $GLOBALS['uwwtd']['treatment_type_color'][$row['field_uwwtreatmenttype_value']];
    				break;
    			default:
    				$complianceState = $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_uwwcompliance_value']];
    		}
            
    		$data[$complianceState]['data'][$table_key_label] = $complianceState;
    		if (is_null($row['loadentering_sum'])) {$row['loadentering_sum'] =0;}
            if(is_null($row['total_count'])) {$row['total_count']=0;}
            $data[$complianceState]['data'][(string)$row['year']] = array(
                'pe'=>$row['loadentering_sum'], 
                'pe_format'=>uwwtd_format_number($row['loadentering_sum'], 0) .' '.t('pe'), 
                'nb'=>$row['total_count'],
                'nb_format'=>uwwtd_format_number($row['total_count'], 0),
                'color'=>$color
            );
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
		'< 10,000 pe' => ' and (f_agggenerated.field_agggenerated_value >= 2000 and f_agggenerated.field_agggenerated_value < 10000) ',
		'10,000 to 100,000 pe' => ' and (f_agggenerated.field_agggenerated_value >= 10000 and f_agggenerated.field_agggenerated_value < 100000) ',
		'> 100,000 pe' => ' and (f_agggenerated.field_agggenerated_value >= 100000) ',
	);
	$query = 'SELECT 
		f_annee.field_anneedata_value as year,
		COUNT(f_agggenerated.field_agggenerated_value) AS number,
    	SUM(f_agggenerated.field_agggenerated_value) AS generated_sum
    FROM {node} n
        LEFT JOIN
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT JOIN
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        LEFT JOIN
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        LEFT JOIN
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
		WHERE n.type = :typenode
         and f_agggenerated.field_agggenerated_value > :agggenerated 
    ' . $conditions;
	$param = array(
		':typenode' => 'agglomeration',
		':agggenerated' => variable_get('min_aggenerated_display', '1999'),
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
						'pe_format'=>uwwtd_format_number($row['generated_sum'], 0) .' '.t('pe'),
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
    
    //Filter uwwtp with "min_aggenerated_display" parameter
    
    $with = 'WITH uwwtp_min_agg AS(
        SELECT n.nid, n.type, n.title 
        FROM {node} n
            LEFT join
              {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
            LEFT JOIN
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
    
    
    
	$data = array();
	$param = array();
    //======= nd@oieau.fr ====================> In some case (like LT) values for BOD, N, COD, P are empty ==> should we exclude these line for the whole calculation ?
    //field_uwwbodincomingcalculated
    //field_uwwbodincomingestimated
	$query = $with.'SELECT
		f_annee.field_anneedata_value as year,
		SUM(COALESCE(field_uwwbodincoming_value,field_uwwbodincomingestimated_value,field_uwwbodincomingcalculated_value)) AS bodin_sum,
		SUM(COALESCE(field_uwwboddischarge_value,field_uwwboddischargeestimated_value,field_uwwboddischargecalculated_value)) AS bodout_sum,
		SUM(COALESCE(field_uwwcodincoming_value,field_uwwcodincomingestimated_value,field_uwwcodincomingcalculated_value)) AS codin_sum,
		SUM(COALESCE(field_uwwcoddischarge_value,field_uwwcoddischargeestimated_value,field_uwwcoddischargecalculated_value)) AS codout_sum,
		SUM(COALESCE(field_uwwnincoming_value,field_uwwnincomingestimated_value,field_uwwnincomingcalculated_value)) AS nin_sum,
		SUM(COALESCE(field_uwwndischarge_value,field_uwwndischargeestimated_value,field_uwwndischargecalculated_value)) AS nout_sum,
		SUM(COALESCE(field_uwwpincoming_value,field_uwwpincomingestimated_value,field_uwwpincomingcalculated_value)) AS pin_sum,
		SUM(COALESCE(field_uwwpdischarge_value,field_uwwpdischargeestimated_value,field_uwwpdischargecalculated_value)) AS pout_sum,
		SUM(f_uwwwastewatertreated.field_uwwwastewatertreated_value) AS uwwt_sum
    FROM {node} n
        join uwwtp_min_agg
             ON n.nid = uwwtp_min_agg.nid 
        LEFT JOIN
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT JOIN
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        LEFT JOIN
            {field_data_field_uwwboddischargeestimated} f_uwwboddischargeestimated
                ON n.nid = f_uwwboddischargeestimated.entity_id
        LEFT JOIN
            {field_data_field_uwwboddischargecalculated} f_uwwboddischargecalculated
                ON n.nid = f_uwwboddischargecalculated.entity_id
                
        LEFT JOIN
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        LEFT JOIN
            {field_data_field_uwwcoddischargeestimated} f_uwwcoddischargeestimated
                ON n.nid = f_uwwcoddischargeestimated.entity_id
        LEFT JOIN
            {field_data_field_uwwcoddischargecalculated} f_uwwcoddischargecalculated
                ON n.nid = f_uwwcoddischargecalculated.entity_id
                
        LEFT JOIN
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        LEFT JOIN
            {field_data_field_uwwndischargeestimated} f_uwwndischargeestimated
                ON n.nid = f_uwwndischargeestimated.entity_id
        LEFT JOIN
            {field_data_field_uwwndischargecalculated} f_uwwndischargecalculated
                ON n.nid = f_uwwndischargecalculated.entity_id
                
        LEFT JOIN
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
         LEFT JOIN
            {field_data_field_uwwpdischargeestimated} f_uwwpdischargeestimated
                ON n.nid = f_uwwpdischargeestimated.entity_id
        LEFT JOIN
            {field_data_field_uwwpdischargecalculated} f_uwwpdischargecalculated
                ON n.nid = f_uwwpdischargecalculated.entity_id   
                
		 LEFT JOIN
            {field_data_field_uwwbodincoming} f_uwwbodincoming
                ON n.nid = f_uwwbodincoming.entity_id
         LEFT JOIN
            {field_data_field_uwwbodincomingestimated} f_uwwbodincomingestimated
                ON n.nid = f_uwwbodincomingestimated.entity_id
         LEFT JOIN
            {field_data_field_uwwbodincomingcalculated} f_uwwbodincomingcalculated
                ON n.nid = f_uwwbodincomingcalculated.entity_id
                
		 LEFT JOIN
            {field_data_field_uwwcodincoming} f_uwwcodincoming
                ON n.nid = f_uwwcodincoming.entity_id
         LEFT JOIN
            {field_data_field_uwwcodincomingestimated} f_uwwcodincomingestimated
                ON n.nid = f_uwwcodincomingestimated.entity_id
         LEFT JOIN
            {field_data_field_uwwcodincomingcalculated} f_uwwcodincomingcalculated
                ON n.nid = f_uwwcodincomingcalculated.entity_id
                
		 LEFT JOIN
            {field_data_field_uwwnincoming} f_uwwnincoming
                ON n.nid = f_uwwnincoming.entity_id
         LEFT JOIN
            {field_data_field_uwwnincomingestimated} f_uwwnincomingestimated
                ON n.nid = f_uwwnincomingestimated.entity_id
         LEFT JOIN
            {field_data_field_uwwnincomingcalculated} f_uwwnincomingcalculated
                ON n.nid = f_uwwnincomingcalculated.entity_id
                
		 LEFT JOIN
            {field_data_field_uwwpincoming} f_uwwpincoming
                ON n.nid = f_uwwpincoming.entity_id
        LEFT JOIN
            {field_data_field_uwwpincomingestimated} f_uwwpincomingestimated
                ON n.nid = f_uwwpincomingestimated.entity_id
         LEFT JOIN
            {field_data_field_uwwpincomingcalculated} f_uwwpincomingcalculated
                ON n.nid = f_uwwpincomingcalculated.entity_id
                
		LEFT JOIN
            {field_data_field_uwwwastewatertreated} f_uwwwastewatertreated
                ON n.nid = f_uwwwastewatertreated.entity_id
        LEFT JOIN
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
    WHERE n.type = :typenode ' . 
    $conditions .
	' group by f_annee.field_anneedata_value order by f_annee.field_anneedata_value';
	$param = array(
        ':typenode' => 'uwwtp',
        ':min_agg' => variable_get('min_aggenerated_display', '1999'),
	);
    //dsm($query);
    
	try {
		$data['BOD']['data']['Parameter'] = 'BOD';
		$data['COD']['data']['Parameter'] = 'COD';
		$data['N']['data']['Parameter'] = 'N';
		$data['P']['data']['Parameter'] = 'P';
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			// Récupération de la somme :
			$sum = (float)is_null($row['uwwt_sum']) ? 1 : $row['uwwt_sum']; // expresse in m3

            //For concentration we want mg/l and we have Ton/year and m3/year so we have to apply a 1 000 000 000 factor
			
			// Récupération du BOD :
			$in = is_null($row['bodin_sum']) ? 0 : array('q'=>$row['bodin_sum'], 'c'=>$row['bodin_sum']*1000000/(float)$sum);
			$out = is_null($row['bodout_sum']) ? 0 : array('q'=>$row['bodout_sum'], 'c'=>$row['bodout_sum']*1000000/(float)$sum);
			$data['BOD']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => (float)$sum);
			
			// Récupération du COD :
			$in = is_null($row['codin_sum']) ? 0 : array('q'=>$row['codin_sum'], 'c'=>$row['codin_sum']*1000000/(float)$sum);
			$out = is_null($row['codout_sum']) ? 0 : array('q'=>$row['codout_sum'], 'c'=>$row['codout_sum']*1000000/(float)$sum);
			$data['COD']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => (float)$sum);
			
			// Récupération du N :
			$in = is_null($row['nin_sum']) ? 0 : array('q'=>$row['nin_sum'], 'c'=>$row['nin_sum']*1000000/(float)$sum );
			$out = is_null($row['nout_sum']) ? 0 : array('q'=>$row['nout_sum'], 'c'=>$row['nout_sum']*1000000/(float)$sum);
			$data['N']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => (float)$sum);
			
			// Récupération du P :
			$in = is_null($row['pin_sum']) ? 0 : array('q'=>$row['pin_sum'], 'c'=>$row['pin_sum']*1000000/(float)$sum);
			$out = is_null($row['pout_sum']) ? 0 : array('q'=>$row['pout_sum'], 'c'=>$row['pout_sum']*1000000/(float)$sum);
			$data['P']['data'][$row['year']] = array('in' => $in, 'out' => $out, 'vol' => (float)$sum);
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
                    'remark'=>isset($node->field_mslremarks['und'])?$node->field_mslremarks['und'][0]['value']:'',
					"sludge"=>array(
						'production'=>isset($node->field_mslsludgeproduction['und'])?(float)$node->field_mslsludgeproduction['und'][0]['value']:0,
						'destination'=>array(
							'dcp'=>isset($node->field_msldischargepipelines['und'])?(float)$node->field_msldischargepipelines['und'][0]['value']:0,
							'dcs'=>isset($node->field_msldischargeships['und'])?(float)$node->field_msldischargeships['und'][0]['value']:0,
							'dco'=>isset($node->field_msldischargeothers['und'])?(float)$node->field_msldischargeothers['und'][0]['value']:0,
							'rua'=>isset($node->field_mslreusesoilagriculture['und'])?(float)$node->field_mslreusesoilagriculture['und'][0]['value']:0,
							'ruo'=>isset($node->field_mslreuseothers['und'])?(float)$node->field_mslreuseothers['und'][0]['value']:0,
							'dil'=>isset($node->field_msldisposallandfill['und'])?(float)$node->field_msldisposallandfill['und'][0]['value']:0,
							'dii'=>isset($node->field_msldisposalincineration['und'])?(float)$node->field_msldisposalincineration['und'][0]['value']:0,
							'dio'=>isset($node->field_msldisposalothers['und'])?(float)$node->field_msldisposalothers['und'][0]['value']:0,
						),
					),
					"waste_water"=>array(
						"Re-used"=>isset($node->field_mslwwreuseexplain['und'])?$node->field_mslwwreuseexplain['und'][0]['value']:'',
						"rate of re-used"=>isset($node->field_mslwwreuseperc['und'])?(float)$node->field_mslwwreuseperc['und'][0]['value']:0,
						"repartition"=>array(
							'agriculture'=>isset($node->field_mslwwreuseagri['und'])?(float)$node->field_mslwwreuseagri['und'][0]['value']:0,
							'industry'=>isset($node->field_mslwwreuseind['und'])?(float)$node->field_mslwwreuseind['und'][0]['value']:0,
							'others'=>isset($node->field_mslwwreuseother['und'])?(float)$node->field_mslwwreuseother['und'][0]['value']:0,
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
						if(abs($delta)>10) {drupal_set_message(t("There are probably an error in sludge destination (delta = @delta t DS/y)",array('@delta'=>$delta)),"error");}
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
	$query = db_SELECT('node', 'n');
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
	
    $data = [
        'comparisonsCompliancesByPe' => $comparisonsCompliancesByPe, 
        'comparisonsCompliancesByAgg' => $comparisonsCompliancesByAgg
     ];
	return $data;
}

function uwwtd_stat_national_ms_level_preprocess_sludge_chart($data, $year){
    $chart_data =array();
    if(isset($data[$year]['sludge']['destination'])){
        foreach($data[$year]['sludge']['destination'] as $k=>$v){
            if($v>0){
                $chart_data[]=array(
                    'label'=>$GLOBALS['ms_level']['sludge'][$k]['label'],
                    'value'=>(float)$v,
                    'valueformat'=>uwwtd_format_number($v).' '.t('T DS/year'),
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
                    'valueformat'=>$v.' '.t('T DS/year'),
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

	$query = 'SELECT
			SUM(f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value) AS LOADENTERING_SUM,
			SUM(f_physicalcapacityinput.field_physicalcapacityinput_value) AS CAPACITYPHY_SUM
    FROM {node} n
        LEFT JOIN
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT JOIN
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        LEFT JOIN
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        LEFT JOIN
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        LEFT JOIN
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        LEFT JOIN
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        LEFT JOIN
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        LEFT JOIN
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        LEFT JOIN
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        LEFT JOIN
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        LEFT JOIN
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        LEFT JOIN
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        LEFT JOIN
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        LEFT JOIN
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        LEFT JOIN
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        LEFT JOIN
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        LEFT JOIN
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        LEFT JOIN
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        LEFT JOIN
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        LEFT JOIN
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        LEFT JOIN
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id                       
        LEFT JOIN
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		LEFT JOIN
            {field_data_field_physicalcapacityinput} f_physicalcapacityinput
                ON n.nid = f_physicalcapacityinput.entity_id    
		WHERE n.type = :typenode
            
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

	$query = 'SELECT SUM(f_agggenerated.field_agggenerated_value) AS GENERATED_SUM
    FROM {node} n
        LEFT JOIN
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT JOIN
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        LEFT JOIN
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        LEFT JOIN
            {field_data_field_aggart3compliance} f_art3
                ON n.nid = f_art3.entity_id
        LEFT JOIN
            {field_data_field_aggart4compliance} f_art4
                ON n.nid = f_art4.entity_id
        LEFT JOIN
            {field_data_field_aggart5compliance} f_art5
                ON n.nid = f_art5.entity_id
        LEFT JOIN
            {field_data_field_aggart6compliance} f_art6
                ON n.nid = f_art6.entity_id
        LEFT JOIN
            {field_data_field_aggcompliance} f_comp
                ON n.nid = f_comp.entity_id
        LEFT JOIN
            {field_data_field_regionnuts} f_nuts
                ON n.nid = f_nuts.entity_id
        LEFT JOIN
            {field_data_field_aggc2} f_aggc2
                ON n.nid = f_aggc2.entity_id
        LEFT JOIN
            {field_data_field_aggc1} f_aggc1
                ON n.nid = f_aggc1.entity_id
        LEFT JOIN
            {field_data_field_aggpercwithouttreatment} f_aggpercwithouttreatment
                ON n.nid = f_aggpercwithouttreatment.entity_id
        LEFT JOIN
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id                       
        LEFT JOIN
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		WHERE n.type = :typenode    
              and f_agggenerated.field_agggenerated_value >= :agggenerated
    '
    . $conditions;
//     dsm($query);
//     dsm($param);
	$param = array(
			':typenode' => 'agglomeration',
            ':agggenerated' => variable_get('min_aggenerated_display', '1999'),
            
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

function uwwtd_stats_sum($data, $year, $col){
    $out = [];
    if(!empty($data)){
        $tot  = 0;
        foreach($data as $cat=>$val){
            if(isset($val['data'][$year]) && isset($val['data'][$year][$col])){
                $out[$cat] =$val['data'][$year][$col];
                $tot +=$val['data'][$year][$col]; 
            }
        }
        $out['Total'] = $tot;
    }
    return $out;
}

function uwwtd_stats_summary_chapter($year){
    
    module_load_include('inc','uwwtd','script/country/country');
    module_load_include('inc','uwwtd','inc/pages');
    
    /*
    module_load_include('inc','uwwtd','inc/register');
    module_load_include('inc','uwwtd','inc/inc/register.config');
    */
    
    
    //===Set options and const
    
    $option['field_anneedata_value'] = $year;
    $country_code = variable_get('siif_eru_country_code', '-1');
    $country_name = variable_get('siif_eru_country_name');
    if(isset($GLOBALS['pop_ms'][strtoupper($country_code)][$year])){
        $population =$GLOBALS['pop_ms'][strtoupper($country_code)][$year];
    }else{
        $population =$GLOBALS['pop_ms'][strtoupper($country_code)]['2018'];
    }
        
    //===Load data
    //=$national_compliance
    $national_compliance = uwwtd_stat_national_compliance($year, []);
    $national_compliance_nb = uwwtd_stats_sum($national_compliance, $year, 'nb');
    $national_compliance_pe = uwwtd_stats_sum($national_compliance, $year, 'pe');
    
    //=$stat_collect
    $stat_collect = uwwtd_stat_collect_data($year, []);
    $stat_collect_values = array_combine(array_column($stat_collect[$year], 'label'), array_column($stat_collect[$year], 'value'));
    $stat_collect_values['Total'] = array_sum(array_column($stat_collect[$year], 'value'));
    
    //=$treatmenttype
    $treatmenttype = uwwtd_stat_national_uwwtp($year, [],'treatmenttype');
    $treatmenttype_nb = uwwtd_stats_sum($treatmenttype, $year, 'nb');
    $treatmenttype_pe = uwwtd_stats_sum($treatmenttype, $year, 'pe');
    
    //= Design capacity 
    $dataUwwtp = uwwtd_get_data_uwwtp($option);
    $uwwtp_ids = [];
    $nbPhysicalCapacity = 0;
	foreach($dataUwwtp as $uwwtp){        
        if($uwwtp['field_status_value']==1 && !in_array($uwwtp['nid'], $uwwtp_ids)){
            /*
            if($uwwtp['field_uwwtreatmenttype_value'] == "P"){
                $nbPrimaryTreatement = $nbPrimaryTreatement+1;
            }elseif($uwwtp['field_uwwtreatmenttype_value'] == "S"){
                $nbSecondaryTreatement = $nbSecondaryTreatement +1;
            }elseif($uwwtp['field_uwwtreatmenttype_value'] == "MS"){
                $nbOtherTreatement = $nbOtherTreatement+1;
            }*/
            $nbPhysicalCapacity = $uwwtp['field_physicalcapacityactivity_value']+ $nbPhysicalCapacity;
            //Keep id of uwwtp in memory in order to prevent double counting
            $uwwtp_ids[] = $uwwtp['nid'];
        }
	}
    $dataAgg = uwwtd_get_data_agglomeration($option);
    $overflow = array_filter( array_column($dataAgg, 'field_aggseweroverflows_m3_value'), function($v){
        return ($v >= 1?true:false);
    } ); 
    //dsm($overflow);
    //= Legal compliance
    $legal_compliance = uwwtd_stat_national_distance_to_compliance($year, []);
    $art3_C = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Compliant"}->{"Article 3"} ;
    $art3_NC = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Not compliant"}->{"Article 3"};
    $art3_nb =  (integer)$art3_C+ $art3_NC;
    $art4_C = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Compliant"}->{"Article 4"} ;
    $art4_NC = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Not compliant"}->{"Article 4"};
    $art4_nb =  (integer)$art4_C+ $art4_NC;
    $art5_C = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Compliant"}->{"Article 5"} ;
    $art5_NC = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Not compliant"}->{"Article 5"};
    $art5_nb =  (integer)$art5_C+ $art5_NC;

    $art3_pe_C = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Compliant"}->{"Article 3"} ;
    $art3_pe_NC = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Not compliant"}->{"Article 3"};
    $art3_pe_nb =  (integer)$art3_pe_C+ $art3_pe_NC;
    $art4_pe_C = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Compliant"}->{"Article 4"} ;
    $art4_pe_NC = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Not compliant"}->{"Article 4"};
    $art4_pe_nb =  (integer)$art4_pe_C+ $art4_pe_NC;
    $art5_pe_C = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Compliant"}->{"Article 5"} ;
    $art5_pe_NC = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Not compliant"}->{"Article 5"};
    $art5_pe_nb =  (integer)$art5_pe_C+ $art5_pe_NC;
    //ms_level
    $ms_level = uwwtd_stat_national_ms_level_data($year, []);
    
    //Reuse
    $dcp_reuse = uwwtd_get_data_dcp(['field_anneedata_value'=>$year]);
    foreach($dcp_reuse as $k=>$line){
        if($line['field_dcpirrigation_value']==''){
            unset($dcp_reuse[$k]);
        }
    }
    
    //$path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
    //include($path);

    
    $output = '<h2>Summary</h2>';

    $output .= '<p><b>'.$country_name.' compliance rate: '. round(100 * ($national_compliance_nb['Compliant']/$national_compliance_nb['Total']),1) .'%</b></p>';
    $output .= '<p>In '.$year.', '.$country_name.' counted '.$national_compliance_nb['Total'].' agglomerations generating ≥2,000 p.e. each, of which:</p>';
    $output .= '<ul>';
    $output .= '    <li>'.format_plural($national_compliance_nb['Compliant'], '1 agglomeration',  '@count agglomerations').', generating '.uwwtd_format_number($national_compliance_pe['Compliant']).' p.e. of waste water, complied with the requirements of the Directive; and</li>';
    $output .= '    <li>'.format_plural($national_compliance_nb['Not Compliant'], '1 agglomeration',  '@count agglomerations').', generating '.uwwtd_format_number($national_compliance_pe['Not Compliant']).' p.e. of waste water, did not comply with the requirements of the Directive.</li>';
    //Manage PD & NR case ?
    $output .= '</ul>';
$output .= '<p>The waste water load generated in '.$country_name.' in '.$year.' was reported to be '.uwwtd_format_number($national_compliance_pe['Total']).' p.e. </p>';

//Display this text only if compliance art 3 is < 100 %
$output .= '<p>In '.$year.', '.$country_name.' had not yet reached the target for collecting and treating waste water to fully comply with the Directive.</p>';
$output .= '<ul>
        <li>'.(
            $stat_collect_values['Discharged without treatment']>0?
            round(100 * $stat_collect_values['Discharged without treatment']/$stat_collect_values['Total'], 1).' % of the waste water load must still be collected (approximately '.uwwtd_format_number(round($stat_collect_values['Discharged without treatment'], -2)).' p.e.); and'
            : 'All the waste water load is connected to collecting system or to Individual and Appropriate Systems; and'
            ).'
        </li>
        <li>'.( $art4_pe_NC>0?
                round(100 * $art4_pe_NC/$art4_pe_nb,1) .'% of the collected waste water load must still undergo secondary treatment in line with the requirements of the Directive (approximately '.uwwtd_format_number(round($art4_pe_NC, -2)).' p.e.),'
                :'All of the collected waste water load underwent secondary treatment in line with the requirements of the Directive,').
        '</li>
        <li>'. ($art5_pe_NC>0?
            round(100 * $art5_pe_NC/$art5_pe_nb,1) .'% of the collected waste water load from agglomerations generating >10,000 p.e. and discharging into sensitive areas must still undergo more stringent treatment in line with the requirements of the Directive (approximately '.uwwtd_format_number(round($art5_pe_NC, -2)).' p.e.).'
            :'All of the collected waste water load from agglomerations generating >10,000 p.e. and discharging into sensitive areas underwent more stringent treatment in line with the requirements of the Directive').'
        </li>
    </ul>';

$output .='<p>'.round(100 * $stat_collect_values['Individual and Appropriate Systems']/$stat_collect_values['Total'], 1).'% of waste water load is addressed by means of individual or other appropriate systems (IAS) i.e. non-centralised sanitation systems (approximately '.uwwtd_format_number(round($stat_collect_values['Individual and Appropriate Systems'], -2)).' p.e.).  </p>';

$output .='<p>For the '.$national_compliance_nb['Total'].' agglomerations, there are '.$treatmenttype_nb['Total'].' urban waste water treatment plants, with a total design capacity of '.uwwtd_format_number(round($nbPhysicalCapacity, -3)).' p.e., of which:</p>
<ul>
    <li>'.(isset($treatmenttype_nb['Secondary treatment'])?format_plural($treatmenttype_nb['Secondary treatment'], '1 plant' , '@count plants'):'There is no plant').' equipped with technology only for primary and secondary treatment,</li>
    <li>'.(isset($treatmenttype_nb['More stringent treatment'])?format_plural($treatmenttype_nb['More stringent treatment'], '1 plant' , '@count plants'):'There is no plant').' equipped with technology for more stringent treatment than secondary.</li>
</ul>
';
$output.='<h3>Compliance rate by Article</h3>';
$output.='
<table class="table">
    <thead>
        <tr>
            <th></th>
            <th colspan="3" align="center">Agglomeration</th>
            <th colspan="3" align="center">Waste water load</th>
        </tr>
        <tr>
            <th>'.$country_name.' in '.$year.'</th>
            <th>Total<br/>[No]</th>
            <th>Comply<br/>[No]</th>
            <th>Comply<br/>[%]</th>
            <th>Total<br/>[p.e.]</th>
            <th>Comply<br/>[p.e.]</th>
            <th>Comply<br/>[%]</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><b>Article 3 (collection)</b></td>
            <td>'. $art3_nb .'</td>
            <td>'.$art3_C.'</td>
            <td>'.round(100 * $art3_C/$art3_nb , 1).' %</td>
            <td>'. uwwtd_format_number($art3_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art3_pe_C).'</td>
            <td>'.round(100 * $art3_pe_C/$art3_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>Article 4 (secondary treatment)</b></td>
            <td>'. $art4_nb .'</td>
            <td>'.$art4_C.'</td>
            <td>'.round(100 * $art4_C/$art4_nb , 1).' %</td>
            <td>'. uwwtd_format_number($art4_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art4_pe_C).'</td>
            <td>'.round(100 * $art4_pe_C/$art4_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>Article 5 (more stringent treatment)</b></td>
            <td>'. $art5_nb .'</td>
            <td>'.$art5_C.'</td>
            <td>'.round(100 * $art5_C/$art5_nb , 1).' %</td>
            <td>'. uwwtd_format_number($art5_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art5_pe_C).'</td>
            <td>'.round(100 * $art5_pe_C/$art5_pe_nb , 1).' %</td>
        </tr>
    </tbody>
</table>
';
$output.='
<p><i>*The total [No of agglomerations] includes only those agglomerations that must provide secondary treatment / or more stringent treatment than secondary before discharging waste water from their treatment plants.</i></p>
<p><i>**The total [p.e. waste water load] is the sum of the load of all of the agglomerations that must provide secondary treatment or more stringent treatment than secondary before discharging waste water from their treatment plants. </i></p>
<p align="right"><i>Data source for table: Register, summary of MS data</i></p>
';

$output.='<h3>Distance to target </h3>';
$output.='
<table class="table">
    <thead>
        <tr>
            <th></th>
            <th colspan="3" align="center">Waste water load</th>
        </tr>
        <tr>
            <th>'.$country_name.' in '.$year.'</th>
            <th>Target<br/>[p.e.]</th>
            <th>Distance to target<br/>[p.e.]</th>
            <th>Distance to target<br/>[%]</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><b>Collection</b></td>
            <td>'. uwwtd_format_number($art3_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art3_pe_NC).'</td>
            <td>'.round(100 * $art3_pe_NC/$art3_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>Secondary treatment</b></td>
            <td>'. uwwtd_format_number($art4_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art4_pe_NC).'</td>
            <td>'.round(100 * $art4_pe_NC/$art4_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>More stringent treatment</b></td>
            <td>'. uwwtd_format_number($art5_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art5_pe_NC).'</td>
            <td>'.round(100 * $art5_pe_NC/$art5_pe_nb , 1).' %</td>
        </tr>
    </tbody>
</table>
';
$output.='
    <p><i>*The target [p.e.] for secondary treatment represents the waste water load (collected and not collected) from all agglomerations that must be subject to secondary treatment, regardless of whether or not the agglomeration complies with Article 3.</i></p>
    <p><i>** The target for more stringent treatment represents the waste water load (collected and not collected) from all agglomerations generating >10,000 p.e. and discharging into sensitive areas (regardless of whether or not the agglomeration complies with Article 3).</i></p>
    <p align="right"><i>Data source for table: Register, summary of MS data</i></p>
';



$art17_mslevel = uwwtd_stat_art17_mslevel(['year'=>$year]);
$art17_mslevel_data = [];
if(!empty($art17_mslevel )){
    foreach($art17_mslevel as $v){
        $v['soeinvtot'] = $v['soecinvtp']+$v['soecinvcol'];
        $v['nb_year'] = 1 + (integer)substr($v['soecendyear'],0,4) - (integer)substr($v['soecstartyear'],0,4);
        $v['yearly_inv'] = $v['soeinvtot']/$v['nb_year'];
        
        $art17_mslevel_data[$v['soecperiod']][substr($v['soecstartyear'],0,4)] = $v;
        
    }
    $cu_invest = array_column($art17_mslevel_data['CU'], 'yearly_inv');
    $cu_investment_inh = (array_sum($cu_invest)*1000000/$population)/count($cu_invest);
    $exp_invest = array_column($art17_mslevel_data['EXP'], 'yearly_inv');
    $exp_invest_inh = (array_sum($exp_invest)*1000000/$population)/count($exp_invest);
    $years = array_keys($art17_mslevel_data['EXP']);
    $min_k = min($years);
    $max_k = max($years);
    $begin_y = (integer)substr($art17_mslevel_data['EXP'][$min_k]['soecstartyear'],0,4);
    $end_y =  (integer)substr($art17_mslevel_data['EXP'][$max_k]['soecendyear'],0,4);
    
    
    //$output.='<p>#CURRENT == The expected annual investment costs per capita for installing and renewing waste water collecting systems and treatment plants is '.uwwtd_format_number(round($cu_investment_inh)).' EUR/inhabitant/year.</p>';
    $output.='<p>The expected annual investment costs per capita for installing and renewing waste water collecting systems and treatment plants is '.uwwtd_format_number(round($exp_invest_inh)).' EUR/inhabitant/year.</p>';
    $output.='<p>';
    $output.='The investment needs  for ensuring adequate urban waste water collection and treatment, i.e. compliance with the Directive, as estimated by the national authorities and included in their national plan, are at the level of EUR '.array_sum(array_column($art17_mslevel_data['EXP'], 'soeinvtot')).' million for the period '.$begin_y.'-'.$end_y.'. ';
    $output.='This covers works on treatment plants, with the forecasted investment cost of EUR '.array_sum(array_column($art17_mslevel_data['EXP'], 'soecinvcol')).' million ('.$begin_y.'-'.$end_y.'), and works on collecting systems and/or IAS, with a forecasted investment cost of EUR '.array_sum(array_column($art17_mslevel_data['EXP'], 'soecinvtp')).' million ('.$begin_y.'-'.$end_y.'). ';
   $output.='However, as indicated in a recent OECD study, '. $country_name.' would need to increase their investments further to reach and maintain compliance. ';
    $output.='The OECD projected that the financing needs by 2030 (total cumulative additional expenditures for water supply and sanitation) is EUR '.round($GLOBALS['ocde_data'][strtoupper($country_code)]['sum'],1).' billion. ';
    $output.='</p>';
}




$no_ms_level = [];
if(isset($ms_level[$year]['sludge']['production']) && $ms_level[$year]['sludge']['production'] > 0){
    $output.='<p>';
    $output.='For the reference year '.$year.', '.$country_name.' reported the production of '.uwwtd_format_number($ms_level[$year]['sludge']['production']).' tonnes of sludge. ';
    if(!empty($ms_level[$year]['sludge']['destination'])){
        arsort($ms_level[$year]['sludge']['destination']);
        $output.='The sludge is mainly ';
        $slude_use = [];
        foreach($ms_level[$year]['sludge']['destination'] as $k=>$v){
            $rate = round(100 * $v /$ms_level[$year]['sludge']['production']);
            if($rate >= 10){
                $slude_use[]= strtolower($GLOBALS['ms_level']['sludge'][$k]['label']).' ('.$rate.' %)';
            }else{
                break;
            }
        }
        $output.= implode(' or ',$slude_use) .'.';
        if($ms_level[$year]['remark']!=''){
            $output.=$ms_level[$year]['remark'];
        }
    }else{
        $no_ms_level[] = 'sludge destination';
    }
    $output.='</p>';
}
else{
    $no_ms_level[] = 'sludge';
}



if(isset($ms_level[$year]['waste_water']['rate of re-used']) || !empty($dcp_reuse)){
    $output.='<p>';
    if(!empty($dcp_reuse)){
        $uses = array_unique(array_column($dcp_reuse, 'field_dcpirrigation_value'));
        if(!empty($uses)){
            foreach($uses as $k=>$v){
                if(!is_null($v) && isset($GLOBALS['uwwtd']['reuse_type'][$v])){
                    $uses[$k] = strtolower($GLOBALS['uwwtd']['reuse_type'][$v]);
                }else{
                    unset($uses[$k]);
                }
            }
        }
        
        $output.='At '.format_plural(count($dcp_reuse), '1 discharge point' , '@count discharge points').', treated waste water is reused for '.implode(' or ', $uses).'.';
    }
    
    if($ms_level[$year]['waste_water']['rate of re-used'] > 0) {
        $output.= 'In '. $year .' '.$ms_level[$year]['waste_water']['rate of re-used'].' % of treated waste water was reported to be reused';
    }
    if($ms_level[$year]['waste_water']['Re-used']!=''){
        $output.=' ('.$ms_level[$year]['waste_water']['Re-used'].')';
    }
    $output.='</p>';
}else{
    $no_ms_level[] = 'waste water reuse';
}
if(!empty($overflow)){
    $output.='<p>Storm water overflows case was reported on '.format_plural(count($overflow), '1 agglomeration' , '@count agglomerations').' for a total of '.uwwtd_format_number(round(array_sum($overflow),2)).' m3</p>';
}else{
    $no_ms_level[] = 'storm water overflows';
}
if(!empty($no_ms_level)){
    $output.='<p>No information was reported on '.implode(', ', $no_ms_level).' by '.$country_name.'.</p>';
}

    return '<div class="view-content">'.$output.'</div>';
}






//=================Art 17
function uwwtd_stat_art17_mslevel($options){
    $data = [];
    $query = '
        SELECT *
        FROM wfs.art17_mslevel    
    ';
    if(isset($options['year'])){
        $query.= 'WHERE year_of_datas = '.$options['year'];
    }
    try {
		$result = db_query($query,[]);
		while($row = $result->fetchAssoc()) {

			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}
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