<?php

/**
 * @file
 * Field for geospatial data using the Field API. Implements field widget.
 */

/**
 * Implements hook_field_widget_info().
 */
function postgis_field_widget_info() {

  $widgets   = array();
  $behaviors = array(
    'multiple values' => FIELD_BEHAVIOR_CUSTOM,
    'default value'   => FIELD_BEHAVIOR_NONE,
  );
  
  $widgets['wkt'] = array(
    'label' => t('Textfield'),
    'field types' => array('postgis'),
    'behaviors' => $behaviors,
  );

  if (module_exists('openlayers')) {
    $widgets['openlayers'] = array(
      'label' => t('OpenLayers Map'),
      'field types' => array('postgis'),
      'settings' => array(
        'map'      => 'default',
        'behavior' => module_exists('ole') ? 
          'openlayers_behavior_ole' : 'openlayers_behavior_drawfeatures',
      ),
      'behaviors' => $behaviors,
    );
  }

  return $widgets;
}

/**
 * Implements hook_field_widget_settings_form().
 */
function postgis_field_widget_settings_form($field, $instance) {

  $widget = $instance['widget'];
  $settings = $widget['settings'];
  $form = array();

  if ($widget['type'] == 'openlayers') {

    $maps = openlayers_map_options();
    $map = isset($settings['map']) ? $settings['map'] : 'default';

    $form['map'] = array(
      '#type' => 'select',
      '#title' => t('OpenLayers Map'),
      '#default_value' => $map,
      '#options' => $maps,
      '#description' => t('Select which OpenLayers map you would like to use.'),
    );

    $behaviors = openlayers_behaviors();
    foreach ($behaviors as $index => $behavior) {
      $behaviors[$index] = $behavior['title'];
    }
    
    if (isset($settings['behavior'])) {
      $behavior = $settings['behavior'];
    } else {
      $behavior = module_exists('ole') ? 
          'openlayers_behavior_ole' : 'openlayers_behavior_drawfeatures';
    }

    $form['behavior'] = array(
      '#type' => 'select',
      '#title' => t('OpenLayers Behavior'),
      '#default_value' => $behavior,
      '#options' => $behaviors,
      '#description' => t('Select an OpenLayers behavior which handles drawing of features. Usually you want “Draw Features” or the more capable “<a href="http://drupal.org/project/ole">OpenLayers Editor</a>” to be selected here.'),
    );

  }

  return $form;
}

/**
 * Renders map or text field for WKT in content editing screen.
 *
 * Implements hook_field_widget_form().
 */
function postgis_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $set = new PostgisGeometrySet($field['settings']['type'], $field['settings']['srid']);
  $set->fromGeometry($items);

  $element[$delta] = array(
    '#delta' => $delta,
  );

  if (module_exists('openlayers') && $instance['widget']['type'] == 'openlayers') {

    switch (strtoupper($field['settings']['type'])) {
      case POSTGIS_PRIMITIVE_POINT:
      case POSTGIS_PRIMITIVE_MULTIPOINT:
        $types = array('point');
        break;
      case POSTGIS_PRIMITIVE_LINESTRING:
      case POSTGIS_PRIMITIVE_MULTILINESTRING:
        $types = array('path');
        break;
      case POSTGIS_PRIMITIVE_POLYGON:
      case POSTGIS_PRIMITIVE_MULTIPOLYGON:
        $types = array('polygon');
        break;
      case POSTGIS_PRIMITIVE_GEOMETRYCOLLECTION:
        $types = array('point', 'path', 'polygon');
        break;
      default:
        $types = array();
    }

    $map = openlayers_map_load($settings['map']);
    $srid = $map->data['projection'];
    if (!is_numeric($srid) && (strpos($srid, ':') != -1)) {
      $srid = substr($srid, 1+strpos($srid, ':'));
    }
    $set->transform($srid);
    
    // Set up drawing tool.
    $behavior = array();
    if(isset($map->data['behaviors'][$settings['behavior']])){
      // Use setting of map configuration (adjustable by user).
      $behavior = $map->data['behaviors'][$settings['behavior']];
    } else if($settings['behavior']==='openlayers_behavior_ole'){
      // Apply default settings for OLE.
      $behavior = array (
        'processing_controls' => array (
          'MergeFeature' => 'MergeFeature',
          'SplitFeature' => 'SplitFeature',
          'DeleteFeature' => 'DeleteFeature',
        ),
        'editing_controls' => array (
          'DragFeature' => 'DragFeature',
          'DrawHole' => 'DrawHole',
          'ModifyFeature' => 'ModifyFeature',
        ),
        'other_controls' => array (
          'Navigation' => 'Navigation',
          'SnappingSettings' => 'SnappingSettings',
        ),
      );
    }

    $textarea_id = $field['field_name'] . '_wkt' . $delta;
    $map->data['behaviors'][$settings['behavior']] = array_merge($behavior, array(
      'element_id'    => $textarea_id,
      'feature_types' => $types,
      // Drupal's unlimited value of -1 is not known to OpenLayers
      // and thus needs to be converted to NULL.
      'feature_limit' => ($field['cardinality'] > 0) ? $field['cardinality'] : NULL,
    ));

    $class = 'form-item-postgis-map';
    if ($element['#required'] && !empty($form_state['input']) &&
        $form_state['input'][$field['field_name']]['und'][0]['wkt'] === '') {
      $class .= ' error';
    }

    $element[$delta]['map'] = array(
      '#title' => $instance['label'],
      '#type' => 'item',
      '#markup' => '<div class="form-item"><div class="'.$class.'">' .
        openlayers_render_map($map->data) . '</div></div>',
      '#required' => $element['#required'],
      '#description' => $element['#description'],
    );

    $element[$delta]['wkt'] = array(
      '#type' => 'textarea',
      '#title' => $instance['label'],
      '#attributes' => array('id' => $textarea_id),
      // Using a format that OpenLayers can handle.
      '#default_value' => ($set->getText() === 'GEOMETRYCOLLECTION(EMPTY)') ? '' : $set->getText(),
      '#required' => $element['#required'],
      '#resizable' => FALSE,
      '#prefix' => '<div class="form-item-postgis-wkt">',
      '#suffix' => '</div>',
    );

    // Remove empty GEOMETRYCOLLECTION coming from OpenLayers.
    $element[$delta]['script'] = array(
      '#markup' => '<script type="text/javascript">(function($){
        var field = $("#' . $textarea_id . '");
        if (field.val() == "GEOMETRYCOLLECTION()") {
          field.html("")
        }
       })(jQuery);</script>',
    );
  }
  else {
    $element[$delta]['wkt'] = array(
      '#title' => t('Well Known Text'),
      '#type' => 'textarea',
      '#default_value' => $set->getText(),
      '#required' => $element['#required'],
    );
  }
  return $element;
}

