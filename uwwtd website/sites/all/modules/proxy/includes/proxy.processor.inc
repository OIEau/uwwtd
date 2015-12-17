<?php

/**
 * @file
 * This file holds the base functions and definitions for proxy
 * processors.
 *
 * @ingroup proxy
 */

/**
 * Get Processor Options
 *
 * Gets the options form array for all the processors.
 *
 * @param $defaults
 *   Defaults, use $defaults['processors'] for processor
 *   defaults.
 * @return
 *   Array of form items.
 */
function _proxy_get_processor_options() {
  $form = array();
  $processors = proxy_processors();

  if (!empty($processors)) {
    foreach ($processors as $key => $plugin) {
      $class = ctools_plugin_get_class($plugin, 'processor');
      if (!empty($class)) {
        // Build form items.  Add prefix as these will be stored in
        // the variables table
        $form[PROXY_PROCESSOR_PREFIX . $key] = array(
          '#type' => 'checkbox',
          '#title' => $plugin['title'],
          '#description' => $plugin['description'],
          '#default_value' => variable_get(PROXY_PROCESSOR_PREFIX . $key, FALSE),
        );

        // Get other options
        $processor = new $class($options - array());
        $form[PROXY_PROCESSOR_PREFIX . $key . '_options'] = $processor->options_form();
      }
    }
  }

  return $form;
}


/**
 * Base Class for Proxy Processors
 */
class proxy_processor {
  var $options;

  /**
   * Constructor
   *
   * Constructor of the class.  You should not override
   * this.
   */
  function __construct($options = array(), $map = array()) {
    $this->options = $options + $this->options_init();
  }

  /**
   * Initiate any options
   *
   * Define any default options that can be stored
   * in the options property of the class.
   *
   * @return
   *   Array of options
   */
  function options_init() {
    return array();
  }

  /**
   * Options form
   *
   * Defines any form items needed for
   * options of the processor.  Note that these
   * values are saved in the variables table
   * which means that you should use your module
   * name to prefix the id of the form item, and
   * the default should come from the variable name.
   *
   * @return
   *   Valid Drupal form array.
   */
  function options_form() {
    return array();
  }

  /**
   * Main Rendering Function
   *
   * Do the processing here.
   *
   * @param $response
   *   Response object to process.  Main content
   *   is in $response->data
   * @param $params
   *   Array of parameters, as sent to proxy().
   */
  function render(&$content, $params) {
    // Process content here
  }
}
