<?php
/**
 * @file
 * wms_getfeatureinfo_demo.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function wms_getfeatureinfo_demo_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'proxy_whitelist_control';
  $strongarm->value = 1;
  $export['proxy_whitelist_control'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'proxy_whitelist_domains';
  $strongarm->value = 'www2.dmsolutions.ca';
  $export['proxy_whitelist_domains'] = $strongarm;

  return $export;
}
