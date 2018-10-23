<?php

/**
 * @file
 * class to read compressed and uncompressed OGR datasets.
 */

class OgrReader {

  /** the name for the geometry field */
  private $geometryFieldName = "geometry";

  /** the name for the featureid field */
  private $fidFieldName = "fid";

  private $defaultSrid = 4326;

  /** list of supported compression formats */
  private $supportedPackFormats = array('application/zip');

  /** the ogr dataset */
  private $dataset = NULL;

  private $targetDirectory = NULL;

  public function __construct($fileName) {
    $packFormat = $this->fileIsPacked($fileName);
    try {
      if ($packFormat) {
        // unpack zipped/compressed files and search for known geodata formats

        // create a temporary directory to unpack the file to
        $this->targetDirectory = rtrim(file_directory_temp(), "/") . "/ogr_" . uniqid();
        if (!file_prepare_directory($this->targetDirectory,  FILE_CREATE_DIRECTORY)) {
          throw new GdalModuleException(t("Could not create temporary directory"));
        }

        if ($packFormat == 'application/zip') {
          $zip = new ZipArchive();
          if (!$zip->open($fileName)) {
            throw new GdalModuleException(t("Could not read uploaded ZIP file"));
          }
          if (!$zip->extractTo($this->targetDirectory)) {
            throw new GdalModuleException(t("Could not extract files from uploaded ZIP file"));
          }
          $zip->close();
        }
        else {
          throw new GdalModuleException(t("Can not uncompress uploaded file"));
        }

        // find the geodata
        $geoData = file_scan_directory($this->targetDirectory, '/\.(shp|gml|kml|xml)$/i', array(
          'recurse' => True,
          'key' => 'uri' // does not seem to contain the public:// protocol here
        ));

        if (empty($geoData)) {
            throw new GdalModuleException(t("Found no supported geodata in the uploaded file."));
        }
        if (count($geoData) > 1) {
            throw new GdalModuleException(t("Found more than one dataset in the uploaded file. Please upload a file with only one dataset."));
        }

        $fileNames = array_keys($geoData);
        $this->ogrProcessFile($fileNames[0]);
      }
      else {
        $this->ogrProcessFile($fileName);
      }
    }
    catch (Exception $e) {
      // try to always delete files
      $this->recursiveDelete($this->targetDirectory);
      throw $e;
    }
  }


  public function __destruct() {
    $this->closeDataset();
    $this->recursiveDelete($this->targetDirectory);
  }

  private function closeDataset() {
    if ($this->dataset) {
      // dataset will be automatically closed if there are no more references to it. issue #3060008
      $this->dataset = NULL;
    }
  }

  private function ogrProcessFile($fileName) {
    if (!extension_loaded("gdal")) {
      throw new GdalModuleException(t("The PHP GDAL extension is not available"));
    }

    OGRRegisterAll();
    $this->dataset = OGRSFDriverRegistrar::Open($fileName, FALSE /* readonly */);
    if (!$this->dataset) {
      throw new GdalModuleException(t("Could not open vector dataset. Unknown format."));
    }
    if ($this->dataset->GetLayerCount() == 0) {
      throw new GdalModuleException(t("Dataset contains no layers."));
    }
    if ($this->dataset->GetLayerCount() > 1) {
      throw new GdalModuleException(t("Dataset contains more than one layer. At this point only datasets with a single layer are supported"));
    }
  }

  public function getFeatureIterator($layerIndex = 0) {
    if (!$this->dataset) {
      throw new GdalModuleException(t("Currently no dataset is opened"));
    }

    $layer = $this->dataset->GetLayer($layerIndex);
    return new OgrFeatureIterator($layer, $this->geometryFieldName, $this->defaultSrid, $this->fidFieldName);
  }


  public function setGeometryFieldName($name) {
    $this->geometryFieldName = $name;
  }

  public function setFIDFieldName($name) {
    $this->fidFieldName = $name;
  }

  public function setDefaultSrid($defaultSrid) {
    $this->defaultSrid = $defaultSrid;
  }

  /**
   * check if a file is an archive which needs to be extracted before
   * it can be opened by ogr
   *
   * returns the mimetype if the files is packed, False if it is not packed
   */
  private function fileIsPacked($fileName) {
    $mimeType = file_get_mimetype($fileName);
    if (in_array($mimeType, $this->supportedPackFormats)) {
      return $mimeType;
    };
    return False;
  }

  /**
   * recursivliy delete a directory
   */
  private function recursiveDelete($directory) {
    if (empty($directory)) {
      return;
    }

    if (is_dir($directory)) {
      $files = new RecursiveIteratorIterator(
          new RecursiveDirectoryIterator($directory, RecursiveDirectoryIterator::SKIP_DOTS),
          RecursiveIteratorIterator::CHILD_FIRST
          );

      foreach ($files as $fileinfo) {
        $todo = ($fileinfo->isDir() ? 'drupal_rmdir' : 'drupal_unlink');
        $todo($fileinfo->getRealPath());
      }
      rmdir($directory);
    }
  }
}


// vim: ft=php tabstop=2 shiftwidth=2
