<?php

/**
 * @file
 * Field for geospatial data using the Field API. Implements field formatter.
 */

/**
 * Describes available presentations of postgis fields
 * for output to publicly available website.
 *
 * Implements hook_field_formatter_info().
 */
function postgis_field_formatter_info() {

  $formatters = array();

  // Add map if OpenLayers is available to paint the map.
  if (module_exists('openlayers')) {
    $formatters['openlayers'] = array(
      'label' => t('OpenLayers Map'),
      'field types' => array('postgis'),
      'settings' => array('map' => 'default'),
    );
  }

  $formatters['measurement'] = array(
    'label' => t('Measurement'),
    'field types' => array('postgis'),
    'settings' => array('type' => 'length'),
  );
  $formatters['wkt'] = array(
    'label' => t('Well Known Text (WKT)'),
    'field types' => array('postgis'),
  );

  return $formatters;
}

/**
 * Defines a form for customization of presentation to user.
 * This allows to adjust the style of OpenLayers' maps when showing features.
 *
 * Implements hook_field_formatter_settings_form().
 */
function postgis_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  if ($display['type'] == 'openlayers' && module_exists('openlayers')) {

    $maps = openlayers_maps();
    $maps_options = array();
    foreach ($maps as $map) {
      $maps_options[$map->name] = $map->title;
    }

    $element['map'] = array(
      '#title' => t('OpenLayers map'),
      '#type' => 'select',
      '#default_value' => $settings['map'] ? $settings['map'] : 'default',
      '#required' => TRUE,
      '#options' => $maps_options,
      '#description' => t('Select which OpenLayers map you would like to use.'),
    );
  }

  return $element;
}

/**
 * Shows a summary of the fields presentation.
 *
 * Implements hook_field_formatter_settings_summary().
 */
function postgis_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  if ($display['type'] == 'openlayers' && isset($settings['map'])) {
    $maps = openlayers_map_options();
    $summary[] = t('Openlayers Map: @data', array('@data' => $maps[$settings['map']]));
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function postgis_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  // Combines multiple values from database to a single container object.
  $set = new PostgisGeometrySet($field['settings']['type'], $field['settings']['srid']);
  $set->fromGeometry($items);

  switch ($display['type']) {

    // Shows geometry as text to user.
    case 'wkt':
      $element[]['#markup'] = $set->getText();
      break;

    // Shows length of geometry to user.
    case 'measurement':
      // TODO: make measurement configurable.
      $element[]['#markup'] = $set->length('km') . ' ' . t('km');
      break;

    // Shows feature of a map.
    case 'openlayers':
      $map_name = $display['settings']['map'] ? $display['settings']['map'] : 'default';
      $map_data = openlayers_map_load($map_name);
      if (!isset($map_data->data['layers']['postgis_formatter_layer'])) {
        $map_data->data['layers']['postgis_formatter_layer'] = 'postgis_formatter_layer';
      }
      if (!isset($map_data->data['behaviors']['openlayers_behavior_zoomtolayer'])) {
        $map_data->data['behaviors']['openlayers_behavior_zoomtolayer'] = array(
          'zoomtolayer' => array(
            'postgis_formatter_layer' => 'postgis_formatter_layer',
          ),
          'point_zoom_level' => '10',
          'zoomtolayer_scale' => '1',
        );
      }
      $map = openlayers_build_map($map_data->data);
      $map['layers']['postgis_formatter_layer']['title'] = $instance['label'];
      
      $set->transform($map_data->data['projection']);

      $map['layers']['postgis_formatter_layer']['features'] = array(array('wkt' => $set->getText()));

      // Render map if no errors found.
      if (empty($map['errors'])) {
        $js = array('openlayers' => array('maps' => array($map['id'] => $map)));
        drupal_add_js($js, 'setting');

        // Push map through theme function.
        $element[0]['#markup'] = theme('openlayers_map', array(
          'map' => $map,
          'map_name' => $map_name,
        ));
      }

      break;
  }

  return $element;
}

