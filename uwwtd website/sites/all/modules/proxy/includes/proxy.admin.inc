<?php

/**
 * @file
 * This file holds the functions for the Proxy Admin settings.
 *
 * @ingroup proxy
 */

/**
 * Menu callback; displays the proxy module settings page.
 *
 * @see system_settings_form()
 */
function proxy_admin_settings($form, &$form_state) {
  module_load_include('processor.inc', 'proxy', 'includes/proxy');
  $form = array();

  // Processes
  $processes = array(
    'example' => t('Example process'),
  );

  // Cache life
  $caches = _proxy_get_cache_intervals();

  // Define Form
  $form['proxy_cache_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache Settings'),
    '#collapsible' => TRUE,
  );
  $form['proxy_cache_fields']['proxy_cache'] = array(
    '#type' => 'select',
    '#title' => t('Default Cache Lifetime'),
    '#description' => t('This defines how long proxied content will live in cache.  This should be as long as the proxied content gets updated.  The longer the cache, the better performance you will see.'),
    '#options' => $caches,
    '#default_value' => variable_get('proxy_cache', PROXY_CACHE_DEFAULT),
  );

  // Whitelist
  $form['proxy_whitelist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Whitelist Settings'),
    '#collapsible' => TRUE,
  );
  $form['proxy_whitelist']['proxy_whitelist_control'] = array(
    '#type' => 'checkbox',
    '#title' => t('Whitelist Control'),
    '#description' => t('Turning this on means that the proxy will only work for the allowed domains.  With this off, the proxy will be open to any URI.'),
    '#default_value' => variable_get('proxy_whitelist_control', PROXY_WHITELIST_CONTROL_DEFAULT),
  );
  $form['proxy_whitelist']['proxy_whitelist_domains'] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelist Domains'),
    '#description' => t('The list of allowed domains for the proxy, when whitelisting is turned on.  Put each domain on a new line.  You can also use a %wildcard as a wildcard; for example, the following will allow all domains with the word Drupal in them: %example.', array('%wildcard' => '*', '%example' => '*drupal*')),
    '#default_value' => variable_get('proxy_whitelist_domains', PROXY_WHITELIST_DOMAINS_DEFAULT),
  );

  // Flood Control
  $form['proxy_flood'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flood Settings'),
    '#collapsible' => TRUE,
  );
  $form['proxy_flood']['proxy_flood_control'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flood Control'),
    '#description' => t('This setting turns on flood control, which means each users access to the proxy is tracked and limited to a certain amount per hour.'),
    '#default_value' => variable_get('proxy_flood_control', PROXY_FLOOD_CONTROL_DEFAULT),
  );
  $form['proxy_flood']['proxy_flood_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Flood Interval'),
    '#description' => t('The number of proxy connections that are allowed per hour.'),
    '#default_value' => variable_get('proxy_flood_interval', PROXY_FLOOD_INTERVAL_DEFAULT),
  );
  $form['proxy_flood']['proxy_flood_autoban'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-Ban'),
    '#description' => t('If turned on, Auto-Ban will automatically ban an IP if it reaches over the flood limit.  Ensure that you understand the repercussions of this feature before turning it on.'),
    '#default_value' => variable_get('proxy_flood_autoban', PROXY_FLOOD_AUTOBAN_DEFAULT),
  );

  // Processors
  $processors_options = _proxy_get_processor_options();
  if (!empty($processors_options) && is_array($processors_options)) {
    $form['proxy_processors'] = $processors_options;
    $form['proxy_processors']['#type'] = 'fieldset';
    $form['proxy_processors']['#title'] = t('Processors');
    $form['proxy_processors']['#description'] = t('Processors are plugins that manipulate the content retrieved through the proxy.  Depending on how you use the proxy, enable the apprproate ones.  Note that each processor will be a performance hit as it means each non-cached load will have to be processed.');
    $form['proxy_processors']['#collapsible'] = TRUE;
  }

  // Make a system setting form and return
  return system_settings_form($form);
}
