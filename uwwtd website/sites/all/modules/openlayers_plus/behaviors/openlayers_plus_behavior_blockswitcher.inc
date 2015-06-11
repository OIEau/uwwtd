<?php
/**
 * @file
 * Behaviour that shows a layerswitcher that is slightly more stylable
 * than the default openlayers one and is able to show itself next to the
 * map as a standard Drupal block
 */

class openlayers_plus_behavior_blockswitcher extends openlayers_behavior {
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
        '#title' => t('Show blockswitcher in map'),
        '#default_value' => isset($defaults['enabled']) ? $defaults['enabled'] : NULL,
      ),
      'open' => array(
        '#type' => 'checkbox',
        '#title' => t('Show blockswitcher open when the map loads'),
        '#default_value' => isset($defaults['open']) ? $defaults['open'] : NULL,
      ),
      'overlay_style' => array(
        '#type' => 'select',
        '#title' => t('Show overlay layers as checkboxes or radio buttons'),
        '#options' => array(
          'checkbox' => t('Checkboxes'),
          'radio' => t('Radio Buttons'),
        ),
        '#default_value' => isset($defaults['overlay_style']) ? $defaults['overlay_style'] : NULL,
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
      . '/behaviors/openlayers_plus_behavior_blockswitcher.js');
    drupal_add_js('misc/jquery.cookie.js', 'file');
    if ($this->options['enabled']) {
      $block = module_invoke('openlayers_plus', 'block_view', 'blockswitcher');
      $this->options['block'] = drupal_render($block['content']);
    }
    return $this->options;
  }
}
