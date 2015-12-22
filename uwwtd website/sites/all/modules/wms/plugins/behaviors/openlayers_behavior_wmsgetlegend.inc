<?php
// $Id$

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * WMSGetLegend Behavior
 */

$plugin = array(
    'openlayers_behavior_wmsgetlegend' => array(
        'title' => t('Get Legend'),
        'description' => t('Queries WMS server for the active layer\'s legend.'),
        'type' => 'layer',
        'behavior' => array(
          'file' => 'openlayers_behavior_wmsgetlegend.inc',
          'class' => 'openlayers_behavior_wmsgetlegend',
          'parent' => 'openlayers_behavior',
        ),
    ),
);


class openlayers_behavior_wmsgetlegend extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array();
  }

  function options_form($defaults) {
    // TODO only return form when there is at least 1 WMS layer
    return array(
      'getlegend_htmlelement' => array(
        '#type' => 'textfield',
        '#description' => t("An HTML element that will be filled
          with the query result. Give the jQuery selector (#id or .class). If you use the GetLegend block, the value should be 'getlegend'"),
        '#default_value' => isset($defaults['getlegend_htmlelement']) ?
          $defaults['getlegend_htmlelement'] : "getlegend",
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'wms') . '/plugins/behaviors/js/openlayers_behavior_wmsgetlegend.js');
    return $this->options;
  }
}

