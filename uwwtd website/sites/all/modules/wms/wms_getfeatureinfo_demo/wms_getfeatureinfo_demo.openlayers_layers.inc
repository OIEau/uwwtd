<?php
/**
 * @file
 * wms_getfeatureinfo_demo.openlayers_layers.inc
 */

/**
 * Implements hook_openlayers_layers().
 */
function wms_getfeatureinfo_demo_openlayers_layers() {
  $export = array();

  $openlayers_layers = new stdClass();
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'canada_bathymetry';
  $openlayers_layers->title = 'Canada Bathymetry';
  $openlayers_layers->description = 'Canada Bathymetry on Demis2';
  $openlayers_layers->data = array(
    'base_url' => 'http://www2.dmsolutions.ca/cgi-bin/mswms_gmap',
    'params' => array(
      'buffer' => '2',
      'ratio' => '1.5',
      'singleTile' => 0,
    ),
    'options' => array(
      'TRANSPARENT' => 0,
      'exceptions' => 'application/vnd.ogc.se_inimage',
      'format' => 'image/png',
      'layers' => array(
        0 => 'bathymetry',
      ),
      'styles' => '',
    ),
    'projection' => array(
      0 => 'EPSG:4326',
    ),
    'isBaseLayer' => 1,
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
  $export['canada_bathymetry'] = $openlayers_layers;

  $openlayers_layers = new stdClass();
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'canada_park';
  $openlayers_layers->title = 'Canada Park';
  $openlayers_layers->description = 'Canada Park on Demis2';
  $openlayers_layers->data = array(
    'base_url' => 'http://www2.dmsolutions.ca/cgi-bin/mswms_gmap',
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
        0 => 'park',
      ),
      'styles' => '',
    ),
    'projection' => array(
      0 => 'EPSG:4326',
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
  $export['canada_park'] = $openlayers_layers;

  $openlayers_layers = new stdClass();
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'canada_populated_place';
  $openlayers_layers->title = 'Canada Populated Place';
  $openlayers_layers->description = 'Canada Populated Places on Demis2';
  $openlayers_layers->data = array(
    'base_url' => 'http://www2.dmsolutions.ca/cgi-bin/mswms_gmap',
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
        0 => 'popplace',
      ),
      'styles' => '',
    ),
    'projection' => array(
      0 => 'EPSG:4326',
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
  $export['canada_populated_place'] = $openlayers_layers;

  $openlayers_layers = new stdClass();
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'canada_rail';
  $openlayers_layers->title = 'Canada Rail';
  $openlayers_layers->description = 'Canada Rail on Demis2';
  $openlayers_layers->data = array(
    'base_url' => 'http://www2.dmsolutions.ca/cgi-bin/mswms_gmap',
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
        0 => 'rail',
      ),
      'styles' => '',
    ),
    'projection' => array(
      0 => 'EPSG:4326',
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
  $export['canada_rail'] = $openlayers_layers;

  return $export;
}
