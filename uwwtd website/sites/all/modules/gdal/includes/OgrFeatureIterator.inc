<?php

/**
 * @file
 * Iterator to iterate over features in an OGRLayer.
 */

class OgrFeatureIterator implements Iterator {

  private $ogrLayer = NULL;
  private $fieldIndices = array();

  var $layerTitle = NULL;

  /** srid of the dataset. will be null if not accessible */
  private $srid = NULL;

  /** the name for the geometry column */
  private $geomFieldName = NULL;

  /** the name for the featureid field */
  private $fidFieldName = NULL;

  /** default SRID if the layer does not provide its own */
  private $defaultSrid = NULL;

  /**
   * the current position in the list of features.
   * This is redundant information as the position is also tracked inside
   * the GDAL library
   */
  private $position = 0;

  /**
   * @param $ogrLayer OGRLayer
   */
  public function __construct($ogrLayer, $geomFieldName, $defaultSrid, $fidFieldName) {
    $this->ogrLayer = $ogrLayer;
    $this->geomFieldName = $geomFieldName;
    $this->fidFieldName = $fidFieldName;
    $this->defaultSrid = $defaultSrid;

    if (empty($this->geomFieldName)) {
      throw new GdalModuleException(t("No name for geometry field given."));
    }

    // collect field indices
    $ogrLayerDefn = $this->ogrLayer->GetLayerDefn();
    for ($fieldIdx = 0; $fieldIdx < $ogrLayerDefn->GetFieldCount(); $fieldIdx) {
      $fieldDefn = $ogrLayerDefn->GetFieldDefn($fieldIdx);
      $this->fieldIndices[$fieldDefn->GetNameRef()] = $fieldIdx;
      $fieldIdx++;
    }

    $this->layerTitle = $ogrLayerDefn->GetName();

    // attempt to find the SRID of the ogr layer
    $ogrSpatialRef = $this->ogrLayer->GetSpatialRef();
    if ($ogrSpatialRef) {
        $spatialref = new SpatialReference($ogrSpatialRef);
        $this->srid = $spatialref->getEpsg();
        $ogrSpatialRef = NULL;
    }
  }


  public function __destruct() {
    if ($this->ogrLayer) {
      $this->ogrLayer = NULL;
    }
  }
  /**
   * return the number of features in this layer
   */
  public function getFeatureCount() {
    return $this->ogrLayer->GetFeatureCount(True);
  }

  /**
   * collect the list of fields in this layer
   */
  public function getLayerFields() {
    $layerFields = array();
    $layerDefn = $this->ogrLayer->GetLayerDefn();
    if (!$layerDefn) {
      throw new GdalModuleException(t("Could not fetch layer definition"));
    }
    for ($fieldDefnIdx = 0; $fieldDefnIdx < $layerDefn->GetFieldCount(); $fieldDefnIdx++) {
      $fieldDefn = $layerDefn->GetFieldDefn($fieldDefnIdx);
      if (!$fieldDefn) {
        throw new GdalModuleException(t("Could not fetch field definition"));
      }
      $fieldName = $fieldDefn->GetNameRef();
      $layerFields[$fieldName] = array(
        'name' => $fieldName
      );
    }
    // append the geometry column which is not part of the fields in the layerDefn
    $layerFields[$this->geomFieldName] = array(
      'name' => $this->geomFieldName
    );
    return $layerFields;
  }

  private function returnFeature($ogrFeature) {
    $feature = NULL;
    if ($ogrFeature) {
      $feature = array();

      foreach ($this->fieldIndices as $fieldName => $fieldIdx) {
        $feature[$fieldName] = $ogrFeature->GetFieldAsString($fieldIdx);
      }

      // add feature id
      if (!empty($this->fidFieldName)) {
        $feature[$this->fidFieldName] = $ogrFeature->GetFID();
      }

      $srid = $this->srid;

      // add geometry
      $ogrGeometry = $ogrFeature->GetGeometryRef();
      $wkt = "";
      if ($ogrGeometry) {
        $wkt = $ogrGeometry->ExportToWkt();

        // SRID
        // TODO: see comment in SpatialReference.inc regarding local coordinate systems
        $ogrSpatialRef = $ogrGeometry->GetSpatialReference();
        if ($ogrSpatialRef) {
          $spatialref = new SpatialReference($ogrSpatialRef);
          $geom_srid = $spatialref->getEpsg();
          if (!$srid || $geom_srid) {
            $srid = $geom_srid;
          }
        }
      }

      if (empty($srid)) {
        $srid = $this->defaultSrid;
      }

      // build EWKT if the SRID is set
      if ($srid) {
        $wkt = "SRID=$srid;$wkt";
      }

      $feature[$this->geomFieldName] = $wkt;
    }
    return $feature;
  }

  function rewind() {
    $this->position = 0;
    $this->ogrLayer->ResetReading();
  }

  function key() {
    return $this->position;
  }

  function seek($pos) {
    if ($pos === $this->position) {
      return;
    }

    $supportsSeek = $this->ogrLayer->TestCapability("FastSetNextByIndex");
    if ($supportsSeek) {
      $ogrerr = $this->ogrLayer->SetNextByIndex($pos);
      if ($ogrerr !== 0) {
        throw new GdalModuleException("Could not seek to position $pos in OGRLayer. OGRErr: $ogrerr");
      }
      $this->position = $pos;
    }
    else {
      // layer does not support seek operations. so iterate to the target position

      if ($pos < $this->position) {
        // only way to iterate backwards is to start from the first feature again
        $this->rewind();
      }
      while ($this->position < $pos) {
        $ogrFeature = $this->ogrLayer->GetNextFeature();
        if (!$ogrFeature) {
          // no more iteration possible although the target position has not been reached
          // so might as well stop iterating here.
          $this->position = $pos;
        }
        else {
          $this->position++;
          $ogrFeature->DestroyFeature($ogrFeature);
        }
      }
    }
  }

  function getPosition() {
    return $this->position;
  }

  function current() {
    $feature = Null;
    $ogrFeature = $this->ogrLayer->GetFeature($this->position);
    if ($ogrFeature) {
      $feature = $this->returnFeature($ogrFeature);
      // ensure that the memory associated with this feature is freed again
      $ogrFeature->DestroyFeature($ogrFeature);
    }
    return $feature;
  }

  function next() {
    $this->position++;
    $feature = Null;
    $ogrFeature = $this->ogrLayer->GetNextFeature();
    if ($ogrFeature) {
      $feature = $this->returnFeature($ogrFeature);
      // ensure that the memory associated with this feature is freed again
      $ogrFeature->DestroyFeature($ogrFeature);
    }
    return $feature;
  }

  function valid() {
    return $this->ogrLayer->GetFeature($this->position) !== NULL;
  }
}


// vim: ft=php tabstop=2 shiftwidth=2
