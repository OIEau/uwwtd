<?php

/**
 * Implements hook_views_plugins().
 */
function views_exposed_groups_views_plugins() {
  return array(
    'exposed_form' => array(
      'views_exposed_groups' => array(
        'title' => t('Grouped filters'),
        'help' => t('Groups fields into fieldgroups or vertical tabs.'),
        'handler' => 'views_exposed_groups_plugin',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'parent' => 'basic',
      ),
    ),
  );
}
