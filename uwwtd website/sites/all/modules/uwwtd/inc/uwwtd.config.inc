<?php

if ( !defined('UWWTD_CACHE_ACTIVE')) {
    define('UWWTD_CACHE_ACTIVE', false);
}
if ( !defined('DEBUG_ALGO')) {
    define('DEBUG_ALGO', true);
}

// $GLOBALS['uwwtd']['message']['boolean_error']
// $GLOBALS['uwwtd']['message']['boolean_error']
$GLOBALS['uwwtd']['message'] = array(
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
);

$GLOBALS['uwwtd_errors']=array();
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

$GLOBALS['uwwtd']['uwwtd_years'] = array(
	'2000' => '2000',
	'2001' => '2001',
	'2002' => '2002',
	'2003' => '2003',
	'2004' => '2004',
	'2005' => '2005',
	'2006' => '2006',
	'2007' => '2007',
	'2008' => '2008',
	'2009' => '2009',
	'2010' => '2010',
	'2011' => '2011',
	'2012' => '2012',
	'2013' => '2013',
	'2014' => '2014',
	'2015' => '2015',
	'2016' => '2016',
	'2017' => '2017',
	'2018' => '2018',
	'2019' => '2019',
	'2020' => '2020'
);

$GLOBALS['uwwtd']['ui']['operator'] = array(
    '<' => 'Is less than',
    '=' => 'Is equal to',
    '!=' => 'Is not equal to',
    '>=' => 'Is greater than or equal to',
    '>' => 'Is greater than',
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
    'asc' => function ($a, $b) {if ($a[$GLOBALS['uwwtd']['ui']['sortfield']] == $b[$GLOBALS['uwwtd']['ui']['sortfield']]) { return 0; } return ($a[$GLOBALS['uwwtd']['ui']['sortfield']] < $b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? -1 : 1;},
    'desc' => function ($a, $b) {if ($a[$GLOBALS['uwwtd']['ui']['sortfield']] == $b[$GLOBALS['uwwtd']['ui']['sortfield']]) { return 0; } return ($a[$GLOBALS['uwwtd']['ui']['sortfield']] < $b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? 1 : -1;},
);
// dsm($data);
$GLOBALS['uwwtd']['ui']['field']['agglomeration'] = array(
    'title'                           =>  t('Name'),
    'field_inspireidlocalid_value'    =>  t('Identifier Code'),
    'field_agggenerated_value'        =>  t('Generated Load (p.e.)'),
    'field_regionnuts_value'          =>  t('Region (NUTS)'),
    'field_aggcompliance_value'       =>  t('Compliance'),
    'field_aggart3compliance_value'   =>  t('Article 3 compliance'),
    'field_aggart4compliance_value'   =>  t('Article 4 compliance'),
    'field_aggart5compliance_value'    =>  t('Article 5 compliance'),
    'field_aggart6compliance_value'   =>  t('Article 6 compliance'),
	'field_article17_agglo_value'   =>  t('Article 17'),
);

$GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment'] = array(
    'title'                           =>  t('Name'),
    'field_inspireidlocalid_value'    =>  t('Identifier Code'),
    'field_agggenerated_value'        =>  t('Generated Load (p.e.)'),
    'field_regionnuts_value'          =>  t('Region (NUTS)'),
    'field_aggc2_value'       =>  t('IAS (% of p.e.)'),
    'field_aggc1_value'   =>  t('Collective system (% of p.e.) '),
    'field_aggpercwithouttreatment_value'   =>  t('Not addressed through IAS (% of p.e.)'),
	'field_article17_agglo_value'   =>  t('Article 17'),
);

// field_aggc1|Collective system|#74FFE0
// field_aggc2|Individual and Appropriate Systems (IAS)|#BD8842
// field_aggpercwithouttreatment|Discharge without treatment|#C00000

$GLOBALS['uwwtd']['ui']['field']['uwwtp'] = array(
    'title'                           =>  t('Name'),
    'field_inspireidlocalid_value'    =>  t('Identifier Code'),
    'field_uwwloadenteringuwwtp_value'        =>  t('Load entering (p.e.)'),
	'field_physicalcapacityactivity_value'    =>  t('Physical Capacity (p.e.)'),
    'field_regionnuts_value'          =>  t('Region (NUTS)'),
    'field_uwwcompliance_value'       =>  t('Compliance'),
    'field_uwwbod5perf_value'   =>  t('Performance: BOD5'),
    'field_uwwcodperf_value'   =>  t('Performance: COD '),
    'field_uwwntotperf_value'   =>  t('Performance: Total nitrogen'),
    'field_uwwptotperf_value'   =>  t('Performance: Total phosphorus'),
    'field_uwwtreatmenttype_value'   =>  t('Treatment type in place'),
	'field_article17_uwwtp_value'   =>  t('Article 17'),
);

$GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment'] = array(
    'title'                             =>  t('Name'),
    'field_inspireidlocalid_value'      =>  t('Identifier Code'),
    'field_uwwloadenteringuwwtp_value'  =>  t('Load entering (p.e.)'),
	'field_physicalcapacityactivity_value'    =>  t('Physical Capacity (p.e.)'),
    'field_regionnuts_value'            =>  t('Region (NUTS)'),
    'field_uwwprimarytreatment_value'   =>  t('Primary treatment'),
    'field_uwwsecondarytreatment_value' =>  t('Secondary treatment'),
    'field_uwwnremoval_value'           =>  t('N removal'),
    'field_uwwpremoval_value'           =>  t('P removal'),
    'field_uwwspecification_value'      =>  t('More stringent treatment'),
	'field_article17_uwwtp_value'   =>  t('Article 17'),
);


$GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure'] = array(
    'title'                             =>  t('Name'),
    'field_inspireidlocalid_value'      =>  t('Identifier Code'),
    'field_uwwloadenteringuwwtp_value'  =>  t('Load entering (p.e.)'),
	'field_physicalcapacityactivity_value'    =>  t('Physical Capacity (p.e.)'),
    'field_uwwboddischarge_value'      =>  t('Discharged load BOD (t per year)'),
    'field_uwwcoddischarge_value'      =>  t('Discharged load COD (t per year)'),
    'field_uwwndischarge_value'      =>  t('Discharged load Ntot (t per year)'),
    'field_uwwpdischarge_value'      =>  t('Discharged load Ptot (t per year)'),
	'field_article17_uwwtp_value'   =>  t('Article 17'),
);


$GLOBALS['uwwtd']['ui']['compliance_connection'] = array(
    'PD' => 'NR',
    'QC' => 'C',
);

$GLOBALS['uwwtd']['all_compliance']  = array(
    'NI' => 'No information',
    'NR' => 'Not relevant',     //comprend aussi PD
    'NC' => 'Not Compliant',
    'C' => 'Compliant',        //comprend aussi QC
    '?' => '?',
    'PD' => 'Pending Deadline',
    'QC' => 'Questionable Compliance',
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
$GLOBALS['uwwtd']['ui']['field']['compliance_label'] = $GLOBALS['uwwtd']['ui']['compliance'];

$GLOBALS['uwwtd']['ui']['test_pass'] = array(
    'All' => '- Any -',
    'P' => 'Pass',
    'F' => 'Fail',
    'NR' => 'Not relevant',
);

$GLOBALS['uwwtd']['ui']['yes_no'] = array(
    'All' => '- Any -',
    '1' => 'Yes',
    '0' => 'No',
);


$GLOBALS['uwwtd']['ui']['treatment_type_in_place'] = array(
    'All' => '- Any -',
    'NI' => 'No information',
    'P' => 'Primary treatment',
    'S' => 'Secondary treatment',
    'MS' => 'More stringent treatment',
);
