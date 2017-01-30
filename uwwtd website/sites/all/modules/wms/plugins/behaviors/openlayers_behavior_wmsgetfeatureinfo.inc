<?php
// $Id$

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * WMSGetfeatureinfo Behavior
 */

$plugin = array(
  'openlayers_behavior_wmsgetfeatureinfo' => array(
    'title' => t('Get Feature Info'),
    'description' => t('Queries WMS server for more info about features on the map.'),
    'type' => 'layer',
    'behavior' => array(
      'file' => 'openlayers_behavior_wmsgetfeatureinfo.inc',
      'class' => 'openlayers_behavior_wmsgetfeatureinfo',
      'parent' => 'openlayers_behavior',
    ),
  ),
);


class openlayers_behavior_wmsgetfeatureinfo extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array();
  }

  function options_form($defaults = array()) {
    /*
<Format>application/vnd.esri.wms_raw_xml</Format>
<Format>application/vnd.esri.wms_featureinfo_xml</Format>
<Format>application/vnd.ogc.wms_xml</Format>
<Format>application/geojson</Format>
<Format>text/xml</Format>
<Format>text/html</Format>
<Format>text/plain</Format>

    */
      
    $info_formats = array(
      'text/plain' => 'text/plain',
      'text/html' => 'text/html',
      'text/xml' => 'text/xml',
      'application/vnd.ogc.gml' => 'application/vnd.ogc.gml',
      'application/vnd.ogc.wms_xml' => 'application/vnd.ogc.wms_xml',
      'application/geojson' => 'application/geojson',
      'application/vnd.esri.wms_featureinfo_xml' => 'application/vnd.esri.wms_featureinfo_xml',
      'application/vnd.esri.wms_raw_xml' => 'application/vnd.esri.wms_raw_xml',
    );
    
    $layers = array();
    
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if ($layer->data['isBaseLayer'] != 1 && $layer->data['layer_handler']=='wms') { 
        $layers[] = $layer->name;
      }
    }
    
    
    
    // TODO only return form when there is at least 1 WMS layer
    $configuration_form = array(
      'getfeatureinfo_usevisiblelayers' => array(
        '#title' => "Use All Visible Layers",
        '#type' => 'checkbox',
        '#description' => t('Query all visible WMS layers on the map.'),
        '#default_value' => isset($defaults['getfeatureinfo_usevisiblelayers']) ?
          $defaults['getfeatureinfo_usevisiblelayers'] : TRUE,
      ),
      'getfeatureinfo_layers' => array(
        '#title' => "Only Query",
        '#type' => 'select',
        '#options' => $this->map['layers'], // TODO filter this on WMS layers only
        '#description' => t("Select the layer from which points are pulled. This must be a WMS layer. 'Use Visible Layers' must be unchecked for this option to have effect"), //TODO Fix this in javascript
        '#default_value' => isset($defaults['getfeatureinfo_layers']) ?
          $defaults['getfeatureinfo_layers'] : NULL,
        '#states' => array(
          // Only show this field when the region is selected.
          'visible' => array(
            ':input[name="behaviors[openlayers_behavior_wmsgetfeatureinfo][options_set][options][getfeatureinfo_usevisiblelayers]"]' => array('checked' => FALSE),
          ),
        ),
      ),
      'getfeatureinfo_info_format' => array(
        '#title' => "Default Format",
        '#type' => 'select',
        '#options' => $info_formats,
        '#description' => t("The format the server should return. text formats are displayed as-is. The Gml
          format is displayed in a Drupal table, and themeable."),
        '#default_value' => isset($defaults['getfeatureinfo_info_format']) ?
          $defaults['getfeatureinfo_info_format'] : NULL,
      ),
      'getfeatureinfo_highlight' => array(
        '#title' => "Highlight selected features",
        '#type' => 'checkbox',
        '#description' => t("Highlight selected feature on the map."),
        '#default_value' => isset($defaults['getfeatureinfo_highlight']) ?
            $defaults['getfeatureinfo_highlight'] : FALSE,
        '#states' => array(
            'visible' => array(
                ':input[name = "behaviors[openlayers_behavior_wmsgetfeatureinfo][options_set][options][getfeatureinfo_info_format]"]' => array('value' => 'application/vnd.ogc.gml'),
            ),
        ),
      ),
      'getfeatureinfo_htmlelement' => array(
        '#type' => 'textfield',
        '#description' => t("An HTML element (#id!) that will be filled
          with the query result. Ommit the # here. If you use the GetFeatureInfo block, the value should be 'getfeatureinfo'"),
        '#default_value' => isset($defaults['getfeatureinfo_htmlelement']) ?
          $defaults['getfeatureinfo_htmlelement'] : "getfeatureinfo",
      ),
      'getfeatureinfo_properties' => array(
        '#title' => t("Properties"),
        '#type' => 'textfield',
        '#description' => t("Properties to query. If empty, all available properties are returned. Use 'name1,name2,...' syntax for single layer and '(layer1name1,layer1name2)(layer2name1,layer2name2)...' for multiple layers. Avoid spaces."),
        '#default_value' => isset($defaults['getfeatureinfo_properties']) ?
          $defaults['getfeatureinfo_properties'] : NULL,
      ),
      'getfeatureinfo_feature_count' => array(
        '#title' => t("Feature count"),
        '#type' => 'textfield',
        '#description' => t("Number of features to return"),
        '#default_value' => isset($defaults['getfeatureinfo_feature_count']) ?
          $defaults['getfeatureinfo_feature_count'] : 50,
      ),
    );
    
    foreach ($layers as $key => $layer_name) {
      $configuration_form['getfeatureinfo_layer_' . $layer_name] = array(
        '#title' => $layer_name,
        '#type' => 'select',
        '#options' => $info_formats,
        '#default_value' =>  isset($defaults['getfeatureinfo_layer_' . $layer_name]) ? $defaults['getfeatureinfo_layer_' . $layer_name] : array(),
        '#description' => t("The format the server should return. text formats are displayed as-is. The Gml and geoJson formats are displayed in a Drupal table, and themeable."),
      );
    }
    
    return $configuration_form;
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'wms') . '/plugins/behaviors/js/openlayers_behavior_wmsgetfeatureinfo.js');
    return $this->options;
  }
}

