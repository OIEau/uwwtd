<?php

/**
 * @file
 * Add tooltips to fields.
 */

/**
 * Field instance object.
 */
class field_tooltips_instance extends i18n_field_instance {
  /**
   * Get translatable properties.
   */
  protected function build_properties() {
    $properties = parent::build_properties();
    $object = $this->object;
    $field = field_info_field($object['field_name']);

    if (!empty($object['settings']['field_tooltips_tooltip']) && i18n_field_type_info($field['type'], 'translate_default')) {
      $format = isset($object['settings']['field_tooltips_tooltip']['format']) ? $object['settings']['field_tooltips_tooltip']['format'] : NULL;
      $properties[$this->get_textgroup()][$object['field_name']][$object['bundle']]['field_tooltips_tooltip']['string'] = $object['settings']['field_tooltips_tooltip']['value'];
      $properties[$this->get_textgroup()][$object['field_name']][$object['bundle']]['field_tooltips_tooltip']['format'] = $format;
    }

    return $properties;
  }

  /**
   * Set field translation for object.
   *
   * Most often, this is a direct field set, but sometimes fields may have different formats.
   *
   * @param $object
   *   A clone of the object or array. Field instance.
   */
  protected function translate_field(&$object, $i18nstring, $langcode, $options) {
    if ($i18nstring->property == 'field_tooltips_tooltip') {
      // Render string without applying format.
      $object['settings']['field_tooltips_tooltip']['value'] = $i18nstring->format_translation($langcode, array('sanitize' => FALSE) + $options);
    }
    else {
      parent::translate_field($object, $i18nstring, $langcode, $options);
    }
  }
}
