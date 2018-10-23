<?php

$view = new view();
$view->name = 'uwwtd_error_list';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Error list';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = true; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Error list';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'nid' => 'nid',
  'field_uwwtd_err_identifier' => 'field_uwwtd_err_identifier',
  'field_uwwtd_err_timestamp' => 'field_uwwtd_err_timestamp',
  'field_uwwtd_err_type' => 'field_uwwtd_err_type',
  'field_uwwtd_err_category' => 'field_uwwtd_err_category',
  'field_uwwtd_err_message' => 'field_uwwtd_err_message',
  'views_bulk_operations' => 'views_bulk_operations',
  'field_uwwtd_err_time' => 'field_uwwtd_err_time',
);
$handler->display->display_options['style_options']['default'] = 'field_uwwtd_err_time';
$handler->display->display_options['style_options']['info'] = array(
  'nid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwtd_err_identifier' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwtd_err_timestamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwtd_err_type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwtd_err_category' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwtd_err_message' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'views_bulk_operations' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_uwwtd_err_time' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = '';
$handler->display->display_options['fields']['nid']['exclude'] = TRUE;
$handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
/* Field: Content: Identifier */
$handler->display->display_options['fields']['field_uwwtd_err_identifier']['id'] = 'field_uwwtd_err_identifier';
$handler->display->display_options['fields']['field_uwwtd_err_identifier']['table'] = 'field_data_field_uwwtd_err_identifier';
$handler->display->display_options['fields']['field_uwwtd_err_identifier']['field'] = 'field_uwwtd_err_identifier';
$handler->display->display_options['fields']['field_uwwtd_err_identifier']['label'] = 'Import identifier';
$handler->display->display_options['fields']['field_uwwtd_err_identifier']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['field_uwwtd_err_identifier']['alter']['path'] = 'node/[nid]';
/* Field: Content: Timestamp */
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['id'] = 'field_uwwtd_err_timestamp';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['table'] = 'field_data_field_uwwtd_err_timestamp';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['field'] = 'field_uwwtd_err_timestamp';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['alter']['text'] = '[field_uwwtd_err_timestamp-value]';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['alter']['path'] = 'node/[nid]';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['type'] = 'date_plain';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['settings'] = array(
  'format_type' => 'long',
  'fromto' => '',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_remaining_days' => 0,
);
/* Field: Content: Time */
$handler->display->display_options['fields']['field_uwwtd_err_time']['id'] = 'field_uwwtd_err_time';
$handler->display->display_options['fields']['field_uwwtd_err_time']['table'] = 'field_data_field_uwwtd_err_time';
$handler->display->display_options['fields']['field_uwwtd_err_time']['field'] = 'field_uwwtd_err_time';
$handler->display->display_options['fields']['field_uwwtd_err_time']['label'] = 'Time of error';
$handler->display->display_options['fields']['field_uwwtd_err_time']['settings'] = array(
  'format_type' => 'time',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_remaining_days' => 0,
);
/* Field: Content: Type */
$handler->display->display_options['fields']['field_uwwtd_err_type']['id'] = 'field_uwwtd_err_type';
$handler->display->display_options['fields']['field_uwwtd_err_type']['table'] = 'field_data_field_uwwtd_err_type';
$handler->display->display_options['fields']['field_uwwtd_err_type']['field'] = 'field_uwwtd_err_type';
/* Field: Content: Category */
$handler->display->display_options['fields']['field_uwwtd_err_category']['id'] = 'field_uwwtd_err_category';
$handler->display->display_options['fields']['field_uwwtd_err_category']['table'] = 'field_data_field_uwwtd_err_category';
$handler->display->display_options['fields']['field_uwwtd_err_category']['field'] = 'field_uwwtd_err_category';
/* Field: Content: Message */
$handler->display->display_options['fields']['field_uwwtd_err_message']['id'] = 'field_uwwtd_err_message';
$handler->display->display_options['fields']['field_uwwtd_err_message']['table'] = 'field_data_field_uwwtd_err_message';
$handler->display->display_options['fields']['field_uwwtd_err_message']['field'] = 'field_uwwtd_err_message';
/* Field: Bulk operations: Content */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::node_assign_owner_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 1,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_revision' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_script_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_make_sticky_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_make_unsticky_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_modify_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'show_all_tokens' => 1,
      'display_values' => array(
        '_all_' => '_all_',
      ),
    ),
  ),
  'action::views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'url' => '',
    ),
  ),
  'action::node_promote_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_publish_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpromote_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_save_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_send_email_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpublish_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpublish_by_keyword_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::pathauto_node_update_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
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
  'uwwtd_error' => 'uwwtd_error',
);
$handler->display->display_options['filters']['type']['group'] = 1;
/* Filter criterion: Content: Identifier (field_uwwtd_err_identifier) */
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['id'] = 'field_uwwtd_err_identifier_value';
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['table'] = 'field_data_field_uwwtd_err_identifier';
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['field'] = 'field_uwwtd_err_identifier_value';
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['group'] = 1;
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['expose']['operator_id'] = 'field_uwwtd_err_identifier_value_op';
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['expose']['label'] = 'Identifier';
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['expose']['operator'] = 'field_uwwtd_err_identifier_value_op';
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['expose']['identifier'] = 'field_uwwtd_err_identifier_value';
$handler->display->display_options['filters']['field_uwwtd_err_identifier_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
/* Filter criterion: Content: Type (field_uwwtd_err_type) */
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['id'] = 'field_uwwtd_err_type_value';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['table'] = 'field_data_field_uwwtd_err_type';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['field'] = 'field_uwwtd_err_type_value';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['group'] = 1;
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['expose']['operator_id'] = 'field_uwwtd_err_type_value_op';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['expose']['operator'] = 'field_uwwtd_err_type_value_op';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['expose']['identifier'] = 'field_uwwtd_err_type_value';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['group_info']['label'] = 'Type (field_uwwtd_err_type)';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['group_info']['identifier'] = 'field_uwwtd_err_type_value';
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['group_info']['remember'] = FALSE;
$handler->display->display_options['filters']['field_uwwtd_err_type_value']['group_info']['group_items'] = array(
  1 => array(),
  2 => array(),
  3 => array(),
);
/* Filter criterion: Content: Category (field_uwwtd_err_category) */
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['id'] = 'field_uwwtd_err_category_value';
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['table'] = 'field_data_field_uwwtd_err_category';
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['field'] = 'field_uwwtd_err_category_value';
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['group'] = 1;
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['expose']['operator_id'] = 'field_uwwtd_err_category_value_op';
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['expose']['label'] = 'Category';
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['expose']['operator'] = 'field_uwwtd_err_category_value_op';
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['expose']['identifier'] = 'field_uwwtd_err_category_value';
$handler->display->display_options['filters']['field_uwwtd_err_category_value']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['access'] = FALSE;
$handler->display->display_options['access']['type'] = 'role';
$handler->display->display_options['access']['role'] = array(
  3 => '3',
  4 => '4',
);
$handler->display->display_options['path'] = 'errors';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Type */
$handler->display->display_options['fields']['field_uwwtd_err_type']['id'] = 'field_uwwtd_err_type';
$handler->display->display_options['fields']['field_uwwtd_err_type']['table'] = 'field_data_field_uwwtd_err_type';
$handler->display->display_options['fields']['field_uwwtd_err_type']['field'] = 'field_uwwtd_err_type';
/* Field: Content: Category */
$handler->display->display_options['fields']['field_uwwtd_err_category']['id'] = 'field_uwwtd_err_category';
$handler->display->display_options['fields']['field_uwwtd_err_category']['table'] = 'field_data_field_uwwtd_err_category';
$handler->display->display_options['fields']['field_uwwtd_err_category']['field'] = 'field_uwwtd_err_category';
/* Field: Content: Timestamp */
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['id'] = 'field_uwwtd_err_timestamp';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['table'] = 'field_data_field_uwwtd_err_timestamp';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['field'] = 'field_uwwtd_err_timestamp';
$handler->display->display_options['fields']['field_uwwtd_err_timestamp']['settings'] = array(
  'format_type' => 'short',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
);
/* Field: Content: Message */
$handler->display->display_options['fields']['field_uwwtd_err_message']['id'] = 'field_uwwtd_err_message';
$handler->display->display_options['fields']['field_uwwtd_err_message']['table'] = 'field_data_field_uwwtd_err_message';
$handler->display->display_options['fields']['field_uwwtd_err_message']['field'] = 'field_uwwtd_err_message';
/* Field: Bulk operations: Content */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::node_assign_owner_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 1,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_revision' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_script_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_make_sticky_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_make_unsticky_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_modify_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'show_all_tokens' => 1,
      'display_values' => array(
        '_all_' => '_all_',
      ),
    ),
  ),
  'action::views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'url' => '',
    ),
  ),
  'action::node_promote_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_publish_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpromote_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_save_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_send_email_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpublish_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpublish_by_keyword_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::pathauto_node_update_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
$handler->display->display_options['path'] = 'error_deletion';
$translatables['uwwtd_error_list'] = array(
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
  t('Import identifier'),
  t('Timestamp'),
  t('[field_uwwtd_err_timestamp-value]'),
  t('Time of error'),
  t('Type'),
  t('Category'),
  t('Message'),
  t('Content'),
  t('- Choose an operation -'),
  t('Identifier'),
  t('Type (field_uwwtd_err_type)'),
  t('Page'),
);
