<?php

$view = new view();
$view->name = 'error_list';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'uwwtd_import_errors';
$view->human_name = 'Error list';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Error list';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'table';
/* Relationship: uwwtd_import_errors: relationship to node table */
$handler->display->display_options['relationships']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['relationships']['entity_id']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['relationships']['entity_id']['field'] = 'entity_id';
/* Field: uwwtd_import_errors: errid */
$handler->display->display_options['fields']['errid']['id'] = 'errid';
$handler->display->display_options['fields']['errid']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['errid']['field'] = 'errid';
$handler->display->display_options['fields']['errid']['separator'] = '';
/* Field: uwwtd_import_errors: year */
$handler->display->display_options['fields']['year']['id'] = 'year';
$handler->display->display_options['fields']['year']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['year']['field'] = 'year';
$handler->display->display_options['fields']['year']['separator'] = '';
/* Field: uwwtd_import_errors: date */
$handler->display->display_options['fields']['date']['id'] = 'date';
$handler->display->display_options['fields']['date']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['date']['field'] = 'date';
$handler->display->display_options['fields']['date']['date_format'] = 'custom';
$handler->display->display_options['fields']['date']['custom_date_format'] = 'Y-m-d H:i:s';
$handler->display->display_options['fields']['date']['second_date_format'] = 'long';
/* Field: uwwtd_import_errors: category */
$handler->display->display_options['fields']['category']['id'] = 'category';
$handler->display->display_options['fields']['category']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['category']['field'] = 'category';
$handler->display->display_options['fields']['category']['alter']['alter_text'] = TRUE;
/* Field: uwwtd_import_errors: type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: uwwtd_import_errors: error */
$handler->display->display_options['fields']['error']['id'] = 'error';
$handler->display->display_options['fields']['error']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['error']['field'] = 'error';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'entity_id';
/* Field: Content: Title */
$handler->display->display_options['fields']['code']['id'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['code']['table'] = 'field_data_field_inspireidlocalid';
$handler->display->display_options['fields']['code']['field'] = 'field_inspireidlocalid';
$handler->display->display_options['fields']['code']['relationship'] = 'entity_id';
/* Field: Content: Bundle */
$handler->display->display_options['fields']['bundle']['id'] = 'bundle';
$handler->display->display_options['fields']['bundle']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['bundle']['field'] = 'bundle';
/* Field: Bulk operations: uwwtd_import_errors */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 1,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
/* Sort criterion: uwwtd_import_errors: date */
$handler->display->display_options['sorts']['date']['id'] = 'date';
$handler->display->display_options['sorts']['date']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['sorts']['date']['field'] = 'date';
$handler->display->display_options['sorts']['date']['order'] = 'DESC';
$handler->display->display_options['sorts']['date']['exposed'] = TRUE;
$handler->display->display_options['sorts']['date']['expose']['label'] = 'date';
/* Sort criterion: uwwtd_import_errors: year */
$handler->display->display_options['sorts']['year']['id'] = 'year';
$handler->display->display_options['sorts']['year']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['sorts']['year']['field'] = 'year';
$handler->display->display_options['sorts']['year']['order'] = 'DESC';
$handler->display->display_options['sorts']['year']['exposed'] = TRUE;
$handler->display->display_options['sorts']['year']['expose']['label'] = 'year';
/* Filter criterion: uwwtd_import_errors: year */
$handler->display->display_options['filters']['year']['id'] = 'year';
$handler->display->display_options['filters']['year']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['filters']['year']['field'] = 'year';
$handler->display->display_options['filters']['year']['group'] = 1;
$handler->display->display_options['filters']['year']['exposed'] = TRUE;
$handler->display->display_options['filters']['year']['expose']['operator_id'] = 'year_op';
$handler->display->display_options['filters']['year']['expose']['label'] = 'year';
$handler->display->display_options['filters']['year']['expose']['operator'] = 'year_op';
$handler->display->display_options['filters']['year']['expose']['identifier'] = 'year';
$handler->display->display_options['filters']['year']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
$handler->display->display_options['filters']['year']['is_grouped'] = TRUE;
$handler->display->display_options['filters']['year']['group_info']['label'] = 'year';
$handler->display->display_options['filters']['year']['group_info']['identifier'] = 'year';
$handler->display->display_options['filters']['year']['group_info']['group_items'] = array(
  1 => array(
    'title' => '2017',
    'operator' => '=',
    'value' => array(
      'value' => '2017',
      'min' => '',
      'max' => '',
    ),
  ),
  2 => array(
    'title' => '2016',
    'operator' => '=',
    'value' => array(
      'value' => '2016',
      'min' => '',
      'max' => '',
    ),
  ),
  3 => array(
    'title' => '2015',
    'operator' => '=',
    'value' => array(
      'value' => '2015',
      'min' => '',
      'max' => '',
    ),
  ),
  4 => array(
    'title' => '2014',
    'operator' => '=',
    'value' => array(
      'value' => '2014',
      'min' => '',
      'max' => '',
    ),
  ),
  5 => array(
    'title' => '2013',
    'operator' => '=',
    'value' => array(
      'value' => '2013',
      'min' => '',
      'max' => '',
    ),
  ),
  6 => array(
    'title' => '2012',
    'operator' => '=',
    'value' => array(
      'value' => '2012',
      'min' => '',
      'max' => '',
    ),
  ),
  7 => array(
    'title' => '2011',
    'operator' => '=',
    'value' => array(
      'value' => '2011',
      'min' => '',
      'max' => '',
    ),
  ),
  8 => array(
    'title' => '2010',
    'operator' => '=',
    'value' => array(
      'value' => '2010',
      'min' => '',
      'max' => '',
    ),
  ),
  9 => array(
    'title' => '2009',
    'operator' => '=',
    'value' => array(
      'value' => '2009',
      'min' => '',
      'max' => '',
    ),
  ),
  10 => array(
    'title' => '2008',
    'operator' => '=',
    'value' => array(
      'value' => '2008',
      'min' => '',
      'max' => '',
    ),
  ),
  11 => array(
    'title' => '2007',
    'operator' => '=',
    'value' => array(
      'value' => '2007',
      'min' => '',
      'max' => '',
    ),
  ),
);
/* Filter criterion: uwwtd_import_errors: date */
$handler->display->display_options['filters']['date']['id'] = 'date';
$handler->display->display_options['filters']['date']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['filters']['date']['field'] = 'date';
$handler->display->display_options['filters']['date']['operator'] = 'between';
$handler->display->display_options['filters']['date']['group'] = 1;
$handler->display->display_options['filters']['date']['exposed'] = TRUE;
$handler->display->display_options['filters']['date']['expose']['operator_id'] = 'date_op';
$handler->display->display_options['filters']['date']['expose']['label'] = 'date';
$handler->display->display_options['filters']['date']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['date']['expose']['operator'] = 'date_op';
$handler->display->display_options['filters']['date']['expose']['identifier'] = 'date';
$handler->display->display_options['filters']['date']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
/* Filter criterion: uwwtd_import_errors: type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['group'] = 1;
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
$handler->display->display_options['filters']['type']['is_grouped'] = TRUE;
$handler->display->display_options['filters']['type']['group_info']['label'] = 'type';
$handler->display->display_options['filters']['type']['group_info']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['group_info']['group_items'] = array(
  1 => array(
    'title' => 'notification',
    'operator' => '=',
    'value' => array(
      'value' => '0',
      'min' => '',
      'max' => '',
    ),
  ),
  2 => array(
    'title' => 'warning',
    'operator' => '=',
    'value' => array(
      'value' => '1',
      'min' => '',
      'max' => '',
    ),
  ),
  3 => array(
    'title' => 'error',
    'operator' => '=',
    'value' => array(
      'value' => '2',
      'min' => '',
      'max' => '',
    ),
  ),
);
/* Filter criterion: uwwtd_import_errors: category */
$handler->display->display_options['filters']['category']['id'] = 'category';
$handler->display->display_options['filters']['category']['table'] = 'uwwtd_import_errors';
$handler->display->display_options['filters']['category']['field'] = 'category';
$handler->display->display_options['filters']['category']['group'] = 1;
$handler->display->display_options['filters']['category']['exposed'] = TRUE;
$handler->display->display_options['filters']['category']['expose']['operator_id'] = 'category_op';
$handler->display->display_options['filters']['category']['expose']['label'] = 'category';
$handler->display->display_options['filters']['category']['expose']['operator'] = 'category_op';
$handler->display->display_options['filters']['category']['expose']['identifier'] = 'category';
$handler->display->display_options['filters']['category']['is_grouped'] = TRUE;
$handler->display->display_options['filters']['category']['group_info']['label'] = 'category';
$handler->display->display_options['filters']['category']['group_info']['identifier'] = 'category';
$handler->display->display_options['filters']['category']['group_info']['group_items'] = array(
  1 => array(
    'title' => 'import data input',
    'operator' => '=',
    'value' => array(
      'value' => '0',
      'min' => '',
      'max' => '',
    ),
  ),
  2 => array(
    'title' => 'import data linking',
    'operator' => '=',
    'value' => array(
      'value' => '1',
      'min' => '',
      'max' => '',
    ),
  ),
  3 => array(
    'title' => 'import data geometry',
    'operator' => '=',
    'value' => array(
      'value' => '2',
      'min' => '',
      'max' => '',
    ),
  ),
  4 => array(
    'title' => 'conformity data',
    'operator' => '=',
    'value' => array(
      'value' => '3',
      'min' => '',
      'max' => '',
    ),
  ),
  5 => array(
    'title' => 'import data format',
    'operator' => '=',
    'value' => array(
      'value' => '4',
      'min' => '',
      'max' => '',
    ),
  ),
  6 => array(
    'title' => 'shape format',
    'operator' => '=',
    'value' => array(
      'value' => '5',
      'min' => '',
      'max' => '',
    ),
  ),
  7 => array(
    'title' => 'article17 format',
    'operator' => '=',
    'value' => array(
      'value' => '6',
      'min' => '',
      'max' => '',
    ),
  ),
  8 => array(
    'title' => 'article17 input',
    'operator' => '=',
    'value' => array(
      'value' => '7',
      'min' => '',
      'max' => '',
    ),
  ),
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'error-list';
$translatables['error_list'] = array(
  t('Master'),
  t('Error list'),
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
  t('relationship to node table'),
  t('errid'),
  t('.'),
  t('year'),
  t('date'),
  t('category'),
  t('type'),
  t('error'),
  t('Title'),
  t('uwwtd_import_errors'),
  t('- Choose an operation -'),
  t('Page'),
);
