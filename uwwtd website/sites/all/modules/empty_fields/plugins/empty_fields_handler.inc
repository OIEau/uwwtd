<?php
/**
 * @file
 * Contains the base class for emptyFieldHandlers
 */

/**
 * Abstract base class defining a field_empty_handler.
 *
 * Defines a way to easily create empty field reaction plugins.
 *
 * @see field_empty_handler_text().
 */
abstract class EmptyFieldHandler {

  public $options;

  /**
   * Create a new EmptyFieldHandler.
   */
  public function __construct() {
    $this->options = $this->defaults();
  }

  /**
   * Default options that map to FAPI elements in EmptyFieldHandler::form().
   */
  public function defaults() {
    return array();
  }

  /**
   * Provide FAPI elements to configure the empty field rendering.
   */
  public function form($context) {
    return array();
  }

  /**
   * Provide the summary text to display on the field display settings page.
   */
  abstract public function summaryText();

  /**
   * Provide a reaction. Set the content of the field.
   */
  abstract public function react($context);

  /**
   * Provide a method to alter the output of the plugin.
   */
  public function alterOutput(&$output, $context) {

  }

}
