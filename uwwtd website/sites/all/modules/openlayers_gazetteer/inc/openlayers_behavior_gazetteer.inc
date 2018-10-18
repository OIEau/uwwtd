<?php
/**
 * @file
 * Extends openlayers_behavior class.
 */

class openlayers_behavior_gazetteer extends openlayers_behavior {
  /**
   * Build option form.
   */
  function options_form($defaults = array()) {
    return array(
      'restriction_query' => array(
        '#title' => t('Restrict location query'),
        '#type' => 'textfield',
        '#description' => t('A string to append to the request (please use \'&\' between parameters). You can use GeoNames parameters to restrict your request results (ex: "country=us" will restrict the results to USA). You can find a list of available parameters on <a href="http://www.geonames.org/export/geonames-search.html" target="_blank">GeoNames website</a>'),
        '#default_value' => isset($defaults['restriction_query']) ? $defaults['restriction_query'] : '',
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_css(drupal_get_path('module', 'openlayers_gazetteer') . '/css/openlayers_gazetteer.css');
    drupal_add_js(array('openlayers_gazetteer' => array('geonames_username' => variable_get('openlayers_gazetteer_geonames_username', ''))), array('type' => 'setting'));
    drupal_add_js(drupal_get_path('module', 'openlayers_gazetteer') . '/inc/openlayers_behavior_gazetteer.js');
    drupal_add_library('system', 'ui.autocomplete');
    return $this->options;
  }
}
