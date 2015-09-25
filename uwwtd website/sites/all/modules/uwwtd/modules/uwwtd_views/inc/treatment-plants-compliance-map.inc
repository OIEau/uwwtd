<?php
$view = new view();
$view->name = 'uwwtp_map';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'UWWTD Treatment Plants- Compliance map';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = true; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'UWWTPs - compliance map';
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
$handler->display->display_options['style_options']['map'] = 'uwwtp_compliance_map';
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
/* Filter criterion: Content: Load entering UWWTP (p.e.) (field_uwwloadenteringuwwtp) */
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['id'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['field'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['filters']['field_uwwloadenteringuwwtp_value']['group'] = 1;
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
/* Filter criterion: Content: Compliance (field_uwwcompliance) */
$handler->display->display_options['filters']['field_uwwcompliance_value']['id'] = 'field_uwwcompliance_value';
$handler->display->display_options['filters']['field_uwwcompliance_value']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['filters']['field_uwwcompliance_value']['field'] = 'field_uwwcompliance_value';
$handler->display->display_options['filters']['field_uwwcompliance_value']['group'] = 1;
$handler->display->display_options['filters']['field_uwwcompliance_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwcompliance_value']['expose']['operator_id'] = 'field_uwwcompliance_value_op';
$handler->display->display_options['filters']['field_uwwcompliance_value']['expose']['label'] = 'Compliance';
$handler->display->display_options['filters']['field_uwwcompliance_value']['expose']['operator'] = 'field_uwwcompliance_value_op';
$handler->display->display_options['filters']['field_uwwcompliance_value']['expose']['identifier'] = 'field_uwwcompliance_value';
$handler->display->display_options['filters']['field_uwwcompliance_value']['expose']['remember_roles'] = array(
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
/* Filter criterion: Content: Treatment performance: BOD5 (field_uwwbod5perf) */
$handler->display->display_options['filters']['field_uwwbod5perf_value']['id'] = 'field_uwwbod5perf_value';
$handler->display->display_options['filters']['field_uwwbod5perf_value']['table'] = 'field_data_field_uwwbod5perf';
$handler->display->display_options['filters']['field_uwwbod5perf_value']['field'] = 'field_uwwbod5perf_value';
$handler->display->display_options['filters']['field_uwwbod5perf_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwbod5perf_value']['expose']['operator_id'] = 'field_uwwbod5perf_value_op';
$handler->display->display_options['filters']['field_uwwbod5perf_value']['expose']['label'] = 'Treatment performance: BOD5';
$handler->display->display_options['filters']['field_uwwbod5perf_value']['expose']['operator'] = 'field_uwwbod5perf_value_op';
$handler->display->display_options['filters']['field_uwwbod5perf_value']['expose']['identifier'] = 'field_uwwbod5perf_value';
$handler->display->display_options['filters']['field_uwwbod5perf_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
/* Filter criterion: Content: Treatment performance: COD (field_uwwcodperf) */
$handler->display->display_options['filters']['field_uwwcodperf_value']['id'] = 'field_uwwcodperf_value';
$handler->display->display_options['filters']['field_uwwcodperf_value']['table'] = 'field_data_field_uwwcodperf';
$handler->display->display_options['filters']['field_uwwcodperf_value']['field'] = 'field_uwwcodperf_value';
$handler->display->display_options['filters']['field_uwwcodperf_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwcodperf_value']['expose']['operator_id'] = 'field_uwwcodperf_value_op';
$handler->display->display_options['filters']['field_uwwcodperf_value']['expose']['label'] = 'Treatment performance: COD';
$handler->display->display_options['filters']['field_uwwcodperf_value']['expose']['operator'] = 'field_uwwcodperf_value_op';
$handler->display->display_options['filters']['field_uwwcodperf_value']['expose']['identifier'] = 'field_uwwcodperf_value';
$handler->display->display_options['filters']['field_uwwcodperf_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
/* Filter criterion: Content: Treatment performance: Total nitrogen (field_uwwntotperf) */
$handler->display->display_options['filters']['field_uwwntotperf_value']['id'] = 'field_uwwntotperf_value';
$handler->display->display_options['filters']['field_uwwntotperf_value']['table'] = 'field_data_field_uwwntotperf';
$handler->display->display_options['filters']['field_uwwntotperf_value']['field'] = 'field_uwwntotperf_value';
$handler->display->display_options['filters']['field_uwwntotperf_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwntotperf_value']['expose']['operator_id'] = 'field_uwwntotperf_value_op';
$handler->display->display_options['filters']['field_uwwntotperf_value']['expose']['label'] = 'Treatment performance: Total Nitrogen';
$handler->display->display_options['filters']['field_uwwntotperf_value']['expose']['operator'] = 'field_uwwntotperf_value_op';
$handler->display->display_options['filters']['field_uwwntotperf_value']['expose']['identifier'] = 'field_uwwntotperf_value';
$handler->display->display_options['filters']['field_uwwntotperf_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
/* Filter criterion: Content: Treatment performance: Total phosphorus (field_uwwptotperf) */
$handler->display->display_options['filters']['field_uwwptotperf_value']['id'] = 'field_uwwptotperf_value';
$handler->display->display_options['filters']['field_uwwptotperf_value']['table'] = 'field_data_field_uwwptotperf';
$handler->display->display_options['filters']['field_uwwptotperf_value']['field'] = 'field_uwwptotperf_value';
$handler->display->display_options['filters']['field_uwwptotperf_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwptotperf_value']['expose']['operator_id'] = 'field_uwwptotperf_value_op';
$handler->display->display_options['filters']['field_uwwptotperf_value']['expose']['label'] = 'Treatment performance: Total Phosphorus';
$handler->display->display_options['filters']['field_uwwptotperf_value']['expose']['operator'] = 'field_uwwptotperf_value_op';
$handler->display->display_options['filters']['field_uwwptotperf_value']['expose']['identifier'] = 'field_uwwptotperf_value';
$handler->display->display_options['filters']['field_uwwptotperf_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
/* Filter criterion: Content: Treatment type in place (field_uwwtreatmenttype) */
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['id'] = 'field_uwwtreatmenttype_value';
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['table'] = 'field_data_field_uwwtreatmenttype';
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['field'] = 'field_uwwtreatmenttype_value';
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['expose']['operator_id'] = 'field_uwwtreatmenttype_value_op';
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['expose']['label'] = 'Treatment type in place';
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['expose']['operator'] = 'field_uwwtreatmenttype_value_op';
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['expose']['identifier'] = 'field_uwwtreatmenttype_value';
$handler->display->display_options['filters']['field_uwwtreatmenttype_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'uwwtps/compliance_view';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'UWWTP compliance map';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: UWWTP Compliance layer */
$handler = $view->new_display('openlayers', 'UWWTP Compliance layer', 'openlayers_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'UWWTP compliance';
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
  'path' => 'path',
  'title' => 'title',
  'field_position_geo' => 'field_position_geo',
  'field_uwwcompliance' => 'field_uwwcompliance',
  'field_uwwloadenteringuwwtp' => 'field_uwwloadenteringuwwtp',
  'field_physicalcapacityactivity' => 'field_physicalcapacityactivity',
  'field_uwwprimarytreatment' => 'field_uwwprimarytreatment',
  'field_uwwsecondarytreatment' => 'field_uwwsecondarytreatment',
  'field_uwwnremoval' => 'field_uwwnremoval',
  'field_uwwpremoval' => 'field_uwwpremoval',
  'field_uwwbod5perf' => 'field_uwwbod5perf',
  'field_uwwcodperf' => 'field_uwwcodperf',
  'field_uwwntotperf' => 'field_uwwntotperf',
  'field_uwwptotperf' => 'field_uwwptotperf',
);
$handler->display->display_options['row_options']['hide_empty'] = TRUE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Path */
$handler->display->display_options['fields']['path']['id'] = 'path';
$handler->display->display_options['fields']['path']['table'] = 'node';
$handler->display->display_options['fields']['path']['field'] = 'path';
$handler->display->display_options['fields']['path']['label'] = '';
$handler->display->display_options['fields']['path']['exclude'] = TRUE;
$handler->display->display_options['fields']['path']['element_label_colon'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['text'] = '<a href="[path]">[title]</a>';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
/* Field: Content: Load entering UWWTP (p.e.) */
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['id'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['field'] = 'field_uwwloadenteringuwwtp';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['label'] = 'Load entering (p.e.)';
$handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Physical Capacity (p.e.) */
$handler->display->display_options['fields']['field_physicalcapacityactivity']['id'] = 'field_physicalcapacityactivity';
$handler->display->display_options['fields']['field_physicalcapacityactivity']['table'] = 'field_data_field_physicalcapacityactivity';
$handler->display->display_options['fields']['field_physicalcapacityactivity']['field'] = 'field_physicalcapacityactivity';
$handler->display->display_options['fields']['field_physicalcapacityactivity']['label'] = 'Capacity';
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['exclude'] = TRUE;
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['exclude'] = TRUE;
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['exclude'] = TRUE;
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['exclude'] = TRUE;
/* Field: Content: Treatment performance: BOD5 */
$handler->display->display_options['fields']['field_uwwbod5perf']['id'] = 'field_uwwbod5perf';
$handler->display->display_options['fields']['field_uwwbod5perf']['table'] = 'field_data_field_uwwbod5perf';
$handler->display->display_options['fields']['field_uwwbod5perf']['field'] = 'field_uwwbod5perf';
$handler->display->display_options['fields']['field_uwwbod5perf']['label'] = 'Performance BOD5';
/* Field: Content: Treatment performance: COD */
$handler->display->display_options['fields']['field_uwwcodperf']['id'] = 'field_uwwcodperf';
$handler->display->display_options['fields']['field_uwwcodperf']['table'] = 'field_data_field_uwwcodperf';
$handler->display->display_options['fields']['field_uwwcodperf']['field'] = 'field_uwwcodperf';
$handler->display->display_options['fields']['field_uwwcodperf']['label'] = 'Performance COD';
/* Field: Content: Treatment performance: Total nitrogen */
$handler->display->display_options['fields']['field_uwwntotperf']['id'] = 'field_uwwntotperf';
$handler->display->display_options['fields']['field_uwwntotperf']['table'] = 'field_data_field_uwwntotperf';
$handler->display->display_options['fields']['field_uwwntotperf']['field'] = 'field_uwwntotperf';
$handler->display->display_options['fields']['field_uwwntotperf']['label'] = 'Performance nitrate';
/* Field: Content: Treatment performance: Total phosphorus */
$handler->display->display_options['fields']['field_uwwptotperf']['id'] = 'field_uwwptotperf';
$handler->display->display_options['fields']['field_uwwptotperf']['table'] = 'field_data_field_uwwptotperf';
$handler->display->display_options['fields']['field_uwwptotperf']['field'] = 'field_uwwptotperf';
$handler->display->display_options['fields']['field_uwwptotperf']['label'] = 'Performance phosphore';

/* Display: Chart */
$handler = $view->new_display('attachment', 'Chart', 'attachment_1');
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
$handler->display->display_options['style_options']['views_charts_x_labels'] = 'field_uwwcompliance';
$handler->display->display_options['style_options']['engine'] = 'flot';
$handler->display->display_options['style_options']['y_min'] = '';
$handler->display->display_options['style_options']['y_legend'] = '';
$handler->display->display_options['style_options']['series_colours'] = 'Compliant|#4f91e1,Not compliant|#d93c3c,Not relevant|#a2a2a2,No information|#6b6b6b,Compliant on equipment only|#ea8b2e';
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
$handler->display->display_options['fields']['title']['label'] = 'Number of plant';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
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
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'views_aggregator';
$handler->display->display_options['style_options']['columns'] = array(
  'field_compliance' => 'field_compliance',
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'field_compliance' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
    'has_aggr' => 0,
    'aggr' => array(
      'views_aggregator_first' => 'views_aggregator_first',
    ),
    'aggr_par' => '',
    'has_aggr_column' => 0,
    'aggr_column' => 'views_aggregator_sum',
    'aggr_par_column' => '',
  ),
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
    'has_aggr' => 0,
    'aggr' => array(
      'views_aggregator_replace' => 'views_aggregator_replace',
    ),
    'aggr_par' => '',
    'has_aggr_column' => 1,
    'aggr_column' => 'views_aggregator_sum',
    'aggr_par_column' => '',
  ),
);
$handler->display->display_options['style_options']['totals_per_page'] = '0';
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
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['group_columns'] = array(
  'value' => 'value',
);
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
  'field_uwwcompliance' => 'field_uwwcompliance',
  'field_position_geo' => 'field_position_geo',
  'field_uwwprimarytreatment' => 'field_uwwprimarytreatment',
  'field_uwwsecondarytreatment' => 'field_uwwsecondarytreatment',
  'field_uwwnremoval' => 'field_uwwnremoval',
  'field_uwwpremoval' => 'field_uwwpremoval',
  'field_uwwmicrofiltration' => 'field_uwwmicrofiltration',
  'field_uwwbod5perf' => 'field_uwwbod5perf',
  'field_uwwcodperf' => 'field_uwwcodperf',
  'field_uwwntotperf' => 'field_uwwntotperf',
  'field_uwwptotperf' => 'field_uwwptotperf',
  'field_uwwtreatmenttype' => 'field_uwwtreatmenttype',
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
  'field_uwwcompliance' => array(
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
  'field_uwwmicrofiltration' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwbod5perf' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwcodperf' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwntotperf' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwptotperf' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwtreatmenttype' => array(
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
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
/* Field: Content: Position */
$handler->display->display_options['fields']['field_position_geo']['id'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['table'] = 'field_data_field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['field'] = 'field_position_geo';
$handler->display->display_options['fields']['field_position_geo']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_position_geo']['click_sort_column'] = 'geom';
$handler->display->display_options['fields']['field_position_geo']['settings'] = array(
  'data' => 'full',
);
/* Field: Content: Primary treatment */
$handler->display->display_options['fields']['field_uwwprimarytreatment']['id'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['table'] = 'field_data_field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['field'] = 'field_uwwprimarytreatment';
$handler->display->display_options['fields']['field_uwwprimarytreatment']['exclude'] = TRUE;
/* Field: Content: Secondary treatment */
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['id'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['table'] = 'field_data_field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['field'] = 'field_uwwsecondarytreatment';
$handler->display->display_options['fields']['field_uwwsecondarytreatment']['exclude'] = TRUE;
/* Field: Content: N removal */
$handler->display->display_options['fields']['field_uwwnremoval']['id'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['table'] = 'field_data_field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['field'] = 'field_uwwnremoval';
$handler->display->display_options['fields']['field_uwwnremoval']['exclude'] = TRUE;
/* Field: Content: P removal */
$handler->display->display_options['fields']['field_uwwpremoval']['id'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['table'] = 'field_data_field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['field'] = 'field_uwwpremoval';
$handler->display->display_options['fields']['field_uwwpremoval']['exclude'] = TRUE;
/* Field: Content: Micro filtration */
$handler->display->display_options['fields']['field_uwwmicrofiltration']['id'] = 'field_uwwmicrofiltration';
$handler->display->display_options['fields']['field_uwwmicrofiltration']['table'] = 'field_data_field_uwwmicrofiltration';
$handler->display->display_options['fields']['field_uwwmicrofiltration']['field'] = 'field_uwwmicrofiltration';
$handler->display->display_options['fields']['field_uwwmicrofiltration']['label'] = 'Microbiological removal';
$handler->display->display_options['fields']['field_uwwmicrofiltration']['exclude'] = TRUE;
/* Field: Content: Treatment performance: BOD5 */
$handler->display->display_options['fields']['field_uwwbod5perf']['id'] = 'field_uwwbod5perf';
$handler->display->display_options['fields']['field_uwwbod5perf']['table'] = 'field_data_field_uwwbod5perf';
$handler->display->display_options['fields']['field_uwwbod5perf']['field'] = 'field_uwwbod5perf';
$handler->display->display_options['fields']['field_uwwbod5perf']['label'] = 'Performance: BOD5';
/* Field: Content: Treatment performance: COD */
$handler->display->display_options['fields']['field_uwwcodperf']['id'] = 'field_uwwcodperf';
$handler->display->display_options['fields']['field_uwwcodperf']['table'] = 'field_data_field_uwwcodperf';
$handler->display->display_options['fields']['field_uwwcodperf']['field'] = 'field_uwwcodperf';
$handler->display->display_options['fields']['field_uwwcodperf']['label'] = 'Performance: COD';
/* Field: Content: Treatment performance: Total nitrogen */
$handler->display->display_options['fields']['field_uwwntotperf']['id'] = 'field_uwwntotperf';
$handler->display->display_options['fields']['field_uwwntotperf']['table'] = 'field_data_field_uwwntotperf';
$handler->display->display_options['fields']['field_uwwntotperf']['field'] = 'field_uwwntotperf';
$handler->display->display_options['fields']['field_uwwntotperf']['label'] = 'Performance: Total nitrogen';
/* Field: Content: Treatment performance: Total phosphorus */
$handler->display->display_options['fields']['field_uwwptotperf']['id'] = 'field_uwwptotperf';
$handler->display->display_options['fields']['field_uwwptotperf']['table'] = 'field_data_field_uwwptotperf';
$handler->display->display_options['fields']['field_uwwptotperf']['field'] = 'field_uwwptotperf';
$handler->display->display_options['fields']['field_uwwptotperf']['label'] = 'Performance: Total phosphorus';
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

/* Display: xml */
$handler = $view->new_display('views_data_export', 'xml', 'views_data_export_1');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_xml';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['filename'] = 'uwwtp_compliance.xml';
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
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
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
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['label'] = 'compliance';
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
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Load entering UWWTP (p.e.) (field_uwwloadenteringuwwtp) */
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['id'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['field'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'uwwtp_compliance_xml_view';
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
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
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
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['label'] = 'compliance';
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
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Load entering UWWTP (p.e.) (field_uwwloadenteringuwwtp) */
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['id'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['field'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'uwwtp_compliance_csv_view';
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
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
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
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['label'] = 'compliance';
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
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Load entering UWWTP (p.e.) (field_uwwloadenteringuwwtp) */
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['id'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['field'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'uwwtp_compliance_xls_view';
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
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
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
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'field_data_field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
$handler->display->display_options['fields']['field_uwwcompliance']['label'] = 'compliance';
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
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Load entering UWWTP (p.e.) (field_uwwloadenteringuwwtp) */
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['id'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['table'] = 'field_data_field_uwwloadenteringuwwtp';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['field'] = 'field_uwwloadenteringuwwtp_value';
$handler->display->display_options['sorts']['field_uwwloadenteringuwwtp_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'uwwtp_compliance_doc_view';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$translatables['uwwtp_map'] = array(
  t('Master'),
  t('UWWTPs - compliance map'),
  t('plus'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Position'),
  t('Year of data'),
  t('Load entering (p.e.)'),
  t('Compliance'),
  t('Treatment performance: BOD5'),
  t('Treatment performance: COD'),
  t('Treatment performance: Total Nitrogen'),
  t('Treatment performance: Total Phosphorus'),
  t('Treatment type in place'),
  t('Page'),
  t('more'),
  t('UWWTP Compliance layer'),
  t('<a href="[path]">[title]</a>'),
  t('Capacity'),
  t('Primary treatment'),
  t('Secondary treatment'),
  t('N removal'),
  t('P removal'),
  t('Performance BOD5'),
  t('Performance COD'),
  t('Performance nitrate'),
  t('Performance phosphore'),
  t('Chart'),
  t('Number of plant'),
  t('Chart data'),
  t('Number of plants'),
  t('Table'),
  t('Name'),
  t('Identifier Code'),
  t('Region (NUTS)'),
  t('Microbiological removal'),
  t('Performance: BOD5'),
  t('Performance: COD'),
  t('Performance: Total nitrogen'),
  t('Performance: Total phosphorus'),
  t('xml'),
  t('name'),
  t('Identifier'),
  t('load_entering'),
  t('compliance'),
  t('primary_treatment'),
  t('secondary_treatment'),
  t('n_removal'),
  t('p_removal'),
  t('csv'),
  t('xls'),
  t('doc'),
);
