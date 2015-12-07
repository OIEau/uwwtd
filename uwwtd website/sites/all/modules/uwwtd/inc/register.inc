<?php

require_once(drupal_get_path('module', 'uwwtd') . '/inc/register.config.inc');
require_once(drupal_get_path('module', 'uwwtd') . '/inc/utils.inc');
require_once(drupal_get_path('module', 'uwwtd') . '/inc/data_center.inc');
require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
/**
 *
 */
function uwwtd_register_form($form, &$form_state){

//  $uwwtd_year = date("Y");
 
 // definition du champ input de type select pour l'annee
 $form['uwwtd_year'] = array(     
  '#type' => 'select',
  '#title' => t('Year'),
  '#default_value' => uwwtd_get_max_annee(),        
  '#options' => uwwtd_get_all_year(),
  '#weight' => 0,
  '#required' => TRUE,
 );
 
 $form['force'] = array(     
  '#type' => 'checkbox',
  '#title' => t('If register already exists, re-create the file.'),
  '#default_value' => 0,        
  '#weight' => 1,
  '#required' => false,
 ); 
 
 // Bouton envoyer
 $form['submit_button'] = array(
  '#type' => 'submit',
  '#value' => t('Submit'),
  '#weight' => 2,
 );

 return $form;
}

/**
 *
 */
function uwwtd_register_form_submit($form, &$form_state){

     $annee = $form_state['values']['uwwtd_year'];
     $force = $form_state['values']['force'];

    //if registre already calculated and no new conformity or new import_data processed since, we send the file
    $filepath = uwwtd_getregister_finalfile_path($annee);
    if (file_exists($filepath) 
        && variable_get('registre_'.$annee, -2) > variable_get('conformity_'.$annee, -1)
        && variable_get('registre_'.$annee, -2) > variable_get('import_data_'.$annee, -1)
        && $force == 0) {
        drupal_set_message(uwwtd_registre_create_html_link($annee));
    } else {
        uwwtd_setup_register_batch($annee);
    }
}

function uwwtd_register_set_operation(&$batch, $annee)
{
    //CSV 
    $batch['operations'][]=array('uwwtd_create_register_2015_rca_sheet1', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_uwwtp_sheet2', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_agglo_sheet3', array($annee));


    $batch['operations'][]=array('uwwtd_create_register_2015_summary_big_cities_sheet8', array($annee));           
    $batch['operations'][]=array('uwwtd_create_register_2015_nuts_level2_analyse_sheet9', array($annee));

    $batch['operations'][]=array('uwwtd_create_register_2015_new_agglomerations_sheet12', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_agglomerationout_sheet13', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_new_treatment_plants_sheet14', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_uwwtpsout_sheet15', array($annee));           

    //excel        
    $batch['operations'][]=array('uwwtd_create_register_2015_file_rca_sheet1', array($annee));                      
    $batch['operations'][]=array('uwwtd_create_register_2015_file_uwwtp_sheet2', array($annee));                    
    $batch['operations'][]=array('uwwtd_create_register_2015_file_agglo_sheet3', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_summary_legal_compliance_sheet4', array($annee));                    
    $batch['operations'][]=array('uwwtd_create_register_2015_file_distance_to_compliance_sheet5', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_summary_installation_in_place_sheet6', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_breach_list_sheet7', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_summary_big_cities_sheet8', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_nuts_level2_analyse_sheet9', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_sewage_sludge_and_re_use_sheet10', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_graph_sheet11', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_new_agglomerations_sheet12', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_agglomerationout_sheet13', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_new_treatment_plants_sheet14', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_uwwtpsout_sheet15', array($annee));                    

    $batch['operations'][]=array('uwwtd_create_register_2015_final_file', array($annee));  
}

function uwwtd_setup_register_batch($annee)
{
	$batch = array(
		'title' => t('Register creation for ' . $annee),
		'operations' => array(),
		'error_message' => t('Error!'),
		'finished' => 'uwwtd_register_finished',
		'file' => drupal_get_path('module','uwwtd').'/inc/register.inc'
	);   
    
    uwwtd_register_set_operation($batch, $annee);
    
  	batch_set($batch);
	batch_process('register');
}

function uwwtd_register_finished($success, $results, $operations)
{
    if ($success) {
		drupal_set_message('success');
	}
	else {
		drupal_set_message(
			t('An error occurred ')
		);  
	}
}

function uwwtd_create_register_2015_file_agglo_sheet3($annee, &$context)
{   
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
    
    $ragglofile = fopen($filename, 'rb');
    
    //pass header line
    if (!isset($context['sandbox']['progress'])) { 
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        //ignore header    
        $agglo = fgetcsv($ragglofile, 5000, ';');
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['line'] = 4; //start line
        
        $path_xlsx = uwwtd_getregister_modelfile_path();
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);        
    } else {        
//         $path_xlsx = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'] .'.xlsx';
        $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);
        $target_path = $path_xlsx;          
    }     
    
    fseek($ragglofile, $context['sandbox']['file_position']);
        
//  	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
                                                                                           
//     $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
//     $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'] .'.xlsx';
//     $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);
    

//     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp);
//     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_sqlite);
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
 
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name'],
                                        ) );
                                        
    $objPHPExcel = $objReader->load(drupal_realpath($path_xlsx));
           
//     dsm(444);
//     return '111';       
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name']);
    
//     $start_line = 4;
//     $sheet->insertNewRowBefore(5, count($listagglo));
    
//     $i = $start_line;
//     $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);//$path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
//     dsm($filename);           
//     $ragglofile = fopen($filename, 'rb');
//     dsm($ragglofile);
    //ignore header //decomment for debug on xls final file
//     $agglo = fgetcsv($ragglofile, 5000, ';');

//     while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {      
//         dsm($agglo);
//         break;        
//     foreach($listagglo as $agglo) {
//         } else {
        $sheet->setCellValueByColumnAndRow($ind['aggcode'], $context['sandbox']['line'], $agglo[$ind['aggcode']]);
        $sheet->setCellValueByColumnAndRow($ind['aggname'], $context['sandbox']['line'], $agglo[$ind['aggname']]);
        $sheet->setCellValueByColumnAndRow($ind['generated_load_p.e.'], $context['sandbox']['line'], $agglo[$ind['generated_load_p.e.']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline_for_art._3_compliance'], $context['sandbox']['line'], $agglo[$ind['deadline_for_art._3_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline_for_art._4_compliance'], $context['sandbox']['line'], $agglo[$ind['deadline_for_art._4_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline_for_art._5_compliance'], $context['sandbox']['line'], $agglo[$ind['deadline_for_art._5_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['pending_or_expired_deadline_exp/pd/exp-pd'], $context['sandbox']['line'], $agglo[$ind['pending_or_expired_deadline_exp/pd/exp-pd']]);
        $sheet->setCellValueByColumnAndRow($ind['nuts_1_code'], $context['sandbox']['line'], $agglo[$ind['nuts_1_code']]);
        $sheet->setCellValueByColumnAndRow($ind['nuts_2_code'], $context['sandbox']['line'], $agglo[$ind['nuts_2_code']]);
        $sheet->setCellValueByColumnAndRow($ind['nuts_3_code'], $context['sandbox']['line'], $agglo[$ind['nuts_3_code']]);
        $sheet->setCellValueByColumnAndRow($ind['bccode'], $context['sandbox']['line'], $agglo[$ind['bccode']]);
        $sheet->setCellValueByColumnAndRow($ind['bcname'], $context['sandbox']['line'], $agglo[$ind['bcname']]);
        $sheet->setCellValueByColumnAndRow($ind['number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration'], $context['sandbox']['line'], $agglo[$ind['number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_reported_?'], $context['sandbox']['line'], $agglo[$ind['previous_reported_?']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_aggcode'], $context['sandbox']['line'], $agglo[$ind['previous_aggcode']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_gen._load_p.e.'], $context['sandbox']['line'], $agglo[$ind['previous_gen._load_p.e.']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_percentage_of_change_of_size_(1)'], $context['sandbox']['line'], $agglo[$ind['previous_percentage_of_change_of_size_(1)']]);
        
        if (abs((float)$agglo[$ind['previous_percentage_of_change_of_size_(1)']]) > 20) {
            $sheet->getStyleByColumnAndRow($ind['previous_percentage_of_change_of_size_(1)'], $context['sandbox']['line'])
                ->getFill()
                ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)      // 255 192 0 FFC000
                ->getStartColor()->setARGB('FFFFC000');        
        }  
                
        $sheet->setCellValueByColumnAndRow($ind['total_%_(2)'], $context['sandbox']['line'], $agglo[$ind['total_%_(2)']]);
        
        if ($agglo[$ind['total_%_(2)']] < 100) {
            $sheet->getStyleByColumnAndRow($ind['total_%_(2)'], $context['sandbox']['line'])
                ->getFill()
                ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)      // 255 192 0 FFC000
                ->getStartColor()->setARGB('FFFFC000');        
        }  
                
        $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system'], $context['sandbox']['line'], $agglo[$ind['connected_to_collecting_system']]);
        $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system_p._e.'], $context['sandbox']['line'], $agglo[$ind['connected_to_collecting_system_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_%_of_gen._load'], $context['sandbox']['line'], $agglo[$ind['addressed_through_ias_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_p._e.'], $context['sandbox']['line'], $agglo[$ind['addressed_through_ias_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_%_of_gen._load'], $context['sandbox']['line'], $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_p._e.'], $context['sandbox']['line'], $agglo[$ind['neither_collecting_system_nor_ias_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art3_%_of_gen._load_(3)'], $context['sandbox']['line'], $agglo[$ind['dist_art3_%_of_gen._load_(3)']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art3_in_p.e.'], $context['sandbox']['line'], $agglo[$ind['dist_art3_in_p.e.']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_compliance'], $context['sandbox']['line'], $agglo[$ind['art3_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art3_compliance'], $context['sandbox']['line'], $agglo[$ind['previous_art3_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['total_entering_load_p._e.'], $context['sandbox']['line'], $agglo[$ind['total_entering_load_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['ratio_connected/enentering_%'], $context['sandbox']['line'], $agglo[$ind['ratio_connected/enentering_%']]);
        $sheet->setCellValueByColumnAndRow($ind['total_treatment_plant_design_capacity_p._e.'], $context['sandbox']['line'], $agglo[$ind['total_treatment_plant_design_capacity_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['ratio_total_entering_load/total_design_capacity_%'], $context['sandbox']['line'], $agglo[$ind['ratio_total_entering_load/total_design_capacity_%']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliant_%_of_gen._load'], $context['sandbox']['line'], $agglo[$ind['art4_compliant_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliant_p._e.'], $context['sandbox']['line'], $agglo[$ind['art4_compliant_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant_%_of_gen._load'], $context['sandbox']['line'], $agglo[$ind['art4_not_compliant_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant'], $context['sandbox']['line'], $agglo[$ind['art4_not_compliant']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant_%_of_gen._load'], $context['sandbox']['line'], $agglo[$ind['art4_not_relevant_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant'], $context['sandbox']['line'], $agglo[$ind['art4_not_relevant']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliance_art._4_(only)'], $context['sandbox']['line'], $agglo[$ind['art4_compliance_art._4_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliance_art._4_(hierarchical_approach)'], $context['sandbox']['line'], $agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_compliance'], $context['sandbox']['line'], $agglo[$ind['previous_art4_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat__%_of_connected_load'], $context['sandbox']['line'], $agglo[$ind['dist_art4_treat__%_of_connected_load']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat_in_p.e._(4)'], $context['sandbox']['line'], $agglo[$ind['dist_art4_treat_in_p.e._(4)']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_%_of_connected_load'], $context['sandbox']['line'], $agglo[$ind['dist_art4_perf_%_of_connected_load']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_in_p.e._(5)'], $context['sandbox']['line'], $agglo[$ind['dist_art4_perf_in_p.e._(5)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_a_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['compliant_5_a_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_a_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['compliant_5_a_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_a_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['not_compliant_5_a_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_a_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['not_compliant_5_a_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_a_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['not_relevant_5_a_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_a_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['not_relevant_5_a_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5a_(only)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5a_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5a_(hierarchical_approach)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5a_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_b_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['compliant_5_b_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_b_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['compliant_5_b_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_b_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['not_compliant_5_b_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_b_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['not_compliant_5_b_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_b_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['not_relevant_5_b_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_b_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['not_relevant_5_b_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5b_(only)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5b_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5b_(hierarchical_approach)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5b_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_c_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['compliant_5_c_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_c__(p._e.)'], $context['sandbox']['line'], $agglo[$ind['compliant_5_c__(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_c_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['not_compliant_5_c_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_c_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['not_compliant_5_c_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_c_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['not_relevant_5_c_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_c_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['not_relevant_5_c_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5c_(only)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5c_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5c_(hierarchical_approach)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5c_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_art._5_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['compliant_art._5_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_art._5_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['not_compliant_art._5_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['art._5_not_relevant_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['art._5_not_relevant_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5_(whole_agglo)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5_(whole_agglo)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)'], $context['sandbox']['line'], $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_compliance'], $context['sandbox']['line'], $agglo[$ind['previous_art5_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['distance_art5_treat_%_of_gen._load'], $context['sandbox']['line'], $agglo[$ind['distance_art5_treat_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['distance_art5_treat_in_p.e._(6)'], $context['sandbox']['line'], $agglo[$ind['distance_art5_treat_in_p.e._(6)']]);
        $sheet->setCellValueByColumnAndRow($ind['distance_art5_perf_%_of_gen._load'], $context['sandbox']['line'], $agglo[$ind['distance_art5_perf_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['distance_art5_perf_in_p.e._(7)'], $context['sandbox']['line'], $agglo[$ind['distance_art5_perf_in_p.e._(7)']]);
        $sheet->setCellValueByColumnAndRow($ind['water_body_type'], $context['sandbox']['line'], $agglo[$ind['water_body_type']]);
        $sheet->setCellValueByColumnAndRow($ind['id_code_of_the_water_body_wfd'], $context['sandbox']['line'], $agglo[$ind['id_code_of_the_water_body_wfd']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_wfd_river_basin_district'], $context['sandbox']['line'], $agglo[$ind['id_of_wfd_river_basin_district']]);
        $sheet->setCellValueByColumnAndRow($ind['entirely_discharging_into_coastal_waters?'], $context['sandbox']['line'], $agglo[$ind['entirely_discharging_into_coastal_waters?']]);
        $sheet->setCellValueByColumnAndRow($ind['type_of_receiving_water'], $context['sandbox']['line'], $agglo[$ind['type_of_receiving_water']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_the_sa_or_lsa'], $context['sandbox']['line'], $agglo[$ind['id_of_the_sa_or_lsa']]);
        $sheet->setCellValueByColumnAndRow($ind['rca_name'], $context['sandbox']['line'], $agglo[$ind['rca_name']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_5.8.'], $context['sandbox']['line'], $agglo[$ind['starting_date_of_application_5.8.']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_5.4.'], $context['sandbox']['line'], $agglo[$ind['starting_date_of_application_5.4.']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_a_n'], $context['sandbox']['line'], $agglo[$ind['starting_date_of_application_a_n']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_a_p'], $context['sandbox']['line'], $agglo[$ind['starting_date_of_application_a_p']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_b'], $context['sandbox']['line'], $agglo[$ind['starting_date_of_application_b']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_c'], $context['sandbox']['line'], $agglo[$ind['starting_date_of_application_c']]);
        $sheet->setCellValueByColumnAndRow($ind['secondary_treatment_installed_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['secondary_treatment_installed_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['secondary_treatment_installed_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['secondary_treatment_installed_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._secondary_ok_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['monitoring_results_f._secondary_ok_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._secondary_ok'], $context['sandbox']['line'], $agglo[$ind['monitoring_results_f._secondary_ok']]);
        $sheet->setCellValueByColumnAndRow($ind['more_str._treatment_installed_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['more_str._treatment_installed_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['more_str._treatment_installed_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['more_str._treatment_installed_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._more_string._ok_(%_of_gen._load)'], $context['sandbox']['line'], $agglo[$ind['monitoring_results_f._more_string._ok_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._more_string._ok_(p._e.)'], $context['sandbox']['line'], $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']]);
        
        //debug data info
        $sheet->setCellValueByColumnAndRow($ind['debug_compliance_agglo_from_algo'], $context['sandbox']['line'], $agglo[$ind['debug_compliance_agglo_from_algo']]);
//         }
//         $i++;
//         break;
        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'], 1);

        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];           
    }
    fclose($ragglofile);
    
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
//         dsm($context['sandbox']['line']);
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_total_name'], $annee);
        $r = fopen($filename, 'rb');
        $line_to_fill = 0;  
        for ($i = 0; $i <= 3; $i++) {            
            $agglo = fgetcsv($r, 5000, ';');
//             dsm($agglo);
            switch($agglo[0]) {
                case 'TOTAL':
                    $line_to_fill = $context['sandbox']['line'] + 2;
                break; 
                case 'expired_deadline':
                     $line_to_fill = $context['sandbox']['line'] + 3;
                break; 
                case 'pending_deadline':
                     $line_to_fill = $context['sandbox']['line'] + 4;
                break; 
                default:
//                     dsm('error for line :');
//                     dsm($agglo);
                    continue 2;                                
            }
//             dsm($ind['generated_load_p.e.'] . ',' .$line_to_fill . ','. $agglo[$ind['generated_load_p.e.']]);
            $sheet->setCellValueByColumnAndRow($ind['generated_load_p.e.'], $line_to_fill, $agglo[$ind['generated_load_p.e.']]);
            $sheet->setCellValueByColumnAndRow($ind['previous_gen._load_p.e.'], $line_to_fill, $agglo[$ind['previous_gen._load_p.e.']]);
            $sheet->setCellValueByColumnAndRow($ind['total_%_(2)'], $line_to_fill, $agglo[$ind['total_%_(2)']]);
            $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system'], $line_to_fill, $agglo[$ind['connected_to_collecting_system']]);
            $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system_p._e.'], $line_to_fill, $agglo[$ind['connected_to_collecting_system_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_%_of_gen._load'], $line_to_fill, $agglo[$ind['addressed_through_ias_%_of_gen._load']]);
            $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_p._e.'], $line_to_fill, $agglo[$ind['addressed_through_ias_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_%_of_gen._load'], $line_to_fill, $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']]);
            $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_p._e.'], $line_to_fill, $agglo[$ind['neither_collecting_system_nor_ias_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art3_%_of_gen._load_(3)'], $line_to_fill, $agglo[$ind['dist_art3_%_of_gen._load_(3)']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art3_in_p.e.'], $line_to_fill, $agglo[$ind['dist_art3_in_p.e.']]);
            $sheet->setCellValueByColumnAndRow($ind['total_entering_load_p._e.'], $line_to_fill, $agglo[$ind['total_entering_load_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['ratio_connected/enentering_%'], $line_to_fill, $agglo[$ind['ratio_connected/enentering_%']]);
            $sheet->setCellValueByColumnAndRow($ind['total_treatment_plant_design_capacity_p._e.'], $line_to_fill, $agglo[$ind['total_treatment_plant_design_capacity_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['ratio_total_entering_load/total_design_capacity_%'], $line_to_fill, $agglo[$ind['ratio_total_entering_load/total_design_capacity_%']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_compliant_%_of_gen._load'], $line_to_fill, $agglo[$ind['art4_compliant_%_of_gen._load']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_compliant_p._e.'], $line_to_fill, $agglo[$ind['art4_compliant_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant_%_of_gen._load'], $line_to_fill, $agglo[$ind['art4_not_compliant_%_of_gen._load']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant'], $line_to_fill, $agglo[$ind['art4_not_compliant']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant_%_of_gen._load'], $line_to_fill, $agglo[$ind['art4_not_relevant_%_of_gen._load']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant'], $line_to_fill, $agglo[$ind['art4_not_relevant']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat__%_of_connected_load'], $line_to_fill, $agglo[$ind['dist_art4_treat__%_of_connected_load']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat_in_p.e._(4)'], $line_to_fill, $agglo[$ind['dist_art4_treat_in_p.e._(4)']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_%_of_connected_load'], $line_to_fill, $agglo[$ind['dist_art4_perf_%_of_connected_load']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_in_p.e._(5)'], $line_to_fill, $agglo[$ind['dist_art4_perf_in_p.e._(5)']]);
            $sheet->setCellValueByColumnAndRow($ind['distance_art5_treat_%_of_gen._load'], $line_to_fill, $agglo[$ind['distance_art5_treat_%_of_gen._load']]);
            $sheet->setCellValueByColumnAndRow($ind['distance_art5_treat_in_p.e._(6)'], $line_to_fill, $agglo[$ind['distance_art5_treat_in_p.e._(6)']]);
            $sheet->setCellValueByColumnAndRow($ind['distance_art5_perf_%_of_gen._load'], $line_to_fill, $agglo[$ind['distance_art5_perf_%_of_gen._load']]);
            $sheet->setCellValueByColumnAndRow($ind['distance_art5_perf_in_p.e._(7)'], $line_to_fill, $agglo[$ind['distance_art5_perf_in_p.e._(7)']]);
            $sheet->setCellValueByColumnAndRow($ind['secondary_treatment_installed_(p._e.)'], $line_to_fill, $agglo[$ind['secondary_treatment_installed_(p._e.)']]);
            $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._secondary_ok'], $line_to_fill, $agglo[$ind['monitoring_results_f._secondary_ok']]);
            $sheet->setCellValueByColumnAndRow($ind['more_str._treatment_installed_(p._e.)'], $line_to_fill, $agglo[$ind['more_str._treatment_installed_(p._e.)']]);
            $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._more_string._ok_(p._e.)'], $line_to_fill, $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']]);            
            
        }
        fclose($r);
        
        
        //write evulation data
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_evaluation_name'], $annee);
        $r = fopen($filename, 'rb');
        for ($i = 0; $i <= 3; $i++) {            
            $agglo = fgetcsv($r, 5000, ';');
            
            switch($i) {
                case 0:
                    $line_to_fill = $context['sandbox']['line'] + 7;
                break; 
                case 1:
                     $line_to_fill = $context['sandbox']['line'] + 17;
                break; 
                case 2:
                     $line_to_fill = $context['sandbox']['line'] + 27;
                break;                              
            }
                   
            //subjected_agglo                            
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill++, $agglo[0]);
            //compliant_agglo
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill++, $agglo[1]);
            //notcompliant_agglo
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill++, $agglo[2]);
            
            //subjected_load     generated
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill, $agglo[3]);
            //subjected_load    collected
            $sheet->setCellValueByColumnAndRow(6, $line_to_fill++, $agglo[4]);
            
            //compliant_load     generated
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill, $agglo[5]);
            //compliant_load     collected
            $sheet->setCellValueByColumnAndRow(6, $line_to_fill++, $agglo[6]);
            
            //notcompliant_load   generated
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill, $agglo[7]);
            //notcompliant_load   collected
            $sheet->setCellValueByColumnAndRow(6, $line_to_fill++, $agglo[8]);
            
            //exempted
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill++, $agglo[9]);
            
            //exempted_load   generated
            $sheet->setCellValueByColumnAndRow(5, $line_to_fill, $agglo[10]);
            //exempted  collected
            $sheet->setCellValueByColumnAndRow(6, $line_to_fill++, $agglo[11]);         
        }
        fclose($r);          
    }    
   
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
//     $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
                                            
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }              
}

function uwwtd_create_register_2015_file_uwwtp_sheet2($annee, &$context)
{   
    uwwtd_trace(__FUNCTION__); 
// 	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }
    $ind = $GLOBALS['uwwtd']['sheet2_uwwtp'];
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] . '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'], $annee);           
    
//     dsm($filename);
// 	if (!isset($context['sandbox']['progress'])) {  
//         $context['sandbox']['file_position'] = 0;  
//         $context['sandbox']['progress'] = $context['sandbox']['file_position'];
//         $context['sandbox']['max'] = filesize($filename);
//     }
    
    $ruwwtpfile = fopen($filename, 'rb');
    
    //pass header line
    if (!isset($context['sandbox']['progress'])) { 
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        
        //ignore header    
        $uwwtp = fgetcsv($ruwwtpfile, 5000, ';');
        $context['sandbox']['file_position'] = ftell($ruwwtpfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['line'] = 4; //start line
        
        
        
        $path_xlsx = uwwtd_getregister_modelfile_path();
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'], $annee);        
    } else {        
//         $path_xlsx = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'] .'.xlsx';
        $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'], $annee);
        $target_path = $path_xlsx;          
    }  
    
//     dsm($context);
//     dsm($path_xlsx);
//     dsm($target_path); 
    
    fseek($ruwwtpfile, $context['sandbox']['file_position']); 


    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'],
                                        ) );
    uwwtd_trace('start load');                                        
    $objPHPExcel = $objReader->load(drupal_realpath($path_xlsx));
    uwwtd_trace('end load');
    uwwtd_trace('start getSheetByName');
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name']);
    uwwtd_trace('end getSheetByName');
    
//     $sheet->insertNewRowBefore(5, count($listuwwtp));
    
//     $i = $context['sandbox']['line'];

    
    //ignore header //decomment for debug on xls final file
//     $uwwtp = fgetcsv($ruwwtpfile, 5000, ';');

//     while ($uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {    
    $j = 0;
    uwwtd_trace('start loop');
    while ($j < 20 && $uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {
            uwwtd_trace($j);    
//         if ($uwwtp[0] == 'TOTAL' || $uwwtp[0] == 'expired_deadline' || $uwwtp[0] == 'pending_deadline' ) {
//             $line_to_fill = 0;
//             switch($uwwtp[0]) {
//                 case 'TOTAL';
//                     $line_to_fill = $context['sandbox']['line'] + 2;
//                 break; 
//                 case 'expired_deadline';
//                      $line_to_fill = $context['sandbox']['line'] + 3;
//                 break; 
//                 case 'pending_deadline';
//                      $line_to_fill = $context['sandbox']['line'] + 4;
//                 break;                                 
//             }
//             $sheet->setCellValueByColumnAndRow($ind['uww_entering this UWWTP % of gen. load'], $line_to_fill, $uwwtp[$ind['uww_entering this UWWTP % of gen. load']]);
//             $sheet->setCellValueByColumnAndRow($ind['uww_entering load  p. e.'], $line_to_fill, $uwwtp[$ind['uww_entering load  p. e.']]);
//             $sheet->setCellValueByColumnAndRow($ind['uww_capacity p. e.'], $line_to_fill, $uwwtp[$ind['uww_capacity p. e.']]);
//             $sheet->setCellValueByColumnAndRow($ind['previous_entering load p. e.'], $line_to_fill, $uwwtp[$ind['previous_entering load p. e.']]);
//             $sheet->setCellValueByColumnAndRow($ind['previous_capacity p. e.'], $line_to_fill, $uwwtp[$ind['previous_capacity p. e.']]);
//             $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of entering load (1) %'], $line_to_fill, $uwwtp[$ind['previous_Percentage of change of entering load (1) %']]);
//             $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of capacity (2) %'], $line_to_fill, $uwwtp[$ind['previous_Percentage of change of capacity (2) %']]);
//         } else {    
                   
    //     foreach($listuwwtp as $uwwtp) {  
        $sheet->setCellValueByColumnAndRow($ind['aggcode'], $context['sandbox']['line'], $uwwtp[$ind['aggcode']]);
        $sheet->setCellValueByColumnAndRow($ind['aggname'], $context['sandbox']['line'], $uwwtp[$ind['aggname']]);
        $sheet->setCellValueByColumnAndRow($ind['aggstatus'], $context['sandbox']['line'], $uwwtp[$ind['aggstatus']]);
        $sheet->setCellValueByColumnAndRow($ind['generated load  p. e.'], $context['sandbox']['line'], $uwwtp[$ind['generated load  p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline for Art. 3 compliance'], $context['sandbox']['line'], $uwwtp[$ind['deadline for Art. 3 compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline for Art. 4 compliance'], $context['sandbox']['line'], $uwwtp[$ind['deadline for Art. 4 compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline for Art. 5 compliance'], $context['sandbox']['line'], $uwwtp[$ind['deadline for Art. 5 compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['collected in collecting system % of gen. load'], $context['sandbox']['line'], $uwwtp[$ind['collected in collecting system % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['collected in collecting system  p. e.'], $context['sandbox']['line'], $uwwtp[$ind['collected in collecting system  p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed through IAS % of gen. load'], $context['sandbox']['line'], $uwwtp[$ind['addressed through IAS % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed through IAS p. e.'], $context['sandbox']['line'], $uwwtp[$ind['addressed through IAS p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['neither collecting system nor IAS % of gen. load'], $context['sandbox']['line'], $uwwtp[$ind['neither collecting system nor IAS % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['neither collecting system nor IAS p. e.'], $context['sandbox']['line'], $uwwtp[$ind['neither collecting system nor IAS p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_code'], $context['sandbox']['line'], $uwwtp[$ind['uww_code']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_name'], $context['sandbox']['line'], $uwwtp[$ind['uww_name']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_status'], $context['sandbox']['line'], $uwwtp[$ind['uww_status']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_entering this UWWTP % of gen. load'], $context['sandbox']['line'], $uwwtp[$ind['uww_entering this UWWTP % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_entering load  p. e.'], $context['sandbox']['line'], $uwwtp[$ind['uww_entering load  p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_capacity p. e.'], $context['sandbox']['line'], $uwwtp[$ind['uww_capacity p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['uww2_status'], $context['sandbox']['line'], $uwwtp[$ind['uww2_status']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_reference year'], $context['sandbox']['line'], $uwwtp[$ind['previous_reference year']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_entering load p. e.'], $context['sandbox']['line'], $uwwtp[$ind['previous_entering load p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_capacity p. e.'], $context['sandbox']['line'], $uwwtp[$ind['previous_capacity p. e.']]);
        
        $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of entering load (1) %'], $context['sandbox']['line'], $uwwtp[$ind['previous_Percentage of change of entering load (1) %']]);
        if (abs((float)$uwwtp[$ind['previous_Percentage of change of entering load (1) %']]) > 20) {
            $sheet->getStyleByColumnAndRow($ind['previous_Percentage of change of entering load (1) %'], $context['sandbox']['line'])
                ->getFill()
                ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)
                ->getStartColor()->setARGB('FFFFC000');
        }

        $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of capacity (2) %'],$context['sandbox']['line'], $uwwtp[$ind['previous_Percentage of change of capacity (2) %']]);
        if (abs((float)$uwwtp[$ind['previous_Percentage of change of capacity (2) %']]) > 0) {
            $sheet->getStyleByColumnAndRow($ind['previous_Percentage of change of capacity (2) %'], $context['sandbox']['line'])
                ->getFill()
                ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)      // 255 192 0 FFC000
                ->getStartColor()->setARGB('FFFFC000');        
        }  
                  
        $sheet->setCellValueByColumnAndRow($ind['water body type'],$context['sandbox']['line'], $uwwtp[$ind['water body type']]);
        $sheet->setCellValueByColumnAndRow($ind['In case of discharge on land please specify the purpose'],$context['sandbox']['line'], $uwwtp[$ind['In case of discharge on land please specify the purpose']]);
        $sheet->setCellValueByColumnAndRow($ind['Id code of the water body WFD'],$context['sandbox']['line'], $uwwtp[$ind['Id code of the water body WFD']]);
        $sheet->setCellValueByColumnAndRow($ind['ID of WFD river basin district'],$context['sandbox']['line'], $uwwtp[$ind['ID of WFD river basin district']]);
        $sheet->setCellValueByColumnAndRow($ind['entirely discharging into coastal waters?'],$context['sandbox']['line'], $uwwtp[$ind['entirely discharging into coastal waters?']]);
        $sheet->setCellValueByColumnAndRow($ind['type of receiving water'],$context['sandbox']['line'], $uwwtp[$ind['type of receiving water']]);
        $sheet->setCellValueByColumnAndRow($ind['ID of the SA or LSA'],$context['sandbox']['line'], $uwwtp[$ind['ID of the SA or LSA']]);
        $sheet->setCellValueByColumnAndRow($ind['rcaname'],$context['sandbox']['line'], $uwwtp[$ind['rcaname']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application 5.8.'],$context['sandbox']['line'], $uwwtp[$ind['Starting date of application 5.8.']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application 5.4.'],$context['sandbox']['line'], $uwwtp[$ind['Starting date of application 5.4.']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application a N'],$context['sandbox']['line'], $uwwtp[$ind['Starting date of application a N']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application a P'],$context['sandbox']['line'], $uwwtp[$ind['Starting date of application a P']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application b'],$context['sandbox']['line'], $uwwtp[$ind['Starting date of application b']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application c'],$context['sandbox']['line'], $uwwtp[$ind['Starting date of application c']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment required'],$context['sandbox']['line'], $uwwtp[$ind['treatment required']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment type in place (4)'],$context['sandbox']['line'], $uwwtp[$ind['treatment type in place (4)']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results - BOD5'],$context['sandbox']['line'], $uwwtp[$ind['Monitoring results - BOD5']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -COD'],$context['sandbox']['line'], $uwwtp[$ind['Monitoring results -COD']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -Ntot'],$context['sandbox']['line'], $uwwtp[$ind['Monitoring results -Ntot']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -Ptot'],$context['sandbox']['line'], $uwwtp[$ind['Monitoring results -Ptot']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -other'],$context['sandbox']['line'], $uwwtp[$ind['Monitoring results -other']]);
        $sheet->setCellValueByColumnAndRow($ind['In cause of failure (3)'],$context['sandbox']['line'], $uwwtp[$ind['In cause of failure (3)']]);
        $sheet->setCellValueByColumnAndRow($ind['difference between monitoring result N and P and treatment in place'],$context['sandbox']['line'], $uwwtp[$ind['difference between monitoring result N and P and treatment in place']]);
        
//         $sheet->setCellValueByColumnAndRow($ind['Number of discharge points for this UWWTP'],$context['sandbox']['line'], $uwwtp[$ind['Number of discharge points for this UWWTP']]);
//         $sheet->setCellValueByColumnAndRow($ind['DCP ID'],$context['sandbox']['line'], $uwwtp[$ind['DCP ID']]);
//         $sheet->setCellValueByColumnAndRow($ind['ID of receiving water body 2'],$context['sandbox']['line'], $uwwtp[$ind['ID of receiving water body 2']]);
//         $sheet->setCellValueByColumnAndRow($ind['water body type 2'],$context['sandbox']['line'], $uwwtp[$ind['water body type 2']]);
//         $sheet->setCellValueByColumnAndRow($ind['ID of the SA or LSA 2'],$context['sandbox']['line'], $uwwtp[$ind['ID of the SA or LSA 2']]);
//         $sheet->setCellValueByColumnAndRow($ind['rcaname 2'],$context['sandbox']['line'], $uwwtp[$ind['rcaname 2']]);
//         $sheet->setCellValueByColumnAndRow($ind['designation date 2'],$context['sandbox']['line'], $uwwtp[$ind['designation date 2']]);  
        
        
        $sheet->setCellValueByColumnAndRow($ind['art4_treatment type'],$context['sandbox']['line'], $uwwtp[$ind['art4_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_monitoring'],$context['sandbox']['line'], $uwwtp[$ind['art4_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_final'],$context['sandbox']['line'], $uwwtp[$ind['art4_final']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_treatment type'],$context['sandbox']['line'], $uwwtp[$ind['art5_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_monitoring'],$context['sandbox']['line'], $uwwtp[$ind['art5_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_final'],$context['sandbox']['line'], $uwwtp[$ind['art5_final']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_expired_treatment type'],$context['sandbox']['line'], $uwwtp[$ind['art4_expired_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_expired_monitoring'],$context['sandbox']['line'], $uwwtp[$ind['art4_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_expired_final'],$context['sandbox']['line'], $uwwtp[$ind['art4_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_expired_treatment type'],$context['sandbox']['line'], $uwwtp[$ind['art5_expired_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_expired_monitoring'],$context['sandbox']['line'], $uwwtp[$ind['art5_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_expired_final'],$context['sandbox']['line'], $uwwtp[$ind['art5_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_expired_treatment'],$context['sandbox']['line'], $uwwtp[$ind['previous_art4_expired_treatment']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_expired_monitoring'],$context['sandbox']['line'], $uwwtp[$ind['previous_art4_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_expired_final'],$context['sandbox']['line'], $uwwtp[$ind['previous_art4_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_treatment type'],$context['sandbox']['line'], $uwwtp[$ind['previous_art5_expired_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_monitoring'],$context['sandbox']['line'], $uwwtp[$ind['previous_art5_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_final'],$context['sandbox']['line'], $uwwtp[$ind['previous_art5_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_difference between the previous situation and now'],$context['sandbox']['line'], $uwwtp[$ind['previous_art5_expired_difference between the previous situation and now']]);
        $sheet->setCellValueByColumnAndRow($ind['Secondary: installed?'],$context['sandbox']['line'], $uwwtp[$ind['Secondary: installed?']]);
        $sheet->setCellValueByColumnAndRow($ind['Secondary:  monitoring passed?'],$context['sandbox']['line'], $uwwtp[$ind['Secondary:  monitoring passed?']]);
        $sheet->setCellValueByColumnAndRow($ind['Secondary:  FINAL'],$context['sandbox']['line'], $uwwtp[$ind['Secondary:  FINAL']]);
        $sheet->setCellValueByColumnAndRow($ind['More stringent:  installed?'],$context['sandbox']['line'], $uwwtp[$ind['More stringent:  installed?']]);
        $sheet->setCellValueByColumnAndRow($ind['More stringent: monitoring passed?'],$context['sandbox']['line'], $uwwtp[$ind['More stringent: monitoring passed?']]);
        $sheet->setCellValueByColumnAndRow($ind['More stringent:  FINAL'],$context['sandbox']['line'], $uwwtp[$ind['More stringent:  FINAL']]);
        $sheet->setCellValueByColumnAndRow($ind['Volume of waste water treated m3/y'],$context['sandbox']['line'], $uwwtp[$ind['Volume of waste water treated m3/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming loads BOD-tot  t/y'],$context['sandbox']['line'], $uwwtp[$ind['Incoming loads BOD-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged loads BOD-tot  t/y'],$context['sandbox']['line'], $uwwtp[$ind['Discharged loads BOD-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['BOD rate of removal %'],$context['sandbox']['line'], $uwwtp[$ind['BOD rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration BOD mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Incoming concentration BOD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration BOD mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Discharge concentration BOD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming cocentration COD-tot   t/y'],$context['sandbox']['line'], $uwwtp[$ind['Incoming cocentration COD-tot   t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged concentration COD-tot  t/y'],$context['sandbox']['line'], $uwwtp[$ind['Discharged concentration COD-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['COD rate of removal %'],$context['sandbox']['line'], $uwwtp[$ind['COD rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration COD mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Incoming concentration COD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration COD mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Discharge concentration COD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming loads N-tot   t/y'],$context['sandbox']['line'], $uwwtp[$ind['Incoming loads N-tot   t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged loads N-tot  t/y'],$context['sandbox']['line'], $uwwtp[$ind['Discharged loads N-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['N rate of removal %'],$context['sandbox']['line'], $uwwtp[$ind['N rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration N mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Incoming concentration N mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration N mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Discharge concentration N mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming cocentration P-tot   t/y'],$context['sandbox']['line'], $uwwtp[$ind['Incoming cocentration P-tot   t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged concentration P-tot  t/y'],$context['sandbox']['line'], $uwwtp[$ind['Discharged concentration P-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['P rate of removal %'],$context['sandbox']['line'], $uwwtp[$ind['P rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration P mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Incoming concentration P mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration P mg/l'],$context['sandbox']['line'], $uwwtp[$ind['average Discharge concentration P mg/l']]);
        
        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'], 1);
//         }
        $j++;        
        $context['sandbox']['file_position'] = ftell($ruwwtpfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];   
    }
    uwwtd_trace('end loop');
    fclose($ruwwtpfile);
    
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_total_name'], $annee);
        $r = fopen($filename, 'rb');
        $line_to_fill = 0;  
        for ($i = 0; $i < 3; $i++) {
            $uwwtp = fgetcsv($r, 5000, ';');
            switch($uwwtp[0]) {
                case 'TOTAL';
                    $line_to_fill = $context['sandbox']['line'] + 2;
                break; 
                case 'expired_deadline';
                     $line_to_fill = $context['sandbox']['line'] + 3;
                break; 
                case 'pending_deadline';
                     $line_to_fill = $context['sandbox']['line'] + 4;
                break;                                 
            }
            $sheet->setCellValueByColumnAndRow($ind['uww_entering this UWWTP % of gen. load'], $line_to_fill, $uwwtp[$ind['uww_entering this UWWTP % of gen. load']]);
            $sheet->setCellValueByColumnAndRow($ind['uww_entering load  p. e.'], $line_to_fill, $uwwtp[$ind['uww_entering load  p. e.']]);
            $sheet->setCellValueByColumnAndRow($ind['uww_capacity p. e.'], $line_to_fill, $uwwtp[$ind['uww_capacity p. e.']]);
            $sheet->setCellValueByColumnAndRow($ind['previous_entering load p. e.'], $line_to_fill, $uwwtp[$ind['previous_entering load p. e.']]);
            $sheet->setCellValueByColumnAndRow($ind['previous_capacity p. e.'], $line_to_fill, $uwwtp[$ind['previous_capacity p. e.']]);
            $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of entering load (1) %'], $line_to_fill, $uwwtp[$ind['previous_Percentage of change of entering load (1) %']]);
            $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of capacity (2) %'], $line_to_fill, $uwwtp[$ind['previous_Percentage of change of capacity (2) %']]);
        }
        fclose($r);    
    }
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
//     $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }              
}

function uwwtd_create_register_2015_file_rca_sheet1($annee, &$context)
{    
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['rca']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rca']['xlsx_name'], $annee);    

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name']);
    
    $start_line = 6;
//     $sheet->insertNewRowBefore(7, count($listrca));   todo    $listrca exist pas
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet1_rca'];
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['rca']['csv_name']. '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rca']['csv_name'], $annee);
               
    $rrcafile = fopen($filename, 'rb');
    
    //ignore header //decomment for debug on xls final file
    $rca = fgetcsv($rrcafile, 5000, ';');
    
    while ($rca = fgetcsv($rrcafile, 5000, ';')) {  
        
//     foreach($listrca as $rca) {
        $sheet->setCellValueByColumnAndRow($ind['id_of_sensitive_area'], $i, $rca[$ind['id_of_sensitive_area']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_sensitive_area'], $i, $rca[$ind['name_of_sensitive_area']]);
        $sheet->setCellValueByColumnAndRow($ind['zone_type_code'], $i, $rca[$ind['zone_type_code']]);
        $sheet->setCellValueByColumnAndRow($ind['specialised_zone_type_code'], $i, $rca[$ind['specialised_zone_type_code']]);
        $sheet->setCellValueByColumnAndRow($ind['parameters_subject_to_more_stringent_treatment:_n'], $i, $rca[$ind['parameters_subject_to_more_stringent_treatment:_n']]);
        $sheet->setCellValueByColumnAndRow($ind['parameters_subject_to_more_stringent_treatment:_p'], $i, $rca[$ind['parameters_subject_to_more_stringent_treatment:_p']]);
        $sheet->setCellValueByColumnAndRow($ind['parameters_subject_to_more_stringent_treatment:_m'], $i, $rca[$ind['parameters_subject_to_more_stringent_treatment:_m']]);
        $sheet->setCellValueByColumnAndRow($ind['parameters_subject_to_more_stringent_treatment:_other'], $i, $rca[$ind['parameters_subject_to_more_stringent_treatment:_other']]);
        $sheet->setCellValueByColumnAndRow($ind['date_of_designation_of_art._5(8)'], $i, $rca[$ind['date_of_designation_of_art._5(8)']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_of_art._5(8)'], $i, $rca[$ind['starting_date_of_application_of_art._5(8)']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_of_art._5(4)'], $i, $rca[$ind['starting_date_of_application_of_art._5(4)']]);
        $sheet->setCellValueByColumnAndRow($ind['designation_criteria_a_n'], $i, $rca[$ind['designation_criteria_a_n']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_designation_a_n'], $i, $rca[$ind['starting_date_of_designation_a_n']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_a_n'], $i, $rca[$ind['starting_date_of_application_a_n']]);
        $sheet->setCellValueByColumnAndRow($ind['designation_criteria_a_p'], $i, $rca[$ind['designation_criteria_a_p']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_designation_a_p'], $i, $rca[$ind['starting_date_of_designation_a_p']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_a_p'], $i, $rca[$ind['starting_date_of_application_a_p']]);
        $sheet->setCellValueByColumnAndRow($ind['designation_criteria_b'], $i, $rca[$ind['designation_criteria_b']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_designation_b'], $i, $rca[$ind['starting_date_of_designation_b']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_b'], $i, $rca[$ind['starting_date_of_application_b']]);
        $sheet->setCellValueByColumnAndRow($ind['designation_criteria_c'], $i, $rca[$ind['designation_criteria_c']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_designation_c'], $i, $rca[$ind['starting_date_of_designation_c']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_c'], $i, $rca[$ind['starting_date_of_application_c']]);
        $sheet->setCellValueByColumnAndRow($ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'], $i, $rca[$ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters']]);
        $sheet->setCellValueByColumnAndRow($ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'], $i, $rca[$ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive']]);
        $sheet->setCellValueByColumnAndRow($ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'], $i, $rca[$ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive']]);
        $sheet->setCellValueByColumnAndRow($ind['number_of_uwwtps_of_the_article_5.4_area_(1)'], $i, $rca[$ind['number_of_uwwtps_of_the_article_5.4_area_(1)']]);
        $sheet->setCellValueByColumnAndRow($ind['total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)'], $i, $rca[$ind['total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)']]);
        $sheet->setCellValueByColumnAndRow($ind['incoming_loads_(t/y)_n-tot_(1)'], $i, $rca[$ind['incoming_loads_(t/y)_n-tot_(1)']]);
        $sheet->setCellValueByColumnAndRow($ind['rate_of_removal_n-tot'], $i, $rca[$ind['rate_of_removal_n-tot']]);
        $sheet->setCellValueByColumnAndRow($ind['discharged_loads_(t/y)_n-tot_(1)'], $i, $rca[$ind['discharged_loads_(t/y)_n-tot_(1)']]);
        $sheet->setCellValueByColumnAndRow($ind['incoming_loads_(t/y)_p-tot_(1)'], $i, $rca[$ind['incoming_loads_(t/y)_p-tot_(1)']]);
        $sheet->setCellValueByColumnAndRow($ind['rate_of_removal_p-tot'], $i, $rca[$ind['rate_of_removal_p-tot']]);
        $sheet->setCellValueByColumnAndRow($ind['discharged_loads_(t/y)_p-tot:_'], $i, $rca[$ind['discharged_loads_(t/y)_p-tot:_']]);
        $sheet->setCellValueByColumnAndRow($ind['general_remark'], $i, $rca[$ind['general_remark']]);
        $sheet->setCellValueByColumnAndRow($ind['rcabeginlife'], $i, $rca[$ind['rcabeginlife']]);
        $sheet->setCellValueByColumnAndRow($ind['rcaendlife'], $i, $rca[$ind['rcaendlife']]);
        $sheet->setCellValueByColumnAndRow($ind['id_code_of_successor_(2)'], $i, $rca[$ind['id_code_of_successor_(2)']]);
        $sheet->setCellValueByColumnAndRow($ind['id_code_of_predecessor_(2)'], $i, $rca[$ind['id_code_of_predecessor_(2)']]);
        $sheet->setCellValueByColumnAndRow($ind['remark_successor/predecessor_sa'], $i, $rca[$ind['remark_successor/predecessor_sa']]);
        $sheet->setCellValueByColumnAndRow($ind['related_sensitive_area_(2)'], $i, $rca[$ind['related_sensitive_area_(2)']]);
        $sheet->setCellValueByColumnAndRow($ind['remark_related_sensitive_area_(2)'], $i, $rca[$ind['remark_related_sensitive_area_(2)']]);
        $i++;
        $sheet->insertNewRowBefore($i, 1);
    }
    fclose($rrcafile);
   
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));   
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name']));    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_summary_big_cities_sheet8($annee) 
{    
	if (!isset($context['sandbox']['progress'])) {
    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['progressbc'] = 0;
        $context['sandbox']['progressagg'] = 0;
        $context['sandbox']['idbc']  = array();
        $context['sandbox']['idagg'] = array();
            
        $query = 'select n.nid
        from {node} n    
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id                                                                                                                                                                                                                                                                                                                                                                                                                                
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
        ';
        
        $param = array(
            ':typenode' => 'big_city',
            ':annee' => $annee,
            );

        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['idbc'][] = $row['nid'];
            }
            unset($result, $row);
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
         
       //get all agglo > 150 000
        $query = 'select n.nid, a.field_agggenerated_value
        from {node} n    
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id   
            left join
                {field_data_field_agggenerated} a
                    ON n.nid = a.entity_id                                                                                                                                                                                                                                                                                                                                                                                                                                                 
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
            and a.field_agggenerated_value::integer > :agggenerated
        ';
        
        $param = array(
            ':typenode' => 'agglomeration',
            ':annee' => $annee,
            ':agggenerated' => 150000,
            );
     
        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                if (false === in_array($row['nid'], $agglotreated)) {
                    $context['sandbox']['idagg'][] = $row['nid'];
                }
            }
            unset($result, $row);
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
        
        $context['sandbox']['maxbc'] = count($context['sandbox']['idbc']);
        $context['sandbox']['maxagg'] = count($context['sandbox']['idagg']);   	
		$context['sandbox']['max'] = $context['sandbox']['maxbc'] + $context['sandbox']['maxagg'];
//         dsm($context['sandbox']);
    }
    
    $ind = $GLOBALS['uwwtd']['sheet8_summary_big_cities'];

//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name'] .'.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name'], $annee);      

    if ($context['sandbox']['progress'] == 0) {
        $r = fopen($filename, 'wb');  
    
        $header = array(
            $ind['id'] => 'ID', 
            $ind['name'] => 'name', 
            $ind['generated_load'] => 'generated load p.e.', 
            $ind['collecting_system'] => 'collecting system % of generated load', 
            $ind['ias'] => 'IAS % of generated load', 
            $ind['no_collecting_system_nor_ias'] => 'no collecting system nor IAS % of generated load', 
            $ind['treatment_0'] => 'best treatment provided 0', 
            $ind['treatment_1'] => 'best treatment provided 1', 
            $ind['treatment_2'] => 'best treatment provided 2', 
            $ind['treatment_3n_p_np'] => 'best treatment provided 3N', 
//             $ind['treatment_3p'] => 'best treatment provided 3P', 
//             $ind['treatment_3np'] => 'best treatment provided 3NP', 
            $ind['treatment_3_other_(e.g._uv,_filtration)'] => 'best treatment provided  3 other (e.g. UV, filtration)', 
            $ind['type_of_receiving_area'] => 'Type of receiving area',  
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
//         dsm($header);
    } else {
        $r = fopen($filename, 'ab');  
    }
    
    $j = 0;
    while ($j < 20 && $context['sandbox']['progressbc'] < $context['sandbox']['maxbc']) {              
//     foreach($id as $nid_bc) {
//         dsm($j + $context['sandbox']['progressbc']);
//         dsm($context['sandbox']['idbc'][$context['sandbox']['progressbc']]);
        $bc = node_load($context['sandbox']['idbc'][$context['sandbox']['progressbc']]);
//         dsm($bc);
        $line = array();
        
        $generated_load = 0;
        $collecting_system = 0;
        $ias = 0;
        $no_collecting_system_nor_ias = 0;
        $treatment_0 = 0;
        $treatment_1 = 0;
        $treatment_2 = 0;
        $treatment_3np_3n_3p = 0;
        $type_of_receiving_area = '';
                
        $line[$ind['id']] = $bc->field_inspireidlocalid['und'][0]['value'];
//         dsm($bc);
//         dsm($line);
        $line[$ind['name']] = $bc->title;
        $dcptype = array();
        $agglotreated = array();
//         dsm($bc->field_bcaggliste['und']);
        foreach($bc->field_bcaggliste['und'] as $nidagglo) {
            $agglotreated[] = $nidagglo['nid'];
            $agg = node_load($nidagglo['nid']); 
            $agg_detail = uwwtd_summary_big_cities_get_agglo_detail($annee, $agg);
//             dsm($agg_detail);
            unset($agg);
            $generated_load += $agg_detail['generated_load'];
            $collecting_system +=  $agg_detail['collecting_system'];
            $ias +=  $agg_detail['ias'];
            $no_collecting_system_nor_ias +=  $agg_detail['no_collecting_system_nor_ias'];
            
            $dcptype = array_merge($dcptype, $agg_detail['dcptype']);
//             dsm($dcptype);  
            $treat_3np_3n_3p_pe +=   $agg_detail['treat_3np_3n_3p_pe']; 
            $treat_3np_3n_3p_agggenerated += $agg_detail['treat_3np_3n_3p_agggenerated'];   
            $treat_2_pe +=           $agg_detail['treat_2_pe'];
            $treat_2_agggenerated += $agg_detail['treat_2_agggenerated'];   
            $treat_1_pe +=           $agg_detail['treat_1_pe'];
            $treat_1_agggenerated += $agg_detail['treat_1_agggenerated'];   
            $treat_0_pe +=           $agg_detail['treat_0_pe'];
            $treat_0_agggenerated += $agg_detail['treat_0_agggenerated'];   
        }
        
      
//         dsm("treat_3np_3n_3p_pe = $treat_3np_3n_3p_pe");
//         dsm("treat_3np_3n_3p_agggenerated = $treat_3np_3n_3p_agggenerated");
//         dsm("treat_2_pe = $treat_2_pe");
//         dsm("treat_2_agggenerated = $treat_2_agggenerated");
//         dsm("treat_1_pe = $treat_1_pe");
//         dsm("treat_1_agggenerated = $treat_1_agggenerated");
//         dsm("treat_0_pe = $treat_0_pe");
//         dsm("treat_0_agggenerated = $treat_0_agggenerated");
                        
        $treatment_3np_3n_3p = $treat_3np_3n_3p_pe * 100 / $treat_3np_3n_3p_agggenerated;
        $treatment_2 = $treat_2_pe * 100 / $treat_2_agggenerated;
        $treatment_1 = $treat_1_pe * 100 / $treat_1_agggenerated;
        $treatment_0 = $treat_0_pe * 100 / $treat_0_agggenerated;
        
//         dsm("treatment_0 = $treatment_0");
//         dsm("treatment_1 = $treatment_1");
//         dsm("treatment_2 = $treatment_2");
//         dsm("treatment_3np_3n_3p = $treatment_3np_3n_3p");             
        
        $type_of_receiving_area = implode(';', array_keys($dcptype));
//         dsm($type_of_receiving_area);
            
        $collecting_system_percent =  $collecting_system * 100 / $generated_load;
        $ias_percent =  $ias * 100 / $generated_load;
        $no_collecting_system_nor_ias_percent =  $no_collecting_system_nor_ias * 100 / $generated_load;
        
        $line[$ind['generated_load']] = $generated_load;
        $line[$ind['collecting_system']] = $collecting_system_percent;
        $line[$ind['ias']] = $ias_percent;
        $line[$ind['no_collecting_system_nor_ias']] = $no_collecting_system_nor_ias_percent;
        
        //if max treatement in place = 0
        $line[$ind['treatment_0']] = $treatment_0;
        //if max treatement in place = 1
        $line[$ind['treatment_1']] = $treatment_1;
        //if max treatement in place = 2
        $line[$ind['treatment_2']] = $treatment_2;
        //if max treatement in place = 3n,3npn3p,3other
        $line[$ind['treatment_3n_p_np']] = $treatment_3np_3n_3p; //tood
//         $line[$ind['treatment_3n']] = $treatment_3n;
//         $line[$ind['treatment_3p']] = $treatment_3p;
//         $line[$ind['treatment_3np']] = $treatment_3np;
        $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = $treatment_3np_3n_3p;
        $line[$ind['type_of_receiving_area']] = $type_of_receiving_area;
//         dsm($line);
        fputcsv($r, $line, ';');
//         dsm($line);
        $context['sandbox']['progressbc']++;
        $context['sandbox']['progress']++;
        $j++;
    }

    if ($context['sandbox']['progressbc'] >= $context['sandbox']['maxbc']) {
        $j = 0;
        while ($j < 20 && $context['sandbox']['progressagg'] < $context['sandbox']['maxagg']) {      
    //     foreach($id as $nidagglo) {
            $line = array();
            $generated_load = 0;
            $collecting_system = 0;
            $ias = 0;
            $no_collecting_system_nor_ias = 0;
            $treatment_0 = '0';
            $treatment_1 = '0';
            $treatment_2 = '0';
            $treatment_3np_3n_3p = '0';
            $type_of_receiving_area = '';
                    
            $agg = node_load($nidagglo['nid']);                                          
            $agg_detail = uwwtd_summary_big_cities_get_agglo_detail($annee, $agg);
            
            $treatment_0 = $agg_detail['treat_0'];
            $treatment_1 = $agg_detail['treat_1'];
            $treatment_2 = $agg_detail['treat_2'];
            $treatment_3np_3n_3p = $agg_detail['treat_3np_3n_3p'];
            
            $generated_load = $agg_detail['generated_load'];
            $collecting_system =  $agg_detail['collecting_system'];
            $ias =  $agg_detail['ias'];
            $no_collecting_system_nor_ias =  $agg_detail['no_collecting_system_nor_ias'];
            
            $collecting_system_percent =  $collecting_system * 100 / $generated_load;
            $ias_percent =  $ias * 100 / $generated_load;
            $no_collecting_system_nor_ias_percent =  $no_collecting_system_nor_ias * 100 / $generated_load;            
                         
            $line[$ind['id']] = $agg->field_inspireidlocalid['und'][0]['value'];
            $line[$ind['name']] = $agg->title;         
            $line[$ind['generated_load']] = $generated_load;
            $line[$ind['collecting_system']] = $collecting_system_percent;
            $line[$ind['ias']] = $ias_percent;
            $line[$ind['no_collecting_system_nor_ias']] = $no_collecting_system_nor_ias_percent;
            $line[$ind['treatment_0']] = $treatment_0;
            $line[$ind['treatment_1']] = $treatment_1;
            $line[$ind['treatment_2']] = $treatment_2;
            
            $line[$ind['treatment_3n_p_np']] = $treatment_3np_3n_3p;  //todo
            $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = $treatment_3np_3n_3p;
            $line[$ind['type_of_receiving_area']] = implode(';', $dcptype);
            unset($agg);
            fputcsv($r, $line, ';');
            
            $context['sandbox']['progressagg']++;
            $context['sandbox']['progress']++;
            $j++;            
        }
    }  
    
    fclose($r);
                                            
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }    
}

/**
 *
 */
function uwwtd_summary_big_cities_get_agglo_detail($annee, $agg)
{          
//     $result['generated_load'] = 0;
//     $result['collecting_system'] = 0;
//     $result['ias'] = 0;
//     $result['no_collecting_system_nor_ias'] = 0;
    $result['dcptype'] = ''; 
//     $agg = node_load($nidagglo);
//     dsm($agg);
    $result['generated_load'] = $agg->field_agggenerated['und'][0]['value'];
    $result['collecting_system'] = $agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
    $result['ias'] = $agg->field_aggc2['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
    $result['no_collecting_system_nor_ias'] = $agg->field_aggpercwithouttreatment['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
    
    $result['treat_0_pe'] = 0;
    $result['treat_1_pe'] = 0;
    $result['treat_2_pe'] = 0;
    $result['treat_3np_3n_3p_pe'] = 0;
    
    $result['treat_0_agggenerated'] = 0;
    $result['treat_1_agggenerated'] = 0;
    $result['treat_2_agggenerated'] = 0;
    $result['treat_3np_3n_3p_agggenerated'] = 0;    
 
        
   //for rca we need to load uwwtp -> dcp -> rca           
   foreach($agg->field_agguwwliste['und'] as $uwwtpnid) {
        $uwwtp = node_load($uwwtpnid['nid']);
        $uwwtp_treat_inplace = uwwtd_get_register_treatmenttype_inplace($uwwtp);
        
        $percent_generated_load = 0;
        //load agglo_uuwtp node
        $agglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $annee, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
        
        if ('' !== $agglo_uwwtpnid) {
            $agglo_uwwtp = node_load($agglo_uwwtpnid);
            $entering_load_pe = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * ($agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']);
//             $percent_of_generating_load = $entering_load_pe * 100 / $agg->field_agggenerated['und'][0]['value']; 
            
            if (false !== strpos($uwwtp_treat_inplace, '3other') || false !== strpos($uwwtp_treat_inplace, '3NP')
                || false !== strpos($uwwtp_treat_inplace, '3P') || false !== strpos($uwwtp_treat_inplace, '3N')) {
                $result['treat_3np_3n_3p_pe'] += $entering_load_pe;
                $result['treat_3np_3n_3p_agggenerated'] += $agg->field_agggenerated['und'][0]['value'];        
            } elseif (false !== strpos($uwwtp_treat_inplace, '2')) {
               $result['treat_2_pe'] += $entering_load_pe;
               $result['treat_2_agggenerated']    += $agg->field_agggenerated['und'][0]['value'];
            } elseif (false !== strpos($uwwtp_treat_inplace, '1')) {
               $result['treat_1_pe'] += $entering_load_pe;
               $result['treat_1_agggenerated']    += $agg->field_agggenerated['und'][0]['value'];
            } else {
               $result['treat_0_pe'] += $entering_load_pe;  
               $result['treat_0_agggenerated']    += $agg->field_agggenerated['und'][0]['value'];
            }
            
            unset($agglo_uwwtp);
        }
        
        foreach($uwwtp->field_uwwdcpliste['und'] as $dcpnid) {
            $dcp = node_load($dcpnid['nid']);
            $result['dcptype'][$dcp->field_rcatype['und'][0]['value']] = 0;
            unset($dcp);
        }
        unset($uwwtp);
    }            
    unset($agg);
    
    $result['treat_3np_3n_3p'] = $result['treat_3np_3n_3p_pe'] * 100 / $result['treat_3np_3n_3p_agggenerated'];
    $result['treat_3other']    = $result['treat_3np_3n_3p'];
    $result['treat_2'] = $result['treat_2_pe'] * 100 / $result['treat_2_agggenerated'];
    $result['treat_1'] = $result['treat_1_pe'] * 100 / $result['treat_1_agggenerated'];
    $result['treat_0'] = $result['treat_0_pe'] * 100 / $result['treat_0_agggenerated'];    

    return $result;
} 

/**
 *
 */ 
function uwwtd_create_register_2015_nuts_level2_analyse_sheet9($annee, &$context)
{
    //get all agglo
    //group by nuts2 code
    $indagglo = $GLOBALS['uwwtd']['sheet3_agglo'];
    $filenameagglo = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);    
    $ragglofile = fopen($filenameagglo, 'rb');
    
    $ind = $GLOBALS['uwwtd']['sheet9_nuts2_analyse'];        
    //pass header line
    if (!isset($context['sandbox']['progress'])) { 
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filenameagglo);
            
        //ignore header line
        $agglo = fgetcsv($ragglofile, 5000, ';');
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['nuts2'] = array();
    }   
    
    fseek($ragglofile, $context['sandbox']['file_position']);
    
    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {    
        //build context value       

        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];

        if (isset($agglo[$indagglo['nuts_2_code']]) && !empty($agglo[$indagglo['nuts_2_code']])) {
            $nuts2 = $agglo[$indagglo['nuts_2_code']];            
        } else {
            continue;
        }
        
        if (!isset($context['sandbox']['nuts2'][$nuts2])) {
            $context['sandbox']['nuts2'][$nuts2] = array();
            $context['sandbox']['nuts2'][$nuts2][$ind['nuts2_code']] = $nuts2;
            $context['sandbox']['nuts2'][$nuts2][$ind['number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['generated_load_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['collected_pe']] = 0; 
            $context['sandbox']['nuts2'][$nuts2][$ind['collected_percent']] = 0;
            
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_target_load']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_compliant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_compliant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notcompliant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notcompliant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notrelevant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notrelevant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_rate']] = 0;
            
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_target_load']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_compliant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_compliant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notcompliant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notcompliant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notrelevant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notrelevant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_rate']] = 0;
            
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_target_load']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_compliant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_compliant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notcompliant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notcompliant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notrelevant_number_agglo']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notrelevant_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_rate']] = 0;            
        }
        
        $context['sandbox']['nuts2'][$nuts2][$ind['number_agglo']]++;
        $context['sandbox']['nuts2'][$nuts2][$ind['generated_load_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        $context['sandbox']['nuts2'][$nuts2][$ind['collected_pe']]      += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        $context['sandbox']['nuts2'][$nuts2][$ind['collected_percent']] += $agglo[$indagglo['connected_to_collecting_system']];
        
        if ($agglo[$indagglo['art3_compliance']] != 'NR') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_target_load']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['art3_compliance']] == 'C') {         
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_compliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_compliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['art3_compliance']] == 'NC') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notcompliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notcompliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['art3_compliance']] == 'NR') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notrelevant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notrelevant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
//         $context['sandbox']['nuts2'][$nuts2][$ind['art3_rate']]= 'TODO';//+=
        
        //art4
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] != 'NR') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_target_load']] += $agglo[$indagglo['generated_load_p.e.']];
        }        
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {         
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_compliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_compliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] == 'NC') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notcompliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notcompliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] == 'NR') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notrelevant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notrelevant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
//         $context['sandbox']['nuts2'][$nuts2][$ind['art4_rate']]= 'TODO';//+=
        
        //art5
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] != 'NR') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_target_load']] += $agglo[$indagglo['generated_load_p.e.']];
        }           
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {         
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_compliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_compliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notcompliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notcompliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NR') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notrelevant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notrelevant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
//         $context['sandbox']['nuts2'][$nuts2][$ind['art5_rate']] = 'TODO';//+=                
                                                                      
  
    }
    fclose($ragglofile);     
    
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        //write final file
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['nuts_level2']['csv_name'], $annee);
        $r = fopen($filename, 'wb');
        $header = array(
            $ind['nuts2_code'] => 'NUTS 2 code',
            $ind['number_agglo'] => 'number of agglo-merations',
            $ind['generated_load_pe'] => 'total generated load pe',
            $ind['collected_pe'] => 'load collected in collecting systems pe', 
            $ind['collected_percent'] => 'target load',
            
            $ind['art3_target_load'] => 'art3 target load',
            $ind['art3_compliant_number_agglo'] => 'art3 compliance number of agglos',
            $ind['art3_compliant_pe'] => 'art3 compliance p.e.',
            $ind['art3_notcompliant_number_agglo'] => 'art3 not compliant number of agglos',
            $ind['art3_notcompliant_pe'] => 'art3 not compliant p.e.',
            $ind['art3_notrelevant_number_agglo'] => 'art3 not relevant number of agglos',
            $ind['art3_notrelevant_pe'] => 'art3 not relevant p.e.',    
            $ind['art3_rate'] => 'Compliance rate Art. 3',
            
            $ind['art4_target_load'] => 'art4 target load',
            $ind['art4_compliant_number_agglo'] => 'art4 compliance number of agglos',
            $ind['art4_compliant_pe'] => 'art4 compliance p.e.',
            $ind['art4_notcompliant_number_agglo'] => 'art4 not compliant number of agglos',
            $ind['art4_notcompliant_pe'] => 'art4 not compliant p.e.',
            $ind['art4_notrelevant_number_agglo'] => 'art4 not relevant number of agglos',
            $ind['art4_notrelevant_pe'] => 'art4 not relevant p.e.',    
            $ind['art4_rate'] => 'Compliance rate Art. 4',  
            
            $ind['art5_target_load'] => 'art5 target load',
            $ind['art5_compliant_number_agglo'] => 'art5 compliance number of agglos',
            $ind['art5_compliant_pe'] => 'art5 compliance p.e.',
            $ind['art5_notcompliant_number_agglo'] => 'art5 not compliant number of agglos',
            $ind['art5_notcompliant_pe'] => 'art5 not compliant p.e.',
            $ind['art5_notrelevant_number_agglo'] => 'art5 not relevant number of agglos',
            $ind['art5_notrelevant_pe'] => 'art5 not relevant p.e.',    
            $ind['art5_rate'] => 'Compliance rate Art. 5',     
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');   
        
        //foreach nuts write line
        foreach ($context['sandbox']['nuts2'] as $nuts2) {
        
            $nuts2[$ind['art3_rate']] = $nuts2[$ind['art3_compliant_pe']] * 100 / $nuts2[$ind['art3_target_load']];
            $nuts2[$ind['art4_rate']] = $nuts2[$ind['art4_compliant_pe']] * 100 / $nuts2[$ind['art4_target_load']];
            $nuts2[$ind['art5_rate']] = $nuts2[$ind['art5_compliant_pe']] * 100 / $nuts2[$ind['art5_target_load']];
            
            $line = array(
                $nuts2[$ind['nuts2_code']],
                $nuts2[$ind['number_agglo']],
                $nuts2[$ind['generated_load_pe']],
                $nuts2[$ind['collected_pe']], 
                $nuts2[$ind['collected_percent']],
                
                $nuts2[$ind['art3_target_load']],
                $nuts2[$ind['art3_compliant_number_agglo']],
                $nuts2[$ind['art3_compliant_pe']],
                $nuts2[$ind['art3_notcompliant_number_agglo']],
                $nuts2[$ind['art3_notcompliant_pe']],
                $nuts2[$ind['art3_notrelevant_number_agglo']],
                $nuts2[$ind['art3_notrelevant_pe']],
                $nuts2[$ind['art3_rate']],
                
                $nuts2[$ind['art4_target_load']],
                $nuts2[$ind['art4_compliant_number_agglo']],
                $nuts2[$ind['art4_compliant_pe']],
                $nuts2[$ind['art4_notcompliant_number_agglo']],
                $nuts2[$ind['art4_notcompliant_pe']],
                $nuts2[$ind['art4_notrelevant_number_agglo']],
                $nuts2[$ind['art4_notrelevant_pe']],
                $nuts2[$ind['art4_rate']],
                
                $nuts2[$ind['art5_target_load']],
                $nuts2[$ind['art5_compliant_number_agglo']],
                $nuts2[$ind['art5_compliant_pe']],
                $nuts2[$ind['art5_notcompliant_number_agglo']],
                $nuts2[$ind['art5_notcompliant_pe']],
                $nuts2[$ind['art5_notrelevant_number_agglo']],
                $nuts2[$ind['art5_notrelevant_pe']],
                $nuts2[$ind['art5_rate']],             
            );
            fputcsv($r, $line, ';');
        }                                     
               
        fclose($r);
    }
    
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/'. $context['sandbox']['max'];    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_summary_big_cities_sheet8($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name']);
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet8_summary_big_cities'];
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name']. '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name'], $annee);
               
    $rbcfile = fopen($filename, 'rb');
    
    $start_line = 4;
    //ignore header //decomment for debug on xls final file
    $bc = fgetcsv($rbcfile, 5000, ';');
    $i = $start_line;
    while ($bc = fgetcsv($rbcfile, 5000, ';')) {
//         dsm($bc);
//         dsm($ind);
//         dsm($bc[$ind['id']]);
        $sheet->setCellValueByColumnAndRow($ind['id'], $i, $bc[$ind['id']]);
//         $sheet->getCell('B'.$i)->setValue($bc[$ind['id']]);
//         $sheet->getCell('A1')->setValue($bc[$ind['id']]);
//         $sheet->getCell('A4')->setValue($bc[$ind['id']]);
//         $sheet->getCell('B4')->setValue($bc[$ind['id']]);
//         dsm('B'.$i);

        $sheet->setCellValueByColumnAndRow($ind['name'], $i, $bc[$ind['name']]);
//         $sheet->setCellValueByColumnAndRow($ind['name'], $i, $bc[$ind['id']]);
        $sheet->setCellValueByColumnAndRow($ind['generated_load'], $i, $bc[$ind['generated_load']]);
        $sheet->setCellValueByColumnAndRow($ind['collecting_system'], $i, $bc[$ind['collecting_system']]);
        $sheet->setCellValueByColumnAndRow($ind['ias'], $i, $bc[$ind['ias']]);
        $sheet->setCellValueByColumnAndRow($ind['no_collecting_system_nor_ias'], $i, $bc[$ind['no_collecting_system_nor_ias']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment_0'], $i, $bc[$ind['treatment_0']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment_1'], $i, $bc[$ind['treatment_1']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment_2'], $i, $bc[$ind['treatment_2']]);
//         
        $sheet->setCellValueByColumnAndRow($ind['treatment_3n_p_np'], $i, $bc[$ind['treatment_3n_p_np']]);
// //         $sheet->setCellValueByColumnAndRow($ind['treatment_3n'], $i, $bc[$ind['treatment_3n']]);
// //         $sheet->setCellValueByColumnAndRow($ind['treatment_3p'], $i, $bc[$ind['treatment_3p']]);
// //         $sheet->setCellValueByColumnAndRow($ind['treatment_3np'], $i, $bc[$ind['treatment_3np']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment_3_other_(e.g._uv,_filtration)'], $i, $bc[$ind['treatment_3_other_(e.g._uv,_filtration)']]);
        $sheet->setCellValueByColumnAndRow($ind['type_of_receiving_area'], $i, $bc[$ind['type_of_receiving_area']]);
        
        $i++;
        $sheet->insertNewRowBefore($i, 1);        
    }
    fclose($rbcfile);    
        
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name']));    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}


/**
 *
 */ 
function uwwtd_create_register_2015_file_nuts_level2_analyse_sheet9($annee, &$context)
{
    $ind = $GLOBALS['uwwtd']['sheet9_nuts2_analyse'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['nuts_level2']['csv_name'], $annee);
    
    $rnuts = fopen($filename, 'rb');
    
    //pass header line
    if (!isset($context['sandbox']['progress'])) { 
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        //ignore header    
        $agglo = fgetcsv($rnuts, 5000, ';');
        $context['sandbox']['file_position'] = ftell($rnuts);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['line'] = 4; //start line
        
        $path_xlsx = uwwtd_getregister_modelfile_path();
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_name'], $annee);        
    } else {        
//         $path_xlsx = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'] .'.xlsx';
        $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_name'], $annee);
        $target_path = $path_xlsx;          
    }     
    
    fseek($rnuts, $context['sandbox']['file_position']);
     
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($path_xlsx));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_model_sheet_name']);
    
    $j = 0;
    while ($j < 20 && $nuts = fgetcsv($rnuts, 5000, ';')) { 
        $sheet->setCellValueByColumnAndRow($ind['nuts2_code'], $context['sandbox']['line'], $nuts[$ind['nuts2_code'] ]);
        $sheet->setCellValueByColumnAndRow($ind['number_agglo'], $context['sandbox']['line'], $nuts[$ind['number_agglo'] ]);
        $sheet->setCellValueByColumnAndRow($ind['generated_load_pe'], $context['sandbox']['line'], $nuts[$ind['generated_load_pe'] ]);
                       
        $sheet->setCellValueByColumnAndRow($ind['collected_pe'] , $context['sandbox']['line'], $nuts[$ind['collected_pe'] ]);
        $sheet->setCellValueByColumnAndRow($ind['collected_percent'] , $context['sandbox']['line'], $nuts[$ind['collected_percent']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_target_load'] , $context['sandbox']['line'], $nuts[$ind['art3_target_load']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_compliant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art3_compliant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_compliant_pe'] , $context['sandbox']['line'], $nuts[$ind['art3_compliant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_notcompliant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art3_notcompliant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_notcompliant_pe'] , $context['sandbox']['line'], $nuts[$ind['art3_notcompliant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_notrelevant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art3_notrelevant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_notrelevant_pe'] , $context['sandbox']['line'], $nuts[$ind['art3_notrelevant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_rate'] , $context['sandbox']['line'], $nuts[$ind['art3_rate']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_target_load'] , $context['sandbox']['line'], $nuts[$ind['art4_target_load']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art4_compliant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliant_pe'] , $context['sandbox']['line'], $nuts[$ind['art4_compliant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_notcompliant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art4_notcompliant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_notcompliant_pe'] , $context['sandbox']['line'], $nuts[$ind['art4_notcompliant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_notrelevant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art4_notrelevant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_notrelevant_pe'] , $context['sandbox']['line'], $nuts[$ind['art4_notrelevant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_rate'] , $context['sandbox']['line'], $nuts[$ind['art4_rate']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_target_load'] , $context['sandbox']['line'], $nuts[$ind['art5_target_load']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_compliant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art5_compliant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_compliant_pe'] , $context['sandbox']['line'], $nuts[$ind['art5_compliant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_notcompliant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art5_notcompliant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_notcompliant_pe'] , $context['sandbox']['line'], $nuts[$ind['art5_notcompliant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_notrelevant_number_agglo'] , $context['sandbox']['line'], $nuts[$ind['art5_notrelevant_number_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_notrelevant_pe'] , $context['sandbox']['line'], $nuts[$ind['art5_notrelevant_pe']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_rate'] , $context['sandbox']['line'], $nuts[$ind['art5_rate'] ]);
      
        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'], 1);
        
        $j++;        
        $context['sandbox']['file_position'] = ftell($rnuts);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];           
    }
    fclose($rnuts);   

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
//     $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }    
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_sewage_sludge_and_re_use_sheet10($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_model_sheet_name']);
    
    $nidms = uwwtd_check_exist(uwwtd_get_siteid('ms_level', array('year' => $annee)));
    
    if ('' != $nidms) {
        $node = node_load($nidms);
        
        //Yearly production of sludge
        $field_mslsludgeproduction = isset($node->field_mslsludgeproduction['und'][0]['value']) ? $node->field_mslsludgeproduction['und'][0]['value'] : ''; 
        //Of which discharged into surface waters: Pipelines 
        $field_msldischargepipelines = isset($node->field_msldischargepipelines['und'][0]['value']) ? $node->field_msldischargepipelines['und'][0]['value'] : ''; 
        //Of which discharged into surface waters: Ships
        $field_msldischargeships = isset($node->field_msldischargeships['und'][0]['value']) ? $node->field_msldischargeships['und'][0]['value'] : ''; 
        //Of which discharged into surface waters: Others
        $field_msldischargeothers = isset($node->field_msldischargeothers['und'][0]['value']) ? $node->field_msldischargeothers['und'][0]['value'] : ''; 
        //Of which: re-used: Soil and agriculture
        $field_mslreusesoilagriculture = isset($node->field_mslreusesoilagriculture['und'][0]['value']) ? $node->field_mslreusesoilagriculture['und'][0]['value'] : ''; 
        //Of which: re-used: Others 
        $field_mslreuseothers = isset($node->field_mslreuseothers['und'][0]['value']) ? $node->field_mslreuseothers['und'][0]['value'] : ''; 
        //Of which: disposed: Landfill
        $field_msldisposallandfill = isset($node->field_msldisposallandfill['und'][0]['value']) ? $node->field_msldisposallandfill['und'][0]['value'] : ''; 
        //Of which: disposed: Incineration
        $field_msldisposalincineration = isset($node->field_msldisposalincineration['und'][0]['value']) ? $node->field_msldisposalincineration['und'][0]['value'] : ''; 
        //Of which: disposed: Others 
        $field_msldisposalothers = isset($node->field_msldisposalothers['und'][0]['value']) ? $node->field_msldisposalothers['und'][0]['value'] : ''; 
        //Remarks
        $field_mslremarks = isset($node->field_mslremarks['und'][0]['value']) ? $node->field_mslremarks['und'][0]['value'] : ''; 
        
        //Volume of waste water treated
        $volume_treated = $context['results']['country_volume_waste_water'];
        //Rate of treated waste water re-used
        $reuse = isset($node->field_mslwwreuseperc['und'][0]['value']) ? $node->field_mslwwreuseperc['und'][0]['value'] : ''; 
        //re-use in agriculture
        $reuse_agriculture = '';
        if (isset($node->field_mslwwreuseagri['und'][0]['value'])) {
            $reuse_agriculture = $node->field_mslwwreuseagri['und'][0]['value'] == 1 ? 'yes' : 'no'; 
        }         
        //re-use in industry
        $reuse_industry = '';
        if (isset($node->field_mslwwreuseind['und'][0]['value'])) {
            $reuse_industry = $node->field_mslwwreuseind['und'][0]['value'] == 1 ? 'yes' : 'no'; 
        }
        //re-use others
        $reuse_others = '';
        if (isset($node->field_mslwwreuseother['und'][0]['value'])) {
            $reuse_others = $node->field_mslwwreuseother['und'][0]['value'] == 1 ? 'yes' : 'no'; 
        }         
        //Remarks
        $remarks = isset($node->field_mslwwreuseexplain['und'][0]['value']) ? $node->field_mslwwreuseexplain['und'][0]['value'] : ''; 
        
        $sheet->getCell('C4')->setValue($field_mslsludgeproduction);
        $sheet->getCell('C5')->setValue($field_msldischargepipelines);
        $sheet->getCell('C6')->setValue($field_msldischargeships);
        $sheet->getCell('C7')->setValue($field_msldischargeothers);
        $sheet->getCell('C8')->setValue($field_mslreusesoilagriculture);
        $sheet->getCell('C9')->setValue($field_mslreuseothers);
        $sheet->getCell('C10')->setValue($field_msldisposallandfill);
        $sheet->getCell('C11')->setValue($field_msldisposalincineration);
        $sheet->getCell('C12')->setValue($field_msldisposalothers);
        $sheet->getCell('C13')->setValue($field_mslremarks);
        
        $sheet->getCell('C21')->setValue($volume_treated);
        $sheet->getCell('C22')->setValue($reuse);
        $sheet->getCell('C23')->setValue($reuse_agriculture);
        $sheet->getCell('C24')->setValue($reuse_industry);
        $sheet->getCell('C25')->setValue($reuse_others);
        $sheet->getCell('C26')->setValue($remarks);         
    }
           
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_model_sheet_name']));    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_graph_sheet11($annee, &$context)
{
    //country name
    $path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
    include($path);
    $country_code = variable_get('siif_eru_country_code', '-1');
    $country_name = variable_get('siif_eru_country_name'); 
            
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['graph']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['graph']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setIncludeCharts(TRUE);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name']);
    
    //get previous year
    $previous_year = uwwtd_get_previous_year($annee);
    $previoussheet = null;
    
    if (false !== $previous_year) {
        $previous_model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['graph']['xlsx_name'], $previous_year);
        if (true === file_exists($previous_model_path)) {    
            $previousobjReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
            $previousobjReader->setIncludeCharts(false);
            $previousobjReader->setLoadSheetsOnly( array(
                                                 $GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name'],
                                                ) );
            $previousobjPHPExcel = $previousobjReader->load(drupal_realpath($previous_model_path));    
            $previoussheet = $previousobjPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name']);
        }    
    }
    
    //size of agglo/pe per class
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
//     
//     $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
//                
//     $ragglofile = fopen($filename, 'rb');
//     //ignore header
//     $agglo = fgetcsv($ragglofile, 5000, ';');
//     
//     //array 1
//     $graph_slice_size = array();
//     $graph_slice_size['0_10000']['pe']  = 0;
//     $graph_slice_size['0_10000']['number'] = 0;
//     $graph_slice_size['10000_100000']['pe']  = 0;
//     $graph_slice_size['10000_100000']['number'] = 0;
//     $graph_slice_size['100000_n']['pe']  = 0;
//     $graph_slice_size['100000_n']['number'] = 0;
//     
//     //array 2
//     $graph_parameters = array();
//     $graph_parameters['total_generated']['pe'] = 0; 
//     $graph_parameters['total_collected']['pe'] = 0;  
//     $graph_parameters['total_load_ias']['pe'] = 0;   
//     $graph_parameters['total_entering']['pe'] = 0; 
//     $graph_parameters['total_capacity']['pe'] = 0;
//    
//     
//     //array 5 6 7
//     $summary_legal['art3_C']['number'] = 0;
//     $summary_legal['art3_C']['pe'] = 0;
//     $summary_legal['art3_NC']['number'] = 0;
//     $summary_legal['art3_NC']['pe'] = 0;
//     $summary_legal['art4_C']['number'] = 0;
//     $summary_legal['art4_C']['pe'] = 0;
//     $summary_legal['art4_NC']['number'] = 0;
//     $summary_legal['art4_NC']['pe'] = 0;
//     $summary_legal['art5_C']['number'] = 0;
//     $summary_legal['art5_C']['pe'] = 0;
//     $summary_legal['art5_NC']['number'] = 0;
//     $summary_legal['art5_NC']['pe'] = 0;
//     
//     todo get info on agglo        
//     while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
//         //array1
//         if ($agglo[$ind['generated_load_p.e.']] < 10001) {
//             $graph_slice_size['0_10000']['pe']  += $agglo[$ind['generated_load_p.e.']];
//             $graph_slice_size['0_10000']['number']++;        
//         } elseif ($agglo[$ind['generated_load_p.e.']]  < 100000) {
//             $graph_slice_size['10000_100000']['pe']  += $agglo[$ind['generated_load_p.e.']];
//             $graph_slice_size['10000_100000']['number']++;             
//         } else {   //>=100000
//             $graph_slice_size['100000_n']['pe']  += $agglo[$ind['generated_load_p.e.']];
//             $graph_slice_size['100000_n']['number']++;             
//         }
//         
//         //array2
//         $graph_parameters['total_generated']['pe'] += $agglo[$ind['generated_load_p.e.']];
//         $graph_parameters['total_collected']['pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
//         $graph_parameters['total_load_ias']['pe'] += $agglo[$ind['addressed_through_ias_p._e.']];
//         $graph_parameters['total_entering']['pe'] += $agglo[$ind['total_entering_load_p._e.']];
//         $graph_parameters['total_capacity']['pe'] += $agglo[$ind['total_treatment_plant_design_capacity_p._e.']];
//         
//         //array 5 6 7
//         if ($agglo[$ind['art3_compliance']] == 'C') {
//             $summary_legal['art3_C']['number']++;
//             $summary_legal['art3_C']['pe'] += $agglo[$ind['generated_load_p.e.']];
//         }
//         if ($agglo[$ind['art3_compliance']] == 'NC') {
//             $summary_legal['art3_NC']['number']++;
//             $summary_legal['art3_NC']['pe'] += $agglo[$ind['generated_load_p.e.']];
//         }
//         if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {     
//             $summary_legal['art4_C']['number']++;
//             $summary_legal['art4_C']['pe'] += $agglo[$ind['generated_load_p.e.']];
//         }
//         if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NC') {
//             $summary_legal['art4_NC']['number']++;
//             $summary_legal['art4_NC']['pe'] += $agglo[$ind['generated_load_p.e.']];
//         }
//         if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {    
//             $summary_legal['art5_C']['number']++;
//             $summary_legal['art5_C']['pe'] += $agglo[$ind['generated_load_p.e.']];
//         }
//         if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC') {
//             $summary_legal['art5_NC']['number']++;
//             $summary_legal['art5_NC']['pe'] += $agglo[$ind['generated_load_p.e.']];        
//         }      
//     }
    
    //array1
//     dsm($graph_slice_size);
    $sheet->setCellValue('B5', $context['results']['graph']['0_10000']['number']);
    $sheet->setCellValue('C5', $context['results']['graph']['0_10000']['pe']);
    $sheet->setCellValue('B6', $context['results']['graph']['10000_100000']['number']);
    $sheet->setCellValue('C6', $context['results']['graph']['10000_100000']['pe']);
    $sheet->setCellValue('B7', $context['results']['graph']['100000_n']['number']);
    $sheet->setCellValue('C7', $context['results']['graph']['100000_n']['pe']);
    
    //previous
    $sheet->setCellValue('D5', uwwtd_register_get_cell_value($previoussheet, 'B5'));
    $sheet->setCellValue('E5', uwwtd_register_get_cell_value($previoussheet, 'C5')); 
    $sheet->setCellValue('D6', uwwtd_register_get_cell_value($previoussheet, 'B6')); 
    $sheet->setCellValue('E6', uwwtd_register_get_cell_value($previoussheet, 'C6')); 
    $sheet->setCellValue('D7', uwwtd_register_get_cell_value($previoussheet, 'B7')); 
    $sheet->setCellValue('E7', uwwtd_register_get_cell_value($previoussheet, 'C7')); 
    
    //array2
    $sheet->setCellValue('B15', $context['results']['graph']['total_generated']['pe']);
    $sheet->setCellValue('B16', $context['results']['graph']['total_collected']['pe']);
    $sheet->setCellValue('B17', $context['results']['graph']['total_load_ias']['pe']);
    $sheet->setCellValue('B18', $context['results']['graph']['total_entering']['pe']);
    $sheet->setCellValue('B19', $context['results']['graph']['total_capacity']['pe']);
    
    //previous
    $sheet->setCellValue('C15', uwwtd_register_get_cell_value($previoussheet, 'B15'));
    $sheet->setCellValue('C16', uwwtd_register_get_cell_value($previoussheet, 'B16'));
    $sheet->setCellValue('C17', uwwtd_register_get_cell_value($previoussheet, 'B17'));
    $sheet->setCellValue('C18', uwwtd_register_get_cell_value($previoussheet, 'B18')); 
    $sheet->setCellValue('C19', uwwtd_register_get_cell_value($previoussheet, 'B19')); 
    
        
    //array 5 6 7
//     $previous_year = uwwtd_get_previous_year($annee);
    $replaceyearby = $previous_year;
    if (false === $previous_year) {
        $replaceyearby = 'N/A';
    }
    $cell = $sheet->getCell('A52');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A53');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A55');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A56');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A58');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A59');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A62');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A63');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A65');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A66');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A68');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A69');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
                                                        
    $sheet->setCellValue('B43', $context['results']['graph']['art3_C']['number']);
    $sheet->setCellValue('C43', $context['results']['graph']['art3_C']['pe']);
    $sheet->setCellValue('B44', $context['results']['graph']['art3_NC']['number']);
    $sheet->setCellValue('C44', $context['results']['graph']['art3_NC']['pe']);
    $sheet->setCellValue('B45', $context['results']['graph']['art4_C']['number']);
    $sheet->setCellValue('C45', $context['results']['graph']['art4_C']['pe']);
    $sheet->setCellValue('B46', $context['results']['graph']['art4_NC']['number']);
    $sheet->setCellValue('C46', $context['results']['graph']['art4_NC']['pe']);
    $sheet->setCellValue('B47', $context['results']['graph']['art5_C']['number']);
    $sheet->setCellValue('C47', $context['results']['graph']['art5_C']['pe']);
    $sheet->setCellValue('B48', $context['results']['graph']['art5_NC']['number']);
    $sheet->setCellValue('C48', $context['results']['graph']['art5_NC']['pe']); 
    
    //previous
    $sheet->setCellValue('D43', uwwtd_register_get_cell_value($previoussheet, 'B43'));
    $sheet->setCellValue('E43', uwwtd_register_get_cell_value($previoussheet, 'C43'));
    $sheet->setCellValue('D44', uwwtd_register_get_cell_value($previoussheet, 'B44'));
    $sheet->setCellValue('E44', uwwtd_register_get_cell_value($previoussheet, 'C44'));
    $sheet->setCellValue('D45', uwwtd_register_get_cell_value($previoussheet, 'B45'));
    $sheet->setCellValue('E45', uwwtd_register_get_cell_value($previoussheet, 'C45'));
    $sheet->setCellValue('D46', uwwtd_register_get_cell_value($previoussheet, 'B46'));
    $sheet->setCellValue('E46', uwwtd_register_get_cell_value($previoussheet, 'C46'));
    $sheet->setCellValue('D47', uwwtd_register_get_cell_value($previoussheet, 'B47'));
    $sheet->setCellValue('E47', uwwtd_register_get_cell_value($previoussheet, 'C47'));
    $sheet->setCellValue('D48', uwwtd_register_get_cell_value($previoussheet, 'B48'));
    $sheet->setCellValue('E48', uwwtd_register_get_cell_value($previoussheet, 'C48'));
    
        
    //array 6
    $sheet->setCellValue('D53', $context['results']['graph']['art3_C']['pe']);  
    $sheet->setCellValue('E53', $context['results']['graph']['art3_NC']['pe']);
    $sheet->setCellValue('D56', $context['results']['graph']['art4_C']['pe']);   
    $sheet->setCellValue('E56', $context['results']['graph']['art4_NC']['pe']);  
    $sheet->setCellValue('D59', $context['results']['graph']['art5_C']['pe']);
    $sheet->setCellValue('E59', $context['results']['graph']['art5_NC']['pe']);
    
    //previous
    $sheet->setCellValue('B52', uwwtd_register_get_cell_value($previoussheet, 'D53')); 
    $sheet->setCellValue('C52', uwwtd_register_get_cell_value($previoussheet, 'E53')); 
    $sheet->setCellValue('B55', uwwtd_register_get_cell_value($previoussheet, 'D56')); 
    $sheet->setCellValue('C55', uwwtd_register_get_cell_value($previoussheet, 'E56')); 
    $sheet->setCellValue('B58', uwwtd_register_get_cell_value($previoussheet, 'D59')); 
    $sheet->setCellValue('C58', uwwtd_register_get_cell_value($previoussheet, 'E59')); 
    
        
    //array 7
    $sheet->setCellValue('D63', $context['results']['graph']['art3_C']['number']);     
    $sheet->setCellValue('E63', $context['results']['graph']['art3_NC']['number']);    
    $sheet->setCellValue('D66', $context['results']['graph']['art4_C']['number']);    
    $sheet->setCellValue('E66', $context['results']['graph']['art4_NC']['number']);    
    $sheet->setCellValue('D69', $context['results']['graph']['art5_C']['number']);    
    $sheet->setCellValue('E69', $context['results']['graph']['art5_NC']['number']); 
    
    //previous
    $sheet->setCellValue('B62', uwwtd_register_get_cell_value($previoussheet, 'D63')); 
    $sheet->setCellValue('C62', uwwtd_register_get_cell_value($previoussheet, 'E63'));
    $sheet->setCellValue('B65', uwwtd_register_get_cell_value($previoussheet, 'D66')); 
    $sheet->setCellValue('C65', uwwtd_register_get_cell_value($previoussheet, 'E66'));  
    $sheet->setCellValue('B68', uwwtd_register_get_cell_value($previoussheet, 'D69')); 
    $sheet->setCellValue('C68', uwwtd_register_get_cell_value($previoussheet, 'E69'));                          
    
    
    //array 3
    $nidms = uwwtd_check_exist(uwwtd_get_siteid('ms_level', array('year' => $annee)));    
    if ('' != $nidms) {
        $node = node_load($nidms);
        
        //Yearly production of sludge
        $field_mslsludgeproduction = isset($node->field_mslsludgeproduction['und'][0]['value']) ? $node->field_mslsludgeproduction['und'][0]['value'] : ''; 
        //Of which: re-used: Soil and agriculture
        $field_mslreusesoilagriculture = isset($node->field_mslreusesoilagriculture['und'][0]['value']) ? $node->field_mslreusesoilagriculture['und'][0]['value'] : ''; 
        //Of which: re-used: Others 
        $field_mslreuseothers = isset($node->field_mslreuseothers['und'][0]['value']) ? $node->field_mslreuseothers['und'][0]['value'] : ''; 
        //Of which: disposed: Landfill
        $field_msldisposallandfill = isset($node->field_msldisposallandfill['und'][0]['value']) ? $node->field_msldisposallandfill['und'][0]['value'] : ''; 
        //Of which: disposed: Incineration
        $field_msldisposalincineration = isset($node->field_msldisposalincineration['und'][0]['value']) ? $node->field_msldisposalincineration['und'][0]['value'] : ''; 
        //Of which: disposed: Others 
        $field_msldisposalothers = isset($node->field_msldisposalothers['und'][0]['value']) ? $node->field_msldisposalothers['und'][0]['value'] : '';
        //reuse percent
        $field_mslwwreuseperc = isset($node->field_mslwwreuseperc['und'][0]['value']) ? $node->field_mslwwreuseperc['und'][0]['value'] : '';
        unset($node);
        
        $sheet->setCellValue('B24', $field_mslsludgeproduction);
        $sheet->setCellValue('B25', $field_mslreusesoilagriculture);
        $sheet->setCellValue('B26', $field_mslreuseothers);
        $sheet->setCellValue('B27', $field_msldisposallandfill);
        $sheet->setCellValue('B28', $field_msldisposalincineration);
        $sheet->setCellValue('B29', $field_msldisposalothers);
        
        if (!empty($field_mslsludgeproduction)) {
            if (!empty($field_mslreusesoilagriculture)) {
                $sheet->setCellValue('C25', $field_mslreusesoilagriculture * 100 / $field_mslsludgeproduction);    
            } 
            if (!empty($field_mslreuseothers)) {
                $sheet->setCellValue('C26', $field_mslreuseothers * 100 / $field_mslsludgeproduction);    
            } 
            if (!empty($field_msldisposallandfill)) {
                $sheet->setCellValue('C27', $field_msldisposallandfill * 100 / $field_mslsludgeproduction);    
            }  
            if (!empty($field_msldisposalincineration)) {
                $sheet->setCellValue('C28', $field_msldisposalincineration * 100 / $field_mslsludgeproduction);    
            } 
            if (!empty($field_msldisposalothers)) {
                $sheet->setCellValue('C29', $field_msldisposalothers * 100 / $field_mslsludgeproduction);    
            }                                                        
        }
        
        //previous
        $sheet->setCellValue('D24', uwwtd_register_get_cell_value($previoussheet, 'B24'));
        $sheet->setCellValue('D25', uwwtd_register_get_cell_value($previoussheet, 'B25'));
        $sheet->setCellValue('D26', uwwtd_register_get_cell_value($previoussheet, 'B26'));
        $sheet->setCellValue('D27', uwwtd_register_get_cell_value($previoussheet, 'B27'));
        $sheet->setCellValue('D28', uwwtd_register_get_cell_value($previoussheet, 'B28'));
        $sheet->setCellValue('D29', uwwtd_register_get_cell_value($previoussheet, 'B29'));
        
        $sheet->setCellValue('E25', uwwtd_register_get_cell_value($previoussheet, 'C25')); 
        $sheet->setCellValue('E26', uwwtd_register_get_cell_value($previoussheet, 'C26')); 
        $sheet->setCellValue('E27', uwwtd_register_get_cell_value($previoussheet, 'C27')); 
        $sheet->setCellValue('E28', uwwtd_register_get_cell_value($previoussheet, 'C28')); 
        $sheet->setCellValue('E29', uwwtd_register_get_cell_value($previoussheet, 'C29'));            
        
        //array 4
        $sheet->setCellValue('C36', $context['results']['country_volume_waste_water']);
        $sheet->setCellValue('C37', $field_mslwwreuseperc);
        
        //previous
        $sheet->setCellValue('D36', uwwtd_register_get_cell_value($previoussheet, 'C36'));
        $sheet->setCellValue('D37', uwwtd_register_get_cell_value($previoussheet, 'C37'));           
        
        
    } 
    
    //array 8 Summary installation in place 
// $context['results']['graph']['summary_installation']
// $sheet->getCell('C10')->setValue($context['sandbox']['summary']['total']['number']);
// $sheet->getCell('D10')->setValue($context['sandbox']['summary']['total']['percentage']);
// $sheet->getCell('E10')->setValue($context['sandbox']['summary']['total']['waste_pe']);
// $sheet->getCell('F10')->setValue($context['sandbox']['summary']['total']['waste_percent']);
// 
// $sheet->getCell('C11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['number']);
// $sheet->getCell('D11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['percent']);
// $sheet->getCell('E11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['waste_pe']);
// $sheet->getCell('F11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['waste_percent']);    
// 
// $sheet->getCell('C12')->setValue($context['sandbox']['summary']['total']['collecting_system']['number']);
// $sheet->getCell('D12')->setValue($context['sandbox']['summary']['total']['collecting_system']['percent']);
// $sheet->getCell('E12')->setValue($context['sandbox']['summary']['total']['collecting_system']['waste_pe']);
// $sheet->getCell('F12')->setValue($context['sandbox']['summary']['total']['collecting_system']['waste_percent']);
// //secondary treatment    
// $sheet->getCell('C15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['number']);
// $sheet->getCell('D15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['percent']);
// $sheet->getCell('E15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe']);
// $sheet->getCell('F15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['waste_percent']);
//     
// $sheet->getCell('C16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['number']);
// $sheet->getCell('D16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['percent']);
// $sheet->getCell('E16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe']);
// $sheet->getCell('F16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['waste_percent']);    
// //more stringent treatment
// $sheet->getCell('C19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['number']);
// $sheet->getCell('D19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['percent']);
// $sheet->getCell('E19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe']);
// $sheet->getCell('F19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_percent']);
//     
// $sheet->getCell('C20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['number']);
// $sheet->getCell('D20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['percent']);
// $sheet->getCell('E20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe']);
// $sheet->getCell('F20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_percent']);
             

                 
    $cell = $sheet->getCell('A73');
    $cell->setValue(str_replace(array('[#COUNTRY#]', '[#current_year#]'), array($country_name, $annee), $cell->getValue()));
                                 
    $sheet->getCell('B75')->setValue($context['results']['graph']['summary_installation']['total']['number']);
    $sheet->getCell('C75')->setValue($context['results']['graph']['summary_installation']['total']['percentage']);
    $sheet->getCell('D75')->setValue($context['results']['graph']['summary_installation']['total']['waste_pe']);
    $sheet->getCell('E75')->setValue($context['results']['graph']['summary_installation']['total']['waste_percent']);
    
    $sheet->getCell('B76')->setValue($context['results']['graph']['summary_installation']['total']['collecting_system']['number']);
    $sheet->getCell('C76')->setValue($context['results']['graph']['summary_installation']['total']['collecting_system']['percentage']);
    $sheet->getCell('D76')->setValue($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_pe']);
    $sheet->getCell('E76')->setValue($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_percent']);
    
    $sheet->getCell('B77')->setValue($context['results']['graph']['summary_installation']['secondary']['installation_in_plac']['number']);
    $sheet->getCell('C77')->setValue($context['results']['graph']['summary_installation']['secondary']['installation_in_plac']['percentage']);
    $sheet->getCell('D77')->setValue($context['results']['graph']['summary_installation']['secondary']['installation_in_plac']['waste_pe']);
    $sheet->getCell('E77')->setValue($context['results']['graph']['summary_installation']['secondary']['installation_in_plac']['waste_percent']);
    
    $sheet->getCell('B78')->setValue($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['number']);
    $sheet->getCell('C78')->setValue($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['percentage']);
    $sheet->getCell('D78')->setValue($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_pe']);
    $sheet->getCell('E78')->setValue($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_percent']);
    
    $sheet->getCell('B79')->setValue($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['number']);
    $sheet->getCell('C79')->setValue($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['percentage']);
    $sheet->getCell('D79')->setValue($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_pe']);
    $sheet->getCell('E79')->setValue($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_percent']);
    
    $sheet->getCell('B80')->setValue($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['number']);
    $sheet->getCell('C80')->setValue($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['percentage']);
    $sheet->getCell('D80')->setValue($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_pe']);
    $sheet->getCell('E80')->setValue($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_percent']);
    
    //previous   
    $sheet->setCellValue('F75', uwwtd_register_get_cell_value($previoussheet, 'B75'));
    $sheet->setCellValue('G75', uwwtd_register_get_cell_value($previoussheet, 'C75'));
    $sheet->setCellValue('H75', uwwtd_register_get_cell_value($previoussheet, 'D75'));
    $sheet->setCellValue('I75', uwwtd_register_get_cell_value($previoussheet, 'E75'));     
                                                                                  
    $sheet->setCellValue('F76', uwwtd_register_get_cell_value($previoussheet, 'B76'));
    $sheet->setCellValue('G76', uwwtd_register_get_cell_value($previoussheet, 'C76'));
    $sheet->setCellValue('H76', uwwtd_register_get_cell_value($previoussheet, 'D76'));
    $sheet->setCellValue('I76', uwwtd_register_get_cell_value($previoussheet, 'E76'));
                                                                                  
    $sheet->setCellValue('F77', uwwtd_register_get_cell_value($previoussheet, 'B77'));
    $sheet->setCellValue('G77', uwwtd_register_get_cell_value($previoussheet, 'C77'));
    $sheet->setCellValue('H77', uwwtd_register_get_cell_value($previoussheet, 'D77'));
    $sheet->setCellValue('I77', uwwtd_register_get_cell_value($previoussheet, 'E77'));
                                                                                  
    $sheet->setCellValue('F78', uwwtd_register_get_cell_value($previoussheet, 'B78'));
    $sheet->setCellValue('G78', uwwtd_register_get_cell_value($previoussheet, 'C78'));
    $sheet->setCellValue('H78', uwwtd_register_get_cell_value($previoussheet, 'D78'));
    $sheet->setCellValue('I78', uwwtd_register_get_cell_value($previoussheet, 'E78'));
                                                                                  
    $sheet->setCellValue('F79', uwwtd_register_get_cell_value($previoussheet, 'B79'));
    $sheet->setCellValue('G79', uwwtd_register_get_cell_value($previoussheet, 'C79'));
    $sheet->setCellValue('H79', uwwtd_register_get_cell_value($previoussheet, 'D79'));
    $sheet->setCellValue('I79', uwwtd_register_get_cell_value($previoussheet, 'E79'));
                                                                                  
    $sheet->setCellValue('F80', uwwtd_register_get_cell_value($previoussheet, 'B80'));
    $sheet->setCellValue('G80', uwwtd_register_get_cell_value($previoussheet, 'C80'));
    $sheet->setCellValue('H80', uwwtd_register_get_cell_value($previoussheet, 'D80'));
    $sheet->setCellValue('I80', uwwtd_register_get_cell_value($previoussheet, 'E80'));                
                                                            
    //array 9 Summary distance to compliance expired deadline
    //Target connection pe
    $sheet->getCell('C88')->setValue($context['results']['distance_to_compliance']['expired']['target_connection']['total_load_pe']);
    //not connected
    $sheet->getCell('B89')->setValue($context['results']['distance_to_compliance']['expired']['target_connection']['distance_%']);
    $sheet->getCell('C89')->setValue($context['results']['distance_to_compliance']['expired']['target_connection']['distance_pe']);
    //2nd treatment target
    $sheet->getCell('C90')->setValue($context['results']['distance_to_compliance']['expired']['secondary_target']['total_load_pe']);
    //2nd treatment not in place
    $sheet->getCell('B91')->setValue($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_%']);
    $sheet->getCell('C91')->setValue($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_pe']);
    //2nd treatment performance not met
    $sheet->getCell('B92')->setValue($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_%']);
    $sheet->getCell('C92')->setValue($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_pe']);
    //Tertiary treatment target
    $sheet->getCell('C93')->setValue($context['results']['distance_to_compliance']['expired']['morestringent_target']['total_load_pe']);
    //Tertiary treatment not in place
    $sheet->getCell('B94')->setValue($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_%']);
    $sheet->getCell('C94')->setValue($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_pe']);
    //Tertiary stringent treatment performance not met
    $sheet->getCell('B95')->setValue($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_%']);
    $sheet->getCell('C95')->setValue($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_pe']);
    
    //previous
    $sheet->setCellValue('E88', uwwtd_register_get_cell_value($previoussheet, 'C88'));
    //not EonneEted                                                               
    $sheet->setCellValue('D89', uwwtd_register_get_cell_value($previoussheet, 'B89'));
    $sheet->setCellValue('E89', uwwtd_register_get_cell_value($previoussheet, 'C89'));
    //2nd treatment targe                                                      arg
    $sheet->setCellValue('E90', uwwtd_register_get_cell_value($previoussheet, 'C90'));
    //2nd treatment not i                                                      ot 
    $sheet->setCellValue('D91', uwwtd_register_get_cell_value($previoussheet, 'B91'));
    $sheet->setCellValue('E91', uwwtd_register_get_cell_value($previoussheet, 'C91'));
    //2nd treatment perfo                                                      erf
    $sheet->setCellValue('D92', uwwtd_register_get_cell_value($previoussheet, 'B92'));
    $sheet->setCellValue('E92', uwwtd_register_get_cell_value($previoussheet, 'C92'));
    //Tertiary treatment                                                       ent
    $sheet->setCellValue('E93', uwwtd_register_get_cell_value($previoussheet, 'C93'));
    //Tertiary treatment                                                       ent
    $sheet->setCellValue('D94', uwwtd_register_get_cell_value($previoussheet, 'B94'));
    $sheet->setCellValue('E94', uwwtd_register_get_cell_value($previoussheet, 'C94'));
    //Tertiary stringent                                                       ent
    $sheet->setCellValue('D95', uwwtd_register_get_cell_value($previoussheet, 'B95'));
    $sheet->setCellValue('E95', uwwtd_register_get_cell_value($previoussheet, 'C95'));    
    
    
    //array 10 summary distance to compliance pending deadlines
    $sheet->getCell('C104')->setValue($context['results']['distance_to_compliance']['pending']['target_connection']['total_load_pe']);
    //not connected
    $sheet->getCell('B105')->setValue($context['results']['distance_to_compliance']['pending']['target_connection']['distance_%']);
    $sheet->getCell('C105')->setValue($context['results']['distance_to_compliance']['pending']['target_connection']['distance_pe']);
    //2nd treatment target
    $sheet->getCell('C106')->setValue($context['results']['distance_to_compliance']['pending']['secondary_target']['total_load_pe']);
    //2nd treatment not in place
    $sheet->getCell('B107')->setValue($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_%']);
    $sheet->getCell('C107')->setValue($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_pe']);
    //2nd treatment performance not met
    $sheet->getCell('B108')->setValue($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_%']);
    $sheet->getCell('C108')->setValue($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_pe']);
    //Tertiary treatment target
    $sheet->getCell('C109')->setValue($context['results']['distance_to_compliance']['pending']['morestringent_target']['total_load_pe']);
    //Tertiary treatment not in place
    $sheet->getCell('B110')->setValue($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_%']);
    $sheet->getCell('C110')->setValue($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_pe']);
    //Tertiary stringent treatment performance not met
    $sheet->getCell('B111')->setValue($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_%']);
    $sheet->getCell('C111')->setValue($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_pe']);
    
    //previous
    $sheet->setCellValue('E104', uwwtd_register_get_cell_value($previoussheet, 'C104'));    
    //not connected                                                                 
    $sheet->setCellValue('D105', uwwtd_register_get_cell_value($previoussheet, 'B105'));    
    $sheet->setCellValue('E105', uwwtd_register_get_cell_value($previoussheet, 'C105'));    
    //2nd treatment target                                                      arge
    $sheet->setCellValue('E106', uwwtd_register_get_cell_value($previoussheet, 'C106'));    
    //2nd treatment not in                                                      ot i
    $sheet->setCellValue('D107', uwwtd_register_get_cell_value($previoussheet, 'B107'));    
    $sheet->setCellValue('E107', uwwtd_register_get_cell_value($previoussheet, 'C107'));    
    //2nd treatment perfor                                                      erfo
    $sheet->setCellValue('D108', uwwtd_register_get_cell_value($previoussheet, 'B108'));    
    $sheet->setCellValue('E108', uwwtd_register_get_cell_value($previoussheet, 'C108'));    
    //Tertiary treatment t                                                      ent 
    $sheet->setCellValue('E109', uwwtd_register_get_cell_value($previoussheet, 'C109'));    
    //Tertiary treatment n                                                      ent 
    $sheet->setCellValue('D110', uwwtd_register_get_cell_value($previoussheet, 'B110'));    
    $sheet->setCellValue('E110', uwwtd_register_get_cell_value($previoussheet, 'C110'));    
    //Tertiary stringent t                                                      ent 
    $sheet->setCellValue('D111', uwwtd_register_get_cell_value($previoussheet, 'B111'));    
    $sheet->setCellValue('E111', uwwtd_register_get_cell_value($previoussheet, 'C111'));    
    
    //array 11 summary distance to compliance total
    $sheet->getCell('C119')->setValue($context['results']['distance_to_compliance']['total']['target_connection']['total_load_pe']);
    //not connected
    $sheet->getCell('B120')->setValue($context['results']['distance_to_compliance']['total']['target_connection']['distance_%']);
    $sheet->getCell('C120')->setValue($context['results']['distance_to_compliance']['total']['target_connection']['distance_pe']);
    //2nd treatment target
    $sheet->getCell('C121')->setValue($context['results']['distance_to_compliance']['total']['secondary_target']['total_load_pe']);
    //2nd treatment not in place
    $sheet->getCell('B122')->setValue($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_%']);
    $sheet->getCell('C122')->setValue($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_pe']);
    //2nd treatment performance not met
    $sheet->getCell('B123')->setValue($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_%']);
    $sheet->getCell('C123')->setValue($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_pe']);
    //Tertiary treatment target
    $sheet->getCell('C124')->setValue($context['results']['distance_to_compliance']['total']['morestringent_target']['total_load_pe']);
    //Tertiary treatment not in place
    $sheet->getCell('B125')->setValue($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_%']);
    $sheet->getCell('C125')->setValue($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_pe']);
    //Tertiary stringent treatment performance not met
    $sheet->getCell('B126')->setValue($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_%']);
    $sheet->getCell('C126')->setValue($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_pe']);
    
    //previous
    $sheet->setCellValue('E119', uwwtd_register_get_cell_value($previoussheet, 'C119'));
    //not connected                                                                 
    $sheet->setCellValue('D120', uwwtd_register_get_cell_value($previoussheet, 'B120'));
    $sheet->setCellValue('E120', uwwtd_register_get_cell_value($previoussheet, 'C120'));
    //2nd treatment target                                                      arge
    $sheet->setCellValue('E121', uwwtd_register_get_cell_value($previoussheet, 'C121'));
    //2nd treatment not in                                                      ot i
    $sheet->setCellValue('D122', uwwtd_register_get_cell_value($previoussheet, 'B122'));
    $sheet->setCellValue('E122', uwwtd_register_get_cell_value($previoussheet, 'C122'));
    //2nd treatment perfor                                                      erfo
    $sheet->setCellValue('D123', uwwtd_register_get_cell_value($previoussheet, 'B123'));
    $sheet->setCellValue('E123', uwwtd_register_get_cell_value($previoussheet, 'C123'));
    //Tertiary treatment t                                                      ent 
    $sheet->setCellValue('E124', uwwtd_register_get_cell_value($previoussheet, 'C124'));
    //Tertiary treatment n                                                      ent 
    $sheet->setCellValue('D125', uwwtd_register_get_cell_value($previoussheet, 'B125'));
    $sheet->setCellValue('E125', uwwtd_register_get_cell_value($previoussheet, 'C125'));
    //Tertiary stringent t                                                      ent 
    $sheet->setCellValue('D126', uwwtd_register_get_cell_value($previoussheet, 'B126'));
    $sheet->setCellValue('E126', uwwtd_register_get_cell_value($previoussheet, 'C126'));
    
    //array 12  treatment details
    //todo
    $sheet->setCellValue('B131', '');
    $sheet->setCellValue('C131', '');
    $sheet->setCellValue('B132', '');
    $sheet->setCellValue('C132', '');
    $sheet->setCellValue('B133', '');
    $sheet->setCellValue('C133', '');
    $sheet->setCellValue('B134', '');
    $sheet->setCellValue('C134', '');
    $sheet->setCellValue('B135', '');
    $sheet->setCellValue('C135', '');
    $sheet->setCellValue('B136', '');
    $sheet->setCellValue('C136', '');
    $sheet->setCellValue('B137', '');
    $sheet->setCellValue('C137', '');
    $sheet->setCellValue('B138', '');
    $sheet->setCellValue('C138', '');
    $sheet->setCellValue('B139', '');
    $sheet->setCellValue('C139', '');  
      
    //array 13    incoming/discharged        
    $sheet->getCell('C148')->setValue($context['results']['graph']['incoming_load_bod']);
    $sheet->getCell('C149')->setValue($context['results']['graph']['discharge_load_bod']);
    $sheet->getCell('C150')->setValue($context['results']['graph']['incoming_load_cod']);
    $sheet->getCell('C151')->setValue($context['results']['graph']['discharge_load_cod']);
    $sheet->getCell('C152')->setValue($context['results']['graph']['incoming_load_n']);
    $sheet->getCell('C153')->setValue($context['results']['graph']['discharge_load_n']);
    $sheet->getCell('C154')->setValue($context['results']['graph']['incoming_load_p']);
    $sheet->getCell('C155')->setValue($context['results']['graph']['discharge_load_p']);
    
    //previous
    $sheet->getCell('D148')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C148'));
    $sheet->getCell('D149')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C149'));
    $sheet->getCell('D150')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C150'));
    $sheet->getCell('D151')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C151'));
    $sheet->getCell('D152')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C152'));
    $sheet->getCell('D153')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C153'));
    $sheet->getCell('D154')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C154'));
    $sheet->getCell('D155')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C155'));        
       
    //array 14    rate
    $sheet->getCell('C159')->setValue((1-($context['results']['graph']['discharge_load_bod'] / $context['results']['graph']['incoming_load_bod'])) *100);
    $sheet->getCell('C160')->setValue((1-($context['results']['graph']['discharge_load_cod'] / $context['results']['graph']['incoming_load_cod'])) *100);
    $sheet->getCell('C161')->setValue((1-($context['results']['graph']['discharge_load_n'] / $context['results']['graph']['incoming_load_n'])) *100);
    $sheet->getCell('C162')->setValue((1-($context['results']['graph']['discharge_load_p'] / $context['results']['graph']['incoming_load_p'])) *100);
    //previous
    $sheet->getCell('D159')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C159'));
    $sheet->getCell('D160')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C160'));
    $sheet->getCell('D161')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C161'));
    $sheet->getCell('D162')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C162'));    
    
    //array 15   totla volume
    $sheet->getCell('C166')->setValue($context['results']['country_volume_waste_water']);
    //previous
    $sheet->getCell('D166')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C166'));
    
    //array 16  average  
    $sheet->getCell('C169')->setValue(($context['results']['graph']['incoming_load_bod'] / $context['results']['country_volume_waste_water']) * 1000000);
    $sheet->getCell('C170')->setValue(($context['results']['graph']['discharge_load_bod'] / $context['results']['country_volume_waste_water']) * 1000000);
    $sheet->getCell('C171')->setValue(($context['results']['graph']['incoming_load_cod'] / $context['results']['country_volume_waste_water']) * 1000000);
    $sheet->getCell('C172')->setValue(($context['results']['graph']['discharge_load_cod'] / $context['results']['country_volume_waste_water']) * 1000000);
    $sheet->getCell('C173')->setValue(($context['results']['graph']['incoming_load_n'] / $context['results']['country_volume_waste_water']) * 1000000);
    $sheet->getCell('C174')->setValue(($context['results']['graph']['discharge_load_n'] / $context['results']['country_volume_waste_water']) * 1000000);
    $sheet->getCell('C175')->setValue(($context['results']['graph']['incoming_load_p'] / $context['results']['country_volume_waste_water']) * 1000000);
    $sheet->getCell('C176')->setValue(($context['results']['graph']['discharge_load_p'] / $context['results']['country_volume_waste_water']) * 1000000);
    
    //previous
    $sheet->getCell('D169')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C169'));
    $sheet->getCell('D170')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C170'));
    $sheet->getCell('D171')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C171'));
    $sheet->getCell('D172')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C172'));
    $sheet->getCell('D173')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C173'));
    $sheet->getCell('D174')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C174'));
    $sheet->getCell('D175')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C175'));
    $sheet->getCell('D176')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C176'));               
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
//     $objWriter->setPreCalculateFormulas(true);
    $objWriter->setIncludeCharts(TRUE);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name']));    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_new_agglomerations_sheet12($annee, &$context)
{
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filenameagglo = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
    $filenameagglo = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
    

//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_agglo']['csv_name'] . '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_agglo']['csv_name'], $annee);
        
    
    $ind = $GLOBALS['uwwtd']['sheet12_newagglo'];
    
	if (!isset($context['sandbox']['progress'])) {  
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filenameagglo);

        $r = fopen($filename, 'wb');  
    
        $header = array(
            $ind['id_of_agglomeration'] => 'id_of_agglomeration', 
            $ind['name_of_agglomeration'] => 'name_of_agglomeration', 
            $ind['size_of_agglomeration'] => 'size_of_agglomeration', 
            $ind['reason'] => 'reason', 
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');  
        
        //get previous year
        $context['sandbox']['previous_year'] = uwwtd_get_previous_year($annee);
        //if not previous year nothing do to
        if (false === $context['sandbox']['previous_year']) {
            $context['message'] = '('.$annee.') ' . t('End of @name', array('@name' => $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name']));
            $context['sandbox']['progress'] = $context['sandbox']['max'];
            $context['finished'] = 1;
            return;    
        }
                     
    } else {
        $r = fopen($filename, 'ab');  
    }     

    $ragglofile = fopen($filenameagglo, 'rb');
    
    //pass header line
    if ($context['sandbox']['progress'] == 0) {
        $agglo = fgetcsv($ragglofile, 5000, ';');
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
    } else {    
        fseek($ragglofile, $context['sandbox']['file_position']);
    }
    $indagglo = $GLOBALS['uwwtd']['sheet3_agglo'];

    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
//     get all agglo from csv file
//         check if agglo > 2000 and exist in previous year
//             if no add a line 
        if ($agglo[$indagglo['generated_load_p.e.']] >= 2000 
            && '' == uwwtd_check_exist(uwwtd_get_siteid('agglomeration', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agglo[$indagglo['aggcode']])))) {
            $line = array();
            $line[$ind['id_of_agglomeration']] = $agglo[$indagglo['aggcode']];    
            $line[$ind['name_of_agglomeration']] = $agglo[$indagglo['aggname']];    
            $line[$ind['size_of_agglomeration']] = $agglo[$indagglo['generated_load_p.e.']];    
            $line[$ind['reason']] = '';  
            fputcsv($r, $line, ';');     
        }       
        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];    
    }
    
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/'. $context['sandbox']['max'];    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 *
 */ 
function uwwtd_create_register_2015_agglomerationout_sheet13($annee, &$context)
{
//     get previous year
//     get all agglo from previous year active >= 2000
//     if agglo current year is not active
//         add a line
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo_out']['csv_name'] . '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['csv_name'], $annee);    
    
    $ind = $GLOBALS['uwwtd']['sheet13_aggloout'];
    
	if (!isset($context['sandbox']['progress'])) {  
        $context['sandbox']['progress'] = 0;
//         dsm($filename);
        $r = fopen($filename, 'wb');  
//         dsm($r);
        $header = array(
            $ind['id_of_agglomeration'] => 'id_of_agglomeration', 
            $ind['name_of_agglomeration'] => 'name_of_agglomeration', 
            $ind['size_of_agglomeration'] => 'size_of_agglomeration',
            $ind['remarks'] => 'remarks', 
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');  
        
        //get previous year
        $context['sandbox']['previous_year'] = uwwtd_get_previous_year($annee);
        //if not previous year nothing do to
        if (false === $context['sandbox']['previous_year']) {
            $context['message'] = '('.$annee.') ' . t('End of  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_model_sheet_name']));
            $context['sandbox']['progress'] = $context['sandbox']['max'];
            $context['finished'] = 1;
            return;    
        }
        
        $query = 'select n.nid
        from {node} n
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id
            left join
                {field_data_field_agggenerated} f_agggenerated
                    ON n.nid = f_agggenerated.entity_id
            left join
                {field_data_field_status} f_status
                    ON n.nid = f_status.entity_id                
                    
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
            and f_agggenerated.field_agggenerated_value >= :agggenerated
            and f_status.field_status_value = :status
        ';
        
        $param = array(
            ':typenode' => 'agglomeration',
            ':annee' => $context['sandbox']['previous_year'],
            ':agggenerated' => 2000,
            ':status' => 1,
        );
        $context['sandbox']['id'] = array();
        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['id'][] = $row['nid'];
            }
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
        
        $context['sandbox']['max'] = count($context['sandbox']['id']);
    } else {
        $r = fopen($filename, 'ab');  
    }
        
    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {
    
        $previousagglo = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
        
        if ('' != ($agglonid = uwwtd_check_exist(uwwtd_get_siteid('agglomeration', array('year' => $annee, 'aggCode' => $previousagglo->field_inspireidlocalid['und'][0]['value'] ))))) {
            $agg = node_load($agglonid);
            if ($agg->field_status['und'][0]['value'] === '0') {
                $line = array(
                    $ind['id_of_agglomeration'] => $previousagglo->field_inspireidlocalid['und'][0]['value'], 
                    $ind['name_of_agglomeration'] => $previousagglo->title, 
                    $ind['size_of_agglomeration'] => $previousagglo->field_agggenerated['und'][0]['value'],
                    $ind['remarks'] => '', 
                );
                fputcsv($r, $line, ';');                 
            }
        }    
        $context['sandbox']['progress']++;
        $j++;
    }    
   
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_model_sheet_name'])) . ' ' .  $context['sandbox']['progress'] . '/'. $context['sandbox']['max'];    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 *
 */ 
function uwwtd_create_register_2015_new_treatment_plants_sheet14($annee, &$context)
{
//         
//     get previous year
//     if not previous year nothing do to

//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filenameuwwtp = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] . '.csv';
    $filenameuwwtp = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'], $annee);

//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_uwwtp']['csv_name'] . '.csv';  
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['csv_name'], $annee);  
    
    $ind = $GLOBALS['uwwtd']['sheet14_newuwwtp'];
    
	if (!isset($context['sandbox']['progress'])) {  
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filenameuwwtp);

        $r = fopen($filename, 'wb');  

        
        $header = array(
            $ind['id_of_uwwtp'] => 'id_of_uwwtp', 
            $ind['name_of_uwwtp'] => 'name_of_uwwtp', 
            $ind['id_of_agglomeration'] => 'id_of_agglomeration',
            $ind['name_of_agglomeration'] => 'name_of_agglomeration', 
            $ind['size_of_uwwtp'] => 'size_of_uwwtp',              
            $ind['reason'] => 'reason', 
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');  
        
        //get previous year
        $context['sandbox']['previous_year'] = uwwtd_get_previous_year($annee);
        //if not previous year nothing do to
        if (false === $context['sandbox']['previous_year']) {
            $context['message'] = '('.$annee.') ' . t('End of @name', array('@name' => $GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_model_sheet_name']));
            $context['sandbox']['progress'] = $context['sandbox']['max'];
            $context['finished'] = 1;
            return;    
        }
                     
    } else {
        $r = fopen($filename, 'ab');  
    }     

    $ruwwtpfile = fopen($filenameuwwtp, 'rb');
    
    //pass header line
    if ($context['sandbox']['progress'] == 0) {
        $uwwtp = fgetcsv($ruwwtpfile, 5000, ';');
        $context['sandbox']['file_position'] = ftell($ruwwtpfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
    } else {    
        fseek($ruwwtpfile, $context['sandbox']['file_position']);
    }
    $induwwtp = $GLOBALS['uwwtd']['sheet2_uwwtp'];

    $j = 0;
    while ($j < 20 && $uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {
//     get all uwwtp from csv file
//         check if exist in previous year
//             if no add a line 
        if ('' == uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $context['sandbox']['previous_year'], 'uwwCode' => $uwwtp[$induwwtp['uww_code']])))) {
            $line = array();
            $line[$ind['id_of_uwwtp']] = $uwwtp[$induwwtp['uww_code']];    
            $line[$ind['name_of_uwwtp']] = $uwwtp[$induwwtp['uww_name']];    
            $line[$ind['id_of_agglomeration']] = $uwwtp[$induwwtp['aggcode']];
            $line[$ind['name_of_agglomeration']] = $uwwtp[$induwwtp['aggname']];
            $line[$ind['size_of_uwwtp']] = $uwwtp[$induwwtp['uww_capacity p. e.']];    
            $line[$ind['reason']] = ''; 
            fputcsv($r, $line, ';');     
        }       
        $j++;        
        $context['sandbox']['file_position'] = ftell($ruwwtpfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];    
    }
    
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_model_sheet_name'])) . ' ' .  $context['sandbox']['progress'] . '/'. $context['sandbox']['max'];    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 *
 */ 
function uwwtd_create_register_2015_uwwtpsout_sheet15($annee, &$context)
{
//     get previous year
//     get all uwwtp from prvious year active 
//     if uwwtp current year is not active
//         add a line 
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp_out']['csv_name'] . '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['csv_name'], $annee);    
    
    $ind = $GLOBALS['uwwtd']['sheet15_uwwtpout'];
    
	if (!isset($context['sandbox']['progress'])) {  
        $context['sandbox']['progress'] = 0;
//         dsm($filename);
        $r = fopen($filename, 'wb');  
//         dsm($r);
   
        $header = array(
            $ind['id_of_uwwtp'] => 'id_of_uwwtp', 
            $ind['name_of_uwwtp'] => 'name_of_uwwtp', 
            $ind['id_of_agglomeration'] => 'id_of_agglomeration',
            $ind['name_of_agglomeration'] => 'name_of_agglomeration',
            $ind['size_of_uwwtp'] => 'size_of_uwwtp',
            $ind['remarks'] => 'remarks', 
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');  
        
        //get previous year
        $context['sandbox']['previous_year'] = uwwtd_get_previous_year($annee);
        //if not previous year nothing do to
        if (false === $context['sandbox']['previous_year']) {
            $context['message'] = '('.$annee.') ' . t('End of @name', array('@name' => $GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_model_sheet_name']));
            $context['sandbox']['progress'] = $context['sandbox']['max'];
            $context['finished'] = 1;
            return;    
        }
        
        $query = 'select n.nid
        from {node} n
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id
            left join
                {field_data_field_status} f_status
                    ON n.nid = f_status.entity_id                
                    
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
            and f_status.field_status_value = :status
        ';
        
        $param = array(
            ':typenode' => 'uwwtp',
            ':annee' => $context['sandbox']['previous_year'],
            ':status' => 1,
        );
        $context['sandbox']['id'] = array();
        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['id'][] = $row['nid'];
            }
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
        
        $context['sandbox']['max'] = count($context['sandbox']['id']);
    } else {
        $r = fopen($filename, 'ab');  
    }
        
    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {
    
        $previousuwwtp = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
        
        if ('' != ($uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $annee, 'uwwCode' => $previousuwwtp->field_inspireidlocalid['und'][0]['value'] ))))) {
            $uwwtp = node_load($uwwtpnid);
            if ($uwwtp->field_status['und'][0]['value'] === '0') {
            
                $list_id_agglo = '';
                $list_name_agglo = '';
                
                foreach($uwwtp->field_uwwaggliste['und'] as $nidagg) {
                    $agg = node_load($nidagg['nid']);
                    $list_id_agglo = ($list_id_agglo == '' ? '' : ';') . $agg->field_inspireidlocalid['und'][0]['value'];
                    $list_name_agglo = ($list_name_agglo == '' ? '' : ';') . $agg->title;
                    unset($agg);
                }
                
                $line = array(
                    $ind['id_of_uwwtp'] => $previousuwwtp->field_inspireidlocalid['und'][0]['value'],
                    $ind['name_of_uwwtp'] => $previousuwwtp->title, 
                    $ind['id_of_agglomeration'] => $list_id_agglo,
                    $ind['name_of_agglomeration'] => $list_name_agglo, 
                    $ind['size_of_uwwtp'] => $previousuwwtp->field_physicalcapacityactivity['und'][0]['value'],
                    $ind['remarks'] => '',
                );
                fputcsv($r, $line, ';');                 
            }
        }    
        $context['sandbox']['progress']++;
        $j++;
    }    
   
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/'. $context['sandbox']['max'];    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}


/**
 *
 */ 
function uwwtd_create_register_2015_file_new_agglomerations_sheet12($annee, &$context)
{
    $ind = $GLOBALS['uwwtd']['sheet12_newagglo'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_agglo']['csv_name'], $annee);
        
    $ragglofile = fopen($filename, 'rb');
        
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        //ignore header    
        $agglo = fgetcsv($ragglofile, 5000, ';');
        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        
        $context['sandbox']['line'] = 6;
        
        $model_path = uwwtd_getregister_modelfile_path();
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_name'], $annee);
        $target_path = $model_path;
    } 
    
    fseek($ragglofile, $context['sandbox']['file_position']);
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
//     $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_name'] .'.xlsx';
//     $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name']);
    
//     $start_line = 6;
//     $sheet->insertNewRowBefore(5, count($listagglo));
    
//     $i = $start_line;
    
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_agglo']['csv_name'] . '.csv';

    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['size_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['reason'], $context['sandbox']['line'], $agglo[$ind['reason']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'], 1);
        
        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];              
    }
    fclose($ragglofile);  
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
//     $context['sandbox']['progress']++;
 
//     $context['message'] = t('new_agglomerations_sheet12 finish');    
//     
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     }
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }        
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_agglomerationout_sheet13($annee, &$context)
{
    $ind = $GLOBALS['uwwtd']['sheet13_aggloout'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['csv_name'], $annee);
        
    $ragglofile = fopen($filename, 'rb');
        
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
       //ignore header    
        $agglo = fgetcsv($ragglofile, 5000, ';');
        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        
        $context['sandbox']['line'] = 6;
        
        $model_path = uwwtd_getregister_modelfile_path();
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_name'], $annee);
        $target_path = $model_path;
    } 
    
    fseek($ragglofile, $context['sandbox']['file_position']);
    
// 	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
//     $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_name'] .'.xlsx';
//     $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_model_sheet_name']);    
    
    
//     $start_line = 6;
//     $sheet->insertNewRowBefore(5, count($listagglo));
    
//     $i = $start_line;
//     $ind = $GLOBALS['uwwtd']['sheet13_aggloout'];
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo_out']['csv_name'] . '.csv';
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['csv_name'], $annee);
        
//     $ragglofile = fopen($filename, 'rb');
//     while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {    
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['size_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['remarks'], $context['sandbox']['line'], $agglo[$ind['remarks']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'], 1);
        
        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];        
    }
    fclose($ragglofile);   
        
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
//     $context['sandbox']['progress']++;
 
//     $context['message'] = t('agglomerationout_sheet13 finish');    
//     
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     }  
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_new_treatment_plants_sheet14($annee, &$context)
{
    $ind = $GLOBALS['uwwtd']['sheet14_newuwwtp'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['csv_name'], $annee);
            
    $rfile = fopen($filename, 'rb');
        
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        //ignore header    
        $agglo = fgetcsv($rfile, 5000, ';');
        
        $context['sandbox']['file_position'] = ftell($rfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        
        $context['sandbox']['line'] = 5;
        
        $model_path = uwwtd_getregister_modelfile_path();
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_name'], $annee);
        $target_path = $model_path;
    } 
    
    fseek($rfile, $context['sandbox']['file_position']);
    
// 	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
//     $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_name'] .'.xlsx';
//     $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_model_sheet_name']);    
    
//     $start_line = 5;
    
//     $i = $start_line;
//     $ind = $GLOBALS['uwwtd']['sheet14_newuwwtp'];
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_uwwtp']['csv_name'] . '.csv';
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['csv_name'], $annee);
//             
//     $rfile = fopen($filename, 'rb');
//     while ($line = fgetcsv($rfile, 5000, ';')) {
    $j = 0;
    while ($j < 20 && $line = fgetcsv($rfile, 5000, ';')) {    
        $sheet->setCellValueByColumnAndRow($ind['id_of_uwwtp'], $context['sandbox']['line'], $line[$ind['id_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_uwwtp'], $context['sandbox']['line'], $line[$ind['name_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $line[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $line[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_uwwtp'], $context['sandbox']['line'], $line[$ind['size_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['reason'], $context['sandbox']['line'], $line[$ind['reason']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'], 1);
        
        $j++;        
        $context['sandbox']['file_position'] = ftell($rfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];           
    }
    fclose($rfile);  
        
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
//     $context['sandbox']['progress']++;
 
//     $context['message'] = t('new_treatment_plants_sheet14 finish');    
//     
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     }
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }       
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_uwwtpsout_sheet15($annee, &$context)
{
    $ind = $GLOBALS['uwwtd']['sheet15_uwwtpout'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['csv_name'], $annee);
        
    $rfile = fopen($filename, 'rb');
        
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        //ignore header    
        $agglo = fgetcsv($rfile, 5000, ';');
        
        $context['sandbox']['file_position'] = ftell($rfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        
        $context['sandbox']['line'] = 6;
        
        $model_path = uwwtd_getregister_modelfile_path();
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_name'], $annee);
        $target_path = $model_path;
    } 
    
    fseek($rfile, $context['sandbox']['file_position']);
    
    
// 	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
//     $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_name'] .'.xlsx';
//     $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_model_sheet_name']);    
    
//     $start_line = 6;
    
//     $i = $start_line;
//     $ind = $GLOBALS['uwwtd']['sheet15_uwwtpout'];
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp_out']['csv_name'] . '.csv';
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['csv_name'], $annee);
                
//     $rfile = fopen($filename, 'rb');
//     while ($line = fgetcsv($rfile, 5000, ';')) {
    $j = 0;
    while ($j < 20 && $line = fgetcsv($rfile, 5000, ';')) {        
        $sheet->setCellValueByColumnAndRow($ind['id_of_uwwtp'], $context['sandbox']['line'], $line[$ind['id_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_uwwtp'], $context['sandbox']['line'], $line[$ind['name_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $line[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $line[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_uwwtp'], $context['sandbox']['line'], $line[$ind['size_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['remarks'], $context['sandbox']['line'], $line[$ind['remarks']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'], 1);
        
        $j++;        
        $context['sandbox']['file_position'] = ftell($rfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];          
    }
    fclose($rfile);  
        
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
//     $context['sandbox']['progress']++;
//  
//     $context['message'] = t('uwwtpsout_sheet15 finish');    
//     
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     }  
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }    
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_breach_list_sheet7($annee, &$context) 
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
    //all agglo NC on art3 or 4 or5
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    $induww = $GLOBALS['uwwtd']['sheet2_uwwtp'];
    $agglo_breach = array();
    
//     $i = 0;
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
               
    $ragglofile = fopen($filename, 'rb');
    
    // art3_count
    $art3_count = 0;
    // art3_generated_load
    $art3_generated_load = 0;
    // art3_collected_load
    $art3_collected_load = 0;
    // art4_count
    $art4_count = 0;
    // art4_generated_load
    $art4_generated_load = 0;
    // art4_collected_load
    $art4_collected_load = 0;
    // art5_count
    $art5_count = 0;
    // art5_generated_load
    $art5_generated_load = 0;
    // art5_collected_load            
    $art5_collected_load = 0;
    //ignore header
    $agglo = fgetcsv($ragglofile, 5000, ';');
    
    $i = 0;    
    while ($agglo = fgetcsv($ragglofile, 5000, ';')) {       
//     foreach ($listagglo as $agglo) {  
        if ($agglo[$ind['art3_compliance']] != 'NC'
            && $agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] != 'NC'
            && $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] != 'NC'
            ) {
            continue;
        }
        
        if ($agglo[$ind['art3_compliance']] == 'NC') {
            $art3_count          ++;
            $art3_generated_load += $agglo[$ind['generated_load_p.e.']];
            $art3_collected_load += $agglo[$ind['connected_to_collecting_system_p._e.']]; 
        }
        
        if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NC') {
            $art4_count          ++;
            $art4_generated_load += $agglo[$ind['generated_load_p.e.']];
            $art4_collected_load += $agglo[$ind['connected_to_collecting_system_p._e.']]; 
        }
        
        if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC') {
            $art5_count          ++;
            $art5_generated_load += $agglo[$ind['generated_load_p.e.']];
            $art5_collected_load += $agglo[$ind['connected_to_collecting_system_p._e.']]; 
        }                

        $agglo_breach[$i] = array(
            // Code
            'aggcode' => $agglo[$ind['aggcode']],
            // Name
            'aggname' => $agglo[$ind['aggname']],
            // NUTS 2 code
            'nuts_2_code' => $agglo[$ind['nuts_2_code']],
            // Generated load (p.e.)
            'generated_load_p.e.' => $agglo[$ind['generated_load_p.e.']],
            // collected load (p.e.)
            'connected_to_collecting_system_p._e.' => $agglo[$ind['connected_to_collecting_system_p._e.']],
            // Load addressed through IAS (p.e.)
            'addressed_through_ias_p._e.' => $agglo[$ind['addressed_through_ias_p._e.']],
            // neither collecting system nor IAS (p.e.)
            'neither_collecting_system_nor_ias_p._e.' => $agglo[$ind['neither_collecting_system_nor_ias_p._e.' ]],
            // entering load (p.e.)
            'total_entering_load_p._e.' => $agglo[$ind['total_entering_load_p._e.']],
            // design capacity (p.e.)
            'total_treatment_plant_design_capacity_p._e.' => $agglo[$ind['total_treatment_plant_design_capacity_p._e.']],
            // Compliance Article 3 NC = Non-Compliant
            'art3_compliance' => $agglo[$ind['art3_compliance']] == 'NC' ? 'NC' : '',
            // Compliance     Article 4 NC = Non-Compliant
            'art4_compliance_art._4_(hierarchical_approach)' => $agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NC' ? 'NC' : '',
            // Compliance     Article 5 NC = Non-Compliant
            'compliance_art._5_(whole_agglo)_(hierarchical_approach)' => $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC' ? 'NC' : '',
            // Type of Receiving Area NA/ SA/ CSA
            'type_of_receiving_water' => $agglo[$ind['type_of_receiving_water']],
        );
         
//         // art3_count
//         $agglo_breach[$i][] = $art3_count;
//         // art3_generated_load
//         $agglo_breach[$i][] = $art3_generated_load;
//         // art3_collected_load
//         $agglo_breach[$i][] = $art3_collected_load;
//         // art4_count
//         $agglo_breach[$i][] = $art4_count;
//         // art4_generated_load
//         $agglo_breach[$i][] = $art4_generated_load;
//         // art4_collected_load
//         $agglo_breach[$i][] = $art4_collected_load;
//         // art5_count
//         $agglo_breach[$i][] = $art5_count;
//         // art5_generated_load
//         $agglo_breach[$i][] = $art5_generated_load;
//         // art5_collected_load            
//         $agglo_breach[$i][] = $art5_collected_load;
        
        $i++;        
    }
    fclose($ragglofile); 
    
//     return $agglo_breach;
//     dsm($agglo_breach);

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = uwwtd_getregister_modelfile_path();
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['breach_list']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['breach_list']['xlsx_name'], $annee);
    
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         'breach_list',
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
    
    $sheet = $objPHPExcel->getSheetByName('breach_list');
    
    // art3_count
    $sheet->getCell('K4')->setValue($art3_count);
    // art3_generated_load
    $sheet->getCell('K5')->setValue($art3_generated_load);
    // art3_collected_load
    $sheet->getCell('K6')->setValue($art3_collected_load);
    // art4_count 
    $sheet->getCell('L4')->setValue($art4_count);
    // art4_generated_load
    $sheet->getCell('L5')->setValue($art4_generated_load);
    // art4_collected_load 
    $sheet->getCell('L6')->setValue($art4_collected_load);
    // art5_count
    $sheet->getCell('M4')->setValue($art5_count);
    // art5_generated_load
    $sheet->getCell('M5')->setValue($art5_generated_load);
    // art5_collected_load            
    $sheet->getCell('M6')->setValue($art5_collected_load);
        
    $start_line = 7;
    $sheet->insertNewRowBefore(8, count($agglo_breach));
    
    foreach($agglo_breach as $breach) {    
        $col = 1;
        // Code
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['aggcode']);
        // Name
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['aggname']);
        // NUTS 2 code
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['nuts_2_code']);
        // Generated load (p.e.)
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['generated_load_p.e.']);
        // collected load (p.e.)
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['connected_to_collecting_system_p._e.']);
        // Load addressed through IAS (p.e.)
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['addressed_through_ias_p._e.']);
        // neither collecting system nor IAS (p.e.)
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['neither_collecting_system_nor_ias_p._e.']);
        // entering load (p.e.)
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['total_entering_load_p._e.']);
        // design capacity (p.e.)
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['total_treatment_plant_design_capacity_p._e.']);
        // Compliance Article 3 NC = Non-Compliant
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['art3_compliance']);
        // Compliance     Article 4 NC = Non-Compliant
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['art4_compliance_art._4_(hierarchical_approach)']);
        // Compliance     Article 5 NC = Non-Compliant
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['compliance_art._5_(whole_agglo)_(hierarchical_approach)']);
        // Type of Receiving Area NA/ SA/ CSA
         $sheet->setCellValueByColumnAndRow($col++, $start_line, $breach['type_of_receiving_water']);        
        
        $start_line++;
    }    
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' .t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['breach_list']['xlsx_model_sheet_name']));    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }         
        
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_summary_installation_in_place_sheet6($annee, &$context) 
{
// 	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     } 
    

//     $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
    
    $ragglofile = fopen($filename, 'rb');
    
   if (!isset($context['sandbox']['progress'])) { 
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        
//         $context['results']['graph']['summary_installation']  = array();
        //ignore header    
        $agglo = fgetcsv($ragglofile, 5000, ';');
        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];    
        
        //MS total
        $context['sandbox']['summary']['total']['number'] = 0;
        $context['sandbox']['summary']['total']['percentage'] = 100;
        $context['sandbox']['summary']['total']['waste_pe'] = 0;
        $context['sandbox']['summary']['total']['waste_percent'] = 100;
        
        $context['sandbox']['summary']['total']['ias_in_place']['number'] = 0;
        $context['sandbox']['summary']['total']['ias_in_place']['percent'] = 0;
        $context['sandbox']['summary']['total']['ias_in_place']['waste_pe'] = 0;
        $context['sandbox']['summary']['total']['ias_in_place']['waste_percent'] = 0;    
        
        $context['sandbox']['summary']['total']['collecting_system']['number'] = 0;
        $context['sandbox']['summary']['total']['collecting_system']['percent'] = 0;
        $context['sandbox']['summary']['total']['collecting_system']['waste_pe'] = 0;
        $context['sandbox']['summary']['total']['collecting_system']['waste_percent'] = 0;
        //secondary treatment    
        $context['sandbox']['summary']['secondary']['installation_in_place']['number'] = 0;
        $context['sandbox']['summary']['secondary']['installation_in_place']['percent'] = 0;
        $context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe'] = 0;
        $context['sandbox']['summary']['secondary']['installation_in_place']['waste_percent'] = 0;
            
        $context['sandbox']['summary']['secondary']['meet_requirement']['number'] = 0;
        $context['sandbox']['summary']['secondary']['meet_requirement']['percent'] = 0;
        $context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe'] = 0;
        $context['sandbox']['summary']['secondary']['meet_requirement']['waste_percent'] = 0;    
        //more stringent treatment
        $context['sandbox']['summary']['more_stringent']['installation_in_place']['number'] = 0;
        $context['sandbox']['summary']['more_stringent']['installation_in_place']['percent'] = 0;
        $context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe'] = 0;
        $context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_percent'] = 0;
            
        $context['sandbox']['summary']['more_stringent']['meet_requirement']['number'] = 0;
        $context['sandbox']['summary']['more_stringent']['meet_requirement']['percent'] = 0;
        $context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe'] = 0;
        $context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_percent'] = 0;    
    }
    
    fseek($ragglofile, $context['sandbox']['file_position']);
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
               
//     $ragglofile = fopen($filename, 'rb');
    
//     $agglo = fgetcsv($ragglofile, 5000, ';');
    
    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {   
        
//     foreach ($listagglo as $agglo) {
    
        //MS total
        $context['sandbox']['summary']['total']['number']++;                
        $context['sandbox']['summary']['total']['waste_pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load
                                       
        if ($agglo[$ind['addressed_through_ias_p._e.']] > 0) {
            $context['sandbox']['summary']['total']['ias_in_place']['number']++;         
            $context['sandbox']['summary']['total']['ias_in_place']['waste_pe'] += $agglo[$ind['addressed_through_ias_p._e.']];  //addressed through IAS pe 
        }                                                     
        if ($agglo[$ind['connected_to_collecting_system_p._e.']] > 0) {                                  
            $context['sandbox']['summary']['total']['collecting_system']['number']++;       
            $context['sandbox']['summary']['total']['collecting_system']['waste_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']]; //connected to collecting system pe   
        }
        
        //secondary treatment
        //•	Number summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, secondary treatment installed, p.e.’, where parameter is  >0. (Excel-function: COUNTIF(‘p.e. for which installation for secondary treatment is in place’;”>0”)
        //•	p.e. summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, secondary treatment installed, p.e.’: sum of the entire column Excel-function: SUM (‘p.e. for which installation for secondary treatment is in place’)
        if ($agglo[$ind['secondary_treatment_installed_(p._e.)']] > 0) {
            $context['sandbox']['summary']['secondary']['installation_in_place']['number']++;
            $context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
        }
        
        //•	Number summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, monitoring results f. secondary OK, p.e.’, where parameter is  >0 (Excel-function: COUNTIF(‘p.e. for which monitoring results meet the requirements for discharge concerning secondary treatment’;”>0”)
        //•	p.e. summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, monitoring results f. secondary OK, p.e.’: sum of the entire column Excel-function: SUM (‘p.e. for which monitoring results meet the requirements for discharge concerning secondary treatment’)
        if ($agglo[$ind['monitoring_results_f._secondary_ok']] > 0) {
            $context['sandbox']['summary']['secondary']['meet_requirement']['number']++;
            $context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe'] += $agglo[$ind['monitoring_results_f._secondary_ok']];
        }        
        
        //more stringent treatment
        if ($agglo[$ind['more_str._treatment_installed_(p._e.)']] > 0) {
            $context['sandbox']['summary']['secondary']['installation_in_place']['number']++;
            $context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe'] += $agglo[$ind['more_str._treatment_installed_(p._e.)']];
        }

        if ($agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']] > 0) {
            $context['sandbox']['summary']['secondary']['meet_requirement']['number']++;
            $context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe'] += $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']];
        }
        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];                      
    }
    fclose($ragglofile);
    
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        //calculate percentage
        //MS total
        $context['sandbox']['summary']['total']['ias_in_place']['percent'] = $context['sandbox']['summary']['total']['ias_in_place']['number'] * 100 / $context['sandbox']['summary']['total']['number'];
        $context['sandbox']['summary']['total']['ias_in_place']['waste_percent'] = $context['sandbox']['summary']['total']['ias_in_place']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];    
    
        $context['sandbox']['summary']['total']['collecting_system']['percent'] = $context['sandbox']['summary']['total']['collecting_system']['number'] * 100 / $context['sandbox']['summary']['total']['number'];
        $context['sandbox']['summary']['total']['collecting_system']['waste_percent'] = $context['sandbox']['summary']['total']['collecting_system']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];    
        
        //secondary treatment    
        $context['sandbox']['summary']['secondary']['installation_in_place']['percent'] = $context['sandbox']['summary']['secondary']['installation_in_place']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        $context['sandbox']['summary']['secondary']['installation_in_place']['waste_percent'] = $context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
            
        $context['sandbox']['summary']['secondary']['meet_requirement']['percent'] = $context['sandbox']['summary']['secondary']['meet_requirement']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        $context['sandbox']['summary']['secondary']['meet_requirement']['waste_percent'] = $context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];    
        //more stringent treatment
        $context['sandbox']['summary']['more_stringent']['installation_in_place']['percent'] = $context['sandbox']['summary']['more_stringent']['installation_in_place']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        $context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_percent'] = $context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
            
        $context['sandbox']['summary']['more_stringent']['meet_requirement']['percent'] = $context['sandbox']['summary']['more_stringent']['meet_requirement']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        $context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_percent'] = $context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
        
    //     return $summary;
    //     dsm($summary);
        
    //     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
        
        $model_path = uwwtd_getregister_modelfile_path();
        //change to public://
    //     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_name'] .'.xlsx';
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_name'], $annee);
        
        $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
        $objReader->setLoadSheetsOnly( array(
                                             $GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_model_sheet_name'],
                                            ) );
        $objPHPExcel = $objReader->load(drupal_realpath($model_path));
        
        $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_model_sheet_name']);    
            
        
        //country name
        $path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
        include($path);
        $country_code = variable_get('siif_eru_country_code', '-1');
        $country_name = variable_get('siif_eru_country_name');
        $reference_date = $annee . '/12/31';   
        $sheet->setCellValue('E1', $country_name);
        unset($country_config);
        $sheet->setCellValue('E2', $reference_date);
                                      
        $cell = $sheet->getCell('B6');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('B8');  
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        
        
        $sheet->getCell('C10')->setValue($context['sandbox']['summary']['total']['number']);
        $sheet->getCell('D10')->setValue($context['sandbox']['summary']['total']['percentage']);
        $sheet->getCell('E10')->setValue($context['sandbox']['summary']['total']['waste_pe']);
        $sheet->getCell('F10')->setValue($context['sandbox']['summary']['total']['waste_percent']);
        
        $sheet->getCell('C11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['number']);
        $sheet->getCell('D11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['percent']);
        $sheet->getCell('E11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['waste_pe']);
        $sheet->getCell('F11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['waste_percent']);    
        
        $sheet->getCell('C12')->setValue($context['sandbox']['summary']['total']['collecting_system']['number']);
        $sheet->getCell('D12')->setValue($context['sandbox']['summary']['total']['collecting_system']['percent']);
        $sheet->getCell('E12')->setValue($context['sandbox']['summary']['total']['collecting_system']['waste_pe']);
        $sheet->getCell('F12')->setValue($context['sandbox']['summary']['total']['collecting_system']['waste_percent']);
        //secondary treatment    
        $sheet->getCell('C15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['number']);
        $sheet->getCell('D15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['percent']);
        $sheet->getCell('E15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe']);
        $sheet->getCell('F15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['waste_percent']);
            
        $sheet->getCell('C16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['number']);
        $sheet->getCell('D16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['percent']);
        $sheet->getCell('E16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe']);
        $sheet->getCell('F16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['waste_percent']);    
        //more stringent treatment
        $sheet->getCell('C19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['number']);
        $sheet->getCell('D19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['percent']);
        $sheet->getCell('E19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe']);
        $sheet->getCell('F19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_percent']);
            
        $sheet->getCell('C20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['number']);
        $sheet->getCell('D20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['percent']);
        $sheet->getCell('E20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe']);
        $sheet->getCell('F20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_percent']);
        
        $context['results']['graph']['summary_installation'] = $context['sandbox']['summary'];  
        
        //write
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
        $objWriter->setPreCalculateFormulas(false);
        //$objWriter->setOffice2003Compatibility(true);
        $objWriter->save(drupal_realpath($target_path));
        unset($objWriter);
        unset($objReader);
    }
    
//     $context['sandbox']['progress']++;
 
//     $context['message'] = t('summary_installation_in_place_sheet6 finish');    
//     
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     } 
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }               
}



/**
 *
 */ 
function uwwtd_create_register_2015_file_distance_to_compliance_sheet5($annee, &$context)
{
// 	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
        
   $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    
   $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
    
   $ragglofile = fopen($filename, 'rb');
   $listtypestat = array('expired', 'pending');
    
   if (!isset($context['sandbox']['progress'])) { 
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        
        $context['results']['distance_to_compliance'] = array();
        //ignore header    
        $agglo = fgetcsv($ragglofile, 5000, ';');
        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $listtypestattmp = $listtypestat;
        $listtypestattmp[] = 'total';  
        foreach($listtypestattmp as $typedeadline) {
            $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'] = 0;            
            $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'] = 0;
            $context['sandbox']['data'][$typedeadline]['target_connection']['ias'] = 0;
                    
            $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] = 0;
            $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'] = 0;
            $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'] = 0;
                          
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] = 0;
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'] = 0;
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'] = 0;        
        }
        unset($listtypestattmp);
    }
    
    fseek($ragglofile, $context['sandbox']['file_position']);
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
               
//     $ragglofile = fopen($filename, 'rb');
    
//     $agglo = fgetcsv($ragglofile, 5000, ';');
    
//     $data = array();
    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
        foreach($listtypestat as $typedeadline) {
//test        
// if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC', 'NC'))) {        
//     $context['sandbox']['data']['total']['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']];            
// }
// if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC'))) {
//     $context['sandbox']['data']['total']['target_connection']['collected_system_inplace'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
//     $context['sandbox']['data']['total']['target_connection']['ias'] += $agglo[$ind['addressed_through_ias_p._e.']];        
// }
// 
// if (true === in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('C', 'NC'))) {
//     $context['sandbox']['data']['total']['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
// }        
// if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {        
//     $context['sandbox']['data']['total']['secondary_target']['treatement_inplace'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
//     $context['sandbox']['data']['total']['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];              
// }
// 
// if (true === in_array($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']], array('C', 'NC'))) {
//     $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
// }
// 
// if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {
//     $context['sandbox']['data']['total']['morestringent_target']['installation_inplace'] += $agglo[$ind['more_str._treatment_installed_(p._e.)']];
//     $context['sandbox']['data']['total']['morestringent_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']];        
// } 
//test            
                    
            //chekc if expired or pending
            if ($typedeadline == 'expired' && $agglo[$ind['pending_or_expired_deadline_exp/pd/exp-pd']] != 'EXP' ) {
                continue;
            }
            if ($typedeadline == 'pending' && $agglo[$ind['pending_or_expired_deadline_exp/pd/exp-pd']] != 'PD') {
                continue;
            }
        
            if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC', 'NC'))) {        
                $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']];            
            }
            if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC'))) {
                $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                $context['sandbox']['data'][$typedeadline]['target_connection']['ias'] += $agglo[$ind['addressed_through_ias_p._e.']];        
            }
            
            if (true === in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('C', 'NC'))) {
                $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
            }        
            if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {        
                $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
                $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];              
            }
            
            if (true === in_array($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']], array('C', 'NC'))) {
                $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
            }
            
            if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {
                $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'] += $agglo[$ind['more_str._treatment_installed_(p._e.)']];
                $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']];        
            }            
        }
    
        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];   
    } 
    
    fclose($ragglofile);
    
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {    
    
        //calcul total
        foreach($listtypestat as $typedeadline) {            
            $context['sandbox']['data']['total']['target_connection']['total_load_pe'] += $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'];            
            $context['sandbox']['data']['total']['target_connection']['collected_system_inplace'] += $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'];
            $context['sandbox']['data']['total']['target_connection']['ias'] += $context['sandbox']['data'][$typedeadline]['target_connection']['ias'];
                    
            $context['sandbox']['data']['total']['secondary_target']['total_load_pe'] += $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'];
            $context['sandbox']['data']['total']['secondary_target']['treatement_inplace'] += $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'];
            $context['sandbox']['data']['total']['secondary_target']['monitoring_result'] += $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'];
                          
            $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] += $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'];
            $context['sandbox']['data']['total']['morestringent_target']['installation_inplace'] += $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'];
            $context['sandbox']['data']['total']['morestringent_target']['monitoring_result'] += $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'];     
        }
        //calcul % and distance
        $listtypestattmp = $listtypestat;
        $listtypestattmp[] = 'total';        
        foreach($listtypestattmp as $typedeadline) {            
            $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace_%'] =  $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'] * 100 / $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'];
            $context['sandbox']['data'][$typedeadline]['target_connection']['ias_%'] =  $context['sandbox']['data'][$typedeadline]['target_connection']['ias'] * 100 / $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'];
            
            $context['sandbox']['data'][$typedeadline]['target_connection']['distance_%']  =  $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace_%'] + $context['sandbox']['data'][$typedeadline]['target_connection']['ias_%'];
            $context['sandbox']['data'][$typedeadline]['target_connection']['distance_pe'] =  $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'] - ($context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'] + $context['sandbox']['data'][$typedeadline]['target_connection']['ias']);
        
            //more stringent % and distance
            $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_%'] = $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace']* 100 / $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'];
            $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'] = $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result']* 100 / $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'];

            $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_distance_pe'] = $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'];
            $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_distance_pe'] = $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'];
            
            $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_distance_%'] = 100 - $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_%'];
            $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_distance_%'] = 100 - $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'];
                        
            //more stringent % and distance                        
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_%'] = $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'] * 100 / $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'];
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_%'] = $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result']* 100 / $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'];                    

            $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_distance_pe'] = $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'];
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_distance_pe'] = $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'];
            
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_distance_%'] = 100 - $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_%'];
            $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_distance_%'] = 100 - $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_%'];            
        }
    
        //build excel sheet
        $model_path = uwwtd_getregister_modelfile_path();
        //change to public://
    //     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_name'] .'.xlsx';
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_name'], $annee);
        
        $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
        $objReader->setLoadSheetsOnly( array(
                                             $GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_model_sheet_name'],
                                            ) );
        $objPHPExcel = $objReader->load(drupal_realpath($model_path));
        
        $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_model_sheet_name']);
        
        //country name
        $path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
        include($path);
        $country_code = variable_get('siif_eru_country_code', '-1');
        $country_name = variable_get('siif_eru_country_name');
        $reference_date = $annee . '/12/31';
        $sheet->setCellValue('C2', $country_name);
        unset($country_config);
        $sheet->setCellValue('C3', $reference_date);
        
        $cell = $sheet->getCell('B10');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('B10');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('B30');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('B49');  
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        
        //expired 
        //target connection
        $sheet->getCell('C12')->setValue($context['sandbox']['data']['expired']['target_connection']['total_load_pe']);
        
        $sheet->getCell('E12')->setValue($context['sandbox']['data']['expired']['target_connection']['distance_%']);
        $sheet->getCell('F12')->setValue($context['sandbox']['data']['expired']['target_connection']['distance_pe']);            
                    
        $sheet->getCell('C13')->setValue($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace']);
        $sheet->getCell('D13')->setValue($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace_%']);
        $sheet->getCell('C14')->setValue($context['sandbox']['data']['expired']['target_connection']['ias']);
        $sheet->getCell('D14')->setValue($context['sandbox']['data']['expired']['target_connection']['ias_%']);
                                             
        //secondary treatement               
        $sheet->getCell('C16')->setValue($context['sandbox']['data']['expired']['secondary_target']['total_load_pe']);
        $sheet->getCell('C17')->setValue($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace']);
        $sheet->getCell('D17')->setValue($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_%']);
        $sheet->getCell('E17')->setValue($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_distance_%']);
        $sheet->getCell('F17')->setValue($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_distance_pe']);
        $sheet->getCell('C18')->setValue($context['sandbox']['data']['expired']['secondary_target']['monitoring_result']);
        $sheet->getCell('D18')->setValue($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_%']);
        $sheet->getCell('E18')->setValue($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_distance_%']);
        $sheet->getCell('F18')->setValue($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_distance_pe']);                
                                         
        //more stringent treatment
        $sheet->getCell('C20')->setValue($context['sandbox']['data']['expired']['morestringent_target']['total_load_pe']);
        $sheet->getCell('C21')->setValue($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace']);
        $sheet->getCell('D21')->setValue($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_%']);
        $sheet->getCell('E21')->setValue($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_distance_%']);
        $sheet->getCell('F21')->setValue($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_distance_pe']);
        $sheet->getCell('C22')->setValue($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result']);
        $sheet->getCell('D22')->setValue($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_%']);
        $sheet->getCell('E22')->setValue($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_%']);  
        $sheet->getCell('F22')->setValue($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_pe']);  
                                         
        //pending
        //target connection
        $sheet->getCell('C32')->setValue($context['sandbox']['data']['pending']['target_connection']['total_load_pe']);
        
        $sheet->getCell('E32')->setValue($context['sandbox']['data']['pending']['target_connection']['distance_%']);
        $sheet->getCell('F32')->setValue($context['sandbox']['data']['pending']['target_connection']['distance_pe']);
                
        $sheet->getCell('C33')->setValue($context['sandbox']['data']['pending']['target_connection']['collected_system_inplace']);
        $sheet->getCell('D33')->setValue($context['sandbox']['data']['pending']['target_connection']['collected_system_inplace_%']);
        $sheet->getCell('C34')->setValue($context['sandbox']['data']['pending']['target_connection']['ias']);
        $sheet->getCell('D34')->setValue($context['sandbox']['data']['pending']['target_connection']['ias_%']);
                                          
        //secondary treatement
        $sheet->getCell('C36')->setValue($context['sandbox']['data']['pending']['secondary_target']['total_load_pe']);
        $sheet->getCell('C37')->setValue($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace']);
        $sheet->getCell('D37')->setValue($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_%']);
        $sheet->getCell('E37')->setValue($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_distance_%']);
        $sheet->getCell('F37')->setValue($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_distance_pe']);
        $sheet->getCell('C38')->setValue($context['sandbox']['data']['pending']['secondary_target']['monitoring_result']);
        $sheet->getCell('D38')->setValue($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_%']);
        $sheet->getCell('E38')->setValue($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_distance_%']);
        $sheet->getCell('F38')->setValue($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_distance_pe']);                
        
        //more stringent treatment
        $sheet->getCell('C40')->setValue($context['sandbox']['data']['pending']['morestringent_target']['total_load_pe']);
        $sheet->getCell('C41')->setValue($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace']);
        $sheet->getCell('D41')->setValue($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_%']);
        $sheet->getCell('E41')->setValue($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_distance_%']);
        $sheet->getCell('F41')->setValue($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_distance_pe']);
        $sheet->getCell('C42')->setValue($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result']);
        $sheet->getCell('D42')->setValue($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_%']);
        $sheet->getCell('E42')->setValue($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_%']);
        $sheet->getCell('F42')->setValue($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_pe']); 
                                         
        //total
        //target connection
        $sheet->getCell('C51')->setValue($context['sandbox']['data']['total']['target_connection']['total_load_pe']);
        
        $sheet->getCell('E51')->setValue($context['sandbox']['data']['total']['target_connection']['distance_%']);
        $sheet->getCell('F51')->setValue($context['sandbox']['data']['total']['target_connection']['distance_pe']);
                
        $sheet->getCell('C52')->setValue($context['sandbox']['data']['total']['target_connection']['collected_system_inplace']);
        $sheet->getCell('D52')->setValue($context['sandbox']['data']['total']['target_connection']['collected_system_inplace_%']);
        $sheet->getCell('C53')->setValue($context['sandbox']['data']['total']['target_connection']['ias']);
        $sheet->getCell('D53')->setValue($context['sandbox']['data']['total']['target_connection']['ias_%']);
                                          
        //secondary treatement
        $sheet->getCell('C55')->setValue($context['sandbox']['data']['total']['secondary_target']['total_load_pe']);
        $sheet->getCell('C56')->setValue($context['sandbox']['data']['total']['secondary_target']['treatement_inplace']);
        $sheet->getCell('D56')->setValue($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_%']);
        $sheet->getCell('E56')->setValue($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_distance_%']);
        $sheet->getCell('F56')->setValue($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_distance_pe']);
        $sheet->getCell('C57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result']);
        $sheet->getCell('D57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result_%']);
        $sheet->getCell('E57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_%']);
        $sheet->getCell('F57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_pe']);                
        
        //more stringent treatment
        $sheet->getCell('C59')->setValue($context['sandbox']['data']['total']['morestringent_target']['total_load_pe']);
        $sheet->getCell('C60')->setValue($context['sandbox']['data']['total']['morestringent_target']['installation_inplace']);
        $sheet->getCell('D60')->setValue($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_%']);
        $sheet->getCell('E60')->setValue($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_distance_%']);
        $sheet->getCell('F60')->setValue($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_distance_pe']);
        $sheet->getCell('C61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result']);
        $sheet->getCell('D61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_%']);
        $sheet->getCell('E61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_%']);
        $sheet->getCell('F61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_pe']);
        
        $context['results']['distance_to_compliance'] = $context['sandbox']['data'];                          
                                         
        //write
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
        $objWriter->setPreCalculateFormulas(false);
        //$objWriter->setOffice2003Compatibility(true);
        $objWriter->save(drupal_realpath($target_path));
        unset($objWriter);
        unset($objReader);
    }
    
//     $context['sandbox']['progress']++;
 
//     $context['message'] = '('.$annee.') ' . t('distance_to_compliance_sheet5 finish');    
//     
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     }  
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_summary_legal_compliance_sheet4($annee, &$context) 
{
//     $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
//     
// 	if (!isset($context['sandbox']['progress'])) {    
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }  
    
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
    
    $ragglofile = fopen($filename, 'rb');
    $list_type = array('all', 'NA', 'CSA/SA');
    //pass header line
    if (!isset($context['sandbox']['progress'])) { 
        $context['sandbox']['file_position'] = 0;  
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        $context['sandbox']['max'] = filesize($filename);
        //ignore header    
        $agglo = fgetcsv($ragglofile, 5000, ';');
        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
    //     $context['sandbox']['line'] = 4; //start line
        
        foreach($list_type as $type) {
            //art3
            $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage'] = 0;    
        
            $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['percentage'] = 0; 
        
            $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['percentage'] = 0;     
            
            $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['percentage'] = 100;
            $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['percentage'] = 100;
            
            //art4
            $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['percentage'] = 0;
            
            $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['percentage'] = 0;
            
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['percentage'] = 0;

            $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['percentage'] = 0;
            
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['percentage'] = 0;            
            
            $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['percentage'] = 100;
            $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['percentage'] = 100; 
            
            //art5
            $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['percentage'] = 0; 
            

            $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['percentage'] = 0; 
            
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['percentage'] = 0; 
            
                                                                                    

            
            
            $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['percentage'] = 100;
            $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['percentage'] = 100;
        }   
            
    //     $path_xlsx = uwwtd_getregister_modelfile_path();
    //     $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);        
    } 
    
    fseek($ragglofile, $context['sandbox']['file_position']);
    
// else {        
// //         $path_xlsx = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'] .'.xlsx';
//     $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);
//     $target_path = $path_xlsx;          
// }
      
//     $header = array(
// 
// 
//     );
//     
//     $filename = $annee . '_summary_legal_compliance_sheet4.csv';
//     $tmpfilename = tempnam('public://tmp/', $filename);
//     $r = fopen($tmpfilename, 'wb');
//     fputcsv($r, $header, ';');
    
       
    
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
//     $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
               
//     $ragglofile = fopen($filename, 'rb');
    
    
    
//     dsm($compliance);
//     while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {         
//     foreach ($listagglo as $agglo) {
//         dsm($agglo);  
        foreach($list_type as $type) {
//             dsm($type);
            $listrcatype = explode(';', $agglo[$ind['type_of_receiving_water']]);
            if ($type == 'NA') {
                    //type of receiving water                                                    
                if (false === in_array('NA', $listrcatype)) {     
                    continue;
                }
            } elseif ($type == 'CSA/SA') {
                //todo A58 ?
                if (false === in_array('SA', $listrcatype) && false === in_array('CSA', $listrcatype)) {     
                    continue;
                }
            }
        
            //art 3 
//             dsm('art3'); 
//             dsm($ind['art3_compliance']);
                                //compliance art3   
            if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC'))) {    
                $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['number']++;      
                $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
    
                                              
                   
                $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage'] =
                         $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] * 100 
                            /  $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe'];
                       
                      
                $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe'] += $agglo[$ind['addressed_through_ias_p._e.']];
                $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['percentage']  =
                         $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe'] * 100 
                            /  $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe']; 
                       
                $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['percentage']   =
                         $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['pe'] * 100 
                            /  $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe'];
            }                                
                                  //compliance art3
            if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC', 'NC'))) {    
                $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number']++;              
                $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
            }  
                                  //compliance art3
            if (true === in_array($agglo[$ind['art3_compliance']], array('NR'))) {                
                $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['number']++;              
                $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
                
                
            } 
            
            $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number']++;            
            $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
           
            $NA = false !== (strpos($agglo[ $ind['type_of_receiving_water']], 'NA'));
            $CSA = false !== (strpos($agglo[ $ind['type_of_receiving_water']], 'CSA'));
            $LSA = false !== (strpos($agglo[ $ind['type_of_receiving_water']], 'LSA'));
            $SA = false !== (strpos($agglo[ $ind['type_of_receiving_water']], 'SA') && !$CSA && !$LSA);
            $A58 = false !== (strpos($agglo[ $ind['type_of_receiving_water']], 'A58'));
            $A54 = false !== (strpos($agglo[ $ind['type_of_receiving_water']], 'A54'));
                        
//             dsm('art4');
//             dsm($ind['art4_compliance_art._4_(hierarchical_approach)']);
            //art 4           
                //Compliance Art. 4 (hierarchical approach)
            if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {                                              
                $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['number']++;      
                $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
            }
            
            if (true === in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('C', 'NC'))) {      
                $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number']++;         
                $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
            }
            
            if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NR') {   
                $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['number']++;    
                $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
    
                //            - due to a size of 2000p.e. - 10000 p.e.     …..and discharge into coastal water            
                if ($agglo[$ind['generated_load_p.e.']] > 2000 && $agglo[$ind['generated_load_p.e.']] < 10000
                    && $agglo[$ind['entirely_discharging_into_coastal_waters?']] == 'yes') {   
                    $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                }
    //            - due to 0% collection in collecting …...system
                if ($agglo[ $ind['connected_to_collecting_system_p._e.']] == 0) {   
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                }    
    //            - due to discharge into LSA
                $listrcatype = explode(';', $agglo[ $ind['type_of_receiving_water']]);
                if ($LSA && !$SA && !$CSA && !$A58) {   
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                } 
                unset($listrcatype);    
    //            - due to transition period 
                if ($annee < substr($agglo[ $ind['deadline_for_art._4_compliance']], 0, 4)) {  
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.                
                }      
            }
            
            $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number']++;    
            $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
            
            //art 5 
//             dsm('art5');
//             dsm($ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']);       
                //Compliance Art. 5 (whole agglo) (hierarchical approach)
            if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {                                           
                $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['number']++;      
                $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];  //connected to collecting system p. e.
            }
            
            if (true === in_array($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']], array('C', 'NC'))) {      
                $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number']++;      
                $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
            }
            
            if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NR') {   
                $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['number']++;    
                $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                
                //- due to a size of ≤ 10000 p.e.     …
                if ($agglo[$ind['generated_load_p.e.']] > 2000 && $agglo[$ind['generated_load_p.e.']] < 10000) {
                    $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                }
                
                //   - size > 10000 p.e., but discharge    …  …..into NA   
                if ($agglo[$ind['generated_load_p.e.']] > 10000 
                    && $NA && !$LSA && !$SA && !$CSA && !$A58) {
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                }
                
                //   - size > 10000 p.e. and discharge into …..CSA designated for criterion c
                if ($agglo[$ind['generated_load_p.e.']] > 10000 
                    && $CSA) {                
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                }
                
                //   - size > 10000 p.e., but discharge into …...Art. 5(4)- area
                if ($agglo[$ind['generated_load_p.e.']] > 10000 
                    && $A54) {                  
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                }
                
                //   - due to 0% collection in collecting …...system
                if ($agglo[ $ind['connected_to_collecting_system_p._e.']] == 0) {
                    $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                }
                
                //   - size > 10000 p.e.  and discharge into   …...LSA
                if ($agglo[$ind['generated_load_p.e.']] > 10000 
                    && !$LSA && !$SA && !$CSA && !$A58) {            
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                }
                
                //   - size > 10000 p.e. and discharge into …..SA/CSA, but transition period pending
                if ($agglo[$ind['generated_load_p.e.']] > 10000 
                    && ($SA  || $CSA)) {                 
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['number']++;
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                }
            }
            
            $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number']++;    
            $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
        } 
        $j++;        
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];                 
    }
//     dsm($compliance);
    fclose($ragglofile); 
    
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        foreach($list_type as $type) {
            if (false === isset($context['sandbox']['compliance'][$type])) {
                continue;
            }
            //calcul percentage art3
            if ($context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'] != 0) {
                $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'];
            }
            if ($context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'] != 0) {
                $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'];
            } 
            
            //calcul percentage art4
            if ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] != 0) {
                $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                
                $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
            }
            if ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] != 0) {
                $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                
                $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe']; 
            }
            
            //calcul percentage art5
            if ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] != 0) {
                $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                
                $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
            }
            if ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] != 0) {
                $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
            
                $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['size_<=_10000']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['percentage'] = $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe']; 
            }
        }
    
//     dsm($compliance);

//     dsm($compliance);
//     return $compliance;


//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
        $model_path = uwwtd_getregister_modelfile_path();
        //change to public://
    //     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_name'] .'.xlsx';
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_name'], $annee);
        
        $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
        $objReader->setLoadSheetsOnly( array(
                                             $GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_model_sheet_name'],
                                            ) );
        $objPHPExcel = $objReader->load(drupal_realpath($model_path));
        
        $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_model_sheet_name']);
        
        //country name
        $path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
        include($path);
        $country_code = variable_get('siif_eru_country_code', '-1');
        $country_name = variable_get('siif_eru_country_name');
        $reference_date = $annee . '/12/31';    
        $sheet->setCellValue('C1', $country_name);
        unset($country_config);
        $sheet->setCellValue('C2', $reference_date);
        
        $cell = $sheet->getCell('B7');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('B9');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('J7');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('J9');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('Q7');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
        $cell = $sheet->getCell('Q9');
        $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
    
        $cell = $sheet->getCell('B18');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue()));
        $cell = $sheet->getCell('J18');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue()));
        $cell = $sheet->getCell('Q18');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue())); 
        
        $cell = $sheet->getCell('B28');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue()));
        $cell = $sheet->getCell('J28');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue()));
        $cell = $sheet->getCell('Q28');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue())); 
        
            
        $cell = $sheet->getCell('B41');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue()));
        $cell = $sheet->getCell('J41');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue()));
        $cell = $sheet->getCell('Q41');
        $cell->setValue(str_replace(array('[#COUNTRY#]'), array($country_name), $cell->getValue()));                    
    
        foreach($list_type as $type) {
    //     foreach(array('all') as $type) {
            if (false === isset($context['sandbox']['compliance'][$type])) {
                continue;
            }
            if ($type == 'all') {
                $col['C'] = 'C';
                $col['D'] = 'D';
                $col['E'] = 'E';
                $col['F'] = 'F';
            } elseif  ($type == 'NA') {
                $col['C'] = 'K';
                $col['D'] = 'L';
                $col['E'] = 'M';
                $col['F'] = 'N';
            }else {
                $col['C'] = 'R';
                $col['D'] = 'S';
                $col['E'] = 'T';
                $col['F'] = 'U';        
            }
                    
            $sheet->getCell($col['E'].'12')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe']);
            $sheet->getCell($col['F'].'12')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage']);
            $sheet->getCell($col['E'].'13')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe']);
            $sheet->getCell($col['F'].'13')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['percentage']);
            $sheet->getCell($col['E'].'14')->setValue($context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['pe']);
            $sheet->getCell($col['F'].'14')->setValue($context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['percentage']);
               
            $sheet->getCell($col['C'].'15')->setValue($context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['number']);
            $sheet->getCell($col['D'].'15')->setValue($context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['percentage']);
            $sheet->getCell($col['E'].'15')->setValue($context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe']);
            $sheet->getCell($col['F'].'15')->setValue($context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'16')->setValue($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number']);
            $sheet->getCell($col['D'].'16')->setValue($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['percentage']);
            $sheet->getCell($col['E'].'16')->setValue($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe']);
            $sheet->getCell($col['F'].'16')->setValue($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'17')->setValue($context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['number']);
            $sheet->getCell($col['D'].'17')->setValue($context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['percentage']);
            $sheet->getCell($col['E'].'17')->setValue($context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['pe']);
            $sheet->getCell($col['F'].'17')->setValue($context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'18')->setValue($context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number']);
            $sheet->getCell($col['D'].'18')->setValue($context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['percentage']);
            $sheet->getCell($col['E'].'18')->setValue($context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe']);
            $sheet->getCell($col['F'].'18')->setValue($context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['percentage']);
            
            //art4
            $sheet->getCell($col['C'].'21')->setValue($context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['number']);
            $sheet->getCell($col['D'].'21')->setValue($context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['percentage']);
            $sheet->getCell($col['E'].'21')->setValue($context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['pe']);
            $sheet->getCell($col['F'].'21')->setValue($context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['percentage']); 
            
            $sheet->getCell($col['C'].'22')->setValue($context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number']);
            $sheet->getCell($col['D'].'22')->setValue($context['sandbox']['compliance'][$type]['art4']['target']['agglo']['percentage']);
            $sheet->getCell($col['E'].'22')->setValue($context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe']);
            $sheet->getCell($col['F'].'22')->setValue($context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['percentage']); 
            
            $sheet->getCell($col['C'].'23')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['number']);
            $sheet->getCell($col['D'].'23')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['percentage']);
            $sheet->getCell($col['E'].'23')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['pe']);
            $sheet->getCell($col['F'].'23')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'24')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['number']);
            $sheet->getCell($col['D'].'24')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['percentage']);
            $sheet->getCell($col['E'].'24')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['pe']);
            $sheet->getCell($col['F'].'24')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'25')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['number']);
            $sheet->getCell($col['D'].'25')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['percentage']);
            $sheet->getCell($col['E'].'25')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['pe']);
            $sheet->getCell($col['F'].'25')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'26')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['number']);
            $sheet->getCell($col['D'].'26')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['percentage']);
            $sheet->getCell($col['E'].'26')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['pe']);
            $sheet->getCell($col['F'].'26')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['percentage']);

            $sheet->getCell($col['C'].'27')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['number']);
            $sheet->getCell($col['D'].'27')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['percentage']);
            $sheet->getCell($col['E'].'27')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['pe']);
            $sheet->getCell($col['F'].'27')->setValue($context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['percentage']);                                                 
            
            $sheet->getCell($col['C'].'28')->setValue($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number']);
            $sheet->getCell($col['D'].'28')->setValue($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['percentage']);
            $sheet->getCell($col['E'].'28')->setValue($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe']);
            $sheet->getCell($col['F'].'28')->setValue($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['percentage']);
            
            //art5
            $sheet->getCell($col['C'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['number']);
            $sheet->getCell($col['D'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['percentage']);
            $sheet->getCell($col['E'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['pe']);
            $sheet->getCell($col['F'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['percentage']); 
            
            $sheet->getCell($col['C'].'32')->setValue($context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number']);
            $sheet->getCell($col['D'].'32')->setValue($context['sandbox']['compliance'][$type]['art5']['target']['agglo']['percentage']);
            $sheet->getCell($col['E'].'32')->setValue($context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe']);
            $sheet->getCell($col['F'].'32')->setValue($context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['percentage']); 
            
            $sheet->getCell($col['C'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['number']);
            $sheet->getCell($col['D'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['percentage']);
            $sheet->getCell($col['E'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe']);
            $sheet->getCell($col['F'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['percentage']);


            
            $sheet->getCell($col['C'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['number']);
            $sheet->getCell($col['D'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['percentage']);
            $sheet->getCell($col['E'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe']);
            $sheet->getCell($col['F'].'33')->setValue($context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['percentage']);             

            $sheet->getCell($col['C'].'34')->setValue($context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['number']);
            $sheet->getCell($col['D'].'34')->setValue($context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['percentage']);
            $sheet->getCell($col['E'].'34')->setValue($context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['pe']);
            $sheet->getCell($col['F'].'34')->setValue($context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'35')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['number']);
            $sheet->getCell($col['D'].'35')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['percentage']);
            $sheet->getCell($col['E'].'35')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['pe']);
            $sheet->getCell($col['F'].'35')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['percentage']);
            
//             $sheet->getCell($col['C'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['number']);
//             $sheet->getCell($col['D'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['percentage']);
//             $sheet->getCell($col['E'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe']);
//             $sheet->getCell($col['F'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'36')->setValue('TODO');
            $sheet->getCell($col['D'].'36')->setValue('TODO');
            $sheet->getCell($col['E'].'36')->setValue('TODO');
            $sheet->getCell($col['F'].'36')->setValue('TODO');            
            
//             $sheet->getCell($col['C'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['number']);
//             $sheet->getCell($col['D'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['percentage']);
//             $sheet->getCell($col['E'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe']);
//             $sheet->getCell($col['F'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['percentage']);

            $sheet->getCell($col['C'].'37')->setValue('TODO');
            $sheet->getCell($col['D'].'37')->setValue('TODO');
            $sheet->getCell($col['E'].'37')->setValue('TODO');
            $sheet->getCell($col['F'].'37')->setValue('TODO');

            
            $sheet->getCell($col['C'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['number']);
            $sheet->getCell($col['D'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['percentage']);
            $sheet->getCell($col['E'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe']);
            $sheet->getCell($col['F'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['percentage']);
            
            $sheet->getCell($col['C'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['number']);
            $sheet->getCell($col['D'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['percentage']);
            $sheet->getCell($col['E'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['pe']);
            $sheet->getCell($col['F'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['percentage']);
            
//             $sheet->getCell($col['C'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['number']);
//             $sheet->getCell($col['D'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['percentage']);
//             $sheet->getCell($col['E'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe']);
//             $sheet->getCell($col['F'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['percentage']);                                                                        

            $sheet->getCell($col['C'].'40')->setValue('TODO (A58 ?, pending ?)');
            $sheet->getCell($col['D'].'40')->setValue('TODO (A58 ?, pending ?)');
            $sheet->getCell($col['E'].'40')->setValue('TODO (A58 ?, pending ?)');
            $sheet->getCell($col['F'].'40')->setValue('TODO (A58 ?, pending ?)');                                                                        

            
            $sheet->getCell($col['C'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number']);
            $sheet->getCell($col['D'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['percentage']);
            $sheet->getCell($col['E'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe']);
            $sheet->getCell($col['F'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['percentage']);      
                            
    //         break;       
            unset($context['sandbox']['compliance'][$type]);
        }
        unset($context['sandbox']['compliance']);
       
        //write
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
        $objWriter->setPreCalculateFormulas(false);
        //$objWriter->setOffice2003Compatibility(true);
        $objWriter->save(drupal_realpath($target_path));
        unset($objWriter);
        unset($objReader);    
    }
    
//     $context['sandbox']['progress']++;
 
//     $context['message'] = t('file_summary_legal_compliance_sheet4 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     } 

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }                                             
}

/**
 *
 */ 
function uwwtd_create_register_2015_agglo_sheet3($annee, &$context) 
{
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    $induww = $GLOBALS['uwwtd']['sheet2_uwwtp'];
    $listtypestat = array('expired', 'pending');
    $list_type_rca = array('all', 'na', 'sa_csa');    
	if (!isset($context['sandbox']['progress'])) {
    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['id'] = array();
            
        $query = 'select n.nid
        from {node} n    
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id                                                                                                                                                                                                                                                                                                                                                                                                                                
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
        ';
        
//         dsm($query);
        $param = array(
            ':typenode' => 'agglomeration',
            ':annee' => $annee,
            );
//         dsm($param);
        $listagg = array();    
        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['id'][] = $row['nid'];
            }
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
        
//         dsm($listagg);
            	
		$context['sandbox']['max'] = count($context['sandbox']['id']);
// 		dsm($context['sandbox']['max']);

        $context['sandbox']['previous_year'] = uwwtd_get_previous_year($annee);

        //prepare data from uwwtp
        //todo to move outside the loop, check memory
//         $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//         $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] .'.csv';
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'], $annee);
                   
        $ruwwtpfile = fopen($filename, 'rb');
        $context['sandbox']['uwwtpinfo_for_agglo'] = array();
//         dsm($filename);
//         foreach ($listuwwtp as $uwwtp) {
        $uwwtp = fgetcsv($ruwwtpfile, 5000, ';');
        
        while ($uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {
//             dsm($uwwtp);
//             break;
//             $uww_aggcode = $uwwtp[$induww['aggcode']];
//             if ($uww_aggcode == $agg->field_inspireidlocalid['und'][0]['value']) {
            $uww_aggcode = $uwwtp[$induww['aggcode']];
//             dsm($uww_aggcode);
            if (false === isset($context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode])) {
//                  dsm('la');
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['capacity'] = 0;
                                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['pe']       = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['percent']  = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['pe']      = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['pe']      = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['pe']      = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['pe']      = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['pe']      = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['percent'] = 0;
                                                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_installed'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_pe'] = 0;
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_passed'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_pe'] = 0;    
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_installed'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_pe'] = 0;
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_passed'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_percent'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_pe'] = 0;
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['uwwtp_collecting_without_treatment'] = 0;
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring'] = 0;
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring_%'] = 0;
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_C_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_C_%'] = 0;                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NR_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NR_%'] = 0; 
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_%'] = 0;                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_%'] = 0; 
                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_%'] = 0;                
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_%'] = 0;                                                                                                       
            } 
//             else {
//                 $context['sandbox']['uwwtpinfo_for_agglo'] &= $context['sandbox']['uwwtpinfo_for_agglo'];
//             } 
//             dsm($context['sandbox']['uwwtpinfo_for_agglo']);    
            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'] += $uwwtp[$induww['uww_entering load  p. e.']];
            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['capacity'] += $uwwtp[$induww['uww_capacity p. e.']];                          
                        
            if ($uwwtp[$induww['art4_final']] == 'C') {          
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['pe']  += $uwwtp[$induww['uww_entering load  p. e.']];     
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['percent']  = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }
            if ($uwwtp[$induww['art4_final']] == 'NC') {              
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['pe']      += $uwwtp[$induww['uww_entering load  p. e.']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['percent']  = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }
            if ($uwwtp[$induww['art4_final']] == 'NR') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['pe']      += $uwwtp[$induww['uww_entering load  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['percent']  = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }
            
            if ($uwwtp[$induww['art4_final']] == 'NI') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['pe']      += $uwwtp[$induww['uww_entering load  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['percent']  = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }
            
            if ($uwwtp[$induww['art4_final']] == 'PD') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['pe']      += $uwwtp[$induww['uww_entering load  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['percent']  = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }
            
            if ($uwwtp[$induww['art4_final']] == '?') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['pe']      += $uwwtp[$induww['uww_entering load  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['percent']  = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }                                    
            
            //Secondary: installed?
            if ($uwwtp[$induww['Secondary: installed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_installed'] = 1;  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_percent'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];                                     
            }   
            
            if ($uwwtp[$induww['Secondary:  monitoring passed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_passed'] = 1;  
                 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_percent'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];                  
            }                               
            
            //tertiary install
            if ($uwwtp[$induww['More stringent:  installed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_installed'] = 1;  
                 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_percent'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];         
            }   
            
            if ($uwwtp[$induww['More stringent: monitoring passed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_passed'] = 1;  
                 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_percent'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_pe'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];                   
            }
            
            if ($uwwtp[$induww['uwwtp_collecting_without_treatment']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['uwwtp_collecting_without_treatment']++;
            }   
            
            if ($uwwtp[$induww['art4_treatment']] == 'NC') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment']   += $uwwtp[$induww['uww_entering load  p. e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment_%'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }
            if ($uwwtp[$induww['art4_monitoring']] == 'NC') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring']   += $uwwtp[$induww['uww_entering load  p. e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring_%'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            } 
            if ($uwwtp[$induww['art5_treatment']] == 'NC') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment']   += $uwwtp[$induww['uww_entering load  p. e.']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment_%'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }
            if ($uwwtp[$induww['art5_monitoring']] == 'NC') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring']   += $uwwtp[$induww['uww_entering load  p. e.']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring_%'] = 
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring'] * 100 / $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'];
            }  
            
//             if ($uwwtp[$induww['required_rcaaP']] == 1 || $uwwtp[$induww['required_rcaaN']] == 1) {
//                 if () {
//                     $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_C_pe'] = 0;
//                     $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_C_%'] = 0;
//                 }                
//                 $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_pe'] = 0;
//                 $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_%'] = 0;
//                 $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NR_pe'] = 0;
//                 $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NR_%'] = 0; 
//             }
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_pe'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_%'] = 0;                
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_pe'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_%'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_pe'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_%'] = 0; 
//             
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_pe'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_%'] = 0;                
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_pe'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_%'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_pe'] = 0;
//             $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_%'] = 0;                        
                                                
//             }
        }
        fclose($ruwwtpfile);
//         dsm($context['sandbox']['uwwtpinfo_for_agglo']);          
	}
    
//     dsm($context['sandbox']['uwwtpinfo_for_agglo']);
//     return '';
    
   
    //get all uwwtp (repeat agglo info if one agglo has more than one uwwtp)

//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] .'.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
                 
    
    if ($context['sandbox']['progress'] == 0) {
        $r = fopen($filename, 'wb');  
        $header = array(
            //Agglomeration info',
            $ind['aggcode'] => 'code',
            $ind['aggname'] => 'name',
            $ind['generated_load_p.e.'] => 'generated load p.e.',
            $ind['deadline_for_art._3_compliance'] => 'deadline for Art. 3 compliance',
            $ind['deadline_for_art._4_compliance'] => 'deadline for Art. 4 compliance',
            $ind['deadline_for_art._5_compliance'] => 'deadline for Art. 5 compliance',
            $ind['pending_or_expired_deadline_exp/pd/exp-pd'] => 'Pending or expired deadline EXP/PD/EXP-PD',
            $ind['nuts_1_code'] => 'NUTS 1 code',
            $ind['nuts_2_code'] => 'NUTS 2 code',
            $ind['nuts_3_code'] => 'NUTS 3 code',
            //big_city_info'] => //Big City info',
            $ind['bccode'] => 'code',
            $ind['bcname'] => 'name',
            //?'] => //?',
            $ind['number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration'] => 'Number of UWWTPs/ Collecting systems without treatment serving the agglomeration',
            //previous_agglomeration_info'] => //Previous agglomeration info',
            $ind['previous_reported_?'] => 'reported ?',
            $ind['previous_aggcode'] => 'code',
            $ind['previous_gen._load_p.e.'] => 'gen. load p.e.',
            $ind['previous_percentage_of_change_of_size_(1)'] => 'Percentage of change of size (1)',
            //connection'] => //Connection',
            $ind['total_%_(2)'] => 'Total % (2)',
            $ind['connected_to_collecting_system'] => 'connected to collecting system',
            $ind['connected_to_collecting_system_p._e.'] => 'connected to collecting system p. e.',
            $ind['addressed_through_ias_%_of_gen._load'] => 'addressed through IAS % of gen. load',
            $ind['addressed_through_ias_p._e.'] => 'addressed through IAS p. e.',
            $ind['neither_collecting_system_nor_ias_%_of_gen._load'] => 'neither collecting system nor IAS % of gen. load',
            $ind['neither_collecting_system_nor_ias_p._e.'] => 'neither collecting system nor IAS p. e.',
            //distance_to_compliance_art._3'] => //Distance to compliance Art. 3',
            $ind['dist_art3_%_of_gen._load_(3)'] => '% of gen. Load (3)',
            $ind['dist_art3_in_p.e.'] => 'in p.e.',
            //compliance_art._3'] => //Compliance Art. 3',
            $ind['art3_compliance'] => 'Compliance',
            //compliance_art._3_previous_reporting'] => //Compliance Art. 3 previous reporting',
            $ind['previous_art3_compliance'] => 'Compliance',
            //ratio_connection/collection/treatment'] => //Ratio connection/collection/treatment',
            $ind['total_entering_load_p._e.'] => 'Total entering load p. e.',
            $ind['ratio_connected/enentering_%'] => 'ratio connected/enentering %',
            $ind['total_treatment_plant_design_capacity_p._e.'] => 'Total treatment plant design capacity  p. e.',
            $ind['ratio_total_entering_load/total_design_capacity_%'] => 'ratio Total entering load/Total design capacity %',
            //compliance_art._4'] => //Compliance Art. 4',
            $ind['art4_compliant_%_of_gen._load'] => 'compliant % of gen. load',
            $ind['art4_compliant_p._e.'] => 'compliant p. e.',
            $ind['art4_not_compliant_%_of_gen._load'] => 'not compliant % of gen. load',
            $ind['art4_not_compliant'] => 'not compliant',
            $ind['art4_not_relevant_%_of_gen._load'] => 'not relevant % of gen. load',
            $ind['art4_not_relevant'] => 'not relevant',
            $ind['art4_compliance_art._4_(only)'] => 'Compliance Art. 4 (only)',
            $ind['art4_compliance_art._4_(hierarchical_approach)'] => 'Compliance Art. 4 (hierarchical approach)',
            //compliance_art._4_previous_reporting'] => //Compliance Art. 4 previous reporting',
            $ind['previous_art4_compliance'] => 'Compliance',
            //distance_to_compliance_art._4'] => //Distance to compliance art. 4',
            //treatment'] => //Treatment',
            $ind['dist_art4_treat__%_of_connected_load'] => '% of connected Load',
            $ind['dist_art4_treat_in_p.e._(4)'] => 'in p.e. (4)',
            //performance'] => //Performance',
            $ind['dist_art4_perf_%_of_connected_load'] => '% of connected Load',
            $ind['dist_art4_perf_in_p.e._(5)'] => 'in p.e. (5)',
            //compliance_art._5'] => //Compliance Art. 5',
            $ind['compliant_5_a_(%_of_gen._load)'] => 'compliant 5 a  (% of gen. load)',
            $ind['compliant_5_a_(p._e.)'] => 'compliant 5 a  (p. e.)',
            $ind['not_compliant_5_a_(%_of_gen._load)'] => 'not compliant 5 a  (% of gen. load)',
            $ind['not_compliant_5_a_(p._e.)'] => 'not compliant 5 a  (p. e.)',
            $ind['not_relevant_5_a_(%_of_gen._load)'] => 'not relevant 5 a   (% of gen. load)',
            $ind['not_relevant_5_a_(p._e.)'] => 'not relevant 5 a   (p. e.)',
            $ind['compliance_art._5a_(only)'] => 'Compliance Art. 5a (only)',
            $ind['compliance_art._5a_(hierarchical_approach)'] => 'Compliance Art. 5a (hierarchical approach)',
            $ind['compliant_5_b_(%_of_gen._load)'] => 'compliant 5 b    (% of gen. load)',
            $ind['compliant_5_b_(p._e.)'] => 'compliant 5 b    (p. e.)',
            $ind['not_compliant_5_b_(%_of_gen._load)'] => 'not compliant 5 b  (% of gen. load)',
            $ind['not_compliant_5_b_(p._e.)'] => 'not compliant 5 b   (p. e.)',
            $ind['not_relevant_5_b_(%_of_gen._load)'] => 'not relevant 5 b   (% of gen. load)',
            $ind['not_relevant_5_b_(p._e.)'] => 'not relevant 5 b   (p. e.)',
            $ind['compliance_art._5b_(only)'] => 'Compliance Art. 5b (only)',
            $ind['compliance_art._5b_(hierarchical_approach)'] => 'Compliance Art. 5b (hierarchical approach)',
            $ind['compliant_5_c_(%_of_gen._load)'] => 'compliant 5 c      (% of gen. load)',
            $ind['compliant_5_c__(p._e.)'] => 'compliant 5 c       (p. e.)',
            $ind['not_compliant_5_c_(%_of_gen._load)'] => 'not compliant 5 c   (% of gen. load)',
            $ind['not_compliant_5_c_(p._e.)'] => 'not compliant 5 c   (p. e.)',
            $ind['not_relevant_5_c_(%_of_gen._load)'] => 'not relevant 5 c   (% of gen. load)',
            $ind['not_relevant_5_c_(p._e.)'] => 'not relevant 5 c  (p. e.)',
            $ind['compliance_art._5c_(only)'] => 'Compliance Art. 5c (only)',
            $ind['compliance_art._5c_(hierarchical_approach)'] => 'Compliance Art. 5c (hierarchical approach)',
            $ind['compliant_art._5_(p._e.)'] => 'compliant Art. 5 (p. e.)',
            $ind['not_compliant_art._5_(p._e.)'] => 'not compliant Art. 5 (p. e.)',
            $ind['art._5_not_relevant_(p._e.)'] => 'Art. 5 not relevant  (p. e.)',
            $ind['compliance_art._5_(whole_agglo)'] => 'Compliance Art. 5 (whole agglo)',
            $ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)'] => 'Compliance Art. 5  (whole agglo) (hierarchical approach)',
            //compliance_art._5_previous_reporting'] => //Compliance Art. 5 previous reporting',
            $ind['previous_art5_compliance'] => 'Compliance',
            //distance_to_compliance_art._5'] => //Distance to compliance art. 5',
            //treatment'] => //Treatment',
            $ind['distance_art5_treat_%_of_gen._load'] => '% of gen. Load',
            $ind['distance_art5_treat_in_p.e._(6)'] => 'in p.e. (6)',
            //performance'] => //Performance',
            $ind['distance_art5_perf_%_of_gen._load'] => '% of gen. Load',
            $ind['distance_art5_perf_in_p.e._(7)'] => 'in p.e. (7)',
            //current_rca_details'] => //current RCA details',
            $ind['water_body_type'] => 'water body type',
            $ind['id_code_of_the_water_body_wfd'] => 'Id code of the water body WFD',
            $ind['id_of_wfd_river_basin_district'] => 'ID of WFD river basin district',
            $ind['entirely_discharging_into_coastal_waters'] => 'entirely discharging into coastal waters?',
            $ind['type_of_receiving_water'] => 'type of receiving water',
            $ind['id_of_the_sa_or_lsa'] => 'ID of the SA or LSA',
            $ind['rca_name'] => 'name',
            $ind['starting_date_of_application_5.8.'] => 'Starting date of application 5.8.',
            $ind['starting_date_of_application_5.4.'] => 'Starting date of application 5.4.',
            $ind['starting_date_of_application_a_n'] => 'Starting date of application a N',
            $ind['starting_date_of_application_a_p'] => 'Starting date of application a P',
            $ind['starting_date_of_application_b'] => 'Starting date of application b',
            $ind['starting_date_of_application_c'] => 'Starting date of application c',
            //installations_for_wastewater_treatment_in_place'] => //Installations for wastewater treatment in place',
            $ind['secondary_treatment_installed_(%_of_gen._load)'] => 'secondary treatment installed (% of gen. load)',
            $ind['secondary_treatment_installed_(p._e.)'] => 'secondary treatment installed (p. e.)',
            $ind['monitoring_results_f._secondary_ok_(%_of_gen._load)'] => 'monitoring results f. secondary OK (% of gen. load)',
            $ind['monitoring_results_f._secondary_ok'] => 'monitoring results f. secondary OK',
            $ind['more_str._treatment_installed_(%_of_gen._load)'] => 'more str. treatment installed (%_of_gen._load)',
            $ind['more_str._treatment_installed_(p._e.)'] => 'more str. treatment installed (p. e.)',
            $ind['monitoring_results_f._more_string._ok_(%_of_gen._load)'] => 'monitoring results f. more string. OK (% of gen. load)',
            $ind['monitoring_results_f._more_string._ok_(p._e.)'] => 'monitoring results f. more string. OK (p. e.)',
            
            //debug value
            $ind['debug_compliance_agglo_from_algo'] => 'debug_compliance_agglo_from_algo',  
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');  
    }
    

//     $allline = array();
//     foreach($listagg as $nidagg) {
    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {    
        $line = array();
//         $agg = node_load($nidagg);
//         dsm($context['sandbox']['id'][$context['sandbox']['progress'] + $j]);
//         dsm($context['sandbox']['id']);
//         break;
        $agg = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
        
        $uwwtpinfo_for_agglo = &$context['sandbox']['uwwtpinfo_for_agglo'][$agg->field_inspireidlocalid['und'][0]['value']];
//         dsm($uwwtpinfo_for_agglo);
//         dsm($uwwtpinfo_for_agglo);
//         break;
        //Agglomeration info',
        //'code',
        $line[$ind['aggcode']] = $agg->field_inspireidlocalid['und'][0]['value'];
        //'name',
        $line[$ind['aggname']] = $agg->title;
        //'generated load p.e.',
        $line[$ind['generated_load_p.e.']] = $agg->field_agggenerated['und'][0]['value']; 
        //'deadline for Art. 3 compliance',
        $line[$ind['deadline_for_art._3_compliance']] = substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10); 
        //'deadline for Art. 4 compliance',
        $line[$ind['deadline_for_art._4_compliance']] = substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10); 
        //'deadline for Art. 5 compliance',
        $line[$ind['deadline_for_art._5_compliance']] = substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10); 
        //'Pending or expired deadline EXP/PD/EXP-PD',
        $pending_or_expired = '';
//         dsm($annee);
//         dsm($line[$ind['deadline_for_art._3_compliance']]);
//         dsm(substr($line[$ind['deadline_for_art._3_compliance']], 0, 4));
//         dsm($line[$ind['deadline_for_art._4_compliance']]);
//         dsm(substr($line[$ind['deadline_for_art._4_compliance']], 0, 4));
//         dsm($line[$ind['deadline_for_art._5_compliance']]);
//         dsm(substr($line[$ind['deadline_for_art._5_compliance']], 0, 4));
//         dsm($agg->field_aggcompliance['und'][0]['value']);
//         dsm(substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) >= $annee);
//         dsm(substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) >= $annee);
//         dsm(substr($line[$ind['deadline_for_art._5_compliance']], 0, 4) >= $annee);
//         
//         dsm(substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) >= $annee
//              && substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) >= $annee
//              && substr($line[$ind['deadline_for_art._5_compliance']], 0, 4) >= $annee);
//              
//         dsm(($agg->field_aggcompliance['und'][0]['value'] == 'PD' || $agg->field_aggcompliance['und'][0]['value'] == 'C' ));
//         dsm((substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) >= $annee
//              && substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) >= $annee
//              && substr($line[$ind['deadline_for_art._5_compliance']], 0, 4) >= $annee
//              && ($agg->field_aggcompliance['und'][0]['value'] == 'PD' || $agg->field_aggcompliance['und'][0]['value'] == 'C' )));             
        
        if ((substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) < $annee
             || substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) < $annee
             || substr($line[$ind['deadline_for_art._5_compliance']], 0, 4) < $annee
            )
             && ($agg->field_aggcompliance['und'][0]['value'] == 'NC' || $agg->field_aggcompliance['und'][0]['value'] == 'C' ))
            {
            $pending_or_expired = 'EXP';
        }   
        if (substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) >= $annee
             && substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) >= $annee
             && substr($line[$ind['deadline_for_art._5_compliance']], 0, 4) >= $annee
             && ($agg->field_aggcompliance['und'][0]['value'] == 'PD' || $agg->field_aggcompliance['und'][0]['value'] == 'C' ))
            {
            $pending_or_expired = 'PD';
        }   
//         dsm($pending_or_expired);                  
        $line[$ind['pending_or_expired_deadline_exp/pd/exp-pd']] = $pending_or_expired;
        //'NUTS 1 code',
        if (strlen($agg->field_regionnuts['und'][0]['value']) >= 3) {
            $line[$ind['nuts_1_code']] = substr($agg->field_regionnuts['und'][0]['value'], 0, 3);
        } else {
            $line[$ind['nuts_1_code']] = '';
        }    
        //'NUTS 2 code',
        if (strlen($agg->field_regionnuts['und'][0]['value']) >= 4) {
            $line[$ind['nuts_2_code']] = substr($agg->field_regionnuts['und'][0]['value'], 0, 4);
        } else {
            $line[$ind['nuts_2_code']] = '';
        }           
        //'NUTS 3 code',
        if (strlen($agg->field_regionnuts['und'][0]['value']) >= 5) {
            $line[$ind['nuts_3_code']] = substr($agg->field_regionnuts['und'][0]['value'], 0, 5);
        } else {
            $line[$ind['nuts_3_code']] = '';
        }           
        
        //Big City info', 
        $bc_code = '';
        $bc_name = ''; 
        foreach($agg->field_aggbcliste['und'] as $listnidbc) {
//             dsm($listnidbc);
            $bc = node_load($listnidbc['nid']);
//             dsm($bc);
            $bc_code .= ($bc_code == '' ? '' : ';') . $bc->field_inspireidlocalid['und'][0]['value'];
            $bc_name .= ($bc_name == '' ? '' : ';') . $bc->title;
//             dsm($bc_code);
//             dsm($bc_name);
            unset($bc);
        }        
        //'code',
        $line[$ind['bccode']] = $bc_code;
        //'name',
        $line[$ind['bcname']] = $bc_name;
        unset($bc_code, $bc_name);
        
        //?',
        //'Number of UWWTPs/ Collecting systems without treatment serving the agglomeration',
        
        $line[$ind['number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration']] = $uwwtpinfo_for_agglo['uwwtp_collecting_without_treatment'];
        
        //Previous agglomeration info',
        $previousagglonid = uwwtd_check_exist(uwwtd_get_siteid('agglomeration', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'])));
        // $previousuwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $context['sandbox']['previous_year'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
        // $previousagglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
        $previous_reported = '';
        $previous_aggcode = '';
        $previous_gen = '';
        $previous_percentage = '';
        
        if ($context['sandbox']['previous_year'] !== false && $previousagglonid == '') {
            $previous_reported = 'no';
        }
        if ($previousagglonid != '') {
            $previousagg = node_load($previousagglonid);
            $previous_reported = 'yes';
//             dsm($previousagg);
            $previous_aggcode =  $previousagg->field_inspireidlocalid['und'][0]['value'];
            $previous_gen = $previousagg->field_agggenerated['und'][0]['value'];
            if ($previousagg->field_agggenerated['und'][0]['value'] == $agg->field_agggenerated['und'][0]['value']) {
                $previous_percentage = '0';                
            } elseif ($previousagg->field_agggenerated['und'][0]['value'] > $agg->field_agggenerated['und'][0]['value']) {
                $previous_percentage = 100 - ($agg->field_agggenerated['und'][0]['value'] * 100 / $previousagg->field_agggenerated['und'][0]['value']);
            } else { //<
                $previous_percentage = -1 * (100 - ($previousagg->field_agggenerated['und'][0]['value'] * 100 / $agg->field_agggenerated['und'][0]['value']));
            }                        
        }            
        //'reported ?',
         $line[$ind['previous_reported_?']] = $previous_reported;
        //'code',
         $line[$ind['previous_aggcode']] = $previous_aggcode;
        //'gen. load p.e.',
         $line[$ind['previous_gen._load_p.e.']] = $previous_gen;
        //'Percentage of change of size (1)',
         $line[$ind['previous_percentage_of_change_of_size_(1)']] = $previous_percentage;
        
        //Connection',
        //'Total % (2)',
        $line[$ind['total_%_(2)']] = $agg->field_aggc1['und'][0]['value'] + $agg->field_aggc2['und'][0]['value'] + $agg->field_aggpercwithouttreatment['und'][0]['value'];
        //'connected to collecting system % of gen. load',
        $line[$ind['connected_to_collecting_system']] = $agg->field_aggc1['und'][0]['value'];
        //'connected to collecting system p. e.',
        $line[$ind['connected_to_collecting_system_p._e.']] = $agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
        //'addressed through IAS % of gen. load',
        $line[$ind['addressed_through_ias_%_of_gen._load']] = $agg->field_aggc2['und'][0]['value'];
        //'addressed through IAS p. e.',
        $line[$ind['addressed_through_ias_p._e.']] = $agg->field_aggc2['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
        //'neither collecting system nor IAS % of gen. load',
        $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] = $agg->field_aggpercwithouttreatment['und'][0]['value'];
        //'neither collecting system nor IAS p. e.',
        $line[$ind['neither_collecting_system_nor_ias_p._e.']] = $agg->field_aggpercwithouttreatment['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
        
        //Distance to compliance Art. 3',
        //take all uwwtp art3 comp NC,NI,PD of the agglo and sum the data 
        //'% of gen. Load (3)',
//         $line[$ind['dist_art3_%_of_gen._load_(3)']] = $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['percent']
//                                                     + $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['percent']
//                                                     + $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['percent']
//                                                     + $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['percent']
//                                                     ;
//         //'in p.e.',
//         $line[$ind['dist_art3_in_p.e.']] = $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['pe']
//                                         + $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['pe']
//                                         + $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['pe']
//                                         + $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['pe']
//                                         ;
        $line[$ind['dist_art3_%_of_gen._load_(3)']] = $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
        //'in p.e.',
        $line[$ind['dist_art3_in_p.e.']] = $line[$ind['neither_collecting_system_nor_ias_p._e.']];                                     
        
        //Compliance Art. 3',
        //'Compliance',
        $line[$ind['art3_compliance']] = $agg->field_aggart3compliance['und'][0]['value'];
        
        //Compliance Art. 3 previous reporting',
        //'Compliance',
        $previous_art3_compliance = '';
        if (isset($previousagg)) {
            $previous_art3_compliance = $previousagg->field_aggart3compliance['und'][0]['value'];
        }
        $line[$ind['previous_art3_compliance']] = $previous_art3_compliance;
        
        //Ratio connection/collection/treatment',
        //'Total entering load p. e.',       
        $line[$ind['total_entering_load_p._e.']] = $uwwtpinfo_for_agglo['entering'];
        //'ratio connected/enentering %',
        $line[$ind['ratio_connected/enentering_%']] = $line[$ind['total_entering_load_p._e.']] * 100 / $line[$ind['connected_to_collecting_system_p._e.']];
        //'Total treatment plant design capacity  p. e.',
        $line[$ind['total_treatment_plant_design_capacity_p._e.']] = $uwwtpinfo_for_agglo['capacity'];
        //'ratio Total entering load/Total design capacity %',
        $line[$ind['ratio_total_entering_load/total_design_capacity_%']] = $line[$ind['total_entering_load_p._e.']] * 100 / $line[$ind['total_treatment_plant_design_capacity_p._e.']];
        
        //Compliance Art. 4',
        //in case multiple uwwtp for this agglo, we must calculate total values  
//         dsm($agg->field_inspireidlocalid['und'][0]['value']);
//         dsm($uwwtpinfo_for_agglo);
        //'compliant % of gen. load',                                                                                     
        $line[$ind['art4_compliant_%_of_gen._load']] = $uwwtpinfo_for_agglo['C']['percent'];
        //'compliant p. e.',
        $line[$ind['art4_compliant_p._e.']] = $uwwtpinfo_for_agglo['C']['pe'];
        //'not compliant % of gen. load', 
        $line[$ind['art4_not_compliant_%_of_gen._load']] = $uwwtpinfo_for_agglo['NC']['percent'];
        //'not compliant',
        $line[$ind['art4_not_compliant']] = $uwwtpinfo_for_agglo['NC']['pe'];
        //'not relevant % of gen. load',
        $line[$ind['art4_not_relevant_%_of_gen._load']] = $uwwtpinfo_for_agglo['NR']['percent'];
        //'not relevant',
        $line[$ind['art4_not_relevant']] = $uwwtpinfo_for_agglo['NR']['pe'];
        //'Compliance Art. 4 (only)',
        $line[$ind['art4_compliance_art._4_(only)']] = $agg->field_aggart4compliance['und'][0]['value'];
        //'Compliance Art. 4 (hierarchical approach)',
        /*
        if art4 = nr then nr
        sinon if comp art 3 in (c, qc) or comp art 4 = c then c
              else if comp art 3 = NC ou comp art 4 = nc then nc
                    else if comp art 4 = nr then nr
                          else nothing
         
        =IF(‘Compliance Art.4 ‘="NR"; 
                ‘Compliance Art.4 ‘;
                IF(
                    AND(
                        OR(‘Compliance Art.3 ‘="C";
                          (‘Compliance Art.3 ‘="QC);
                          ‘Compliance Art.4 ‘="C");
                            "C";
                            IF(
                                OR(
                                    (
                                     ‘Compliance Art.3 ‘="NC"; 
                                     ‘Compliance Art.4 ‘NC"
                                     );
                                        "NC"; 
                                        IF(
                                           ‘Compliance Art.4 ‘="NR";
                                                "NR";
                                                "EXIT"
                                           )
                                   )
                               )
                        ) 
        */
        $compart4hierarchical = '';
        if ($agg->field_aggart4compliance['und'][0]['value'] == 'NR') { 
            $compart4hierarchical = $agg->field_aggart4compliance['und'][0]['value'];
        } elseif ($agg->field_aggart3compliance['und'][0]['value'] == 'C'
                  || $agg->field_aggart3compliance['und'][0]['value'] == 'QC'
                  || $agg->field_aggart4compliance['und'][0]['value'] == 'C' 
                  ) {
            $compart4hierarchical = 'C';
        } elseif ($agg->field_aggart3compliance['und'][0]['value'] == 'NC'
                  || $agg->field_aggart4compliance['und'][0]['value'] == 'NC' 
                  ) {
            $compart4hierarchical = 'NC';
        }                          
        $line[$ind['art4_compliance_art._4_(hierarchical_approach)']] = $compart4hierarchical;
        
        //Compliance Art. 4 previous reporting',
        //'Compliance',
        $previous_art4_compliance = '';
        if (isset($previousagg)) {
            $previous_art4_compliance = $previousagg->field_aggart4compliance['und'][0]['value'];
        }
        $line[$ind['previous_art4_compliance']] = $previous_art4_compliance;        
        

        //Distance to compliance art. 4',
        //Treatment',
        //'% of connected Load',
        $line[$ind['dist_art4_treat__%_of_connected_load']] =  $uwwtpinfo_for_agglo['distart4_treatment_%'];
        //'in p.e. (4)',
        $line[$ind['dist_art4_treat_in_p.e._(4)']] = $uwwtpinfo_for_agglo['distart4_treatment'];
        //Performance',
        //'% of connected Load',
        $line[$ind['dist_art4_perf_%_of_connected_load']] = $uwwtpinfo_for_agglo['distart4_monitoring_%'];
        //'in p.e. (5)',
        $line[$ind['dist_art4_perf_in_p.e._(5)']] = $uwwtpinfo_for_agglo['distart4_monitoring'];
        
        //Compliance Art. 5',
        //'compliant 5 a  (% of gen. load)', 
        $line[$ind['compliant_5_a_(%_of_gen._load)']] = 'TODO';
        //'compliant 5 a  (p. e.)',     
        $line[$ind['compliant_5_a_(p._e.)']] = 'TODO';
        //'not compliant 5 a  (% of gen. load)',
        $line[$ind['not_compliant_5_a_(%_of_gen._load)']] = 'TODO';
        //'not compliant 5 a  (p. e.)',
        $line[$ind['not_compliant_5_a_(p._e.)']] = 'TODO';
        //'not relevant 5 a   (% of gen. load)',
        $line[$ind['not_relevant_5_a_(%_of_gen._load)']] = 'TODO';
        //'not relevant 5 a   (p. e.)',
        $line[$ind['not_relevant_5_a_(p._e.)']] = 'TODO';
        //'Compliance Art. 5a (only)',
        $line[$ind['compliance_art._5a_(only)']] = 'TODO';
        //'Compliance Art. 5a (hierarchical approach)',
        $line[$ind['compliance_art._5a_(hierarchical_approach)']] = 'TODO';
        //'compliant 5 b    (% of gen. load)',
        $line[$ind['compliant_5_b_(%_of_gen._load)']] = 'TODO';
        //'compliant 5 b    (p. e.)',
        $line[$ind['compliant_5_b_(p._e.)']] = 'TODO';
        //'not compliant 5 b  (% of gen. load)',
        $line[$ind['not_compliant_5_b_(%_of_gen._load)']] = 'TODO';
        //'not compliant 5 b   (p. e.)',
        $line[$ind['not_compliant_5_b_(p._e.)']] = 'TODO';
        //'not relevant 5 b   (% of gen. load)',
        $line[$ind['not_relevant_5_b_(%_of_gen._load)']] = 'TODO';
        //'not relevant 5 b   (p. e.)',
        $line[$ind['not_relevant_5_b_(p._e.)']] = 'TODO';
        //'Compliance Art. 5b (only)',
        $line[$ind['compliance_art._5b_(only)']] = 'TODO';
        //'Compliance Art. 5b (hierarchical approach)',
        $line[$ind['compliance_art._5b_(hierarchical_approach)']] = 'TODO';
        //'compliant 5 c      (% of gen. load)',
        $line[$ind['compliant_5_c_(%_of_gen._load)']] = 'TODO';
        //'compliant 5 c       (p. e.)',
        $line[$ind['compliant_5_c__(p._e.)']] = 'TODO';        
        //'not compliant 5 c   (% of gen. load)',
        $line[$ind['not_compliant_5_c_(%_of_gen._load)']] = 'TODO';
        //'not compliant 5 c   (p. e.)',
        $line[$ind['not_compliant_5_c_(p._e.)']] = 'TODO';
        //'not relevant 5 c   (% of gen. load)',
        $line[$ind['not_relevant_5_c_(%_of_gen._load)']] = 'TODO';
        //'not relevant 5 c  (p. e.)',
        $line[$ind['not_relevant_5_c_(p._e.)']] = 'TODO';
        //'Compliance Art. 5c (only)',
        $line[$ind['compliance_art._5c_(only)']] = 'TODO';
        //'Compliance Art. 5c (hierarchical approach)',
        $line[$ind['compliance_art._5c_(hierarchical_approach)']] = 'TODO';
        //'compliant Art. 5 (p. e.)',
        $line[$ind['compliant_art._5_(p._e.)']] = 'TODO';
        //'not compliant Art. 5 (p. e.)',
        $line[$ind['not_compliant_art._5_(p._e.)']] = 'TODO';
        //'Art. 5 not relevant  (p. e.)',
        $line[$ind['art._5_not_relevant_(p._e.)']] = 'TODO';
        //'Compliance Art. 5 (whole agglo)',
        $line[$ind['compliance_art._5_(whole_agglo)']] = $agg->field_aggart5compliance['und'][0]['value'];
        //'Compliance Art. 5  (whole agglo) (hierarchical approach)',
        /*
        if compart5 = nr then nr
        elseif generated load <= 10000 then NR
        elseif compart4 = C and compart5 = C then C
        elseif compart4 = NC or compart 5 = NC then NC
        elseif compart 5 =NR then NR
        else nothing
                        
        =IF(
            ‘Compliance Art. 5 (only)’="NR"; 
                ‘Compliance Art. 5 (only)’;
                IF(‘generated load’ <=10000;
                    "NR";
                    IF(
                        AND(
                            ‘Compliance Art.4’ ="C"; 
                            ‘Compliance Art. 5 (only)’="C"
                            );
                                "C"; 
                                IF(
                                    OR(
                                        Compliance Art.4’ ="NC";
                                        ‘Compliance Art. 5 (only)’="NC"
                                       ); 
                                        "NC";
                                        IF(
                                            ‘Compliance Art. 5 (only)’="NR";
                                                "NR"; 
                                                "EXIT"
                                           )
                                   )
                       )
                )
          )
        */        
        $compart5hierarchical = '';
        if ($agg->field_aggart5compliance['und'][0]['value'] == 'NR') { 
            $compart5hierarchical = $agg->field_aggart5compliance['und'][0]['value'];
        } elseif ($agg->field_agggenerated['und'][0]['value'] <= 10000) {
            $compart5hierarchical = 'NR';
        } elseif ($agg->field_aggart4compliance['und'][0]['value'] == 'C'
                  && $agg->field_aggart5compliance['und'][0]['value'] == 'C' 
                  ) {
            $compart5hierarchical = 'C';
        } elseif ($agg->field_aggart4compliance['und'][0]['value'] == 'NC'
                  || $agg->field_aggart5compliance['und'][0]['value'] == 'NC' 
                  ) {
            $compart5hierarchical = 'NC';
        }                          
        $line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] = $compart5hierarchical;
        
        //Compliance Art. 5 previous reporting',
        //'Compliance',
        $previous_art5_compliance = '';
        if (isset($previousagg)) {
            $previous_art5_compliance = $previousagg->field_aggart5compliance['und'][0]['value'];
        }        
        $line[$ind['previous_art5_compliance']] = $previous_art5_compliance;        
        
        //Distance to compliance art. 5',
        //Treatment',
        //'% of gen. Load', 
        $line[$ind['distance_art5_treat_%_of_gen._load']] = $uwwtpinfo_for_agglo['distart5_treatment_%'];
        //'in p.e. (6)', 
        $line[$ind['distance_art5_treat_in_p.e._(6)']] = $uwwtpinfo_for_agglo['distart5_treatment'];
        //Performance',
        //'% of gen. Load', 
        $line[$ind['distance_art5_perf_%_of_gen._load']] = $uwwtpinfo_for_agglo['distart5_monitoring_%'];
        //'in p.e. (7)', 
        $line[$ind['distance_art5_perf_in_p.e._(7)']] = $uwwtpinfo_for_agglo['distart5_monitoring'];
        
        //current RCA details',
        $dcpWaterBodyType = '';
        $dcpWaterbodyID = '';
        $dcpWFDRBD = '';
        $dcpTypeOfReceivingArea = '';
        $rcaID = '';
        $rcaName = '';
        $rcaName = '';
        $rcaDateArt54 = '';
        $rcaDateArt58 = '';
        $rcatreated = array();
        foreach($agg->field_agguwwliste['und'] as $uwwtpnid) {
            $uwwtp = node_load($uwwtpnid['nid']);
            foreach($uwwtp->field_uwwdcpliste['und'] as $dcpnid) {
                $dcp = node_load($dcpnid['nid']);

                foreach($dcp->field_dcprcaliste['und'] as $rcanid) {
                    $rca = node_load($rcanid['nid']);
                    
                    if (isset($rcatreated[$rca->field_inspireidlocalid['und'][0]['value']])) {
                        continue;
                    }
                    $rcatreated[$rca->field_inspireidlocalid['und'][0]['value']] = true;
                    
                    $dcpWaterBodyType .= ($dcpWaterBodyType == '' ? '' : ';') . $dcp->field_dcpwaterbodytype['und'][0]['value'];
                    $dcpWaterbodyID .= ($dcpWaterbodyID == '' ? '' : ';') . $dcp->field_dcpwaterbodyid['und'][0]['value'];
                    $dcpWFDRBD .= ($dcpWFDRBD == '' ? '' : ';') . $dcp->field_dcpwfdrbd['und'][0]['value'];
                    $dcpTypeOfReceivingArea .= ($dcpTypeOfReceivingArea == '' ? '' : ';') . $dcp->field_rcatype['und'][0]['value'];                    
                                        
                    $rcaID .= ($rcaID == '' ? '' : ';') . $rca->field_inspireidlocalid['und'][0]['value'];
                    $rcaName .= ($rcaName == '' ? '' : ';') . $rca->title;
                    $rcaDateArt54 .= ($rcaDateArt54 == '' ? '' : ';') . substr($rca->field_rcadateart54['und'][0]['value'], 0, 10);
                    $rcaDateArt58 .= ($rcaDateArt58 == '' ? '' : ';') . substr($rca->field_rcadateart5854['und'][0]['value'], 0, 10);
                                        
                    unset($rca);
                }     
                unset($dcp);           
            }
            unset($uwwtp);   
        }  
        unset($rcatreated);   
        //'water body type',   ?dcpWaterBodyType
        $line[$ind['water_body_type']] = $dcpWaterBodyType;
        //'Id code of the water body WFD',    ?dcpWaterbodyID
        $line[$ind['id_code_of_the_water_body_wfd']] = $dcpWaterbodyID;
        //'ID of WFD river basin district',    ?dcpWFDRBD
        $line[$ind['id_of_wfd_river_basin_district']] = $dcpWFDRBD;
        //'entirely discharging into coastal waters?',
        $WBT = explode(';', $dcpWaterBodyType);
        $entirely_discharge_in_CW = (false !== strpos($dcpWaterBodyType, 'CW') ? 'yes' : 'no');
        foreach($WBT as $oneWBT) {
            if ($oneWBT != 'CW') {
                $entirely_discharge_in_CW = 'no';
                break;
            }
        }
        unset($WBT);
        $line[$ind['entirely_discharging_into_coastal_waters']] = $entirely_discharge_in_CW;  //=yes if column Z= CW?;
        //'type of receiving water',   ?dcpTypeOfReceivingArea
        $line[$ind['type_of_receiving_water']] = $dcpTypeOfReceivingArea;
        //'ID of the SA or LSA',   ?rcaID
        $line[$ind['id_of_the_sa_or_lsa']] = $rcaID;
        //'name',         ?title
        $line[$ind['rca_name']] = $rcaName;
        //'Starting date of application 5.8.',
        $line[$ind['starting_date_of_application_5.8.']] = $rcaDateArt58;  
        //'Starting date of application 5.4.',
        $line[$ind['starting_date_of_application_5.4.']] = $rcaDateArt54;
        //'Starting date of application a N',
        $line[$ind['starting_date_of_application_a_n']] = 'TODO';
        //'Starting date of application a P',
        $line[$ind['starting_date_of_application_a_p']] = 'TODO';
        //'Starting date of application b',
        $line[$ind['starting_date_of_application_b']] = 'TODO';
        //'Starting date of application c',
        $line[$ind['starting_date_of_application_c']] = 'TODO';
        
        //Installations for wastewater treatment in place',
        //'secondary treatment installed (% of gen. load)',
        //Aggregated from sheet ‘UWWTPs’ via the Excel function of Pivot-tables  (based on parameters ‘UWWTP info, entering this UWWTP’ and ‘Installations for wastewater treatment in place, Secondary: installed?’
        //Calculated on basis of parameters ‘Secondary treatment installed -  %of gen. load’ and the generated load of the agglomeration (p.e.)
        $secondary_install_pe      = '';
        $secondary_install_percent = '';  
        if ($uwwtpinfo_for_agglo['secondary_installed'] == 1) {  
            $secondary_install_pe = $uwwtpinfo_for_agglo['secondary_treatement_percent']; 
            $secondary_install_percent = $uwwtpinfo_for_agglo['secondary_treatement_pe']; 
        }
        $line[$ind['secondary_treatment_installed_(%_of_gen._load)']] = $secondary_install_percent;
        //'secondary treatment installed (p. e.)', 
        $line[$ind['secondary_treatment_installed_(p._e.)']] = $secondary_install_pe;
        
        //'monitoring results f. secondary OK (% of gen. load)',
        $secondary_passed_pe      = '';
        $secondary_passed_percent = '';  
        if ($uwwtpinfo_for_agglo['secondary_passed'] == 1) {  
            $secondary_passed_pe = $uwwtpinfo_for_agglo['secondary_monitoring_percent']; 
            $secondary_passed_percent = $uwwtpinfo_for_agglo['secondary_monitoring_pe']; 
        }          
        $line[$ind['monitoring_results_f._secondary_ok_(%_of_gen._load)']] = $secondary_passed_percent;
        //'monitoring results f. secondary OK (p. e.)', 
        $line[$ind['monitoring_results_f._secondary_ok']] = $secondary_passed_pe;
                
        //'more str. treatment installed (%_of_gen._load',
        $morestringent_install_pe      = '';
        $morestringent_install_percent = '';  
        if ($uwwtpinfo_for_agglo['morestringent_installed'] == 1) {  
            $morestringent_install_pe = $uwwtpinfo_for_agglo['morestringent_treatement_percent']; 
            $morestringent_install_percent = $uwwtpinfo_for_agglo['morestringent_treatement_pe']; 
        }   
                  
        $line[$ind['more_str._treatment_installed_(%_of_gen._load)']] = $morestringent_install_percent;
        //'more str. treatment installed (p. e.)', 
        $line[$ind['more_str._treatment_installed_(p._e.)']] = $morestringent_install_pe;
        
        //'monitoring results f. more string. OK (% of gen. load)',
        $morestringent_passed_pe      = '';
        $morestringent_passed_percent = '';  
        if ($uwwtpinfo_for_agglo['morestringent_passed'] == 1) {  
            $secondary_passed_pe = $uwwtpinfo_for_agglo['morestringent_monitoring_percent']; 
            $morestringent_passed_percent = $uwwtpinfo_for_agglo['morestringent_monitoring_pe']; 
        }            
        $line[$ind['monitoring_results_f._more_string._ok_(%_of_gen._load)']] = $morestringent_passed_percent;
        //'monitoring results f. more string. OK (p. e.)', 
        $line[$ind['monitoring_results_f._more_string._ok_(p._e.)']] =$morestringent_passed_pe;
        
                  
        if (isset($context['sandbox']['uwwtpinfo_for_agglo'][$agg->field_inspireidlocalid['und']])) {
            unset($context['sandbox']['uwwtpinfo_for_agglo'][$agg->field_inspireidlocalid['und']]);
        } 
        
        //debug data info
        $line[$ind['debug_compliance_agglo_from_algo']] = $agg->field_aggcompliance['und'][0]['value'];
                   
        fputcsv($r, $line, ';');
        
        //expired', 'pending')
        foreach ($listtypestat as $typestat) {
            if ( !isset($context['sandbox'][$typestat])) {
                $context['sandbox'][$typestat]['generated_pe'] = 0;
                $context['sandbox'][$typestat]['previous_generated_pe'] = 0;
                $context['sandbox'][$typestat]['total_percent'] = 0;
                $context['sandbox'][$typestat]['collected_percent'] = 0;
                $context['sandbox'][$typestat]['collected_pe'] = 0;
                $context['sandbox'][$typestat]['ias_percent'] = 0;
                $context['sandbox'][$typestat]['ias_pe'] = 0;
                $context['sandbox'][$typestat]['not_collected_percent'] = 0;
                $context['sandbox'][$typestat]['not_collected_pe'] = 0;
                $context['sandbox'][$typestat]['distance_art3_percent'] = 0;
                $context['sandbox'][$typestat]['distance_art3_pe'] = 0;        
                $context['sandbox'][$typestat]['ratio_entering'] = 0;
                $context['sandbox'][$typestat]['ratio_connected_entering'] = 0;
                $context['sandbox'][$typestat]['ratio_capacity'] = 0;
                $context['sandbox'][$typestat]['ratio_entering_capacity'] = 0;
                $context['sandbox'][$typestat]['art4_c_percent'] = 0;
                $context['sandbox'][$typestat]['art4_c_pe'] = 0; 
                $context['sandbox'][$typestat]['art4_nc_percent'] = 0;
                $context['sandbox'][$typestat]['art4_nc_pe'] = 0;
                $context['sandbox'][$typestat]['art4_nr_percent'] = 0;
                $context['sandbox'][$typestat]['art4_nr_pe'] = 0;
                $context['sandbox'][$typestat]['distance_art4_treatment_percent'] = 0;
                $context['sandbox'][$typestat]['distance_art4_treatment_pe'] = 0;                                 
                $context['sandbox'][$typestat]['distance_art4_performance_percent'] = 0;
                $context['sandbox'][$typestat]['distance_art4_performance_pe'] = 0;
                $context['sandbox'][$typestat]['distance_art5_treatment_percent'] = 0;
                $context['sandbox'][$typestat]['distance_art5_treatment_pe'] = 0;                                 
                $context['sandbox'][$typestat]['distance_art5_performance_percent'] = 0;
                $context['sandbox'][$typestat]['distance_art5_performance_pe'] = 0;        
                $context['sandbox'][$typestat]['secondary_installed'] = 0;
                $context['sandbox'][$typestat]['secondary_monitoring'] = 0;
                $context['sandbox'][$typestat]['morestringent_installed'] = 0;
                $context['sandbox'][$typestat]['morestringent_monitoring'] = 0; 
            }        
            //expired deadline   (toutes les NC +  les C qui ont date dépassée)
            if ($typestat == 'expired' && $pending_or_expired != 'EXP') {        
//                 if (false === (
//                     (substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) < $annee
//                      || substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) < $annee
//                      || substr($line[$ind['deadline_for_art._5_compliance']], 0, 4) < $annee
//                     )
//                      && ($agg->field_aggcompliance['und'][0]['value'] == 'NC' || $agg->field_aggcompliance['und'][0]['value'] == 'C' )
//                     )) {
                    continue;
//                 }
            }
            
            //pending deadline    PD + les C avec date pas dépassée
            if ($typestat == 'pending' && $pending_or_expired != 'PD') {     
//                 if (false === (
//                     ((substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) >= $annee
//                      && substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) >= $annee
//                      && substr($line[$ind['ddeadline_for_art._5_compliance']], 0, 4) >= $annee
//                     )
//                     && ($agg->field_aggcompliance['und'][0]['value'] == 'PD' || $agg->field_aggcompliance['und'][0]['value'] == 'C' ))
//                     )) {
                    continue;
//                 }
            }
                 
            $context['sandbox'][$typestat]['generated_pe'] += $line[$ind['generated_load_p.e.']];
            $context['sandbox'][$typestat]['previous_generated_pe'] += $line[$ind['previous_gen._load_p.e.']];
            $context['sandbox'][$typestat]['total_percent'] += $line[$ind['total_%_(2)']];
            $context['sandbox'][$typestat]['collected_percent'] += $line[$ind['connected_to_collecting_system']];
            $context['sandbox'][$typestat]['collected_pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
            $context['sandbox'][$typestat]['ias_percent'] += $line[$ind['addressed_through_ias_%_of_gen._load']];
            $context['sandbox'][$typestat]['ias_pe'] += $line[$ind['addressed_through_ias_p._e.']];
            $context['sandbox'][$typestat]['not_collected_percent'] += $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
            $context['sandbox'][$typestat]['not_collected_pe'] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            
            $context['sandbox'][$typestat]['distance_art3_percent'] += $line[$ind['dist_art3_%_of_gen._load_(3)']];
            $context['sandbox'][$typestat]['distance_art3_pe'] += $line[$ind['dist_art3_in_p.e.']];
            
            $context['sandbox'][$typestat]['ratio_entering'] += $line[$ind['total_entering_load_p._e.']];
            $context['sandbox'][$typestat]['ratio_connected_entering'] += $line[$ind['ratio_connected/enentering_%']];
            $context['sandbox'][$typestat]['ratio_capacity'] += $line[$ind['total_treatment_plant_design_capacity_p._e.']];
            $context['sandbox'][$typestat]['ratio_entering_capacity'] += $line[$ind['ratio_total_entering_load/total_design_capacity_%']];
            
            $context['sandbox'][$typestat]['art4_c_percent'] += $line[$ind['art4_compliant_%_of_gen._load']];
            $context['sandbox'][$typestat]['art4_c_pe'] += $line[$ind['art4_compliant_p._e.']];
            $context['sandbox'][$typestat]['art4_nc_percent'] += $line[$ind['art4_not_compliant_%_of_gen._load']];
            $context['sandbox'][$typestat]['art4_nc_pe'] += $line[$ind['art4_not_compliant']];
            $context['sandbox'][$typestat]['art4_nr_percent'] += $line[$ind['art4_not_relevant_%_of_gen._load']];
            $context['sandbox'][$typestat]['art4_nr_pe'] += $line[$ind['art4_not_relevant']];
            
            $context['sandbox'][$typestat]['distance_art4_treatment_percent'] += $line[$ind['dist_art4_treat__%_of_connected_load']];
            $context['sandbox'][$typestat]['distance_art4_treatment_pe'] += $line[$ind['dist_art4_treat_in_p.e._(4)']];                           
            $context['sandbox'][$typestat]['distance_art4_performance_percent'] += $line[$ind['dist_art4_perf_%_of_connected_load']];
            $context['sandbox'][$typestat]['distance_art4_performance_pe'] += $line[$ind['dist_art4_perf_in_p.e._(5)']];
            
            $context['sandbox'][$typestat]['distance_art5_treatment_percent'] += $line[$ind['distance_art5_treat_%_of_gen._load']];
            $context['sandbox'][$typestat]['distance_art5_treatment_pe'] += $line[$ind['distance_art5_treat_in_p.e._(6)']];                        
            $context['sandbox'][$typestat]['distance_art5_performance_percent'] += $line[$ind['distance_art5_perf_%_of_gen._load']];
            $context['sandbox'][$typestat]['distance_art5_performance_pe'] += $line[$ind['distance_art5_perf_in_p.e._(7)']];
            
            $context['sandbox'][$typestat]['secondary_installed'] += $line[$ind['secondary_treatment_installed_(p._e.)']];
            $context['sandbox'][$typestat]['secondary_monitoring'] += $line[$ind['monitoring_results_f._secondary_ok']];
            $context['sandbox'][$typestat]['morestringent_installed'] += $line[$ind['more_str._treatment_installed_(p._e.)']];
            $context['sandbox'][$typestat]['morestringent_monitoring'] += $line[$ind['monitoring_results_f._more_string._ok_(p._e.)']];  
        } 
        
        //get all stat of agglo for NA or CSA/NA/A58 RCA
        foreach ($list_type_rca as $type_rca) {
            if ( !isset($context['sandbox'][$type_rca])) {
                $context['sandbox'][$type_rca]['subjected_agglo'] = 0;
                $context['sandbox'][$type_rca]['compliant_agglo'] = 0;
                $context['sandbox'][$type_rca]['notcompliant_agglo'] = 0;
                $context['sandbox'][$type_rca]['subjected_load']['generated'] = 0;
                $context['sandbox'][$type_rca]['subjected_load']['collected'] = 0;
                $context['sandbox'][$type_rca]['compliant_load']['generated'] = 0;
                $context['sandbox'][$type_rca]['compliant_load']['collected'] = 0;
                $context['sandbox'][$type_rca]['notcompliant_load']['generated'] = 0;
                $context['sandbox'][$type_rca]['notcompliant_load']['collected'] = 0;
                $context['sandbox'][$type_rca]['exempted'] = 0;
                $context['sandbox'][$type_rca]['exempted_load']['generated'] = 0;
                $context['sandbox'][$type_rca]['exempted_load']['collected'] = 0;            
            }
            
            if ($type_rca == 'na' && false === strpos($line[$ind['type_of_receiving_water']], 'NA')) {
                continue;
            }
            if ($type_rca == 'sa_csa' 
                && false === strpos($line[$ind['type_of_receiving_water']], 'SA')
                && false === strpos($line[$ind['type_of_receiving_water']], 'CSA')
                && false === strpos($line[$ind['type_of_receiving_water']], 'A58')) {
                continue;
            }            
            
            $context['sandbox'][$type_rca]['subjected_agglo']++;
            $context['sandbox'][$type_rca]['subjected_load']['generated'] += $line[$ind['generated_load_p.e.']];
            $context['sandbox'][$type_rca]['subjected_load']['collected'] += $line[$ind['connected_to_collecting_system']];
                
            if ($agg->field_aggcompliance['und'][0]['value'] == 'C') {
                $context['sandbox'][$type_rca]['compliant_agglo']++;
                $context['sandbox'][$type_rca]['compliant_load']['generated'] += $line[$ind['generated_load_p.e.']];
                $context['sandbox'][$type_rca]['compliant_load']['collected'] += $line[$ind['connected_to_collecting_system']];             
            }
            if ($agg->field_aggcompliance['und'][0]['value'] == 'NC') {
                $context['sandbox'][$type_rca]['notcompliant_agglo']++;
                $context['sandbox'][$type_rca]['notcompliant_load']['generated'] += $line[$ind['generated_load_p.e.']];
                $context['sandbox'][$type_rca]['notcompliant_load']['collected'] += $line[$ind['connected_to_collecting_system']];           
            }   
            if ($pending_or_expired == 'PD') {
                $context['sandbox'][$type_rca]['exempted']++;
                $context['sandbox'][$type_rca]['exempted_load']['generated'] += $line[$ind['generated_load_p.e.']];
                $context['sandbox'][$type_rca]['exempted_load']['collected'] += $line[$ind['connected_to_collecting_system']];           
            }         
        }  
        
        ////////////////////////////////////////////////////////////////////////
        //get data for graph sheet
        ////////////////////////////////////////////////////////////////////////
        if ($line[$ind['generated_load_p.e.']] < 10001) {
            $context['results']['graph']['0_10000']['pe']  += $line[$ind['generated_load_p.e.']];
            $context['results']['graph']['0_10000']['number']++;        
        } elseif ($line[$ind['generated_load_p.e.']]  < 100000) {
            $context['results']['graph']['10000_100000']['pe']  += $line[$ind['generated_load_p.e.']];
            $context['results']['graph']['10000_100000']['number']++;             
        } else {   //>=100000
            $context['results']['graph']['100000_n']['pe']  += $line[$ind['generated_load_p.e.']];
            $context['results']['graph']['100000_n']['number']++;             
        }
        
        //array2
        $context['results']['graph']['total_generated']['pe'] += $line[$ind['generated_load_p.e.']];
        $context['results']['graph']['total_collected']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
        $context['results']['graph']['total_load_ias']['pe'] += $line[$ind['addressed_through_ias_p._e.']];
        $context['results']['graph']['total_entering']['pe'] += $line[$ind['total_entering_load_p._e.']];
        $context['results']['graph']['total_capacity']['pe'] += $line[$ind['total_treatment_plant_design_capacity_p._e.']];
        
        //array 5 6 7
        if ($line[$ind['art3_compliance']] == 'C') {
            $context['results']['graph']['art3_C']['number']++;
            $context['results']['graph']['art3_C']['pe'] += $line[$ind['generated_load_p.e.']];
        }
        if ($line[$ind['art3_compliance']] == 'NC') {
            $context['results']['graph']['art3_NC']['number']++;
            $context['results']['graph']['art3_NC']['pe'] += $line[$ind['generated_load_p.e.']];
        }
        if ($line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {     
            $context['results']['graph']['art4_C']['number']++;
            $context['results']['graph']['art4_C']['pe'] += $line[$ind['generated_load_p.e.']];
        }
        if ($line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NC') {
            $context['results']['graph']['art4_NC']['number']++;
            $context['results']['graph']['art4_NC']['pe'] += $line[$ind['generated_load_p.e.']];
        }
        if ($line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {    
            $context['results']['graph']['art5_C']['number']++;
            $context['results']['graph']['art5_C']['pe'] += $line[$ind['generated_load_p.e.']];
        }
        if ($line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC') {
            $context['results']['graph']['art5_NC']['number']++;
            $context['results']['graph']['art5_NC']['pe'] += $line[$ind['generated_load_p.e.']];        
        }       
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
                      

        unset($agg);
        $j++;
        $context['sandbox']['progress']++;
    } 
    fclose($r);
    //write last lines with aggregate data
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_total_name'], $annee);
        $r = fopen($filename, 'wb');
        $totalline = array_pad(array(), count($ind), '');
        $totalline[0] = 'TOTAL';
        $expired = array_pad(array(), count($ind), '');
        $expired[0] = 'expired_deadline'; 
        $pending = array_pad(array(), count($ind), '');
        $pending[0] = 'pending_deadline';                    
//         dsm($expired);
//         dsm($pending);
//         dsm($listtypestat);
//         foreach ($listtypestat as $typestat) {
//             dsm($typestat);
//             dsm($$typestat);
//             dsm($$typestat[$ind['generated_load_p.e.']]);
//         }
//         dsm($context['sandbox']);
//         dsm($ind);
        foreach ($listtypestat as $typestat) {
            ${$typestat}[$ind['generated_load_p.e.']] = $context['sandbox'][$typestat]['generated_pe'] ;
            ${$typestat}[$ind['previous_gen._load_p.e.']] = $context['sandbox'][$typestat]['previous_generated_pe'] ;
            ${$typestat}[$ind['total_%_(2)']] = $context['sandbox'][$typestat]['total_percent'] ;
            ${$typestat}[$ind['connected_to_collecting_system']] = $context['sandbox'][$typestat]['collected_percent'] ;
            ${$typestat}[$ind['connected_to_collecting_system_p._e.']] = $context['sandbox'][$typestat]['collected_pe'] ;
            ${$typestat}[$ind['addressed_through_ias_%_of_gen._load']] = $context['sandbox'][$typestat]['ias_percent'] ;
            ${$typestat}[$ind['addressed_through_ias_p._e.']] = $context['sandbox'][$typestat]['ias_pe'] ;
            ${$typestat}[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] = $context['sandbox'][$typestat]['not_collected_percent'] ;
            ${$typestat}[$ind['neither_collecting_system_nor_ias_p._e.']] = $context['sandbox'][$typestat]['not_collected_pe'] ;
            ${$typestat}[$ind['dist_art3_%_of_gen._load_(3)']] = $context['sandbox'][$typestat]['distance_art3_percent'] ;
            ${$typestat}[$ind['dist_art3_in_p.e.']] = $context['sandbox'][$typestat]['distance_art3_pe']         ;
            ${$typestat}[$ind['total_entering_load_p._e.']] = $context['sandbox'][$typestat]['ratio_entering'] ;
            ${$typestat}[$ind['ratio_connected/enentering_%']] = $context['sandbox'][$typestat]['ratio_connected_entering'] ;
            ${$typestat}[$ind['total_treatment_plant_design_capacity_p._e.']] = $context['sandbox'][$typestat]['ratio_capacity'] ;
            ${$typestat}[$ind['ratio_total_entering_load/total_design_capacity_%']] = $context['sandbox'][$typestat]['ratio_entering_capacity'] ;
            ${$typestat}[$ind['art4_compliant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_c_percent'] ;
            ${$typestat}[$ind['art4_compliant_p._e.']] = $context['sandbox'][$typestat]['art4_c_pe']  ;
            ${$typestat}[$ind['art4_not_compliant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_nc_percent'] ;
            ${$typestat}[$ind['art4_not_compliant']] = $context['sandbox'][$typestat]['art4_nc_pe'] ;
            ${$typestat}[$ind['art4_not_relevant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_nr_percent'] ;
            ${$typestat}[$ind['art4_not_relevant']] = $context['sandbox'][$typestat]['art4_nr_pe'] ;
            ${$typestat}[$ind['dist_art4_treat__%_of_connected_load']] = $context['sandbox'][$typestat]['distance_art4_treatment_percent'] ;
            ${$typestat}[$ind['dist_art4_treat_in_p.e._(4)']] = $context['sandbox'][$typestat]['distance_art4_treatment_pe'];
            ${$typestat}[$ind['dist_art4_perf_%_of_connected_load']] = $context['sandbox'][$typestat]['distance_art4_performance_percent'] ;
            ${$typestat}[$ind['dist_art4_perf_in_p.e._(5)']] = $context['sandbox'][$typestat]['distance_art4_performance_pe'] ;
            ${$typestat}[$ind['distance_art5_treat_%_of_gen._load']] = $context['sandbox'][$typestat]['distance_art5_treatment_percent'] ;
            ${$typestat}[$ind['distance_art5_treat_in_p.e._(6)']] = $context['sandbox'][$typestat]['distance_art5_treatment_pe'];
            ${$typestat}[$ind['distance_art5_perf_%_of_gen._load']] = $context['sandbox'][$typestat]['distance_art5_performance_percent'] ;
            ${$typestat}[$ind['distance_art5_perf_in_p.e._(7)']] = $context['sandbox'][$typestat]['distance_art5_performance_pe']         ;
            ${$typestat}[$ind['secondary_treatment_installed_(p._e.)']] = $context['sandbox'][$typestat]['secondary_installed'] ;
            ${$typestat}[$ind['monitoring_results_f._secondary_ok']] = $context['sandbox'][$typestat]['secondary_monitoring'] ;
            ${$typestat}[$ind['more_str._treatment_installed_(p._e.)']] = $context['sandbox'][$typestat]['morestringent_installed'] ;
            ${$typestat}[$ind['monitoring_results_f._more_string._ok_(p._e.)']] = $context['sandbox'][$typestat]['morestringent_monitoring'] ;

//             dsm($$typestat);
            for ($j = 1, $max = count($ind); $j < $max; $j++) {
                if ('' !== (${$typestat}[$j])) {
                    $totalline[$j] += ${$typestat}[$j];
                }
            }  
        }
        
//         dsm($totalline);
//         dsm($expired);
//         dsm($pending);
        fputcsv($r, $totalline, ';');
        fputcsv($r, $expired, ';');
        fputcsv($r, $pending, ';');  
        fclose($r);  
        
        
        //calcul the evaluation line at the bottom of the sheet
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_evaluation_name'], $annee);
        $r = fopen($filename, 'wb');
        foreach ($list_type_rca as $type_rca) {
            
            $line = array(
                $context['sandbox'][$type_rca]['subjected_agglo'],
                $context['sandbox'][$type_rca]['compliant_agglo'],
                $context['sandbox'][$type_rca]['notcompliant_agglo'],
                $context['sandbox'][$type_rca]['subjected_load']['generated'],
                $context['sandbox'][$type_rca]['subjected_load']['collected'],
                $context['sandbox'][$type_rca]['compliant_load']['generated'],
                $context['sandbox'][$type_rca]['compliant_load']['collected'],
                $context['sandbox'][$type_rca]['notcompliant_load']['generated'],
                $context['sandbox'][$type_rca]['notcompliant_load']['collected'],
                $context['sandbox'][$type_rca]['exempted'],
                $context['sandbox'][$type_rca]['exempted_load']['generated'],
                $context['sandbox'][$type_rca]['exempted_load']['collected'],
            );             
            fputcsv($r, $line, ';');    
        }
        fclose($r);
                 
    }
           
    
    
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }       

//     dsm(file_get_contents($tmpfilename));
// 
//     ini_set("auto_detect_line_endings", false); //for Mac user
//     $aHeaders = array(
//           'Content-Type'        => 'application/octet-stream',
//           'Content-Disposition' => 'attachment; filename="'.$filename.'"',
//           'Expires'             => '0',
//           'Cache-Control'       => 'must-revalidate',
//           'Pragma'              =>'public',
//           'Content-Length'      => filesize($tmpfilename),
//       );
//     foreach ($aHeaders as $sName => $sValue) {
//         drupal_add_http_header($sName, $sValue);
//     }
// 
//     readfile($tmpfilename);
//     unlink($tmpfilename);
//     exit();
//     return $allline;
}

/**
 *
 */ 
function uwwtd_create_register_2015_uwwtp_sheet2($annee, &$context) 
{
    $listtypestat = array('expired', 'pending');
    
	if (!isset($context['sandbox']['progress'])) {
    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['id'] = array();
        
        $context['results']['country_volume_waste_water'] = 0;
        $context['sandbox']['list_uwwtp_treated'] = array();
        $context['sandbox']['expired']['list_agglo_treated'] = array();
        $context['sandbox']['pending']['list_agglo_treated'] = array();
            
        //get all uwwtp (repeat agglo info if one agglo has more than one uwwtp)
        $query = 'select n.nid
        from {node} n    
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id                                                                                                                                                                                                                                                                                                                                                                                                                                
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
        ';
        
        $param = array(
            ':typenode' => 'uwwtp',
            ':annee' => $annee,
            );
//         dsm($query);
//         dsm($param);
        $listuwwtp = array();    
        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['id'][] = $row['nid'];
            }
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
            	
		$context['sandbox']['max'] = count($context['sandbox']['id']);
        
        $context['sandbox']['previous_year'] = uwwtd_get_previous_year($annee);

       
// 		dsm($context);
	} 
    $ind = $GLOBALS['uwwtd']['sheet2_uwwtp'];

//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] .'.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'], $annee);
          
    
    if ($context['sandbox']['progress'] == 0) {
        $r = fopen($filename, 'wb');  
        $header = array(
            //Agglomeration info',
            $ind['aggcode'] => 'code',
            $ind['aggname'] => 'name',
            $ind['aggstatus'] => 'status',
            $ind['generated load  p. e.'] => 'generated load  p. e.',
            $ind['deadline for Art. 3 compliance'] => 'deadline for Art. 3 compliance',
            $ind['deadline for Art. 4 compliance'] => 'deadline for Art. 4 compliance',
            $ind['deadline for Art. 5 compliance'] => 'deadline for Art. 5 compliance',
            //Collection',
            $ind['collected in collecting system % of gen. load'] => 'collected in collecting system % of gen. load',
            $ind['collected in collecting system  p. e.'] => 'collected in collecting system  p. e.',
            $ind['addressed through IAS % of gen. load'] => 'addressed through IAS % of gen. load',
            $ind['addressed through IAS p. e.'] => 'addressed through IAS p. e.',
            $ind['neither collecting system nor IAS % of gen. load'] => 'neither collecting system nor IAS % of gen. load',
            $ind['neither collecting system nor IAS p. e.'] => 'neither collecting system nor IAS p. e.',
            //UWWTP general info',
            $ind['uww_code'] => 'code',
            $ind['uww_name'] => 'name',
            $ind['uww_status'] => 'status',
            $ind['uww_entering this UWWTP % of gen. load'] => 'entering this UWWTP % of gen. load',
            $ind['uww_entering load  p. e.'] => 'entering load  p. e.',
            $ind['uww_capacity p. e.'] => 'capacity p. e.',
            $ind['uww2_status'] => 'status',
            //Previous UWWTP general info',
            $ind['previous_reference year'] => 'reference year',
            $ind['previous_entering load p. e.'] => 'entering load p. e.',
            $ind['previous_capacity p. e.'] => 'capacity p. e.',
            $ind['previous_Percentage of change of entering load (1) %'] => 'Percentage of change of entering load (1) %',
            $ind['previous_Percentage of change of capacity (2) %'] => 'Percentage of change of capacity (2) %',
            //current RCA details',
            $ind['water body type'] => 'water body type',
            $ind['In case of discharge on land please specify the purpose'] => 'In case of discharge on land please specify the purpose',
            $ind['Id code of the water body WFD'] => 'Id code of the water body WFD',
            $ind['ID of WFD river basin district'] => 'ID of WFD river basin district',
            $ind['entirely discharging into coastal waters?'] => 'entirely discharging into coastal waters?',
            $ind['type of receiving water'] => 'type of receiving water',
            $ind['ID of the SA or LSA'] => 'ID of the SA or LSA',
            $ind['rcaname'] => 'name',
            $ind['Starting date of application 5.8.'] => 'Starting date of application 5.8.',
            $ind['Starting date of application 5.4.'] => 'Starting date of application 5.4.',
            $ind['Starting date of application a N'] => 'Starting date of application a N',
            $ind['Starting date of application a P'] => 'Starting date of application a P',
            $ind['Starting date of application b'] => 'Starting date of application b',
            $ind['Starting date of application c'] => 'Starting date of application c',
            //UWWTP treatment and performance',
            $ind['treatment required'] => 'treatment required',
            $ind['treatment type in place (4)'] => 'treatment type in place (4)',
            $ind['Monitoring results - BOD5'] => 'Monitoring results - BOD5',
            $ind['Monitoring results -COD'] => 'Monitoring results -COD',
            $ind['Monitoring results -Ntot'] => 'Monitoring results -Ntot',
            $ind['Monitoring results -Ptot'] => 'Monitoring results -Ptot',
            $ind['Monitoring results -other'] => 'Monitoring results -other',
            $ind['In cause of failure (3)'] => 'In cause of failure (3)',
            $ind['difference between monitoring result N and P and treatment in place'] => 'difference between monitoring result N and P and treatment in place',
            
            
//             $ind['Number of discharge points for this UWWTP'] => 'Number of discharge points for this UWWTP',
//             $ind['DCP ID'] => 'DCP ID',
//             //current RCA info (Q 2013)
//             $ind['ID of receiving water body 2'] => 'ID of receiving water body',
//             $ind['water body type 2'] => 'water body type',
//             $ind['ID of the SA or LSA 2'] => 'ID',
//             $ind['rcaname 2'] => 'name',
//             $ind['designation date 2'] => 'designation date',          
            
            
            //Compliance of treatment plant under expired and pending deadline',
            //compliance Art. 4',
            $ind['art4_treatment type'] => 'treatment type',
            $ind['art4_monitoring'] => 'monitoring',
            $ind['art4_final'] => 'final',
            //compliance Art. 5',
            $ind['art5_treatment type'] => 'treatment type',
            $ind['art5_monitoring'] => 'monitoring',
            $ind['art5_final'] => 'final',
            //Compliance of treatment plant under expired deadlines',
            //compliance Art. 4',
            $ind['art4_expired_treatment type'] => 'treatment type',
            $ind['art4_expired_monitoring'] => 'monitoring',
            $ind['art4_expired_final'] => 'final',
            //compliance Art. 5',
            $ind['art5_expired_treatment type'] => 'treatment type',
            $ind['art5_expired_monitoring'] => 'monitoring',
            $ind['art5_expired_final'] => 'final',
            //Previous compliance of the treatment under expired deadlines',
            //compliance Art. 4',
            $ind['previous_art4_expired_treatment']=> 'treatment type',
            $ind['previous_art4_expired_monitoring'] => 'monitoring',
            $ind['previous_art4_expired_final'] => 'final',
            //compliance Art. 5',
            $ind['previous_art5_expired_treatment type'] => 'treatment type',
            $ind['previous_art5_expired_monitoring'] => 'monitoring',
            $ind['previous_art5_expired_final'] => 'final',
            
//             //compliance Art. 4',
//              $ind['previous_art4_expired_treatment'] => 'previous_art4_expired_treatment',
//              $ind['previous_art4_expired_monitoring'] => 'previous_art4_expired_monitoring',
//              $ind['previous_art4_expired_final'] => 'previous_art4_expired_final',
//             //compliance Art. 5',
//              $ind['previous_art5_expired_treatment type'] => 'previous_art5_expired_treatment',
//              $ind['previous_art5_expired_monitoring'] => 'previous_art5_expired_monitoring',
//              $ind['previous_art5_expired_final'] => 'previous_art5_expired_final',

            
            $ind['previous_art5_expired_difference between the previous situation and now'] => 'difference between the previous situation and now',
            //Installations for wastewater treatment in place',
            $ind['Secondary: installed?'] => 'Secondary: installed?',
            $ind['Secondary:  monitoring passed?'] => 'Secondary:  monitoring passed?',
            $ind['Secondary:  FINAL'] => 'Secondary:  FINAL',
            $ind['More stringent:  installed?'] => 'More stringent:  installed?',
            $ind['More stringent: monitoring passed?'] => 'More stringent: monitoring passed?',
            $ind['More stringent:  FINAL'] => 'More stringent:  FINAL',
            //UWWTP incoming and discharged load',
            $ind['Volume of waste water treated m3/y'] => 'Volume of waste water treated m3/y',
            $ind['Incoming loads BOD-tot  t/y'] => 'Incoming loads BOD-tot  t/y',
            $ind['Discharged loads BOD-tot  t/y'] => 'Discharged loads BOD-tot  t/y',
            $ind['BOD rate of removal %'] => 'BOD rate of removal %',
            $ind['average Incoming concentration BOD mg/l'] => 'average Incoming concentration BOD mg/l',
            $ind['average Discharge concentration BOD mg/l'] => 'average Discharge concentration BOD mg/l',
            $ind['Incoming cocentration COD-tot   t/y'] => 'Incoming cocentration COD-tot   t/y',
            $ind['Discharged concentration COD-tot  t/y'] => 'Discharged concentration COD-tot  t/y',
            $ind['COD rate of removal %'] => 'COD rate of removal %',
            $ind['average Incoming concentration COD mg/l'] => 'average Incoming concentration COD mg/l',
            $ind['average Discharge concentration COD mg/l'] => 'average Discharge concentration COD mg/l',
            $ind['Incoming loads N-tot   t/y'] => 'Incoming loads N-tot   t/y',
            $ind['Discharged loads N-tot  t/y'] => 'Discharged loads N-tot  t/y',
            $ind['N rate of removal %'] => 'N rate of removal %',
            $ind['average Incoming concentration N mg/l'] => 'average Incoming concentration N mg/l',
            $ind['average Discharge concentration N mg/l'] => 'average Discharge concentration N mg/l',
            $ind['Incoming cocentration P-tot   t/y'] => 'Incoming cocentration P-tot   t/y',
            $ind['Discharged concentration P-tot  t/y'] => 'Discharged concentration P-tot  t/y',
            $ind['P rate of removal %'] => 'P rate of removal %',
            $ind['average Incoming concentration P mg/l'] => 'average Incoming concentration P mg/l',
            $ind['average Discharge concentration P mg/l'] => 'average Discharge concentration P mg/l',
            ////////////
            $ind['required_rcaaP'] => 'required_rcaaP',
            $ind['required_rcaaN'] => 'required_rcaaN',
            $ind['required_rcab'] => 'required_rcab', 
            $ind['required_rcac'] => 'required_rcac',  
        );
        
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');  
    }
//     dsm($header);
    

    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {
        
//         $uwwtp = node_load($niduwwtp);
        $uwwtp = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
//         dsm($uwwtp->field_uwwaggliste['und']);
        //a uwwtp can be attach to multiple agglo
        
        if (!isset($context['sandbox']['list_uwwtp_treated'][$agg->field_inspireidlocalid['und'][0]['value']])) {
            $context['results']['country_volume_waste_water'] += $uwwtp->field_uwwwastewatertreated['und'][0]['value'];
        }
        
        $context['sandbox']['list_uwwtp_treated'][$agg->field_inspireidlocalid['und'][0]['value']] = 1;
        
        foreach($uwwtp->field_uwwaggliste['und'] as $nidagg) {
       
            $line = array();
//             dsm($nidagg);
            $agg = node_load($nidagg['nid']);
            //Agglomeration info
            //'code',
            $line[$ind['aggcode']] = $agg->field_inspireidlocalid['und'][0]['value'];
            //'name',
            $line[$ind['aggname']] = $agg->title;
            //'status',
            $line[$ind['aggstatus']] = $agg->field_status['und'][0]['value'] == 1 ? 'active' : 'inactive';
            //'generated load  p. e.',     
            $line[$ind['generated load  p. e.']] = $agg->field_agggenerated['und'][0]['value'];
            //'deadline for Art. 3 compliance',    
            $line[$ind['deadline for Art. 3 compliance']] = substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10);
            //'deadline for Art. 4 compliance',
            $line[$ind['deadline for Art. 4 compliance']] = substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10);
            //'deadline for Art. 5 compliance',
            $line[$ind['deadline for Art. 5 compliance']] = substr($agg->field_aggperiodover5['und'][0]['value'], 0, 10);
            
            //Collection
            //'collected in collecting system % of gen. load',
            $line[$ind['collected in collecting system % of gen. load']] = $agg->field_aggc1['und'][0]['value']; //%
            //'collected in collecting system  p. e.',
            $line[$ind['collected in collecting system  p. e.']] = $agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe
            //'addressed through IAS % of gen. load', 
            $line[$ind['addressed through IAS % of gen. load']] = $agg->field_aggc2['und'][0]['value']; //%
            //'addressed through IAS p. e.',        
            $line[$ind['addressed through IAS p. e.']] = $agg->field_aggc2['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe
            //'neither collecting system nor IAS % of gen. load',      
            $line[$ind['neither collecting system nor IAS % of gen. load']] = $agg->field_aggpercwithouttreatment['und'][0]['value']; //%
            //'neither collecting system nor IAS p. e.',   
            $line[$ind['neither collecting system nor IAS p. e.']] = $agg->field_aggpercwithouttreatment['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe
            
            //UWWTP general info            
            //'code',
            $line[$ind['uww_code']] = $uwwtp->field_inspireidlocalid['und'][0]['value'];
            //'name',
            $line[$ind['uww_name']] = $uwwtp->title;
            //'status',
            $line[$ind['uww_status']] = $uwwtp->field_status['und'][0]['value'] == 1 ? 'active' : 'inactive';

            //load the agg_uwwtp node to get the percentage enering the uwwtp for this agglo
//             dsm($agg);
//             dsm(array('year' => $annee, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value']));
            $siteid = uwwtd_get_siteid('agglo_uww', array('year' => $annee, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value']));
//             dsm($siteid);
            $nidagglouwwtp = uwwtd_check_exist($siteid);
            if ('' != $nidagglouwwtp) {
                $agglo_uwwtp = node_load($nidagglouwwtp);
    //             dsm($agglo_uwwtp);
                //'entering this UWWTP % of gen. load',
                $uww_entering_load_pe = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * ($agg->field_aggc1['und'][0]['value']/ 100 * $agg->field_agggenerated['und'][0]['value']);
                $line[$ind['uww_entering this UWWTP % of gen. load']] = $uww_entering_load_pe * 100 / $agg->field_agggenerated['und'][0]['value'];              
                //'entering load  p. e.',
                $line[$ind['uww_entering load  p. e.']] = $uww_entering_load_pe;
                unset($agglo_uwwtp, $siteid);
            } else {
                $line[$ind['uww_entering this UWWTP % of gen. load']] = '';              
                //'entering load  p. e.',
                $line[$ind['uww_entering load  p. e.']] = ''; 
            }                        
            //'capacity p. e.',
            $line[$ind['uww_capacity p. e.']] = $uwwtp->field_physicalcapacityactivity['und'][0]['value'];
            //'status', 
            $line[$ind['uww2_status']] = $uwwtp->field_status['und'][0]['value'] == 1 ? 'active' : 'inactive'; //...again...
            
            //Previous UWWTP general info
            $previousagglonid = uwwtd_check_exist(uwwtd_get_siteid('agglomeration', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'])));
            $previousuwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $context['sandbox']['previous_year'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
            $previousagglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
//             dsm(array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value']));
//             dsm($previousagglonid);
//             dsm(array('year' => $context['sandbox']['previous_year'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value']));
//             dsm($previousuwwtpnid);
//             dsm(array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value']));
//             dsm($previousagglo_uwwtpnid);
            $previous_reference_year = ''; 
            $previous_entering_load_pe = '';
            $previous_capacity_pe = '';   
            $previous_percentage_of_change_of_entering_load = '';
            $previous_percentage_of_change_of_capacity = '';            
            if ($previousuwwtpnid != '' && $previousagglonid != '' && $previousagglo_uwwtpnid != '') {
                $previousuwwtp = node_load($previousuwwtpnid);
                $previousagglo_uwwtp = node_load($previousagglo_uwwtpnid);
                $previousagglo = node_load($previousagglonid);
//                 dsm($previousuwwtp);
//                 dsm($previousagglo_uwwtp);
//                 dsm($previousagglo);
                $previous_reference_year = $context['sandbox']['previous_year']; 
//                 $previous_entering_load_pe = $previousagglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $previousagglo->field_agggenerated['und'][0]['value'];
                $previous_entering_load_pe = $previousagglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * ($previousagglo->field_aggc1['und'][0]['value']/ 100 * $previousagglo->field_agggenerated['und'][0]['value']);
                $previous_capacity_pe = $previousuwwtp->field_physicalcapacityactivity['und'][0]['value'];
                if (isset($line[$ind['uww_entering load  p. e.']])) {
                    $previous_percentage_of_change_of_entering_load = $previous_entering_load_pe * 100 / $line[$ind['uww_entering load  p. e.']];
                }
                if (isset($line[$ind['uww_capacity p. e.']])) {
                    $previous_percentage_of_change_of_capacity = $previous_capacity_pe * 100 / $line[$ind['uww_capacity p. e.']];
                }
//                 unset($previousuwwtp);
//                 unset($previousagglo_uwwtp);
//                 unset($previousagglo);
            }
            
            //'reference year',
            $line[$ind['previous_reference year']] = $previous_reference_year;
            //'entering load p. e.',
            $line[$ind['previous_entering load p. e.']] = $previous_entering_load_pe;
            //'capacity p. e.',
            $line[$ind['previous_capacity p. e.']] = $previous_capacity_pe;
            //'Percentage of change of entering load (1) %',
            $line[$ind['previous_Percentage of change of entering load (1) %']] = $previous_percentage_of_change_of_entering_load;
            //'Percentage of change of capacity (2) %',
            $line[$ind['previous_Percentage of change of capacity (2) %']] = $previous_percentage_of_change_of_capacity;            
            
            //current RCA details
            //a uwwtp can have multiple dcp and rca, thi case is not documented, so we put all info
            //on each rca separated by a ';'
            $dcpWaterBodyType = '';
            $dcpIrrigation = '';
            $dcpWaterbodyID = '';
            $dcpWFDRBD = '';
            $dcpTypeOfReceivingArea = '';
            $rcaID = '';
            $rcaName = '';
            $rcaName = '';
            $rcaDateArt54 = '';
            $rcaDateArt58 = '';
            $rcatreated = array();
            $startingDateOfApplication_an = '';
            $startingDateOfApplication_ap = '';
            $startingDateOfApplication_b = '';
            $startingDateOfApplication_c = '';
            foreach($uwwtp->field_uwwdcpliste['und'] as $dcpnid) {
                $dcp = node_load($dcpnid['nid']);
                
                foreach($dcp->field_dcprcaliste['und'] as $rcanid) {
                    $rca = node_load($rcanid['nid']);
                    
                    if (isset($rcatreated[$rca->field_inspireidlocalid['und'][0]['value']])) {
                        continue;
                    }
                    $rcatreated[$rca->field_inspireidlocalid['und'][0]['value']] = true;
                    
                    $dcpWaterBodyType .= ($dcpWaterBodyType == '' ? '' : ';') . $dcp->field_dcpwaterbodytype['und'][0]['value'];
                    $dcpIrrigation .= ($dcpIrrigation == '' ? '' : ';') . $dcp->field_dcpirrigation['und'][0]['value'];
                    $dcpWaterbodyID .= ($dcpWaterbodyID == '' ? '' : ';') . $dcp->field_dcpwaterbodyid['und'][0]['value'];
                    $dcpWFDRBD .= ($dcpWFDRBD == '' ? '' : ';') . $dcp->field_dcpwfdrbd['und'][0]['value'];
                    $dcpTypeOfReceivingArea .= ($dcpTypeOfReceivingArea == '' ? '' : ';') . $dcp->field_rcatype['und'][0]['value'];   
                                        
                    $rcaID .= ($rcaID == '' ? '' : ';') . $rca->field_inspireidlocalid['und'][0]['value'];
                    $rcaName .= ($rcaName == '' ? '' : ';') . $rca->title;
                    $rcaDateArt54 .= ($rcaDateArt54 == '' ? '' : ';') . substr($rca->field_rcadateart54['und'][0]['value'], 0, 10);
                    $rcaDateArt58 .= ($rcaDateArt58 == '' ? '' : ';') . substr($rca->field_rcadateart5854['und'][0]['value'], 0, 10);
                    
                    $startingDateOfApplication_an = ($startingDateOfApplication_an == '' ? '' : ';') . (isset($rca->field_rcaapdatedesignation_value['und'][0]['value']) ? substr($rca->field_rcaapdatedesignation_value['und'][0]['value'], 0, 10)  : '-');
                    $startingDateOfApplication_ap = $startingDateOfApplication_an;
                    $startingDateOfApplication_b = $startingDateOfApplication_an;
                    $startingDateOfApplication_c = ($startingDateOfApplication_c == '' ? '' : ';') . (isset($rca->field_rcacdateotherdirective['und'][0]['value']) ? substr($rca->field_rcacdateotherdirective['und'][0]['value'], 0, 10)  : '-');                 
                                        
                    unset($rca);
                }     
                unset($dcp);           
            }
            unset($rcatreated);
            
            //'water body type',   ?dcpWaterBodyType
            $line[$ind['water body type']] = $dcpWaterBodyType;
            //'In case of discharge on land please specify the purpose',  ?dcpIrrigation
            $line[$ind['In case of discharge on land please specify the purpose']] = $dcpIrrigation;
            //'Id code of the water body WFD',    ?dcpWaterbodyID
            $line[$ind['Id code of the water body WFD']] = $dcpWaterbodyID;
            //'ID of WFD river basin district',    ?dcpWFDRBD
            $line[$ind['ID of WFD river basin district']] = $dcpWFDRBD;
            //'entirely discharging into coastal waters?',
            $WBT = explode(';', $dcpWaterBodyType);
            $entirely_discharge_in_CW = (false !== strpos($dcpWaterBodyType, 'CW') ? 'yes' : 'no');
            foreach($WBT as $oneWBT) {
                if ($oneWBT != 'CW') {
                    $entirely_discharge_in_CW = 'no';
                    break;
                }
            }
            unset($WBT);            
            $line[$ind['entirely discharging into coastal waters?']] = $entirely_discharge_in_CW;  //=yes if column Z= CW?
            //'type of receiving water',   ?dcpTypeOfReceivingArea
            $line[$ind['type of receiving water']] = $dcpTypeOfReceivingArea;
            //'ID of the SA or LSA',   ?rcaID
            $line[$ind['ID of the SA or LSA']] = $rcaID;
            //'name',         ?title
            $line[$ind['rcaname']] = $rcaName;
            //'Starting date of application 5.8.',
            $line[$ind['Starting date of application 5.8.']] = $rcaDateArt58;  
            //'Starting date of application 5.4.',
            $line[$ind['Starting date of application 5.4.']] = $rcaDateArt54;
            //'Starting date of application a N',
            $line[$ind['Starting date of application a N']] = $startingDateOfApplication_an; //'TODO';  field_rcaapdatedesignation_value + 7
            //'Starting date of application a P',
            $line[$ind['Starting date of application a P']] = $startingDateOfApplication_ap; //'TODO';  field_rcaapdatedesignation_value + 7
            //'Starting date of application b',     
            $line[$ind['Starting date of application b']] = $startingDateOfApplication_b; //'TODO';    field_rcaapdatedesignation_value + 7
            //'Starting date of application c',
            $line[$ind['Starting date of application c']] = $startingDateOfApplication_c; //'TODO';    field_rcacdateotherdirective_value + 7
            
            //UWWTP treatment and performance
            //'treatment required',
            $treatment_required = uwwtd_get_register_treatment_required($uwwtp->field_uwwtreatmentrequired['und'][0]['value']);
            
            
            $line[$ind['treatment required']] = $treatment_required;//$uwwtp->field_uwwtreatmentrequired['und'][0]['value'];
            
            $previous_treatment_required = '';
            if (isset($previousuwwtp)) {
                $previous_treatment_required = uwwtd_get_register_treatment_required($previousuwwtp->field_uwwtreatmentrequired['und'][0]['value']);
            }
            $previous_treatment_required = '';
// if (isset($previousuwwtp)) {
//     switch($previousuwwtp->field_uwwtreatmentrequired['und'][0]['value']) {
//         case 'P':
//             $previous_treatment_required = '1';
//             break;
//         case 'S':
//             $previous_treatment_required = '2';
//             break;
//         case 'N':
//             $previous_treatment_required = '3N';
//             break;
//         case 'NP':
//             $previous_treatment_required = '3NP';
//             break;
//         case 'P':
//             $previous_treatment_required = '3P';
//             break;
//         case 'O':
//             $previous_treatment_required = '3other';
//             break;
//         case 'Appropriate':
//             $previous_treatment_required = 'Appropriate';
//             break;  
//         case 'NI':
//             $previous_treatment_required = '0';
//             break; 
//         default:  
//             $previous_treatment_required = '';                                                                                                                                         
//     }            
// }
            //'treatment type in place (4)',
            /*            
            IIf([uwwPRemoval]=True And [uwwNRemoval]=True,
                "3NP",
                IIf([uwwPRemoval]=True And [uwwNRemoval]=False,
                    "3P",
                    IIf([uwwPRemoval]=False And [uwwNRemoval]=True,
                        "3N",
                        IIf([uwwOtherTreatment]=True And [uwwNRemoval]=False And [uwwPRemoval]=False,
                            "3other",
                            IIf([uwwPRemoval]=False And [uwwNRemoval]=False And [uwwOtherTreatment]=False And [uwwSecondaryTreatment]=True,
                                2,
                                IIf([uwwPRemoval]=False And [uwwNRemoval]=False And [uwwOtherTreatment]=False And [uwwSecondaryTreatment]=False And [uwwPrimaryTreatment]=True,
                                    1,
                                    IIf([uwwPRemoval]=False And [uwwNRemoval]=False And [uwwOtherTreatment]=False And [uwwSecondaryTreatment]=False And [uwwPrimaryTreatment]=False,
                                        0,
                                        "error"))))))) 
            */
//             $treatmenttypeinplace = '';
//                  
//             if ($node->field_uwwpremoval['und'][0]['value'] == '1' && $node->field_uwwnremoval['und'][0]['value'] = '1') {
//                 $treatmenttypeinplace = '3NP';
//             } elseif ($node->field_uwwpremoval['und'][0]['value'] == '1' && $node->field_uwwnremoval['und'][0]['value'] = '0') {
//                 $treatmenttypeinplace = '3P';
//             } elseif ($node->field_uwwpremoval['und'][0]['value'] == '0' && $node->field_uwwnremoval['und'][0]['value'] = '1') {
//                 $treatmenttypeinplace = '3N';
//             } elseif ($node->field_uwwothertreat['und'][0]['value'] == '1' && $node->field_uwwpremoval['und'][0]['value'] == '0' && $node->field_uwwnremoval['und'][0]['value'] = '0') {
//                 $treatmenttypeinplace = '3other';
//             } elseif ($node->field_uwwsecondarytreatment['und'][0]['value'] == '1' && $node->field_uwwothertreat['und'][0]['value'] == '0' && $node->field_uwwpremoval['und'][0]['value'] == '0' && $node->field_uwwnremoval['und'][0]['value'] = '0') {
//                 $treatmenttypeinplace = '2';
//             } elseif ($node->field_uwwprimarytreatment['und'][0]['value'] == '1' && $node->field_uwwsecondarytreatment['und'][0]['value'] == '0' && $node->field_uwwothertreat['und'][0]['value'] == '0' && $node->field_uwwpremoval['und'][0]['value'] == '0' && $node->field_uwwnremoval['und'][0]['value'] = '0') {
//                 $treatmenttypeinplace = '1';
//             } elseif ($node->field_uwwprimarytreatment['und'][0]['value'] == '0' && $node->field_uwwsecondarytreatment['und'][0]['value'] == '0' && $node->field_uwwothertreat['und'][0]['value'] == '0' && $node->field_uwwpremoval['und'][0]['value'] == '0' && $node->field_uwwnremoval['und'][0]['value'] = '0') {
//                 $treatmenttypeinplace = '0';
//             } else {
//                 $treatmenttypeinplace = 'error';
//             }
             
            
            $treatmenttypeinplace = uwwtd_get_register_treatmenttype_inplace($uwwtp);
            
            $previous_treatmenttypeinplace = '';
            if (isset($previousuwwtp)) {
                $previous_treatmenttypeinplace = uwwtd_get_register_treatmenttype_inplace($previousuwwtp);
            }
            
//             if (isset($previousuwwtp)) {
//                 if ($previousuwwtp->field_uwwprimarytreatment['und'][0]['value'] == '1') {
//                     $previous_treatmenttypeinplace .= ($previous_treatmenttypeinplace == '' ? '': ';' ) .'1';
//                 }
//                 if ($previousuwwtp->field_uwwsecondarytreatment['und'][0]['value'] == '1') {
//                     $previous_treatmenttypeinplace .= ($previous_treatmenttypeinplace == '' ? '': ';' ) .'2';
//                 }
//                 if ($previousuwwtp->field_uwwpremoval['und'][0]['value'] == '1' && $previousuwwtp->field_uwwnremoval['und'][0]['value'] == '1') {
//                     $previous_treatmenttypeinplace .= ($previous_treatmenttypeinplace == '' ? '': ';' ) .'3NP';
//                 } elseif ($previousuwwtp->field_uwwpremoval['und'][0]['value'] == '1' && $previousuwwtp->field_uwwnremoval['und'][0]['value'] == '0') {
//                     $previous_treatmenttypeinplace .= ($previous_treatmenttypeinplace == '' ? '': ';' ) .'3P';
//                 } elseif ($previousuwwtp->field_uwwpremoval['und'][0]['value'] == '0' && $previousuwwtp->field_uwwnremoval['und'][0]['value'] == '1') {
//                     $previous_treatmenttypeinplace .= ($previous_treatmenttypeinplace == '' ? '': ';' ) .'3N';
//                 }
//                 if ($previousuwwtp->field_uwwothertreat['und'][0]['value'] == '1') {
//                     $previous_treatmenttypeinplace .= ($previous_treatmenttypeinplace == '' ? '': ';' ) .'3other';
//                 }
//                 if ($previous_treatmenttypeinplace == '') {
//                     $previous_treatmenttypeinplace .= '0';
//                 }
//             }             
            
                                                                      
            $line[$ind['treatment type in place (4)']] = $treatmenttypeinplace;        
            //$line[$ind['replace_____']] = $uwwtp->field_uwwtreatmenttype['und'][0]['value'];
            //'Monitoring results - BOD5',
            $line[$ind['Monitoring results - BOD5']] = ($uwwtp->field_uwwbod5perf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwbod5perf['und'][0]['value'] == 'F' ? 'Fail' : ''));
            //'Monitoring results -COD',
            $line[$ind['Monitoring results -COD']] = ($uwwtp->field_uwwcodperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwcodperf['und'][0]['value'] == 'F' ? 'Fail' : ''));
            //'Monitoring results -Ntot',
            $line[$ind['Monitoring results -Ntot']] = ($uwwtp->field_uwwntotperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwntotperf['und'][0]['value'] == 'F' ? 'Fail' : ''));
            //'Monitoring results -Ptot',
            $line[$ind['Monitoring results -Ptot']] = ($uwwtp->field_uwwptotperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwptotperf['und'][0]['value'] == 'F' ? 'Fail' : ''));
            //'Monitoring results -other',
            $line[$ind['Monitoring results -other']] = ($uwwtp->field_uwwotherperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwotherperf['und'][0]['value'] == 'F' ? 'Fail' : ''));
            
            if (isset($previousuwwtp)) {    
                $bod5monitoring = ($previousuwwtp->field_uwwbod5perf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwbod5perf['und'][0]['value'] == 'F' ? 'Fail' : ''));
                //'Monitoring results -COD',
                $codmonitoring = ($previousuwwtp->field_uwwcodperf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwcodperf['und'][0]['value'] == 'F' ? 'Fail' : ''));
                //'Monitoring results -Ntot',
                $monitoring_Ntot = ($previousuwwtp->field_uwwntotperf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwntotperf['und'][0]['value'] == 'F' ? 'Fail' : ''));
                //'Monitoring results -Ptot',
                $monitoring_Ptot = ($previousuwwtp->field_uwwptotperf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwptotperf['und'][0]['value'] == 'F' ? 'Fail' : ''));            
            }
            
            //'In cause of failure (3)',            
            $causefailure = '';
            if ($uwwtp->field_uwwbaddesign['und'][0]['value'] == '1') {
                 $causefailure .= ($causefailure == '' ? '' : ';' ) . 'Bad design or dimensioning';
            } 
            if ($uwwtp->field_uwwaccidents['und'][0]['value'] == '1') {
                 $causefailure .= ($causefailure == '' ? '' : ';' ) . 'Major accidents';
            } 
            if ($uwwtp->field_uwwbadperfdesign['und'][0]['value'] == '1') {
                 $causefailure .= ($causefailure == '' ? '' : ';' ) . 'Bad performance';
            }  
            if (isset($uwwtp->field_uwwinformation['und'][0]['value'])) {
                 $causefailure .= ($causefailure == '' ? '' : ';' ) . $uwwtp->field_uwwinformation['und'][0]['value'];
            }                                    
            $line[$ind['In cause of failure (3)']] = $causefailure;
            unset($causefailure);
            //'difference between monitoring result N and P and treatment in place',
            $difference_monitoring = '';
            if (false !== strpos($treatmenttypeinplace, '3NP')) {
                if ($line[$ind['Monitoring results -Ntot']] == 'Pass' && $line[$ind['Monitoring results -Ptot']] == 'Pass') {
                    $difference_monitoring = 'no';
                } else {
                    $difference_monitoring = 'yes';
                }  
            } elseif (false !== strpos($treatmenttypeinplace, '3N')) {
                if ($line[$ind['Monitoring results -Ntot']] == 'Pass') {
                    $difference_monitoring = 'no';
                } else {
                    $difference_monitoring = 'yes';
                }            
            } elseif (false !== strpos($treatmenttypeinplace, '3P')) {
                if ($line[$ind['Monitoring results -Ptot']] == 'Pass') {
                    $difference_monitoring = 'no';
                } else {
                    $difference_monitoring = 'yes';
                }                                                      
            }
            $line[$ind['difference between monitoring result N and P and treatment in place']] = $difference_monitoring;
            
            ////////////////////////////////////////
            //todo check : to delete ?? 
//             $line[$ind['Number of discharge points for this UWWTP']] = 'TODO delete ?';
//             $line[$ind['DCP ID']] = 'TODO delete ?';
//             //current RCA info (Q 2013)
//             $line[$ind['ID of receiving water body 2']] = 'TODO delete ?';
//             $line[$ind['water body type 2']] = 'TODO delete ?';
//             $line[$ind['ID of the SA or LSA 2']] = 'TODO delete ?';
//             $line[$ind['rcaname 2']] = 'TODO delete ?';
//             $line[$ind['designation date 2']] = 'TODO delete ?';       
            //todo check : to delete ??
            ////////////////////////////////////////
            
            //check if uwwtp under expired or pending deadline
            $pending_or_expired = uwwtd_get_register_pending_or_expired_uwwtp($annee,
                                                                              substr($line[$ind['deadline for Art. 3 compliance']], 0, 4),
                                                                              substr($line[$ind['deadline for Art. 4 compliance']], 0, 4),
                                                                              substr($line[$ind['deadline for Art. 5 compliance']], 0, 4),
                                                                              $uwwtp);
                                  
           
            $previous_pending_or_expired = '';
            if (isset($previousuwwtp)) {                 
                $previous_pending_or_expired = uwwtd_get_register_pending_or_expired_uwwtp($annee,
                                                                                  substr($line[$ind['deadline for Art. 3 compliance']], 0, 4),
                                                                                  substr($line[$ind['deadline for Art. 4 compliance']], 0, 4),
                                                                                  substr($line[$ind['deadline for Art. 5 compliance']], 0, 4),
                                                                                  $previousuwwtp);
            }
                                                                                          
// $previous_pending_or_expired = '';
// if (isset($previousuwwtp)) {        
//     if (
//         (substr($line[$ind['deadline for Art. 3 compliance']], 0, 4) < $annee
//          || substr($line[$ind['deadline for Art. 4 compliance']], 0, 4) < $annee
//          || substr($line[$ind['deadline for Art. 5 compliance']], 0, 4) < $annee
//         )
//          && ($previousuwwtp->field_uwwcompliance['und'][0]['value'] == 'NC' || $previousuwwtp->field_uwwcompliance['und'][0]['value'] == 'C' )
//         ) {
//         $previous_pending_or_expired = 'EXP';
//     }
//     
//     //pending deadline    PD + les C avec date pas dépassée
//     if (
//         (substr($line[$ind['deadline for Art. 3 compliance']], 0, 4) >= $annee
//          && substr($line[$ind['deadline for Art. 4 compliance']], 0, 4) >= $annee
//          && substr($line[$ind['deadline for Art. 5 compliance']], 0, 4) >= $annee
//         )
//         && ($previousuwwtp->field_uwwcompliance['und'][0]['value'] == 'PD' || $previousuwwtp->field_uwwcompliance['und'][0]['value'] == 'C' ))
//         ) {
//         $previous_pending_or_expired = 'PD';
//     } 
// }               
         
            

            
            //Compliance of treatment plant under expired and pending deadline
            
            //calculate compliance of art 4 and 5 for this station and this agglo
            $uwwtp_compliance = uwwtd_article4_5_6_station_compliance($agg, $uwwtp);
            
            $previous_uwwtp_compliance = array();
            if (isset($previousagglo) && isset($previousuwwtp)) {
                $previous_uwwtp_compliance = uwwtd_article4_5_6_station_compliance($previousagglo, $previousuwwtp);
            }
                 
            //compliance Art. 4    //secondary treatment in place
            //'treatment type', //Treatment met
            
            // =IF(
            //     OR(‘generated load ‘<2000; 
            //         ‘neither collecting system nor IAS’ =100; 
            //         ‘entering this UWWTP’=0; 
            //         ‘deadline for Art. 4 compliance’ >’reference date’; 
            //         ‘UWWTP status’=”inactive”; 
            //         AND(
            //             OR(‘water body type’="to coastal water"; 
            //                ‘water body type’="on land (catchment of coastal water)"
            //                ); 
            //             ‘generated load’ <=10000
            //             )
            //         );
            //         "NR";
            //         IFERROR(IF(SEARCH("1";’treatment type’)>0;
            //                     "NC"
            //                   );
            //                     IFERROR(IF(SEARCH("0";’treatment type’)>0;
            //                                 "NC"
            //                                );
            //                                 "C"
            //                             )
            //                 )
            //     )            

            $art4_treatment_type = uwwtd_get_register_art4_treatement_type_compliance($treatment_required, $treatmenttypeinplace);
            
            $previous_art4_treatment_type = '';
            if (isset($previousuwwtp)) {
                $previous_art4_treatment_type = uwwtd_get_register_art4_treatement_type_compliance($previous_treatment_required, $previous_treatmenttypeinplace);
            }
            
//             if ($uwwtp->field_uwwtreatment_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['treatementmet']['false']) {
//                 $line[$ind['art4_treatment type']] = 'TODO';
//             } elseif ($uwwtp->field_uwwtreatment_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['treatementmet']['true']) {
//                 $line[$ind['art4_treatment type']] = 'TODO';
//             } elseif ($uwwtp->field_uwwtreatment_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['treatementmet']['not_calculable']) {
//                 $line[$ind['art4_treatment type']] = 'TODO';
//             } else {
//                 $line[$ind['art4_treatment type']] = 'TODO';
//             }

            //'monitoring',     //Performance met
            
            // =IF(OR(‘generated load’ <2000; 
            //        ‘neither collecting system nor IAS’ =100; 
            //        ‘entering this UWWTP’=0;
            //        ’deadline for Art. 4 compliance’ >”reference date”;
            //        ’ Monitoring results - BOD5’="reported as NR"; 
            //        ‘Monitoring results –COD’="reported as NR"; 
            //        ‘Compliance Art. 4 – treatment type’="NR");
            //        "NR";
            //        IF(OR(; ’Monitoring results - BOD5’="fail”; 
            //                ‘Monitoring results - BOD5’=” not reported"; 
            //                ‘Monitoring results –COD’="fail"; 
            //                ‘Monitoring results –COD’="not reported"; 
            //                ‘Monitoring results - BOD5’="--"; 
            //                ‘Monitoring results –COD’="--");
            //                "NC";
            //                IF(AND(‘Monitoring results - BOD5’="pass"; 
            //                       ‘Monitoring results –COD’="pass");
            //                       "C";
            //                       "NC")
            //          )
            //    )
   
            
            $art4_monitoring = uwwtd_get_register_art4_monitoring_compliance($treatment_required, $line[$ind['Monitoring results - BOD5']], $line[$ind['Monitoring results -COD']]);
            
            $previous_art4_monitoring = '';
            if (isset($previousuwwtp)) {
                $previous_art4_monitoring = uwwtd_get_register_art4_monitoring_compliance($previous_treatment_required, $bod5monitoring, $codmonitoring);
            } 
            
//             if ($uwwtp->field_uwwperformance_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['performancemet']['false']) {
//                 $line[$ind['art4_monitoring']] = 'TODO';
//             } elseif ($uwwtp->field_uwwperformance_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['performancemet']['true']) {
//                 $line[$ind['art4_monitoring']] = 'TODO';
//             } elseif ($uwwtp->field_uwwperformance_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['performancemet']['not_calculable']) {
//                 $line[$ind['art4_monitoring']] = 'TODO';
//             } else {
//                 $line[$ind['art4_monitoring']] = 'TODO';
//             }   
         
            //'final',
            // =IF(OR(‘Compliance Art. 4 – treatment type’="NC";
            //        ’Compliance Art. 4 – monitoring results’="NC");
            //        "NC";
            //        IF(OR(‘Compliance Art. 4 – treatment type’="NR";
            //              ’Compliance Art. 4 – monitoring results’="NR");
            //              "NR";
            //              IF(AND(‘Compliance Art. 4 – treatment type’="C";
            //                     ’Compliance Art. 4 – monitoring results’="C");
            //                     "C";
            //                     "EXIT")))
            $art4_final = '';
            if ($line[$ind['art4_treatment type']] == 'NC' || $line[$ind['art4_monitoring']] == 'NC') {
                $art4_final = 'NC';
            } elseif ($line[$ind['art4_treatment type']] == 'NR' || $line[$ind['art4_monitoring']] == 'NR') {
                $art4_final = 'NR';
            } elseif ($line[$ind['art4_treatment type']] == 'C' && $line[$ind['art4_monitoring']] == 'C') {
                $art4_final = 'C';
            }

             
            //compliance Art. 5   //more stringent treatment in place
            //'treatment type',
            $art5_treatment = uwwtd_get_register_art5_treatement_type_compliance($treatment_required, $treatmenttypeinplace);
          
            $previous_art5_treatment = '';
            if (isset($previousuwwtp)) {
                $previous_art5_treatment = uwwtd_get_register_art5_treatement_type_compliance($previous_treatment_required, $previous_treatmenttypeinplace);
            }
            
            //'monitoring',
            $art5_monitoring = uwwtd_get_register_art5_monitoring_compliance($treatment_required, $line[$ind['Monitoring results -Ntot']], $line[$ind['Monitoring results -Ptot']]);
            
            $previous_art5_monitoring = '';
            if (isset($previousuwwtp)) {
                $previous_art5_monitoring = uwwtd_get_register_art5_monitoring_compliance($previous_treatment_required, $monitoring_Ntot, $monitoring_Ptot);
            }
            
            //'final',
            $art5_final =  $uwwtp_compliance['compArt5'];
            
            //save date in array
            if ($pending_or_expired == 'PD' || $pending_or_expired == 'EXP') {
                $line[$ind['art4_treatment type']] = $art4_treatment_type;
                $line[$ind['art4_monitoring']] = $art4_monitoring;
                $line[$ind['art4_final']] = $uwwtp_compliance['compArt4'];            
                $line[$ind['art5_treatment type']] = $art5_treatment;
                $line[$ind['art5_monitoring']] = $art5_monitoring;
                $line[$ind['art5_final']] = $uwwtp_compliance['compArt5'];                
            } else {
                $line[$ind['art4_treatment type']] = '';
                $line[$ind['art4_monitoring']] = '';            
                $line[$ind['art4_final']] = '';            
                $line[$ind['art5_treatment type']] = '';
                $line[$ind['art5_monitoring']] = '';
                $line[$ind['art5_final']] = '';                     
            } 
            
            
            //Compliance of treatment plant under expired deadlines
            if ($pending_or_expired == 'EXP') {
                $line[$ind['art4_expired_treatment type']] = $art4_treatment_type;
                $line[$ind['art4_expired_monitoring']] = $art4_monitoring;
                $line[$ind['art4_expired_final']] = $uwwtp_compliance['compArt4'];            
                $line[$ind['art5_expired_treatment type']] = $art5_treatment;
                $line[$ind['art5_expired_monitoring']] = $art5_monitoring;
                $line[$ind['art5_expired_final']] = $uwwtp_compliance['compArt5'];                
            } else {
                $line[$ind['art4_expired_treatment type']] = '';
                $line[$ind['art4_expired_monitoring']] = '';            
                $line[$ind['art4_expired_final']] = '';            
                $line[$ind['art5_expired_treatment type']] = '';
                $line[$ind['art5_expired_monitoring']] = '';
                $line[$ind['art5_expired_final']] = '';                     
            }             

             
            //Compliance of treatment plant under expired deadlines
//             $art4_expired_treatment = '';
//             $art4_expired_monitoring = '';
//             $art4_expired_final = '';
//             $art5_expired_treatment = '';
//             $art5_expired_monitoring = '';
//             $art5_expired_final = '';
//             if ($uwwtp->field_uwwcompliance['und'][0]['value'] != 'PD') {
//                 $art4_expired_treatment = $line[$ind['art4_treatment type']];
//                 $art4_expired_monitoring = $line[$ind['art4_monitoring']];
//                 $art4_expired_final = $line[$ind['art4_final']];
//                 $art5_expired_treatment = $line[$ind['art5_treatment type']];
//                 $art5_expired_monitoring = $line[$ind['art5_monitoring']];
//                 $art5_expired_final = $line[$ind['art5_final']];
//             }     
            //compliance Art. 4 
            //'treatment type',
//             $line[$ind['art4_expired_treatment type']] = $art4_expired_treatment;
//             //'monitoring',
//             $line[$ind['art4_expired_monitoring']] = $art4_expired_monitoring;
//             //'final',
//             $line[$ind['art4_expired_final']] = $art4_expired_final;              
//             //compliance Art. 5 
//             //'treatment type',
//             $line[$ind['art5_expired_treatment type']] = $art5_expired_treatment;
//             //'monitoring',
//             $line[$ind['art5_expired_monitoring']] = $art5_expired_monitoring;
//             //'final',
//             $line[$ind['art5_expired_final']] = $art5_expired_final;
            
            unset($uwwtp_compliance);
             
            //Previous compliance of the treatment under expired deadlines
//             $previous_art4_expired_treatment = 'TODO';
//             $previous_art4_expired_monitoring = 'TODO';
//             $previous_art4_expired_final = 'TODO';
//             $previous_art5_expired_treatment = 'TODO';
//             $previous_art5_expired_monitoring = 'TODO';
//             $previous_art5_expired_final = 'TODO';
//             $previous_art5_expired_difference = 'TODO';
//             if (false !== $previousagglo_uwwtpnid && isset($previousuwwtp)) {
//                 $previous_art4_expired_treatment = 'TODO';
//                 $previous_art4_expired_monitoring = 'TODO';
//                 $previous_art4_expired_final = 'TODO';
//                 $previous_art5_expired_treatment = 'TODO';
//                 $previous_art5_expired_monitoring = 'TODO';
//                 $previous_art5_expired_final = 'TODO';  
//                 $previous_art5_expired_difference = 'TODO';          
//             }
//             //'treatment type', 
//             $line[$ind['previous_art4_expired_treatment']] = $previous_art4_expired_treatment; 
//             //'monitoring',
//             $line[$ind['previous_art4_expired_monitoring']] = $previous_art4_expired_monitoring;
//             //'final', 
//             $line[$ind['previous_art4_expired_final']] = $previous_art4_expired_final;
//             //'treatment type', 
//             $line[$ind['previous_art5_expired_treatment type']] = $previous_art5_expired_treatment;
//             //'monitoring', 
//             $line[$ind['previous_art5_expired_monitoring']] = $previous_art5_expired_monitoring;
//             //'final', 
//             $line[$ind['previous_art5_expired_final']] = $previous_art5_expired_final;
//              
//             //'difference between the previous situation and now',
//             $line[$ind['previous_art5_expired_difference between the previous situation and now']] = $previous_art5_expired_difference;
            
            $line[$ind['previous_art4_expired_treatment']] = '';
            $line[$ind['previous_art4_expired_monitoring']] = '';            
            $line[$ind['previous_art4_expired_final']] = '';            
            $line[$ind['previous_art5_expired_treatment type']] = '';
            $line[$ind['previous_art5_expired_monitoring']] = '';
            $line[$ind['previous_art5_expired_final']] = '';      
            $line[$ind['previous_art5_expired_difference between the previous situation and now']] = '';            
            if (isset($previousuwwtp)) {
//                 $previous_art4_treatment_type = '';
//                 $previous_art4_monitoring = '';
//                 $previous_art5_treatment = '';
//                 $previous_art5_monitoring = '';
//                 $previous_art5_expired_difference = '';
                
                if ($previous_pending_or_expired == 'EXP') {
                    $line[$ind['previous_art4_expired_treatment']] = $previous_art4_treatment_type;
                    $line[$ind['previous_art4_expired_monitoring']] = $previous_art4_monitoring;
                    $line[$ind['previous_art4_expired_final']] = $previous_uwwtp_compliance['compArt4'];            
                    $line[$ind['previous_art5_expired_treatment type']] = $previous_art5_treatment;
                    $line[$ind['previous_art5_expired_monitoring']] = $previous_art5_monitoring;
                    $line[$ind['previous_art5_expired_final']] = $previous_uwwtp_compliance['compArt5'];
                    
                    if ($line[$ind['previous_art4_expired_final']] != $line[$ind['art4_expired_final']]     
                        || $line[$ind['previous_art5_expired_final']] != $line[$ind['art5_expired_final']]) {
                            $previous_art5_expired_difference = 'yes';
                    } else {
                        $previous_art5_expired_difference = 'no';
                    }
                    
                    $line[$ind['previous_art5_expired_difference between the previous situation and now']] = $previous_art5_expired_difference;                
                }
            }            
            
            if (isset($previous_uwwtp_compliance)) {
                unset($previous_uwwtp_compliance);
            }
            if (isset($previousuwwtp)) {
                unset($previousuwwtp);
            }
            if (isset($previousagglo_uwwtp)) {
                unset($previousagglo_uwwtp);
            }
            if (isset($previousagglo)) {
                unset($previousagglo);
            }                        

             
            //Installations for wastewater treatment in place
            //'Secondary: installed?',
            /*
            =IF((‘generated load’ <2000);
                    "NR";
                    IFERROR (
                            IF(SEARCH("1";’treatment type’)>0;
                                "0"
                            );                 
                            IFERROR(
                                    IF(SEARCH("0";’treatment type’)>0;
                                        "0"
                                      );
                                    "1"
                                   )
                            )
                ) 
            */            
//             if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
//                 $secondaryinstalled = 'NR';
//             } elseif ($uwwtp->field_uwwtreatmenttype['und'][0]['value'] == 'MS' || $uwwtp->field_uwwtreatmenttype['und'][0]['value']== 'S') {
//                 $secondaryinstalled = '1';
//             } else {
//                 $secondaryinstalled = '0';
//             }   
            
//             dsm('$treatmenttypeinplace : ' . $treatmenttypeinplace);
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $secondaryinstalled = 'NR';
            } elseif (false !== strpos($treatmenttypeinplace, '2')) {
                $secondaryinstalled = '1';
            } else {
                $secondaryinstalled = '0';
            }
//             dsm('$secondaryinstalled:'.$secondaryinstalled);
               
            $line[$ind['Secondary: installed?']] = $secondaryinstalled; 
            //'Secondary:  monitoring passed?',
            /*            
            =IF(‘generated load’ <2000;
                "NR";
                IF(‘Secondary – installed’ =0;
                    0;
                    IF(AND(‘Monitoring results - BOD5’="pass"; 
                           ‘Monitoring results –COD’);
                        1;
                        0))) 
            */            
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $secondarypass = 'NR';
            } elseif ($secondaryinstalled == '0') {
                $secondarypass = '0';                            
            } elseif ($uwwtp->field_uwwbod5perf['und'][0]['value'] == 'P' && $uwwtp->field_uwwcodperf['und'][0]['value'] == 'P') {
                $secondarypass = '1';
            } else {
                $secondarypass = '0';
            }                       
            $line[$ind['Secondary:  monitoring passed?']] = $secondarypass; 
            //'Secondary:  FINAL',
            /*            
            =IF(‘generated load’ <2000;
                "NR"; 
                ‘Secondary – installed’ * ‘Secondary - monitoring passed?’)
            */    
            $secondaryfinal = ($secondaryinstalled == '1' && $secondarypass == '1' ? '1': '0');
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $secondaryfinal = 'NR';
            }            
            $line[$ind['Secondary:  FINAL']] = $secondaryfinal; 
            //'More stringent:  installed?',
            /*            
            =IF(OR(‘neither collecting system nor IAS’ =100; 
                   ‘entering this UWWTP’ =0);
                   0;
                   IF(‘generated load’ <2000;
                    "NR";
                    IFERROR(IF(SEARCH("3";’treatment type’)>0;
                                1);
                            0)))
            */                
            $morestringentinstalled = '0';
//             if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
//                 $morestringentinstalled = 'NR';
//             }elseif ($uwwtp->field_uwwtreatmenttype['und'][0]['value'] == 'MS') {
//                 $morestringentinstalled = '1';
//             }  
            
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $morestringentinstalled = 'NR';
            } elseif (false !== strpos($treatmenttypeinplace, '3')) {
                $morestringentinstalled = '1';
            } else {
                $morestringentinstalled = '0';
            }
                                  
            $line[$ind['More stringent:  installed?']] = $morestringentinstalled; 
            //'More stringent: monitoring passed?',
//             /*            
            // Part 1
            // =IF(‘generated load’ <2000;
            //     "NR";
            //     IFERROR(IF(AND(
            //                     FIND("3NP";’treatment type’)>0;
            //                     OR(AND(‘Monitoring results –Ntot’="pass";
            //                            ’Monitoring results –Ptot’="pass");
            //                        AND(‘Monitoring results –Ntot’="pass";
            //                            ’Monitoring results –Ptot’="not relevant";
            //                            ’SECONDARY:  MONITORING PASSED’=1);
            //                        AND(‘Monitoring results –Ntot’="not relevant";
            //                            ’Monitoring results –Ptot’="not relevant";
            //                            ’SECONDARY:  MONITORING PASSED’=1);
            //                        AND(‘Monitoring results –Ntot’="not relevant";
            //                            ’Monitoring results –Ptot’="pass";
            //                            ’SECONDARY:  MONITORING PASSED’=1)
            //                       )
            //                     ) ;
            //                     1;
            //                     0
            //                 );
            //             IFERROR(IF(AND(
            //                             FIND("3N";’treatment type’)>0;
            //                             OR(‘Monitoring results –Ntot’="pass";
            //                                AND(‘Monitoring results –Ntot’="not relevant";
            //                                    ’SECONDARY:  MONITORING PASSED’=1)
            //                                )
            //                           ) ;
            //                         1;
            //                         0); 
            //                       IFERROR(IF(AND(
            //                                      FIND("3P";’treatment type’)>0;
            //                                      OR(‘Monitoring results –Ptot’="pass";
            //                                         AND(‘Monitoring results –Ptot’="not relevant";
            //                                             ’SECONDARY:  MONITORING PASSED’=1)
            //                                         )
            //                                      );
            //                                 1;
            //                                 0);
            //                             "EXIT")
            //                     )
            //          ))
            // Part 2
            // =IF(NOT(‘Part 1’="EXIT");’Part 1’;
            //     IFERROR(IF(AND(FIND("3other";’treatment type’)>0;
            //                    ’SECONDARY:  MONITORING PASSED’=1);
            //                1;
            //                0);
            //             IFERROR(IF(AND(FIND("2";’treatment type’)>0;
            //                            OR(‘Monitoring results –Ntot’="pass";
            //                                ’Monitoring results –Ptot’="pass");
            //                                ’SECONDARY:  MONITORING PASSED’=1
            //                            );
            //                         1;
            //                         0);
            //                       IF(OR(‘treatment type’=0;
            //                             ’treatment type’=1
            //                             );
            //                         0;
            //                         0)
            //                       )
            //               )
            //     )
//             */    
            
            $morestringentpassed = '';
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {   
                $morestringentpassed = 'NR';
            }
            if ($morestringentpassed == '' && false !== strpos($treatmenttypeinplace, '3NP')) {
                if ((
                        $line[$ind['Monitoring results -Ntot']] == 'Pass'
                        && $line[$ind['Monitoring results -Ntot']] == 'Pass'
                    )
                    ||
                    (
                        $line[$ind['Monitoring results -Ntot']] == 'Pass'
                        && $secondarypass == '1'
                    ) 
                    ||
                    (
                        $line[$ind['Monitoring results -Ptot']] == 'Pass'
                        && $secondarypass == '1'
                    ) 
                    ||
                    (
                        $secondarypass == '1'
                    ) 
                    ) {
                    $morestringentpassed = '1';
                } else {
                    $morestringentpassed = '0';
                }             
            } 
            if ($morestringentpassed == '' &&  false !== strpos($treatmenttypeinplace, '3N')) {
                if ($line[$ind['Monitoring results -Ntot']] == 'Pass'
                    || $secondarypass == '1') {
                    $morestringentpassed = '1';
                } else {
                    $morestringentpassed = '0';
                }              
            }
            if ($morestringentpassed == '' && false !== strpos($treatmenttypeinplace, '3P')) {
                if ($line[$ind['Monitoring results -Ptot']] == 'Pass'
                    || $secondarypass == '1') {
                    $morestringentpassed = '1';
                } else {
                    $morestringentpassed = '0';
                }             
            }
            if ($morestringentpassed == '' && false !== strpos($treatmenttypeinplace, '3other')) {
                if ($secondarypass == '1') {
                    $morestringentpassed = '1';
                } else {
                    $morestringentpassed = '0';
                }            
            }
            if ($morestringentpassed == '' && false !== strpos($treatmenttypeinplace, '2')) {
                if ($line[$ind['Monitoring results -Ntot']] == 'Pass' 
                    || $line[$ind['Monitoring results -Ptot']] == 'Pass'
                    || $secondarypass == '1') {
                    $morestringentpassed = '1';
                } else {
                    $morestringentpassed = '0';
                }                    
            }                        
            if ($morestringentpassed == '') {
                $morestringentpassed = '0';
            } 
            
            
            $line[$ind['More stringent: monitoring passed?']] = $morestringentpassed; 
            //'More stringent:  FINAL',
            /*            
            =IF(‘generated load’ <2000;
                "NR"; ‘More stringent  - installed’ * ‘More stringent - monitoring passed’)
            */      
            $morestringentfinal = ($morestringentinstalled == '1' && $morestringentpassed = '1' ? '1': '0');
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $morestringentfinal = 'NR';
            }            
            $line[$ind['More stringent:  FINAL']] = $morestringentfinal;                       
            
            //UWWTP incoming and discharged load
            //'Volume of waste water treated' 'm3/y',     field_uwwwastewatertreated                          
            $line[$ind['Volume of waste water treated m3/y']] = $uwwtp->field_uwwwastewatertreated['und'][0]['value'];
            
            //'Incoming loads BOD-tot  t/y',       field_uwwbodincoming
            $line[$ind['Incoming loads BOD-tot  t/y']] = $uwwtp->field_uwwbodincoming['und'][0]['value'];
            $context['results']['graph']['incoming_load_bod'] += $line[$ind['Incoming loads BOD-tot  t/y']];
            //'Discharged loads BOD-tot  t/y',    field_uwwboddischarge 
            $line[$ind['Discharged loads BOD-tot  t/y']] = $uwwtp->field_uwwboddischarge['und'][0]['value'];
            $context['results']['graph']['discharge_load_bod'] += $line[$ind['Discharged loads BOD-tot  t/y']] ;
            //'BOD rate of removal %',      round(((1- ($uwwtp->field_uwwboddischarge['und'][0]['value'] / $uwwtp->field_uwwbodincoming['und'][0]['value']))*100), 2)
            $line[$ind['BOD rate of removal %']] = (1- ($uwwtp->field_uwwboddischarge['und'][0]['value'] / $uwwtp->field_uwwbodincoming['und'][0]['value']))*100;
            //'average Incoming concentration BOD mg/l', ($uwwtp->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;  
            $line[$ind['average Incoming concentration BOD mg/l']] = ($uwwtp->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            //'average Discharge concentration BOD mg/l', ($uwwtp->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Discharge concentration BOD mg/l']] = ($uwwtp->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;

            //'Incoming cocentration COD-tot   t/y',   field_uwwcodincoming
            $line[$ind['Incoming cocentration COD-tot   t/y']] = $uwwtp->field_uwwcodincoming['und'][0]['value'];
            $context['results']['graph']['incoming_load_cod'] += $line[$ind['Incoming cocentration COD-tot   t/y']];
            //'Discharged concentration COD-tot  t/y',    field_uwwcoddischarge
            $line[$ind['Discharged concentration COD-tot  t/y']] = $uwwtp->field_uwwcoddischarge['und'][0]['value'];
            $context['results']['graph']['discharge_load_cod'] += $line[$ind['Discharged concentration COD-tot  t/y']];
            //'COD rate of removal %',   round(( (1-($uwwtp->field_uwwcoddischarge['und'][0]['value'] / $uwwtp->field_uwwcodincoming['und'][0]['value'])) *100), 2) . '%'
            $line[$ind['COD rate of removal %']] =  ( (1-($uwwtp->field_uwwcoddischarge['und'][0]['value'] / $uwwtp->field_uwwcodincoming['und'][0]['value'])) *100);
            //'average Incoming concentration COD mg/l',  ($uwwtp->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            $line[$ind['average Incoming concentration COD mg/l']] = ($uwwtp->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            //'average Discharge concentration COD mg/l',  ($uwwtp->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Discharge concentration COD mg/l']] = ($uwwtp->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;

            //'Incoming loads N-tot   t/y',   field_uwwnincoming
            $line[$ind['Incoming loads N-tot   t/y']] = $uwwtp->field_uwwnincoming['und'][0]['value'];
            $context['results']['graph']['incoming_load_n'] += $line[$ind['Incoming loads N-tot   t/y']];
            //'Discharged loads N-tot  t/y',   field_uwwndischarge
            $line[$ind['Discharged loads N-tot  t/y']] = $uwwtp->field_uwwndischarge['und'][0]['value'];
            $context['results']['graph']['discharge_load_n'] += $context['results']['graph']['discharge_load_n'];
            //'N rate of removal %',   round(( (1- ($uwwtp->field_uwwndischarge['und'][0]['value'] / $uwwtp->field_uwwnincoming['und'][0]['value'])) *100), 2) . '%'
            $line[$ind['N rate of removal %']] = ( (1- ($uwwtp->field_uwwndischarge['und'][0]['value'] / $uwwtp->field_uwwnincoming['und'][0]['value'])) *100);
            //'average Incoming concentration N mg/l',         ($uwwtp->field_uwwnincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Incoming concentration N mg/l']] = ($uwwtp->field_uwwnincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            //'average Discharge concentration N mg/l',    ($uwwtp->field_uwwndischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Discharge concentration N mg/l']] = ($uwwtp->field_uwwndischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;

            //'Incoming cocentration P-tot   t/y',    field_uwwpincoming
            $line[$ind['Incoming cocentration P-tot   t/y']] = $uwwtp->field_uwwpincoming['und'][0]['value'];
            $context['results']['graph']['incoming_load_p'] += $line[$ind['Incoming cocentration P-tot   t/y']];
            //'Discharged concentration P-tot  t/y',    field_uwwpdischarge
            $line[$ind['Discharged concentration P-tot  t/y']] = $uwwtp->field_uwwpdischarge['und'][0]['value'];
            $context['results']['graph']['discharge_load_p'] += $line[$ind['Discharged concentration P-tot  t/y']];
            //'P rate of removal %',   round(( (1-($uwwtp->field_uwwpdischarge['und'][0]['value'] / $uwwtp->field_uwwpincoming['und'][0]['value'])) *100), 2) . '%'
            $line[$ind['P rate of removal %']] = ( (1-($uwwtp->field_uwwpdischarge['und'][0]['value'] / $uwwtp->field_uwwpincoming['und'][0]['value'])) *100);
            //'average Incoming concentration P mg/l',  ($uwwtp->field_uwwpincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Incoming concentration P mg/l']] = ($uwwtp->field_uwwpincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            //'average Discharge concentration P mg/l', ($uwwtp->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            $line[$ind['average Discharge concentration P mg/l']] = ($uwwtp->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            
            //needed on next sheets :
            //here same test as in tpl node--uwwtp.tpl.php
            if((!isset($uwwtp->field_uwwcollectingsystem['und'][0]['value']) 
                || $uwwtp->field_uwwcollectingsystem['und'][0]['value'] == 'NOTCON') 
               || 
               (!isset($uwwtp->field_physicalcapacityactivity['und'][0]['value']) 
                || $uwwtp->field_physicalcapacityactivity['und'][0]['value'] == '0')){
                $line[$ind['uwwtp_collecting_without_treatment']] = 1;
            } else {
                $line[$ind['uwwtp_collecting_without_treatment']] = 0;
            }
            
            //here field for next sheet
            $line[$ind['required_rcaaP']] = $uwwtp->field_uwww_required_rcaap['und'][0]['value'];
            $line[$ind['required_rcaaN']] = $uwwtp->field_uwww_required_rcaan['und'][0]['value'];
            $line[$ind['required_rcab']]  = $uwwtp->field_uwww_required_rcab['und'][0]['value'];
            $line[$ind['required_rcac']]  = $uwwtp->field_uwww_required_rcac['und'][0]['value'];
            
            //debug value
            $line[$ind['debug_pending_or_expired']] = $pending_or_expired;
            $line[$ind['debug_art4_final']] = $art4_final;
            
            fputcsv($r, $line, ';');
            
            
            foreach ($listtypestat as $typestat) {
                if ( !isset($context['sandbox'][$typestat])) {
                    $context['sandbox'][$typestat]['generated_pe'] = 0;
                    $context['sandbox'][$typestat]['entering_pe'] = 0;
                    $context['sandbox'][$typestat]['capacity_pe'] = 0;
                    $context['sandbox'][$typestat]['previous_entering_pe'] = 0;
                    $context['sandbox'][$typestat]['previous_capacity_pe'] = 0;          
                }
                
                //expired deadline   (toutes les NC +  les C qui ont date dépassée)
                if ($typestat == 'expired' && $pending_or_expired != 'EXP') {        
    //                 if (false === (
    //                     (substr($line[$ind['deadline for Art. 3 compliance']], 0, 4) < $annee
    //                      || substr($line[$ind['deadline for Art. 4 compliance']], 0, 4) < $annee
    //                      || substr($line[$ind['deadline for Art. 5 compliance']], 0, 4) < $annee
    //                     )
    //                      && ($uwwtp->field_uwwcompliance['und'][0]['value'] == 'NC' || $uwwtp->field_uwwcompliance['und'][0]['value'] == 'C' )
    //                     )) {
                        continue;
    //                 }
                }
                
                //pending deadline    PD + les C avec date pas dépassée
                if ($typestat == 'pending' && $pending_or_expired != 'PD') {     
    //                 if (false === (
    //                     ((substr($line[$ind['deadline for Art. 3 compliance']], 0, 4) >= $annee
    //                      && substr($line[$ind['deadline for Art. 4 compliance']], 0, 4) >= $annee
    //                      && substr($line[$ind['deadline for Art. 5 compliance']], 0, 4) >= $annee
    //                     )
    //                     && ($uwwtp->field_uwwcompliance['und'][0]['value'] == 'PD' || $uwwtp->field_uwwcompliance['und'][0]['value'] == 'C' ))
    //                     )) {
                        continue;
    //                 } 
                }
    
                if (!isset($context['sandbox'][$typestat]['list_agglo_treated'][$line[$ind['aggcode']]])) {
                    $context['sandbox'][$typestat]['generated_pe'] += $line[$ind['generated load  p. e.']];
                    
                    $context['sandbox'][$typestat]['list_agglo_treated'][$line[$ind['aggcode']]] = 1;                    
                }
                $context['sandbox'][$typestat]['entering_pe'] +=  $line[$ind['uww_entering load  p. e.']];
                $context['sandbox'][$typestat]['capacity_pe'] +=  $line[$ind['uww_capacity p. e.']];
                $context['sandbox'][$typestat]['previous_entering_pe'] +=  $line[$ind['previous_entering load p. e.']];
                $context['sandbox'][$typestat]['previous_capacity_pe'] +=  $line[$ind['previous_capacity p. e.']];                        
            }                           
        }
         
        unset($uwwtp);
//         dsm($line);
//         return;
//     $context['sandbox']['progress'] = $context['sandbox']['max'];
//         break;        
//         $allline[] = $line;
        $j++;
        $context['sandbox']['progress']++;   
    } 
    fclose($r);
    
    //write last lines with aggregate data
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_total_name'], $annee);
        $r = fopen($filename, 'wb');
        $line = array_pad(array(), count($ind), '');
//         $line = array(
        $line[0] = 'TOTAL';
        $entering_load_pe = $context['sandbox']['expired']['entering_pe'] + $context['sandbox']['pending']['entering_pe']; 
        
        $line[$ind['uww_entering this UWWTP % of gen. load']] = 
             $entering_load_pe * 100 /  ($context['sandbox']['expired']['generated_pe'] + $context['sandbox']['pending']['generated_pe']);
        
        $line[$ind['uww_entering load  p. e.']] = $entering_load_pe;
        $line[$ind['uww_capacity p. e.']] = $context['sandbox']['expired']['capacity_pe'] + $context['sandbox']['pending']['capacity_pe'];
        $line[$ind['previous_entering load p. e.']] = $context['sandbox']['expired']['previous_entering_pe'] + $context['sandbox']['pending']['previous_entering_pe'];
        $line[$ind['previous_capacity p. e.']] = $context['sandbox']['expired']['previous_capacity_pe'] + $context['sandbox']['pending']['previous_capacity_pe'];
        $line[$ind['previous_Percentage of change of entering load (1) %']] = 
            (($line[$ind['uww_entering load  p. e.']] /$line[$ind['previous_capacity p. e.']]) - 1 ) * 100;

        if ($line[$ind['previous_Percentage of change of entering load (1) %']] == -100) {
            $line[$ind['previous_Percentage of change of entering load (1) %']] = '0';
        }

        $line[$ind['previous_Percentage of change of capacity (2) %']] = 
                    (($line[$ind['uww_capacity p. e.']] / $line[$ind['previous_capacity p. e.']]) - 1 ) * 100;        
//         );

        if ($line[$ind['previous_Percentage of change of capacity (2) %']] == -100) {
            $line[$ind['previous_Percentage of change of capacity (2) %']] = '0';
        }
        
        fputcsv($r, $line, ';');
            
        $line = array_pad(array(), count($ind), '');
//         $line = array(
        $line[0] = 'expired_deadline';
        $entering_load_pe = $context['sandbox']['expired']['entering_pe'];
        $line[$ind['uww_entering this UWWTP % of gen. load']] = 
             $entering_load_pe * 100 /  $context['sandbox']['expired']['generated_pe'];
             
        $line[$ind['uww_entering load  p. e.']] = $entering_load_pe;
        $line[$ind['uww_capacity p. e.']] = $context['sandbox']['expired']['capacity_pe'];
        $line[$ind['previous_entering load p. e.']] = $context['sandbox']['expired']['previous_entering_pe'];
        $line[$ind['previous_capacity p. e.']] = $context['sandbox']['expired']['previous_capacity_pe'];
        $line[$ind['previous_Percentage of change of entering load (1) %']] = 
            (($line[$ind['uww_entering load  p. e.']] /$line[$ind['previous_capacity p. e.']]) - 1 ) * 100;

        $line[$ind['previous_Percentage of change of capacity (2) %']] = 
                    (($line[$ind['uww_capacity p. e.']] / $line[$ind['previous_capacity p. e.']]) - 1 ) * 100;    
//         );
        fputcsv($r, $line, ';');
        
        $line = array_pad(array(), count($ind), '');  
//         $line = array(
        $line[0] = 'pending_deadline';
        $entering_load_pe = $context['sandbox']['pending']['entering_pe'];
        $line[$ind['uww_entering this UWWTP % of gen. load']] = 
             $entering_load_pe * 100 /  $context['sandbox']['pending']['generated_pe'];
             
        $line[$ind['uww_entering load  p. e.']] = $entering_load_pe;

        $line[$ind['uww_capacity p. e.']] = $context['sandbox']['pending']['capacity_pe'];
        $line[$ind['previous_entering load p. e.']] = $context['sandbox']['pending']['previous_entering_pe'];
        $line[$ind['previous_capacity p. e.']] = $context['sandbox']['pending']['previous_capacity_pe'];
        $line[$ind['previous_Percentage of change of entering load (1) %']] = 
            (($line[$ind['uww_entering load  p. e.']] /$line[$ind['previous_capacity p. e.']]) - 1 ) * 100;

        $line[$ind['previous_Percentage of change of capacity (2) %']] = 
                    (($line[$ind['uww_capacity p. e.']] / $line[$ind['previous_capacity p. e.']]) - 1 ) * 100;    
//         );
        fputcsv($r, $line, ';');  
        fclose($r);              
    }                                           
    
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }    
}

/**
 *
 */ 
function uwwtd_create_register_2015_rca_sheet1($annee, &$context) 
{
//     dsm('la');
	if (!isset($context['sandbox']['progress'])) {	
		$context['sandbox']['max'] = 1;
		$context['sandbox']['progress'] = 0;
	} 
    $ind = $GLOBALS['uwwtd']['sheet1_rca'];
  
//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['rca']['csv_name']. '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rca']['csv_name'], $annee);
    
    $r = fopen($filename, 'wb');

    //create sensitive area register
    $header = array(
        $ind['id_of_sensitive_area'] => 'ID of Sensitive Area', //; rcaCode 
        $ind['name_of_sensitive_area'] => 'Name of Sensitive Area', //; rcaName
        $ind['zone_type_code'] => 'Zone Type Code', //; rcaType
        $ind['specialised_zone_type_code'] => 'Specialised Zone type Code', //; ?rcaType
        $ind['parameters_subject_to_more_stringent_treatment:_n'] => 'Parameters subject to More Stringent Treatment: N', //; rcaParameterN
        $ind['parameters_subject_to_more_stringent_treatment:_p'] => 'Parameters subject to More Stringent Treatment: P', //; rcaParameterP
        $ind['parameters_subject_to_more_stringent_treatment:_m'] => 'Parameters subject to More Stringent Treatment: m', //; ?
        $ind['parameters_subject_to_more_stringent_treatment:_other'] => 'Parameters subject to More Stringent Treatment: other', //; rcaParameterOther
        $ind['date_of_designation_of_art._5(8)'] => 'Date of designation of Art. 5(8)', //; ?
        $ind['starting_date_of_application_of_art._5(8)'] => 'Starting date of application of Art. 5(8)', //; rcaDateArt58
        $ind['starting_date_of_application_of_art._5(4)'] => 'Starting date of application of Art. 5(4)', //; rcaDateArt54
        $ind['designation_criteria_a_n'] => 'Designation criteria a N', //; rcaANitro
        $ind['starting_date_of_designation_a_n'] => 'Starting date of designation a N', //; ?
        $ind['starting_date_of_application_a_n'] => 'Starting date of application a N', //; ?
        $ind['designation_criteria_a_p'] => 'Designation criteria a P', //; rcaAPhos
        $ind['starting_date_of_designation_a_p'] => 'Starting date of designation a P', //; ?
        $ind['starting_date_of_application_a_p'] => 'Starting date of application a P', //; ?
        $ind['designation_criteria_b'] => 'Designation criteria b', //; rcaB
        $ind['starting_date_of_designation_b'] => 'Starting date of designation', //; ?
        $ind['starting_date_of_application_b'] => 'Starting date of application', //; ?
        $ind['designation_criteria_c'] => 'Designation criteria c', //; rcaC
        $ind['starting_date_of_designation_c'] => 'Starting date of designation', //; ?
        $ind['starting_date_of_application_c'] => 'Starting date of application', //; ?
        $ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'] => 'Designation criteria c - relevant EU-Directives and related parameters', //; ?
        $ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'] => 'Designation criteria c - ID applied to the area according to this Directive', //; ?
        $ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'] => 'Designation criteria c - reference date of the area according to this Directive', //; ?
        $ind['number_of_uwwtps_of_the_article_5.4_area_(1)'] => 'Number of UWWTPs of the article 5.4 area (1)', //; ?rcaPlants
        $ind['total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)'] => 'Total organic design capacity of the UWWTPs (p.e.) of the article 5.4 area (1)', //; ?rcaPlantsCapacity
        $ind['incoming_loads_(t/y)_n-tot_(1)'] => 'Incoming loads (t/y) N-tot (1)', //; rcaNIncomingMeasured + rcaNIncomingCalculated + rcaNIncomingEstimated
        $ind['rate_of_removal_n-tot'] => 'Rate of Removal N-tot', //; ?
        $ind['discharged_loads_(t/y)_n-tot_(1)'] => 'Discharged loads (t/y) N-tot (1)', //; rcaNDischargedMeasured + rcaNDischargedCalculated + rcaNDischargedEstimated 
        $ind['incoming_loads_(t/y)_p-tot_(1)'] => 'Incoming loads (t/y) P-tot (1)', //; rcaPIncomingMeasured + rcaPIncomingCalculated + rcaPIncomingEstimated 
        $ind['rate_of_removal_p-tot'] => 'Rate of Removal P-tot', //; ?
        $ind['discharged_loads_(t/y)_p-tot:_'] => 'Discharged loads (t/y) P-tot: ', //; rcaPDischargedMeasured+ rcaPDischargedCalculated + rcaPDischargedEstimated
        $ind['general_remark'] => 'general Remark', //; rcaRemarks
        $ind['rcabeginlife'] => 'rcaBeginLife', //; ?
        $ind['rcaendlife'] => 'rcaEndLife', //; ? 
        $ind['id_code_of_successor_(2)'] => 'ID code of successor (2)', //; ?
        $ind['id_code_of_predecessor_(2)'] => 'ID code of predecessor (2)', //; ?
        $ind['remark_successor/predecessor_sa'] => 'remark Successor/Predecessor SA', //; ?
        $ind['related_sensitive_area_(2)'] => 'Related sensitive area (2)', //; ?
        $ind['remark_related_sensitive_area_(2)'] => 'Remark Related sensitive area (2)', //; ?
    );

    $rca = uwwtd_get_all_rca($annee);
//     dsm($rca);
//     return;

    fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
    fputcsv($r, $header, ';');
        
//     $allline = array();
    for ($i = 0, $max = count($rca); $i < $max; $i++) {
        if (false === in_array($rca[$i]['field_specialisedzonetype_value'], array('SA', 'LSA', 'CSA', 'A58'))) {
            continue;
        }
        $line = array(
            //'ID of Sensitive Area', //;       rcaCode 
            $ind['id_of_sensitive_area'] => $rca[$i]['field_inspireidlocalid_value'],
            //'Name of Sensitive Area', //;     rcaName
            $ind['name_of_sensitive_area'] => $rca[$i]['title'],
            //'Zone Type Code', //;             rcaType
            $ind['zone_type_code'] => $rca[$i]['field_specialisedzonetype_value'],
            //'Specialised Zone type Code', //;   ?rcaType
            $ind['specialised_zone_type_code'] => 'TODO', 
            //'Parameters subject to More Stringent Treatment: N', //;      rcaParameterN
            $ind['parameters_subject_to_more_stringent_treatment:_n'] => $rca[$i]['field_rca_parameter_n_value'],
            //'Parameters subject to More Stringent Treatment: P', //;      rcaParameterP
            $ind['parameters_subject_to_more_stringent_treatment:_p'] => $rca[$i]['field_rca_parameter_p_value'],
           // 'Parameters subject to More Stringent Treatment: m', //;      ?
            $ind['parameters_subject_to_more_stringent_treatment:_m'] => 'TODO',
            //'Parameters subject to More Stringent Treatment: other', //;   rcaParameterOther
            $ind['parameters_subject_to_more_stringent_treatment:_other'] => $rca[$i]['field_rca_parameter_other_value'],
            //'Date of designation of Art. 5(8)', //;                  ?
            $ind['date_of_designation_of_art._5(8)'] => 'TODO', 
//             $ind['date_of_designation_of_art._5(8)'] => ('' != $rca[$i]['field_rcadateart5854_value'] ?  substr($rca[$i]['field_rcadateart5854_value'], 0, 10) - 7 : ''), 
            //'Starting date of application of Art. 5(8)', //;         rcaDateArt58
            $ind['starting_date_of_application_of_art._5(8)'] => $rca[$i]['field_rcadateart5854_value'],
            //'Starting date of application of Art. 5(4)', //;         rcaDateArt54
            $ind['starting_date_of_application_of_art._5(4)'] => $rca[$i]['field_rcadateart54_value'],
            //'Designation criteria  a N', //;                         rcaANitro
            $ind['designation_criteria_a_n'] => $rca[$i]['field_rcaanitro_value'],
            //'Starting date of designation a N', //;                  ?
//             $ind['starting_date_of_designation_a_n'] => 'TODO',  rcaDateDesignation  $node->field_rcaapdatedesignation['und'][0]['value']
            $ind['starting_date_of_designation_a_n'] => ('' != $rca[$i]['field_rcaapdatedesignation_value'] ?  substr($rca[$i]['field_rcaapdatedesignation_value'], 0, 10) : ''),
            //'Starting date of application a N', //;                  ?
            $ind['starting_date_of_application_a_n'] => 'TODO', // =rcaDateDesignation + 7 years
//             $ind['starting_date_of_application_a_n'] => ('' != $rca[$i]['field_rcaapdatedesignation_value'] ?  substr($rca[$i]['field_rcaapdatedesignation_value'], 0, 10) + 7 : ''), 
            //'Designation criteria  a P', //;                         rcaAPhos
            $ind['designation_criteria_a_p'] => $rca[$i]['field_rcaaphos_value'],
            //'Starting date of designation a P', //;                  ?
//             $ind['starting_date_of_designation_a_p'] => 'TODO',   rcaDateDesignation
            $ind['starting_date_of_designation_a_p'] => ('' != $rca[$i]['field_rcaapdatedesignation_value'] ?  substr($rca[$i]['field_rcaapdatedesignation_value'], 0, 10) : ''),
            //'Starting date of application a P', //;                  ?
            $ind['starting_date_of_application_a_p'] => 'TODO', // =rcaDateDesignation + 7 years
//             $ind['starting_date_of_application_a_p'] => ('' != $rca[$i]['field_rcaapdatedesignation_value'] ?  substr($rca[$i]['field_rcaapdatedesignation_value'], 0, 10) + 7 : ''), 
            //'Designation criteria  b', //;                           rcaB
            $ind['designation_criteria_b'] => $rca[$i]['field_rcab_value'],
            //'Starting date of designation', //;                      ?
//             $ind['starting_date_of_designation_b'] => 'TODO',       rcaDateDesignation
            $ind['starting_date_of_designation_b'] => ('' != $rca[$i]['field_rcaapdatedesignation_value'] ?  substr($rca[$i]['field_rcaapdatedesignation_value'], 0, 10) : ''),
            //'Starting date of application', //;                      ?
            $ind['starting_date_of_application_b'] => 'TODO',     // =rcaDateDesignation + 7 years
//             $ind['starting_date_of_application_b'] => ('' != $rca[$i]['field_rcaapdatedesignation_value'] ?  substr($rca[$i]['field_rcaapdatedesignation_value'], 0, 10) + 7 : ''), 
            //'Designation criteria  c', //;                           rcaC
            $ind['designation_criteria_c'] => $rca[$i]['field_rcac_value'],
            //'Starting date of designation', //;                      ?
//             $ind['starting_date_of_designation_c'] => 'TODO',    rcaCDateOtherDirective       field_rcacdateotherdirective
            $ind['starting_date_of_designation_c'] => ('' != $rca[$i]['field_rcacdateotherdirective_value'] ?  substr($rca[$i]['field_rcacdateotherdirective_value'], 0, 10) : ''),
            //'Starting date of application', //;                      ?
            $ind['starting_date_of_application_c'] => 'TODO',   // =rcaCDateOtherDirective + 7 years
//             $ind['starting_date_of_application_c'] => ('' != $rca[$i]['field_rcacdateotherdirective_value'] ?  substr($rca[$i]['field_rcacdateotherdirective_value'], 0, 10) + 7 : ''), 
            //'Designation criteria c - relevant EU-Directives and related parameters', //;            ?
//             $ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'] => 'TODO',    rcaCRelevantDirective        field_rcacrelevantdirective
            $ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'] => $rca[$i]['field_rcacrelevantdirective_value'] ,
            //'Designation criteria c - ID applied to the area according to this Directive', //;       ?
//             $ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'] => 'TODO',  rcaCIDOtherDirective    field_rcacidotherdirective
            $ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'] => $rca[$i]['field_rcacidotherdirective_value'],
            //'Designation criteria c - reference date of the area according to this Directive', //;   ?
//             $ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'] => 'TODO',  rcaCDateOtherDirective   field_rcacdateotherdirective
            $ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'] => ('' != $rca[$i]['field_rcacdateotherdirective_value'] ?  substr($rca[$i]['field_rcacdateotherdirective_value'], 0, 10) : ''),
            //'Number of UWWTPs of the article 5.4 area (1)', //;                                      ?rcaPlants
            $ind['number_of_uwwtps_of_the_article_5.4_area_(1)'] => $rca[$i]['field_rcaplants_value'],            
            //'Total organic design capacity of the UWWTPs (p.e.) of the article 5.4 area (1)', //;    ?rcaPlantsCapacity
            $ind['total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)'] => $rca[$i]['field_rcaplantscapacity_value'],
            
            //'Incoming loads (t/y) N-tot (1)', //;    rcaNIncomingMeasured + rcaNIncomingCalculated + rcaNIncomingEstimated
            $ind['incoming_loads_(t/y)_n-tot_(1)'] => ($rca[$i]['field_rcanincomingmeasured_value'] + $rca[$i]['field_rcanincomingcalculated_value'] + $rca[$i]['field_rcanincomingestimated_value']),
            //'Rate of Removal N-tot', //;              ?
            //$ind['rate_of_removal_n-tot'] => 'TODO',         =discharged loads Ntot/incoming loads Ntot
            $ind['rate_of_removal_n-tot'] =>  ($rca[$i]['field_rcandischargedmeasured_value'] + $rca[$i]['field_rcandischargedcalculated_value'] + $rca[$i]['field_rcandischargedestimated_value'])
                                                /
                                               ($rca[$i]['field_rcanincomingmeasured_value'] + $rca[$i]['field_rcanincomingcalculated_value'] + $rca[$i]['field_rcanincomingestimated_value']) ,
            //'Discharged loads (t/y) N-tot (1)', //; rcaNDischargedMeasured + rcaNDischargedCalculated + rcaNDischargedEstimated    
            $ind['discharged_loads_(t/y)_n-tot_(1)'] => ($rca[$i]['field_rcandischargedmeasured_value'] + $rca[$i]['field_rcandischargedcalculated_value'] + $rca[$i]['field_rcandischargedestimated_value']),
            
            //'Incoming loads (t/y) P-tot (1)', //;  rcaPIncomingMeasured + rcaPIncomingCalculated + rcaPIncomingEstimated    
            $ind['incoming_loads_(t/y)_p-tot_(1)'] => ($rca[$i]['field_rcapincomingmeasured_value'] + $rca[$i]['field_rcapincomingcalculated_value'] + $rca[$i]['field_rcapincomingestimated_value']),
            //'Rate of Removal P-tot', //;              ?
//             $ind['rate_of_removal_p-tot'] => 'TODO',  =discharged loads Ptot/incoming loads Ptot
            $ind['rate_of_removal_p-tot'] => ($rca[$i]['field_rcapdischargedmeasured_value'] + $rca[$i]['field_rcapdischargedcalculated_value'] +$rca[$i]['field_rcapdischargedestimated_value'])
                                             /
                                             ($rca[$i]['field_rcapincomingmeasured_value'] + $rca[$i]['field_rcapincomingcalculated_value'] + $rca[$i]['field_rcapincomingestimated_value']),
            //'Discharged loads (t/y) P-tot: ', //; rcaPDischargedMeasured+ rcaPDischargedCalculated + rcaPDischargedEstimated
            $ind['discharged_loads_(t/y)_p-tot:_'] => ($rca[$i]['field_rcapdischargedmeasured_value'] + $rca[$i]['field_rcapdischargedcalculated_value'] +$rca[$i]['field_rcapdischargedestimated_value']),
            
            //'general Remark', //;                   rcaRemarks
            $ind['general_remark'] => $rca[$i]['field_rcaremarks_value'],
            
            //'rcaBeginLife', //;                        ?
            $ind['rcabeginlife'] => 'TODO',     //new reporting format?
            //'rcaEndLife', //;                          ?
            $ind['rcaendlife'] => 'TODO',     //new reporting format?
            //'ID code of successor (2)', //;            ?
            $ind['id_code_of_successor_(2)'] => 'TODO',       //new reporting format?
            //'ID code of predecessor (2)', //;          ?
            $ind['id_code_of_predecessor_(2)'] => 'TODO',   //new reporting format?
            //'remark Successor/Predecessor SA', //;     ?
            $ind['remark_successor/predecessor_sa'] => 'TODO',   //new reporting format?
            //'Related sensitive area (2)', //;          ?
//             $ind['related_sensitive_area_(2)'] => 'TODO',       si CSA, voir rcaSensitiveArea field_rca_sensitive_area
            $ind['related_sensitive_area_(2)'] => ($rca[$i]['field_specialisedzonetype_value'] != 'CSA' ? '' : $rca[$i]['field_rca_sensitive_area_value']),
            //'Remark Related sensitive area (2)', //;   ?
            $ind['remark_related_sensitive_area_(2)'] => 'TODO',        
        );
        fputcsv($r, $line, ';');
//         $allline[] = $line;    
    }    
    fclose($r);

    $context['sandbox']['progress']++;
//     $context['results'][] = 'sheet 1 rca create';    
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
}

function uwwtd_get_all_rca($annee)
{
    $query = 'select n.title,
                     f_rcacode.field_inspireidlocalid_value,
                     f_rcatype.field_specialisedzonetype_value,
                     f_rca_param_n.field_rca_parameter_n_value,
                     f_rca_param_p.field_rca_parameter_p_value,
                     f_rca_param_o.field_rca_parameter_other_value,
                     f_art58.field_rcadateart5854_value,
                     f_art54.field_rcadateart54_value,  
                     f_datedesign.field_rcaapdatedesignation_value,
                     f_dateotherdir.field_rcacdateotherdirective_value,
                     
                     f_relevantdir.field_rcacrelevantdirective_value,
                     f_idotherdir.field_rcacidotherdirective_value,
                     f_rca_sensitivearea.field_rca_sensitive_area_value,
                                          
                     f_rcaan.field_rcaanitro_value,
                     f_rcaap.field_rcaaphos_value,
                     f_rcab.field_rcab_value,
                     f_rcac.field_rcac_value,
                     f_rcaplant.field_rcaPlants_value,
                     
                     f_capacity.field_rcaplantscapacity_value,
                     f_nincoming_m.field_rcanincomingmeasured_value,
                     f_nincoming_c.field_rcanincomingcalculated_value,
                     f_nincoming_e.field_rcanincomingestimated_value,
                     
                     f_ndischarged_m.field_rcandischargedmeasured_value,
                     f_ndischarged_c.field_rcandischargedcalculated_value,
                     f_ndischarged_e.field_rcandischargedestimated_value,
                     
                     f_pincoming_m.field_rcapincomingmeasured_value,
                     f_pincoming_c.field_rcapincomingcalculated_value,
                     f_pincoming_e.field_rcapincomingestimated_value,
                     
                     f_pdischarged_m.field_rcapdischargedmeasured_value,
                     f_pdischarged_c.field_rcapdischargedcalculated_value,
                     f_pdischarged_e.field_rcapdischargedestimated_value,
                     
                     f_remarks.field_rcaremarks_value  
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id    
        left join
            {field_data_field_inspireidlocalid} f_rcacode
                ON n.nid = f_rcacode.entity_id
        left join
            {field_data_field_specialisedzonetype} f_rcatype
                ON n.nid = f_rcatype.entity_id
        left join
            {field_data_field_rca_parameter_n} f_rca_param_n
                ON n.nid = f_rca_param_n.entity_id
        left join
            {field_data_field_rca_parameter_p} f_rca_param_p
                ON n.nid = f_rca_param_p.entity_id
        left join
            {field_data_field_rca_parameter_other} f_rca_param_o
                ON n.nid = f_rca_param_o.entity_id
        left join
            {field_data_field_rcadateart5854} f_art58
                ON n.nid = f_art58.entity_id
        left join
            {field_data_field_rcadateart54} f_art54
                ON n.nid = f_art54.entity_id
        left join
            {field_data_field_rcaapdatedesignation} f_datedesign
                ON n.nid = f_datedesign.entity_id                
        left join
            {field_data_field_rcacdateotherdirective} f_dateotherdir
                ON n.nid = f_dateotherdir.entity_id                   
        left join
            {field_data_field_rcacrelevantdirective} f_relevantdir
                ON n.nid = f_relevantdir.entity_id 
        left join
            {field_data_field_rcacidotherdirective} f_idotherdir
                ON n.nid = f_idotherdir.entity_id
        left join
            {field_data_field_rca_sensitive_area} f_rca_sensitivearea
                ON n.nid = f_rca_sensitivearea.entity_id                
                                                             
        left join
            {field_data_field_rcaanitro} f_rcaan
                ON n.nid = f_rcaan.entity_id
        left join
            {field_data_field_rcaaphos} f_rcaap
                ON n.nid = f_rcaap.entity_id
        left join
            {field_data_field_rcab} f_rcab
                ON n.nid = f_rcab.entity_id
        left join
            {field_data_field_rcac} f_rcac
                ON n.nid = f_rcac.entity_id
        left join
            {field_data_field_rcaplants} f_rcaplant
                ON n.nid = f_rcaplant.entity_id
        left join
            {field_data_field_rcaplantscapacity} f_capacity
                ON n.nid = f_capacity.entity_id
        left join
            {field_data_field_rcanincomingmeasured} f_nincoming_m
                ON n.nid = f_nincoming_m.entity_id
        left join
            {field_data_field_rcanincomingcalculated} f_nincoming_c
                ON n.nid = f_nincoming_c.entity_id
        left join
            {field_data_field_rcanincomingestimated} f_nincoming_e
                ON n.nid = f_nincoming_e.entity_id
        left join
            {field_data_field_rcandischargedmeasured} f_ndischarged_m
                ON n.nid = f_ndischarged_m.entity_id
        left join
            {field_data_field_rcandischargedcalculated} f_ndischarged_c
                ON n.nid = f_ndischarged_c.entity_id
        left join
            {field_data_field_rcandischargedestimated} f_ndischarged_e
                ON n.nid = f_ndischarged_e.entity_id
        left join
            {field_data_field_rcapincomingmeasured} f_pincoming_m
                ON n.nid = f_pincoming_m.entity_id
        left join
            {field_data_field_rcapincomingcalculated} f_pincoming_c
                ON n.nid = f_pincoming_c.entity_id
        left join
            {field_data_field_rcapincomingestimated} f_pincoming_e
                ON n.nid = f_pincoming_e.entity_id
        left join
            {field_data_field_rcapdischargedmeasured} f_pdischarged_m
                ON n.nid = f_pdischarged_m.entity_id
        left join
            {field_data_field_rcapdischargedcalculated} f_pdischarged_c
                ON n.nid = f_pdischarged_c.entity_id
        left join
            {field_data_field_rcapdischargedestimated} f_pdischarged_e
                ON n.nid = f_pdischarged_e.entity_id   
        left join
            {field_data_field_rcaremarks} f_remarks
                ON n.nid = f_remarks.entity_id                                                                                                                                                                                                                                                                                                                                                                                                                                
    where n.type = :typenode
        and f_annee.field_anneedata_value = :annee
    order by 
            CASE 
                WHEN f_rcatype.field_specialisedzonetype_value = \'CSA\' THEN 1
                WHEN f_rcatype.field_specialisedzonetype_value = \'SA\'  THEN 2
                WHEN f_rcatype.field_specialisedzonetype_value = \'LSA\' THEN 3
                WHEN f_rcatype.field_specialisedzonetype_value = \'A58\' THEN 4
                ELSE 99
            END     
        asc
    ';
    
//     dsm($query);
    $param = array(
        ':typenode' => 'receiving_area',
        ':annee' => $annee,
        );
    
    $data = array();    
    try {
        $result = db_query($query, $param);
        while($row = $result->fetchAssoc()) {
            $data[] = $row;
        }
    }catch(Exception $e) {
        dsm($e->getMessage());
    }

    return $data;                               
}

function uwwtd_create_register_2015_final_file($annee, &$context)
{
 	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }
    
    $configsheet = $GLOBALS['uwwtd']['sheet'];
    $listesheet = array('rca',
                        'uwwtp', 
                        'agglo',
                        'summary_legal_comp',
                        'distance_to_comp',
                        'summary_installation_in_place',
                        'breach_list',
                        'summary_big_cities',
                        'nuts_level2',
                        'sewage_sludge',
                        'graph',
                        'new_agglo',
                        'agglo_out',
                        'new_uwwtp',
                        'uwwtp_out',
                        );
    
    $finalPHPExcel = new PHPExcel();
    $sheetIndex = $finalPHPExcel->getIndex($finalPHPExcel->getSheetByName('Worksheet')); //default worksheet
    $finalPHPExcel->removeSheetByIndex($sheetIndex);    
    
    foreach($listesheet as $sheet) {
//         dsm($sheet);
        //load sheet         
//         $sheet_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_name'] .'.xlsx';
        $sheet_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_name'], $annee);
        
        if (!file_exists($sheet_path)) {
            continue;
        }
//         dsm($sheet_path);
    //     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp);
//         PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_sqlite);
        $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
        $objReader->setLoadSheetsOnly(array($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']));                                        
        $objPHPExcel = $objReader->load(drupal_realpath($sheet_path));     
//         $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']);
//         dsm(1);
        $finalPHPExcel->addExternalSheet(clone $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']));
//         dsm(2);
        unset($objPHPExcel, $objReader);
    } 
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($finalPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $target_path = uwwtd_getregister_finalfile_path($annee);//$GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['final_register'];
//     dsm($target_path);    
    $objWriter->save(drupal_realpath($target_path));
    unset($finalPHPExcel);
    unset($objWriter);  
    $context['sandbox']['progress']++;
 
    $context['message'] = '('.$annee.') ' . t('Finalization');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }  


    variable_set('registre_'.$annee, time());
    drupal_set_message(uwwtd_registre_create_html_link($annee));             
}


function uwwtd_getregister_file_path($filename, $annee)
{
    return $GLOBALS['uwwtd']['sheet']['path_created_file'] . $annee . $filename;    
}

function uwwtd_getregister_modelfile_path()
{
    return $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];    
}

function uwwtd_getregister_finalfile_path($annee)
{
    return $GLOBALS['uwwtd']['sheet']['path_created_file'] . $annee . $GLOBALS['uwwtd']['sheet']['final_register'];    
}


function uwwtd_get_register_treatment_required($required_calculated) {            
    $treatment_required = '';
    switch($required_calculated) {
        case 'P':
            $treatment_required = '1';
            break;
        case 'S':
            $treatment_required = '2';
            break;
        case 'N':
            $treatment_required = '3N';
            break;
        case 'NP':
            $treatment_required = '3NP';
            break;
        case 'P':
            $treatment_required = '3P';
            break;
        case 'O':
            $treatment_required = '3other';
            break;
        case 'Appropriate':
            $treatment_required = 'Appropriate';
            break;  
        case 'NI':
            $treatment_required = '0';
            break; 
        default:  
            $treatment_required = '';                                                                                                                                         
    }
    return $treatment_required;
}

function uwwtd_get_register_treatmenttype_inplace($uwwtp) 
{
    $treatmenttypeinplace = '';
    if ($uwwtp->field_uwwprimarytreatment['und'][0]['value'] == '1') {
        $treatmenttypeinplace .= ($treatmenttypeinplace == '' ? '': ';' ) .'1';
    }
    if ($uwwtp->field_uwwsecondarytreatment['und'][0]['value'] == '1') {
        $treatmenttypeinplace .= ($treatmenttypeinplace == '' ? '': ';' ) .'2';
    }
    if ($uwwtp->field_uwwpremoval['und'][0]['value'] == '1' && $uwwtp->field_uwwnremoval['und'][0]['value'] == '1') {
        $treatmenttypeinplace .= ($treatmenttypeinplace == '' ? '': ';' ) .'3NP';
    } elseif ($uwwtp->field_uwwpremoval['und'][0]['value'] == '1' && $uwwtp->field_uwwnremoval['und'][0]['value'] == '0') {
        $treatmenttypeinplace .= ($treatmenttypeinplace == '' ? '': ';' ) .'3P';
    } elseif ($uwwtp->field_uwwpremoval['und'][0]['value'] == '0' && $uwwtp->field_uwwnremoval['und'][0]['value'] == '1') {
        $treatmenttypeinplace .= ($treatmenttypeinplace == '' ? '': ';' ) .'3N';
    }
    if ($uwwtp->field_uwwothertreat['und'][0]['value'] == '1') {
        $treatmenttypeinplace .= ($treatmenttypeinplace == '' ? '': ';' ) .'3other';
    }
    if ($treatmenttypeinplace == '') {
        $treatmenttypeinplace .= '0';
    } 
    return $treatmenttypeinplace;
}

function uwwtd_get_register_pending_or_expired_uwwtp($annee, $deadlineart3, $deadlineart4, $deadlineart5, $uwwtp)
{                                                                             
    $pending_or_expired = '';        
    if (
        ($deadlineart3 < $annee
         || $deadlineart4 < $annee
         || $deadlineart5 < $annee
        )
         && ($uwwtp->field_uwwcompliance['und'][0]['value'] == 'NC' || $uwwtp->field_uwwcompliance['und'][0]['value'] == 'C' )
        ) {
        $pending_or_expired = 'EXP';
    }
    
    //pending deadline    PD + les C avec date pas dépassée
    if (
        ($deadlineart3 >= $annee
         && $deadlineart4 >= $annee
         && $deadlineart5 >= $annee
        )
        && ($uwwtp->field_uwwcompliance['und'][0]['value'] == 'PD' || $uwwtp->field_uwwcompliance['und'][0]['value'] == 'C' )
        ) {
        $pending_or_expired = 'PD';
    } 
    
    return $pending_or_expired;
} 

function uwwtd_get_register_art4_treatement_type_compliance($treatment_required, $treatmenttypeinplace)
{  
                      //if treatment required is 2 or 3 (if not NR), check if treatment in place is 2 or 3 if yes=C if no NC
    $art4_treatment_type = '';
//             if ($line[$ind['generated load  p. e.']] < 2000
//                 || $line[$ind['neither collecting system nor IAS % of gen. load']] == 100
//                 || $line[$ind['uww_entering this UWWTP % of gen. load']] = 0
//                 || $line[$ind['deadline for Art. 4 compliance']] > $annee
//                 || $line[$ind['uww_status']] == 'inactive'
//                 || (
//                         (false !== strpos($line[$ind['water body type']], 'CW')
//                          || false !== strpos($line[$ind['water body type']], 'LC')
//                         )
//                         && $line[$ind['generated load  p. e.']] <= 10000
//                    ) 
//                ) {
//                 $art4_treatment_type = 'NR';   
//             } else {
    switch($treatment_required) {
        case '2':
            if (false !== strpos($treatmenttypeinplace, '2')) {
                $art4_treatment_type = 'C';
            } else {
                $art4_treatment_type = 'NC';
            }
            break;
        case '3N':
            if (false !== strpos($treatmenttypeinplace, '3N')) {
                $art4_treatment_type = 'C';
            } else {
                $art4_treatment_type = 'NC';
            }                
            break; 
        case '3P':
            if (false !== strpos($treatmenttypeinplace, '3P')) {
                $art4_treatment_type = 'C';
            } else {
                $art4_treatment_type = 'NC';
            }                
            break;
        case '3NP':
            if (false !== strpos($treatmenttypeinplace, '3NP')) {
                $art4_treatment_type = 'C';
            } else {
                $art4_treatment_type = 'NC';
            }                
            break;  
        case '3other':
            if (false !== strpos($treatmenttypeinplace, '3other')) {
                $art4_treatment_type = 'C';
            } else {
                $art4_treatment_type = 'NC';
            }                
            break;
        case 'Appropriate':  
        case 'NR':
            $art4_treatment_type = 'NR';
            break;
        case '0':
        case '1':
            $art4_treatment_type = 'NC';                                                                                          
    }
//             }
    return $art4_treatment_type;
}

function uwwtd_get_register_art4_monitoring_compliance($treatment_required, $bod5monitoring, $codmonitoring) 
{   
            //if treatment required is 2 or 3 (if not NR) then if 'uwwBOD5Perf=pass+uwwCODPerf=pass = C if not = NC
    $art4_monitoring = '';
//             if ($line[$ind['generated load  p. e.']] < 2000
//                 || $line[$ind['neither collecting system nor IAS % of gen. load']] == 100
//                 || $line[$ind['uww_entering this UWWTP % of gen. load']] = 0
//                 || $line[$ind['deadline for Art. 4 compliance']] > $annee
//                 || $line[$ind['Monitoring results - BOD5']] == ''
//                 || $line[$ind['Monitoring results -COD']] == ''
//                 || $line[$ind['art4_treatment type']] == 'NR'
//                ) {
//                 $art4_monitoring = 'NR';
//             } else {
    switch($treatment_required) {
        case '2':
        case '3N':
        case '3P':
        case '3NP':
        case '3other':
            if ($bod5monitoring ==  'Pass' && $codmonitoring == 'Pass') {
                $art4_monitoring = 'C';
            } else {
                $art4_monitoring = 'NC';
            }                
            break;
        case 'Appropriate':  
        case 'NR':
            $art4_monitoring = 'NR';
            break;                                                                                          
    }                
//             }
    return $art4_monitoring;
}

function uwwtd_get_register_art5_treatement_type_compliance($treatment_required, $treatmenttypeinplace)
{            
    //if treatment required is 3N, NP P Other (if not NR), check if treatment in place is the same 3N, NP, P Other if yes=C if no NC
    $art5_treatment = '';
    switch($treatment_required) {
        case '3N':
            if (false !== strpos($treatmenttypeinplace, '3N')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }                
            break;                
        case '3P':
            if (false !== strpos($treatmenttypeinplace, '3P')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }                
            break;                
        case '3NP':
            if (false !== strpos($treatmenttypeinplace, '3NP')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }                
            break;                
        case '3other':
            if (false !== strpos($treatmenttypeinplace, '3other')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }                
            break;
        case 'Appropriate':  
        case 'NR':
            $art5_treatment = 'NR';
            break;                                                                                          
    }                
//             }  
    return $art5_treatment;          
}  

function uwwtd_get_register_art5_monitoring_compliance($treatment_required, $monitoring_Ntot, $monitoring_Ptot)
{            
    //if treatment required is 3N, NP P Other (if not NR)then if 3n 'NtotPerf=pass = C if not =NC, if 3np NtotPerf=pass+PtotPerf=pass = C if not = NC, if 3p PtotPerf=pass = C if not = NC
    $art5_monitoring = '';            
    switch($treatment_required) {
        case '3N':   
            if ($monitoring_Ntot == 'Pass') {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }                
            break;                
        case '3P':
            if ($monitoring_Ptot == 'Pass') {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }                 
            break;                
        case '3NP':
            if ($monitoring_Ntot == 'Pass' && $monitoring_Ptot == 'Pass') {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }                 
            break;                
        case 'Appropriate':  
        case 'NR':
            $art5_monitoring = 'NR';
            break;                                                                                          
    }
    
    return $art5_monitoring;            
}


function uwwtd_register_get_cell_value($sheet, $coord)
{
    if (is_null($sheet)) {
        return '';
    }
    
    return $sheet->getCell($coord)->getValue();
}

function uwwtd_registre_create_html_link($annee)
{
    return '<a href="'.file_create_url(uwwtd_getregister_finalfile_path($annee)).'">'. t('Get the register for year @year', array('@year' => $annee)) .'</a>';
}
