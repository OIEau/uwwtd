<?php
define('ALL_YEARS', 'allyears');

require_once 'uwwtd.stats.inc';

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
	drupal_add_js('sites/all/libraries/d3/d3.js');
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
    	t("Generated Load by collection type (population equivalent – p.e.)"), 'graph_collect_type', $frontAndBackGraph1[0], $frontAndBackGraph1[1],
    	t("Generated Load by sewage network type (population equivalent – p.e.)"), 'graph_cs_type', $frontAndBackGraph2[0], $frontAndBackGraph2[1],
    	true, $yearsPossibles);
    
    //==============================================
    //=============== ROW 2 : Agglomeration : Generated load by compliance
    //==============================================
    
    // Récupération des données des graphs :
	$results = uwwtd_stat_national_compliance($yearSelected, $yearsPossibles);
	$frontAndBackGraph1And2 = getRow2Graph1and2($results, $yearSelected);
    
	// Création du code html pour ces graphs :
	$formResult = getHtmlPlusHtmlChartsPies($formResult, t("Agglomeration : Generated load by compliance"),
		t("Total generated load by compliance (population equivalent – p.e.)"), 'graph_generatedByCompliance', $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1],
		t("Number of agglomerations by compliance (number of agglomerations)"), 'graph_agglomerationByCompliance', $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3],
		false, $yearsPossibles);
    //==============================================
    //=============== ROW 3 : Waste water treatement plant : Load entering by compliance
    //==============================================
    
    // Récupération des données des graphs :
	$results = uwwtd_stat_national_uwwtp($yearSelected, $yearsPossibles, 'compliance');
    $frontAndBackGraph1 = getRow2Graph1and2($results, $yearSelected);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsPies($formResult, t("Waste water treatement plant : Load entering by compliance"),
    	t("Total load entering by compliance (population equivalent – p.e.)"), 'graph_loadenteringByCompliance', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
    	t("Number of treatment plant by compliance (number of agglomerations)"), 'graph_loadenteringByComplianceAgglomeration', $frontAndBackGraph1[2], $frontAndBackGraph1[3],
    	true, $yearsPossibles);
    
    
    //==============================================
    //=============== ROW 4 : Waste water treatement plant : Load entering by treatment type
    //==============================================
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_uwwtp($yearSelected, $yearsPossibles,'treatmenttype');
    $frontAndBackGraph1 = getRow2Graph1and2($results, $yearSelected);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsPies($formResult, t("Waste water treatement plant : Load entering by treatment type"),
    	t("Total load entering by treatment type (population equivalent – p.e.)"), 'graph_loadenteringByTreatmenttype', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
    	t("Number of treatment plant by treatment type (number of agglomerations)"), 'graph_loadenteringByTreatmenttypeAgglomeration', $frontAndBackGraph1[2], $frontAndBackGraph1[3],
    	false, $yearsPossibles);
    
	
    //==============================================
    //=============== ROW 5 : Agglomeration : generated load by agglomeration size
    //==============================================
    
    // Récupération des données des graphs :
	$results = uwwtd_stat_national_generated_by_agglomeration($yearSelected, $yearsPossibles);
    $frontAndBackGraph1And2 = getRow2Graph1and2($results, $yearSelected);

    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Agglomeration : generated load by agglomeration size"), array(
            array(
                'graph_generatedByAgglomeration', t("Total generated load by agglomeration size (population equivalent – p.e.)"), $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1], true,
                'graph_generatedByAgglomerationNumber', t("Number of agglomeration by size (number of agglomerations)"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3], true
            )
        ), $yearsPossibles
    );
    
    
    
    //==============================================
    //=============== ROW 6 : Waste water treatement plant : Total load entering and discharged
    //==============================================
    //Display only for annual stats and not for interannual
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_treatmentplant($yearSelected, $yearsPossibles);
    if($yearSelected!=ALL_YEARS){
        $frontAndBackGraph1And2 = getRow6Graph1And2($results);
        // Création du code html pour ces graphs :
        if($frontAndBackGraph1And2){
            $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Waste water treatement plant : Total load entering and discharged"), array(
                array(
                    'uwwtd_load_ent_and_dis', t("Total load entering and discharged"), $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1], false,
                    'per_ent_and_dis', t("BOD, COD, N, P concentration"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3], false
                )
              ), $yearsPossibles
            );
        }
    }
    else{
        //dsm($results);
        $frontAndBackGraphs = getRow6Graphs($results);
        $rows = array();
        if(isset($frontAndBackGraphs['BOD'])){
            $sum = 0;
            foreach($frontAndBackGraphs['BOD'][0]['data'] as $line){
                $sum = $sum+$line[1]+$line[2];
            }
            if($sum>0){
                $rows[] = array(
                    'uwwtd_load_ent_and_dis_bod', t("Total BOD load entering and discharged"), $frontAndBackGraphs['BOD'][0],  $frontAndBackGraphs['BOD'][1], false,
                    'per_ent_and_dis_bod', t("BOD concentration"), $frontAndBackGraphs['BOD'][2], $frontAndBackGraphs['BOD'][3], false
                );
            }
        }
        if(isset($frontAndBackGraphs['COD'])){
            $sum = 0;
            foreach($frontAndBackGraphs['COD'][0]['data'] as $line){
                $sum = $sum+$line[1]+$line[2];
            }
            if($sum>0){
                $rows[] = array(
                    'uwwtd_load_ent_and_dis_cod', t("Total COD load entering and discharged"), $frontAndBackGraphs['COD'][0],  $frontAndBackGraphs['COD'][1], false,
                    'per_ent_and_dis_cod', t("COD concentration"), $frontAndBackGraphs['COD'][2], $frontAndBackGraphs['COD'][3], false
                );
            }
        }
        if(isset($frontAndBackGraphs['N'])){
            $sum = 0;
            foreach($frontAndBackGraphs['N'][0]['data'] as $line){
                $sum = $sum+$line[1]+$line[2];
            }
            if($sum>0){
                $rows[] = array(
                    'uwwtd_load_ent_and_dis_n', t("Total Nitrogen load entering and discharged"), $frontAndBackGraphs['N'][0],  $frontAndBackGraphs['N'][1], false,
                    'per_ent_and_dis_n', t("Nitrogen concentration"), $frontAndBackGraphs['N'][2], $frontAndBackGraphs['N'][3], false
                );
            }
        }
        if(isset($frontAndBackGraphs['P'])){
            $sum = 0;
            foreach($frontAndBackGraphs['P'][0]['data'] as $line){
                $sum = $sum+$line[1]+$line[2];
            }
            if($sum>0){
                $rows[] = array(
                    'uwwtd_load_ent_and_dis_p', t("Total phosphorus load entering and discharged"), $frontAndBackGraphs['P'][0],  $frontAndBackGraphs['P'][1], false,
                    'per_ent_and_dis_p', t("Phosphorus concentration"), $frontAndBackGraphs['P'][2], $frontAndBackGraphs['P'][3], false
                );
            }
        }
        
        // Création du code html pour ces graphs :
        if(!empty($rows)){
            $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Waste water treatement plant : Total load entering and discharged"), 
            		$rows, $yearsPossibles);
        }
    }
    
	
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
	
    //==============================================
    //=============== ROW 8 : Distance to compliance art 3, 4, 5
    //==============================================
    
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_distance_to_compliance($yearSelected, $yearsPossibles);
    $frontAndBackGraphs = getRow8Graphs($results);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Legal compliance"), 
	    array(
	    	array(
	    		'graph_art3_pe', t("Legal compliance for connection in p.e."), $frontAndBackGraphs['graph1Front'],  $frontAndBackGraphs['graph1Back'], true,
	    		'graph_art3_agg', t("Legal compliance for connection by number of agglomerations"), $frontAndBackGraphs['graph2Front'],  $frontAndBackGraphs['graph2Back'], true
	    	),
    		array(
    			'graph_art4_pe', t("Legal compliance for 2nd treatment in p.e."), $frontAndBackGraphs['graph3Front'],  $frontAndBackGraphs['graph3Back'], true,
    			'graph_art4_agg', t("Legal compliance for 2nd treatment by number of agglomerations"), $frontAndBackGraphs['graph4Front'],  $frontAndBackGraphs['graph4Back'], true
    		),
    		array(
    			'graph_art5_pe', t("Legal compliance for 3rd treatment in p.e."), $frontAndBackGraphs['graph5Front'],  $frontAndBackGraphs['graph5Back'], true,
    			'graph_art5_agg', t("Legal compliance for 3rd treatment by number of agglomerations"), $frontAndBackGraphs['graph6Front'],  $frontAndBackGraphs['graph6Back'], true
    		),
	    ), $yearsPossibles
    );
    
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
		// Pour un graph avec plusieurs années :
		if ($yearSelected == ALL_YEARS) {
			// Pour chaque année :
			foreach ($frontTmp as $year => $groupElements) {
				// Suppresion du groupe du total :
				array_shift($frontTmp[$year]);
			}
			// Pour chaque année :
			foreach ($frontTmp as $year => $yearRow) {
				foreach ($yearRow as $groupElements) {
					if (array_key_exists('label', $groupElements) && 
						array_key_exists('value', $groupElements) && 
						array_key_exists('color', $groupElements)) {
						$front['data'][$year][0] = (string)$year;
						$front['data'][$year][$groupElements['label']] = $groupElements['value'];
						$front['color'][$groupElements['label']] = $groupElements['color'];
						$front['legend'][$groupElements['label']] = $groupElements['label'];
					}
				}
			}

			// Remplacement des keys par des 0, 1, 2 ... (nécessaire pour le traceurs de graph) :
			foreach ($front['data'] as $year => $groupElements) {
				$front['data'][$year] = array_values($groupElements);
			}
			$front['data'] = array_values($front['data']);
			$front['legend'] = array_values($front['legend']);
			$front['color'] = array_values($front['color']);
            
            foreach($front['data'] as $k=>$line){
                //$rows[$k] = $line;
                //Remove cell of year
                $year = array_shift($line);
                $line[] = array_sum($line);
                $rows[$k][0] = '<b>'.$year.'</b>';
                foreach($line as $i=>$c){
                    $rows[$k][$i+1] = uwwtd_format_number($c);
                }
            }
            $header = $front['legend'];
            array_unshift( $header, t("Year"));
            array_push( $header, t("Total"));
            $back =theme('table', array('rows' => $rows, 'header' => $header));
            
            
		} 
        else {
			$front = $frontTmp;
			
			// Pour chaque année :
			foreach ($front as $year => $groupElements) {
				// Suppresion de la ligne du total :
				array_shift($front[$year]);
			}
			// Pour chaque année (restantes) :
			foreach ($front as $year => $groupElements) {
				// Pour chaque groupe d'éléments  :
				foreach ($groupElements as $elementsKey => $elements) {
					if (array_key_exists('label', $elements) && array_key_exists('valueformat', $elements)) {
						// Réécriture de l'intitulé du label :
						$front[$year][$elementsKey]['label'] = $elements['label']. ' ['. $elements['valueformat'] .']';
					}
				}
			}
			
            $rows = array();
        
            // Transforme les clefs en 0, 1, 2, ...
            $dataByYears = array_values($frontTmp);
            
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
            foreach ($frontTmp as $year => $groupElements) {
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
		// Pour un graph avec plusieurs années :
		if ($yearSelected == ALL_YEARS) {
			// Pour chaque Network type :
            
			foreach ($frontTmp as $newtworkTypeName => $networkType) {
				if (array_key_exists('data', $networkType)) {
					// Pour chaque annéee :
					foreach ($networkType['data'] as $year => $values) {
						$front['data'][$year][0] = (string)$year;
						$front['data'][$year][$newtworkTypeName] = (Integer)$values['equValue'];
						$front['color'][$newtworkTypeName] = $values['color'];
						$front['legend'][$newtworkTypeName] = $newtworkTypeName;
					}
				}
			}
            
			// Remplacement des keys par des 0, 1, 2 ... (nécessaire pour le traceurs de graph) :
			foreach ($front['data'] as $year => $groupElements) {
				$front['data'][$year] = array_values($groupElements);
			}
			$front['data'] = array_values($front['data']);
			$front['legend'] = array_values($front['legend']);
			$front['color'] = array_values($front['color']);
		} else {
			// Pour chaque Network type :
			foreach ($frontTmp as $newtworkTypeName => $networkType) {
				if (array_key_exists('data', $networkType)) {
					// Pour chaque annéee :
					foreach ($networkType['data'] as $year => $values) {
						// Création d'une ligne pour le front :
						$front[$year][] = array('value' => $values['equValue'], 
							 'label' => $newtworkTypeName,
							 'valueformat' => $values['equFormated'],
							 'color' => $values['color']);
					}
				}
			}
		}
		
		$rows = $frontTmp;
		$columns = array();
		
		// Définition des entete de colonnes :
		$columns[0][]['data'] = t("Network type");
		
		// Pour chaque Network type :
		foreach ($rows as $newtworkTypeName => $networkType) {
			// Si ce NetworkType contient des data :
			if (array_key_exists('data', $networkType)) {
				// Ajout au début du tableau de la case avec l'intitulé du network type :
				$rows[$newtworkTypeName]['data'][0] = $newtworkTypeName;
				
				// Pour chaque année :
				foreach ($networkType['data'] as $year => $values) {
					// Définition des entete de colonnes :
					$columns[0][$year]['data'] = $year;
					
					// Réécriture de la ligne :
					$rows[$newtworkTypeName]['data'][$year] = $values['equFormated'] .'<br />'. $values['aggFormated'];
				}
				
				// Tri selon les clés :
				ksort($rows[$newtworkTypeName]['data']);
			}
		}
		
		// Récupération du DOM du tableau :
		$back = theme('table', array('rows' => $rows, 'header' => $columns, 'rows_multiple' => true));
		
		// Ajout des légendes :
		$back .= '
		<table class="stats-graphs table table-hover table-striped sticky-enabled">
 			<thead>
				<tr>
					<th><i>*pe: Population equivalent, *na: Number of agglomeration</i></th>
				</tr>
			</thead>
		</table>';
	} 
	
	return array($front, $back);
}

/**
 * Retourne les données pour les graphes "Total generated load by compliance" et "Number of agglomerations by compliance".
 * && Waste water treatement plant : Load entering by treatment type
 */
function getRow2Graph1and2($results, $yearSelected) {
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
	//dsm($results);
	if (!empty($results)) {
		// Création du tableau pour le front :
		if ($yearSelected == ALL_YEARS) {
			// Transformer le $results en trois tableau $color, $legend et $data utilisable par la fonction uwwtd_graph_render_column.
            foreach($results as $state=>$v){
                $data = $v['data'];
                reset( $data );
                $label = current( $data );
                $key   = key( $data );
                if($key){
                    unset( $data[ $key ]);
                }
                foreach($data as $year=>$item){
                    $graph1Front['data'][$year][0] = (string)$year;
                    $graph1Front['data'][$year][$state] = (integer)$item['pe'];
                    $graph1Front['color'][$state] = $item['color'];
                    $graph1Front['legend'][$state] = $label;
                    $graph2Front['data'][$year][0] = (string)$year;
                    $graph2Front['data'][$year][$state] = (integer)$item['nb'];
                    $graph2Front['color'][$state] = $item['color'];
                    $graph2Front['legend'][$state] = $label;
                    
                }
            }
            //tri par année
            ksort($graph1Front['data'], SORT_NUMERIC);
            ksort($graph2Front['data'], SORT_NUMERIC);
            
            //Bouche trou 
            foreach($graph1Front['data'] as $key=>$values){
                $item = array($values[0]);
                unset($values[0]);
                foreach($graph1Front['legend'] as $legend){
                    $item[] = isset($values[$legend])?$values[$legend]:0;  
                }
                $graph1Front['data'][$key] = $item;
            }
            foreach($graph2Front['data'] as $key=>$values){
                $item = array($values[0]);
                unset($values[0]);
                foreach($graph2Front['legend'] as $legend){
                    $item[] = isset($values[$legend])?$values[$legend]:0;  
                }
                $graph2Front['data'][$key] = $item;
            }
            
            $graph1Front['data'] = array_values($graph1Front['data']);
			$graph1Front['legend'] = array_values($graph1Front['legend']);
			$graph1Front['color'] = array_values($graph1Front['color']);
            $graph2Front['data'] = array_values($graph2Front['data']);
			$graph2Front['legend'] = array_values($graph2Front['legend']);
			$graph2Front['color'] = array_values($graph2Front['color']);
            
            $rows = array();
            foreach($graph1Front['data'] as $k=>$line){
                //Remove cell of year
                $year = array_shift($line);
                $line[] = array_sum($line);
                $rows[$k][0] = '<b>'.$year.'</b>';
                foreach($line as $i=>$c){
                    $rows[$k][$i+1] = uwwtd_format_number($c);
                }
            }
            $header = $graph1Front['legend'];
            array_unshift( $header, t("Year"));
            array_push( $header, t("Total"));
            $graph1Back =theme('table', array('rows' => $rows, 'header' => $header));
            
            $rows = array();
            foreach($graph2Front['data'] as $k=>$line){
                //Remove cell of year
                $year = array_shift($line);
                $line[] = array_sum($line);
                $rows[$k][0] = '<b>'.$year.'</b>';
                foreach($line as $i=>$c){
                    $rows[$k][$i+1] = uwwtd_format_number($c);
                }
            }
            $header = $graph2Front['legend'];
            array_unshift( $header, t("Year"));
            array_push( $header, t("Total"));
            $graph2Back =theme('table', array('rows' => $rows, 'header' => $header));
            

		} 
        else {
			foreach($results as $state=>$v){
                $data = $v['data'];
                reset( $data );
                $label = current( $data );
                $key   = key( $data );
                if($key){
                    unset( $data[ $key ]);
                }
                foreach($data as $year=>$item){
                    $graph1Front[$year][]= array(
                        'value'=>(integer)$item['pe'],
                        'valueformat'=>$item['pe_format'],
                        'label'=>$label,
                        'color'=>$item['color']
                    );
                    $graph2Front[$year][]=array(
                        'value'=>(integer)$item['nb'],
                        'valueformat'=>$item['nb_format'],
                        'label'=>$label,
                        'color'=>$item['color']
                    );
                    
                }
            }

            // Création du tableau pour le back :
            foreach ($results as $complianceStateName => $complianceState) {
                if (array_key_exists('data', $complianceState)) {
                    foreach ($complianceState['data'] as $keyElement => $element) {
                        // Définition des entete de colonnes :
                        $columns[0][$keyElement]['data'] = $keyElement;
                        
                        // Si c'est une valeur pour une année :
                        if (is_array($element) && 
                            array_key_exists('pe', $element) && 
                            array_key_exists('pe_format', $element) &&
                            array_key_exists('nb', $element) &&
                            array_key_exists('nb_format', $element)) {
                            // Redéfinition de la valeur de l'année :
                            $graph1Rows[$complianceStateName]['data'][$keyElement] = $element['pe_format'];
                            $graph2Rows[$complianceStateName]['data'][$keyElement] = $element['nb_format'];
                            
                            // Calcul de la somme :
                            if (!array_key_exists($keyElement, $graph1Sums)) {
                                $graph1Sums[$keyElement] = 0;
                            }
                            $graph1Sums[$keyElement] += $element['pe'];
                            if (!array_key_exists($keyElement, $graph2Sums)) {
                                $graph2Sums[$keyElement] = 0;
                            }
                            $graph2Sums[$keyElement] += $element['nb'];
                        }
                    }
                }
            }
            
            // Pour chaque ligne :
	        foreach ($graph1Rows as $complianceStateName => $complianceState) {
	            if (array_key_exists('data', $complianceState) && array_key_exists('pe', $columns)) {
	                // Pour toutes les colonnes existantes :
	                foreach (array_keys($columns['pe']) as $columsName) {
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
		
	        // Ajout des Totaux :
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
		}
	}
	
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
    $sum =0;
    
	if (!empty($results)) {
		foreach ($results as $rowName => $row) {
			if (array_key_exists('data', $row)) {
				foreach ($row['data'] as $keyElement => $element) {
					// Définition des entete de colonnes :
                    $columns[0][$keyElement] = array(
                        'data' => $keyElement,
                    );
                    
						
					// Si c'est une valeur pour une année :
					if (is_array($element)) {
						if (array_key_exists('in', $element) && 
							array_key_exists('q', $element['in']) && 
							array_key_exists('out', $element) && 
							array_key_exists('q', $element['out']) && 
							array_key_exists('vol', $element)) {
							// Redéfinition de la valeur de l'année :
							$graph1Rows[$rowName]['data'][$keyElement] = array(
                                'Entering' => $element['in']['q'] , 
                                'Discharged'=>$element['out']['q']
                            );
                            $sum+=$element['in']['q'];
							//Convert to mg/l
							$graph2Rows[$rowName]['data'][$keyElement] = array(
                                'Entering'=>round(($element['in']['c']) , 3),
                                'Discharged'=>round(($element['out']['c']) , 3),
                            );
						} else {
							$graph1Rows[$rowName]['data'][$keyElement] = array('Entering'=>0, 'Discharged'=>0);
							$graph2Rows[$rowName]['data'][$keyElement] = array('Entering'=>0, 'Discharged'=>0);
						}
					}
				}
			}
		}
	}
    if($sum==0) return false;
    
    $colors=array(
        'Entering'=>'#5B3349', 
        'Discharged'=>'#F28030'
    );
    //=====================Graph 1
    
	// Pour chaque ligne :
	foreach ($graph1Rows as $rowName => $row) {
		if (array_key_exists('data', $row) && array_key_exists(0, $columns)) {
			// Pour toutes les colonnes existantes :
			foreach (array_keys($columns[0]) as $columsName) {
				if (!array_key_exists($columsName, $row['data']) || empty($graph1Rows[$rowName]['data'][$columsName])) {
					// Remplissage des trous :
					$graph1Rows[$rowName]['data'][$columsName] = array('Entering'=>0, 'Discharged'=>0);
				}
			}
		}
	}
    $header1 = array();
	foreach ($graph1Rows as $rowName => $row) {
        $line = array();
        
        foreach($row['data'] as $key=>$item){
            
            if(is_array($item)){
                $header1[0][$key]=array('data'=>$key, 'colspan'=>count($item));
                
                foreach($item as $k=>$v){
                    $header1[1][$key.'_'.$k]=$k;
                    $line[$k] = uwwtd_format_number($v,2).' '.t('tons/year');
                    $graph1Front[$rowName][]=array(
                        'value'=>(float)$v,
                        'valueformat'=>uwwtd_format_number($v,2).' '.t('tons/year'),
                        'label'=>$k,
                        'color'=>$colors[$k]
                    );
                }
            }else{
                $header1[0][$key]='';
                $header1[1][$key]=$key;
                $line[$key]=$item;
            }
        }
        $graph1Rows[$rowName]['data'] = $line;
	}
    
    
    //=====================Graph 2
	// Pour chaque ligne :
	foreach ($graph2Rows as $rowName => $row) {
		if (array_key_exists('data', $row) && array_key_exists(0, $columns)) {
			// Pour toutes les colonnes existantes :
			foreach (array_keys($columns[0]) as $columsName) {
				if (!array_key_exists($columsName, $row['data']) || empty($graph2Rows[$rowName]['data'][$columsName])) {
					// Remplissage des trous :
					$graph2Rows[$rowName]['data'][$columsName] = array('Entering'=>0, 'Discharged'=>0);
				}
			}
		}
	}
    $header2 = array();
	foreach ($graph2Rows as $rowName => $row) {
        $line = array();
        
        foreach($row['data'] as $key=>$item){
            
            if(is_array($item)){
                $header2[0][$key]=array('data'=>$key, 'colspan'=>count($item));
                
                foreach($item as $k=>$v){
                    $header2[1][$key.'_'.$k]=$k;
                    $line[$k] = uwwtd_format_number($v,2).' '.t('mg/l');
                    $graph2Front[$rowName][]=array(
                        'value'=>(float)$v,
                        'valueformat'=>uwwtd_format_number($v,2).' '.t('mg/l'),
                        'label'=>$k,
                        'color'=>$colors[$k]
                    );
                }
            }else{
                $header2[0][$key]='';
                $header2[1][$key]=$key;
                $line[$key]=$item;
            }
        }
        $graph2Rows[$rowName]['data'] = $line;
	}
    
	$graph1Back = theme('table', array('rows' => $graph1Rows, 'header' => $header1, 'rows_multiple' => true, 'attributes' => array('class' => 'customTable')));
	$graph2Back = theme('table', array('rows' => $graph2Rows, 'header' => $header2, 'rows_multiple' => true, 'attributes' => array('class' => 'customTable')));
	
	return array($graph1Front, $graph1Back, $graph2Front, $graph2Back);
}


function getRow6Graphs($results){
    if(!empty($results)){
        $legends = array(t("Entering"),t("Discharged"));
        $colors = array(
            'N'=>array('#87ceeb', '#c8e8f5'),
            'P'=>array('#ff9900', '#ffcf95'),
            'COD'=>array('#d87600', '#ffcf95'),
            'BOD'=>array('#5B3349', '#dcc1d0'),
            
        );       
        foreach($results as $rowName=>$data){
            $graph1Front = array();
            $graph2Front = array();
            $graph1FrontRows = array();
            $graph2FrontRows = array();
            $graph1Back = "";
            $graph2Back = "";
            if(isset($data['data'])){
                foreach($data['data'] as $key=>$item){
                    if(is_array($item)){
                        $graph1Front['data'][] = array(
                            (string)$key,
                            $item['in']['q'],
                            $item['out']['q'],
                        );
                        $graph1FrontRows[] = array(
                            (string)$key,
                            uwwtd_format_number($item['in']['q']),
                            uwwtd_format_number($item['out']['q']),
                            uwwtd_format_number($item['out']['q']*100/$item['in']['q'] ,1) .'%'
                        );
                        
                        // BB - 26/10/16
                        $graph2Front['data'][] = array(
                            (string)$key,
                            // $item['vol']>0 ?round($item['in']['c']*1000/$item['vol'],3):0,
                            // $item['vol']>0 ?round($item['out']['c']*1000/$item['vol'],3):0,
                            round($item['in']['c'], 3),
                            round($item['out']['c'], 3),
                        );
                        $graph2FrontRows[] = array(
                            (string)$key,
                            // $item['vol']>0 ?round($item['in']['c']*1000/$item['vol'],3):'-',
                            // $item['vol']>0 ?round($item['out']['c']*1000/$item['vol'],3):'-',
                            // $item['vol']>0 ? uwwtd_format_number($item['out']['c']*100/$item['in']['c'] ,1) .'%' :'-',
                            $item['in']['c'] > 0 ? round($item['in']['c'], 3) : '-',
                            $item['out']['c'] > 0 ? round($item['out']['c'], 3) : '-',
                            $item['out']['c'] * 100 / $item['in']['c'] > 0 ? uwwtd_format_number($item['out']['c'] * 100 / $item['in']['c'], 1) . '%' : '-'

                        );
                        
                    }
                }
                
                $graph1Front['legend'] = $legends;
                $graph1Front['color'] = $colors[$rowName];
                $graph1Back = theme('table', array(
                    'rows'=>$graph1FrontRows, 
                    'header'=>array(t("Year"),t("Entering (Tons)"),t("Discharged (Tons)"),t("Rate"))
                ));
                
                $graph2Front['legend'] = $legends;
                $graph2Front['color'] = $colors[$rowName];
                $graph2Back = theme('table', array(
                    'rows'=>$graph2FrontRows, 
                    'header'=>array(t("Year"),t("Entering (mg/l)"),t("Discharged (mg/l)"),t("Rate"))
                ));
            }
            $rows[$rowName] = array($graph1Front, $graph1Back, $graph2Front, $graph2Back);
        }  
    }
    
    return $rows;
}

/**
 * Retourne les données pour le graph "Sludge destination".
 */
function getRow7Graph1($yearSelected, $results) {
	$front = array();
	$back = '';
	$columns = array();
	$rows = array();
	
	$columns[0]['Destination']['data'] = t('Destination');
	if (!empty($results)) {
        if($yearSelected == ALL_YEARS){
            $legend = array();
            $color = array();
            foreach ($results as $year => $row) {
                $columns[0][$year]['data'] = $year;
                $item = array();
                $item[0] = (string)$year;   
                if (array_key_exists('sludge', $row) && array_key_exists('destination', $row['sludge'])) {
                    foreach ($row['sludge']['destination'] as $keyElement => $element) {
                        if (!empty($element)) {
                            $rows[$keyElement]['data']['Destination'] = $GLOBALS['ms_level']['sludge'][$keyElement]['label'];
                            
                            // Ajout de la ligne :
                            $rows[$keyElement]['data'][$year] = $element;
                            $item[$keyElement] =$element;
                            $legend[$keyElement] = $GLOBALS['ms_level']['sludge'][$keyElement]['label'];
                            $color[$keyElement] = $GLOBALS['ms_level']['sludge'][$keyElement]['color'];
                            
                        }
                    }
                }
                $front['data'][]=$item;
            }
            //Fix the order in legend
            $order = array_keys($GLOBALS['ms_level']['sludge']);
            $legend_ord = array();
            $color_ord = array();
            foreach($order as $val){
                if(isset($legend[$val])){
                    $legend_ord[$val] = $legend[$val];
                    $color_ord[$val] = $color[$val];
                    
                }
            }
            $front['legend'] = array_values($legend_ord);
            $front['color'] = array_values($color_ord);
            //ON bouche les trous et on fait le tableau de données
            $header = array(t("Destination"));
            foreach($front['data'] as $key=>$values){
                $item = array();
                $year = $values[0];
                $header[$year] = $year;
                $item[]=$year;
                foreach($legend_ord as $l=>$lab){
                    $item[] = (isset($values[$l])?$values[$l]:0);
                    if(!isset($table[$l])) $table[$l][0]= $lab;
                    $table[$l][$year] =  (isset($values[$l])? uwwtd_format_number($values[$l], 1):0);
                }
                $front['data'][$key] = $item;
            }
            $back = theme('table', array('rows' => $table, 'header' =>$header)); // 'header' => $columns, 'rows_multiple' => true
        }
        else{
            foreach ($results as $year => $row) {
                // Définition des entete de colonnes :
                $columns[0][$year]['data'] = $year;
                
                if (array_key_exists('sludge', $row) && array_key_exists('destination', $row['sludge'])) {
                    foreach ($row['sludge']['destination'] as $keyElement => $element) {
                        if (!empty($element)) {
                            $rows[$keyElement]['data']['Destination'] = $GLOBALS['ms_level']['sludge'][$keyElement]['label'];
                            
                            // Ajout de la ligne :
                            //$rows[$keyElement]['data'][$year] = $element;
                            $rows[$keyElement]['data'][$year] = uwwtd_format_number($element).' '.t('T DS/year');
                            
                            $front[$year][] =array(
                                'value'=>$element,
                                'label'=>$GLOBALS['ms_level']['sludge'][$keyElement]['label'],
                                'valueformat'=>uwwtd_format_number($element).' '.t('T DS/year'),
                                'color'=>$GLOBALS['ms_level']['sludge'][$keyElement]['color'],
                            );
                            
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
                            <th><i>*: '.t('Tons of Dry Substance per year T DS/year').'</i></th>
                        </tr>
                    </thead>
                </table>';
            
        }
		
	}
	
	
	
	return array($front, $back);
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
		if($ms_level[$yearToProcess]['waste_water']['rate of re-used']){
			if ($ms_level[$yearToProcess]['waste_water']['rate of re-used']){
				$ww_reuse .=' '.t('@reuseRate % of treated waste water are re-used.', array('@reuseRate'=>$ms_level[$yearToProcess]['waste_water']['rate of re-used'])).' ';
			}
			$rep='';
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['agriculture']){
				$rep = ' '. t('in agriculture');
			}
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['industry']){
				$rep .= ($rep!=''?', ':'').t('in industry');
			}
			if ($ms_level[$yearToProcess]['waste_water']['repartition']['others']){
				$rep .= ($rep!=''?' '.t('and').' ':'').t('in others use.');
			}
			$ww_reuse .=($rep!=''?'This water is used ':'').$rep;
            if ($ms_level[$yearToProcess]['waste_water']['Re-used']){
                $ww_reuse .= '<div>'.t('Explanation').' : '.t($ms_level[$yearToProcess]['waste_water']['Re-used']).'</div>';
            }
		} else{
			$ww_reuse .=t('No information');
		}
		$ww_reuse .= '</div>';
	}
	return $ww_reuse;
}

/**
 * Retourne les données pour le graph1 et 2 de la ligne 8, à propos des Comparison of compliance.
 */
function getRow8Graphs($results) {
	$colors = array(
		'Compliant' => $GLOBALS['uwwtd']['compliance_color']['C'],
		'Not compliant' => $GLOBALS['uwwtd']['compliance_color']['NC'],
		'Not relevant' => $GLOBALS['uwwtd']['compliance_color']['NR'],
	);
	
	$graph1FrontAndBack = getFrontAndBackFormated($results['comparisonsCompliancesByPe'], 'Article 3', $colors);
	$graph2FrontAndBack = getFrontAndBackFormated($results['comparisonsCompliancesByAgg'], 'Article 3', $colors);
	$graph3FrontAndBack = getFrontAndBackFormated($results['comparisonsCompliancesByPe'], 'Article 4', $colors);
	$graph4FrontAndBack = getFrontAndBackFormated($results['comparisonsCompliancesByAgg'], 'Article 4', $colors);
	$graph5FrontAndBack = getFrontAndBackFormated($results['comparisonsCompliancesByPe'], 'Article 5', $colors);
	$graph6FrontAndBack = getFrontAndBackFormated($results['comparisonsCompliancesByAgg'], 'Article 5', $colors);
	
	return array(
		'graph1Front' => $graph1FrontAndBack['front'], 'graph1Back' => $graph1FrontAndBack['back'], 
		'graph2Front' => $graph2FrontAndBack['front'], 'graph2Back' => $graph2FrontAndBack['back'],
		'graph3Front' => $graph3FrontAndBack['front'], 'graph3Back' => $graph3FrontAndBack['back'],
		'graph4Front' => $graph4FrontAndBack['front'], 'graph4Back' => $graph4FrontAndBack['back'],
		'graph5Front' => $graph5FrontAndBack['front'], 'graph5Back' => $graph5FrontAndBack['back'],
		'graph6Front' => $graph6FrontAndBack['front'], 'graph6Back' => $graph6FrontAndBack['back']);
}

/**
 * Retourne le tableau avec les donnés pour l'unité et l'article donné, au format attendu pour afficher le graph,
 * ainsi que le html pour affiche le tableau en back.
 */
function getFrontAndBackFormated($arraySource, $articleName, $colors) {
	$front = array();
	$columns = array();
	$columns['year'] = 'Year';
	$backRows = array();
	$back = '';
	
	// Pour le graph en p.e. :
	if (!empty($arraySource)) {
		// Pour chaque année :
		foreach ($arraySource as $year => $yearRow) {
			// Pour chaque données d'année :
			foreach ($yearRow as $complianceName => $compliance) {
				$columns[$complianceName] = $complianceName;
				
				// Pour chaque article :
				foreach ($compliance as $name => $article) {
					if ($name === $articleName) {
						// Définition de la couleur :
						$color = '#005572';
						if (array_key_exists($complianceName, $colors)) {
							$color = $colors[$complianceName];
						}
						
						// Création ligne front :
						$front[$year][$complianceName]['value'] = $article;
						$front[$year][$complianceName]['valueformat'] = $article .' pe';
						$front[$year][$complianceName]['label'] = $complianceName;
						$front[$year][$complianceName]['color'] = $color;
						
						// Création ligne back :
						$backRows[$year]['year'] = '<b>'. $year .'</b>';
						$backRows[$year][$complianceName] = $article;
					}
				}
			}
		}
	}

	// Remplissage des trous :
	foreach ($backRows as $year => $yearRow) {
		foreach ($yearRow as $complianceName => $compliance) {
			if (empty($compliance)) {
				$backRows[$year][$complianceName] = ' - ';
			}
		}
	}

	// Récupération du DOM des tableau :
	$back = theme('table', array('rows' => $backRows, 'header' => $columns, 'rows_multiple' => false));
	
	return array('front' => $front, 'back' => $back);
}

/**
 * Ajoute au code html donné le code html pour afficher deux graphs camemberts.
 */
function getHtmlPlusHtmlChartsPies($html, $rowTitle,
		$graph1Title, $graph1Id, $graph1Front, $graph1Back, 
		$graph2Title, $graph2Id, $graph2Front, $graph2Back,
		$isBigTable, $yearsPossibles) {
	// Vérification qu'il y a des données :
	$isEmptyGraph1 = isEmptyGraph($graph1Front, $yearsPossibles);
	$isEmptyGraph2 = isEmptyGraph($graph2Front, $yearsPossibles);

	if (!$isEmptyGraph1 || !$isEmptyGraph2) {
		$html.='<h2 style="clear:both;">'. $rowTitle .'</h2>';
	}
	$classCustom = '';
	if ($isBigTable) {
		$classCustom = '-extended';
	}
	$html.='<div class="rows charts-row">';
		// Si le graph a des données :
		if (!$isEmptyGraph1) {
			// Création du code html pour le graph 1 :
			if (count($graph1Front) <= 1) {
				$front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
				$keys = array_keys($graph1Front);
				$graph1Front = $graph1Front[$keys[0]];
				$front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph1Front).", '". $graph1Id ."');});</script>";
			} elseif (array_key_exists('data', $graph1Front) && 
				array_key_exists('color', $graph1Front) && 
				array_key_exists('legend', $graph1Front)) {
				$front .= uwwtd_graph_render_column($graph1Id, $graph1Front['legend'], $graph1Front['color'], $graph1Front['data'], true);
			}
			$html.= '<div class="cell charts-rows-cell"><div class="cell-content'. $classCustom .'">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back).'</div></div>';
		}
		

		// Si le graph a des données :
		if (!$isEmptyGraph2) {
			// Création du code html pour le graph 2 :
			if (count($graph2Front) <= 1) {
				$front = '<svg id="'. $graph2Id .'" style="margin:10px;"/>';
				$keys = array_keys($graph2Front);
				$graph2Front = $graph2Front[$keys[0]];
				$front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph2Front).", '". $graph2Id ."');});</script>";
			} elseif (array_key_exists('data', $graph2Front) &&
				array_key_exists('color', $graph2Front) &&
				array_key_exists('legend', $graph2Front)) {
				$front .= uwwtd_graph_render_column($graph2Id, $graph2Front['legend'], $graph2Front['color'], $graph2Front['data'], true);
			}
			$html.= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph2Title, $front, $graph2Back).'</div></div>';
		}
	$html.='</div>';
	return $html;
}

/**
 * Ajoute au code html donné le code html pour afficher deux graphs en barres verticales.
 */
function getHtmlPlusHtmlChartsBars($html, $rowTitle, $rows, $yearsPossibles) { 
	$isFirst = true;
    foreach($rows as $i=>$row){
        list(
            $graph1Id, $graph1Title, $graph1Front, $graph1Back, $graph1Stack, 
            $graph2Id, $graph2Title, $graph2Front, $graph2Back, $graph2Stack
        ) = $row;
        // Vérification qu'il y a des données :
        $isEmptyGraph1 = isEmptyGraph($graph1Front, $yearsPossibles);
        $isEmptyGraph2 = isEmptyGraph($graph2Front, $yearsPossibles);
        
        if (!$isEmptyGraph1 || !$isEmptyGraph2) {
        	if ($isFirst) {
        		$html.='<h2 style="clear:both;">' .$rowTitle. '</h2>';
        		$isFirst = false;
        	}
        }
        
        $html.='<div class="rows charts-row">';
	        // Si le graph a des données :
	        if (!$isEmptyGraph1) {
	            // Création du code html pour le graph1 :
	            if (!empty($graph1Front)){
	                if(!isset($graph1Front['data'])){
	                    $graph1Front_rows =array();
	                    $graph1FrontLegend = array();
	                    $graph1FrontColor = array();
	                    foreach($graph1Front as $key=>$val){
	                        $item = array((string)$key);
	                        foreach($val as $k=>$v){
	                            $item[] =$v['value'];
	                            $graph1FrontLegend[$v['label']] =$v['label']; 
	                            $graph1FrontColor[$v['label']] =$v['color']; 
	                        }
	                        $graph1Front_rows[] = $item;
	                    }
	                    $front = uwwtd_graph_render_column($graph1Id, array_values($graph1FrontLegend), array_values($graph1FrontColor), $graph1Front_rows, $graph1Stack);
	                }
	                else{
	                    $front = uwwtd_graph_render_column($graph1Id, $graph1Front['legend'], $graph1Front['color'], $graph1Front['data'], $graph1Stack);
	                }
	            } 
	            else {
	                $front = t("No information");
	            }
	            $html.= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back).'</div></div>';
	        }
	        
	        // Si le graph a des données :
	        if (!$isEmptyGraph2) {
	            // Création du code html pour le graph2 :
	            if (!empty($graph2Front)){
	                if(!isset($graph1Front['data'])){
	                    $graph2Front_rows =array();
	                    $graph2FrontLegend = array();
	                    $graph2FrontColor = array();
	                    foreach($graph2Front as $key=>$val){
	                        $item = array((string)$key);
	                        foreach($val as $k=>$v){
	                            $item[] =$v['value'];
	                            $graph2FrontLegend[$v['label']] =$v['label']; 
	                            $graph2FrontColor[$v['label']] =$v['color']; 
	                        }
	                        $graph2Front_rows[] = $item;
	                    }
	                    $front = uwwtd_graph_render_column($graph2Id, array_values($graph2FrontLegend), array_values($graph2FrontColor), $graph2Front_rows, $graph2Stack);
	                }
	                else{
	                    $front = uwwtd_graph_render_column($graph2Id, $graph2Front['legend'], $graph2Front['color'], $graph2Front['data'], $graph2Stack);
	                }
	            } else {
	                $front = t("No information");
	            }
	            $html.= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph2Title, $front, $graph2Back).'</div></div>';
	        }
        $html.='</div>';
    }
	return $html;
}

/**
 * Ajoute au code html donné le code html pour afficher un graph camembert et un texte.
 */
function getHtmlPlusHtmlChartPieAndText($html, $rowTitle,
		$graph1Title, $graph1Id, $results, $graph1Front, $graph1Back,
		$graph2Title, $graph2Text, $yearSelected, $yearsPossibles) {
	// Vérification qu'il y a des données :
    $isEmptyGraph1 = isEmptyGraph($graph1Front, $yearsPossibles);
    $isEmptyText = isEmptyText($graph2Text, $yearsPossibles);
    
    if (!$isEmptyGraph1 || !$isEmptyText) {
    	$html .='<h2 style="clear:both;">'. $rowTitle .'</h2>';
    }
    $html .='<div class="rows charts-row">';
    
    // Si le graph a des données :
    if (!$isEmptyGraph1) {
		// Création du code html pour le graph1 :
		switch ($yearSelected) {
			case ALL_YEARS :
				if (!empty($yearsPossibles)) {
					$msg_sludge .= ' '.t("(T DS/year)");
                    $front .= uwwtd_graph_render_column($graph1Id, $graph1Front['legend'], $graph1Front['color'], $graph1Front['data'], true);
				}
				break;
			default :
				if ($results[$yearSelected]['sludge']['production'] > 0) {
                    if (count($graph1Front) <= 1) {
                        $front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
                        $keys = array_keys($graph1Front);
                        $graph1Front = $graph1Front[$keys[0]];
                        $front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph1Front).", '". $graph1Id ."');});</script>";
                    }
					$msg_sludge .= t('Total production : ').$results[$yearSelected]['sludge']['production']." ".t("T DS/year");
                    if($results[$yearSelected]['sludge']['remark']!=''){
                        $msg_sludge_remark = t($results[$yearSelected]['sludge']['remark']);
                    }
				}
		}
		$html .= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back, $msg_sludge, $msg_sludge_remark).'</div></div>';
    }
    
    
    // Si le graph a des données :
    if (!$isEmptyText) {
		// Création du code html pour le graph2 :
		$html .= '<div class="cell charts-rows-cell"><div class="cell-content"><div class="flot-title">Water re-use</div><p>'. $graph2Text .'</p></div></div>';
    }
    
	$html.='</div>';
	return $html;
}

function uwwtd_graph_render_flipcard($title, $front, $back, $msg='', $msgEnd=''){
    $content = 
        '<div class="container flip">'
          .'<div class="leftgraph front">'
            . '<div class="flot-title">'
                .'<div class="flip-title">'.$title.'</div>'
                .'<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper to-table" title="See the data table" alt="See the data table"></div>'
            . '</div>'
            . '<div class="flip-content"><div class="flip-content-header">'.$msg .'</div>'.$front.'<div class="flip-content-footer">'.$msgEnd .'</div></div>'
          .'</div>'

          .'<div class="leftgraph back">'
            . '<div class="flot-title flip-title-back">'
                .'<div class="flip-title">'.$title.'</div>'
                .'<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper to-chart" title="See diagram" alt="See diagram"></div>'
            . '</div>'
            . '<div class="flip-content"><div class="flip-content-header">'.$msg .'</div>'.$back.'<div class="flip-content-footer">'.$msgEnd .'</div></div>'
          .'</div>'
        .'</div>';
    return $content;
}

function uwwtd_graph_render_column($id, $legend, $color, $rows, $stacked=false){
  $chart = array(
	'id' => $id,
	'type' => ($stacked?'StackedColumnChart':'ColumnChart'),
	'legend' => $legend,
	'rows' => $rows,
	'width'=>510,
	'height'=>220,
	);

	if(!empty($color)){
		$chart['color'] = $color;
	}


	return d3_draw($chart);
}

/**
 * Vérifis si le graph contient des données.
 */
function isEmptyGraph($graphFront, $yearsPossibles) {
	if (!empty($graphFront)) {
		// Test si c'est un graph multi année :
		if (array_key_exists('data', $graphFront) && array_key_exists('color', $graphFront) && array_key_exists('legend', $graphFront)) {
			if (!empty($graphFront['data'])) {
				foreach ($graphFront['data'] as $dataRow) {
					foreach ($dataRow as $element) {
						// Si ce n'est pas une année et que la valeur est différente de zéro :
						if (!in_array($element, $yearsPossibles) && $element != 0) {
							return false;
						}
					}
				}
			}
		} else {
			// Pour chaque année :
			foreach ($graphFront as $yearData) {
				foreach ($yearData as $groupElements) {
					foreach ($groupElements as $elementName => $element) {
						if ($elementName === 'value') {
							// Si ce n'est pas une année et que la valeur est différente de zéro :
							if (!in_array($element, $yearsPossibles) && $element != 0) {
								return false;
							}
						}
					}
				}
			}
		}
	} 
	return true;
}

/**
 * Vérifis si le text n'est pas "No information".
 */
function isEmptyText($text) {
	return empty($text) || strstr($text, 'No information</div>');
}


//====================================================================== PAGES HISTORY ==========================
//nd@oieau.fr 10/10/2018
//This new page display historical information on agglomeration 
function uwwtd_agg_history_page($inspireIdLocalId){
    $data = uwwtd_getAggHistoryData($inspireIdLocalId);
    dsm($data);
    drupal_set_title($data['last_node']->title.' - history');
            drupal_add_js('sites/all/libraries/d3/d3.js');
            drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
            drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    libraries_load('d3.drupal');
    
    $html ='';
    $html.= '<div class="uwwcontainer">';
        $fieldstat = field_view_field('node', $data['last_node'], 'field_status');
        if (!empty($fieldstat[0]['#markup']) && $fieldstat[0]['#markup'] == '2') {
            $fieldstat[0]['#markup'] = 'Inactive';
        }
        $fieldnuts = field_view_field('node', $data['last_node'], 'field_regionnuts');
        $html.= '<div class="google-map-banner"><h1><span class="white-title">'. t('Agglomeration').' : </span><span class="node-title">'. $data['last_node']->title.'</span><span class="white-title"> - '.t("Identifier").' : </span><span class="inspireidlocalid">'. $data['last_node']->field_inspireidlocalid['und'][0]['value'].'</span><span class="white-title"> - '. t("Status").' : </span><span class="status status-'.drupal_clean_css_identifier(strtolower($fieldstat[0]['#markup'])).'">'.$fieldstat[0]['#markup'].'</span><br /><small>'. t("Region (NUTS) Code").' : '. $data['last_node']->field_regionnuts['und'][0]['value'].' - '. t("Region (NUTS) Name").' : '.$fieldnuts[0]['#markup'].'</small></h1></div>';
        $html.= render(field_view_field('node',  $data['last_node'], 'field_position_geo', 'openlayers_map'));
        $html.= '<div class="uwwhalf" style="clear:left;">'.uwwtd_timeline_output($data['last_node']).'</div>';
    $html.= '</div>';
    if(!empty($data['years'])){

        //==================Graph sur le generated load
        $genload_graph=array(
            'data'=>array(),
            'color'=>array(
                $GLOBALS['ms_level']['connection']['cs']['color'],
                $GLOBALS['ms_level']['connection']['ias']['color'],
                $GLOBALS['ms_level']['connection']['wot']['color'],
            ),
            'legend'=>array(
                $GLOBALS['ms_level']['connection']['cs']['label'],
                $GLOBALS['ms_level']['connection']['ias']['label'],
                $GLOBALS['ms_level']['connection']['wot']['label'],
            ),
        );
        $genload_table_rows = array();
        foreach($data['years'] as $y=>$info){
            //==========Generated load
            $genload_graph['data'][] = array(
                (string)$y,
                (float)$info['cs'],
                (float)$info['ias'],
                (float)$info['wot'],
            );
            $genload_table_rows[] = array(
                '<b>'.$y.'</b>',
                uwwtd_format_number((float)$info['cs']),
                uwwtd_format_number((float)$info['ias']),
                uwwtd_format_number((float)$info['wot']),
                uwwtd_format_number((float)$info['Generated load']),
            );
            
            //============= Distance to compliance
            
        }
        $genload_table = theme(
            'table', array(
                'rows' => $genload_table_rows, 
                'header' => array(t('Year'), t($GLOBALS['ms_level']['connection']['cs']['label']),t($GLOBALS['ms_level']['connection']['ias']['label']), t($GLOBALS['ms_level']['connection']['wot']['label']), '<b>'.t('Total').'</b>'))
        );
        //dsm( $genload_graph);
        $html = getHtmlPlusHtmlChartsPies($html, $rowTitle,
		'Generated Load by collection type (population equivalent – p.e.)', 'genloadbyct', $genload_graph, $genload_table, 
		'', '', array(), '',
		false, []);
    }

    return '<article>'.$html.'</article>';
}

function uwwtd_getAggHistoryData($inspireIdLocalId){
    $inspireIdLocalId = strtolower($inspireIdLocalId);
    $query = db_select('node', 'n');
    $query->innerJoin('field_data_field_anneedata', 'a', 'n.nid = a.entity_id AND a.entity_type = \'node\'');
    $query->leftJoin('field_data_field_inspireidlocalid', 'ins', 'n.nid = ins.entity_id AND ins.entity_type = \'node\'');
    $query->fields('a', array('field_anneedata_value'));
    $query->fields('n', array('nid'));
    $query->condition('n.type', 'agglomeration', '=');
    $query->condition('n.status', 1, '=');
    $query->where("lower(ins.field_inspireidlocalid_value) = :id", array(':id'=>strtolower($inspireIdLocalId)));
    $query->orderBy('a.field_anneedata_value', 'ASC');
    $ids = $query->execute()->fetchAllAssoc("field_anneedata_value");
    if(!empty($ids)){
        $data = array();
        foreach($ids as $y=>$id){
            $node = node_load($id->nid);
            $wrapper =  entity_metadata_wrapper('node', $node);
            $data[$y] = array(
                'Year'=>$y,
                'agg'=>$node,
                'Generated load'=>$wrapper->field_agggenerated->value(),
                'cs'=>$wrapper->field_agg_connected_load_pe->value(),
                'ias'=>$wrapper->field_agg_ias_load_pe->value(),
                'wot'=>$wrapper->field_agg_discharge_pe->value(),
                'uwwtps'=>array(),
                'dtc'=>uwwtd_get_distance_compliance($node),
            );
            $uwwtps = $wrapper->field_agguwwliste->value();
            if(!empty($uwwtps)){
                foreach($uwwtps as $uwwtp){
                     $data[$y]['uwwtps'][$uwwtp->nid] = l($uwwtp->title, 'node/'.$uwwtp->nid);
                }
            }
        }
    }
    return array('last_node'=>$node, 'years'=>$data);
}
