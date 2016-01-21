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

	// Récupération des données des graphs :
    $frontAndBackGraph1 = getRow1Graph1($yearSelected, $yearsPossibles);
    $frontAndBackGraph2 = getRow1Graph2($yearSelected, $yearsPossibles);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsPies($formResult, t('Agglomeration : Generated load by collection'), 
    	t("Generated Load by collection type"), 'graph_collect_type', $frontAndBackGraph1[0], $frontAndBackGraph1[1],
    	t("Generated Load by sewage network type"), 'graph_cs_type', $frontAndBackGraph2[0], $frontAndBackGraph2[1],
    	true);
    
    //==============================================
    //=============== ROW 2 : Agglomeration : Generated load by compliance
    //==============================================
    
    // Récupération des données des graphs :
	$results = uwwtd_stat_national_compliance($yearSelected, $yearsPossibles);
	$frontAndBackGraph1And2 = getRow2Graph1and2($results);
	
	// Création du code html pour ces graphs :
	$formResult = getHtmlPlusHtmlChartsPies($formResult, t("Agglomeration : Generated load by compliance"),
		t("Total generated load by compliance"), 'graph_generatedByCompliance', $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1],
		t("Number of agglomerations by compliance"), 'graph_agglomerationByCompliance', $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3],
		false);
    
    
    //==============================================
    //=============== ROW 3 : Waste water treatement plant : Load entering by compliance
    //==============================================
    
    // Récupération des données des graphs :
	$results = uwwtd_stat_national_uwwtp($yearSelected, $yearsPossibles, 'compliance');
    $frontAndBackGraph1 = getRow2Graph1and2($results);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsPies($formResult, t("Waste water treatement plant : Load entering by compliance"),
    	t("Total load entering by compliance"), 'graph_loadenteringByCompliance', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
    	t("Number of treatment plant by compliance"), 'graph_loadenteringByComplianceAgglomeration', $frontAndBackGraph1[2], $frontAndBackGraph1[3],
    	true);
    

    //==============================================
    //=============== ROW 4 : Waste water treatement plant : Load entering by treatment type
    //==============================================

    // Récupération des données des graphs :
    $results = uwwtd_stat_national_uwwtp($yearSelected, $yearsPossibles,'treatmenttype');
    $frontAndBackGraph1 = getRow2Graph1and2($results);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsPies($formResult, t("Waste water treatement plant : Load entering by treatment type"),
    	t("Total load entering by treatment type"), 'graph_loadenteringByTreatmenttype', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
    	t("Number of treatment plant by treatment type"), 'graph_loadenteringByTreatmenttypeAgglomeration', $frontAndBackGraph1[2], $frontAndBackGraph1[3]);
    
	
    //==============================================
    //=============== ROW 5 : Agglomeration : generated load by agglomeration size
    //==============================================
    
    // Récupération des données des graphs :
	$results = uwwtd_stat_national_generated_by_agglomeration($yearSelected, $yearsPossibles);
    $frontAndBackGraph1And2 = getRow2Graph1and2($results);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Agglomeration : generated load by agglomeration size"),
    	array(t("Generated load")), 'graph_generatedByAgglomeration', t("Total generated load by agglomeration size"), $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1],
    	array(t("Number of agglomeration")), 'graph_generatedByAgglomerationNumber', t("Number of agglomeration by size"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3]);
    
    
    //==============================================
    //=============== ROW 6 : Waste water treatement plant : Total load entering and discharged
    //==============================================
    
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_treatmentplant($yearSelected, $yearsPossibles);
    $frontAndBackGraph1And2 = getRow6Graph1And2($results);
    
    // Création du code html pour ces graphs :
    $legend =  array(t('incoming'), t('discharged'));
    $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Waste water treatement plant : Total load entering and discharged"),
    	$legend, 'uwwtd_load_ent_and_dis', t("Total generated load by agglomeration size"), $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1],
    	$legend, 'per_ent_and_dis', t("Number of agglomeration by size"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3]);
    
	
	//==============================================
    //=============== ROW 7 : MS Level statistics
    //==============================================
    
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_ms_level_data($yearSelected, $yearsPossibles);
    $frontAndBackGraph1 = getRow7Graph1($yearSelected, $results);
    $frontGraph2 = getRow7Graph2($results, $yearSelected, $yearsPossibles);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartPieAndText($formResult, t("MS Level statistics"),
    	t("Sludge destination"), 'graph_sludge_dest', $results,  $frontAndBackGraph1[0], $frontAndBackGraph1[1],
    	t("Water re-use"), $frontGraph2, $yearSelected, $yearsPossibles);
	
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
		
		// Définition des entete de colonnes :
		$columns[0][]['data'] = t("Network type");
		
		// Pour chaque Network type :
		foreach ($front as $newtworkTypeName => $networkType) {
			// Si ce NetworkType contient des data :
			if (array_key_exists('data', $networkType)) {
				// Ajout au début du tableau de la case avec l'intitulé du network type :
				array_unshift($rows[$newtworkTypeName]['data'], $newtworkTypeName);
				
				// Pour chaque année :
				foreach (array_keys($networkType['data']) as $year) {
					// Définition des entete de colonnes :
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
 * Retourne les données pour les graphes "Total generated load by compliance" et "Number of agglomerations by compliance".
 */
function getRow2Graph1and2($results) {
	// Variables du graph 1 :
	$graph1Front = array();
	$graph1Back = '';
	$columns = array();
	$graph1Rows = $results;
	$graph1Sums = array();
	
	// Variables du graph 2 :
	$graph2Front = array();
	$graph2Back = '';
	$graph2Rows = $results;
	$graph2Sums = array();
	
	if (!empty($results)) {
		foreach ($results as $complianceStateName => $complianceState) {
			if (array_key_exists('data', $complianceState)) {
				foreach ($complianceState['data'] as $keyElement => $element) {
					// Définition des entete de colonnes :
					$columns[0][$keyElement]['data'] = $keyElement;
					
					// Si c'est une valeur pour une année :
					if (is_array($element) && 
						array_key_exists(0, $element) && 
						array_key_exists(1, $element) &&
						array_key_exists(2, $element) &&
						array_key_exists(3, $element)) {
						// Redéfinition de la valeur de l'année :
						$graph1Rows[$complianceStateName]['data'][$keyElement] = $element[1];
						$graph2Rows[$complianceStateName]['data'][$keyElement] = $element[3];
						
						// Calcul de la somme :
						if (!array_key_exists($keyElement, $graph1Sums)) {
							$graph1Sums[$keyElement] = 0;
						}
						$graph1Sums[$keyElement] += $element[0];
						if (!array_key_exists($keyElement, $graph2Sums)) {
							$graph2Sums[$keyElement] = 0;
						}
						$graph2Sums[$keyElement] += $element[2];
					}
				}
			}
		}
	}
	
	// Pour chaque ligne :
	foreach ($graph1Rows as $complianceStateName => $complianceState) {
		if (array_key_exists('data', $complianceState) && array_key_exists(0, $columns)) {
			// Pour toutes les colonnes existantes :
			foreach (array_keys($columns[0]) as $columsName) {
				if (!array_key_exists($columsName, $complianceState['data'])) {
					// Remplissage des trous :
					$graph1Rows[$complianceStateName]['data'][$columsName] = ' - ';
				}
			}
		}
	}
	foreach ($graph2Rows as $complianceStateName => $complianceState) {
		if (array_key_exists('data', $complianceState) && array_key_exists(0, $columns)) {
			// Pour toutes les colonnes existantes :
			foreach (array_keys($columns[0]) as $columsName) {
				if (!array_key_exists($columsName, $complianceState['data'])) {
					// Remplissage des trous :
					$graph2Rows[$complianceStateName]['data'][$columsName] = ' - ';
				}
			}
		}
	}
	
	// Ajout des Total :
	$graph1Rows['Total']['data']['Compliant state'] = 'Total';
	foreach ($graph1Sums as $year => $sum) {
		$graph1Rows['Total']['data'][$year] = uwwtd_format_number($sum, 0) .' pe';
	}
	$graph2Rows['Total']['data']['Compliant state'] = 'Total';
	foreach ($graph2Sums as $year => $sum) {
		$graph2Rows['Total']['data'][$year] = uwwtd_format_number($sum, 0);
	}
	
	// Récupération du DOM des tableau :
	$graph1Back = theme('table', array('rows' => $graph1Rows, 'header' => $columns, 'rows_multiple' => true));
	$graph2Back = theme('table', array('rows' => $graph2Rows, 'header' => $columns, 'rows_multiple' => true));
	
	// Ajout des légendes :
	$graph1Back .= '
		<table class="stats-graphs table table-hover table-striped sticky-enabled">
 			<thead>
				<tr>
					<th><i>*pe: Population equivalent</i></th>
				</tr>
			</thead>
		</table>';
	$graph2Back .= '
		<table class="stats-graphs table table-hover table-striped sticky-enabled">
 			<thead>
				<tr>
					<th><i>*: Number of agglomerations</i></th>
				</tr>
			</thead>
		</table>';
	
	return array($graph1Front, $graph1Back, $graph2Front, $graph2Back);
}

/**
 * Retourne les données pour les graph 1 et 2 de la ligne 6.
 */
function getRow6Graph1And2($results) {
	// Variables du graph 1 :
	$graph1Front = array();
	$graph1Back = '';
	$columns = array();
	$graph1Rows = $results;
	
	// Variables du graph 2 :
	$graph2Front = array();
	$graph2Back = '';
	$graph2Rows = $results;
	if (!empty($results)) {
		foreach ($results as $rowName => $row) {
			if (array_key_exists('data', $row)) {
				foreach ($row['data'] as $keyElement => $element) {
					// Définition des entete de colonnes :
					$columns[0][$keyElement]['data'] = $keyElement;
						
					// Si c'est une valeur pour une année :
					if (is_array($element)) {
						if (array_key_exists('in', $element) && 
							array_key_exists(0, $element['in']) && 
							array_key_exists(1, $element['in']) &&
							array_key_exists('out', $element) && 
							array_key_exists(0, $element['out']) && 
							array_key_exists(1, $element['out']) &&
							array_key_exists('sum', $element)) {
							// Redéfinition de la valeur de l'année :
							$graph1Rows[$rowName]['data'][$keyElement] = $element['in'][1] .' el, '. $element['out'][1] .' dl';
							
							// Vincent Godé 20/01/2016, Attention : à revoir, résultats étranges :
							$graph2Rows[$rowName]['data'][$keyElement] = 
								round(($element['in'][0] / $element['sum']) , 1). ', '.
								round(($element['out'][0] / $element['sum']) , 1). ', '.
								round(((1 - ($element['out'][0] / $element['sum']))*100) / 1000, 1) . ' %';
						} else {
							$graph1Rows[$rowName]['data'][$keyElement] = ' - ';
							$graph2Rows[$rowName]['data'][$keyElement] = ' - ';
						}
					}
				}
			}
		}
	}
	
	// Pour chaque ligne :
	foreach ($graph1Rows as $rowName => $row) {
		if (array_key_exists('data', $row) && array_key_exists(0, $columns)) {
			// Pour toutes les colonnes existantes :
			foreach (array_keys($columns[0]) as $columsName) {
				if (!array_key_exists($columsName, $row['data']) || empty($graph1Rows[$rowName]['data'][$columsName])) {
					// Remplissage des trous :
					$graph1Rows[$rowName]['data'][$columsName] = ' - ';
				}
			}
		}
	}
	
	foreach ($graph2Rows as $rowName => $row) {
		if (array_key_exists('data', $row) && array_key_exists(0, $columns)) {
			// Pour toutes les colonnes existantes :
			foreach (array_keys($columns[0]) as $columsName) {
				if (!array_key_exists($columsName, $row['data']) || empty($graph2Rows[$rowName]['data'][$columsName])) {
					// Remplissage des trous :
					$graph2Rows[$rowName]['data'][$columsName] = ' - ';
				}
			}
		}
	}

	// Récupération du DOM des tableau :
	$graph1Back = theme('table', array('rows' => $graph1Rows, 'header' => $columns, 'rows_multiple' => true));
	$graph2Back = theme('table', array('rows' => $graph2Rows, 'header' => $columns, 'rows_multiple' => true));
	
	// Ajout des légendes :
	$graph1Back .= '
		<table class="stats-graphs table table-hover table-striped sticky-enabled">
 			<thead>
				<tr>
					<th><i>*el: Entering load (t/year), *dl: Discharged load (t/year)</i></th>
				</tr>
			</thead>
		</table>';
	$graph2Back .= '
		<table class="stats-graphs table table-hover table-striped sticky-enabled">
 			<thead>
				<tr>
					<th><i>*e: Entering (mg/l), *d: Discharged (mg/l), *%: Rate (%)</i></th>
				</tr>
			</thead>
		</table>';
	
	return array($graph1Front, $graph1Back, $graph2Front, $graph2Back);
}

/**
 * Retourne les données pour le graph "Sludge destination".
 */
function getRow7Graph1($yearSelected, $results) {
	$front = array();
	$back = '';
	$columns = array();
	$rows = array();
	
	$columns[0]['Destination']['data'] = 'Destination';
	if (!empty($results)) {
		foreach ($results as $year => $row) {
			// Définition des entete de colonnes :
			$columns[0][$year]['data'] = $year;
			
			if (array_key_exists('sludge', $row) && array_key_exists('destination', $row['sludge'])) {
				foreach ($row['sludge']['destination'] as $keyElement => $element) {
					if (!empty($element)) {
						$rows[$keyElement]['data']['Destination'] = $GLOBALS['ms_level']['sludge'][$keyElement]['label'];
						
						// Ajout de la ligne :
						$rows[$keyElement]['data'][$year] = $element;
					}
				}
			}
		}
	}
	
	// Pour chaque ligne :
	foreach ($rows as $rowName => $row) {
		if (array_key_exists('data', $row) && array_key_exists(0, $columns)) {
			// Pour toutes les colonnes existantes :
			foreach (array_keys($columns[0]) as $columsName) {
				if (!array_key_exists($columsName, $row['data'])) {
					// Remplissage des trous :
					$rows[$rowName]['data'][$columsName] = ' - ';
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
					<th><i>*: DS/year</i></th>
				</tr>
			</thead>
		</table>';
	
	return array($back, $front);
}

/**
 * Retourne les données pour le cadre "Water re-use".
 */
function getRow7Graph2($ms_level, $yearSelected, $yearsPossibles) {
	$ww_reuse = '';
	$yearsToProcess = array();
	if ($yearSelected === ALL_YEARS && !empty($yearsPossibles)) {
		$yearsToProcess = $yearsPossibles;
		if (array_key_exists(ALL_YEARS, $yearsToProcess)) {
			unset($yearsToProcess[ALL_YEARS]);
		}
	} else {
		$yearsToProcess[] = $yearSelected;
	}
	
	// Pour chaque année :
	foreach ($yearsToProcess as $yearToProcess) {
		$ww_reuse .= '<div><b>'. $yearToProcess .' : </b>';
		if (!empty($ms_level[$yearToProcess]['waste_water']['Re-used']) && $ms_level[$yearToProcess]['waste_water']['Re-used'] > 0) {
			$ww_reuse .= $ms_level[$yearToProcess]['waste_water']['Re-used'].' m3 of treated waste water are re-used.';
			if ($ms_level[$yearToProcess]['waste_water']['rate of re-used']){
				$ww_reuse .=' It represent '.$ms_level[$yearToProcess]['waste_water']['rate of re-used'].' % of the total volume treated';
			}
			$rep='';
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['agriculture']){
				$rep = ' in agriculture';
			}
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['industry']){
				$rep .= ($rep!=''?', ':'').' in industry';
			}
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['others']){
				$rep .= ($rep!=''?', ':'').' in others use.';
			}
			$ww_reuse .=($rep!=''?'This water is used ':'').$rep;
		} elseif($ms_level[$yearToProcess]['waste_water']['rate of re-used']){
			$ww_reuse .=' It represent '.$ms_level[$yearToProcess]['waste_water']['rate of re-used'].' % of treated waste water are re-used.';
			$rep='';
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['agriculture']){
				$rep = ' in agriculture';
			}
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['industry']){
				$rep .= ($rep!=''?', ':'').' in industry';
			}
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['others']){
				$rep .= ($rep!=''?', ':'').' in others use.';
			}
			$ww_reuse .=($rep!=''?'This water is used ':'').$rep;
		} else{
			$ww_reuse .='No information';
		}
		$ww_reuse .= '</div>';
	}
	return $ww_reuse;
}

/**
 * Ajoute au code html donné le code html pour afficher deux graphs camemberts.
 */
function getHtmlPlusHtmlChartsPies($html, $rowTitle,
		$graph1Title, $graph1Id, $graph1Front, $graph1Back, 
		$graph2Title, $graph2Id, $graph2Front, $graph2Back,
		$isBigTable) {
	$html.='<h2 style="clear:both;">'. $rowTitle .'</h2>';
	$classCustom = '';
	if ($isBigTable) {
		$classCustom = '-extended';
	}
	$html.='<div class="rows charts-row">';
		// Création du code html pour le graph1 :
		$front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
		$front.= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph1Front).", '". $graph1Id ."');});</script>";
		$html.= '<div class="cell charts-rows-cell"><div class="cell-content'. $classCustom .'">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back).'</div></div>';
		
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
		$graph1Title, $graph1Id, $results, $graph1Front, $graph1Back,
		$graph2Title, $graph2Text, $yearSelected, $yearsPossibles) {
	$html .='<h2 style="clear:both;">'. $rowTitle .'</h2>';
	$html .='<div class="rows charts-row">';
		// Création du code html pour le graph1 :
		$front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
		$front.= "<script>jQuery(document).ready(function(){display_piechart_custom(". json_encode($graph1Back) .", '". $graph1Id ."','middle');});</script>";
		$msg_sludge = 'Production : ';
		
		// Gestion des productions par années :
		switch ($yearSelected){
			case ALL_YEARS :
				if (!empty($yearsPossibles)) {
					foreach ($yearsPossibles as $yearPossible) {
						if ($results[$yearPossible]['sludge']['production'] > 0) {
							$msg_sludge .= '<i>'. $yearPossible .': </i>'. $results[$yearPossible]['sludge']['production'] .', ';
						}
					}
					$msg_sludge .= " (T DS/year)";
				}
				break;
			default :
				if ($results[$yearSelected]['sludge']['production'] > 0) {
					$msg_sludge .= $results[$yearSelected]['sludge']['production']." T DS/year";
				}
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