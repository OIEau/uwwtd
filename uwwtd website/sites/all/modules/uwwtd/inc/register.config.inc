<?php
$GLOBALS['uwwtd']['register']['message'] = array(
  'info_not_provided' => 'NI',
  'info_empty' => '',
  
);





// $GLOBALS['uwwtd']['register']['message']['info_not_provided']

//todo move in conf file
// $GLOBALS['uwwtd']['sheet']['path_model_file']
$GLOBALS['uwwtd']['sheet'] = array(
    'BOM' => "\xEF\xBB\xBF",
    'model' => '',  //deprecated  from 2016
    'model_reporting_2014' => '2018_register_model_20181114.xlsx', //'2015_register_model_20161214.xlsx',  original.xlsx
    //Be carefull!!!!!
    //IMPORTANT
    //the model MUST be modify on Microsoft office Excel 2007, or all the chart will lose their style and format (color, thousand separtor on axis, ...)
    'model_reporting_2016' => '2018_register_model_20190321.xlsx',   //2018_register_model_20181114 2018_register_model_20190307
    'model_reporting_2018' => '2020_register_model_20210212.xlsx',   
    'model_graph_2018' => '2018_graph_final.xlsm',   
                
//     'model_reporting_2016_avec_acronym_et_label' => '2018_register_model_20181015_glossaire_colonne.xlsx',
    
    'final_register' => '_register_model_final.xlsx',
    'final_register_graph' => '_graph_final.xlsm',
    'model_type' => 'Excel2007',
    'target_type' => 'Excel2007',
    'path_model_file' => drupal_get_path('module', 'uwwtd') . '/model/',
//     'path_created_file' => drupal_get_path('module', 'uwwtd') . '/model/test/',
//     'path_created_file' => drupal_realpath('public://') . '/register/',
    'path_created_file' => 'public://register/',
    'glossary' => array(
//         'csv_name' => '_rca.csv',
        'xlsx_name' => '_glossary.xlsx',
        'xlsx_model_sheet_name' => 'Glossary',
    ),
    'rca' => array(
        'csv_name' => '_rca.csv',
        'xlsx_name' => '_rca.xlsx',
        'xlsx_model_sheet_name' => 'Information on sensitive area',
    ),
    'rcaA54' => array(
        'csv_name' => '_rcaA54.csv',
        'xlsx_name' => '_rcaA54.xlsx',
        'xlsx_model_sheet_name' => '5(4)-areas',
    ),
    'rca_history' => array(
        //'csv_name' => '_rcaA54.csv',
        'xlsx_name' => '_rca_history.xlsx',
        'xlsx_model_sheet_name' => 'SA-history',
    ),
    'uwwtp' => array(
        'csv_name' => '_uwwtp.csv',
        'csv_total_name' => '_uwwtp.total.csv',   
        'xlsx_name' => '_uwwtp.xlsx',
        'xlsx_model_sheet_name' => 'UWWTP Level',
    ),
    'agglo' => array(
        'csv_name' => '_agglo.csv',
        'csv_total_name' => '_agglo.total.csv',
        'csv_evaluation_name' => '_agglo.evaluation.csv',   
        'csv_distance_name' => '_agglo.distance.csv',  
        'xlsx_name' => '_agglo.xlsx',
        'xlsx_model_sheet_name' => 'agglomeration level',
    ),
    'agglo_debug' => array(
        'csv_name' => '_agglo_debug.csv',
        // 'csv_total_name' => '_agglo.total.csv',
        // 'csv_evaluation_name' => '_agglo.evaluation.csv',   
        // 'csv_distance_name' => '_agglo.distance.csv',  
        'xlsx_name' => '_agglo_debug.xlsx',
        'xlsx_model_sheet_name' => 'agglomeration debug level',
    ),
    'summary_legal_comp' => array(
        'xlsx_name' => '_summary_legal_comp.xlsx',
        'xlsx_model_sheet_name' => 'Summary_legal_compliance',
    ),
    'distance_to_comp' => array(
        'xlsx_name' => '_distance_to_comp.xlsx',
        'xlsx_model_sheet_name' => 'distance to compliance',
    ),
    'summary_installation_in_place' => array(
        'xlsx_name' => '_summary_installation_in_place.xlsx',
        'xlsx_model_sheet_name' => 'Summary_installation_in_place',
    ),
    'breach_list' => array(
        'xlsx_name' => '_breach_list.xlsx',
        'xlsx_model_sheet_name' => 'breach_list',
    ),
    'summary_big_cities' => array(
        'csv_name' => '_summary_big_cities.csv',
        'xlsx_name' => '_summary_big_cities.xlsx',
        'xlsx_model_sheet_name' => 'Summary_big_cities',
    ),
    'nuts_level2' => array(
        'csv_name' => '_nuts_level2.csv',
        'csv_distance_name' => '_nuts_level2_distance.csv',
        'xlsx_name' => '_nuts_level2.xlsx',
        'xlsx_model_sheet_name' => 'Nuts2_level analyse',
    ),
    'sewage_sludge' => array(
        'xlsx_name' => '_sewage_sludge.xlsx',
        'xlsx_model_sheet_name' => 'sewage_sludge_and_re-use',
    ),
    'graph' => array(
        'xlsx_name' => '_graph.xlsx',
        'xlsx_model_sheet_name' => 'Graphs',
    ), 
    'graph_2' => array(
        'xlsx_name' => '_graph2.xlsx',
        'xlsx_model_sheet_name' => 'National report Graphs',
    ), 
//     'graph_test' => array(
//         'xlsx_name' => '_Feuil1.xlsx',
//         'xlsx_model_sheet_name' => 'Feuil1',
//     ), 
    'new_agglo' => array(
        'csv_name' => '_new_agglo.csv',
        'xlsx_name' => '_new_agglo.xlsx',
        'xlsx_model_sheet_name' => 'New agglomerations',
    ),  
    'agglo_out' => array(
        'csv_name' => '_agglo_out.csv',
        'xlsx_name' => '_agglo_out.xlsx',
        'xlsx_model_sheet_name' => 'Agglomerationout',
    ),  
    'new_uwwtp' => array(
        'csv_name' => '_new_uwwtp.csv',
        'xlsx_name' => '_new_uwwtp.xlsx',
        'xlsx_model_sheet_name' => 'New treatment plants',
    ),  
    'uwwtp_out' => array(
        'csv_name' => '_uwwtp_out.csv',
        'xlsx_name' => '_uwwtp_out.xlsx',
        'xlsx_model_sheet_name' => 'UWWTPsout',
    ),  
    'agglo_compliance' => array(
        'csv_name' => '_agglo_compliance.csv',
        'xlsx_name' => '_agglo_compliance.xlsx',
        'xlsx_model_sheet_name' => 'Agglomeration legal compliance',
    ),                                                       
);

$i = 0;
$GLOBALS['uwwtd']['sheet1_rca'] = array(    
    'id_of_sensitive_area' => $i++,
    'name_of_sensitive_area' => $i++,
    'zone_type_code' => $i++,
    'specialised_zone_type_code' => $i++,
    'parameters_subject_to_more_stringent_treatment:_n' => $i++,
    'parameters_subject_to_more_stringent_treatment:_p' => $i++,
    'parameters_subject_to_more_stringent_treatment:_m' => $i++,
    'parameters_subject_to_more_stringent_treatment:_other' => $i++,
    'date_of_designation_of_art._5(8)' => $i++,
    'starting_date_of_application_of_art._5(8)' => $i++,
    'starting_date_of_application_of_art._5(4)' => $i++,
    'designation_criteria_a_n' => $i++,
    'starting_date_of_designation_a_n' => $i++,
    'starting_date_of_application_a_n' => $i++,
    'designation_criteria_a_p' => $i++,
    'starting_date_of_designation_a_p' => $i++,
    'starting_date_of_application_a_p' => $i++,
    'designation_criteria_b' => $i++,
    'starting_date_of_designation_b' => $i++,
    'starting_date_of_application_b' => $i++,
    'designation_criteria_c' => $i++,
    'starting_date_of_designation_c' => $i++,
    'starting_date_of_application_c' => $i++,
    'designation_criteria_c_-_relevant_eu-directives_and_related_parameters' => $i++,
    'designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive' => $i++,
    'designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive' => $i++,
    'number_of_uwwtps_of_the_article_5.4_area_(1)' => $i++,
    'total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)' => $i++,
    'incoming_loads_(t/y)_n-tot_(1)' => $i++,
    'rate_of_removal_n-tot' => $i++,
    'discharged_loads_(t/y)_n-tot_(1)' => $i++,
    'incoming_loads_(t/y)_p-tot_(1)' => $i++,
    'rate_of_removal_p-tot' => $i++,
    'discharged_loads_(t/y)_p-tot:_' => $i++,
    'general_remark' => $i++,
    'rcabeginlife' => $i++,
    'rcaendlife' => $i++,
    'id_code_of_successor_(2)' => $i++,
    'id_code_of_predecessor_(2)' => $i++,
    'remark_successor/predecessor_sa' => $i++,
    'related_sensitive_area_(2)' => $i++,
    'remark_related_sensitive_area_(2)' => $i++,
    'nb_agg'=>$i++
);  

$i = 0;
$GLOBALS['uwwtd']['sheet1_rcaA54'] = array(    
    'code' => $i++,
    'name' => $i++,
    'designation_date' => $i++,
    'deadline_n' => $i++,
    'deadline_p' => $i++,
    'application_art_5_4' => $i++,
    'uwwtp_number' => $i++,
    'uwwtp_capacity_total' => $i++,
    'n_in' => $i++,
    'n_out' => $i++,
    'p_in' => $i++,
    'p_out' => $i++,
    'n_reduction' => $i++,
    'p_reduction' => $i++,
    'compliant_5_4' => $i++,
    'transition_period_pending' => $i++,
    'remarks_by_ms' => $i++,
); 


$i = 0;
$GLOBALS['uwwtd']['sheet2_uwwtp'] = array(
    'aggcode' => array('k'=>$i++, 'xls'=>true),
    'aggname' => array('k'=>$i++, 'xls'=>true),
    'aggstatus' => array('k'=>$i++, 'xls'=>true),
    'generated load  p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'deadline for Art. 3 compliance' => array('k'=>$i++, 'xls'=>true),
    'deadline for Art. 4 compliance' => array('k'=>$i++, 'xls'=>true),
    'deadline for Art. 5 compliance' => array('k'=>$i++, 'xls'=>true),
    //Collection',
    'collected in collecting system % of gen. load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'collected in collecting system  p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'addressed through IAS % of gen. load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'addressed through IAS p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'neither collecting system nor IAS % of gen. load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'neither collecting system nor IAS p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    //UWWTP general info',
    'uww_code' => array('k'=>$i++, 'xls'=>true),
    'uww_name' => array('k'=>$i++, 'xls'=>true),
    'uww_status' => array('k'=>$i++, 'xls'=>true),
    'uww_iscon' => array('k'=>$i++, 'xls'=>false),
    
    'uww_entering_this_UWWTP_UwwtpAgglo_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'uww_entering_load_calculated_UwwtpAgglo_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    
    'uww_entering this UWWTP % of gen. load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'uww_entering load  p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    
    'uww_capacity p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
//     'uww2_status' => $i++,
    //Previous UWWTP general info',
    'previous_reference year' => array('k'=>$i++, 'xls'=>true),
    'previous_entering load p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'previous_capacity p. e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'previous_Percentage of change of entering load (1) %' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'previous_Percentage of change of capacity (2) %' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    //current RCA details',
    'water body type' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'In case of discharge on land please specify the purpose' => array('k'=>$i++, 'xls'=>true),
    'Id code of the water body WFD' => array('k'=>$i++, 'xls'=>true),
    'ID of WFD river basin district' => array('k'=>$i++, 'xls'=>true),
    'entirely discharging into coastal waters?' => array('k'=>$i++, 'xls'=>true),
    'type_of_receiving_water' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'ID of the SA or LSA' => array('k'=>$i++, 'xls'=>true),
    'rcaname' => array('k'=>$i++, 'xls'=>true),
    'Starting date of application 5.8.' => array('k'=>$i++, 'xls'=>true),
    'Starting date of application 5.4.' => array('k'=>$i++, 'xls'=>true),
    'Starting date of application a N' => array('k'=>$i++, 'xls'=>true),
    'Starting date of application a P' => array('k'=>$i++, 'xls'=>true),
    'Starting date of application b' => array('k'=>$i++, 'xls'=>true),
    'Starting date of application c' => array('k'=>$i++, 'xls'=>true),
    //UWWTP treatment and performance',
    'treatment required' => array('k'=>$i++, 'xls'=>true, 'glossary_composed' => false,),
    'treatment type in place (4)' => array('k'=>$i++, 'xls'=>true, 'glossary_composed' => false),
    'Monitoring results - BOD5' => array('k'=>$i++, 'xls'=>true),
    'Monitoring results -COD' => array('k'=>$i++, 'xls'=>true),
    'Monitoring results -Ntot' => array('k'=>$i++, 'xls'=>true),
    'Monitoring results -Ptot' => array('k'=>$i++, 'xls'=>true),
    'Monitoring results -other' => array('k'=>$i++, 'xls'=>true),
    'In cause of failure (3)' => array('k'=>$i++, 'xls'=>true),
    'difference between monitoring result N and P and treatment in place' => array('k'=>$i++, 'xls'=>true),
    
//     'Number of discharge points for this UWWTP' => $i++,
//     'DCP ID' => $i++,
//     //current RCA info (Q 2013)
//     'ID of receiving water body 2' => $i++,
//     'water body type 2' => $i++,
//     'ID of the SA or LSA 2' => $i++,
//     'rcaname 2' => $i++,
//     'designation date 2' => $i++, 

    
    
    //Compliance of treatment plant under expired and pending deadline',
    //compliance Art. 4',
    'art4_treatment type' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art4_monitoring' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art4_final' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //compliance Art. 5',
    'art5_treatment type' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art5_monitoring' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art5_final' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //Compliance of treatment plant under expired deadlines',
    //compliance Art. 4',
    'art4_expired_treatment type' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art4_expired_monitoring' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art4_expired_final' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //compliance Art. 5',
    'art5_expired_treatment type' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art5_expired_monitoring' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art5_expired_final' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //Previous compliance of the treatment under expired deadlines',
    //compliance Art. 4',
    'previous_art4_expired_treatment' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'previous_art4_expired_monitoring' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'previous_art4_expired_final' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //compliance Art. 5',
    'previous_art5_expired_treatment type' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'previous_art5_expired_monitoring' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'previous_art5_expired_final' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'previous_art5_expired_difference between the previous situation and now' => array('k'=>$i++, 'xls'=>true),
    //Installations for wastewater treatment in place',
    'Secondary: installed?' => array('k'=>$i++, 'xls'=>true),
    'Secondary:  monitoring passed?' => array('k'=>$i++, 'xls'=>true),
    'Secondary:  FINAL' => array('k'=>$i++, 'xls'=>true),
    'More stringent:  installed?' => array('k'=>$i++, 'xls'=>true),
    'More stringent: monitoring passed?' => array('k'=>$i++, 'xls'=>true),
    'More stringent:  FINAL' => array('k'=>$i++, 'xls'=>true),
    //UWWTP incoming and discharged load',
    'Volume of waste water treated m3/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'Incoming loads BOD-tot  t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'Discharged loads BOD-tot  t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'BOD rate of removal %' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Incoming concentration BOD mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Discharge concentration BOD mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'Incoming concentration COD-tot   t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'Discharged concentration COD-tot  t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'COD rate of removal %' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Incoming concentration COD mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Discharge concentration COD mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'Incoming loads N-tot   t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'Discharged loads N-tot  t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'N rate of removal %' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Incoming concentration N mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Discharge concentration N mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'Incoming concentration P-tot   t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'Discharged concentration P-tot  t/y' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'P rate of removal %' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Incoming concentration P mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'average Discharge concentration P mg/l' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),    
    
    // info needed for next sheets
    'uwwtp_collecting_without_treatment' => array('k'=>$i++, 'xls'=>false), //todo commenté apres debug
    'required_rcaaP' => array('k'=>$i++, 'xls'=>false),
    'required_rcaaN' => array('k'=>$i++, 'xls'=>false),
    'required_rcab'  => array('k'=>$i++, 'xls'=>false),
    'required_rcac'  => array('k'=>$i++, 'xls'=>false), 
    'field_agglo_uww_perc_ent_uw' => array('k'=>$i++, 'xls'=>false),
    'compliance5a' => array('k'=>$i++, 'xls'=>false),
    'compliance5b' => array('k'=>$i++, 'xls'=>false),
    'compliance5c' => array('k'=>$i++, 'xls'=>false),
    'treatment_met' => array('k'=>$i++, 'xls'=>false),
    
    //debug info
    'debug_pending_or_expired' => array('k'=>$i++, 'xls'=>false),
    'debug_art4_final' => array('k'=>$i++, 'xls'=>false),
);

$i = 0;
$GLOBALS['uwwtd']['sheet3_agglo'] = array(
    'aggcode' => array('k'=>$i++, 'xls'=>true),               
    'aggname' => array('k'=>$i++, 'xls'=>true),
    'generated_load_p.e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'deadline_for_art._3_compliance' => array('k'=>$i++, 'xls'=>true),
    'deadline_for_art._4_compliance' => array('k'=>$i++, 'xls'=>true),
    'deadline_for_art._5_compliance' => array('k'=>$i++, 'xls'=>true),
    'pending_or_expired_deadline_exp/pd/exp-pd' => array('k'=>$i++, 'xls'=>true, 'glossary' => false), //'glossary' => true
    'nuts_1_code' => array('k'=>$i++, 'xls'=>true),
    'nuts_2_code' => array('k'=>$i++, 'xls'=>true),
    'nuts_3_code' => array('k'=>$i++, 'xls'=>true),
    //big_city_info' => array('k'=>$i++, 'xls'=>true),
    'bccode' => array('k'=>$i++, 'xls'=>true),
    'bcname' => array('k'=>$i++, 'xls'=>true),
    //?' => array('k'=>$i++, 'xls'=>true),
    'number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration' => array('k'=>$i++, 'xls'=>true),
    //previous_agglomeration_info' => array('k'=>$i++, 'xls'=>true),
    'previous_reported_?' => array('k'=>$i++, 'xls'=>true),
    'previous_aggcode' => array('k'=>$i++, 'xls'=>true),
    'previous_gen._load_p.e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'previous_percentage_of_change_of_size_(1)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    //connection' => array('k'=>$i++, 'xls'=>true),
    'total_%_(2)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'connected_to_collecting_system' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'connected_to_collecting_system_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'), //19
    'addressed_through_ias_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'addressed_through_ias_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'neither_collecting_system_nor_ias_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'neither_collecting_system_nor_ias_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    //distance_to_compliance_art._3' => array('k'=>$i++, 'xls'=>true),
    'dist_art3_%_of_gen._load_(3)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'dist_art3_in_p.e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    //compliance_art._3' => array('k'=>$i++, 'xls'=>true),
    'art3_compliance' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),   //26
    //compliance_art._3_previous_reporting' => array('k'=>$i++, 'xls'=>true),
    'previous_art3_compliance' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //ratio_connection/collection/treatment' => array('k'=>$i++, 'xls'=>true),
    'total_entering_load_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'ratio_connected/enentering_%' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'total_treatment_plant_design_capacity_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'), //30
    'ratio_total_entering_load/total_design_capacity_%' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    //compliance_art._4' => array('k'=>$i++, 'xls'=>true),
    'art4_compliant_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'art4_compliant_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'art4_not_compliant_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'art4_not_compliant' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'art4_not_relevant_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'art4_not_relevant' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'art4_compliance_art._4_(only)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'art4_compliance_art._4_(hierarchical_approach)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //compliance_art._4_previous_reporting' => array('k'=>$i++, 'xls'=>true),
    'previous_art4_compliance' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //distance_to_compliance_art._4' => array('k'=>$i++, 'xls'=>true),
    //treatment' => array('k'=>$i++, 'xls'=>true),
    'dist_art4_treat__%_of_connected_load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'dist_art4_treat_in_p.e._(4)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    //performance' => array('k'=>$i++, 'xls'=>true),
    'dist_art4_perf_%_of_connected_load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'dist_art4_perf_in_p.e._(5)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    //compliance_art._5' => array('k'=>$i++, 'xls'=>true),
    'compliant_5_a_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'compliant_5_a_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'not_compliant_5_a_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'not_compliant_5_a_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'not_relevant_5_a_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'not_relevant_5_a_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'compliance_art._5a_(only)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'compliance_art._5a_(hierarchical_approach)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'compliant_5_b_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'compliant_5_b_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'not_compliant_5_b_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'not_compliant_5_b_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'not_relevant_5_b_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'not_relevant_5_b_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'compliance_art._5b_(only)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'compliance_art._5b_(hierarchical_approach)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'compliant_5_c_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'compliant_5_c__(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'not_compliant_5_c_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'not_compliant_5_c_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'not_relevant_5_c_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'not_relevant_5_c_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'compliance_art._5c_(only)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'compliance_art._5c_(hierarchical_approach)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'compliant_art._5_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'not_compliant_art._5_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'), //70
    'art._5_not_relevant_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'compliance_art._5_(whole_agglo)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    'compliance_art._5_(whole_agglo)_(hierarchical_approach)' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //compliance_art._5_previous_reporting' => array('k'=>$i++, 'xls'=>true),
    'previous_art5_compliance' => array('k'=>$i++, 'xls'=>true, 'glossary' => false),
    //distance_to_compliance_art._5' => array('k'=>$i++, 'xls'=>true),
    //treatment' => array('k'=>$i++, 'xls'=>true),
// 'previous_art5_treat_%_of_gen._load' => array('k'=>$i++, 'xls'=>true),
// 'previous_art5_treat_in_p.e._(6)' => array('k'=>$i++, 'xls'=>true),
// //performance' => array('k'=>$i++, 'xls'=>true),
// 'previous_art5_perf_%_of_gen._load' => array('k'=>$i++, 'xls'=>true),
// 'previous_art5_perf_in_p.e._(7)' => array('k'=>$i++, 'xls'=>true),
    
    'distance_art5_treat_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'distance_art5_treat_in_p.e._(6)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    //performance' => array('k'=>$i++, 'xls'=>true),
    'distance_art5_perf_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'distance_art5_perf_in_p.e._(7)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    
    //This column only appear in 2018
    'global_compliance'=> array('k'=>$i++, 'xls'=>true),
    
    //current_rca_details' => array('k'=>$i++, 'xls'=>true),
    'water_body_type' => array('k'=>$i++, 'xls'=>true),
    'id_code_of_the_water_body_wfd' => array('k'=>$i++, 'xls'=>true),
    'id_of_wfd_river_basin_district' => array('k'=>$i++, 'xls'=>true),
    'agg_art_applied_from_rca' => array('k'=>$i++, 'xls'=>false),
    'entirely_discharging_into_coastal_waters' => array('k'=>$i++, 'xls'=>true),
    'type_of_receiving_water' => array('k'=>$i++, 'xls'=>true, 'glossary_composed' => false),
    'id_of_the_sa_or_lsa' => array('k'=>$i++, 'xls'=>true),
    'rca_name' => array('k'=>$i++, 'xls'=>true),
    'starting_date_of_application_5.8.' => array('k'=>$i++, 'xls'=>true),
    'starting_date_of_application_5.4.' => array('k'=>$i++, 'xls'=>true),
    'starting_date_of_application_a_n' => array('k'=>$i++, 'xls'=>true),
    'starting_date_of_application_a_p' => array('k'=>$i++, 'xls'=>true),
    'starting_date_of_application_b' => array('k'=>$i++, 'xls'=>true),
    'starting_date_of_application_c' => array('k'=>$i++, 'xls'=>true),
    //installations_for_wastewater_treatment_in_place' => array('k'=>$i++, 'xls'=>true),
    'secondary_treatment_installed_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'secondary_treatment_installed_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'monitoring_results_f._secondary_ok_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'monitoring_results_f._secondary_ok' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'more_str._treatment_installed_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>false, 'wrapper'=>'round1'),
    'more_str._treatment_installed_(p._e.)' => array('k'=>$i++, 'xls'=>false, 'wrapper'=>'round'),
    'more_str._treatment_installed_treatment_met_(%_of_gen._load)'  => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'more_str._treatment_installed_treatment_met_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    'monitoring_results_f._more_string._ok_(%_of_gen._load)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
    'monitoring_results_f._more_string._ok_(p._e.)' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
    
    //data needed fort later sheet    
    'required_rcaaP' => array('k'=>$i++, 'xls'=>false), //TODO comment//todo commenté apres debug
    'required_rcaaN' => array('k'=>$i++, 'xls'=>false),
    'required_rcab'  => array('k'=>$i++, 'xls'=>false),
    'required_rcac'  => array('k'=>$i++, 'xls'=>false), 
            
    //debug data
    'compliance_agglo_global' => array('k'=>$i++, 'xls'=>false), //use in sheet16_agglo_compliance
    'rca_compliance' => array('k'=>$i++, 'xls'=>false), //use in sheet16_agglo_compliance
    
);

$i = 0;
$GLOBALS['uwwtd']['sheet4_summary_legal'] = array(
    'all_agglomerations_number' => $i++,
    'all_agglomerations_percent' => $i++,
    'all_wastewater_load_pe' => $i++,
    'all_wastewater_load_percent' => $i++,
    
    'na_agglomerations_number' => $i++,
    'na_agglomerations_percent' => $i++,
    'na_wastewater_load_pe' => $i++,
    'na_wastewater_load_percent' => $i++,

    'sa_csa_agglomerations_number' => $i++,
    'sa_csa_agglomerations_percent' => $i++,
    'sa_csa_wastewater_load_pe' => $i++,
    'sa_csa_wastewater_load_percent' => $i++, 
);  

		
$i = 0;
$GLOBALS['uwwtd']['sheet8_summary_big_cities'] = array(  
//     'big_dischargers' => $i++,  //e.v add 10th reports (2016)   
    'id' => $i++, 
    'name' => $i++, 
    'generated_load' => $i++, 
    'collecting_system' => $i++, 
    'collecting_system_pe' => $i++, 
    'ias' => $i++, 
    'ias_pe' => $i++, 
    'no_collecting_system_nor_ias' => $i++, 
    'no_collecting_system_nor_ias_pe' => $i++, 
    

    'treatment_0' => $i++, 
    'treatment_1' => $i++, 
    'treatment_2' => $i++, 
    'treatment_3n_p_np' => $i++, 
//     'treatment_3p' => $i++, 
//     'treatment_3np' => $i++, 
    'treatment_3_other_(e.g._uv,_filtration)' => $i++, 
    'type_of_receiving_area' => $i++,
    'id_of_sensitive_area' => $i++,
    'name_of_sensitive_area' => $i++,
    'list_code_agglo' => $i++, 
    'list_name_agglo' => $i++, 
    'target_connection_distance_%' => $i++,      
    'target_connection_distance_pe' => $i++,   
    'secondary_target_monitoring_result_distance_%' => $i++,
    'secondary_target_monitoring_result_distance_pe' => $i++,    
    'morestringent_target_monitoring_result_distance_%' => $i++,    
    'morestringent_target_monitoring_result_distance_pe' => $i++,    
    
); 

$i = 0;
$GLOBALS['uwwtd']['sheet9_nuts2_analyse'] = array(
    'nuts2_code' => $i++,
    'number_agglo' => $i++,
    'generated_load_pe' => $i++,
    'collected_pe' => $i++, 
    'collected_percent' => $i++,
    'ias_pe' => $i++, 
    'ias_percent' => $i++,
    
    'art3_target_load' => $i++,
    'art3_compliant_number_agglo' => $i++,
    'art3_compliant_pe' => $i++,
    'art3_notcompliant_number_agglo' => $i++,
    'art3_notcompliant_pe' => $i++,
    'art3_notrelevant_number_agglo' => $i++,
    'art3_notrelevant_pe' => $i++,    
    'art3_rate' => $i++,
    
    'art4_target_load' => $i++,
    'art4_compliant_number_agglo' => $i++,
    'art4_compliant_pe' => $i++,
    'art4_notcompliant_number_agglo' => $i++,
    'art4_notcompliant_pe' => $i++,
    'art4_notrelevant_number_agglo' => $i++,
    'art4_notrelevant_pe' => $i++,    
    'art4_rate' => $i++,  
    
    'art5_target_load' => $i++,
    'art5_compliant_number_agglo' => $i++,
    'art5_compliant_pe' => $i++,
    'art5_notcompliant_number_agglo' => $i++,
    'art5_notcompliant_pe' => $i++,
    'art5_notrelevant_number_agglo' => $i++,
    'art5_notrelevant_pe' => $i++,    
    'art5_rate' => $i++,     
    'target_connection_distance_%' => $i++,  
    'secondary_target_monitoring_result_distance_%' => $i++,  
    'morestringent_target_monitoring_result_distance_%' => $i++,  
); 

$i = 0;
$GLOBALS['uwwtd']['sheet12_newagglo'] = array(
    'id_of_agglomeration' => $i++,
    'name_of_agglomeration' => $i++, 
    'size_of_agglomeration' => $i++, 
    'reason' => $i++,  
); 

$i = 0;
$GLOBALS['uwwtd']['sheet13_aggloout'] = array(
    'id_of_agglomeration' => $i++,
    'name_of_agglomeration' => $i++, 
    'size_of_agglomeration' => $i++, 
    'remarks' => $i++,  
); 


$i = 0;
$GLOBALS['uwwtd']['sheet14_newuwwtp'] = array(
    'id_of_uwwtp' => $i++,
    'name_of_uwwtp' => $i++, 
    'id_of_agglomeration' => $i++,
    'name_of_agglomeration' => $i++,     
    'size_of_uwwtp' => $i++, 
    'reason' => $i++,  
);

$i = 0;
$GLOBALS['uwwtd']['sheet15_uwwtpout'] = array(
    'id_of_uwwtp' => $i++,
    'name_of_uwwtp' => $i++, 
    'id_of_agglomeration' => $i++,
    'name_of_agglomeration' => $i++,     
    'size_of_uwwtp' => $i++, 
    'remarks' => $i++,  
);  


$i = 0;
$GLOBALS['uwwtd']['sheet16_agglo_compliance'] = array(
    //This data are from csv sheet3_agglo!!
    'aggcode' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['aggcode']['k'], 'xls'=>true),               
    'aggname' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['aggname']['k'], 'xls'=>true),
    'generated_load_p_e' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['generated_load_p.e.']['k'], 'xls'=>true, 'wrapper'=>'round'),
    'deadline_for_art._3_compliance' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['deadline_for_art._3_compliance']['k'], 'xls'=>true),
    'deadline_for_art._4_compliance' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['deadline_for_art._4_compliance']['k'], 'xls'=>true),
    'deadline_for_art._5_compliance' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['deadline_for_art._5_compliance']['k'], 'xls'=>true),
    'pending_or_expired_deadline_exp/pd/exp-pd' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['pending_or_expired_deadline_exp/pd/exp-pd']['k'], 'xls'=>true, 'glossary' => false), //'glossary' => true
    
    'type_of_receiving_water' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['type_of_receiving_water']['k'], 'xls'=>true, 'glossary_composed' => false),
    'id_of_the_sa_or_lsa' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['id_of_the_sa_or_lsa']['k'], 'xls'=>true),
    'rca_name' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['rca_name']['k'], 'xls'=>true),
    'rca_compliance' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['rca_compliance']['k'], 'xls'=>true),
    
    'art3_compliance' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['art3_compliance']['k'], 'xls'=>true, 'glossary' => false), 
    'art4_compliance_art._4_(hierarchical_approach)' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['art4_compliance_art._4_(hierarchical_approach)']['k'], 'xls'=>true, 'glossary' => false),
    'compliance_art._5_(whole_agglo)_(hierarchical_approach)' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['compliance_art._5_(whole_agglo)_(hierarchical_approach)']['k'], 'xls'=>true, 'glossary' => false),
    
    //'global_compliance'=> array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['global_compliance']['k'], 'xls'=>false),
    'compliance_agglo_global' => array('k'=>$GLOBALS['uwwtd']['sheet3_agglo']['compliance_agglo_global']['k'], 'xls'=>true),
    
    
    
    
    
);

unset($i);
