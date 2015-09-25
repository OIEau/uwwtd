<?php
$view = new view();
$view->name = 'waste_water_network';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Waste water network';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = true; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Waste water network';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'time';
$handler->display->display_options['cache']['results_lifespan'] = '518400';
$handler->display->display_options['cache']['results_lifespan_custom'] = '0';
$handler->display->display_options['cache']['output_lifespan'] = '518400';
$handler->display->display_options['cache']['output_lifespan_custom'] = '0';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'openlayers_map';
$handler->display->display_options['style_options']['map'] = 'waste_water_treatment_network_map';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['label'] = '';
$handler->display->display_options['fields']['field_position_geo']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
/* Field: Content: Receiving water */
$handler->display->display_options['fields']['field_dcpwaterbodytype']['id'] = 'field_dcpwaterbodytype';
$handler->display->display_options['fields']['field_dcpwaterbodytype']['table'] = 'field_data_field_dcpwaterbodytype';
$handler->display->display_options['fields']['field_dcpwaterbodytype']['field'] = 'field_dcpwaterbodytype';
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Year of data (field_anneedata) */
$handler->display->display_options['filters']['field_anneedata_value']['id'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['table'] = 'field_data_field_anneedata';
$handler->display->display_options['filters']['field_anneedata_value']['field'] = 'field_anneedata_value';
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

/* Display: Master */
$handler = $view->new_display('page', 'Master', 'page');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['path'] = 'network';

/* Display: Waste water network agglos */
$handler = $view->new_display('openlayers', 'Waste water network agglos', 'openlayers_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Agglomerations';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'openlayers_data';
$handler->display->display_options['style_options']['data_source'] = array(
  'value' => 'wkt',
  'other_lat' => 'field_position_geo',
  'other_lon' => 'field_position_geo',
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
  'field_agggenerated' => 'field_agggenerated',
  'field_aggcompliance' => 'field_aggcompliance',
  'field_aggart3compliance' => 'field_aggart3compliance',
  'field_aggart4compliance' => 'field_aggart4compliance',
  'field_aggart5compliance' => 'field_aggart5compliance',
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
$handler->display->display_options['fields']['field_position_geo']['label'] = '';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => '',
  'prefix_suffix' => 0,
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
/* Field: Content: Article 3 compliance */
$handler->display->display_options['fields']['field_aggart3compliance']['id'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['table'] = 'field_data_field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['field'] = 'field_aggart3compliance';
/* Field: Content: Article 4 compliance */
$handler->display->display_options['fields']['field_aggart4compliance']['id'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['table'] = 'field_data_field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['field'] = 'field_aggart4compliance';
/* Field: Content: Article 5 compliance */
$handler->display->display_options['fields']['field_aggart5compliance']['id'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['table'] = 'field_data_field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['field'] = 'field_aggart5compliance';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'agglomeration' => 'agglomeration',
);
/* Filter criterion: Content: Year of data (field_anneedata) */
$handler->display->display_options['filters']['field_anneedata_value']['id'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['table'] = 'field_data_field_anneedata';
$handler->display->display_options['filters']['field_anneedata_value']['field'] = 'field_anneedata_value';
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

/* Display: Waste water network uwwtps */
$handler = $view->new_display('openlayers', 'Waste water network uwwtps', 'openlayers_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'UWWTPs';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'openlayers_data';
$handler->display->display_options['style_options']['data_source'] = array(
  'value' => 'wkt',
  'other_lat' => 'field_position_geo',
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
  'field_uwwcompliance' => 'field_uwwcompliance',
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
$handler->display->display_options['fields']['field_position_geo']['label'] = '';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
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
$handler->display->display_options['fields']['field_uwwnremoval']['label'] = 'N-removal';
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['label'] = 'P-removal';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
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

/* Display: Waste water network discharge points */
$handler = $view->new_display('openlayers', 'Waste water network discharge points', 'openlayers_3');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Discharge points';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'openlayers_data';
$handler->display->display_options['style_options']['data_source'] = array(
  'value' => 'wkt',
  'other_lat' => 'field_position_geo',
  'other_lon' => 'title',
  'wkt' => 'field_position_geo',
  'other_top' => 'title',
  'other_right' => 'title',
  'other_bottom' => 'title',
  'other_left' => 'title',
  'name_field' => 'title',
  'description_field' => '',
  'style_field' => '',
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['inline'] = array(
  'title' => 'title',
  'field_position_geo' => 'field_position_geo',
  'field_uwwcompliance' => 'field_uwwcompliance',
  'field_dcpwaterbodytype' => 'field_dcpwaterbodytype',
  'field_rcatype' => 'field_rcatype',
);
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
$handler->display->display_options['fields']['field_position_geo']['label'] = '';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Receiving water */
$handler->display->display_options['fields']['field_dcpwaterbodytype']['id'] = 'field_dcpwaterbodytype';
$handler->display->display_options['fields']['field_dcpwaterbodytype']['table'] = 'field_data_field_dcpwaterbodytype';
$handler->display->display_options['fields']['field_dcpwaterbodytype']['field'] = 'field_dcpwaterbodytype';
$handler->display->display_options['fields']['field_dcpwaterbodytype']['label'] = 'Discharge';
/* Field: Content: Type of receiving area */
$handler->display->display_options['fields']['field_rcatype']['id'] = 'field_rcatype';
$handler->display->display_options['fields']['field_rcatype']['table'] = 'field_data_field_rcatype';
$handler->display->display_options['fields']['field_rcatype']['field'] = 'field_rcatype';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Year of data (field_anneedata) */
$handler->display->display_options['filters']['field_anneedata_value']['id'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['table'] = 'field_data_field_anneedata';
$handler->display->display_options['filters']['field_anneedata_value']['field'] = 'field_anneedata_value';
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
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'discharge_point' => 'discharge_point',
);

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'openlayers_map';
$handler->display->display_options['style_options']['map'] = 'waste_water_treatment_network_map';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['block_description'] = 'Urban waste water treatment network block';
$translatables['waste_water_network'] = array(
  t('Master'),
  t('Waste water network'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Compliance'),
  t('Receiving water'),
  t('Year of data'),
  t('Waste water network agglos'),
  t('Generated load (p.e.)'),
  t('Article 3 compliance'),
  t('Article 4 compliance'),
  t('Article 5 compliance'),
  t('Waste water network uwwtps'),
  t('Primary treatment'),
  t('Secondary treatment'),
  t('N-removal'),
  t('P-removal'),
  t('Waste water network discharge points'),
  t('Discharge'),
  t('Type of receiving area'),
  t('Block'),
  t('Urban waste water treatment network block'),
);
