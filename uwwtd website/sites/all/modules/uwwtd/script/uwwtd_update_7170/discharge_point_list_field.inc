<?php 

$fields_instance = array (
  'field_dcpmsprovide' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Has the Member State provided these studies to the Commission (art.6.2)',
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
      'weight' => '24',
    ),
    'id' => '252',
    'field_id' => '209',
    'field_name' => 'field_dcpmsprovide',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_linked_treatment_plants' => 
  array (
    'label' => 'Linked treatment plants',
    'widget' => 
    array (
      'weight' => '55',
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
    'id' => '257',
    'field_id' => '33',
    'field_name' => 'field_linked_treatment_plants',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcptype' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Type of discharge point (UWWTP/collecting system)',
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
    'id' => '247',
    'field_id' => '207',
    'field_name' => 'field_dcptype',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_rcatype' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '10',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Type of receiving area',
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
    'id' => '245',
    'field_id' => '205',
    'field_name' => 'field_rcatype',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'type' => 'text_default',
        'weight' => '48',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'ID of the report',
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
    'id' => '250',
    'field_id' => '42',
    'field_name' => 'field_repcode',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpwaterbodyid' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Linked water body',
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
      'weight' => '15',
    ),
    'id' => '241',
    'field_id' => '201',
    'field_name' => 'field_dcpwaterbodyid',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '5',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '41',
    ),
    'id' => '253',
    'field_id' => '95',
    'field_name' => 'field_sourcefile',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpbeginlife' => 
  array (
    'label' => 'dcpbeginlife',
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 56,
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
        'weight' => '44',
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
    'id' => '449',
    'field_id' => '357',
    'field_name' => 'field_dcpbeginlife',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_dcpwfdrbd' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'ID of WFD river basin district',
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
    'id' => '272',
    'field_id' => '218',
    'field_name' => 'field_dcpwfdrbd',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '11',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '39',
    ),
    'id' => '277',
    'field_id' => '86',
    'field_name' => 'field_anneedata',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpnotaffect' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Do comprehensive studies indicate that the discharge does not adversely affect the environment? (art.6.2)',
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
      'weight' => '23',
    ),
    'id' => '286',
    'field_id' => '229',
    'field_name' => 'field_dcpnotaffect',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
      'weight' => '35',
    ),
    'id' => '292',
    'field_id' => '85',
    'field_name' => 'field_position_geo',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '34',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '37',
    ),
    'id' => '255',
    'field_id' => '53',
    'field_name' => 'field_sensitivearealink',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpwaterbodyreferencedate' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '17',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => false,
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
    'label' => 'Reference date of the WFD waterbody ',
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
      'weight' => '17',
    ),
    'id' => '251',
    'field_id' => '208',
    'field_name' => 'field_dcpwaterbodyreferencedate',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_linked_receiving_areas' => 
  array (
    'label' => 'Linked receiving areas',
    'widget' => 
    array (
      'weight' => '54',
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
        'weight' => '14',
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
    'id' => '249',
    'field_id' => '90',
    'field_name' => 'field_linked_receiving_areas',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpeffluentflowtype' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'EffluentFlowType',
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
    'id' => '242',
    'field_id' => '202',
    'field_name' => 'field_dcpeffluentflowtype',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpwaterbodyname' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Name of the WFD waterbody',
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
      'weight' => '16',
    ),
    'id' => '243',
    'field_id' => '203',
    'field_name' => 'field_dcpwaterbodyname',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '31',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '33',
    ),
    'id' => '254',
    'field_id' => '72',
    'field_name' => 'field_beginlifespanversion',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpirrigation' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '12',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'In case of discharge on land please specify the purpose',
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
      'weight' => '13',
    ),
    'id' => '264',
    'field_id' => '213',
    'field_name' => 'field_dcpirrigation',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '47',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Status of the discharge point',
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
    'id' => '262',
    'field_id' => '61',
    'field_name' => 'field_status',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_utilityfacilityreference' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '12',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'ID of UWWTP (or collecting system without treatment)',
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
    'id' => '265',
    'field_id' => '214',
    'field_name' => 'field_utilityfacilityreference',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpsurfacewaters' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Surface water',
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
      'weight' => '43',
    ),
    'id' => '258',
    'field_id' => '210',
    'field_name' => 'field_dcpsurfacewaters',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_specialisedzonetype' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '33',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'specialisedZoneType',
    'required' => false,
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'module' => 'text',
      'settings' => 
      array (
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => '36',
    ),
    'id' => '259',
    'field_id' => '35',
    'field_name' => 'field_specialisedzonetype',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpreceivingwater' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '38',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Linked River Basin District',
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
      'weight' => '46',
    ),
    'id' => '260',
    'field_id' => '211',
    'field_name' => 'field_dcpreceivingwater',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '28',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '30',
    ),
    'id' => '261',
    'field_id' => '169',
    'field_name' => 'field_validfrom',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpwaterbodytype' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '9',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Receiving water',
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
      'weight' => '12',
    ),
    'id' => '263',
    'field_id' => '212',
    'field_name' => 'field_dcpwaterbodytype',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpremarks' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '26',
    ),
    'id' => '273',
    'field_id' => '219',
    'field_name' => 'field_dcpremarks',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '4',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '52',
    ),
    'id' => '274',
    'field_id' => '100',
    'field_name' => 'field_regionnuts',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_verticalposition' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'verticalPosition',
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
    'id' => '275',
    'field_id' => '220',
    'field_name' => 'field_verticalposition',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '46',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Identifier of the discharge point',
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
    'id' => '267',
    'field_id' => '17',
    'field_name' => 'field_inspireidlocalid',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_rcaid' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'ID of receiving area',
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
      'weight' => '14',
    ),
    'id' => '268',
    'field_id' => '215',
    'field_name' => 'field_rcaid',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '10',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '40',
    ),
    'id' => '269',
    'field_id' => '30',
    'field_name' => 'field_siteid',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_appurtenancetype' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '27',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'appurtenanceType',
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
      'weight' => '28',
    ),
    'id' => '270',
    'field_id' => '216',
    'field_name' => 'field_appurtenancetype',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_governmentalservicereferen' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'governmentalServiceReference',
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
      'weight' => '32',
    ),
    'id' => '271',
    'field_id' => '217',
    'field_name' => 'field_governmentalservicereferen',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpuntreated' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
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
    'label' => 'Volume of untreated waste water   (m3/y)  in case of situation 125 and 126 of EFFLU (LOV)',
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
      'weight' => '20',
    ),
    'id' => '278',
    'field_id' => '222',
    'field_name' => 'field_dcpuntreated',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpaccoverflownumber' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '20',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'prefix_suffix' => true,
          'scale' => 0,
          'thousand_separator' => ' ',
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
    'label' => 'Indicate number of days overflows each year (average for the last five years if known)  in case of situation 125 and 126 of EFFL',
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
      'weight' => '21',
    ),
    'id' => '279',
    'field_id' => '223',
    'field_name' => 'field_dcpaccoverflownumber',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcplatitude' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '13',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
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
      'weight' => '10',
    ),
    'id' => '280',
    'field_id' => '224',
    'field_name' => 'field_dcplatitude',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpgroundwater' => 
  array (
    'label' => 'ID of WFD groundwater body',
    'widget' => 
    array (
      'weight' => '50',
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
    'id' => '281',
    'field_id' => '225',
    'field_name' => 'field_dcpgroundwater',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpuwwliste' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '35',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Linked waste water treatment plant(s)',
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
      'weight' => '45',
    ),
    'id' => '282',
    'field_id' => '226',
    'field_name' => 'field_dcpuwwliste',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '32',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '34',
    ),
    'id' => '283',
    'field_id' => '98',
    'field_name' => 'field_endlifespanversion',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpwfdrbdname' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '18',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Name of the WFD river basin district',
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
    'id' => '287',
    'field_id' => '230',
    'field_name' => 'field_dcpwfdrbdname',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpimpact' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Does the discharge affect the objective of other relevant directives (MSFD, WFD, BD,…) annex I.B.4',
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
      'weight' => '22',
    ),
    'id' => '293',
    'field_id' => '235',
    'field_name' => 'field_dcpimpact',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpwfdsubunit' => 
  array (
    'label' => 'ID of WFD sub-unit',
    'widget' => 
    array (
      'weight' => '51',
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
        'weight' => '43',
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
    'id' => '294',
    'field_id' => '236',
    'field_name' => 'field_dcpwfdsubunit',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpsuccessor' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '7',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
      'weight' => '9',
    ),
    'id' => '290',
    'field_id' => '233',
    'field_name' => 'field_dcpsuccessor',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpendlife' => 
  array (
    'label' => 'dcpendlife',
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 57,
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
        'weight' => '45',
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
    'id' => '450',
    'field_id' => '358',
    'field_name' => 'field_dcpendlife',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_dcp_wfdsubunitrefdate' => 
  array (
    'label' => 'Reference date of the WFD sub-unit',
    'widget' => 
    array (
      'weight' => '49',
      'type' => 'date_text',
      'module' => 'date',
      'active' => 1,
      'settings' => 
      array (
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'year_range' => '-3:+3',
        'increment' => 1,
        'label_position' => 'above',
        'text_parts' => 
        array (
        ),
      ),
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
        'weight' => '41',
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
    'required' => 0,
    'description' => '',
    'id' => '244',
    'field_id' => '204',
    'field_name' => 'field_dcp_wfdsubunitrefdate',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpwfdrbdreferencedate' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '40',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => false,
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
    'label' => 'WFDRBD reference date',
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
      'weight' => '48',
    ),
    'id' => '289',
    'field_id' => '232',
    'field_name' => 'field_dcpwfdrbdreferencedate',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpdcpgroundwaterreference' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '37',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => false,
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
    'label' => 'Ground water reference date',
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
      'weight' => '38',
    ),
    'id' => '288',
    'field_id' => '231',
    'field_name' => 'field_dcpdcpgroundwaterreference',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpdateexported' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '26',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => false,
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
      'weight' => '27',
    ),
    'id' => '284',
    'field_id' => '227',
    'field_name' => 'field_dcpdateexported',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpcomaccept' => 
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
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Did the Commission formally accept that the conditions under Art. 6(2) are met?',
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
      'weight' => '25',
    ),
    'id' => '246',
    'field_id' => '206',
    'field_name' => 'field_dcpcomaccept',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_linked_agglomerations' => 
  array (
    'label' => 'Linked agglomerations',
    'widget' => 
    array (
      'weight' => '53',
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
        'weight' => '12',
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
    'id' => '248',
    'field_id' => '181',
    'field_name' => 'field_linked_agglomerations',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '6',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'id' => '266',
    'field_id' => '9',
    'field_name' => 'field_inspireidnamespace',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcplongitude' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '14',
        'settings' => 
        array (
          'decimal_separator' => '.',
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'prefix_suffix' => true,
          'scale' => 2,
          'thousand_separator' => ' ',
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
      'weight' => '11',
    ),
    'id' => '276',
    'field_id' => '221',
    'field_name' => 'field_dcplongitude',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcprcaliste' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '36',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'label' => 'Linked receiving area(s)',
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
      'weight' => '44',
    ),
    'id' => '285',
    'field_id' => '228',
    'field_name' => 'field_dcprcaliste',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
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
        'weight' => '9',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
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
    'id' => '256',
    'field_id' => '54',
    'field_name' => 'field_inspireidversionid',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
  'field_dcpreceivingwaterreference' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '39',
        'settings' => 
        array (
          'empty_fields_emptyfieldtext_empty_text' => '',
          'empty_fields_handler' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => false,
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
    'label' => 'Receiving water reference date',
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
      'weight' => '47',
    ),
    'id' => '291',
    'field_id' => '234',
    'field_name' => 'field_dcpreceivingwaterreference',
    'entity_type' => 'node',
    'bundle' => 'discharge_point',
    'deleted' => '0',
  ),
);