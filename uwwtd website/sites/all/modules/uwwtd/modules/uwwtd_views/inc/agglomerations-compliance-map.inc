<?php
$view = new view();
$view->name = 'agglomeration_conformity';
$view->description = '2015-09-23 16:01:49';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'UWWTD Agglomerations - Compliance map';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = true; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Compliance Map';
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
$handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Trier par';
$handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'openlayers_map';
$handler->display->display_options['style_options']['map'] = 'agg_compliance_map';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['group_type'] = 'count';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
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
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['label'] = 'Generated load (p.e.)';
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['operator'] = 'field_agggenerated_value_op';
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['identifier'] = 'field_agggenerated_value';
$handler->display->display_options['filters']['field_agggenerated_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Compliance (field_aggcompliance) */
$handler->display->display_options['filters']['field_aggcompliance_value']['id'] = 'field_aggcompliance_value';
$handler->display->display_options['filters']['field_aggcompliance_value']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['filters']['field_aggcompliance_value']['field'] = 'field_aggcompliance_value';
$handler->display->display_options['filters']['field_aggcompliance_value']['group'] = 1;
$handler->display->display_options['filters']['field_aggcompliance_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggcompliance_value']['expose']['operator_id'] = 'field_aggcompliance_value_op';
$handler->display->display_options['filters']['field_aggcompliance_value']['expose']['label'] = 'Compliance';
$handler->display->display_options['filters']['field_aggcompliance_value']['expose']['operator'] = 'field_aggcompliance_value_op';
$handler->display->display_options['filters']['field_aggcompliance_value']['expose']['identifier'] = 'field_aggcompliance_value';
$handler->display->display_options['filters']['field_aggcompliance_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Article 3 compliance (field_aggart3compliance) */
$handler->display->display_options['filters']['field_aggart3compliance_value']['id'] = 'field_aggart3compliance_value';
$handler->display->display_options['filters']['field_aggart3compliance_value']['table'] = 'field_data_field_aggart3compliance';
$handler->display->display_options['filters']['field_aggart3compliance_value']['field'] = 'field_aggart3compliance_value';
$handler->display->display_options['filters']['field_aggart3compliance_value']['value'] = array(
  'all' => 'all',
  'NI' => 'NI',
  'C' => 'C',
  'AddQC' => 'AddQC',
  'NR' => 'NR',
);
$handler->display->display_options['filters']['field_aggart3compliance_value']['group'] = 1;
$handler->display->display_options['filters']['field_aggart3compliance_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggart3compliance_value']['expose']['operator_id'] = 'field_aggart3compliance_value_op';
$handler->display->display_options['filters']['field_aggart3compliance_value']['expose']['label'] = 'Article 3 compliance';
$handler->display->display_options['filters']['field_aggart3compliance_value']['expose']['operator'] = 'field_aggart3compliance_value_op';
$handler->display->display_options['filters']['field_aggart3compliance_value']['expose']['identifier'] = 'field_aggart3compliance_value';
$handler->display->display_options['filters']['field_aggart3compliance_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
$handler->display->display_options['filters']['field_aggart3compliance_value']['expose']['reduce'] = TRUE;
/* Filter criterion: Content: Article 4 compliance (field_aggart4compliance) */
$handler->display->display_options['filters']['field_aggart4compliance_value']['id'] = 'field_aggart4compliance_value';
$handler->display->display_options['filters']['field_aggart4compliance_value']['table'] = 'field_data_field_aggart4compliance';
$handler->display->display_options['filters']['field_aggart4compliance_value']['field'] = 'field_aggart4compliance_value';
$handler->display->display_options['filters']['field_aggart4compliance_value']['value'] = array(
  'all' => 'all',
  'C' => 'C',
  'NC' => 'NC',
  'NR' => 'NR',
  'NI' => 'NI',
);
$handler->display->display_options['filters']['field_aggart4compliance_value']['group'] = 1;
$handler->display->display_options['filters']['field_aggart4compliance_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggart4compliance_value']['expose']['operator_id'] = 'field_aggart4compliance_value_op';
$handler->display->display_options['filters']['field_aggart4compliance_value']['expose']['label'] = 'Article 4 compliance';
$handler->display->display_options['filters']['field_aggart4compliance_value']['expose']['operator'] = 'field_aggart4compliance_value_op';
$handler->display->display_options['filters']['field_aggart4compliance_value']['expose']['identifier'] = 'field_aggart4compliance_value';
$handler->display->display_options['filters']['field_aggart4compliance_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
$handler->display->display_options['filters']['field_aggart4compliance_value']['expose']['reduce'] = TRUE;
/* Filter criterion: Content: Article 5 compliance (field_aggart5compliance) */
$handler->display->display_options['filters']['field_aggart5compliance_value']['id'] = 'field_aggart5compliance_value';
$handler->display->display_options['filters']['field_aggart5compliance_value']['table'] = 'field_data_field_aggart5compliance';
$handler->display->display_options['filters']['field_aggart5compliance_value']['field'] = 'field_aggart5compliance_value';
$handler->display->display_options['filters']['field_aggart5compliance_value']['group'] = 1;
$handler->display->display_options['filters']['field_aggart5compliance_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggart5compliance_value']['expose']['operator_id'] = 'field_aggart5compliance_value_op';
$handler->display->display_options['filters']['field_aggart5compliance_value']['expose']['label'] = 'Article 5 compliance';
$handler->display->display_options['filters']['field_aggart5compliance_value']['expose']['operator'] = 'field_aggart5compliance_value_op';
$handler->display->display_options['filters']['field_aggart5compliance_value']['expose']['identifier'] = 'field_aggart5compliance_value';
$handler->display->display_options['filters']['field_aggart5compliance_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Content: Article 6 compliance (field_aggart6compliance) */
$handler->display->display_options['filters']['field_aggart6compliance_value']['id'] = 'field_aggart6compliance_value';
$handler->display->display_options['filters']['field_aggart6compliance_value']['table'] = 'field_data_field_aggart6compliance';
$handler->display->display_options['filters']['field_aggart6compliance_value']['field'] = 'field_aggart6compliance_value';
$handler->display->display_options['filters']['field_aggart6compliance_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_aggart6compliance_value']['expose']['operator_id'] = 'field_aggart6compliance_value_op';
$handler->display->display_options['filters']['field_aggart6compliance_value']['expose']['label'] = 'Article 6 compliance';
$handler->display->display_options['filters']['field_aggart6compliance_value']['expose']['operator'] = 'field_aggart6compliance_value_op';
$handler->display->display_options['filters']['field_aggart6compliance_value']['expose']['identifier'] = 'field_aggart6compliance_value';
$handler->display->display_options['filters']['field_aggart6compliance_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['path'] = 'agglomerations/compliance_view';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Agglomeration';
$handler->display->display_options['menu']['weight'] = '3';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: Premier plan (overlay) de données OpenLayers */
$handler = $view->new_display('openlayers', 'Premier plan (overlay) de données OpenLayers', 'openlayers_1');
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
  'field_aggcompliance' => 'field_aggcompliance',
  'field_aggart3compliance' => 'field_aggart3compliance',
  'field_aggart4compliance' => 'field_aggart4compliance',
  'field_aggart5compliance' => 'field_aggart5compliance',
  'field_position_geo' => 'field_position_geo',
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
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['label'] = 'Generatd Load (p.e.)';
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

/* Display: Chart */
$handler = $view->new_display('attachment', 'Chart', 'attachment_1');
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
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
$handler->display->display_options['style_options']['views_charts_x_labels'] = 'field_aggcompliance';
$handler->display->display_options['style_options']['engine'] = 'flot';
$handler->display->display_options['style_options']['y_min'] = '';
$handler->display->display_options['style_options']['y_legend'] = '';
$handler->display->display_options['style_options']['series_colours'] = 'Compliant|#4f91e1,Not relevant|#a2a2a2,Not compliant|#d93c3c,No information|#6b6b6b';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
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
  'field_aggcompliance' => 'field_aggcompliance',
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
      'views_aggregator_first' => 'views_aggregator_first',
    ),
    'aggr_par' => '',
    'has_aggr_column' => 1,
    'aggr_column' => 'views_aggregator_sum',
    'aggr_par_column' => '',
  ),
  'field_aggcompliance' => array(
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
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: Chart p.e */
$handler = $view->new_display('attachment', 'Chart p.e', 'attachment_5');
$handler->display->display_options['display_comment'] = 'Chart of the compliance by load (p.e)';
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
  'field_agggenerated' => 'field_agggenerated',
);
$handler->display->display_options['style_options']['views_charts_x_labels'] = 'field_aggcompliance';
$handler->display->display_options['style_options']['engine'] = 'flot';
$handler->display->display_options['style_options']['y_min'] = '';
$handler->display->display_options['style_options']['y_legend'] = 'test legend';
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
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['group_type'] = 'sum';
$handler->display->display_options['fields']['field_agggenerated']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => '',
  'prefix_suffix' => 1,
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: Chart p.e data */
$handler = $view->new_display('attachment', 'Chart p.e data', 'attachment_4');
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'views_aggregator';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_agggenerated' => 'field_agggenerated',
  'field_aggcompliance' => 'field_aggcompliance',
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
      'views_aggregator_first' => 'views_aggregator_first',
    ),
    'aggr_par' => '',
    'has_aggr_column' => 1,
    'aggr_column' => 'views_aggregator_sum',
    'aggr_par_column' => '',
  ),
  'field_agggenerated' => array(
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
    'has_aggr_column' => 1,
    'aggr_column' => 'views_aggregator_sum',
    'aggr_par_column' => '',
  ),
  'field_aggcompliance' => array(
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
/* Field: Content: Generated load (p.e.) */
$handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
$handler->display->display_options['fields']['field_agggenerated']['group_type'] = 'sum';
$handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Compliance */
$handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['table'] = 'field_data_field_aggcompliance';
$handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;

/* Display: Table */
$handler = $view->new_display('attachment', 'Table', 'attachment_3');
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_inspireidlocalid' => 'field_inspireidlocalid',
  'field_agggenerated' => 'field_agggenerated',
  'field_regionnuts' => 'field_regionnuts',
  'field_aggcompliance' => 'field_aggcompliance',
  'field_aggart3compliance' => 'field_aggart3compliance',
  'field_aggart4compliance' => 'field_aggart4compliance',
  'field_aggart5compliance' => 'field_aggart5compliance',
  'field_aggart6compliance' => 'field_aggart6compliance',
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
  'field_regionnuts' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggcompliance' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggart3compliance' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggart4compliance' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggart5compliance' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_aggart6compliance' => array(
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
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
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
$handler->display->display_options['style_options']['root_node'] = 'agglomerations';
$handler->display->display_options['style_options']['item_node'] = 'agglomeration';
$handler->display->display_options['defaults']['fields'] = FALSE;
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
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['label'] = 'region';
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
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_conformity_xml_view';
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
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['label'] = 'region';
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
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_conformity_csv_view';
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
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['label'] = 'region';
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
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_conformity_xls_view';
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
/* Field: Content: Region (NUTS) */
$handler->display->display_options['fields']['field_regionnuts']['id'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['table'] = 'field_data_field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['field'] = 'field_regionnuts';
$handler->display->display_options['fields']['field_regionnuts']['label'] = 'region';
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
/* Field: Content: Identifier of the big city */
$handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Generated load (p.e.) (field_agggenerated) */
$handler->display->display_options['sorts']['field_agggenerated_value']['id'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['table'] = 'field_data_field_agggenerated';
$handler->display->display_options['sorts']['field_agggenerated_value']['field'] = 'field_agggenerated_value';
$handler->display->display_options['sorts']['field_agggenerated_value']['order'] = 'DESC';
$handler->display->display_options['path'] = 'agglo_conformity_doc_view';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$translatables['agglomeration_conformity'] = array(
  t('Master'),
  t('Compliance Map'),
  t('plus'),
  t('Apply'),
  t('Reset'),
  t('Trier par'),
  t('Asc'),
  t('Desc'),
  t('Compliance'),
  t('Year of data'),
  t('Generated load (p.e.)'),
  t('Article 3 compliance'),
  t('Article 4 compliance'),
  t('Article 5 compliance'),
  t('Article 6 compliance'),
  t('Page'),
  t('more'),
  t('Premier plan (overlay) de données OpenLayers'),
  t('Generatd Load (p.e.)'),
  t('Chart'),
  t('Chart data'),
  t('Number of plants'),
  t('Chart p.e'),
  t('Chart p.e data'),
  t('Table'),
  t('Name'),
  t('Identifier Code'),
  t('Generated Load (p.e.)'),
  t('Region (NUTS)'),
  t('xml'),
  t('name'),
  t('population'),
  t('region'),
  t('compliance'),
  t('article_3_compliance'),
  t('article_4_compliance'),
  t('article_5_compliance'),
  t('Identifier'),
  t('csv'),
  t('xls'),
  t('doc'),
);
