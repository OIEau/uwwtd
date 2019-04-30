<?php

if ( !defined('UWWTD_CACHE_ACTIVE')) {
    define('UWWTD_CACHE_ACTIVE', true);
}
if ( !defined('DEBUG_ALGO')) {
    define('DEBUG_ALGO', true);    
}

// $GLOBALS['uwwtd']['message']['boolean_error']
// $GLOBALS['uwwtd']['message']['boolean_error']
$GLOBALS['uwwtd']['message'] = array(
    'other_treatment_error' => 'More stringents detected while field set to false',
     'boolean_error' => 'The input data is not in boolean format (true or false).',
     'numeric_error' => 'The input data is not in numeric format.',
     'decimal_error' => 'The input data is not in numeric or decimal format.',
     'date_empty_error' => 'Date found to be empty.',
     'date_format_error' => 'Unknown date format: %',
     'date_exist_error' => 'Entered date doesn\'t exist: %',
     'coord_decimal_error' => 'Decimal point for this coordinate was inproperly placed.',
     'coord_latitude_europe_error' => 'Latitude not found to be in Europe.',
     'coord_longitude_europe_error' => 'Longitude not found to be in Europe.',
     'percent_interval_error' => 'The number must be between 0 and 100.',
     'percent_not_digit_error' => 'The input data is not in digital format.',    
     'total_load_entering_uwwtd_null' => 'The total all load entering is equal 0.',    
);

$GLOBALS['uwwtd_errors'] = array();
// 0|Input
// 1|Linking
// 2|Geometry
// 3|Conformity
// 4|Format     
//$GLOBALS['uwwtd_errors_type']['category']['format']
//$GLOBALS['uwwtd_errors_type']['category']['format']
//$GLOBALS['uwwtd_errors_type']['category']['format']
//$GLOBALS['uwwtd_errors_type']['category']['format']
//$GLOBALS['uwwtd_errors_type']['category']['import data format']
//if a new value is added here, you need to add the value in the drupal view error_list
$GLOBALS['uwwtd_errors_type']['category'] = array(
    'import data input' => 0,
    'import data linking' => 1,
    'import data geometry' => 2,
    'conformity data' => 3,
    'import data format' => 4,
    'shape format' => 5,
    'article17 format' => 6,   //ok
    'article17 input' => 7,   //ok
);


// article 17
// uwwtd_add_error(2, 4,       article17 format
// 
// uwwtd_add_error($GLOBALS['uwwtd_errors_type']['type']['warning format'], $GLOBALS['uwwtd_errors_type']['category']['article17 format'],
// 
// utils
// uwwtd_add_error(2, 4,
// uwwtd_add_error(1, 4,
// 
// data_center
//     import
//     import shape
//     conformity
    
// 0|Notification
// 1|Warning
// 2|Error
//$GLOBALS['uwwtd_errors_type']['type']['error']
//if a new value is added here, you need to add the value in the drupal view error_list
$GLOBALS['uwwtd_errors_type']['type'] = array(
    'notification' => 0,
    'warning' => 1,
    'error' => 2,          
);

$GLOBALS['uwwtd_value']['treatementmet'] = array(
    'false' => 0,
    'true' => 1,
    'not_calculable' => 2,
);
$GLOBALS['uwwtd_value']['performancemet'] = array(
    'false' => 0,
    'true' => 1,
    'not_calculable' => 2,
);
//$GLOBALS['uwwtd_value']['compliance']['not_relevant']
//$GLOBALS['uwwtd_value']['compliance']['compliant']
//$GLOBALS['uwwtd_value']['compliance']['compliantQC']
//$GLOBALS['uwwtd_value']['compliance']['not_compliantQC']
$GLOBALS['uwwtd_value']['compliance'] = array(
    'not_compliantQC'  => 'AddQC', //AddQC|Not compliant  (art3comp)
    'compliant'      => 'C',    //C|Compliant       (agglo	art3comp	art4com	art5com	art6com	uwwtd)  
    'not_compliant'  => 'NC',  //NC|Not compliant   (agglo	art3comp	art4com	art5com	art6com	uwwtd)  
    'no_information' => 'NI',  //NI|No information  (agglo	art3comp	art4com	art5com	art6com	uwwtd)  
    'not_relevant'   => 'NR',   //NR|Not relevant  (agglo	art3comp	art4com	art5com	art6com	uwwtd)   
    'not_compliantR'  => 'RNC', //RNC|Not compliant    (art4com, art6com)
    'compliantQC'      => 'QC', //QC|Compliant     (art3comp)     
    'compliant_on_equipment_only' => 'CE', //CE|Compliant on equipment only   (uwwtd)
    'unknown'       => '?',  //(art4,5,6, uwwtp)
    'pending_deadline'  => 'PD',
);
//TODO add label for each value
$GLOBALS['uwwtd_value']['rcatype'] = array(
    'catchment_of_sensitive_area' => 'CSA',
    'less_sensitive_area' => 'LSA', //Less Sensitive Area
    'normal_area' => 'NA',
    'sensitive_area' => 'SA',
    '?' => 'A58',
);

$GLOBALS['uwwtd_value']['rcacat_labels']=array(
    'All' => t('- Any -'),
    'ES' => t('Estuary'),
    'CW' => t('Coastal waters'),
    'FW' => t('Freshwater'),
);

$GLOBALS['uwwtd_value']['rca_spe_type_labels']=array(
    'All' => t('- Any -'),
    'A523'=>t('Art 5.(2,3) area'),
    'A54'=>t('Art. 5(4) area'),
    'A54523'=>t('Art. 5(4) and 5 (2,3) area'),
    'A5854'=>t('Art. 5(8) and 5(4) area'),
    'A58523'=>t('Art. 5(8) and 5 (2,3) area'),
    'A5854523'=>t('Art. 5(8), 5(4) and 5 (2,3) area'),
);

$GLOBALS['uwwtd_value']['rcatype_labels']=array(
    'All' => t('- Any -'),
    'UWWCMSA' => t("Catchment of Sensitive area"),
    'UWW55CMSA' => t("Catchment of Sensitive area defined in the sense of Article 5.5 of the UWWTD"),
    'UWWCASA' => t("Coast area Sensitive area"),
    'UWWCLSA' => t("Coast line Sensitive area"),
    'UWWLKSA'  => t("Lake Sensitive area"),
    'UWWRISA' => t("River Sensitive area"),
    'UWWTWSA' => t("Transitional water Sensitive area"),
    'UWWTDLSA' => t("Less Sensitive Area"),
    
);
$GLOBALS['uwwtd_value']['rcatype_color']=array(
    'All' => array(
      'pointRadius' => 6,
      'fillColor' => "#aaaaaa",
      'strokeColor' => '#000000',
      'strokeWidth' => 1,
      'fillOpacity' => 0.9,
      'strokeOpacity' => 0.9,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWCMSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#ff0000",
      'strokeColor' => "#ff0000",
      'strokeWidth' => 3,
      'fillOpacity' => 0.8,
      'strokeOpacity' => 1,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWW55CMSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#800080",
      'strokeColor' => '#800080',
      'strokeWidth' => 1,
      'fillOpacity' => 0.4,
      'strokeOpacity' => 0.8,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWCASA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#ffdead",
      'strokeColor' => "#ffdead",
      'strokeWidth' => 1,
      'fillOpacity' => 0.6,
      'strokeOpacity' => 0.8,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWCLSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#4682b4",
      'strokeColor' => '#000000',
      'strokeWidth' => 1,
      'fillOpacity' => 0.6,
      'strokeOpacity' => 0.8,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWLKSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#ffff00",
      'strokeColor' => '#000000',
      'strokeWidth' => 1,
      'fillOpacity' => 0.9,
      'strokeOpacity' => 0.9,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWRISA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#9acd32",
      'strokeColor' => '#000000',
      'strokeWidth' => 1,
      'fillOpacity' => 0.9,
      'strokeOpacity' => 0.9,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWTWSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#4169e1",
      'strokeColor' => '#000000',
      'strokeWidth' => 1,
      'fillOpacity' => 0.9,
      'strokeOpacity' => 0.9,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWTDLSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#8a2be2",
      'strokeColor' => '#000000',
      'strokeWidth' => 1,
      'fillOpacity' => 0.9,
      'strokeOpacity' => 0.9,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    )
);


//$GLOBALS['uwwtd_value']['required']['appropriate']
//$GLOBALS['uwwtd_value']['required']['notrelevant']
//$GLOBALS['uwwtd_value']['required']['primary']
//$GLOBALS['uwwtd_value']['required']['secondary']
$GLOBALS['uwwtd_value']['required'] = array(
    'primary' => 'P',
    'secondary' => 'S',
    'appropriate' => 'Appropriate',
    'nitrogen' => 'N',
    'phosphorus' => 'MP',
    'other' => 'O',
    'nitrogen_phosphorus' => 'NP',
    'notrelevant' => 'NR',
    'morestringent' => 'MS',
    'nitrogen_phosphorus_other' => 'NPO',
    'nitrogen_other' => 'NO',
    'phosphorus_other' => 'PO',    
); 

//$GLOBALS['uwwtd_value']['inplace']['noinformation']
// $GLOBALS['uwwtd_value']['inplace']['primary']
// $GLOBALS['uwwtd_value']['inplace']['secondary']
// $GLOBALS['uwwtd_value']['inplace']['morestringent']
$GLOBALS['uwwtd_value']['inplace'] = array(
    'primary' => 'P',
    'secondary' => 'S',
    'appropriate' => 'Appropriate',
    'nitrogen' => 'N',
    'phosphorus' => 'MP',
    'other' => 'O',
    'nitrogen_phosphorus' => 'NP',
    'notrelevant' => 'NR',
    'morestringent' => 'MS',
    'noinformation' => 'NI',    
);

$GLOBALS['uwwtd']['uwwtd_years'] = range(2000, date('Y') + 5);
$GLOBALS['uwwtd']['uwwtd_years'] = array_combine($GLOBALS['uwwtd']['uwwtd_years'], $GLOBALS['uwwtd']['uwwtd_years']);

$GLOBALS['uwwtd']['ui']['operator'] = array(
    '<' => t('Is less than'),
    '=' => t('Is equal to'),
    '!=' => t('Is not equal to'),
    '>=' => t('Is greater than or equal to'),
    '>' => t('Is greater than'),
);

$GLOBALS['uwwtd']['ui']['operatorfunction'] = array(
    '<' => function ($a, $b) {return $a < $b;},      //'uwwtd_is_less_than',
    '<=' => function ($a, $b) {return $a <= $b;},     //'uwwtd_is_less_than_or_equal_to',
    '=' => function ($a, $b) {return $a == $b;},    // 'uwwtd_is_equal_to',
    '!=' => function ($a, $b) {return $a != $b;},     //'uwwtd_is_not_equal_to',
    '>=' => function ($a, $b) {return $a >= $b;},     //'uwwtd_is greater_than_or_equal_to',
    '>' => function ($a, $b) {return $a > $b;},     //'uwwtd_is_greater_than',
);

$GLOBALS['uwwtd']['ui']['sort'] = array(
    'asc' => function ($a, $b) {
        if ($a[$GLOBALS['uwwtd']['ui']['sortfield']] == $b[$GLOBALS['uwwtd']['ui']['sortfield']]) { return 0; } 
        if($GLOBALS['uwwtd']['ui']['sortmod']=='numeric') return ((float)$a[$GLOBALS['uwwtd']['ui']['sortfield']] < (float)$b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? -1 : 1;
        elseif($GLOBALS['uwwtd']['ui']['sortmod']=='text') return ((string)$a[$GLOBALS['uwwtd']['ui']['sortfield']] < (string)$b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? -1 : 1;
        else return ($a[$GLOBALS['uwwtd']['ui']['sortfield']] < $b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? -1 : 1;
        
    },
    'desc' => function ($a, $b) {
        if ($a[$GLOBALS['uwwtd']['ui']['sortfield']] == $b[$GLOBALS['uwwtd']['ui']['sortfield']]) { return 0; } 
        if($GLOBALS['uwwtd']['ui']['sortmod']=='numeric') return ((float)$a[$GLOBALS['uwwtd']['ui']['sortfield']] < (float)$b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? 1 : -1;
        elseif($GLOBALS['uwwtd']['ui']['sortmod']=='text') return ((string)$a[$GLOBALS['uwwtd']['ui']['sortfield']] < (string)$b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? 1 : -1; 
        else return ($a[$GLOBALS['uwwtd']['ui']['sortfield']] < $b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? 1 : -1;
    },
);


$GLOBALS['uwwtd']['ui']['field']['agglomeration'] = array(
    'title'                                  =>  array('label'=>t('Name')),
    'field_inspireidlocalid_value'           =>  array('label'=>t('Identifier Code')),
    'field_agggenerated_value_format'        =>  array('label'=>t('Generated Load (p.e.)'), 'sort_field'=>'field_agggenerated_value', 'sort_mod'=>'numeric'),
    'sum_uwwtp_format'                    =>  array('label'=>t('Total UWWTP capacity (p.e.)'), 'sort_field'=>'sum_uwwtp', 'sort_mod'=>'numeric'),
    'n3nm'                                   =>  array('label'=>t('Region (NUTS)')),
    'rbd'                                    =>  array('label'=>t('RBD')),
    'field_aggcompliance_value_format'       =>  array('label'=>t('Global compliance'), 'sort_field'=>'field_aggcompliance_value'),
	'field_aggart3compliance_value_format'   =>  array('label'=>t('Collection compliance'), 'sort_field'=>'field_aggart3compliance_value'),
    'field_aggart4compliance_value_format'   =>  array('label'=>t('2nd treatment compliance'), 'sort_field'=>'field_aggart4compliance_value'),
    'field_aggart5compliance_value_format'   =>  array('label'=>t('3rd treatment compliance'), 'sort_field'=>'field_aggart5compliance_value'),
    'field_aggart6compliance_value_format'   =>  array('label'=>t('Article 6 compliance'), 'sort_field'=>'field_aggart6compliance_value'),
	'field_article17_agglo_value_format'     =>  array('label'=>t('Article 17'), 'sort_field'=>'field_article17_agglo_value'), 
);
$GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment'] = array(
    'title'                               =>  array('label'=>t('Name')),
    'field_inspireidlocalid_value'        =>  array('label'=>t('Identifier Code')),
    'field_agggenerated_value_format'            =>  array('label'=>t('Generated Load (p.e.)'), 'sort_field'=>'field_agggenerated_value', 'sort_mod'=>'numeric'),
    'sum_uwwtp_format'                    =>  array('label'=>t('Total UWWTP capacity (p.e.)'), 'sort_field'=>'sum_uwwtp', 'sort_mod'=>'numeric'),
    'n3nm'                                =>  array('label'=>t('Region (NUTS)')),
    'rbd'                                 =>  array('label'=>t('RBD')),
    'field_aggc1_value_format'                   =>  array('label'=>t('Collecting system (% of p.e.) '), 'sort_field'=>'field_aggc1_value', 'sort_mod'=>'numeric'),
    'field_aggc2_value_format'                   =>  array('label'=>t('IAS (% of p.e.)'), 'sort_field'=>'field_aggc2_value', 'sort_mod'=>'numeric'),
    'field_aggpercwithouttreatment_value_format' =>  array('label'=>t('Discharge without treatment (% of p.e.)'), 'sort_field'=>'field_aggpercwithouttreatment_value', 'sort_mod'=>'numeric'),
	'field_article17_agglo_value_format'         =>  array('label'=>t('Article 17'), 'sort_field'=>'field_article17_agglo_value'), 
);


$GLOBALS['uwwtd']['ui']['field']['uwwtp'] = array(
    'title'                           =>  array('label'=>t('Name')),
    'field_inspireidlocalid_value'    =>  array('label'=>t('Identifier Code')),
    'field_uwwloadenteringuwwtp_value_format'        =>  array('label'=>t('Load entering (p.e.)'), 'sort_field'=>'field_uwwloadenteringuwwtp_value', 'sort_mod'=>'numeric'),
	'field_physicalcapacityactivity_value_format'    =>  array('label'=>t('Physical Capacity (p.e.)'), 'sort_field'=>'field_physicalcapacityactivity_value', 'sort_mod'=>'numeric'),
    'n3nm'          =>  array('label'=>t('Region (NUTS)')),
    'rbd'          =>  array('label'=>t('RBD')),
    'field_uwwtreatmenttype_value_format'   =>  array('label'=>t('Treatment in place')),
    'field_uwwcompliance_value_format'       =>  array('label'=>t('Global compliance')),
    'field_uwwbod5perf_value_format'   =>  array('label'=>t('BOD5'), 'sort_field'=>'field_uwwbod5perf_value'),
    'field_uwwcodperf_value_format'   =>  array('label'=>t('COD'), 'sort_field'=>'field_uwwcodperf_value'),
    'field_uwwntotperf_value_format'   =>  array('label'=>t('Total nitrogen'), 'sort_field'=>'field_uwwntotperf_value'),
    'field_uwwptotperf_value_format'   =>  array('label'=>t('Total phosphorus'), 'sort_field'=>'field_uwwptotperf_value'),    
	'field_article17_uwwtp_value_format'   =>  array('label'=>t('Article 17'), 'sort_field'=>'field_article17_uwwtp_value'),
);

$GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment'] = array(
    'title'                             =>  array('label'=>t('Name')),
    'field_inspireidlocalid_value'      =>  array('label'=>t('Identifier Code')),
    'field_uwwloadenteringuwwtp_value_format'  =>  array('label'=>t('Load entering (p.e.)'), 'sort_field'=>'field_uwwloadenteringuwwtp_value', 'sort_mod'=>'numeric'),
	'field_physicalcapacityactivity_value_format'    =>  array('label'=>t('Physical Capacity (p.e.)'), 'sort_field'=>'field_physicalcapacityactivity_value', 'sort_mod'=>'numeric'),
    'n3nm'            =>  array('label'=>t('Region (NUTS)')),
    'rbd'          =>  array('label'=>t('RBD')),
    'field_uwwprimarytreatment_value_format'   =>  array('label'=>t('Primary treatment'), 'sort_field'=>'field_uwwprimarytreatment_value'),
    'field_uwwsecondarytreatment_value_format' =>  array('label'=>t('Secondary treatment'), 'sort_field'=>'field_uwwsecondarytreatment_value'),
    'field_uwwnremoval_value_format'           =>  array('label'=>t('N removal'), 'sort_field'=>'field_uwwnremoval_value'),
    'field_uwwpremoval_value_format'           =>  array('label'=>t('P removal'), 'sort_field'=>'field_uwwpremoval_value'),
    'field_uwwspecification_value'      =>  array('label'=>t('Other more stringent treatment')),
	'field_article17_uwwtp_value_format'   =>  array('label'=>t('Article 17'), 'sort_field'=>'field_article17_uwwtp_value'),
);


$GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure'] = array(
    'title'                             =>  array('label'=>t('Name')),
    'field_inspireidlocalid_value'      =>  array('label'=>t('Identifier Code')),
    'field_uwwloadenteringuwwtp_value_format'  =>  array('label'=>t('Load entering (p.e.)'), 'sort_field'=>'field_uwwloadenteringuwwtp_value', 'sort_mod'=>'numeric'),
	'field_physicalcapacityactivity_value_format'    =>  array('label'=>t('Physical Capacity (p.e.)'), 'sort_field'=>'field_physicalcapacityactivity_value', 'sort_mod'=>'numeric'),
    'n3nm'            =>  array('label'=>t('Region (NUTS)')),
    'rbd'          =>  array('label'=>t('RBD')),
    'field_uwwboddischarge_value_format'      =>  array('label'=>t('Discharged load BOD (t per year)'), 'sort_field'=>'field_uwwboddischarge_value', 'sort_mod'=>'numeric'),
    'field_uwwcoddischarge_value_format'      =>  array('label'=>t('Discharged load COD (t per year)'), 'sort_field'=>'field_uwwcoddischarge_value', 'sort_mod'=>'numeric'),
    'field_uwwndischarge_value_format'      =>  array('label'=>t('Discharged load Ntot (t per year)'), 'sort_field'=>'field_uwwndischarge_value', 'sort_mod'=>'numeric'),
    'field_uwwpdischarge_value_format'      =>  array('label'=>t('Discharged load Ptot (t per year)'), 'sort_field'=>'field_uwwpdischarge_value', 'sort_mod'=>'numeric'),
	'field_article17_uwwtp_value_format'   =>  array('label'=>t('Article 17'), 'sort_field'=>'field_article17_uwwtp_value'),
);
$GLOBALS['uwwtd']['ui']['field']['rca'] = array(
    'title'                           =>  array('label'=>t('Name')),
    'field_inspireidlocalid_value'    =>  array('label'=>t('Identifier Code')),
    'zonetype_format'=>array('label'=>t('Type of area')),
    'spezonetype_format'=>array('label'=>t('Sub-type of receiving area ')),
    'rca52_format'=>array('label'=>t('Article 52 applied')),
    'rca54_format'=>array('label'=>t('Article 54 applied')),
    'rca58_format'=>array('label'=>t('Article 58 applied')),
    'aggs'=>array('label'=>t('Number of agglomerations')),
    'uwwtps'=>array('label'=>t('Number of UWWTP')),
    'tot_design_capacity_format'=>array('label'=>t('Total physical Capacity (p.e.)'), 'sort_field'=>'tot_design_capacity', 'sort_mod'=>'numeric'),
    'tot_entering_load_format'=>array('label'=>t('Total load entering (p.e.)'), 'sort_field'=>'tot_entering_load', 'sort_mod'=>'numeric'),
    'starting_dates_application'=>array('label'=>t('Starting dates of application')),
);
$GLOBALS['uwwtd']['ui']['field']['dcp'] = array(
    'title'                           =>  array('label'=>t('Name')),
    'field_inspireidlocalid_value'    =>  array('label'=>t('Identifier Code')),
    'rca_format'=>array('label'=>t('Linked receiving areas')),
    'zonetype_format'=>array('label'=>t('Type of area')),
    'uwwtp_format'=>array('label'=>t('Linked treatment plant')),
    'field_uwwcompliance_value_format'=>array('label'=>t('Compliance of treatment plant')),
    'rcacat_format'=>array('label'=>t('Type of receiving area')),
    'field_dcpirrigation_value'=>array('label'=>t('Type of reuse')),
     'field_uwwwastewatertreated_value'=>array('label'=>t('Volume (m3/y)')),
    
);

$GLOBALS['uwwtd']['ui']['compliance_connection'] = array(
    'PD' => 'NR',
    'QC' => 'C',
    'C' => 'C',
    'NC' => 'NC',
);

$GLOBALS['uwwtd']['all_compliance']  = array(
    'C' => t('Compliant'),        //comprend aussi QC
    'QC' => t('Questionable Compliance'),
    'PD' => t('Pending Deadline'),
    'NC' => t('Not Compliant'),
    'NR' => t('Not relevant'),     //comprend aussi PD
    'NI' => t('No information'),
    '?' => t('?'),
);

$GLOBALS['uwwtd']['compliance_color']  = array(
    'NI' => '#6b6b6b',
    'NR' => '#a2a2a2',
    'NC' => '#d93c3c',
    'C' => '#4f91e1',
    '?' => '#ea8b2e',
    'PD' => '#8fbc8f',
    'QC' => '#e9967a',
);
$GLOBALS['uwwtd']['treatment_type_color']  = array(
    'P' => '#ea8b2e',
    'S' => '#3cc53c',
    'MS' => '#5ddfff',
    'NI' => '#d93c3c',
);
$GLOBALS['uwwtd']['uwwtp_size_color']  = array(
    '< 10,000 pe' => '#005572',
    '10,000 to 100,000 pe' => '#008B8D',//#006573
    '> 100,000 pe' => '#81BEAA',
    //#81BEAA
    //#F4D4AD
);
$GLOBALS['uwwtd']['reuse_type']=array(
    'IR'=>'Irrigation',
    'IN'=>'Industry',
    'OT'=>'Others'
);
$GLOBALS['uwwtd']['reuse_type_color']=array(
    'IR'=>'#459436',
    'IN'=>'#334D5C',
    'OT'=>'#02658C'
);

$GLOBALS['uwwtd']['ui']['compliance'] = array(
    'All' => '- Any -',
    'NI' => $GLOBALS['uwwtd']['all_compliance']['NI'],
    'NR' => $GLOBALS['uwwtd']['all_compliance']['NR'],     //comprend aussi PD
    'NC' => $GLOBALS['uwwtd']['all_compliance']['NC'],
    'C' => $GLOBALS['uwwtd']['all_compliance']['C'],        //comprend aussi QC
    '?' => $GLOBALS['uwwtd']['all_compliance']['?'],
//     'PD' => 'Pending Deadline',
//     'QC' => 'Questionable Compliance',
);

//todo delete this one if all works
$GLOBALS['uwwtd']['ui']['field']['compliance_label'] = $GLOBALS['uwwtd']['all_compliance'];

$GLOBALS['uwwtd']['ui']['test_pass'] = array(
    'All' => t('- Any -'),
    'P' => t('Pass'),
    'F' => t('Fail'),
    'NR' => t('Not relevant'),
);

$GLOBALS['uwwtd']['ui']['yes_no'] = array(
    'All' => t('- Any -'),
    '1' => t('Yes'),
    '0' => t('No'),
);


$GLOBALS['uwwtd']['ui']['treatment_type_in_place'] = array(
    'All' => t('- Any -'),
    'NI' => t('No information'),
    'P' => t('Primary treatment'),
    'S' => t('Secondary treatment'),
    'MS' => t('More stringent treatment'),
);

$GLOBALS['ms_level'] = array(
    'sludge'=>array(
        'dcp'=>array('color'=>'#B5B5BF','label'=>t('Discharged into surface waters : Pipelines')),
        'dcs'=>array('color'=>'#686872','label'=>t('Discharged into surface waters : Ships')),
        'dco'=>array('color'=>'#2D2D37','label'=>t('Discharged into surface waters : Others')),
        'rua'=>array('color'=>'#459436','label'=>t('Re-used: Soil and agriculture')),
        'ruo'=>array('color'=>'#8EC547','label'=>t('Re-used: Others')),
        'dil'=>array('color'=>'#5B3349','label'=>t('Disposed: Landfill')),
        'dii'=>array('color'=>'#F28030','label'=>t('Disposed: Incineration')),
        'dio'=>array('color'=>'#F29F80','label'=> t('Disposed: Others')),
        'missing'=>array('color'=>'#E74C3C','label'=> t('Missing')),
    ),
    'waste_water'=>array(
        'agriculture'=>array('color'=>'#459436','label'=>t('Agriculture')),
        'industry'=>array('color'=>'#334D5C','label'=>t('Industry')),
        'others'=>array('color'=>'#02658C','label'=>t('Others'))
    ),
	'connection'=>array(
		'cs'=>array('color'=>'#00658e','label'=>t('Collected by collecting system')),
        'ias'=>array('color'=>'#00ced1','label'=>t('Individual and Appropriate Systems')),
        'wot'=>array('color'=>'#d93c3c','label'=>t('Discharged without treatment'))
	),
	'sewage_network'=>array(
		'B'=>array('color'=>'#459436','label'=>t('Separative and combine system')),
        'S'=>array('color'=>'#4f91e1','label'=>t('Separative system')),
        'C'=>array('color'=>'#bd8842','label'=>t('Combine system')),
		'NI'=>array('color'=>'#a2a2a2','label'=>t('No information')),
	)
);


// Article 17 glossary
$GLOBALS['article17']['glossary'] = array( 
  "CU" => "current",
  "EXP" => "expected",
  "P" => "past"
);

$GLOBALS['report_number'] = array(
  2010 => '7th',
  2012 => '8th',                          
  2014 => '9th',
  2016 => '10th',
  2018 => '11th', 
  2020 => '12th',                           
);

// Global glossary
$GLOBALS['glossary'] = array(
'ES' =>            'Estuary',
'CW' =>            'Coastal waters',
'FW' =>            'Freshwater',
'SA' =>            'Sensitive Area',
'CSA' =>           'Catchment of Sensitive Area',
'LSA' =>           'Less Sensitive Area',
'NA' =>            'Normal Area',
'A54' =>           'A54 Sensitive Area',
'A58' =>           'A58 Sensitive Area',
// 'P' =>             'Primary',
// 'S' =>             'Secondary',
'appropriate' =>   'Appropriate',
'N' =>             'Nitrogen',
'MP' =>            'Phosphorus',
'O' =>             'Other',
'NP' =>            'Nitrogen & Phosphorus',
'NR' =>            'Not Relevant',
'MS' =>            'More Stringent',
'NPO' =>           'Nitrogen & Phosphorus & Other',
'NO' =>            'Nitrogen Other',
'PO' =>            'Phosphorus & Other',
'NI' =>            'No Information',
'C' =>             'Compliant',
'QC' =>            'Questionable Compliance',
'PD' =>            'Pending Deadline',
'NC' =>            'Not Compliant',
'NR' =>            'Not relevant ',
'NI' =>            'No information',
// 'P' =>             'Pass',
// 'F' =>             'Fail',
'NR' =>            'Not relevant',
'NI' =>            'No information',
'P' =>             'Primary treatment',
'S' =>             'Secondary treatment',
'MS' =>            'More stringent treatment',
// 'B' =>             'Separative and combine system',
// 'S' =>             'Separative system',
// 'C' =>             'Combine system',
// 'NI' =>            'No information',
'EXP' =>           'Expire deadline',
'PD' =>            'Pending dealine',
'EXP-PD' =>        'Expire deadline - Pending dealine',
'IAS' =>           'Individual & appropriated system',
'1' => 'Primary treatment',
'2' => 'Secondary treatment',
'3other' => 'More stringent treatment : Nitrogen & phosphorus',
'3NP' => 'More stringent treatment: other (UV, Chlorination...)',
);


$GLOBALS['methods'] = [
    'C'=>t('calculated'),
    'E'=>t('estimated'),
    'M'=>t('measured'),
];
