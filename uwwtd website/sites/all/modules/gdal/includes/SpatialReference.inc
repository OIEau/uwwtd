<?php

/**
 * @file
 * wrapper arround OGRSpatialReference class
 */

/**
 * wrapper arround OGRSpatialReference class
 */
class SpatialReference {

  private $authority = NULL;
  private $authcode = NULL;

  public function __construct($spatialref) {
    if (!$spatialref->IsLocal()) {
      // local defined srs can not be correlated to an epsg number
      // TODO: it would be possible to transform local projections to a knows SRID (4326)
      // before passing them to the feeds module. This would add the cost of an additional
      // transformation

      $cstype = 'PROJCS';
      if ($spatialref->IsGeographic()) {
        $cstype = 'GEOGCS';
      }

      $this->authority = $spatialref->GetAuthorityName($cstype);
      $this->authcode = $spatialref->GetAuthorityCode($cstype);
    }
  }

  /** return the epsg number for the srs. returns null if no epsg is available */
  public function getEpsg() {
    if (($this->authority === 'EPSG') && (!empty($this->authcode) && (is_numeric($this->authcode)))) {
      return $this->authcode;
    }
  }
}


// vim: ft=php tabstop=2 shiftwidth=2
