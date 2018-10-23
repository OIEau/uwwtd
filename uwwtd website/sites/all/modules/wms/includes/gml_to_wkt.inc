<?php
// $Id$

/**
 * @file
 * Has functions that help with interpreting gml
 */

function wms_getwkt($gml) {
  if (is_string($gml)) {
    $gml = simplexml_load_string($rawxml);
  }
  if (isset($gml->Point)) {
    $coordinates = $gml->Point->coordinates;
    return ("POINT(" . $coordinates . ")");
  }
  if (isset($gml->MultiPolygon)) {
    print $gml;
    $coordinates = $gml->MultiPolygon->polygonMember->Polygon->outerBoundaryIs->LinearRing->coordinates;
    return ("MULTIPOLYGON(" . $coordinates . ")");
  }
  if (isset($gml->MultiLineString)) {
    $coordinates = $gml->MultiLineString->lineStringMember->LineString->coordinates;
    return ("MULTILINESTRING(" . $coordinates . ")");
  }
  return '';
}
