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
 
 // Bouton envoyer
 $form['submit_button'] = array(
  '#type' => 'submit',
  '#value' => t('Submit'),
 );

 return $form;
}

/**
 *
 */
function uwwtd_register_form_submit($form, &$form_state){
     $annee = $form['uwwtd_year']['#value'];
     $filepath = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['final_register'];
    //if registre already calculated and no new conformity or new import_data processed since, we send the file
//     dsm($filepath);
//     dsm(file_create_url($filepath));                                                             
    if (file_exists($filepath) 
        && variable_get('registre_'.$annee, -2) > variable_get('conformity_'.$annee, -1)
        && variable_get('registre_'.$annee, -2) > variable_get('import_data_'.$annee, -1)) {
        drupal_set_message('<a href="'.file_create_url($filepath).'">get the register for year '.$annee.'</a>');
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
 	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
                                                                                           
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'] .'.xlsx';

//     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp);
    PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_sqlite);
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
 
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name'],
                                        ) );
                                        
    $objPHPExcel = $objReader->load($model_path);
           
//     dsm(444);
//     return '111';       
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name']);
    
    $start_line = 4;
//     $sheet->insertNewRowBefore(5, count($listagglo));
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
//     dsm($filename);           
    $ragglofile = fopen($filename, 'rb');
//     dsm($ragglofile);
    //ignore header //decomment for debug on xls final file
//     $agglo = fgetcsv($ragglofile, 5000, ';');

    while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
//         dsm($agglo);
//         break;        
//     foreach($listagglo as $agglo) {
        $sheet->setCellValueByColumnAndRow($ind['aggcode'], $i, $agglo[$ind['aggcode']]);
        $sheet->setCellValueByColumnAndRow($ind['aggname'], $i, $agglo[$ind['aggname']]);
        $sheet->setCellValueByColumnAndRow($ind['generated_load_p.e.'], $i, $agglo[$ind['generated_load_p.e.']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline_for_art._3_compliance'], $i, $agglo[$ind['deadline_for_art._3_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline_for_art._4_compliance'], $i, $agglo[$ind['deadline_for_art._4_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline_for_art._5_compliance'], $i, $agglo[$ind['deadline_for_art._5_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['pending_or_expired_deadline_exp/pd/exp-pd'], $i, $agglo[$ind['pending_or_expired_deadline_exp/pd/exp-pd']]);
        $sheet->setCellValueByColumnAndRow($ind['nuts_1_code'], $i, $agglo[$ind['nuts_1_code']]);
        $sheet->setCellValueByColumnAndRow($ind['nuts_2_code'], $i, $agglo[$ind['nuts_2_code']]);
        $sheet->setCellValueByColumnAndRow($ind['nuts_3_code'], $i, $agglo[$ind['nuts_3_code']]);
        $sheet->setCellValueByColumnAndRow($ind['bccode'], $i, $agglo[$ind['bccode']]);
        $sheet->setCellValueByColumnAndRow($ind['bcname'], $i, $agglo[$ind['bcname']]);
        $sheet->setCellValueByColumnAndRow($ind['number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration'], $i, $agglo[$ind['number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_reported_?'], $i, $agglo[$ind['previous_reported_?']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_aggcode'], $i, $agglo[$ind['previous_aggcode']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_gen._load_p.e.'], $i, $agglo[$ind['previous_gen._load_p.e.']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_percentage_of_change_of_size_(1)'], $i, $agglo[$ind['previous_percentage_of_change_of_size_(1)']]);
        $sheet->setCellValueByColumnAndRow($ind['total_%_(2)'], $i, $agglo[$ind['total_%_(2)']]);
        $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system'], $i, $agglo[$ind['connected_to_collecting_system']]);
        $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system_p._e.'], $i, $agglo[$ind['connected_to_collecting_system_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_%_of_gen._load'], $i, $agglo[$ind['addressed_through_ias_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_p._e.'], $i, $agglo[$ind['addressed_through_ias_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_%_of_gen._load'], $i, $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_p._e.'], $i, $agglo[$ind['neither_collecting_system_nor_ias_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art3_%_of_gen._load_(3)'], $i, $agglo[$ind['dist_art3_%_of_gen._load_(3)']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art3_in_p.e.'], $i, $agglo[$ind['dist_art3_in_p.e.']]);
        $sheet->setCellValueByColumnAndRow($ind['art3_compliance'], $i, $agglo[$ind['art3_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art3_compliance'], $i, $agglo[$ind['previous_art3_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['total_entering_load_p._e.'], $i, $agglo[$ind['total_entering_load_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['ratio_connected/enentering_%'], $i, $agglo[$ind['ratio_connected/enentering_%']]);
        $sheet->setCellValueByColumnAndRow($ind['total_treatment_plant_design_capacity_p._e.'], $i, $agglo[$ind['total_treatment_plant_design_capacity_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['ratio_total_entering_load/total_design_capacity_%'], $i, $agglo[$ind['ratio_total_entering_load/total_design_capacity_%']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliant_%_of_gen._load'], $i, $agglo[$ind['art4_compliant_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliant_p._e.'], $i, $agglo[$ind['art4_compliant_p._e.']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant_%_of_gen._load'], $i, $agglo[$ind['art4_not_compliant_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant'], $i, $agglo[$ind['art4_not_compliant']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant_%_of_gen._load'], $i, $agglo[$ind['art4_not_relevant_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant'], $i, $agglo[$ind['art4_not_relevant']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliance_art._4_(only)'], $i, $agglo[$ind['art4_compliance_art._4_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_compliance_art._4_(hierarchical_approach)'], $i, $agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_compliance'], $i, $agglo[$ind['previous_art4_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat__%_of_connected_load'], $i, $agglo[$ind['dist_art4_treat__%_of_connected_load']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat_in_p.e._(4)'], $i, $agglo[$ind['dist_art4_treat_in_p.e._(4)']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_%_of_connected_load'], $i, $agglo[$ind['dist_art4_perf_%_of_connected_load']]);
        $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_in_p.e._(5)'], $i, $agglo[$ind['dist_art4_perf_in_p.e._(5)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_a_(%_of_gen._load)'], $i, $agglo[$ind['compliant_5_a_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_a_(p._e.)'], $i, $agglo[$ind['compliant_5_a_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_a_(%_of_gen._load)'], $i, $agglo[$ind['not_compliant_5_a_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_a_(p._e.)'], $i, $agglo[$ind['not_compliant_5_a_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_a_(%_of_gen._load)'], $i, $agglo[$ind['not_relevant_5_a_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_a_(p._e.)'], $i, $agglo[$ind['not_relevant_5_a_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5a_(only)'], $i, $agglo[$ind['compliance_art._5a_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5a_(hierarchical_approach)'], $i, $agglo[$ind['compliance_art._5a_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_b_(%_of_gen._load)'], $i, $agglo[$ind['compliant_5_b_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_b_(p._e.)'], $i, $agglo[$ind['compliant_5_b_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_b_(%_of_gen._load)'], $i, $agglo[$ind['not_compliant_5_b_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_b_(p._e.)'], $i, $agglo[$ind['not_compliant_5_b_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_b_(%_of_gen._load)'], $i, $agglo[$ind['not_relevant_5_b_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_b_(p._e.)'], $i, $agglo[$ind['not_relevant_5_b_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5b_(only)'], $i, $agglo[$ind['compliance_art._5b_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5b_(hierarchical_approach)'], $i, $agglo[$ind['compliance_art._5b_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_c_(%_of_gen._load)'], $i, $agglo[$ind['compliant_5_c_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_5_c__(p._e.)'], $i, $agglo[$ind['compliant_5_c__(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_c_(%_of_gen._load)'], $i, $agglo[$ind['not_compliant_5_c_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_5_c_(p._e.)'], $i, $agglo[$ind['not_compliant_5_c_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_c_(%_of_gen._load)'], $i, $agglo[$ind['not_relevant_5_c_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_relevant_5_c_(p._e.)'], $i, $agglo[$ind['not_relevant_5_c_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5c_(only)'], $i, $agglo[$ind['compliance_art._5c_(only)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5c_(hierarchical_approach)'], $i, $agglo[$ind['compliance_art._5c_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliant_art._5_(p._e.)'], $i, $agglo[$ind['compliant_art._5_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['not_compliant_art._5_(p._e.)'], $i, $agglo[$ind['not_compliant_art._5_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['art._5_not_relevant_(p._e.)'], $i, $agglo[$ind['art._5_not_relevant_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5_(whole_agglo)'], $i, $agglo[$ind['compliance_art._5_(whole_agglo)']]);
        $sheet->setCellValueByColumnAndRow($ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)'], $i, $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_compliance'], $i, $agglo[$ind['previous_art5_compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_treat_%_of_gen._load'], $i, $agglo[$ind['previous_art5_treat_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_treat_in_p.e._(6)'], $i, $agglo[$ind['previous_art5_treat_in_p.e._(6)']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_perf_%_of_gen._load'], $i, $agglo[$ind['previous_art5_perf_%_of_gen._load']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_perf_in_p.e._(7)'], $i, $agglo[$ind['previous_art5_perf_in_p.e._(7)']]);
        $sheet->setCellValueByColumnAndRow($ind['water_body_type'], $i, $agglo[$ind['water_body_type']]);
        $sheet->setCellValueByColumnAndRow($ind['id_code_of_the_water_body_wfd'], $i, $agglo[$ind['id_code_of_the_water_body_wfd']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_wfd_river_basin_district'], $i, $agglo[$ind['id_of_wfd_river_basin_district']]);
        $sheet->setCellValueByColumnAndRow($ind['entirely_discharging_into_coastal_waters?'], $i, $agglo[$ind['entirely_discharging_into_coastal_waters?']]);
        $sheet->setCellValueByColumnAndRow($ind['type_of_receiving_water'], $i, $agglo[$ind['type_of_receiving_water']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_the_sa_or_lsa'], $i, $agglo[$ind['id_of_the_sa_or_lsa']]);
        $sheet->setCellValueByColumnAndRow($ind['rca_name'], $i, $agglo[$ind['rca_name']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_5.8.'], $i, $agglo[$ind['starting_date_of_application_5.8.']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_5.4.'], $i, $agglo[$ind['starting_date_of_application_5.4.']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_a_n'], $i, $agglo[$ind['starting_date_of_application_a_n']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_a_p'], $i, $agglo[$ind['starting_date_of_application_a_p']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_b'], $i, $agglo[$ind['starting_date_of_application_b']]);
        $sheet->setCellValueByColumnAndRow($ind['starting_date_of_application_c'], $i, $agglo[$ind['starting_date_of_application_c']]);
        $sheet->setCellValueByColumnAndRow($ind['secondary_treatment_installed_(%_of_gen._load)'], $i, $agglo[$ind['secondary_treatment_installed_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['secondary_treatment_installed_(p._e.)'], $i, $agglo[$ind['secondary_treatment_installed_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._secondary_ok_(%_of_gen._load)'], $i, $agglo[$ind['monitoring_results_f._secondary_ok_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._secondary_ok'], $i, $agglo[$ind['monitoring_results_f._secondary_ok']]);
        $sheet->setCellValueByColumnAndRow($ind['more_str._treatment_installed_(%_of_gen._load)'], $i, $agglo[$ind['more_str._treatment_installed_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['more_str._treatment_installed_(p._e.)'], $i, $agglo[$ind['more_str._treatment_installed_(p._e.)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._more_string._ok_(%_of_gen._load)'], $i, $agglo[$ind['monitoring_results_f._more_string._ok_(%_of_gen._load)']]);
        $sheet->setCellValueByColumnAndRow($ind['monitoring_results_f._more_string._ok_(p._e.)'], $i, $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']]);
        $i++;
//         break;
        $sheet->insertNewRowBefore($i, 1);
    }
    fclose($ragglofile);
   
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('file agglo sheet 3 finish');    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }              
}

function uwwtd_create_register_2015_file_uwwtp_sheet2($annee, &$context)
{    
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }
    
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'] .'.xlsx';
//     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp);
//     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_sqlite);
//     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_sqlite3);
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name']);
    
    $start_line = 4;
//     $sheet->insertNewRowBefore(5, count($listuwwtp));
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet2_uwwtp'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] . '.csv';           
    $ruwwtpfile = fopen($filename, 'rb');
    
    //ignore header //decomment for debug on xls final file
//     $uwwtp = fgetcsv($ruwwtpfile, 5000, ';');

    while ($uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {     
//     foreach($listuwwtp as $uwwtp) {
        $sheet->setCellValueByColumnAndRow($ind['aggcode'], $i, $uwwtp[$ind['aggcode']]);
        $sheet->setCellValueByColumnAndRow($ind['aggname'], $i, $uwwtp[$ind['aggname']]);
        $sheet->setCellValueByColumnAndRow($ind['aggstatus'], $i, $uwwtp[$ind['aggstatus']]);
        $sheet->setCellValueByColumnAndRow($ind['generated load  p. e.'], $i, $uwwtp[$ind['generated load  p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline for Art. 3 compliance'], $i, $uwwtp[$ind['deadline for Art. 3 compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline for Art. 4 compliance'], $i, $uwwtp[$ind['deadline for Art. 4 compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['deadline for Art. 5 compliance'], $i, $uwwtp[$ind['deadline for Art. 5 compliance']]);
        $sheet->setCellValueByColumnAndRow($ind['collected in collecting system % of gen. load'], $i, $uwwtp[$ind['collected in collecting system % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['collected in collecting system  p. e.'], $i, $uwwtp[$ind['collected in collecting system  p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed through IAS % of gen. load'], $i, $uwwtp[$ind['addressed through IAS % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['addressed through IAS p. e.'], $i, $uwwtp[$ind['addressed through IAS p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['neither collecting system nor IAS % of gen. load'], $i, $uwwtp[$ind['neither collecting system nor IAS % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['neither collecting system nor IAS p. e.'], $i, $uwwtp[$ind['neither collecting system nor IAS p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_code'], $i, $uwwtp[$ind['uww_code']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_name'], $i, $uwwtp[$ind['uww_name']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_status'], $i, $uwwtp[$ind['uww_status']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_entering this UWWTP % of gen. load'], $i, $uwwtp[$ind['uww_entering this UWWTP % of gen. load']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_entering load  p. e.'], $i, $uwwtp[$ind['uww_entering load  p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_capacity p. e.'], $i, $uwwtp[$ind['uww_capacity p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['uww_status'], $i, $uwwtp[$ind['uww_status']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_reference year'], $i, $uwwtp[$ind['previous_reference year']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_entering load p. e.'], $i, $uwwtp[$ind['previous_entering load p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_capacity p. e.'], $i, $uwwtp[$ind['previous_capacity p. e.']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of entering load (1) %'], $i, $uwwtp[$ind['previous_Percentage of change of entering load (1) %']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of capacity (2) %'], $i, $uwwtp[$ind['previous_Percentage of change of capacity (2) %']]);
        $sheet->setCellValueByColumnAndRow($ind['water body type'], $i, $uwwtp[$ind['water body type']]);
        $sheet->setCellValueByColumnAndRow($ind['In case of discharge on land please specify the purpose'], $i, $uwwtp[$ind['In case of discharge on land please specify the purpose']]);
        $sheet->setCellValueByColumnAndRow($ind['Id code of the water body WFD'], $i, $uwwtp[$ind['Id code of the water body WFD']]);
        $sheet->setCellValueByColumnAndRow($ind['ID of WFD river basin district'], $i, $uwwtp[$ind['ID of WFD river basin district']]);
        $sheet->setCellValueByColumnAndRow($ind['entirely discharging into coastal waters?'], $i, $uwwtp[$ind['entirely discharging into coastal waters?']]);
        $sheet->setCellValueByColumnAndRow($ind['type of receiving water'], $i, $uwwtp[$ind['type of receiving water']]);
        $sheet->setCellValueByColumnAndRow($ind['ID of the SA or LSA'], $i, $uwwtp[$ind['ID of the SA or LSA']]);
        $sheet->setCellValueByColumnAndRow($ind['rcaname'], $i, $uwwtp[$ind['rcaname']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application 5.8.'], $i, $uwwtp[$ind['Starting date of application 5.8.']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application 5.4.'], $i, $uwwtp[$ind['Starting date of application 5.4.']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application a N'], $i, $uwwtp[$ind['Starting date of application a N']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application a P'], $i, $uwwtp[$ind['Starting date of application a P']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application b'], $i, $uwwtp[$ind['Starting date of application b']]);
        $sheet->setCellValueByColumnAndRow($ind['Starting date of application c'], $i, $uwwtp[$ind['Starting date of application c']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment required'], $i, $uwwtp[$ind['treatment required']]);
        $sheet->setCellValueByColumnAndRow($ind['treatment type in place (4)'], $i, $uwwtp[$ind['treatment type in place (4)']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results - BOD5'], $i, $uwwtp[$ind['Monitoring results - BOD5']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -COD'], $i, $uwwtp[$ind['Monitoring results -COD']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -Ntot'], $i, $uwwtp[$ind['Monitoring results -Ntot']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -Ptot'], $i, $uwwtp[$ind['Monitoring results -Ptot']]);
        $sheet->setCellValueByColumnAndRow($ind['Monitoring results -other'], $i, $uwwtp[$ind['Monitoring results -other']]);
        $sheet->setCellValueByColumnAndRow($ind['In cause of failure (3)'], $i, $uwwtp[$ind['In cause of failure (3)']]);
        $sheet->setCellValueByColumnAndRow($ind['difference between monitoring result N and P and treatment in place'], $i, $uwwtp[$ind['difference between monitoring result N and P and treatment in place']]);
        
        $sheet->setCellValueByColumnAndRow($ind['Number of discharge points for this UWWTP'], $i, $uwwtp[$ind['Number of discharge points for this UWWTP']]);
        $sheet->setCellValueByColumnAndRow($ind['DCP ID'], $i, $uwwtp[$ind['DCP ID']]);
        $sheet->setCellValueByColumnAndRow($ind['ID of receiving water body 2'], $i, $uwwtp[$ind['ID of receiving water body 2']]);
        $sheet->setCellValueByColumnAndRow($ind['water body type 2'], $i, $uwwtp[$ind['water body type 2']]);
        $sheet->setCellValueByColumnAndRow($ind['ID of the SA or LSA 2'], $i, $uwwtp[$ind['ID of the SA or LSA 2']]);
        $sheet->setCellValueByColumnAndRow($ind['rcaname 2'], $i, $uwwtp[$ind['rcaname 2']]);
        $sheet->setCellValueByColumnAndRow($ind['designation date 2'], $i, $uwwtp[$ind['designation date 2']]);  
        
        
        $sheet->setCellValueByColumnAndRow($ind['art4_treatment type'], $i, $uwwtp[$ind['art4_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_monitoring'], $i, $uwwtp[$ind['art4_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_final'], $i, $uwwtp[$ind['art4_final']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_treatment type'], $i, $uwwtp[$ind['art5_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_monitoring'], $i, $uwwtp[$ind['art5_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_final'], $i, $uwwtp[$ind['art5_final']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_expired_treatment type'], $i, $uwwtp[$ind['art4_expired_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_expired_monitoring'], $i, $uwwtp[$ind['art4_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art4_expired_final'], $i, $uwwtp[$ind['art4_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_expired_treatment type'], $i, $uwwtp[$ind['art5_expired_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_expired_monitoring'], $i, $uwwtp[$ind['art5_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['art5_expired_final'], $i, $uwwtp[$ind['art5_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_expired_treatment'], $i, $uwwtp[$ind['previous_art4_expired_treatment']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_expired_monitoring'], $i, $uwwtp[$ind['previous_art4_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art4_expired_final'], $i, $uwwtp[$ind['previous_art4_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_treatment type'], $i, $uwwtp[$ind['previous_art5_expired_treatment type']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_monitoring'], $i, $uwwtp[$ind['previous_art5_expired_monitoring']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_final'], $i, $uwwtp[$ind['previous_art5_expired_final']]);
        $sheet->setCellValueByColumnAndRow($ind['previous_art5_expired_difference between the previous situation and now'], $i, $uwwtp[$ind['previous_art5_expired_difference between the previous situation and now']]);
        $sheet->setCellValueByColumnAndRow($ind['Secondary: installed?'], $i, $uwwtp[$ind['Secondary: installed?']]);
        $sheet->setCellValueByColumnAndRow($ind['Secondary:  monitoring passed?'], $i, $uwwtp[$ind['Secondary:  monitoring passed?']]);
        $sheet->setCellValueByColumnAndRow($ind['Secondary:  FINAL'], $i, $uwwtp[$ind['Secondary:  FINAL']]);
        $sheet->setCellValueByColumnAndRow($ind['More stringent:  installed?'], $i, $uwwtp[$ind['More stringent:  installed?']]);
        $sheet->setCellValueByColumnAndRow($ind['More stringent: monitoring passed?'], $i, $uwwtp[$ind['More stringent: monitoring passed?']]);
        $sheet->setCellValueByColumnAndRow($ind['More stringent:  FINAL'], $i, $uwwtp[$ind['More stringent:  FINAL']]);
        $sheet->setCellValueByColumnAndRow($ind['Volume of waste water treated m3/y'], $i, $uwwtp[$ind['Volume of waste water treated m3/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming loads BOD-tot  t/y'], $i, $uwwtp[$ind['Incoming loads BOD-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged loads BOD-tot  t/y'], $i, $uwwtp[$ind['Discharged loads BOD-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['BOD rate of removal %'], $i, $uwwtp[$ind['BOD rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration BOD mg/l'], $i, $uwwtp[$ind['average Incoming concentration BOD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration BOD mg/l'], $i, $uwwtp[$ind['average Discharge concentration BOD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming cocentration COD-tot   t/y'], $i, $uwwtp[$ind['Incoming cocentration COD-tot   t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged concentration COD-tot  t/y'], $i, $uwwtp[$ind['Discharged concentration COD-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['COD rate of removal %'], $i, $uwwtp[$ind['COD rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration COD mg/l'], $i, $uwwtp[$ind['average Incoming concentration COD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration COD mg/l'], $i, $uwwtp[$ind['average Discharge concentration COD mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming loads N-tot   t/y'], $i, $uwwtp[$ind['Incoming loads N-tot   t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged loads N-tot  t/y'], $i, $uwwtp[$ind['Discharged loads N-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['N rate of removal %'], $i, $uwwtp[$ind['N rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration N mg/l'], $i, $uwwtp[$ind['average Incoming concentration N mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration N mg/l'], $i, $uwwtp[$ind['average Discharge concentration N mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['Incoming cocentration P-tot   t/y'], $i, $uwwtp[$ind['Incoming cocentration P-tot   t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['Discharged concentration P-tot  t/y'], $i, $uwwtp[$ind['Discharged concentration P-tot  t/y']]);
        $sheet->setCellValueByColumnAndRow($ind['P rate of removal %'], $i, $uwwtp[$ind['P rate of removal %']]);
        $sheet->setCellValueByColumnAndRow($ind['average Incoming concentration P mg/l'], $i, $uwwtp[$ind['average Incoming concentration P mg/l']]);
        $sheet->setCellValueByColumnAndRow($ind['average Discharge concentration P mg/l'], $i, $uwwtp[$ind['average Discharge concentration P mg/l']]);
        $i++;
        $sheet->insertNewRowBefore($i, 1);
//         if ($i%10 == 0) {
//             break;
//         }
    }
    fclose($ruwwtpfile);
   
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('file uwwtp sheet 2 finish');    
    
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
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['rca']['xlsx_name'] .'.xlsx';

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name']);
    
    $start_line = 6;
//     $sheet->insertNewRowBefore(7, count($listrca));   todo    $listrca exist pas
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet1_rca'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['rca']['csv_name']. '.csv';           
    $rrcafile = fopen($filename, 'rb');
    
    //ignore header //decomment for debug on xls final file
//     $rca = fgetcsv($rrcafile, 5000, ';');
    
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
 
    $context['message'] = t('file_rca_sheet1 finish');    
    
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

    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name'] .'.csv';      

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
    while ($j < 10 && $context['sandbox']['progressbc'] < $context['sandbox']['maxbc']) {              
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
        $treatment_0 = 'TODO';
        $treatment_1 = 'TODO';
        $treatment_2 = 'TODO';
        $treatment_3n = 'TODO';
        $treatment_3p = 'TODO';
        $treatment_3np = 'TODO';
        $treatment_3_other = 'TODO';
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
            $agg_detail = uwwtd_summary_big_cities_get_agglo_detail($agg);
//             dsm($agg_detail);
            unset($agg);
            $generated_load += $agg_detail['generated_load'];
            $collecting_system +=  $agg_detail['collecting_system'];
            $ias +=  $agg_detail['ias'];
            $no_collecting_system_nor_ias +=  $agg_detail['no_collecting_system_nor_ias'];
            
            $dcptype = array_merge($dcptype, $agg_detail['dcptype']);
//             dsm($dcptype);            
        }
        
        $type_of_receiving_area = implode(';', array_keys($dcptype));
//         dsm($type_of_receiving_area);
            
        $collecting_system_percent =  $collecting_system * 100 / $generated_load;
        $ias_percent =  $ias * 100 / $generated_load;
        $no_collecting_system_nor_ias_percent =  $no_collecting_system_nor_ias * 100 / $generated_load;
        
        $line[$ind['generated_load']] = $generated_load;
        $line[$ind['collecting_system']] = $collecting_system_percent;
        $line[$ind['ias']] = $ias_percent;
        $line[$ind['no_collecting_system_nor_ias']] = $no_collecting_system_nor_ias_percent;
        $line[$ind['treatment_0']] = $treatment_0;
        $line[$ind['treatment_1']] = $treatment_1;
        $line[$ind['treatment_2']] = $treatment_2;
        $line[$ind['treatment_3n_p_np']] = $treatment_3n; //tood
//         $line[$ind['treatment_3n']] = $treatment_3n;
//         $line[$ind['treatment_3p']] = $treatment_3p;
//         $line[$ind['treatment_3np']] = $treatment_3np;
        $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = $treatment_3_other;
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
        while ($j < 10 && $context['sandbox']['progressagg'] < $context['sandbox']['maxagg']) {      
    //     foreach($id as $nidagglo) {
            $line = array();
            $generated_load = 0;
            $collecting_system = 0;
            $ias = 0;
            $no_collecting_system_nor_ias = 0;
            $treatment_0 = 'TODO';
            $treatment_1 = 'TODO';
            $treatment_2 = 'TODO';
            $treatment_3n = 'TODO';
            $treatment_3p = 'TODO';
            $treatment_3np = 'TODO';
            $treatment_3_other = 'TODO';
            $type_of_receiving_area = '';
                    
            $agg = node_load($nidagglo['nid']);                                          
            $agg_detail = uwwtd_summary_big_cities_get_agglo_detail($agg);
             
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
            
            $line[$ind['treatment_3n_p_np']] = $treatment_3n;  //todo
//             $line[$ind['treatment_3n']] = $treatment_3n;
//             $line[$ind['treatment_3p']] = $treatment_3p;
//             $line[$ind['treatment_3np']] = $treatment_3np;
            $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = $treatment_3_other;
            $line[$ind['type_of_receiving_area']] = implode(';', $dcptype);
            unset($agg);
            fputcsv($r, $line, ';');
            
            $context['sandbox']['progressagg']++;
            $context['sandbox']['progress']++;
            $j++;            
        }
    }  
    
    fclose($r);
    
    $context['message'] = t('Processing big cities sheet 8 : ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max']);   
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }    
}

/**
 *
 */
function uwwtd_summary_big_cities_get_agglo_detail($agg)
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
    
   //for rca we need to load uwwtp -> dcp -> rca           
   foreach($agg->field_agguwwliste['und'] as $uwwtpnid) {
        $uwwtp = node_load($uwwtpnid['nid']);
        foreach($uwwtp->field_uwwdcpliste['und'] as $dcpnid) {
            $dcp = node_load($dcpnid['nid']);
            $result['dcptype'][$dcp->field_rcatype['und'][0]['value']] = 0;
            unset($dcp);
        }
        unset($uwwtp);
    }            
    unset($agg);

    return $result;
} 

/**
 *
 */ 
function uwwtd_create_register_2015_nuts_level2_analyse_sheet9($annee)
{
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
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name']);
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet8_summary_big_cities'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name']. '.csv';           
    $rbcfile = fopen($filename, 'rb');
    
    $start_line = 4;
    //ignore header //decomment for debug on xls final file
//     $bc = fgetcsv($rbcfile, 5000, ';');
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
        $sheet->setCellValueByColumnAndRow($ind['treatment_3n_p_np'], $i, $bc[$ind['treatment_3n']]);
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
 
    $context['message'] = t('summary_big_cities_sheet8 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}


/**
 *
 */ 
function uwwtd_create_register_2015_file_nuts_level2_analyse_sheet9($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_model_sheet_name']);    
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('nuts_level2_analyse_sheet9 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
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
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['sewage_sludge']['xlsx_model_sheet_name']);
    
    $nidms = uwwtd_check_exist(uwwtd_get_siteid('ms_level', array('year' => $annee)));
    
    if (false !== $nidms) {
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
        $volume_treated = 'TODO';
        //Rate of treated waste water re-used
        $reuse = 'TODO';
        //re-use in agriculture
        $reuse_agriculture = 'TODO';
        //re-use in industry
        $reuse_industry = 'TODO';
        //re-use others
        $reuse_others = 'TODO';
        //Remarks
        $remarks = 'TODO';
        
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
 
    $context['message'] = t('sewage_sludge_and_re_use_sheet10 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_graph_sheet11($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['graph']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name']);    
    
    //size of agglo/pe per class
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];           
    $ragglofile = fopen($filename, 'rb');
    
    $agglo = fgetcsv($ragglofile, 5000, ';');
    
    $graph_slice_size = array();
    $graph_slice_size['0_10000']['pe']  = 0;
    $graph_slice_size['0_10000']['number'] = 0;
    $graph_slice_size['10000_100000']['pe']  = 0;
    $graph_slice_size['10000_100000']['number'] = 0;
    $graph_slice_size['100000_n']['pe']  = 0;
    $graph_slice_size['100000_n']['number'] = 0;
            
    while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
        if ($agglo[$ind['generated_load_p.e.']] < 10001) {
            $graph_slice_size['0_10000']['pe']  += $agglo[$ind['generated_load_p.e.']];
            $graph_slice_size['0_10000']['number']++;        
        } elseif ($agglo[$ind['generated_load_p.e.']]  < 100000) {
            $graph_slice_size['10000_100000']['pe']  += $agglo[$ind['generated_load_p.e.']];
            $graph_slice_size['10000_100000']['number']++;             
        } else {   //>=100000
            $graph_slice_size['100000_n']['pe']  += $agglo[$ind['generated_load_p.e.']];
            $graph_slice_size['100000_n']['number']++;             
        }      
    }
    
    dsm($graph_slice_size);
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('graph_sheet11 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_new_agglomerations_sheet12($annee, &$context)
{
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filenameagglo = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';

    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_agglo']['csv_name'] . '.csv';    
    
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
            $context['message'] = t('new_agglomerations_sheet12 finish');
            $context['sandbox']['progress'] == $context['sandbox']['max'];
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
    while ($j < 10 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
//     get all agglo from csv file
//         check if agglo > 2000 and exist in previous year
//             if no add a line 
        if ($agglo[$indagglo['generated_load_p.e.']] >= 2000 
            && false === uwwtd_check_exist(uwwtd_get_siteid('agglomeration', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agglo[$indagglo['aggcode']])))) {
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
    
    $context['message'] = t('new_agglomerations_sheet12 Processing ... ' . $context['sandbox']['progress'] . '/'. $context['sandbox']['max']);    
    
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
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo_out']['csv_name'] . '.csv';    
    
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
            $context['message'] = t('agglomerationout_sheet13 finish');
            $context['sandbox']['progress'] == $context['sandbox']['max'];
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
    while ($j < 10 && $context['sandbox']['progress'] < $context['sandbox']['max']) {
    
        $previousagglo = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
        
        if (false !== ($agglonid = uwwtd_check_exist(uwwtd_get_siteid('agglomeration', array('year' => $annee, 'aggCode' => $previousagglo->field_inspireidlocalid['und'][0]['value'] ))))) {
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
   
    $context['message'] = t('agglomerationout_sheet13 Processing ... ' . $context['sandbox']['progress'] . '/'. $context['sandbox']['max']);    
    
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

    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filenameuwwtp = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] . '.csv';

    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_uwwtp']['csv_name'] . '.csv';    
    
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
            $context['message'] = t('new_treatment_plants_sheet14 finish');
            $context['sandbox']['progress'] == $context['sandbox']['max'];
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
    while ($j < 10 && $uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {
//     get all uwwtp from csv file
//         check if exist in previous year
//             if no add a line 
        if (false === uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $context['sandbox']['previous_year'], 'uwwCode' => $uwwtp[$induwwtp['uww_code']])))) {
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
    
    $context['message'] = t('new_treatment_plants_sheet14 Processing ... ' . $context['sandbox']['progress'] . '/'. $context['sandbox']['max']);    
    
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
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp_out']['csv_name'] . '.csv';    
    
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
            $context['message'] = t('uwwtpsout_sheet15 finish');
            $context['sandbox']['progress'] == $context['sandbox']['max'];
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
    while ($j < 10 && $context['sandbox']['progress'] < $context['sandbox']['max']) {
    
        $previousuwwtp = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
        
        if (false !== ($uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $annee, 'uwwCode' => $previousuwwtp->field_inspireidlocalid['und'][0]['value'] ))))) {
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
   
    $context['message'] = t('uwwtpsout_sheet15 Processing ... ' . $context['sandbox']['progress'] . '/'. $context['sandbox']['max']);    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}


/**
 *
 */ 
function uwwtd_create_register_2015_file_new_agglomerations_sheet12($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name']);
    
    $start_line = 6;
//     $sheet->insertNewRowBefore(5, count($listagglo));
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet12_newagglo'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_agglo']['csv_name'] . '.csv';
        
    $ragglofile = fopen($filename, 'rb');
    while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $i, $agglo[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $i, $agglo[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_agglomeration'], $i, $agglo[$ind['size_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['reason'], $i, $agglo[$ind['reason']]);

        $i++;
        $sheet->insertNewRowBefore($i, 1);
    }
    fclose($ragglofile);    
    
        
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('new_agglomerations_sheet12 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_agglomerationout_sheet13($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_model_sheet_name']);    
    
    
    $start_line = 6;
//     $sheet->insertNewRowBefore(5, count($listagglo));
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet13_aggloout'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo_out']['csv_name'] . '.csv';
        
    $ragglofile = fopen($filename, 'rb');
    while ($agglo = fgetcsv($ragglofile, 5000, ';')) {
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $i, $agglo[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $i, $agglo[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_agglomeration'], $i, $agglo[$ind['size_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['remarks'], $i, $agglo[$ind['remarks']]);

        $i++;
        $sheet->insertNewRowBefore($i, 1);
    }
    fclose($ragglofile);   
        
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('agglomerationout_sheet13 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_new_treatment_plants_sheet14($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_model_sheet_name']);    
    
    $start_line = 5;
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet14_newuwwtp'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['new_uwwtp']['csv_name'] . '.csv';
            
    $rfile = fopen($filename, 'rb');
    while ($line = fgetcsv($rfile, 5000, ';')) {
        $sheet->setCellValueByColumnAndRow($ind['id_of_uwwtp'], $i, $line[$ind['id_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_uwwtp'], $i, $line[$ind['name_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $i, $line[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $i, $line[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_uwwtp'], $i, $line[$ind['size_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['reason'], $i, $line[$ind['reason']]);

        $i++;
        $sheet->insertNewRowBefore($i, 1);
    }
    fclose($rfile);  
        
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('new_treatment_plants_sheet14 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_uwwtpsout_sheet15($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_model_sheet_name']);    
    
    $start_line = 6;
    
    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet15_uwwtpout'];
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp_out']['csv_name'] . '.csv';
                
    $rfile = fopen($filename, 'rb');
    while ($line = fgetcsv($rfile, 5000, ';')) {
        $sheet->setCellValueByColumnAndRow($ind['id_of_uwwtp'], $i, $line[$ind['id_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_uwwtp'], $i, $line[$ind['name_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $i, $line[$ind['id_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $i, $line[$ind['name_of_agglomeration']]);
        $sheet->setCellValueByColumnAndRow($ind['size_of_uwwtp'], $i, $line[$ind['size_of_uwwtp']]);
        $sheet->setCellValueByColumnAndRow($ind['remarks'], $i, $line[$ind['remarks']]);

        $i++;
        $sheet->insertNewRowBefore($i, 1);
    }
    fclose($rfile);  
        
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('uwwtpsout_sheet15 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }     
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
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';           
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
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['breach_list']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         'breach_list',
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
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
 
    $context['message'] = t('breach_list_sheet7 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }         
        
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_summary_installation_in_place_sheet6($annee, &$context) 
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    

    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    //MS total
    $summary['total']['number'] = 0;
    $summary['total']['percentage'] = 100;
    $summary['total']['waste_pe'] = 0;
    $summary['total']['waste_percent'] = 100;
    
    $summary['total']['ias_in_place']['number'] = 0;
    $summary['total']['ias_in_place']['percent'] = 0;
    $summary['total']['ias_in_place']['waste_pe'] = 0;
    $summary['total']['ias_in_place']['waste_percent'] = 0;    
    
    $summary['total']['collecting_system']['number'] = 0;
    $summary['total']['collecting_system']['percent'] = 0;
    $summary['total']['collecting_system']['waste_pe'] = 0;
    $summary['total']['collecting_system']['waste_percent'] = 0;
    //secondary treatment    
    $summary['secondary']['installation_in_place']['number'] = 0;
    $summary['secondary']['installation_in_place']['percent'] = 0;
    $summary['secondary']['installation_in_place']['waste_pe'] = 0;
    $summary['secondary']['installation_in_place']['waste_percent'] = 0;
        
    $summary['secondary']['meet_requirement']['number'] = 0;
    $summary['secondary']['meet_requirement']['percent'] = 0;
    $summary['secondary']['meet_requirement']['waste_pe'] = 0;
    $summary['secondary']['meet_requirement']['waste_percent'] = 0;    
    //more stringent treatment
    $summary['more_stringent']['installation_in_place']['number'] = 0;
    $summary['more_stringent']['installation_in_place']['percent'] = 0;
    $summary['more_stringent']['installation_in_place']['waste_pe'] = 0;
    $summary['more_stringent']['installation_in_place']['waste_percent'] = 0;
        
    $summary['more_stringent']['meet_requirement']['number'] = 0;
    $summary['more_stringent']['meet_requirement']['percent'] = 0;
    $summary['more_stringent']['meet_requirement']['waste_pe'] = 0;
    $summary['more_stringent']['meet_requirement']['waste_percent'] = 0;    
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';           
    $ragglofile = fopen($filename, 'rb');
    
    $agglo = fgetcsv($ragglofile, 5000, ';');
    
    while ($agglo = fgetcsv($ragglofile, 5000, ';')) {  
        
//     foreach ($listagglo as $agglo) {
    
        //MS total
        $summary['total']['number']++;                
        $summary['total']['waste_pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load
                                       
        if ($agglo[$ind['addressed_through_ias_p._e.']] > 0) {
            $summary['total']['ias_in_place']['number']++;         
            $summary['total']['ias_in_place']['waste_pe'] += $agglo[$ind['addressed_through_ias_p._e.']];  //addressed through IAS pe 
        }                                                     
        if ($agglo[$ind['connected_to_collecting_system_p._e.']] > 0) {                                  
            $summary['total']['collecting_system']['number']++;       
            $summary['total']['collecting_system']['waste_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']]; //connected to collecting system pe   
        }
        
        //secondary treatment
        //•	Number summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, secondary treatment installed, p.e.’, where parameter is  >0. (Excel-function: COUNTIF(‘p.e. for which installation for secondary treatment is in place’;”>0”)
        //•	p.e. summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, secondary treatment installed, p.e.’: sum of the entire column Excel-function: SUM (‘p.e. for which installation for secondary treatment is in place’)
        if ($agglo[$ind['secondary_treatment_installed_(p._e.)']] > 0) {
            $summary['secondary']['installation_in_place']['number']++;
            $summary['secondary']['installation_in_place']['waste_pe'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
        }
        
        //•	Number summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, monitoring results f. secondary OK, p.e.’, where parameter is  >0 (Excel-function: COUNTIF(‘p.e. for which monitoring results meet the requirements for discharge concerning secondary treatment’;”>0”)
        //•	p.e. summarized from sheet ‘Agglomerations’, hidden column ‘Installations for wastewater treatment in place, monitoring results f. secondary OK, p.e.’: sum of the entire column Excel-function: SUM (‘p.e. for which monitoring results meet the requirements for discharge concerning secondary treatment’)
        if ($agglo[$ind['monitoring_results_f._secondary_ok']] > 0) {
            $summary['secondary']['meet_requirement']['number']++;
            $summary['secondary']['meet_requirement']['waste_pe'] += $agglo[$ind['monitoring_results_f._secondary_ok']];
        }        
        
        //more stringent treatment
        if ($agglo[$ind['more_str._treatment_installed_(p._e.)']] > 0) {
            $summary['secondary']['installation_in_place']['number']++;
            $summary['secondary']['installation_in_place']['waste_pe'] += $agglo[$ind['more_str._treatment_installed_(p._e.)']];
        }

        if ($agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']] > 0) {
            $summary['secondary']['meet_requirement']['number']++;
            $summary['secondary']['meet_requirement']['waste_pe'] += $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']];
        }           
    }
    fclose($ragglofile);
    
    //calculate percentage
    //MS total
    $summary['total']['ias_in_place']['percent'] = $summary['total']['ias_in_place']['number'] * 100 / $summary['total']['number'];
    $summary['total']['ias_in_place']['waste_percent'] = $summary['total']['ias_in_place']['waste_pe'] * 100 / $summary['total']['waste_pe'];    

    $summary['total']['collecting_system']['percent'] = $summary['total']['collecting_system']['number'] * 100 / $summary['total']['number'];
    $summary['total']['collecting_system']['waste_percent'] = $summary['total']['collecting_system']['waste_pe'] * 100 / $summary['total']['waste_pe'];    
    
    //secondary treatment    
    $summary['secondary']['installation_in_place']['percent'] = $summary['secondary']['installation_in_place']['number'] * 100 /  $summary['total']['number'];
    $summary['secondary']['installation_in_place']['waste_percent'] = $summary['secondary']['installation_in_place']['waste_pe'] * 100 / $summary['total']['waste_pe'];
        
    $summary['secondary']['meet_requirement']['percent'] = $summary['secondary']['meet_requirement']['number'] * 100 /  $summary['total']['number'];
    $summary['secondary']['meet_requirement']['waste_percent'] = $summary['secondary']['meet_requirement']['waste_pe'] * 100 / $summary['total']['waste_pe'];    
    //more stringent treatment
    $summary['more_stringent']['installation_in_place']['percent'] = $summary['more_stringent']['installation_in_place']['number'] * 100 /  $summary['total']['number'];
    $summary['more_stringent']['installation_in_place']['waste_percent'] = $summary['more_stringent']['installation_in_place']['waste_pe'] * 100 / $summary['total']['waste_pe'];
        
    $summary['more_stringent']['meet_requirement']['percent'] = $summary['more_stringent']['meet_requirement']['number'] * 100 /  $summary['total']['number'];
    $summary['more_stringent']['meet_requirement']['waste_percent'] = $summary['more_stringent']['meet_requirement']['waste_pe'] * 100 / $summary['total']['waste_pe'];
    
//     return $summary;
//     dsm($summary);
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['summary_installation_in_place']['xlsx_model_sheet_name']);    
        
    
    //country name
    $path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
    include($path);
    $country_code = variable_get('siif_eru_country_code', '-1');
    $country_name = $country_config[$country_code]['name'];
    $reference_date = '2012/12/31';    
    $sheet->setCellValue('E1', $country_name);
    unset($country_config);
    $sheet->setCellValue('E2', $reference_date);
    
    $cell = $sheet->getCell('B6');
    $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
    $cell = $sheet->getCell('B8');  
    $cell->setValue(str_replace(array('[#COUNTRY#]', '[#DATE#]'), array($country_name, $reference_date), $cell->getValue()));
    
    
    $sheet->getCell('C10')->setValue($summary['total']['number']);
    $sheet->getCell('D10')->setValue($summary['total']['percentage']);
    $sheet->getCell('E10')->setValue($summary['total']['waste_pe']);
    $sheet->getCell('F10')->setValue($summary['total']['waste_percent']);
    
    $sheet->getCell('C11')->setValue($summary['total']['ias_in_place']['number']);
    $sheet->getCell('D11')->setValue($summary['total']['ias_in_place']['percent']);
    $sheet->getCell('E11')->setValue($summary['total']['ias_in_place']['waste_pe']);
    $sheet->getCell('F11')->setValue($summary['total']['ias_in_place']['waste_percent']);    
    
    $sheet->getCell('C12')->setValue($summary['total']['collecting_system']['number']);
    $sheet->getCell('D12')->setValue($summary['total']['collecting_system']['percent']);
    $sheet->getCell('E12')->setValue($summary['total']['collecting_system']['waste_pe']);
    $sheet->getCell('F12')->setValue($summary['total']['collecting_system']['waste_percent']);
    //secondary treatment    
    $sheet->getCell('C15')->setValue($summary['secondary']['installation_in_place']['number']);
    $sheet->getCell('D15')->setValue($summary['secondary']['installation_in_place']['percent']);
    $sheet->getCell('E15')->setValue($summary['secondary']['installation_in_place']['waste_pe']);
    $sheet->getCell('F15')->setValue($summary['secondary']['installation_in_place']['waste_percent']);
        
    $sheet->getCell('C16')->setValue($summary['secondary']['meet_requirement']['number']);
    $sheet->getCell('D16')->setValue($summary['secondary']['meet_requirement']['percent']);
    $sheet->getCell('E16')->setValue($summary['secondary']['meet_requirement']['waste_pe']);
    $sheet->getCell('F16')->setValue($summary['secondary']['meet_requirement']['waste_percent']);    
    //more stringent treatment
    $sheet->getCell('C19')->setValue($summary['more_stringent']['installation_in_place']['number']);
    $sheet->getCell('D19')->setValue($summary['more_stringent']['installation_in_place']['percent']);
    $sheet->getCell('E19')->setValue($summary['more_stringent']['installation_in_place']['waste_pe']);
    $sheet->getCell('F19')->setValue($summary['more_stringent']['installation_in_place']['waste_percent']);
        
    $sheet->getCell('C20')->setValue($summary['more_stringent']['meet_requirement']['number']);
    $sheet->getCell('D20')->setValue($summary['more_stringent']['meet_requirement']['percent']);
    $sheet->getCell('E20')->setValue($summary['more_stringent']['meet_requirement']['waste_pe']);
    $sheet->getCell('F20')->setValue($summary['more_stringent']['meet_requirement']['waste_percent']);  
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('summary_installation_in_place_sheet6 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }              
}



/**
 *
 */ 
function uwwtd_create_register_2015_file_distance_to_compliance_sheet5($annee, &$context)
{
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    } 
    
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['distance_to_comp']['xlsx_model_sheet_name']);
    
    //country name
    $path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
    include($path);
    $country_code = variable_get('siif_eru_country_code', '-1');
    $country_name = $country_config[$country_code]['name'];
    $reference_date = '2012/12/31';    
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
    
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('distance_to_compliance_sheet5 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }       
}

/**
 *
 */ 
function uwwtd_create_register_2015_file_summary_legal_compliance_sheet4($annee, &$context) 
{
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    
	if (!isset($context['sandbox']['progress'])) {    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }    
//     $header = array(
// 
// 
//     );
//     
//     $filename = $annee . '_summary_legal_compliance_sheet4.csv';
//     $tmpfilename = tempnam('public://tmp/', $filename);
//     $r = fopen($tmpfilename, 'wb');
//     fputcsv($r, $header, ';');
    
    foreach(array('all', 'NA', 'CSA/SA') as $type) {
        //art3
        $compliance[$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] = 0;
        $compliance[$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage'] = 0;    
    
        $compliance[$type]['art3']['connected_to_ias']['waste_water']['pe'] = 0;
        $compliance[$type]['art3']['connected_to_ias']['waste_water']['percentage'] = 0; 
    
        $compliance[$type]['art3']['not_connected']['waste_water']['pe'] = 0;
        $compliance[$type]['art3']['not_connected']['waste_water']['percentage'] = 0;     
        
        $compliance[$type]['art3']['total_actual']['agglo']['number'] = 0;
        $compliance[$type]['art3']['total_actual']['agglo']['percentage'] = 0;
        $compliance[$type]['art3']['total_actual']['waste_water']['pe'] = 0;
        $compliance[$type]['art3']['total_actual']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art3']['target']['agglo']['number'] = 0;
        $compliance[$type]['art3']['target']['agglo']['percentage'] = 0;
        $compliance[$type]['art3']['target']['waste_water']['pe'] = 0;
        $compliance[$type]['art3']['target']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art3']['exempt_due_to_transition']['agglo']['number'] = 0;
        $compliance[$type]['art3']['exempt_due_to_transition']['agglo']['percentage'] = 0;
        $compliance[$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] = 0;
        $compliance[$type]['art3']['exempt_due_to_transition']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art3']['country_total']['agglo']['number'] = 0;
        $compliance[$type]['art3']['country_total']['agglo']['percentage'] = 100;
        $compliance[$type]['art3']['country_total']['waste_water']['pe'] = 0;
        $compliance[$type]['art3']['country_total']['waste_water']['percentage'] = 100;
        
        //art4
        $compliance[$type]['art4']['total_actual']['agglo']['number'] = 0;
        $compliance[$type]['art4']['total_actual']['agglo']['percentage'] = 0;
        $compliance[$type]['art4']['total_actual']['waste_water']['pe'] = 0;
        $compliance[$type]['art4']['total_actual']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art4']['target']['agglo']['number'] = 0;
        $compliance[$type]['art4']['target']['agglo']['percentage'] = 0;
        $compliance[$type]['art4']['target']['waste_water']['pe'] = 0;
        $compliance[$type]['art4']['target']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art4']['exempt_due_to_transition']['agglo']['number'] = 0;
        $compliance[$type]['art4']['exempt_due_to_transition']['agglo']['percentage'] = 0;
        $compliance[$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] = 0;
        $compliance[$type]['art4']['exempt_due_to_transition']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art4']['country_total']['agglo']['number'] = 0;
        $compliance[$type]['art4']['country_total']['agglo']['percentage'] = 100;
        $compliance[$type]['art4']['country_total']['waste_water']['pe'] = 0;
        $compliance[$type]['art4']['country_total']['waste_water']['percentage'] = 100; 
        
        //art5
        $compliance[$type]['art5']['total_actual']['agglo']['number'] = 0;
        $compliance[$type]['art5']['total_actual']['agglo']['percentage'] = 0;
        $compliance[$type]['art5']['total_actual']['waste_water']['pe'] = 0;
        $compliance[$type]['art5']['total_actual']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art5']['target']['agglo']['number'] = 0;
        $compliance[$type]['art5']['target']['agglo']['percentage'] = 0;
        $compliance[$type]['art5']['target']['waste_water']['pe'] = 0;
        $compliance[$type]['art5']['target']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art5']['exempt_due_to_transition']['agglo']['number'] = 0;
        $compliance[$type]['art5']['exempt_due_to_transition']['agglo']['percentage'] = 0;
        $compliance[$type]['art5']['exempt_due_to_transition']['waste_water']['pe'] = 0;
        $compliance[$type]['art5']['exempt_due_to_transition']['waste_water']['percentage'] = 0; 
        
        $compliance[$type]['art5']['country_total']['agglo']['number'] = 0;
        $compliance[$type]['art5']['country_total']['agglo']['percentage'] = 100;
        $compliance[$type]['art5']['country_total']['waste_water']['pe'] = 0;
        $compliance[$type]['art5']['country_total']['waste_water']['percentage'] = 100;
    }          
    
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] . '.csv';           
    $ragglofile = fopen($filename, 'rb');
    
    $agglo = fgetcsv($ragglofile, 5000, ';');
    
//     dsm($compliance);
    while ($agglo = fgetcsv($ragglofile, 5000, ';')) {    
//     foreach ($listagglo as $agglo) {
//         dsm($agglo);  
        foreach(array('all', 'NA', 'CSA/SA') as $type) {
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
                $compliance[$type]['art3']['total_actual']['agglo']['number']++;      
                $compliance[$type]['art3']['total_actual']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
    
    //             'connected to collecting system % of gen. load',
    //             'connected to collecting system p. e.',
    //             'addressed through IAS % of gen. load',
    //             'addressed through IAS p. e.',
    //             'neither collecting system nor IAS % of gen. load',
    //             'neither collecting system nor IAS p. e.',                                               
                $compliance[$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage'] += $agglo[$ind['connected_to_collecting_system']];   
                $compliance[$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];       
                $compliance[$type]['art3']['connected_to_ias']['waste_water']['percentage'] += $agglo[$ind['addressed_through_ias_%_of_gen._load']];      
                $compliance[$type]['art3']['connected_to_ias']['waste_water']['pe'] += $agglo[$ind['addressed_through_ias_p._e.']];          
                $compliance[$type]['art3']['not_connected']['waste_water']['percentage'] += $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];       
                $compliance[$type]['art3']['not_connected']['waste_water']['pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];              
            }                                
                                  //compliance art3
            if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC', 'NC'))) {    
                $compliance[$type]['art3']['target']['agglo']['number']++;              
                $compliance[$type]['art3']['target']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
            }  
                                  //compliance art3
            if (true === in_array($agglo[$ind['art3_compliance']], array('NR'))) {                
                $compliance[$type]['art3']['exempt_due_to_transition']['agglo']['number']++;              
                $compliance[$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
            } 
            
            $compliance[$type]['art3']['country_total']['agglo']['number']++;            
            $compliance[$type]['art3']['country_total']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
            
//             dsm('art4');
//             dsm($ind['art4_compliance_art._4_(hierarchical_approach)']);
            //art 4           
                //Compliance Art. 4 (hierarchical approach)
            if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {                                              
                $compliance[$type]['art4']['total_actual']['agglo']['number']++;      
                $compliance[$type]['art4']['total_actual']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
            }
            
            if (true === in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('C', 'NC'))) {      
                $compliance[$type]['art4']['target']['agglo']['number']++;         
                $compliance[$type]['art4']['target']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
            }
            
            if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NR') {   
                $compliance[$type]['art4']['exempt_due_to_transition']['agglo']['number']++;    
                $compliance[$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                
                //todo
    //            - due to a size of 2000p.e. - 10000 p.e.     …..and discharge into coastal water
    //            - due to 0% collection in collecting …...system
    //            - due to discharge into LSA
    //            - due to transition period            
            }
            
            $compliance[$type]['art4']['country_total']['agglo']['number']++;    
            $compliance[$type]['art4']['country_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
            
            //art 5 
//             dsm('art5');
//             dsm($ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']);       
                //Compliance Art. 5 (whole agglo) (hierarchical approach)
            if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {                                           
                $compliance[$type]['art5']['total_actual']['agglo']['number']++;      
                $compliance[$type]['art5']['total_actual']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];  //connected to collecting system p. e.
            }
            
            if (true === in_array($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']], array('C', 'NC'))) {      
                $compliance[$type]['art5']['target']['agglo']['number']++;      
                $compliance[$type]['art5']['target']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
            }
            
            if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NR') {   
                $compliance[$type]['art5']['exempt_due_to_transition']['agglo']['number']++;    
                $compliance[$type]['art5']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
            }
            
            $compliance[$type]['art5']['country_total']['agglo']['number']++;    
            $compliance[$type]['art5']['country_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
        }        
    }
//     dsm($compliance);
    fclose($ragglofile); 
    
    foreach(array('all', 'NA', 'CSA/SA') as $type) {
        if (false === isset($compliance[$type])) {
            continue;
        }
        //calcul percentage art3
        if ($compliance[$type]['art3']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art3']['total_actual']['agglo']['percentage'] = $compliance[$type]['art3']['total_actual']['agglo']['number'] * 100 / $compliance[$type]['art3']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art3']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art3']['total_actual']['waste_water']['percentage'] = $compliance[$type]['art3']['total_actual']['waste_water']['pe'] * 100 / $compliance[$type]['art3']['country_total']['waste_water']['pe'];
        } 
            
        if ($compliance[$type]['art3']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art3']['target']['agglo']['percentage'] = $compliance[$type]['art3']['target']['agglo']['number'] * 100 / $compliance[$type]['art3']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art3']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art3']['target']['waste_water']['percentage'] = $compliance[$type]['art3']['target']['waste_water']['pe'] * 100 / $compliance[$type]['art3']['country_total']['waste_water']['pe'];
        }
        
        if ($compliance[$type]['art3']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art3']['exempt_due_to_transition']['agglo']['percentage'] = $compliance[$type]['art3']['exempt_due_to_transition']['agglo']['number'] * 100 / $compliance[$type]['art3']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art3']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art3']['exempt_due_to_transition']['waste_water']['percentage'] = $compliance[$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] * 100 / $compliance[$type]['art3']['country_total']['waste_water']['pe'];
        }
        
        //calcul percentage art4
        if ($compliance[$type]['art4']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art4']['total_actual']['agglo']['percentage'] = $compliance[$type]['art4']['total_actual']['agglo']['number'] * 100 / $compliance[$type]['art4']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art4']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art4']['total_actual']['waste_water']['percentage'] = $compliance[$type]['art4']['total_actual']['waste_water']['pe'] * 100 / $compliance[$type]['art4']['country_total']['waste_water']['pe']; 
        }
            
        if ($compliance[$type]['art4']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art4']['target']['agglo']['percentage'] = $compliance[$type]['art4']['target']['agglo']['number'] * 100 / $compliance[$type]['art4']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art4']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art4']['target']['waste_water']['percentage'] = $compliance[$type]['art4']['target']['waste_water']['pe'] * 100 / $compliance[$type]['art4']['country_total']['waste_water']['pe'];
        }
        
        if ($compliance[$type]['art4']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art4']['exempt_due_to_transition']['agglo']['percentage'] = $compliance[$type]['art4']['exempt_due_to_transition']['agglo']['number'] * 100 / $compliance[$type]['art4']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art4']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art4']['exempt_due_to_transition']['waste_water']['percentage'] = $compliance[$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] * 100 / $compliance[$type]['art4']['country_total']['waste_water']['pe'];
        }
        
        //calcul percentage art5
        if ($compliance[$type]['art5']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art5']['total_actual']['agglo']['percentage'] = $compliance[$type]['art5']['total_actual']['agglo']['number'] * 100 / $compliance[$type]['art5']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art5']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art5']['total_actual']['waste_water']['percentage'] = $compliance[$type]['art5']['total_actual']['waste_water']['pe'] * 100 / $compliance[$type]['art5']['country_total']['waste_water']['pe']; 
        }
            
        if ($compliance[$type]['art5']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art5']['target']['agglo']['percentage'] = $compliance[$type]['art5']['target']['agglo']['number'] * 100 / $compliance[$type]['art5']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art5']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art5']['target']['waste_water']['percentage'] = $compliance[$type]['art5']['target']['waste_water']['pe'] * 100 / $compliance[$type]['art5']['country_total']['waste_water']['pe'];
        }
        
        if ($compliance[$type]['art5']['country_total']['agglo']['number'] != 0) {
            $compliance[$type]['art5']['exempt_due_to_transition']['agglo']['percentage'] = $compliance[$type]['art5']['exempt_due_to_transition']['agglo']['number'] * 100 / $compliance[$type]['art5']['country_total']['agglo']['number'];
        }
        if ($compliance[$type]['art5']['country_total']['waste_water']['pe'] != 0) {
            $compliance[$type]['art5']['exempt_due_to_transition']['waste_water']['percentage'] = $compliance[$type]['art5']['exempt_due_to_transition']['waste_water']['pe'] * 100 / $compliance[$type]['art5']['country_total']['waste_water']['pe'];
        }
    }
//     dsm($compliance);

//     dsm($compliance);
//     return $compliance;


//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
    
    $model_path = $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
    //change to public://
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_name'] .'.xlsx';
    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load($model_path);
    
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['summary_legal_comp']['xlsx_model_sheet_name']);
    
    //country name
    $path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
    include($path);
    $country_code = variable_get('siif_eru_country_code', '-1');
    $country_name = $country_config[$country_code]['name'];
    $reference_date = '2012/12/31';    
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

    foreach(array('all', 'NA', 'CSA/SA') as $type) {
//     foreach(array('all') as $type) {
        if (false === isset($compliance[$type])) {
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
                
        $sheet->getCell($col['E'].'12')->setValue($compliance[$type]['art3']['connected_to_collected_sytem']['waste_water']['pe']);
        $sheet->getCell($col['F'].'12')->setValue($compliance[$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage']);
        $sheet->getCell($col['E'].'13')->setValue($compliance[$type]['art3']['connected_to_ias']['waste_water']['pe']);
        $sheet->getCell($col['F'].'13')->setValue($compliance[$type]['art3']['connected_to_ias']['waste_water']['percentage']);
        $sheet->getCell($col['E'].'14')->setValue($compliance[$type]['art3']['not_connected']['waste_water']['pe']);
        $sheet->getCell($col['F'].'14')->setValue($compliance[$type]['art3']['not_connected']['waste_water']['percentage']);
           
        $sheet->getCell($col['C'].'15')->setValue($compliance[$type]['art3']['total_actual']['agglo']['number']);
        $sheet->getCell($col['D'].'15')->setValue($compliance[$type]['art3']['total_actual']['agglo']['percentage']);
        $sheet->getCell($col['E'].'15')->setValue($compliance[$type]['art3']['total_actual']['waste_water']['pe']);
        $sheet->getCell($col['F'].'15')->setValue($compliance[$type]['art3']['total_actual']['waste_water']['percentage']);
        
        $sheet->getCell($col['C'].'16')->setValue($compliance[$type]['art3']['target']['agglo']['number']);
        $sheet->getCell($col['D'].'16')->setValue($compliance[$type]['art3']['target']['agglo']['percentage']);
        $sheet->getCell($col['E'].'16')->setValue($compliance[$type]['art3']['target']['waste_water']['pe']);
        $sheet->getCell($col['F'].'16')->setValue($compliance[$type]['art3']['target']['waste_water']['percentage']);
        
        $sheet->getCell($col['C'].'17')->setValue($compliance[$type]['art3']['exempt_due_to_transition']['agglo']['number']);
        $sheet->getCell($col['D'].'17')->setValue($compliance[$type]['art3']['exempt_due_to_transition']['agglo']['percentage']);
        $sheet->getCell($col['E'].'17')->setValue($compliance[$type]['art3']['exempt_due_to_transition']['waste_water']['pe']);
        $sheet->getCell($col['F'].'17')->setValue($compliance[$type]['art3']['exempt_due_to_transition']['waste_water']['percentage']);
        
        $sheet->getCell($col['C'].'18')->setValue($compliance[$type]['art3']['country_total']['agglo']['number']);
        $sheet->getCell($col['D'].'18')->setValue($compliance[$type]['art3']['country_total']['agglo']['percentage']);
        $sheet->getCell($col['E'].'18')->setValue($compliance[$type]['art3']['country_total']['waste_water']['pe']);
        $sheet->getCell($col['F'].'18')->setValue($compliance[$type]['art3']['country_total']['waste_water']['percentage']);
        
        //art4
        $sheet->getCell($col['C'].'21')->setValue($compliance[$type]['art4']['total_actual']['agglo']['number']);
        $sheet->getCell($col['D'].'21')->setValue($compliance[$type]['art4']['total_actual']['agglo']['percentage']);
        $sheet->getCell($col['E'].'21')->setValue($compliance[$type]['art4']['total_actual']['waste_water']['pe']);
        $sheet->getCell($col['F'].'21')->setValue($compliance[$type]['art4']['total_actual']['waste_water']['percentage']); 
        
        $sheet->getCell($col['C'].'22')->setValue($compliance[$type]['art4']['target']['agglo']['number']);
        $sheet->getCell($col['D'].'22')->setValue($compliance[$type]['art4']['target']['agglo']['percentage']);
        $sheet->getCell($col['E'].'22')->setValue($compliance[$type]['art4']['target']['waste_water']['pe']);
        $sheet->getCell($col['F'].'22')->setValue($compliance[$type]['art4']['target']['waste_water']['percentage']); 
        
        $sheet->getCell($col['C'].'23')->setValue($compliance[$type]['art4']['exempt_due_to_transition']['agglo']['number']);
        $sheet->getCell($col['D'].'23')->setValue($compliance[$type]['art4']['exempt_due_to_transition']['agglo']['percentage']);
        $sheet->getCell($col['E'].'23')->setValue($compliance[$type]['art4']['exempt_due_to_transition']['waste_water']['pe']);
        $sheet->getCell($col['F'].'23')->setValue($compliance[$type]['art4']['exempt_due_to_transition']['waste_water']['percentage']); 
        
        $sheet->getCell($col['C'].'28')->setValue($compliance[$type]['art4']['country_total']['agglo']['number']);
        $sheet->getCell($col['D'].'28')->setValue($compliance[$type]['art4']['country_total']['agglo']['percentage']);
        $sheet->getCell($col['E'].'28')->setValue($compliance[$type]['art4']['country_total']['waste_water']['pe']);
        $sheet->getCell($col['F'].'28')->setValue($compliance[$type]['art4']['country_total']['waste_water']['percentage']);
        
        //art5
        $sheet->getCell($col['C'].'31')->setValue($compliance[$type]['art5']['total_actual']['agglo']['number']);
        $sheet->getCell($col['D'].'31')->setValue($compliance[$type]['art5']['total_actual']['agglo']['percentage']);
        $sheet->getCell($col['E'].'31')->setValue($compliance[$type]['art5']['total_actual']['waste_water']['pe']);
        $sheet->getCell($col['F'].'31')->setValue($compliance[$type]['art5']['total_actual']['waste_water']['percentage']); 
        
        $sheet->getCell($col['C'].'32')->setValue($compliance[$type]['art5']['target']['agglo']['number']);
        $sheet->getCell($col['D'].'32')->setValue($compliance[$type]['art5']['target']['agglo']['percentage']);
        $sheet->getCell($col['E'].'32')->setValue($compliance[$type]['art5']['target']['waste_water']['pe']);
        $sheet->getCell($col['F'].'32')->setValue($compliance[$type]['art5']['target']['waste_water']['percentage']); 
        
        $sheet->getCell($col['C'].'33')->setValue($compliance[$type]['art5']['exempt_due_to_transition']['agglo']['number']);
        $sheet->getCell($col['D'].'33')->setValue($compliance[$type]['art5']['exempt_due_to_transition']['agglo']['percentage']);
        $sheet->getCell($col['E'].'33')->setValue($compliance[$type]['art5']['exempt_due_to_transition']['waste_water']['pe']);
        $sheet->getCell($col['F'].'33')->setValue($compliance[$type]['art5']['exempt_due_to_transition']['waste_water']['percentage']); 
        
        $sheet->getCell($col['C'].'41')->setValue($compliance[$type]['art5']['country_total']['agglo']['number']);
        $sheet->getCell($col['D'].'41')->setValue($compliance[$type]['art5']['country_total']['agglo']['percentage']);
        $sheet->getCell($col['E'].'41')->setValue($compliance[$type]['art5']['country_total']['waste_water']['pe']);
        $sheet->getCell($col['F'].'41')->setValue($compliance[$type]['art5']['country_total']['waste_water']['percentage']);      
                        
//         break;       
        unset($compliance[$type]);
    }
    unset($compliance);
   
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    unset($objWriter);
    unset($objReader);    
    
    
    $context['sandbox']['progress']++;
 
    $context['message'] = t('file_summary_legal_compliance_sheet4 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }                                             
}

/**
 *
 */ 
function uwwtd_create_register_2015_agglo_sheet3($annee, &$context) 
{
    $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
    $induww = $GLOBALS['uwwtd']['sheet2_uwwtp'];
        
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
        $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
        $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] .'.csv';           
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
            } 
//             else {
//                 $context['sandbox']['uwwtpinfo_for_agglo'] &= $context['sandbox']['uwwtpinfo_for_agglo'];
//             } 
//             dsm($context['sandbox']['uwwtpinfo_for_agglo']);    
            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'] += $uwwtp[$induww['uww_entering load  p. e.']];
            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['capacity'] += $uwwtp[$induww['uww_capacity p. e.']];                          
                        
            if ($uwwtp[$induww['art4_final']] == 'C') {          
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['pe']  += $uwwtp[$induww['collected in collecting system  p. e.']];     
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['percent']  += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
            }
            if ($uwwtp[$induww['art4_final']] == 'NC') {              
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['pe']      += $uwwtp[$induww['collected in collecting system  p. e.']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['percent'] += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
            }
            if ($uwwtp[$induww['art4_final']] == 'NR') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['pe']      += $uwwtp[$induww['collected in collecting system  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['percent'] += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
            }
            
            if ($uwwtp[$induww['art4_final']] == 'NI') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['pe']      += $uwwtp[$induww['collected in collecting system  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['percent'] += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
            }
            
            if ($uwwtp[$induww['art4_final']] == 'PD') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['pe']      += $uwwtp[$induww['collected in collecting system  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['percent'] += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
            }
            
            if ($uwwtp[$induww['art4_final']] == '?') {  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['pe']      += $uwwtp[$induww['collected in collecting system  p. e.']];  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['percent'] += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
            }                                    
            
            //Secondary: installed?
            if ($uwwtp[$induww['Secondary: installed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_installed'] = 1;  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
            }   
            
            if ($uwwtp[$induww['Secondary:  monitoring passed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_passed'] = 1;  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
            }                               
            
            //tertiary install
            if ($uwwtp[$induww['More stringent:  installed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_installed'] = 1;  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
            }   
            
            if ($uwwtp[$induww['More stringent: monitoring passed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_passed'] = 1;  
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
            }
//             }
        }
        fclose($ruwwtpfile);
//         dsm($context['sandbox']['uwwtpinfo_for_agglo']);          
	}
    
//     dsm($context['sandbox']['uwwtpinfo_for_agglo']);
//     return '';
    
   
    //get all uwwtp (repeat agglo info if one agglo has more than one uwwtp)

    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['agglo']['csv_name'] .'.csv';             
    
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
            $ind['previous_art5_treat_%_of_gen._load'] => '% of gen. Load',
            $ind['previous_art5_treat_in_p.e._(6)'] => 'in p.e. (6)',
            //performance'] => //Performance',
            $ind['previous_art5_perf_%_of_gen._load'] => '% of gen. Load',
            $ind['previous_art5_perf_in_p.e._(7)'] => 'in p.e. (7)',
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
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');  
    }
    

//     $allline = array();
//     foreach($listagg as $nidagg) {
    $j = 0;
    while ($j < 10 && $context['sandbox']['progress'] < $context['sandbox']['max']) {    
        $line = array();
//         $agg = node_load($nidagg);
//         dsm($context['sandbox']['id'][$context['sandbox']['progress'] + $j]);
//         dsm($context['sandbox']['id']);
//         break;
        $agg = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
        
//         //todo to move outside the loop, check memory
//         $uwwtpinfo_for_agglo = array();
//         foreach ($listuwwtp as $uwwtp) {
//             $uww_aggcode = $uwwtp[$induww['aggcode']];
//             if ($uww_aggcode == $agg->field_inspireidlocalid['und'][0]['value']) {
//                 if (false === isset($uwwtpinfo_for_agglo[$uww_aggcode])) {
// 
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['entering'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['capacity'] = 0;
//                                     
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['C']['pe']       = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['C']['percent']  = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NC']['pe']      = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NC']['percent'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NR']['pe']      = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NR']['percent'] = 0;
//                     
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_installed'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_treatement_percent'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_treatement_pe'] = 0;
//                     
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_passed'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_monitoring_percent'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_monitoring_pe'] = 0;    
//                     
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_installed'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_treatement_percent'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_treatement_pe'] = 0;
//                     
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_passed'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_monitoring_percent'] = 0;
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_monitoring_pe'] = 0;                                        
//                 }
//                 
//                 $uwwtpinfo_for_agglo[$uww_aggcode]['entering'] += $uwwtp[$induww['uww_entering load  p. e.']];
//                 $uwwtpinfo_for_agglo[$uww_aggcode]['capacity'] += $uwwtp[$induww['uww_capacity p. e.']];                          
//                             
//                 if ($uwwtp[$induww['art4_final']] == 'C') {          
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['C']['pe']  += $uwwtp[$induww['collected in collecting system  p. e.']];     
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['C']['percent']  += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
//                 }
//                 if ($uwwtp[$induww['art4_final']] == 'NC') {              
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NC']['pe']      += $uwwtp[$induww['collected in collecting system  p. e.']]; 
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NC']['percent'] += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
//                 }
//                 if ($uwwtp[$induww['art4_final']] == 'NR') {  
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NR']['pe']      += $uwwtp[$induww['collected in collecting system  p. e.']];  
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['NR']['percent'] += $uwwtp[$induww['collected in collecting system % of gen. load']]; 
//                 }
//                 
//                 //Secondary: installed?
//                 if ($uwwtp[$induww['Secondary: installed?']] == '1') {
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_installed'] = 1;  
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_treatement_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_treatement_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
//                 }   
//                 
//                 if ($uwwtp[$induww['Secondary:  monitoring passed?']] == '1') {
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_passed'] = 1;  
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_monitoring_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['secondary_monitoring_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
//                 }                               
//                 
//                 //tertiary install
//                 if ($uwwtp[$induww['More stringent:  installed?']] == '1') {
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_installed'] = 1;  
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_treatement_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_treatement_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
//                 }   
//                 
//                 if ($uwwtp[$induww['More stringent: monitoring passed?']] == '1') {
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_passed'] = 1;  
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_monitoring_percent'] += $uwwtp[$induww['uww_entering this UWWTP % of gen. load']]; 
//                     $uwwtpinfo_for_agglo[$uww_aggcode]['morestringent_monitoring_pe'] += $uwwtp[$induww['uww_entering load  p. e.']];                      
//                 }
//             }
//         }        
        
        $uwwtpinfo_for_agglo = &$context['sandbox']['uwwtpinfo_for_agglo'];
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
        $line[$ind['pending_or_expired_deadline_exp/pd/exp-pd']] = 'TODO';
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
        $line[$ind['number_of_uwwtps/_collecting_systems_without_treatment_serving_the_agglomeration']] = 'TODO';
        
        //Previous agglomeration info',
        $previousagglonid = uwwtd_check_exist(uwwtd_get_siteid('agglomeration', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'])));
        // $previousuwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $context['sandbox']['previous_year'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
        // $previousagglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $context['sandbox']['previous_year'], 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
        $previous_reported = '';
        $previous_aggcode = '';
        $previous_gen = '';
        $previous_percentage = '';
        
        if ($context['sandbox']['previous_year'] !== false && $previousagglonid === false) {
            $previous_reported = 'no';
        }
        if ($previousagglonid !== false) {
            $previousagg = node_load($previousagglonid);
            $previous_reported = 'yes';
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
        $line[$ind['dist_art3_%_of_gen._load_(3)']] = 'TODO';
        //'in p.e.',
        $line[$ind['dist_art3_in_p.e.']] = 'TODO';                                        
        
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
        $line[$ind['total_entering_load_p._e.']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['entering'];
        //'ratio connected/enentering %',
        $line[$ind['ratio_connected/enentering_%']] = $line[$ind['total_entering_load_p._e.']] * 100 / $line[$ind['connected_to_collecting_system_p._e.']];
        //'Total treatment plant design capacity  p. e.',
        $line[$ind['total_treatment_plant_design_capacity_p._e.']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['capacity'];
        //'ratio Total entering load/Total design capacity %',
        $line[$ind['ratio_total_entering_load/total_design_capacity_%']] = $line[$ind['total_entering_load_p._e.']] * 100 / $line[$ind['total_treatment_plant_design_capacity_p._e.']];
        
        //Compliance Art. 4',
        //in case multiple uwwtp for this agglo, we must calculate total values  

        //'compliant % of gen. load',
        $line[$ind['art4_compliant_%_of_gen._load']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['C']['pe'];
        //'compliant p. e.',
        $line[$ind['art4_compliant_p._e.']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['C']['percent'];
        //'not compliant % of gen. load', 
        $line[$ind['art4_not_compliant_%_of_gen._load']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['NC']['pe'];
        //'not compliant',
        $line[$ind['art4_not_compliant']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['NC']['percent'];
        //'not relevant % of gen. load',
        $line[$ind['art4_not_relevant_%_of_gen._load']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['NR']['pe'];
        //'not relevant',
        $line[$ind['art4_not_relevant']] = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['NR']['percent'];
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
        $line[$ind['dist_art4_treat__%_of_connected_load']] = 'TODO';
        //'in p.e. (4)',
        $line[$ind['dist_art4_treat_in_p.e._(4)']] = 'TODO';
        //Performance',
        //'% of connected Load',
        $line[$ind['dist_art4_perf_%_of_connected_load']] = 'TODO';
        //'in p.e. (5)',
        $line[$ind['dist_art4_perf_in_p.e._(5)']] = 'TODO';
        
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
        $line[$ind['previous_art5_treat_%_of_gen._load']] = 'TODO';
        //'in p.e. (6)', 
        $line[$ind['previous_art5_treat_in_p.e._(6)']] = 'TODO';
        //Performance',
        //'% of gen. Load', 
        $line[$ind['previous_art5_perf_%_of_gen._load']] = 'TODO';
        //'in p.e. (7)', 
        $line[$ind['previous_art5_perf_in_p.e._(7)']] = 'TODO';
        
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
        $line[$ind['entirely_discharging_into_coastal_waters']] = 'TODO';
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
        if ($uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['secondary_installed'] == 1) {  
            $secondary_install_pe = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['secondary_treatement_percent']; 
            $secondary_install_percent = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['secondary_treatement_pe']; 
        }
        $line[$ind['secondary_treatment_installed_(%_of_gen._load)']] = $secondary_install_percent;
        //'secondary treatment installed (p. e.)', 
        $line[$ind['secondary_treatment_installed_(p._e.)']] = $secondary_install_pe;
        
        //'monitoring results f. secondary OK (% of gen. load)',
        $secondary_passed_pe      = '';
        $secondary_passed_percent = '';  
        if ($uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['secondary_passed'] == 1) {  
            $secondary_passed_pe = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['secondary_monitoring_percent']; 
            $secondary_passed_percent = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['secondary_monitoring_pe']; 
        }          
        $line[$ind['monitoring_results_f._secondary_ok_(%_of_gen._load)']] = $secondary_passed_percent;
        //'monitoring results f. secondary OK (p. e.)', 
        $line[$ind['monitoring_results_f._secondary_ok']] = $secondary_passed_pe;
                
        //'more str. treatment installed (%_of_gen._load',
        $morestringent_install_pe      = '';
        $morestringent_install_percent = '';  
        if ($uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['morestringent_installed'] == 1) {  
            $morestringent_install_pe = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['morestringent_treatement_percent']; 
            $morestringent_install_percent = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['morestringent_treatement_pe']; 
        }   
                  
        $line[$ind['more_str._treatment_installed_(%_of_gen._load)']] = $morestringent_install_percent;
        //'more str. treatment installed (p. e.)', 
        $line[$ind['more_str._treatment_installed_(p._e.)']] = $morestringent_install_pe;
        
        //'monitoring results f. more string. OK (% of gen. load)',
        $morestringent_passed_pe      = '';
        $morestringent_passed_percent = '';  
        if ($uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['morestringent_passed'] == 1) {  
            $secondary_passed_pe = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['morestringent_monitoring_percent']; 
            $morestringent_passed_percent = $uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]['morestringent_monitoring_pe']; 
        }            
        $line[$ind['monitoring_results_f._more_string._ok_(%_of_gen._load)']] = $morestringent_passed_percent;
        //'monitoring results f. more string. OK (p. e.)', 
        $line[$ind['monitoring_results_f._more_string._ok_(p._e.)']] =$morestringent_passed_pe;
        
        
        if (isset($uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']])) {
            unset($uwwtpinfo_for_agglo[$agg->field_inspireidlocalid['und'][0]['value']]);
        } 
        unset($agg);           
        fputcsv($r, $line, ';');

        $j++;
        $context['sandbox']['progress']++;
    }    
    fclose($r);
    
    $context['message'] = t('Processing agglo sheet 3 : ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max']);   
    
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
	if (!isset($context['sandbox']['progress'])) {
    
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['id'] = array();
            
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

    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'] .'.csv';      
    
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
            
            
            $ind['Number of discharge points for this UWWTP'] => 'Number of discharge points for this UWWTP',
            $ind['DCP ID'] => 'DCP ID',
            //current RCA info (Q 2013)
            $ind['ID of receiving water body 2'] => 'ID of receiving water body',
            $ind['water body type 2'] => 'water body type',
            $ind['ID of the SA or LSA 2'] => 'ID',
            $ind['rcaname 2'] => 'name',
            $ind['designation date 2'] => 'designation date',          
            
            
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
        );
        
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');  
    }
//     dsm($header);
    

    $j = 0;
    while ($j < 10 && $context['sandbox']['progress'] < $context['sandbox']['max']) {
        
//         $uwwtp = node_load($niduwwtp);
        $uwwtp = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
//         dsm($uwwtp->field_uwwaggliste['und']);
        //a uwwtp can be attach to multiple agglo
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
            $agglo_uwwtp = node_load(uwwtd_check_exist($siteid));
//             dsm($agglo_uwwtp);
            //'entering this UWWTP % of gen. load',
            $line[$ind['uww_entering this UWWTP % of gen. load']] = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];              
            //'entering load  p. e.',
            $line[$ind['uww_entering load  p. e.']] = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
            unset($agglo_uwwtp, $siteid);            
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
            if ($previousuwwtpnid !== false && $previousagglonid !== false && $previousagglo_uwwtpnid !== false) {
                $previousuwwtp = node_load($previousuwwtpnid);
                $previousagglo_uwwtp = node_load($previousagglo_uwwtpnid);
                $previousagglo = node_load($previousagglonid);
//                 dsm($previousuwwtp);
//                 dsm($previousagglo_uwwtp);
//                 dsm($previousagglo);
                $previous_reference_year = $context['sandbox']['previous_year']; 
                $previous_entering_load_pe = $previousagglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $previousagglo->field_agggenerated['und'][0]['value'];
                $previous_capacity_pe = $previousuwwtp->field_physicalcapacityactivity['und'][0]['value'];
                if (isset($line[$ind['uww_entering load  p. e.']])) {
                    $previous_percentage_of_change_of_entering_load = $previous_entering_load_pe * 100 / $line[$ind['uww_entering load  p. e.']];
                }
                if (isset($line[$ind['uww_capacity p. e.']])) {
                    $previous_percentage_of_change_of_capacity = $previous_capacity_pe * 100 / $line[$ind['uww_capacity p. e.']];
                }
//                 unset($previousuwwtp);
                unset($previousagglo_uwwtp);
                unset($previousagglo);
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
            $line[$ind['entirely discharging into coastal waters?']] = 'TODO';
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
            $line[$ind['Starting date of application a N']] = 'TODO';
            //'Starting date of application a P',
            $line[$ind['Starting date of application a P']] = 'TODO';
            //'Starting date of application b',     
            $line[$ind['Starting date of application b']] = 'TODO';
            //'Starting date of application c',
            $line[$ind['Starting date of application c']] = 'TODO';
            
            //UWWTP treatment and performance
            //'treatment required',
            $line[$ind['treatment required']] = $uwwtp->field_uwwtreatmentrequired['und'][0]['value'];
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
            $treatmenttypeinplace = '';                 
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
            $line[$ind['difference between monitoring result N and P and treatment in place']] = 'TODO';
            
            
            $line[$ind['Number of discharge points for this UWWTP']] = 'TODO';
            $line[$ind['DCP ID']] = 'TODO';
            //current RCA info (Q 2013)
            $line[$ind['ID of receiving water body 2']] = 'TODO';
            $line[$ind['water body type 2']] = 'TODO';
            $line[$ind['ID of the SA or LSA 2']] = 'TODO';
            $line[$ind['rcaname 2']] = 'TODO';
            $line[$ind['designation date 2']] = 'TODO';       
            
            
            //Compliance of treatment plant under expired and pending deadline
            
            //calculate compliance of art 4 and 5 for this station and this agglo
             $uwwtp_compliance = uwwtd_article4_5_6_station_compliance($agg, $uwwtp);
                 
            //compliance Art. 4    //secondary treatment in place
            //'treatment type', //Treatment met
            if ($uwwtp->field_uwwtreatment_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['treatementmet']['false']) {
                $line[$ind['art4_treatment type']] = 'TODO';
            } elseif ($uwwtp->field_uwwtreatment_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['treatementmet']['true']) {
                $line[$ind['art4_treatment type']] = 'TODO';
            } elseif ($uwwtp->field_uwwtreatment_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['treatementmet']['not_calculable']) {
                $line[$ind['art4_treatment type']] = 'TODO';
            } else {
                $line[$ind['art4_treatment type']] = 'TODO';
            }
            //'monitoring',     //Performance met
            if ($uwwtp->field_uwwperformance_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['performancemet']['false']) {
                $line[$ind['art4_monitoring']] = 'TODO';
            } elseif ($uwwtp->field_uwwperformance_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['performancemet']['true']) {
                $line[$ind['art4_monitoring']] = 'TODO';
            } elseif ($uwwtp->field_uwwperformance_met['und'][0]['value'] == $GLOBALS['uwwtd_value']['performancemet']['not_calculable']) {
                $line[$ind['art4_monitoring']] = 'TODO';
            } else {
                $line[$ind['art4_monitoring']] = 'TODO';
            }            
            //'final',
            $line[$ind['art4_final']] = $uwwtp_compliance['compArt4'];
             
            //compliance Art. 5   //more stringent treatment in place
            //'treatment type',
            $line[$ind['art5_treatment type']] = 'TODO';
            //'monitoring',
            $line[$ind['art5_monitoring']] = 'TODO';
            //'final',
            $line[$ind['art5_final']] = $uwwtp_compliance['compArt5'];
             
            //Compliance of treatment plant under expired deadlines
            $art4_expired_treatment = '';
            $art4_expired_monitoring = '';
            $art4_expired_final = '';
            $art5_expired_treatment = '';
            $art5_expired_monitoring = '';
            $art5_expired_final = '';
            if ($uwwtp->field_uwwcompliance['und'][0]['value'] != 'PD') {
                $art4_expired_treatment = $line[$ind['art4_treatment type']];
                $art4_expired_monitoring = $line[$ind['art4_monitoring']];
                $art4_expired_final = $line[$ind['art4_final']];
                $art5_expired_treatment = $line[$ind['art5_treatment type']];
                $art5_expired_monitoring = $line[$ind['art5_monitoring']];
                $art5_expired_final = $line[$ind['art5_final']];
            }     
            //compliance Art. 4 
            //'treatment type',
            $line[$ind['art4_expired_treatment type']] = $art4_expired_treatment;
            //'monitoring',
            $line[$ind['art4_expired_monitoring']] = $art4_expired_monitoring;
            //'final',
            $line[$ind['art4_expired_final']] = $art4_expired_final;              
            //compliance Art. 5 
            //'treatment type',
            $line[$ind['art5_expired_treatment type']] = $art5_expired_treatment;
            //'monitoring',
            $line[$ind['art5_expired_monitoring']] = $art5_expired_monitoring;
            //'final',
            $line[$ind['art5_expired_final']] = $art5_expired_final;
            
            unset($uwwtp_compliance);
             
            //Previous compliance of the treatment under expired deadlines
            $previous_art4_expired_treatment = 'TODO';
            $previous_art4_expired_monitoring = 'TODO';
            $previous_art4_expired_final = 'TODO';
            $previous_art5_expired_treatment = 'TODO';
            $previous_art5_expired_monitoring = 'TODO';
            $previous_art5_expired_final = 'TODO';
            $previous_art5_expired_difference = 'TODO';
            if (false !== $previousagglo_uwwtpnid && isset($previousuwwtp)) {
                $previous_art4_expired_treatment = 'TODO';
                $previous_art4_expired_monitoring = 'TODO';
                $previous_art4_expired_final = 'TODO';
                $previous_art5_expired_treatment = 'TODO';
                $previous_art5_expired_monitoring = 'TODO';
                $previous_art5_expired_final = 'TODO';  
                $previous_art5_expired_difference = 'TODO';          
            }
            //'treatment type', 
            $line[$ind['previous_art4_expired_treatment type']] = $previous_art4_expired_treatment; 
            //'monitoring',
            $line[$ind['previous_art4_expired_monitoring']] = $previous_art4_expired_monitoring;
            //'final', 
            $line[$ind['previous_art4_expired_final']] = $previous_art4_expired_final;
            //'treatment type', 
            $line[$ind['previous_art5_expired_treatment type']] = $previous_art5_expired_treatment;
            //'monitoring', 
            $line[$ind['previous_art5_expired_monitoring']] = $previous_art5_expired_monitoring;
            //'final', 
            $line[$ind['previous_art5_expired_final']] = $previous_art5_expired_final; 
            //'difference between the previous situation and now',
            $line[$ind['previous_art5_expired_difference between the previous situation and now']] = $previous_art5_expired_difference;
            
            unset($previousuwwtp);
             
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
            $secondaryfinal = ($secondaryinstalled == '1' && $secondarypass = '1' ? '1': '0');
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
            /*            
            Part 1
            =IF(‘generated load’ <2000;
                "NR";
                IFERROR(IF(AND(
                                FIND("3NP";’treatment type’)>0;
                                OR(AND(‘Monitoring results –Ntot’="pass";
                                       ’Monitoring results –Ptot’="pass");
                                   AND(‘Monitoring results –Ntot’="pass";
                                       ’Monitoring results –Ptot’="not relevant";
                                       ’SECONDARY:  MONITORING PASSED’=1);
                                   AND(‘Monitoring results –Ntot’="not relevant";
                                       ’Monitoring results –Ptot’="not relevant";
                                       ’SECONDARY:  MONITORING PASSED’=1);
                                   AND(‘Monitoring results –Ntot’="not relevant";
                                       ’Monitoring results –Ptot’="pass";
                                       ’SECONDARY:  MONITORING PASSED’=1)
                                  )
                                ) ;
                            1;
                            0);
                        IFERROR(IF(AND(
                                        FIND("3N";’treatment type’)>0;
                                        OR(‘Monitoring results –Ntot’="pass";
                                           AND(‘Monitoring results –Ntot’="not relevant";
                                               ’SECONDARY:  MONITORING PASSED’=1)
                                           )
                                      ) ;
                                    1;
                                    0); 
                                  IFERROR(IF(AND(
                                                 FIND("3P";’treatment type’)>0;
                                                 OR(‘Monitoring results –Ptot’="pass";
                                                    AND(‘Monitoring results –Ptot’="not relevant";
                                                        ’SECONDARY:  MONITORING PASSED’=1)
                                                    )
                                                 );
                                            1;
                                            0);
                                        "EXIT"))))
            Part 2
            =IF(NOT(‘Part 1’="EXIT");’Part 1’;
                IFERROR(IF(AND(FIND("3other";’treatment type’)>0;
                               ’SECONDARY:  MONITORING PASSED’=1);
                           1;
                           0);
                        IFERROR(IF(AND(FIND("2";’treatment type’)>0;
                                       OR(‘Monitoring results –Ntot’="pass";
                                           ’Monitoring results –Ptot’="pass");
                                           ’SECONDARY:  MONITORING PASSED’=1
                                       );
                                    1;
                                    0);
                                  IF(OR(‘treatment type’=0;
                                        ’treatment type’=1
                                        );
                                    0;
                                    0)
                                  )
                          )
                )
            */    
            
            $morestringentpassed = '0';
            $line[$ind['More stringent: monitoring passed?']] = 'TODO'; 
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
            //'Discharged loads BOD-tot  t/y',    field_uwwboddischarge 
            $line[$ind['Discharged loads BOD-tot  t/y']] = $uwwtp->field_uwwboddischarge['und'][0]['value'];
            //'BOD rate of removal %',      round(((1- ($uwwtp->field_uwwboddischarge['und'][0]['value'] / $uwwtp->field_uwwbodincoming['und'][0]['value']))*100), 2)
            $line[$ind['BOD rate of removal %']] = (1- ($uwwtp->field_uwwboddischarge['und'][0]['value'] / $uwwtp->field_uwwbodincoming['und'][0]['value']))*100;
            //'average Incoming concentration BOD mg/l', ($uwwtp->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;  
            $line[$ind['average Incoming concentration BOD mg/l']] = ($uwwtp->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            //'average Discharge concentration BOD mg/l', ($uwwtp->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Discharge concentration BOD mg/l']] = ($uwwtp->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            //'Incoming cocentration COD-tot   t/y',   field_uwwcodincoming
            $line[$ind['Incoming cocentration COD-tot   t/y']] = $uwwtp->field_uwwcodincoming['und'][0]['value'];
            //'Discharged concentration COD-tot  t/y',    field_uwwcoddischarge
            $line[$ind['Discharged concentration COD-tot  t/y']] = $uwwtp->field_uwwcoddischarge['und'][0]['value'];
            //'COD rate of removal %',   round(( (1-($uwwtp->field_uwwcoddischarge['und'][0]['value'] / $uwwtp->field_uwwcodincoming['und'][0]['value'])) *100), 2) . '%'
            $line[$ind['COD rate of removal %']] =  ( (1-($uwwtp->field_uwwcoddischarge['und'][0]['value'] / $uwwtp->field_uwwcodincoming['und'][0]['value'])) *100);
            //'average Incoming concentration COD mg/l',  ($uwwtp->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            $line[$ind['average Incoming concentration COD mg/l']] = ($uwwtp->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            //'average Discharge concentration COD mg/l',  ($uwwtp->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Discharge concentration COD mg/l']] = ($uwwtp->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            //'Incoming loads N-tot   t/y',   field_uwwnincoming
            $line[$ind['Incoming loads N-tot   t/y']] = $uwwtp->field_uwwnincoming['und'][0]['value'];
            //'Discharged loads N-tot  t/y',   field_uwwndischarge
            $line[$ind['Discharged loads N-tot  t/y']] = $uwwtp->field_uwwndischarge['und'][0]['value'];
            //'N rate of removal %',   round(( (1- ($uwwtp->field_uwwndischarge['und'][0]['value'] / $uwwtp->field_uwwnincoming['und'][0]['value'])) *100), 2) . '%'
            $line[$ind['N rate of removal %']] = ( (1- ($uwwtp->field_uwwndischarge['und'][0]['value'] / $uwwtp->field_uwwnincoming['und'][0]['value'])) *100); 
            //'average Incoming concentration N mg/l',         ($uwwtp->field_uwwnincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Incoming concentration N mg/l']] = ($uwwtp->field_uwwnincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            //'average Discharge concentration N mg/l',    ($uwwtp->field_uwwndischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Discharge concentration N mg/l']] = ($uwwtp->field_uwwndischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            //'Incoming cocentration P-tot   t/y',    field_uwwpincoming
            $line[$ind['Incoming cocentration P-tot   t/y']] = $uwwtp->field_uwwpincoming['und'][0]['value'];
            //'Discharged concentration P-tot  t/y',    field_uwwpdischarge
            $line[$ind['Discharged concentration P-tot  t/y']] = $uwwtp->field_uwwpdischarge['und'][0]['value'];
            //'P rate of removal %',   round(( (1-($uwwtp->field_uwwpdischarge['und'][0]['value'] / $uwwtp->field_uwwpincoming['und'][0]['value'])) *100), 2) . '%'
            $line[$ind['P rate of removal %']] = ( (1-($uwwtp->field_uwwpdischarge['und'][0]['value'] / $uwwtp->field_uwwpincoming['und'][0]['value'])) *100); 
            //'average Incoming concentration P mg/l',  ($uwwtp->field_uwwpincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            $line[$ind['average Incoming concentration P mg/l']] = ($uwwtp->field_uwwpincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            //'average Discharge concentration P mg/l', ($uwwtp->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
            $line[$ind['average Discharge concentration P mg/l']] = ($uwwtp->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000; 
        }
        fputcsv($r, $line, ';'); 
        
//         dsm($line);
//         return;
//     $context['sandbox']['progress'] = $context['sandbox']['max'];
//         break;        
//         $allline[] = $line;
        $j++;
        $context['sandbox']['progress']++;   
    }    
    fclose($r);
    
    $context['message'] = t('Processing uwwtp sheet 2 : ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max']);    
    
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
  
    $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
    $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['rca']['csv_name']. '.csv';
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
    
    //DEBUG
    $line = array(
        //'ID of Sensitive Area', //;       rcaCode 
        $ind['id_of_sensitive_area'] =>'field_inspireidlocalid_value',
        //'Name of Sensitive Area', //;     rcaName
        $ind['name_of_sensitive_area'] => 'title',
        //'Zone Type Code', //;             rcaType
        $ind['zone_type_code'] => 'field_specialisedzonetype_value',
        //'Specialised Zone type Code', //;   ?rcaType
        $ind['specialised_zone_type_code'] => 'TODO', 
        //'Parameters subject to More Stringent Treatment: N', //;      rcaParameterN
        $ind['parameters_subject_to_more_stringent_treatment:_n'] => 'field_rca_parameter_n_value',
        //'Parameters subject to More Stringent Treatment: P', //;      rcaParameterP
        $ind['parameters_subject_to_more_stringent_treatment:_p'] =>'field_rca_parameter_p_value',
       // 'Parameters subject to More Stringent Treatment: m', //;      ?
        $ind['parameters_subject_to_more_stringent_treatment:_m'] => 'TODO',
        //'Parameters subject to More Stringent Treatment: other', //;   rcaParameterOther
        $ind['parameters_subject_to_more_stringent_treatment:_other'] => 'field_rca_parameter_other_value',
        //'Date of designation of Art. 5(8)', //;                  ?
        $ind['date_of_designation_of_art._5(8)'] => 'TODO',
        //'Starting date of application of Art. 5(8)', //;         rcaDateArt58
        $ind['starting_date_of_application_of_art._5(8)'] => 'field_rcadateart5854_value',
        //'Starting date of application of Art. 5(4)', //;         rcaDateArt54
        $ind['starting_date_of_application_of_art._5(4)'] => 'field_rcadateart54_value',
        //'Designation criteria  a N', //;                         rcaANitro
        $ind['designation_criteria_a_n'] => 'field_rcaanitro_value',
        //'Starting date of designation a N', //;                  ?
        $ind['starting_date_of_designation_a_n'] => 'TODO',
        //'Starting date of application a N', //;                  ?
        $ind['starting_date_of_application_a_n'] => 'TODO',
        //'Designation criteria  a P', //;                         rcaAPhos
        $ind['designation_criteria_a_p'] => 'field_rcaaphos_value',
        //'Starting date of designation a P', //;                  ?
        $ind['starting_date_of_designation_a_p'] => 'TODO',
        //'Starting date of application a P', //;                  ?
        $ind['starting_date_of_application_a_p'] => 'TODO',
        //'Designation criteria  b', //;                           rcaB
        $ind['designation_criteria_b'] => 'field_rcab_value',
        //'Starting date of designation', //;                      ?
        $ind['starting_date_of_designation_b'] => 'TODO',
        //'Starting date of application', //;                      ?
        $ind['starting_date_of_application_b'] => 'TODO',
        //'Designation criteria  c', //;                           rcaC
        $ind['designation_criteria_c'] => 'field_rcac_value',
        //'Starting date of designation', //;                      ?
        $ind['starting_date_of_designation_c'] => 'TODO',
        //'Starting date of application', //;                      ?
        $ind['starting_date_of_application_c'] => 'TODO',
        //'Designation criteria c - relevant EU-Directives and related parameters', //;            ?
        $ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'] => 'TODO',
        //'Designation criteria c - ID applied to the area according to this Directive', //;       ?
        $ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'] => 'TODO',
        //'Designation criteria c - reference date of the area according to this Directive', //;   ?
        $ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'] => 'TODO',
        //'Number of UWWTPs of the article 5.4 area (1)', //;                                      ?rcaPlants
        $ind['number_of_uwwtps_of_the_article_5.4_area_(1)'] => 'field_rcaplants_value',            
        //'Total organic design capacity of the UWWTPs (p.e.) of the article 5.4 area (1)', //;    ?rcaPlantsCapacity
        $ind['total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)'] => 'field_rcaplantscapacity_value',
        
        //'Incoming loads (t/y) N-tot (1)', //;    rcaNIncomingMeasured + rcaNIncomingCalculated + rcaNIncomingEstimated
        $ind['incoming_loads_(t/y)_n-tot_(1)'] => ('field_rcanincomingmeasured_value = field_rcanincomingcalculated_value = field_rcanincomingestimated_value'),
        //'Rate of Removal N-tot', //;              ?
        $ind['rate_of_removal_n-tot'] => 'TODO',
        //'Discharged loads (t/y) N-tot (1)', //; rcaNDischargedMeasured + rcaNDischargedCalculated + rcaNDischargedEstimated    
        $ind['discharged_loads_(t/y)_n-tot_(1)'] =>('field_rcandischargedmeasured_value=field_rcandischargedcalculated_value=field_rcandischargedestimated_value'),
        
        //'Incoming loads (t/y) P-tot (1)', //;  rcaPIncomingMeasured + rcaPIncomingCalculated + rcaPIncomingEstimated    
        $ind['incoming_loads_(t/y)_p-tot_(1)'] => ('field_rcapincomingmeasured_value=field_rcapincomingcalculated_value=field_rcapincomingestimated_value'),
        //'Rate of Removal P-tot', //;              ?
        $ind['rate_of_removal_p-tot'] => 'TODO',
        //'Discharged loads (t/y) P-tot: ', //; rcaPDischargedMeasured+ rcaPDischargedCalculated + rcaPDischargedEstimated
        $ind['discharged_loads_(t/y)_p-tot:_'] => ('field_rcapdischargedmeasured_value=field_rcapdischargedcalculated_value=field_rcapdischargedestimated_value'),
        
        //'general Remark', //;                   rcaRemarks
        $ind['general_remark'] => 'field_rcaremarks_value',
        
        //'rcaBeginLife', //;                        ?
        $ind['rcabeginlife'] => 'TODO',
        //'rcaEndLife', //;                          ?
        $ind['rcaendlife'] => 'TODO',
        //'ID code of successor (2)', //;            ?
        $ind['id_code_of_successor_(2)'] => 'TODO',
        //'ID code of predecessor (2)', //;          ?
        $ind['id_code_of_predecessor_(2)'] => 'TODO',
        //'remark Successor/Predecessor SA', //;     ?
        $ind['remark_successor/predecessor_sa'] => 'TODO',
        //'Related sensitive area (2)', //;          ?
        $ind['related_sensitive_area_(2)'] => 'TODO',
        //'Remark Related sensitive area (2)', //;   ?
        $ind['remark_related_sensitive_area_(2)'] => 'TODO',        
    );
    fputcsv($r, $line, ';');
    //DEBUG
        
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
            //'Starting date of application of Art. 5(8)', //;         rcaDateArt58
            $ind['starting_date_of_application_of_art._5(8)'] => $rca[$i]['field_rcadateart5854_value'],
            //'Starting date of application of Art. 5(4)', //;         rcaDateArt54
            $ind['starting_date_of_application_of_art._5(4)'] => $rca[$i]['field_rcadateart54_value'],
            //'Designation criteria  a N', //;                         rcaANitro
            $ind['designation_criteria_a_n'] => $rca[$i]['field_rcaanitro_value'],
            //'Starting date of designation a N', //;                  ?
            $ind['starting_date_of_designation_a_n'] => 'TODO',
            //'Starting date of application a N', //;                  ?
            $ind['starting_date_of_application_a_n'] => 'TODO',
            //'Designation criteria  a P', //;                         rcaAPhos
            $ind['designation_criteria_a_p'] => $rca[$i]['field_rcaaphos_value'],
            //'Starting date of designation a P', //;                  ?
            $ind['starting_date_of_designation_a_p'] => 'TODO',
            //'Starting date of application a P', //;                  ?
            $ind['starting_date_of_application_a_p'] => 'TODO',
            //'Designation criteria  b', //;                           rcaB
            $ind['designation_criteria_b'] => $rca[$i]['field_rcab_value'],
            //'Starting date of designation', //;                      ?
            $ind['starting_date_of_designation_b'] => 'TODO',
            //'Starting date of application', //;                      ?
            $ind['starting_date_of_application_b'] => 'TODO',
            //'Designation criteria  c', //;                           rcaC
            $ind['designation_criteria_c'] => $rca[$i]['field_rcac_value'],
            //'Starting date of designation', //;                      ?
            $ind['starting_date_of_designation_c'] => 'TODO',
            //'Starting date of application', //;                      ?
            $ind['starting_date_of_application_c'] => 'TODO',
            //'Designation criteria c - relevant EU-Directives and related parameters', //;            ?
            $ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'] => 'TODO',
            //'Designation criteria c - ID applied to the area according to this Directive', //;       ?
            $ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'] => 'TODO',
            //'Designation criteria c - reference date of the area according to this Directive', //;   ?
            $ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'] => 'TODO',
            //'Number of UWWTPs of the article 5.4 area (1)', //;                                      ?rcaPlants
            $ind['number_of_uwwtps_of_the_article_5.4_area_(1)'] => $rca[$i]['field_rcaplants_value'],            
            //'Total organic design capacity of the UWWTPs (p.e.) of the article 5.4 area (1)', //;    ?rcaPlantsCapacity
            $ind['total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)'] => $rca[$i]['field_rcaplantscapacity_value'],
            
            //'Incoming loads (t/y) N-tot (1)', //;    rcaNIncomingMeasured + rcaNIncomingCalculated + rcaNIncomingEstimated
            $ind['incoming_loads_(t/y)_n-tot_(1)'] => ($rca[$i]['field_rcanincomingmeasured_value'] + $rca[$i]['field_rcanincomingcalculated_value'] + $rca[$i]['field_rcanincomingestimated_value']),
            //'Rate of Removal N-tot', //;              ?
            $ind['rate_of_removal_n-tot'] => 'TODO',
            //'Discharged loads (t/y) N-tot (1)', //; rcaNDischargedMeasured + rcaNDischargedCalculated + rcaNDischargedEstimated    
            $ind['discharged_loads_(t/y)_n-tot_(1)'] =>($rca[$i]['field_rcandischargedmeasured_value'] + $rca[$i]['field_rcandischargedcalculated_value'] + $rca[$i]['field_rcandischargedestimated_value']),
            
            //'Incoming loads (t/y) P-tot (1)', //;  rcaPIncomingMeasured + rcaPIncomingCalculated + rcaPIncomingEstimated    
            $ind['incoming_loads_(t/y)_p-tot_(1)'] => ($rca[$i]['field_rcapincomingmeasured_value'] + $rca[$i]['field_rcapincomingcalculated_value'] + $rca[$i]['field_rcapincomingestimated_value']),
            //'Rate of Removal P-tot', //;              ?
            $ind['rate_of_removal_p-tot'] => 'TODO',
            //'Discharged loads (t/y) P-tot: ', //; rcaPDischargedMeasured+ rcaPDischargedCalculated + rcaPDischargedEstimated
            $ind['discharged_loads_(t/y)_p-tot:_'] => ($rca[$i]['field_rcapdischargedmeasured_value'] + $rca[$i]['field_rcapdischargedcalculated_value'] +$rca[$i]['field_rcapdischargedestimated_value']),
            
            //'general Remark', //;                   rcaRemarks
            $ind['general_remark'] => $rca[$i]['field_rcaremarks_value'],
            
            //'rcaBeginLife', //;                        ?
            $ind['rcabeginlife'] => 'TODO',
            //'rcaEndLife', //;                          ?
            $ind['rcaendlife'] => 'TODO',
            //'ID code of successor (2)', //;            ?
            $ind['id_code_of_successor_(2)'] => 'TODO',
            //'ID code of predecessor (2)', //;          ?
            $ind['id_code_of_predecessor_(2)'] => 'TODO',
            //'remark Successor/Predecessor SA', //;     ?
            $ind['remark_successor/predecessor_sa'] => 'TODO',
            //'Related sensitive area (2)', //;          ?
            $ind['related_sensitive_area_(2)'] => 'TODO',
            //'Remark Related sensitive area (2)', //;   ?
            $ind['remark_related_sensitive_area_(2)'] => 'TODO',        
        );
        fputcsv($r, $line, ';');
//         $allline[] = $line;    
    }    
    fclose($r);

    $context['sandbox']['progress']++;
    $context['results'][] = 'sheet 1 rca create';    
    $context['message'] = t('Processing sheet 1 rca : ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max']);
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
        $sheet_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_name'] .'.xlsx';
        if (!file_exists($sheet_path)) {
            continue;
        }
//         dsm($sheet_path);
    //     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp);
        PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_sqlite);
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
    $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['final_register'];
//     dsm($target_path);    
    $objWriter->save(drupal_realpath($target_path));
    unset($finalPHPExcel);
    unset($objWriter);  
    $context['sandbox']['progress']++;
 
    $context['message'] = t('file_summary_legal_compliance_sheet4 finish');    
    
//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }  


    variable_set('registre_'.$annee, time());
    drupal_set_message('<a href="'.file_create_url($GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['final_register']).'">get the register for year '.$annee.'</a>');          
}
