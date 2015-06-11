<?php
/**
 * @file
 * Wrapper to integrate the OgrFeatureIterator in the feeds module as a FeedsParserResult
 */


class OgrFeedsParserResult extends FeedsParserResult {

  /**
   * the OgrFeatureIterator this Feeds-style iterator is based upon
   */
  private $iterator = Null;

  /**
   * the FeedsState object used to track progress
   */
  private $state = Null;

  /**
   * the position to start interating from in the current request
   */
  private $start_pos = Null;

  /**
   * number of items to iterate over within a single request.
   * This setting is pulled from the importer configuration
   */
  private $limit = Null;

  /**
   * @param $ogrLayer OGRLayer
   */
  public function __construct(FeedsSource $source, OgrFeatureIterator $iterator) {
    parent::__construct(array());
    $this->iterator = $iterator;
    
    // number of features to import in one pass
    $this->limit = $source->importer->getLimit();

    // report progress
    $this->state = $source->state(FEEDS_PARSE);
    $this->state->total = $this->iterator->getFeatureCount();
    // the pointer may be already set if drupal dug out s serialized object for $state
    // from an earlier request. In this case wh want to keep its value
    $this->start_pos = $this->state->pointer ? $this->state->pointer : 0;

    // seek to the position in the layer the import has reached its limit in the last request
    if ($this->start_pos) {
        $this->iterator->seek($this->start_pos);
    }
  }

  public function shiftItem() {

    // add 1 to the current position as the position will only be incremented when calling next()
    // a few lines down
    if ((($this->limit + $this->start_pos) <= ($this->iterator->getPosition() + 1))) {
        return; // reached limit of features for iteration in this request
    }

    $this->current_item = $this->iterator->next();

    // report progress
    $this->state->pointer = $this->iterator->getPosition();
    $this->state->progress($this->state->total, $this->state->pointer);

    return $this->current_item;
  }
}


// vim: ft=php tabstop=2 shiftwidth=2
