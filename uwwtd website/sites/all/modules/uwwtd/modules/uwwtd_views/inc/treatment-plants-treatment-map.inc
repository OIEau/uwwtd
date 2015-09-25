<?php
$view = new view();
$view->name = 'uwwtp_treatment';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'UWWTD Treatment Plants - Treatment map';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = true; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'UWWTP';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['use_more_text'] = 'plus';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'time';
$handler->display->display_options['cache']['results_lifespan'] = '21600';
$handler->display->display_options['cache']['results_lifespan_custom'] = '0';
$handler->display->display_options['cache']['output_lifespan'] = '-1';
$handler->display->display_options['cache']['output_lifespan_custom'] = '0';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
$handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'openlayers_map';
$handler->display->display_options['style_options']['map'] = 'uwwtp_treatment_map';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
/* Field: Content: Organic design capacity (p.e.) */
$handler->display->display_options['fields']['field_physicalcapacityinput']['id'] = 'field_physicalcapacityinput';
$handler->display->display_options['fields']['field_physicalcapacityinput']['table'] = 'field_data_field_physicalcapacityinput';
$handler->display->display_options['fields']['field_physicalcapacityinput']['field'] = 'field_physicalcapacityinput';
$handler->display->display_options['fields']['field_physicalcapacityinput']['label'] = '';
$handler->display->display_options['fields']['field_physicalcapacityinput']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_physicalcapacityinput']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'uwwtp' => 'uwwtp',
);
/* Filter criterion: Content: Year of data (field_anneedata) */
$handler->display->display_options['filters']['field_anneedata_value']['id'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['table'] = 'field_data_field_anneedata';
$handler->display->display_options['filters']['field_anneedata_value']['field'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['value'] = array(
  2010 => '2010',
);
$handler->display->display_options['filters']['field_anneedata_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_anneedata_value']['expose']['operator_id'] = 'field_anneedata_value_op';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['label'] = 'Year of data';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['operator'] = 'field_anneedata_value_op';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['identifier'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Load entering UWWTP (p.e.) (field_uwwloadenteringuwwtp) */
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['id'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['field'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['expose']['operator_id'] = 'field_uwwloadenteringuwwtp_value_op';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['expose']['label'] = 'Load entering (p.e.)';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['expose']['operator'] = 'field_uwwloadenteringuwwtp_value_op';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['expose']['identifier'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Primary treatment (field_uwwprimarytreatment) */
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['id'] = 'field_uwwprimarytreatment_value';
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['field'] = 'field_uwwprimarytreatment_value';
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['expose']['operator_id'] = 'field_uwwprimarytreatment_value_op';
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['expose']['label'] = 'Primary treatment';
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['expose']['operator'] = 'field_uwwprimarytreatment_value_op';
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['expose']['identifier'] = 'field_uwwprimarytreatment_value';
$handler->display->display_options['filters']['field_uwwprimarytreatment_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Secondary treatment (field_uwwsecondarytreatment) */
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['id'] = 'field_uwwsecondarytreatment_value';
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['field'] = 'field_uwwsecondarytreatment_value';
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['expose']['operator_id'] = 'field_uwwsecondarytreatment_value_op';
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['expose']['label'] = 'Secondary treatment';
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['expose']['operator'] = 'field_uwwsecondarytreatment_value_op';
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['expose']['identifier'] = 'field_uwwsecondarytreatment_value';
$handler->display->display_options['filters']['field_uwwsecondarytreatment_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: N removal (field_uwwnremoval) */
$handler->display->display_options['filters']['field_uwwnremoval_value']['id'] = 'field_uwwnremoval_value';
$handler->display->display_options['filters']['field_uwwnremoval_value']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['filters']['field_uwwnremoval_value']['field'] = 'field_uwwnremoval_value';
$handler->display->display_options['filters']['field_uwwnremoval_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwnremoval_value']['expose']['operator_id'] = 'field_uwwnremoval_value_op';
$handler->display->display_options['filters']['field_uwwnremoval_value']['expose']['label'] = 'N-removal';
$handler->display->display_options['filters']['field_uwwnremoval_value']['expose']['operator'] = 'field_uwwnremoval_value_op';
$handler->display->display_options['filters']['field_uwwnremoval_value']['expose']['identifier'] = 'field_uwwnremoval_value';
$handler->display->display_options['filters']['field_uwwnremoval_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: P removal (field_uwwpremoval) */
$handler->display->display_options['filters']['field_uwwpremoval_value']['id'] = 'field_uwwpremoval_value';
$handler->display->display_options['filters']['field_uwwpremoval_value']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['filters']['field_uwwpremoval_value']['field'] = 'field_uwwpremoval_value';
$handler->display->display_options['filters']['field_uwwpremoval_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwpremoval_value']['expose']['operator_id'] = 'field_uwwpremoval_value_op';
$handler->display->display_options['filters']['field_uwwpremoval_value']['expose']['label'] = 'P-removal';
$handler->display->display_options['filters']['field_uwwpremoval_value']['expose']['operator'] = 'field_uwwpremoval_value_op';
$handler->display->display_options['filters']['field_uwwpremoval_value']['expose']['identifier'] = 'field_uwwpremoval_value';
$handler->display->display_options['filters']['field_uwwpremoval_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Physical Capacity (p.e.) (field_physicalcapacityactivity) */
$handler->display->display_options['filters']['field_physicalcapacityactivity_value']['id'] = 'field_physicalcapacityactivity_value';
$handler->display->display_options['filters']['field_physicalcapacityactivity_value']['table'] = 'field_data_field_physicalcapacityactivity';
$handler->display->display_options['filters']['field_physicalcapacityactivity_value']['field'] = 'field_physicalcapacityactivity_value';
$handler->display->display_options['filters']['field_physicalcapacityactivity_value']['operator'] = 'longerthan';
$handler->display->display_options['filters']['field_physicalcapacityactivity_value']['value']['min'] = '1';
$handler->display->display_options['filters']['field_physicalcapacityactivity_value']['value']['max'] = '1';
$handler->display->display_options['filters']['field_physicalcapacityactivity_value']['value']['value'] = '1';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'uwwtps/treatment_view';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'UWWTP treatment type';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: UWWTP treatment type layer */
$handler = $view->new_display('openlayers', 'UWWTP treatment type layer', 'openlayers_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'UWWTP treatment type';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'openlayers_data';
$handler->display->display_options['style_options']['data_source'] = array(
  'value' => 'wkt',
  'other_lat' => 'title',
  'other_lon' => 'title',
  'wkt' => 'field_position_geo',
  'other_top' => 'title',
  'other_right' => 'title',
  'other_bottom' => 'title',
  'other_left' => 'title',
  'name_field' => 'title',
  'description_field' => '#row',
  'style_field' => '',
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['inline'] = array(
  'title' => 'title',
  'field_position_geo' => 'field_position_geo',
  'field_uwwtreatmenttype' => 'field_uwwtreatmenttype',
  'field_physicalcapacityactivity' => 'field_physicalcapacityactivity',
  'field_uwwcompliance' => 'field_uwwcompliance',
  'field_uwwspecification' => 'field_uwwspecification',
  'field_uwwprimarytreatment' => 'field_uwwprimarytreatment',
  'field_uwwsecondarytreatment' => 'field_uwwsecondarytreatment',
  'field_uwwnremoval' => 'field_uwwnremoval',
  'field_uwwpremoval' => 'field_uwwpremoval',
);
$handler->display->display_options['row_options']['hide_empty'] = TRUE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Treatment type in place */
$handler->display->display_options['fields']['field_uwwtreatmenttype']['id'] = 'field_uwwtreatmenttype';
$handler->display->display_options['fields']['field_uwwtreatmenttype']['table'] = 'field_data_field_uwwtreatmenttype';
$handler->display->display_options['fields']['field_uwwtreatmenttype']['field'] = 'field_uwwtreatmenttype';
/* Field: Content: Physical Capacity (p.e.) */
$handler->display->display_options['fields']['field_physicalcapacityactivity']['id'] = 'field_physicalcapacityactivity';
$handler->display->display_options['fields']['field_physicalcapacityactivity']['table'] = 'field_data_field_physicalcapacityactivity';
$handler->display->display_options['fields']['field_physicalcapacityactivity']['field'] = 'field_physicalcapacityactivity';
$handler->display->display_options['fields']['field_physicalcapacityactivity']['label'] = 'Physical Capacity';
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
/* Field: Content: Please specify other type of more stringent treatment */
$handler->display->display_options['fields']['field_uwwspecification']['id'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['table'] = 'field_data_field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['field'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['label'] = 'More stringent information';
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';

/* Display: Chart */
$handler = $view->new_display('attachment', 'Chart', 'attachment_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'UWWTP treatment type chart';
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'charts';
$handler->display->display_options['style_options']['height'] = '200';
$handler->display->display_options['style_options']['type'] = array(
  'bluff' => 'line',
  'flot' => 'pie',
);
$handler->display->display_options['style_options']['showlegend'] = 1;
$handler->display->display_options['style_options']['zoom'] = 0;
$handler->display->display_options['style_options']['views_charts_series_fields'] = array(
  'title' => 'title',
);
$handler->display->display_options['style_options']['views_charts_x_labels'] = 'field_uwwtreatmenttype';
$handler->display->display_options['style_options']['engine'] = 'flot';
$handler->display->display_options['style_options']['y_min'] = '';
$handler->display->display_options['style_options']['y_legend'] = '';
$handler->display->display_options['style_options']['series_colours'] = 'More stringent treatment|#5ddfff,Secondary treatment|#3cc53c,Primary treatment|#ea8b2e,No information|#d92424';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['group_type'] = 'count';
$handler->display->display_options['fields']['title']['label'] = 'Number of plants';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Treatment type in place */
$handler->display->display_options['fields']['field_uwwtreatmenttype']['id'] = 'field_uwwtreatmenttype';
$handler->display->display_options['fields']['field_uwwtreatmenttype']['table'] = 'field_data_field_uwwtreatmenttype';
$handler->display->display_options['fields']['field_uwwtreatmenttype']['field'] = 'field_uwwtreatmenttype';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: Chart data */
$handler = $view->new_display('attachment', 'Chart data', 'attachment_2');
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'views_aggregator';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_uwwtreatmenttype' => 'field_uwwtreatmenttype',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
    'has_aggr' => 0,
    'aggr' => array(
      'views_aggregator_count' => 'views_aggregator_count',
    ),
    'aggr_par' => '',
    'has_aggr_column' => 1,
    'aggr_column' => 'views_aggregator_sum',
    'aggr_par_column' => '',
  ),
  'field_uwwtreatmenttype' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
    'has_aggr' => 0,
    'aggr' => array(
      'views_aggregator_group_and_compress' => 'views_aggregator_group_and_compress',
    ),
    'aggr_par' => '',
    'has_aggr_column' => 0,
    'aggr_column' => 'views_aggregator_sum',
    'aggr_par_column' => '',
  ),
);
$handler->display->display_options['style_options']['totals_per_page'] = '1';
$handler->display->display_options['style_options']['totals_row_position'] = array(
  1 => 0,
  2 => '2',
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['group_type'] = 'count';
$handler->display->display_options['fields']['title']['label'] = 'Number of plants';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Treatment type in place */
$handler->display->display_options['fields']['field_uwwtreatmenttype']['id'] = 'field_uwwtreatmenttype';
$handler->display->display_options['fields']['field_uwwtreatmenttype']['table'] = 'field_data_field_uwwtreatmenttype';
$handler->display->display_options['fields']['field_uwwtreatmenttype']['field'] = 'field_uwwtreatmenttype';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: Table */
$handler = $view->new_display('attachment', 'Table', 'attachment_3');
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_inspireidlocalid' => 'field_inspireidlocalid',
  'field_uwwloadenteringuwwtp' => 'field_uwwloadenteringuwwtp',
  'field_regionnuts' => 'field_regionnuts',
  'field_uwwprimarytreatment' => 'field_uwwprimarytreatment',
  'field_uwwsecondarytreatment' => 'field_uwwsecondarytreatment',
  'field_uwwnremoval' => 'field_uwwnremoval',
  'field_uwwpremoval' => 'field_uwwpremoval',
  'field_uwwspecification' => 'field_uwwspecification',
);
$handler->display->display_options['style_options']['default'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_inspireidlocalid' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwloadenteringuwwtp' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_regionnuts' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwprimarytreatment' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwsecondarytreatment' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwnremoval' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwpremoval' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwspecification' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['sticky'] = TRUE;
$handler->display->display_options['style_options']['empty_table'] = TRUE;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Name';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier Code';
/* Field: Content: Load entering UWWTP (p.e.) */
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['id'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['field'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['label'] = 'Load entering (p.e.)';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
/* Field: Content: Please specify other type of more stringent treatment */
$handler->display->display_options['fields']['field_uwwspecification']['id'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['table'] = 'field_data_field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['field'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['label'] = 'More stringent treatment';
$handler->display->display_options['fields']['field_uwwspecification']['element_label_colon'] = FALSE;
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: xml */
$handler = $view->new_display('views_data_export', 'xml', 'views_data_export_1');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_xml';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['transform'] = 1;
$handler->display->display_options['style_options']['root_node'] = 'uwwtps';
$handler->display->display_options['style_options']['item_node'] = 'uwwtp';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier ';
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['element_label_colon'] = FALSE;
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Load entering UWWTP (p.e.) */
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['id'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['field'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['label'] = 'load_entering';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['label'] = 'primary_treatment';
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['label'] = 'secondary_treatment';
/* Field: Content: Please specify other type of more stringent treatment */
$handler->display->display_options['fields']['field_uwwspecification']['id'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['table'] = 'field_data_field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['field'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['label'] = 'More stringent treatment';
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['label'] = 'n_removal';
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['label'] = 'p_removal';
$handler->display->display_options['path'] = 'uwwtp_treatment_xml_view';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);

/* Display: csv */
$handler = $view->new_display('views_data_export', 'csv', 'views_data_export_csv');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['quote'] = 1;
$handler->display->display_options['style_options']['trim'] = 0;
$handler->display->display_options['style_options']['replace_newlines'] = 0;
$handler->display->display_options['style_options']['header'] = 1;
$handler->display->display_options['style_options']['keep_html'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier ';
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Load entering UWWTP (p.e.) */
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['id'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['field'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['label'] = 'load_entering';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['label'] = 'primary_treatment';
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['label'] = 'secondary_treatment';
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['label'] = 'n_removal';
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['label'] = 'p_removal';
/* Field: Content: Please specify other type of more stringent treatment */
$handler->display->display_options['fields']['field_uwwspecification']['id'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['table'] = 'field_data_field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['field'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['label'] = 'More stringent treatment';
$handler->display->display_options['fields']['field_uwwspecification']['element_label_colon'] = FALSE;
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['element_label_colon'] = FALSE;
$handler->display->display_options['path'] = 'uwwtp_treatment_csv_view';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);

/* Display: xls */
$handler = $view->new_display('views_data_export', 'xls', 'views_data_export_xls');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_xls';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier ';
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Load entering UWWTP (p.e.) */
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['id'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['field'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['label'] = 'load_entering';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['label'] = 'primary_treatment';
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['label'] = 'secondary_treatment';
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['label'] = 'n_removal';
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['label'] = 'p_removal';
/* Field: Content: Please specify other type of more stringent treatment */
$handler->display->display_options['fields']['field_uwwspecification']['id'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['table'] = 'field_data_field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['field'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['label'] = 'More stringent treatment';
$handler->display->display_options['fields']['field_uwwspecification']['element_label_colon'] = FALSE;
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['element_label_colon'] = FALSE;
$handler->display->display_options['path'] = 'uwwtp_treatment_xls_view';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);

/* Display: doc */
$handler = $view->new_display('views_data_export', 'doc', 'views_data_export_2');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_doc';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['filename'] = '%.doc';
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier ';
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Load entering UWWTP (p.e.) */
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['id'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['field'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['label'] = 'load_entering';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['label'] = 'primary_treatment';
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['label'] = 'secondary_treatment';
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['label'] = 'n_removal';
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['label'] = 'p_removal';
/* Field: Content: Please specify other type of more stringent treatment */
$handler->display->display_options['fields']['field_uwwspecification']['id'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['table'] = 'field_data_field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['field'] = 'field_uwwspecification';
$handler->display->display_options['fields']['field_uwwspecification']['label'] = 'More stringent treatment';
$handler->display->display_options['fields']['field_uwwspecification']['element_label_colon'] = FALSE;
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['element_label_colon'] = FALSE;
$handler->display->display_options['path'] = 'uwwtp_treatment_doc_view';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$translatables['uwwtp_treatment'] = array(
  t('Master'),
  t('UWWTP'),
  t('plus'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Position'),
  t('Year of data'),
  t('Load entering (p.e.)'),
  t('Primary treatment'),
  t('Secondary treatment'),
  t('N-removal'),
  t('P-removal'),
  t('Page'),
  t('more'),
  t('UWWTP treatment type layer'),
  t('Treatment type in place'),
  t('Physical Capacity'),
  t('Compliance'),
  t('More stringent information'),
  t('N removal'),
  t('P removal'),
  t('Chart'),
  t('UWWTP treatment type chart'),
  t('Number of plants'),
  t('Chart data'),
  t('Table'),
  t('Name'),
  t('Identifier Code'),
  t('Region (NUTS)'),
  t('More stringent treatment'),
  t('xml'),
  t('name'),
  t('Identifier '),
  t('load_entering'),
  t('primary_treatment'),
  t('secondary_treatment'),
  t('n_removal'),
  t('p_removal'),
  t('csv'),
  t('xls'),
  t('doc'),
);
