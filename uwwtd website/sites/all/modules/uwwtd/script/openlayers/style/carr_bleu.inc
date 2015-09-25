<?php

$openlayers_styles = new stdClass();
$openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
$openlayers_styles->api_version = 1;
$openlayers_styles->name = 'carr_bleu';
$openlayers_styles->title = 'Carré bleu';
$openlayers_styles->description = date('Y-m-d H:i:s');
$openlayers_styles->data = array(
  'externalGraphic' => 'sites/default/files/pictures/carre.png',
  'pointRadius' => 6,
  'fillColor' => '#EE9900',
  'strokeColor' => '#EE9900',
  'strokeWidth' => 1,
  'fillOpacity' => 1,
  'strokeOpacity' => 1,
  'strokeLinecap' => 'round',
  'strokeDashstyle' => 'solid',
  'graphicOpacity' => 1,
  'labelAlign' => 'cm',
);
