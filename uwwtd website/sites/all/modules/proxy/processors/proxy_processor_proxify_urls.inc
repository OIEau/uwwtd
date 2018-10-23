<?php

/**
 * @file
 * Proxy Processor Plugin: proxy_processor_proxify_urls
 */

/**
 * Implementation of Proxy Processor Plugin
 */
class proxy_processor_proxify_urls {
  var $options;

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
   * options of the processor.
   *
   * @return
   *   Valid Drupal form array.
   */
  function options_form() {
    return array();
  }

  /**
   * Create base URL from request
   *
   * @param $request_uri
   *   Requested URI
   * @return
   *   Base URL in form like http://drupal.org
   */
  function get_base_request($request_uri = '') {
    // Determine base host
    $url_parts = parse_url($request_uri);
    // Put back topgether
    return (isset($url_parts['scheme']) ? $url_parts['scheme'] . '://' : '') .
      (isset($url_parts['user']) ? $url_parts['user'] . ':' : '') .
      (isset($url_parts['pass']) ? $url_parts['pass'] . '@' : '') .
      (isset($url_parts['host']) ? $url_parts['host'] : '') .
      (isset($url_parts['port']) ? ':' . $url_parts['port'] : '');
  }

  /**
   * Main Rendering Function
   *
   * This processing basically looks in the content for matches
   * of specific attributes with variable delimters (", '), then replaces
   * the URL's appropriately.
   *
   * NOTE: These operations should be done with the DOM object in PHP,
   * but it requires a specific extesion php5-dom
   *
   * @param $response
   *   Response object to process.  Main content
   *   is in $response->data
   * @param $params
   *   Array of parameters, as sent to proxy().
   */
  function render(&$response, $params) {
    $proxy_base = base_path() . PROXY_ROUTER_PATH . '?' . PROXY_ROUTER_QUERY_VAR . '=';
    $request_base = $this->get_base_request($params['request_uri']);
    $redirect_base = $proxy_base . $request_base;
    $content = $response->data;

    // Set up arrays for matching
    // Relative paths (needs work)
    $matches[] = '#(href|src|url)=["|\'](^([\/|http|https|ftp|ftps])[^"\']*)(["|\'])#';
    $replaces[] = '$1="' . $redirect_base . '/$2$3';
    // Absolute paths
    $matches[] = '#(href|src|url)=["|\'](\/[^"\']*)(["|\'])#';
    $replaces[] = '$1="' . $redirect_base . '$2$3';
    // Full URLS
    $matches[] = '#(href|src|url)=["|\']([http|https|ftp|ftps][^"\']*)(["|\'])#';
    $replaces[] = '$1="' . $proxy_base . '$2$3';

    // Change content
    $response->data = preg_replace($matches, $replaces, $response->data);
  }
}
