<?php 

$fields_instance = array (
  'field_aggaccoverflows' => 
  array (
    'label' => 'What are the measures based on: Acceptable number of overflows per year',
    'widget' => 
    array (
      'weight' => '97',
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
        'weight' => '81',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '105',
    'field_id' => '92',
    'field_name' => 'field_aggaccoverflows',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_environmentaldomain2' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '64',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'environmentalDomain2',
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
    'id' => '88',
    'field_id' => '75',
    'field_name' => 'field_environmentaldomain2',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '69',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '63',
    ),
    'id' => '111',
    'field_id' => '98',
    'field_name' => 'field_endlifespanversion',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_comp_register' => 
  array (
    'label' => 'Global compliance official',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '101',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 117,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '502',
    'field_id' => '407',
    'field_name' => 'field_agg_comp_register',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
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
        'weight' => '73',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '69',
    ),
    'id' => '45',
    'field_id' => '32',
    'field_name' => 'field_sensitiveareatype',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggdatecompliance6' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'hidden',
        'weight' => '118',
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
    'label' => 'date of compliance for article 6',
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
      'weight' => '30',
    ),
    'id' => '97',
    'field_id' => '84',
    'field_name' => 'field_aggdatecompliance6',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_competentauthority' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '70',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'competentAuthority',
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
      'weight' => '64',
    ),
    'id' => '26',
    'field_id' => '13',
    'field_name' => 'field_competentauthority',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agghaveregistrationsystem' => 
  array (
    'default_value' => NULL,
    'description' => 'Is a registration system for leaks in place?',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '27',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Leaks registration',
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
      'weight' => '44',
    ),
    'id' => '59',
    'field_id' => '46',
    'field_name' => 'field_agghaveregistrationsystem',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_compliance_explication' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '75',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Compliance explication',
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
      'weight' => '86',
    ),
    'id' => '57',
    'field_id' => '44',
    'field_name' => 'field_compliance_explication',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggdatecompliance4' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '38',
        'settings' => 
        array (
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'date of compliance for article 4',
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
      'weight' => '26',
    ),
    'id' => '114',
    'field_id' => '101',
    'field_name' => 'field_aggdatecompliance4',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggdatecompliance5' => 
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
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'date of compliance for article 5',
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
      'weight' => '28',
    ),
    'id' => '58',
    'field_id' => '45',
    'field_name' => 'field_aggdatecompliance5',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_linked_receiving_areas' => 
  array (
    'label' => 'Linked receiving areas',
    'widget' => 
    array (
      'weight' => '94',
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
        'weight' => '28',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '103',
    'field_id' => '90',
    'field_name' => 'field_linked_receiving_areas',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggperiodover4' => 
  array (
    'description' => 'Date of the relevant deadline of UWWTD or Transitional Period article 4',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '25',
        'settings' => 
        array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Deadline 2nd treatment compliance',
    'required' => 0,
    'settings' => 
    array (
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '25',
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
    'id' => '54',
    'field_id' => '41',
    'field_name' => 'field_aggperiodover4',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_discharge_pe' => 
  array (
    'label' => 'Load discharged without treatment',
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
        'weight' => '96',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 112,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '497',
    'field_id' => '402',
    'field_name' => 'field_agg_discharge_pe',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggendlife' => 
  array (
    'label' => 'Agglomeration end life',
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 106,
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
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '446',
    'field_id' => '354',
    'field_name' => 'field_aggendlife',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggart4compliance' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => '2nd treatment compliance',
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
      'weight' => '82',
    ),
    'id' => '63',
    'field_id' => '50',
    'field_name' => 'field_aggart4compliance',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '116',
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
    'id' => '55',
    'field_id' => '42',
    'field_name' => 'field_repcode',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_namenativeness' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '20',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'nameNativeness',
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
    'id' => '21',
    'field_id' => '8',
    'field_name' => 'field_namenativeness',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '72',
    ),
    'id' => '43',
    'field_id' => '30',
    'field_name' => 'field_siteid',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggpercstringenttreatment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '49',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'IAS - in situ and/ or transported to UWWTPs by trucks: How much in % of generated load of agglomeration with more stringent trea',
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
      'weight' => '39',
    ),
    'id' => '90',
    'field_id' => '77',
    'field_name' => 'field_aggpercstringenttreatment',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggpercsectreatment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '48',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'IAS - in situ and/or transported to UWWTPs by trucks: How much in % of generated load of agglomeration with secondary treatment',
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
      'weight' => '38',
    ),
    'id' => '78',
    'field_id' => '65',
    'field_name' => 'field_aggpercsectreatment',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggnathyperlink' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '60',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Agglomeration national Internet hyperlink',
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
    'id' => '112',
    'field_id' => '99',
    'field_name' => 'field_aggnathyperlink',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggperiodover5' => 
  array (
    'description' => 'Date of the relevant deadline of UWWTD or Transitional Period article 5',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '39',
        'settings' => 
        array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Deadline 3rd treatment compliance',
    'required' => 0,
    'settings' => 
    array (
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '27',
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
    'id' => '72',
    'field_id' => '59',
    'field_name' => 'field_aggperiodover5',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '74',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '70',
    ),
    'id' => '66',
    'field_id' => '53',
    'field_name' => 'field_sensitivearealink',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_namestatus' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'nameStatus',
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
    'id' => '70',
    'field_id' => '57',
    'field_name' => 'field_namestatus',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggdatecompliance3' => 
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
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'date of compliance for article 3',
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
      'weight' => '24',
    ),
    'id' => '35',
    'field_id' => '22',
    'field_name' => 'field_aggdatecompliance3',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agggenerated' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '8',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'label' => 'Generated load (p.e.)',
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
      'weight' => '18',
    ),
    'id' => '101',
    'field_id' => '88',
    'field_name' => 'field_agggenerated',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggforecast' => 
  array (
    'label' => 'When will the total generated load of the agglomeration be collected through collecting systems or adressed through IAS?',
    'widget' => 
    array (
      'weight' => '101',
      'type' => 'date_text',
      'module' => 'date',
      'active' => 1,
      'settings' => 
      array (
        'input_format' => 'd/m/Y - H:i:s',
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
        'weight' => '85',
        'settings' => 
        array (
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '109',
    'field_id' => '96',
    'field_name' => 'field_aggforecast',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agginhabitantsconnectedias' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '57',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'label' => 'No of inhabitants served by IAS per agglomeration',
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
      'weight' => '51',
    ),
    'id' => '52',
    'field_id' => '39',
    'field_name' => 'field_agginhabitantsconnectedias',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agguwwliste' => 
  array (
    'default_value' => NULL,
    'description' => 'Linked Urban Waste Water Treatment plant(s)',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '25',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Linked Treatment Plant(s)',
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
      'weight' => '75',
    ),
    'id' => '77',
    'field_id' => '64',
    'field_name' => 'field_agguwwliste',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_linked_discharge_points' => 
  array (
    'label' => 'Linked discharge points',
    'widget' => 
    array (
      'weight' => '92',
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
        'weight' => '27',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '40',
    'field_id' => '27',
    'field_name' => 'field_linked_discharge_points',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_typeof_sa' => 
  array (
    'label' => 'Type of sensitive area',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '113',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 129,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '514',
    'field_id' => '419',
    'field_name' => 'field_agg_typeof_sa',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggart6compliance' => 
  array (
    'label' => 'Article 6 compliance',
    'widget' => 
    array (
      'type' => 'options_select',
      'weight' => '102',
      'settings' => 
      array (
      ),
      'module' => 'options',
    ),
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '86',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'required' => false,
    'description' => '',
    'default_value' => NULL,
    'id' => '25',
    'field_id' => '12',
    'field_name' => 'field_aggart6compliance',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_connected_load_pe' => 
  array (
    'label' => 'Connected load',
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
        'weight' => '94',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 110,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '495',
    'field_id' => '400',
    'field_name' => 'field_agg_connected_load_pe',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_agg_equip_ms' => 
  array (
    'label' => 'More stringent in place',
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
        'weight' => '106',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 122,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '507',
    'field_id' => '412',
    'field_name' => 'field_agg_equip_ms',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_agglongitude' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '28',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '17',
    ),
    'id' => '86',
    'field_id' => '73',
    'field_name' => 'field_agglongitude',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggperiodover6' => 
  array (
    'description' => 'Date of the relevant deadline of UWWTD or Transitional Period article 6',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'hidden',
        'weight' => '117',
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
    'label' => 'Deadline article 6',
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
      'weight' => '29',
    ),
    'id' => '94',
    'field_id' => '81',
    'field_name' => 'field_aggperiodover6',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_namelanguage' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '19',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'nameLanguage',
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
    'id' => '42',
    'field_id' => '29',
    'field_name' => 'field_namelanguage',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggcompliance' => 
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
        'weight' => '23',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '76',
    ),
    'id' => '36',
    'field_id' => '23',
    'field_name' => 'field_aggcompliance',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_legislationcitation' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '71',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'LegislationCitation',
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
      'weight' => '65',
    ),
    'id' => '68',
    'field_id' => '55',
    'field_name' => 'field_legislationcitation',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggchanges' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '35',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Are there any significant changes of the generated load compared to the previous Report?',
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
      'weight' => '21',
    ),
    'id' => '31',
    'field_id' => '18',
    'field_name' => 'field_aggchanges',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_region_nuts3' => 
  array (
    'label' => 'Region (NUTS3)',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '93',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 109,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '494',
    'field_id' => '399',
    'field_name' => 'field_region_nuts3',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggc1' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '41',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Connected to collecting system',
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
      'weight' => '31',
    ),
    'id' => '82',
    'field_id' => '69',
    'field_name' => 'field_aggc1',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggothermeasures' => 
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
        'weight' => '54',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'What type of best technical knowledge/measures are used for building and maintanance of collecting system to prevent leaks: Othe',
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
      'weight' => '48',
    ),
    'id' => '47',
    'field_id' => '34',
    'field_name' => 'field_aggothermeasures',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggperiodover3' => 
  array (
    'description' => 'Date of the relevant deadline of UWWTD or Transitional Period article 3',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '24',
        'settings' => 
        array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Deadline connection compliance',
    'required' => 0,
    'settings' => 
    array (
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'weight' => '23',
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
    'id' => '53',
    'field_id' => '40',
    'field_name' => 'field_aggperiodover3',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggsuccessor' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '26',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'ID of the successor',
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
    'id' => '87',
    'field_id' => '74',
    'field_name' => 'field_aggsuccessor',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_article_3_compliance_expli' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '76',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Article 3 compliance explication',
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
      'weight' => '87',
    ),
    'id' => '84',
    'field_id' => '71',
    'field_name' => 'field_article_3_compliance_expli',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_art6_comp_register' => 
  array (
    'label' => 'Less sensitive area compliance (Art 6) official',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '100',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'required' => false,
    'description' => '',
    'id' => '501',
    'field_id' => '406',
    'field_name' => 'field_agg_art6_comp_register',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggpercwithouttreatment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '45',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Discharged without treatment',
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
      'weight' => '35',
    ),
    'id' => '76',
    'field_id' => '63',
    'field_name' => 'field_aggpercwithouttreatment',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '65',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '59',
    ),
    'id' => '61',
    'field_id' => '48',
    'field_name' => 'field_thematicid',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_zonetype' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '62',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'zoneType',
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
    'id' => '470',
    'field_id' => '43',
    'field_name' => 'field_zonetype',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '68',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '62',
    ),
    'id' => '85',
    'field_id' => '72',
    'field_name' => 'field_beginlifespanversion',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggmunicipalities' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Name of municipalities/communes of this agglomeration',
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
      'weight' => '13',
    ),
    'id' => '29',
    'field_id' => '16',
    'field_name' => 'field_aggmunicipalities',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_sourceofname' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'sourceOfName',
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
    'id' => '89',
    'field_id' => '76',
    'field_name' => 'field_sourceofname',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggseweroverflows_pe' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '51',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'label' => 'How much raw sewage has been discharged through combined sewer overflows (CSOs) in the year reported: p.e.',
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
      'weight' => '43',
    ),
    'id' => '96',
    'field_id' => '83',
    'field_name' => 'field_aggseweroverflows_pe',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_region_nuts2' => 
  array (
    'label' => 'Region (NUTS2)',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '92',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 108,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '493',
    'field_id' => '398',
    'field_name' => 'field_region_nuts2',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_namepronunciation' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'NamePronunciation',
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
    'id' => '91',
    'field_id' => '78',
    'field_name' => 'field_namepronunciation',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_perf_s' => 
  array (
    'label' => 'Monitoring results ok for secondary treatment',
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
        'weight' => '103',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 119,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '504',
    'field_id' => '409',
    'field_name' => 'field_agg_perf_s',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_designationperiod' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '67',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'designationPeriod',
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
      'weight' => '61',
    ),
    'id' => '39',
    'field_id' => '26',
    'field_name' => 'field_designationperiod',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggchangescomment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '36',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Comments on significant changes of the generated load compared to the previous reported load',
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
      'weight' => '22',
    ),
    'id' => '38',
    'field_id' => '25',
    'field_name' => 'field_aggchangescomment',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_dilution_rates' => 
  array (
    'label' => 'What are the measures based on: Dilution rates',
    'widget' => 
    array (
      'weight' => '99',
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
        'weight' => '83',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '62',
    'field_id' => '49',
    'field_name' => 'field_agg_dilution_rates',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_bc_morestringet_other' => 
  array (
    'label' => 'Big city - Best treatment : More stringent other',
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
        'weight' => '112',
        'settings' => 
        array (
          'thousand_separator' => '',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 128,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '513',
    'field_id' => '418',
    'field_name' => 'field_agg_bc_morestringet_other',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggexistmaintenanceplan' => 
  array (
    'default_value' => NULL,
    'description' => 'Is a maintenance plan (quality assurance) in place?',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '28',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Maintenance plan',
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
    'id' => '64',
    'field_id' => '51',
    'field_name' => 'field_aggexistmaintenanceplan',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_equip_met_ms' => 
  array (
    'label' => 'Installation in place for more stringent treatment',
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
        'weight' => '104',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 120,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '505',
    'field_id' => '410',
    'field_name' => 'field_agg_equip_met_ms',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggmethodc2' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Method used to obtain the %-value (calculated or estimated, for aggC2)',
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
    'id' => '69',
    'field_id' => '56',
    'field_name' => 'field_aggmethodc2',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggcalculation' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '34',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Methods used for calculation of the generated load of the agglomeration',
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
      'weight' => '20',
    ),
    'id' => '83',
    'field_id' => '70',
    'field_name' => 'field_aggcalculation',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggaccoverflownumber' => 
  array (
    'label' => 'Indicate number of overflows',
    'widget' => 
    array (
      'weight' => '100',
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
        'weight' => '84',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'id' => '92',
    'field_id' => '79',
    'field_name' => 'field_aggaccoverflownumber',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggpressuretest' => 
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
        'weight' => '52',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'What type of best technical knowledge/measures are used for building and maintanance of collecting system to prevent leaks: Pres',
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
      'weight' => '46',
    ),
    'id' => '106',
    'field_id' => '93',
    'field_name' => 'field_aggpressuretest',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_perf_met_ms' => 
  array (
    'label' => 'Monitoring results ok for more stringent treatment',
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
        'weight' => '105',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 121,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '506',
    'field_id' => '411',
    'field_name' => 'field_agg_perf_met_ms',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '71',
    ),
    'id' => '99',
    'field_id' => '86',
    'field_name' => 'field_anneedata',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_art_applied_sa' => 
  array (
    'label' => 'Article applied for SA',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '114',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 130,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '515',
    'field_id' => '420',
    'field_name' => 'field_agg_art_applied_sa',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggcritca' => 
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
        'weight' => '30',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Article 5 criterion c a',
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
    'id' => '41',
    'field_id' => '28',
    'field_name' => 'field_aggcritca',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggart3compliance' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'list_default',
        'weight' => '24',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Connection compliance',
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
      'weight' => '81',
    ),
    'id' => '49',
    'field_id' => '36',
    'field_name' => 'field_aggart3compliance',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggcapacity' => 
  array (
    'label' => 'What are the measures based on: Capacity in relation to dry weather flow',
    'widget' => 
    array (
      'weight' => '98',
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
        'weight' => '82',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '104',
    'field_id' => '91',
    'field_name' => 'field_aggcapacity',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_ias_load_pe' => 
  array (
    'label' => 'Load treated through IAS',
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
        'weight' => '95',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 111,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '496',
    'field_id' => '401',
    'field_name' => 'field_agg_ias_load_pe',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
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
        'weight' => '15',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '84',
    ),
    'id' => '113',
    'field_id' => '100',
    'field_name' => 'field_regionnuts',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '13',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '73',
    ),
    'id' => '108',
    'field_id' => '95',
    'field_name' => 'field_sourcefile',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggart3addcompliance' => 
  array (
    'label' => 'Additionnal Compliance article3',
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'options_select',
      'settings' => 
      array (
      ),
      'weight' => 103,
      'module' => 'options',
    ),
    'required' => false,
    'description' => '',
    'default_value' => NULL,
    'id' => '50',
    'field_id' => '37',
    'field_name' => 'field_aggart3addcompliance',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_bc_secondary' => 
  array (
    'label' => 'Big city - Best treatment : Secondary',
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
        'weight' => '110',
        'settings' => 
        array (
          'thousand_separator' => '',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 126,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '511',
    'field_id' => '416',
    'field_name' => 'field_agg_bc_secondary',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggbcliste' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'node_reference_default',
        'weight' => '9',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Linked Big city(ies)',
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
      'weight' => '74',
    ),
    'id' => '51',
    'field_id' => '38',
    'field_name' => 'field_aggbcliste',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggbeginlife' => 
  array (
    'label' => 'Agglomeration begin life',
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 105,
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
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '445',
    'field_id' => '353',
    'field_name' => 'field_aggbeginlife',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_agg_art4_comp_register' => 
  array (
    'label' => '2nd treatment compliance (Art 4) official',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '98',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 114,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '499',
    'field_id' => '404',
    'field_name' => 'field_agg_art4_comp_register',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
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
        'weight' => '66',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '48',
    'field_id' => '35',
    'field_name' => 'field_specialisedzonetype',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_bc_primary' => 
  array (
    'label' => 'Big city - Best treatment : Primary',
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
        'weight' => '109',
        'settings' => 
        array (
          'thousand_separator' => '',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 125,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '510',
    'field_id' => '415',
    'field_name' => 'field_agg_bc_primary',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggseweroverflows_m3' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '50',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'label' => 'How much raw sewage has been discharged through combined sewer overflows (CSOs) in the year reported: m3/y',
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
      'weight' => '42',
    ),
    'id' => '110',
    'field_id' => '97',
    'field_name' => 'field_aggseweroverflows_m3',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_bc_morestringet_np' => 
  array (
    'label' => 'Big city - Best treatment : More stringent NP',
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
        'weight' => '111',
        'settings' => 
        array (
          'thousand_separator' => '',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 127,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '512',
    'field_id' => '417',
    'field_name' => 'field_agg_bc_morestringet_np',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggdateexported' => 
  array (
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '61',
        'settings' => 
        array (
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_remaining_days' => 0,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '55',
    ),
    'id' => '102',
    'field_id' => '89',
    'field_name' => 'field_aggdateexported',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_linked_treatment_plants' => 
  array (
    'label' => 'Linked treatment plants',
    'widget' => 
    array (
      'type' => 'node_reference_autocomplete',
      'weight' => '90',
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'size' => 60,
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
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
        'weight' => '79',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'default_value' => 
    array (
    ),
    'id' => '46',
    'field_id' => '33',
    'field_name' => 'field_linked_treatment_plants',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggmethodwithouttreatment' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Method used to obtain the %-value (calculated or estimated, for aggPercWithoutTreatment)',
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
      'weight' => '36',
    ),
    'id' => '107',
    'field_id' => '94',
    'field_name' => 'field_aggmethodwithouttreatment',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggmethodc1' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Method used to obtain the %-value (calculated or estimated, for aggC1)',
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
    'id' => '27',
    'field_id' => '14',
    'field_name' => 'field_aggmethodc1',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggart5compliance' => 
  array (
    'default_value' => NULL,
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => '3rd treatment compliance',
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
      'weight' => '83',
    ),
    'id' => '20',
    'field_id' => '7',
    'field_name' => 'field_aggart5compliance',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_rbd_code' => 
  array (
    'label' => 'RBD Code',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '115',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 131,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '516',
    'field_id' => '421',
    'field_name' => 'field_agg_rbd_code',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_documentationcitation' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '72',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'DocumentationCitation',
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
      'weight' => '66',
    ),
    'id' => '81',
    'field_id' => '68',
    'field_name' => 'field_documentationcitation',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_namegrammaticalgender' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'NameGrammaticalGender',
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
    'id' => '71',
    'field_id' => '58',
    'field_name' => 'field_namegrammaticalgender',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_article_4_compliance_expli' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '77',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Article 4 compliance explication',
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
      'weight' => '88',
    ),
    'id' => '65',
    'field_id' => '52',
    'field_name' => 'field_article_4_compliance_expli',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_article17_agglo' => 
  array (
    'label' => 'Article 17 Agglo',
    'widget' => 
    array (
      'type' => 'node_reference_autocomplete',
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'size' => 60,
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
      'weight' => 104,
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
        'weight' => '88',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '115',
    'field_id' => '102',
    'field_name' => 'field_article17_agglo',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_art3_comp_register' => 
  array (
    'label' => 'Connection compliance (Art 3) official',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '97',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 113,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '498',
    'field_id' => '403',
    'field_name' => 'field_agg_art3_comp_register',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
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
        'weight' => '7',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Identifier of the agglomeration',
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
    'id' => '30',
    'field_id' => '17',
    'field_name' => 'field_inspireidlocalid',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggcritcb' => 
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
        'weight' => '31',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Article 5 criterion c b',
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
      'weight' => '79',
    ),
    'id' => '60',
    'field_id' => '47',
    'field_name' => 'field_aggcritcb',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_article_5_compliance_expli' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '78',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Article 5 compliance explication',
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
      'weight' => '89',
    ),
    'id' => '33',
    'field_id' => '20',
    'field_name' => 'field_article_5_compliance_expli',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '18',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '67',
    'field_id' => '54',
    'field_name' => 'field_inspireidversionid',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '17',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '22',
    'field_id' => '9',
    'field_name' => 'field_inspireidnamespace',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agglatitude' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '27',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '16',
    ),
    'id' => '24',
    'field_id' => '11',
    'field_name' => 'field_agglatitude',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agginhabitantswithouttreat' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '58',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'label' => 'No of inhabitants without treatment per agglomeration',
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
      'weight' => '52',
    ),
    'id' => '75',
    'field_id' => '62',
    'field_name' => 'field_agginhabitantswithouttreat',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggbesttechnicalknowledge' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Is best technical knowledge to limit pollution applied?',
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
      'weight' => '41',
    ),
    'id' => '37',
    'field_id' => '24',
    'field_name' => 'field_aggbesttechnicalknowledge',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_bc_notreatment' => 
  array (
    'label' => 'Big city - Best treatment : No treatment',
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
        'weight' => '108',
        'settings' => 
        array (
          'thousand_separator' => '',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 124,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '509',
    'field_id' => '414',
    'field_name' => 'field_agg_bc_notreatment',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_environmentaldomain1' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'text_default',
        'weight' => '63',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'environmentalDomain1',
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
    'id' => '93',
    'field_id' => '80',
    'field_name' => 'field_environmentaldomain1',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_rcadcpliste' => 
  array (
    'label' => 'Linked discharge point(s)',
    'widget' => 
    array (
      'type' => 'node_reference_autocomplete',
      'weight' => '94',
      'settings' => 
      array (
        'autocomplete_match' => 'contains',
        'size' => 60,
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
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
        'weight' => '80',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'id' => '73',
    'field_id' => '60',
    'field_name' => 'field_rcadcpliste',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_art5_comp_register' => 
  array (
    'label' => '3rd treatment compliance (Art 5) official',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '99',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'required' => false,
    'description' => '',
    'id' => '500',
    'field_id' => '405',
    'field_name' => 'field_agg_art5_comp_register',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_agg_perf_ms' => 
  array (
    'label' => 'More stringent monitoring ok',
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
        'weight' => '107',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 123,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '508',
    'field_id' => '413',
    'field_name' => 'field_agg_perf_ms',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggremarks' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
      'weight' => '53',
    ),
    'id' => '44',
    'field_id' => '31',
    'field_name' => 'field_aggremarks',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggcritb' => 
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
        'weight' => '29',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Article 5 criterion b',
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
      'weight' => '80',
    ),
    'id' => '28',
    'field_id' => '15',
    'field_name' => 'field_aggcritb',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggexplanationother' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Short explanation of other measures',
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
      'weight' => '49',
    ),
    'id' => '95',
    'field_id' => '82',
    'field_name' => 'field_aggexplanationother',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggbigid' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'ID of the big city/ big discharger',
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
    'id' => '100',
    'field_id' => '87',
    'field_name' => 'field_aggbigid',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggvideoinspections' => 
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
        'weight' => '53',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'What type of best technical knowledge/measures are used for building and maintanance of collecting system to prevent leaks: Regu',
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
      'weight' => '47',
    ),
    'id' => '80',
    'field_id' => '67',
    'field_name' => 'field_aggvideoinspections',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggpercprimtreatment' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '47',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'IAS - in situ and/ or transported to UWWTPs by trucks: How much in % of generated load of agglomeration with primary treatment',
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
      'weight' => '37',
    ),
    'id' => '19',
    'field_id' => '6',
    'field_name' => 'field_aggpercprimtreatment',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '8',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Status of the agglomeration',
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
    'id' => '74',
    'field_id' => '61',
    'field_name' => 'field_status',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_region_nuts1' => 
  array (
    'label' => 'Region (NUTS1)',
    'settings' => 
    array (
      'text_processing' => 0,
      'user_register_form' => false,
    ),
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '91',
        'settings' => 
        array (
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'widget' => 
    array (
      'type' => 'text_textfield',
      'settings' => 
      array (
        'size' => 60,
      ),
      'weight' => 107,
      'module' => 'text',
    ),
    'required' => false,
    'description' => '',
    'id' => '492',
    'field_id' => '397',
    'field_name' => 'field_region_nuts1',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_agginhabitant' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_integer',
        'weight' => '56',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'label' => 'Population (inhabitants) of the agglomeration',
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
      'weight' => '50',
    ),
    'id' => '23',
    'field_id' => '10',
    'field_name' => 'field_agginhabitant',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
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
        'weight' => '0',
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
    'required' => false,
    'settings' => 
    array (
      'user_register_form' => false,
    ),
    'widget' => 
    array (
      'module' => 'geofield',
      'settings' => 
      array (
      ),
      'type' => 'geofield_wkt',
      'weight' => '68',
    ),
    'id' => '98',
    'field_id' => '85',
    'field_name' => 'field_position_geo',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_agg_equip_s' => 
  array (
    'label' => 'Installation in place for secondary treatment',
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
        'weight' => '102',
        'settings' => 
        array (
          'thousand_separator' => '',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
          'decimal_separator' => '.',
          'scale' => 0,
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
    'widget' => 
    array (
      'type' => 'number',
      'settings' => 
      array (
      ),
      'weight' => 118,
      'module' => 'number',
    ),
    'required' => false,
    'description' => '',
    'id' => '503',
    'field_id' => '408',
    'field_name' => 'field_agg_equip_s',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
    'default_value' => NULL,
  ),
  'field_aggc2' => 
  array (
    'default_value' => NULL,
    'description' => '',
    'display' => 
    array (
      'default' => 
      array (
        'label' => 'inline',
        'type' => 'number_decimal',
        'weight' => '43',
        'settings' => 
        array (
          'thousand_separator' => ' ',
          'decimal_separator' => '.',
          'scale' => '2',
          'prefix_suffix' => 1,
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Individual or Appropriate Systems (IAS)',
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
      'weight' => '33',
    ),
    'id' => '32',
    'field_id' => '19',
    'field_name' => 'field_aggc2',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_aggsewagenetwork' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'Type of sewage network',
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
    'id' => '34',
    'field_id' => '21',
    'field_name' => 'field_aggsewagenetwork',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
  'field_namegrammaticalnumber' => 
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
          'empty_fields_handler' => 'EmptyFieldText',
          'empty_fields_emptyfieldtext_empty_text' => 'Not available',
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
    'label' => 'NameGrammaticalNumber',
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
    'id' => '79',
    'field_id' => '66',
    'field_name' => 'field_namegrammaticalnumber',
    'entity_type' => 'node',
    'bundle' => 'agglomeration',
    'deleted' => '0',
  ),
);