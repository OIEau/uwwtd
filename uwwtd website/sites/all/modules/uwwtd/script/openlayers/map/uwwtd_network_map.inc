<?php

$openlayers_maps = new stdClass();
$openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
$openlayers_maps->api_version = 1;
$openlayers_maps->name = 'uwwtd_network_map';
$openlayers_maps->title = 'Waste water treatment network map';
$openlayers_maps->description = date('Y-m-d H:i:s');
$openlayers_maps->data = array(
  'width' => 'auto',
  'height' => '500px',
  'image_path' => 'sites/all/modules/openlayers/themes/default_dark/img/',
  'css_path' => 'sites/all/modules/openlayers/themes/default_dark/style.css',
  'proxy_host' => '',
  'hide_empty_map' => 0,
  'center' => array(
    'initial' => array(
      'centerpoint' => '-6.32812500605299, 53.33087298053941',
      'zoom' => '6',
    ),
    'restrict' => array(
      'restrictextent' => 0,
      'restrictedExtent' => '',
    ),
  ),
  'behaviors' => array(
    'openlayers_behavior_fullscreen' => array(
      'activated' => 0,
    ),
//     'openlayers_behavior_keyboarddefaults' => array(),  
    'openlayers_behavior_attribution' => array(
      'separator' => '; ',
    ),
    'openlayers_behavior_layerswitcher' => array(
      'ascending' => 1,
      'sortBaseLayer' => '0',
      'roundedCorner' => 1,
      'roundedCornerColor' => '#222222',
      'maximizeDefault' => 0,
      'div' => '',
    ),
    'openlayers_behavior_navigation' => array(
      'zoomWheelEnabled' => 1,
      'zoomBoxEnabled' => 1,
      'documentDrag' => 0,
    ),
    'openlayers_plus_behavior_legend' => array(
      'layers' => array(
        'uwwtp_map_openlayers_1' => 'uwwtp_map_openlayers_1',
      ),
    ),
    'openlayers_plus_behavior_scalepoints' => array(
      'min' => array(
        'value' => '0',
        'pointRadius' => '3',
        'fillOpacity' => '0.4',
        'strokeWidth' => '1',
      ),
      'max' => array(
        'value' => '100000',
        'pointRadius' => '10',
        'fillOpacity' => '0.7',
        'strokeWidth' => '1',
      ),
      'distinct' => '3',
      'arbitrary_classes' => '0|2000
2000|10000
10000|',
      'method' => 'area',
      'fields' => 'field_physicalcapacityactivity',
      'min_label' => '',
      'fields_colors' => 'field_uwwcompliance',
      'fields_colors_values' => 'Compliant|Compliant|#3cc53c
Nonrelevant|Non relevant|#5ddfff
Noinformation|No information|#5c5c5c
Noncompliant|Non Compliant|#d92424
Compliantonequipmentonly|Compliant on equipment only|#ea8b2e',
    ),
    'openlayers_behavior_panzoombar' => array(
      'zoomWorldIcon' => 0,
      'panIcons' => 1,
    ),
    'openlayers_behavior_popup' => array(
      'layers' => array(
        'uwwtp_map_openlayers_1' => 'uwwtp_map_openlayers_1',
      ),
      'panMapIfOutOfView' => 0,
      'keepInMap' => 1,
      'zoomToPoint' => 0,
      'zoomToCluster' => 0,
    ),
    'openlayers_behavior_zoomtolayer' => array(
      'zoomtolayer' => array(
        'uwwtp_map_openlayers_1' => 'uwwtp_map_openlayers_1',
        'google_satellite' => 0,
        'google_hybrid' => 0,
        'google_normal' => 0,
        'google_physical' => 0,
      ),
      'point_zoom_level' => '8',
      'zoomtolayer_scale' => '1',
    ),
  ),
  'default_layer' => 'google_normal',
  'layers' => array(
    'google_satellite' => 'google_satellite',
    'google_hybrid' => 'google_hybrid',
    'google_normal' => 'google_normal',
    'google_physical' => 'google_physical',
    'waste_water_network_openlayers_4' => 'waste_water_network_openlayers_4',
    'waste_water_network_openlayers_3' => 'waste_water_network_openlayers_3',
    'waste_water_network_openlayers_2' => 'waste_water_network_openlayers_2',
    'waste_water_network_openlayers_1' => 'waste_water_network_openlayers_1',
  ),
  'layer_weight' => array(
    'agglomeration_conformity_openlayers_1' => '0',
    'agglomeration_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'postgis_formatter_layer' => '0',
    'uwwtp_map_openlayers_1' => '0',
    'waste_water_network_openlayers_4' => '0',
    'waste_water_network_openlayers_3' => '0',
    'openlayers_geojson_picture_this' => '0',
    'openlayers_kml_example' => '0',
    'waste_water_network_page' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'geofield_formatter' => '0',
  ),
  'layer_styles' => array(
    'geofield_formatter' => '0',
    'openlayers_kml_example' => '0',
    'openlayers_geojson_picture_this' => '0',
    'waste_water_network_page' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'waste_water_network_openlayers_3' => '0',
    'waste_water_network_openlayers_4' => '0',
    'agglomeration_openlayers_1' => '0',
    'agglomeration_conformity_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'postgis_formatter_layer' => '0',
    'uwwtp_map_openlayers_1' => '0',
  ),
  'layer_styles_select' => array(
    'geofield_formatter' => '0',
    'openlayers_kml_example' => '0',
    'openlayers_geojson_picture_this' => '0',
    'waste_water_network_page' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'waste_water_network_openlayers_3' => '0',
    'waste_water_network_openlayers_4' => '0',
    'agglomeration_openlayers_1' => '0',
    'agglomeration_conformity_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'postgis_formatter_layer' => '0',
    'uwwtp_map_openlayers_1' => '0',
  ),
  'layer_styles_temporary' => array(
    'geofield_formatter' => '0',
    'openlayers_kml_example' => '0',
    'openlayers_geojson_picture_this' => '0',
    'waste_water_network_page' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'waste_water_network_openlayers_3' => '0',
    'waste_water_network_openlayers_4' => '0',
    'agglomeration_openlayers_1' => '0',
    'agglomeration_conformity_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'postgis_formatter_layer' => '0',
    'uwwtp_map_openlayers_1' => '0',
  ),
  'layer_activated' => array(
    'waste_water_network_openlayers_1' => 'waste_water_network_openlayers_1',
    'geofield_formatter' => 0,
    'openlayers_kml_example' => 0,
    'openlayers_geojson_picture_this' => 0,
    'waste_water_network_page' => 0,
    'waste_water_network_openlayers_2' => 0,
    'waste_water_network_openlayers_3' => 0,
    'waste_water_network_openlayers_4' => 0,
    'agglomeration_openlayers_1' => 0,
    'agglomeration_conformity_openlayers_1' => 0,
    'uwwtp_map_openlayers_1' => 0,
    'uwwtp_treatment_openlayers_2' => 0,
    'postgis_formatter_layer' => 0,
  ),
  'layer_switcher' => array(
    'waste_water_network_openlayers_2' => 'waste_water_network_openlayers_2',
    'waste_water_network_openlayers_1' => 'waste_water_network_openlayers_1',
    'waste_water_network_openlayers_3' => 'waste_water_network_openlayers_3',
    'waste_water_network_openlayers_4' => 'waste_water_network_openlayers_4',
    'geofield_formatter' => 0,
    'openlayers_kml_example' => 0,
    'openlayers_geojson_picture_this' => 0,
    'waste_water_network_page' => 0,
    'agglomeration_openlayers_1' => 0,
    'agglomeration_conformity_openlayers_1' => 0,
    'uwwtp_map_openlayers_1' => 0,
    'uwwtp_treatment_openlayers_2' => 0,
    'postgis_formatter_layer' => 0,
  ),
  'projection' => 'EPSG:3857',
  'displayProjection' => 'EPSG:4326',
  'styles' => array(
    'default' => 'default',
    'select' => 'default_select',
    'temporary' => 'default',
  ),
);
