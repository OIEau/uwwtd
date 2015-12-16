<?php

/**
 * @file
 * This file holds the functions for proxy router.
 *
 * @ingroup proxy
 */

/**
 * Menu callback; route the proxy and return content
 *
 * @param $request_uri
 *   The requested URI to proxy.
 * @return
 *   Processed content
 */
function proxy_router() {
  $reset = FALSE;

  // Define parameters
  $params = array(
    'request_uri' => !empty($_GET[PROXY_ROUTER_QUERY_VAR]) ? urldecode($_GET[PROXY_ROUTER_QUERY_VAR]) : '',
  );

  // Check for reset
  if (isset($_GET['reset']) && $_GET['reset'] && user_access('reset cache via router')) {
    $reset = TRUE;
  }

  // Proxy it up!
  $response = proxy($params, $reset);
  if (!is_array($response)) {
    // Check for data
    if (is_object($response) && !empty($response->data)) {
      // Set headers
      if (is_array($response->headers)) {
        foreach ($response->headers as $header => $value) {
          drupal_add_http_header($header, $value);
        }
      }
      else {
        // Should we guess at headers? (TO DO)
      }

      // Print actual data
      print $response->data;
      exit;
    }
    else {
      watchdog('proxy', 'Data not provided to router.', array(), WATCHDOG_WARNING);
    }
  }
  else {
    // Watchdog
    foreach ($response as $message) {
      watchdog('proxy', $message, array(), WATCHDOG_WARNING);
    }
  }

  // Default is 404, which helps masks the fact that the proxy exists.
  // Also, reset a couple variables so that Drupal does not try to
  // redirect here
  $_GET['destination'] = '';
  $_GET['q'] = '';
  drupal_not_found();
}
