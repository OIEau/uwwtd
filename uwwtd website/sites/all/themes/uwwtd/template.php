<?php
/**
 * @file
 * template.php
 */

function uwwtd_adjustBrightness($hex, $steps){
    // Steps should be between -255 and 255. Negative = darker, positive = lighter
    $steps = max(-255, min(255, $steps));

    // Format the hex color string
    $hex = str_replace('#', '', $hex);
    if (strlen($hex) == 3) {
        $hex = str_repeat(substr($hex,0,1), 2).str_repeat(substr($hex,1,1), 2).str_repeat(substr($hex,2,1), 2);
    }

    // Get decimal values
    $r = hexdec(substr($hex,0,2));
    $g = hexdec(substr($hex,2,2));
    $b = hexdec(substr($hex,4,2));

    // Adjust number of steps and keep it inside 0 to 255
    $r = max(0,min(255,$r + $steps));
    $g = max(0,min(255,$g + $steps));  
    $b = max(0,min(255,$b + $steps));

    $r_hex = str_pad(dechex($r), 2, '0', STR_PAD_LEFT);
    $g_hex = str_pad(dechex($g), 2, '0', STR_PAD_LEFT);
    $b_hex = str_pad(dechex($b), 2, '0', STR_PAD_LEFT);

    return '#'.$r_hex.$g_hex.$b_hex;
}

function uwwtd_check_history($id, $annee){
	//Get all the nodes with the same id
	$query = db_select('field_data_field_inspireidlocalid', 'n');
	$query->fields('n', array('entity_id'));
	$query->condition('n.field_inspireidlocalid_value', $id, '=');
	$ids = array();
	$results = $query->execute();
	foreach($results as $result){
		$node = node_load($result->entity_id);		
		$ids[] = $node->nid;
	}

	if(count($ids) == 0) return false;
	else return $ids;
}

function uwwtd_preprocess_field(&$variables){

	// For discharge point values
	if (
		$variables['element']['#field_name'] == 'field_rcatype' ||
		$variables['element']['#field_name'] == 'field_dcpwaterbodytype'
	){
		// rcatype
		if($variables['element']['#items']['0']['value'] == 'NA') $variables['items']['0']['#markup'] = t('Normal Area');
		if($variables['element']['#items']['0']['value'] == 'SA') $variables['items']['0']['#markup'] = t('Sensitive Area');
		if($variables['element']['#items']['0']['value'] == 'CSA') $variables['items']['0']['#markup'] = t('Catchment sensitive area');
		if($variables['element']['#items']['0']['value'] == 'LSA') $variables['items']['0']['#markup'] = t('Less sensititve area');

		// waterbody
		if($variables['element']['#items']['0']['value'] == 'ES') $variables['items']['0']['#markup'] = t('Estuary');
		if($variables['element']['#items']['0']['value'] == 'CW') $variables['items']['0']['#markup'] = t('Coastal waters');
		if($variables['element']['#items']['0']['value'] == 'FW') $variables['items']['0']['#markup'] = t('Freshwater');
	}

	// For booleans
	if (
		$variables['element']['#field_name'] == 'field_uwwprimarytreatment' ||
		$variables['element']['#field_name'] == 'field_uwwsecondarytreatment' ||
		$variables['element']['#field_name'] == 'field_uwwpremoval' ||
		$variables['element']['#field_name'] == 'field_uwwnremoval' ||
		$variables['element']['#field_name'] == 'field_uwwuv' ||
		$variables['element']['#field_name'] == 'field_uwwchlorination' ||
		$variables['element']['#field_name'] == 'field_uwwozonation' ||
		$variables['element']['#field_name'] == 'field_uwwsandfiltration' ||
		$variables['element']['#field_name'] == 'field_uwwmicrofiltration' ||
		$variables['element']['#field_name'] == 'field_uwwothertreat' ||
		$variables['element']['#field_name'] == 'field_uwwaccidents' ||
		$variables['element']['#field_name'] == 'field_dcpsurfacewaters' ||
		$variables['element']['#field_name'] == 'field_aggcritb' ||
		$variables['element']['#field_name'] == 'field_aggcritca' ||
		$variables['element']['#field_name'] == 'field_aggcritcb' ||
		$variables['element']['#field_name'] == 'field_aggchanges' ||
		$variables['element']['#field_name'] == 'field_aggbesttechnicalknowledge' ||
		$variables['element']['#field_name'] == 'field_agghaveregistrationsystem' ||
		$variables['element']['#field_name'] == 'field_aggexistmaintenanceplan' ||
		$variables['element']['#field_name'] == 'field_aggpressuretest' ||
		$variables['element']['#field_name'] == 'field_aggvideoinspections' ||
		$variables['element']['#field_name'] == 'field_aggothermeasures' ||
		$variables['element']['#field_name'] == 'field_aggaccoverflows' ||
		$variables['element']['#field_name'] == 'field_aggcapacity' ||
		$variables['element']['#field_name'] == 'field_agg_dilution_rates' ||
		$variables['element']['#field_name'] == 'field_rcaanitro' ||
		$variables['element']['#field_name'] == 'field_rcaaphos' ||
		$variables['element']['#field_name'] == 'field_rcab' ||
		$variables['element']['#field_name'] == 'field_rcac' ||
		$variables['element']['#field_name'] == 'field_rcamorphology' ||
		$variables['element']['#field_name'] == 'field_rcahydrologie' ||
		$variables['element']['#field_name'] == 'field_rcahydraulic' ||
		$variables['element']['#field_name'] == 'field_rcaabsencerisk' ||
		$variables['element']['#field_name'] == 'field_rca54applied' ||
		$variables['element']['#field_name'] == 'field_rca_parameter_n' ||
		$variables['element']['#field_name'] == 'field_rca_parameter_p' ||
		$variables['element']['#field_name'] == 'field_rca_parameter_other' ||
		$variables['element']['#field_name'] == 'field_rca52applied' ||
		$variables['element']['#field_name'] == 'field_rca58applied'
		

	){
		//check for custom tooltips
		$tt = '';
		$field_info = field_info_field($variables['element']['#field_name']);
		if(isset($field_info['custom_tooltip']) && $field_info['custom_tooltip'] !== ''){
			$tt = $variables['items'][0]['#markup'];
		}
		
		if($variables['element']['#items']['0']['value'] === '1' || $variables['element']['#items']['0']['value'] === 'P'){
			$variables['items']['0']['#markup'] = $tt.'<img style="position: relative; top: -2px; margin-left: 5px;" height="10px" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/tick.png" />';
		}
		elseif($variables['element']['#items']['0']['value'] === '0' || $variables['element']['#items']['0']['value'] === 'F'){
			$variables['items']['0']['#markup'] = $tt.'<img style="position: relative; top: -2px; margin-left: 5px;" height="10px" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/cross.png" />';
		}
	}

	// For percentage '%'
	if (
        
		$variables['element']['#field_name'] == 'field_aggc1' ||
		$variables['element']['#field_name'] == 'field_aggc2' ||
		$variables['element']['#field_name'] == 'field_aggpercwithouttreatment' ||
		$variables['element']['#field_name'] == 'field_aggpercprimtreatment' ||
		$variables['element']['#field_name'] == 'field_aggpercsectreatment' ||
		$variables['element']['#field_name'] == 'field_aggpercstringenttreatment'
		
		
	){
        if ($variables['items']['0']['#markup'] != '<p>'.t('Not provided').'</p>') {
		  $variables['items']['0']['#markup'] = $variables['items']['0']['#markup'].' %';
        }
//         dsm($variables);
	}
    
//     if ($variables['element']['#field_name'] == 'field_aggc1' ||
// 		$variables['element']['#field_name'] == 'field_aggc2' ||
// 		$variables['element']['#field_name'] == 'field_aggpercwithouttreatment') {
//         $pe = $variables['element']['#object']->field_agggenerated['und'][0]['value'] * $variables['element']['#object']->{$variables['element']['#field_name']}['und'][0]['value']/ 100;
//         $variables['items']['0']['#markup'] .= ' (' . $pe .')';
//     }

	// For compliance colors
	if (
		$variables['element']['#field_name'] == 'field_aggart3compliance' ||
		$variables['element']['#field_name'] == 'field_aggart4compliance' ||
		$variables['element']['#field_name'] == 'field_aggart5compliance' ||
		$variables['element']['#field_name'] == 'field_aggart6compliance' ||
		$variables['element']['#field_name'] == 'field_aggcompliance' ||
		$variables['element']['#field_name'] == 'field_uwwcompliance'
	){
		if($variables['element']['#items']['0']['value'] == 'C') $spanclass ='c';
		if($variables['element']['#items']['0']['value'] == 'NC') $spanclass ='nc';
		if($variables['element']['#items']['0']['value'] == 'AddQC') $spanclass ='nc';
		if($variables['element']['#items']['0']['value'] == 'QC') $spanclass ='c';
		if($variables['element']['#items']['0']['value'] == 'NR') $spanclass ='nr';
		if($variables['element']['#items']['0']['value'] == 'NI') $spanclass ='ni';
		if($variables['element']['#items']['0']['value'] == 'CE') $spanclass ='ce';
		if($variables['element']['#items']['0']['value'] == 'RNC') $spanclass ='c';

		$variables['items']['0']['#markup'] = '<span class="'.$spanclass.'">'.$variables['items']['0']['#markup'].'</span>';
	}
}

function uwwtd_timeline_output($node){
	$output = '';
	$histories = uwwtd_check_history($node->field_inspireidlocalid['und'][0]['value'], $node->field_anneedata['und'][0]['value']);
	if($histories != false){
		$otherList = array();
		$output .= '<div class="uwwtd-history">';
		$output .= '<fieldset class="uwwtd-history field-group-fieldset panel panel-default form-wrapper">
		<legend class="panel-heading">
		<div class="panel-title fieldset-legend">'.t('Compliance timeline').'</div>
		</legend>';
		foreach($histories as $history){
			$other = node_load($history);
			$otherY = $other->field_anneedata['und'][0]['value'];
			$otherList[$otherY] = array('node'=>$other);
		}
		ksort($otherList);
		foreach($otherList as $other){
			$ting = $other['node'];
			if($node->type == 'agglomeration') $val = $ting->field_aggcompliance['und'][0]['value'];
			if($node->type == 'uwwtp') $val = $ting->field_uwwcompliance['und'][0]['value'];

			//default colors
			$color = '#6b6b6b'; $borderc = '#6b5e66';

			if(isset($val)){
				if($val == 'C') {$color = '#4f91e1'; $borderc = '#4faaf9'; $txtc = '#ffffff'; $bulleTxt = t('Compliant');}
				if($val == 'NC') {$color = '#d93c3c'; $borderc = '#d91a10'; $txtc = '#ffffff'; $bulleTxt = t('Not compliant');}
				if($val == 'NR') {$color = '#a2a2a2'; $borderc = '#a6a2a2'; $txtc = '#ffffff'; $bulleTxt = t('Not relevant');}
				if($val == 'NI') {$color = '#6b6b6b'; $borderc = '#6b5e66'; $txtc = '#ffffff'; $bulleTxt = t('No information');}
       			if($val == 'CE') {$color = '#ea8b2e'; $borderc = '#ea7810'; $txtc = '#ffffff'; $bulleTxt = t('Compliant on equipment');}
                if($val == '?') {$color = '#ea8b2e'; $borderc = '#ea7810'; $txtc = '#ffffff'; $bulleTxt = t('?');}
			}

			$color = uwwtd_adjustBrightness($color, 50);

			if($ting->field_anneedata['und'][0]['value'] == $node->field_anneedata['und'][0]['value']){
				$output .= '<div class="uwwtd-history-element uwwtd-history-element-current">';
				$output .= '<h4>'.$ting->field_anneedata['und'][0]['value'].'</h4>';
				if(isset($val)) $output .= '<span class="current" style="background-color: '.$color.'; border-color: '.$borderc.'; color: '.$txtc.'" title="'.$bulleTxt.'">'.$bulleTxt.'</span>';
				$output .= '</div>';
			}
			else{
				$output .= '<div class="uwwtd-history-element">';
				$output .= '<h4>'.l($ting->field_anneedata['und'][0]['value'], 'node/'.$ting->nid).'</h4>';
				if(isset($val)) $output .= '<span style="background-color: '.$color.'; border-color: '.$borderc.'; color: '.$txtc.'" title="'.$bulleTxt.'">'.$val.'</span>';
				$output .= '</div>';
			}
		}
		$output .= '</fieldset></div><br>';
	}

	return $output;
}

function uwwtd_field_attach_view_alter(&$output, $context){
	if ($context['entity_type'] != 'node' || $context['view_mode'] != 'full') {
		return;
	}

	$node = $context['entity'];
	// Load all instances of the fields for the node.
	$instances = _field_invoke_get_instances('node', $node->type, array('default' => TRUE, 'deleted' => FALSE));
	//dsm($instances);
 
	foreach($instances as $field_name => $instance){
		// Set content for fields they are empty.
		if(empty($node->{$field_name})){
			$display = field_get_display($instance, 'full', $node);
			// Do not add field that is hidden in current display.
			if($display['type'] == 'hidden'){
				continue;
			}
			// Load field settings.
			$field = field_info_field($field_name);

			// test for dates
			//dsm($field['type']);
			if($field['type'] === 'date'){
				if(empty($output[$field_name]['#items'])){
					$output[$field_name] = array(
						'#theme' => 'field',
						'#title' => $instance['label'],
						'#label_display' => 'inline',
						'#field_type' => $field['type'],
						'#field_name' => $field_name,
						'#bundle' => $node->type,
						'#object' => $node,
						'#items' => array(0 => array('value' => '')),
						'#entity_type' => 'node',
						'#weight' => $display['weight'],
						0 => array('#markup' => '<p>'.t('-').'</p>'),
					);
					//dsm($output);
				}
			}

			//dsm($field);
			if(empty($output[$field_name]['#items'])){
				$output[$field_name] = array(
					'#theme' => 'field',
					'#title' => $instance['label'],
					'#label_display' => 'inline',
					'#field_type' => $field['type'],
					'#field_name' => $field_name,
					'#bundle' => $node->type,
					'#object' => $node,
					'#items' => array(0 => array('value' => '')),
					'#entity_type' => 'node',
					'#weight' => $display['weight'],
					0 => array('#markup' => '<p>'.t('Not provided').'</p>'),
				);
			}
			//dsm($field);
		}
	}
}

function uwwtd_preprocess_node(&$vars){
    if($vars["is_front"]){
       $vars["theme_hook_suggestions"][] = "node__front";
    }  
}

function uwwtd_render_article17($nid){
	$art17 = node_load($nid);

	$output = '';
	//field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
	if(isset($art17->field_art_17_reason['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_reason', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_mesures_forseen['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_mesures_forseen', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_reason_uwwtp['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_reason_uwwtp', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_expected_completion['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_expected_completion', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_expected_start['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_expected_start', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_expected_system['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_expected_system', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_forcast_investment['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_forcast_investment', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_eu_fund['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_eu_fund', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_eu_fund_amount['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_eu_fund_amount', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_relative_comments['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_relative_comments', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_load_expected['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_load_expected', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_capacity_planned['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_capacity_planned', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_treatment_planned['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_treatment_planned', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_expected_date_comp['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_expected_date_comp', array('display' => 'inline'));
		$output .= render($champ);
	}

	if(isset($art17->field_art_17_expected_comp_uwwtp['und'][0]['value'])){
		$champ = field_view_field('node', $art17, 'field_art_17_expected_comp_uwwtp', array('display' => 'inline'));
		$output .= render($champ);
	}

	return $output;
}

function uwwtd_insert_errors_tab($node){
    global $user;
    $admin = 'administrator';
    $editor = 'editor';
    $roles = $user->roles;
    //dsm($roles);
    $bFound = false;
    foreach($roles as $role){
        if($role == $admin || $role == $editor){
            $bFound = true;
            break; 
        }    
    }
    if ($bFound === false) {
        return '';
    }

	$values = array();

// 	// Get list of all elements
// 	$raws = $node->field_uwwtd_error_link['und'];
// 	$all = array();
// 	foreach($raws as $raw){
// 		$all[] = $raw['nid'];
// 	}
// 
// 	// group by import
// 	foreach($all as $single){
// 		$error = node_load($single);
// 		if($error->nid !== null){
// 			$id = $error->field_uwwtd_err_identifier['und'][0]['value'];
// 			$type = $error->field_uwwtd_err_type['und'][0]['value'];
// 			$cat = $error->field_uwwtd_err_category['und'][0]['value'];
// 
// 			// Set values
// 			$values[$id][$type][$cat][] = array(
// 				'id' => $id,
// 				'timestamp' => $error->field_uwwtd_err_timestamp['und'][0]['value'],
// 				'time' => $error->field_uwwtd_err_time['und'][0]['value'],
// 				'message' => $error->field_uwwtd_err_message['und'][0]['value'],
// 				'type' => $type,
// 				'category' => $error->field_uwwtd_err_category['und'][0]['value'],
// 				'nid' => $error->nid
// 			);
// 		}
// 	}
                      
    $query = 'select errid as id, 
                    type, category, 
                    error as message, 
                    date as timestamp, 
                    errid as nid  
        from {uwwtd_import_errors} 
        where year = :year 
        and entity_type = :entity_type 
        and entity_id = :entity_id';
    
    $param = array(
        ':year' => $node->field_anneedata['und'][0]['value'],
        ':entity_type' => 'node',
        ':entity_id' => $node->nid,        
    );                        
    $result = db_query($query, $param);
    while($row = $result->fetchAssoc()) {
        $values[ $row['id'] ][ $row['type'] ][ $row['category'] ][] = array(
        	'id' => $row['id'],
        	'timestamp' => $row['timestamp'],
        	'message' => $row['message'],
        	'type' => $row['type'],
        	'category' => $row['category'],
        	'nid' => $row['nid'],
        );
    }    

	if(empty($values)) return false;

	// get overall total errors for node
	$overallTotal = 0;
	foreach($values as $value){
		foreach($value as $type){
			$overallTotal = count($type['0'])+ count($type['1']) + count($type['2'])+ count($type['3']) + count($type['4']) +$overallTotal;
		}
	}
	if($overallTotal === 0) $overallTotal = null;

	// Start output
	$output = '<div class="uwwtd_errors_tab">';
		$output .= '<h3><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_total">'.$overallTotal.'</span></span> Errors</h3>';
		$count = 0 ;
		foreach($values as $value){
			$totalW = 0;$totalWI = 0;$totalWL = 0;$totalWG = 0;$totalWC = 0;$totalWF = 0;
			$totalN = 0;$totalNI = 0;$totalNL = 0;$totalNG = 0;$totalNC = 0;$totalNF = 0;
			$totalE = 0;$totalEI = 0;$totalEL = 0;$totalEG = 0;$totalEC = 0;$totalEF = 0;
			$total= 0;
			// get Category
// 			$field = field_info_field('field_uwwtd_err_category');
// 			$label = $field['settings']['allowed_values'];
				
			// get date
			//type erreur warning
			if(isset($value['1']))
			{	
				if(isset($value['1']['0']))
				{
					foreach ($value['1']['0'] as $categories){
						$labelWI = 'Warning Input';$totalWI = count($value['1']['0']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['1']['1']))
				{
					foreach ($value['1']['1'] as $categories){
						$labelWL = 'Warning Linking';$totalWL = count($value['1']['1']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['1']['2']))
				{
					foreach ($value['1']['2'] as $categories){
						$labelWG = 'Warning Geometry';$totalWG = count($value['1']['2']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['1']['3']))
				{
					foreach ($value['1']['3'] as $categories){
						$labelWC = 'Warning Conformity';$totalWC = count($value['1']['3']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['1']['4']))
				{
					foreach ($value['1']['4'] as $categories){
						$labelWF = 'Warning Format';$totalWF = count($value['1']['4']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				$totalW = $totalWI + $totalWL + $totalWG + $totalWC + $totalWF;	
			}
			//type erreur notification
			if(isset($value['0']))
			{
				if(isset($value['0']['0']))
				{
					foreach ($value['0']['0'] as $categories){
						$labelNI = 'Notification Input';$totalNI = count($value['0']['0']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['0']['1']))
				{
					foreach ($value['0']['1'] as $categories){
						$labelNL = 'Notification Linking';$totalNL = count($value['0']['1']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['0']['2']))
				{
					foreach ($value['0']['2'] as $categories){
						$labelNG = 'Notification Geometry';$totalNG = count($value['0']['2']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['0']['3']))
				{
					foreach ($value['0']['3'] as $categories){
						$labelNC = 'Notification Conformity';$totalNC = count($value['0']['3']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['0']['4']))
				{
					foreach ($value['0']['4'] as $categories){
						$labelNF = 'Notification Format';$totalNF = count($value['0']['4']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				$totalN = $totalNI + $totalNL + $totalNG + $totalNC + $totalNF;
			}
			//type erreur error
			if(isset($value['2']))
			{
				if(isset($value['2']['0']))
				{
					foreach ($value['2']['0'] as $categories){
						$labelEI = 'Error Input';$totalEI = count($value['2']['0']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['2']['1']))
				{
					foreach ($value['2']['1'] as $categories){
						$labelEL = 'Error Linking';$totalEL = count($value['2']['1']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['2']['2']))
				{
					foreach ($value['2']['2'] as $categories){
						$labelEG = 'Error Geometry';$totalEG = count($value['2']['2']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['2']['3']))
				{
					foreach ($value['2']['3'] as $categories){
						$labelEC = 'Error Conformity';$totalEC = count($value['2']['3']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				if(isset($value['2']['4']))
				{
					foreach ($value['2']['4'] as $categories){
						$labelEF = 'Error Format';$totalEF = count($value['2']['4']);
						//$time = $categories['timestamp'];
						$date =date('d-m-Y H:i:s', $categories['timestamp']);
					}
				}
				$totalE = $totalEI + $totalEL + $totalEG + $totalEC + $totalEF;
			}
			$total = $totalW + $totalN + $totalE;
			if($total === 0) $total = null;
			$output .= '<div class="uwwtd_errors_tab_header">';
				$output .= '<h4><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_total">'.$total.'</span></span><span class="uwwtd_errors_tab_header_title"> Imported at : '.$date.'</span></h4>';
				
				if(isset($value['0'])){
					$output .= '<div class="uwwtd_errors_tab_type_header">';
					
					//catégorie notification input
					if(isset($value['0']['0'])){
						$output .= '<div class="uwwtd_error_Input">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalNI">'.$totalNI.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelNI.':</span></h5>';	
						foreach($value['0']['0'] as $error){
							//dsm($error);
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_notification.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						//$totalI = 0;
					}
					if(isset($value['0']['1']))
					{//catégorie notification Linking
						$output .= '<div class="uwwtd_error_Linking">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalNL">'.$totalNL.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelNL.':</span></h5>';	
						foreach($value['0']['1'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_notification.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalL = 0;
					}
					if(isset($value['0']['2']))
					{//catégorie notification Geometry
						$output .= '<div class="uwwtd_error_Geometry">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalNG">'.$totalNG.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelNG.':</span></h5>';	
						foreach($value['0']['2'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_notification.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalG = 0;
					}
					if(isset($value['0']['3']))
					{//catégorie notification Conformity
						$output .= '<div class="uwwtd_error_Conformity">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalNC">'.$totalNC.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelNC.':</span></h5>';	
						foreach($value['0']['3'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_notification.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalC = 0;
					}
					if(isset($value['0']['4']))
					{//catégorie notification Format
						$output .= '<div class="uwwtd_error_Format">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalNF">'.$totalNF.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelNF.':</span></h5>';	
						foreach($value['0']['4'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_notification.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalF = 0;
					}
					$output .= '</div>';
					
				}
				if(isset($value['1'])){
					$output .= '<div class="uwwtd_errors_tab_type_header">';
					
					//catégorie warning input
					if(isset($value['1']['0'])){
						$output .= '<div class="uwwtd_error_Input">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalWI">'.$totalWI.'</span></span><span class="uwwtd_errors_tab_header_titleWI">'.$labelWI.':</span></h5>';	
						foreach($value['1']['0'] as $error){
							//dsm($error);
							$output .= '<div class="uwwtd_error errorWI">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_warning.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWI.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalI = 0;
					}
					if(isset($value['1']['1']))
					{//catégorie warning Linking
						$output .= '<div class="uwwtd_error_Linking">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalWL">'.$totalWL.'</span></span><span class="uwwtd_errors_tab_header_titleWL">'.$labelWL.':</span></h5>';	
						foreach($value['1']['1'] as $error){
							$output .= '<div class="uwwtd_error errorWL">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_warning.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWL.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalL = 0;
					}
					if(isset($value['1']['2']))
					{//catégorie warning Geometry
						$output .= '<div class="uwwtd_error_Geometry">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalWG">'.$totalWG.'</span></span><span class="uwwtd_errors_tab_header_titleWG">'.$labelWG.':</span></h5>';	
						foreach($value['1']['2'] as $error){
							$output .= '<div class="uwwtd_error errorWG">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_warning.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWG.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalG = 0;
					}
					if(isset($value['1']['3']))
					{//catégorie warning Conformity
						$output .= '<div class="uwwtd_error_Conformity">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalWC">'.$totalWC.'</span></span><span class="uwwtd_errors_tab_header_titleWC">'.$labelWC.':</span></h5>';	
						foreach($value['1']['3'] as $error){
							$output .= '<div class="uwwtd_error errorWC">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_warning.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWC.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalC = 0;
					}
					if(isset($value['1']['4']))
					{//catégorie warning Format
						$output .= '<div class="uwwtd_error_Format">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalWF">'.$totalWF.'</span></span><span class="uwwtd_errors_tab_header_titleWF">'.$labelWF.':</span></h5>';	
						foreach($value['1']['4'] as $error){
							$output .= '<div class="uwwtd_error errorWF">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_warning.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWF.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalF = 0;
					}
					$output .= '</div>';
				}
				if(isset($value['2'])){
					$output .= '<div class="uwwtd_errors_tab_type_header">';
					//catégorie error input
					if(isset($value['2']['0'])){
						$output .= '<div class="uwwtd_error_Input">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalEI">'.$totalEI.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelEI.':</span></h5>';	
						foreach($value['2']['0'] as $error){
							//dsm($error);
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_error.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalI = 0;
					}
					if(isset($value['2']['1']))
					{//catégorie error Linking
						$output .= '<div class="uwwtd_error_Linking">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalEL">'.$totalEL.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelEL.':</span></h5>';	
						foreach($value['2']['1'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_error.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalL = 0;
					}
					if(isset($value['2']['2']))
					{//catégorie error Geometry
						$output .= '<div class="uwwtd_error_Geometry">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalEG">'.$totalEG.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelEG.':</span></h5>';	
						foreach($value['2']['2'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_error.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalG = 0;
					}
					if(isset($value['2']['3']))
					{//catégorie error Conformity
						$output .= '<div class="uwwtd_error_Conformity">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalEC">'.$totalEC.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelEC.':</span></h5>';	
						foreach($value['2']['3'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_error.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalC = 0;
					}
					if(isset($value['2']['4']))
					{//catégorie error Format
						$output .= '<div class="uwwtd_error_Format">';
							$output .= '<h5><span class="uwwtd_tab_header_total_wrapper"><span class="uwwtd_tab_header_totalEF">'.$totalEF.'</span></span><span class="uwwtd_errors_tab_header_title">'.$labelEF.':</span></h5>';	
						foreach($value['2']['4'] as $error){
							$output .= '<div class="uwwtd_error">';
								$output .= '<ul>';
									$output .= '<li><img src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_error.png" /></li>';
									$output .= '<li>'.$error['message'].'</li>';
// 									$output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
								$output .= '</ul>';
							$output .= '</div>';							
						}$output .= '</div>';
						$totalF = 0;
					}
					$output .= '</div>';
				}
			$output .= '</div>';
		}
	$output .= '</div>';

	return $output;
}

function uwwtd_get_uww_graphic($node){
	global $base_url;
	$src = $base_url . '/' . drupal_get_path('theme', 'uwwtd');

	$nbAgglos = 0;
	$reseau = array();
	$reseau['nid'] = $node->nid;
	$reseau['title'] = $node->title;
	$reseau['id'] = $node->field_siteid['und'][0]['value'];
	$reseau['compliance'] = $node->field_uwwcompliance['und'][0]['value'];
	$reseau['load'] = $node->field_uwwloadenteringuwwtp['und'][0]['value'];

	$reseau['primary'] = array(
		'treatment' => $node->field_uwwprimarytreatment['und'][0]['value']
	);

	$reseau['secondary'] = array(
		'treatment' => $node->field_uwwsecondarytreatment['und'][0]['value']
	);

	$reseau['n-removal'] = array(
		'treatment' => $node->field_uwwnremoval['und'][0]['value'],
		'performance' => $node->field_uwwntotperf['und'][0]['value']
	);

	$reseau['p-removal'] = array(
		'treatment' => $node->field_uwwpremoval['und'][0]['value'],
		'performance' => $node->field_uwwptotperf['und'][0]['value']
	);

	$reseau['uv'] = array(
		'treatment' => $node->field_uwwuv['und'][0]['value'],
		'performance' => $node->field_uwwotherperf['und'][0]['value']
	);

	$reseau['micro'] = array(
		'treatment' => $node->uwwmicrofiltration['und'][0]['value'],
		'performance' => $node->field_uwwotherperf['und'][0]['value']
	);

	$reseau['chlorination'] = array(
		'treatment' => $node->field_uwwchlorination['und'][0]['value'],
		'performance' => $node->field_uwwotherperf['und'][0]['value']
	);

	$reseau['ozonation'] = array(
		'treatment' => $node->field_uwwozonation['und'][0]['value'],
		'performance' => $node->field_uwwotherperf['und'][0]['value']
	);

	$reseau['other'] = array(
		'treatment' => $node->field_uwwothertreat['und'][0]['value'],
		'performance' => $node->field_uwwotherperf['und'][0]['value']
	);

	$reseau['sand'] = array(
		'treatment' => $node->field_uwwsandfiltration['und'][0]['value'],
		'performance' => $node->field_uwwotherperf['und'][0]['value']
	);

	$reseau['agglos'] = array();
	foreach($node->field_linked_agglomerations['und'] as $aggs){
		$nbAgglos++;
		$agg = node_load($aggs['nid']);
		$reseau['agglos'][$agg->nid]['nid'] = $agg->nid;
		$reseau['agglos'][$agg->nid]['title'] = $agg->title;
		$reseau['agglos'][$agg->nid]['id'] = $agg->field_siteid['und'][0]['value'];
		$reseau['agglos'][$agg->nid]['generated'] = $agg->field_agggenerated['und'][0]['value'];
	}

	$nbDcps = 0;
	foreach($node->field_linked_discharge_points['und'] as $dcps){
		$nbDcps++;
		$dcp = node_load($dcps['nid']);
		$rca = node_load($dcp->field_linked_receiving_areas['und'][0]['nid']);
		$reseau['dcps'][$dcp->nid]['nid'] = $dcp->nid;
		$reseau['dcps'][$dcp->nid]['title'] = $dcp->title;
		$reseau['dcps'][$dcp->nid]['id'] = $dcp->field_siteid['und'][0]['value'];
		$reseau['dcps'][$dcp->nid]['rcaNid'] = $rca->nid;
		$reseau['dcps'][$dcp->nid]['rcaTitle'] = $rca->title;
		$reseau['dcps'][$dcp->nid]['rcaType'] = $dcp->field_rcatype['und'][0]['value'];
	}
// 	dsm($reseau);

	$output = '<div class="graphic-container">';

	$output .= '<div class="agglomeration-graphic">
	        <img width="270px" src="'.$src.'/images/graphic/reseau.png" alt="reseau">
	        <div class="uww-graphic-title">';

	foreach($reseau['agglos'] as $agglo){
	          $output .= ''.l($agglo['title'], "node/".$agglo['nid']).'
	          ('.$agglo['generated'].' p.e)<br>';
	}

	$output .= '</div></div>';

	$output .= '<div class="connectors">';
        $output .= '<img width="150px" src="'.$src.'/images/graphic/single.png" alt="reseau">';
    $output .= '</div>';

    $output .= '<div class="station" style="position: relative; top: -'.$offset.'px">';

	    if($reseau['compliance'] == 'C') $output .= '<img src="'.$src.'/images/graphic/station-c.png" alt="reseau">';
	    elseif($reseau['compliance'] == 'NC') $output .= '<img src="'.$src.'/images/graphic/station-nc.png" alt="reseau">';
	    else $output .= '<img src="'.$src.'/images/graphic/station.png" alt="reseau">';
	  
		$output .= '<div class="graphic-title">
			'.l($reseau['title'], "node/".$reseau['nid']).'
		</div>
		<div class="station-load">'.$reseau['load'].' p.e</div>
	</div>';

	$output .= '<div class="more-wrapper">';
		if($reseau['n-removal']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['n-removal']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['n-removal']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('N').'</div>';
			$output .= '</div>';
		}

		if($reseau['p-removal']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['p-removal']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['p-removal']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('P').'</div>';
			$output .= '</div>';
		}

		if($reseau['uv']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['uv']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['uv']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('UV').'</div>';
			$output .= '</div>';
		}

		if($reseau['micro']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['micro']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['micro']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('MICRO').'</div>';
			$output .= '</div>';
		}

		if($reseau['chlorination']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['chlorination']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['chlorination']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('CHLOR').'</div>';
			$output .= '</div>';
		}

		if($reseau['ozonation']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['ozonation']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['ozonation']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('OZONE').'</div>';
			$output .= '</div>';
		}

		if($reseau['sand']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['sand']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['sand']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('SAND').'</div>';
			$output .= '</div>';
		}

		if($reseau['other']['treatment'] == '1'){
			$output .= '<div class="more">';
			if($reseau['other']['performance'] == 'P'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau">';
			}
			elseif($reseau['other']['performance'] == 'F'){
				$output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau">';
			}
			else{
				$output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau">';
			}
			$output .= '<div class="more-text">'.t('O').'</div>';
			$output .= '</div>';
		}
	$output .= '</div>';

	$offset = 0;
    $totalDcps = 0;
    $totalDcpsT = 0;
    $totalDcpsB = 0;

    $output .= '<div class="dcp-connections" style="top: -'.$offset.'px">';

	$nbDcps = count($reseau['dcps']);
	$totalDcps = $totalDcps + $nbDcps;
	$nbDcpsT = 0;
	$nbDcpsB = 0;
	for ($i=0; $i <= $nbDcps; $i++) { 

		if($i > 1){    

		  if($i % 2 == 0){
		    $nbDcpsT ++;
		    $totalDcpsT++;
		  }
		  else{
		    $nbDcpsB ++;
		    $totalDcpsB++;
		  }
		}
	}
	if($nbDcpsT == 1) $topMarge = 50;
	else $topMarge = 0 + ($nbDcpsT * 48);
	if($nbPlants > 1) $topMarge = $topMarge - 1;
	if($nbPlants == 1 && $nbDcps == 1) $topMarge = 3;
	if($topMarge < 0) $topMarge = 0;
	$topMarge = $topMarge - (2 * $topMarge);


	$output .= '<div class="station-dcp-connections">';
	$output .= '<div>';

	
	 $output .= '<img width="75px" src="'.$src.'/images/graphic/singlesmall.png" alt="reseau">';

	    $output .= '</div>
	  </div>';

    $output .= '</div>

    <div class="dcps-wrapper">';


	$output .= '<div class="station-dcp" style="top: 0px;">
		<div>
			<img width="80px" src="'.$src.'/images/graphic/dcp.png" alt="reseau">
		  	<div class="graphic-title">';
				$output .= l(count($reseau['dcps'])." DCP(S)", "#");
				$output .= '<div class="dcps-hidden-list">';
					foreach($reseau['dcps'] as $dcp) {
		    			$output .= l($dcp['title'], "node/".$dcp['nid']);
		    			$output .= '<br>';
		    			$output .= l($dcp['rcaTitle'], "node/".$dcp['rcaNid']);
		    			$output .=  '('.$dcp['rcaType'].')';
		    			$output .= '<br><br>';
					}
		  		$output .= '</div>
		  	</div>
		</div>
	</div>';


    $output .= '</div>';

	return $output;   
}

function uwwtd_get_agglo_graphic($node){
	global $base_url;
      $src = $base_url . '/' . drupal_get_path('theme', 'uwwtd');

      $totalout = floor(($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggc1['und'][0]['value']);
      $totalWOT = floor(($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggpercwithouttreatment['und'][0]['value']);
      $totalIAS = floor(($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggc2['und'][0]['value']);

      $nbPlants = 0;
      $reseau = array();
      foreach($node->field_linked_treatment_plants['und'] as $uwws){
        $nbPlants++;
        $uww = node_load($uwws['nid']);

        //get station entering sums
        $query = db_select('node', 'n');
        $query->join('field_data_field_agglo_uww_agglo', 'a', 'a.entity_id = n.nid');
        $query->join('field_data_field_agglo_uww_uww', 'u', 'u.entity_id = n.nid');
        $query->join('field_data_field_agglo_uww_perc_ent_uw', 'perce', 'perce.entity_id = n.nid');
        $query->join('field_data_field_agglo_uww_mperc_ent_uw', 'mperce', 'mperce.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->fields('perce', array('field_agglo_uww_perc_ent_uw_value'));
        $query->fields('mperce', array('field_agglo_uww_mperc_ent_uw_value'));
        $query->condition('a.field_agglo_uww_agglo_nid', $node->nid, '=');
        $query->condition('u.field_agglo_uww_uww_nid', $uwws['nid'], '=');
        $result = $query->execute();
        while($record = $result->fetchAssoc()){
          $perce = $record['field_agglo_uww_perc_ent_uw_value'];
          $mperce = $record['field_agglo_uww_mperc_ent_uw_value'];
        }

        $totale = floor(($totalout / 100) * $perce);

        $msType = false;
        if($uww->field_uwwnremoval['und'][0]['value'] == '1'){
          $msType = 'N';
          if($uww->field_uwwpremoval['und'][0]['value'] == '1'){
            $msType = 'NP';
            if(
              $uww->field_uwwuv['und'][0]['value'] == '1' ||
              $uww->field_uwwchlorination['und'][0]['value'] == '1' ||
              $uww->field_uwwozonation['und'][0]['value'] == '1' ||
              $uww->field_uwwsandfiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwmicrofiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwothertreat['und'][0]['value'] == '1'
            ){
              $msType = 'NPB';
            }
          }
          else{
            if(
              $uww->field_uwwuv['und'][0]['value'] == '1' ||
              $uww->field_uwwchlorination['und'][0]['value'] == '1' ||
              $uww->field_uwwozonation['und'][0]['value'] == '1' ||
              $uww->field_uwwsandfiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwmicrofiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwothertreat['und'][0]['value'] == '1'
            ){
              $msType = 'NB';
            }
          }
        }
        else{
          if($uww->field_uwwpremoval['und'][0]['value'] == '1'){
            $msType = 'P';
            if(
              $uww->field_uwwuv['und'][0]['value'] == '1' ||
              $uww->field_uwwchlorination['und'][0]['value'] == '1' ||
              $uww->field_uwwozonation['und'][0]['value'] == '1' ||
              $uww->field_uwwsandfiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwmicrofiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwothertreat['und'][0]['value'] == '1'
            ){
              $msType = 'PB';
            }
          }
          else{
            if(
              $uww->field_uwwuv['und'][0]['value'] == '1' ||
              $uww->field_uwwchlorination['und'][0]['value'] == '1' ||
              $uww->field_uwwozonation['und'][0]['value'] == '1' ||
              $uww->field_uwwsandfiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwmicrofiltration['und'][0]['value'] == '1' ||
              $uww->field_uwwothertreat['und'][0]['value'] == '1'
            ){
              $msType = 'B';
            }
          }
        }

        $reseau[$uwws['nid']] = array('nid' => $uwws['nid'], 'dcps' => array());
        $reseau[$uwws['nid']]['loadEntering'] = $totale;
        $reseau[$uwws['nid']]['percEntering'] = $perce;
        $reseau[$uwws['nid']]['mstype'] = $msType;
        $reseau[$uwws['nid']]['title'] = $uww->title;
        $reseau[$uwws['nid']]['compStation'] = $uww->field_uwwcompliance['und'][0]['value'];
        foreach($uww->field_linked_discharge_points['und'] as $dcps){
          $loadedDcp = node_load($dcps['nid']);
          $dcpNid = $loadedDcp->nid;
          $dcpTitle = $loadedDcp->title;
          $rca = node_load($loadedDcp->field_linked_receiving_areas['und'][0]['nid']);
          $reseau[$uwws['nid']]['dcps'][$dcpNid]['nid'] = $dcpNid;
          $reseau[$uwws['nid']]['dcps'][$dcpNid]['title'] = $dcpTitle;
          $reseau[$uwws['nid']]['dcps'][$dcpNid]['rcaNid'] = $rca->nid;
          $reseau[$uwws['nid']]['dcps'][$dcpNid]['rcaTitle'] = $rca->title;
          $reseau[$uwws['nid']]['dcps'][$dcpNid]['rcaType'] = $loadedDcp->field_rcatype['und'][0]['value'];
        }
      }
      
      $nbPlantsT = 0;
      $nbPlantsB = 0;
      for ($i=0; $i <= $nbPlants; $i++) { 

        if($i > 1){    

          if($i % 2 == 0){
            $nbPlantsT ++;
          }
          else{
            $nbPlantsB ++;
          }
        }
      }
      $topMarge = $nbPlantsB * 95;

      $output = '';

    $output .= '<div class="ias">
      <div class="graphic-title">
        '.t('Indivual Appropriate Systems:').' '.$totalIAS.' p.e ('.$node->field_aggc2['und'][0]['value'].'%)
      </div>
      <img width="1098px" src="'.$src.'/images/graphic/ias.png" alt="reseau">
    </div>
    <div class="graphic-container">

      <div class="agglomeration-graphic">
        <img width="270px" src="'.$src.'/images/graphic/reseau.png" alt="reseau">
        <div class="graphic-title">
          '.l($node->title, "node/".$node->nid).'<br>
          '.$node->field_agggenerated['und'][0]['value'].' p.e
        </div>
        <div class="agglo-load">
          '.floor(($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggc1['und'][0]['value']).' p.e <br>('.$node->field_aggc1['und'][0]['value'] .'%)
        </div>
      </div>

      <div class="connectors" style="margin-top: -'.$topMarge.'px; top: '.$topMarge.'px;">';

      for ($i=0; $i < $nbPlantsT; $i++) { 
        $output .= '<img width="150px" src="'.$src.'/images/graphic/topcap.png" alt="reseau">';
      }

      if($nbPlants > 0){ 
        $output .= '<img width="150px" src="'.$src.'/images/graphic/single.png" alt="reseau">';
      }

      for ($i=0; $i < $nbPlantsB; $i++) { 
        $output .= '<img width="150px" src="'.$src.'/images/graphic/botcap.png" alt="reseau">';
      }

      $output .= '</div>
      <div class="stations">';

      $offset = 0;
      if($nbPlants % 2 == 0){
        $offset = 50;
      }

      foreach($reseau as $station){

        $output .= '<div class="station" style="position: relative; top: -'.$offset.'px">';

            if($station['compStation'] == 'C') $output .= '<img src="'.$src.'/images/graphic/station-c.png" alt="reseau">';
            elseif($station['compStation'] == 'NC') $output .= '<img src="'.$src.'/images/graphic/station-nc.png" alt="reseau">';
            else $output .= '<img src="'.$src.'/images/graphic/station.png" alt="reseau">';
          
          $output .= '<div class="graphic-title">
            '.l($station['title'], "node/".$station['nid']).'
          </div>

          <div class="station-load">'.$station['loadEntering'].' p.e <br>('.$station['percEntering'].'%)</div>
        </div>';

      }
    
    $output .= '</div>';

    $topMarge = 0;
    if($nbPlants % 2 == 0){
      $topMarge = 48;
    }

    $output .= '<div class="ms-wrapper" style="top: -'. $topMarge .'px">'; 
      foreach($reseau as $station){

        if($station['mstype'] != false){
              
        $output .= '<div class="ms">
          <img src="'.$src.'/images/graphic/ms.png" alt="reseau">
          <div class="ms-type">
            '.$station['mstype'].'
          </div>
        </div>';
 
        }
        else{
          $output .= '<div class="ms"><img class="single" width="150px" src="'.$src.'/images/graphic/single.png" alt="reseau"></div>';
        }
      }

    $output .= '</div>';

    $offset = $nbPlants * 16;
    $totalDcps = 0;
    $totalDcpsT = 0;
    $totalDcpsB = 0;

    $output .= '<div class="dcp-connections" style="top: -'.$topMarge.'px">';

        foreach($reseau as $station){
          $nbDcps = count($station['dcps']);
          $totalDcps = $totalDcps + $nbDcps;
          $nbDcpsT = 0;
          $nbDcpsB = 0;
          for ($i=0; $i <= $nbDcps; $i++) { 

            if($i > 1){    
    
              if($i % 2 == 0){
                $nbDcpsT ++;
                $totalDcpsT++;
              }
              else{
                $nbDcpsB ++;
                $totalDcpsB++;
              }
            }
          }

          $output .= '<div class="station-dcp-connections">';
              $output .= '<div>';

            // for ($i=0; $i < $nbDcpsT; $i++) { 
            //   $output .= '<img width="75px" src="'.$src.'/images/graphic/topcapsmall.png" alt="reseau">';
            // }

            // if($nbDcps > 0){ 
              $output .= '<img width="75px" src="'.$src.'/images/graphic/singlesmall.png" alt="reseau">';
            // }

            // for ($i=0; $i < $nbDcpsB; $i++) { 
            //   $output .= '<img width="75px" src="'.$src.'/images/graphic/botcapsmall.png" alt="reseau">';
            // }

            $output .= '</div>
          </div>';
        }

    $output .= '</div>

    <div class="dcps-wrapper" style="top:-'. $topMarge .'px">';
      
    foreach($reseau as $station){  
    	$output .= '<div class="station-dcp" style="top: 0px;">
			<div>
			  	<img width="80px" src="'.$src.'/images/graphic/dcp.png" alt="reseau">';
			  	$output .= '<div class="graphic-title">';
			  		$output .= l(count($station['dcps'])." DCP(S)", "#");
			  		$output .= '<div class="dcps-hidden-list">';
    					foreach($station['dcps'] as $dcp) {
			    			$output .= l($dcp['title'], "node/".$dcp['nid']);
			    			$output .= '<br>';
			    			$output .= l($dcp['rcaTitle'], "node/".$dcp['rcaNid']);
			    			$output .= ' ('.$dcp['rcaType'].')';
			    			$output .= '<br>';
			    			$output .= '<br>';
			  			}
			  		$output .= '</div>
			  	</div>
			</div>
		</div>';
    }

    $output .= '</div>
  </div>
  <div class="dischage-wot">
    <div class="graphic-title">
      '.t('Discharge without treatment:').' '.$totalWOT.' p.e ('.$node->field_aggpercwithouttreatment['und'][0]['value'].'%)
    </div>
    <img width="1098px" src="'.$src.'/images/graphic/wot.png" alt="reseau">
  </div>';

  return $output;
}

function uwwtd_render_field_with_pe($field)
{               
//     $field[0]['#markup'] = 'Not provided';
    $pe = '';
    if (true === is_numeric($field[0]['#markup'])) {
        $pe = ' % ('. uwwtd_format_number($field['#items'][0]['value'] / 100 * $field['#object']->field_agggenerated['und'][0]['value']) .' p.e.)';
    }
    return '<div class="field field-name-field-aggc1 field-type-number-decimal field-label-inline clearfix">
        <div class="field-label">'.$field['#title'].':&nbsp;</div>
        <div class="field-items">
            <div class="field-item even">'.$field[0]['#markup']. $pe .'</div>
        </div>
    </div>';    
}

// function uwwtd_preprocess_field(&$vars) {
//     dpl($vars);
//   if ($vars['element']['#field_name'] == 'field_my_custom_field') {
//     $vars['items'][0]['#markup'] = "I changed the output of my field!";
//   }
// }

function uwwtd_piechart_agglonode($node, &$content)
{    
    drupal_add_js('sites/all/libraries/d3/d3.v3.min.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    
// echo '<pre>';var_export($content);echo '</pre>';
// echo '<pre>';var_export($content);echo '</pre>';
// exit;
    $aData = array();
    $aData[] = array(
        "value" => $node->field_aggc1['und'][0]['value'],
        "label" => $content['field_agggenerated']['#title'],
        "valueformat" => ($node->field_aggc1['und'][0]['value'] == 0 ? '' : uwwtd_format_number($node->field_aggc1['und'][0]['value']) . ' %'),
    );
    $aData[] = array(
        "value" => $node->field_aggc2['und'][0]['value'],
        "label" => $content['field_aggc2']['#title'],
        "valueformat" =>  ($node->field_aggc1['und'][0]['value'] == 0 ? '' : uwwtd_format_number($node->field_aggc2['und'][0]['value']) . ' %'),
    );  
    $aData[] = array(
        "value" => $node->field_aggpercwithouttreatment['und'][0]['value'],
        "label" => $content['field_aggpercwithouttreatment']['#title'],
        "valueformat" =>  ($node->field_aggc1['und'][0]['value'] == 0 ? '' : uwwtd_format_number($node->field_aggpercwithouttreatment['und'][0]['value']) . ' %'),
    );     
//     return "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/d3/3.5.5/d3.min.js\"></script>
//     return "
//     <script>
//     (function ($) {
//     $(document).ready(function() {
//         display_uwwtp_stackedbar(".json_encode($aData).");        
//         });    
//     })(jQuery);  
//     </script>
//     ";  
    return "
    <script>
    jQuery(document).ready(function(){                     
        display_agglo_piechart(".json_encode($aData).");        
    });    
    </script>
    ";       
}

function uwwtd_stackedbar_uwwtpnode($node)
{       
    drupal_add_js('sites/all/libraries/d3/d3.v3.min.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');            
    $aData = array();
    $aData[] = array(
        "type" => 'BOD',
//         "label" => 'BOD load',
        "incoming" => $node->field_uwwbodincoming['und'][0]['value'],
        "discharged" => $node->field_uwwboddischarge['und'][0]['value'],
    );                 
    $aData[] = array(
        "type" => 'COD',
//         "label" => 'COD load',
        "incoming" => $node->field_uwwcodincoming['und'][0]['value'],
        "discharged" => $node->field_uwwcoddischarge['und'][0]['value'],
    );
    $aData[] = array(
        "type" => 'N',
//         "label" => 'N load',
        "incoming" => $node->field_uwwnincoming['und'][0]['value'],
        "discharged" => $node->field_uwwndischarge['und'][0]['value'],
    );  
    $aData[] = array(
        "type" => 'P',
//         "label" => 'P load',
        "incoming" => $node->field_uwwpincoming['und'][0]['value'],
        "discharged" => $node->field_uwwpdischarge['und'][0]['value'],
    );              
//     dsm($aData); 
//     echo '<pre>';var_export($aData);echo '</pre>';          
//     return "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/d3/3.5.5/d3.min.js\"></script>
//     <style>
// .axis path,
// .axis line {
// fill: none;
// stroke: #000;
// shape-rendering: crispEdges;
// }
// 
// .bar {
// fill: steelblue;
// }
// 
// .x.axis path {
// display: none;
// }
//     </style>
    return "
    <script>
    jQuery(document).ready(function(){                     
        display_uwwtp_stackedbar(".json_encode($aData).");        
    });    
    </script>
    ";
}


// Ajoute un niveau de profondeur au menu principal

function uwwtd_menu_link__main_menu(&$variables)
{
	$element = $variables['element'];

  	$sub_menu = ($element['#below'] ? drupal_render($element['#below']) : '');
  	$depth = $element['#original_link']['depth'] + 1;

  	$unused = array('leaf', 'first', 'menu-mlid-' . $element['#original_link']['mlid']);
  	$element['#attributes']['class'] = array_diff($element['#attributes']['class'], $unused);

  	$element['#attributes']['class'][] = 'menu-item';
  	$element['#attributes']['class'][] = 'menu-item--depth-' . $element['#original_link']['depth'];

  	$element['#localized_options']['html'] = TRUE;
  	$element['#localized_options']['attributes']['class'][] = 'menu-link';
  	$element['#localized_options']['attributes']['class'][] = 'menu-link--depth-' . $element['#original_link']['depth'];

  	if (!empty($sub_menu)) {
    	// Wrap menu-level for each level.
    	$sub_menu = '<div class="menu-level menu-level--' . $depth . '">' .  $sub_menu . '</div>';
  	}
  	// krumo($element);
  	if ($depth == 2 && $element['#below']) {
  		$link = l('<span class="title">' . $element['#title'] . ' <i style="float:right;line-height:20px;" class="fa fa-caret-down"></i></span>', $element['#href'], $element['#localized_options']);
  	} elseif ($depth == 3 && $element['#below']) {
  		$link = l('<span class="title">' . $element['#title'] . ' <i style="float:right;line-height:20px;" class="fa fa-caret-right"></i></span>', $element['#href'], $element['#localized_options']);
  	} else {	
  		$link = l('<span class="title">' . $element['#title'] . '</span>', $element['#href'], $element['#localized_options']);
  	}
  	return '<li' . drupal_attributes($element['#attributes']) . '>' . $link . $sub_menu . '</li>';
	
}

function uwwtd_menu_link__user_menu(&$variables) {
	$element = $variables['element'];
	// krumo($element);
	$menu_link = $element['#href'];
	switch ($menu_link) {
	 	case 'user':
	 		return '<li><a href="'. $menu_link .'"><i class="fa fa-user"></i></a></li>';
	 		break;
	 	case 'user/logout':
	 		return '<li><a href="'. $menu_link .'"><i class="fa fa-power-off"></i></a></li>';
	 		break;
	 	
	 	default:
	 		return '<li><a href="'. $menu_link .'">'. $element['#title'] .'</a></li>';
	 		break;
	 } 
}

function uwwtd_preprocess_html(&$vars) {
    // Add font awesome cdn.
  	drupal_add_css('//maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.css', array(
    	'type' => 'external'
    ));
}