<?php 

/**
 * Generates the sensitive areas stats table.
 */

 
function uwwtd_get_data_sensitive_areas($option){
    //Note : Receving area = sensitive area
    
    //        dsm(__FUNCTION__);
    $cache_name = uwwtd_get_cache_key_sensitive_area($option);
    //     dsm($cache_name);
    $cache = '';
	$putincache =false;
    /*
	if(isset($option['rbd']) && $option['rbd'] !="All"){
		$putincache =false;
	}*/
    if ($putincache && UWWTD_CACHE_ACTIVE === true) {
        $cache = cache_get($cache_name, 'cache');
    }
    //     dsm($cache);
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return  $cache->data;
    }

    $data = array();
    
    //field_rca_sensitive_area
    
    $query = '
        SELECT 
            n.nid,
            n.title,
            f_annee.field_anneedata_value,
            id.field_inspireidlocalid_value,
            zt.field_specialisedzonetype_value as zonetype,
            rca52.field_rca52applied_value as rca52,
            rca54.field_rca54applied_value as rca54,
            rca58.field_rca58applied_value as rca58,
            st_astext(st_transform(f_geo.the_geom, 4326)) wkt,
            st_area(st_transform(f_geo.the_geom, 3035)) area,
            COUNT(uwwtp.field_linked_treatment_plants_nid) as uwwtps,
            SUM(capa.field_physicalcapacityactivity_value) as tot_design_capacity,
            SUM(entering.field_uwwloadenteringuwwtp_value ) as tot_entering_load
        FROM {node} n
        LEFT join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT join
            {field_data_field_inspireidlocalid} id
                ON n.nid = id.entity_id
        LEFT join
            {field_data_field_specialisedzonetype} zt
                ON n.nid = zt.entity_id
        LEFT join
            {field_data_field_rca52applied} rca52
                ON n.nid = rca52.entity_id
        LEFT join
            {field_data_field_rca54applied} rca54
                ON n.nid = rca54.entity_id
        LEFT join
            {field_data_field_rca58applied} rca58
                ON n.nid = rca58.entity_id
        LEFT JOIN 
            {field_data_field_position_geo} f_geo 
                ON n.nid = f_geo.entity_id
        LEFT join
            {field_data_field_linked_treatment_plants} uwwtp
                ON n.nid = uwwtp.entity_id
        LEFT join
            {field_data_field_physicalcapacityactivity} capa
                ON uwwtp.field_linked_treatment_plants_nid = capa.entity_id
        LEFT join
            {field_data_field_uwwloadenteringuwwtp} entering
                ON uwwtp.field_linked_treatment_plants_nid = entering.entity_id
              
        WHERE n.type = :typenode
            AND f_annee.field_anneedata_value = :annee
        GROUP BY 
            n.nid,
            n.title,
            f_annee.field_anneedata_value,
            id.field_inspireidlocalid_value,
            zt.field_specialisedzonetype_value,
            rca52.field_rca52applied_value,
            rca54.field_rca54applied_value,
            rca58.field_rca58applied_value,
            f_geo.the_geom
        ORDER BY n.title
    ';
    
    // add rca state
    
    $param = array(
        ':typenode' => 'receiving_area',
        ':annee' => $option['field_anneedata_value'],
    );
    try {
        $result = db_query($query, $param);
        /*
        Article 52 applied : traitement plus poussé sur station > 10000.
        Article 54 applied : 75% phosphore + 75 % nitrates de la somme des entering load
        Article 58 applied : état membre complet : 75% phosphore + 75 % nitrates de la somme des entering load
        */
        
        $rcatype = array_flip($GLOBALS['uwwtd_value']['rcatype']);
        while($row = $result->fetchAssoc()) {
            
            $row['tot_design_capacity_format'] =  ($row['tot_design_capacity']?uwwtd_format_number($row['tot_design_capacity']):'-');
            $row['tot_entering_load_format'] =  ($row['tot_entering_load']?uwwtd_format_number($row['tot_entering_load']):'-');
            $row['zonetype_format'] =  (isset($GLOBALS['uwwtd_value']['rcatype_labels'][$row['zonetype']])? $GLOBALS['uwwtd_value']['rcatype_labels'][$row['zonetype']] :$GLOBALS['uwwtd_value']['rcatype_labels']['default']);
            $row['rca52_format'] = $GLOBALS['uwwtd']['ui']['yes_no'][$row['rca52']];
            $row['rca54_format'] = $GLOBALS['uwwtd']['ui']['yes_no'][$row['rca54']];
            $row['rca58_format'] = $GLOBALS['uwwtd']['ui']['yes_no'][$row['rca58']];
            $row['area_format'] =  ($row['area']?uwwtd_format_number($row['area']/1000000).' km²':'-');
            $data[] = $row;
        }
		if($putincache){
			cache_set($cache_name, $data, 'cache', CACHE_PERMANENT);
		}
        
    }catch(Exception $e) {
        dsm($e->getMessage());
    }
    return $data;
    
}
 
function uwwtd_sensitive_areas_page($type_export = ''){
    $title = 'Sensitive areas';
    drupal_set_title($title);
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
//     drupal_add_js(drupal_get_path('module', 'uwwtd') . '/../../libraries/d3/d3.v3.min.js')
	$type = "receiving_area";
    $option = array();

    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['rca'];
    $option['default']['order'] = 'tot_entering_load';//$GLOBALS['uwwtd']['ui']['field']['rca']['_format'];
    $option['default']['sort_mod'] = 'numeric';
    $option['default']['sort'] = 'desc';
    $option['allyear'] = uwwtd_get_all_year();
    $option['zonetype'] = $GLOBALS['uwwtd_value']['rcatype_labels'];

    $filter = uwwtd_get_form_filter($option);
    $preset_name = 'receiving_area_test';
    $preset = openlayers_preset_load($preset_name);
    $map    = openlayers_build_map($preset->data);

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_sensitive_area($filter['filter']);

    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
//         dsm('in cache');
        $render_graph_nb = theme('flot_graph', $cache->data['flot1']);
        $render_graph = theme('flot_graph', $cache->data['flot2']);
        return $cache->data['page'];
    }
	$data = uwwtd_get_data_sensitive_areas($filter['filter']);
    $list_field = array(
        'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
        'tot_design_capacity'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_design_capacity_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'tot_entering_load'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_entering_load_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'rca52'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca52_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca54'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca54_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca58'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca58_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'zonetype'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['zonetype_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd_value']['rcatype_labels']),  
    );
// dsm($list_field);
    $form = uwwtd_filter_form($list_field, $filter);


//     dsm($data);
    //e.vincent / bruno want to show only agglo > 2000
    $filter_force = array();
    /*
    
    $filter_force['filter']['field_agggenerated_value_op'] = '>';
    $filter_force['filter']['field_agggenerated_value']['value'] = variable_get('min_aggenerated_display', '2000');
    if (variable_get('display_only_active', '1') == 1) {
        $filter_force['filter']['field_status_value'] = '1';
    }
    */
	//dsm($filter);
    uwwtd_filter_data($filter, $data, $filter_force);        
//     uwwtd_filter_data($filter, $data);
    
    /*
	  if($art17 == "No"){
		  unset($header[9]);
	  }
	  unset($header[8]);
    */
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['rca'], $data);
//     dsm($data);

    $optionstyle = array();
    //get the color for the point
    $optionstyle['color']['list'] =$GLOBALS['uwwtd_value']['rcatype_color'];
    $optionstyle['color']['field'] = 'zonetype';
    $optionstyle['style_prefix'] = 'rca_';
    //get the size of the point
/*
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['field'] = 'field_agggenerated_value';
    
*/	
    $optionstyle['popup_title_type'] = 'receiving-area';
    $optionstyle['popup'] = array(
        'field_inspireidlocalid_value' => array('label' => 'ID'),
        'zonetype_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['zonetype_format']['label']),
        'rca52_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca52_format']['label']),
        'rca54_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca54_format']['label']),
        'rca58_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca58_format']['label']),
        'area_format' => array('label' => 'Area'),
        'tot_design_capacity_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_design_capacity_format']['label']),
        'tot_entering_load_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_entering_load_format']['label']),

    );

    //todo format data in table
    $graph = array();
    foreach($data as $row) {
        $rows[] = array('data' => array(
            'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path('receiving-area', array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
            'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
            'zonetype_format'=>$row['zonetype_format'],
            'rca52_format'=>$row['rca52_format'],
            'rca54_format'=>$row['rca54_format'],
            'rca58_format'=>$row['rca58_format'],
            'uwwtps'=>$row['uwwtps'],
            'tot_design_capacity_format'=>$row['tot_design_capacity_format'],
            'tot_entering_load_format'=>$row['tot_entering_load_format']
        ));

    }

    $header = array();
    $header_export = array();
    foreach($GLOBALS['uwwtd']['ui']['field']['rca'] as $field => $info) {
        if(isset($rows[0]['data'][$field])){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }
    
    //here export data and exit
    if ($type_export != '') {
        uwwtd_export_data($type_export, 'rca_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    }

    //dsm($map);
    $features['test_receiving_areas_openlayers_1'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    $rendermap = uwwtd_build_map($map, $features);
    unset($features);
    unset($data);

    //first graph number agglo
    $data_pie = array();
    $xaxis = array();
    $i = 0;
//     dsm($optionstyle);
    foreach($graph as $compliance_format => $detail) {
        $d1 = new flotData($detail['nb']);
        $d1->label = $compliance_format;//$GLOBALS['uwwtd']['ui']['field']['compliance_label'][$compliance];//$detail['label'];
        $d1->color = $optionstyle['color']['list'][$compliance_format];//$detail['label'];
        $data_pie[] = $d1;
//         $xaxis[] = array($i++, $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$compliance]);
//         $xaxis[] = array($i++, $compliance_format.'__pppp');
    }

    $options = new flotStylePie();
//     $options->axis_range('yaxis', array(null,100), 4);
//     $options->axis_ticks('xaxis', $xaxis);
    $options->legend  = new StdClass();
    $options->legend->show = true;
    $options->legend->position = "ne";
    $options->resize = true;
    $options->legend->labelFormatter = 'uwwtd_labelformaterpie_legend';
    $options->series->pie->label->formatter = 'uwwtd_labelformaterpie';
    //!!//custom for replace string by callback in module flot...//!!//
    $options->replace = new StdClass();
    $options->replace->transform_on_callback = array($options->legend->labelFormatter, $options->series->pie->label->formatter);

    $variables_nb = array(
        'data' => $data_pie,
        'element' => array(
            'id' => 'flot-auto-identifier-1',
            'style' => 'width: 500px; height: 200px; padding: 0px; position: relative;',
        ),
//         'legend' => TRUE,
//         'zoom' => false,
        'options' => clone $options,
    );

//     $render_graph_nb = theme('flot_graph', $variables);

    $render_table_graph_nb = '

    <table id="views-aggregator-datatable-nb" class="views-table cols-2">
    <thead>
    <tr>
        <th class="views-field views-field-title">
        Number of agglomeration
        </th>
        <th class="views-field views-field-field-aggcompliance">
        Compliance
        </th>
    </tr>
    </thead>
    <tbody>
      ';
      $i = 0;
      $total = 0;
      foreach($graph as $compliance_format => $detail) {
        $render_table_graph_nb .= '<tr class="'.($i % 2 == 0 ? 'odd' : 'even').' views-row-first">
            <td class="views-field views-field-title">
            ' . uwwtd_format_number($detail['nb']) . '
            </td>
            <td class="views-field views-field-field-aggcompliance">
            '. $compliance_format . '
            </td>
        </tr>';
        $i++;
        $total += $detail['nb'];
      }

      $render_table_graph_nb .= '</tbody>
        <tfoot>
        <tr>
            <th class="views-field views-field-title">
            ' . uwwtd_format_number($total) . '
            </th>
            <th class="views-field views-field-field-aggcompliance">
            </th>
        </tr>
        </tfoot>
        </table>
        ';

    //second graph load generated
    $data_pie = array();

    $xaxis = array();
    $i = 0;
//     $aData = array();

    foreach($graph as $compliance_format => $detail) {
        $d1 = new flotData($detail['field_agggenerated_value']);
        $d1->label =$compliance_format;//$detail['label'];
        $d1->color = $optionstyle['color']['list'][$compliance_format];//$detail['label'];
        $data_pie[] = $d1;
    }

    $options = new flotStylePie();
//     $options->axis_range('yaxis', array(null,100), 4);
//     $options->axis_ticks('xaxis', $xaxis);
    $options->legend  =new StdClass();
    $options->legend->show = true;
    $options->legend->position = "ne";
    $options->resize = true;
    $options->legend->labelFormatter = 'uwwtd_labelformaterpie_legend';
    $options->series->pie->label->formatter = 'uwwtd_labelformaterpie';
    //!!//custom for replace string by callback in module flot...//!!//
    $options->replace = new StdClass();
    $options->replace->transform_on_callback = array($options->legend->labelFormatter, $options->series->pie->label->formatter);
    //!!//custom for replace string by callback in module flot...//!!//

    $variables = array(
        'data' => $data_pie,
        'element' => array(
            'id' => 'flot-auto-identifier-2',
            'style' => 'width: 500px; height: 200px; padding: 0px; position: relative;',
        ),
//         'legend' => TRUE,
//         'zoom' => false,
        'options' => clone $options,
    );

    $render_graph = theme('flot_graph', $variables);

    $render_table_graph = '

    <table id="views-aggregator-datatable" class="views-table cols-2">
    <thead>
    <tr>
        <th class="views-field views-field-title">
        Generated load (p.e.)
        </th>
        <th class="views-field views-field-field-aggcompliance">
        Compliance
        </th>
    </tr>
    </thead>
    <tbody>
      ';
      $i = 0;
      $total = 0;
      foreach($graph as $compliance_format => $detail) {
        $render_table_graph .= '<tr class="'.($i % 2 == 0 ? 'odd' : 'even').' views-row-first">
            <td class="views-field views-field-title">
            ' . uwwtd_format_number($detail['field_agggenerated_value']). '
            </td>
            <td class="views-field views-field-field-aggcompliance">
            '. $compliance_format . '
            </td>
        </tr>';
        $i++;
        $total += $detail['field_agggenerated_value'];
      }

      $render_table_graph .= '</tbody>
        <tfoot>
        <tr>
            <th class="views-field views-field-title">
            ' . uwwtd_format_number($total) . '
            </th>
            <th class="views-field views-field-field-aggcompliance">
            </th>
        </tr>
        </tfoot>
        </table>
        ';

    //dsm($rows[0]['data']);
    $build['tablesort_table'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
        );
    //build export data on ODT,XML,CSV,DOC...
    //$render_graph_nb = theme('flot_graph', $variables_nb);
    //$render_graph = theme('flot_graph', $variables);


   //============================Render the page ==> TODO use a true template/theme function 

    $page = ''
            . '<h1>'.$title.'</h1>'
            . $form
            . '<div class="rows map-row" >'.$rendermap.'</div>'
            /*
            . '<div class="rows charts-row" >'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content">'
                    .'<div class="container flip">'
                      .'<div class="leftgraph front">'
                        . '<div class="flot-title"><div class="flip-title">Global compliance by number of agglomeration (&gt2000 pe)</div>


                        <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper to-table" title="See the data table" alt="See the data table">
                        </div>'
                        . '<div class="view-content">'.$render_graph_nb.'</div>'
                      .'</div>'

                      .'<div class="leftgraph back">'
                        . '<div class="flot-title flip-title-back"><div class="flip-title">Global compliance by number of agglomeration (&gt2000 pe)</div>

                        <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper to-chart" title="See diagram" alt="See diagram">

                        </div>'
                        . '<div class="view-content">'.$render_table_graph_nb.'</div>'
                      .'</div>'
                    .'</div>'
                  .'</div>'
                .'</div>'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content">'
                    .'<div class="container flip">'

                      .'<div class="rightgraph front">'
                        . '<div class="flot-title"><div class="flip-title">Global compliance by Generated load (p.e.)</div>


                        <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper to-table" title="See the data table" alt="See the data table">
                        </div>'
                        . '<div class="view-content">'.$render_graph.'</div>'

                      .'</div>'

                      .'<div class="rightgraph back">'
                        . '<div class="flot-title flip-title-back"><div class="flip-title">Global compliance by Generated load (p.e.)</div>


                        <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper to-chart" title="See diagram" alt="See diagram">
                        </div>'
                        . '<div class="view-content">'.$render_table_graph.'</div>'
                      .'</div>'
                    .'</div>'
                  .'</div>'
                .'</div>'
            .'</div>'
            */
            . '<div class="rows data-table-row">'
			 . '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'agglomerations/compliance/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'agglomerations/compliance/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
				
                . '<div class="view-content">'. drupal_render($build) . '</div>'
			
                . '<!--markerprinttable-->'  //use for print wkhtmltopdf
            .'</div>'
     ;
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page,
                                    'flot1' => $variables_nb,
                                    'flot2' => $variables), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}

 
 
/* 
function uwwtd_sensitive_areas_page() {
  $form = drupal_get_form('uwwtd_sensitive_areas_form');
  //drupal_add_css(drupal_get_path('module', 'uwwtd').'/css/statistics.css');
  return render($form);
}
*/
/**
 * Implements hook_form().
 */
function uwwtd_sensitive_areas_form($form, &$form_state) {
  // Get available years for data
  $query = db_select('field_data_field_anneedata', 'a');
  $query->fields('a', array('field_anneedata_value'));
  $query->orderBy('field_anneedata_value', 'DESC');
  $results = $query->execute();
  while($record = $results->fetchAssoc()) {
    $years[$record['field_anneedata_value']] = $record['field_anneedata_value'];
  }

  $form['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => $years,
    '#default_value' => isset($form_state['values']['year']) ? $form_state['values']['year'] : current($years),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#attributes' => array('class' => array('btn-primary', 'btn')),
    '#value' => t('Filter')
  );

  if(isset($form_state['storage']['result'])) {
    $form['#suffix'] = $form_state['storage']['result'];
  } else {
    $form['#suffix'] = theme_datatable(uwwtd_build_sensitive_areas_table(current($years)));
  }

  return $form;
}

/**
 * Form submit.
 */
function uwwtd_sensitive_areas_form_submit(&$form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $table = uwwtd_build_sensitive_areas_table($form_state['values']['year']);
  dsm($table);
  //$form_state['storage']['result'] = theme_datatable($table);
}

function uwwtd_build_sensitive_areas_table($year = '') {
  $rows = array();
  // LG $header = array('ID', 'Name', 'UWWTPs', 'N-removal', 'P-removal', 'N & P-removal');
  $header = array('ID', 'Name', 'UWWTPs', 'N-removal', 'P-removal', 'Article 5.4 applied');

  $type = 'receiving_area';

  $query = "SELECT nid FROM {node} n";

  if($year != '') {
    $query .= " LEFT JOIN {field_data_field_anneedata} fdfa ON fdfa.entity_id = n.nid WHERE type = :type AND fdfa.field_anneedata_value = :year";
    $result = db_query($query, array(':type' => $type, ':year' => $year));
  } else{
    $query .= " WHERE type = :type";
    $result = db_query($query, array(':type' => $type));
  }

  $nids = array();

  foreach ($result as $row) {
    $nids[] = $row->nid;
  }

  $nodes = node_load_multiple($nids);

  foreach ($nodes as $key => $node) {
    // Initialize counters
    $counter_stations = 0;
    $counter_stations_n = 0;
    $counter_stations_p = 0;
    //$counter_stations_n_and_p = 0;


    // Load discharge points, stations and agglomerations
    if(isset($node->field_rcadcpliste['und'])) {
      foreach ($node->field_rcadcpliste['und'] as $key => $dp_ref) {
        $discharge_point = node_load($dp_ref['nid']);
        if(isset($discharge_point->field_dcpuwwliste['und'])) {
          foreach ($discharge_point->field_dcpuwwliste['und'] as $key => $station_ref) {
            $station = node_load($station_ref['nid']);
            if(isset($station->field_uwwpremoval['und'][0]['value']) && $station->field_uwwpremoval['und'][0]['value'] == 1) {
              $counter_stations_p++;
            }
            if(isset($station->field_uwwnremoval['und'][0]['value']) && $station->field_uwwnremoval['und'][0]['value'] == 1) {
              $counter_stations_n++;
            }

            //LG
			//if((isset($station->field_uwwnremoval['und'][0]['value']) && $station->field_uwwnremoval['und'][0]['value'] == 1) && (isset($station->field_uwwpremoval['und'][0]['value']) && $station->field_uwwpremoval['und'][0]['value'] == 1)) {
              //$counter_stations_n_and_p++;
            //}
            $counter_stations++;
          }
        }
      }
    }
   if ($counter_stations_n != 0) $counter_stations_n = "Yes"; else $counter_stations_n = "No";
   if ($counter_stations_p != 0) $counter_stations_p = "Yes"; else $counter_stations_p = "No";
   if ($node->field_rca54applied['und'][0]['value'] == 1) $display_rca54applied = "Yes"; else $display_rca54applied = "No";

    $rows[] = array(
        array('data' => $node->field_inspireidlocalid['und'][0]['value']),
        array('data' => l($node->title, 'node/'.$node->nid)),
        array('data' => $counter_stations),
        array('data' => $counter_stations_n),
        array('data' => $counter_stations_p),
        array('data' => $display_rca54applied),
      );
  }
  $table = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('class' => 'table', 'id' => 'statsTable', 'datatable_options' => array('bTableTools' => TRUE)),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  return $table;
}
