<?php
require_once 'uwwtd.stats.inc';

define(ALL_YEARS, 'allyears');

function uwwtd_stats_graphs_page(){

	$form = drupal_get_form('uwwtd_stats_graphs_form');

	return render($form);
}

function uwwtd_stats_graphs_form($form, &$form_state){
    $year = uwwtd_get_max_annee();
	$form['#prefix'] = '<h1 align="center">' . t('Graphs for national stats') . '</h1>';
	

	// Get available years for data
		$query = db_select('field_data_field_anneedata', 'a');
		$query->fields('a', array('field_anneedata_value'));
		$query->orderBy('field_anneedata_value', 'DESC');
		$results = $query->execute();
		//$years = array('all' => 'Show all');
        $years = array();
        $count = 0;
		while($record = $results->fetchAssoc()) {
			$years[$record['field_anneedata_value']] = $record['field_anneedata_value'];
            if($record['field_anneedata_value']!=$year){$count++;}
		}
		
        // TODO : add RBD, nuts and other filters ?
        // If we have more than one year of availability
        if($count){
        	// Ajout du choix pour toutes les années :
        	$years[ALL_YEARS] = 'All years';
        	
            $form['description'] = array('#markup' => '<p>'.t('Select the year to generate the graphs.').'</p>');
            $form['yearselected'] = array(
            	'#title' => t('Available years'),
                '#type' => 'select',
                '#options' => $years,
                '#default_value' => isset($form_state['values']['year']) ? $form_state['values']['year'] : $year,
            );
            $form['yearspossibles'] = array(
            	'#type' => 'hidden',
            	'#value' => $years,
            );
            $form['submit'] = array(
            	'#type' => 'submit',
            	'#attributes' => array('class' => array('btn-primary', 'btn')),
            	'#value' => t('Refresh graphs')
            );
        }
        
		if(!isset($form_state['storage']['result']) && empty($form_state['input'])) {
			$form_state['storage']['result'] = uwwtd_page_stats_graphs_result($year);
		}
        $form['#suffix'] .= $form_state['storage']['result'];
	return $form;

}

/**
 * Form validation.
 */
function uwwtd_stats_graphs_form_validate(&$form, &$form_state) {


}



/**
 * Form submit.
 */
function uwwtd_stats_graphs_form_submit(&$form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$table = uwwtd_page_stats_graphs_result($form_state['values']['yearselected'], $form_state['values']['yearspossibles']);
	$form_state['storage']['result'] = $table;
}


/**
 * Retourne le code html du contenu de la page stats/graphs.
 */
function uwwtd_page_stats_graphs_result($yearSelected = array(), $yearsPossibles = array()) {
	drupal_add_js('sites/all/libraries/d3/d3.v3.min.js');
	drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
	drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');

	// Valeur par défaut :
	if (empty($yearSelected)) {
		$yearSelected = uwwtd_get_max_annee();
	}
	
	$formResult = "";
	
	// Texte d'introduction :
	if ($yearSelected != ALL_YEARS) {
		$formResult.='<br /><p class="national-stat-str" align="justify">'.uwwtd_get_national_stat_str($yearSelected).'</p>';
	}
	
	
	//==============================================
    //=============== ROW 1: Agglomeration : Generated load by collection
    //==============================================

	// Récupération des données des graphs:
    $frontAndBackGraph1 = getRow1Graph1($yearSelected, $yearsPossibles);
    $frontAndBackGraph2 = getRow1Graph2($yearSelected, $yearsPossibles);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsPies($formResult, t('Agglomeration : Generated load by collection'), 
    	t("Generated Load by collection type"), 'graph_collect_type', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
    	t("Generated Load by sewage network type"), 'graph_cs_type', $frontAndBackGraph2[0], $frontAndBackGraph2[1]);
    
    
    
    // S'il n'y a qu'une année de sélectionnée :
//     if (count($yearsSelected) == 1) {
//     	// Récupération des données des graphs:
//     	$frontAndBackGraph1 = getRow1Graph1($yearsSelected[0]);
//     	$frontAndBackGraph2 = getRow1Graph2($yearsSelected[0]);
    	 
//     	// Création du code html pour ces graphs :
//     	$formResult = getHtmlPlusHtmlChartsPies($formResult, t('Agglomeration : Generated load by collection'),
//     			t("Generated Load by collection type"), 'graph_collect_type', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
//     			t("Generated Load by sewage network type"), 'graph_cs_type', $frontAndBackGraph2[0], $frontAndBackGraph2[1]);
//     } else {
//     	$frontAndBackGraphs1 = array();
//     	$frontAndBackGraphs2 = array();
    
//     	// Pour chaque année selectionnée :
//     	foreach($yearsSelected as $yearSelected) {
//     		// Récupération des données du graph :
//     		$frontAndBackGraphs1[] = getRow1Graph1($yearSelected);
//     		$frontAndBackGraphs2[] = getRow1Graph2($yearSelected);
//     	}
//     	$formResult = getHtmlPlusHtmlChartsBars($formResult, t("Agglomeration : Generated load by collection"),
//     			$yearsSelected, 'graph_collect_type', t("Generated Load by collection type"), $frontAndBackGraphs1,
//     			$yearsSelected, 'graph_cs_type', t("Generated Load by sewage network type"), $frontAndBackGraphs2);
//     }
    
    
    //==============================================
    //=============== ROW 2 : Agglomeration : Generated load by compliance
    //==============================================
    
// 	// Récupération des données des graphs:
// 	$national_compliance = uwwtd_stat_national_compliance($options);
// 	$frontAndBackGraph1 = getRow2Graph1($national_compliance);
// 	$frontAndBackGraph2 = getRow2Graph2($national_compliance);
	
// 	// Création du code html pour ces graphs :
// 	$formResult = getHtmlPlusHtmlChartsPies($formResult, t("Agglomeration : Generated load by compliance"),
// 			t("Total generated load by compliance"), 'graph_generatedByCompliance', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
// 			t("Number of agglomerations by compliance"), 'graph_agglomerationByCompliance', $frontAndBackGraph2[0], $frontAndBackGraph2[1]);
    
    
    //==============================================
    //=============== ROW 3 : Waste water treatement plant : Load entering by compliance
    //==============================================
    
//     // Récupération des données des graphs:
//     $frontAndBackGraph1 = getRow3Graph1($options);
//     $frontAndBackGraph2 = getRow3Graph2($options);
    
//     // Création du code html pour ces graphs :
//     $formResult = getHtmlPlusHtmlChartsPies($formResult, t("Waste water treatement plant : Load entering by compliance"),
//     		t("Total load entering by compliance"), 'graph_loadenteringByCompliance', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
//     		t("Number of treatment plant by compliance"), 'graph_loadenteringByComplianceAgglomeration', $frontAndBackGraph2[0], $frontAndBackGraph2[1]);
    

    //==============================================
    //=============== ROW 4 : Waste water treatement plant : Load entering by treatment type
    //==============================================

//     // Récupération des données des graphs:
//     $frontAndBackGraph1 = getRow4Graph1($options);
//     $frontAndBackGraph2 = getRow4Graph2($options);
    
//     // Création du code html pour ces graphs :
//     $formResult = getHtmlPlusHtmlChartsPies($formResult, t("Waste water treatement plant : Load entering by treatment type"),
//     		t("Total load entering by treatment type"), 'graph_loadenteringByTreatmenttype', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
//     		t("Number of treatment plant by treatment type"), 'graph_loadenteringByTreatmenttypeAgglomeration', $frontAndBackGraph2[0], $frontAndBackGraph2[1]);
    
	
    //==============================================
    //=============== ROW 5 : Agglomeration : generated load by agglomeration size
    //==============================================
    
//     // Récupération des données des graphs:
//     $frontAndBackGraph1And2 = getRow5Graphs1And2($options);
    
//     // Création du code html pour ces graphs :
//     $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Agglomeration : generated load by agglomeration size"),
//     		array(t("Generated load")), 'graph_generatedByAgglomeration', t("Total generated load by agglomeration size"), $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1],
//     		array(t("Number of agglomeration")), 'graph_generatedByAgglomerationNumber', t("Number of agglomeration by size"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3]);
    
    
    //==============================================
    //=============== ROW 6 : Waste water treatement plant : Total load entering and discharged
    //==============================================
    
//     // Récupération des données des graphs:
//     $frontAndBackGraph1 = getRow6Graph1($options);
//     $frontAndBackGraph2 = getRow6Graph2($options);
    
//     // Création du code html pour ces graphs :
//     $legend =  array(t('incoming'), t('discharged'));
//     $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Waste water treatement plant : Total load entering and discharged"),
//     		$legend, 'uwwtd_load_ent_and_dis', t("Total generated load by agglomeration size"), $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
//     		$legend, 'per_ent_and_dis', t("Number of agglomeration by size"), $frontAndBackGraph2[0], $frontAndBackGraph2[1]);
    
	
	//==============================================
    //=============== ROW 7 : MS Level statistics
    //==============================================
    
//     // Récupération des données des graphs:
//     $ms_levelFrontAndBackGraph1 = getRow7Graph1($options);
//     $frontGraph2 = getRow7Graph2($ms_levelFrontAndBackGraph1[0], $options);
    
//     // Création du code html pour ces graphs :
//     $formResult = getHtmlPlusHtmlChartPieAndText($formResult, t("MS Level statistics"),
//     	t("Sludge destination"), 'graph_sludge_dest', $ms_levelFrontAndBackGraph1[0],  $ms_levelFrontAndBackGraph1[1], $ms_levelFrontAndBackGraph1[2],
//     	t("Water re-use"), $frontGraph2, $options);
	
	return  $formResult;
}


/**
 * Retourne les données pour le graph "Generated Load by collection type".
 */
function getRow1Graph1($yearSelected, $yearsPossibles) {
	$back = '';
	$front = array();
	$frontTmp = uwwtd_stat_collect_data($yearSelected, $yearsPossibles);
	if (!empty($frontTmp)) {
		$front = $frontTmp;
		$rows = array();
	
		// Transforme les clefs en 0, 1, 2, ...
		$dataByYears = array_values($front);
		
		// Pour chaque groupe d'éléments de la première année :
		$destinations = array();
		if (array_key_exists('0', $dataByYears) && !empty($dataByYears[0])) {
			// Pour chaque groupe d'éléments :
			$isFirst = true;
			foreach ($dataByYears[0] as $elements) {
				// Discard le premier groupe d'éléments :
				if ($isFirst) {
					$isFirst = false;
				} else {
					// Récupération de l'intitulé de la destination :
					if (array_key_exists('label', $elements) && !empty($elements['label'])) {
						$destinations[] = $elements['label'];
					}
				}
			}
				
			// Ajout du Total :
			$destinations[] = t('Total');
		}
		
		// Pour les entetes du tableau :
		$columns = array();
		$columns[0][]['data'] = 'Destination';
		
		// Pour chaque destinations possibles :
		foreach ($destinations as $destination) {
			// Ecriture des intitulés de destination :
			$rows[$destination]['data']['Destination'] = $destination;
		}
		
		// Pour chaque année :
		foreach ($front as $year => $groupElements) {
			// Récupération du total :
			$all = array_shift($groupElements);
				
			// Pour les entetes du tableau :
			$columns[0][]['data'] = $year;
				
			// Remise à zéro du Total :
			$sum = 0;
			$sum_rate = 0;
				
			// Pour chaque groupe d'éléments :
			foreach ($groupElements as $elements) {
				// Pour chaque destinations :
				foreach ($destinations as $destination) {
					// Si la destination correspond au label :
					if (array_key_exists('label', $elements) && ($destination == $elements['label'] || $destination == t('Total'))) {
						// Si c'est la ligne Total :
						if ($destination == t('Total')) {
							$rows[$destination]['data'][$year] = uwwtd_format_number($sum).' pe, '.  uwwtd_format_number($sum_rate).' %';
						} else {
							$rows[$destination]['data'][$year] = $elements['valueformat'] .', '. round($elements['value'] / $all['value']*100 ,1) .' %';
							$sum += $elements['value'];
							$sum_rate += round($elements['value'] / $all['value']*100);
						}
					}
				}
			}
		}
	
		// Récupération du DOM du tableau :
		$back = theme('table', array('rows' => $rows, 'header' => $columns, 'rows_multiple' => true));
		
		// Ajout des légendes :
		$back .= '
		<table class="stats-graphs table table-hover table-striped sticky-enabled">
 			<thead> 
				<tr>
					<th><i>*pe: Population equivalent, *%: Rate</i></th>
				</tr>
			</thead>
		</table>';
		
	}
	return array($front, $back);
}

/**
 * Retourne les données pour le graph "Generated Load by sewage network type" :
 */
function getRow1Graph2($yearSelected, $yearsPossibles) {
	$back = '';
	$front = array();
	$frontTmp = uwwtd_stat_sewage_network_data($yearSelected, $yearsPossibles);
	if (!empty($frontTmp)) {
		$front = $frontTmp;
		$rows = $front;
		$columns = array();
		
		// Pour chaque années possible :
		$columns[0][]['data'] = t("Network type");
		
		// Pour chaque Network type :
		foreach ($front as $newtworkTypeName => $networkType) {
			// Si ce NetworkType contient des data :
			if (array_key_exists('data', $networkType)) {
				// Ajout au début du tableau de la case avec l'intitulé du network type :
				array_unshift($rows[$newtworkTypeName]['data'], $newtworkTypeName);
				
				// Pour chaque année :
				foreach (array_keys($networkType['data']) as $year) {
					// Récupération des entetes du tableau :
					$columns[0][$year]['data'] = $year;
				}
			}
		}
		
		// Récupération du DOM du tableau :
		$back = theme('table', array('rows' => $rows, 'header' => $columns, 'rows_multiple' => true));
		
		// Ajout des légendes :
		$back .= '
		<table class="stats-graphs table table-hover table-striped sticky-enabled">
 			<thead>
				<tr>
					<th><i>*pe: Population equivalent, *: Number of agglomeration</i></th>
				</tr>
			</thead>
		</table>';
	} else {
		$front['sum'] = array();
	}
	return array($front['sum'], $back);
}

/**
 * Retourne les données pour le graph "Total generated load by compliance".
 */
function getRow2Graph1($national_compliance) {
	$Data_uwwtd_stat_national_compliance = array();
	$String_uwwtd_stat_national_compliance = array();
	$sum = 0;
	
	if (!empty($national_compliance)) {
		foreach ($national_compliance as $result){
			$Data_uwwtd_stat_national_compliance[] = array(
							"value" => $result['generated_sum'],
							"label" => $result['field_aggcompliance_value_format'] .' ['.uwwtd_format_number($result['generated_sum']).' pe]',
							"valueformat" => $result['generated_sum_format'].' pe',
							"color"=>isset($result['color'])?$result['color']:null
			);
			$sum+=$result['generated_sum'];
			$String_uwwtd_stat_national_compliance[]= array($result['field_aggcompliance_value_format'], $result['generated_sum_format'] . ' pe');
		}
	}
	$String_uwwtd_stat_national_compliance[] = array('Total', uwwtd_format_number($sum) . ' pe');
	$String_uwwtd_stat_national_compliance = theme('table', array("rows"=>$String_uwwtd_stat_national_compliance,"header"=>array(t("Compliance state"), t("Generated load"))));
	
	return array($Data_uwwtd_stat_national_compliance, $String_uwwtd_stat_national_compliance);
}

/**
 * Retourne les données pour le graph "Number of agglomerations by compliance".
 */
function getRow2Graph2($national_compliance) {
	$Data_uwwtd_stat_agglomeration_compliance = array();
	$String_uwwtd_stat_agglomeration_compliance = array();
	$sum = 0;
	
	if (!empty($national_compliance)) {
		foreach ($national_compliance as $result){
			$Data_uwwtd_stat_agglomeration_compliance[] = array(
							"value" => (integer)$result['total_count'],
							"label" => $result['field_aggcompliance_value_format'] .' ['.uwwtd_format_number($result['total_count']).']',
							"valueformat" => $result['total_count_format'],
							"color"=>isset($result['color'])?$result['color']:null
			);
			$sum += (integer)$result['total_count'];
			$String_uwwtd_stat_agglomeration_compliance[]= array($result['field_aggcompliance_value_format'],$result['total_count_format']);
		}
	}
	$String_uwwtd_stat_agglomeration_compliance[]= array('Total',$sum);
	$String_uwwtd_stat_agglomeration_compliance = theme('table', array("rows"=>$String_uwwtd_stat_agglomeration_compliance,"header"=>array(t("Compliance state"), t("Number"))));
	
	return array($Data_uwwtd_stat_agglomeration_compliance, $String_uwwtd_stat_agglomeration_compliance);
}

/**
 * Retourne les données pour le graph "Total load entering by compliance".
 */
function getRow3Graph1($options) {
	$aData_uwwtd_stat_national_uwwtpC = array();
	$aString_uwwtd_stat_national_uwwtpC = array();
	$results = uwwtd_stat_national_uwwtp($options,'compliance');
	$sum = 0;
	
	foreach ($results as $result){
		$aData_uwwtd_stat_national_uwwtpC[] = array(
						"value" => $result['loadentering_sum'],
						"label" => $result['field_uwwcompliance_value_format'].' ['.$result['loadentering_sum'].' pe]',
						"valueformat" => $result['loadentering_sum_format'].' pe',
						"color"=>isset($result['color'])?$result['color']:null
		);
		$sum+=$result['loadentering_sum'];
		$aString_uwwtd_stat_national_uwwtpC[]= array($result['field_uwwcompliance_value_format'] , $result['loadentering_sum_format'] . ' pe');
	}
	$aString_uwwtd_stat_national_uwwtpC[]= array(t('Total'),uwwtd_format_number($sum) . ' pe');
	$aString_uwwtd_stat_national_uwwtpC = theme('table', array("rows"=>$aString_uwwtd_stat_national_uwwtpC,"header"=>array(t("Compliance state"), t("Load entering"))));
	
	return array($aData_uwwtd_stat_national_uwwtpC, $aString_uwwtd_stat_national_uwwtpC);
}

/**
 * Retourne les données pour le graph "Number of treatment plant by compliance".
 */
function getRow3Graph2($options) {
	$aData_uwwtd_stat_agglomeration_uwwtpC = array();
	$aString_uwwtd_stat_agglomeration_uwwtpC = array();
	$results = uwwtd_stat_national_uwwtp($options,'compliance');
	$sum = 0;
	
	foreach ($results as $result){
		$aData_uwwtd_stat_agglomeration_uwwtpC[] = array(
						"value" => $result['total_count'],
						"label" => $result['field_uwwcompliance_value_format'] .' ['.$result['total_count'].']',
						"valueformat" => $result['total_count_format'],
						"color"=>isset($result['color'])?$result['color']:null
		);
		$sum+=$result['total_count'];
		$aString_uwwtd_stat_agglomeration_uwwtpC[]= array($result['field_uwwcompliance_value_format'], $result['total_count_format'] );
	}
	$aString_uwwtd_stat_agglomeration_uwwtpC[]= array('Total',uwwtd_format_number($sum));
	$aString_uwwtd_stat_agglomeration_uwwtpC = theme('table', array("rows"=>$aString_uwwtd_stat_agglomeration_uwwtpC,"header"=>array(t("Compliance state"), t("Number"))));
	
	return array($aData_uwwtd_stat_agglomeration_uwwtpC, $aString_uwwtd_stat_agglomeration_uwwtpC);
}

/**
 * Retourne les données pour le graph "Total load entering by treatment type".
 */
function getRow4Graph1($options) {
	$aData_uwwtd_stat_national_uwwtpT = array();
	$aString_uwwtd_stat_national_uwwtpT = array();
	$results = uwwtd_stat_national_uwwtp($options,'treatmenttype');
	$sum = 0;
	
	foreach ($results as $result){
		$aData_uwwtd_stat_national_uwwtpT[] = array(
						"value" => $result['loadentering_sum'],
						"label" => $result['field_uwwtreatmenttype_value_format'] .' ['.uwwtd_format_number($result['loadentering_sum']).' pe]',
						"valueformat" => $result['loadentering_sum_format'],
						"color"=>isset($result['color'])?$result['color']:null
		);
		$sum+=$result['loadentering_sum'];
		$aString_uwwtd_stat_national_uwwtpT[]= array($result['field_uwwtreatmenttype_value_format'], $result['loadentering_sum_format'].' pe' );
	}
	$aString_uwwtd_stat_national_uwwtpT[]= array(t('Total'),uwwtd_format_number($sum).' pe');
	$aString_uwwtd_stat_national_uwwtpT = theme('table', array("rows"=>$aString_uwwtd_stat_national_uwwtpT,"header"=>array(t("Compliance state"), t("Load entering"))));
	
	return array($aData_uwwtd_stat_national_uwwtpT, $aString_uwwtd_stat_national_uwwtpT);
}

/**
 * Retourne les données pour le graph "Number of treatment plant by treatment type".
 */
function getRow4Graph2($options) {
	$aData_uwwtd_stat_agglomeration_uwwtpT = array();
	$aString_uwwtd_stat_agglomeration_uwwtpT = array();
	$results = uwwtd_stat_national_uwwtp($options,'treatmenttype');
	$sum = 0;
	
	foreach ($results as $result){
		$aData_uwwtd_stat_agglomeration_uwwtpT[] = array(
						"value" => $result['total_count'],
						"label" => $result['field_uwwtreatmenttype_value_format'] .' ['.$result['total_count'].']',
						"valueformat" => $result['total_count_format'],
						"color"=>isset($result['color'])?$result['color']:null
		);
		$sum+=$result['total_count'];
		$aString_uwwtd_stat_agglomeration_uwwtpT[] = array($result['field_uwwtreatmenttype_value_format'], $result['total_count_format']);
	}
	$aString_uwwtd_stat_agglomeration_uwwtpT[]= array(t('Total'),uwwtd_format_number($sum));
	$aString_uwwtd_stat_agglomeration_uwwtpT = theme('table', array("rows"=>$aString_uwwtd_stat_agglomeration_uwwtpT,"header"=>array(t("Compliance state"), t("Number"))));
	
	return array($aData_uwwtd_stat_agglomeration_uwwtpT, $aString_uwwtd_stat_agglomeration_uwwtpT);
}

/**
 * Retourne les données pour le graph "Total generated load by agglomeration size" et
 * pour le graph "Number of agglomeration by size".
 */
function getRow5Graphs1And2($options) {
	$aData_uwwtd_stat_national_generated_by_agglomeration = array();
	$aString_uwwtd_stat_national_generated_by_agglomeration = array();
	$aData_uwwtd_stat_national_generated_by_agglomeration_count = array();
	$aString_uwwtd_stat_national_generated_by_agglomeration_count = array();
	$results = uwwtd_stat_national_generated_by_agglomeration($options);
	$sum = 0;
	$sum_count = 0;
	
	foreach ($results as $result){
		$aData_uwwtd_stat_national_generated_by_agglomeration[] = array($result['type'], (integer)$result['generated_sum']);
		$sum+=$result['generated_sum'];
		$aString_uwwtd_stat_national_generated_by_agglomeration[]= array($result['type'] , uwwtd_format_number($result['generated_sum']).' pe');
		$aData_uwwtd_stat_national_generated_by_agglomeration_count[] = array($result['type'], (integer)$result['number']);
		$sum_count+=(integer)$result['number'];
		$aString_uwwtd_stat_national_generated_by_agglomeration_count[]= array($result['type'] , uwwtd_format_number($result['number']));
	}
	$aString_uwwtd_stat_national_generated_by_agglomeration[]= array('Total',uwwtd_format_number($sum).' pe');
	$aString_uwwtd_stat_national_generated_by_agglomeration_count[]= array('Total',uwwtd_format_number($sum_count));
	$aString_uwwtd_stat_national_generated_by_agglomeration = theme('table', array("rows"=>$aString_uwwtd_stat_national_generated_by_agglomeration, "header"=>array(t("Size"),t("Values"))));
	$aString_uwwtd_stat_national_generated_by_agglomeration_count = theme('table', array("rows"=>$aString_uwwtd_stat_national_generated_by_agglomeration_count, "header"=>array(t("Size"),t("Values"))));
	
	return array($aData_uwwtd_stat_national_generated_by_agglomeration, $aString_uwwtd_stat_national_generated_by_agglomeration,
		$aData_uwwtd_stat_national_generated_by_agglomeration_count, $aString_uwwtd_stat_national_generated_by_agglomeration_count);
}

/**
 * Retourne les données pour le graph "Total load entering and discharged".
 */
function getRow6Graph1($options) {
	$aData_uwwtd_load_ent_and_dis = array();
	$aString_uwwtd_load_ent_and_dis = '<table>';
	$aString_uwwtd_load_ent_and_dis.= '<tr><th>'.t("Parameter").'</th><th>'.t("Entering load (t/year)").'</th><th>'.t("Discharged load (t/year)").'</th></tr>';
	$results = uwwtd_stat_national_treatmentplant($options);
	foreach ($results as $result){
		$aData_uwwtd_load_ent_and_dis[] = array("BOD", (float)$result['bodin_sum'], (float)$result['bodout_sum']);
		$aString_uwwtd_load_ent_and_dis .= '<tr><td>BOD</td><td>' . $result['bodin_sum_format'] . '</td><td>'.$result['bodout_sum_format'] . '</td></tr>';
		$aData_uwwtd_load_ent_and_dis[] = array("COD", (float)$result['codin_sum'], (float)$result['codout_sum']);
		$aString_uwwtd_load_ent_and_dis .= '<tr><td>COD</td><td>' . $result['codin_sum_format'] . '</td><td>'.$result['codout_sum_format'] . '</td></tr>';
		$aData_uwwtd_load_ent_and_dis[] = array("N", (float)$result['nin_sum'], (float)$result['nout_sum']);
		$aString_uwwtd_load_ent_and_dis .= '<tr><td>N</td><td>' . $result['nin_sum_format'] . '</td><td>'.$result['nout_sum_format'] . '</td></tr>';
		$aData_uwwtd_load_ent_and_dis[] = array("P", (float)$result['pin_sum'], (float)$result['pout_sum']);
		$aString_uwwtd_load_ent_and_dis .= '<tr><td>P</td><td>' . $result['pin_sum_format'] . '</td><td>'.$result['pout_sum_format'] . '</td></tr>';
	}
	$aString_uwwtd_load_ent_and_dis.= '</table>';
	
	return array($aData_uwwtd_load_ent_and_dis, $aString_uwwtd_load_ent_and_dis);
}

/**
 * Retourne les données pour le graph "BOD, COD, N, P concentration".
 */
function getRow6Graph2($options) {
	$aData_uwwtd_per_ent_and_dis = array();
	$aString_uwwtd_per_ent_and_dis = array();
	$results = uwwtd_stat_national_treatmentplant($options);
	foreach ($results as $result) {
		$aData_uwwtd_per_ent_and_dis[] = array("BOD", 
			(float)($result['bodin_sum']/$result['uwwt_sum'])*bcpow(10, 9), 
			(float)($result['bodout_sum']/$result['uwwt_sum'])*bcpow(10, 9));
		$aString_uwwtd_per_ent_and_dis[] = array('BOD', 
			round($result['bodin_sum_format']/$result['uwwt_sum']*bcpow(10, 9) , 1), 
			round($result['bodout_sum_format']/$result['uwwt_sum']*bcpow(10, 9), 1), 
			round(((1- ($result['bodout_sum']/ $result['bodin_sum']))*100), 1) . ' %');
		$aData_uwwtd_per_ent_and_dis[] = array("COD", 
			(float)($result['codin_sum']/$result['uwwt_sum'])*bcpow(10, 9), 
			(float)($result['codout_sum']/$result['uwwt_sum'])*bcpow(10, 9));
		$aString_uwwtd_per_ent_and_dis[] = array('COD', 
			round($result['codin_sum_format']/$result['uwwt_sum']*bcpow(10, 9), 1), 
			round($result['codout_sum_format']/$result['uwwt_sum']*bcpow(10, 9), 1), 
			round(((1- ($result['codout_sum_format']/ $result['codin_sum_format']))*100), 1) . ' %');
		$aData_uwwtd_per_ent_and_dis[] = array("N", 
			(float)($result['nin_sum']/$result['uwwt_sum'])*bcpow(10, 9), 
			(float)($result['nout_sum']/$result['uwwt_sum'])*bcpow(10, 9));
		$aString_uwwtd_per_ent_and_dis[] = array('N', 
			round(($result['nin_sum_format']/$result['uwwt_sum'])*bcpow(10, 9), 1) , 
			round(($result['nout_sum_format']/$result['uwwt_sum'])*bcpow(10, 9), 1), 
			round(((1- ($result['nout_sum_format']/ $result['nin_sum_format']))*100), 1) . ' %');
		$aData_uwwtd_per_ent_and_dis[] = array("P", 
			(float)($result['pin_sum']/$result['uwwt_sum'])*bcpow(10, 9), 
			(float)($result['pout_sum']/$result['uwwt_sum'])*bcpow(10, 9));
	
		//FIX ME : why only 10^6
		$aString_uwwtd_per_ent_and_dis[]=array( 'P',
			round(($result['pin_sum']/$result['uwwt_sum'])*bcpow(10, 6), 1) ,
			round(($result['pout_sum']/$result['uwwt_sum'])*bcpow(10, 6), 1),
			round(((1- ($result['pout_sum_format']/ $result['pin_sum_format']))*100), 1) . ' %'
		);
	}
	$aString_uwwtd_per_ent_and_dis= theme('table', array("header"=>array('Parameter','Entering (mg/l)','Discharged (mg/l)','Rate (%)'), "rows"=>$aString_uwwtd_per_ent_and_dis));
	
	return array($aData_uwwtd_per_ent_and_dis, $aString_uwwtd_per_ent_and_dis);
}

/**
 * Retourne les données pour le graph "Sludge destination".
 */
function getRow7Graph1($options) {
	$sludge_destination_str = '';
	$ms_level = uwwtd_stat_national_ms_level_data($options);
	$sludge_destination = uwwtd_stat_national_ms_level_preprocess_sludge_chart($ms_level, $options['year']);
	if (!empty($sludge_destination)) {
		$lines= array();
		foreach ($sludge_destination as $k=>$dest) {
			$lines[] = array(
				$dest['label'],
				$dest['valueformat'],
			);
			$sludge_destination[$k]['label'].=' ['.$dest['valueformat'].']';
		}
		$sludge_destination_str = theme('table', array("rows"=>$lines, "header"=>array("Destination","Values")));
	}
	
	return array($ms_level, $sludge_destination_str, $sludge_destination);
}

/**
 * Retourne les données pour le cadre "Water re-use".
 */
function getRow7Graph2($ms_level, $options) {
	$ww_reuse = '';
	if (!empty($ms_level[$options['year']]['waste_water']['Re-used']) && $ms_level[$options['year']]['waste_water']['Re-used'] > 0) {
		$ww_reuse = $ms_level[$options['year']]['waste_water']['Re-used'].' m3 of treated waste water are re-used.';
		if ($ms_level[$options['year']]['waste_water']['rate of re-used']){
			$ww_reuse .=' It represent '.$ms_level[$options['year']]['waste_water']['rate of re-used'].' % of the total volume treated';
		}
		$rep='';
		if ($ms_level[$options['year']]['waste_water']['repartition']['agriculture']){
			$rep = ' in agriculture';
		}
		if ($ms_level[$options['year']]['waste_water']['repartition']['industry']){
			$rep .= ($rep!=''?', ':'').' in industry';
		}
		if ($ms_level[$options['year']]['waste_water']['repartition']['others']){
			$rep .= ($rep!=''?', ':'').' in others use.';
		}
		$ww_reuse .=($rep!=''?'This water is used ':'').$rep;
	} elseif($ms_level[$options['year']]['waste_water']['rate of re-used']){
		$ww_reuse .=' It represent '.$ms_level[$options['year']]['waste_water']['rate of re-used'].' % of treated waste water are re-used.';
		$rep='';
		if ($ms_level[$options['year']]['waste_water']['repartition']['agriculture']){
			$rep = ' in agriculture';
		}
		if ($ms_level[$options['year']]['waste_water']['repartition']['industry']){
			$rep .= ($rep!=''?', ':'').' in industry';
		}
		if ($ms_level[$options['year']]['waste_water']['repartition']['others']){
			$rep .= ($rep!=''?', ':'').' in others use.';
		}
		$ww_reuse .=($rep!=''?'This water is used ':'').$rep;
	} else{
		$ww_reuse ='No information';
	}
	
	return $ww_reuse;
}

/**
 * Ajoute au code html donné le code html pour afficher deux graphs camemberts.
 */
function getHtmlPlusHtmlChartsPies($html, $rowTitle,
		$graph1Title, $graph1Id, $graph1Front, $graph1Back, 
		$graph2Title, $graph2Id, $graph2Front, $graph2Back) {
	$html.='<h2 style="clear:both;">'. $rowTitle .'</h2>';
	$html.='<div class="rows charts-row">';
		// Création du code html pour le graph1 :
		$front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
		$front.= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph1Front).", '". $graph1Id ."');});</script>";
		$html.= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back).'</div></div>';
		
		// Création du code html pour le graph2 :
		$front = '<svg id="'. $graph2Id .'" style="margin:10px;"/>';
		$front.= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph2Front).", '". $graph2Id ."');});</script>";
		$html.= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph2Title, $front, $graph2Back).'</div></div>';
	
	$html.='</div>';
	return $html;
}

/**
 * Ajoute au code html donné le code html pour afficher deux graphs en barres verticales.
 */
function getHtmlPlusHtmlChartsBars($html, $rowTitle, 
		$graph1Legend, $graph1Id, $graph1Title, $graph1Front, $graph1Back, 
		$graph2Legend, $graph2Id, $graph2Title, $graph2Front, $graph2Back) {
	$html.='<h2 style="clear:both;">' .$rowTitle. '</h2>';
	$html.='<div class="rows charts-row">';
		// Création du code html pour le graph1 :
		if (array_sum (array_column($graph1Front, 1))){
			$front = uwwtd_graph_render_column($graph1Id, $graph1Legend, $graph1Front);
		} else {
			$front = t("No information");
		}
		$html.= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back).'</div></div>';
		
		// Création du code html pour le graph2 :
		if (array_sum (array_column($graph2Front, 1))) {
			$front = uwwtd_graph_render_column($graph2Id, $graph2Legend, $graph2Front);
		} else {
			$front = t("No information");
		}
		$html.= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph2Title, $front, $graph2Back).'</div></div>';
	$html.='</div>';
	return $html;
}

/**
 * Ajoute au code html donné le code html pour afficher un graph camembert et un texte.
 */
function getHtmlPlusHtmlChartPieAndText($html, $rowTitle,
		$graph1Title, $graph1Id, $ms_level, $graph1Front, $graph1Back,
		$graph2Title, $graph2Text, $options) {
	$html .='<h2 style="clear:both;">'. $rowTitle .'</h2>';
	$html .='<div class="rows charts-row">';
		// Création du code html pour le graph1 :
		$front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
		$front.= "<script>jQuery(document).ready(function(){display_piechart_custom(". json_encode($graph1Back) .", '". $graph1Id ."','middle');});</script>";
		$msg_sludge = '';
		if ($ms_level[$options['year']]['sludge']['production'] > 0) {
			$msg_sludge = "Production : ". $ms_level[$options['year']]['sludge']['production']." T DS/year";
		}
		$html .= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Front, $msg_sludge).'</div></div>';
	
		// Création du code html pour le graph2 :
		$html .= '<div class="cell charts-rows-cell"><div class="cell-content"><div class="flot-title">Water re-use</div><p>'. $graph2Text .'</p></div></div>';
	
	$html.='</div>';
	return $html;
}

function uwwtd_graph_render_flipcard($title, $front, $back, $msg=''){
    $content = 
        '<div class="container flip">'
          .'<div class="leftgraph front">'
            . '<div class="flot-title">'
                .'<div class="flip-title">'.$title.'</div>'
                .'<img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper to-table" title="See the data table" alt="See the data table">'
            . '</div>'
            . '<div class="flip-content"><b>'.$msg .'</b>'.$front.'</div>'
          .'</div>'

          .'<div class="leftgraph back">'
            . '<div class="flot-title flip-title-back">'
                .'<div class="flip-title">'.$title.'</div>'
                .'<img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper to-chart" title="See diagram" alt="See diagram">'
            . '</div>'
            . '<div class="flip-content"><b>'.$msg .'</b>'. $back .'</div>'
          .'</div>'
        .'</div>';
    return $content;
}

function uwwtd_graph_render_column($id, $legend, $rows){
  $chart = array(
    'id' => $id,
    'type' => 'ColumnChart',
    'legend' => $legend,
    'rows' => $rows,
    'width'=>510,
    'height'=>200,
  );
  return d3_draw($chart);
}

?>