<?php
/**
 * @file
 * An interaction with point-based maps that result in following links
 * on hover.
 */

class openlayers_plus_behavior_tooltips extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    return array(
      'positioned' => FALSE,
      'layers' => array(),
    );
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {

    $vector_layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if (isset($layer->data['vector']) && $layer->data['vector'] == TRUE) {
        $vector_layers[$id] = $name;
      }
    }

    return array(
      'positioned' => array(
        '#type' => 'checkbox',
        '#title' => t('Position tooltip over feature'),
        '#default_value' => isset($defaults['positioned']) ? $defaults['positioned'] : NULL,
      ),

      'layers' => array(
        '#title' => t('Layers'),
        '#type' => 'checkboxes',
        '#options' => $vector_layers,
        '#description' => t('Select layer to apply tooltips to.'),
        '#default_value' => isset($defaults['layers']) ? $defaults['layers'] : array(),
      ),
    );
  }

  /**
   * Render.
   */
  public function render(&$map) {
    drupal_add_css(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_tooltips.css');
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_tooltips.js');
    return $this->options;
  }
}
