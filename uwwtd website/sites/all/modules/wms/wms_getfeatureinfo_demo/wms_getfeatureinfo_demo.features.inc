<?php
/**
 * @file
 * wms_getfeatureinfo_demo.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wms_getfeatureinfo_demo_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "openlayers" && $api == "openlayers_layers") {
    return array("version" => "1");
  }
  if ($module == "openlayers" && $api == "openlayers_layers") {
    return array("version" => "2");
  }
  if ($module == "openlayers" && $api == "openlayers_maps") {
    return array("version" => "1");
  }
  if ($module == "openlayers" && $api == "openlayers_maps") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function wms_getfeatureinfo_demo_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
