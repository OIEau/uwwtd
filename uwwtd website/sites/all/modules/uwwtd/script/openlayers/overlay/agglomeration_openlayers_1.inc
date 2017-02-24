<?php

$openlayers_layers = new openlayers_layer_type_openlayers_views_vector();
$openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
$openlayers_layers->api_version = 1;
$openlayers_layers->name = 'agglomeration_openlayers_1';
$openlayers_layers->title = 'Agglomeration connection map and list - Premier plan (overlay) de données OpenLayers';
$openlayers_layers->description = date('Y-m-d H:i:s');
$openlayers_layers->data = array(
  'layer_handler' => 'openlayers_views_vector',
  'layer_type' => 'openlayers_views_vector',
  'projection' => array(
    0 => 'EPSG:4326',
  ),
  'isBaseLayer' => FALSE,
  'vector' => TRUE,
  'type' => 'Vector',
  'url' => array(),
  'options' => array(
    'rendererOptions' => array(
      'yOrdering' => TRUE,
    ),
  ),
  'events' => array(),
  'views' => array(
    'view' => 'agglomeration',
    'display' => 'openlayers_1',
  ),
);
