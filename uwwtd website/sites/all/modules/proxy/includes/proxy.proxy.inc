<?php

/**
 * @file
 * This file holds the actual functions to do the proxy request.
 *
 * @ingroup proxy
 */

/**
 * Proxy "private" method.
 *
 * See proxy(). for parameters.
 */
function _proxy($params = array(), $reset = FALSE) {
  $errors = array();
  $content = '';

  // Static cache for the same request caching.
  static $proxied = array();

  // Check parameters against defaults.  We use standard
  // names to make this process easier.
  $available = array('cache', 'whitelist_control', 'whitelist_domains', 'flood_control', 'flood_interval', 'flood_autoban');
  foreach ($available as $param) {
    $params[$param] = (!empty($params[$param])) ? $params[$param] : variable_get('proxy_' . $param, constant('PROXY_' . drupal_strtoupper($param) . '_DEFAULT'));
  }

  // Flood control
  if ($params['flood_control']) {
    // Ensure flood interval is above zero.
    $params['flood_interval'] = ((int) $params['flood_interval']) <= 0 ? PROXY_FLOOD_INTERVAL_DEFAULT : $params['flood_interval'];
    // Check flood
    if (flood_is_allowed('proxy', $params['flood_interval'])) {
      // Register
      flood_register_event('proxy');
    }
    else {
      // Check autoban
      if ($params['flood_autoban']) {
        // Deny host
        _proxy_deny_host(ip_address());
        // Redirect, or otherwise, page will load
        drupal_goto('<front>');
      }
      // Create error
      $errors[] = t('Flood threshold of %interval reached for host: %host.', array(
        '%interval' => $params['flood_interval'],
        '%host' => ip_address(),
      ));
      return $errors;
    }
  }

  // Whitelist
  if ($params['whitelist_control']) {
    // Parse out URL
    $parsed = parse_url($params['request_uri']);
    // Check domain, default if empty is do not use
    $domain = $parsed['host'];
    if (!empty($domain)) {
      // Check whitelist
      if (!empty($params['whitelist_domains'])) {
        // Check whitelist
        $approved = _proxy_match_domain($domain, $params['whitelist_domains']);
        if (!$approved) {
          $errors[] = t('Domain not in whitelist: %domain', array('%domain' => $domain));
        }
      }
      else {
        $errors[] = t('Whitelist empty, no need to check.');
      }
    }
    else {
      $errors[] = t('Could not determine domain to whitelist.');
    }

    // If any errors, return
    if (!empty($errors)) {
      return $errors;
    }
  }

  // Check caches
  if (!$reset) {
    // Check static cache
    if (!empty($proxied[$params['request_uri']])) {
      return $proxied[$params['request_uri']];
    }

    // Check Drupal cache.  Do not attempt to get cache if it is off
    if ($params['cache'] != PROXY_NO_CACHE) {
      $drupal_cache = cache_get($params['request_uri'], 'cache_proxy');
      if ($drupal_cache && REQUEST_TIME < $drupal_cache->expire && !empty($drupal_cache->data)) {
        return $drupal_cache->data;
      }
    }
  }

  // No cache, so do the actual proxy.
  $response = drupal_http_request($params['request_uri']);

  // Check response and error codes
  if (empty($response)) {
    $errors[] = t('Invalid response: Unknown.');
  }
  if (empty($response->data)) {
    $errors[] = t('Data not returned.  Response code: %code', array('%code' => $response->code));
  }
  // Handle specific codes
  switch ($response->code) {
    case 200:
    case 304:
      // OK
      break;

    case 301:
    case 302:
    case 307:
      // Permanent Redirect
      break;

    default:
      $errors[] = t('Invalid response: %code: %error', array(
        '%code' => $response->code,
        '%error' => $response->error,
      ));
      break;
  }

  // If no errors, process
  if (empty($errors)) {
    // Get processors
    $processors = proxy_processors();

    // Check processors
    if (!empty($processors)) {
      $params['processors'] = (isset($params['processors']) && is_array($params['processors'])) ? $params['processors'] : array();

      // First we check params and settings to determine
      // if enabled or not.  If calling proxy()
      // directly, then program can enable or disable
      // as needed, otherwise use settings defaults.
      foreach ($processors as $key => $plugin) {
        $params['processors'][$key]['enabled'] = isset($params['processors'][$key]['enabled']) ? $params['processors'][$key]['enabled'] : variable_get(PROXY_PROCESSOR_PREFIX . $key, FALSE);
        // TO DO, set options
        $params['processors'][$key]['options'] = array();
      }

      // Go through the processors from params
      foreach ($params['processors'] as $key => $param) {
        // Check if enabled
        if ($param['enabled']) {
          // Get class via ctools
          $class = ctools_plugin_get_class($processors[$key], 'processor');
          if (!empty($class)) {
            $processor = new $class($param['options']);
            if (is_object($processor)) {
              // Process!
              $processor->render($response, $params);
            }
          }
        }
      }
    }
  }

  // Cache content, check how long to cache.  Cache even if response is
  // not good, so that proxy is not abused.
  if (in_array($params['cache'], array(CACHE_TEMPORARY, CACHE_PERMANENT))) {
    cache_set($params['request_uri'], $response, 'cache_proxy', $params['cache']);
  }
  elseif ($params['cache'] == PROXY_NO_CACHE) {
    // Do not cache
  }
  else {
    // Assume that we have a time amount in seconds
    cache_set($params['request_uri'], $response, 'cache_proxy', REQUEST_TIME + $params['cache']);
  }
  // Set static cache
  $proxied[$params['request_uri']] = $response;

  // Return content or errors
  if (count($errors) > 0) {
    return $errors;
  }
  else {
    return $response;
  }
}

/**
 * Match domains
 *
 * Matches domain agaist whitelist domains.  Based
 * from drupal_match_path().
 *
 * @param $domain
 *   Domain to check
 * @param $whitelist
 *   List of domains separated by /r/n or /n
 * @return
 *   TRUE or FALSE if the domain is valid
 */
function _proxy_match_domain($domain = '', $whitelist = '') {
  static $regexps;

  // Let's remove any www. , as it just gets in the way
  $domain = str_replace('www.', '', $domain);
  $whitelist = str_replace('www.', '', $whitelist);

  // Default patterns
  if (!isset($regexps[$whitelist])) {
    $regexps[$whitelist] = '/^(' .
      preg_replace(
        array('/(\r\n?|\n)/', '/\\\\\*/'),
        array('|', '.*'),
        preg_quote($whitelist, '/')
      ) . ')$/';
  }
  return preg_match($regexps[$whitelist], $domain);
}

/**
 * Deny host
 *
 * Updates Drupal's access rules to deny a specific
 * host.
 *
 * @param $host
 *   IP of host to ban
 * @return
 *   TRUE or FALSE if operation was successful
 */
function _proxy_deny_host($host = '') {
  if (empty($host)) {
    return FALSE;
  }

  // Check if rul already exists
  $query = "SELECT aid FROM {access} WHERE mask = '%s' AND type = 'host' AND status = 0";
  $aid = db_query("SELECT aid FROM {access} WHERE mask = :mask AND type = :type AND status = :status", array(':mask' => $host, ':type' => 'host', ':status' => 0))->fetchField();
  if (empty($aid)) {
    $query = "INSERT INTO {access} (mask, type, status) VALUES ('%s', 'host', 0)";
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($query, $host) */
    return $id = db_insert('access')
      ->fields(array(
        'mask' => $host,
        'type' => 'host',
        'status' => 0,
      ))
      ->execute();
  }
}
