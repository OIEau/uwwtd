<?php
$view = new view();
$view->name = 'agglomeration';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Agglomeration connection map and list ';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = true; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Agglomeration connection map and list';
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
$handler->display->display_options['style_options']['uses_fields'] = TRUE;
$handler->display->display_options['style_options']['map'] = 'agglomeration';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'Population (p.e)';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Population (inhabitants) of the agglomeration */
$handler->display->display_options['fields']['field_agginhabitant']['id'] = 'field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['table'] = 'field_data_field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['field'] = 'field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['label'] = '';
$handler->display->display_options['fields']['field_agginhabitant']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_agginhabitant']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
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
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'Collecting system';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
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
$handler->display->display_options['filters']['type']['group'] = 1;
/* Filter criterion: Content: Year of data (field_anneedata) */
$handler->display->display_options['filters']['field_anneedata_value']['id'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['table'] = 'field_data_field_anneedata';
$handler->display->display_options['filters']['field_anneedata_value']['field'] = 'field_anneedata_value';
$handler->display->display_options['filters']['field_anneedata_value']['group'] = 1;
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
/* Filter criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['filters']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['filters']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['filters']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['filters']['field_agggenerated_value']['group'] = 1;
$handler->display->display_options['filters']['field_agggenerated_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['operator_id'] = 'field_agggenerated_value_op';
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['label'] = 'Generated load (p.e.) ';
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['operator'] = 'field_agggenerated_value_op';
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['identifier'] = 'field_agggenerated_value';
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Individual and Appropriate Systems (IAS) (field_aggc2) */
$handler->display->display_options['filters']['field_aggc2_value']['id'] = 'field_aggc2_value';
$handler->display->display_options['filters']['field_aggc2_value']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['filters']['field_aggc2_value']['field'] = 'field_aggc2_value';
$handler->display->display_options['filters']['field_aggc2_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggc2_value']['expose']['operator_id'] = 'field_aggc2_value_op';
$handler->display->display_options['filters']['field_aggc2_value']['expose']['label'] = 'IAS (% of p.e.)';
$handler->display->display_options['filters']['field_aggc2_value']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['field_aggc2_value']['expose']['operator'] = 'field_aggc2_value_op';
$handler->display->display_options['filters']['field_aggc2_value']['expose']['identifier'] = 'field_aggc2_value';
$handler->display->display_options['filters']['field_aggc2_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Collective system (field_aggc1) */
$handler->display->display_options['filters']['field_aggc1_value']['id'] = 'field_aggc1_value';
$handler->display->display_options['filters']['field_aggc1_value']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['filters']['field_aggc1_value']['field'] = 'field_aggc1_value';
$handler->display->display_options['filters']['field_aggc1_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggc1_value']['expose']['operator_id'] = 'field_aggc1_value_op';
$handler->display->display_options['filters']['field_aggc1_value']['expose']['label'] = 'Collective system (% of p.e.)';
$handler->display->display_options['filters']['field_aggc1_value']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['field_aggc1_value']['expose']['operator'] = 'field_aggc1_value_op';
$handler->display->display_options['filters']['field_aggc1_value']['expose']['identifier'] = 'field_aggc1_value';
$handler->display->display_options['filters']['field_aggc1_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Discharge without treatment (field_aggpercwithouttreatment) */
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['id'] = 'field_aggpercwithouttreatment_value';
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['field'] = 'field_aggpercwithouttreatment_value';
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['expose']['operator_id'] = 'field_aggpercwithouttreatment_value_op';
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['expose']['label'] = 'Not addressed through IAS (% of p.e.)';
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['expose']['operator'] = 'field_aggpercwithouttreatment_value_op';
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['expose']['identifier'] = 'field_aggpercwithouttreatment_value';
$handler->display->display_options['filters']['field_aggpercwithouttreatment_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
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
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'Population (p.e)';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Population (inhabitants) of the agglomeration */
$handler->display->display_options['fields']['field_agginhabitant']['id'] = 'field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['table'] = 'field_data_field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['field'] = 'field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['label'] = '';
$handler->display->display_options['fields']['field_agginhabitant']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_agginhabitant']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
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
/* Field: Content: Article 6 compliance */
$handler->display->display_options['fields']['field_aggart6compliance']['id'] = 'field_aggart6compliance';
$handler->display->display_options['fields']['field_aggart6compliance']['table'] = 'field_data_field_aggart6compliance';
$handler->display->display_options['fields']['field_aggart6compliance']['field'] = 'field_aggart6compliance';
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
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['label'] = 'IAS (% of p.e.)';
$handler->display->display_options['fields']['field_aggc2']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'Collected through collecting system (% of p.e.)';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
$handler->display->display_options['path'] = 'agglomerations/treatment_view';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Agglomeration connection map and list';
$handler->display->display_options['menu']['weight'] = '3';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: Premier plan (overlay) de données OpenLayers */
$handler = $view->new_display('openlayers', 'Premier plan (overlay) de données OpenLayers', 'openlayers_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Agglomeration connection map and list';
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
  'field_agggenerated' => 'field_agggenerated',
  'field_agginhabitant' => 'field_agginhabitant',
  'field_aggcompliance' => 'field_aggcompliance',
  'field_aggart3compliance' => 'field_aggart3compliance',
  'field_aggart4compliance' => 'field_aggart4compliance',
  'field_aggart5compliance' => 'field_aggart5compliance',
  'field_position_geo' => 'field_position_geo',
  'field_aggc2' => 'field_aggc2',
  'field_aggc1' => 'field_aggc1',
  'field_aggpercwithouttreatment' => 'field_aggpercwithouttreatment',
);
$handler->display->display_options['row_options']['hide_empty'] = TRUE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['empty'] = FALSE;
/* No results behavior: Global: PHP */
$handler->display->display_options['empty']['php']['id'] = 'php';
$handler->display->display_options['empty']['php']['table'] = 'views';
$handler->display->display_options['empty']['php']['field'] = 'php';
$handler->display->display_options['empty']['php']['empty'] = TRUE;
$handler->display->display_options['empty']['php']['php_output'] = '<h4>There is no result for the selected criteria. Modify the criteria them orclick on <b>Reset</b></h4>

';

/* Display: Chart */
$handler = $view->new_display('attachment', 'Chart', 'attachment_2');
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'charts';
$handler->display->display_options['style_options']['display_in_single_chart'] = 0;
$handler->display->display_options['style_options']['missing_default_value'] = '';
$handler->display->display_options['style_options']['height'] = '200';
$handler->display->display_options['style_options']['type'] = array(
  'bluff' => 'line',
  'flot' => 'pie',
);
$handler->display->display_options['style_options']['showlegend'] = 1;
$handler->display->display_options['style_options']['zoom'] = 0;
$handler->display->display_options['style_options']['views_charts_series_fields'] = array(
  'field_aggc1' => 'field_aggc1',
  'field_aggc2' => 'field_aggc2',
  'field_aggpercwithouttreatment' => 'field_aggpercwithouttreatment',
);
$handler->display->display_options['style_options']['views_charts_x_labels'] = 'field_aggc1';
$handler->display->display_options['style_options']['engine'] = 'flot';
$handler->display->display_options['style_options']['y_min'] = '';
$handler->display->display_options['style_options']['y_legend'] = '';
$handler->display->display_options['style_options']['series_colours'] = 'Collective system|#74FFE0,Individual and Appropriate Systems (IAS)|#BD8842,Discharge without treatment|#C00000';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['group_type'] = 'avg';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['group_type'] = 'avg';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['group_type'] = 'avg';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: Chart data */
$handler = $view->new_display('attachment', 'Chart data', 'attachment_3');
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'field_aggc1' => 'field_aggc1',
  'field_aggc2' => 'field_aggc2',
  'field_aggpercwithouttreatment' => 'field_aggpercwithouttreatment',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'field_aggc1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggc2' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggpercwithouttreatment' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['group_type'] = 'avg';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'Collecting system';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['group_type'] = 'avg';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['group_type'] = 'avg';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: Table */
$handler = $view->new_display('attachment', 'Table', 'attachment_1');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_inspireidlocalid' => 'field_inspireidlocalid',
  'field_agggenerated' => 'field_agggenerated',
  'field_agginhabitant' => 'field_agginhabitant',
  'field_regionnuts' => 'field_regionnuts',
  'field_position_geo' => 'field_position_geo',
  'field_aggc2' => 'field_aggc2',
  'field_aggc1' => 'field_aggc1',
  'field_aggpercwithouttreatment' => 'field_aggpercwithouttreatment',
);
$handler->display->display_options['style_options']['default'] = 'field_agggenerated';
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
  'field_agggenerated' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_agginhabitant' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
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
  'field_position_geo' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggc2' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggc1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggpercwithouttreatment' => array(
    'sortable' => 1,
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
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'Generated Load (p.e.)';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Population (inhabitants) of the agglomeration */
$handler->display->display_options['fields']['field_agginhabitant']['id'] = 'field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['table'] = 'field_data_field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['field'] = 'field_agginhabitant';
$handler->display->display_options['fields']['field_agginhabitant']['label'] = '';
$handler->display->display_options['fields']['field_agginhabitant']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_agginhabitant']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
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
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['label'] = 'IAS (% of p.e.)';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'Collected through collecting system (% of p.e.)';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
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
$handler->display->display_options['style_options']['filename'] = 'agglomeration_treatment.xml';
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['transform'] = 1;
$handler->display->display_options['style_options']['root_node'] = 'agglomerations';
$handler->display->display_options['style_options']['item_node'] = 'agglomeration';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'population';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['label'] = 'compliance';
/* Field: Content: Article 3 compliance */
$handler->display->display_options['fields']['field_aggart3compliance']['id'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['table'] = 'field_data_field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['field'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['label'] = 'article_3_compliance';
/* Field: Content: Article 4 compliance */
$handler->display->display_options['fields']['field_aggart4compliance']['id'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['table'] = 'field_data_field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['field'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['label'] = 'article_4_compliance';
/* Field: Content: Article 5 compliance */
$handler->display->display_options['fields']['field_aggart5compliance']['id'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['table'] = 'field_data_field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['field'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['label'] = 'article_5_compliance';
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['label'] = 'ias';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'collecting_system';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['label'] = 'non_ias';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_treatment_xml_view';
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
$handler->display->display_options['style_options']['replace_newlines'] = 1;
$handler->display->display_options['style_options']['header'] = 1;
$handler->display->display_options['style_options']['keep_html'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'population';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['label'] = 'compliance';
/* Field: Content: Article 3 compliance */
$handler->display->display_options['fields']['field_aggart3compliance']['id'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['table'] = 'field_data_field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['field'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['label'] = 'article_3_compliance';
/* Field: Content: Article 4 compliance */
$handler->display->display_options['fields']['field_aggart4compliance']['id'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['table'] = 'field_data_field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['field'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['label'] = 'article_4_compliance';
/* Field: Content: Article 5 compliance */
$handler->display->display_options['fields']['field_aggart5compliance']['id'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['table'] = 'field_data_field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['field'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['label'] = 'article_5_compliance';
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['label'] = 'ias';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'collecting_system';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['label'] = 'non_ias';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_treatment_csv_view';
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
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'population';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['label'] = 'compliance';
/* Field: Content: Article 3 compliance */
$handler->display->display_options['fields']['field_aggart3compliance']['id'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['table'] = 'field_data_field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['field'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['label'] = 'article_3_compliance';
/* Field: Content: Article 4 compliance */
$handler->display->display_options['fields']['field_aggart4compliance']['id'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['table'] = 'field_data_field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['field'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['label'] = 'article_4_compliance';
/* Field: Content: Article 5 compliance */
$handler->display->display_options['fields']['field_aggart5compliance']['id'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['table'] = 'field_data_field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['field'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['label'] = 'article_5_compliance';
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['label'] = 'ias';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'collecting_system';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['label'] = 'non_ias';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_treatment_xls_view';
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
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'name';
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'population';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['label'] = 'compliance';
/* Field: Content: Article 3 compliance */
$handler->display->display_options['fields']['field_aggart3compliance']['id'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['table'] = 'field_data_field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['field'] = 'field_aggart3compliance';
$handler->display->display_options['fields']['field_aggart3compliance']['label'] = 'article_3_compliance';
/* Field: Content: Article 4 compliance */
$handler->display->display_options['fields']['field_aggart4compliance']['id'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['table'] = 'field_data_field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['field'] = 'field_aggart4compliance';
$handler->display->display_options['fields']['field_aggart4compliance']['label'] = 'article_4_compliance';
/* Field: Content: Article 5 compliance */
$handler->display->display_options['fields']['field_aggart5compliance']['id'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['table'] = 'field_data_field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['field'] = 'field_aggart5compliance';
$handler->display->display_options['fields']['field_aggart5compliance']['label'] = 'article_5_compliance';
/* Field: Content: Individual and Appropriate Systems (IAS) */
$handler->display->display_options['fields']['field_aggc2']['id'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['table'] = 'field_data_field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['field'] = 'field_aggc2';
$handler->display->display_options['fields']['field_aggc2']['label'] = 'ias';
$handler->display->display_options['fields']['field_aggc2']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Collective system */
$handler->display->display_options['fields']['field_aggc1']['id'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['table'] = 'field_data_field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['field'] = 'field_aggc1';
$handler->display->display_options['fields']['field_aggc1']['label'] = 'collecting_system';
$handler->display->display_options['fields']['field_aggc1']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
/* Field: Content: Discharge without treatment */
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['id'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['table'] = 'field_data_field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['field'] = 'field_aggpercwithouttreatment';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['label'] = 'non_ias';
$handler->display->display_options['fields']['field_aggpercwithouttreatment']['settings'] = array(
  'thousand_separator' => ' ',
  'decimal_separator' => '.',
  'scale' => '2',
  'prefix_suffix' => 1,
);
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_treatment_doc_view';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$translatables['agglomeration'] = array(
  t('Master'),
  t('Agglomeration connection map and list'),
  t('plus'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Population (p.e)'),
  t('Compliance'),
  t('Article 3 compliance'),
  t('Article 4 compliance'),
  t('Article 5 compliance'),
  t('Individual and Appropriate Systems (IAS)'),
  t('Collecting system'),
  t('Discharge without treatment'),
  t('Year of data'),
  t('Generated load (p.e.) '),
  t('IAS (% of p.e.)'),
  t('Collective system (% of p.e.)'),
  t('Not addressed through IAS (% of p.e.)'),
  t('Page'),
  t('more'),
  t('Article 6 compliance'),
  t('Collected through collecting system (% of p.e.)'),
  t('Premier plan (overlay) de données OpenLayers'),
  t('Chart'),
  t('Collective system'),
  t('Chart data'),
  t('Table'),
  t('Name'),
  t('Identifier Code'),
  t('Generated Load (p.e.)'),
  t('Region (NUTS)'),
  t('xml'),
  t('Identifier'),
  t('name'),
  t('population'),
  t('compliance'),
  t('article_3_compliance'),
  t('article_4_compliance'),
  t('article_5_compliance'),
  t('ias'),
  t('collecting_system'),
  t('non_ias'),
  t('csv'),
  t('xls'),
  t('doc'),
);
