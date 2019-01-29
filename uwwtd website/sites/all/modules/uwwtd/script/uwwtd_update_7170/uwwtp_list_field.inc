<?php 

$fields_instance = array (
  'field_uwwtssperf' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '9',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment performance: TSS',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '70',
    ),
    'id' => '139',
    'field_id' => '122',
    'field_name' => 'field_uwwtssperf',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_linked_receiving_areas' => 
  array (
    'label' => 'Linked receiving area(s)',
    'widget' => 
    array (
      'weight' => '83',
      'type' => 'node_reference_autocomplete',
      'module' => 'node_reference',
      'active' => 1,
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'size' => '60',
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '26',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'node_reference',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '190',
    'field_id' => '90',
    'field_name' => 'field_linked_receiving_areas',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwmicrofiltration' => 
  array (
    'default_value' => 
    array (
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '41',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Micro filtration',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
        'display_label' => 0,
      ),
      'type' => 'options_buttons',
      'weight' => '28',
    ),
    'id' => '137',
    'field_id' => '120',
    'field_name' => 'field_uwwmicrofiltration',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwbeginlife' => 
  array (
    'label' => 'uwwbeginlife',
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 115,
      'module' => 'text',
    ),
    'settings' => 
    array (
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'date_default',
        'weight' => '89',
        'settings' => 
        array (
          'format_type' => 'long',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'fromto' => 'both',
          'show_remaining_days' => false,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'date',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => false,
    'description' => '',
    'id' => '447',
    'field_id' => '355',
    'field_name' => 'field_uwwbeginlife',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_uwww_required_rcab' => 
  array (
    'label' => 'Treatment required rcaB',
    'widget' => 
    array (
      'weight' => '113',
      'type' => 'options_buttons',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'list_default',
        'weight' => '87',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => 
    array (
      0 => 
      array (
        'value' => '0',
      ),
    ),
    'id' => '118',
    'field_id' => '105',
    'field_name' => 'field_uwww_required_rcab',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwnincoming' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '34',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Incoming load Ntot (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '34',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '187',
    'field_id' => '163',
    'field_name' => 'field_uwwnincoming',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_sourcefile' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'file_default',
        'weight' => '14',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'file',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Source of data',
    'required' => 0,
    'settings' => 
    array (
      'description_field' => 0,
      'file_directory' => 'data_sources',
      'file_extensions' => 'txt csv xml',
      'max_filesize' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'file',
      'settings' => 
      array (
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => '64',
    ),
    'id' => '169',
    'field_id' => '95',
    'field_name' => 'field_sourcefile',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_inspireidversionid' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '17',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'inspireIdVersionId',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '5',
    ),
    'id' => '133',
    'field_id' => '54',
    'field_name' => 'field_inspireidversionid',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_physicalcapacityoutput' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '28',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'physicalCapacityOutput',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '17',
    ),
    'id' => '221',
    'field_id' => '190',
    'field_name' => 'field_physicalcapacityoutput',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_sensitivearealink' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'node_reference_default',
        'weight' => '58',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'node_reference',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'sensitiveAreaLink',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'node_reference',
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'node_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => '61',
    ),
    'id' => '197',
    'field_id' => '53',
    'field_name' => 'field_sensitivearealink',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_inspireidlocalid' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '8',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Identifier of UWWTP/ collecting system without treatment',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '2',
    ),
    'id' => '193',
    'field_id' => '17',
    'field_name' => 'field_inspireidlocalid',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwaccidents' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '60',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Accidents',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_buttons',
      'weight' => '78',
    ),
    'id' => '122',
    'field_id' => '108',
    'field_name' => 'field_uwwaccidents',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwww_required_rcaap' => 
  array (
    'label' => 'Treatment required rcaAPhos',
    'widget' => 
    array (
      'weight' => '112',
      'type' => 'options_buttons',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'list_default',
        'weight' => '86',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => 
    array (
      0 => 
      array (
        'value' => '0',
      ),
    ),
    'id' => '219',
    'field_id' => '188',
    'field_name' => 'field_uwww_required_rcaap',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwendlife' => 
  array (
    'label' => 'uwwendlife',
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 116,
      'module' => 'text',
    ),
    'settings' => 
    array (
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'date_default',
        'weight' => '90',
        'settings' => 
        array (
          'format_type' => 'long',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'fromto' => 'both',
          'show_remaining_days' => false,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'date',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => false,
    'description' => '',
    'id' => '448',
    'field_id' => '356',
    'field_name' => 'field_uwwendlife',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_uwwbodincomingestimated' => 
  array (
    'label' => 'Incoming load BOD-tot Estimated',
    'widget' => 
    array (
      'weight' => '103',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '78',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '191',
    'field_id' => '166',
    'field_name' => 'field_uwwbodincomingestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwperctruck' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '26',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Rate of entering load transported to this UWWTP by trucks (%)',
    'required' => 0,
    'settings' => 
    array (
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 0,
      'module' => 'number',
      'settings' => 
      array (
      ),
      'type' => 'number',
      'weight' => '15',
    ),
    'id' => '204',
    'field_id' => '176',
    'field_name' => 'field_uwwperctruck',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_linked_discharge_points' => 
  array (
    'label' => 'Linked discharge point(s)',
    'widget' => 
    array (
      'weight' => '82',
      'type' => 'node_reference_autocomplete',
      'module' => 'node_reference',
      'active' => 1,
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'size' => '60',
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '29',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'node_reference',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '177',
    'field_id' => '27',
    'field_name' => 'field_linked_discharge_points',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwperformance_met' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '18',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Performance met',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '76',
    ),
    'id' => '135',
    'field_id' => '119',
    'field_name' => 'field_uwwperformance_met',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcoddischargeestimated' => 
  array (
    'label' => 'Discharged load COD-tot Estimated',
    'widget' => 
    array (
      'weight' => '94',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '70',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '185',
    'field_id' => '161',
    'field_name' => 'field_uwwcoddischargeestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwboddischarge' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '36',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Discharged load BOD (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '36',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '180',
    'field_id' => '156',
    'field_name' => 'field_uwwboddischarge',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwntotperf' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '10',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment performance: Total nitrogen',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '71',
    ),
    'id' => '126',
    'field_id' => '111',
    'field_name' => 'field_uwwntotperf',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwdateexported' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '45',
        'settings' => 
        array (
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => false,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'date',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Time of creation of correct export data set',
    'required' => 0,
    'settings' => 
    array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'date',
      'settings' => 
      array (
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => 
        array (
        ),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_text',
      'weight' => '46',
    ),
    'id' => '214',
    'field_id' => '185',
    'field_name' => 'field_uwwdateexported',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpincomingcalculated' => 
  array (
    'label' => 'Incoming load P-tot Calculated',
    'widget' => 
    array (
      'weight' => '90',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '66',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '142',
    'field_id' => '125',
    'field_name' => 'field_uwwpincomingcalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpremoval' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '36',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'P removal',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_buttons',
      'weight' => '23',
    ),
    'id' => '155',
    'field_id' => '136',
    'field_name' => 'field_uwwpremoval',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcollectingsystem' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '25',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Identification whether it is the existing UWWTP (in operation) or a collecting system without UWWTPs ',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '10',
    ),
    'id' => '160',
    'field_id' => '141',
    'field_name' => 'field_uwwcollectingsystem',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwotherperf' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '12',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment performance: Other',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '73',
      'type' => 'options_select',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'id' => '175',
    'field_id' => '152',
    'field_name' => 'field_uwwotherperf',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwndischarge' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '38',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Discharged load Ntot (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '38',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '181',
    'field_id' => '157',
    'field_name' => 'field_uwwndischarge',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcodperf' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '8',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment performance: COD',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '69',
    ),
    'id' => '205',
    'field_id' => '177',
    'field_name' => 'field_uwwcodperf',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwsandfiltration' => 
  array (
    'default_value' => 
    array (
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '39',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Sand filtration',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
        'display_label' => 0,
      ),
      'type' => 'options_buttons',
      'weight' => '27',
    ),
    'id' => '172',
    'field_id' => '149',
    'field_name' => 'field_uwwsandfiltration',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwnincomingestimated' => 
  array (
    'label' => 'Incoming load N-tot Estimated',
    'widget' => 
    array (
      'weight' => '105',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '80',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '189',
    'field_id' => '165',
    'field_name' => 'field_uwwnincomingestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_physicalcapacityinput' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '27',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 0,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Organic design capacity (p.e.)',
    'required' => 0,
    'settings' => 
    array (
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 0,
      'module' => 'number',
      'settings' => 
      array (
      ),
      'type' => 'number',
      'weight' => '16',
    ),
    'id' => '134',
    'field_id' => '118',
    'field_name' => 'field_physicalcapacityinput',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwbod5perf' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '7',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment performance: BOD5',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '68',
    ),
    'id' => '213',
    'field_id' => '184',
    'field_name' => 'field_uwwbod5perf',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpdischargeestimated' => 
  array (
    'label' => 'Discharged load P-tot Estimated',
    'widget' => 
    array (
      'weight' => '97',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '73',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '208',
    'field_id' => '179',
    'field_name' => 'field_uwwpdischargeestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpincomingestimated' => 
  array (
    'label' => 'Incoming load P-tot Estimated',
    'widget' => 
    array (
      'weight' => '106',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '81',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '201',
    'field_id' => '173',
    'field_name' => 'field_uwwpincomingestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwloadenteringuwwtp' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '4',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 0,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Load entering UWWTP (p.e.)',
    'required' => 0,
    'settings' => 
    array (
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 0,
      'module' => 'number',
      'settings' => 
      array (
      ),
      'type' => 'number',
      'weight' => '14',
    ),
    'id' => '212',
    'field_id' => '183',
    'field_name' => 'field_uwwloadenteringuwwtp',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_functiondescription' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '50',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'functionDescription',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '51',
    ),
    'id' => '220',
    'field_id' => '189',
    'field_name' => 'field_functiondescription',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_position_geo' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'hidden',
        'type' => 'geofield_openlayers',
        'weight' => '1',
        'settings' => 
        array (
          'map_preset' => 'uwwtd_geofield_formatter_map',
          'data' => 'full',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'geofield',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Position',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'geofield',
      'settings' => 
      array (
      ),
      'type' => 'geofield_wkt',
      'weight' => '59',
    ),
    'id' => '215',
    'field_id' => '85',
    'field_name' => 'field_position_geo',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_thematicid' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '46',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'thematicId',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '47',
    ),
    'id' => '207',
    'field_id' => '48',
    'field_name' => 'field_thematicid',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_functioninput' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '48',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'functionInput',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '49',
    ),
    'id' => '117',
    'field_id' => '104',
    'field_name' => 'field_functioninput',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwtdaggid' => 
  array (
    'label' => 'ID of agglomeration',
    'widget' => 
    array (
      'weight' => '86',
      'type' => 'text_textfield',
      'module' => 'text',
      'active' => 1,
      'settings' => 
      array (
        'size' => '60',
      ),
    ),
    'settings' => 
    array (
      'text_processing' => '0',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '62',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '121',
    'field_id' => '107',
    'field_name' => 'field_uwwtdaggid',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_endlifespanversion' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '56',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'endLifespanVersion',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '57',
    ),
    'id' => '120',
    'field_id' => '98',
    'field_name' => 'field_endlifespanversion',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwtreatmentrequired' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '16',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment required',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '79',
      'type' => 'options_select',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'id' => '116',
    'field_id' => '103',
    'field_name' => 'field_uwwtreatmentrequired',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_physicalcapacitytime' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '29',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'physicalCapacityTime',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '18',
    ),
    'id' => '151',
    'field_id' => '133',
    'field_name' => 'field_physicalcapacitytime',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_anneedata' => 
  array (
    'default_value' => 
    array (
      0 => 
      array (
        'value' => 2014,
      ),
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '12',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Year of data',
    'required' => 1,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '62',
    ),
    'id' => '192',
    'field_id' => '86',
    'field_name' => 'field_anneedata',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwnathyperlink' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '44',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'UWWTP national Internet hyperlink',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '45',
    ),
    'id' => '130',
    'field_id' => '115',
    'field_name' => 'field_uwwnathyperlink',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_physicalcapacitydescriptio' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '30',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'physicalCapacityDescription',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '19',
    ),
    'id' => '147',
    'field_id' => '130',
    'field_name' => 'field_physicalcapacitydescriptio',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcompliance' => 
  array (
    'default_value' => 
    array (
      0 => 
      array (
        'value' => 'NI',
      ),
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '14',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Compliance',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '67',
    ),
    'id' => '125',
    'field_id' => '110',
    'field_name' => 'field_uwwcompliance',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_facilitydescription' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '53',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'facilityDescription',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '54',
    ),
    'id' => '158',
    'field_id' => '139',
    'field_name' => 'field_facilitydescription',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_validfrom' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '57',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'validFrom',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '58',
    ),
    'id' => '196',
    'field_id' => '169',
    'field_name' => 'field_validfrom',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_article17_uwwtp' => 
  array (
    'label' => 'Article 17 Uwwtp',
    'widget' => 
    array (
      'type' => 'node_reference_autocomplete',
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'size' => 60,
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
      'weight' => '110',
      'module' => 'node_reference',
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '84',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'node_reference',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => false,
    'description' => '',
    'default_value' => NULL,
    'id' => '211',
    'field_id' => '182',
    'field_name' => 'field_article17_uwwtp',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpinhabitantsconnected' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '43',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 0,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Number of inhabitants connected to particular UWWTP/collecting system',
    'required' => 0,
    'settings' => 
    array (
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 0,
      'module' => 'number',
      'settings' => 
      array (
      ),
      'type' => 'number',
      'weight' => '44',
    ),
    'id' => '124',
    'field_id' => '109',
    'field_name' => 'field_uwwpinhabitantsconnected',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcodincoming' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '33',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Incoming load COD (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '33',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '223',
    'field_id' => '192',
    'field_name' => 'field_uwwcodincoming',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwspecification' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '15',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Please specify other type of more stringent treatment',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '30',
    ),
    'id' => '170',
    'field_id' => '147',
    'field_name' => 'field_uwwspecification',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_siteid' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '11',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Site unique ID',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '63',
    ),
    'id' => '136',
    'field_id' => '30',
    'field_name' => 'field_siteid',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwnremoval' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '35',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'N removal',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_buttons',
      'weight' => '22',
    ),
    'id' => '138',
    'field_id' => '121',
    'field_name' => 'field_uwwnremoval',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_functionactivity' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '47',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'functionActivity',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '48',
    ),
    'id' => '154',
    'field_id' => '135',
    'field_name' => 'field_functionactivity',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwhistorie' => 
  array (
    'label' => 'Explanation for closing of the UWWTP/What happened with the wastewater since last reporting exercise',
    'widget' => 
    array (
      'weight' => '99',
      'type' => 'text_textfield',
      'module' => 'text',
      'active' => 1,
      'settings' => 
      array (
        'size' => '60',
      ),
    ),
    'settings' => 
    array (
      'text_processing' => '0',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '74',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '173',
    'field_id' => '150',
    'field_name' => 'field_uwwhistorie',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwmethodwastewatertreated' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '41',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Method used to determine the volume of waste water treated',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '42',
    ),
    'id' => '195',
    'field_id' => '168',
    'field_name' => 'field_uwwmethodwastewatertreated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwtreatmenttype' => 
  array (
    'default_value' => 
    array (
      0 => 
      array (
        'value' => 'NI',
      ),
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '15',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment type in place',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '74',
      'type' => 'options_select',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'id' => '156',
    'field_id' => '137',
    'field_name' => 'field_uwwtreatmenttype',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwinformation' => 
  array (
    'label' => 'In cause of failure: Further information on cause of failure',
    'widget' => 
    array (
      'weight' => '102',
      'type' => 'text_textfield',
      'module' => 'text',
      'active' => 1,
      'settings' => 
      array (
        'size' => '60',
      ),
    ),
    'settings' => 
    array (
      'text_processing' => '0',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '77',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '143',
    'field_id' => '126',
    'field_name' => 'field_uwwinformation',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_compliance_explication' => 
  array (
    'label' => 'Compliance explication',
    'widget' => 
    array (
      'weight' => '109',
      'type' => 'text_textarea',
      'module' => 'text',
      'active' => 1,
      'settings' => 
      array (
        'rows' => '5',
      ),
    ),
    'settings' => 
    array (
      'text_processing' => '0',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '83',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '123',
    'field_id' => '44',
    'field_name' => 'field_compliance_explication',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_functionoutput' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '49',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'functionOutput',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '50',
    ),
    'id' => '144',
    'field_id' => '127',
    'field_name' => 'field_functionoutput',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwe_prtrcode' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '21',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'ID of E-PRTR facility',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '6',
    ),
    'id' => '128',
    'field_id' => '113',
    'field_name' => 'field_uwwe_prtrcode',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwuv' => 
  array (
    'default_value' => 
    array (
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '37',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'UV',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
        'display_label' => 0,
      ),
      'type' => 'options_buttons',
      'weight' => '24',
    ),
    'id' => '129',
    'field_id' => '114',
    'field_name' => 'field_uwwuv',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_regionnuts' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '10',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Region (NUTS)',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '75',
    ),
    'id' => '161',
    'field_id' => '100',
    'field_name' => 'field_regionnuts',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_beginlifespanversion' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '55',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'beginLifespanVersion',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '56',
    ),
    'id' => '168',
    'field_id' => '72',
    'field_name' => 'field_beginlifespanversion',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_status' => 
  array (
    'default_value' => 
    array (
      0 => 
      array (
        'value' => 0,
      ),
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '9',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Status of the waste water treatment plant',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_buttons',
      'weight' => '4',
    ),
    'id' => '150',
    'field_id' => '61',
    'field_name' => 'field_status',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwbaddesign' => 
  array (
    'label' => 'In cause of failure: Bad design or dimensioning',
    'widget' => 
    array (
      'weight' => '101',
      'type' => 'options_buttons',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'list_default',
        'weight' => '76',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '178',
    'field_id' => '154',
    'field_name' => 'field_uwwbaddesign',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpdischarge' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '39',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Discharged load Ptot (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '39',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '182',
    'field_id' => '158',
    'field_name' => 'field_uwwpdischarge',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwsecondarytreatment' => 
  array (
    'default_value' => 
    array (
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '34',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Secondary treatment',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
        'display_label' => 0,
      ),
      'type' => 'options_buttons',
      'weight' => '21',
    ),
    'id' => '217',
    'field_id' => '187',
    'field_name' => 'field_uwwsecondarytreatment',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_permission' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '54',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'permission',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '55',
    ),
    'id' => '166',
    'field_id' => '145',
    'field_name' => 'field_permission',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwnincomingcalculated' => 
  array (
    'label' => 'Incoming load N-tot Calculated',
    'widget' => 
    array (
      'weight' => '89',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '65',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '132',
    'field_id' => '117',
    'field_name' => 'field_uwwnincomingcalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcodincomingestimated' => 
  array (
    'label' => 'Incoming load COD-tot Estimated',
    'widget' => 
    array (
      'weight' => '104',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '79',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '203',
    'field_id' => '175',
    'field_name' => 'field_uwwcodincomingestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwsuccessor' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '22',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'ID of the successor (historical management)',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '7',
    ),
    'id' => '222',
    'field_id' => '191',
    'field_name' => 'field_uwwsuccessor',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwprimarytreatment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '33',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Primary treatment',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_buttons',
      'weight' => '20',
    ),
    'id' => '127',
    'field_id' => '112',
    'field_name' => 'field_uwwprimarytreatment',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwsludgetreatment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '24',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'type of treatment of sludge',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '9',
    ),
    'id' => '159',
    'field_id' => '140',
    'field_name' => 'field_uwwsludgetreatment',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwboddischargecalculated' => 
  array (
    'label' => 'Discharged load BOD-tot Calculated',
    'widget' => 
    array (
      'weight' => '91',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '67',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '209',
    'field_id' => '180',
    'field_name' => 'field_uwwboddischargecalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwbodincoming' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '32',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Incoming load BOD (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '32',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '176',
    'field_id' => '153',
    'field_name' => 'field_uwwbodincoming',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwwastewatertreated' => 
  array (
    'default_value' => NULL,
    'description' => 'Volume of waste water treated and partially treated (m3/y) in the UWWTP',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '6',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 0,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Waste water treated (m3/y)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '41',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '198',
    'field_id' => '170',
    'field_name' => 'field_uwwwastewatertreated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_sensitiveareatype' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '59',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'sensitiveAreaType',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '60',
    ),
    'id' => '218',
    'field_id' => '32',
    'field_name' => 'field_sensitiveareatype',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_validto' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '13',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Date of closing',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '11',
    ),
    'id' => '152',
    'field_id' => '134',
    'field_name' => 'field_validto',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwother' => 
  array (
    'label' => 'Other type of more stringent ',
    'widget' => 
    array (
      'weight' => '100',
      'type' => 'options_buttons',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'list_default',
        'weight' => '75',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '179',
    'field_id' => '155',
    'field_name' => 'field_uwwother',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwloadcalculation' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '40',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Method used to determine the load ',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '40',
    ),
    'id' => '131',
    'field_id' => '116',
    'field_name' => 'field_uwwloadcalculation',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwothertreat' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '42',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Other treatment',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_buttons',
      'weight' => '77',
    ),
    'id' => '146',
    'field_id' => '129',
    'field_name' => 'field_uwwothertreat',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwtreatment_met' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '17',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment met',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '80',
    ),
    'id' => '119',
    'field_id' => '106',
    'field_name' => 'field_uwwtreatment_met',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcoddischarge' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '37',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Discharged load COD (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '37',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '141',
    'field_id' => '124',
    'field_name' => 'field_uwwcoddischarge',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_repcode' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'hidden',
        'weight' => '91',
        'settings' => 
        array (
        ),
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Report ID',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '1',
    ),
    'id' => '153',
    'field_id' => '42',
    'field_name' => 'field_repcode',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwchlorination' => 
  array (
    'default_value' => 
    array (
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '40',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Chlorination',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
        'display_label' => 0,
      ),
      'type' => 'options_buttons',
      'weight' => '25',
    ),
    'id' => '199',
    'field_id' => '171',
    'field_name' => 'field_uwwchlorination',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwaggliste' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '25',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'node_reference',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Linked agglomeration(s)',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'node_reference',
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'node_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => '65',
    ),
    'id' => '171',
    'field_id' => '148',
    'field_name' => 'field_uwwaggliste',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_type' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '51',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'type',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '52',
    ),
    'id' => '163',
    'field_id' => '143',
    'field_name' => 'field_type',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwbadperfdesign' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '31',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'In cause of failure: Bad operating/Bad design or dimensioning',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '31',
    ),
    'id' => '164',
    'field_id' => '144',
    'field_name' => 'field_uwwbadperfdesign',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_physicalcapacityactivity' => 
  array (
    'label' => 'Physical Capacity (p.e.)',
    'widget' => 
    array (
      'weight' => '85',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_integer',
        'weight' => '5',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 0,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '149',
    'field_id' => '132',
    'field_name' => 'field_physicalcapacityactivity',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpdischargecalculated' => 
  array (
    'label' => 'Discharged load P-tot Calculated',
    'widget' => 
    array (
      'weight' => '96',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '72',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '188',
    'field_id' => '164',
    'field_name' => 'field_uwwpdischargecalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwlatitude' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '18',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Latitude',
    'required' => 0,
    'settings' => 
    array (
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 0,
      'module' => 'number',
      'settings' => 
      array (
      ),
      'type' => 'number',
      'weight' => '12',
    ),
    'id' => '202',
    'field_id' => '174',
    'field_name' => 'field_uwwlatitude',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwdcpliste' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '13',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'node_reference',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Linked discharge point(s)',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'node_reference',
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'node_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => '66',
    ),
    'id' => '157',
    'field_id' => '138',
    'field_name' => 'field_uwwdcpliste',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwlongitude' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '19',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Longitude',
    'required' => 0,
    'settings' => 
    array (
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 0,
      'module' => 'number',
      'settings' => 
      array (
      ),
      'type' => 'number',
      'weight' => '13',
    ),
    'id' => '200',
    'field_id' => '172',
    'field_name' => 'field_uwwlongitude',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_servicehours' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '52',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'serviceHours',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '53',
    ),
    'id' => '167',
    'field_id' => '146',
    'field_name' => 'field_servicehours',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwndischargecalculated' => 
  array (
    'label' => 'Discharged load N-tot Calculated',
    'widget' => 
    array (
      'weight' => '107',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '82',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '162',
    'field_id' => '142',
    'field_name' => 'field_uwwndischargecalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwptotperf' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '11',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Treatment performance: Total phosphorus',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
      ),
      'type' => 'options_select',
      'weight' => '72',
    ),
    'id' => '140',
    'field_id' => '123',
    'field_name' => 'field_uwwptotperf',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcoddischargecalculated' => 
  array (
    'label' => 'Discharged load COD-tot Calculated',
    'widget' => 
    array (
      'weight' => '93',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '69',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '145',
    'field_id' => '128',
    'field_name' => 'field_uwwcoddischargecalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwpincoming' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '35',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Incoming load Ptot (t per year)',
    'required' => 0,
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '35',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'id' => '194',
    'field_id' => '167',
    'field_name' => 'field_uwwpincoming',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwwatertreatment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '23',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'type of treatment of sewage',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '8',
    ),
    'id' => '174',
    'field_id' => '151',
    'field_name' => 'field_uwwwatertreatment',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_inspireidnamespace' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '16',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'inspireIdNamespace',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '3',
    ),
    'id' => '165',
    'field_id' => '9',
    'field_name' => 'field_inspireidnamespace',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwcodincomingcalculated' => 
  array (
    'label' => 'Incoming load COD-tot Calculated',
    'widget' => 
    array (
      'weight' => '88',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '64',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '183',
    'field_id' => '159',
    'field_name' => 'field_uwwcodincomingcalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwbodincomingcalculated' => 
  array (
    'label' => 'Incoming load BOD-tot Calculated',
    'widget' => 
    array (
      'weight' => '87',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '63',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '186',
    'field_id' => '162',
    'field_name' => 'field_uwwbodincomingcalculated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwww_required_rcac' => 
  array (
    'label' => 'Treatment required rcaC',
    'widget' => 
    array (
      'weight' => '114',
      'type' => 'options_buttons',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'list_default',
        'weight' => '88',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => 
    array (
      0 => 
      array (
        'value' => '0',
      ),
    ),
    'id' => '225',
    'field_id' => '194',
    'field_name' => 'field_uwww_required_rcac',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwboddischargeestimated' => 
  array (
    'label' => 'Discharged load BOD-tot Estimated',
    'widget' => 
    array (
      'weight' => '92',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '68',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '184',
    'field_id' => '160',
    'field_name' => 'field_uwwboddischargeestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwozonation' => 
  array (
    'default_value' => 
    array (
    ),
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '38',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Ozonation',
    'required' => 0,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'options',
      'settings' => 
      array (
        'display_label' => 0,
      ),
      'type' => 'options_buttons',
      'weight' => '26',
    ),
    'id' => '148',
    'field_id' => '131',
    'field_name' => 'field_uwwozonation',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_linked_agglomerations' => 
  array (
    'label' => 'Linked agglomeration(s)',
    'widget' => 
    array (
      'weight' => '81',
      'type' => 'node_reference_autocomplete',
      'module' => 'node_reference',
      'active' => 1,
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'size' => '60',
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '61',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'node_reference',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '210',
    'field_id' => '181',
    'field_name' => 'field_linked_agglomerations',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwww_required_rcaan' => 
  array (
    'label' => 'Treatment required rcaANitro',
    'widget' => 
    array (
      'weight' => '111',
      'type' => 'options_buttons',
      'module' => 'options',
      'active' => 1,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'list_default',
        'weight' => '85',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'list',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => 
    array (
      0 => 
      array (
        'value' => '0',
      ),
    ),
    'id' => '216',
    'field_id' => '186',
    'field_name' => 'field_uwww_required_rcaan',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwremarks' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '42',
        'settings' => 
        array (
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'text',
      ),
      'teaser' => 
      array (
        'label' => 'above',
        'settings' => 
        array (
        ),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'label' => 'Remarks',
    'required' => 0,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'active' => 1,
      'module' => 'text',
      'settings' => 
      array (
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => '43',
    ),
    'id' => '206',
    'field_id' => '178',
    'field_name' => 'field_uwwremarks',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
  'field_uwwndischargeestimated' => 
  array (
    'label' => 'Discharged load N-tot Estimated',
    'widget' => 
    array (
      'weight' => '95',
      'type' => 'number',
      'module' => 'number',
      'active' => 0,
      'settings' => 
      array (
      ),
    ),
    'settings' => 
    array (
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'number_decimal',
        'weight' => '71',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => 2,
          'prefix_suffix' => true,
          'empty_fields_handler' => '',
          'empty_fields_emptyfieldtext_empty_text' => '',
        ),
        'module' => 'number',
      ),
      'teaser' => 
      array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => 
        array (
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'id' => '224',
    'field_id' => '193',
    'field_name' => 'field_uwwndischargeestimated',
    'entity_type' => 'node',
    'bundle' => 'uwwtp',
    'deleted' => '0',
  ),
);