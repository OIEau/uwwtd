<?php
/**
 * @file
 * uwwtd_search.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function uwwtd_search_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_default_node_index';
  $view->human_name = 'Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Displaying from @start to @end of @total results';
  /* Field: Indexed Content: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['link_to_entity'] = 0;
  /* Field: Indexed Content: Content type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['type']['format_name'] = 1;
  /* Field: Indexed Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<h3>[title] - [type]</h3>';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_entity'] = 1;
  /* Field: Indexed Content: Year of data */
  $handler->display->display_options['fields']['field_anneedata']['id'] = 'field_anneedata';
  $handler->display->display_options['fields']['field_anneedata']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['field_anneedata']['field'] = 'field_anneedata';
  $handler->display->display_options['fields']['field_anneedata']['label'] = 'Reporting year';
  /* Field: Indexed Content: Identifier of the area */
  $handler->display->display_options['fields']['field_inspireidlocalid']['id'] = 'field_inspireidlocalid';
  $handler->display->display_options['fields']['field_inspireidlocalid']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['field_inspireidlocalid']['field'] = 'field_inspireidlocalid';
  $handler->display->display_options['fields']['field_inspireidlocalid']['label'] = 'Identifier';
  /* Field: Indexed Content: Status of the receiving area */
  $handler->display->display_options['fields']['field_status']['id'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['field_status']['field'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['label'] = 'Status';
  $handler->display->display_options['fields']['field_status']['hide_empty'] = TRUE;
  /* Field: Indexed Content: Compliance */
  $handler->display->display_options['fields']['field_uwwcompliance']['id'] = 'field_uwwcompliance';
  $handler->display->display_options['fields']['field_uwwcompliance']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['field_uwwcompliance']['field'] = 'field_uwwcompliance';
  $handler->display->display_options['fields']['field_uwwcompliance']['hide_empty'] = TRUE;
  /* Field: Indexed Content: Compliance */
  $handler->display->display_options['fields']['field_aggcompliance']['id'] = 'field_aggcompliance';
  $handler->display->display_options['fields']['field_aggcompliance']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['field_aggcompliance']['field'] = 'field_aggcompliance';
  $handler->display->display_options['fields']['field_aggcompliance']['hide_empty'] = TRUE;
  /* Field: Indexed Content: Generated load (p.e.) */
  $handler->display->display_options['fields']['field_agggenerated']['id'] = 'field_agggenerated';
  $handler->display->display_options['fields']['field_agggenerated']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['field_agggenerated']['field'] = 'field_agggenerated';
  $handler->display->display_options['fields']['field_agggenerated']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_agggenerated']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Indexed Content: Load entering UWWTP (p.e.) */
  $handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['id'] = 'field_uwwloadenteringuwwtp';
  $handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['field'] = 'field_uwwloadenteringuwwtp';
  $handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_uwwloadenteringuwwtp']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Indexed Content: Content type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'search_api_index_default_node_index';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'agglomeration' => 'agglomeration',
    'big_city' => 'big_city',
    'discharge_point' => 'discharge_point',
    'receiving_area' => 'receiving_area',
    'uwwtp' => 'uwwtp',
  );
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Search in';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['type']['expose']['reduce'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['path'] = 'search';
  $translatables['search'] = array(
    t('Master'),
    t('Search'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Displaying from @start to @end of @total results'),
    t('Node ID'),
    t('.'),
    t(','),
    t('<h3>[title] - [type]</h3>'),
    t('Reporting year'),
    t('Identifier'),
    t('Status'),
    t('Compliance'),
    t('Generated load (p.e.)'),
    t('Load entering UWWTP (p.e.)'),
    t('Search in'),
    t('Page'),
  );
  $export['search'] = $view;

  return $export;
}
