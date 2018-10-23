<?php

/**
 * Implements hook_drush_command().
 */
function uwwtd_drush_command() {
  $items['create_content_type_config'] = array(
    'description' => 'Create content type config file',
    'callback' => 'uwwtd_create_content_type_config',    
  );
  return $items;
}

/**
 *
 */
function uwwtd_create_content_type_config() {

    $r = fopen(drupal_get_path('module', 'uwwtd') . '/script/content_type/bundle.config.inc', 'wb');
    fputs($r, "<?"."php\n//automatically generate from dev platform by drush create_content_type_config");
    
    require_once(drupal_get_path('module', 'uwwtd') . '/script/content_type/list_bundle.inc');

    $list_contenttype = array();
    foreach ($list_content_type as $contenttype_name) {
        $contenttypeinfo = node_type_load($contenttype_name);

        $list_contenttype[$contenttype_name]['node_type_load'] = (array)$contenttypeinfo;    
    }
    
    $list_field = array();
    foreach ($list_content_type as $contenttype_name) {
        $info_instance = field_info_instances('node', $contenttype_name);
        $treated = array();
        foreach($info_instance as $fieldname => $field_info_instance) {
            $list_contenttype[$contenttype_name]['field_info_instance'][] = $field_info_instance;
            
            if (isset($treated[$fieldname])) {
                continue;
            }
            $treated[$fieldname] = array();
            $infofield = field_info_field($fieldname);

            $infofield['bundles'] = array();        
            $list_field[$fieldname] = $infofield;
        }      
    }    
    
    fputs($r, "\n\$list_field = " . var_export($list_field, true) . ";");
    fputs($r, "\n\$list_contenttype = " . var_export($list_contenttype, true) . ";");
    
    /////////////
    fputs($r, "\n");
    fclose($r);
  
}
