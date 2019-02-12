<?php
$openlayers_maps = new stdClass();
$openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
$openlayers_maps->api_version = 1;
$openlayers_maps->name = 'guadeloupe';
$openlayers_maps->title = 'guadeloupe';
$openlayers_maps->description = '';
$openlayers_maps->data = array(
  'width' => '150px',
  'height' => '110px',
  'image_path' => 'sites/all/modules/openlayers/themes/default_dark/img/',
  'css_path' => 'sites/all/modules/openlayers/themes/default_dark/style.css',
  'proxy_host' => '',
  'hide_empty_map' => 0,
  'center' => array(
    'initial' => array(
      'centerpoint' => '-61.52343749616607, 16.29905101863868',
      'zoom' => '7',
    ),
    'restrict' => array(
      'restrictextent' => 0,
      'restrictedExtent' => '',
    ),
  ),
  'behaviors' => array(
    'openlayers_behavior_keyboarddefaults' => array(),
    'openlayers_behavior_navigation' => array(
      'zoomWheelEnabled' => 1,
      'zoomBoxEnabled' => 0,
      'documentDrag' => 0,
    ),
  ),
  'default_layer' => 'test',
  'layers' => array(
    'osm_mapnik' => 'osm_mapnik',
    'oieau_nuts' => 'oieau_nuts',
  ),
  'layer_weight' => array(
    'uwwtp_n_pressure_' => '0',
    'openlayers_kml_example' => '0',
    'oieau_eprtr_ptot' => '0',
    'bathing_water' => '0',
    'oieau_rbd_rbdsu' => '0',
    'soe_monitoring_stations' => '0',
    'soe_bod' => '0',
    'soe_nh3' => '0',
    'wise_soe_phosphorus' => '0',
    'geofield_formatter' => '0',
    'openlayers_geojson_picture_this' => '0',
    'oieau_nuts_3' => '0',
    'agglomeration_openlayers_1' => '0',
    'agglomeration_conformity_openlayers_1' => '0',
    'test_receiving_areas_openlayers_1' => '0',
    'uwwtp_map_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_3' => '0',
    'wms_highlight_layer' => '0',
    'oieau_rbdsu' => '0',
    'oieau_nuts_1' => '0',
    'uwwtp_p_pressure_' => '0',
    'oieau_nuts_2' => '0',
    'agglomerations' => '0',
    'natura_2000' => '0',
    'dcp' => '0',
    'compliance_map' => '0',
    'agglomeration_treatment' => '0',
    'uwwtp_treatment' => '0',
    'uwwtp_cod_pressure_' => '0',
    'oieau_nuts_0' => '0',
    'oieau_rbd' => '0',
    'uwwtp_bod_pressure_' => '0',
    'oieau_eprtr_ntot' => '0',
    'oieau_eprtr_toc' => '0',
    'corine_land_cover_2012' => '0',
    'soe_nitrates_2011' => '0',
    'soe_phosphate_2011' => '0',
    'uwwtp' => '0',
    'rca' => '0',
    'wise_soe_nitrate' => '0',
    'uwwtp_compliance_map' => '0',
    'oieau_nuts' => '0',
  ),
  'layer_styles' => array(
    'uwwtp_n_pressure_' => '0',
    'uwwtp_p_pressure_' => '0',
    'agglomerations' => '0',
    'natura_2000' => '0',
    'dcp' => '0',
    'compliance_map' => '0',
    'agglomeration_treatment' => '0',
    'uwwtp_treatment' => '0',
    'uwwtp_cod_pressure_' => '0',
    'oieau_nuts_0' => '0',
    'oieau_nuts_2' => '0',
    'oieau_rbd' => '0',
    'oieau_eprtr_ntot' => '0',
    'oieau_eprtr_toc' => '0',
    'corine_land_cover_2012' => '0',
    'soe_nitrates_2011' => '0',
    'soe_phosphate_2011' => '0',
    'uwwtp' => '0',
    'rca' => '0',
    'wise_soe_nitrate' => '0',
    'uwwtp_compliance_map' => '0',
    'uwwtp_bod_pressure_' => '0',
    'oieau_nuts_1' => '0',
    'oieau_nuts_3' => '0',
    'oieau_rbdsu' => '0',
    'oieau_eprtr_ptot' => '0',
    'bathing_water' => '0',
    'oieau_rbd_rbdsu' => '0',
    'soe_monitoring_stations' => '0',
    'soe_bod' => '0',
    'soe_nh3' => '0',
    'wise_soe_phosphorus' => '0',
    'geofield_formatter' => '0',
    'openlayers_kml_example' => '0',
    'openlayers_geojson_picture_this' => '0',
    'agglomeration_openlayers_1' => '0',
    'agglomeration_conformity_openlayers_1' => '0',
    'test_receiving_areas_openlayers_1' => '0',
    'uwwtp_map_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_3' => '0',
    'wms_highlight_layer' => '0',
    'oieau_nuts' => '0',
  ),
  'layer_styles_select' => array(
    'uwwtp_n_pressure_' => '0',
    'uwwtp_p_pressure_' => '0',
    'agglomerations' => '0',
    'natura_2000' => '0',
    'dcp' => '0',
    'compliance_map' => '0',
    'agglomeration_treatment' => '0',
    'uwwtp_treatment' => '0',
    'uwwtp_cod_pressure_' => '0',
    'oieau_nuts_0' => '0',
    'oieau_nuts_2' => '0',
    'oieau_rbd' => '0',
    'oieau_eprtr_ntot' => '0',
    'oieau_eprtr_toc' => '0',
    'corine_land_cover_2012' => '0',
    'soe_nitrates_2011' => '0',
    'soe_phosphate_2011' => '0',
    'uwwtp' => '0',
    'rca' => '0',
    'wise_soe_nitrate' => '0',
    'uwwtp_compliance_map' => '0',
    'uwwtp_bod_pressure_' => '0',
    'oieau_nuts_1' => '0',
    'oieau_nuts_3' => '0',
    'oieau_rbdsu' => '0',
    'oieau_eprtr_ptot' => '0',
    'bathing_water' => '0',
    'oieau_rbd_rbdsu' => '0',
    'soe_monitoring_stations' => '0',
    'soe_bod' => '0',
    'soe_nh3' => '0',
    'wise_soe_phosphorus' => '0',
    'geofield_formatter' => '0',
    'openlayers_kml_example' => '0',
    'openlayers_geojson_picture_this' => '0',
    'agglomeration_openlayers_1' => '0',
    'agglomeration_conformity_openlayers_1' => '0',
    'test_receiving_areas_openlayers_1' => '0',
    'uwwtp_map_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_3' => '0',
    'wms_highlight_layer' => '0',
    'oieau_nuts' => '0',
  ),
  'layer_styles_temporary' => array(
    'uwwtp_n_pressure_' => '0',
    'uwwtp_p_pressure_' => '0',
    'agglomerations' => '0',
    'natura_2000' => '0',
    'dcp' => '0',
    'compliance_map' => '0',
    'agglomeration_treatment' => '0',
    'uwwtp_treatment' => '0',
    'uwwtp_cod_pressure_' => '0',
    'oieau_nuts_0' => '0',
    'oieau_nuts_2' => '0',
    'oieau_rbd' => '0',
    'oieau_eprtr_ntot' => '0',
    'oieau_eprtr_toc' => '0',
    'corine_land_cover_2012' => '0',
    'soe_nitrates_2011' => '0',
    'soe_phosphate_2011' => '0',
    'uwwtp' => '0',
    'rca' => '0',
    'wise_soe_nitrate' => '0',
    'uwwtp_compliance_map' => '0',
    'uwwtp_bod_pressure_' => '0',
    'oieau_nuts_1' => '0',
    'oieau_nuts_3' => '0',
    'oieau_rbdsu' => '0',
    'oieau_eprtr_ptot' => '0',
    'bathing_water' => '0',
    'oieau_rbd_rbdsu' => '0',
    'soe_monitoring_stations' => '0',
    'soe_bod' => '0',
    'soe_nh3' => '0',
    'wise_soe_phosphorus' => '0',
    'geofield_formatter' => '0',
    'openlayers_kml_example' => '0',
    'openlayers_geojson_picture_this' => '0',
    'agglomeration_openlayers_1' => '0',
    'agglomeration_conformity_openlayers_1' => '0',
    'test_receiving_areas_openlayers_1' => '0',
    'uwwtp_map_openlayers_1' => '0',
    'uwwtp_treatment_openlayers_2' => '0',
    'waste_water_network_openlayers_1' => '0',
    'waste_water_network_openlayers_2' => '0',
    'waste_water_network_openlayers_3' => '0',
    'wms_highlight_layer' => '0',
    'oieau_nuts' => '0',
  ),
  'layer_activated' => array(
    'oieau_nuts' => 'oieau_nuts',
    'uwwtp_n_pressure_' => 0,
    'uwwtp_p_pressure_' => 0,
    'agglomerations' => 0,
    'natura_2000' => 0,
    'dcp' => 0,
    'compliance_map' => 0,
    'agglomeration_treatment' => 0,
    'uwwtp_treatment' => 0,
    'uwwtp_cod_pressure_' => 0,
    'oieau_nuts_0' => 0,
    'oieau_nuts_2' => 0,
    'oieau_rbd' => 0,
    'oieau_eprtr_ntot' => 0,
    'oieau_eprtr_toc' => 0,
    'corine_land_cover_2012' => 0,
    'soe_nitrates_2011' => 0,
    'soe_phosphate_2011' => 0,
    'uwwtp' => 0,
    'rca' => 0,
    'wise_soe_nitrate' => 0,
    'uwwtp_compliance_map' => 0,
    'uwwtp_bod_pressure_' => 0,
    'oieau_nuts_1' => 0,
    'oieau_nuts_3' => 0,
    'oieau_rbdsu' => 0,
    'oieau_eprtr_ptot' => 0,
    'bathing_water' => 0,
    'oieau_rbd_rbdsu' => 0,
    'soe_monitoring_stations' => 0,
    'soe_bod' => 0,
    'soe_nh3' => 0,
    'wise_soe_phosphorus' => 0,
    'geofield_formatter' => 0,
    'openlayers_kml_example' => 0,
    'openlayers_geojson_picture_this' => 0,
    'agglomeration_openlayers_1' => 0,
    'agglomeration_conformity_openlayers_1' => 0,
    'test_receiving_areas_openlayers_1' => 0,
    'uwwtp_map_openlayers_1' => 0,
    'uwwtp_treatment_openlayers_2' => 0,
    'waste_water_network_openlayers_1' => 0,
    'waste_water_network_openlayers_2' => 0,
    'waste_water_network_openlayers_3' => 0,
    'wms_highlight_layer' => 0,
  ),
  'layer_switcher' => array(
    'oieau_nuts' => 0,
    'uwwtp_n_pressure_' => 0,
    'uwwtp_p_pressure_' => 0,
    'agglomerations' => 0,
    'natura_2000' => 0,
    'dcp' => 0,
    'compliance_map' => 0,
    'agglomeration_treatment' => 0,
    'uwwtp_treatment' => 0,
    'uwwtp_cod_pressure_' => 0,
    'oieau_nuts_0' => 0,
    'oieau_nuts_2' => 0,
    'oieau_rbd' => 0,
    'oieau_eprtr_ntot' => 0,
    'oieau_eprtr_toc' => 0,
    'corine_land_cover_2012' => 0,
    'soe_nitrates_2011' => 0,
    'soe_phosphate_2011' => 0,
    'uwwtp' => 0,
    'rca' => 0,
    'wise_soe_nitrate' => 0,
    'uwwtp_compliance_map' => 0,
    'uwwtp_bod_pressure_' => 0,
    'oieau_nuts_1' => 0,
    'oieau_nuts_3' => 0,
    'oieau_rbdsu' => 0,
    'oieau_eprtr_ptot' => 0,
    'bathing_water' => 0,
    'oieau_rbd_rbdsu' => 0,
    'soe_monitoring_stations' => 0,
    'soe_bod' => 0,
    'soe_nh3' => 0,
    'wise_soe_phosphorus' => 0,
    'geofield_formatter' => 0,
    'openlayers_kml_example' => 0,
    'openlayers_geojson_picture_this' => 0,
    'agglomeration_openlayers_1' => 0,
    'agglomeration_conformity_openlayers_1' => 0,
    'test_receiving_areas_openlayers_1' => 0,
    'uwwtp_map_openlayers_1' => 0,
    'uwwtp_treatment_openlayers_2' => 0,
    'waste_water_network_openlayers_1' => 0,
    'waste_water_network_openlayers_2' => 0,
    'waste_water_network_openlayers_3' => 0,
    'wms_highlight_layer' => 0,
  ),
  'projection' => 'EPSG:3857',
  'displayProjection' => 'EPSG:3857',
  'styles' => array(
    'default' => 'default',
    'select' => 'default_select',
    'temporary' => 'default',
  ),
);