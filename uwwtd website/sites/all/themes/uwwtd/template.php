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
  if (in_array($variables['element']['#field_name'], array(
        'field_uwwprimarytreatment','field_uwwsecondarytreatment','field_uwwpremoval','field_uwwnremoval','field_uwwuv','field_uwwchlorination','field_uwwozonation','field_uwwsandfiltration','field_uwwmicrofiltration','field_uwwothertreat','field_uwwaccidents',
        'field_dcpsurfacewaters',
        'field_aggcritb','field_aggcritca','field_aggcritcb','field_aggchanges','field_aggbesttechnicalknowledge','field_agghaveregistrationsystem','field_aggexistmaintenanceplan','field_aggpressuretest','field_aggvideoinspections','field_aggothermeasures','field_aggaccoverflows','field_aggcapacity','field_agg_dilution_rates',
        'field_rcaanitro','field_rcaaphos','field_rcab','field_rcac','field_rcamorphology','field_rcahydrologie','field_rcahydraulic','field_rcaabsencerisk','field_rca54applied','field_rca_parameter_n','field_rca_parameter_p','field_rca_parameter_other','field_rca52applied','field_rca58applied'
    ))){
    //check for custom tooltips
    $tt = '';
    $field_info = field_info_field($variables['element']['#field_name']);
    if(isset($field_info['custom_tooltip']) && $field_info['custom_tooltip'] !== ''){
      $tt = $variables['items'][0]['#markup'];
    }

    if($variables['element']['#items']['0']['value'] === '1' || $variables['element']['#items']['0']['value'] === 'P'){
      $variables['items']['0']['#markup'] = $tt.'<img style="position: relative; top: -2px; margin-left: 5px;" height="10px" src="'.url(path_to_theme().'/images/tick.png'). '"/>';
    }
    elseif($variables['element']['#items']['0']['value'] === '0' || $variables['element']['#items']['0']['value'] === 'F'){
      $variables['items']['0']['#markup'] = $tt.'<img style="position: relative; top: -2px; margin-left: 5px;" height="10px" src="'.url(path_to_theme().'/images/cross.png'). '"/>';
    }
  }

  // For percentage '%'
  if (in_array($variables['element']['#field_name'], array('field_aggc1','field_aggc2','field_aggpercwithouttreatment','field_aggpercprimtreatment','field_aggpercsectreatment','field_aggpercstringenttreatment'))
       && $variables['items']['0']['#markup'] != '<p>'.t('Not provided').'</p>'
  ){
    $variables['items']['0']['#markup'] = $variables['items']['0']['#markup'].' %';
  }

//     if ($variables['element']['#field_name'] == 'field_aggc1' ||
//    $variables['element']['#field_name'] == 'field_aggc2' ||
//    $variables['element']['#field_name'] == 'field_aggpercwithouttreatment') {
//         $pe = $variables['element']['#object']->field_agggenerated['und'][0]['value'] * $variables['element']['#object']->{$variables['element']['#field_name']}['und'][0]['value']/ 100;
//         $variables['items']['0']['#markup'] .= ' (' . $pe .')';
//     }

  // For compliance colors
  if (in_array($variables['element']['#field_name'], array('field_aggart3compliance','field_aggart4compliance','field_aggart5compliance','field_aggart6compliance','field_aggcompliance','field_uwwcompliance'))){
//         dsm($variables['element']);
        //override PD and QC value, we don't want to display QC and PD (for now)
        /* This function disable the tooltips ===> why*/
        if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $variables['element']['#items']['0']['value'] ])) {
            $variables['element']['#items']['0']['value'] = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $variables['element']['#items']['0']['value'] ];
            $variables['element']['0']['#markup'] = $GLOBALS['uwwtd']['ui']['compliance'][ $variables['element']['#items']['0']['value'] ];
            //We need to keep the tooltip.
            if($pos = strpos($variables['items']['0']['#markup'], '</div>')){
                $variables['items']['0']['#markup'] = substr($variables['items']['0']['#markup'],0,$pos+6) .$variables['element']['0']['#markup'];
            }
            else{
                $variables['items']['0']['#markup'] = $variables['element']['0']['#markup'];
            }
            
        }
        
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
    $output .='<table class = "dispo-annee">
              <tbody>
                <tr>
                  <td class="dispo-before"><div></div></td>';
    foreach($otherList as $other){
      $ting = $other['node'];
      if($node->type == 'agglomeration') $val = $ting->field_aggcompliance['und'][0]['value'];
      if($node->type == 'uwwtp') $val = $ting->field_uwwcompliance['und'][0]['value'];

            //override PD and QC value, we don't want to display QC and PD (for now)
            if (isset($val) && isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $val ])) {
                $val = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $val ];
            }

      //default colors
      $color = '#6b6b6b'; $borderc = '#6b5e66';

      if(isset($val)){
        if($val == 'C') {$color = '#4f91e1'; $borderc = '#4faaf9'; $txtc = '#ffffff'; $bulleTxt = t('Compliant'); $dispo =1;}
        if($val == 'NC') {$color = '#d93c3c'; $borderc = '#d91a10'; $txtc = '#ffffff'; $bulleTxt = t('Not compliant');$dispo =2;}
        if($val == 'NR') {$color = '#a2a2a2'; $borderc = '#a6a2a2'; $txtc = '#ffffff'; $bulleTxt = t('Not relevant');$dispo =3;}
        if($val == 'NI') {$color = '#6b6b6b'; $borderc = '#6b5e66'; $txtc = '#ffffff'; $bulleTxt = t('No information'); $dispo =4;}
            // if($val == 'CE') {$color = '#ea8b2e'; $borderc = '#ea7810'; $txtc = '#ffffff'; $bulleTxt = t('Compliant on equipment');$dispo =5;}
                if($val == '?') {$color = '#6b6b6b'; $borderc = '#6b5e66'; $txtc = '#ffffff'; $bulleTxt = t('?');$dispo =0;}
      }
      // $color = uwwtd_adjustBrightness($color, 50);
      $nbAnnee = count($otherY); 
      $j=0;
            $cell = 0;
      for($a=1;$a<=$nbAnnee;$a++){
                if($ting->field_anneedata['und'][0]['value'] == $node->field_anneedata['und'][0]['value']){
              $output.='<td class="dispo-annee dispo-'.$dispo.' uwwtd-history-element uwwtd-history-element-current">';
            $output .= '<h4 style="color: '.$txtc.';" title="'.$bulleTxt.'">'.$ting->field_anneedata['und'][0]['value'].'</h4>';
            // if(isset($val)) $output .= '<span class="current" style="background-color: '.$color.'; border-color: '.$borderc.'; color: '.$txtc.'" title="'.$bulleTxt.'">'.$bulleTxt.'</span>';
          $output .= '</td>';
        }else{
          $output.='<td class="dispo-annee dispo-'.$dispo.' uwwtd-history-element">';
            $output .= '<h4 style="color: '.$txtc.';" title="'.$bulleTxt.'">'.l($ting->field_anneedata['und'][0]['value'], 'node/'.$ting->nid).'</h4>';
            // if(isset($val)) $output .= '<span style="background-color: '.$color.'; border-color: '.$borderc.'; color: '.$txtc.'" title="'.$bulleTxt.'">'.$val.'</span>';
          $output .= '</td>';
        }
                //Si on arrive a 16 cellules, un retour à la ligne s'impose
                if($cell==16){
                    $output.='<td class="dispo-next">...</td></tr><tr><td class="dispo-next">...</td>';
                    $cell=0;
                }
                else{
                    $cell++;
                }
                $j++;
            }

      // if($ting->field_anneedata['und'][0]['value'] == $node->field_anneedata['und'][0]['value']){
        // $output .= '<div class="uwwtd-history-element uwwtd-history-element-current">';
        // $output .= '<h4>'.$ting->field_anneedata['und'][0]['value'].'</h4>';
        // if(isset($val)) $output .= '<span class="current" style="background-color: '.$color.'; border-color: '.$borderc.'; color: '.$txtc.'" title="'.$bulleTxt.'">'.$bulleTxt.'</span>';
        // $output .= '</div>';
      // }
      // else{
        // $output .= '<div class="uwwtd-history-element">';
        // $output .= '<h4>'.l($ting->field_anneedata['und'][0]['value'], 'node/'.$ting->nid).'</h4>';
        // if(isset($val)) $output .= '<span style="background-color: '.$color.'; border-color: '.$borderc.'; color: '.$txtc.'" title="'.$bulleTxt.'">'.$val.'</span>';
        // $output .= '</div>';
      // }
    }
      $output.='<td class="dispo-after"><div></div></td>';
          $output.='</tr>';
        $output.='</table>';
    $output.='<div class="dispo-legend">';
          $output.='<div class="dispo-legend-item"><div class="dispo-legend-color dispo-1">&nbsp;</div><div class="dispo-legend-label"> '.t('Compliant').'</div></div>';
          $output.='<div class="dispo-legend-item"><div class="dispo-legend-color dispo-2">&nbsp;</div><div class="dispo-legend-label"> '.t('Not compliant').'</div></div>';
          $output.='<div class="dispo-legend-item"><div class="dispo-legend-color dispo-3">&nbsp;</div><div class="dispo-legend-label"> '.t('Not relevant').'</div></div>';
          $output.='<div class="dispo-legend-item"><div class="dispo-legend-color dispo-4">&nbsp;</div><div class="dispo-legend-label"> '.t('No information').'</div></div>';
          // $output.='<div class="dispo-legend-item"><div class="dispo-legend-color dispo-5">&nbsp;</div><div class="dispo-legend-label"> '.t('Compliant on equipment').'</div></div>';
          $output.='<div class="dispo-legend-item"><div class="dispo-legend-color dispo-0">&nbsp;</div><div class="dispo-legend-label"> '.t('?').'</div></div>';
          $output.='<br class="spacer"/>';
        $output.='</div>';
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

function uwwtd_render_article17_aglo($nid){
  $art17 = node_load($nid);

  $output = '';
  //field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
  if(isset($art17->field_art17_flaggreasons['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggreasons', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flaggmeasures['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggmeasures', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flaggexpecdatestart['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggexpecdatestart', array('display' => 'inline', 'settings' => array('format_type' => 'custom_siif')));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flaggexpecdatestartw['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggexpecdatestartw', array('display' => 'inline', 'settings' => array('format_type' => 'custom_siif')));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flaggexpecdatecomple['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggexpecdatecomple', array('display' => 'inline', 'settings' => array('format_type' => 'custom_siif')));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flagginv['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flagginv', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flaggeufundname['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggeufundname', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flaggeufund['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggeufund', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flaggcomments['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flaggcomments', array('display' => 'inline'));
    $output .= render($champ);
  }
  return $output;
}

function uwwtd_render_article17_uwwtp($nid){
  $art17 = node_load($nid);

  $output = '';
  //field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
  if(isset($art17->field_art17_flatpreasons['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpreasons', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpmeasures['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpmeasures', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpexpecdatestartw['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpexpecdatestartw', array('display' => 'inline', 'settings' => array('format_type' => 'custom_siif')));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpexpecdatecomple['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpexpecdatecomple', array('display' => 'inline', 'settings' => array('format_type' => 'custom_siif')));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpinv['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpinv', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpeufundname['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpeufundname', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpeufund['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpeufund', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpcomments['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpcomments', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpexpload['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpexpload', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpexpcapacity['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpexpcapacity', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatptexpectreatment['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatptexpectreatment', array('display' => 'inline'));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpexpecdateperfor['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpexpecdateperfor', array('display' => 'inline', 'settings' => array('format_type' => 'custom_siif')));
    $output .= render($champ);
  }

  if(isset($art17->field_art17_flatpexpecdatestart['und'][0]['value'])){
    $champ = field_view_field('node', $art17, 'field_art17_flatpexpecdatestart', array('display' => 'inline', 'settings' => array('format_type' => 'custom_siif')));
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

//  // Get list of all elements
//  $raws = $node->field_uwwtd_error_link['und'];
//  $all = array();
//  foreach($raws as $raw){
//    $all[] = $raw['nid'];
//  }
//
//  // group by import
//  foreach($all as $single){
//    $error = node_load($single);
//    if($error->nid !== null){
//      $id = $error->field_uwwtd_err_identifier['und'][0]['value'];
//      $type = $error->field_uwwtd_err_type['und'][0]['value'];
//      $cat = $error->field_uwwtd_err_category['und'][0]['value'];
//
//      // Set values
//      $values[$id][$type][$cat][] = array(
//        'id' => $id,
//        'timestamp' => $error->field_uwwtd_err_timestamp['und'][0]['value'],
//        'time' => $error->field_uwwtd_err_time['und'][0]['value'],
//        'message' => $error->field_uwwtd_err_message['und'][0]['value'],
//        'type' => $type,
//        'category' => $error->field_uwwtd_err_category['und'][0]['value'],
//        'nid' => $error->nid
//      );
//    }
//  }

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
      $overallTotal = (isset($type['0']) ? count($type['0']) : 0) 
                            + (isset($type['1']) ? count($type['1']) : 0) 
                            + (isset($type['2']) ? count($type['2']) : 0) 
                            + (isset($type['3']) ? count($type['3']) : 0) 
                            + (isset($type['4']) ? count($type['4']) : 0) 
                            + $overallTotal;
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
//      $field = field_info_field('field_uwwtd_err_category');
//      $label = $field['settings']['allowed_values'];

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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWI.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWL.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWG.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWC.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" data-category="'.$labelWF.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
//                  $output .= '<li><img data-id="'.$error['nid'].'" data-attached="'.$node->nid.'" class="uwwtd_ignore" src="http://'.$_SERVER['HTTP_HOST'].base_path().path_to_theme().'/images/uwwtd_ignore.png" /></li>';
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
    $reseau['collectingSystem'] = $node->field_uwwcollectingsystem['und'][0]['value'];
    $reseau['load'] = $node->field_uwwloadenteringuwwtp['und'][0]['value'];
    $reseau['required'] = $node->field_uwwtreatmentrequired['und'][0]['value'];

    $reseau['primary'] = array(
        'treatment' => (isset($node->field_uwwprimarytreatment['und'][0]['value']) ? $node->field_uwwprimarytreatment['und'][0]['value'] : '')
    );

    $reseau['secondary'] = array(
        'treatment' => (isset($node->field_uwwsecondarytreatment['und'][0]['value']) ? $node->field_uwwsecondarytreatment['und'][0]['value'] : '')
    );

    $reseau['n-removal'] = array(
        'treatment' => (isset($node->field_uwwnremoval['und'][0]['value']) ? $node->field_uwwnremoval['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwntotperf['und'][0]['value']) ? $node->field_uwwntotperf['und'][0]['value'] : '')
    );

    $reseau['p-removal'] = array(
        'treatment' => (isset($node->field_uwwpremoval['und'][0]['value']) ? $node->field_uwwpremoval['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwptotperf['und'][0]['value']) ? $node->field_uwwptotperf['und'][0]['value'] : '')
    );

    $reseau['uv'] = array(
        'treatment' => (isset($node->field_uwwuv['und'][0]['value']) ? $node->field_uwwuv['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwotherperf['und'][0]['value']) ? $node->field_uwwotherperf['und'][0]['value'] : '')
    );

    $reseau['micro'] = array(
        'treatment' => (isset($node->uwwmicrofiltration['und'][0]['value']) ? $node->uwwmicrofiltration['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwotherperf['und'][0]['value']) ? $node->field_uwwotherperf['und'][0]['value'] : '')
    );

    $reseau['chlorination'] = array(
        'treatment' => (isset($node->field_uwwchlorination['und'][0]['value']) ? $node->field_uwwchlorination['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwotherperf['und'][0]['value']) ? $node->field_uwwotherperf['und'][0]['value']  : '')
    );

    $reseau['ozonation'] = array(
        'treatment' => (isset($node->field_uwwozonation['und'][0]['value']) ? $node->field_uwwozonation['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwotherperf['und'][0]['value']) ? $node->field_uwwotherperf['und'][0]['value'] : '')
    );

    $reseau['other'] = array(
        'treatment' => (isset($node->field_uwwothertreat['und'][0]['value']) ? $node->field_uwwothertreat['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwotherperf['und'][0]['value']) ? $node->field_uwwotherperf['und'][0]['value'] : '')
    );

    $reseau['sand'] = array(
        'treatment' => (isset($node->field_uwwsandfiltration['und'][0]['value']) ? $node->field_uwwsandfiltration['und'][0]['value'] : ''),
        'performance' => (isset($node->field_uwwotherperf['und'][0]['value']) ? $node->field_uwwotherperf['und'][0]['value'] : '')
    );

    $reseau['agglos'] = array();
    foreach ($node->field_linked_agglomerations['und'] as $aggs) {
        $nbAgglos++;
        $agg = node_load($aggs['nid']);
        if ($agg->field_aggcompliance['und'][0]['value'] == 'NC') {
            $aggComplianceNC = true;
        } elseif ($agg->field_aggcompliance['und'][0]['value'] == 'NR' || $agg->field_aggcompliance['und'][0]['value'] == 'PD') {
            $aggComplianceNR = true;
        }
        

        $reseau['agglos'][$agg->nid]['nid'] = $agg->nid;
        $reseau['agglos'][$agg->nid]['title'] = $agg->title;
        $reseau['agglos'][$agg->nid]['id'] = $agg->field_siteid['und'][0]['value'];
        $reseau['agglos'][$agg->nid]['generated'] = $agg->field_agggenerated['und'][0]['value'];
    }

    $nbDcps = 0;
    foreach ($node->field_linked_discharge_points['und'] as $dcps) {
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

    $output = '<div class="graphic-container">';
    $output .= '<div class="agglomeration-graphic">';

    if ($aggComplianceNR === true) {
        $output .= '<img width="270px" src="'.$src.'/images/graphic/reseau.png" alt="reseau">';
    } elseif ($aggComplianceNC === true) {
        $output .= '<img width="270px" src="'.$src.'/images/graphic/reseau-nc.png" alt="reseau">';
    } else {
        $output .= '<img width="270px" src="'.$src.'/images/graphic/reseau-c.png" alt="reseau">';
    }
          
    $output .= '<div class="graphic-title">';
    $output .= '<a href="#">Agglomerations : '.count($reseau['agglos']).'</a>
            <span class="aggs-list">';
    foreach($reseau['agglos'] as $agglo){
        $output .= 'Agglomeration : '.l($agglo['title'], "node/".$agglo['nid']).'<br>
                (Generated : '.uwwtd_format_number($agglo['generated'], 0).' p.e)<br>';
    }

    $output .= '</span></div></div>';

    $output .= '<div class="connectors">';
    $output .= '<img width="150px" src="'.$src.'/images/graphic/single.png" alt="reseau">';
    $output .= '</div>';
    $offset = 0;
    $output .= '<div class="station" style="position: relative; top: -'.$offset.'px">';

    if ($reseau['collectingSystem'] == 'ISCON') {
        if ($reseau['compliance'] == 'C') { 
            $output .= '<img src="'.$src.'/images/graphic/station-c.png" alt="reseau" title="Main treatment">';
        } elseif ($reseau['compliance'] == 'NC') { 
            $output .= '<img src="'.$src.'/images/graphic/station-nc.png" alt="reseau"  title="Main treatment">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/station.png" alt="reseau" title="Main treatment">';
        }
    } else {
        if ($reseau['compliance'] == 'C') { 
            $output .= '<img src="'.$src.'/images/graphic/station-notcon-c.png" alt="reseau" title="Main treatment">';
        } elseif ($reseau['compliance'] == 'NC') { 
            $output .= '<img src="'.$src.'/images/graphic/station-notcon-nc.png" alt="reseau"  title="Main treatment">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/station-notcon.png" alt="reseau" title="Main treatment">';
        }
    }

    $output .= '<div class="graphic-title">
            '.l($reseau['title'], "node/".$reseau['nid']).'
            </div>
            <div class="station-load">Entering :<br>'.uwwtd_format_number($reseau['load'], 0).' p.e</div>
    </div>';

    $output .= '<div class="more-wrapper">';
    if ($reseau['n-removal']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['n-removal']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['n-removal']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        }
        $output .= '<div class="more-text">'.t('N').'</div>';
        $output .= '</div>';
    }

    if ($reseau['p-removal']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['p-removal']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['p-removal']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        }
        $output .= '<div class="more-text">'.t('P').'</div>';
        $output .= '</div>';
    }

    if ($reseau['uv']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['uv']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['uv']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        }
        $output .= '<div class="more-text">'.t('UV').'</div>';
        $output .= '</div>';
    }

    if ($reseau['micro']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['micro']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['micro']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        }
        $output .= '<div class="more-text">'.t('MICRO').'</div>';
        $output .= '</div>';
    }

    if ($reseau['chlorination']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['chlorination']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['chlorination']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        }
        $output .= '<div class="more-text">'.t('CHLOR').'</div>';
        $output .= '</div>';
    }

    if ($reseau['ozonation']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['ozonation']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['ozonation']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        }
        $output .= '<div class="more-text">'.t('OZONE').'</div>';
        $output .= '</div>';
    }

    if ($reseau['sand']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['sand']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['sand']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        }
        $output .= '<div class="more-text">'.t('SAND').'</div>';
        $output .= '</div>';
    }

    if ($reseau['other']['treatment'] == '1') {
        $output .= '<div class="more">';
        if ($reseau['other']['performance'] == 'P') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } elseif ($reseau['other']['performance'] == 'F') {
            $output .= '<img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
        } else {
            $output .= '<img src="'.$src.'/images/graphic/ms-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
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
        if ($i > 1) {
            if ($i % 2 == 0) {
                $nbDcpsT ++;
                $totalDcpsT++;
            } else {
                $nbDcpsB ++;
                $totalDcpsB++;
            }
        }
    }
    if ($nbDcpsT == 1) {
        $topMarge = 50;
    } else {
        $topMarge = 0 + ($nbDcpsT * 48);
    }
    if($topMarge < 0) { 
        $topMarge = 0;
    }
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
                <img width="80px" src="'.$src.'/images/graphic/dcp.png" alt="reseau"  title="Discharge point">
                    <div class="graphic-title">';
                        $output .= l(count($reseau['dcps'])." DCP(S)", "#");
                        $output .= '<div class="dcps-hidden-list">';
                        $output .= 'Discharge point: ';
                        foreach ($reseau['dcps'] as $dcp) {
                            $output .= l($dcp['title'], "node/".$dcp['nid']);
                            $output .= '<br>';
                            $output .= l($dcp['rcaTitle'], "node/".$dcp['rcaNid']);
                            $output .=  '('.$dcp['rcaType'].')';
                            $output .= '<br><br>';
                        }
                    $output .= '</div>
                </div>
            </div>
        </div>
    </div>';

    $output .= '</div><div class="graphic-legend">
            <ul><b>Main treatment : </b>
                <li><img src="'.$src.'/images/graphic/station-c.png" alt="reseau"> : Compliant</li>
                <li><img src="'.$src.'/images/graphic/station-nc.png" alt="reseau"> : Not compliant</li>
                <li><img src="'.$src.'/images/graphic/station.png" alt="reseau"> : No information / Not relevant</li>
            </ul>
            <ul><b>More stringent treatment : <br> Performance : </b>
                <li><img src="'.$src.'/images/graphic/ms-small-c.png" alt="reseau"> : Pass performance</li>
                <li><img src="'.$src.'/images/graphic/ms-small-nc.png" alt="reseau"> : Fail performance</li>
                <li><img src="'.$src.'/images/graphic/ms-small.png" alt="reseau"> : Not relevant</li>
            </ul>
            <ul><br><b>Type :</b>
                <li><span>N</span> : Nitrogen removal</li>
                <li><span>P</span> : Phosphorus removal</li>
                <li><span>UV</span> : UV treatment</li>
                <li><span>MICRO</span> : Micro Filtration</li>
            </ul>
            <ul><br><b>Type : </b>
                <li><span>CHLOR</span> : Chlorination</li>
                <li><span>OZONE</span> : Ozonation</li>
                <li><span>SAND</span> : Sand filtration</li>
                <li><span>O</span> : Other more stringent</li>
            </ul>
            <ul><b>Discharge  point : </b>
                <li><img src="'.$src.'/images/graphic/dcp.png" alt="reseau"> : Discharge point</li>
                <li><span>DCP(S)</span> : Discharge point(s)</li>
            </ul>
        </div>';
    return $output;
}

function uwwtd_get_agglo_graphic($node){
    global $base_url;
    $src = $base_url . '/' . drupal_get_path('theme', 'uwwtd');

    //$totalout = $node->field_agggenerated['und'][0]['value'] / 100 * $node->field_aggc1['und'][0]['value'];
    $totalWOT = $node->field_agggenerated['und'][0]['value'] / 100 * $node->field_aggpercwithouttreatment['und'][0]['value'];
    $totalIAS = $node->field_agggenerated['und'][0]['value'] / 100 * $node->field_aggc2['und'][0]['value'];

    $nbPlants = 0;
    $reseau = array();
    foreach ($node->field_linked_treatment_plants['und'] as $uwws) {
        $nbPlants++;
        $uww = node_load($uwws['nid']);
        //get station entering sums
        $query = db_select('node', 'n');
        $query->join('field_data_field_agglo_uww_agglo', 'a', 'a.entity_id = n.nid');
        $query->join('field_data_field_agglo_uww_uww', 'u', 'u.entity_id = n.nid');
        $query->join('field_data_field_agglo_uww_perc_ent_uw', 'perce', 'perce.entity_id = n.nid');
        //$query->join('field_data_field_agglo_uww_mperc_ent_uw', 'mperce', 'mperce.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->fields('perce', array('field_agglo_uww_perc_ent_uw_value'));
        //$query->fields('mperce', array('field_agglo_uww_mperc_ent_uw_value'));
        $query->condition('a.field_agglo_uww_agglo_nid', $node->nid, '=');
        $query->condition('u.field_agglo_uww_uww_nid', $uwws['nid'], '=');
        $result = $query->execute();
        while ($record = $result->fetchAssoc()) {
            $perce_entering = $record['field_agglo_uww_perc_ent_uw_value'];
            //$mperce = $record['field_agglo_uww_mperc_ent_uw_value'];
        }

        //$totale = floor(($totalout / 100) * $perce);
        $total_entering = $node->field_agggenerated['und'][0]['value'] / 100 * $perce_entering;

        $msType = array();

        $msType['N'] = array(
            'treatment' => (isset($uww->field_uwwnremoval['und'][0]['value']) ? $uww->field_uwwnremoval['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwntotperf['und'][0]['value']) ? $uww->field_uwwntotperf['und'][0]['value'] : '')
        );

        $msType['P'] = array(
            'treatment' => (isset($uww->field_uwwpremoval['und'][0]['value']) ? $uww->field_uwwpremoval['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwptotperf['und'][0]['value']) ? $uww->field_uwwptotperf['und'][0]['value'] : '')
        );

        $msType['UV'] = array(
            'treatment' => (isset($uww->field_uwwuv['und'][0]['value']) ? $uww->field_uwwuv['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwotherperf['und'][0]['value']) ? $uww->field_uwwotherperf['und'][0]['value'] : '')
        );

        $msType['MICRO'] = array(
            'treatment' => (isset($uww->uwwmicrofiltration['und'][0]['value']) ? $uww->uwwmicrofiltration['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwotherperf['und'][0]['value']) ? $uww->field_uwwotherperf['und'][0]['value'] : '')
        );
    
        $msType['CHLOR'] = array(
            'treatment' => (isset($uww->field_uwwchlorination['und'][0]['value']) ? $uww->field_uwwchlorination['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwotherperf['und'][0]['value']) ? $uww->field_uwwotherperf['und'][0]['value']  : '')
        );
    
        $msType['OZONE'] = array(
            'treatment' => (isset($uww->field_uwwozonation['und'][0]['value']) ? $uww->field_uwwozonation['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwotherperf['und'][0]['value']) ? $uww->field_uwwotherperf['und'][0]['value'] : '')
        );
    
        $msType['SAND'] = array(
            'treatment' => (isset($uww->field_uwwsandfiltration['und'][0]['value']) ? $uww->field_uwwsandfiltration['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwotherperf['und'][0]['value']) ? $uww->field_uwwotherperf['und'][0]['value'] : '')
        );

        $msType['Other'] = array(
            'treatment' => (isset($uww->field_uwwothertreat['und'][0]['value']) ? $uww->field_uwwothertreat['und'][0]['value'] : ''),
            'performance' => (isset($uww->field_uwwotherperf['und'][0]['value']) ? $uww->field_uwwotherperf['und'][0]['value'] : '')
        );

        // Remove values like 0, '', null, false
        $msType = array_filter($msType);

        $reseau[$uwws['nid']] = array('nid' => $uwws['nid'], 'dcps' => array());
        $reseau[$uwws['nid']]['loadEntering'] = $total_entering;
        $reseau[$uwws['nid']]['percEntering'] = $perce_entering;
        $reseau[$uwws['nid']]['mstype'] = $msType;
        $reseau[$uwws['nid']]['hasMoreStringent'] = $uww->field_uwwtreatmenttype['und'][0]['value'] == 'MS' ? true : false;
        $reseau[$uwws['nid']]['title'] = $uww->title;
        $reseau[$uwws['nid']]['compStation'] = $uww->field_uwwcompliance['und'][0]['value'];
        $reseau[$uwws['nid']]['collectingSystem'] = $uww->field_uwwcollectingsystem['und'][0]['value'];

        foreach ($uww->field_linked_discharge_points['und'] as $dcps) {
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
    dsm($reseau);
    $nbPlantsT = 0;
    $nbPlantsB = 0;
    for ($i=0; $i <= $nbPlants; $i++) {
        if ($i > 1) {
            if ($i % 2 == 0) {
                $nbPlantsT ++;
            } else {
                $nbPlantsB ++;
            }
        }
    }
    $topMarge = $nbPlantsB * 95;
    $output = '';
    $output .= '<div class="ias">
            <div class="graphic-title">
                '.t('Individual And Appropriate Systems:').' '.uwwtd_format_number($totalIAS, 0).' p.e ('.uwwtd_format_number($node->field_aggc2['und'][0]['value'], 1).'%)
            </div>
            <img width="1098px" src="'.$src.'/images/graphic/ias.png" alt="reseau">
        </div>
    <div class="graphic-container">
        <div class="agglomeration-graphic">';
    if ($node->field_aggart3compliance['und'][0]['value'] == 'NC') {
        $output .= '<img width="270px" src="'.$src.'/images/graphic/reseau-nc.png" alt="reseau">';
    } elseif ($node->field_aggart3compliance['und'][0]['value'] == 'QC') {
        $output .= '<img width="270px" src="'.$src.'/images/graphic/reseau-c.png" alt="reseau">';
    } else {
        $output .= '<img width="270px" src="'.$src.'/images/graphic/reseau.png" alt="reseau">';
    }

    $output.= '<div class="graphic-title">
            '.l($node->title, "node/".$node->nid).'<br>
            Generated load : '.uwwtd_format_number($node->field_agggenerated['und'][0]['value'], 0).' p.e
        </div>
        <div class="agglo-load">
            Collective system :<br>'.uwwtd_format_number($node->field_agggenerated['und'][0]['value'] / 100 * $node->field_aggc1['und'][0]['value'], 0).' p.e <br>('.uwwtd_format_number($node->field_aggc1['und'][0]['value'], 1) .'%)
        </div>
    </div>
    <div class="connectors" style="margin-top: -'.$topMarge.'px; top: '.$topMarge.'px;">';

    for ($i=0; $i < $nbPlantsT; $i++) {
        $output .= '<img width="150px" src="'.$src.'/images/graphic/topcap.png" alt="reseau">';
    }

    if ($nbPlants > 0) {
        $output .= '<img width="150px" src="'.$src.'/images/graphic/single.png" alt="reseau">';
    }

    for ($i=0; $i < $nbPlantsB; $i++) {
        $output .= '<img width="150px" src="'.$src.'/images/graphic/botcap.png" alt="reseau">';
    }

    $output .= '</div>
            <div class="stations">';

    $offset = 0;
    if ($nbPlants % 2 == 0) {
        $offset = 50;
    }

    foreach ($reseau as $station) {
        $output .= '<div class="station" style="position: relative; top: -'.$offset.'px">';

        if ($station['collectingSystem'] == 'ISCON') {
            if ($station['compStation'] == 'C') {
                $output .= '<img src="'.$src.'/images/graphic/station-c.png" alt="reseau" title="Main treatment">';
            } elseif ($station['compStation'] == 'NC') { 
                $output .= '<img src="'.$src.'/images/graphic/station-nc.png" alt="reseau" title="Main treatment">';
            } else {
                $output .= '<img src="'.$src.'/images/graphic/station.png" alt="reseau" title="Main treatment">';
            }
        } else {
            if ($station['compStation'] == 'C') {
                $output .= '<img src="'.$src.'/images/graphic/station-notcon-c.png" alt="reseau" title="Main treatment">';
            } elseif ($station['compStation'] == 'NC') { 
                $output .= '<img src="'.$src.'/images/graphic/station-notcon-nc.png" alt="reseau" title="Main treatment">';
            } else {
                $output .= '<img src="'.$src.'/images/graphic/station-notcon.png" alt="reseau" title="Main treatment">';
            }
        }

        $output .= '<div class="graphic-title">
                '.l($station['title'], "node/".$station['nid']).'
            </div>
            <div class="station-load">Load entering from:<br>'.$node->title.'<br>'.uwwtd_format_number($station['loadEntering'], 0).' p.e <br>('.uwwtd_format_number($station['percEntering'], 1).'%)</div>
            </div>';
    }

    $output .= '</div>';

    $output .= '<div class="ms-wrapper">';
    foreach ($reseau as $station) {
        $output .= '<div class="moreAgg" style="top: -'.($offset - 2).'px;">';
        if ($station['hasMoreStringent'] === true) {
            foreach ($station['mstype'] as $nameMS => $moreStringentStatus) {
                if ($moreStringentStatus['treatment'] === '1') {
                    $output .= '<div class="msbox" >';
                    if ($moreStringentStatus['performance'] == 'P') {
                        $output .= '<img src="'.$src.'/images/graphic/msagg-small-c.png" alt="reseau" title="More stringent treatment : Performance & Type">';
                    } elseif ($moreStringentStatus['performance'] == 'F') {
                        $output .= '<img src="'.$src.'/images/graphic/msagg-small-nc.png" alt="reseau" title="More stringent treatment : Performance & Type">';
                    } else {
                        $output .= '<img src="'.$src.'/images/graphic/msagg-small.png" alt="reseau" title="More stringent treatment : Performance & Type">';
                    }
                    $output .= '<div class="more-text">'.t($nameMS).'</div>';
                    $output .= '</div>';
                }
            }
        } else {
            $output .= '<div style="top: -'.($offset - 2).'px"><img class="single" width="150px" src="'.$src.'/images/graphic/singleagg.png" alt="reseau"></div>';
        }
        $output .= '</div>';
    }

    $output .= '</div>';

    //$offset = $nbPlants * 16;
    $totalDcps = 0;
    $totalDcpsT = 0;
    $totalDcpsB = 0;

    $output .= '<div class="dcp-connections">';
    foreach ($reseau as $station) {
        $nbDcps = count($station['dcps']);
        $totalDcps = $totalDcps + $nbDcps;
        $nbDcpsT = 0;
        $nbDcpsB = 0;
        for ($i=0; $i <= $nbDcps; $i++) {
            if ($i > 1) {
                if ($i % 2 == 0) {
                    $nbDcpsT ++;
                    $totalDcpsT++;
                } else {
                    $nbDcpsB ++;
                    $totalDcpsB++;
                }
            }
        }

        $output .= '<div class="station-dcp-connections" style="top: -'.($offset - 2).'px">';
        $output .= '<div style="top: -'.($offset - 2).'px">';

        // for ($i=0; $i < $nbDcpsT; $i++) {
        //   $output .= '<img width="75px" src="'.$src.'/images/graphic/topcapsmall.png" alt="reseau">';
        // }

        // if($nbDcps > 0){
        $output .= '<img src="'.$src.'/images/graphic/singlesmallagg.png" alt="reseau">';
        // }

        // for ($i=0; $i < $nbDcpsB; $i++) {
        //   $output .= '<img width="75px" src="'.$src.'/images/graphic/botcapsmall.png" alt="reseau">';
        // }

        $output .= '</div>
                </div>';
    }

    $output .= '</div>
            <div class="dcps-wrapper">';

    foreach ($reseau as $station) {
        $output .= '<div class="station-dcp" style="top: -'.($offset - 2).'px;">
                <div>
                    <img width="80px" src="'.$src.'/images/graphic/dcp.png" alt="reseau" title="Discharge point">';
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
                    '.t('Discharge without treatment:').' '.uwwtd_format_number($totalWOT, 0).' p.e ('.uwwtd_format_number($node->field_aggpercwithouttreatment['und'][0]['value'], 1).'%)
                </div>
                <img width="1098px" src="'.$src.'/images/graphic/wot.png" alt="reseau">
                <div class="graphic-legend">
                    <ul><b>Main treatment :</b>
                        <li><img src="'.$src.'/images/graphic/station-c.png" alt="reseau"> : Compliant</li>
                        <li><img src="'.$src.'/images/graphic/station-nc.png" alt="reseau"> : Not compliant</li>
                        <li><img src="'.$src.'/images/graphic/station.png" alt="reseau"> : No information / Not relevant</li>
                    </ul>
                    <ul><b>More stringent treatment : <br>Type : </b>
                        <li><span>N</span> : Nitrogen removal</li>
                        <li><span>P</span> : Phosphorus removal</li>
                        <li><span>Other</span> : Other more stringent</li>
                        <li><span>UV</span> : UV treatment</li>
                    </ul>
                    <ul>
                        <b><br>Type :</b>
                        <li><span>OZONE</span> : Ozonation</li>
                        <li><span>SAND</span> : Sand filtration</li>
                        <li><span>MICRO</span> : Micro Filtration</li>
                        <li><span>CHLOR</span> : Chlorination</li>
                    </ul>
                    <ul><b>Discharge point :</b>
                        <li><img src="'.$src.'/images/graphic/dcp.png" alt="reseau"> : Discharge point</li>
                        <li><span>DCP(S)</span> : Discharge point(s)</li>
                    </ul>
                </div>
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
            <div class="field-item even">'.uwwtd_format_number($field[0]['#markup'], 1). $pe .'</div>
        </div>
    </div>';
}

function uwwtd_field_num($field)
{
//     $field[0]['#markup'] = 'Not provided';
    return uwwtd_format_number($field[0]['#markup'], 1);
}

function uwwtd_field_pe($field, $format=true)
{
//     $field[0]['#markup'] = 'Not provided';
    $pe = '';
	
    if (true === is_numeric($field[0]['#markup'])) {
        $pe = $field['#items'][0]['value'] / 100 * $field['#object']->field_agggenerated['und'][0]['value'];
    }elseif(true === is_numeric($field[0]['#markup']['und'][0]['value'])){
		$pe = $field[0]['#markup']['und'][0]['value'] / 100 * $field['#object']['und'][0]['value'];
	}
	if($format==true){
		return uwwtd_format_number($pe);
	}else{
		return $pe;
	}
    
}
// function uwwtd_preprocess_field(&$vars) {
//     dpl($vars);
//   if ($vars['element']['#field_name'] == 'field_my_custom_field') {
//     $vars['items'][0]['#markup'] = "I changed the output of my field!";
//   }
// }

function uwwtd_piechart_agglonode($node, &$content){
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    drupal_add_js('sites/all/libraries/d3/d3.v3.min.js');
    drupal_add_js(drupal_get_path('module', 'd3')."/js/d3.js");
    drupal_add_js(drupal_get_path('module', 'd3')."/libraries/d3.extend/d3.extend.js");
    drupal_add_js(drupal_get_path('module', 'd3')."/libraries/d3.tooltip/tooltip.js");
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');

    $aData = array();
    $aData[] = array(
        "value" => (float)$node->field_aggc1['und'][0]['value'],
        "label" => $GLOBALS['ms_level']['connection']['cs']['label'],
        "color" => $GLOBALS['ms_level']['connection']['cs']['color'],
        "valueformat" => ($node->field_aggc1['und'][0]['value'] == 0 ? '' : uwwtd_format_number($node->field_aggc1['und'][0]['value']) . ' %'),
    );
    $aData[] = array(
        "value" => (float)$node->field_aggc2['und'][0]['value'],
        "label" => $GLOBALS['ms_level']['connection']['ias']['label'],
        "color" => $GLOBALS['ms_level']['connection']['ias']['color'],
        "valueformat" =>  ($node->field_aggc2['und'][0]['value'] == 0 ? '' : uwwtd_format_number($node->field_aggc2['und'][0]['value']) . ' %'),
    );
    $aData[] = array(
        "value" => (float)$node->field_aggpercwithouttreatment['und'][0]['value'],
        "label" => $GLOBALS['ms_level']['connection']['wot']['label'],
        "color" => $GLOBALS['ms_level']['connection']['wot']['color'],
        "valueformat" =>  ($node->field_aggpercwithouttreatment['und'][0]['value'] == 0 ? '' : uwwtd_format_number($node->field_aggpercwithouttreatment['und'][0]['value']) . ' %'),
    );

    return "
    <script>
    jQuery(document).ready(function(){
        display_agglo_piechart(".json_encode($aData).");
    });
    </script>
    ";
}

function uwwtd_stackedbar_uwwtpnode($node){
  //dsm($node);
    //drupal_add_js('sites/all/libraries/d3/d3.v3.min.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
// field_aggc1|Collective system|#74FFE0
// field_aggc2|Individual and Appropriate Systems (IAS)|#BD8842
// field_aggpercwithouttreatment|Discharge without treatment|#C00000
  $legend =  array (
        'incoming',
        'discharged'
  );


    $aData = array();
    $aData[] = array(
        'BOD',
        (float)(isset($node->field_uwwbodincoming['und'][0]['value']) ? $node->field_uwwbodincoming['und'][0]['value'] : 0),
        (float)(isset($node->field_uwwboddischarge['und'][0]['value']) ? $node->field_uwwboddischarge['und'][0]['value'] : 0),
    );
    $aData[] = array(
        'COD',
        (float)(isset($node->field_uwwcodincoming['und'][0]['value']) ? $node->field_uwwcodincoming['und'][0]['value'] : 0),
        (float)(isset($node->field_uwwcoddischarge['und'][0]['value']) ? $node->field_uwwcoddischarge['und'][0]['value'] : 0),
    );
    $aData[] = array(
        'N',
        (float)(isset($node->field_uwwnincoming['und'][0]['value']) ? $node->field_uwwnincoming['und'][0]['value'] : 0),
        (float)(isset($node->field_uwwndischarge['und'][0]['value']) ? $node->field_uwwndischarge['und'][0]['value'] : 0),
    );
    $aData[] = array(
        'P',
        (float)(isset($node->field_uwwpincoming['und'][0]['value']) ? $node->field_uwwpincoming['und'][0]['value'] : 0),
        (float)(isset($node->field_uwwpdischarge['und'][0]['value']) ? $node->field_uwwpdischarge['und'][0]['value'] : 0),
    );
    //$aColor['domain'] = array ('incoming', 'discharged');
    //$aColor['range'] = array ('#74FFE0', '#C00000');


  $chart = array(
    'id' => 'uwwtp_ent_dis_chart',
    'type' => 'ColumnChart',
    'legend' => $legend,
    'rows' => $aData,
    'width'=>280,
    'height'=>300,
  );
  return d3_draw($chart);

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


function uwwtd_wkhtmltopdf_tag($selector, $options)
{
    switch (arg(0)) {
        case 'agglomerations':
        case 'uwwtps':
            return wkhtmltopdf_tag(array('.main-container', '.region-content'), $options);
            break;
    }
    return '';
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


function uwwtd_table($vars) {
  $header= $vars['header'];
  $rows= $vars['rows'];
  $attributes= $vars['attributes'];
  $caption= $vars['caption'];
  $rows_multiple= $vars['rows_multiple'];
 
  
  // Add sticky headers, if applicable.
  if (count($header)) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
//     dsm($attributes['class']);
//     $attributes['class'] = empty($attributes['class']) ? 'sticky-enabled' : ($attributes['class'] .' sticky-enabled');
    $attributes['class'] = empty($attributes['class']) ? 'sticky-enabled' : (implode(' ', $attributes['class']) .' sticky-enabled');
  }    

  $output = '<table'. drupal_attributes($attributes) .">\n";

  if (isset($caption)) {
    $output .= '<caption>'. $caption ."</caption>\n";
  }
   
    // Multiple header rows
    if($rows_multiple){
      $thead_set = '';
      // Format the table header:
      if (count($header)) {
      $output .=' <thead>';
        $ts = tablesort_init($head);
          foreach($header as $number => $head){
            
            // HTML requires that the thead tag has tr tags in it followed by tbody
            // tags. Using if clause to check and see if we have any rows and whether
            // the thead tag is already open
                $output .= ' <tr>';
        
            //$output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
            foreach ($head as $cell) {
              $cell = uwwtd_tablesort_header($cell, $head, $ts);
              $output .= _theme_table_cell($cell, TRUE);
            }
            $output .='</tr>';
        }
            // Using ternary operator to close the tags based on whether or not there are rows
            $output .= (count($rows) ? " </thead>\n" : "</tr>\n");
      }
      else {
        $ts = array();
      }
    
    }
    // One header row
    else{
         // Format the table header:
      if (count($header)) {
        $ts = tablesort_init($header);
        // HTML requires that the thead tag has tr tags in it followed by tbody
        // tags. Using ternary operator to check and see if we have any rows.
        $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
        foreach ($header as $cell) {
          $cell = uwwtd_tablesort_header($cell, $header, $ts);
          $output .= _theme_table_cell($cell, TRUE);
        }
        // Using ternary operator to close the tags based on whether or not there are rows
        $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
      }
      else {
        $ts = array();
      }
    }
   
  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';
    foreach ($rows as $number => $row) {
      $attributes = array();

      // Check if we're dealing with a simple or complex row
      if (isset($row['data'])) {
        foreach ($row as $key => $value) {
          if ($key == 'data') {
            $cells = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cells = $row;
      }
      if (count($cells)) {
        // Add odd/even class
        $class = $flip[$class];
        if (isset($attributes['class'])) {
          $attributes['class'] .= ' '. $class;
        }
        else {
          $attributes['class'] = $class;
        }

        // Build row
        $output .= ' <tr'. drupal_attributes($attributes) .'>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  $output .= "</table>\n";
  return $output;
}

function uwwtd_tablesort_header($cell, $header, $ts){
  // Special formatting for the currently sorted column header.
  if (is_array($cell) && isset($cell['field'])) {
    //Manage the sort field
    if(isset($cell['sorter'])) $order = $cell['sorter'];
    elseif(isset($cell['field'])) $order = $cell['data'];
    else $order = $cell['data'];
    $title = t('sort by @s', array('@s' => $cell['data']));
    if ($order == $ts['name'] || $order == $_REQUEST['order']) {
      $ts['sort'] = (($ts['sort'] == 'asc') ? 'desc' : 'asc');
      $cell['class'][] = 'active';
      $image = theme('tablesort_indicator', array('style' => $ts['sort']));
    }
    else {
      // If the user clicks a different header, we want to sort ascending initially.
      $ts['sort'] = 'asc';
      $image = '';
    }

    $cell['data'] = l($cell['data'] . $image, $_GET['q'], array('attributes' => array('title' => $title), 'query' => array_merge($ts['query'], array('sort' => $ts['sort'], 'order' => $order, 'sort_mod' => isset($cell['sort_mod'])?$cell['sort_mod']:'text')), 'html' => TRUE));

    unset($cell['field'], $cell['sort']);
  }
  return $cell;
}