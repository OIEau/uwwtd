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
      'fillColor' => "#daa520",
      'strokeColor' => "#000000",
      'strokeWidth' => 1,
      'fillOpacity' => 0.8,
      'strokeOpacity' => 1,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWCMSA_A54'=>array(
      'pointRadius' => 6,
      'fillColor' => "#a0522d",
      'strokeColor' => "#000000",
      'strokeWidth' => 1,
      'fillOpacity' => 0.8,
      'strokeOpacity' => 1,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWW55CMSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#ffa500",
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
      'fillColor' => "#191970",
      'strokeColor' => "#ffdead",
      'strokeWidth' => 1,
      'fillOpacity' => 0.6,
      'strokeOpacity' => 0.8,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWCASA_A54'=>array(
      'pointRadius' => 6,
      'fillColor' => "#191970",
      'strokeColor' => "#bdb76b",
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
      'fillColor' => "#ff00ff",
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
      'fillColor' => "#8fbc8f",
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
      'fillColor' => "#6495ed",
      'strokeColor' => '#6495ed',
      'strokeWidth' => 3,
      'fillOpacity' => 0.9,
      'strokeOpacity' => 0.9,
      'strokeLinecap' => 'round',
      'strokeDashstyle' => 'solid',
      'graphicOpacity' => 1,
      'labelAlign' => 'cm',
    ),
    'UWWTWSA'=>array(
      'pointRadius' => 6,
      'fillColor' => "#008b8b",
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
      'fillColor' => "#adff2f",
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
$GLOBALS['uwwtd_value']['conn'] = array(
    'ISCON' => t('Connected'),
    'NOTCON' => t('Not connected'),
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
    'IN'=>'Infiltration',
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
        'industry'=>array('color'=>'#334D5C','label'=>t('Infiltration')),
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

$GLOBALS['ocde_data']=[
    'AT'=>['bas'=>'1.822069044','water_supply'=>'0.430952454','waste_water'=>'4.022657967','sum'=>'6.275679465'],
    'BE'=>['bas'=>'1.452600469','water_supply'=>'1.263736812','waste_water'=>'5.287289152','sum'=>'8.003626433'],
    'BG'=>['bas'=>'0','water_supply'=>'0.362760206','waste_water'=>'5.075338233','sum'=>'5.438098439'],
    'HR'=>['bas'=>'0.040266676','water_supply'=>'0.362663095','waste_water'=>'1.734190372','sum'=>'2.137120144'],
    'CY'=>['bas'=>'0.133768887','water_supply'=>'0.023127016','waste_water'=>'0.445627728','sum'=>'0.602523631'],
    'CZ'=>['bas'=>'0.356201424','water_supply'=>'0.823582307','waste_water'=>'4.918287644','sum'=>'6.098071376'],
    'DK'=>['bas'=>'1.205497713','water_supply'=>'0.258779391','waste_water'=>'2.646366634','sum'=>'4.110643739'],
    'EE'=>['bas'=>'0.001730375','water_supply'=>'0.079125922','waste_water'=>'0.572907573','sum'=>'0.653763869'],
    'FI'=>['bas'=>'0.117620494','water_supply'=>'0.246152412','waste_water'=>'2.523568512','sum'=>'2.887341417'],
    'FR'=>['bas'=>'10.00575862','water_supply'=>'4.190502172','waste_water'=>'30.9578515','sum'=>'45.1541123'],
    'DE'=>['bas'=>'11.74941404','water_supply'=>'3.757259099','waste_water'=>'36.44307588','sum'=>'51.94974902'],
    'EL'=>	['bas'=>'0','water_supply'=>'0.437192286','waste_water'=>'4.610523647','sum'=>'5.047715934'],
    'HU'=>['bas'=>'0.325779979','water_supply'=>'0.312839369','waste_water'=>'4.283223607','sum'=>'4.921842956'],
    'IE'	=>['bas'=>'1.991589125','water_supply'=>'0.526432428','waste_water'=>'2.711916402','sum'=>'5.229937955'],
    'IT'	=>['bas'=>'2.272142967','water_supply'=>'4.848246052','waste_water'=>'34.2121212','sum'=>'41.33251022'],
    'LT'=>	['bas'=>'0','water_supply'=>'0.181497421','waste_water'=>'0.781147606','sum'=>'0.962645028'],
    'LU'=>['bas'=>'0','water_supply'=>'0.160963971','waste_water'=>'1.121900897','sum'=>'1.282864868'],
    'LV'=>['bas'=>'1.566673131','water_supply'=>'0.08142126','waste_water'=>'0.316408051','sum'=>'1.964502442'],
    'M'=>	['bas'=>'0.076057026','water_supply'=>'0.019640022','waste_water'=>'0.308272171','sum'=>'0.403969218'],
    'NL'=>['bas'=>'5.006340386','water_supply'=>'0.756035056','waste_water'=>'7.757505037','sum'=>'13.51988048'],
    'PL'=>	['bas'=>'0.295169908','water_supply'=>'2.845497414','waste_water'=>'18.62158781','sum'=>'21.76225513'],
    'PT'=>['bas'=>'0.331039697','water_supply'=>'0.373088167','waste_water'=>'4.761468635','sum'=>'5.465596499'],
    'RO'=>['bas'=>'0','water_supply'=>'6.469159752','waste_water'=>'15.13639704','sum'=>'21.60555679'],
    'SK'=>['bas'=>'0.081562979','water_supply'=>'0.671221525','waste_water'=>'2.583144532','sum'=>'3.335929036'],
    'SI'=>['bas'=>'0.150157914','water_supply'=>'0.172263058','waste_water'=>'1.020280392','sum'=>'1.342701364'],
    'ES'=>['bas'=>'1.618431322','water_supply'=>'2.306545812','waste_water'=>'24.86517218','sum'=>'28.79014931'],
    'SE'=>['bas'=>'1.716823735','water_supply'=>'0.5952282','waste_water'=>'4.773387905','sum'=>'7.08543984'],
    'UK'=>['bas'=>'14.58328945','water_supply'=>'3.266860548','waste_water'=>'30.88498476','sum'=>'48.73513476'],
];
$GLOBALS['pop_ms']=[
    'AD'=>['2007'=>81222, '2008'=>83137, '2009'=>84484, '2010'=>84082, '2011'=>78115, '2012'=>78115, '2013'=>76246, '2014'=>null, '2015'=>null, '2016'=>71732, '2017'=>null, '2018'=>null],
    'AL'=>['2007'=>2981755, '2008'=>2958266, '2009'=>2936355, '2010'=>2918674, '2011'=>2907361, '2012'=>2903008, '2013'=>2897770, '2014'=>2892394, '2015'=>2885796, '2016'=>2875592, '2017'=>2876591, '2018'=>2870324],
    'AM'=>['2007'=>3222953, '2008'=>3230086, '2009'=>3237976, '2010'=>3249482, '2011'=>3262650, '2012'=>3274285, '2013'=>3026878, '2014'=>null, '2015'=>3010598, '2016'=>2998577, '2017'=>2986151, '2018'=>2972732],
    'AT'=>['2007'=>8282984, '2008'=>8307989, '2009'=>8335003, '2010'=>8351643, '2011'=>8375164, '2012'=>8408121, '2013'=>8451860, '2014'=>8507786, '2015'=>8584926, '2016'=>8700471, '2017'=>8772865, '2018'=>8822267],
    'AZ'=>['2007'=>8532700, '2008'=>8629900, '2009'=>8896900, '2010'=>8997586, '2011'=>9111078, '2012'=>9235085, '2013'=>9356483, '2014'=>9477119, '2015'=>9593038, '2016'=>9705643, '2017'=>9809981, '2018'=>9898085],
    'BA'=>['2007'=>3844017, '2008'=>3843846, '2009'=>3843998, '2010'=>3844046, '2011'=>3843183, '2012'=>3839265, '2013'=>3835645, '2014'=>3830911, '2015'=>3825334, '2016'=>3515982, '2017'=>3509728, '2018'=>3502550],
    'BE'=>['2007'=>10584534, '2008'=>10666866, '2009'=>10753080, '2010'=>10839905, '2011'=>11000638, '2012'=>11075889, '2013'=>11137974, '2014'=>11180840, '2015'=>11237274, '2016'=>11311117, '2017'=>11351727, '2018'=>11413058],
    'BG'=>['2007'=>7572673, '2008'=>7518002, '2009'=>7467119, '2010'=>7421766, '2011'=>7369431, '2012'=>7327224, '2013'=>7284552, '2014'=>7245677, '2015'=>7202198, '2016'=>7153784, '2017'=>7101859, '2018'=>7050034],
    'BY'=>['2007'=>9714461, '2008'=>9689770, '2009'=>9671912, '2010'=>9480178, '2011'=>9481193, '2012'=>9465150, '2013'=>9463840, '2014'=>9468154, '2015'=>9480868, '2016'=>9498364, '2017'=>9504704, '2018'=>9491823],
    'CH'=>['2007'=>7508739, '2008'=>7593494, '2009'=>7701856, '2010'=>7785806, '2011'=>7870134, '2012'=>7954662, '2013'=>8039060, '2014'=>8139631, '2015'=>8237666, '2016'=>8327126, '2017'=>8419550, '2018'=>8482152],
    'CY'=>['2007'=>757916, '2008'=>776333, '2009'=>796930, '2010'=>819140, '2011'=>839751, '2012'=>862011, '2013'=>865878, '2014'=>858000, '2015'=>847008, '2016'=>848319, '2017'=>854802, '2018'=>864236],
    'CZ'=>['2007'=>10254233, '2008'=>10343422, '2009'=>10425783, '2010'=>10462088, '2011'=>10486731, '2012'=>10505445, '2013'=>10516125, '2014'=>10512419, '2015'=>10538275, '2016'=>10553843, '2017'=>10578820, '2018'=>10610055],
    'DE'=>['2007'=>82314906, '2008'=>82217837, '2009'=>82002356, '2010'=>81802257, '2011'=>80222065, '2012'=>80327900, '2013'=>80523746, '2014'=>80767463, '2015'=>81197537, '2016'=>82175684, '2017'=>82521653, '2018'=>82850000],
    'DK'=>['2007'=>5447084, '2008'=>5475791, '2009'=>5511451, '2010'=>5534738, '2011'=>5560628, '2012'=>5580516, '2013'=>5602628, '2014'=>5627235, '2015'=>5659715, '2016'=>5707251, '2017'=>5748769, '2018'=>5781190],
    'EA18'=>['2007'=>327954596, '2008'=>329884170, '2009'=>331286399, '2010'=>332124448, '2011'=>331520001, '2012'=>332285283, '2013'=>333073061, '2014'=>334597167, '2015'=>335638985, '2016'=>336991491, '2017'=>337871520, '2018'=>338718365],
    'EA19'=>['2007'=>331204579, '2008'=>333096775, '2009'=>334470255, '2010'=>335266424, '2011'=>334572589, '2012'=>335288924, '2013'=>336044966, '2014'=>337540639, '2015'=>338560247, '2016'=>339880049, '2017'=>340719424, '2018'=>341527266],
    'EE'=>['2007'=>1342920, '2008'=>1338440, '2009'=>1335740, '2010'=>1333290, '2011'=>1329660, '2012'=>1325217, '2013'=>1320174, '2014'=>1315819, '2015'=>1314870, '2016'=>1315944, '2017'=>1315635, '2018'=>1319133],
    'EL'=>['2007'=>11036008, '2008'=>11060937, '2009'=>11094745, '2010'=>11119289, '2011'=>11123392, '2012'=>11086406, '2013'=>11003615, '2014'=>10926807, '2015'=>10858018, '2016'=>10783748, '2017'=>10768193, '2018'=>10738868],
    'ES'=>['2007'=>44784666, '2008'=>45668939, '2009'=>46239273, '2010'=>46486619, '2011'=>46667174, '2012'=>46818219, '2013'=>46727890, '2014'=>46512199, '2015'=>46449565, '2016'=>46440099, '2017'=>46527039, '2018'=>46659302],
    'EU27'=>['2007'=>493987245, '2008'=>495985066, '2009'=>497780439, '2010'=>498867771, '2011'=>498674980, '2012'=>499771980, '2013'=>500900868, '2014'=>502764521, '2015'=>504314787, '2016'=>506086508, '2017'=>507367473, '2018'=>508605473],
    'EU28'=>['2007'=>498300775, '2008'=>500297033, '2009'=>502090235, '2010'=>503170618, '2011'=>502964837, '2012'=>504047964, '2013'=>505163008, '2014'=>507011330, '2015'=>508540103, '2016'=>510277177, '2017'=>511521686, '2018'=>512710966],
    'FI'=>['2007'=>5276955, '2008'=>5300484, '2009'=>5326314, '2010'=>5351427, '2011'=>5375276, '2012'=>5401267, '2013'=>5426674, '2014'=>5451270, '2015'=>5471753, '2016'=>5487308, '2017'=>5503297, '2018'=>5513130],
    'FR'=>['2007'=>63645065, '2008'=>64007193, '2009'=>64350226, '2010'=>64658856, '2011'=>64978721, '2012'=>65276983, '2013'=>65600350, '2014'=>65942267, '2015'=>66456279, '2016'=>66730453, '2017'=>66989083, '2018'=>67221943],
    'FX'=>['2007'=>61795238, '2008'=>62134866, '2009'=>62465709, '2010'=>62765235, '2011'=>63070344, '2012'=>63375971, '2013'=>63697865, '2014'=>null, '2015'=>null, '2016'=>null, '2017'=>null, '2018'=>null],
    'GE'=>['2007'=>4394702, '2008'=>4382070, '2009'=>4385429, '2010'=>4436391, '2011'=>4469250, '2012'=>4497617, '2013'=>null, '2014'=>4490498, '2015'=>3729500, '2016'=>3720400, '2017'=>3726374, '2018'=>3729633],
    'HR'=>['2007'=>4313530, '2008'=>4311967, '2009'=>4309796, '2010'=>4302847, '2011'=>4289857, '2012'=>4275984, '2013'=>4262140, '2014'=>4246809, '2015'=>4225316, '2016'=>4190669, '2017'=>4154213, '2018'=>4105493],
    'HU'=>['2007'=>10066158, '2008'=>10045401, '2009'=>10030975, '2010'=>10014324, '2011'=>9985722, '2012'=>9931925, '2013'=>9908798, '2014'=>9877365, '2015'=>9855571, '2016'=>9830485, '2017'=>9797561, '2018'=>9778371],
    'IE'=>['2007'=>4340118, '2008'=>4457765, '2009'=>4521322, '2010'=>4549428, '2011'=>4570881, '2012'=>4589287, '2013'=>4609779, '2014'=>4637852, '2015'=>4677627, '2016'=>4726286, '2017'=>4784383, '2018'=>4838259],
    'IS'=>['2007'=>307672, '2008'=>315459, '2009'=>319368, '2010'=>317630, '2011'=>318452, '2012'=>319575, '2013'=>321857, '2014'=>325671, '2015'=>329100, '2016'=>332529, '2017'=>338349, '2018'=>348450],
    'IT'=>['2007'=>58223744, '2008'=>58652875, '2009'=>59000586, '2010'=>59190143, '2011'=>59364690, '2012'=>59394207, '2013'=>59685227, '2014'=>60782668, '2015'=>60795612, '2016'=>60665551, '2017'=>60589445, '2018'=>60483973],
    'LI'=>['2007'=>35168, '2008'=>35356, '2009'=>35589, '2010'=>35894, '2011'=>36149, '2012'=>36475, '2013'=>36838, '2014'=>37129, '2015'=>37366, '2016'=>37622, '2017'=>37810, '2018'=>38114],
    'LT'=>['2007'=>3249983, '2008'=>3212605, '2009'=>3183856, '2010'=>3141976, '2011'=>3052588, '2012'=>3003641, '2013'=>2971905, '2014'=>2943472, '2015'=>2921262, '2016'=>2888558, '2017'=>2847904, '2018'=>2808901],
    'LU'=>['2007'=>476187, '2008'=>483799, '2009'=>493500, '2010'=>502066, '2011'=>511840, '2012'=>524853, '2013'=>537039, '2014'=>549680, '2015'=>562958, '2016'=>576249, '2017'=>590667, '2018'=>602005],
    'LV'=>['2007'=>2208840, '2008'=>2191810, '2009'=>2162834, '2010'=>2120504, '2011'=>2074605, '2012'=>2044813, '2013'=>2023825, '2014'=>2001468, '2015'=>1986096, '2016'=>1968957, '2017'=>1950116, '2018'=>1934379],
    'MC'=>['2007'=>null, '2008'=>null, '2009'=>null, '2010'=>null, '2011'=>null, '2012'=>null, '2013'=>null, '2014'=>null, '2015'=>37700, '2016'=>38200, '2017'=>null, '2018'=>null],
    'MD'=>['2007'=>3581110, '2008'=>3572703, '2009'=>3567512, '2010'=>3563695, '2011'=>3560430, '2012'=>3559541, '2013'=>3559497, '2014'=>3557634, '2015'=>3555159, '2016'=>3553056, '2017'=>3550852, '2018'=>3547539],
    'ME'=>['2007'=>614624, '2008'=>615543, '2009'=>617157, '2010'=>619001, '2011'=>619850, '2012'=>620308, '2013'=>620893, '2014'=>621521, '2015'=>622099, '2016'=>622218, '2017'=>622387, '2018'=>622359],
    'MK'=>['2007'=>2041941, '2008'=>2045177, '2009'=>2048619, '2010'=>2052722, '2011'=>2057284, '2012'=>2059794, '2013'=>2062294, '2014'=>2065769, '2015'=>2069172, '2016'=>2071278, '2017'=>2073702, '2018'=>2075301],
    'MT'=>['2007'=>405616, '2008'=>407832, '2009'=>410926, '2010'=>414027, '2011'=>414989, '2012'=>417546, '2013'=>422509, '2014'=>429424, '2015'=>439691, '2016'=>450415, '2017'=>460297, '2018'=>475701],
    'NL'=>['2007'=>16357992, '2008'=>16405399, '2009'=>16485787, '2010'=>16574989, '2011'=>16655799, '2012'=>16730348, '2013'=>16779575, '2014'=>16829289, '2015'=>16900726, '2016'=>16979120, '2017'=>17081507, '2018'=>17181084],
    'NO'=>['2007'=>4681134, '2008'=>4737171, '2009'=>4799252, '2010'=>4858199, '2011'=>4920305, '2012'=>4985870, '2013'=>5051275, '2014'=>5107970, '2015'=>5166493, '2016'=>5210721, '2017'=>5258317, '2018'=>5295619],
    'PL'=>['2007'=>38125479, '2008'=>38115641, '2009'=>38135876, '2010'=>38022869, '2011'=>38062718, '2012'=>38063792, '2013'=>38062535, '2014'=>38017856, '2015'=>38005614, '2016'=>37967209, '2017'=>37972964, '2018'=>37976687],
    'PT'=>['2007'=>10532588, '2008'=>10553339, '2009'=>10563014, '2010'=>10573479, '2011'=>10572721, '2012'=>10542398, '2013'=>10487289, '2014'=>10427301, '2015'=>10374822, '2016'=>10341330, '2017'=>10309573, '2018'=>10291027],
    'RO'=>['2007'=>21130503, '2008'=>20635460, '2009'=>20440290, '2010'=>20294683, '2011'=>20199059, '2012'=>20095996, '2013'=>20020074, '2014'=>19947311, '2015'=>19870647, '2016'=>19760314, '2017'=>19644350, '2018'=>19523621],
    'RS'=>['2007'=>7397651, '2008'=>7365507, '2009'=>7334937, '2010'=>7306677, '2011'=>7251549, '2012'=>7216649, '2013'=>7181505, '2014'=>7146759, '2015'=>7114393, '2016'=>7076372, '2017'=>7040272, '2018'=>7001444],
    'RU'=>['2007'=>142220968, '2008'=>142008838, '2009'=>141903979, '2010'=>141914509, '2011'=>142856536, '2012'=>143056383, '2013'=>null, '2014'=>143666931, '2015'=>null, '2016'=>null, '2017'=>null, '2018'=>null],
    'SE'=>['2007'=>9113257, '2008'=>9182927, '2009'=>9256347, '2010'=>9340682, '2011'=>9415570, '2012'=>9482855, '2013'=>9555893, '2014'=>9644864, '2015'=>9747355, '2016'=>9851017, '2017'=>9995153, '2018'=>10120242],
    'SI'=>['2007'=>2010377, '2008'=>2010269, '2009'=>2032362, '2010'=>2046976, '2011'=>2050189, '2012'=>2055496, '2013'=>2058821, '2014'=>2061085, '2015'=>2062874, '2016'=>2064188, '2017'=>2065895, '2018'=>2066880],
    'SK'=>['2007'=>5373180, '2008'=>5376064, '2009'=>5382401, '2010'=>5390410, '2011'=>5392446, '2012'=>5404322, '2013'=>5410836, '2014'=>5415949, '2015'=>5421349, '2016'=>5426252, '2017'=>5435343, '2018'=>5443120],
    'SM'=>['2007'=>30368, '2008'=>32054, '2009'=>31269, '2010'=>null, '2011'=>31863, '2012'=>33376, '2013'=>33562, '2014'=>32520, '2015'=>32789, '2016'=>33005, '2017'=>null, '2018'=>null],
    'TR'=>['2007'=>69729967, '2008'=>70586256, '2009'=>71517100, '2010'=>72561312, '2011'=>73722988, '2012'=>74724269, '2013'=>75627384, '2014'=>76667864, '2015'=>77695904, '2016'=>78741053, '2017'=>79814871, '2018'=>80810525],
    'UA'=>['2007'=>46465691, '2008'=>46192309, '2009'=>45963359, '2010'=>45782592, '2011'=>45598179, '2012'=>45453282, '2013'=>45372692, '2014'=>45245894, '2015'=>42759661, '2016'=>42590879, '2017'=>42414905, '2018'=>42216766],
    'UK'=>['2007'=>61073279, '2008'=>61571647, '2009'=>62042343, '2010'=>62510197, '2011'=>63022532, '2012'=>63495303, '2013'=>63905297, '2014'=>64351155, '2015'=>64875165, '2016'=>65382556, '2017'=>65808573, '2018'=>66238007],
    'XK'=>['2007'=>2126708, '2008'=>2153139, '2009'=>2180686, '2010'=>2208107, '2011'=>1794180, '2012'=>1798645, '2013'=>1815605, '2014'=>1820631, '2015'=>1804944, '2016'=>1771604, '2017'=>1783531, '2018'=>1798506],
];
