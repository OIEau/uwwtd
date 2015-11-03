<?php

function uwwtd_page_stats_graphs(){

	require_once 'uwwtd.stats.inc';
	drupal_add_js('sites/all/libraries/d3/d3.v3.min.js');
	drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
	drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');


	$page = "";
	$title = 'Graphs for national stats';
	drupal_set_title($title);  //todo
	$page =  '<h1 align="center">'.$title.'</h1><br/>';

	$Data_uwwtd_stat_national_compliance = array();

	$results = uwwtd_stat_national_compliance();
	foreach ($results as $result){
		$Data_uwwtd_stat_national_compliance[] = array(
				"value" => $result['generated_sum'],
				"label" => $result['field_aggcompliance_value_format'],
				"valueformat" => $result['generated_sum_format'],
		);
	}

	$aData_uwwtd_stat_national_uwwtpC = array();

	$results = uwwtd_stat_national_uwwtp('compliance');
	foreach ($results as $result){
		$aData_uwwtd_stat_national_uwwtpC[] = array(
				"value" => $result['loadentering_sum'],
				"label" => $result['field_uwwcompliance_value_format'],
				"valueformat" => $result['loadentering_sum_format'],
		);
	}

	$aData_uwwtd_stat_national_uwwtpT = array();

	$results = uwwtd_stat_national_uwwtp('treatmenttype');
	foreach ($results as $result){
		$aData_uwwtd_stat_national_uwwtpT[] = array(
				"value" => $result['loadentering_sum'],
				"label" => $result['field_uwwtreatmenttype_value_format'],
				"valueformat" => $result['loadentering_sum_format'],
		);
	}

	$aData_uwwtd_stat_national_generated_by_agglomeration = array();

	$results = uwwtd_stat_national_generated_by_agglomeration();
	foreach ($results as $result){

		$aData_uwwtd_stat_national_generated_by_agglomeration[] = array(
				"type" => $result['type'],
        		"generated" => $result['generated_sum'],
		);

	}

	$aData_uwwtd_load_ent_and_dis = array();

	$results = uwwtd_stat_national_treatmentplant();
	foreach ($results as $result){

		$aData_uwwtd_load_ent_and_dis[] = array(
				"type" => "BOD",
				"entering" => $result['bodin_sum'],
				"discharged" => $result['bodout_sum']
		);
		$aData_uwwtd_load_ent_and_dis[] = array(
				"type" => "COD",
				"entering" => $result['codin_sum'],
				"discharged" => $result['codout_sum']
		);
		$aData_uwwtd_load_ent_and_dis[] = array(
				"type" => "N",
				"entering" => $result['nin_sum'],
				"discharged" => $result['nout_sum']
		);
		$aData_uwwtd_load_ent_and_dis[] = array(
				"type" => "P",
				"entering" => $result['pin_sum'],
				"discharged" => $result['pout_sum']
		);


	}
	dsm($aData_uwwtd_load_ent_and_dis);
	$aColor_uwwtd_load_ent_and_dis['domain'] = array ('entering', 'discharged');
	$aColor_uwwtd_load_ent_and_dis['range'] = array ('#74FFE0', '#C00000');

	$page .= '<fieldset><legend>Total generated load by compliance</legend><div id="graph_generatedByCompliance"/></fieldset>';
	$page .= '<fieldset><legend>Total load entering by compliance</legend><div id="graph_loadenteringByCompliance"/></fieldset>';
	$page .= '<fieldset><legend>Total load entering by treatment type</legend><div id="graph_loadenteringByTreatmenttype"/></fieldset>';
	$page .= '<fieldset><legend>Total generated load by agglomeration</legend><svg id="graph_generatedByAgglomeration"/></fieldset>';
	$page .= '<fieldset><legend>Total load entering and discharged</legend><svg id="graph_load_ent_and_dis"/></fieldset>';
	$page .= "
    <script>
    jQuery(document).ready(function(){
        display_piechart_custom(".json_encode($Data_uwwtd_stat_national_compliance).",'graph_generatedByCompliance','end');
        display_piechart_custom(".json_encode($aData_uwwtd_stat_national_uwwtpC).",'graph_loadenteringByCompliance','middle');
        display_piechart_custom(".json_encode($aData_uwwtd_stat_national_uwwtpT).",'graph_loadenteringByTreatmenttype','middle');
        stackedbar_nat_gen_load(".json_encode($aData_uwwtd_stat_national_generated_by_agglomeration).");
        stackedbar_load_ent_and_dis(".json_encode($aData_uwwtd_load_ent_and_dis).','. json_encode($aColor_uwwtd_load_ent_and_dis).");

    });
    </script>
    ";


	return $page;

}



?>