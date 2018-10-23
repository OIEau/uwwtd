<?php

/**
* Implements hook_views_default_views().
*/
function uwwtd_views_views_default_views() {
    static $views;
    
    if (isset($views)) {
        return $views;
    }
    
    $files = file_scan_directory(drupal_get_path('module', 'uwwtd_views') . '/inc', '/\.inc$/');

    foreach ($files as $filepath => $file) {
        include $filepath;
        if (isset($view)) {
            $views[$view->name] = $view;
        }        
    }

    return $views;
}    
