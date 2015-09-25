<?php

$openlayers_styles = new stdClass();
$openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
$openlayers_styles->api_version = 1;
$openlayers_styles->name = 'agglomerations_style';
$openlayers_styles->title = 'Agglomerations style';
$openlayers_styles->description = date('Y-m-d H:i:s');
$openlayers_styles->data = array(
  'pointRadius' => 6,
  'fillColor' => '#F2EB0C',
  'strokeColor' => '#CCC616',
  'strokeWidth' => 1,
  'fillOpacity' => 0.9,
  'strokeOpacity' => 0.9,
  'strokeLinecap' => 'square',
  'strokeDashstyle' => 'solid',
  'graphicOpacity' => 1,
  'labelAlign' => 'cm',
);
