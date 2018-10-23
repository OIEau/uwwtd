<?php
/**
 * @file
 * An interaction with point-based, clustered maps that allows
 * clicking on points that results in scanning between items.
 */

class Openlayers_plus_behavior_popup extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  public function options_init() {
    return array(
      'layers' => array(),
    );
  }

  /**
   * Form defintion for per map customizations.
   */
  public function options_form($defaults = array()) {
    // Only prompt for vector layers.
    $vector_layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if (isset($layer->data['vector']) && $layer->data['vector'] == TRUE) {
        $vector_layers[$id] = $name;
      }
    }

    return array(
      'layers' => array(
        '#title' => t('Layers'),
        '#type' => 'checkboxes',
        '#options' => $vector_layers,
        '#description' => t('Select layer to apply popups to.'),
        '#default_value' => isset($defaults['layers']) ? $defaults['layers'] : array(),
      ),

      'hover' => array(
        '#title' => t('Hover'),
        '#type' => 'checkbox',
        '#description' => t('Select to display popup on mouse hover.'),
        '#default_value' => isset($defaults['hover']) ? $defaults['hover'] : NULL,
      ),
    );
  }

  /**
   * Render.
   */
  public function render(&$map) {
    drupal_add_css(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_popup.css');
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_popup.js');
    drupal_add_js(array('popup' => $this->options), 'setting');
    return $this->options;
  }
}
