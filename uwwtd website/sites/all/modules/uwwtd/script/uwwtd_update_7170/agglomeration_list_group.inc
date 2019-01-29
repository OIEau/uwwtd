<?php 

$groups = array (
  'group_graphical' => 
  (object)(array(
     'id' => '4',
     'identifier' => 'group_graphical|node|agglomeration|default',
     'group_name' => 'group_graphical',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Graphical network summary',
     'weight' => '4',
     'children' => 
    array (
      0 => 'virtual_graphic',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Graphical network summary',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-graphical field-group-fieldset container visible-md visible-lg',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_site_information' => 
  (object)(array(
     'id' => '8',
     'identifier' => 'group_site_information|node|agglomeration|default',
     'group_name' => 'group_site_information',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_container2',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Site information',
     'weight' => '9',
     'children' => 
    array (
      0 => 'virtual_field_source_file_link',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Site information',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-site-information field-group-fieldset col-sm-6',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_characteristics' => 
  (object)(array(
     'id' => '5',
     'identifier' => 'group_characteristics|node|agglomeration|default',
     'group_name' => 'group_characteristics',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_container1',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Characteristics',
     'weight' => '24',
     'children' => 
    array (
      0 => 'field_agghaveregistrationsystem',
      1 => 'field_aggperiodover4',
      2 => 'field_aggperiodover3',
      3 => 'field_aggexistmaintenanceplan',
      4 => 'field_aggbesttechnicalknowledge',
      5 => 'field_aggsewagenetwork',
      6 => 'virtual_field_latitude_custom',
      7 => 'virtual_field_longitude_custom',
      8 => 'virtual_field_aggperiodover5_custom',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Characteristics',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-characteristics field-group-fieldset col-sm-6',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_timeline' => 
  (object)(array(
     'id' => '2',
     'identifier' => 'group_timeline|node|agglomeration|default',
     'group_name' => 'group_timeline',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'compliance Time Line',
     'weight' => '2',
     'children' => 
    array (
      0 => 'virtual_compliance_time_line',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'compliance Time Line',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-timeline field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_container1' => 
  (object)(array(
     'id' => '11',
     'identifier' => 'group_container1|node|agglomeration|default',
     'group_name' => 'group_container1',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'container_1',
     'weight' => '5',
     'children' => 
    array (
      0 => 'group_characteristics',
      1 => 'group_waste_water',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'container_1',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-container1 field-group-div  container',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_map' => 
  (object)(array(
     'id' => '1',
     'identifier' => 'group_map|node|agglomeration|default',
     'group_name' => 'group_map',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'map',
     'weight' => '1',
     'children' => 
    array (
      0 => 'field_position_geo',
      1 => 'virtual_field_map',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'map',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => '',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_waste_water' => 
  (object)(array(
     'id' => '6',
     'identifier' => 'group_waste_water|node|agglomeration|default',
     'group_name' => 'group_waste_water',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_container1',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Waste Water Network Connexions',
     'weight' => '26',
     'children' => 
    array (
      0 => 'field_linked_receiving_areas',
      1 => 'field_linked_discharge_points',
      2 => 'field_agguwwliste',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Waste Water Network Connexions',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-waste-water field-group-fieldset col-sm-6',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_forward_looking_aspect' => 
  (object)(array(
     'id' => '39',
     'identifier' => 'group_forward_looking_aspect|node|agglomeration|default',
     'group_name' => 'group_forward_looking_aspect',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_container2',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'label' => 'Forward looking aspect',
     'weight' => '10',
     'children' => 
    array (
      0 => 'virtual_field_article_17',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Forward looking aspect',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-forward-looking-aspect field-group-fieldset col-sm-6',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_description' => 
  (object)(array(
     'id' => '3',
     'identifier' => 'group_description|node|agglomeration|default',
     'group_name' => 'group_description',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Description',
     'weight' => '3',
     'children' => 
    array (
      0 => 'group_description_left',
      1 => 'group_description_right',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Description',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-description field-group-fieldset container',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_container2' => 
  (object)(array(
     'id' => '12',
     'identifier' => 'group_container2|node|agglomeration|default',
     'group_name' => 'group_container2',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'container_2',
     'weight' => '6',
     'children' => 
    array (
      0 => 'group_site_information',
      1 => 'group_forward_looking_aspect',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'container_2',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-container2 field-group-div container',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_description_right' => 
  (object)(array(
     'id' => '10',
     'identifier' => 'group_description_right|node|agglomeration|default',
     'group_name' => 'group_description_right',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_description',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Description Right',
     'weight' => '1',
     'children' => 
    array (
      0 => 'field_aggcompliance',
      1 => 'field_aggart3compliance',
      2 => 'virtual_field_aggart4compliance_hierarchical',
      3 => 'virtual_field_aggart5compliance_hierarchical',
      4 => 'virtual_field_table_distance_to_compliance',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'Description Right',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-description-right field-group-div col-sm-6',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_data_flip' => 
  (object)(array(
     'id' => '14',
     'identifier' => 'group_data_flip|node|agglomeration|default',
     'group_name' => 'group_data_flip',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_description_left',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'data flip',
     'weight' => '2',
     'children' => 
    array (
      0 => 'virtual_field_piechart_agglonode',
      1 => 'group_data_flip_front',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'data flip',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'flip',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_description_left' => 
  (object)(array(
     'id' => '9',
     'identifier' => 'group_description_left|node|agglomeration|default',
     'group_name' => 'group_description_left',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_description',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Description left',
     'weight' => '0',
     'children' => 
    array (
      0 => 'group_data_flip',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'Description left',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-description-left field-group-div col-sm-6',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_data_flip_front' => 
  (object)(array(
     'id' => '17',
     'identifier' => 'group_data_flip_front|node|agglomeration|default',
     'group_name' => 'group_data_flip_front',
     'entity_type' => 'node',
     'bundle' => 'agglomeration',
     'mode' => 'default',
     'parent_name' => 'group_data_flip',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Data flip front agglomeration',
     'weight' => '7',
     'children' => 
    array (
      0 => 'field_agggenerated',
      1 => 'virtual_field_aggc1_with_pe',
      2 => 'virtual_field_aggc2_with_pe',
      3 => 'virtual_field_aggpercwithouttreatment_with_pe',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Data flip front agglomeration',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-data-flip-front field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  )),
);