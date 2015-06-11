<?php
/**
 * @file
 * A version of BlockSwitcher that toggles between two different
 * layers only. Useful for situations in which layers represent the
 * same data in slightly different ways
 */

class openlayers_plus_behavior_blocktoggle extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    $options = array(
      'enabled' => FALSE,
      'position' => 'se',
    );
    return $options;
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    return array(
      'enabled' => array(
        '#type' => 'checkbox',
        '#title' => t('Show blocktoggle in map'),
        '#default_value' => isset($defaults['enabled']) ? $defaults['enabled'] : NULL,
      ),
      'a_label' => array(
        '#type' => 'textfield',
        '#title' => t('Layer A Label'),
        '#description' => t('This can either be the name of the layer, or a shorter name you choose.'),
        '#default_value' => isset($defaults['a_label']) ? $defaults['a_label'] : array(),
      ),
      'a' => array(
        '#type' => 'select',
        '#options' => $this->map['layers'],
        '#title' => t('Layer A'),
        '#default_value' => isset($defaults['a']) ? $defaults['a'] : NULL,
      ),
      'b_label' => array(
        '#type' => 'textfield',
        '#title' => t('Layer B Label'),
        '#description' => t('This can either be the name of the layer, or a shorter name you choose.'),
        '#default_value' => isset($defaults['b_label']) ? $defaults['b_label'] : array(),
      ),
      'b' => array(
        '#type' => 'select',
        '#options' => $this->map['layers'],
        '#title' => t('Layer B'),
        '#default_value' => isset($defaults['b']) ? $defaults['b'] : NULL,
      ),
      'position' => array(
        '#type' => 'select',
        '#title' => t('Position'),
        '#options' => array(
          'ne' => t('Top right'),
          'se' => t('Bottom right'),
          'sw' => t('Bottom left'),
          'nw' => t('Top left'),
        ),
        '#default_value' => isset($defaults['position']) ? $defaults['position'] : NULL,
      ),
    );
  }

  /**
   * Render.
   */
  public function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers_plus')
      . '/behaviors/openlayers_plus_behavior_blocktoggle.js');
    if ($this->options['enabled']) {
      $block = module_invoke('openlayers_plus', 'block_view', 'blocktoggle');
      $this->options['block'] = drupal_render($block);
    }
    return $this->options;
  }
}
