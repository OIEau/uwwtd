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
    'asc' => function ($a, $b) {if ($a[$GLOBALS['uwwtd']['ui']['sortfield']] == $b[$GLOBALS['uwwtd']['ui']['sortfield']]) { return 0; } return ($a[$GLOBALS['uwwtd']['ui']['sortfield']] < $b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? -1 : 1;},
    'desc' => function ($a, $b) {if ($a[$GLOBALS['uwwtd']['ui']['sortfield']] == $b[$GLOBALS['uwwtd']['ui']['sortfield']]) { return 0; } return ($a[$GLOBALS['uwwtd']['ui']['sortfield']] < $b[$GLOBALS['uwwtd']['ui']['sortfield']]) ? 1 : -1;},
);
// dsm($data);
$GLOBALS['uwwtd']['ui']['field']['agglomeration'] = array(
    'title'                           =>  t('Name'),
    'field_inspireidlocalid_value'    =>  t('Identifier Code'),
    'field_agggenerated_value'        =>  t('Generated Load (p.e.)'),
    'field_regionnuts_value'          =>  t('Region (NUTS)'),
    'field_aggcompliance_value'       =>  t('Global compliance'),
	/*
    'field_aggart3compliance_value'   =>  t('Article 3 compliance'),
    'field_aggart4compliance_value'   =>  t('Article 4 compliance'),
    'field_aggart5compliance_value'   =>  t('Article 5 compliance'),
    'field_aggart6compliance_value'   =>  t('Article 6 compliance'),
	*/
	'field_aggart3compliance_value'   =>  t('Connection compliance'),
    'field_aggart4compliance_value'   =>  t('2nd treatment compliance'),
    'field_aggart5compliance_value'   =>  t('3rd treatment compliance'),
    'field_aggart6compliance_value'   =>  t('Article 6 compliance'),
	
	'field_article17_agglo_value'     =>  t('Article 17'),
);

$GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment'] = array(
    'title'                           =>  t('Name'),
    'field_inspireidlocalid_value'    =>  t('Identifier Code'),
    'field_agggenerated_value'        =>  t('Generated Load (p.e.)'),
    'field_regionnuts_value'          =>  t('Region (NUTS)'),
    'field_aggc2_value'       =>  t('IAS (% of p.e.)'),
    'field_aggc1_value'   =>  t('Collecting system (% of p.e.) '),
    'field_aggpercwithouttreatment_value'   =>  t('Discharge without treatment (% of p.e.)'),
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
//     'field_uwwcompliance_value'       =>  t('Compliance'),
//     'field_uwwbod5perf_value'   =>  t('Performance: BOD5'),
//     'field_uwwcodperf_value'   =>  t('Performance: COD '),
//     'field_uwwntotperf_value'   =>  t('Performance: Total nitrogen'),
//     'field_uwwptotperf_value'   =>  t('Performance: Total phosphorus'),
    'field_uwwtreatmenttype_value'   =>  t('Treatment type in place'),
    'field_uwwcompliance_value'       =>  t('Compliance'),
    'field_uwwbod5perf_value'   =>  t('BOD5'),
    'field_uwwcodperf_value'   =>  t('COD '),
    'field_uwwntotperf_value'   =>  t('Total nitrogen'),
    'field_uwwptotperf_value'   =>  t('Total phosphorus'),    
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
    'field_uwwspecification_value'      =>  t('Other more stringent treatment'),
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
    'NI' => t('No information'),
    'NR' => t('Not relevant'),     //comprend aussi PD
    'NC' => t('Not Compliant'),
    'C' => t('Compliant'),        //comprend aussi QC
    '?' => t('?'),
    'PD' => t('Pending Deadline'),
    'QC' => t('Questionable Compliance'),
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
		'cs'=>array('color'=>'#74ffe0','label'=>t('Connected to collecting system')),
        'ias'=>array('color'=>'#bd8842','label'=>t('Individual and Appropriate Systems')),
        'wot'=>array('color'=>'#d93c3c','label'=>t('Discharged without treatment'))
	),
	'sewage_network'=>array(
		'B'=>array('color'=>'#4f91e1','label'=>t('Separative and combine system')),
        'S'=>array('color'=>'#bd8842','label'=>t('Separative system')),
        'C'=>array('color'=>'#bd8842','label'=>t('Combine system')),
		'NI'=>array('color'=>'#a2a2a2','label'=>t('No information')),
	)
	
	//E74C3C
);

