<?php

/**************************************************************
 * AGGLO virtual field
 */

/**
 *
 */
function uwwtd_virtual_field_error($node) {
//   return __FUNCTION__;
  return uwwtd_insert_errors_tab($node);
}


/**
 *
 */
function uwwtd_virtual_field_article_17($node) {
//   return __FUNCTION__;
  $html = '';

  switch ($node->type) {
    case 'agglomeration':
      if(isset($node->field_article17_agglo['und'][0]['nid'])) {
//           $html .= '
//           <fieldset class="group-aggarticle17 field-group-fieldset group-description panel panel-default form-wrapper col-sm-6">
//             <legend class="panel-heading">
//               <div class="panel-title fieldset-legend">'. t('Forward looking aspect') . '</div>
//             </legend>
//             <div class="panel-body">
//               ' . uwwtd_render_article17_aglo($node->field_article17_agglo['und'][0]['nid']) . '
//             </div>
//           </fieldset>
//           ';
          $html .= uwwtd_render_article17_aglo($node->field_article17_agglo['und'][0]['nid']);
      }
      break;
    case 'uwwtp':
      if(isset($node->field_article17_uwwtp['und'][0]['nid'])) {
//           $html .= '
//           <fieldset class="group-aggarticle17 field-group-fieldset group-description panel panel-default form-wrapper col-sm-6">
//             <legend class="panel-heading">
//               <div class="panel-title fieldset-legend">'. t('Forward looking aspect') . '</div>
//             </legend>
//             <div class="panel-body">
//               ' . uwwtd_render_article17_uwwtp($node->field_article17_uwwtp['und'][0]['nid']) . '
//             </div>
//           </fieldset>
//           ';
          $html .= uwwtd_render_article17_uwwtp($node->field_article17_uwwtp['und'][0]['nid']);
      }
     break;
    default:
        return null;
  }

  return $html;
}


/**
 *
 */
function uwwtd_virtual_field_map($node, $view_mode) {  
//   return __FUNCTION__;
  $html = '';

  switch ($node->type) {
    case 'agglomeration':
      $nodetype = t('Agglomeration');
      break;
    case 'uwwtp':
      $nodetype = t('UWWTP');
      break;
    case 'discharge_point':
      $nodetype = t('Discharge point');
      break;  
  }

  switch ($node->type) {
    case 'agglomeration':
    case 'uwwtp':
    case 'discharge_point':
        $printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
        
        if ($view_mode == 'full' && !empty($node->title)) {
            $fieldstat = field_view_field('node', $node, 'field_status');
            if (!empty($fieldstat[0]['#markup']) && $fieldstat[0]['#markup'] == '2') {
                $fieldstat[0]['#markup'] = t('Inactive');
            }
            $conn = '';
            if(isset($node->field_uwwcollectingsystem['und'])){
                $conn = $GLOBALS['uwwtd_value']['conn'][$node->field_uwwcollectingsystem['und'][0]['value']];
            }
            $fieldnuts = field_view_field('node', $node, 'field_regionnuts');
            $html = '
            <div class="google-map-banner">
            	<h1>                       
                    <span class="white-title">' .  t($nodetype) . ' : </span><span class="node-title">' .  $node->title . '</span>
                    <span class="white-title"> - ' .  t("Identifier") . ' : </span><span class="inspireidlocalid">' .  $node->field_inspireidlocalid['und'][0]['value'] . '</span>
                    <span class="white-title"> - ' .  t("Status") . ' : </span><span class="status status-' .  drupal_clean_css_identifier(strtolower($fieldstat[0]['#markup'])) . '">' .  $fieldstat[0]['#markup'] .'</span>
                    '.($conn!=''?'<span class="connection "> - ' .  $conn .'</span>':'').'
                    <span class="white-title"> - ' .  t("Reporting year") . ' : </span><span class="reporting-year">' .  $node->field_anneedata['und'][0]['value'] . '</span>
                    
            		<br />
            		<small>';
          	 if (!empty($node->field_regionnuts['und'][0]['value'])) {
                $html .= '
                      		'.	t("Region (NUTS) Code") . ' : ' .  $node->field_regionnuts['und'][0]['value'] . ' - 
                              '.    t("Region (NUTS) Name") . ' : ' .  $fieldnuts[0]['#markup'] . '
                        ';
             }
             $html .= '                       
            		</small>
            	</h1>
            </div>
            ';
        }
      break;
  }


  return $html . render($printy);
}




/**
 *
 */
function uwwtd_virtual_field_aggc1_with_pe($node, $view_mode) { 
//   return __FUNCTION__;
//   return uwwtd_render_field_with_pe($content['field_aggc1']);

  $pe = '';
  if (true === is_numeric($node->field_aggc1['und'][0]['value'])) {
    $pe = ' % (' . uwwtd_format_number($node->field_aggc1['und'][0]['value'] / 100 * $node->field_agggenerated['und'][0]['value']) . ' p.e.)';
  }

  if(isset($node->field_aggmethodc1['und']) && $node->field_aggmethodc1['und'][0]['value']!=''){
    $pe .=' '.t('based on :method method', [':method'=>$GLOBALS['methods'][$node->field_aggmethodc1['und'][0]['value']]]);  
  }

  return  uwwtd_format_number($node->field_aggc1['und'][0]['value'], 1) . $pe;
}


/**
 *
 */
function uwwtd_virtual_field_aggc2_with_pe($node) { 
//   return __FUNCTION__; 
//   return uwwtd_render_field_with_pe($content['field_aggc2']);

  $pe = '';
  if (true === is_numeric($node->field_aggc2['und'][0]['value'])) {
    $pe = ' % (' . uwwtd_format_number($node->field_aggc2['und'][0]['value'] / 100 * $node->field_agggenerated['und'][0]['value']) . ' p.e.)';
  }
  if(isset($node->field_aggmethodc2['und']) && $node->field_aggmethodc2['und'][0]['value']!=''){
    $pe .=' '.t('based on :method method', [':method'=>$GLOBALS['methods'][$node->field_aggmethodc2['und'][0]['value']]]);  
  }
  return uwwtd_format_number($node->field_aggc2['und'][0]['value'], 1) . $pe;
}


/**
 *
 */
function uwwtd_virtual_field_aggpercwithouttreatment_with_pe($node) {  
//   return __FUNCTION__; 
//   return uwwtd_render_field_with_pe($content['field_aggpercwithouttreatment']);

  $pe = '';
  if (true === is_numeric($node->field_aggpercwithouttreatment['und'][0]['value'])) {
    $pe = ' % (' . uwwtd_format_number($node->field_aggpercwithouttreatment['und'][0]['value'] / 100 * $node->field_agggenerated['und'][0]['value']) . ' p.e.)';
  }
  if(isset($node->field_aggmethodwithouttreatment['und']) && $node->field_aggmethodwithouttreatment['und'][0]['value']!=''){
    $pe .=' '.t('based on :method method', [':method'=>$GLOBALS['methods'][$node->field_aggmethodwithouttreatment['und'][0]['value']]]);  
  }
  return uwwtd_format_number($node->field_aggpercwithouttreatment['und'][0]['value'], 1) . $pe;
}


/**
 *
 */
function uwwtd_virtual_field_piechart_agglonode($node) {
//   return __FUNCTION__;
  $content = array();
  $html ='
         <div class="back">
         <div class="flip-title">' . t("Generated Load by collection type in population equivalent") . '</div>
         <div class="flip-image"><img src="' . file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png') . '" class="button-flipper chart-to-table" title="See the data table" alt="See the data table"></div>

         <svg id="agglo_piechart_back"></svg>
        ';  
  $html .=  uwwtd_piechart_agglonode($node, $content);

  $html .=  '</div>';
             
  return $html; 
}


/**
 *
 */
function uwwtd_virtual_field_aggart4compliance_hierarchical($node) { 
//   return __FUNCTION__;

  //Gestion de la "legal compliance" (hierarchical)
  $field_aggart4compliance = $node->field_aggart4compliance['und'][0]['value'];
  if($node->field_aggart3compliance['und'][0]['value']=='NC' && $field_aggart4compliance=='C'){
    $field_aggart4compliance='NC';  
  }

  $variables['element']['#items']['0']['value'] = $field_aggart4compliance;
  $variables['element']['0']['#markup'] = $field_aggart4compliance;
  $variables['items']['0']['#markup'] = $field_aggart4compliance;

  uwwtd_get_format_markup_compliance($variables);

  return $variables['items']['0']['#markup'];
}


/**
 *
 */
function uwwtd_virtual_field_aggart5compliance_hierarchical($node) {   
//   return __FUNCTION__;
  //Gestion de la "legal compliance" (hierarchical)
  $field_aggart5compliance = $node->field_aggart5compliance['und'][0]['value'];
  $msg = '';
  if($node->isInRca54){
    //$field_aggart5compliance='NR'; 
    $field_aggart523compliance = $node->field_aggart523compliance['und'][0]['value'];
    $msg.='</span> <span>('.t('Art 5.4 is applied for the receiving area. At agglomeration level the assessment is :compliance',  [':compliance'=>$GLOBALS['uwwtd']['ui']['compliance'][$field_aggart523compliance]]).')';
  }
  elseif($node->field_aggart4compliance['und'][0]['value']=='NC' && $field_aggart5compliance=='C'){
    $field_aggart5compliance='NC';  
  }

  $variables['element']['#items']['0']['value'] = $field_aggart5compliance;
  $variables['element']['0']['#markup'] = $GLOBALS['uwwtd']['ui']['compliance'][$field_aggart5compliance] . $msg;
  $variables['items']['0']['#markup'] = $GLOBALS['uwwtd']['ui']['compliance'][$field_aggart5compliance] . $msg;

  uwwtd_get_format_markup_compliance($variables);
  return $variables['items']['0']['#markup'];
}

/**
 *
 */
function uwwtd_virtual_field_table_distance_to_compliance($node) {
//   return __FUNCTION__;

  $distanceToCompliance = uwwtd_get_distance_compliance($node);
  // Set the colors of "Distance to compliance" table
  // Article 3
  if ($node->field_aggart3compliance['und'][0]['value'] == 'C'
      || $node->field_aggart3compliance['und'][0]['value'] == 'QC') 
  {
      $colorart3 = '#4f91e1';
  } elseif ($node->field_aggart3compliance['und'][0]['value'] == 'NC') {
      $colorart3 = '#d93c3c';
  } elseif ($node->field_aggart3compliance['und'][0]['value'] == 'PD' 
      && $node->field_aggpercwithouttreatment['und'][0]['value'] > 0) 
  {
      $colorart3 = '#FF5200';
  } else {
      $colorart3 = '#a2a2a2';
  }
  
  // Article 4
  if ($distanceToCompliance['art4_treatment_compliance'] == 'NC' &&
      deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === true &&
      $distanceToCompliance['art4_treat_pe'] > 0) {
      $colorart4T = '#d93c3c';
  } elseif ($distanceToCompliance['art4_treatment_compliance'] == 'NC' && 
      deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === false) {
      $colorart4T = '#FF5200';
  } else {
      $colorart4T = '#4f91e1';
  }
  if ($distanceToCompliance['art4_perf_compliance'] == 'NC' &&
      deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === true &&
      $distanceToCompliance['art4_perf_pe'] > 0) {
      $colorart4P = '#d93c3c';
  } elseif ($distanceToCompliance['art4_perf_compliance'] == 'NC' && 
          deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === false) {
      $colorart4P = '#FF5200';
  } else {
      $colorart4P = '#4f91e1';
  }
  
  if ($node->field_aggart4compliance['und'][0]['value'] == 'NR') {
      $colorart4T = '#a2a2a2';
      $colorart4P = '#a2a2a2';
  } 
  
  // Article 5
  if ($distanceToCompliance['art5_treat_compliance'] == 'NC' &&
      deadline_beforeorequal_to_referenceyear($node->field_aggperiodover5['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === true &&
      $distanceToCompliance['art5_treat_pe'] > 0) {
      $colorart5T = '#d93c3c';
  } elseif ($distanceToCompliance['art5_treat_compliance'] == 'NC' &&
      deadline_beforeorequal_to_referenceyear($node->field_aggperiodover5['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === false) {
      $colorart5T = '#FF5200';
  } else {
      $colorart5T = '#4f91e1';
  }
  
  if ($distanceToCompliance['art5_perf_compliance'] == 'NC' &&
      deadline_beforeorequal_to_referenceyear($node->field_aggperiodover5['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === true &&
      $distanceToCompliance['art5_perf_pe'] > 0) {
      $colorart5P = '#d93c3c';
  } elseif ($distanceToCompliance['art5_perf_compliance'] == 'NC' &&
      deadline_beforeorequal_to_referenceyear($node->field_aggperiodover5['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === false) {
      $colorart5P = '#FF5200';
  } else {
      $colorart5P = '#4f91e1';
  }
  
  //if ($node->field_aggart5compliance['und'][0]['value'] == 'NR' || $node->isInRca54) {
  if ($node->field_aggart5compliance['und'][0]['value'] == 'NR' ) {
      $colorart5T = '#a2a2a2';
      $colorart5P = '#a2a2a2';
  }
  elseif ($node->field_aggart5compliance['und'][0]['value'] == 'PD'){
      if($distanceToCompliance['art5_perf_pe'] > 0) $colorart5P = '#FF5200';
      if($distanceToCompliance['art5_treat_pe'] > 0) $colorart5T = '#FF5200';
  } 
  
  
  if ($distanceToCompliance['art5_perf_pe'] == '-') {
      $colorart5P = '#a2a2a2';
  }
  
  $html = '

  <div class="distance">
      <div class="field custom_tooltip_wrapper">
          <div class="field-label">' . t('Distance to target:') . '</div>
              <span class="custom_tooltip">' . t('The distance to target concept present the rate of waste water load that is:') . '
              <ul>
                  <li>' . t('adequately connected to a centralised urban waste water collecting system or addressed via Individual or Appropriate System (IAS)') . '</li>
              </ul>
              ' . t('and then when collected:') . '
              <ul>
                  <li>' . t('treated at an adequate level (secondary or more stringent treatment) as required by the directive,') . '</li>
                  <li>' . t('and with the performance requirements under tables 1 or 2 of the annex I of Directive 91/271/EEC, (UWWTD).') . '</li>
              </ul>
              ' . t('When countries joined the EU, they have obtained a delay to implement the Directive. 
              In such case the deadline for implementing the above (see in section characteristics) may be different, 
              and if the above is not already implemented for the agglomeration it is identified with orange colour in the table.') . '</span>
      </div>

      <table class="dtcomp">
          <tr>
              <th></th>
              <th>' . t('Equipment') . '</th>
              <th>' . t('Performance') . '</th>
          </tr>
          <tr>
              <td rowspan="2"><b>' . t('Collection') . '</b></td>
              <td style="background-color:' . $colorart3 . ';color:white;">' . uwwtd_format_number($node->field_aggpercwithouttreatment['und'][0]['value'], 1) . '%</td>
              <td></td>
          </tr>
          <tr>
              <td style="background-color:' . $colorart3 . ';color:white;">' . 
                  uwwtd_format_number($node->field_aggpercwithouttreatment['und'][0]['value'] / 100 *  $node->field_agggenerated['und'][0]['value'])
              . ' p.e</td>
          </tr>
          <tr>
              <td rowspan="2"><b>' . t('2nd treatment') . '</b></td>
              <td style="background-color:' . $colorart4T . ';color:white;">' . number_format($distanceToCompliance['art4_treat_percent'],1, ',', ' ') . '%</td>
              <td style="background-color:' . $colorart4P . ';color:white;">' . number_format($distanceToCompliance['art4_perf_percent'],1, ',', ' ') . '%</td>
          </tr>
          <tr>
              <td style="background-color:' . $colorart4T . ';color:white;">' . number_format($distanceToCompliance['art4_treat_pe'],0, ',', ' ') . ' p.e </td>
              <td style="background-color:' . $colorart4P . ';color:white;">' . number_format($distanceToCompliance['art4_perf_pe'],0, ',', ' ') . ' p.e </td>
          </tr>
          <tr>
              <td rowspan="2"><b>' . t('3rd treatment') . '</b></td>
              <td style="background-color:' . $colorart5T . ';color:white;">' . ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_treat_percent'],1).'%') . '</td>
              <td style="background-color:' . $colorart5P . ';color:white;">' . ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_perf_percent'],1).'%') . '</td>
          </tr>
          <tr>
              <td style="background-color:' . $colorart5T . ';color:white;">' . ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_treat_pe'],0).' p.e') . '</td>
              <td style="background-color:' . $colorart5P . ';color:white;">' . ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_perf_pe'],0).' p.e') . '</td>
          </tr>
      </table>
  </div>
  ';

  return $html;

}




/**
 *
 */
function uwwtd_virtual_field_latitude_custom($node) { 
//   return __FUNCTION__;
  $html = '';

  switch ($node->type) {
    case 'agglomeration':
      $latitude = $node->field_agglatitude['und'][0]['value'];
      $longitude = $node->field_agglongitude['und'][0]['value'];
      $html = '
      <div class="field field-name-field-agglatitude field-type-number-decimal field-label-inline clearfix">
        <div class = "field-label"><a href="' .  url('<front>') . '#zoom=17&lat=' .  $latitude . '&lon=' . $longitude . '&layers=Agglomerations&baseLayers=Google%20Maps%20Normal" target="_blank"> Latitude : </a></div>
        <div class="field-items">
          <div class="field-item even"> &nbsp;' . $latitude . '</div>
        </div>
      </div>
      ';
      break;
    case 'uwwtp':
      $latitude = $node->field_uwwlatitude['und'][0]['value'];
      $longitude = $node->field_uwwlongitude['und'][0]['value'];
      $html = '
        <div class ="field field-name-field-uwwlatitude field-type-number-decimal field-label-inline clearfix">
          <div class = "field-label"><a href="'.url('<front>').'#zoom=17&lat='.$latitude.'&lon='.$longitude.'&layers=Treatment plants&baseLayers=Google%20Maps%20Normal" target="_blank"> Latitude : </a> </div>
          <div class="field-items">
            <div class="field-item even"> &nbsp;' . $latitude .'</div>
          </div>
        </div>
      ';
     break;
    case 'discharge_point':
      $latitude = $node->field_dcplatitude['und'][0]['value'];
      $longitude = $node->field_dcplongitude['und'][0]['value'];
      $html = '
        <div class ="field field-name-field-uwwlatitude field-type-number-decimal field-label-inline clearfix">
          <div class = "field-label"><a href="'.url('<front>').'#zoom=17&lat='.$latitude.'&lon='.$longitude.'&layers=Discharge%20points&baseLayers=Google%20Maps%20Normal" target="_blank"> Latitude : </a> </div>
          <div class="field-items">
            <div class="field-item even"> &nbsp;' . $latitude .'</div>
          </div>
        </div>
      ';
     break;
  }

  return $html;
}

/**
 *
 */
function uwwtd_virtual_field_longitude_custom($node) { 
//   return __FUNCTION__;

  $html = '';

  switch ($node->type) {
    case 'agglomeration':
      $latitude = $node->field_agglatitude['und'][0]['value'];
      $longitude = $node->field_agglongitude['und'][0]['value'];
      $html = '
      <div class="field field-name-field-agglongitude field-type-number-decimal field-label-inline clearfix">
        <div class = "field-label"><a href="' .  url('<front>') . '#zoom=17&lat=' .  $latitude . '&lon=' . $longitude . '&layers=Agglomerations&baseLayers=Google%20Maps%20Normal" target="_blank"> Longitude : </a></div>
        <div class="field-items">
          <div class="field-item even"> &nbsp;' . $longitude . '</div>
        </div>
      </div>
      ';
      break;
    case 'uwwtp':
      $latitude = $node->field_uwwlatitude['und'][0]['value'];
      $longitude = $node->field_uwwlongitude['und'][0]['value'];
      $html = '
        <div class ="field field-name-field-uwwlatitude field-type-number-decimal field-label-inline clearfix">
          <div class = "field-label"><a href="'.url('<front>').'#zoom=17&lat='.$latitude.'&lon='.$longitude.'&layers=Treatment plants&baseLayers=Google%20Maps%20Normal" target="_blank"> Longitude : </a> </div>
          <div class="field-items">
            <div class="field-item even"> &nbsp;' . $longitude .'</div>
          </div>
        </div>
      ';
     break;
    case 'discharge_point':
      $latitude = $node->field_dcplatitude['und'][0]['value'];
      $longitude = $node->field_dcplongitude['und'][0]['value'];
      $html = '
        <div class ="field field-name-field-uwwlatitude field-type-number-decimal field-label-inline clearfix">
          <div class = "field-label"><a href="'.url('<front>').'#zoom=17&lat='.$latitude.'&lon='.$longitude.'&layers=Discharge%20points&baseLayers=Google%20Maps%20Normal" target="_blank"> Longitude : </a> </div>
          <div class="field-items">
            <div class="field-item even"> &nbsp;' . $longitude .'</div>
          </div>
        </div>
      ';
     break;
  }

  return $html;
}


/**
 *   agglo , uwwtp and history page aglo et uwwtp
 */
function uwwtd_virtual_compliance_time_line($node) { 
//   return __FUNCTION__;
  return uwwtd_timeline_output($node); //calll on agglo , uwwtp and history page aglo et uwwtp
}


/**
 *
 */
function uwwtd_virtual_graphic($node) { 
//   return __FUNCTION__;
  switch ($node->type) {
    case 'agglomeration':
      return uwwtd_get_agglo_graphic($node);
      break;
    case 'uwwtp':
      return uwwtd_get_uww_graphic($node);
     break;
  }
  return '';
}




/**
 *
 */
function uwwtd_virtual_field_source_file_link($node) {   
//   return __FUNCTION__;

//   switch ($node->type) {
//     case 'agglomeration':
//       
//       break;
//     case 'uwwtp':
// 
//      break;
//     case 'discharge_point':
// 
//      break;
//   }

    // Get all agglomeration with this field_inspireidlocalid :
  $sourcesFilesUri = uwwtd_get_sourcesfilesuri($node->field_inspireidlocalid[LANGUAGE_NONE][0]['value'], $node->type);
  $html = '';

  $html = '
  <div class="panel-body">
  ';

                                                  
                                                  
  if (array_key_exists($node->field_anneedata['und'][0]['value'], $sourcesFilesUri)) {
      $html .= '
      <b>' .  t('Year of data'). ':</b> ' . $sourcesFilesUri[$node->field_anneedata['und'][0]['value']]->field_anneedata_value .'
      <br/>
      <b>' . t('Source of data') . ':</b> <img src="' . url('modules/file/icons/application-octet-stream.png') . '" title="application/xml" alt="file"/>
      ' . l(t('See sourcefile'), file_create_url($sourcesFilesUri[$node->field_anneedata['und'][0]['value']]->uri)) . '
      ';
  }

  $html .= '
  <br/>
  <br/>
  <b><i>' . t('Other years'). ' :</i></b>
  <br/>
  ';

  foreach ($sourcesFilesUri as $sourceFileUri) {
      if ($sourceFileUri->field_anneedata_value != $node->field_anneedata['und'][0]['value']) {                     
          $html .= '
          <b>' . t('Year of data') . ':</b> ' . $sourceFileUri->field_anneedata_value . '
          <br/>
          <b>' . t('Source of data') . ':</b> <img src="' . url('modules/file/icons/application-octet-stream.png') . '" title="application/xml" alt="file"/>
          ' . l(t('See sourcefile'), file_create_url($sourceFileUri->uri)) . '
          <br/>
          <br/>
          ';
      }
  }

  $html .= '
  </div>  
  ';



  return $html;
}


/**
 *
 */
function uwwtd_virtual_field_aggperiodover5_custom($node) {   
//   return __FUNCTION__;
  $value = uwwtd_get_MM_DD_YYYY_from_YYYY_MM_DD_XXX($node->field_aggperiodover5['und'][0]['value']);
  if((integer)$node->field_agggenerated['und'][0]['value'] < 10000) {
    $value ='Not Relevant';
  }      

  return '<div class="field-item even">'.$value.'</div>';
}


/**************************************************************
 * UWWTP virtual field
 */

/**
 *
 */
// function uwwtd_virtual_field_error($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_map($node) {   
//   return __FUNCTION__;
// }

/**
 *
 */
// function uwwtd_virtual_compliance_time_line($node) {   
//   return __FUNCTION__;
// }


/**
 *
 */
function uwwtd_vitual_get_info_stacked_bar($node) {
    $data = array( 
    'incomingLoadBod' => (float) uwwtd_get_multiple_field_value($node, ['field_uwwbodincoming',  'field_uwwbodincomingcalculated', 'field_uwwbodincomingestimated'], -1), 
    'incomingBod' => null, //$incomingBod,
    'resultIncomingBod' => null, //$resultIncomingBod,  
    'dischargeLoadBod' =>   (float) uwwtd_get_multiple_field_value($node, ['field_uwwboddischarge', 'field_uwwboddischargecalculated', 'field_uwwboddischargeestimated'], -1), //$dischargeLoadBod,
    'dischargeBod' => null, //$dischargeBod,  
    'resultDischargedBod' =>  null, //$resultDischargedBod,
    'rateBod' =>  null, //$rateBod,  
    'incomingLoadCod' =>  (float) uwwtd_get_multiple_field_value($node, ['field_uwwcodincoming',  'field_uwwcodincomingcalculated', 'field_uwwcodincomingestimated'], -1), //$incomingLoadCod,
    'incomingCod' =>  null, //$incomingCod,  
    'resultIncomingCod' =>  null, //$resultIncomingCod,
    'dischargeLoadCod' =>  (float) uwwtd_get_multiple_field_value($node, ['field_uwwcoddischarge', 'field_uwwcoddischargecalculated', 'field_uwwcoddischargeestimated'], -1), //$dischargeLoadCod,
    'dischargeCod' =>  null, //$dischargeCod,
    'resultDischargedCod' =>  null, //$resultDischargedCod,
    'rateCod' => null, //$rateCod,
    'incomingLoadN' => (float) uwwtd_get_multiple_field_value($node, ['field_uwwnincoming',  'field_uwwnincomingcalculated', 'field_uwwnincomingestimated'], -1), //$incomingLoadN,
    'incomingN' => null, //$incomingN,
    'resultIncomingN' => null, //$resultIncomingN,
    'dischargeLoadN' =>  (float) uwwtd_get_multiple_field_value($node, ['field_uwwndischarge', 'field_uwwndischargecalculated', 'field_uwwndischargeestimated'], -1), //$dischargeLoadN,
    'dischargeN' =>  null, //$dischargeN,
    'resultDischargedN' =>  null, //$resultDischargedN,
    'rateN' =>  null, //,$rateN
    'incomingLoadP' =>  (float) uwwtd_get_multiple_field_value($node, ['field_uwwpincoming',  'field_uwwpincomingcalculated', 'field_uwwpincomingestimated'], -1), //$incomingLoadP,
    'incomingP' => null, //$incomingP,
    'resultIncomingP' => null, //$resultIncomingP,
    'dischargeLoadP' =>  (float) uwwtd_get_multiple_field_value($node, ['field_uwwpdischarge', 'field_uwwpdischargecalculated', 'field_uwwpdischargeestimated'], -1), //$dischargeLoadP,
    'dischargeP' =>  null, //$dischargeP,
    'resultDischargedP' =>  null, //$resultDischargedP,
    'rateP' =>  null, //$rateP,
  );
  if(isset($node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']!='' && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']>0){
      $v_treated = $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'];
  }else{
      $v_treated = false;
  }
  
   if($data['incomingLoadBod']>0 && $v_treated){
       $data['incomingBod'] = 1000000 * $data['incomingLoadBod'] / $v_treated;
       $data['resultIncomingBod'] = round($data['incomingBod'],2);
  }
  if($data['incomingLoadCod']>0 && $v_treated){
       $data['incomingCod'] = 1000000 * $data['incomingLoadCod'] / $v_treated;
       $data['resultIncomingCod'] = round($data['incomingCod'],2);
  }
  if($data['incomingLoadN']>0 && $v_treated){
       $data['incomingN'] = 1000000 * $data['incomingLoadN'] / $v_treated;
       $data['resultIncomingN'] = round($data['incomingN'],2);
  }
  if($data['incomingLoadP']>0  && $v_treated){
       $data['incomingP'] = 1000000 * $data['incomingLoadP'] / $v_treated;
       $data['resultIncomingP'] = round($data['incomingP'],2);
  }
  if($data['dischargeLoadBod']>0 && $v_treated){
       $data['dischargeBod'] = 1000000 * $data['dischargeLoadBod'] / $v_treated;
       $data['resultDischargedBod'] = round($data['dischargeBod'],2);
  }
  if($data['dischargeLoadCod']>0 && $v_treated){
       $data['dischargeCod'] = 1000000 * $data['dischargeLoadCod'] / $v_treated;
       $data['resultDischargedCod'] = round($data['dischargeCod'],2);
  }
  if($data['dischargeLoadN']>0 && $v_treated){
       $data['dischargeN'] = 1000000 * $data['dischargeLoadN'] / $v_treated;
       $data['resultDischargedN'] = round($data['dischargeN'],2);
  }
  if($data['dischargeLoadP']>0 && $v_treated){
       $data['dischargeP'] = 1000000 * $data['dischargeLoadP'] / $v_treated;
       $data['resultDischargedP'] = round($data['dischargeP'],2);
  }
  
  
  
  if($data['incomingLoadBod']>0 &&$data['dischargeLoadBod']>0 ){
      $data['rateBod'] = round((1-($data['dischargeLoadBod']/$data['incomingLoadBod']))*100 , 1). '%';
  }
  if($data['incomingLoadCod']>0 &&$data['dischargeLoadCod']>0 ){
      $data['rateCod'] = round((1-($data['dischargeLoadCod']/$data['incomingLoadCod']))*100 , 1). '%';
  }
  if($data['incomingLoadN']>0 &&$data['dischargeLoadN']>0 ){
      $data['rateN'] = round((1-($data['dischargeLoadN']/$data['incomingLoadN']))*100 , 1). '%';
  }
  if($data['incomingLoadP']>0 &&$data['dischargeLoadP']>0 ){
      $data['rateP'] = round((1-($data['dischargeLoadP']/$data['incomingLoadP']))*100 , 1). '%';
  }
  foreach($data as $k=>$v){
      if($v == -1){
          $data[$k] = null;
      }
  }
  //dsm($data);
  return $data;
}


/**
 *
 */
function uwwtd_virtual_field_data_stackedbar($node) {   
//   return __FUNCTION__;

  $data = uwwtd_vitual_get_info_stacked_bar($node);
  $output = '';
       
  if (isset($data['incomingLoadBod'])|| isset($data['resultIncomingBod']) || isset($data['dischargeLoadBod'])|| isset($data['resultDischargedBod'])|| 
      isset($data['incomingLoadCod'])|| isset($data['resultIncomingCod'])|| isset($data['dischargeLoadCod'])|| isset($data['resultDischargedCod'])||
      isset($data['incomingLoadN'])|| isset($data['resultIncomingN'])|| isset($data['dischargeLoadN'])|| isset($data['resultDischargedN'])||
      isset($data['incomingLoadP'])|| isset($data['resultIncomingP'])|| isset($data['dischargeLoadP'])|| isset($data['resultDischargedP'])
     )
  {
    print '<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper table-to-chart" title="See diagram" alt="See diagram"></div>';

//     $output = '<p style="font-weight:bold;">'.t("Load and concentration per parameter").' :</p>';
						  
    $output .='<table id="UwwtpDescription">
    <tr>
      <td style="border: 1px solid #000;" class="black" ></td>
      <td style="font-weight: bold;border: 1px solid #000;" class="black">'.t("Incoming").'</td>
      <td style="font-weight: bold;border: 1px solid #000;" class="black">'.t("Discharged").'</td>
      <td style="font-weight: bold;border: 1px solid #000;" class="black">'.t("Rate").'</td>
    </tr>';

    $output .='<tr>
    <td rowspan=2 style="font-weight: bold;" class="black">'.t("BOD").'</td>
      <td class="light">'.(isset($data['incomingLoadBod'])?$data['incomingLoadBod'].' '.t('t/year'):'').'</td>
      <td class="light">'.(isset($data['dischargeLoadBod'])?$data['dischargeLoadBod'].' '.t('t/year'):'').'</td>
      <td rowspan=2  class="black">'.(isset($data['rateBod'])?$data['rateBod']:'').'</td>
    </tr>';

    $output .='<tr>
      <td class="light">'.(isset($data['resultIncomingBod'])?$data['resultIncomingBod'].' '.t('mg/l'):'').'</td>
      <td class="light">'.(isset($data['resultDischargedBod'])?$data['resultDischargedBod'].' '.t('mg/l'):'').'</td>
    </tr>';

    $output .='<tr>
      <td rowspan=2 style="font-weight: bold;" class="black">'.t('COD').'</td>
      <td class="light">'.(isset($data['incomingLoadCod'])?$data['incomingLoadCod'].' '.t('t/year'):'').'</td>
      <td class="light">'.(isset($data['dischargeLoadCod'])?$data['dischargeLoadCod'].' '.t('t/year'):'').'</td>
      <td rowspan=2  class="black">'.(isset($data['rateCod'])?$data['rateCod']:'').'</td>
    </tr>';

    $output .='<tr>
      <td class="light">'.(isset($data['resultIncomingCod'])?$data['resultIncomingCod'].' '.t('mg/l'):'').'</td>
      <td class="light">'.(isset($data['resultDischargedCod'])?$data['resultDischargedCod'].' '.t('mg/l'):'').'</td>
    </tr>';

    $output .='<tr>
      <td rowspan=2 style="font-weight: bold;" class="black">'.t('Nitrogen').'</td>
      <td class="light">'.(isset($data['incomingLoadN'])?$data['incomingLoadN'].' '.t('t/year'):'').'</td>
      <td class="light">'.(isset($data['dischargeLoadN'])?$data['dischargeLoadN'].' '.t('t/year'):'').'</td>
      <td rowspan=2  class="black">'.(isset($data['rateN'])?$data['rateN']:'').'</td>
    </tr>';

    $output .='<tr>
      <td class="light">'.(isset($data['resultIncomingN'])?$data['resultIncomingN'].' '.t('mg/l'):'').'</td>
      <td class="light">'.(isset($data['resultDischargedN'])?$data['resultDischargedN'].' '.t('mg/l'):'').'</td>
    </tr>';

    $output .='<tr>
      <td rowspan=2 style="font-weight: bold;" class="black">'.t('Phosphorus').'</td>
      <td class="light">'.(isset($data['incomingLoadP'])?$data['incomingLoadP'].' '.t('t/year'):'').'</td>
      <td class="light">'.(isset($data['dischargeLoadP'])?$data['dischargeLoadP'].' '.t('t/year'):'').'</td>
      <td rowspan=2  class="black">'.(isset($data['rateP'])?$data['rateP']:'').'</td>
    </tr>';	

    $output .='<tr>
      <td class="light">'.(isset($data['resultIncomingP'])?$data['resultIncomingP'].' '.t('mg/l'):'').'</td>
      <td class="light">'.(isset($data['resultDischargedP'])?$data['resultDischargedP'].' '.t('mg/l'):'').'</td>
    </tr>';	
	
    $output .= '</table>';

    $output .= '* '.t('Concentration calculated using the annual load and the annual volume of wastewater treated.');

  }

  return $output;
}

/**
 *
 */
function uwwtd_virtual_field_stackedbar_uwwtpnode($node) {   
//   return __FUNCTION__;
  $data = uwwtd_vitual_get_info_stacked_bar($node);

  $html =  '<div class="back">';

  if (isset($data['incomingLoadBod'])|| isset($data['resultIncomingBod']) || isset($data['dischargeLoadBod'])|| isset($data['resultDischargedBod'])|| 
      isset($data['incomingLoadCod'])|| isset($data['resultIncomingCod'])|| isset($data['dischargeLoadCod'])|| isset($data['resultDischargedCod'])||
      isset($data['incomingLoadN'])|| isset($data['resultIncomingN'])|| isset($data['dischargeLoadN'])|| isset($data['resultDischargedN'])||
      isset($data['incomingLoadP'])|| isset($data['resultIncomingP'])|| isset($data['dischargeLoadP'])|| isset($data['resultDischargedP'])
     )
  {      
    $html .=   '<div class="flip-title">'.t("Incoming and discharged loads (t/year)").'</div>';
    $html .=  '<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper chart-to-table" title="'.t('See the data table').'" alt="'.t('See the data table').'"></div>';
    $html .=   uwwtd_stackedbar_uwwtpnode($node);
  } 
  $html .=  '   </div> '; 

  return $html;
}

/**
 *
 */
// function uwwtd_virtual_agglo_graphic($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_latitude_custom($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_longitude_custom($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_source_file_link($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_article_17($node) {   
//   return __FUNCTION__;
// }




/**************************************************************
 * DCP virtual field
 */

/**
 *
 */
// function uwwtd_virtual_field_error($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_map($node) {   
//   return __FUNCTION__;
// }

// /**
//  *
//  */
// function uwwtd_virtual_field_latitude_custom($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_longitude_custom($node) {   
//   return __FUNCTION__;
// }
// 
// /**
//  *
//  */
// function uwwtd_virtual_field_source_file_link($node) {   
//   return __FUNCTION__;
// }


