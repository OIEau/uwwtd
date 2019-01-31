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
        $form['#suffix'] = $form_state['storage']['result'];
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
    libraries_load('d3.drupal');
    
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
		t("Number of agglomerations by compliance"), 'graph_agglomerationByCompliance', $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3],
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
    	t("Number of treatment plant by compliance"), 'graph_loadenteringByComplianceAgglomeration', $frontAndBackGraph1[2], $frontAndBackGraph1[3],
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
    	t("Number of treatment plant by treatment type"), 'graph_loadenteringByTreatmenttypeAgglomeration', $frontAndBackGraph1[2], $frontAndBackGraph1[3],
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
                'graph_generatedByAgglomerationNumber', t("Number of agglomeration by size"), $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3], true
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
    //===========nd@oieau.fr 10/01/2019 : FIX ME : the legend is too long on sludge chart
    $frontAndBackGraph1 = getRow7Graph1($yearSelected, $results);
    $frontGraph2 = getRow7Graph2($results, $yearSelected, $yearsPossibles);
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartPieAndText($formResult, t("MS Level statistics"),
    	t("Sludge destination"), 'graph_sludge_dest', $results,  $frontAndBackGraph1[0], $frontAndBackGraph1[1],
    	t("Water re-use"), $frontGraph2, $yearSelected, $yearsPossibles);
	
    //==============================================
    //=============== ROW 8 : Distance to target art 3, 4, 5
    //==============================================
    
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_distance_to_compliance($yearSelected, $yearsPossibles);
    $frontAndBackGraphs = getRow8Graphs($results);
    
    // Création du code html pour ces graphs :
    $formResult = getHtmlPlusHtmlChartsBars($formResult, t("Legal compliance"), 
	    array(
	    	array(
	    		'graph_art3_pe', t("Legal compliance for collection in p.e."), $frontAndBackGraphs['graph1Front'],  $frontAndBackGraphs['graph1Back'], true,
	    		'graph_art3_agg', t("Legal compliance for collection by number of agglomerations"), $frontAndBackGraphs['graph2Front'],  $frontAndBackGraphs['graph2Back'], true
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
            /*
			foreach ($frontTmp as $year => $groupElements) {
				// Suppresion du groupe du total :
				array_shift($frontTmp[$year]);
			}*/
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
            /*
			foreach ($front as $year => $groupElements) {
				// Suppresion de la ligne du total :
				array_shift($front[$year]);
			}*/
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
							array_key_exists('out', $element) && 
							array_key_exists('vol', $element)) {
							// Redéfinition de la valeur de l'année :
							$graph1Rows[$rowName]['data'][$keyElement] = array(
                                'Entering' => isset($element['in']['q'])?$element['in']['q']:$element['in'], 
                                'Discharged'=>isset($element['out']['q'])?$element['out']['q']:$element['out']
                            );

                            if(isset($element['in']['q'])) $sum+=$element['in']['q'];
                            else $sum+=$element['in'];
							//Convert to mg/l
							$graph2Rows[$rowName]['data'][$keyElement] = array(
                                'Entering'=>isset($element['in']['c'])?$element['in']['c']:round(($element['in']/$element['vol']) , 3),
                                'Discharged'=>isset($element['out']['c'])?$element['out']['c']:round(($element['out']/$element['vol']) , 3),
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
                            $item['in'] ? uwwtd_format_number(($item['in']['q']- $item['out']['q'])*100/$item['in']['q'] ,1) .'%':'-'
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
                            $item['in']['c']> 0 ? round($item['in']['c'], 3) : '-',
                            $item['out']['c']> 0 ? round($item['out']['c'], 3) : '-',
                            $item['in']['c'] && ($item['out']['c'] * 100 / $item['in']['c']) > 0 ? uwwtd_format_number(($item['in']['c'] - $item['out']['c']) * 100 / $item['in']['c'], 1) . '%' : '-'

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
            $legend = array();
            $color = array();
            foreach ($results as $year => $row) {
                $columns[0][$year]['data'] = $year;
                $item = array();
                $item[0] = (string)$year;   
                if (array_key_exists('sludge', $row) && array_key_exists('destination', $row['sludge'])) {
                    foreach ($row['sludge']['destination'] as $keyElement => $element) {
                        if (!is_null($element)) {
                            $rows[$keyElement]['data']['Destination'] = $GLOBALS['ms_level']['sludge'][$keyElement]['label'];
                            
                            // Ajout de la ligne :
                            $rows[$keyElement]['data'][(string)$year] = $element;
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
                $tot = 0;
                foreach($legend_ord as $l=>$lab){
                    $val = (isset($values[$l])?$values[$l]:0);
                    $item[] = $val;
                    $tot +=$val;
                    if(!isset($table[$l])) $table[$l][0]= $lab;
                    $table[$l][$year] =  (isset($values[$l])? uwwtd_format_number($values[$l], 1):0);
                }
                if(empty($table['Total'])) $table['Total'][0] = '<strong>'.t('Total'). '</strong>';
                 $table['Total'][$year] =  '<strong>'.uwwtd_format_number($tot, 1). '</strong>';
                $front['data'][$key] = $item;
            } 
            $back = theme('table', array('rows' => $table, 'header' =>$header)); // 'header' => $columns, 'rows_multiple' => true
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
            $front='';
			if (count($graph1Front) <= 1) {
				$front.= '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
				$keys = array_keys($graph1Front);
				$graph1Front = $graph1Front[$keys[0]];
				$front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph1Front).", '". $graph1Id ."');});</script>";
			} elseif (array_key_exists('data', $graph1Front) && 
				array_key_exists('color', $graph1Front) && 
				array_key_exists('legend', $graph1Front)) {
				$front .= uwwtd_graph_render_column($graph1Id, $graph1Front['legend'], $graph1Front['color'], $graph1Front['data'], isset($graph1Front['stacked'])?$graph1Front['stacked']:true);
			}
			$html.= '<div class="cell charts-rows-cell"><div class="cell-content'. $classCustom .'">'.uwwtd_graph_render_flipcard($graph1Title, $front, $graph1Back).'</div></div>';
		}
		

		// Si le graph a des données :
		if (!$isEmptyGraph2) {
			// Création du code html pour le graph 2 :
            $front = '';
			if (count($graph2Front) <= 1) {
				$front.= '<svg id="'. $graph2Id .'" style="margin:10px;"/>';
				$keys = array_keys($graph2Front);
				$graph2Front = $graph2Front[$keys[0]];
				$front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($graph2Front).", '". $graph2Id ."');});</script>";
			} 
            elseif (array_key_exists('data', $graph2Front) &&
				array_key_exists('color', $graph2Front) &&
				array_key_exists('legend', $graph2Front)) {
				$front .= uwwtd_graph_render_column($graph2Id, $graph2Front['legend'], $graph2Front['color'], $graph2Front['data'], isset($graph2Front['stacked'])?$graph2Front['stacked']:true);
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
                    $msg_sludge_remark = '';
                    $msg_sludge = '';
                    if($yearSelected!=ALL_YEARS && isset($results[$yearSelected])){
                        if($results[$yearSelected]['sludge']['production']!=''){
                            $msg_sludge = t(
                                "In @year, @production Tons DS of sludge was generated", 
                                ['@year'=>$yearSelected, '@production'=>uwwtd_format_number($results[$yearSelected]['sludge']['production'])]
                            );
                        }
                        if($results[$yearSelected]['sludge']['remark']!=''){
                            $msg_sludge_remark = $results[$yearSelected]['sludge']['remark'];
                        }
                    }
                    $front = uwwtd_graph_render_column($graph1Id, $graph1Front['legend'], $graph1Front['color'], $graph1Front['data'], true);
				
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

function uwwtd_graph_render_column($id, $legend, $color, $rows, $stacked=false, $width=510, $height=260){
  $chart = array(
	'id' => $id,
	'type' => ($stacked?'StackedColumnChart':'ColumnChart'),
	'legend' => $legend,
	'rows' => $rows,
	'width'=>$width,
	'height'=>$height,
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
    //dsm($inspireIdLocalId);
    $data = uwwtd_getAggHistoryData($inspireIdLocalId);
    //dsm($data);
    if(empty($data)) return "Error in data. We can't display this page";
    
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
        $html.= '<div class="uwwhalf" style="clear:left;">'.uwwtd_timeline_output($data['last_node'], true).'</div>';
    $html.= '</div>';
    if(!empty($data['years'])){
        $years = array_keys($data['years']);
        sort($years);
        $uwwtp_data = [];
        $uwwtp_list = [];
        $dtc_equipment =[];
            $dtc_equipment[]='<tr><th>'.t("Year").'</th><th>'.t("Collection").'</th><th>'.t("2nd treatment").'</th><th>'.t("3rd treatment").'</th></tr>';
        $dtc_performance =[];
            $dtc_performance[]='<tr><th>'.t("Year").'</th><th>'.t("Collection").'</th><th>'.t("2nd treatment").'</th><th>'.t("3rd treatment").'</th></tr>'; 
        
        
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
        $collection_table_rows = array();
        $collection_graph = array(
            'data'=>array(),
            'legend'=>array(t($GLOBALS['ms_level']['connection']['cs']['label']),t("Agglomeration's Entering load")),
            'color'=>array($GLOBALS['ms_level']['connection']['cs']['color'], '#87ceeb'),
            'stacked'=>false
        );
        $prev_y = null;
        foreach($years as $y){
            if(isset($data['years'][$y])){
                $info = $data['years'][$y];
                //==========Generated load
                $genload_graph['data'][] = array(
                    (string)$y,
                    (float)$info['cs'],
                    (float)$info['ias'],
                    (float)$info['wot'],
                );
                $genload_table_rows[] = array(
                    '<b>'.$y.'</b>',
                    uwwtd_format_number((float)$info['cs']) .' p.e.',
                    uwwtd_format_number((float)$info['ias']) .' p.e.',
                    uwwtd_format_number((float)$info['wot']) .' p.e.',
                    uwwtd_format_number((float)$info['Generated load']) .' p.e.'
                );
                //Collection
                $ent_load = array_sum (array_column($info['uwwtps'], 'entering_load_pe'));
                $collection_graph['data'][] = array(
                    (string)$y,
                    round((float)$info['cs']),
                    round((float)$ent_load),
                );
                $collection_delta = round((float)$info['cs']-(float)$ent_load);
                $collection_color = '#4f4f4f';
                if($collection_delta!=0){
                    $collection_color = '#ffa500';
                    //TODO : raise warning on delta
                }
                if($collection_delta>=2000){
                    $collection_color = '#d93c3c';
                    //TODO : raise warning on delta
                }elseif((float)$info['cs']>0 && $collection_delta>0 && ($collection_delta/(float)$info['cs']) >= 0.01 ){
                    $collection_color = '#d93c3c';
                    //TODO : raise warning on delta
                }
                
                $collection_table_rows[$y] = array(
                    'year'=>'<b>'.$y.'</b>',
                    'cs'=>uwwtd_format_number(round((float)$info['cs'])) .' p.e.',
                    'ent'=>uwwtd_format_number(round((float)$ent_load)) .' p.e.',
                    'delta'=>'<b style="color:'.$collection_color.'">'.uwwtd_format_number($collection_delta) .' p.e.</b> ('.uwwtd_get_percent((float)$info['cs'], (float)$ent_load).')'
                );
                
                
                //============= Distance to target
                $dtc_equipment[]= '
                    <tr>
                        <td rowspan="2"><b>'.$y.'</b></td>
                        <td style="color:white;background-color:'.$info['dtc']['art3']['Equipment']['color'].'">'.uwwtd_format_number($info['dtc']['art3']['Equipment']['pe']).' p.e.</td>
                        <td style="color:white;background-color:'.$info['dtc']['art4']['Equipment']['color'].'">'.uwwtd_format_number($info['dtc']['art4']['Equipment']['pe']).' p.e.</td>
                        <td style="color:white;background-color:'.$info['dtc']['art5']['Equipment']['color'].'">'.uwwtd_format_number($info['dtc']['art5']['Equipment']['pe']).' p.e.</td>
                    </tr>
                    <tr>
                        <td style="color:white;background-color:'.$info['dtc']['art3']['Equipment']['color'].'">'.uwwtd_format_number($info['dtc']['art3']['Equipment']['percent'], 1).'%</td>
                        <td style="color:white;background-color:'.$info['dtc']['art4']['Equipment']['color'].'">'.uwwtd_format_number($info['dtc']['art4']['Equipment']['percent'], 1).'%</td>
                        <td style="color:white;background-color:'.$info['dtc']['art5']['Equipment']['color'].'">'.uwwtd_format_number($info['dtc']['art5']['Equipment']['percent'], 1).'%</td>
                    </tr>';
                $dtc_performance[]= '
                    <tr>
                        <td rowspan="2"><b>'.$y.'</b></td>
                        <td>-</td>
                        <td style="color:white;background-color:'.$info['dtc']['art4']['Performance']['color'].'">'.uwwtd_format_number($info['dtc']['art4']['Performance']['pe']).' p.e.</td>
                        <td style="color:white;background-color:'.$info['dtc']['art5']['Performance']['color'].'">'.uwwtd_format_number($info['dtc']['art5']['Performance']['pe']).' p.e.</td>
                    </tr>
                    <tr>
                        <td>-</td>
                        <td style="color:white;background-color:'.$info['dtc']['art4']['Performance']['color'].'">'.uwwtd_format_number($info['dtc']['art4']['Performance']['percent'],1).'%</td>
                        <td style="color:white;background-color:'.$info['dtc']['art5']['Performance']['color'].'">'.uwwtd_format_number($info['dtc']['art5']['Performance']['percent'],1).'%</td>
                    </tr>';
                    
                $capa = array_sum (array_column($info['uwwtps'], 'physicalcapacityactivity'));
                    $capa_str = '<b>'.uwwtd_format_number($capa) .' p.e.</b>';
                $ent = array_sum (array_column($info['uwwtps'], 'uwwloadenteringuwwtp'));
                    $ent_str = '<b>'.uwwtd_format_number($ent) .' p.e.</b>';
                if(!is_null($prev_y)){
                    $prev_capa = array_sum (array_column($data['years'][$prev_y]['uwwtps'], 'physicalcapacityactivity'));
                        $capa_str.='<div class="delta">'.$prev_y.'\'s delta : '. uwwtd_format_number($capa-$prev_capa) .' p.e. ('.uwwtd_get_percent($capa, $prev_capa).')</div>';
                    $prev_ent = array_sum (array_column($data['years'][$prev_y]['uwwtps'], 'uwwloadenteringuwwtp'));
                        $ent_str.='<div class="delta">'.$prev_y.'\'s delta : '. uwwtd_format_number($ent-$prev_ent) .' p.e. ('.uwwtd_get_percent($ent, $prev_ent).')</div>';
                }
                
                $uwwtp_data[$y]=[
                    'year'=>'<b>'.$y.'</b>',
                    'links'=>theme('item_list', ['items'=>array_column($info['uwwtps'], 'link')]),
                    'capa'=> $capa_str,
                    'ent'=>$ent_str
                    
                ];
                $uwwtp_list[$y]=array_column($info['uwwtps'], 'inspireidlocalid');
            }
            else{
                $genload_graph['data'][] = array((string)$y,null,null,null);
                $genload_table_rows[] = array('<b>'.$y.'</b>', '-', '-', '-');
            }
            $prev_y = $y;
        }
        $genload_table = theme(
            'table', array(
                'rows' => $genload_table_rows, 
                'header' => array(
                    t('Year'), 
                    '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t($GLOBALS['ms_level']['connection']['cs']['label']).'</div><span class="custom_tooltip"> Generated load (p.e.) &lt;aggGenerated&gt; x Rate of generated load of agglomeration collected through collecting system (% of p.e.) &lt;aggC1&gt; </span></div>',
                    '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t($GLOBALS['ms_level']['connection']['ias']['label']).'</div><span class="custom_tooltip"> Generated load (p.e.) &lt;aggGenerated&gt; x Rate of generated load of agglomeration addressed through IAS (% of p.e.) (% of p.e.) &lt;aggC2&gt; </span></div>',
                   '<div class="field custom_tooltip_wrapper"><div class="field-label">'. t($GLOBALS['ms_level']['connection']['wot']['label']).'</div><span class="custom_tooltip"> Generated load (p.e.) &lt;aggGenerated&gt; x Rate of generated load of agglomeration not collected through collecting systems and not addressed through IAS (% of p.e.)&lt;aggPercWithoutTreatment&gt; </span></div>',
                    '<b>'.t('Total').'</b>')
                )
        );
        
        //dsm($collection_table_rows);
        $collection_table = theme(
            'table', array(
                'rows' => $collection_table_rows, 
                'header' => array(
                    t('Year'),
                    '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t($GLOBALS['ms_level']['connection']['cs']['label']).'</div><span class="custom_tooltip"> Generated load (p.e.) &lt;aggGenerated&gt; x Rate of generated load of agglomeration collected through collecting system (% of p.e.) &lt;aggC1&gt; </span></div>',
                    '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t("Agglomeration's Entering load").'</div><span class="custom_tooltip">Sum of entering load of linked UWWTP from &lt;uwwLoadEnteringUWWTP&gt;</span></div>',
                    '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Delta').'</div><span class="custom_tooltip"> Difference between '.t($GLOBALS['ms_level']['connection']['cs']['label']).' and '.t("Agglomeration's Entering load").'. Warning : the delta compared theorical value (generated load) and measured value (entering load)</span></div>',
                )
        ));
        //dsm( $genload_graph);
        
        $rowTitle = t("Generated load and collection");
        $html = getHtmlPlusHtmlChartsPies($html, $rowTitle,
		'Generated Load by collection type (population equivalent – p.e.)', 'genloadbyct', $genload_graph, $genload_table, 
		"Generated Load collected by collective system vs UWWTP's entering load", 'collection-leaks', $collection_graph, $collection_table,
		false, []);
        
        
        //Distance to target
        $html .='<div class="rows">';
            $html .='<h2>'.t("Distance to target").'</h2>';
            $html .='<div class="charts-rows-cell cell dtc dtc-equipment"><div class="cell-content"><h3>'.t("On equipment").'</h3><div><table class="dtcomp">'.implode('', $dtc_equipment).'</table></div></div></div>';
            $html .='<div class="charts-rows-cell cell dtc dtc-performance"><div class="cell-content"><h3>'.t("On performance").'</h3><div><table class="dtcomp">'.implode('', $dtc_performance).'</table></div></div></div>';
            $html .='<div style="padding:1em;"><br/><p>'.  t('The distance to target concept present the rate of waste water load that is:').'
                            <ul>
                                <li>'. t('adequately connected to a centralised urban waste water collecting system or addressed via Individual or Appropriate System (IAS)').'</li>
                            </ul>
                            '. t('and then when collected:').'
                            <ul>
                                <li>'.t('treated at an adequate level (secondary or more stringent treatment) as required by the directive,').'</li>
                                <li>'.t('and with the performance requirements under tables 1 or 2 of the annex I of Directive 91/271/EEC, (UWWTD).').'</li>
                            </ul>
                            '. t('When countries joined the EU, they have obtained a delay to implement the Directive. 
                            In such case the deadline for implementing the above (see in section characteristics) may be different, 
                            and if the above is not already implemented for the agglomeration it is identified with orange colour in the table.').'</p></div>';
        $html .='</div>';
        
        //List of UWWTP
        $html .='<div class="rows">';
        $html .='<h2>'.t('Treatment plants').'</h2>';
            //dsm($uwwtp_list);
            $html .=uwwtd_compare_idlist($uwwtp_list);
            //Mettre un message s'il y a un nombre différent et ou des id qui change au fil du temps
            $html .=theme('table', [
                'rows'=>$uwwtp_data,
                'header'=>[
                    t('Year'), 
                    t('Linked Treatment Plant(s)'),
                    '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Total design capacity').'</div><span class="custom_tooltip">Sum of linked UWWTP\'s organic design capacity from field &lt;uwwCapacity&gt; : Organic design capacity (p.e.)</span></div>',
                    '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Total entering load').'</div><span class="custom_tooltip">Sum of linked UWWTP\'s load entering design capacity from field &lt;uwwLoadEnteringUWWTP&gt; : Load entering UWWTP (p.e.)</span></div>',
                ]
            ]);
        $html .='</div>';
    }

    return '<article>'.$html.'</article>';
}
function uwwtd_compare_idlist($ids){
    $output = '';
    $msg =[]; 
    if(!empty($ids)){
        $nb = null;
        $prev_id = null;
        $prev_list = null;
        foreach($ids as $id=>$list){
            if(!is_null($nb)){
                if($nb!=count($list)){
                    $msg[] = t("There is a change in number of elements between :previous and :current", [':previous'=>$prev_id, ':current'=>$id]);
                }
                //Check if we have change in list               
                $diff_new = array_diff_key (array_flip($list), array_flip($prev_list));
                if(!empty($diff_new)){
                    $msg[] = t("There are new elements between :previous and :current", [':previous'=>$prev_id, ':current'=>$id]) .': '.implode(',', array_keys($diff_new)).'.';
                }
                $diff_miss= array_diff_key (array_flip($prev_list), array_flip($list));
                if(!empty($diff_miss)){
                    $msg[] = t("There are missing elements between :previous and :current", [':previous'=>$prev_id, ':current'=>$id]) .': '.implode(',', array_keys($diff_miss)).'.';
                }
                unset($diff_new, $diff_miss);
            }
            $nb=count($list);
            $prev_id = $id;
            $prev_list =$list;
        }
    }
    if(!empty($msg)){
        $output.= '<div class="alert alert-block alert-danger messages error">';
        $output.= theme('item_list', array('items'=>$msg));
        $output.= '</div>';
    }
    return $output;
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
    
    //Greek case
    if(in_array(strtolower(variable_get('siif_eru_country_code')), ['gr', 'el'])){
        $localids=[$inspireIdLocalId];
        $localids[]=str_replace('el', 'gr',$inspireIdLocalId);
        $localids[]=str_replace('gr', 'el',$inspireIdLocalId);
        $query->where("replace(lower(ins.field_inspireidlocalid_value),'_', '') IN ('". implode("','",$localids) ."')");
    }
    else{
        $query->where("replace(lower(ins.field_inspireidlocalid_value),'_', '') = :id", array(':id'=>$inspireIdLocalId));
    }
    $query->orderBy('a.field_anneedata_value', 'ASC');
    $ids = $query->execute()->fetchAllAssoc("field_anneedata_value");
    if(!empty($ids)){
        $data = array();
        foreach($ids as $y=>$id){
            $node = node_load($id->nid);
            $wrapper =  entity_metadata_wrapper('node', $node);
            $genLoad = $wrapper->field_agggenerated->value();
            $data[$y] = array(
                'Year'=>$y,
                'agg'=>$node,
                'Generated load'=>$genLoad,
                'cs'=>round($genLoad * $wrapper->field_aggc1->value()/100),
                'ias'=>round($genLoad * $wrapper->field_aggc2->value()/100),
                'wot'=>round($genLoad * (100 - ($wrapper->field_aggc1->value() + $wrapper->field_aggc2->value()))/100),
                /*
                'cs'=>$wrapper->field_agg_connected_load_pe->value(),
                'ias'=>$wrapper->field_agg_ias_load_pe->value(),
                'wot'=>$wrapper->field_agg_discharge_pe->value(),
                */
                //$agg_leak = ($connected_load - $entering_load);
                //        - connected_load (float) : load connected to collectiong system
                //        - entering_load (float) : total load entering in the WWTP linked to the agglomeration
                'leak'=>0, 
                'uwwtps'=>array(),
                'dtc'=>uwwtd_get_distance_compliance($node),
            );
            $uwwtps = $wrapper->field_agguwwliste->value();
            //dsm($uwwtps);
            if(!empty($uwwtps)){
                foreach($uwwtps as $uwwtp){
                    //dsm($uwwtp);
                    if($uwwtp->field_status['und'][0]['value']==1){
                        $data[$y]['uwwtps'][$uwwtp->nid] =[
                            'link' =>'<span class="'.strtolower($uwwtp->field_uwwcompliance['und'][0]['value']).'"> '.$GLOBALS['uwwtd']['all_compliance'][$uwwtp->field_uwwcompliance['und'][0]['value']].' </span> '.  l($uwwtp->title .' ['.$uwwtp->field_inspireidlocalid['und'][0]['value'].']', 'node/'.$uwwtp->nid),
                            'inspireidlocalid'=>$uwwtp->field_inspireidlocalid['und'][0]['value'],
                            'compliance'=>$uwwtp->field_uwwcompliance['und'][0]['value'],
                            'uwwloadenteringuwwtp'=>$uwwtp->field_uwwloadenteringuwwtp['und'][0]['value'],
                            'physicalcapacityactivity'=>$uwwtp->field_physicalcapacityactivity['und'][0]['value'],
                            'entering_load_pe'=>0
                        ];
                        
                        $agglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $y, 'aggCode' => $node->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
                        if ('' !== $agglo_uwwtpnid) {
                            $agglo_uwwtp = node_load($agglo_uwwtpnid);
                            $data[$y]['uwwtps'][$uwwtp->nid]['entering_load_pe'] = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $node->field_agggenerated['und'][0]['value'];
                        }
                    }
                }
            }
        }
    }
    return array('last_node'=>$node, 'years'=>$data);
}

function uwwtd_getUWWTPHistoryData($inspireIdLocalId){
    $inspireIdLocalId = strtolower($inspireIdLocalId);
    $query = db_select('node', 'n');
    $query->innerJoin('field_data_field_anneedata', 'a', 'n.nid = a.entity_id AND a.entity_type = \'node\'');
    $query->leftJoin('field_data_field_inspireidlocalid', 'ins', 'n.nid = ins.entity_id AND ins.entity_type = \'node\'');
    $query->fields('a', array('field_anneedata_value'));
    $query->fields('n', array('nid'));
    $query->condition('n.type', 'uwwtp', '=');
    $query->condition('n.status', 1, '=');
    if(in_array(strtolower(variable_get('siif_eru_country_code')), ['gr', 'el'])){
        $localids=[$inspireIdLocalId];
        $localids[]=str_replace('el', 'gr',$inspireIdLocalId);
        $localids[]=str_replace('gr', 'el',$inspireIdLocalId);
        $query->where("replace(lower(ins.field_inspireidlocalid_value),'_', '') IN ('". implode("','",$localids) ."')");
    }
    else{
        $query->where("replace(lower(ins.field_inspireidlocalid_value),'_', '') = :id", array(':id'=>$inspireIdLocalId));
    }
    $query->orderBy('a.field_anneedata_value', 'ASC');
    $ids = $query->execute()->fetchAllAssoc("field_anneedata_value");
    if(!empty($ids)){
        $data = array();
        foreach($ids as $y=>$id){
            $node = node_load($id->nid);
            $wrapper =  entity_metadata_wrapper('node', $node);
            $data[$y] = array(
                'Year'=>$y,
                'uwwtp'=>$node,
                'loadentering'=>$wrapper->field_uwwloadenteringuwwtp->value(),
                'physicalcapacityactivity'=>$wrapper->field_physicalcapacityactivity->value(),
                'wastewatertreated'=>$wrapper->field_uwwwastewatertreated->value(),
                'bod'=>array(
                    'in'=>$wrapper->field_uwwbodincoming->value(), 
                    'out'=>$wrapper->field_uwwboddischarge->value(), 
                ),
                'cod'=>array(
                    'in'=>$wrapper->field_uwwcodincoming->value(), 
                    'out'=>$wrapper->field_uwwcoddischarge->value(), 
                ),
                'n'=>array(
                    'in'=>$wrapper->field_uwwnincoming->value(), 
                    'out'=>$wrapper->field_uwwndischarge->value(), 
                ),
                'p'=>array(
                    'in'=>$wrapper->field_uwwpincoming->value(), 
                    'out'=>$wrapper->field_uwwpdischarge->value(), 
                ),
                'dcps'=>array(),
                'aggs'=>array(),
            );
            
            //List Dicharge points
            //field_uwwdcpliste
            $dcps = $wrapper->field_uwwdcpliste->value();
            if(!empty($dcps)){
                foreach($dcps as $dcp){
                    if($dcp->field_status['und'][0]['value']==1){
                        $w = entity_metadata_wrapper('node', $dcp);
                        $rca = $w->field_linked_receiving_areas->value();
                        $rca_link = l($rca[0]->title.' ['.$rca[0]->field_inspireidlocalid['und'][0]['value'].']', 'node/'.$rca[0]->nid);
                        
                        $data[$y]['dcps'][$dcp->nid] =[
                            'link' =>  l($dcp->title .' ['.$w->field_inspireidlocalid->value().']', 'node/'.$dcp->nid) .' '.t('in receiving area').' '.$rca_link . ' '.t('of type').' "'.$GLOBALS['uwwtd_value']['rcatype_labels'][$w->field_rcatype->value()].'"',
                            //	field_rcatype
                            //field_linked_receiving_areas
                            'inspireidlocalid'=>$w->field_inspireidlocalid->value(),
                        ];
                    }
                }
            }
            
            
            //List Agglomeration
            $aggs = $wrapper->field_uwwaggliste->value();
            //dsm($aggs);
            if(!empty($aggs)){
                foreach($aggs as $agg){
                    //dsm($uwwtp);
                    if($agg->field_status['und'][0]['value']==1){
                        $data[$y]['aggs'][$agg->nid] =[
                            'link' =>'<span class="'.strtolower($agg->field_aggcompliance['und'][0]['value']).'"> '.$GLOBALS['uwwtd']['all_compliance'][$agg->field_aggcompliance['und'][0]['value']].' </span> '.  l($agg->title .' ['.$agg->field_inspireidlocalid['und'][0]['value'].']', 'node/'.$agg->nid),
                            'inspireidlocalid'=>$agg->field_inspireidlocalid['und'][0]['value'],
                            'compliance'=>$agg->field_aggcompliance['und'][0]['value'],
                            'genload'=>$agg->field_agggenerated['und'][0]['value'],
                        ];
                        
                        /* get value of entering load from */
                        $agglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $y, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $node->field_inspireidlocalid['und'][0]['value'])));
                        if ('' !== $agglo_uwwtpnid) {
                            $agglo_uwwtp = node_load($agglo_uwwtpnid);
                            $data[$y]['aggs'][$agg->nid]['entering_load_pe'] = round(($agg->field_aggc1['und'][0]['value'] / 100) * ($agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100) * $agg->field_agggenerated['und'][0]['value']);
                            $data[$y]['aggs'][$agg->nid]['link'].= ': '.uwwtd_format_number($data[$y]['aggs'][$agg->nid]['entering_load_pe']).' p.e.';
                        }
                        
                    }
                }
            }
        }
    }
    return array('last_node'=>$node, 'years'=>$data);
}

//This new page display historical information on uwwtp 
function uwwtd_uwwtp_history_page($inspireIdLocalId){
    $data = uwwtd_getUWWTPHistoryData($inspireIdLocalId);
    if(empty($data)) return "Error in data. We can't display this page";
    
    drupal_set_title($data['last_node']->title.' - history');
            drupal_add_js('sites/all/libraries/d3/d3.js');
            drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
            drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    libraries_load('d3.drupal');
    
    //dsm($data);
    
    $html='';
    $html.= '<div class="uwwcontainer">';
        $fieldstat = field_view_field('node', $data['last_node'], 'field_status');
        if (!empty($fieldstat[0]['#markup']) && $fieldstat[0]['#markup'] == '2') {
            $fieldstat[0]['#markup'] = 'Inactive';
        }
        $fieldnuts = field_view_field('node', $data['last_node'], 'field_regionnuts');
        $html.= '<div class="google-map-banner"><h1><span class="white-title">'. t('UWWTP').' : </span><span class="node-title">'. $data['last_node']->title.'</span><span class="white-title"> - '.t("Identifier").' : </span><span class="inspireidlocalid">'. $data['last_node']->field_inspireidlocalid['und'][0]['value'].'</span><span class="white-title"> - '. t("Status").' : </span><span class="status status-'.drupal_clean_css_identifier(strtolower($fieldstat[0]['#markup'])).'">'.$fieldstat[0]['#markup'].'</span><br /><small>'. t("Region (NUTS) Code").' : '. $data['last_node']->field_regionnuts['und'][0]['value'].' - '. t("Region (NUTS) Name").' : '.$fieldnuts[0]['#markup'].'</small></h1></div>';
        $html.= render(field_view_field('node',  $data['last_node'], 'field_position_geo', 'openlayers_map'));
        $html.= '<div class="uwwhalf" style="clear:left;">'.uwwtd_timeline_output($data['last_node'], true).'</div>';
    $html.= '</div>';
    $years =[];
    
    if(!empty($data['years'])){
        $years = array_keys($data['years']);
        
        //UWWTP description
        $uwwInfo = array();
        
        //entering load / physicalcapacityactivity 
        $elpc_graph = array(
            'data'=>array(),
            'color'=>array(
                '#00658e',
                '#459436'
            ),
            'legend'=>array(
                t('Entering load'),
                t('Physical Capacity'),
            ),
            'stacked'=>false
        );
        $elpc_table = array();
        
        //Waste water treated
        $wwt_graph = array(
            'data'=>array(),
            'color'=>array('#00658e'),
            'legend'=>array(t('Waste water treated (m3/y)')),
        );
        $wwt_table = array();
        $prev_wastewatertreated = null;
        // N
        $n_graph = array(
            'data'=>array(),
            'color'=>array('#87ceeb', '#c8e8f5'),
            'legend'=>array(
                t('Entering'),
                t('Discharged')
            ),
            'stacked'=>false
        );
        $n_table = array();
        //P
        $p_graph = array(
            'data'=>array(),
            'color'=>array('#ff9900', '#ffcf95'),
            'legend'=>array(
                t('Entering'),
                t('Discharged')
            ),
            'stacked'=>false
        );
        $p_table = array();
        //COD
        $cod_graph = array(
            'data'=>array(),
            'color'=>array('#d87600', '#ffcf95'),
            'legend'=>array(
                t('Entering'),
                t('Discharged')
            ),
            'stacked'=>false
        );
        $cod_table = array();
        //BOD
        $bod_graph = array(
            'data'=>array(),
            'color'=>array('#5B3349', '#dcc1d0'),
            'legend'=>array(
                t('Entering'),
                t('Discharged')
            ),
            'stacked'=>false
        );
        $bod_table = array();
        
        //Agglomerations
        $aggInfo = array();
        $agg_list = array();
        
        //DCP
        $dcpInfo = array();
        $dcp_list = array();  
        
        foreach($years as $year){
            $elpc_graph['data'][]=array(
                (string)$year,
                (integer)$data['years'][$year]['loadentering'],
                (integer)$data['years'][$year]['physicalcapacityactivity'],
            );
            
            $load_rate = null;
            if(!is_null($data['years'][$year]['physicalcapacityactivity']) && $data['years'][$year]['physicalcapacityactivity'] > 0) {
                $load_rate = round((integer)$data['years'][$year]['loadentering']/(integer)$data['years'][$year]['physicalcapacityactivity']*100 , 1);
            }
            $load_rate_color = '#4f4f4f';
            $load_mention ='';
            if(is_null($load_rate)){
                //catch 
            }
            //Si > 100 % : surcharge, Si < 70 % sous charge
            elseif(($load_rate>100 && $load_rate<=110)) {
                $load_rate_color = '#ff8c00';
                $load_mention = t('overload');
            }
            elseif($load_rate<=70){
                $load_rate_color = '#ff8c00';
                $load_mention = t('underload');
            }
            //Si > 110 %  surcharge problématique
            elseif($load_rate>110) {
                $load_rate_color = '#ff0000';
                $load_mention = t('overload');
                //raise message ?
            }
            $elpc_table[$year] = array(
                '<b>'.$year.'</b>',
                uwwtd_format_number($data['years'][$year]['loadentering']) .' p.e.',
                uwwtd_format_number($data['years'][$year]['physicalcapacityactivity']) .' p.e.',
                !is_null($load_rate)?'<b style="color:'.$load_rate_color.'">'. $load_rate .' % '.$load_mention.'</b>':''
            );
            $wwt_graph['data'][]=array(
                (string)$year,
                (integer)$data['years'][$year]['wastewatertreated'],
            );
            $wwt_table[$year] = array(
                '<b>'.$year.'</b>',
                uwwtd_format_number($data['years'][$year]['wastewatertreated']) .' m3/y',
                !is_null($prev_wastewatertreated)?round((($data['years'][$year]['wastewatertreated']-$prev_wastewatertreated)/$prev_wastewatertreated)*100, 1) .' %':'-'
            );
            
            $treatment = 'required : '.$GLOBALS['glossary'][$data['years'][$year]['uwwtp']->field_uwwtreatmentrequired['und'][0]['value']].'<br/>';
            $treatment.= 'in place : '.$GLOBALS['glossary'][$data['years'][$year]['uwwtp']->field_uwwtreatmenttype['und'][0]['value']].'<br/>';
            
            $load = '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Capacity').':</div> '.uwwtd_format_number($data['years'][$year]['physicalcapacityactivity']) .' p.e.<span class="custom_tooltip">From field &lt;uwwCapacity&gt; : Organic design capacity (p.e.)</span></div>';
            $load.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Entering').':</div> '.uwwtd_format_number($data['years'][$year]['loadentering']) .' p.e.<span class="custom_tooltip">From field &lt;uwwLoadEnteringUWWTP&gt; : Load entering UWWTP (p.e.)</span></div>';
            $load.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Load rate').':</div> <b style="color:'.$load_rate_color.'">'. $load_rate .' % '.$load_mention.'</b><span class="custom_tooltip">&lt;uwwLoadEnteringUWWTP&gt; / &lt;uwwCapacity&gt;  x 100 </span></div>';
            
            $comp = 'Compliance : <span class="'.strtolower($data['years'][$year]['uwwtp']->field_uwwcompliance['und'][0]['value']).'">'.$GLOBALS['uwwtd']['all_compliance'][$data['years'][$year]['uwwtp']->field_uwwcompliance['und'][0]['value']].'</span><br/>';
            $comp.= 'Treatment met : <span class="met-'.strtolower($data['years'][$year]['uwwtp']->field_uwwtreatment_met['und'][0]['value']).'">'.$GLOBALS['uwwtd']['ui']['yes_no'][$data['years'][$year]['uwwtp']->field_uwwtreatment_met['und'][0]['value']].'</span><br/>';
            $comp.= 'Performance met : <span class="met-'.strtolower($data['years'][$year]['uwwtp']->field_uwwperformance_met['und'][0]['value']).'">'.$GLOBALS['uwwtd']['ui']['yes_no'][$data['years'][$year]['uwwtp']->field_uwwperformance_met['und'][0]['value']].'</span><br/>';
            
            $perf = '';
        if($data['years'][$year]['uwwtp']->field_uwwbod5perf['und'][0]['value']!=''){
            $perf.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('BOD').':</div> <span class="test-'.strtolower($data['years'][$year]['uwwtp']->field_uwwbod5perf['und'][0]['value']).'">'. $GLOBALS['uwwtd']['ui']['test_pass'][$data['years'][$year]['uwwtp']->field_uwwbod5perf['und'][0]['value']].'</span><span class="custom_tooltip">From field &lt;uwwBOD5Perf&gt;</span></div>';
        }
        if($data['years'][$year]['uwwtp']->field_uwwcodperf['und'][0]['value']!=''){
            $perf.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('COD').':</div> <span class="test-'.strtolower($data['years'][$year]['uwwtp']->field_uwwcodperf['und'][0]['value']).'">'. $GLOBALS['uwwtd']['ui']['test_pass'][$data['years'][$year]['uwwtp']->field_uwwcodperf['und'][0]['value']].'</span><span class="custom_tooltip">From field &lt;uwwCODPerf&gt;</span></div>';
        }
        if($data['years'][$year]['uwwtp']->field_uwwtssperf['und'][0]['value']!=''){
            $perf.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('TSS').':</div> <span class="test-'.strtolower($data['years'][$year]['uwwtp']->field_uwwtssperf['und'][0]['value']).'">'. $GLOBALS['uwwtd']['ui']['test_pass'][$data['years'][$year]['uwwtp']->field_uwwtssperf['und'][0]['value']].'</span><span class="custom_tooltip">From field &lt;uwwTSSPerf&gt;</span></div>';
        }
        if($data['years'][$year]['uwwtp']->field_uwwntotperf['und'][0]['value']!=''){
            $perf.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Total Nitrogen').':</div> <span class="test-'.strtolower($data['years'][$year]['uwwtp']->field_uwwntotperf['und'][0]['value']).'">'. $GLOBALS['uwwtd']['ui']['test_pass'][$data['years'][$year]['uwwtp']->field_uwwntotperf['und'][0]['value']].'</span><span class="custom_tooltip">From field &lt;uwwNTotPerf&gt;</span></div>';
        }
        if($data['years'][$year]['uwwtp']->field_uwwptotperf['und'][0]['value']!=''){
            $perf.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Total phosphorus').':</div> <span class="test-'.strtolower($data['years'][$year]['uwwtp']->field_uwwptotperf['und'][0]['value']).'">'. $GLOBALS['uwwtd']['ui']['test_pass'][$data['years'][$year]['uwwtp']->field_uwwptotperf['und'][0]['value']].'</span><span class="custom_tooltip">From field &lt;uwwPTotPerf&gt;</span></div>';
        }
        if($data['years'][$year]['uwwtp']->field_uwwotherperf['und'][0]['value']!=''){
            $perf.= '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Other').':</div> <span class="test-'.strtolower($data['years'][$year]['uwwtp']->field_uwwotherperf['und'][0]['value']).'">'.$GLOBALS['uwwtd']['ui']['test_pass'][$data['years'][$year]['uwwtp']->field_uwwotherperf['und'][0]['value']].'</span><span class="custom_tooltip">From field &lt;uwwOtherPerf&gt;</span></div>';
        }
            
            $uwwInfo[$year] = array(
                '<b>'.$year.'</b>',
                $comp, 
                $treatment,
                $load,
                $perf
            );
            
            
            $prev_wastewatertreated = $data['years'][$year]['wastewatertreated'];
            $n_graph['data'][]=array(
                (string)$year,
                (float)$data['years'][$year]['n']['in'],
                (float)$data['years'][$year]['n']['out'],
            );
            $n_table[$year] = array(
                '<b>'.$year.'</b>',
                uwwtd_format_number($data['years'][$year]['n']['in']) .' T/y',
                uwwtd_format_number($data['years'][$year]['n']['out']) .' T/y',
                ($data['years'][$year]['n']['out'] && $data['years'][$year]['n']['in']?uwwtd_format_number((1- $data['years'][$year]['n']['out']/$data['years'][$year]['n']['in'])*100, 1 ).' %':'-')
            );
            $p_graph['data'][]=array(
                (string)$year,
                (float)$data['years'][$year]['p']['in'],
                (float)$data['years'][$year]['p']['out'],
            );
            $p_table[$year] = array(
                '<b>'.$year.'</b>',
                uwwtd_format_number($data['years'][$year]['p']['in']) .' T/y',
                uwwtd_format_number($data['years'][$year]['p']['out']) .' T/y',
                ($data['years'][$year]['p']['out'] && $data['years'][$year]['p']['in']?uwwtd_format_number((1- $data['years'][$year]['p']['out']/$data['years'][$year]['p']['in'])*100, 1 ).' %':'-')
            );
            
            $cod_graph['data'][]=array(
                (string)$year,
                (float)$data['years'][$year]['cod']['in'],
                (float)$data['years'][$year]['cod']['out'],
            );
            $cod_table[$year] = array(
                '<b>'.$year.'</b>',
                uwwtd_format_number($data['years'][$year]['cod']['in']) .' T/y',
                uwwtd_format_number($data['years'][$year]['cod']['out']) .' T/y',
                ($data['years'][$year]['cod']['out'] && $data['years'][$year]['cod']['in']?uwwtd_format_number((1- $data['years'][$year]['cod']['out']/$data['years'][$year]['cod']['in'])*100, 1 ).' %':'-')
            );
            
            $bod_graph['data'][]=array(
                (string)$year,
                (float)$data['years'][$year]['bod']['in'],
                (float)$data['years'][$year]['bod']['out'],
            );
            $bod_table[$year] = array(
                '<b>'.$year.'</b>',
                uwwtd_format_number($data['years'][$year]['bod']['in']) .' T/y',
                uwwtd_format_number($data['years'][$year]['bod']['out']) .' T/y',
                ($data['years'][$year]['bod']['out'] && $data['years'][$year]['bod']['in']?uwwtd_format_number((1- $data['years'][$year]['bod']['out']/$data['years'][$year]['bod']['in'])*100, 1 ).' %':'-')
            );
            //Agglomeration 
            $aggInfo[$year] = array(
                '<b>'.$year.'</b>',
                uwwtd_format_number($data['years'][$year]['physicalcapacityactivity']) .'p.e.',
                uwwtd_format_number($data['years'][$year]['loadentering']) .'p.e.',
                uwwtd_format_number(array_sum(array_column($data['years'][$year]['aggs'], 'entering_load_pe'))) .'p.e.',
                theme('item_list', array('items'=>array_column($data['years'][$year]['aggs'], 'link'))),
            );
            $agg_list[$year] = array_column($data['years'][$year]['aggs'], 'inspireidlocalid');
            
            //DCP
            $dcpInfo[$year] = array(
                 '<b>'.$year.'</b>',
                theme('item_list', array('items'=>array_column($data['years'][$year]['dcps'], 'link'))),
            );
            $dcp_list[$year] = array_column($data['years'][$year]['dcps'], 'inspireidlocalid');
        }
    } 

    //Line 1  : Description
    $html .='<div class="rows">';
    $html .='<h2>'.t('Description').'</h2>';
        $html .=theme('table', [
            'rows'=>$uwwInfo,
            'header'=>[t('Year'), t('Compliance'), t('Treatment type'),t('Load'), t('Treatment performance')]
        ]);
    $html .='</div>';
    //Line 2  : entering load / physicalcapacityactivity | Waste water treated
    $rowTitle = t("Entering load and design capacity");
    $html = getHtmlPlusHtmlChartsPies($html, $rowTitle,
    'Entering load vs Physical Capacity (p.e.)', 'el-pc', $elpc_graph, theme('table', array('rows'=>$elpc_table, 'header'=>array(t('Year'), t('Entering'), t('Capacity'), t('Load rate')))), 
    'Waste water treated (m3/y)', 'waste-water-treated', $wwt_graph, theme('table', array('rows'=>$wwt_table,'header'=>array(t('Year'), t('Waste water treated'), t('Delta')))),
    false, $years);
    
    //Line 3 : BOD | COD 
    $rowTitle = t("COD & BOD");
    $html = getHtmlPlusHtmlChartsPies($html, $rowTitle,
    'COD load entering and discharged (Tons per year)', 'coded', $cod_graph, theme('table', array('rows'=>$cod_table,'header'=>array(t('Year'), t('Entering'), t('Discharged'), t('Performance rate')))), 
    'BOD load entering and discharged (Tons per year)', 'boded', $bod_graph, theme('table', array('rows'=>$bod_table,'header'=>array(t('Year'), t('Entering'), t('Discharged'), t('Performance rate')))),
    false, $years);
    
    //Line 4 : Nitrogen | Phosphorus
    $rowTitle = t("Nitrogen & Phosphorus");
    $html = getHtmlPlusHtmlChartsPies($html, $rowTitle,
    'Nitrogen load entering and discharged (Tons per year)', 'ned', $n_graph, theme('table', array('rows'=>$n_table,'header'=>array(t('Year'), t('Entering'), t('Discharged'), t('Performance rate')))), 
    'Phosphorus load entering and discharged (Tons per year)', 'ped', $p_graph, theme('table', array('rows'=>$p_table,'header'=>array(t('Year'), t('Entering'), t('Discharged'), t('Performance rate')))),
    false, $years);
    
    //Line 5 : List of Agg 
    $html .='<div class="rows">';
    $html .='<h2>'.t('Agglomerations').'</h2>';
        $html .=uwwtd_compare_idlist($agg_list);
        $html .=theme('table', [
            'rows'=>$aggInfo,
            'header'=>[
                t('Year'), 
                '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('UWWTP capacity') .'</div><span class="custom_tooltip">Value define in &lt;uwwCapacity&gt; field for the UWWTP in the report file with definition : "Organic design capacity (p.e.)"</span></div>', 
                '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('UWWTP Entering load').'</div><span class="custom_tooltip">Value define in &lt;uwwLoadEnteringUWWTP&gt; field for the UWWTP in the report file with definition : "Load entering UWWTP (p.e.)"</span></div>', 
                '<div class="field custom_tooltip_wrapper"><div class="field-label">'.t('Total of load collected by collecting systems').'</div><span class="custom_tooltip">Sum of load adressed to UWWTP by linked agglomeration. The load addressed by an agglomeration to an UWWTP is calculated with the following formula : <i>Generated load (p.e.) of the agglomeration (&lt;aggGenerated&gt;)</i> x <i>Rate of generated load of agglomeration collected through collecting system (% of p.e.) (&lt;aggC1&gt;) </i> x <i>% of load of agglomeration collected in collecting system and entering that treatmentplant (&lt;aucPercEnteringUWWTP&gt;) </i></span></div>',
                t('Linked Agglomeration(s) with addressed load to the UWWTP')
            ]
        ]);
        $html .='<div class="description"><u>Notice</u> : "UWWTP entering load" is based on measure of yearly average, "Total of load collected" is based on estimation of maximum over a year.</div>';
    
    $html .='</div>';
    
    //Line 6 : List of DCP ==> RCA
    
    $html .='<div class="rows">';
    $html .='<h2>'.t('Discharge points').'</h2>';
        //dsm($uwwtp_list);
        $html .=uwwtd_compare_idlist($dcp_list);
        //Mettre un message s'il y a un nombre différent et ou des id qui change au fil du temps
        $html .=theme('table', [
            'rows'=>$dcpInfo,
            'header'=>[t('Year'), t('Linked Discharge point(s)')]
        ]);
        
    $html .='</div>';
    
   
    
    
    
    return '<article>'.$html.'</article>';
}
