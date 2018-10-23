<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * mapeditor Behavior
 */
class openlayers_plus_behavior_mapeditor extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    $options = array();
    return $options;
  }
  
  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    $options['position'] = array(
            '#type' => 'select',
            '#title' => t('Position'),
            '#options' => array(
                'na' => t('None'),
                'ne' => t('Top right'),
                'se' => t('Bottom right'),
                'sw' => t('Bottom left'),
                'nw' => t('Top left')
            ),
            '#default_value' => isset($defaults['position']) ? $defaults['position'] : NULL
    );
    return $options;
  }
  
  /**
   * Render.
   */
  function render(&$map) {
    $option = array(
        'mapeditorposition' => isset($this -> options['position']) ? $this -> options['position'] : 0
    );
    drupal_add_js(array(
    'openlayers_plus' => $option
    ), 'setting');
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_mapeditor.js');
    return $option;
  }
}
