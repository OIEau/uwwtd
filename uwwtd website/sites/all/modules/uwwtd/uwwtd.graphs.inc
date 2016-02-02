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
	$frontAndBackGraph1And2 = getRow2Graph1and2($results, $yearSelected);
    
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
    $frontAndBackGraph1 = getRow2Graph1and2($results, $yearSelected);
    
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
    $frontAndBackGraph1 = getRow2Graph1and2($results, $yearSelected);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsPies($formResult, t("Waste water treatement plant : Load entering by treatment type"),
    	t("Total load entering by treatment type"), 'graph_loadenteringByTreatmenttype', $frontAndBackGraph1[0],  $frontAndBackGraph1[1],
    	t("Number of treatment plant by treatment type"), 'graph_loadenteringByTreatmenttypeAgglomeration', $frontAndBackGraph1[2], $frontAndBackGraph1[3]);
    
	
    //==============================================
    //=============== ROW 5 : Agglomeration : generated load by agglomeration size
    //==============================================
    
    // Récupération des données des graphs :
	$results = uwwtd_stat_national_generated_by_agglomeration($yearSelected, $yearsPossibles);
    $frontAndBackGraph1And2 = getRow2Graph1and2($results, $yearSelected);
    dsm($frontAndBackGraph1And2[0]);
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Agglomeration : generated load by agglomeration size"), array(
            array(
                'graph_generatedByAgglomeration', t("Total generated load by agglomeration size"), $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1], true,
                'graph_generatedByAgglomerationNumber', t("Number of agglomeration by size"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3], true
            )
        )
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
              )
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
                    'per_ent_and_dis_p', t("phosphorus concentration"), $frontAndBackGraphs['P'][2], $frontAndBackGraphs['P'][3], false
                );
            }
        }
        
        // Création du code html pour ces graphs :
        if(!empty($rows)){
            $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Waste water treatement plant : Total load entering and discharged"), $rows);
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
    
    //================ TODO =================================
    //idée : pour obtenir cette info, il faudrait qu'a la fin du clacul du registre,
    //les valeurs pour les distances to compliance par article soient conservées au niveau du noeud "MS_LEVEL" de l'année concernée.
    //Les graphs ne seraient affichés que si l'info est présente dans le MS LEVEL ===> voir fonction "uwwtd_stat_national_ms_level_data"
    
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_distance_to_compliance($yearSelected, $yearsPossibles);
    $frontAndBackGraph1 = getRow8Graph1and2($results);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Distance to Compliance"), 
	    array(
	    	array(
	    		'graph_art3_pe', t("Article 3 by p.e."), $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1], true,
	    		'graph_art3_agg', t("Article 3 by number of agglomerations"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3], true
	    	),
	    )
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
					$rows[$newtworkTypeName]['data'][$year] = $values['equFormated'] .', '. $values['aggFormated'];
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
					<th><i>*pe: Population equivalent, *: Number of agglomeration</i></th>
				</tr>
			</thead>
		</table>';
	} 
	
	return array($front, $back);
}

/**
 * Retourne les données pour les graphes "Total generated load by compliance" et "Number of agglomerations by compliance".
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
                                'Entering'=>round(($element['in']['c'])*1000 , 3),
                                'Discharged'=>round(($element['out']['c'])*1000 , 3),
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
                    $line[$k] = uwwtd_format_number($v,2);
                    $graph1Front[$rowName][]=array(
                        'value'=>(float)$v,
                        'valueformat'=>uwwtd_format_number($v,2),
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
                    $line[$k] = uwwtd_format_number($v,2);
                    $graph2Front[$rowName][]=array(
                        'value'=>(float)$v,
                        'valueformat'=>uwwtd_format_number($v,2),
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
            $$graph1FrontRows = array();
            $$graph2FrontRows = array();
            $graph1Back = "";
            $graph2Back = "";
            
            if(isset($data['data'])){
                foreach($data['data'] as $key=>$item){
                    if(is_array($item)){
                        $graph1Front['data'][] = array(
                            (string)$key,
                            $item['in'],
                            $item['out'],
                        );
                        $graph1FrontRows[] = array(
                            (string)$key,
                            uwwtd_format_number($item['in'],2),
                            uwwtd_format_number($item['out'],2),
                            uwwtd_format_number($item['out']*100/$item['in'] ,1) .'%'
                        );
                        
                        $graph2Front['data'][] = array(
                            (string)$key,
                            $item['vol']>0 ?round($item['in']*1000/$item['vol'],3):0,
                            $item['vol']>0 ?round($item['out']*1000/$item['vol'],3):0,
                        );
                        $graph2FrontRows[] = array(
                            (string)$key,
                            $item['vol']>0 ?round($item['in']*1000/$item['vol'],3):0,
                            $item['vol']>0 ?round($item['out']*1000/$item['vol'],3):0,
                            uwwtd_format_number($item['out']*100/$item['in'] ,1) .'%'
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
	
	$columns[0]['Destination']['data'] = 'Destination';
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
            //ON bouche les trous
            foreach($front['data'] as $key=>$values){
                $item = array();
                $item[]=$values[0];
                foreach($legend_ord as $l=>$lab){
                    $item[] = (isset($values[$l])?$values[$l]:0);
                }
                $front['data'][$key] = $item;
            }
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
                            $rows[$keyElement]['data'][$year] = $element;
                            
                            $front[$year][] =array(
                                'value'=>$element,
                                'label'=>$GLOBALS['ms_level']['sludge'][$keyElement]['label'],
                                'valueformat'=>uwwtd_format_number($element).' T DS/year',
                                'color'=>$GLOBALS['ms_level']['sludge'][$keyElement]['color'],
                            );
                            
                        }
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
 * Retourne les données pour le graph1 et 2 de la ligne 8, à propos des Comparison of compliance.
 */
function getRow8Graph1and2($results) {
	// Variables du graph 1 :
	$graph1Front = array();
	$graph1Back = '';
	$columns = array();
	$graph1Rows = $results;
	$graph1Sums = array();
	
	dsm($results);
	
	// Variables du graph 2 :
	$graph2Front = array();
	$graph2Back = '';
	$graph2Rows = $results;
	$graph2Sums = array();
	
	
	return array($graph1Front, $graph1Back, $graph2Front, $graph2Back);
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
	$html.='</div>';
	return $html;
}

/**
 * Ajoute au code html donné le code html pour afficher deux graphs en barres verticales.
 */
function getHtmlPlusHtmlChartsBars($html, $rowTitle, $rows) {    
	$html.='<h2 style="clear:both;">' .$rowTitle. '</h2>';
    foreach($rows as $i=>$row){
        list(
            $graph1Id, $graph1Title, $graph1Front, $graph1Back, $graph1Stack, 
            $graph2Id, $graph2Title, $graph2Front, $graph2Back, $graph2Stack
        ) = $row;
        $html.='<div class="rows charts-row">';
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
	$html .='<h2 style="clear:both;">'. $rowTitle .'</h2>';
	$html .='<div class="rows charts-row">';
		// Création du code html pour le graph1 :
		//$front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
		//$front.= "<script>jQuery(document).ready(function(){display_piechart_custom(". json_encode($graph1Back) .", '". $graph1Id ."','middle');});</script>";
		//$msg_sludge = 'Production : ';
		
		// Gestion des productions par années :
        
		switch ($yearSelected){
			case ALL_YEARS :
				if (!empty($yearsPossibles)) {
					$msg_sludge .= " (T DS/year)";
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
					$msg_sludge .= $results[$yearSelected]['sludge']['production']." T DS/year";
				}
		}
		
		$html .= '<div class="cell charts-rows-cell"><div class="cell-content">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back, $msg_sludge).'</div></div>';
	
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
                .'<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper to-table" title="See the data table" alt="See the data table"></div>'
            . '</div>'
            . '<div class="flip-content"><b>'.$msg .'</b>'.$front.'</div>'
          .'</div>'

          .'<div class="leftgraph back">'
            . '<div class="flot-title flip-title-back">'
                .'<div class="flip-title">'.$title.'</div>'
                .'<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper to-chart" title="See diagram" alt="See diagram"></div>'
            . '</div>'
            . '<div class="flip-content"><b>'.$msg .'</b>'. $back .'</div>'
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
