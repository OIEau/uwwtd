<?php
/**
 * @file
 * wms_getfeatureinfo_demo.openlayers_maps.inc
 */

/**
 * Implements hook_openlayers_maps().
 */
function wms_getfeatureinfo_demo_openlayers_maps() {
  $export = array();

  $openlayers_maps = new stdClass();
  $openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'canada_getfeatureinfo_demo';
  $openlayers_maps->title = 'Canada Getfeatureinfo Demo';
  $openlayers_maps->description = 'Canada Getfeatureinfo Demo';
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => 'proxy?request=',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-99.49218748936487, 56.94497417797037',
        'zoom' => '3',
      ),
      'restrict' => array(
        'restrictextent' => 1,
        'restrictedExtent' => '-16319611.284075,4852834.0509,-4891969.809375,11818999.05945',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_wmsgetfeatureinfo' => array(
        'getfeatureinfo_usevisiblelayers' => 1,
        'getfeatureinfo_layers' => 'canada_bathymetry',
        'getfeatureinfo_info_format' => 'application/vnd.ogc.gml',
        'getfeatureinfo_htmlelement' => 'getfeatureinfo',
      ),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_attribution' => array(
        'separator' => '',
      ),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'sortBaseLayer' => '0',
        'roundedCorner' => 1,
        'roundedCornerColor' => 'darkblue',
        'maximizeDefault' => 0,
        'div' => '',
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
        'zoomBoxEnabled' => 1,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(
        'zoomWorldIcon' => 0,
        'panIcons' => 1,
      ),
    ),
    'default_layer' => 'canada_bathymetry',
    'layers' => array(
      'canada_bathymetry' => 'canada_bathymetry',
      'canada_populated_place' => 'canada_populated_place',
      'canada_rail' => 'canada_rail',
      'canada_park' => 'canada_park',
    ),
    'layer_weight' => array(
      'canada_populated_place' => '0',
      'canada_rail' => '0',
      'canada_park' => '0',
      'canada_openlayers_1' => '0',
      'openlayers_searchresult_layer' => '0',
      'osm_4326_hybrid' => '0',
      'openlayers_kml_example' => '0',
      'openlayers_geojson_picture_this' => '0',
      'geofield_formatter' => '0',
    ),
    'layer_styles' => array(
      'geofield_formatter' => '0',
      'osm_4326_hybrid' => '0',
      'openlayers_kml_example' => '0',
      'openlayers_geojson_picture_this' => '0',
      'openlayers_searchresult_layer' => '0',
      'canada_openlayers_1' => '0',
      'canada_populated_place' => '0',
      'canada_rail' => '0',
      'canada_park' => '0',
    ),
    'layer_styles_select' => array(
      'geofield_formatter' => '0',
      'osm_4326_hybrid' => '0',
      'openlayers_kml_example' => '0',
      'openlayers_geojson_picture_this' => '0',
      'openlayers_searchresult_layer' => '0',
      'canada_openlayers_1' => '0',
      'canada_populated_place' => '0',
      'canada_rail' => '0',
      'canada_park' => '0',
    ),
    'layer_styles_temporary' => array(
      'geofield_formatter' => '0',
      'osm_4326_hybrid' => '0',
      'openlayers_kml_example' => '0',
      'openlayers_geojson_picture_this' => '0',
      'openlayers_searchresult_layer' => '0',
      'canada_openlayers_1' => '0',
      'canada_populated_place' => '0',
      'canada_rail' => '0',
      'canada_park' => '0',
    ),
    'layer_activated' => array(
      'canada_populated_place' => 'canada_populated_place',
      'canada_rail' => 'canada_rail',
      'canada_park' => 'canada_park',
      'geofield_formatter' => 0,
      'osm_4326_hybrid' => 0,
      'openlayers_kml_example' => 0,
      'openlayers_geojson_picture_this' => 0,
      'openlayers_searchresult_layer' => 0,
      'canada_openlayers_1' => 0,
    ),
    'layer_switcher' => array(
      'canada_populated_place' => 'canada_populated_place',
      'canada_rail' => 'canada_rail',
      'canada_park' => 'canada_park',
      'geofield_formatter' => 0,
      'osm_4326_hybrid' => 0,
      'openlayers_kml_example' => 0,
      'openlayers_geojson_picture_this' => 0,
      'openlayers_searchresult_layer' => 0,
      'canada_openlayers_1' => 0,
    ),
    'projection' => 'EPSG:4326',
    'displayProjection' => 'EPSG:4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default',
      'temporary' => 'default',
    ),
  );
  $export['canada_getfeatureinfo_demo'] = $openlayers_maps;

  return $export;
}
