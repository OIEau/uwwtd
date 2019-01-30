<?php 

$groups = array (
  'default' => 
  array (
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
       'label' => 'Description left',
       'weight' => '3',
       'children' => 
      array (
        0 => 'field_uwwbaddesign',
        1 => 'field_uwwntotperf',
        2 => 'field_uwwptotperf',
        3 => 'field_uwwwastewatertreated',
        4 => 'field_uwwbadperfdesign',
        5 => 'field_physicalcapacityactivity',
        6 => 'field_uwwtssperf',
        7 => 'field_uwwperctruck',
        8 => 'field_uwwperformance_met',
        9 => 'field_uwwcodperf',
        10 => 'field_uwwloadenteringuwwtp',
        11 => 'field_uwwtreatmentrequired',
        12 => 'field_uwwcompliance',
        13 => 'field_uwwmethodwastewatertreated',
        14 => 'field_uwwtreatmenttype',
        15 => 'field_uwwinformation',
        16 => 'field_uwwloadcalculation',
        17 => 'field_uwwtreatment_met',
        18 => 'field_uwwbod5perf',
        19 => 'field_uwwotherperf',
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
       'label' => 'Waste Water Network Connexions',
       'weight' => '29',
       'children' => 
      array (
        0 => 'field_uwwaggliste',
        1 => 'field_linked_receiving_areas',
        2 => 'field_linked_discharge_points',
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
    'group_characteristics' => 
    (object)(array(
       'id' => '29',
       'identifier' => 'group_characteristics|node|uwwtp|default',
       'group_name' => 'group_characteristics',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'default',
       'parent_name' => 'group_container2',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'label' => 'Characteristics',
       'weight' => '31',
       'children' => 
      array (
        0 => 'field_uwwhistorie',
        1 => 'field_uwwsuccessor',
        2 => 'field_uwwremarks',
        3 => 'field_uwwspecification',
        4 => 'field_uwwsecondarytreatment',
        5 => 'field_uwwother',
        6 => 'field_uwwmicrofiltration',
        7 => 'field_uwwaccidents',
        8 => 'field_uwwnremoval',
        9 => 'field_uwwe_prtrcode',
        10 => 'field_uwwuv',
        11 => 'field_uwwprimarytreatment',
        12 => 'field_validto',
        13 => 'field_uwwothertreat',
        14 => 'field_uwwsandfiltration',
        15 => 'field_uwwozonation',
        16 => 'field_uwwpremoval',
        17 => 'field_uwwchlorination',
        18 => 'virtual_field_latitude_custom',
        19 => 'virtual_field_longitude_custom',
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
       'label' => 'container_2',
       'weight' => '6',
       'children' => 
      array (
        0 => 'group_characteristics',
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
    'group_site_information' => 
    (object)(array(
       'id' => '32',
       'identifier' => 'group_site_information|node|uwwtp|default',
       'group_name' => 'group_site_information',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'default',
       'parent_name' => 'group_container1',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'label' => 'Site information',
       'weight' => '30',
       'children' => 
      array (
        0 => 'field_uwwnathyperlink',
        1 => 'virtual_field_source_file_link',
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
       'label' => 'Forward looking aspect',
       'weight' => '32',
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
       'label' => 'container_1',
       'weight' => '5',
       'children' => 
      array (
        0 => 'group_waste_water',
        1 => 'group_site_information',
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
  ),
  'form' => 
  array (
    'group_uwwtp_incom_disch' => 
    (object)(array(
       'id' => '54',
       'identifier' => 'group_uwwtp_incom_disch|node|uwwtp|form',
       'group_name' => 'group_uwwtp_incom_disch',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => 'group_uwwtp_description',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'Incoming/Discharged',
       'weight' => '23',
       'children' => 
      array (
        0 => 'field_uwwboddischargeestimated',
        1 => 'field_uwwboddischargecalculated',
        2 => 'field_uwwcoddischargecalculated',
        3 => 'field_uwwnincomingestimated',
        4 => 'field_uwwpincoming',
        5 => 'field_uwwndischargecalculated',
        6 => 'field_uwwboddischarge',
        7 => 'field_uwwpincomingcalculated',
        8 => 'field_uwwbodincomingcalculated',
        9 => 'field_uwwbodincoming',
        10 => 'field_uwwcodincomingcalculated',
        11 => 'field_uwwndischarge',
        12 => 'field_uwwnincoming',
        13 => 'field_uwwbodincomingestimated',
        14 => 'field_uwwpincomingestimated',
        15 => 'field_uwwcoddischargeestimated',
        16 => 'field_uwwcodincoming',
        17 => 'field_uwwpdischargeestimated',
        18 => 'field_uwwpdischarge',
        19 => 'field_uwwnincomingcalculated',
        20 => 'field_uwwcodincomingestimated',
        21 => 'field_uwwcoddischarge',
        22 => 'field_uwwndischargeestimated',
        23 => 'field_uwwpdischargecalculated',
      ),
       'format_type' => 'fieldset',
       'format_settings' => 
      array (
        'formatter' => 'collapsible',
        'instance_settings' => 
        array (
          'description' => '',
          'classes' => 'group-uwwtp-incom-disch field-group-fieldset',
          'required_fields' => 1,
          'id' => '',
        ),
      ),
    )),
    'group_uwwpt_system' => 
    (object)(array(
       'id' => '52',
       'identifier' => 'group_uwwpt_system|node|uwwtp|form',
       'group_name' => 'group_uwwpt_system',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => 'group_uwwtp_admin',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'System',
       'weight' => '4',
       'children' => 
      array (
        0 => 'field_sourcefile',
        1 => 'field_sensitiveareatype',
        2 => 'field_repcode',
        3 => 'field_anneedata',
        4 => 'field_siteid',
      ),
       'format_type' => 'tab',
       'format_settings' => 
      array (
        'formatter' => 'closed',
        'instance_settings' => 
        array (
          'description' => '',
          'classes' => 'group-uwwpt-system field-group-tab',
          'required_fields' => 1,
        ),
      ),
    )),
    'group_uwwtp_deprecated' => 
    (object)(array(
       'id' => '48',
       'identifier' => 'group_uwwtp_deprecated|node|uwwtp|form',
       'group_name' => 'group_uwwtp_deprecated',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => 'group_uwwtp_admin',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'Deprecated fields',
       'weight' => '5',
       'children' => 
      array (
        0 => 'field_uwww_required_rcaan',
        1 => 'field_uwww_required_rcaap',
        2 => 'field_thematicid',
        3 => 'field_uwwtdaggid',
        4 => 'field_uwww_required_rcac',
        5 => 'field_validfrom',
        6 => 'field_facilitydescription',
        7 => 'field_servicehours',
        8 => 'field_uwwdateexported',
        9 => 'field_type',
        10 => 'field_uwwwatertreatment',
        11 => 'field_uwww_required_rcab',
        12 => 'field_functionactivity',
        13 => 'field_inspireidnamespace',
        14 => 'field_inspireidversionid',
        15 => 'field_physicalcapacityoutput',
        16 => 'field_functiondescription',
        17 => 'field_functionoutput',
        18 => 'field_uwwpinhabitantsconnected',
        19 => 'field_permission',
        20 => 'field_functioninput',
        21 => 'field_endlifespanversion',
        22 => 'field_uwwsludgetreatment',
        23 => 'field_physicalcapacitydescriptio',
        24 => 'field_physicalcapacitytime',
        25 => 'field_beginlifespanversion',
      ),
       'format_type' => 'tab',
       'format_settings' => 
      array (
        'formatter' => 'closed',
        'instance_settings' => 
        array (
          'description' => '',
          'classes' => 'group-uwwtp-deprecated field-group-tab',
          'required_fields' => 1,
        ),
      ),
    )),
    'group_uuwwtp_perf_equip' => 
    (object)(array(
       'id' => '55',
       'identifier' => 'group_uuwwtp_perf_equip|node|uwwtp|form',
       'group_name' => 'group_uuwwtp_perf_equip',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => 'group_uwwtp_description',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'Perf & equipment',
       'weight' => '22',
       'children' => 
      array (
        0 => 'field_uwwbaddesign',
        1 => 'field_uwwntotperf',
        2 => 'field_uwwptotperf',
        3 => 'field_uwwozonation',
        4 => 'field_uwwotherperf',
        5 => 'field_uwwspecification',
        6 => 'field_uwwbadperfdesign',
        7 => 'field_uwwsecondarytreatment',
        8 => 'field_uwwother',
        9 => 'field_uwwtssperf',
        10 => 'field_uwwmicrofiltration',
        11 => 'field_uwwaccidents',
        12 => 'field_uwwpremoval',
        13 => 'field_uwwcodperf',
        14 => 'field_uwwnremoval',
        15 => 'field_uwwinformation',
        16 => 'field_uwwuv',
        17 => 'field_uwwprimarytreatment',
        18 => 'field_uwwothertreat',
        19 => 'field_uwwsandfiltration',
        20 => 'field_uwwbod5perf',
        21 => 'field_uwwchlorination',
      ),
       'format_type' => 'fieldset',
       'format_settings' => 
      array (
        'formatter' => 'collapsible',
        'instance_settings' => 
        array (
          'description' => '',
          'classes' => 'group-uuwwtp-perf-equip field-group-fieldset',
          'required_fields' => 1,
          'id' => '',
        ),
      ),
    )),
    'group_uwwtp_compliance' => 
    (object)(array(
       'id' => '51',
       'identifier' => 'group_uwwtp_compliance|node|uwwtp|form',
       'group_name' => 'group_uwwtp_compliance',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => 'group_uwwtp_admin',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'Compliance',
       'weight' => '2',
       'children' => 
      array (
        0 => 'field_uwwperformance_met',
        1 => 'field_uwwtreatmentrequired',
        2 => 'field_uwwcompliance',
        3 => 'field_uwwtreatmenttype',
        4 => 'field_compliance_explication',
        5 => 'field_uwwtreatment_met',
      ),
       'format_type' => 'tab',
       'format_settings' => 
      array (
        'formatter' => 'closed',
        'instance_settings' => 
        array (
          'description' => '',
          'classes' => 'group-uwwtp-compliance field-group-tab',
          'required_fields' => 1,
        ),
      ),
    )),
    'group_uwwtp_description' => 
    (object)(array(
       'id' => '53',
       'identifier' => 'group_uwwtp_description|node|uwwtp|form',
       'group_name' => 'group_uwwtp_description',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => 'group_uwwtp_admin',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'Description',
       'weight' => '1',
       'children' => 
      array (
        0 => 'field_uwwwastewatertreated',
        1 => 'field_uwwbeginlife',
        2 => 'field_inspireidlocalid',
        3 => 'field_uwwperctruck',
        4 => 'field_uwwloadenteringuwwtp',
        5 => 'field_uwwnathyperlink',
        6 => 'field_uwwhistorie',
        7 => 'field_uwwmethodwastewatertreated',
        8 => 'field_uwwsuccessor',
        9 => 'field_validto',
        10 => 'field_uwwloadcalculation',
        11 => 'field_uwwlongitude',
        12 => 'field_uwwendlife',
        13 => 'field_uwwremarks',
        14 => 'field_position_geo',
        15 => 'field_uwwcollectingsystem',
        16 => 'field_physicalcapacityactivity',
        17 => 'field_uwwlatitude',
        18 => 'field_status',
        19 => 'field_physicalcapacityinput',
        20 => 'title',
        21 => 'group_uwwtp_incom_disch',
        22 => 'group_uuwwtp_perf_equip',
      ),
       'format_type' => 'tab',
       'format_settings' => 
      array (
        'formatter' => 'closed',
        'instance_settings' => 
        array (
          'description' => '',
          'classes' => 'group-uwwtp-description field-group-tab',
          'required_fields' => 1,
        ),
      ),
    )),
    'group_uwwtp_links' => 
    (object)(array(
       'id' => '49',
       'identifier' => 'group_uwwtp_links|node|uwwtp|form',
       'group_name' => 'group_uwwtp_links',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => 'group_uwwtp_admin',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'Links',
       'weight' => '3',
       'children' => 
      array (
        0 => 'field_uwwdcpliste',
        1 => 'field_linked_agglomerations',
        2 => 'field_linked_receiving_areas',
        3 => 'field_sensitivearealink',
        4 => 'field_linked_discharge_points',
        5 => 'field_article17_uwwtp',
        6 => 'field_uwwe_prtrcode',
        7 => 'field_uwwaggliste',
        8 => 'field_regionnuts',
      ),
       'format_type' => 'tab',
       'format_settings' => 
      array (
        'formatter' => 'closed',
        'instance_settings' => 
        array (
          'description' => '',
          'classes' => 'group-uwwtp-links field-group-tab',
          'required_fields' => 1,
        ),
      ),
    )),
    'group_uwwtp_admin' => 
    (object)(array(
       'id' => '50',
       'identifier' => 'group_uwwtp_admin|node|uwwtp|form',
       'group_name' => 'group_uwwtp_admin',
       'entity_type' => 'node',
       'bundle' => 'uwwtp',
       'mode' => 'form',
       'parent_name' => '',
       'table' => 'field_group',
       'type' => 'Normal',
       'export_type' => 1,
       'disabled' => false,
       'label' => 'UWWTP',
       'weight' => '0',
       'children' => 
      array (
        0 => 'group_uwwpt_system',
        1 => 'group_uwwtp_deprecated',
        2 => 'group_uwwtp_compliance',
        3 => 'group_uwwtp_description',
        4 => 'group_uwwtp_links',
      ),
       'format_type' => 'tabs',
       'format_settings' => 
      array (
        'formatter' => '',
        'instance_settings' => 
        array (
          'classes' => 'group-uwwtp-admin field-group-tabs',
          'id' => '',
        ),
      ),
    )),
  ),
);