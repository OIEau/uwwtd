<?php
/**
 * @file
 * A version of Permalink optimized to persist layers between pages
 * with different layer setups and without explicitly using the control.
 */

class openlayers_plus_behavior_permalink extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    return array();
  }
  /**
   * Render.
   */
  public function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_permalink.js');
    return $this->options;
  }
}
