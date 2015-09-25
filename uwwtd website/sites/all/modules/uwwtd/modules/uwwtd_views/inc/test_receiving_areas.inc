<?php

$view = new view();
$view->name = 'test_receiving_areas';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'test receiving areas';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = true; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'test receiving areas';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'node';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Position (field_position_geo) - proximity */
$handler->display->display_options['fields']['field_geofield_distance']['id'] = 'field_geofield_distance';
$handler->display->display_options['fields']['field_geofield_distance']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_geofield_distance']['field'] = 'field_geofield_distance';
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
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'receiving_area' => 'receiving_area',
);
/* Filter criterion: Content: Year of data (field_anneedata) */
$handler->display->display_options['filters']['field_anneedata_value']['id'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['table'] = 'field_data_field_anneedata';
$handler->display->display_options['filters']['field_anneedata_value']['field'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_anneedata_value']['expose']['operator_id'] = 'field_anneedata_value_op';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['label'] = 'Year of data (field_anneedata)';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['operator'] = 'field_anneedata_value_op';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['identifier'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'openlayers_map';
$handler->display->display_options['style_options']['map'] = 'receiving_area_test';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['path'] = 'test-receiving-areas';

/* Display: OpenLayers Data Overlay */
$handler = $view->new_display('openlayers', 'OpenLayers Data Overlay', 'openlayers_1');
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
  'field_specialisedzonetype' => 'field_specialisedzonetype',
  'field_uwwnremoval' => 'field_uwwnremoval',
  'field_uwwpremoval' => 'field_uwwpremoval',
  'field_rcaanitro' => 'field_rcaanitro',
  'field_rcaaphos' => 'field_rcaaphos',
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
/* Field: Content: specialisedZoneType */
$handler->display->display_options['fields']['field_specialisedzonetype']['id'] = 'field_specialisedzonetype';
$handler->display->display_options['fields']['field_specialisedzonetype']['table'] = 'field_data_field_specialisedzonetype';
$handler->display->display_options['fields']['field_specialisedzonetype']['field'] = 'field_specialisedzonetype';
$handler->display->display_options['fields']['field_specialisedzonetype']['label'] = 'Type of area';
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
/* Field: Content: Designation Criteria Eutrophication - Nitrogen */
$handler->display->display_options['fields']['field_rcaanitro']['id'] = 'field_rcaanitro';
$handler->display->display_options['fields']['field_rcaanitro']['table'] = 'field_data_field_rcaanitro';
$handler->display->display_options['fields']['field_rcaanitro']['field'] = 'field_rcaanitro';
$handler->display->display_options['fields']['field_rcaanitro']['label'] = 'Nitrogen Criteria';
/* Field: Content: Designation Criteria Eutrophication - Phosphorus */
$handler->display->display_options['fields']['field_rcaaphos']['id'] = 'field_rcaaphos';
$handler->display->display_options['fields']['field_rcaaphos']['table'] = 'field_data_field_rcaaphos';
$handler->display->display_options['fields']['field_rcaaphos']['field'] = 'field_rcaaphos';
$handler->display->display_options['fields']['field_rcaaphos']['label'] = 'Phosphorus Criteria';
$translatables['test_receiving_areas'] = array(
  t('Master'),
  t('test receiving areas'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Position (field_position_geo) - proximity'),
  t('.'),
  t(','),
  t('Year of data (field_anneedata)'),
  t('Page'),
  t('OpenLayers Data Overlay'),
  t('Position'),
  t('Type of area'),
  t('N-removal'),
  t('P-removal'),
  t('Nitrogen Criteria'),
  t('Phosphorus Criteria'),
);
