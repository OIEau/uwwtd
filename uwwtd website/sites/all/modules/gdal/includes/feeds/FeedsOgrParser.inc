<?php

/**
 * @file
 * Wrapper to integrate the OgrReader in the feeds module as a FeedsParser
 */


class FeedsOgrParser extends FeedsParser {


  /** list of fields in the parsed layer.
    * only available after ogrProcessLayer has been called
    */
  var $layerFields = array();

  /**
   * index of the layer to return.
   * supporting only one layer at this point
   */
  var $layerIdx = 0;

  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $fileName = drupal_realpath($fetcher_result->getFilePath());

    $reader = new OgrReader($fileName);
    $reader->setDefaultSrid(preg_replace("/^EPSG:/i", "", $this->config['srs_epsg']));
    $reader->setGeometryFieldName($this->config['geom_field_name']);
    $reader->setFIDFieldName($this->config['fid_field_name']);

    $iterator = $reader->getFeatureIterator($this->layerIdx);
    $this->layerFields = $iterator->getLayerFields();

    return new OgrFeedsParserResult($source, $iterator);
  }

  public function getMappingSources() {
    // parse has to be called before to return correct results
    $result = parent::getMappingSources() + $this->layerFields;
    return $result;
  }

  /**
   * Define default configuration.
   */
  public function configDefaults() {
    return array(
        'geom_field_name' => '_geometry_',
        'fid_field_name' => '_fid_',
        'srs_epsg' => 4326,
        );
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['geom_field_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name for the geometry field'),
        '#description' => t('The name for the geometry field. The geometry will be avialable under this name in the feed processor. The data from the field will be returned in WKT format.'),
        '#default_value' => $this->config['geom_field_name'],
        );
    $form['fid_field_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name for the FeatureID field'),
        '#description' => t('The name for the FeeatureID field. The geometry will be avialable under this name in the feed processor. This setting is only necessary if you need to access the value of the FeatureID.'),
        '#default_value' => $this->config['fid_field_name'],
        );
    $form['srs_epsg'] = array(
        '#type' => 'textfield',
        '#title' => t('Projection (EPSG code)'),
        '#description' => t('Projection for this coordinate system. This projection will be used if the vector dataset does not provide an projection. Example: EPSG:4326'),
        '#default_value' => $this->config['srs_epsg'],
        );

    $form['#validate'][] = 'feeds_ogr_parser_form_config_validate';

    return $form;
  }

}

/**
 * validation for the config form
 *
 * the feeds module currently does not appear to be using this function. Maybe
 * it will sometime call its validation functions
 */
function feeds_ogr_parser_form_config_validate($form, &$form_state) {
  if (!empty($form_state['values']['srs_epsg']) && !is_int($form_state['values']['srs_epsg'])) {
    form_set_error('srs_epsg', t('The EPSG code must be an integer number.'));
  }
}


// vim: ft=php tabstop=2 shiftwidth=2
