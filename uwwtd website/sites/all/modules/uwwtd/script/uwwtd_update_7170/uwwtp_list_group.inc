<?php 

$groups = array (
  'group_description_right' => 
  (object)(array(
     'id' => '26',
     'identifier' => 'group_description_right|node|uwwtp|default',
     'group_name' => 'group_description_right',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_description',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Description Right',
     'weight' => '6',
     'children' => 
    array (
      0 => 'group_data_flip',
    ),
     'format_type' => 'div',
     'format_settings' => 
    array (
      'label' => 'Description Right',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-description-right field-group-div  col-sm-6',
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
     'id' => '30',
     'identifier' => 'group_waste_water|node|uwwtp|default',
     'group_name' => 'group_waste_water',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_container1',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Waste Water Network Connexions',
     'weight' => '29',
     'children' => 
    array (
      0 => 'field_linked_receiving_areas',
      1 => 'field_linked_discharge_points',
      2 => 'field_uwwaggliste',
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
  'group_container2' => 
  (object)(array(
     'id' => '31',
     'identifier' => 'group_container2|node|uwwtp|default',
     'group_name' => 'group_container2',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
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
  'group_map' => 
  (object)(array(
     'id' => '20',
     'identifier' => 'group_map|node|uwwtp|default',
     'group_name' => 'group_map',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
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
  'group_timeline' => 
  (object)(array(
     'id' => '21',
     'identifier' => 'group_timeline|node|uwwtp|default',
     'group_name' => 'group_timeline',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
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
  'group_site_information' => 
  (object)(array(
     'id' => '32',
     'identifier' => 'group_site_information|node|uwwtp|default',
     'group_name' => 'group_site_information',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_container2',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Site information',
     'weight' => '21',
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
  'group_graphical' => 
  (object)(array(
     'id' => '27',
     'identifier' => 'group_graphical|node|uwwtp|default',
     'group_name' => 'group_graphical',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
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
        'classes' => 'group-graphical field-group-fieldset  container visible-md visible-lg',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
  'group_container1' => 
  (object)(array(
     'id' => '28',
     'identifier' => 'group_container1|node|uwwtp|default',
     'group_name' => 'group_container1',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
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
      0 => 'group_waste_water',
      1 => 'group_characteristics',
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
  'group_description' => 
  (object)(array(
     'id' => '22',
     'identifier' => 'group_description|node|uwwtp|default',
     'group_name' => 'group_description',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
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
      0 => 'group_description_right',
      1 => 'group_description_left',
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
  'group_characteristics' => 
  (object)(array(
     'id' => '29',
     'identifier' => 'group_characteristics|node|uwwtp|default',
     'group_name' => 'group_characteristics',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_container1',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Characteristics',
     'weight' => '28',
     'children' => 
    array (
      0 => 'field_uwwmicrofiltration',
      1 => 'field_uwwpremoval',
      2 => 'field_uwwsandfiltration',
      3 => 'field_uwwnremoval',
      4 => 'field_uwwuv',
      5 => 'field_uwwsecondarytreatment',
      6 => 'field_uwwprimarytreatment',
      7 => 'field_uwwothertreat',
      8 => 'field_uwwchlorination',
      9 => 'field_uwwozonation',
      10 => 'virtual_field_latitude_custom',
      11 => 'virtual_field_longitude_custom',
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
  'group_data_flip' => 
  (object)(array(
     'id' => '24',
     'identifier' => 'group_data_flip|node|uwwtp|default',
     'group_name' => 'group_data_flip',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_description_right',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'data flip',
     'weight' => '30',
     'children' => 
    array (
      0 => 'virtual_field_stackedbar_uwwtpnode',
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
     'id' => '23',
     'identifier' => 'group_description_left|node|uwwtp|default',
     'group_name' => 'group_description_left',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_description',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Description left',
     'weight' => '3',
     'children' => 
    array (
      0 => 'field_uwwtssperf',
      1 => 'field_uwwperformance_met',
      2 => 'field_uwwntotperf',
      3 => 'field_uwwotherperf',
      4 => 'field_uwwcodperf',
      5 => 'field_uwwloadenteringuwwtp',
      6 => 'field_uwwbod5perf',
      7 => 'field_uwwtreatmentrequired',
      8 => 'field_uwwcompliance',
      9 => 'field_uwwtreatmenttype',
      10 => 'field_validto',
      11 => 'field_uwwwastewatertreated',
      12 => 'field_uwwtreatment_met',
      13 => 'field_physicalcapacityactivity',
      14 => 'field_uwwptotperf',
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
  'group_forward_looking_aspect' => 
  (object)(array(
     'id' => '40',
     'identifier' => 'group_forward_looking_aspect|node|uwwtp|default',
     'group_name' => 'group_forward_looking_aspect',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_container2',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Forward looking aspect',
     'weight' => '22',
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
  'group_data_flip_front' => 
  (object)(array(
     'id' => '25',
     'identifier' => 'group_data_flip_front|node|uwwtp|default',
     'group_name' => 'group_data_flip_front',
     'entity_type' => 'node',
     'bundle' => 'uwwtp',
     'mode' => 'default',
     'parent_name' => 'group_data_flip',
     'table' => 'field_group',
     'type' => 'Normal',
     'export_type' => 1,
     'disabled' => false,
     'label' => 'Data flip front uwwtp',
     'weight' => '5',
     'children' => 
    array (
      0 => 'virtual_field_data_stackedbar',
    ),
     'format_type' => 'fieldset',
     'format_settings' => 
    array (
      'label' => 'Data flip front uwwtp',
      'instance_settings' => 
      array (
        'id' => '',
        'classes' => 'group-data-flip-front field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  )),
);