<?php

$openlayers_layers = new stdClass();
$openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
$openlayers_layers->api_version = 1;
$openlayers_layers->name = 'oieau_rbdsu';
$openlayers_layers->title = 'River basin districts subunits';
$openlayers_layers->description = '';
$openlayers_layers->data = array(
  'base_url' => 'https://maps.oieau.fr/services/background',
  'params' => array(
    'buffer' => '2',
    'ratio' => '1.5',
    'singleTile' => 0,
  ),
  'options' => array(
    'TRANSPARENT' => 'true',
    'exceptions' => 'application/vnd.ogc.se_inimage',
    'format' => 'image/png',
    'layers' => array(
      0 => 'RBDSU',
    ),
    'styles' => '',
  ),
  'projection' => array(
    0 => 'EPSG:3857',
    1 => 'EPSG:4326',
    2 => 'EPSG:900913',
  ),
  'isBaseLayer' => 0,
  'layer_type' => 'openlayers_layer_type_wms',
  'layer_handler' => 'wms',
  'transitionEffect' => 'resize',
  'weight' => 0,
  'serverResolutions' => array(
    0 => 156543.0339,
    1 => 78271.51695,
    2 => 39135.758475,
    3 => 19567.8792375,
    4 => 9783.93961875,
    5 => 4891.969809375,
    6 => 2445.9849046875,
    7 => 1222.9924523438,
    8 => 611.49622617188,
    9 => 305.74811308594,
    10 => 152.87405654297,
    11 => 76.437028271484,
    12 => 38.218514135742,
    13 => 19.109257067871,
    14 => 9.5546285339355,
    15 => 4.7773142669678,
    16 => 2.3886571334839,
    17 => 1.1943285667419,
    18 => 0.59716428337097,
    19 => 0.29858214169741,
    20 => 0.1492910708487,
    21 => 0.074645535424352,
  ),
  'resolutions' => array(
    0 => 156543.0339,
    1 => 78271.51695,
    2 => 39135.758475,
    3 => 19567.8792375,
    4 => 9783.93961875,
    5 => 4891.969809375,
    6 => 2445.9849046875,
    7 => 1222.9924523438,
    8 => 611.49622617188,
    9 => 305.74811308594,
    10 => 152.87405654297,
    11 => 76.437028271484,
    12 => 38.218514135742,
    13 => 19.109257067871,
    14 => 9.5546285339355,
    15 => 4.7773142669678,
    16 => 2.3886571334839,
    17 => 1.1943285667419,
    18 => 0.59716428337097,
    19 => 0.29858214169741,
    20 => 0.1492910708487,
    21 => 0.074645535424352,
  ),
);
