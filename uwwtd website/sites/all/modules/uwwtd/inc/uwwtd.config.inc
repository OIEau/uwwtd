<?php

if ( !defined('UWWTD_CACHE_ACTIVE')) {
    define('UWWTD_CACHE_ACTIVE', true);
}
if ( !defined('DEBUG_ALGO')) {
    define('DEBUG_ALGO', true);    
}


$GLOBALS['uwwtd']['default_by_cc'] = array(
    'at'=>array(
        'rcaType'=>'SA',
        'formal'=>'5854'
	),
    'be'=>array(
        'rcaType'=>'SA',
        'formal'=>'54/523'
    ),
    'bg'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
	),
    'cy'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
    ),
    'cz'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
	),
    'de'=>array(
        'rcaType'=>'A54',
        'formal'=>'5454'
	),
    'dk'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
    ),
    'ee'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
	),
    'es'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
	),
    'fi'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
	),
    'fr'=>array(
        'rcaType'=>'NA',
        'formal'=>'54/523/NA'
    ),
    'gr'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
	),
    'hr'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
    ),
    'hu'=>array(
        'rcaType'=>'NA',
        'formal'=>'54'
	),
    'ie'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
    ),
    'it'=>array(
        'rcaType'=>'NA',
        'formal'=>'54/523/NA'
	),
    'lt'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
    ),
    'lu'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
	),
    'lv'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
	),
    'mt'=>array(
        'rcaType'=>'SA',
        'formal'=>'NA/523'
	),
    'nl'=>array(
        'rcaType'=>'NA',
        'formal'=>'5854'
	),
    'pl'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
    ),
    'pt'=>array(
        'rcaType'=>'NA',
        'formal'=>'LSA/NA/523'
	),
    'ro'=>array(
        'rcaType'=>'SA',
        'formal'=>'58523'
    ),
    'se'=>array(
        'rcaType'=>'SA',
        'formal'=>'523'
	),
    'si'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
    ),
    'sk'=>array(
        'rcaType'=>'CSA',
        'formal'=>'58523'
	),
    'uk'=>array(
        'rcaType'=>'NA',
        'formal'=>'NA/523'
	), 
);



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
    'UWWCMSA' => t("Catchment of sensitive area"),
    'UWW55CMSA' => t("Catchment of sensitive area defined in the sense of Article 5.5 of the UWWTD"),
    'UWWCASA' => t("Coast area sensitive area"),
    'UWWCLSA' => t("Coast line sensitive area"),
    'UWWLKSA'  => t("Lake sensitive area"),
    'UWWRISA' => t("River sensitive area"),
    'UWWTWSA' => t("Transitional water sensitive area"),
    'UWWTDLSA' => t("Less sensitive Area"),
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
    'NOTCON' => '#696969',
);
$GLOBALS['uwwtd']['treatment_type_color']  = array(
    'P' => '#ea8b2e',
    'S' => '#3cc53c',
    'MS' => '#5ddfff',
    'NI' => '#d93c3c',
    'NOTCON' => '#696969',
    
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
  1998 => '1th',
  2000 => '2th',
  2002 => '3th',
  2004 => '4th',
  2006 => '5th',
  2008 => '6th',
  2010 => '7th',
  2011 => '8th',
  2012 => '8th',                                                   
  2014 => '9th',                          
  2015 => '9th',                          
  2016 => '10th',
  2018 => '11th', 
  2020 => '12th',
  2022 => '13th',
  2024 => '14th'
  
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
$GLOBALS['big_cities'] = [
"AT006"=>['name'=>"GRAZ", 'cc'=>"AT"],
"AT007"=>['name'=>"INNSBRUCK", 'cc'=>"AT"],
"AT014"=>['name'=>"LINZ", 'cc'=>"AT"],
"AT016"=>['name'=>"SALZBURG", 'cc'=>"AT"],
"AT022"=>['name'=>"WIEN", 'cc'=>"AT"],
"ATAG_2-K1931629"=>['name'=>"Klagenfurt (Worthersee Ost)", 'cc'=>"AT"],
"ATAG_2-K1939997"=>['name'=>"Villach", 'cc'=>"AT"],
"ATAG_3-11"=>['name'=>"Raum Schwechat (Schwechat-Mannsworth)", 'cc'=>"AT"],
"ATAG_3-119"=>['name'=>"Raum Krems (Krems-Weinzierl)", 'cc'=>"AT"],
"ATAG_3-449"=>['name'=>"Raum Wiener Neustadt (Wiener Neustadt-Lichtenworth)", 'cc'=>"AT"],
"ATAG_3-47"=>['name'=>"Raum St. Polten (Traismauer-Stollhofen)", 'cc'=>"AT"],
"ATAG_4-41812001"=>['name'=>"Wels und Umgebung", 'cc'=>"AT"],
"ATAG_7-7930111"=>['name'=>"Strass/Zillertal (Achental-Inntal-Zillertal)", 'cc'=>"AT"],
"ATAG_8-Hofsteig"=>['name'=>"Hard (Region Hofsteig)", 'cc'=>"AT"],
"ATAG_8-Hohenems"=>['name'=>"Hohenems (Region Hohenems)", 'cc'=>"AT"],
"ATAG_8-Meiningen"=>['name'=>"Meiningen (Region Feldkirch)", 'cc'=>"AT"],
"BE004"=>['name'=>"ANTWERPEN", 'cc'=>"BE"],
"BE013"=>['name'=>"BRUGGE", 'cc'=>"BE"],
"BE014"=>['name'=>"BRUXELLES/BRUSSEL", 'cc'=>"BE"],
"BE015"=>['name'=>"CHARLEROI", 'cc'=>"BE"],
"BE022"=>['name'=>"GENT", 'cc'=>"BE"],
"BE034"=>['name'=>"LIEGE", 'cc'=>"BE"],
"BE044"=>['name'=>"MONS", 'cc'=>"BE"],
"BE2"=>['name'=>"OOSTENDE", 'cc'=>"BE"],
"BE310"=>['name'=>"WAVRE", 'cc'=>"BE"],
"BE324"=>['name'=>"MOUSCRON", 'cc'=>"BE"],
"BE325"=>['name'=>"LA LOUVIERE", 'cc'=>"BE"],
"BG003"=>['name'=>"BURGAS", 'cc'=>"BG"],
"BG013"=>['name'=>"PLEVEN", 'cc'=>"BG"],
"BG014"=>['name'=>"PLOVDIV", 'cc'=>"BG"],
"BG016"=>['name'=>"RUSE", 'cc'=>"BG"],
"BG019"=>['name'=>"SLIVEN", 'cc'=>"BG"],
"BG020"=>['name'=>"SOFIYA", 'cc'=>"BG"],
"BG021"=>['name'=>"STARA ZAGORA", 'cc'=>"BG"],
"BG022"=>['name'=>"DOBRICH", 'cc'=>"BG"],
"BG023"=>['name'=>"VARNA", 'cc'=>"BG"],
"CH_CITY_0230"=>['name'=>"Winterthur", 'cc'=>"CH"],
"CH_CITY_0261"=>['name'=>"Zürich", 'cc'=>"CH"],
"CH_CITY_0351"=>['name'=>"Bern", 'cc'=>"CH"],
"CH_CITY_2701"=>['name'=>"Basel", 'cc'=>"CH"],
"CH_CITY_5586"=>['name'=>"Lausanne", 'cc'=>"CH"],
"CH_CITY_6621"=>['name'=>"Geneve", 'cc'=>"CH"],
"CY001"=>['name'=>"NICOSIA", 'cc'=>"CY"],
"CY005"=>['name'=>"LIMASSOL", 'cc'=>"CY"],
"CZ003"=>['name'=>"BRNO", 'cc'=>"CZ"],
"CZ015"=>['name'=>"HRADEC KRALOVE", 'cc'=>"CZ"],
"CZ029"=>['name'=>"LIBEREC", 'cc'=>"CZ"],
"CZ039"=>['name'=>"OLOMOUC", 'cc'=>"CZ"],
"CZ042"=>['name'=>"OSTRAVA", 'cc'=>"CZ"],
"CZ046"=>['name'=>"PLZEN", 'cc'=>"CZ"],
"CZ047"=>['name'=>"PRAHA", 'cc'=>"CZ"],
"DE001"=>['name'=>"AACHEN", 'cc'=>"DE"],
"DE025"=>['name'=>"AUGSBURG", 'cc'=>"DE"],
"DE055"=>['name'=>"BERGISCH GLADBACH", 'cc'=>"DE"],
"DE057"=>['name'=>"BERLIN", 'cc'=>"DE"],
"DE059"=>['name'=>"BIELEFELD", 'cc'=>"DE"],
"DE067"=>['name'=>"BOCHUM", 'cc'=>"DE"],
"DE068"=>['name'=>"BONN", 'cc'=>"DE"],
"DE072"=>['name'=>"BOTTROP", 'cc'=>"DE"],
"DE076"=>['name'=>"BRAUNSCHWEIG", 'cc'=>"DE"],
"DE077"=>['name'=>"BREMEN", 'cc'=>"DE"],
"DE078"=>['name'=>"BREMERHAVEN", 'cc'=>"DE"],
"DE093"=>['name'=>"CHEMNITZ", 'cc'=>"DE"],
"DE098"=>['name'=>"COTTBUS", 'cc'=>"DE"],
"DE103"=>['name'=>"DARMSTADT", 'cc'=>"DE"],
"DE109"=>['name'=>"DESSAU", 'cc'=>"DE"],
"DE119"=>['name'=>"DORTMUND", 'cc'=>"DE"],
"DE121"=>['name'=>"DRESDEN", 'cc'=>"DE"],
"DE123"=>['name'=>"DUISBURG", 'cc'=>"DE"],
"DE126"=>['name'=>"DUESSELDORF", 'cc'=>"DE"],
"DE144"=>['name'=>"ERFURT", 'cc'=>"DE"],
"DE147"=>['name'=>"ERLANGEN", 'cc'=>"DE"],
"DE151"=>['name'=>"ESSEN", 'cc'=>"DE"],
"DE163"=>['name'=>"FRANKFURT AM MAIN", 'cc'=>"DE"],
"DE167"=>['name'=>"FREIBURG IM BREISGAU", 'cc'=>"DE"],
"DE187"=>['name'=>"GELSENKIRCHEN", 'cc'=>"DE"],
"DE189"=>['name'=>"GERA", 'cc'=>"DE"],
"DE204"=>['name'=>"GOETTINGEN", 'cc'=>"DE"],
"DE216"=>['name'=>"HAGEN", 'cc'=>"DE"],
"DE219"=>['name'=>"HALLE", 'cc'=>"DE"],
"DE221"=>['name'=>"HAMBURG", 'cc'=>"DE"],
"DE223"=>['name'=>"HAMM", 'cc'=>"DE"],
"DE226"=>['name'=>"HANNOVER", 'cc'=>"DE"],
"DE230"=>['name'=>"HEIDELBERG", 'cc'=>"DE"],
"DE233"=>['name'=>"HEILBRONN", 'cc'=>"DE"],
"DE244"=>['name'=>"HERNE", 'cc'=>"DE"],
"DE250"=>['name'=>"HILDESHEIM", 'cc'=>"DE"],
"DE267"=>['name'=>"JENA", 'cc'=>"DE"],
"DE274"=>['name'=>"KARLSRUHE", 'cc'=>"DE"],
"DE275"=>['name'=>"KASSEL", 'cc'=>"DE"],
"DE283"=>['name'=>"KIEL", 'cc'=>"DE"],
"DE287"=>['name'=>"KOBLENZ", 'cc'=>"DE"],
"DE288"=>['name'=>"KOELN", 'cc'=>"DE"],
"DE295"=>['name'=>"KREFELD", 'cc'=>"DE"],
"DE314"=>['name'=>"LEIPZIG", 'cc'=>"DE"],
"DE319"=>['name'=>"LEVERKUSEN", 'cc'=>"DE"],
"DE331"=>['name'=>"LUEBECK", 'cc'=>"DE"],
"DE336"=>['name'=>"LUDWIGSHAFEN AM RHEIN", 'cc'=>"DE"],
"DE339"=>['name'=>"MAGDEBURG", 'cc'=>"DE"],
"DE341"=>['name'=>"MAINZ", 'cc'=>"DE"],
"DE342"=>['name'=>"MANNHEIM", 'cc'=>"DE"],
"DE361"=>['name'=>"MOERS", 'cc'=>"DE"],
"DE362"=>['name'=>"MONCHEN-GLADBACH", 'cc'=>"DE"],
"DE369"=>['name'=>"MUELHEIM AN DER RUHR", 'cc'=>"DE"],
"DE370"=>['name'=>"MUENCHEN", 'cc'=>"DE"],
"DE372"=>['name'=>"MUENSTER", 'cc'=>"DE"],
"DE387"=>['name'=>"NEUSS", 'cc'=>"DE"],
"DE400"=>['name'=>"NURNBERG", 'cc'=>"DE"],
"DE402"=>['name'=>"OBERHAUSEN", 'cc'=>"DE"],
"DE407"=>['name'=>"OFFENBACH AM MAIN", 'cc'=>"DE"],
"DE409"=>['name'=>"OLDENBURG", 'cc'=>"DE"],
"DE412"=>['name'=>"OSNABRUECK", 'cc'=>"DE"],
"DE417"=>['name'=>"PADERBORN", 'cc'=>"DE"],
"DE423"=>['name'=>"PFORZHEIM", 'cc'=>"DE"],
"DE431"=>['name'=>"POTSDAM", 'cc'=>"DE"],
"DE443"=>['name'=>"RECKLINGHAUSEN", 'cc'=>"DE"],
"DE444"=>['name'=>"REGENSBURG", 'cc'=>"DE"],
"DE447"=>['name'=>"REMSCHEID", 'cc'=>"DE"],
"DE461"=>['name'=>"ROSTOCK", 'cc'=>"DE"],
"DE468"=>['name'=>"SAARBRUECKEN", 'cc'=>"DE"],
"DE470"=>['name'=>"SALZGITTER", 'cc'=>"DE"],
"DE490"=>['name'=>"SCHWERIN", 'cc'=>"DE"],
"DE498"=>['name'=>"SIEGEN", 'cc'=>"DE"],
"DE503"=>['name'=>"SOLINGEN", 'cc'=>"DE"],
"DE526"=>['name'=>"STUTTGART", 'cc'=>"DE"],
"DE538"=>['name'=>"ULM", 'cc'=>"DE"],
"DE583"=>['name'=>"WIESBADEN", 'cc'=>"DE"],
"DE591"=>['name'=>"WITTEN", 'cc'=>"DE"],
"DE596"=>['name'=>"WOLFSBURG", 'cc'=>"DE"],
"DE600"=>['name'=>"WUPPERTAL", 'cc'=>"DE"],
"DE602"=>['name'=>"WUERZBURG", 'cc'=>"DE"],
"DE607"=>['name'=>"ZWICKAU", 'cc'=>"DE"],
"DEAG_BW1155100000016"=>['name'=>"Boblingen-Sindelfingen/KW 1", 'cc'=>"DE"],
"DEAG_BW1175100000062"=>['name'=>"Goppingen", 'cc'=>"DE"],
"DEAG_BW2165100000007"=>['name'=>"Buhl und Umgebung", 'cc'=>"DE"],
"DEAG_BW2265100000005"=>['name'=>"Bergstrasse", 'cc'=>"DE"],
"DEAG_BW2265100000007"=>['name'=>"Schwetzingen-Ketsch", 'cc'=>"DE"],
"DEAG_BW3355100000005"=>['name'=>"Konstanz", 'cc'=>"DE"],
"DEAG_BW3365100000001"=>['name'=>"Wieseverband", 'cc'=>"DE"],
"DEAG_BW4165100000066"=>['name'=>"Tubingen", 'cc'=>"DE"],
"DEAG_BW4365100000025"=>['name'=>"Mariatal", 'cc'=>"DE"],
"DEAG_BYAB-K0022"=>['name'=>"Gemeinschaftsklaranlage Bayer. Untermain GmbH", 'cc'=>"DE"],
"DEAG_BYHO-K0002"=>['name'=>"ZV Sachsische Saale", 'cc'=>"DE"],
"DEAG_BYHO-K0089"=>['name'=>"Bayreuth", 'cc'=>"DE"],
"DEAG_BYHO-K0180"=>['name'=>"Kulmbach", 'cc'=>"DE"],
"DEAG_BYIN-K0003"=>['name'=>"ZV ZENTRALKLAERANLAGE INGOLSTADT", 'cc'=>"DE"],
"DEAG_BYKC-K0001"=>['name'=>"BAMBERG", 'cc'=>"DE"],
"DEAG_BYKE-K0020"=>['name'=>"ZV GRUPPENKLAERWERK KEMPTEN S. LAUBEN", 'cc'=>"DE"],
"DEAG_BYKG-K0001"=>['name'=>"Schweinfurt", 'cc'=>"DE"],
"DEAG_BYM-K0121"=>['name'=>"AmperVerband", 'cc'=>"DE"],
"DEAG_BYN-K0003"=>['name'=>"FUERTH", 'cc'=>"DE"],
"DEAG_BYRO-K0001"=>['name'=>"ROSENHEIM", 'cc'=>"DE"],
"DEAG_HE064350140131"=>['name'=>"Hanau / Nordwest", 'cc'=>"DE"],
"DEAG_HE065310050471"=>['name'=>"Giessen / Margaretenhutte/Sudliche Lahns", 'cc'=>"DE"],
"DEAG_HE065340143511"=>['name'=>"Marburg / Cappel", 'cc'=>"DE"],
"DEAG_HE066310091101"=>['name'=>"Fulda / Glaserzell", 'cc'=>"DE"],
"DEAG_MV06.06.1 wbu1533"=>['name'=>"Wismar", 'cc'=>"DE"],
"DEAG_MV52.74.3 wbu2539"=>['name'=>"Stavenhagen", 'cc'=>"DE"],
"DEAG_NI252006201"=>['name'=>"Hameln", 'cc'=>"DE"],
"DEAG_NI255023201"=>['name'=>"Holzminden", 'cc'=>"DE"],
"DEAG_NI352011201"=>['name'=>"Cuxhaven", 'cc'=>"DE"],
"DEAG_NI355022201"=>['name'=>"Luneburg", 'cc'=>"DE"],
"DEAG_NI454032201"=>['name'=>"Lingen", 'cc'=>"DE"],
"DEAG_NI456015201"=>['name'=>"Nordhorn", 'cc'=>"DE"],
"DEAG_NW1913"=>['name'=>"Stadt Sankt Augustin", 'cc'=>"DE"],
"DEAG_NW2516"=>['name'=>"Stadt Lunen", 'cc'=>"DE"],
"DEAG_NW2529"=>['name'=>"Stadt Kamen", 'cc'=>"DE"],
"DEAG_NW2782"=>['name'=>"Stadt Minden", 'cc'=>"DE"],
"DEAG_NW2797"=>['name'=>"Stadt Herford", 'cc'=>"DE"],
"DEAG_NW3013"=>['name'=>"Stadt Coesfeld", 'cc'=>"DE"],
"DEAG_NW3061"=>['name'=>"Stadt Rheine", 'cc'=>"DE"],
"DEAG_NW316"=>['name'=>"Stadt Gutersloh", 'cc'=>"DE"],
"DEAG_NW329"=>['name'=>"Stadt Rheda-Wiedenbruck", 'cc'=>"DE"],
"DEAG_NW35"=>['name'=>"Stadt Duren", 'cc'=>"DE"],
"DEAG_NW905"=>['name'=>"Stadt Dinslaken", 'cc'=>"DE"],
"DEAG_RP100601008"=>['name'=>"Kaiserslautern", 'cc'=>"DE"],
"DEAG_RP102430091"=>['name'=>"Worms", 'cc'=>"DE"],
"DEAG_SH56027"=>['name'=>"AZV Pinneberg", 'cc'=>"DE"],
"DEAG_SH58044"=>['name'=>"Neumunster", 'cc'=>"DE"],
"DEAG_ST151541000001"=>['name'=>"Bitterfeld", 'cc'=>"DE"],
"DK001"=>['name'=>"AALBORG", 'cc'=>"DK"],
"DK013"=>['name'=>"KOEBENHAVN", 'cc'=>"DK"],
"DK017"=>['name'=>"ODENSE", 'cc'=>"DK"],
"DK026"=>['name'=>"AARHUS", 'cc'=>"DK"],
"DK278"=>['name'=>"ESBJERG", 'cc'=>"DK"],
"DK367"=>['name'=>"FREDERICIA CENTRALRENS", 'cc'=>"DK"],
"DK378"=>['name'=>"HOLSTEBRO", 'cc'=>"DK"],
"DK400"=>['name'=>"?BY", 'cc'=>"DK"],
"DK429"=>['name'=>"SKAGEN", 'cc'=>"DK"],
"DK434"=>['name'=>"RANDERS", 'cc'=>"DK"],
"DK437"=>['name'=>"HERNING", 'cc'=>"DK"],
"DK455"=>['name'=>"HORSENS", 'cc'=>"DK"],
"EE002"=>['name'=>"TALLINN", 'cc'=>"EE"],
"EE003"=>['name'=>"TARTU", 'cc'=>"EE"],
"EEIV0044"=>['name'=>"Kohtla-Jarve", 'cc'=>"EE"],
"ES003"=>['name'=>"ALBACETE", 'cc'=>"ES"],
"ES005"=>['name'=>"ALCALA DE HENARES", 'cc'=>"ES"],
"ES014"=>['name'=>"ALGECIRAS", 'cc'=>"ES"],
"ES016"=>['name'=>"ALICANTE", 'cc'=>"ES"],
"ES019"=>['name'=>"ALMERIA", 'cc'=>"ES"],
"ES030"=>['name'=>"BADAJOZ", 'cc'=>"ES"],
"ES033"=>['name'=>"BARCELONA", 'cc'=>"ES"],
"ES037"=>['name'=>"BILBAO", 'cc'=>"ES"],
"ES039"=>['name'=>"BURGOS", 'cc'=>"ES"],
"ES042"=>['name'=>"CADIZ", 'cc'=>"ES"],
"ES052"=>['name'=>"CASTELLON DE LA PLANA", 'cc'=>"ES"],
"ES059"=>['name'=>"CORDOBA", 'cc'=>"ES"],
"ES068"=>['name'=>"DONOSTIA-SAN SEBASTIAN", 'cc'=>"ES"],
"ES075"=>['name'=>"ELCHE", 'cc'=>"ES"],
"ES081"=>['name'=>"FUENLABRADA", 'cc'=>"ES"],
"ES086"=>['name'=>"GIJON", 'cc'=>"ES"],
"ES087"=>['name'=>"GRANADA", 'cc'=>"ES"],
"ES092"=>['name'=>"HUELVA", 'cc'=>"ES"],
"ES099"=>['name'=>"JAEN", 'cc'=>"ES"],
"ES100"=>['name'=>"JEREZ DE LA FRONTERA", 'cc'=>"ES"],
"ES10030310001022"=>['name'=>"BENIDORM", 'cc'=>"ES"],
"ES10030660001010"=>['name'=>"ELDA", 'cc'=>"ES"],
"ES10031330002060"=>['name'=>"TORREVIEJA", 'cc'=>"ES"],
"ES102"=>['name'=>"LA CORUNA", 'cc'=>"ES"],
"ES10461100001010"=>['name'=>"ALAQUAS", 'cc'=>"ES"],
"ES10461310002010"=>['name'=>"RJO", 'cc'=>"ES"],
"ES10461990001010"=>['name'=>"ALBUIXECH", 'cc'=>"ES"],
"ES108"=>['name'=>"LAS PALMAS DE GRAN CANARIA", 'cc'=>"ES"],
"ES110"=>['name'=>"LEON", 'cc'=>"ES"],
"ES111"=>['name'=>"LERIDA", 'cc'=>"ES"],
"ES114"=>['name'=>"LOGRONO", 'cc'=>"ES"],
"ES119"=>['name'=>"MADRID", 'cc'=>"ES"],
"ES121"=>['name'=>"MALAGA", 'cc'=>"ES"],
"ES12150782911010"=>['name'=>"SANTIAGO", 'cc'=>"ES"],
"ES127"=>['name'=>"MATARO", 'cc'=>"ES"],
"ES1290670005020"=>['name'=>"PEÑON DEL CUERVO", 'cc'=>"ES"],
"ES1290690015010"=>['name'=>"ARROYO DE LA VIBORA", 'cc'=>"ES"],
"ES13280130002010"=>['name'=>"ARANJUEZ", 'cc'=>"ES"],
"ES13280140001010"=>['name'=>"ARGANDA", 'cc'=>"ES"],
"ES13280470001010"=>['name'=>"COLLADO VILLALBA", 'cc'=>"ES"],
"ES13280960003010"=>['name'=>"NAVALCARNERO", 'cc'=>"ES"],
"ES13281300002010"=>['name'=>"SAN FERNANDO DE HENARES", 'cc'=>"ES"],
"ES13281480001010"=>['name'=>"TORREJÓN DE ARDOZ", 'cc'=>"ES"],
"ES139"=>['name'=>"MURCIA", 'cc'=>"ES"],
"ES1410700001010"=>['name'=>"ALJARAFE II", 'cc'=>"ES"],
"ES14300160001030"=>['name'=>"CARTAGENA", 'cc'=>"ES"],
"ES14300161912010"=>['name'=>"MAR MENOR SUR", 'cc'=>"ES"],
"ES14300270011011"=>['name'=>"MOLINA DE SEGURA NORTE", 'cc'=>"ES"],
"ES146"=>['name'=>"OVIEDO", 'cc'=>"ES"],
"ES148"=>['name'=>"PALMA DE MALLORCA", 'cc'=>"ES"],
"ES149"=>['name'=>"PAMPLONA", 'cc'=>"ES"],
"ES169"=>['name'=>"SABADELL", 'cc'=>"ES"],
"ES171"=>['name'=>"SALAMANCA", 'cc'=>"ES"],
"ES181"=>['name'=>"SANTA CRUZ DE TENERIFE", 'cc'=>"ES"],
"ES183"=>['name'=>"SANTANDER", 'cc'=>"ES"],
"ES18510010000000"=>['name'=>"CEUTA", 'cc'=>"ES"],
"ES188"=>['name'=>"SEVILLA", 'cc'=>"ES"],
"ES194"=>['name'=>"TERRASA", 'cc'=>"ES"],
"ES206"=>['name'=>"VALENCIA", 'cc'=>"ES"],
"ES208"=>['name'=>"VALLADOLID", 'cc'=>"ES"],
"ES211"=>['name'=>"VIGO", 'cc'=>"ES"],
"ES220"=>['name'=>"VITORIA-GASTEIZ", 'cc'=>"ES"],
"ES225"=>['name'=>"ZARAGOZA", 'cc'=>"ES"],
"ES232"=>['name'=>"ALCORCON", 'cc'=>"ES"],
"ES259"=>['name'=>"BADALONA", 'cc'=>"ES"],
"ES306"=>['name'=>"MONTCADA", 'cc'=>"ES"],
"ES332"=>['name'=>"GETAFE", 'cc'=>"ES"],
"ES3330040101010"=>['name'=>"AVILES", 'cc'=>"ES"],
"ES3330441901011"=>['name'=>"NORA-NOREA", 'cc'=>"ES"],
"ES335"=>['name'=>"HOSPITALET DE LLOBREGAT (L')", 'cc'=>"ES"],
"ES345"=>['name'=>"LEGANES", 'cc'=>"ES"],
"ES374"=>['name'=>"MOSTOLES", 'cc'=>"ES"],
"ES424"=>['name'=>"SAN VICENTE DE BARAKALDO", 'cc'=>"ES"],
"ES436"=>['name'=>"SANTA COLOMA DE GRAMENET", 'cc'=>"ES"],
"ES5380010800010"=>['name'=>"ADEJE-ARONA OESTE", 'cc'=>"ES"],
"ES6390850005010"=>['name'=>"VUELTA OSTRERA", 'cc'=>"ES"],
"ES7050190003010"=>['name'=>"AVILA", 'cc'=>"ES"],
"ES7341200001010"=>['name'=>"PALENCIA", 'cc'=>"ES"],
"ES8130050002010"=>['name'=>"ALCAZAR DE SAN JUAN- CAMPO DE CRIPTANA", 'cc'=>"ES"],
"ES8130340002010"=>['name'=>"CIUDAD REAL-MIGUELTURRA", 'cc'=>"ES"],
"ES8451650006010"=>['name'=>"TALAVERA DE LA REINA", 'cc'=>"ES"],
"ES9080960001010"=>['name'=>"GRANOLLERS", 'cc'=>"ES"],
"ES9081050001010"=>['name'=>"LLAGOSTA", 'cc'=>"ES"],
"ES9081360001010"=>['name'=>"MONTORNES DEL VALLES", 'cc'=>"ES"],
"ES9081630001040"=>['name'=>"PINEDA DE MAR", 'cc'=>"ES"],
"ES9081690001010"=>['name'=>"PRAT DE LLOBREGAT (EL)", 'cc'=>"ES"],
"ES9081840001010"=>['name'=>"RUBI", 'cc'=>"ES"],
"ES9082110001010"=>['name'=>"SANT FELIU DE LLOBREGAT", 'cc'=>"ES"],
"ES9082980001010"=>['name'=>"VIC", 'cc'=>"ES"],
"ES9083010001010"=>['name'=>"GAVA-VILADECANS", 'cc'=>"ES"],
"ES9170790005010"=>['name'=>"GIRONA", 'cc'=>"ES"],
"ES9431480013010"=>['name'=>"TARRAGONA", 'cc'=>"ES"],
"FI003"=>['name'=>"HELSINKI", 'cc'=>"FI"],
"FI019"=>['name'=>"LAHTI", 'cc'=>"FI"],
"FI023"=>['name'=>"OULU", 'cc'=>"FI"],
"FI034"=>['name'=>"TAMPERE", 'cc'=>"FI"],
"FI036"=>['name'=>"TURKU", 'cc'=>"FI"],
"FIYKR05_T001477"=>['name'=>"Pori", 'cc'=>"FI"],
"FR009"=>['name'=>"AMIENS", 'cc'=>"FR"],
"FR010"=>['name'=>"ANGERS", 'cc'=>"FR"],
"FR010000159220"=>['name'=>"FACHES-THUMESNIL", 'cc'=>"FR"],
"FR010000159512"=>['name'=>"ROUBAIX", 'cc'=>"FR"],
"FR010000162041"=>['name'=>"ARRAS", 'cc'=>"FR"],
"FR010000162160"=>['name'=>"BOULOGNE-SUR-MER", 'cc'=>"FR"],
"FR010000162351"=>['name'=>"FOUQUIERES-LES-LENS", 'cc'=>"FR"],
"FR011"=>['name'=>"ANGOULEME", 'cc'=>"FR"],
"FR012"=>['name'=>"ANNECY", 'cc'=>"FR"],
"FR020000168066"=>['name'=>"COLMAR", 'cc'=>"FR"],
"FR024"=>['name'=>"AVIGNON", 'cc'=>"FR"],
"FR028"=>['name'=>"BIARRITZ", 'cc'=>"FR"],
"FR030000177243"=>['name'=>"LAGNY-SUR-MARNE", 'cc'=>"FR"],
"FR030000177284"=>['name'=>"MEAUX", 'cc'=>"FR"],
"FR030000178646"=>['name'=>"VERSAILLES", 'cc'=>"FR"],
"FR030000191228"=>['name'=>"EVRY", 'cc'=>"FR"],
"FR030000195127"=>['name'=>"CERGY-PONTOISE", 'cc'=>"FR"],
"FR030000195585"=>['name'=>"SARCELLES", 'cc'=>"FR"],
"FR033"=>['name'=>"BESANCON", 'cc'=>"FR"],
"FR034"=>['name'=>"BETHUNE", 'cc'=>"FR"],
"FR037"=>['name'=>"BORDEAUX", 'cc'=>"FR"],
"FR040000129232"=>['name'=>"QUIMPER", 'cc'=>"FR"],
"FR040000153130"=>['name'=>"LAVAL", 'cc'=>"FR"],
"FR042"=>['name'=>"BREST", 'cc'=>"FR"],
"FR045"=>['name'=>"CAEN", 'cc'=>"FR"],
"FR047"=>['name'=>"CALAIS", 'cc'=>"FR"],
"FR050000112202"=>['name'=>"RODEZ", 'cc'=>"FR"],
"FR050000119031"=>['name'=>"BRIVE-LA-GAILLARDE", 'cc'=>"FR"],
"FR056"=>['name'=>"CHAMBERY", 'cc'=>"FR"],
"FR060000171270"=>['name'=>"MACON", 'cc'=>"FR"],
"FR060000183061"=>['name'=>"FREJUS", 'cc'=>"FR"],
"FR064"=>['name'=>"CLERMONT-FERRAND", 'cc'=>"FR"],
"FR074"=>['name'=>"DIJON", 'cc'=>"FR"],
"FR076"=>['name'=>"DOUAI", 'cc'=>"FR"],
"FR079"=>['name'=>"COUDEKERQUE-BRANCHE", 'cc'=>"FR"],
"FR095"=>['name'=>"CANNES-GRASSE-ANTIBES", 'cc'=>"FR"],
"FR096"=>['name'=>"GRENOBLE", 'cc'=>"FR"],
"FR098"=>['name'=>"HAGONDANGE", 'cc'=>"FR"],
"FR100000197414"=>['name'=>"SAINT-LOUIS", 'cc'=>"FR"],
"FR107"=>['name'=>"ROCHELLE", 'cc'=>"FR"],
"FR112"=>['name'=>"HAVRE", 'cc'=>"FR"],
"FR113"=>['name'=>"LE MANS", 'cc'=>"FR"],
"FR115"=>['name'=>"LENS", 'cc'=>"FR"],
"FR118"=>['name'=>"LILLE", 'cc'=>"FR"],
"FR119"=>['name'=>"LIMOGES", 'cc'=>"FR"],
"FR122"=>['name'=>"LORIENT", 'cc'=>"FR"],
"FR124"=>['name'=>"LYON", 'cc'=>"FR"],
"FR128"=>['name'=>"MARSEILLE-AIX-EN-PROVENCE", 'cc'=>"FR"],
"FR130"=>['name'=>"MAUBEUGE", 'cc'=>"FR"],
"FR132"=>['name'=>"MELUN", 'cc'=>"FR"],
"FR134"=>['name'=>"METZ", 'cc'=>"FR"],
"FR140"=>['name'=>"MONTBELIARD", 'cc'=>"FR"],
"FR145"=>['name'=>"MONTPELLIER", 'cc'=>"FR"],
"FR147"=>['name'=>"MULHOUSE", 'cc'=>"FR"],
"FR148"=>['name'=>"NANCY", 'cc'=>"FR"],
"FR149"=>['name'=>"NANTES", 'cc'=>"FR"],
"FR152"=>['name'=>"NICE", 'cc'=>"FR"],
"FR153"=>['name'=>"NIMES", 'cc'=>"FR"],
"FR156"=>['name'=>"ORLEANS", 'cc'=>"FR"],
"FR159"=>['name'=>"PARIS", 'cc'=>"FR"],
"FR160"=>['name'=>"PAU", 'cc'=>"FR"],
"FR162"=>['name'=>"PERPIGNAN", 'cc'=>"FR"],
"FR165"=>['name'=>"POITIERS", 'cc'=>"FR"],
"FR170"=>['name'=>"REIMS", 'cc'=>"FR"],
"FR171"=>['name'=>"RENNES", 'cc'=>"FR"],
"FR176"=>['name'=>"ROUEN", 'cc'=>"FR"],
"FR181"=>['name'=>"SAINT ETIENNE", 'cc'=>"FR"],
"FR184"=>['name'=>"SAINT NAZAIRE", 'cc'=>"FR"],
"FR197"=>['name'=>"STRASBOURG", 'cc'=>"FR"],
"FR199"=>['name'=>"THIONVILLE", 'cc'=>"FR"],
"FR201"=>['name'=>"TOULON", 'cc'=>"FR"],
"FR202"=>['name'=>"TOULOUSE", 'cc'=>"FR"],
"FR203"=>['name'=>"TOURS", 'cc'=>"FR"],
"FR205"=>['name'=>"TROYES", 'cc'=>"FR"],
"FR208"=>['name'=>"VALENCE", 'cc'=>"FR"],
"FR209"=>['name'=>"VALENCIENNES", 'cc'=>"FR"],
"GR005"=>['name'=>"ATHINAI", 'cc'=>"GR"],
"GR010"=>['name'=>"IRAKLION", 'cc'=>"GR"],
"GR023"=>['name'=>"LARISA", 'cc'=>"GR"],
"GR025"=>['name'=>"PATRAI", 'cc'=>"GR"],
"GR031"=>['name'=>"THESSALONIKI", 'cc'=>"GR"],
"GR035"=>['name'=>"VOLOS", 'cc'=>"GR"],
"HRBC_0001116"=>['name'=>"Rijeka", 'cc'=>"HR"],
"HRBC_0001131"=>['name'=>"Split-Solin", 'cc'=>"HR"],
"HRBC_0004008"=>['name'=>"Zagreb", 'cc'=>"HR"],
"HRBC_0007012"=>['name'=>"Osijek", 'cc'=>"HR"],
"HU006"=>['name'=>"BUDAPEST", 'cc'=>"HU"],
"HU009"=>['name'=>"DEBRECEN", 'cc'=>"HU"],
"HU018"=>['name'=>"GYOR", 'cc'=>"HU"],
"HU038"=>['name'=>"MISKOLC", 'cc'=>"HU"],
"HU049"=>['name'=>"PECS", 'cc'=>"HU"],
"HU054"=>['name'=>"SZEGED", 'cc'=>"HU"],
"HU055"=>['name'=>"SZEKESFEHERVAR", 'cc'=>"HU"],
"HU062"=>['name'=>"Kecskemet - Szennyvizelvezetesi Agglomeracio", 'cc'=>"HU"],
"HU064"=>['name'=>"Nyiregyhaza - Szennyvizelvezetesi Agglomeracio", 'cc'=>"HU"],
"HU065"=>['name'=>"Szombathely - Szennyvizelvezetesi Agglomeracio", 'cc'=>"HU"],
"IE002"=>['name'=>"CORK", 'cc'=>"IE"],
"IE006"=>['name'=>"DUBLIN", 'cc'=>"IE"],
"IE010"=>['name'=>"Dundalk", 'cc'=>"IE"],
"IE011"=>['name'=>"Killybegs", 'cc'=>"IE"],
"IS001"=>['name'=>"REYKJAVIK", 'cc'=>"IS"],
"IT009"=>['name'=>"CERVIA", 'cc'=>"IT"],
"IT019"=>['name'=>"ANCONA", 'cc'=>"IT"],
"IT035"=>['name'=>"AVERSA", 'cc'=>"IT"],
"IT042"=>['name'=>"BARI", 'cc'=>"IT"],
"IT048"=>['name'=>"BERGAMO", 'cc'=>"IT"],
"IT054"=>['name'=>"BOLOGNA", 'cc'=>"IT"],
"IT058"=>['name'=>"BRESCIA", 'cc'=>"IT"],
"IT063"=>['name'=>"CAGLIARI", 'cc'=>"IT"],
"IT084"=>['name'=>"CASERTA", 'cc'=>"IT"],
"IT093"=>['name'=>"CATANIA", 'cc'=>"IT"],
"IT123"=>['name'=>"COMO", 'cc'=>"IT"],
"IT131"=>['name'=>"COSENZA", 'cc'=>"IT"],
"IT152"=>['name'=>"FERRARA", 'cc'=>"IT"],
"IT154"=>['name'=>"FIRENZE", 'cc'=>"IT"],
"IT155"=>['name'=>"FOGGIA", 'cc'=>"IT"],
"IT170"=>['name'=>"GALLARATE", 'cc'=>"IT"],
"IT174"=>['name'=>"GENOVA", 'cc'=>"IT"],
"IT200"=>['name'=>"LA SPEZIA", 'cc'=>"IT"],
"IT213"=>['name'=>"LIVORNO", 'cc'=>"IT"],
"IT215"=>['name'=>"LUCCA", 'cc'=>"IT"],
"IT231"=>['name'=>"MASSA", 'cc'=>"IT"],
"IT239"=>['name'=>"MESSINA", 'cc'=>"IT"],
"IT240"=>['name'=>"MILANO", 'cc'=>"IT"],
"IT246"=>['name'=>"MODENA", 'cc'=>"IT"],
"IT267"=>['name'=>"NAPOLI", 'cc'=>"IT"],
"IT270"=>['name'=>"FORLÌ", 'cc'=>"IT"],
"IT293"=>['name'=>"PADOVA", 'cc'=>"IT"],
"IT295"=>['name'=>"PALERMO", 'cc'=>"IT"],
"IT298"=>['name'=>"PARMA", 'cc'=>"IT"],
"IT302"=>['name'=>"PERUGIA", 'cc'=>"IT"],
"IT304"=>['name'=>"PESCARA", 'cc'=>"IT"],
"IT311"=>['name'=>"PISA", 'cc'=>"IT"],
"IT312"=>['name'=>"PISTOIA", 'cc'=>"IT"],
"IT327"=>['name'=>"PRATO", 'cc'=>"IT"],
"IT335"=>['name'=>"REGGIO DI CALABRIA", 'cc'=>"IT"],
"IT336"=>['name'=>"REGGIO NELL'EMILIA", 'cc'=>"IT"],
"IT342"=>['name'=>"RIMINI", 'cc'=>"IT"],
"IT344"=>['name'=>"ROMA", 'cc'=>"IT"],
"IT353"=>['name'=>"SALERNO", 'cc'=>"IT"],
"IT395"=>['name'=>"SIRACUSA", 'cc'=>"IT"],
"IT402"=>['name'=>"TARANTO", 'cc'=>"IT"],
"IT410"=>['name'=>"TORINO", 'cc'=>"IT"],
"IT419"=>['name'=>"TREVISO", 'cc'=>"IT"],
"IT421"=>['name'=>"TRIESTE", 'cc'=>"IT"],
"IT423"=>['name'=>"UDINE", 'cc'=>"IT"],
"IT426"=>['name'=>"VARESE", 'cc'=>"IT"],
"IT434"=>['name'=>"VERONA", 'cc'=>"IT"],
"IT437"=>['name'=>"VICENZA", 'cc'=>"IT"],
"IT737"=>['name'=>"MESTRE", 'cc'=>"IT"],
"IT789"=>['name'=>"COMACCHIO", 'cc'=>"IT"],
"IT945"=>['name'=>"RAVENNA", 'cc'=>"IT"],
"IT946"=>['name'=>"PIACENZA", 'cc'=>"IT"],
"LT002"=>['name'=>"KAUNAS", 'cc'=>"LT"],
"LT003"=>['name'=>"KLAIPEDA", 'cc'=>"LT"],
"LT004"=>['name'=>"PANEVEZYS", 'cc'=>"LT"],
"LT005"=>['name'=>"SIAULIAI", 'cc'=>"LT"],
"LT006"=>['name'=>"VILNIUS", 'cc'=>"LT"],
"LU002"=>['name'=>"LUXEMBOURG", 'cc'=>"LU"],
"LV001"=>['name'=>"DAUGAVPILS", 'cc'=>"LV"],
"LV003"=>['name'=>"LIEPAJA", 'cc'=>"LV"],
"LV004"=>['name'=>"RIGA", 'cc'=>"LV"],
"MT001"=>['name'=>"VALLETTA", 'cc'=>"MT"],
"NLBG01033"=>['name'=>"GRONINGEN", 'cc'=>"NL"],
"NLBG05013"=>['name'=>"ENSCHEDE", 'cc'=>"NL"],
"NLBG05017"=>['name'=>"HENGELO", 'cc'=>"NL"],
"NLBG07020"=>['name'=>"ARNHEM", 'cc'=>"NL"],
"NLBG08011"=>['name'=>"HARDERWIJK", 'cc'=>"NL"],
"NLBG09024"=>['name'=>"NIJMEGEN", 'cc'=>"NL"],
"NLBG10009"=>['name'=>"UTRECHT", 'cc'=>"NL"],
"NLBG10027"=>['name'=>"AMERSFOORT", 'cc'=>"NL"],
"NLBG12004"=>['name'=>"BEVERWIJK", 'cc'=>"NL"],
"NLBG15005"=>['name'=>"'S-GRAVENHAGE", 'cc'=>"NL"],
"NLBG17061"=>['name'=>"ROTTERDAM", 'cc'=>"NL"],
"NLBG25017"=>['name'=>"BREDA", 'cc'=>"NL"],
"NLBG25021"=>['name'=>"BERGEN OP ZOOM", 'cc'=>"NL"],
"NLBG27003"=>['name'=>"EINDHOVEN", 'cc'=>"NL"],
"NLBG27008"=>['name'=>"'S HERTOGENBOSCH", 'cc'=>"NL"],
"NLBG28003"=>['name'=>"HELMOND", 'cc'=>"NL"],
"NLBG28005"=>['name'=>"VEGHEL", 'cc'=>"NL"],
"NLBG29002"=>['name'=>"OSS", 'cc'=>"NL"],
"NLBG30015"=>['name'=>"HEERLEN", 'cc'=>"NL"],
"NLBG30017"=>['name'=>"VENLO", 'cc'=>"NL"],
"NLBG31003"=>['name'=>"AMSTERDAM", 'cc'=>"NL"],
"NLBG32001"=>['name'=>"TILBURG", 'cc'=>"NL"],
"NO003"=>['name'=>"BERGEN", 'cc'=>"NO"],
"NO017"=>['name'=>"OSLO", 'cc'=>"NO"],
"NO025"=>['name'=>"STAVANGER", 'cc'=>"NO"],
"NO029"=>['name'=>"TRONDHEIM", 'cc'=>"NO"],
"PL008"=>['name'=>"BIALYSTOK", 'cc'=>"PL"],
"PL010"=>['name'=>"BIELSKO-BIALA", 'cc'=>"PL"],
"PL017"=>['name'=>"BYDGOSZCZ", 'cc'=>"PL"],
"PL018"=>['name'=>"BYTOM", 'cc'=>"PL"],
"PL023"=>['name'=>"CHORZOW", 'cc'=>"PL"],
"PL027"=>['name'=>"CZESTOCHOWA", 'cc'=>"PL"],
"PL028"=>['name'=>"DABROWA GORNICZA", 'cc'=>"PL"],
"PL032"=>['name'=>"ELBLAG", 'cc'=>"PL"],
"PL034"=>['name'=>"GDANSK", 'cc'=>"PL"],
"PL035"=>['name'=>"GDYNIA", 'cc'=>"PL"],
"PL037"=>['name'=>"GLIWICE", 'cc'=>"PL"],
"PL042"=>['name'=>"GORZOWWIELKOPOLSKI", 'cc'=>"PL"],
"PL047"=>['name'=>"GRUDZIADZ", 'cc'=>"PL"],
"PL059"=>['name'=>"KALISZ", 'cc'=>"PL"],
"PL061"=>['name'=>"KATOWICE", 'cc'=>"PL"],
"PL064"=>['name'=>"KIELCE", 'cc'=>"PL"],
"PL073"=>['name'=>"KOSZALIN", 'cc'=>"PL"],
"PL074"=>['name'=>"KRAKOW", 'cc'=>"PL"],
"PL084"=>['name'=>"LEGNICA", 'cc'=>"PL"],
"PL086"=>['name'=>"LODZ", 'cc'=>"PL"],
"PL093"=>['name'=>"LUBLIN", 'cc'=>"PL"],
"PL113"=>['name'=>"OLSZTYN", 'cc'=>"PL"],
"PL115"=>['name'=>"OPOLE", 'cc'=>"PL"],
"PL128"=>['name'=>"PLOCK", 'cc'=>"PL"],
"PL130"=>['name'=>"POZNAN", 'cc'=>"PL"],
"PL138"=>['name'=>"RADOM", 'cc'=>"PL"],
"PL141"=>['name'=>"RYBNIK", 'cc'=>"PL"],
"PL142"=>['name'=>"RZESZOW", 'cc'=>"PL"],
"PL150"=>['name'=>"SLUPSK", 'cc'=>"PL"],
"PL153"=>['name'=>"SOSNOWIEC", 'cc'=>"PL"],
"PL167"=>['name'=>"SZCZECIN", 'cc'=>"PL"],
"PL171"=>['name'=>"TARNOW", 'cc'=>"PL"],
"PL176"=>['name'=>"TORUN", 'cc'=>"PL"],
"PL179"=>['name'=>"TYCHY", 'cc'=>"PL"],
"PL181"=>['name'=>"WALBRZYCH", 'cc'=>"PL"],
"PL183"=>['name'=>"WARSZAWA", 'cc'=>"PL"],
"PL186"=>['name'=>"WLOCLAWEK", 'cc'=>"PL"],
"PL189"=>['name'=>"WROCLAW", 'cc'=>"PL"],
"PL192"=>['name'=>"ZABRZE", 'cc'=>"PL"],
"PL201"=>['name'=>"ZIELONA GORA", 'cc'=>"PL"],
"PL206"=>['name'=>"RUDA SLASKA", 'cc'=>"PL"],
"PL207"=>['name'=>"JASTRZEBIE-ZDROJ", 'cc'=>"PL"],
"PT004"=>['name'=>"AMADORA", 'cc'=>"PT"],
"PT018"=>['name'=>"LISBOA", 'cc'=>"PT"],
"PT022"=>['name'=>"PORTO", 'cc'=>"PT"],
"PTAGL016"=>['name'=>"Barreiro/Moita", 'cc'=>"PT"],
"PTAGL048"=>['name'=>"Coimbra", 'cc'=>"PT"],
"PTAGL049"=>['name'=>"Costa de Aveiro", 'cc'=>"PT"],
"PTAGL060"=>['name'=>"Alcanena", 'cc'=>"PT"],
"PTAGL070"=>['name'=>"Costa do Estoril", 'cc'=>"PT"],
"PTAGL085"=>['name'=>"Setubal", 'cc'=>"PT"],
"PTAGL105"=>['name'=>"Matosinhos", 'cc'=>"PT"],
"PTAGL116"=>['name'=>"Santo Tirso", 'cc'=>"PT"],
"PTAGL117"=>['name'=>"Gaia/Litoral", 'cc'=>"PT"],
"PTAGL131"=>['name'=>"Loures/Frielas", 'cc'=>"PT"],
"PTAGL306"=>['name'=>"Famalicao/Sto Tirso/Trofa", 'cc'=>"PT"],
"RO005"=>['name'=>"ARAD", 'cc'=>"RO"],
"RO006"=>['name'=>"BACAU", 'cc'=>"RO"],
"RO007"=>['name'=>"BAIA MARE", 'cc'=>"RO"],
"RO016"=>['name'=>"BOTOSANI", 'cc'=>"RO"],
"RO018"=>['name'=>"BRAILA", 'cc'=>"RO"],
"RO019"=>['name'=>"BRASOV", 'cc'=>"RO"],
"RO020"=>['name'=>"BUCURESTI", 'cc'=>"RO"],
"RO022"=>['name'=>"BUZAU", 'cc'=>"RO"],
"RO033"=>['name'=>"CLUJ-NAPOCA", 'cc'=>"RO"],
"RO036"=>['name'=>"CONSTANTA", 'cc'=>"RO"],
"RO038"=>['name'=>"CRAIOVA", 'cc'=>"RO"],
"RO049"=>['name'=>"GALATI", 'cc'=>"RO"],
"RO056"=>['name'=>"IASI", 'cc'=>"RO"],
"RO070"=>['name'=>"ORADEA", 'cc'=>"RO"],
"RO075"=>['name'=>"PIATRA NEAMT", 'cc'=>"RO"],
"RO076"=>['name'=>"PITESTI", 'cc'=>"RO"],
"RO077"=>['name'=>"PLOIESTI", 'cc'=>"RO"],
"RO082"=>['name'=>"RIMNICU VILCEA", 'cc'=>"RO"],
"RO087"=>['name'=>"SATU MARE", 'cc'=>"RO"],
"RO090"=>['name'=>"SIBIU", 'cc'=>"RO"],
"RO094"=>['name'=>"SUCEAVA", 'cc'=>"RO"],
"RO096"=>['name'=>"TIMISOARA", 'cc'=>"RO"],
"RO100"=>['name'=>"TIRGU MURES", 'cc'=>"RO"],
"RO112"=>['name'=>"DROBETA-TURNU SEVERIN", 'cc'=>"RO"],
"SE008"=>['name'=>"GOETEBORG", 'cc'=>"SE"],
"SE026"=>['name'=>"MALMOE", 'cc'=>"SE"],
"SE036"=>['name'=>"STOCKHOLM", 'cc'=>"SE"],
"SE044"=>['name'=>"UPPSALA", 'cc'=>"SE"],
"SE054"=>['name'=>"VAESTERAAS", 'cc'=>"SE"],
"SE_AGGLO_1004"=>['name'=>"HELSINGBORG", 'cc'=>"SE"],
"SE_AGGLO_1005"=>['name'=>"LINKOEPING", 'cc'=>"SE"],
"SI009"=>['name'=>"LJUBLJANA", 'cc'=>"SI"],
"SI010"=>['name'=>"MARIBOR", 'cc'=>"SI"],
"SK002"=>['name'=>"NITRA", 'cc'=>"SK"],
"SK004"=>['name'=>"BRATISLAVA", 'cc'=>"SK"],
"SK006"=>['name'=>"LIPTOVSKY MIKULAS", 'cc'=>"SK"],
"SK008"=>['name'=>"RUZOMBEROK", 'cc'=>"SK"],
"SK010"=>['name'=>"ZILINA", 'cc'=>"SK"],
"SK012"=>['name'=>"KOSICE", 'cc'=>"SK"],
"UK002"=>['name'=>"ABERDEEN", 'cc'=>"UK"],
"UK008"=>['name'=>"ALDERSHOT", 'cc'=>"UK"],
"UK026"=>['name'=>"BASILDON", 'cc'=>"UK"],
"UK031"=>['name'=>"BELFAST", 'cc'=>"UK"],
"UK034"=>['name'=>"BIRKENHEAD", 'cc'=>"UK"],
"UK038"=>['name'=>"BLACKBURN/DARWEN", 'cc'=>"UK"],
"UK039"=>['name'=>"BLACKPOOL", 'cc'=>"UK"],
"UK044"=>['name'=>"BOURNEMOUTH", 'cc'=>"UK"],
"UK050"=>['name'=>"BRIDGEND", 'cc'=>"UK"],
"UK051"=>['name'=>"BRIGHTON/WORTHING/LITTLEHAMPTON", 'cc'=>"UK"],
"UK052"=>['name'=>"BRISTOL", 'cc'=>"UK"],
"UK055"=>['name'=>"BURNLEY/NELSON", 'cc'=>"UK"],
"UK062"=>['name'=>"CAMBRIDGE", 'cc'=>"UK"],
"UK066"=>['name'=>"CARDIFF", 'cc'=>"UK"],
"UK070"=>['name'=>"CHELTENHAM/CHARLTON KINGS", 'cc'=>"UK"],
"UK073"=>['name'=>"CHESTERFIELD/STAVELEY", 'cc'=>"UK"],
"UK087"=>['name'=>"COVENTRY/BEDWORTH", 'cc'=>"UK"],
"UK089"=>['name'=>"CRAWLEY", 'cc'=>"UK"],
"UK095"=>['name'=>"DEARNE VALLEY", 'cc'=>"UK"],
"UK097"=>['name'=>"DERBY", 'cc'=>"UK"],
"UK098"=>['name'=>"DONCASTER", 'cc'=>"UK"],
"UK103"=>['name'=>"DUNDEE", 'cc'=>"UK"],
"UK110"=>['name'=>"EDINBURGH", 'cc'=>"UK"],
"UK122"=>['name'=>"GLASGOW", 'cc'=>"UK"],
"UK125"=>['name'=>"GLOUCESTER", 'cc'=>"UK"],
"UK132"=>['name'=>"LONDON", 'cc'=>"UK"],
"UK133"=>['name'=>"GREATER MANCHESTER", 'cc'=>"UK"],
"UK135"=>['name'=>"GRIMSBY/CLEETHORPES", 'cc'=>"UK"],
"UK143"=>['name'=>"HASTINGS/BEXHILL", 'cc'=>"UK"],
"UK149"=>['name'=>"HIGH WYCOMBE", 'cc'=>"UK"],
"UK159"=>['name'=>"IPSWICH", 'cc'=>"UK"],
"UK168"=>['name'=>"KINGSTON-UPON-HULL", 'cc'=>"UK"],
"UK173"=>['name'=>"LEICESTER", 'cc'=>"UK"],
"UK178"=>['name'=>"LIVERPOOL", 'cc'=>"UK"],
"UK184"=>['name'=>"LUTON/DUNSTABLE", 'cc'=>"UK"],
"UK190"=>['name'=>"MANSFIELD", 'cc'=>"UK"],
"UK193"=>['name'=>"MILTON KEYNES", 'cc'=>"UK"],
"UK200"=>['name'=>"NEWPORT", 'cc'=>"UK"],
"UK204"=>['name'=>"NORTHAMPTON", 'cc'=>"UK"],
"UK207"=>['name'=>"NORWICH", 'cc'=>"UK"],
"UK208"=>['name'=>"NOTTINGHAM", 'cc'=>"UK"],
"UK211"=>['name'=>"OXFORD", 'cc'=>"UK"],
"UK214"=>['name'=>"PETERBOROUGH", 'cc'=>"UK"],
"UK216"=>['name'=>"PLYMOUTH", 'cc'=>"UK"],
"UK221"=>['name'=>"PORTSMOUTH", 'cc'=>"UK"],
"UK223"=>['name'=>"PRESTON", 'cc'=>"UK"],
"UK224"=>['name'=>"READING/WOKINGHAM", 'cc'=>"UK"],
"UK238"=>['name'=>"SHEFFIELD", 'cc'=>"UK"],
"UK241"=>['name'=>"SLOUGH", 'cc'=>"UK"],
"UK242"=>['name'=>"SOUTHAMPTON/EASTLEIGH", 'cc'=>"UK"],
"UK243"=>['name'=>"SOUTHEND", 'cc'=>"UK"],
"UK244"=>['name'=>"SOUTHPORT/FORMBY", 'cc'=>"UK"],
"UK246"=>['name'=>"ST. ALBANS/HATFIELD", 'cc'=>"UK"],
"UK256"=>['name'=>"SUNDERLAND/WHITBURN", 'cc'=>"UK"],
"UK259"=>['name'=>"SWANSEA", 'cc'=>"UK"],
"UK260"=>['name'=>"SWINDON", 'cc'=>"UK"],
"UK263"=>['name'=>"TEESSIDE", 'cc'=>"UK"],
"UK267"=>['name'=>"THANET", 'cc'=>"UK"],
"UK268"=>['name'=>"THE MEDWAY TOWNS", 'cc'=>"UK"],
"UK269"=>['name'=>"THE POTTERIES", 'cc'=>"UK"],
"UK271"=>['name'=>"TORBAY", 'cc'=>"UK"],
"UK274"=>['name'=>"TYNESIDE", 'cc'=>"UK"],
"UK275"=>['name'=>"WARRINGTON", 'cc'=>"UK"],
"UK279"=>['name'=>"WEST MIDLANDS", 'cc'=>"UK"],
"UK280"=>['name'=>"WEST YORKSHIRE", 'cc'=>"UK"],
"UK287"=>['name'=>"WIGAN", 'cc'=>"UK"],
"UK301"=>['name'=>"YORK", 'cc'=>"UK"],
"UK319"=>['name'=>"TELFORD", 'cc'=>"UK"],
"UK320"=>['name'=>"Wellingborough", 'cc'=>"UK"],
"UK321"=>['name'=>"Tilbury", 'cc'=>"UK"],
"UK322"=>['name'=>"Hertford", 'cc'=>"UK"],
"UK323"=>['name'=>"Burton", 'cc'=>"UK"],
"UK324"=>['name'=>"Bedford", 'cc'=>"UK"],
"UK325"=>['name'=>"Fareham", 'cc'=>"UK"],
"UK326"=>['name'=>"Poole", 'cc'=>"UK"],
"UK327"=>['name'=>"Levenmouth", 'cc'=>"UK"],
"UK328"=>['name'=>"Meadowhead", 'cc'=>"UK"],
];

$GLOBALS['nuts'] = [
"BE"=>"BELGIQUE-BELGIË","BE1"=>"RÉGION DE BRUXELLES-CAPITALE/BRUSSELS HOOFDSTEDELIJK GEWEST","BE2"=>"VLAAMS GEWEST","BE3"=>"RÉGION WALLONNE","BEZ"=>"EXTRA-REGIO NUTS 1","BG"=>"БЪЛГАРИЯ (BULGARIA)","BG3"=>"СЕВЕРНА И ЮГОИЗТОЧНА БЪЛГАРИЯ (SEVERNA I YUGOIZTOCHNA BULGARIA)","BG4"=>"ЮГОЗАПАДНА И ЮЖНА ЦЕНТРАЛНА БЪЛГАРИЯ (YUGOZAPADNA I YUZHNA TSENTRALNA BULGARIA)","BGZ"=>"EXTRA-REGIO NUTS 1","CZ"=>"ČESKÁ REPUBLIKA","CZ0"=>"ČESKÁ REPUBLIKA","CZZ"=>"EXTRA-REGIO NUTS 1","DK"=>"DANMARK","DK0"=>"DANMARK","DKZ"=>"EXTRA-REGIO NUTS 1","DE"=>"DEUTSCHLAND","DE1"=>"BADEN-WÜRTTEMBERG","DE2"=>"BAYERN","DE3"=>"BERLIN","DE4"=>"BRANDENBURG","DE5"=>"BREMEN","DE6"=>"HAMBURG","DE7"=>"HESSEN","DE8"=>"MECKLENBURG-VORPOMMERN","DE9"=>"NIEDERSACHSEN","DEA"=>"NORDRHEIN-WESTFALEN","DEB"=>"RHEINLAND-PFALZ","DEC"=>"SAARLAND","DED"=>"SACHSEN","DEE"=>"SACHSEN-ANHALT","DEF"=>"SCHLESWIG-HOLSTEIN","DEG"=>"THÜRINGEN","DEZ"=>"EXTRA-REGIO NUTS 1","EE"=>"EESTI","EE0"=>"EESTI","EEZ"=>"EXTRA-REGIO NUTS 1","IE"=>"IRELAND","IE0"=>"IRELAND","IEZ"=>"EXTRA-REGIO NUTS 1","EL"=>"ΕΛΛΑΔΑ (ELLADA)","EL3"=>"ATTIKΗ (ATTIKI)","EL4"=>"NΗΣΙΑ ΑΙΓΑΙΟΥ, KΡΗΤΗ (NISIA AIGAIOU, KRITI)","EL5"=>"ΒΟΡΕΙΑ ΕΛΛΑΔΑ (VOREIA ELLADA)","EL6"=>"ΚΕΝΤΡΙΚΗ ΕΛΛΑΔΑ (KENTRIKI ELLADA)","ELZ"=>"EXTRA-REGIO NUTS 1","ES"=>"ESPAÑA","ES1"=>"NOROESTE","ES2"=>"NORESTE","ES3"=>"COMUNIDAD DE MADRID","ES4"=>"CENTRO (ES)","ES5"=>"ESTE","ES6"=>"SUR","ES7"=>"CANARIAS","ESZ"=>"EXTRA-REGIO NUTS 1","FR"=>"FRANCE","FR1"=>"ILE-DE-FRANCE","FRB"=>"CENTRE — VAL DE LOIRE","FRC"=>"BOURGOGNE-FRANCHE-COMTÉ","FRD"=>"NORMANDIE","FRE"=>"NORD-PAS DE CALAIS-PICARDIE","FRF"=>"ALSACE-CHAMPAGNE-ARDENNE-LORRAINE","FRG"=>"PAYS DE LA LOIRE","FRH"=>"BRETAGNE","FRI"=>"AQUITAINE-LIMOUSIN-POITOU-CHARENTES","FRJ"=>"LANGUEDOC-ROUSSILLON-MIDI-PYRÉNÉES","FRK"=>"AUVERGNE-RHÔNE-ALPES","FRL"=>"PROVENCE-ALPES-CÔTE D’AZUR","FRM"=>"CORSE","FRY"=>"RUP FR — RÉGIONS ULTRAPÉRIPHÉRIQUES FRANÇAISES","FRZ"=>"EXTRA-REGIO NUTS 1","HR"=>"HRVATSKA","HR0"=>"HRVATSKA","HRZ"=>"EXTRA-REGIO NUTS 1","IT"=>"ITALIA","ITC"=>"NORD-OVEST","ITF"=>"SUD","ITG"=>"ISOLE","ITH"=>"NORD-EST","ITI"=>"CENTRO (IT)","ITZ"=>"EXTRA-REGIO NUTS 1","CY"=>"ΚΥΠΡΟΣ (KYPROS)","CY0"=>"ΚΥΠΡΟΣ (Kypros)","CYZ"=>"EXTRA-REGIO NUTS 1","LV"=>"LATVIJA","LV0"=>"LATVIJA","LVZ"=>"EXTRA-REGIO NUTS 1","LT"=>"LIETUVA","LT0"=>"LIETUVA","LTZ"=>"EXTRA-REGIO NUTS 1","LU"=>"LUXEMBOURG","LU0"=>"LUXEMBOURG","LUZ"=>"EXTRA-REGIO NUTS 1","HU"=>"MAGYARORSZÁG","HU1"=>"KÖZÉP-MAGYARORSZÁG","HU2"=>"DUNÁNTÚL","HU3"=>"ALFÖLD ÉS ÉSZAK","HUZ"=>"EXTRA-REGIO NUTS 1","MT"=>"MALTA","MT0"=>"MALTA","MTZ"=>"EXTRA-REGIO NUTS 1","NL"=>"NEDERLAND","NL1"=>"NOORD-NEDERLAND","NL2"=>"OOST-NEDERLAND","NL3"=>"WEST-NEDERLAND","NL4"=>"ZUID-NEDERLAND","NLZ"=>"EXTRA-REGIO NUTS 1","AT"=>"ÖSTERREICH","AT1"=>"OSTÖSTERREICH","AT2"=>"SÜDÖSTERREICH","AT3"=>"WESTÖSTERREICH","ATZ"=>"EXTRA-REGIO NUTS 1","PL"=>"POLSKA","PL2"=>"MAKROREGION POŁUDNIOWY","PL4"=>"MAKROREGION PÓŁNOCNO-ZACHODNI","PL5"=>"MAKROREGION POŁUDNIOWO-ZACHODNI","PL6"=>"MAKROREGION PÓŁNOCNY","PL7"=>"MAKROREGION CENTRALNY","PL8"=>"MAKROREGION WSCHODNI","PL9"=>"MAKROREGION WOJEWÓDZTWO MAZOWIECKIE","PLZ"=>"EXTRA-REGIO NUTS 1","PT"=>"PORTUGAL","PT1"=>"CONTINENTE","PT2"=>"REGIÃO AUTÓNOMA DOS AÇORES","PT3"=>"REGIÃO AUTÓNOMA DA MADEIRA","PTZ"=>"EXTRA-REGIO NUTS 1","RO"=>"ROMÂNIA","RO1"=>"MACROREGIUNEA UNU","RO2"=>"MACROREGIUNEA DOI","RO3"=>"MACROREGIUNEA TREI","RO4"=>"MACROREGIUNEA PATRU","ROZ"=>"EXTRA-REGIO NUTS 1","SI"=>"SLOVENIJA","SI0"=>"SLOVENIJA","SIZ"=>"EXTRA-REGIO NUTS 1","SK"=>"SLOVENSKO","SK0"=>"SLOVENSKO","SKZ"=>"EXTRA-REGIO NUTS 1","FI"=>"SUOMI / FINLAND","FI1"=>"MANNER-SUOMI","FI2"=>"ÅLAND","FIZ"=>"EXTRA-REGIO NUTS 1","SE"=>"SVERIGE","SE1"=>"ÖSTRA SVERIGE","SE2"=>"SÖDRA SVERIGE","SE3"=>"NORRA SVERIGE","SEZ"=>"EXTRA-REGIO NUTS 1","UK"=>"UNITED KINGDOM","UKC"=>"NORTH EAST (ENGLAND)","UKD"=>"NORTH WEST (ENGLAND)","UKE"=>"YORKSHIRE AND THE HUMBER","UKF"=>"EAST MIDLANDS (ENGLAND)","UKG"=>"WEST MIDLANDS (ENGLAND)","UKH"=>"EAST OF ENGLAND","UKI"=>"LONDON","UKJ"=>"SOUTH EAST (ENGLAND)","UKK"=>"SOUTH WEST (ENGLAND)","UKL"=>"WALES","UKM"=>"SCOTLAND","UKN"=>"NORTHERN IRELAND","UKZ"=>"EXTRA-REGIO NUTS 1",""=>"NUTS LEVEL 2 ","BE"=>"BELGIQUE-BELGIË","BE10"=>"Région de Bruxelles-Capitale/ Brussels Hoofdstedelijk Gewest","BE21"=>"Prov. Antwerpen","BE22"=>"Prov. Limburg (BE)","BE23"=>"Prov. Oost-Vlaanderen","BE24"=>"Prov. Vlaams-Brabant","BE25"=>"Prov. West-Vlaanderen","BE31"=>"Prov. Brabant Wallon","BE32"=>"Prov. Hainaut","BE33"=>"Prov. Liège","BE34"=>"Prov. Luxembourg (BE)","BE35"=>"Prov. Namur","BEZZ"=>"Extra-Regio NUTS 2","BG"=>"БЪЛГАРИЯ (BULGARIA)","BG31"=>"Северозападен (Severozapaden)","BG32"=>"Северен централен (Severen tsentralen)","BG33"=>"Североизточен (Severoiztochen)","BG34"=>"Югоизточен (Yugoiztochen)","BG41"=>"Югозападен (Yugozapaden)","BG42"=>"Южен централен (Yuzhen tsentralen)","BGZZ"=>"Extra-Regio NUTS 2","CZ"=>"ČESKÁ REPUBLIKA","CZ01"=>"Praha","CZ02"=>"Střední Čechy","CZ03"=>"Jihozápad","CZ04"=>"Severozápad","CZ05"=>"Severovýchod","CZ06"=>"Jihovýchod","CZ07"=>"Střední Morava","CZ08"=>"Moravskoslezsko","CZZZ"=>"Extra-Regio NUTS 2","DK"=>"DANMARK","DK01"=>"Hovedstaden","DK02"=>"Sjælland","DK03"=>"Syddanmark","DK04"=>"Midtjylland","DK05"=>"Nordjylland","DKZZ"=>"Extra-Regio NUTS 2","DE"=>"DEUTSCHLAND","DE11"=>"Stuttgart","DE12"=>"Karlsruhe","DE13"=>"Freiburg","DE14"=>"Tübingen","DE21"=>"Oberbayern","DE22"=>"Niederbayern","DE23"=>"Oberpfalz","DE24"=>"Oberfranken","DE25"=>"Mittelfranken","DE26"=>"Unterfranken","DE27"=>"Schwaben","DE30"=>"Berlin","DE40"=>"Brandenburg","DE50"=>"Bremen","DE60"=>"Hamburg","DE71"=>"Darmstadt","DE72"=>"Gießen","DE73"=>"Kassel","DE80"=>"Mecklenburg-Vorpommern","DE91"=>"Braunschweig","DE92"=>"Hannover","DE93"=>"Lüneburg",""=>"","DE94"=>"Weser-Ems","DEA1"=>"Düsseldorf","DEA2"=>"Köln","DEA3"=>"Münster","DEA4"=>"Detmold","DEA5"=>"Arnsberg","DEB1"=>"Koblenz","DEB2"=>"Trier","DEB3"=>"Rheinhessen-Pfalz","DEC0"=>"Saarland","DED2"=>"Dresden","DED4"=>"Chemnitz","DED5"=>"Leipzig","DEE0"=>"Sachsen-Anhalt","DEF0"=>"Schleswig-Holstein","DEG0"=>"Thüringen","DEZZ"=>"Extra-Regio NUTS 2","EE"=>"EESTI","EE00"=>"Eesti","EEZZ"=>"Extra-Regio NUTS 2","IE"=>"IRELAND","IE04"=>"Northern and Western","IE05"=>"Southern","IE06"=>"Eastern and Midland","IEZZ"=>"Extra-Regio NUTS 2","EL"=>"ΕΛΛΑΔΑ (ELLADA)","EL30"=>"Aττική (Attiki)","EL41"=>"Βόρειο Αιγαίο (Voreio Aigaio)","EL42"=>"Νότιο Αιγαίο (Notio Aigaio)","EL43"=>"Κρήτη (Kriti)","EL51"=>"Aνατολική Μακεδονία, Θράκη (Anatoliki Makedonia, Thraki)","EL52"=>"Κεντρική Μακεδονία (Kentriki Makedonia)","EL53"=>"Δυτική Μακεδονία (Dytiki Makedonia)","EL54"=>"Ήπειρος (Ipeiros)","EL61"=>"Θεσσαλία (Thessalia)","EL62"=>"Ιόνια Νησιά (Ionia Nisia)","EL63"=>"Δυτική Ελλάδα (Dytiki Ellada)","EL64"=>"Στερεά Ελλάδα (Sterea Ellada)","EL65"=>"Πελοπόννησος (Peloponnisos)","ELZZ"=>"Extra-Regio NUTS 2","ES"=>"ESPAÑA","ES11"=>"Galicia","ES12"=>"Principado de Asturias","ES13"=>"Cantabria","ES21"=>"País Vasco","ES22"=>"Comunidad Foral de Navarra","ES23"=>"La Rioja","ES24"=>"Aragón","ES30"=>"Comunidad de Madrid","ES41"=>"Castilla y León","ES42"=>"Castilla-La Mancha","ES43"=>"Extremadura","ES51"=>"Cataluña","ES52"=>"Comunidad Valenciana","ES53"=>"Illes Balears","ES61"=>"Andalucía","ES62"=>"Región de Murcia","ES63"=>"Ciudad Autónoma de Ceuta","ES64"=>"Ciudad Autónoma de Melilla","ES70"=>"Canarias","ESZZ"=>"Extra-Regio NUTS 2","FR"=>"FRANCE","FR10"=>"Ile-de-France","FRB0"=>"Centre — Val de Loire","FRC1"=>"Bourgogne","FRC2"=>"Franche-Comté","FRD1"=>"Basse-Normandie","FRD2"=>"Haute-Normandie","FRE1"=>"Nord-Pas de Calais","FRE2"=>"Picardie","FRF1"=>"Alsace","FRF2"=>"Champagne-Ardenne","FRF3"=>"Lorraine","FRG0"=>"Pays de la Loire","FRH0"=>"Bretagne","FRI1"=>"Aquitaine","FRI2"=>"Limousin","FRI3"=>"Poitou-Charentes","FRJ1"=>"Languedoc-Roussillon","FRJ2"=>"Midi-Pyrénées","FRK1"=>"Auvergne","FRK2"=>"Rhône-Alpes","FRL0"=>"Provence-Alpes-Côte d’Azur","FRM0"=>"Corse","FRY1"=>"Guadeloupe","FRY2"=>"Martinique","FRY3"=>"Guyane","FRY4"=>"La Réunion","FRY5"=>"Mayotte","FRZZ"=>"Extra-Regio NUTS 2","HR"=>"HRVATSKA","HR03"=>"Jadranska Hrvatska","HR04"=>"Kontinentalna Hrvatska","HRZZ"=>"Extra-Regio NUTS 2","IT"=>"ITALIA","ITC1"=>"Piemonte","ITC2"=>"Valle d’Aosta/Vallée d’Aoste","ITC3"=>"Liguria","ITC4"=>"Lombardia","ITF1"=>"Abruzzo","ITF2"=>"Molise","ITF3"=>"Campania","ITF4"=>"Puglia","ITF5"=>"Basilicata","ITF6"=>"Calabria","ITG1"=>"Sicilia","ITG2"=>"Sardegna","ITH1"=>"Provincia Autonoma di Bolzano/Bozen","ITH2"=>"Provincia Autonoma di Trento","ITH3"=>"Veneto","ITH4"=>"Friuli-Venezia Giulia","ITH5"=>"Emilia-Romagna","ITI1"=>"Toscana","ITI2"=>"Umbria","ITI3"=>"Marche","ITI4"=>"Lazio","ITZZ"=>"Extra-Regio NUTS 2","CY"=>"ΚΥΠΡΟΣ (KYPROS)","CY00"=>"Κύπρος (Kypros)","CYZZ"=>"Extra-Regio NUTS 2","LV"=>"LATVIJA","LV00"=>"Latvija","LVZZ"=>"Extra-Regio NUTS 2","LT"=>"LIETUVA","LT01"=>"Sostinės regionas","LT02"=>"Vidurio ir vakarų Lietuvos regionas","LTZZ"=>"Extra-Regio NUTS 2","LU"=>"LUXEMBOURG","LU00"=>"Luxembourg","LUZZ"=>"Extra-Regio NUTS 2","HU"=>"MAGYARORSZÁG","HU11"=>"Budapest","HU12"=>"Pest","HU21"=>"Közép-Dunántúl","HU22"=>"Nyugat-Dunántúl","HU23"=>"Dél-Dunántúl","HU31"=>"Észak-Magyarország","HU32"=>"Észak-Alföld","HU33"=>"Dél-Alföld","HUZZ"=>"Extra-Regio NUTS 2","MT"=>"MALTA","MT00"=>"Malta","MTZZ"=>"Extra-Regio NUTS 2","NL"=>"NEDERLAND","NL11"=>"Groningen","NL12"=>"Friesland (NL)","NL13"=>"Drenthe","NL21"=>"Overijssel","NL22"=>"Gelderland","NL23"=>"Flevoland","NL31"=>"Utrecht","NL32"=>"Noord-Holland","NL33"=>"Zuid-Holland","NL34"=>"Zeeland","NL41"=>"Noord-Brabant","NL42"=>"Limburg (NL)","NLZZ"=>"Extra-Regio NUTS 2","AT"=>"ÖSTERREICH","AT11"=>"Burgenland","AT12"=>"Niederösterreich","AT13"=>"Wien","AT21"=>"Kärnten","AT22"=>"Steiermark","AT31"=>"Oberösterreich","AT32"=>"Salzburg","AT33"=>"Tirol","AT34"=>"Vorarlberg","ATZZ"=>"Extra-Regio NUTS 2","PL"=>"POLSKA","PL21"=>"Małopolskie","PL22"=>"Śląskie","PL41"=>"Wielkopolskie","PL42"=>"Zachodniopomorskie","PL43"=>"Lubuskie","PL51"=>"Dolnośląskie","PL52"=>"Opolskie","PL61"=>"Kujawsko-pomorskie","PL62"=>"Warmińsko-mazurskie","PL63"=>"Pomorskie","PL71"=>"Łódzkie","PL72"=>"Świętokrzyskie","PL81"=>"Lubelskie","PL82"=>"Podkarpackie","PL84"=>"Podlaskie","PL91"=>"Warszawski stołeczny","PL92"=>"Mazowiecki regionalny","PLZZ"=>"Extra-Regio NUTS 2","PT"=>"PORTUGAL","PT11"=>"Norte","PT15"=>"Algarve","PT16"=>"Centro (PT)","PT17"=>"Área Metropolitana de Lisboa","PT18"=>"Alentejo","PT20"=>"Região Autónoma dos Açores","PT30"=>"Região Autónoma da Madeira","PTZZ"=>"Extra-Regio NUTS 2","RO"=>"ROMÂNIA","RO11"=>"Nord-Vest","RO12"=>"Centru","RO21"=>"Nord-Est","RO22"=>"Sud-Est","RO31"=>"Sud - Muntenia","RO32"=>"Bucureşti - Ilfov","RO41"=>"Sud-Vest Oltenia","RO42"=>"Vest","ROZZ"=>"Extra-Regio NUTS 2","SI"=>"SLOVENIJA","SI03"=>"Vzhodna Slovenija","SI04"=>"Zahodna Slovenija","SIZZ"=>"Extra-Regio NUTS 2","SK"=>"SLOVENSKO","SK01"=>"Bratislavský kraj","SK02"=>"Západné Slovensko","SK03"=>"Stredné Slovensko","SK04"=>"Východné Slovensko","SKZZ"=>"Extra-Regio NUTS 2","FI"=>"SUOMI / FINLAND","FI19"=>"Länsi-Suomi","FI1B"=>"Helsinki-Uusimaa","FI1C"=>"Etelä-Suomi","FI1D"=>"Pohjois- ja Itä-Suomi","FI20"=>"Åland","FIZZ"=>"Extra-Regio NUTS 2","SE"=>"SVERIGE","SE11"=>"Stockholm","SE12"=>"Östra Mellansverige","SE21"=>"Småland med öarna","SE22"=>"Sydsverige","SE23"=>"Västsverige","SE31"=>"Norra Mellansverige","SE32"=>"Mellersta Norrland","SE33"=>"Övre Norrland","SEZZ"=>"Extra-Regio NUTS 2","UK"=>"UNITED KINGDOM","UKC1"=>"Tees Valley and Durham","UKC2"=>"Northumberland and Tyne and Wear","UKD1"=>"Cumbria","UKD3"=>"Greater Manchester","UKD4"=>"Lancashire","UKD6"=>"Cheshire","UKD7"=>"Merseyside","UKE1"=>"East Yorkshire and Northern Lincolnshire","UKE2"=>"North Yorkshire","UKE3"=>"South Yorkshire","UKE4"=>"West Yorkshire","UKF1"=>"Derbyshire and Nottinghamshire","UKF2"=>"Leicestershire, Rutland and Northamptonshire","UKF3"=>"Lincolnshire","UKG1"=>"Herefordshire, Worcestershire and Warwickshire","UKG2"=>"Shropshire and Staffordshire","UKG3"=>"West Midlands","UKH1"=>"East Anglia","UKH2"=>"Bedfordshire and Hertfordshire","UKH3"=>"Essex","UKI3"=>"Inner London — West","UKI4"=>"Inner London — East","UKI5"=>"Outer London — East and North East","UKI6"=>"Outer London — South","UKI7"=>"Outer London — West and North West","UKJ1"=>"Berkshire, Buckinghamshire and Oxfordshire","UKJ2"=>"Surrey, East and West Sussex","UKJ3"=>"Hampshire and Isle of Wight","UKJ4"=>"Kent","UKK1"=>"Gloucestershire, Wiltshire and Bristol/Bath area","UKK2"=>"Dorset and Somerset","UKK3"=>"Cornwall and Isles of Scilly","UKK4"=>"Devon","UKL1"=>"West Wales and The Valleys","UKL2"=>"East Wales","UKM5"=>"North Eastern Scotland","UKM6"=>"Highlands and Islands","UKM7"=>"Eastern Scotland","UKM8"=>"West Central Scotland","UKM9"=>"Southern Scotland","UKN0"=>"Northern Ireland","UKZZ"=>"Extra-Regio NUTS 2",""=>"NUTS LEVEL 3","BE"=>"BELGIQUE-BELGIË","BE100"=>"Arr. de Bruxelles-Capitale/Arr. van Brussel-Hoofdstad","BE211"=>"Arr. Antwerpen","BE212"=>"Arr. Mechelen","BE213"=>"Arr. Turnhout","BE221"=>"Arr. Hasselt","BE222"=>"Arr. Maaseik","BE223"=>"Arr. Tongeren","BE231"=>"Arr. Aalst","BE232"=>"Arr. Dendermonde","BE233"=>"Arr. Eeklo","BE234"=>"Arr. Gent","BE235"=>"Arr. Oudenaarde","BE236"=>"Arr. Sint-Niklaas","BE241"=>"Arr. Halle-Vilvoorde","BE242"=>"Arr. Leuven","BE251"=>"Arr. Brugge","BE252"=>"Arr. Diksmuide","BE253"=>"Arr. Ieper","BE254"=>"Arr. Kortrijk","BE255"=>"Arr. Oostende","BE256"=>"Arr. Roeselare","BE257"=>"Arr. Tielt","BE258"=>"Arr. Veurne","BE310"=>"Arr. Nivelles","BE321"=>"Arr. Ath","BE322"=>"Arr. Charleroi","BE323"=>"Arr. Mons","BE324"=>"Arr. Mouscron","BE325"=>"Arr. Soignies","BE326"=>"Arr. Thuin","BE327"=>"Arr. Tournai","BE331"=>"Arr. Huy","BE332"=>"Arr. Liège","BE334"=>"Arr. Waremme","BE335"=>"Arr. Verviers — communes francophones","BE336"=>"Bezirk Verviers — Deutschsprachige Gemeinschaft","BE341"=>"Arr. Arlon","BE342"=>"Arr. Bastogne","BE343"=>"Arr. Marche-en-Famenne","BE344"=>"Arr. Neufchâteau","BE345"=>"Arr. Virton","BE351"=>"Arr. Dinant","BE352"=>"Arr. Namur","BE353"=>"Arr. Philippeville","BEZZZ"=>"Extra-Regio NUTS 3","BG"=>"БЪЛГАРИЯ (BULGARIA)","BG311"=>"Видин (Vidin)","BG312"=>"Монтана (Montana)","BG313"=>"Враца (Vratsa)","BG314"=>"Плевен (Pleven)","BG315"=>"Ловеч (Lovech)","BG321"=>"Велико Търново (Veliko Tarnovo)","BG322"=>"Габрово (Gabrovo)","BG323"=>"Русе (Ruse)","BG324"=>"Разград (Razgrad)","BG325"=>"Силистра (Silistra)","BG331"=>"Варна (Varna)","BG332"=>"Добрич (Dobrich)","BG333"=>"Шумен (Shumen)","BG334"=>"Търговище (Targovishte)","BG341"=>"Бургас (Burgas)","BG342"=>"Сливен (Sliven)","BG343"=>"Ямбол (Yambol)","BG344"=>"Стара Загора (Stara Zagora)","BG411"=>"София (столица) (Sofia (stolitsa))","BG412"=>"София (Sofia)","BG413"=>"Благоевград (Blagoevgrad)","BG414"=>"Перник (Pernik)","BG415"=>"Кюстендил (Kyustendil)","BG421"=>"Пловдив (Plovdiv)","BG422"=>"Хасково (Haskovo)","BG423"=>"Пазарджик (Pazardzhik)","BG424"=>"Смолян (Smolyan)","BG425"=>"Кърджали (Kardzhali)","BGZZZ"=>"Extra-Regio NUTS 3","CZ"=>"ČESKÁ REPUBLIKA","CZ010"=>"Hlavní město Praha","CZ020"=>"Středočeský kraj","CZ031"=>"Jihočeský kraj","CZ032"=>"Plzeňský kraj","CZ041"=>"Karlovarský kraj","CZ042"=>"Ústecký kraj","CZ051"=>"Liberecký kraj","CZ052"=>"Královéhradecký kraj","CZ053"=>"Pardubický kraj","CZ063"=>"Kraj Vysočina","CZ064"=>"Jihomoravský kraj","CZ071"=>"Olomoucký kraj","CZ072"=>"Zlínský kraj","CZ080"=>"Moravskoslezský kraj","CZZZZ"=>"Extra-Regio NUTS 3","DK"=>"DANMARK","DK011"=>"Byen København","DK012"=>"Københavns omegn","DK013"=>"Nordsjælland","DK014"=>"Bornholm","DK021"=>"Østsjælland","DK022"=>"Vest- og Sydsjælland","DK031"=>"Fyn","DK032"=>"Sydjylland","DK041"=>"Vestjylland","DK042"=>"Østjylland","DK050"=>"Nordjylland","DKZZZ"=>"Extra-Regio NUTS 3","DE"=>"DEUTSCHLAND","DE111"=>"Stuttgart, Stadtkreis","DE112"=>"Böblingen","DE113"=>"Esslingen","DE114"=>"Göppingen","DE115"=>"Ludwigsburg","DE116"=>"Rems-Murr-Kreis","DE117"=>"Heilbronn, Stadtkreis","DE118"=>"Heilbronn, Landkreis","DE119"=>"Hohenlohekreis","DE11A"=>"Schwäbisch Hall","DE11B"=>"Main-Tauber-Kreis","DE11C"=>"Heidenheim","DE11D"=>"Ostalbkreis","DE121"=>"Baden-Baden, Stadtkreis","DE122"=>"Karlsruhe, Stadtkreis","DE123"=>"Karlsruhe, Landkreis","DE124"=>"Rastatt","DE125"=>"Heidelberg, Stadtkreis","DE126"=>"Mannheim, Stadtkreis","DE127"=>"Neckar-Odenwald-Kreis","DE128"=>"Rhein-Neckar-Kreis","DE129"=>"Pforzheim, Stadtkreis","DE12A"=>"Calw","DE12B"=>"Enzkreis","DE12C"=>"Freudenstadt","DE131"=>"Freiburg im Breisgau, Stadtkreis","DE132"=>"Breisgau-Hochschwarzwald","DE133"=>"Emmendingen","DE134"=>"Ortenaukreis","DE135"=>"Rottweil","DE136"=>"Schwarzwald-Baar-Kreis","DE137"=>"Tuttlingen","DE138"=>"Konstanz","DE139"=>"Lörrach","DE13A"=>"Waldshut","DE141"=>"Reutlingen","DE142"=>"Tübingen, Landkreis","DE143"=>"Zollernalbkreis","DE144"=>"Ulm, Stadtkreis","DE145"=>"Alb-Donau-Kreis","DE146"=>"Biberach","DE147"=>"Bodenseekreis","DE148"=>"Ravensburg","DE149"=>"Sigmaringen","DE211"=>"Ingolstadt, Kreisfreie Stadt","DE212"=>"München, Kreisfreie Stadt","DE213"=>"Rosenheim, Kreisfreie Stadt","DE214"=>"Altötting","DE215"=>"Berchtesgadener Land","DE216"=>"Bad Tölz-Wolfratshausen","DE217"=>"Dachau","DE218"=>"Ebersberg","DE219"=>"Eichstätt","DE21A"=>"Erding","DE21B"=>"Freising","DE21C"=>"Fürstenfeldbruck","DE21D"=>"Garmisch-Partenkirchen","DE21E"=>"Landsberg am Lech","DE21F"=>"Miesbach","DE21G"=>"Mühldorf a. Inn","DE21H"=>"München, Landkreis","DE21I"=>"Neuburg-Schrobenhausen","DE21J"=>"Pfaffenhofen a. d. Ilm","DE21K"=>"Rosenheim, Landkreis","DE21L"=>"Starnberg","DE21M"=>"Traunstein","DE21N"=>"Weilheim-Schongau","DE221"=>"Landshut, Kreisfreie Stadt","DE222"=>"Passau, Kreisfreie Stadt","DE223"=>"Straubing, Kreisfreie Stadt","DE224"=>"Deggendorf","DE225"=>"Freyung-Grafenau","DE226"=>"Kelheim","DE227"=>"Landshut, Landkreis","DE228"=>"Passau, Landkreis","DE229"=>"Regen","DE22A"=>"Rottal-Inn","DE22B"=>"Straubing-Bogen","DE22C"=>"Dingolfing-Landau","DE231"=>"Amberg, Kreisfreie Stadt","DE232"=>"Regensburg, Kreisfreie Stadt","DE233"=>"Weiden i. d. Opf, Kreisfreie Stadt","DE234"=>"Amberg-Sulzbach","DE235"=>"Cham","DE236"=>"Neumarkt i. d. OPf.","DE237"=>"Neustadt a. d. Waldnaab","DE238"=>"Regensburg, Landkreis","DE239"=>"Schwandorf","DE23A"=>"Tirschenreuth","DE241"=>"Bamberg, Kreisfreie Stadt","DE242"=>"Bayreuth, Kreisfreie Stadt","DE243"=>"Coburg, Kreisfreie Stadt","DE244"=>"Hof, Kreisfreie Stadt","DE245"=>"Bamberg, Landkreis","DE246"=>"Bayreuth, Landkreis","DE247"=>"Coburg, Landkreis","DE248"=>"Forchheim","DE249"=>"Hof, Landkreis","DE24A"=>"Kronach","DE24B"=>"Kulmbach","DE24C"=>"Lichtenfels","DE24D"=>"Wunsiedel i. Fichtelgebirge","DE251"=>"Ansbach, Kreisfreie Stadt","DE252"=>"Erlangen, Kreisfreie Stadt","DE253"=>"Fürth, Kreisfreie Stadt","DE254"=>"Nürnberg, Kreisfreie Stadt","DE255"=>"Schwabach, Kreisfreie Stadt","DE256"=>"Ansbach, Landkreis","DE257"=>"Erlangen-Höchstadt","DE258"=>"Fürth, Landkreis","DE259"=>"Nürnberger Land","DE25A"=>"Neustadt a. d. Aisch-Bad Windsheim","DE25B"=>"Roth","DE25C"=>"Weißenburg-Gunzenhausen","DE261"=>"Aschaffenburg, Kreisfreie Stadt","DE262"=>"Schweinfurt, Kreisfreie Stadt","DE263"=>"Würzburg, Kreisfreie Stadt","DE264"=>"Aschaffenburg, Landkreis","DE265"=>"Bad Kissingen","DE266"=>"Rhön-Grabfeld","DE267"=>"Haßberge","DE268"=>"Kitzingen","DE269"=>"Miltenberg","DE26A"=>"Main-Spessart","DE26B"=>"Schweinfurt, Landkreis","DE26C"=>"Würzburg, Landkreis","DE271"=>"Augsburg, Kreisfreie Stadt","DE272"=>"Kaufbeuren, Kreisfreie Stadt","DE273"=>"Kempten (Allgäu), Kreisfreie Stadt","DE274"=>"Memmingen, Kreisfreie Stadt","DE275"=>"Aichach-Friedberg","DE276"=>"Augsburg, Landkreis","DE277"=>"Dillingen a.d. Donau","DE278"=>"Günzburg","DE279"=>"Neu-Ulm","DE27A"=>"Lindau (Bodensee)","DE27B"=>"Ostallgäu","DE27C"=>"Unterallgäu","DE27D"=>"Donau-Ries","DE27E"=>"Oberallgäu","DE300"=>"Berlin","DE401"=>"Brandenburg an der Havel, Kreisfreie Stadt","DE402"=>"Cottbus, Kreisfreie Stadt","DE403"=>"Frankfurt (Oder), Kreisfreie Stadt","DE404"=>"Potsdam, Kreisfreie Stadt","DE405"=>"Barnim","DE406"=>"Dahme-Spreewald","DE407"=>"Elbe-Elster","DE408"=>"Havelland","DE409"=>"Märkisch-Oderland","DE40A"=>"Oberhavel","DE40B"=>"Oberspreewald-Lausitz","DE40C"=>"Oder-Spree","DE40D"=>"Ostprignitz-Ruppin","DE40E"=>"Potsdam-Mittelmark","DE40F"=>"Prignitz","DE40G"=>"Spree-Neiße","DE40H"=>"Teltow-Fläming","DE40I"=>"Uckermark","DE501"=>"Bremen, Kreisfreie Stadt","DE502"=>"Bremerhaven, Kreisfreie Stadt","DE600"=>"Hamburg","DE711"=>"Darmstadt, Kreisfreie Stadt","DE712"=>"Frankfurt am Main, Kreisfreie Stadt","DE713"=>"Offenbach am Main, Kreisfreie Stadt","DE714"=>"Wiesbaden, Kreisfreie Stadt","DE715"=>"Bergstraße","DE716"=>"Darmstadt-Dieburg","DE717"=>"Groß-Gerau","DE718"=>"Hochtaunuskreis","DE719"=>"Main-Kinzig-Kreis","DE71A"=>"Main-Taunus-Kreis","DE71B"=>"Odenwaldkreis","DE71C"=>"Offenbach, Landkreis","DE71D"=>"Rheingau-Taunus-Kreis","DE71E"=>"Wetteraukreis","DE721"=>"Gießen, Landkreis","DE722"=>"Lahn-Dill-Kreis","DE723"=>"Limburg-Weilburg","DE724"=>"Marburg-Biedenkopf","DE725"=>"Vogelsbergkreis","DE731"=>"Kassel, Kreisfreie Stadt","DE732"=>"Fulda","DE733"=>"Hersfeld-Rotenburg","DE734"=>"Kassel, Landkreis","DE735"=>"Schwalm-Eder-Kreis","DE736"=>"Waldeck-Frankenberg","DE737"=>"Werra-Meißner-Kreis","DE803"=>"Rostock, Kreisfreie Stadt","DE804"=>"Schwerin, Kreisfreie Stadt","DE80J"=>"Mecklenburgische Seenplatte","DE80N"=>"Vorpommern-Greifswald","DE80K"=>"Landkreis Rostock","DE80L"=>"Vorpommern-Rügen","DE80M"=>"Nordwestmecklenburg","DE80O"=>"Ludwigslust-Parchim","DE911"=>"Braunschweig, Kreisfreie Stadt","DE912"=>"Salzgitter, Kreisfreie Stadt","DE913"=>"Wolfsburg, Kreisfreie Stadt","DE914"=>"Gifhorn","DE916"=>"Goslar","DE917"=>"Helmstedt","DE918"=>"Northeim","DE91A"=>"Peine","DE91B"=>"Wolfenbüttel","DE91C"=>"Göttingen","DE922"=>"Diepholz","DE923"=>"Hameln-Pyrmont","DE925"=>"Hildesheim","DE926"=>"Holzminden","DE927"=>"Nienburg (Weser)","DE928"=>"Schaumburg","DE929"=>"Region Hannover","DE931"=>"Celle","DE932"=>"Cuxhaven","DE933"=>"Harburg","DE934"=>"Lüchow-Dannenberg","DE935"=>"Lüneburg, Landkreis","DE936"=>"Osterholz","DE937"=>"Rotenburg (Wümme)","DE938"=>"Heidekreis","DE939"=>"Stade","DE93A"=>"Uelzen","DE93B"=>"Verden","DE941"=>"Delmenhorst, Kreisfreie Stadt","DE942"=>"Emden, Kreisfreie Stadt","DE943"=>"Oldenburg (Oldenburg), Kreisfreie Stadt","DE944"=>"Osnabrück, Kreisfreie Stadt","DE945"=>"Wilhelmshaven, Kreisfreie Stadt","DE946"=>"Ammerland","DE947"=>"Aurich","DE948"=>"Cloppenburg","DE949"=>"Emsland","DE94A"=>"Friesland (DE)","DE94B"=>"Grafschaft Bentheim","DE94C"=>"Leer","DE94D"=>"Oldenburg, Landkreis","DE94E"=>"Osnabrück, Landkreis","DE94F"=>"Vechta","DE94G"=>"Wesermarsch","DE94H"=>"Wittmund","DEA11"=>"Düsseldorf, Kreisfreie Stadt","DEA12"=>"Duisburg, Kreisfreie Stadt","DEA13"=>"Essen, Kreisfreie Stadt","DEA14"=>"Krefeld, Kreisfreie Stadt","DEA15"=>"Mönchengladbach, Kreisfreie Stadt","DEA16"=>"Mülheim an der Ruhr, Kreisfreie Stadt","DEA17"=>"Oberhausen, Kreisfreie Stadt","DEA18"=>"Remscheid, Kreisfreie Stadt","DEA19"=>"Solingen, Kreisfreie Stadt","DEA1A"=>"Wuppertal, Kreisfreie Stadt","DEA1B"=>"Kleve","DEA1C"=>"Mettmann","DEA1D"=>"Rhein-Kreis Neuss","DEA1E"=>"Viersen","DEA1F"=>"Wesel","DEA22"=>"Bonn, Kreisfreie Stadt","DEA23"=>"Köln, Kreisfreie Stadt","DEA24"=>"Leverkusen, Kreisfreie Stadt","DEA26"=>"Düren","DEA27"=>"Rhein-Erft-Kreis","DEA28"=>"Euskirchen","DEA29"=>"Heinsberg","DEA2A"=>"Oberbergischer Kreis","DEA2B"=>"Rheinisch-Bergischer Kreis","DEA2C"=>"Rhein-Sieg-Kreis","DEA2D"=>"Städteregion Aachen","DEA31"=>"Bottrop, Kreisfreie Stadt","DEA32"=>"Gelsenkirchen, Kreisfreie Stadt","DEA33"=>"Münster, Kreisfreie Stadt","DEA34"=>"Borken","DEA35"=>"Coesfeld","DEA36"=>"Recklinghausen","DEA37"=>"Steinfurt","DEA38"=>"Warendorf","DEA41"=>"Bielefeld, Kreisfreie Stadt","DEA42"=>"Gütersloh","DEA43"=>"Herford","DEA44"=>"Höxter","DEA45"=>"Lippe","DEA46"=>"Minden-Lübbecke","DEA47"=>"Paderborn","DEA51"=>"Bochum, Kreisfreie Stadt","DEA52"=>"Dortmund, Kreisfreie Stadt","DEA53"=>"Hagen, Kreisfreie Stadt","DEA54"=>"Hamm, Kreisfreie Stadt","DEA55"=>"Herne, Kreisfreie Stadt","DEA56"=>"Ennepe-Ruhr-Kreis","DEA57"=>"Hochsauerlandkreis","DEA58"=>"Märkischer Kreis","DEA59"=>"Olpe","DEA5A"=>"Siegen-Wittgenstein","DEA5B"=>"Soest","DEA5C"=>"Unna","DEB11"=>"Koblenz, Kreisfreie Stadt","DEB12"=>"Ahrweiler","DEB13"=>"Altenkirchen (Westerwald)","DEB14"=>"Bad Kreuznach","DEB15"=>"Birkenfeld","DEB17"=>"Mayen-Koblenz","DEB18"=>"Neuwied","DEB1A"=>"Rhein-Lahn-Kreis","DEB1B"=>"Westerwaldkreis","DEB1C"=>"Cochem-Zell","DEB1D"=>"Rhein-Hunsrück-Kreis","DEB21"=>"Trier, Kreisfreie Stadt","DEB22"=>"Bernkastel-Wittlich","DEB23"=>"Eifelkreis Bitburg-Prüm","DEB24"=>"Vulkaneifel","DEB25"=>"Trier-Saarburg","DEB31"=>"Frankenthal (Pfalz), Kreisfreie Stadt","DEB32"=>"Kaiserslautern, Kreisfreie Stadt","DEB33"=>"Landau in der Pfalz, Kreisfreie Stadt","DEB34"=>"Ludwigshafen am Rhein, Kreisfreie Stadt","DEB35"=>"Mainz, Kreisfreie Stadt","DEB36"=>"Neustadt an der Weinstraße, Kreisfreie Stadt","DEB37"=>"Pirmasens, Kreisfreie Stadt","DEB38"=>"Speyer, Kreisfreie Stadt","DEB39"=>"Worms, Kreisfreie Stadt","DEB3A"=>"Zweibrücken, Kreisfreie Stadt","DEB3B"=>"Alzey-Worms","DEB3C"=>"Bad Dürkheim","DEB3D"=>"Donnersbergkreis","DEB3E"=>"Germersheim","DEB3F"=>"Kaiserslautern, Landkreis","DEB3G"=>"Kusel","DEB3H"=>"Südliche Weinstraße","DEB3I"=>"Rhein-Pfalz-Kreis","DEB3J"=>"Mainz-Bingen","DEB3K"=>"Südwestpfalz","DEC01"=>"Regionalverband Saarbrücken","DEC02"=>"Merzig-Wadern","DEC03"=>"Neunkirchen","DEC04"=>"Saarlouis","DEC05"=>"Saarpfalz-Kreis","DEC06"=>"St. Wendel","DED21"=>"Dresden, Kreisfreie Stadt","DED2C"=>"Bautzen","DED2D"=>"Görlitz","DED2E"=>"Meißen","DED2F"=>"Sächsische Schweiz-Osterzgebirge","DED41"=>"Chemnitz, Kreisfreie Stadt","DED42"=>"Erzgebirgskreis","DED43"=>"Mittelsachsen","DED44"=>"Vogtlandkreis","DED45"=>"Zwickau","DED51"=>"Leipzig, Kreisfreie Stadt","DED52"=>"Leipzig","DED53"=>"Nordsachsen","DEE01"=>"Dessau-Roßlau, Kreisfreie Stadt","DEE02"=>"Halle (Saale), Kreisfreie Stadt","DEE03"=>"Magdeburg, Kreisfreie Stadt","DEE04"=>"Altmarkkreis Salzwedel","DEE05"=>"Anhalt-Bitterfeld","DEE06"=>"Jerichower Land","DEE07"=>"Börde","DEE08"=>"Burgenlandkreis","DEE09"=>"Harz","DEE0A"=>"Mansfeld-Südharz","DEE0B"=>"Saalekreis","DEE0C"=>"Salzlandkreis","DEE0D"=>"Stendal","DEE0E"=>"Wittenberg","DEF01"=>"Flensburg, Kreisfreie Stadt","DEF02"=>"Kiel, Kreisfreie Stadt","DEF03"=>"Lübeck, Kreisfreie Stadt","DEF04"=>"Neumünster, Kreisfreie Stadt","DEF05"=>"Dithmarschen","DEF06"=>"Herzogtum Lauenburg","DEF07"=>"Nordfriesland","DEF08"=>"Ostholstein","DEF09"=>"Pinneberg","DEF0A"=>"Plön","DEF0B"=>"Rendsburg-Eckernförde","DEF0C"=>"Schleswig-Flensburg","DEF0D"=>"Segeberg","DEF0E"=>"Steinburg","DEF0F"=>"Stormarn","DEG01"=>"Erfurt, Kreisfreie Stadt","DEG02"=>"Gera, Kreisfreie Stadt","DEG03"=>"Jena, Kreisfreie Stadt","DEG04"=>"Suhl, Kreisfreie Stadt","DEG05"=>"Weimar, Kreisfreie Stadt","DEG06"=>"Eichsfeld","DEG07"=>"Nordhausen","DEG09"=>"Unstrut-Hainich-Kreis","DEG0A"=>"Kyffhäuserkreis","DEG0B"=>"Schmalkalden-Meiningen","DEG0C"=>"Gotha","DEG0D"=>"Sömmerda","DEG0E"=>"Hildburghausen","DEG0F"=>"Ilm-Kreis","DEG0G"=>"Weimarer Land","DEG0H"=>"Sonneberg","DEG0I"=>"Saalfeld-Rudolstadt","DEG0J"=>"Saale-Holzland-Kreis","DEG0K"=>"Saale-Orla-Kreis","DEG0L"=>"Greiz","DEG0M"=>"Altenburger Land","DEG0N"=>"Eisenach, Kreisfreie Stadt","DEG0P"=>"Wartburgkreis","DEZZZ"=>"Extra-Regio NUTS 3","EE"=>"EESTI","EE001"=>"Põhja-Eesti","EE004"=>"Lääne-Eesti","EE006"=>"Kesk-Eesti","EE007"=>"Kirde-Eesti","EE008"=>"Lõuna-Eesti","EEZZZ"=>"Extra-Regio NUTS 3","IE"=>"IRELAND","IE041"=>"Border","IE042"=>"West","IE051"=>"Mid-West","IE052"=>"South-East","IE053"=>"South-West","IE061"=>"Dublin","IE062"=>"Mid-East","IE063"=>"Midland","IEZZZ"=>"Extra-Regio NUTS 3","EL"=>"ΕΛΛΑΔΑ (ELLADA)","EL301"=>"Βόρειος Τομέας Αθηνών (Voreios Tomeas Athinon)","EL302"=>"Δυτικός Τομέας Αθηνών (Dytikos Tomeas Athinon)","EL303"=>"Κεντρικός Τομέας Αθηνών (Kentrikos Tomeas Athinon)","EL304"=>"Νότιος Τομέας Αθηνών (Notios Tomeas Athinon)","EL305"=>"Ανατολική Αττική (Anatoliki Attiki)","EL306"=>"Δυτική Αττική (Dytiki Attiki)","EL307"=>"Πειραιάς, Νήσοι (Peiraias, Nisoi)","EL411"=>"Λέσβος, Λήμνος (Lesvos, Limnos)","EL412"=>"Ικαρία, Σάμος (Ikaria, Samos)","EL413"=>"Χίος (Chios)","EL421"=>"Κάλυμνος, Κάρπαθος, Κως, Ρόδος (Kalymnos, Karpathos, Kos, Rodos)","EL422"=>"Άνδρος, Θήρα, Κέα, Μήλος, Μύκονος, Νάξος, Πάρος, Σύρος, Τήνος (Andros, Thira, Kea, Milos, Mykonos, Naxos, Paros,  Syros, Tinos)","EL431"=>"Ηράκλειο (Irakleio)","EL432"=>"Λασίθι (Lasithi)","EL433"=>"Ρεθύμνη (Rethymni)","EL434"=>"Χανιά (Chania)","EL511"=>"Έβρος (Evros)","EL512"=>"Ξάνθη (Xanthi)","EL513"=>"Ροδόπη (Rodopi)","EL514"=>"Δράμα (Drama)","EL515"=>"Θάσος, Καβάλα (Thasos, Kavala)","EL521"=>"Ημαθία (Imathia)","EL522"=>"Θεσσαλονίκη (Thessaloniki)","EL523"=>"Κιλκίς (Kilkis)","EL524"=>"Πέλλα (Pella)","EL525"=>"Πιερία (Pieria)","EL526"=>"Σέρρες (Serres)","EL527"=>"Χαλκιδική (Chalkidiki)","EL531"=>"Γρεβενά, Κοζάνη (Grevena, Kozani)","EL532"=>"Καστοριά (Kastoria)","EL533"=>"Φλώρινα (Florina)","EL541"=>"Άρτα, Πρέβεζα (Arta, Preveza)","EL542"=>"Θεσπρωτία (Thesprotia)","EL543"=>"Ιωάννινα (Ioannina)","EL611"=>"Καρδίτσα, Τρίκαλα (Karditsa, Trikala)","EL612"=>"Λάρισα (Larisa)","EL613"=>"Μαγνησία, Σποράδες (Magnisia, Sporades)","EL621"=>"Ζάκυνθος (Zakynthos)","EL622"=>"Κέρκυρα (Kerkyra)","EL623"=>"Ιθάκη, Κεφαλληνία (Ithaki, Kefallinia)","EL624"=>"Λευκάδα (Lefkada)","EL631"=>"Αιτωλοακαρνανία (Aitoloakarnania)","EL632"=>"Αχαΐα (Achaia)","EL633"=>"Ηλεία (Ileia)","EL641"=>"Βοιωτία (Voiotia)","EL642"=>"Εύβοια (Evvoia)","EL643"=>"Ευρυτανία (Evrytania)","EL644"=>"Φθιώτιδα (Fthiotida)","EL645"=>"Φωκίδα (Fokida)","EL651"=>"Αργολίδα, Αρκαδία (Argolida, Arkadia)","EL652"=>"Κορινθία (Korinthia)","EL653"=>"Λακωνία, Μεσσηνία (Lakonia, Messinia)","ELZZZ"=>"Extra-Regio NUTS 3","ES"=>"ESPAÑA","ES111"=>"A Coruña","ES112"=>"Lugo","ES113"=>"Ourense","ES114"=>"Pontevedra","ES120"=>"Asturias","ES130"=>"Cantabria","ES211"=>"Araba/Álava","ES212"=>"Gipuzkoa","ES213"=>"Bizkaia","ES220"=>"Navarra","ES230"=>"La Rioja","ES241"=>"Huesca","ES242"=>"Teruel","ES243"=>"Zaragoza","ES300"=>"Madrid","ES411"=>"Ávila","ES412"=>"Burgos","ES413"=>"León","ES414"=>"Palencia","ES415"=>"Salamanca","ES416"=>"Segovia","ES417"=>"Soria","ES418"=>"Valladolid","ES419"=>"Zamora","ES421"=>"Albacete","ES422"=>"Ciudad Real","ES423"=>"Cuenca","ES424"=>"Guadalajara","ES425"=>"Toledo","ES431"=>"Badajoz","ES432"=>"Cáceres","ES511"=>"Barcelona","ES512"=>"Girona","ES513"=>"Lleida","ES514"=>"Tarragona","ES521"=>"Alicante / Alacant","ES522"=>"Castellón / Castelló","ES523"=>"Valencia / València","ES531"=>"Eivissa y Formentera","ES532"=>"Mallorca","ES533"=>"Menorca","ES611"=>"Almería","ES612"=>"Cádiz","ES613"=>"Córdoba","ES614"=>"Granada","ES615"=>"Huelva","ES616"=>"Jaén","ES617"=>"Málaga","ES618"=>"Sevilla","ES620"=>"Murcia","ES630"=>"Ceuta","ES640"=>"Melilla","ES703"=>"El Hierro","ES704"=>"Fuerteventura","ES705"=>"Gran Canaria","ES706"=>"La Gomera","ES707"=>"La Palma","ES708"=>"Lanzarote","ES709"=>"Tenerife","ESZZZ"=>"Extra-Regio NUTS 3","FR"=>"FRANCE","FR101"=>"Paris","FR102"=>"Seine-et-Marne","FR103"=>"Yvelines","FR104"=>"Essonne","FR105"=>"Hauts-de-Seine","FR106"=>"Seine-Saint-Denis","FR107"=>"Val-de-Marne","FR108"=>"Val-d’Oise","FRB01"=>"Cher","FRB02"=>"Eure-et-Loir","FRB03"=>"Indre","FRB04"=>"Indre-et-Loire","FRB05"=>"Loir-et-Cher","FRB06"=>"Loiret","FRC11"=>"Côte-d’Or","FRC12"=>"Nièvre","FRC13"=>"Saône-et-Loire","FRC14"=>"Yonne","FRC21"=>"Doubs","FRC22"=>"Jura","FRC23"=>"Haute-Saône","FRC24"=>"Territoire de Belfort","FRD11"=>"Calvados","FRD12"=>"Manche","FRD13"=>"Orne","FRD21"=>"Eure","FRD22"=>"Seine-Maritime","FRE11"=>"Nord","FRE12"=>"Pas-de-Calais","FRE21"=>"Aisne","FRE22"=>"Oise","FRE23"=>"Somme","FRF11"=>"Bas-Rhin","FRF12"=>"Haut-Rhin","FRF21"=>"Ardennes","FRF22"=>"Aube","FRF23"=>"Marne","FRF24"=>"Haute-Marne","FRF31"=>"Meurthe-et-Moselle","FRF32"=>"Meuse","FRF33"=>"Moselle","FRF34"=>"Vosges","FRG01"=>"Loire-Atlantique","FRG02"=>"Maine-et-Loire","FRG03"=>"Mayenne","FRG04"=>"Sarthe","FRG05"=>"Vendée","FRH01"=>"Côtes-d’Armor","FRH02"=>"Finistère","FRH03"=>"Ille-et-Vilaine","FRH04"=>"Morbihan","FRI11"=>"Dordogne","FRI12"=>"Gironde","FRI13"=>"Landes","FRI14"=>"Lot-et-Garonne","FRI15"=>"Pyrénées-Atlantiques","FRI21"=>"Corrèze","FRI22"=>"Creuse","FRI23"=>"Haute-Vienne","FRI31"=>"Charente","FRI32"=>"Charente-Maritime","FRI33"=>"Deux-Sèvres","FRI34"=>"Vienne","FRJ11"=>"Aude","FRJ12"=>"Gard","FRJ13"=>"Hérault","FRJ14"=>"Lozère","FRJ15"=>"Pyrénées-Orientales","FRJ21"=>"Ariège","FRJ22"=>"Aveyron","FRJ23"=>"Haute-Garonne","FRJ24"=>"Gers","FRJ25"=>"Lot","FRJ26"=>"Hautes-Pyrénées","FRJ27"=>"Tarn","FRJ28"=>"Tarn-et-Garonne","FRK11"=>"Allier","FRK12"=>"Cantal","FRK13"=>"Haute-Loire","FRK14"=>"Puy-de-Dôme","FRK21"=>"Ain","FRK22"=>"Ardèche","FRK23"=>"Drôme","FRK24"=>"Isère","FRK25"=>"Loire","FRK26"=>"Rhône","FRK27"=>"Savoie","FRK28"=>"Haute-Savoie","FRL01"=>"Alpes-de-Haute-Provence","FRL02"=>"Hautes-Alpes","FRL03"=>"Alpes-Maritimes","FRL04"=>"Bouches-du-Rhône","FRL05"=>"Var","FRL06"=>"Vaucluse","FRM01"=>"Corse-du-Sud","FRM02"=>"Haute-Corse","FRY10"=>"Guadeloupe","FRY20"=>"Martinique","FRY30"=>"Guyane","FRY40"=>"La Réunion","FRY50"=>"Mayotte","FRZZZ"=>"Extra-Regio NUTS 3","HR"=>"HRVATSKA","HR031"=>"Primorsko-goranska županija","HR032"=>"Ličko-senjska županija","HR033"=>"Zadarska županija","HR034"=>"Šibensko-kninska županija","HR035"=>"Splitsko-dalmatinska županija","HR036"=>"Istarska županija","HR037"=>"Dubrovačko-neretvanska županija","HR041"=>"Grad Zagreb","HR042"=>"Zagrebačka županija","HR043"=>"Krapinsko-zagorska županija","HR044"=>"Varaždinska županija","HR045"=>"Koprivničko-križevačka županija","HR046"=>"Međimurska županija","HR047"=>"Bjelovarsko-bilogorska županija","HR048"=>"Virovitičko-podravska županija","HR049"=>"Požeško-slavonska županija","HR04A"=>"Brodsko-posavska županija","HR04B"=>"Osječko-baranjska županija","HR04C"=>"Vukovarsko-srijemska županija","HR04D"=>"Karlovačka županija","HR04E"=>"Sisačko-moslavačka županija","HRZZZ"=>"Extra-Regio NUTS 3","IT"=>"ITALIA","ITC11"=>"Torino","ITC12"=>"Vercelli","ITC13"=>"Biella","ITC14"=>"Verbano-Cusio-Ossola","ITC15"=>"Novara","ITC16"=>"Cuneo","ITC17"=>"Asti","ITC18"=>"Alessandria","ITC20"=>"Valle d’Aosta/Vallée d’Aoste","ITC31"=>"Imperia","ITC32"=>"Savona","ITC33"=>"Genova","ITC34"=>"La Spezia","ITC41"=>"Varese","ITC42"=>"Como","ITC43"=>"Lecco","ITC44"=>"Sondrio","ITC46"=>"Bergamo","ITC47"=>"Brescia","ITC48"=>"Pavia","ITC49"=>"Lodi","ITC4A"=>"Cremona","ITC4B"=>"Mantova","ITC4C"=>"Milano","ITC4D"=>"Monza e della Brianza","ITF11"=>"L’Aquila","ITF12"=>"Teramo","ITF13"=>"Pescara","ITF14"=>"Chieti","ITF21"=>"Isernia","ITF22"=>"Campobasso","ITF31"=>"Caserta","ITF32"=>"Benevento","ITF33"=>"Napoli","ITF34"=>"Avellino","ITF35"=>"Salerno","ITF43"=>"Taranto","ITF44"=>"Brindisi","ITF45"=>"Lecce","ITF46"=>"Foggia","ITF47"=>"Bari","ITF48"=>"Barletta-Andria-Trani","ITF51"=>"Potenza","ITF52"=>"Matera","ITF61"=>"Cosenza","ITF62"=>"Crotone","ITF63"=>"Catanzaro","ITF64"=>"Vibo Valentia","ITF65"=>"Reggio di Calabria","ITG11"=>"Trapani","ITG12"=>"Palermo","ITG13"=>"Messina","ITG14"=>"Agrigento","ITG15"=>"Caltanissetta","ITG16"=>"Enna","ITG17"=>"Catania","ITG18"=>"Ragusa","ITG19"=>"Siracusa","ITG25"=>"Sassari","ITG26"=>"Nuoro","ITG27"=>"Cagliari","ITG28"=>"Oristano","ITG29"=>"Olbia-Tempio","ITG2A"=>"Ogliastra","ITG2B"=>"Medio Campidano","ITG2C"=>"Carbonia-Iglesias","ITH10"=>"Bolzano-Bozen","ITH20"=>"Trento","ITH31"=>"Verona","ITH32"=>"Vicenza","ITH33"=>"Belluno","ITH34"=>"Treviso","ITH35"=>"Venezia","ITH36"=>"Padova","ITH37"=>"Rovigo","ITH41"=>"Pordenone","ITH42"=>"Udine","ITH43"=>"Gorizia","ITH44"=>"Trieste","ITH51"=>"Piacenza","ITH52"=>"Parma","ITH53"=>"Reggio nell’Emilia","ITH54"=>"Modena","ITH55"=>"Bologna","ITH56"=>"Ferrara","ITH57"=>"Ravenna","ITH58"=>"Forlì-Cesena","ITH59"=>"Rimini","ITI11"=>"Massa-Carrara","ITI12"=>"Lucca","ITI13"=>"Pistoia","ITI14"=>"Firenze","ITI15"=>"Prato","ITI16"=>"Livorno","ITI17"=>"Pisa","ITI18"=>"Arezzo","ITI19"=>"Siena","ITI1A"=>"Grosseto","ITI21"=>"Perugia","ITI22"=>"Terni","ITI31"=>"Pesaro e Urbino","ITI32"=>"Ancona","ITI33"=>"Macerata","ITI34"=>"Ascoli Piceno","ITI35"=>"Fermo","ITI41"=>"Viterbo","ITI42"=>"Rieti","ITI43"=>"Roma","ITI44"=>"Latina","ITI45"=>"Frosinone","ITZZZ"=>"Extra-Regio NUTS 3","CY"=>"ΚΥΠΡΟΣ (KYPROS)","CY000"=>"Κύπρος (Kypros)","CYZZZ"=>"Extra-Regio NUTS 3","LV"=>"LATVIJA","LV003"=>"Kurzeme","LV005"=>"Latgale","LV006"=>"Rīga","LV007"=>"Pierīga","LV008"=>"Vidzeme","LV009"=>"Zemgale","LVZZZ"=>"Extra-Regio NUTS 3","LT"=>"LIETUVA","LT011"=>"Vilniaus apskritis","LT021"=>"Alytaus apskritis","LT022"=>"Kauno apskritis","LT023"=>"Klaipėdos apskritis","LT024"=>"Marijampolės apskritis","LT025"=>"Panevėžio apskritis","LT026"=>"Šiaulių apskritis","LT027"=>"Tauragės apskritis","LT028"=>"Telšių apskritis","LT029"=>"Utenos apskritis","LTZZZ"=>"Extra-Regio NUTS 3","LU"=>"LUXEMBOURG","LU000"=>"Luxembourg","LUZZZ"=>"Extra-Regio NUTS 3","HU"=>"MAGYARORSZÁG","HU110"=>"Budapest","HU120"=>"Pest","HU211"=>"Fejér","HU212"=>"Komárom-Esztergom","HU213"=>"Veszprém","HU221"=>"Győr-Moson-Sopron","HU222"=>"Vas","HU223"=>"Zala","HU231"=>"Baranya","HU232"=>"Somogy","HU233"=>"Tolna","HU311"=>"Borsod-Abaúj-Zemplén","HU312"=>"Heves","HU313"=>"Nógrád","HU321"=>"Hajdú-Bihar","HU322"=>"Jász-Nagykun-Szolnok","HU323"=>"Szabolcs-Szatmár-Bereg","HU331"=>"Bács-Kiskun","HU332"=>"Békés","HU333"=>"Csongrád","HUZZZ"=>"Extra-Regio NUTS 3","MT"=>"MALTA","MT001"=>"Malta","MT002"=>"Gozo and Comino/Għawdex u Kemmuna","MTZZZ"=>"Extra-Regio NUTS 3","NL"=>"NEDERLAND","NL111"=>"Oost-Groningen","NL112"=>"Delfzijl en omgeving","NL113"=>"Overig Groningen","NL124"=>"Noord-Friesland","NL125"=>"Zuidwest-Friesland","NL126"=>"Zuidoost-Friesland","NL131"=>"Noord-Drenthe","NL132"=>"Zuidoost-Drenthe","NL133"=>"Zuidwest-Drenthe","NL211"=>"Noord-Overijssel","NL212"=>"Zuidwest-Overijssel","NL213"=>"Twente","NL221"=>"Veluwe","NL224"=>"Zuidwest-Gelderland","NL225"=>"Achterhoek","NL226"=>"Arnhem/Nijmegen","NL230"=>"Flevoland","NL310"=>"Utrecht","NL321"=>"Kop van Noord-Holland","NL323"=>"IJmond","NL324"=>"Agglomeratie Haarlem","NL325"=>"Zaanstreek","NL327"=>"Het Gooi en Vechtstreek","NL328"=>"Alkmaar en omgeving","NL329"=>"Groot-Amsterdam","NL332"=>"Agglomeratie ’s-Gravenhage","NL333"=>"Delft en Westland","NL337"=>"Agglomeratie Leiden en Bollenstreek","NL33A"=>"Zuidoost-Zuid-Holland","NL33B"=>"Oost-Zuid-Holland","NL33C"=>"Groot-Rijnmond","NL341"=>"Zeeuwsch-Vlaanderen","NL342"=>"Overig Zeeland","NL411"=>"West-Noord-Brabant","NL412"=>"Midden-Noord-Brabant","NL413"=>"Noordoost-Noord-Brabant","NL414"=>"Zuidoost-Noord-Brabant","NL421"=>"Noord-Limburg","NL422"=>"Midden-Limburg","NL423"=>"Zuid-Limburg","NLZZZ"=>"Extra-Regio NUTS 3","AT"=>"ÖSTERREICH","AT111"=>"Mittelburgenland","AT112"=>"Nordburgenland","AT113"=>"Südburgenland","AT121"=>"Mostviertel-Eisenwurzen","AT122"=>"Niederösterreich-Süd","AT123"=>"Sankt Pölten","AT124"=>"Waldviertel","AT125"=>"Weinviertel","AT126"=>"Wiener Umland/Nordteil","AT127"=>"Wiener Umland/Südteil","AT130"=>"Wien","AT211"=>"Klagenfurt-Villach","AT212"=>"Oberkärnten","AT213"=>"Unterkärnten","AT221"=>"Graz","AT222"=>"Liezen","AT223"=>"Östliche Obersteiermark","AT224"=>"Oststeiermark","AT225"=>"West- und Südsteiermark","AT226"=>"Westliche Obersteiermark","AT311"=>"Innviertel","AT312"=>"Linz-Wels","AT313"=>"Mühlviertel","AT314"=>"Steyr-Kirchdorf","AT315"=>"Traunviertel","AT321"=>"Lungau","AT322"=>"Pinzgau-Pongau","AT323"=>"Salzburg und Umgebung","AT331"=>"Außerfern","AT332"=>"Innsbruck","AT333"=>"Osttirol","AT334"=>"Tiroler Oberland","AT335"=>"Tiroler Unterland","AT341"=>"Bludenz-Bregenzer Wald","AT342"=>"Rheintal-Bodenseegebiet","ATZZZ"=>"Extra-Regio NUTS 3","PL"=>"POLSKA","PL213"=>"Miasto Kraków","PL214"=>"Krakowski","PL217"=>"Tarnowski","PL218"=>"Nowosądecki","PL219"=>"Nowotarski","PL21A"=>"Oświęcimski","PL224"=>"Częstochowski","PL225"=>"Bielski","PL227"=>"Rybnicki","PL228"=>"Bytomski","PL229"=>"Gliwicki","PL22A"=>"Katowicki","PL22B"=>"Sosnowiecki","PL22C"=>"Tyski","PL411"=>"Pilski","PL414"=>"Koniński","PL415"=>"Miasto Poznań","PL416"=>"Kaliski","PL417"=>"Leszczyński","PL418"=>"Poznański","PL424"=>"Miasto Szczecin","PL426"=>"Koszaliński","PL427"=>"Szczecinecko-pyrzycki","PL428"=>"Szczeciński","PL431"=>"Gorzowski","PL432"=>"Zielonogórski","PL514"=>"Miasto Wrocław","PL515"=>"Jeleniogórski","PL516"=>"Legnicko-głogowski","PL517"=>"Wałbrzyski","PL518"=>"Wrocławski","PL523"=>"Nyski","PL524"=>"Opolski","PL613"=>"Bydgosko-toruński","PL616"=>"Grudziądzki","PL617"=>"Inowrocławski","PL618"=>"Świecki","PL619"=>"Włocławski","PL621"=>"Elbląski","PL622"=>"Olsztyński","PL623"=>"Ełcki","PL633"=>"Trójmiejski","PL634"=>"Gdański","PL636"=>"Słupski","PL637"=>"Chojnicki","PL638"=>"Starogardzki","PL711"=>"Miasto Łódź","PL712"=>"Łódzki","PL713"=>"Piotrkowski","PL714"=>"Sieradzki","PL715"=>"Skierniewicki","PL721"=>"Kielecki","PL722"=>"Sandomiersko-jędrzejowski","PL811"=>"Bialski","PL812"=>"Chełmsko-zamojski","PL814"=>"Lubelski","PL815"=>"Puławski","PL821"=>"Krośnieński","PL822"=>"Przemyski","PL823"=>"Rzeszowski","PL824"=>"Tarnobrzeski","PL841"=>"Białostocki","PL842"=>"Łomżyński","PL843"=>"Suwalski","PL911"=>"Miasto Warszawa","PL912"=>"Warszawski wschodni","PL913"=>"Warszawski zachodni","PL921"=>"Radomski","PL922"=>"Ciechanowski","PL923"=>"Płocki","PL924"=>"Ostrołęcki","PL925"=>"Siedlecki","PL926"=>"Żyrardowski","PLZZZ"=>"Extra-Regio NUTS 3","PT"=>"PORTUGAL","PT111"=>"Alto Minho","PT112"=>"Cávado","PT119"=>"Ave","PT11A"=>"Área Metropolitana do Porto","PT11B"=>"Alto Tâmega","PT11C"=>"Tâmega e Sousa","PT11D"=>"Douro","PT11E"=>"Terras de Trás-os-Montes","PT150"=>"Algarve","PT16B"=>"Oeste","PT16D"=>"Região de Aveiro","PT16E"=>"Região de Coimbra","PT16F"=>"Região de Leiria","PT16G"=>"Viseu Dão Lafões","PT16H"=>"Beira Baixa","PT16I"=>"Médio Tejo","PT16J"=>"Beiras e Serra da Estrela","PT170"=>"Área Metropolitana de Lisboa","PT181"=>"Alentejo Litoral","PT184"=>"Baixo Alentejo","PT185"=>"Lezíria do Tejo","PT186"=>"Alto Alentejo","PT187"=>"Alentejo Central","PT200"=>"Região Autónoma dos Açores","PT300"=>"Região Autónoma da Madeira","PTZZZ"=>"Extra-Regio NUTS 3","RO"=>"ROMÂNIA","RO111"=>"Bihor","RO112"=>"Bistriţa-Năsăud","RO113"=>"Cluj","RO114"=>"Maramureş","RO115"=>"Satu Mare","RO116"=>"Sălaj","RO121"=>"Alba","RO122"=>"Braşov","RO123"=>"Covasna","RO124"=>"Harghita","RO125"=>"Mureş","RO126"=>"Sibiu","RO211"=>"Bacău","RO212"=>"Botoşani","RO213"=>"Iaşi","RO214"=>"Neamţ","RO215"=>"Suceava","RO216"=>"Vaslui","RO221"=>"Brăila","RO222"=>"Buzău","RO223"=>"Constanţa","RO224"=>"Galaţi","RO225"=>"Tulcea","RO226"=>"Vrancea","RO311"=>"Argeş","RO312"=>"Călăraşi","RO313"=>"Dâmboviţa","RO314"=>"Giurgiu","RO315"=>"Ialomiţa","RO316"=>"Prahova","RO317"=>"Teleorman","RO321"=>"Bucureşti","RO322"=>"Ilfov","RO411"=>"Dolj","RO412"=>"Gorj","RO413"=>"Mehedinţi","RO414"=>"Olt","RO415"=>"Vâlcea","RO421"=>"Arad","RO422"=>"Caraş-Severin","RO423"=>"Hunedoara","RO424"=>"Timiş","ROZZZ"=>"Extra-Regio NUTS 3","SI"=>"SLOVENIJA","SI031"=>"Pomurska","SI032"=>"Podravska","SI033"=>"Koroška","SI034"=>"Savinjska","SI035"=>"Zasavska","SI036"=>"Posavska","SI037"=>"Jugovzhodna Slovenija","SI038"=>"Primorsko-notranjska","SI041"=>"Osrednjeslovenska","SI042"=>"Gorenjska","SI043"=>"Goriška","SI044"=>"Obalno-kraška","SIZZZ"=>"Extra-Regio NUTS 3","SK"=>"SLOVENSKO","SK010"=>"Bratislavský kraj","SK021"=>"Trnavský kraj","SK022"=>"Trenčiansky kraj","SK023"=>"Nitriansky kraj","SK031"=>"Žilinský kraj","SK032"=>"Banskobystrický kraj","SK041"=>"Prešovský kraj","SK042"=>"Košický kraj","SKZZZ"=>"Extra-Regio NUTS 3","FI"=>"SUOMI / FINLAND","FI193"=>"Keski-Suomi","FI194"=>"Etelä-Pohjanmaa","FI195"=>"Pohjanmaa","FI196"=>"Satakunta","FI197"=>"Pirkanmaa","FI1B1"=>"Helsinki-Uusimaa","FI1C1"=>"Varsinais-Suomi","FI1C2"=>"Kanta-Häme","FI1C3"=>"Päijät-Häme","FI1C4"=>"Kymenlaakso","FI1C5"=>"Etelä-Karjala","FI1D1"=>"Etelä-Savo","FI1D2"=>"Pohjois-Savo","FI1D3"=>"Pohjois-Karjala","FI1D5"=>"Keski-Pohjanmaa","FI1D7"=>"Lappi","FI1D8"=>"Kainuu","FI1D9"=>"Pohjois-Pohjanmaa","FI200"=>"Åland","FIZZZ"=>"Extra-Regio NUTS 3","SE"=>"SVERIGE","SE110"=>"Stockholms län","SE121"=>"Uppsala län","SE122"=>"Södermanlands län","SE123"=>"Östergötlands län","SE124"=>"Örebro län","SE125"=>"Västmanlands län","SE211"=>"Jönköpings län","SE212"=>"Kronobergs län","SE213"=>"Kalmar län","SE214"=>"Gotlands län","SE221"=>"Blekinge län","SE224"=>"Skåne län","SE231"=>"Hallands län","SE232"=>"Västra Götalands län","SE311"=>"Värmlands län","SE312"=>"Dalarnas län","SE313"=>"Gävleborgs län","SE321"=>"Västernorrlands län","SE322"=>"Jämtlands län","SE331"=>"Västerbottens län","SE332"=>"Norrbottens län","SEZZZ"=>"Extra-Regio NUTS 3","UK"=>"UNITED KINGDOM","UKC11"=>"Hartlepool and Stockton-on-Tees","UKC12"=>"South Teesside","UKC13"=>"Darlington","UKC14"=>"Durham CC","UKC21"=>"Northumberland","UKC22"=>"Tyneside","UKC23"=>"Sunderland","UKD11"=>"West Cumbria","UKD12"=>"East Cumbria","UKD33"=>"Manchester","UKD34"=>"Greater Manchester South West","UKD35"=>"Greater Manchester South East","UKD36"=>"Greater Manchester North West","UKD37"=>"Greater Manchester North East","UKD41"=>"Blackburn with Darwen","UKD42"=>"Blackpool","UKD44"=>"Lancaster and Wyre","UKD45"=>"Mid Lancashire","UKD46"=>"East Lancashire","UKD47"=>"Chorley and West Lancashire","UKD61"=>"Warrington","UKD62"=>"Cheshire East","UKD63"=>"Cheshire West and Chester","UKD71"=>"East Merseyside","UKD72"=>"Liverpool","UKD73"=>"Sefton","UKD74"=>"Wirral","UKE11"=>"Kingston upon Hull, City of","UKE12"=>"East Riding of Yorkshire","UKE13"=>"North and North East Lincolnshire","UKE21"=>"York","UKE22"=>"North Yorkshire CC","UKE31"=>"Barnsley, Doncaster and Rotherham","UKE32"=>"Sheffield","UKE41"=>"Bradford","UKE42"=>"Leeds","UKE44"=>"Calderdale and Kirklees","UKE45"=>"Wakefield","UKF11"=>"Derby","UKF12"=>"East Derbyshire","UKF13"=>"South and West Derbyshire","UKF14"=>"Nottingham","UKF15"=>"North Nottinghamshire","UKF16"=>"South Nottinghamshire","UKF21"=>"Leicester","UKF22"=>"Leicestershire CC and Rutland","UKF24"=>"West Northamptonshire","UKF25"=>"North Northamptonshire","UKF30"=>"Lincolnshire","UKG11"=>"Herefordshire, County of","UKG12"=>"Worcestershire","UKG13"=>"Warwickshire","UKG21"=>"Telford and Wrekin","UKG22"=>"Shropshire CC","UKG23"=>"Stoke-on-Trent","UKG24"=>"Staffordshire CC","UKG31"=>"Birmingham","UKG32"=>"Solihull","UKG33"=>"Coventry","UKG36"=>"Dudley","UKG37"=>"Sandwell","UKG38"=>"Walsall","UKG39"=>"Wolverhampton","UKH11"=>"Peterborough","UKH12"=>"Cambridgeshire CC","UKH14"=>"Suffolk","UKH15"=>"Norwich and East Norfolk","UKH16"=>"North and West Norfolk","UKH17"=>"Breckland and South Norfolk","UKH21"=>"Luton","UKH23"=>"Hertfordshire","UKH24"=>"Bedford","UKH25"=>"Central Bedfordshire","UKH31"=>"Southend-on-Sea","UKH32"=>"Thurrock","UKH34"=>"Essex Haven Gateway","UKH35"=>"West Essex","UKH36"=>"Heart of Essex","UKH37"=>"Essex Thames Gateway","UKI31"=>"Camden and City of London","UKI32"=>"Westminster","UKI33"=>"Kensington & Chelsea and Hammersmith & Fulham","UKI34"=>"Wandsworth","UKI41"=>"Hackney and Newham","UKI42"=>"Tower Hamlets","UKI43"=>"Haringey and Islington","UKI44"=>"Lewisham and Southwark","UKI45"=>"Lambeth","UKI51"=>"Bexley and Greenwich","UKI52"=>"Barking & Dagenham and Havering","UKI53"=>"Redbridge and Waltham Forest","UKI54"=>"Enfield","UKI61"=>"Bromley","UKI62"=>"Croydon","UKI63"=>"Merton, Kingston upon Thames and Sutton","UKI71"=>"Barnet","UKI72"=>"Brent","UKI73"=>"Ealing","UKI74"=>"Harrow and Hillingdon","UKI75"=>"Hounslow and Richmond upon Thames","UKJ11"=>"Berkshire","UKJ12"=>"Milton Keynes","UKJ13"=>"Buckinghamshire CC","UKJ14"=>"Oxfordshire","UKJ21"=>"Brighton and Hove","UKJ22"=>"East Sussex CC","UKJ25"=>"West Surrey","UKJ26"=>"East Surrey","UKJ27"=>"West Sussex (South West)","UKJ28"=>"West Sussex (North East)","UKJ31"=>"Portsmouth","UKJ32"=>"Southampton","UKJ34"=>"Isle of Wight","UKJ35"=>"South Hampshire","UKJ36"=>"Central Hampshire","UKJ37"=>"North Hampshire","UKJ41"=>"Medway","UKJ43"=>"Kent Thames Gateway","UKJ44"=>"East Kent","UKJ45"=>"Mid Kent","UKJ46"=>"West Kent","UKK11"=>"Bristol, City of","UKK12"=>"Bath and North East Somerset, North Somerset and South Gloucestershire","UKK13"=>"Gloucestershire","UKK14"=>"Swindon","UKK15"=>"Wiltshire CC","UKK21"=>"Bournemouth and Poole","UKK22"=>"Dorset CC","UKK23"=>"Somerset","UKK30"=>"Cornwall and Isles of Scilly","UKK41"=>"Plymouth","UKK42"=>"Torbay","UKK43"=>"Devon CC","UKL11"=>"Isle of Anglesey","UKL12"=>"Gwynedd","UKL13"=>"Conwy and Denbighshire","UKL14"=>"South West Wales","UKL15"=>"Central Valleys","UKL16"=>"Gwent Valleys","UKL17"=>"Bridgend and Neath Port Talbot","UKL18"=>"Swansea","UKL21"=>"Monmouthshire and Newport","UKL22"=>"Cardiff and Vale of Glamorgan","UKL23"=>"Flintshire and Wrexham","UKL24"=>"Powys","UKM50"=>"Aberdeen City and Aberdeenshire","UKM61"=>"Caithness & Sutherland and Ross & Cromarty","UKM62"=>"Inverness & Nairn and Moray, Badenoch & Strathspey","UKM63"=>"Lochaber, Skye & Lochalsh, Arran & Cumbrae and Argyll & Bute","UKM64"=>"Na h-Eileanan Siar (Western Isles)","UKM65"=>"Orkney Islands","UKM66"=>"Shetland Islands","UKM71"=>"Angus and Dundee City","UKM72"=>"Clackmannanshire and Fife","UKM73"=>"East Lothian and Midlothian","UKM75"=>"Edinburgh, City of","UKM76"=>"Falkirk","UKM77"=>"Perth & Kinross and Stirling","UKM78"=>"West Lothian","UKM81"=>"East Dunbartonshire, West Dunbartonshire and Helensburgh & Lomond","UKM82"=>"Glasgow City","UKM83"=>"Inverclyde, East Renfrewshire and Renfrewshire","UKM84"=>"North Lanarkshire","UKM91"=>"Scottish Borders","UKM92"=>"Dumfries & Galloway","UKM93"=>"East Ayrshire and North Ayrshire mainland","UKM94"=>"South Ayrshire","UKM95"=>"South Lanarkshire","UKN06"=>"Belfast","UKN07"=>"Armagh City, Banbridge and Craigavon","UKN08"=>"Newry, Mourne and Down","UKN09"=>"Ards and North Down","UKN10"=>"Derry City and Strabane","UKN11"=>"Mid Ulster","UKN12"=>"Causeway Coast and Glens","UKN13"=>"Antrim and Newtownabbey","UKN14"=>"Lisburn and Castlereagh","UKN15"=>"Mid and East Antrim","UKN16"=>"Fermanagh and Omagh","UKZZZ"=>"Extra-Regio NUTS 3"
];