<?php
/**
 * @file
 * An interaction with point-based maps that
 * results in following links on hover that you specify the field.
 */

class Openlayers_plus_behavior_tooltips_field extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    return array('positioned' => FALSE);
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    $form = parent::options_form();
    $form['positioned'] = array(
      '#type' => 'checkbox',
      '#title' => t('Position tooltip over feature displaying specific field'),
      '#default_value' => isset($this->options['positioned']) ? $this->options['positioned'] : NULL,
    );
    $form['field_displayed'] = array(
      '#type' => 'textfield',
      '#title' => t('Field to Display'),
      '#description' => t('Field from openlayers view to be displayed. (usually field_your_special_field)'),
      '#default_value' => isset($this->options['field_displayed']) ? $this->options['field_displayed'] : NULL,
    );
    return $form;
  }

  /**
   * Render.
   */
  public function render(&$map) {
    $settings = array(
      'openlayers_plus_behavior_tooltips_field' => array(
        'field' => "field_category",
      ),
    );
    drupal_add_js($settings, array('type' => 'setting'));
    drupal_add_js(drupal_get_path('module', 'openlayers_plus')
        . '/behaviors/openlayers_plus_behavior_tooltips_field.js');
    return $this->options;
  }
}
