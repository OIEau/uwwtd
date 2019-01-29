<?php 

$groups = array (
  'group_container2' => 
  (object)(array(
     'id' => '37',
     'identifier' => 'group_container2|node|discharge_point|default',
     'group_name' => 'group_container2',
     'entity_type' => 'node',
     'bundle' => 'discharge_point',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'container_2',
     'weight' => '3',
     'children' => 
    array (
      0 => 'group_site_information',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'container_2',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-container2 field-group-div  container',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_site_information' => 
  (object)(array(
     'id' => '38',
     'identifier' => 'group_site_information|node|discharge_point|default',
     'group_name' => 'group_site_information',
     'entity_type' => 'node',
     'bundle' => 'discharge_point',
     'mode' => 'default',
     'parent_name' => 'group_container2',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Site information',
     'weight' => '64',
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
        'classes' => 'group-site-information field-group-fieldset  col-sm-6',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_map' => 
  (object)(array(
     'id' => '33',
     'identifier' => 'group_map|node|discharge_point|default',
     'group_name' => 'group_map',
     'entity_type' => 'node',
     'bundle' => 'discharge_point',
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
        'classes' => 'group-map field-group-div',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_container1' => 
  (object)(array(
     'id' => '34',
     'identifier' => 'group_container1|node|discharge_point|default',
     'group_name' => 'group_container1',
     'entity_type' => 'node',
     'bundle' => 'discharge_point',
     'mode' => 'default',
     'parent_name' => '',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'container_1',
     'weight' => '2',
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
  'group_characteristics' => 
  (object)(array(
     'id' => '35',
     'identifier' => 'group_characteristics|node|discharge_point|default',
     'group_name' => 'group_characteristics',
     'entity_type' => 'node',
     'bundle' => 'discharge_point',
     'mode' => 'default',
     'parent_name' => 'group_container1',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Characteristics',
     'weight' => '50',
     'children' => 
    array (
      0 => 'field_rcatype',
      1 => 'field_dcpwaterbodytype',
      2 => 'field_dcpirrigation',
      3 => 'field_dcpsurfacewaters',
      4 => 'virtual_field_latitude_custom',
      5 => 'virtual_field_longitude_custom',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Characteristics',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-characteristics field-group-fieldset  col-sm-6',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_waste_water' => 
  (object)(array(
     'id' => '36',
     'identifier' => 'group_waste_water|node|discharge_point|default',
     'group_name' => 'group_waste_water',
     'entity_type' => 'node',
     'bundle' => 'discharge_point',
     'mode' => 'default',
     'parent_name' => 'group_container1',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Waste Water Network Connexions',
     'weight' => '51',
     'children' => 
    array (
      0 => 'field_linked_treatment_plants',
      1 => 'field_dcpwaterbodyid',
      2 => 'field_linked_receiving_areas',
      3 => 'field_dcpwfdrbd',
      4 => 'field_linked_agglomerations',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Waste Water Network Connexions',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-waste-water field-group-fieldset  col-sm-6',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
);