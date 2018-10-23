<?php
/**
 * @file
 * wms_getfeatureinfo_demo.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wms_getfeatureinfo_demo_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'canada';
  $context->description = 'Canada Getfeatureinfo Demo';
  $context->tag = '';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'canada' => 'canada',
        'canada:page_1' => 'canada:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wms-getfeatureinforesults' => array(
          'module' => 'wms',
          'delta' => 'getfeatureinforesults',
          'region' => 'content',
          'weight' => '8',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Canada Getfeatureinfo Demo');
  $export['canada'] = $context;

  return $export;
}
