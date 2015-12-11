<?php


function uwwtd_page_stats_querys()
{
	$title = 'Querys for national stats';
	drupal_set_title($title);  //todo
	/*
	$page =  '<h1 align="center">'.$title.'</h1><br/>';
	dsm('function uwwtd_stat_national_compliance');
	dsm(uwwtd_stat_national_compliance());
	dsm('function uwwtd_stat_national_uwwtp(\'compliance\')');
	dsm(uwwtd_stat_national_uwwtp('compliance'));
	dsm('function uwwtd_stat_national_uwwtp(\'treatmenttype\')');
	dsm(uwwtd_stat_national_uwwtp('treatmenttype'));
	dsm('function uwwtd_stat_national_treatmentplant');
	dsm(uwwtd_stat_national_treatmentplant());
	dsm('function uwwtd_stat_national_ms_level');
	dsm(uwwtd_stat_national_ms_level());
	dsm('function uwwtd_stat_national_uwwtp_agglo');
	dsm(uwwtd_stat_national_uwwtp_agglo());
	dsm('function uwwtd_stat_national_generated_by_agglomeration');
	dsm(uwwtd_stat_national_generated_by_agglomeration());
*/
$options = array('year'=> 2012);
//uwwtd_stat_collect_data($options);
$data = uwwtd_stat_sewage_network_data($options);
dsm($data);
	return 'fin';
}

function uwwtd_stat_sewage_network_data($options){
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
	$params = array(
		':typenode' => 'agglomeration',
		':agggenerated' => variable_get('min_aggenerated_display', '2000'), 
	);  
	try{  
		$result = db_query("
			SELECT s.field_aggsewagenetwork_value AS type, SUM(g.field_agggenerated_value) AS sn_sum,  COUNT (*) AS sn_count
			FROM {node n
				LEFT JOIN {field_data_field_anneedata} f_annee ON n.nid = f_annee.entity_id
				LEFT JOIN {field_data_field_agggenerated} g ON n.vid = g.revision_id
				LEFT JOIN {field_data_field_status} f_status ON n.nid = f_status.entity_id 
				LEFT JOIN {field_data_field_aggsewagenetwork} s ON n.nid = s.entity_id
			WHERE n.type = :typenode
				AND n.status = 1
				AND g.field_agggenerated_value > :agggenerated
			
		".$conditions."
		GROUP BY s.field_aggsewagenetwork_value
		ORDER BY s.field_aggsewagenetwork_value ASC"
		, $params);
		
		$out = array();
		while($row = $result->fetchAssoc()) {
			if($row['type']=='') $row['type']='NI';
			$out['count'][]=array(
				"value" => (integer)$row['sn_count'],
				"label" => $GLOBALS['ms_level']['sewage_network'][$row['type']]['label'],
				"valueformat" => uwwtd_format_number((integer)$row['sn_count']),
				"color"=>$GLOBALS['ms_level']['sewage_network'][$row['type']]['color'],
			);
			$out['sum'][]=array(
				"value" => (integer)$row['sn_sum'],
				"label" => $GLOBALS['ms_level']['sewage_network'][$row['type']]['label'],
				"valueformat" => uwwtd_format_number((integer)$row['sn_sum']) .' pe',
				"color"=>$GLOBALS['ms_level']['sewage_network'][$row['type']]['color'],
			);
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}
	return $out;
	
}

function uwwtd_stat_collect_data($options){
	
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
	$params = array(
		':typenode' => 'agglomeration',
		':agggenerated' => variable_get('min_aggenerated_display', '2000'), 
	);  
	  
	$result = db_query("
		SELECT
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
			AND g.field_agggenerated_value > :agggenerated
	".$conditions
	, $params);
	
	$out = array();
	if($row = $result->fetchAssoc()){
		
		foreach($row as $k=>$v){
			$out[]=array(
				"value" => (float)$v,
				"label" => $GLOBALS['ms_level']['connection'][$k]['label'],
				"valueformat" => uwwtd_format_number(round($v,0)).' pe',
				"color"=>$GLOBALS['ms_level']['connection'][$k]['color'],
			);
			
		}
		
	}
	
	return $out;
}



function uwwtd_stat_national_ms_level_preprocess_sludge_chart($data, $year){
    $chart_data =array();
    if(isset($data[$year]['sludge']['destination'])){
        foreach($data[$year]['sludge']['destination'] as $k=>$v){
            if($v>0){
                $chart_data[]=array(
                    'label'=>$GLOBALS['ms_level']['sludge'][$k]['label'],
                    'value'=>(float)$v,
                    'valueformat'=>$v.' T DS/year',
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
function uwwtd_stat_national_ms_level_data($options){
    $data = array();
    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'ms_level')
    ->propertyCondition('status', 1);
    if(isset($options['year'])){
        $query->fieldCondition('field_anneedata', 'value', $options['year'], '=');
    }

    if($result = $query->execute()){
        foreach($result['node'] as $id=>$obj){
            if($node = node_load($obj->nid)){
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
                if($data[$year]['sludge']['production'] > 0){
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

    /*
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

	$data = array();
	$param = array();

	$query = 'select
				SUM(f_msldischargepipelines.field_msldischargepipelines_value) AS disc_pipelines_SUM,
				SUM(f_msldischargeships.field_msldischargeships_value) AS disc_ships_SUM,
				SUM(f_msldischargeothers.field_msldischargeothers_value) AS disc_others_SUM,
				SUM(f_mslreusesoilagriculture.field_mslreusesoilagriculture_value) AS ru_agriculture_SUM,
				SUM(f_mslreuseothers.field_mslreuseothers_value) AS ru_others_SUM,
				SUM(f_msldisposallandfill.field_msldisposallandfill_value) AS disp_landfill_SUM,
				SUM(f_msldisposalincineration.field_msldisposalincineration_value) AS disp_incineration_SUM,
				SUM(f_msldisposalothers.field_msldisposalothers_value) AS disp_others_SUM
    from {node} n
    	left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
		 left join
            {field_data_field_msldischargepipelines} f_msldischargepipelines
                ON n.nid = f_msldischargepipelines.entity_id
		left join
            {field_data_field_msldischargeships} f_msldischargeships
                ON n.nid = f_msldischargeships.entity_id
		left join
            {field_data_field_msldischargeothers} f_msldischargeothers
                ON n.nid = f_msldischargeothers.entity_id
		left join
            {field_data_field_mslreusesoilagriculture} f_mslreusesoilagriculture
                ON n.nid = f_mslreusesoilagriculture.entity_id
		left join
            {field_data_field_mslreuseothers} f_mslreuseothers
                ON n.nid = f_mslreuseothers.entity_id
		left join
            {field_data_field_msldisposallandfill} f_msldisposallandfill
                ON n.nid = f_msldisposallandfill.entity_id
		left join
            {field_data_field_msldisposalincineration} f_msldisposalincineration
                ON n.nid = f_msldisposalincineration.entity_id
		left join
            {field_data_field_msldisposalothers} f_msldisposalothers
                ON n.nid = f_msldisposalothers.entity_id 
    where n.type = :typenode' . $conditions;


	$param = array(
			':typenode' => 'ms_level',
	);


	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {

			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	//     dsm($data);
    */
}

function uwwtd_stat_national_treatmentplant($options)
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
	$param = array();

	$query = 'select
					SUM(f_uwwbodincoming.field_uwwbodincoming_value)AS bodin_sum,
					SUM(f_uwwboddischarge.field_uwwboddischarge_value)AS bodout_sum,
					SUM(f_uwwcodincoming.field_uwwcodincoming_value)AS codin_sum,
					SUM(f_uwwcoddischarge.field_uwwcoddischarge_value)AS codout_sum,
					SUM(f_uwwnincoming.field_uwwnincoming_value)AS nin_sum,
					SUM(f_uwwndischarge.field_uwwndischarge_value)AS nout_sum,
					SUM(f_uwwpincoming.field_uwwpincoming_value)AS pin_sum,
					SUM(f_uwwpdischarge.field_uwwpdischarge_value)AS pout_sum,
					SUM(f_uwwwastewatertreated.field_uwwwastewatertreated_value)AS uwwt_sum
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
    where n.type = :typenode
    
    ' . $conditions;


	$param = array(
			':typenode' => 'uwwtp',
            
				);
//     dsm($query);
//     dsm($param);

	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			$row['bodin_sum_format'] = is_null($row['bodin_sum']) ? '0' : uwwtd_format_number($row['bodin_sum'], 2);
			$row['bodout_sum_format'] = is_null($row['bodout_sum']) ? '0' : uwwtd_format_number($row['bodout_sum'], 2);
			$row['codin_sum_format'] = is_null($row['codin_sum']) ? '0' : uwwtd_format_number($row['codin_sum'], 2);
			$row['codout_sum_format'] = is_null($row['codout_sum']) ? '0' : uwwtd_format_number($row['codout_sum'], 2);
			$row['nin_sum_format'] = is_null($row['nin_sum']) ? '0' : uwwtd_format_number($row['nin_sum'], 2);
			$row['nout_sum_format'] = is_null($row['nout_sum']) ? '0' : uwwtd_format_number($row['nout_sum'], 2);
			$row['pin_sum_format'] = is_null($row['pin_sum']) ? '0' : uwwtd_format_number($row['pin_sum'], 2);
			$row['pout_sum_format'] = is_null($row['pout_sum']) ? '0' : uwwtd_format_number($row['pout_sum'], 2);
			$row['uwwt_sum_format'] = is_null($row['uwwt_sum']) ? '0' : uwwtd_format_number($row['uwwt_sum'], 2);
			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	return $data;
}

function uwwtd_stat_national_uwwtp($options, $GroupBy){
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

	switch ($GroupBy){
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

	$query = 'select ' . $group_by . ',';
	$query .= ' COUNT(n.title) AS  TOTAL_COUNT,
			SUM(f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value) AS LOADENTERING_SUM
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
		where n.type = :typenode 
    
    ' .
		$conditions . '
		group by ' . $group_by;

	$param = array(
			':typenode' => 'uwwtp',
	);
//     dsm($query);
//     dsm($param);
	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			switch ($group_by){
				case 'f_comp.field_uwwcompliance_value' :
					$row['field_uwwcompliance_value_format'] = is_null($row['field_uwwcompliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_uwwcompliance_value']];
					$row['color'] = isset($GLOBALS['uwwtd']['compliance_color'][$row['field_uwwcompliance_value']])?$GLOBALS['uwwtd']['compliance_color'][$row['field_uwwcompliance_value']]:null;
			
                    break;
				case 'f_uwwtreatmenttype.field_uwwtreatmenttype_value' :
					$row['field_uwwtreatmenttype_value_format'] = is_null($row['field_uwwtreatmenttype_value']) ? '' : $GLOBALS['uwwtd']['ui']['treatment_type_in_place'][$row['field_uwwtreatmenttype_value']];
					$row['color'] = isset($GLOBALS['uwwtd']['treatment_type_color'][$row['field_uwwtreatmenttype_value']])?$GLOBALS['uwwtd']['treatment_type_color'][$row['field_uwwtreatmenttype_value']]:null;
			
                    break;
			}
			$row['total_count_format'] = is_null($row['total_count']) ? '0' : uwwtd_format_number($row['total_count'], 0);
			$row['loadentering_sum_format'] = is_null($row['loadentering_sum']) ? '0' : uwwtd_format_number($row['loadentering_sum'], 0);
            
			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	return $data;
}




function uwwtd_stat_national_compliance($options)
{

	$data = array();
	$param = array();
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
      

	$query = 'select f_comp.field_aggcompliance_value,
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
        and f_agggenerated.field_agggenerated_value > :agggenerated
    ' .
		$conditions . '
	group by f_comp.field_aggcompliance_value
    ';

	$param = array(
			':typenode' => 'agglomeration',
            ':agggenerated' => variable_get('min_aggenerated_display', '2000'),
	);
//     dsm($query);
//     dsm($param);
	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			$row['field_aggcompliance_value_format'] = is_null($row['field_aggcompliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggcompliance_value']];
			$row['total_count_format'] = is_null($row['total_count']) ? '0' : uwwtd_format_number($row['total_count'], 0);
			$row['aggc1_sum_format'] = is_null($row['aggc1_sum']) ? '0' : uwwtd_format_number($row['aggc1_sum'], 2);
			$row['aggc2_sum_format'] = is_null($row['aggc2_sum']) ? '0' : uwwtd_format_number($row['aggc2_sum'], 2);
			$row['aggpcwt_sum_format'] = is_null($row['aggpcwt_sum']) ? '0' : uwwtd_format_number($row['aggpcwt_sum'], 2);
			$row['generated_sum_format'] = is_null($row['generated_sum']) ? '0' : uwwtd_format_number($row['generated_sum'], 0);
            $row['color'] = isset($GLOBALS['uwwtd']['compliance_color'][$row['field_aggcompliance_value']])?$GLOBALS['uwwtd']['compliance_color'][$row['field_aggcompliance_value']]:null;
			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	//     dsm($data);
	return $data;
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

function uwwtd_stat_national_generated_by_agglomeration($options)
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
	$param = array();
	$aConditions = array(
		'lower than 10000 pe' => ' and (f_agggenerated.field_agggenerated_value > 2000 and f_agggenerated.field_agggenerated_value <= 10000) ',
		'10000 to 100000 pe' => ' and (f_agggenerated.field_agggenerated_value > 10000 and f_agggenerated.field_agggenerated_value <= 100000) ',
		'greater than 100000 pe' => ' and (f_agggenerated.field_agggenerated_value > 100000) ',
	);

	$query = 'select
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
//     dsm($query);
//     dsm($param);
	try {
			foreach ($aConditions as $libelle => $condition){
				$result = db_query($query . $condition,$param);
				while($row = $result->fetchAssoc()) {


					$row['type'] = $libelle;
					$row['generated_count_format'] = is_null($row['number']) ? '0' : uwwtd_format_number($row['number'], 0);
					$row['generated_sum_format'] = is_null($row['generated_sum']) ? '0' : uwwtd_format_number($row['generated_sum'], 0);

					$data[] = $row;
				}
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}


	return $data;
}

?>