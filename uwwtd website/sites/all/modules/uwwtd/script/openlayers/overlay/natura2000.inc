<?php

$openlayers_layers = new stdClass();
$openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
$openlayers_layers->api_version = 1;
$openlayers_layers->name = 'natura_2000';
$openlayers_layers->title = 'Natura 2000';
$openlayers_layers->description = 'Natura 2000 is the key instrument to protect biodiversity in the European Union. It is an ecological network of protected areas, set up to ensure the survival of Europe\'s most valuable species and habitats. Natura 2000 is based on the 1979 Birds Directive and the 1992 Habitats Directive. The green infrastructure it provides safeguards numerous ecosystem services and ensures that Europe\'s natural systems remain healthy and resilient.';
$openlayers_layers->data = array(
  //'base_url' => 'http://bio.discomap.eea.europa.eu/arcgis/services/ProtectedSites/EUNIS_Website_Dyna_WM/MapServer/WMSServer',
  'base_url' => 'http://bio.discomap.eea.europa.eu/arcgis/services/ProtectedSites/Natura2000Sites/MapServer/WMSServer',
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
      0 => '0,1,2',
    ),
    'styles' => '',
  ),
  'projection' => array(
    0 => 'EPSG:3857',
    1 => 'EPSG:4326',
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
