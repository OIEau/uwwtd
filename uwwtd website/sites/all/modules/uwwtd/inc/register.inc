<?php

require_once(drupal_get_path('module', 'uwwtd') . '/inc/register.config.inc');
require_once(drupal_get_path('module', 'uwwtd') . '/inc/utils.inc');
require_once(drupal_get_path('module', 'uwwtd') . '/inc/data_center.inc');
require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

// dsm( DRUPAL_ROOT . '/vendor/autoload.php');

// require DRUPAL_ROOT . '/vendor/autoload.php';
// 
// use PhpOffice\PhpSpreadsheet\Spreadsheet;
// use PhpOffice\PhpSpreadsheet\IOFactory;
// use PhpOffice\PhpSpreadsheet\Writer\Xlsx;




$cacheMethod = PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp;
$cacheSettings = array('memoryCacheSize' => '4MB', 'cacheTime' => '1000'); 
PHPExcel_Settings::setCacheStorageMethod($cacheMethod, $cacheSettings);

/**
 *
 */
function uwwtd_register_form($form, &$form_state){

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

function uwwtd_register_set_operation(&$batch, $annee){
    //CSV
    $batch['operations'][] = array('uwwtd_create_register_2015_rca_sheet1', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_rca54_sheet1_1', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_uwwtp_sheet2', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_agglo_without_uwwtp_sheet2', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_agglo_sheet3', array($annee));

    if ($annee < 2016) {
      $batch['operations'][] = array('uwwtd_create_register_2015_summary_big_cities_sheet8', array($annee));
    } else {
      $batch['operations'][] = array('uwwtd_create_register_2015_summary_big_cities_10th_report_step_bigdischargers_sheet8', array($annee));
      $batch['operations'][] = array('uwwtd_create_register_2015_summary_big_cities_10th_report_step_bigcity_sheet8', array($annee));
    }
    $batch['operations'][] = array('uwwtd_create_register_2015_nuts_level2_analyse_sheet9', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_new_agglomerations_sheet12', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_agglomerationout_sheet13', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_new_treatment_plants_sheet14', array($annee));
    $batch['operations'][] = array('uwwtd_create_register_2015_uwwtpsout_sheet15', array($annee));

 //    //excel 
    $batch['operations'][]=array('uwwtd_create_register_2015_file_glossary_sheet0', array($annee));   
	$batch['operations'][]=array('uwwtd_create_register_2015_file_rca_sheet1', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_rca54_sheet1_1', array($annee));
    if ($annee >= 2018) {$batch['operations'][]=array('uwwtd_create_register_2020_sa_history', array($annee));}
 //    //optimize : ok
	$batch['operations'][]=array('uwwtd_create_register_2015_file_uwwtp_sheet2', array($annee));
	// //optimize : ok
    $batch['operations'][]=array('uwwtd_create_register_2015_file_agglo_sheet3', array($annee));
	//===> nd@oieau.fr : this part need optimisation on memory usage => big dataset imply too large comsuption of RAM with phpExcel lib.
    $batch['operations'][]=array('uwwtd_create_register_2015_file_summary_legal_compliance_sheet4', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_distance_to_compliance_sheet5', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_summary_installation_in_place_sheet6', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_breach_list_sheet7', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_summary_big_cities_sheet8', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_nuts_level2_analyse_sheet9', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_sewage_sludge_and_re_use_sheet10', array($annee));
    if ($annee >= 2020) {
        dsm("uwwtd_create_register_2020_file_graph_sheet11 need new column mapping on Graph Sheet (from line 53 to 72)");
        //$batch['operations'][]=array('uwwtd_create_register_2020_file_graph_sheet11', array($annee));
    }
    elseif ($annee == 2018) {
        $batch['operations'][]=array('uwwtd_create_register_2020_file_graph_sheet11', array($annee));
    }
    else{
        $batch['operations'][]=array('uwwtd_create_register_2015_file_graph_sheet11', array($annee));
// //     $batch['operations'][]=array('uwwtd_create_register_2015_file_graph_test_sheet11', array($annee));   

        $batch['operations'][]=array('uwwtd_create_register_2015_file_graph2_sheet11_1', array($annee));
        
    }
    $batch['operations'][]=array('uwwtd_create_register_2015_file_new_agglomerations_sheet12', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_agglomerationout_sheet13', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_new_treatment_plants_sheet14', array($annee));
    $batch['operations'][]=array('uwwtd_create_register_2015_file_uwwtpsout_sheet15', array($annee));
    //FIX ME for 8th report, this function crash the batch operation
    if ($annee >= 2016) { 
        $batch['operations'][]=array('uwwtd_create_register_2015_file_agglo_compliance_sheet16', array($annee));
    }
    $batch['operations'][]=array('uwwtd_create_register_2015_file_agglo_debug', array($annee));


// 
    $batch['operations'][]=array('uwwtd_create_register_2015_final_file', array($annee));

    $batch['operations'][]=array('uwwtd_create_report', array($annee));
    //=============== 
    
    //     $batch['operations'][]=array('uwwtd_create_register_2015_final_file_testspreadsheet', array($annee));
    $batch['operations'][]=array('uwwtd_retreive_agg_dtt', array($annee));
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
    //$debug_times=array();
	// uwwtd_trace(__FUNCTION__);
    $indConf = $GLOBALS['uwwtd']['sheet3_agglo'];
	//Keep compatibility
	$ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
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
        $context['sandbox']['line'] = 5; //start line   //5 -> 6

        $path_xlsx = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);
        //Remove the old file, if needed
        drupal_unlink($target_path);
    } else {
        $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);
        $target_path = $path_xlsx;
    }

    fseek($ragglofile, $context['sandbox']['file_position']);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);

    $objReader->setLoadSheetsOnly( array(
		$GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name'],
    ));

    $objPHPExcel = $objReader->load(drupal_realpath($path_xlsx));
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name']);

    $dataArray = $sheet->rangeToArray(
        'A7:CV41',   //A6:CV40        A7:CV41
        NULL,
        FALSE,
        TRUE,
        FALSE
    );

	// uwwtd_trace('init xlsx sheet');
//     $start_line = 4;

	
	$j = 0;
    $lines = array();
	//Warning if the csv file contain more than 3000 lines => the system can probably never re-open the excel file
    while ($j < 10000 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
        $line = array();
        
        foreach($indConf as $k=>$conf){
            if($conf['xls']){
                if(isset($agglo[$conf['k']])){
                    $val = $agglo[$conf['k']];
                    //If we have a format wrapper
                    if(isset($conf['wrapper'])){
                        switch($conf['wrapper']){
                            case 'round':
                                $val = round($val);
                              break;
                            case 'round1':
                                $val = round($val,1);
                              break;
                            
                        }
                    }
                }else{
                    $val = null;
                }
                //Other condition on field ?
                $line[$conf['k']] = $val;
                $line[$conf['k']] = uwwtd_register_get_glossary_label($conf, $line[$conf['k']]);
            }

            
        }
        $lines[$j] = $line;
//         uwwtd_trace(var_export($line, true));
        $j++;
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        //$sheet->insertNewRowBefore($context['sandbox']['line']+$j, 1);
    }
//     $sheet->insertNewRowBefore($context['sandbox']['line'], $j+1);
    $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, $j+1);
    $sheet->fromArray($lines, null, 'A'.$context['sandbox']['line'], true);
	//Special rules for cell's style
	
    if(!empty($lines)){
        foreach($lines as $lid=>$line){
            $idx = $indConf['previous_percentage_of_change_of_size_(1)']['k'];
            if(isset($line[$idx]) && abs((float)$line[$idx])>20){
                $sheet->getStyleByColumnAndRow($idx, $context['sandbox']['line']+$lid)
                        ->getFill()
                        ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)
                        ->getStartColor()->setARGB('FFFFC000');
            }
            $idx = $indConf['total_%_(2)']['k'];
            if (isset($line[$idx]) && $line[$idx] < 100) {
                $sheet->getStyleByColumnAndRow($idx, $context['sandbox']['line']+$lid)
                    ->getFill()
                    ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)      // 255 192 0 FFC000
                    ->getStartColor()->setARGB('FFFFC000');
            }
        }
    }   
    
    $context['sandbox']['line']=$context['sandbox']['line']+$j;
	
    fclose($ragglofile);
    unset($ragglofile);
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_total_name'], $annee);
        $r = fopen($filename, 'rb');
        $line_to_fill = 0;

        // Adding bottom tables
        $sheet->fromArray($dataArray, null, 'A'.($context['sandbox']['line'] + 2), true);

        for ($i = 0; $i <= 3; $i++) {
            $agglo = fgetcsv($r, 5000, ';');
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
            $sheet->setCellValueByColumnAndRow($ind['total_%_(2)'], $line_to_fill, round($agglo[$ind['total_%_(2)']], 1));
            $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system'], $line_to_fill, round($agglo[$ind['connected_to_collecting_system']], 1));
            $sheet->setCellValueByColumnAndRow($ind['connected_to_collecting_system_p._e.'], $line_to_fill, $agglo[$ind['connected_to_collecting_system_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_%_of_gen._load'], $line_to_fill, round($agglo[$ind['addressed_through_ias_%_of_gen._load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['addressed_through_ias_p._e.'], $line_to_fill, $agglo[$ind['addressed_through_ias_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_%_of_gen._load'], $line_to_fill, round($agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['neither_collecting_system_nor_ias_p._e.'], $line_to_fill, $agglo[$ind['neither_collecting_system_nor_ias_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art3_%_of_gen._load_(3)'], $line_to_fill, round($agglo[$ind['dist_art3_%_of_gen._load_(3)']], 1));
            $sheet->setCellValueByColumnAndRow($ind['dist_art3_in_p.e.'], $line_to_fill, $agglo[$ind['dist_art3_in_p.e.']]);
            $sheet->setCellValueByColumnAndRow($ind['total_entering_load_p._e.'], $line_to_fill, $agglo[$ind['total_entering_load_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['ratio_connected/enentering_%'], $line_to_fill, round($agglo[$ind['ratio_connected/enentering_%']], 1));
            $sheet->setCellValueByColumnAndRow($ind['total_treatment_plant_design_capacity_p._e.'], $line_to_fill, $agglo[$ind['total_treatment_plant_design_capacity_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['ratio_total_entering_load/total_design_capacity_%'], $line_to_fill, round($agglo[$ind['ratio_total_entering_load/total_design_capacity_%']], 1));
            $sheet->setCellValueByColumnAndRow($ind['art4_compliant_%_of_gen._load'], $line_to_fill, round($agglo[$ind['art4_compliant_%_of_gen._load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['art4_compliant_p._e.'], $line_to_fill, $agglo[$ind['art4_compliant_p._e.']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant_%_of_gen._load'], $line_to_fill, round($agglo[$ind['art4_not_compliant_%_of_gen._load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['art4_not_compliant'], $line_to_fill, $agglo[$ind['art4_not_compliant']]);
            $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant_%_of_gen._load'], $line_to_fill, round($agglo[$ind['art4_not_relevant_%_of_gen._load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['art4_not_relevant'], $line_to_fill, $agglo[$ind['art4_not_relevant']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat__%_of_connected_load'], $line_to_fill, round($agglo[$ind['dist_art4_treat__%_of_connected_load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_treat_in_p.e._(4)'], $line_to_fill, $agglo[$ind['dist_art4_treat_in_p.e._(4)']]);
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_%_of_connected_load'], $line_to_fill, round($agglo[$ind['dist_art4_perf_%_of_connected_load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['dist_art4_perf_in_p.e._(5)'], $line_to_fill, $agglo[$ind['dist_art4_perf_in_p.e._(5)']]);
            $sheet->setCellValueByColumnAndRow($ind['distance_art5_treat_%_of_gen._load'], $line_to_fill, round($agglo[$ind['distance_art5_treat_%_of_gen._load']], 1));
            $sheet->setCellValueByColumnAndRow($ind['distance_art5_treat_in_p.e._(6)'], $line_to_fill, $agglo[$ind['distance_art5_treat_in_p.e._(6)']]);
            $sheet->setCellValueByColumnAndRow($ind['distance_art5_perf_%_of_gen._load'], $line_to_fill, round($agglo[$ind['distance_art5_perf_%_of_gen._load']], 1));
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
                    $context['results']['auto_report']['NB_AGGLO'] = $agglo[0]; //save for auto generation of report
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
    //$debug_times['end_loop2'] = time()-$debug_times['start'];
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
/*    
    $objWriter->__destruct();
    $objReader->__destruct();
*/     
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);
    gc_collect_cycles();

	$context['sandbox']['progress']++;
	
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 * 
 * Add distance to compliance info
 */
function uwwtd_create_register_2015_file_agglo_debug($annee, &$context)
{
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_debug']['csv_name'], $annee);
    $ragglofile = fopen($filename, 'rb');
    $filename2 = $filename . '.with_distance.csv';
    $ragglofile2 = fopen($filename2, 'wb');
    //we get the distance to compliance info by agg from csv file
    $filename_aggdistance = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_distance_name'], $annee);
    $raggdistance = fopen($filename_aggdistance, 'rb');
    
    fgetcsv($raggdistance, 0, ';');
    while ($line = fgetcsv($raggdistance, 0, ';')) {
        $context['sandbox']['dist'][$line[0]] = $line;
    }
    fclose($raggdistance);   

    // $j = 0;
    // $lines = array();
	//Warning if the csv file contain more than 3000 lines => the system can probably never re-open the excel file
    $agglo = fgetcsv($ragglofile, 5000, ';');
        $line = array();
        // uwwtd_trace('============================================>'.$j);
        foreach($agglo as $k=>$conf){
 
            // $val = $agglo[$k];
            //Other condition on field ?
            $line[$k] = $agglo[$k];
            // $line[$k] = uwwtd_register_get_glossary_label($conf, $line[$k]);

                
            // }

            
        }

        // 'target_connection_total_load_pe',
        // 'target_connection_distance_pe', 
        // 'target_connection_distance_%', 
        // 'secondary_target_total_load_pe',
        // 'secondary_target_monitoring_result_distance_pe', 
        // 'secondary_target_monitoring_result_distance_%',
        // 'morestringent_total_load_pe',
        // 'morestringent_target_monitoring_result_distance_pe', 
        // 'morestringent_target_monitoring_result_distance_%', 
        if (isset($context['sandbox']['dist'][ $line[0] ])) {    
                
            $codeagglo = $line[0];
            $line[] = $context['sandbox']['dist'][$codeagglo][1];
            $line[] = $context['sandbox']['dist'][$codeagglo][2];
            $line[] = $context['sandbox']['dist'][$codeagglo][3];
            $line[] = $context['sandbox']['dist'][$codeagglo][4];
            $line[] = $context['sandbox']['dist'][$codeagglo][5];
            $line[] = $context['sandbox']['dist'][$codeagglo][6];
            $line[] = $context['sandbox']['dist'][$codeagglo][7];
            $line[] = $context['sandbox']['dist'][$codeagglo][8];
            $line[] = $context['sandbox']['dist'][$codeagglo][9];
        }
        fputcsv($ragglofile2, $line, ';');
        // $lines[$j] = $line;
        // uwwtd_trace(var_export($line, true));
        // $j++;
        // $context['sandbox']['file_position'] = ftell($ragglofile);
        // $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        //$sheet->insertNewRowBefore($context['sandbox']['line']+$j, 1);

}

/**
 * Too much memory
 */
function uwwtd_create_register_2015_file_agglo_debug_old($annee, &$context)
{
    //$debug_times=array();
	uwwtd_trace(__FUNCTION__);
    $indConf = $GLOBALS['uwwtd']['sheet3_agglo'];
	//Keep compatibility
	$ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_debug']['csv_name'], $annee);

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
        $context['sandbox']['line'] = 5; //start line   //5 -> 6

        $path_xlsx = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_debug']['xlsx_name'], $annee);
        //Remove the old file, if needed
        drupal_unlink($target_path);

        //we get the distance to compliance info by agg from csv file
        $filename_aggdistance = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_distance_name'], $annee);
        $raggdistance = fopen($filename_aggdistance, 'rb');
        fgetcsv($raggdistance, 0, ';');
        while ($line = fgetcsv($raggdistance, 0, ';')) {
            $context['sandbox']['dist'][$line[0]] = $line;
        }
        fclose($raggdistance);    
        
    } else {
        $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_debug']['xlsx_name'], $annee);
        $target_path = $path_xlsx;
    }

    fseek($ragglofile, $context['sandbox']['file_position']);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);

    $objReader->setLoadSheetsOnly( array(
		$GLOBALS['uwwtd']['sheet']['agglo_debug']['xlsx_model_sheet_name'],
    ));

    $objPHPExcel = $objReader->load(drupal_realpath($path_xlsx));
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['agglo_debug']['xlsx_model_sheet_name']);

    // $dataArray = $sheet->rangeToArray(
    //     'A7:CV41',   //A6:CV40        A7:CV41
    //     NULL,
    //     FALSE,
    //     TRUE,
    //     FALSE
    // );

	// uwwtd_trace('init xlsx sheet');
//     $start_line = 4;

// uwwtd_trace('init xlsx sheet');
	$j = 0;
    $lines = array();
	//Warning if the csv file contain more than 3000 lines => the system can probably never re-open the excel file
    while ($j < 10000 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
        $line = array();
        uwwtd_trace('============================================>'.$j);
        foreach($agglo as $k=>$conf){
 
            // $val = $agglo[$k];
            //Other condition on field ?
            $line[$k] = $agglo[$k];
            // $line[$k] = uwwtd_register_get_glossary_label($conf, $line[$k]);

                
            // }

            
        }

        // 'target_connection_total_load_pe',
        // 'target_connection_distance_pe', 
        // 'target_connection_distance_%', 
        // 'secondary_target_total_load_pe',
        // 'secondary_target_monitoring_result_distance_pe', 
        // 'secondary_target_monitoring_result_distance_%',
        // 'morestringent_total_load_pe',
        // 'morestringent_target_monitoring_result_distance_pe', 
        // 'morestringent_target_monitoring_result_distance_%', 
        if (isset($context['sandbox']['dist'][ $line[0] ])) {    
                
            $codeagglo = $line[0];
            $line[] = $context['sandbox']['dist'][$codeagglo][1];
            $line[] = $context['sandbox']['dist'][$codeagglo][2];
            $line[] = $context['sandbox']['dist'][$codeagglo][3];
            $line[] = $context['sandbox']['dist'][$codeagglo][4];
            $line[] = $context['sandbox']['dist'][$codeagglo][5];
            $line[] = $context['sandbox']['dist'][$codeagglo][6];
            $line[] = $context['sandbox']['dist'][$codeagglo][7];
            $line[] = $context['sandbox']['dist'][$codeagglo][8];
            $line[] = $context['sandbox']['dist'][$codeagglo][9];
        }

        $lines[$j] = $line;
        uwwtd_trace(var_export($line, true));
        $j++;
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        //$sheet->insertNewRowBefore($context['sandbox']['line']+$j, 1);
    }
//     $sheet->insertNewRowBefore($context['sandbox']['line'], $j+1);
    $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, $j+1);
    $sheet->fromArray($lines, null, 'A'.$context['sandbox']['line'], true);
    
    $context['sandbox']['line']=$context['sandbox']['line']+$j;
	
    fclose($ragglofile);
    unset($ragglofile);

    //$debug_times['end_loop2'] = time()-$debug_times['start'];
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
/*    
    $objWriter->__destruct();
    $objReader->__destruct();
*/     
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);
    gc_collect_cycles();

	$context['sandbox']['progress']++;
	
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_create_register_2015_file_uwwtp_sheet2($annee, &$context)
{
    // uwwtd_trace(__FUNCTION__);
    $start_time = time();
    $ind = $GLOBALS['uwwtd']['sheet2_uwwtp'];
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'], $annee);
    // uwwtd_trace('Open file: '.$filename);

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
        $context['sandbox']['line'] = 5; //start line   5 -> 6
        // uwwtd_trace('-- file size : '.$context['sandbox']['max']);


        $path_xlsx = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'], $annee);
         //Remove the old file, if needed
        //  uwwtd_trace('Open xlsx: '.$target_path);
        drupal_unlink($target_path);
        
    } else {
//         $path_xlsx = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'] .'.xlsx';
        $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'], $annee);
        $target_path = $path_xlsx;
        // uwwtd_trace('ReOpen xlsx: '.$target_path);
    }

//     dsm($context);
//     dsm($path_xlsx);
//     dsm($target_path);

    fseek($ruwwtpfile, $context['sandbox']['file_position']);


    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'],
                                        ) );
    //uwwtd_trace('start load');
    $objPHPExcel = $objReader->load(drupal_realpath($path_xlsx));
    //uwwtd_trace('end load');
    //uwwtd_trace('start getSheetByName');
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name']);

    $dataArray = $sheet->rangeToArray(
        'A7:CQ13',   //A6:CQ12   A7:CQ13
        NULL,
        FALSE,
        TRUE,
        FALSE
    );

    $j = 0;
    // uwwtd_trace('start loop');
    $lines = array();
    $trace = 0;
	//Warning if the csv file contain more than 10000 lines => the system can probably never re-open the excel file
    while ($j < 10000 && $uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {

        //if($trace == 10){
//             uwwtd_trace('-- line : '.$j .' - time : '. (time() - $start_time));
        //}
        //else{
        //    $trace = 0;
        //}
        //$trace ++;
        $line = array();

        foreach($ind as $k=>$conf){
            if($conf['xls']){
                if(isset($uwwtp[$conf['k']])){
                    $val = $uwwtp[$conf['k']];
                    //If we have a format wrapper
                    if(isset($conf['wrapper'])){
                        switch($conf['wrapper']){
                            case 'round':
                                $val = round($val);
                              break;
                            case 'round1':
                                $val = round($val,1);
                              break;

                        }
                    }
                }else{
                    $val = null;
                }
                //Other condition on field ?
                $line[$conf['k']] = (string)$val;
                $line[$conf['k']] = uwwtd_register_get_glossary_label($conf, $line[$conf['k']]);
            }

            
            

   
            
        }
        $lines[$j] = $line;
        $j++;
        $context['sandbox']['file_position'] = ftell($ruwwtpfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];

        //$sheet->insertNewRowBefore($context['sandbox']['line']+$j, 1);
    }
    $sheet->insertNewRowBefore($context['sandbox']['line']+1, $j+1);    //+1 pour    viter de copier le style de la premi  �re ligne! et copier la ligne blanche vide
    $sheet->fromArray($lines, null, 'A'.$context['sandbox']['line'], true);

    //Special rules for cell's style

    if(!empty($lines)){
//         uwwtd_trace('set style');
        foreach($lines as $lid=>$line){
            $idx = $ind['previous_Percentage of change of entering load (1) %']['k'];
            if(isset($line[$idx]) && abs((float)$line[$idx])>20){
                $sheet->getStyleByColumnAndRow($idx, $context['sandbox']['line']+$lid)
                        ->getFill()
                        ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)
                        ->getStartColor()->setARGB('FFFFC000');
            }
            $idx = $ind['previous_Percentage of change of capacity (2) %']['k'];
            if (isset($line[$idx]) && $line[$idx] != 'NI' && abs((float)$line[$idx])>0  ) {
                $sheet->getStyleByColumnAndRow($idx, $context['sandbox']['line']+$lid)
                    ->getFill()
                    ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)      // 255 192 0 FFC000
                    ->getStartColor()->setARGB('FFFFC000');
            }
            //est-ce n   cessaire ?
            // else{
                    // $sheet->getStyleByColumnAndRow($idx, $context['sandbox']['line']+$lid)
                        // ->getFill()
                        // ->setFillType(PHPExcel_Style_Fill::FILL_SOLID)      // 255 192 0 FFC000
                        // ->getStartColor()->setARGB('FFFFFFFF');
            // }

        }
//         uwwtd_trace('end style');
    }

    $context['sandbox']['line']=$context['sandbox']['line']+$j;
//     uwwtd_trace('end loop');
	//uwwtd_trace('Progress : '.$context['sandbox']['progress'].' / '.$context['sandbox']['max']);
    fclose($ruwwtpfile);
// 	uwwtd_trace('Start Total data');

    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_total_name'], $annee);
        $r = fopen($filename, 'rb');
        $line_to_fill = 0;

        // Adding bottom tables
        $sheet->fromArray($dataArray, null, 'A'.($context['sandbox']['line'] + 2), true);

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
			if(isset($line_to_fill)){
				if (isset($ind['uww_entering this UWWTP % of gen. load']['k']) && isset($uwwtp[$ind['uww_entering this UWWTP % of gen. load']['k']]))
					$sheet->setCellValueByColumnAndRow($ind['uww_entering this UWWTP % of gen. load']['k'], $line_to_fill, round($uwwtp[$ind['uww_entering this UWWTP % of gen. load']['k']], 1));
				if (isset($ind['uww_entering load  p. e.']['k']) && isset($uwwtp[$ind['uww_entering load  p. e.']['k']]))
					$sheet->setCellValueByColumnAndRow($ind['uww_entering load  p. e.']['k'], $line_to_fill, round($uwwtp[$ind['uww_entering load  p. e.']['k']]));
				if (isset($ind['uww_capacity p. e.']['k']) && isset($uwwtp[$ind['uww_capacity p. e.']['k']]))
					$sheet->setCellValueByColumnAndRow($ind['uww_capacity p. e.']['k'], $line_to_fill, round($uwwtp[$ind['uww_capacity p. e.']['k']]));
				if (isset($ind['previous_entering load p. e.']['k']) && isset($uwwtp[$ind['previous_entering load p. e.']['k']]))
					$sheet->setCellValueByColumnAndRow($ind['previous_entering load p. e.']['k'], $line_to_fill, round($uwwtp[$ind['previous_entering load p. e.']['k']]));
				if (isset($ind['previous_capacity p. e.']['k']) && isset($uwwtp[$ind['previous_capacity p. e.']['k']]))
					$sheet->setCellValueByColumnAndRow($ind['previous_capacity p. e.']['k'], $line_to_fill, round($uwwtp[$ind['previous_capacity p. e.']['k']]));
				if (isset($ind['previous_Percentage of change of entering load (1) %']['k']) )
					$sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of entering load (1) %']['k'], $line_to_fill, round($uwwtp[$ind['previous_Percentage of change of entering load (1) %']['k']], 1));
				if (isset($ind['previous_Percentage of change of capacity (2) %']['k']) )
					$sheet->setCellValueByColumnAndRow($ind['previous_Percentage of change of capacity (2) %']['k'], $line_to_fill, round($uwwtp[$ind['previous_Percentage of change of capacity (2) %']['k']], 1));
			}
        }
        fclose($r);
    }
	//uwwtd_trace('End Total data');
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));

    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);

     $context['sandbox']['progress']++;

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}


function uwwtd_create_register_2015_file_glossary_sheet0($annee, &$context)
{
//     if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

    $model_path = uwwtd_getregister_modelfile_path($annee);
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['rca']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['glossary']['xlsx_name'], $annee);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['glossary']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));

    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['glossary']['xlsx_model_sheet_name']);

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);
	
	//In order to finish the script 
//     $context['sandbox']['progress']++;

//     $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name']));

//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }
}

function uwwtd_create_register_2015_file_rca_sheet1($annee, &$context)
{
    if (!isset($context['sandbox']['progress'])) {
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

    $model_path = uwwtd_getregister_modelfile_path($annee);
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['rca']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rca']['xlsx_name'], $annee);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));

    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name']);

    $start_line = 7; //6 -> 8 (l �gende + filtre)
//     $sheet->insertNewRowBefore(7, count($listrca));   todo    $listrca exist pas

    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet1_rca'];

    //get content of cell A10 (1) the amount has to be calculated with the sum of measured, calculated, estimated
    $cell = $sheet->getCell('A10');
    $A10 = $cell->getValue();
    $cell->setValue('');
    //get content of cell A11 (2) possibility to have several related sensitive areas, successor and predecessors. If it is a case separation with ;
    $cell = $sheet->getCell('A11');
    $A11 =  $cell->getValue();
    $cell->setValue('');
    $i++;
//     $sheet->insertNewRowBefore($i, 1);


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

    // Set "Rate of Removal N-tot" and "Rate of Removal P-tot" column in percent format :
    $sheet->getStyle('AD6:AD10000')->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE));
    $sheet->getStyle('AG6:AG10000')->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE));

    //we rewrite the legend
    $i++;
//     $sheet->insertNewRowBefore($i, 1);
    $sheet->setCellValueByColumnAndRow(0, $i, $A10);
    $i++;
//     $sheet->insertNewRowBefore($i, 1);
    $sheet->setCellValueByColumnAndRow(0, $i, $A11);

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);
	
	//In order to finish the script 
    $context['sandbox']['progress']++;

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name']));

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_create_register_2020_sa_history($annee, &$context){
    if (!isset($context['sandbox']['progress'])) {
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }
    
    $model_path = uwwtd_getregister_modelfile_path($annee);
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rca_history']['xlsx_name'], $annee);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
        $GLOBALS['uwwtd']['sheet']['rca_history']['xlsx_model_sheet_name'],
    ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));

    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['rca_history']['xlsx_model_sheet_name']);

    $cc = strtoupper(variable_get('siif_eru_country_code', '-1'));
    $data_history = json_decode(file_get_contents(drupal_get_path('module', 'uwwtd').'/inc/sa_history.json'), true);
    $data_history = $data_history[$cc];
    
    //Report number of the current year
    $cyear = (int)substr($GLOBALS['report_number'][$annee ], 0, -2);
    
    $out = [];
    
    //Get active sensitive area list
    $rca = uwwtd_get_all_rca($annee);
    if(!empty($rca)){
        usort($rca, "sort_rca_by_title"); 
        //For each sensitive area we get the history
        foreach($rca as $node){
            $histories = uwwtd_check_history($node->field_inspireidlocalid['und'][0]['value'], $node->field_anneedata['und'][0]['value'], $node->type);
            $localIds = uwwtd_get_possible_localids($node->field_inspireidlocalid['und'][0]['value']);
            if($histories != false){
                foreach($histories as $history){  

                    $other = node_load($history);
                    $otherY = $other->field_anneedata['und'][0]['value'];
                        
                        $designation_dt = [];
                    
                        $designation_criteria = $GLOBALS['uwwtd_value']['rcatype_labels'][$other->field_zonetype['und'][0]['value']];
                        if($other->field_specialisedzonetype['und'][0]['value']!=''){
                            $designation_criteria .= ' '.substr($GLOBALS['uwwtd_value']['rca_spe_type_labels'][$other->field_specialisedzonetype['und'][0]['value']], 0, -4);
                        } 
                        if($other->field_rcaanitro['und'][0]['value']==1 && $other->field_rcaaphos['und'][0]['value']==1){
                            $designation_criteria .= ', a (N+P)';
                            if($other->field_rcaadatedesignation['und'][0]['value']!='') $designation_dt['aN'] = strtotime($other->field_rcaadatedesignation['und'][0]['value']);
                            if($other->field_rcaapdatedesignation['und'][0]['value']!='') $designation_dt['aP'] = strtotime($other->field_rcaapdatedesignation['und'][0]['value']);
                        }elseif($other->field_rcaanitro['und'][0]['value']==1){
                            $designation_criteria .= ', a (N)';
                            if($other->field_rcaadatedesignation['und'][0]['value']!='') $designation_dt['aN'] = strtotime($other->field_rcaadatedesignation['und'][0]['value']);
                        }
                        elseif($other->field_rcaaphos['und'][0]['value']==1){
                            $designation_criteria .= ', a (P)';
                            if($other->field_rcaapdatedesignation['und'][0]['value']!='') $designation_dt['aP'] = strtotime($other->field_rcaapdatedesignation['und'][0]['value']);
                        }
                        if($other->field_rcab['und'][0]['value']==1){
                            $designation_criteria .= ', b';
                            if($other->field_rcabdatedesignation['und'][0]['value']!='') $designation_dt['b'] = strtotime($other->field_rcabdatedesignation['und'][0]['value']);
                        }
                        if($other->field_rcac['und'][0]['value']==1){
                            $designation_criteria .= ', c';
                            if($other->field_rcacdatedesignation['und'][0]['value']!='') $designation_dt['c'] = strtotime($other->field_rcacdatedesignation['und'][0]['value']);
                            if($other->field_rcacrelevantdirective['und'][0]['value'] !='' ){
                                $designation_criteria .= '('.$other->field_rcacrelevantdirective['und'][0]['value'].')';
                            }
                        }
                        /*
                        if($other->field_rca54applied['und'][0]['value']==1){
                            $designation_dt['54'] = substr($other->field_rcadateart54['und'][0]['value'], 0, 10);
                        }*/
                        if($other->field_rca58applied['und'][0]['value']==1){
                            $designation_criteria .= ', c';
                            if($other->field_rcaart58datedesign['und'][0]['value']!='') $designation_dt['58'] = strtotime($other->field_rcaart58datedesign['und'][0]['value']);
                        }
                        //a : field_rcaanitro + field_rcaaphos + field_rcaadatedesignation
                        //b : field_rcab + field_rcabdatedesignation 
                        //c : field_rcac  + field_rcacdatedesignation + field_rcacrelevantdirective
                        //field_rca52applied / field_rca58applied / field_rca54applied
                        //field_specialisedzonetype
                        //field_zonetype
                    $otherList[(int)substr($GLOBALS['report_number'][$otherY], 0, -2)] =  [
                    //$otherList[$otherY] =  [
                        'id'=>$other->field_inspireidlocalid['und'][0]['value'],
                        'name'=>$other->title,
                        'designation_dt'=>date('d/m/Y', max(array_values($designation_dt))), 
                         'designation_criteria'=>$designation_criteria,                    
                        //'node'=>$other
                    ];
                }
                krsort($otherList);
            }
            //Try to find historic data
            if(!empty($localIds)){
                foreach($localIds as $id){
                    if(isset($data_history[$id])){
                       if(isset($data_history[$id]['8th']) && !isset($otherList['8th'])){
                           $otherList[8] = $data_history[$id]['8th'];
                       }
                       if(isset($data_history[$id]['7th']) && !isset($otherList['7th'])){
                           $otherList[7] = $data_history[$id]['7th'];
                       }
                       if(isset($data_history[$id]['6th']) && !isset($otherList['6th'])){
                           $otherList[6] = $data_history[$id]['6th'];
                       }
                       if(isset($data_history[$id]['5th']) && !isset($otherList['5th'])){
                           $otherList[5] = $data_history[$id]['5th'];
                       }
                       if(isset($data_history[$id]['4th']) && !isset($otherList['4th'])){
                           $otherList[4] = $data_history[$id]['4th'];
                       }
                       unset($data_history[$id]);
                       break;
                    }
                }
            }
            //Fill empty cells
            for($i=$cyear; $i>4; $i--){
                if(!isset($otherList[$i])){
                    $otherList[$i] = [
                        'id'=>'',
                        'name'=>'',
                        'designation_criteria'=>'',
                        'designation_dt'=>''
                    ];
                }
                
            }
            krsort($otherList);
           $out[$node->field_inspireidlocalid['und'][0]['value']] = $otherList;
        }
    }
   unset($rca, $node, $histories, $otherList, $localIds);
    
    //If some area are still the data_history, they are probalably disable
    $not_reported = [];
    
    if(!empty($data_history)){
        foreach($data_history as $id=>$hist){
            $nr = [
                8=>$hist['8th'],
                7=>$hist['7th'],
                6=>$hist['6th'],
                5=>$hist['5th'],
                4=>$hist['4th']
            ];
            
            for($i=$cyear; $i>8; $i--){
                $nr[$i] = [
                    'id'=>'Not reported - '.$id,
                    'name'=>'',
                    'designation_criteria'=>'',
                    'designation_dt'=>''
                ];
            }
            krsort($nr);
            $not_reported[$id] = $nr;
        }
    }
    unset($data_history, $hist);
    /*
    dsm($out);
    dsm($not_reported );
    */
    //Render the output
    $rows = [];
    $keys = [
        'id'=>'ID of Sensitive Area',
        'name'=>'Name of Sensitive Area',
        'designation_criteria'=>'Designation criteria',
        'designation_dt'=>'Date of designation'
    ];
    $headers = [];
    for($i=$cyear; $i>=4; $i--){
        $headers[0][] = $i.'th Synthesis Report';
        $headers[0][] = '';
        $headers[0][] = '';
        $headers[0][] = '';
        
        foreach($keys as $k=>$label){
            $headers[1][] = $label;
        }
    }
    
    foreach($out as $id=>$area){
        $row = [];
        foreach($area as $report=>$info){
            foreach($keys as $k=>$label){
                $row[] = $info[$k];
            }
        }
        $rows[] = $row;
        unset($row);
    }
    if(!empty($not_reported )){
        foreach($not_reported as $id=>$area){
            foreach($area as $report=>$info){
                foreach($keys as $k=>$label){
                    $row[] = $info[$k];
                }
            }
            $rows[] = $row;
            unset($row);
        }
    }
    
    // $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, $j+1);
    $sheet->fromArray($headers, null, 'A1', true);
    $sheet->fromArray($rows, null, 'A3', true);
    
    /*
    $output = theme('table', 
        [
            'header'=>$headers, 
            'rows'=>$rows,
            'rows_multiple'=>true,
            'attributes' => array('class' => 'customTable'),    
        ]
    );
    */
    //write
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
        $objWriter->setPreCalculateFormulas(false);
        //$objWriter->setOffice2003Compatibility(true);
        $objWriter->save(drupal_realpath($target_path));
        
        $objPHPExcel->__destruct();
        unset($objPHPExcel);
        unset($objWriter);
        unset($objReader);

	//In order to finish the script 
    $context['sandbox']['progress']++;

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rca_history']['xlsx_model_sheet_name']));

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
    
}

/**
 *
 */
function uwwtd_create_register_2015_file_rca54_sheet1_1($annee, &$context)
{
    if (!isset($context['sandbox']['progress'])) {
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }

    $model_path = uwwtd_getregister_modelfile_path($annee);
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rcaA54']['xlsx_name'], $annee);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['rcaA54']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));

    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['rcaA54']['xlsx_model_sheet_name']);

    $start_line = 4; //e.v delete line 1 in header in model 20190319


    $i = $start_line;
    $ind = $GLOBALS['uwwtd']['sheet1_rcaA54'];

 

//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['rca']['csv_name']. '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rcaA54']['csv_name'], $annee);
    if(file_exists ($filename)){
        $rrcafile = fopen($filename, 'rb');

        //ignore header //decomment for debug on xls final file
        $rca = fgetcsv($rrcafile, 5000, ';');

        while ($rca = fgetcsv($rrcafile, 5000, ';')) {
    //       dsm($i);
    //       dsm($rca);
    //     foreach($listrca as $rca) {
            $sheet->setCellValueByColumnAndRow($ind['code'], $i, $rca[$ind['code']]);
            $sheet->setCellValueByColumnAndRow($ind['name'], $i, $rca[$ind['name']]);
            $sheet->setCellValueByColumnAndRow($ind['designation_date'], $i, $rca[$ind['designation_date']]);
            $sheet->setCellValueByColumnAndRow($ind['deadline_n'], $i, $rca[$ind['deadline_n']]);
            $sheet->setCellValueByColumnAndRow($ind['deadline_p'], $i, $rca[$ind['deadline_p']]);
            $sheet->setCellValueByColumnAndRow($ind['application_art_5_4'], $i, $rca[$ind['application_art_5_4']]);
            $sheet->setCellValueByColumnAndRow($ind['uwwtp_number'], $i, $rca[$ind['uwwtp_number']]);
            $sheet->setCellValueByColumnAndRow($ind['uwwtp_capacity_total'], $i, $rca[$ind['uwwtp_capacity_total']]);
            $sheet->setCellValueByColumnAndRow($ind['n_in'], $i, $rca[$ind['n_in']]);
            $sheet->setCellValueByColumnAndRow($ind['n_out'], $i, $rca[$ind['n_out']]);
            $sheet->setCellValueByColumnAndRow($ind['p_in'], $i, $rca[$ind['p_in']]);
            $sheet->setCellValueByColumnAndRow($ind['p_out'], $i, $rca[$ind['p_out']]);
            $sheet->setCellValueByColumnAndRow($ind['n_reduction'], $i, $rca[$ind['n_reduction']]);
            $sheet->setCellValueByColumnAndRow($ind['p_reduction'], $i, $rca[$ind['p_reduction']]);
            $sheet->setCellValueByColumnAndRow($ind['compliant_5_4'], $i, $rca[$ind['compliant_5_4']]);
            $sheet->setCellValueByColumnAndRow($ind['transition_period_pending'], $i, $rca[$ind['transition_period_pending']]);
            $sheet->setCellValueByColumnAndRow($ind['remarks_by_ms'], $i, $rca[$ind['remarks_by_ms']]);

            $i++;

            $sheet->insertNewRowBefore($i, 1);
        }
        fclose($rrcafile);

        // Set "Rate of Removal N-tot" and "Rate of Removal P-tot" column in percent format :
    //     $sheet->getStyle('AD6:AD10000')->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE));
    //     $sheet->getStyle('AG6:AG10000')->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE));



        //write
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
        $objWriter->setPreCalculateFormulas(false);
        //$objWriter->setOffice2003Compatibility(true);
        $objWriter->save(drupal_realpath($target_path));
        
        $objPHPExcel->__destruct();
        unset($objPHPExcel);
        unset($objWriter);
        unset($objReader);
	}else{
        unlink(drupal_realpath($target_path));
    }
	//In order to finish the script 
    $context['sandbox']['progress']++;

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rcaA54']['xlsx_model_sheet_name']));

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 *
 */
function uwwtd_create_register_2015_summary_big_cities_10th_report_step_bigcity_sheet8($annee) 
{
// uwwtd_trace(__FUNCTION__); 
  $ind = $GLOBALS['uwwtd']['sheet8_summary_big_cities'];
  if (!isset($context['sandbox']['progress'])) {

      $context['sandbox']['progress'] = 0;
      $context['sandbox']['idagg'] = array();


     // //we get all cities with big city ID info filled in field_aggbigid
      $query = 'select n.nid
      from {node} n
          inner join
              {field_data_field_anneedata} f_annee
                  ON n.nid = f_annee.entity_id   and n.vid = f_annee.revision_id
          inner join
              {field_data_field_aggbigid} b
                  ON n.nid = b.entity_id  and n.vid = b.revision_id
      where n.type = :typenode
          and f_annee.field_anneedata_value = :annee
          and (b.field_aggbigid_value is not null and  b.field_aggbigid_value != \'\')
      order by b.field_aggbigid_value asc, n.title asc
      ';

      $param = array(
          ':typenode' => 'agglomeration',
          ':annee' => $annee,
          );

      try {
          $result = db_query($query, $param);
          while($row = $result->fetchAssoc()) {
              $context['sandbox']['idagg'][] = $row['nid'];
          }
          unset($result, $row);
      }catch(Exception $e) {
          dsm($e->getMessage());
      }
      $context['sandbox']['max'] = count($context['sandbox']['idagg']);
      $context['sandbox']['progressagg'] = 0;

  }

  $sum_by_bigid = array();
  while ($context['sandbox']['progressagg'] < $context['sandbox']['max']) {
      $line = array();      
      if($agg = node_load($context['sandbox']['idagg'][$context['sandbox']['progressagg']])){
          $bigID = $agg->field_aggbigid['und'][0]['value'];  
// uwwtd_trace('before uwwtd_summary_big_cities_get_agglo_detail');
          $agg_detail = uwwtd_summary_big_cities_get_agglo_detail($annee, $agg);
          
        //   uwwtd_trace($agg->title . "(" . $agg->nid . ") / " . $bigID);

          if (!isset($sum_by_bigid[$bigID])) {
            $sum_by_bigid[$bigID] = array(
              'generated_load' => 0,

              'collecting_system' => 0,
              'ias' => 0,
              'no_collecting_system_nor_ias' => 0,

              'type_of_receiving_area' => array(),
              'id_of_sensitive_area' => array(),
              'name_of_sensitive_area' => array(), 

              'treat_3np_3n_3p_pe' => 0,
              'treat_3other_pe' => 0,
              'treat_2_pe' => 0,
              'treat_1_pe' => 0,
              'treat_0_pe' => 0,

              'treat_3np_3n_3p_agggenerated' => 0,
              'treat_3other_agggenerated' => 0,
              'treat_2_agggenerated' => 0,
              'treat_1_agggenerated' => 0,
              'treat_0_agggenerated' => 0,

              'list_code_agglo' => array(), 
              'list_name_agglo' => array(),      
            );
          }  
    
          $sum_by_bigid[$bigID]['generated_load'] += $agg_detail['generated_load'];
          $sum_by_bigid[$bigID]['collecting_system'] += $agg_detail['collecting_system'];
          $sum_by_bigid[$bigID]['ias'] += $agg_detail['ias'];
          $sum_by_bigid[$bigID]['no_collecting_system_nor_ias'] += $agg_detail['no_collecting_system_nor_ias'];

          $sum_by_bigid[$bigID]['treat_3np_3n_3p_pe'] += $agg_detail['treat_3np_3n_3p_pe'];
          $sum_by_bigid[$bigID]['treat_3np_3n_3p_agggenerated'] += $agg_detail['treat_3np_3n_3p_agggenerated'];

          $sum_by_bigid[$bigID]['treat_3other_pe'] += $agg_detail['treat_3other_pe'];
          $sum_by_bigid[$bigID]['treat_3other_agggenerated'] += $agg_detail['treat_3other_agggenerated'];

          $sum_by_bigid[$bigID]['treat_2_pe'] += $agg_detail['treat_2_pe'];
          $sum_by_bigid[$bigID]['treat_2_agggenerated'] += $agg_detail['treat_2_agggenerated'];

          $sum_by_bigid[$bigID]['treat_1_pe'] += $agg_detail['treat_1_pe'];
          $sum_by_bigid[$bigID]['treat_1_agggenerated'] += $agg_detail['treat_1_agggenerated'];

          $sum_by_bigid[$bigID]['treat_0_pe'] += $agg_detail['treat_0_pe'];
          $sum_by_bigid[$bigID]['treat_0_agggenerated'] += $agg_detail['treat_0_agggenerated'];


          $sum_by_bigid[$bigID]['type_of_receiving_area'] = array_merge($sum_by_bigid[$bigID]['type_of_receiving_area'], $agg_detail['dcptype']);
          $sum_by_bigid[$bigID]['id_of_sensitive_area'] = array_merge($sum_by_bigid[$bigID]['id_of_sensitive_area'], $agg_detail['rcaCode']);
          $sum_by_bigid[$bigID]['name_of_sensitive_area'] = array_merge($sum_by_bigid[$bigID]['list_name_agglo'], $agg_detail['rcaName']);

          $sum_by_bigid[$bigID]['list_code_agglo'][] = $agg->field_inspireidlocalid['und'][0]['value'];
          $sum_by_bigid[$bigID]['list_name_agglo'][] = $agg->title;     

      }
      $context['sandbox']['progressagg']++;
  }
 
//   uwwtd_trace(var_export($sum_by_bigid, true));
 
  $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name'], $annee);
  $r = fopen($filename, 'ab');  //create by the precedent function batch with list of big dischargers
  //we write the csv
  foreach ($sum_by_bigid as $bigID => $bigid_detail) { 
    //nd@oieau.fr 16/12/2020 : we keep only big cities with generated load > 150 000 pe
    if($bigid_detail['generated_load'] >=150000){
      $line = array();
      $line[$ind['id']] = $bigID;
      $line[$ind['name']] = $GLOBALS['big_cities'][$bigID]['name'];

      $line[$ind['generated_load']] = $bigid_detail['generated_load'];
      
      $line[$ind['collecting_system']] = $bigid_detail['collecting_system'] * 100 / $bigid_detail['generated_load']  ;
      $line[$ind['collecting_system_pe']] = $bigid_detail['collecting_system'] ;
      
      $line[$ind['ias']] = $bigid_detail['ias'] * 100 / $bigid_detail['generated_load']  ;
      $line[$ind['ias_pe']] = $bigid_detail['ias'] ;

      
      $line[$ind['no_collecting_system_nor_ias']] = $bigid_detail['no_collecting_system_nor_ias'] * 100 / $bigid_detail['generated_load']  ;
      $line[$ind['no_collecting_system_nor_ias_pe']] = $bigid_detail['no_collecting_system_nor_ias']  ;

      $line[$ind['treatment_0']] = 0;
      $line[$ind['treatment_1']] = 0;
      $line[$ind['treatment_2']] = 0;
      $line[$ind['treatment_3n_p_np']] = 0;
      $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = 0;

      if ($bigid_detail['treat_0_agggenerated'] > 0) {
        $line[$ind['treatment_0']] = $bigid_detail['treat_0_pe'] * 100 / $bigid_detail['treat_0_agggenerated']  ;    //NAN -> divide by 0 on CY for example
      }
      if ($bigid_detail['treat_1_agggenerated'] > 0) {
        $line[$ind['treatment_1']] = $bigid_detail['treat_1_pe'] * 100 / $bigid_detail['treat_1_agggenerated']  ;   
      }
      if ($bigid_detail['treat_2_agggenerated'] > 0) {
        $line[$ind['treatment_2']] =$bigid_detail['treat_2_pe'] * 100 / $bigid_detail['treat_2_agggenerated']  ;         
      }
      if ($bigid_detail['treat_3np_3n_3p_agggenerated'] > 0) {
        $line[$ind['treatment_3n_p_np']] = $bigid_detail['treat_3np_3n_3p_pe'] * 100 / $bigid_detail['treat_3np_3n_3p_agggenerated']  ;
      }
      if ($bigid_detail['treat_3other_agggenerated'] > 0) {
        $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = $bigid_detail['treat_3other_pe'] * 100 / $bigid_detail['treat_3other_agggenerated']  ;   
      }

      $line[$ind['type_of_receiving_area']] = implode(';', $bigid_detail['type_of_receiving_area']);
      
      $line[$ind['id_of_sensitive_area']] = implode(';', $bigid_detail['id_of_sensitive_area']);
      $line[$ind['name_of_sensitive_area']] = implode(';', $bigid_detail['name_of_sensitive_area']);

      $line[$ind['list_code_agglo']] = implode(';', $bigid_detail['list_code_agglo']);
      $line[$ind['list_name_agglo']] = implode(';', $bigid_detail['list_name_agglo']);
      fputcsv($r, $line, ';');
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
function uwwtd_create_register_2015_summary_big_cities_10th_report_step_bigdischargers_sheet8($annee){
// uwwtd_trace(__FUNCTION__);
   //on prend toutes les agglos > 150 000 avec la colonne field_aggbigid rempli
    if (!isset($context['sandbox']['progress'])) {

        $context['sandbox']['progress'] = 0;
        $context['sandbox']['idagg'] = array();

       //get all agglo > 150 000
        $query = 'select n.nid, a.field_agggenerated_value
        from {node} n
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id   and n.vid = f_annee.revision_id
            left join
                {field_data_field_agggenerated} a
                    ON n.nid = a.entity_id   and n.vid = a.revision_id
            left join
                {field_data_field_aggbigid} b
                    ON n.nid = b.entity_id  and n.vid = b.revision_id
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
            and a.field_agggenerated_value::integer > :agggenerated
            and (b.field_aggbigid_value is null or b.field_aggbigid_value = \'\')
        order by n.title
        ';

        $param = array(
            ':typenode' => 'agglomeration',
            ':annee' => $annee,
            ':agggenerated' => 150000,
            );

        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['idagg'][] = $row['nid'];
            }
            unset($result, $row);
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
        $context['sandbox']['max'] = count($context['sandbox']['idagg']);
        $context['sandbox']['progressagg'] = 0;



    }

    $ind = $GLOBALS['uwwtd']['sheet8_summary_big_cities'];
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name'], $annee);

    if ($context['sandbox']['progress'] == 0) {
        $r = fopen($filename, 'wb');

        $header = array(
//             $ind['big_dischargers'] => 'big_dischargers',
            $ind['id'] => 'ID',
            $ind['name'] => 'name',
            $ind['generated_load'] => 'generated load p.e.',
            $ind['collecting_system'] => 'collecting system % of generated load',
            $ind['collecting_system_pe'] => 'collecting system % of generated load pe',
            $ind['ias'] => 'IAS % of generated load',
            $ind['ias_pe'] => 'IAS % of generated load pe',
            $ind['no_collecting_system_nor_ias'] => 'no collecting system nor IAS % of generated load',
            $ind['no_collecting_system_nor_ias_pe'] => 'no collecting system nor IAS % of generated load pe',
            $ind['treatment_0'] => 'best treatment provided 0',
            $ind['treatment_1'] => 'best treatment provided 1',
            $ind['treatment_2'] => 'best treatment provided 2',
            $ind['treatment_3n_p_np'] => 'best treatment provided 3N',
            $ind['treatment_3_other_(e.g._uv,_filtration)'] => 'best treatment provided  3 other (e.g. UV, filtration)',
            $ind['type_of_receiving_area'] => 'Type of receiving area',
            $ind['id_of_sensitive_area'] => 'ID of Sensitive Area',
            $ind['name_of_sensitive_area'] => 'Name of Sensitive Area',
            $ind['list_code_agglo'] => 'list_name_agglo',
            $ind['list_name_agglo'] => 'list_name_agglo',
        );

        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');
    }



    while ($context['sandbox']['progressagg'] < $context['sandbox']['max']) {
        $line = array();
        
        if($agg = node_load($context['sandbox']['idagg'][$context['sandbox']['progressagg']])){
// uwwtd_trace('call uwwtd_summary_big_cities_get_agglo_detail');
            $agg_detail = uwwtd_summary_big_cities_get_agglo_detail($annee, $agg);

            $generated_load = 0;
            $collecting_system = 0;
            $ias = 0;
            $no_collecting_system_nor_ias = 0;
            $treatment_0 = 0;
            $treatment_1 = 0;
            $treatment_2 = 0;
            $treatment_3np_3n_3p = 0;
			$treatment_3other = 0;
            $treat_3np_3n_3p_pe = 0;
			$treat_3other_pe = 0;
            $treat_3np_3n_3p_agggenerated = 0;
			$treat_3other_agggenerated = 0;
            $type_of_receiving_area = '';

            
// uwwtd_trace(var_export($agg_detail, true));
            $treatment_0 = $agg_detail['treat_0'];
            $treatment_1 = $agg_detail['treat_1'];
            $treatment_2 = $agg_detail['treat_2'];
            $treatment_3np_3n_3p = $agg_detail['treat_3np_3n_3p'];
			$treatment_3other = $agg_detail['treat_3other'];

            $generated_load = $agg_detail['generated_load'];
            $collecting_system =  $agg_detail['collecting_system'];
            $ias =  $agg_detail['ias'];
            $no_collecting_system_nor_ias =  $agg_detail['no_collecting_system_nor_ias'];

            $collecting_system_percent =  $collecting_system * 100 / $generated_load;
            $ias_percent =  $ias * 100 / $generated_load;
            $no_collecting_system_nor_ias_percent =  $no_collecting_system_nor_ias * 100 / $generated_load;

//             $line[$ind['big_dischargers']] = 'yes';
            $line[$ind['id']] = '';
            $line[$ind['name']] = '';
            $line[$ind['generated_load']] = $generated_load;
            $line[$ind['collecting_system']] = $collecting_system_percent;
            $line[$ind['collecting_system_pe']] = $collecting_system;
            $line[$ind['ias']] = $ias_percent;
            $line[$ind['ias_pe']] = $ias;
            $line[$ind['no_collecting_system_nor_ias']] = $no_collecting_system_nor_ias_percent;
            $line[$ind['no_collecting_system_nor_ias_pe']] = $no_collecting_system_nor_ias;
            $line[$ind['treatment_0']] = $treatment_0;
            $line[$ind['treatment_1']] = $treatment_1;
            $line[$ind['treatment_2']] = $treatment_2;

            $line[$ind['treatment_3n_p_np']] = $treatment_3np_3n_3p;
            $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = $treatment_3other;
            $line[$ind['type_of_receiving_area']] = implode(';', $agg_detail['dcptype']);

            $line[$ind['id_of_sensitive_area']] = implode(';', $agg_detail['rcaCode']);
            $line[$ind['name_of_sensitive_area']] = implode(';', $agg_detail['rcaName']);


            $line[$ind['list_code_agglo']] = $agg->field_inspireidlocalid['und'][0]['value'];
            $line[$ind['list_name_agglo']] = $agg->title;

// uwwtd_trace(var_export($line, true));
            
            // SAVING FOR EU PORTION
            // Search for this string to find all the fields
    
//             $agg->field_agg_bc_notreatment['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_0']]);
//             $agg->field_agg_bc_primary['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_1']]);
//             $agg->field_agg_bc_secondary['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_2']]);
//             $agg->field_agg_bc_morestringet_np['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_3n_p_np']]);
//             $agg->field_agg_bc_morestringet_other['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_3_other_(e.g._uv,_filtration)']]);
// 
//             node_save($agg);
    
            unset($agg);
            fputcsv($r, $line, ';');
        }
        $context['sandbox']['progressagg']++;
    }


    fclose($r);

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}



/**
 * Generate the "Big Cities" Sheet
 * since the 9th report (2016), BigCitiesDischarger node was suppress so we have to use "bigcitiesid" field in agglomeration node 
 * note : a big citie is an agglo with field_agggenerated_value > 150 000. A Big city can only have one agglomeration
 */
function uwwtd_create_register_2015_summary_big_cities_sheet8($annee){
    if (!isset($context['sandbox']['progress'])) {

        $context['sandbox']['progress'] = 0;
        $context['sandbox']['progressagg'] = 0;
        $context['sandbox']['idagg'] = array();

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
        order by a.field_agggenerated_value desc
        ';

        $param = array(
            ':typenode' => 'agglomeration',
            ':annee' => $annee,
            ':agggenerated' => 150000,
            );

        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['idagg'][] = $row['nid'];
            }
            unset($result, $row);
        }catch(Exception $e) {
            dsm($e->getMessage());
        }
        $context['sandbox']['max'] = count($context['sandbox']['idagg']);
        $context['sandbox']['progressagg'] = 0;
    }

    $ind = $GLOBALS['uwwtd']['sheet8_summary_big_cities'];
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
            $ind['treatment_3_other_(e.g._uv,_filtration)'] => 'best treatment provided  3 other (e.g. UV, filtration)',
            $ind['type_of_receiving_area'] => 'Type of receiving area',
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');
    }



    while ($context['sandbox']['progressagg'] < $context['sandbox']['max']) {
        $line = array();
        
        if($agg = node_load($context['sandbox']['idagg'][$context['sandbox']['progressagg']])){
            $agg_detail = uwwtd_summary_big_cities_get_agglo_detail($annee, $agg);

            $generated_load = 0;
            $collecting_system = 0;
            $ias = 0;
            $no_collecting_system_nor_ias = 0;
            $treatment_0 = 0;
            $treatment_1 = 0;
            $treatment_2 = 0;
            $treatment_3np_3n_3p = 0;
			$treatment_3other = 0;
            $treat_3np_3n_3p_pe = 0;
			$treat_3other_pe = 0;
            $treat_3np_3n_3p_agggenerated = 0;
			$treat_3other_agggenerated = 0;
            $type_of_receiving_area = '';

            

            $treatment_0 = $agg_detail['treat_0'];
            $treatment_1 = $agg_detail['treat_1'];
            $treatment_2 = $agg_detail['treat_2'];
            $treatment_3np_3n_3p = $agg_detail['treat_3np_3n_3p'];
			$treatment_3other = $agg_detail['treat_3other'];

            $generated_load = $agg_detail['generated_load'];
            $collecting_system =  $agg_detail['collecting_system'];
            $ias =  $agg_detail['ias'];
            $no_collecting_system_nor_ias =  $agg_detail['no_collecting_system_nor_ias'];

            $collecting_system_percent =  $collecting_system * 100 / $generated_load;
            $ias_percent =  $ias * 100 / $generated_load;
            $no_collecting_system_nor_ias_percent =  $no_collecting_system_nor_ias * 100 / $generated_load;

//             $line[$ind['big_dischargers']] = '';
            $line[$ind['id']] = $agg->field_inspireidlocalid['und'][0]['value'];
            $line[$ind['name']] = $agg->title;
            $line[$ind['generated_load']] = $generated_load;
            $line[$ind['collecting_system']] = $collecting_system_percent;
            $line[$ind['ias']] = $ias_percent;
            $line[$ind['no_collecting_system_nor_ias']] = $no_collecting_system_nor_ias_percent;
            $line[$ind['treatment_0']] = $treatment_0;
            $line[$ind['treatment_1']] = $treatment_1;
            $line[$ind['treatment_2']] = $treatment_2;

            $line[$ind['treatment_3n_p_np']] = $treatment_3np_3n_3p;
            $line[$ind['treatment_3_other_(e.g._uv,_filtration)']] = $treatment_3other;
            $line[$ind['type_of_receiving_area']] = implode(';', $agg_detail['dcptype']);
            
            // SAVING FOR EU PORTION
            // Search for this string to find all the fields
    
            $agg->field_agg_bc_notreatment['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_0']]);
            $agg->field_agg_bc_primary['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_1']]);
            $agg->field_agg_bc_secondary['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_2']]);
            $agg->field_agg_bc_morestringet_np['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_3n_p_np']]);
            $agg->field_agg_bc_morestringet_other['und'][0]['value'] = uwwtd_format_decimal($line[$ind['treatment_3_other_(e.g._uv,_filtration)']]);

            node_save($agg);
    
            unset($agg);
            fputcsv($r, $line, ';');
        }
        $context['sandbox']['progressagg']++;
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
    $result['dcptype'] = array();
    $result['rcaCode'] = array();
    $result['rcaName'] = array();
//     $agg = node_load($nidagglo);
//     dsm($agg);
    $result['generated_load'] = $agg->field_agggenerated['und'][0]['value'];
    $result['collecting_system'] = $agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
    $result['field_aggc1'] = $agg->field_aggc1['und'][0]['value'];
    $result['ias'] = $agg->field_aggc2['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
    $result['field_aggc2'] = $agg->field_aggc2['und'][0]['value'];
    $result['no_collecting_system_nor_ias'] = $agg->field_aggpercwithouttreatment['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
    $result['field_aggpercwithouttreatment'] = $agg->field_aggpercwithouttreatment['und'][0]['value'];

    $result['treat_0'] = 0;
    $result['treat_1'] = 0;
    $result['treat_2'] = 0;
    $result['treat_3np_3n_3p'] = 0;
	$result['treat_3other'] = 0;

    $result['treat_0_pe'] = 0;
    $result['treat_1_pe'] = 0;
    $result['treat_2_pe'] = 0;
    $result['treat_3np_3n_3p_pe'] = 0;
	$result['treat_3other_pe'] = 0;

    $result['treat_0_agggenerated'] = 0;
    $result['treat_1_agggenerated'] = 0;
    $result['treat_2_agggenerated'] = 0;
    $result['treat_3np_3n_3p_agggenerated'] = 0;
	$result['treat_3other_agggenerated'] = 0;


   //for rca we need to load uwwtp -> dcp -> rca
   foreach($agg->field_agguwwliste['und'] as $uwwtpnid) {
        $uwwtp = node_load($uwwtpnid['nid']);
        $uwwtp_treat_inplace = uwwtd_get_register_treatmenttype_inplace($uwwtp);

        $percent_generated_load = 0;
        //load agglo_uuwtp node
        $agglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $annee, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));

        if ('' !== $agglo_uwwtpnid) {
            $agglo_uwwtp = node_load($agglo_uwwtpnid);
//             $entering_load_pe = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * ($agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']);
            $entering_load_pe = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
//             $percent_of_generating_load = $entering_load_pe * 100 / $agg->field_agggenerated['und'][0]['value'];

            if (false !== strpos($uwwtp_treat_inplace, '3other') || false !== strpos($uwwtp_treat_inplace, '3NP')
                || false !== strpos($uwwtp_treat_inplace, '3P') || false !== strpos($uwwtp_treat_inplace, '3N')) {
				if(false !== strpos($uwwtp_treat_inplace, '3NP')
                   || false !== strpos($uwwtp_treat_inplace, '3P')
			       || false !== strpos($uwwtp_treat_inplace, '3N')){
                   $result['treat_3np_3n_3p_pe'] += $entering_load_pe;
                   $result['treat_3np_3n_3p_agggenerated'] = $agg->field_agggenerated['und'][0]['value'];
				}
				if(false !== strpos($uwwtp_treat_inplace, '3other')){
				   $result['treat_3other_pe'] += $entering_load_pe;
                   $result['treat_3other_agggenerated'] = $agg->field_agggenerated['und'][0]['value'];
				}
            } elseif (false !== strpos($uwwtp_treat_inplace, '2')) {
               $result['treat_2_pe'] += $entering_load_pe;
               $result['treat_2_agggenerated']    = $agg->field_agggenerated['und'][0]['value'];
            } elseif (false !== strpos($uwwtp_treat_inplace, '1')) {
               $result['treat_1_pe'] += $entering_load_pe;
               $result['treat_1_agggenerated']    = $agg->field_agggenerated['und'][0]['value'];
            } else {
               $result['treat_0_pe'] += $entering_load_pe;
               $result['treat_0_agggenerated']    = $agg->field_agggenerated['und'][0]['value'];
            }

            unset($agglo_uwwtp);
        }

        foreach($uwwtp->field_uwwdcpliste['und'] as $dcpnid) {
            $dcp = node_load($dcpnid['nid']);
            $result['dcptype'][] = $dcp->field_rcatype['und'][0]['value'];

            if(isset($dcp->field_dcprcaliste['und'])){
              foreach($dcp->field_dcprcaliste['und'] as $rcanid) {
                  $rca = node_load($rcanid['nid']);
                  if (isset($result['rcaCode'][$rca->field_inspireidlocalid['und'][0]['value']])) {
                      continue;
                  }
              
                  $result['rcaCode'][] = $rca->field_inspireidlocalid['und'][0]['value'];
                  $result['rcaName'][] = $rca->title;
              }
            }
            unset($dcp);
        }
        unset($uwwtp);
    }
    unset($agg);

    //control divided by 0 -> NAN
    if ($result['treat_3np_3n_3p_agggenerated'] > 0) {
      $result['treat_3np_3n_3p'] = $result['treat_3np_3n_3p_pe'] * 100 / $result['treat_3np_3n_3p_agggenerated'];
    }
    if ($result['treat_3other_agggenerated'] > 0) {
	  $result['treat_3other'] = $result['treat_3other_pe'] * 100 / $result['treat_3other_agggenerated'];           
    }
    if ($result['treat_2_agggenerated'] > 0) {
      $result['treat_2'] = $result['treat_2_pe'] * 100 / $result['treat_2_agggenerated'];
    }
    if ($result['treat_1_agggenerated'] > 0) {
      $result['treat_1'] = $result['treat_1_pe'] * 100 / $result['treat_1_agggenerated'];
    }
    if ($result['treat_0_agggenerated'] > 0) {
      $result['treat_0'] = $result['treat_0_pe'] * 100 / $result['treat_0_agggenerated'];
    }
// uwwtd_trace(__LINE__);
// uwwtd_trace(var_export($result, true));
    return $result;
}

/**
 *
 */
function uwwtd_create_register_2015_nuts_level2_analyse_sheet9($annee, &$context)
{
    //get all agglo
    //group by nuts2 code
	$indagglo = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
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
            $context['sandbox']['nuts2'][$nuts2][$ind['ias_pe']] = 0;
            $context['sandbox']['nuts2'][$nuts2][$ind['ias_percent']] = 0;

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
        $context['sandbox']['nuts2'][$nuts2][$ind['ias_pe']]      += $agglo[$indagglo['addressed_through_ias_p._e.']];
        $context['sandbox']['nuts2'][$nuts2][$ind['ias_percent']] += $agglo[$indagglo['addressed_through_ias_%_of_gen._load']];
        /*
            'neither_collecting_system_nor_ias_%_of_gen._load' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round1'),
            'neither_collecting_system_nor_ias_p._e.' => array('k'=>$i++, 'xls'=>true, 'wrapper'=>'round'),
        */

        if ($agglo[$indagglo['art3_compliance']] != 'NR' && $agglo[$indagglo['art3_compliance']] != 'PD' ) {
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_target_load']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        //dsm($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']]);
        if ($agglo[$indagglo['art3_compliance']] == 'C' || $agglo[$indagglo['art3_compliance']] == 'QC') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_compliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_compliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['art3_compliance']] == 'NC') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notcompliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notcompliant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
        if ($agglo[$indagglo['art3_compliance']] == 'NR' || $agglo[$indagglo['art3_compliance']] == 'PD') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notrelevant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art3_notrelevant_pe']] += $agglo[$indagglo['generated_load_p.e.']];
        }
//         $context['sandbox']['nuts2'][$nuts2][$ind['art3_rate']]= 'TODO';//+=

        //art4
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] != 'NR' && 
            $agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] != 'PD') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_target_load']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        }
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_compliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_compliant_pe']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        }
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] == 'NC') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notcompliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notcompliant_pe']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        }
        if ($agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] == 'NR' ||
            $agglo[$indagglo['art4_compliance_art._4_(hierarchical_approach)']] == 'PD') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notrelevant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art4_notrelevant_pe']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        }
//         $context['sandbox']['nuts2'][$nuts2][$ind['art4_rate']]= 'TODO';//+=

        //art5
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] != 'NR' &&
            $agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] != 'PD') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_target_load']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        }
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_compliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_compliant_pe']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        }
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notcompliant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notcompliant_pe']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
        }
        if ($agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NR' ||
            $agglo[$indagglo['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'PD') {
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notrelevant_number_agglo']]++;
            $context['sandbox']['nuts2'][$nuts2][$ind['art5_notrelevant_pe']] += $agglo[$indagglo['connected_to_collecting_system_p._e.']];
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
            $ind['ias_pe'] => 'load addressed through IAS pe',
            $ind['ias_percent'] => 'ias target load',

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
            if (!empty($nuts2[$ind['art3_compliant_pe']]) && !empty($nuts2[$ind['art3_target_load']])) {
                $nuts2[$ind['art3_rate']] = $nuts2[$ind['art3_compliant_pe']] * 100 / $nuts2[$ind['art3_target_load']];
            }
            if (!empty($nuts2[$ind['art4_compliant_pe']]) && !empty($nuts2[$ind['art4_target_load']])) {
                $nuts2[$ind['art4_rate']] = $nuts2[$ind['art4_compliant_pe']] * 100 / $nuts2[$ind['art4_target_load']];
            }
            if (!empty($nuts2[$ind['art5_compliant_pe']]) && !empty($nuts2[$ind['art5_target_load']])) {
                $nuts2[$ind['art5_rate']] = $nuts2[$ind['art5_compliant_pe']] * 100 / $nuts2[$ind['art5_target_load']];
            }

            $line = array(
                $nuts2[$ind['nuts2_code']],
                $nuts2[$ind['number_agglo']],
                $nuts2[$ind['generated_load_pe']],
                $nuts2[$ind['collected_pe']],
                $nuts2[$ind['collected_percent']],
                $nuts2[$ind['ias_pe']],
                $nuts2[$ind['ias_percent']],

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

        //we get the distance to compliance info by agg from csv file
        $filename_aggdistance = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_distance_name'], $annee);
        $raggdistance = fopen($filename_aggdistance, 'rb');
        fgetcsv($raggdistance, 0, ';');
        while ($line = fgetcsv($raggdistance, 0, ';')) {
            $context['sandbox']['dist'][$line[0]] = $line;
        }
        fclose($raggdistance);        
    }

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

    $model_path = uwwtd_getregister_modelfile_path($annee);
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_name'], $annee);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));

    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['summary_big_cities']['xlsx_model_sheet_name']);

    $ind = $GLOBALS['uwwtd']['sheet8_summary_big_cities'];

//     $path = $GLOBALS['uwwtd']['sheet']['path_created_file'];
//     $filename = $path . $annee . $GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name']. '.csv';
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['summary_big_cities']['csv_name'], $annee);

    $rbcfile = fopen($filename, 'rb');

    $start_line = 5;   //4 -> 5
    //ignore header //decomment for debug on xls final file
    $bc = fgetcsv($rbcfile, 5000, ';');
    $i = $start_line;
    $total = [
        'generated_load' => 0,
        'collecting_system_pe' => 0,
        'ias_pe' => 0,
        'no_collecting_system_nor_ias_pe' => 0,
        'target_connection_distance_pe' => 0,
        'target_connection_total_pe' => 0,
        'secondary_distance_pe' => 0,
        'secondary_total_pe' => 0,
        'morestringent_distance_pe' => 0,
        'morestringent_total_pe' => 0
    ];
    while ($bc = fgetcsv($rbcfile, 5000, ';')) {
//         dsm($bc);
//         dsm($ind);
//         dsm($bc[$ind['id']]);
           
//         $sheet->setCellValueByColumnAndRow($ind['big_dischargers'], $i, $bc[$ind['big_dischargers']]);
        $sheet->setCellValueByColumnAndRow($ind['id'], $i, $bc[$ind['id']]);
//         $sheet->getCell('B'.$i)->setValue($bc[$ind['id']]);
//         $sheet->getCell('A1')->setValue($bc[$ind['id']]);
//         $sheet->getCell('A4')->setValue($bc[$ind['id']]);
//         $sheet->getCell('B4')->setValue($bc[$ind['id']]);
//         dsm('B'.$i);

        $sheet->setCellValueByColumnAndRow($ind['name'], $i, $bc[$ind['name']]);
//         $sheet->setCellValueByColumnAndRow($ind['name'], $i, $bc[$ind['id']]);
        $sheet->setCellValueByColumnAndRow($ind['generated_load'], $i, $bc[$ind['generated_load']]);
            $total['generated_load'] +=$bc[$ind['generated_load']];
        $sheet->setCellValueByColumnAndRow($ind['collecting_system'], $i, $bc[$ind['collecting_system']]);
        $sheet->setCellValueByColumnAndRow($ind['collecting_system_pe'], $i, $bc[$ind['collecting_system_pe']]);
            $total['collecting_system_pe'] +=$bc[$ind['collecting_system_pe']];
        $sheet->setCellValueByColumnAndRow($ind['ias'], $i, $bc[$ind['ias']]);
        $sheet->setCellValueByColumnAndRow($ind['ias_pe'], $i, $bc[$ind['ias_pe']]);
            $total['ias_pe'] +=$bc[$ind['ias_pe']];
        $sheet->setCellValueByColumnAndRow($ind['no_collecting_system_nor_ias'], $i, $bc[$ind['no_collecting_system_nor_ias']]);
        $sheet->setCellValueByColumnAndRow($ind['no_collecting_system_nor_ias_pe'], $i, $bc[$ind['no_collecting_system_nor_ias_pe']]);
            $total['no_collecting_system_nor_ias_pe'] +=$bc[$ind['no_collecting_system_nor_ias_pe']];
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

        if ($annee >= 2016) {
          $sheet->setCellValueByColumnAndRow($ind['id_of_sensitive_area'], $i, $bc[$ind['id_of_sensitive_area']]);
          $sheet->setCellValueByColumnAndRow($ind['name_of_sensitive_area'], $i, $bc[$ind['name_of_sensitive_area']]);
        }

        $sheet->setCellValueByColumnAndRow($ind['list_code_agglo'], $i, $bc[$ind['list_code_agglo']]);
        $sheet->setCellValueByColumnAndRow($ind['list_name_agglo'], $i, $bc[$ind['list_name_agglo']]);

        //Complete with the distance to compliance info
        $target_connection_distance_pe = 0;
        $target_connection_distance_percent = 0;
        $target_connection_total_pe = 0;

        $secondary_distance_pe = 0;
        $secondary_distance_percent = 0;
        $secondary_total_pe = 0;

        $morestringent_distance_pe = 0;
        $morestringent_distance_percent = 0;
        $morestringent_total_pe = 0;

    //     ['nuts2code', 
    //     'target_connection_total_load_pe',
    //     'target_connection_distance_pe', 
    //     'target_connection_distance_%', 
    //     'secondary_target_total_load_pe',
    //     'secondary_target_monitoring_result_distance_pe', 
    //     'secondary_target_monitoring_result_distance_%',
    //     'morestringent_total_load_pe',
    //     'morestringent_target_monitoring_result_distance_pe', 
    //     'morestringent_target_monitoring_result_distance_%', 
    //   ],

        foreach (explode(';', $bc[$ind['list_code_agglo']]) as $codeagglo) {
            if (isset($context['sandbox']['dist'][$codeagglo])) {
                $target_connection_total_pe += $context['sandbox']['dist'][$codeagglo][1];
                $secondary_total_pe += $context['sandbox']['dist'][$codeagglo][4];
                $morestringent_total_pe += $context['sandbox']['dist'][$codeagglo][7];

                $target_connection_distance_pe += $context['sandbox']['dist'][$codeagglo][2];
                $secondary_distance_pe += $context['sandbox']['dist'][$codeagglo][5];
                $morestringent_distance_pe += $context['sandbox']['dist'][$codeagglo][8]; 
            } 
        }
        if ($target_connection_total_pe > 0) {
            $target_connection_distance_percent = round(100 * $target_connection_distance_pe / $target_connection_total_pe, 1);
            $total['target_connection_distance_pe'] +=$target_connection_distance_pe;
            $total['target_connection_total_pe'] +=$target_connection_total_pe;
        }
        if ($secondary_total_pe > 0) {
            $secondary_distance_percent = round(100 * $secondary_distance_pe / $secondary_total_pe, 1);
            $total['secondary_target_monitoring_result_distance_pe'] +=$secondary_distance_pe;
            $total['secondary_total_pe'] +=$secondary_total_pe;
        }
        if ($morestringent_total_pe > 0) {
            $morestringent_distance_percent = round(100 * $morestringent_distance_pe / $morestringent_total_pe, 1);
            $total['morestringent_target_monitoring_result_distance_pe'] +=$morestringent_distance_pe;
            $total['morestringent_total_pe'] +=$morestringent_total_pe;
        }
        if ($annee >= 2018) {$sheet->setCellValueByColumnAndRow($ind['target_connection_distance_pe'], $i, $target_connection_distance_pe);}
        $sheet->setCellValueByColumnAndRow($ind['target_connection_distance_%'], $i, $target_connection_distance_percent);
        if ($annee >= 2018) {$sheet->setCellValueByColumnAndRow($ind['secondary_target_monitoring_result_distance_pe'], $i, $secondary_distance_pe);}
        $sheet->setCellValueByColumnAndRow($ind['secondary_target_monitoring_result_distance_%'], $i, $secondary_distance_percent);
        if ($annee >= 2018) {$sheet->setCellValueByColumnAndRow($ind['morestringent_target_monitoring_result_distance_pe'], $i, $morestringent_distance_pe);}
        $sheet->setCellValueByColumnAndRow($ind['morestringent_target_monitoring_result_distance_%'], $i, $morestringent_distance_percent);

        $i++;
        $sheet->insertNewRowBefore($i, 1);
    }
    $i++;
    $i++;
    if ($total['morestringent_total_pe'] > 0) {
        $total['morestringent_target_monitoring_result_distance_%'] = round(100 * $total['morestringent_target_monitoring_result_distance_pe'] /$total['morestringent_total_pe'], 1);
    }
    if ($total['secondary_total_pe'] > 0) {
        $total['secondary_target_monitoring_result_distance_%'] = round(100 * $total['secondary_target_monitoring_result_distance_pe'] /$total['secondary_total_pe'], 1);
    }
    if ($total['target_connection_total_pe'] > 0) {
        $total['target_connection_distance_%'] = round(100 * $total['target_connection_distance_pe'] /$total['target_connection_total_pe'], 1);
    }
    //Insert total
    if ($annee >= 2018) {
        foreach($total as $k=>$v){
            $sheet->setCellValueByColumnAndRow($ind[$k], $i, $v);
        }
    }
    
    
    fclose($rbcfile);
    unset($context['sandbox']['dist']);


    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
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
        $context['sandbox']['line'] = 5; //start line   4 -> 5

        $path_xlsx = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['nuts_level2']['xlsx_name'], $annee);

        //we get the distance to compliance info by agg from csv file
        $filename_nuts2distance = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['nuts_level2']['csv_distance_name'], $annee);
        $rnuts2distance = fopen($filename_nuts2distance, 'rb');
        fgetcsv($rnuts2distance, 0, ';');
        while ($line = fgetcsv($rnuts2distance, 0, ';')) {
            $context['sandbox']['dist'][$line[0]] = $line;
        }
        fclose($rnuts2distance);   
        
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
        //$sheet->setCellValueByColumnAndRow($ind['collected_percent'] , $context['sandbox']['line'], $nuts[$ind['collected_percent']]);
        $sheet->setCellValueByColumnAndRow($ind['collected_percent'] , $context['sandbox']['line'], $nuts[$ind['collected_pe']]/$nuts[$ind['generated_load_pe'] ]*100 );
        $sheet->setCellValueByColumnAndRow($ind['ias_pe'] , $context['sandbox']['line'], $nuts[$ind['ias_pe'] ]);
        $sheet->setCellValueByColumnAndRow($ind['ias_percent'] , $context['sandbox']['line'], $nuts[$ind['ias_pe']]/$nuts[$ind['generated_load_pe'] ]*100 );
        
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

        //Complete with the distaance to compliance info
 
    //     ['nuts2code', 
    //     'target_connection_total_load_pe',
    //     'target_connection_distance_pe', 
    //     'target_connection_distance_%', 
    //     'secondary_target_total_load_pe',
    //     'secondary_target_monitoring_result_distance_pe', 
    //     'secondary_target_monitoring_result_distance_%',
    //     'morestringent_total_load_pe',
    //     'morestringent_target_monitoring_result_distance_pe', 
    //     'morestringent_target_monitoring_result_distance_%', 
    //   ],

        if (isset($context['sandbox']['dist'][$nuts[$ind['nuts2_code'] ]])) {
            $sheet->setCellValueByColumnAndRow($ind['target_connection_distance_%'], $context['sandbox']['line'], round($context['sandbox']['dist'][$nuts[$ind['nuts2_code'] ]][3], 1));
            $sheet->setCellValueByColumnAndRow($ind['secondary_target_monitoring_result_distance_%'], $context['sandbox']['line'], round($context['sandbox']['dist'][$nuts[$ind['nuts2_code'] ]][6], 1));
            $sheet->setCellValueByColumnAndRow($ind['morestringent_target_monitoring_result_distance_%'], $context['sandbox']['line'], round($context['sandbox']['dist'][$nuts[$ind['nuts2_code'] ]][9], 1));             
        } 

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
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
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

    $model_path = uwwtd_getregister_modelfile_path($annee);
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
        // $field_msldischargepipelines = isset($node->field_msldischargepipelines['und'][0]['value']) ? $node->field_msldischargepipelines['und'][0]['value'] : '';
        //Of which discharged into surface waters: Ships
        // $field_msldischargeships = isset($node->field_msldischargeships['und'][0]['value']) ? $node->field_msldischargeships['und'][0]['value'] : '';
        //Of which discharged into surface waters: Others
        // $field_msldischargeothers = isset($node->field_msldischargeothers['und'][0]['value']) ? $node->field_msldischargeothers['und'][0]['value'] : '';
        //Of which: re-used: Soil and agriculture
        $field_mslreusesoilagriculture = isset($node->field_mslreusesoilagriculture['und'][0]['value']) ? $node->field_mslreusesoilagriculture['und'][0]['value'] : '';
        if ($field_mslreusesoilagriculture && $field_mslsludgeproduction) {
           $field_mslreusesoilagriculture_percent = $field_mslreusesoilagriculture * 100 / $field_mslsludgeproduction;
        }
        //Of which: re-used: Others
        $field_mslreuseothers = isset($node->field_mslreuseothers['und'][0]['value']) ? $node->field_mslreuseothers['und'][0]['value'] : '';
        if ($field_mslreuseothers && $field_mslsludgeproduction) {
           $field_mslreuseothers_percent = $field_mslreuseothers * 100 / $field_mslsludgeproduction;
        }
        
        //Of which: disposed: Landfill
        $field_msldisposallandfill = isset($node->field_msldisposallandfill['und'][0]['value']) ? $node->field_msldisposallandfill['und'][0]['value'] : '';
        if ($field_msldisposallandfill && $field_mslsludgeproduction) {
           $field_msldisposallandfill_percent = $field_msldisposallandfill * 100 / $field_mslsludgeproduction;
        }
        //Of which: disposed: Incineration
        $field_msldisposalincineration = isset($node->field_msldisposalincineration['und'][0]['value']) ? $node->field_msldisposalincineration['und'][0]['value'] : '';
        if ($field_msldisposalincineration && $field_mslsludgeproduction) {
           $field_msldisposalincineration_percent = $field_msldisposalincineration * 100 / $field_mslsludgeproduction;
        }
        //Of which: disposed: Others
        $field_msldisposalothers = isset($node->field_msldisposalothers['und'][0]['value']) ? $node->field_msldisposalothers['und'][0]['value'] : '';
        if ($field_msldisposalothers && $field_mslsludgeproduction) {
           $field_msldisposalothers_percent = $field_msldisposalothers * 100 / $field_mslsludgeproduction;
        }
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
        $sheet->getCell('D4')->setValue(100);
        // $sheet->getCell('C5')->setValue($field_msldischargepipelines);
        // $sheet->getCell('C6')->setValue($field_msldischargeships);
        // $sheet->getCell('C7')->setValue($field_msldischargeothers);
        $sheet->getCell('C5')->setValue($field_mslreusesoilagriculture);//8 ->5
        $sheet->getCell('D5')->setValue($field_mslreusesoilagriculture_percent);

        $sheet->getCell('C6')->setValue($field_mslreuseothers);
        $sheet->getCell('D6')->setValue($field_mslreuseothers_percent);

        $sheet->getCell('C7')->setValue($field_msldisposallandfill);
        $sheet->getCell('D7')->setValue($field_msldisposallandfill_percent);

        $sheet->getCell('C8')->setValue($field_msldisposalincineration);
        $sheet->getCell('D8')->setValue($field_msldisposalincineration_percent);

        $sheet->getCell('C9')->setValue($field_msldisposalothers);
        $sheet->getCell('D9')->setValue($field_msldisposalothers_percent);

        $sheet->getCell('C10')->setValue($field_mslremarks);//13 >10

        $sheet->getCell('C18')->setValue($volume_treated);//21 > 18
        $sheet->getCell('C19')->setValue($reuse);
        $sheet->getCell('C20')->setValue($reuse_agriculture);
        $sheet->getCell('C21')->setValue($reuse_industry);
        $sheet->getCell('C22')->setValue($reuse_others);
        $sheet->getCell('C23')->setValue($remarks);
    }


    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
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

    $model_path = uwwtd_getregister_modelfile_path($annee);
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
    //array1
// uwwtd_trace('Graph : context result = ');
// uwwtd_trace(var_export($context['results'], true));

    if (isset($context['results']['graph']['0_10000']['number'])) {
    	$sheet->setCellValue('B5', $context['results']['graph']['0_10000']['number']);
    }
    if (isset($context['results']['graph']['0_10000']['pe'])) {
    	$sheet->setCellValue('C5', round($context['results']['graph']['0_10000']['pe'], 0));
    }
    if (isset($context['results']['graph']['10000_100000']['number'])) {
    	$sheet->setCellValue('B6', $context['results']['graph']['10000_100000']['number']);
    }
    if (isset($context['results']['graph']['10000_100000']['pe'])) {
    	$sheet->setCellValue('C6', round($context['results']['graph']['10000_100000']['pe'], 0));
    }
    if (isset($context['results']['graph']['100000_n']['number'])) {
    	$sheet->setCellValue('B7', $context['results']['graph']['100000_n']['number']);
    }
    if (isset($context['results']['graph']['100000_n']['pe'])) {
	    $sheet->setCellValue('C7', round($context['results']['graph']['100000_n']['pe'], 0));
    } 

//debug to comment
// 951	4500183
// 500	14164306
// 71	19872061

// dsm('delete all debug value B5 ...');
// $sheet->setCellValue('B5', 951);
// $sheet->setCellValue('C5', 4500183);
// $sheet->setCellValue('B6', 500);
// $sheet->setCellValue('C6', 14164306);
// $sheet->setCellValue('B7', 71);
// $sheet->setCellValue('C7', 19872061);
//debug to comment


    //previous
    $sheet->setCellValue('D5', uwwtd_register_get_cell_value($previoussheet, 'B5'));
    $sheet->setCellValue('E5', uwwtd_register_get_cell_value($previoussheet, 'C5'));
    $sheet->setCellValue('D6', uwwtd_register_get_cell_value($previoussheet, 'B6'));
    $sheet->setCellValue('E6', uwwtd_register_get_cell_value($previoussheet, 'C6'));
    $sheet->setCellValue('D7', uwwtd_register_get_cell_value($previoussheet, 'B7'));
    $sheet->setCellValue('E7', uwwtd_register_get_cell_value($previoussheet, 'C7'));

    //array2
    if (isset($context['results']['graph']['total_generated']['pe'])) {
    	$sheet->setCellValue('B15', round($context['results']['graph']['total_generated']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_collected']['pe'])) {
    	$sheet->setCellValue('B16', round($context['results']['graph']['total_collected']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_load_ias']['pe'])) {
    	$sheet->setCellValue('B17', round($context['results']['graph']['total_load_ias']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_entering']['pe'])) {
    	$sheet->setCellValue('B18', round($context['results']['graph']['total_entering']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_capacity']['pe'])) {
    	$sheet->setCellValue('B19', round($context['results']['graph']['total_capacity']['pe'], 0));
    }

//debug to comment
// dsm('delete all debug value B15 ...');
// $sheet->setCellValue('B15', 38536550);
// $sheet->setCellValue('B16', 34946534);
// $sheet->setCellValue('B17', 3350373);
// $sheet->setCellValue('B18', 34990743);
// $sheet->setCellValue('B19', 49645180);
//debug to comment

    //previous
    $previousB15 = uwwtd_register_get_cell_value($previoussheet, 'B15');
    $sheet->setCellValue('C15', $previousB15);
    $sheet->setCellValue('C16', uwwtd_register_get_cell_value($previoussheet, 'B16'));
    $sheet->setCellValue('C17', uwwtd_register_get_cell_value($previoussheet, 'B17'));
    $sheet->setCellValue('C18', uwwtd_register_get_cell_value($previoussheet, 'B18'));
    $sheet->setCellValue('C19', uwwtd_register_get_cell_value($previoussheet, 'B19'));

         
    $context['results']['auto_report']['COUNTRY']  = $country_name;
    $context['results']['auto_report']['CURRENT_REPORT_YEAR']  = $annee;
    $context['results']['auto_report']['PREVIOUS_REPORT_YEAR']  = $previous_year;
    $context['results']['auto_report']['PREVIOUS_REPORT_NUMBER'] = $GLOBALS['report_number'][$previous_year];  
    $context['results']['auto_report']['CURRENT_REPORT_NUMBER'] = $GLOBALS['report_number'][$annee];
    $context['results']['auto_report']['TOTAL_GENERATED_LOAD'] = $context['results']['graph']['total_generated']['pe'];
    //Cell B15 �� cellule C15 auto generation report
    $context['results']['auto_report']['WASTER_WATER_LOAD_INCREASED'] = round($context['results']['graph']['total_generated']['pe'], 0)  -  $previousB15; 

    //array 5 6 7
//     $previous_year = uwwtd_get_previous_year($annee);
    $replaceyearby = $previous_year;
    if (false === $previous_year) {
        $replaceyearby = 'N/A';
    }

    $cell_to_repalce = array('B3','D3','AA51','AA52','AA54','AA55','AA57','AA58','AA65','AA66','AA68','AA69','AA71','AA72',
                           'AA41','AB41','AC41','AD41','AE41','AF41','AB48','AB62','B74','A54','A55','A57','A58','A60','A61',
                           'A64','A65','A67','A68','A70','A71','AA31','B21','C13','Z33','AD76','AA85','AA102','AA117','AA11',
                           'AA128', 'AF24', 'AF25',
                           );
    foreach ($cell_to_repalce as $x){
        $cell = $sheet->getCell($x);
        $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    }
    /*$cell = $sheet->getCell('A54');

    $years_cells = array('B3','D3','B21','B74','AB48','A62','AA41','AB41','AB62','AC41','AD41','AE41','AF41',
    'A54','A55','A57','A58','A60','A61','A64','A65','A67','A68','A70','A71','AA65','AA66','AA68','AA69','AA71','AA72');
    foreach($years_cells as $yc){
        $cell = $sheet->getCell($yc);
        $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    }
    /*
    $cell = $sheet->getCell('A54');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A55');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A57');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A58');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A60');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A61');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A64');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A65');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A67');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A68');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A70');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));
    $cell = $sheet->getCell('A71');
    $cell->setValue(str_replace(array('[#previous_year#]', '[#current_year#]'), array($replaceyearby, $annee), $cell->getValue()));*/

	if (isset($context['results']['graph']['art3_C']['number'])) {
    	$sheet->setCellValue('B43', $context['results']['graph']['art3_C']['number']);
	}
	if (isset($context['results']['graph']['art3_C']['pe'])) {
    	$sheet->setCellValue('C43', round($context['results']['graph']['art3_C']['pe']));
	}
	if (isset($context['results']['graph']['art3_NC']['number'])) {
    	$sheet->setCellValue('B44', $context['results']['graph']['art3_NC']['number']);
	}
	if (isset($context['results']['graph']['art3_NC']['pe'])) {
    	$sheet->setCellValue('C44', round($context['results']['graph']['art3_NC']['pe']));
	}
	if (isset($context['results']['graph']['art3_NR']['number'])) {
    	$sheet->setCellValue('B45', $context['results']['graph']['art3_NR']['number']);
	}
	if (isset($context['results']['graph']['art3_NR']['pe'])) {
    	$sheet->setCellValue('C45', round($context['results']['graph']['art3_NR']['pe']));
	}
	if (isset($context['results']['graph']['art4_C']['number'])) {
    	$sheet->setCellValue('B46', $context['results']['graph']['art4_C']['number']);
	}
	if (isset($context['results']['graph']['art4_C']['pe'])) {
    	$sheet->setCellValue('C46', round($context['results']['graph']['art4_C']['pe']));
	}
	if (isset($context['results']['graph']['art4_NC']['number'])) {
    	$sheet->setCellValue('B47', $context['results']['graph']['art4_NC']['number']);
	}
	if (isset($context['results']['graph']['art4_NC']['pe'])) {
    	$sheet->setCellValue('C47', round($context['results']['graph']['art4_NC']['pe']));
	}
	if (isset($context['results']['graph']['art4_NR']['number'])) {
    	$sheet->setCellValue('B48', $context['results']['graph']['art4_NR']['number']);
	}
	if (isset($context['results']['graph']['art4_NR']['pe'])) {
    	$sheet->setCellValue('C48', round($context['results']['graph']['art4_NR']['pe']));
	}
	if (isset($context['results']['graph']['art5_C']['number'])) {
    	$sheet->setCellValue('B49', $context['results']['graph']['art5_C']['number']);
	}
	if (isset($context['results']['graph']['art5_C']['pe'])) {
    	$sheet->setCellValue('C49', round($context['results']['graph']['art5_C']['pe']));
	}
	if (isset($context['results']['graph']['art5_NC']['number'])) {
    	$sheet->setCellValue('B50', $context['results']['graph']['art5_NC']['number']);
	}
	if (isset($context['results']['graph']['art5_NC']['pe'])) {
    	$sheet->setCellValue('C50', round($context['results']['graph']['art5_NC']['pe']));
	}
	if (isset($context['results']['graph']['art5_NR']['number'])) {
    	$sheet->setCellValue('B51', $context['results']['graph']['art5_NR']['number']);
	}
	if (isset($context['results']['graph']['art5_NR']['pe'])) {
    	$sheet->setCellValue('C51', round($context['results']['graph']['art5_NR']['pe']));
	}

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
    $sheet->setCellValue('D49', uwwtd_register_get_cell_value($previoussheet, 'B49'));
    $sheet->setCellValue('E49', uwwtd_register_get_cell_value($previoussheet, 'C49'));
    $sheet->setCellValue('D50', uwwtd_register_get_cell_value($previoussheet, 'B50'));
    $sheet->setCellValue('E50', uwwtd_register_get_cell_value($previoussheet, 'C50'));
    $sheet->setCellValue('D51', uwwtd_register_get_cell_value($previoussheet, 'B51'));
    $sheet->setCellValue('E51', uwwtd_register_get_cell_value($previoussheet, 'C51'));


    //array 6
    if (isset($context['results']['graph']['art3_C']['pe'])) {
    	$sheet->setCellValue('E55', round($context['results']['graph']['art3_C']['pe']));
    }
    if (isset($context['results']['graph']['art3_NC']['pe'])) {
    	$sheet->setCellValue('F55', round($context['results']['graph']['art3_NC']['pe']));
    }
    if (isset($context['results']['graph']['art3_NR']['pe'])) {
    	$sheet->setCellValue('G55', round($context['results']['graph']['art3_NR']['pe']));
    }
    if (isset($context['results']['graph']['art4_C']['pe'])) {
    	$sheet->setCellValue('E58', round($context['results']['graph']['art4_C']['pe']));
    }
    if (isset($context['results']['graph']['art4_NC']['pe'])) {
    	$sheet->setCellValue('F58', round($context['results']['graph']['art4_NC']['pe']));
    }
    if (isset($context['results']['graph']['art4_NR']['pe'])) {
    	$sheet->setCellValue('G58', round($context['results']['graph']['art4_NR']['pe']));
    }
    if (isset($context['results']['graph']['art5_C']['pe'])) {
    	$sheet->setCellValue('E61', round($context['results']['graph']['art5_C']['pe']));
    }
    if (isset($context['results']['graph']['art5_NC']['pe'])) {
    	$sheet->setCellValue('F61', round($context['results']['graph']['art5_NC']['pe']));
    }
    if (isset($context['results']['graph']['art5_NR']['pe'])) {
    	$sheet->setCellValue('G61', round($context['results']['graph']['art5_NR']['pe']));
    }

    //previous
    $sheet->setCellValue('B54', uwwtd_register_get_cell_value($previoussheet, 'E55'));
    $sheet->setCellValue('C54', uwwtd_register_get_cell_value($previoussheet, 'F55'));
    $sheet->setCellValue('D54', uwwtd_register_get_cell_value($previoussheet, 'G55'));
    $sheet->setCellValue('B57', uwwtd_register_get_cell_value($previoussheet, 'E58'));
    $sheet->setCellValue('C57', uwwtd_register_get_cell_value($previoussheet, 'F58'));
    $sheet->setCellValue('D57', uwwtd_register_get_cell_value($previoussheet, 'G58'));
    $sheet->setCellValue('B60', uwwtd_register_get_cell_value($previoussheet, 'E61'));
    $sheet->setCellValue('C60', uwwtd_register_get_cell_value($previoussheet, 'F61'));
    $sheet->setCellValue('D60', uwwtd_register_get_cell_value($previoussheet, 'G61'));


    //array 7
    if (isset($context['results']['graph']['art3_C']['number'])) {
    	$sheet->setCellValue('E65', $context['results']['graph']['art3_C']['number']);
    }
    if (isset($context['results']['graph']['art3_NC']['number'])) {
    	$sheet->setCellValue('F65', $context['results']['graph']['art3_NC']['number']);
	}
	if (isset($context['results']['graph']['art3_NR']['number'])) {
    	$sheet->setCellValue('G65', $context['results']['graph']['art3_NR']['number']);
	}
	if (isset($context['results']['graph']['art4_C']['number'])) {
	    $sheet->setCellValue('E68', $context['results']['graph']['art4_C']['number']);
	}
	if (isset($context['results']['graph']['art4_NC']['number'])) {
    	$sheet->setCellValue('F68', $context['results']['graph']['art4_NC']['number']);
	}
	if (isset($context['results']['graph']['art4_NR']['number'])) {
    	$sheet->setCellValue('G68', $context['results']['graph']['art4_NR']['number']);
	}
	if (isset($context['results']['graph']['art5_C']['number'])) {
    	$sheet->setCellValue('E71', $context['results']['graph']['art5_C']['number']);
	}
	if (isset($context['results']['graph']['art5_NC']['number'])) {
    	$sheet->setCellValue('F71', $context['results']['graph']['art5_NC']['number']);
	}
	if (isset($context['results']['graph']['art5_NR']['number'])) {
    	$sheet->setCellValue('G71', $context['results']['graph']['art5_NR']['number']);
	}

    //previous
    $sheet->setCellValue('B64', uwwtd_register_get_cell_value($previoussheet, 'E65'));
    $sheet->setCellValue('C64', uwwtd_register_get_cell_value($previoussheet, 'F65'));
    $sheet->setCellValue('D64', uwwtd_register_get_cell_value($previoussheet, 'G65'));
    $sheet->setCellValue('B67', uwwtd_register_get_cell_value($previoussheet, 'E68'));
    $sheet->setCellValue('C67', uwwtd_register_get_cell_value($previoussheet, 'F68'));
    $sheet->setCellValue('D67', uwwtd_register_get_cell_value($previoussheet, 'G68'));
    $sheet->setCellValue('B70', uwwtd_register_get_cell_value($previoussheet, 'E71'));
    $sheet->setCellValue('C70', uwwtd_register_get_cell_value($previoussheet, 'F71'));
    $sheet->setCellValue('D70', uwwtd_register_get_cell_value($previoussheet, 'G71'));


    //array 3
    $nidms = uwwtd_check_exist(uwwtd_get_siteid('ms_level', array('year' => $annee)));
    if ('' != $nidms) {
        $node = node_load($nidms);

        //Yearly production of sludge
        $field_mslsludgeproduction = isset($node->field_mslsludgeproduction['und'][0]['value']) ? $node->field_mslsludgeproduction['und'][0]['value'] : 0;
        // Discharged into surface waters : Pipelines
        $field_msldischargepipelines = isset($node->field_msldischargepipelines['und'][0]['value']) ? $node->field_msldischargepipelines['und'][0]['value'] : 0;
        // Discharged into surface waters : Ships
        $field_msldischargeships = isset($node->field_msldischargeships['und'][0]['value']) ? $node->field_msldischargeships['und'][0]['value'] : 0;
        // Discharged into surface waters : Others
        $field_msldischargeothers = isset($node->field_msldischargeothers['und'][0]['value']) ? $node->field_msldischargeothers['und'][0]['value'] : 0;
        //Of which: re-used: Soil and agriculture
        $field_mslreusesoilagriculture = isset($node->field_mslreusesoilagriculture['und'][0]['value']) ? $node->field_mslreusesoilagriculture['und'][0]['value'] : 0;
        //Of which: re-used: Others
        $field_mslreuseothers = isset($node->field_mslreuseothers['und'][0]['value']) ? $node->field_mslreuseothers['und'][0]['value'] : 0;
        //Of which: disposed: Landfill
        $field_msldisposallandfill = isset($node->field_msldisposallandfill['und'][0]['value']) ? $node->field_msldisposallandfill['und'][0]['value'] : 0;
        //Of which: disposed: Incineration
        $field_msldisposalincineration = isset($node->field_msldisposalincineration['und'][0]['value']) ? $node->field_msldisposalincineration['und'][0]['value'] : 0;
        //Of which: disposed: Others
        $field_msldisposalothers = isset($node->field_msldisposalothers['und'][0]['value']) ? $node->field_msldisposalothers['und'][0]['value'] : 0;
        //reuse percent
        $field_mslwwreuseperc = isset($node->field_mslwwreuseperc['und'][0]['value']) ? $node->field_mslwwreuseperc['und'][0]['value'] : 0;
        unset($node);

        $field_others = $field_msldischargepipelines + $field_msldischargeships + $field_msldischargeothers;

        $sheet->setCellValue('B24', round($field_mslsludgeproduction));
        $sheet->setCellValue('B25', round($field_mslreusesoilagriculture));
        $sheet->setCellValue('B26', round($field_mslreuseothers));
        $sheet->setCellValue('B27', round($field_msldisposallandfill));
        $sheet->setCellValue('B28', round($field_msldisposalincineration));
        $sheet->setCellValue('B29', round($field_msldisposalothers));
        $sheet->setCellValue('B30', round($field_others));

//debug to comment
// dsm('delete all debug value B24 ...');
// $sheet->setCellValue('B24', 603142);
//debug to comment

        if (!empty($field_mslsludgeproduction)) {
            if (!empty($field_mslreusesoilagriculture)) {
                $sheet->setCellValue('C25', round($field_mslreusesoilagriculture * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_mslreuseothers)) {
                $sheet->setCellValue('C26', round($field_mslreuseothers * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_msldisposallandfill)) {
                $sheet->setCellValue('C27', round($field_msldisposallandfill * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_msldisposalincineration)) {
                $sheet->setCellValue('C28', round($field_msldisposalincineration * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_msldisposalothers)) {
                $sheet->setCellValue('C29', round($field_msldisposalothers * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_others)) {
                $sheet->setCellValue('C30', round($field_others * 100 / $field_mslsludgeproduction, 1));
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
        if (isset($context['results']['country_volume_waste_water'])) {
        	$sheet->setCellValue('C36', $context['results']['country_volume_waste_water']);
        }
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


    $cell = $sheet->getCell('A75');
    $cell->setValue(str_replace(array('[#COUNTRY#]', '[#current_year#]'), array($country_name, $annee), $cell->getValue()));

    if (isset($context['results']['graph']['summary_installation']['total']['number'])) {
    	$sheet->getCell('B77')->setValue($context['results']['graph']['summary_installation']['total']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['total']['percent'])) {
    	$sheet->getCell('C77')->setValue(round($context['results']['graph']['summary_installation']['total']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['waste_pe'])) {
    	$sheet->getCell('D77')->setValue(round($context['results']['graph']['summary_installation']['total']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['waste_percent'])) {
    	$sheet->getCell('E77')->setValue(round($context['results']['graph']['summary_installation']['total']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['number'])) {
    	$sheet->getCell('B78')->setValue($context['results']['graph']['summary_installation']['total']['collecting_system']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['percent'])) {
    	$sheet->getCell('C78')->setValue(round($context['results']['graph']['summary_installation']['total']['collecting_system']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_pe'])) {
    	$sheet->getCell('D78')->setValue(round($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_percent'])) {
    	$sheet->getCell('E78')->setValue(round($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['number'])) {
    	$sheet->getCell('B79')->setValue($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['percent'])) {
    	$sheet->getCell('C79')->setValue(round($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_pe'])) {
    	$sheet->getCell('D79')->setValue(round($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_percent'])) {
    	$sheet->getCell('E79')->setValue(round($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['number'])) {
    	$sheet->getCell('B80')->setValue($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['percent'])) {
    	$sheet->getCell('C80')->setValue(round($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_pe'])) {
    	$sheet->getCell('D80')->setValue(round($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_percent'])) {
    	$sheet->getCell('E80')->setValue(round($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['number'])) {
    	$sheet->getCell('B81')->setValue($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['percent'])) {
    	$sheet->getCell('C81')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_pe'])) {
    	$sheet->getCell('D81')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_percent'])) {
    	$sheet->getCell('E81')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['number'])) {
    	$sheet->getCell('B82')->setValue($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['percent'])) {
    	$sheet->getCell('C82')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_pe'])) {
    	$sheet->getCell('D82')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_percent'])) {
    	$sheet->getCell('E82')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_percent'], 1));
    }

    //previous
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

    $sheet->setCellValue('F81', uwwtd_register_get_cell_value($previoussheet, 'B81'));
    $sheet->setCellValue('G81', uwwtd_register_get_cell_value($previoussheet, 'C81'));
    $sheet->setCellValue('H81', uwwtd_register_get_cell_value($previoussheet, 'D81'));
    $sheet->setCellValue('I81', uwwtd_register_get_cell_value($previoussheet, 'E81'));

    $sheet->setCellValue('F82', uwwtd_register_get_cell_value($previoussheet, 'B82'));
    $sheet->setCellValue('G82', uwwtd_register_get_cell_value($previoussheet, 'C82'));
    $sheet->setCellValue('H82', uwwtd_register_get_cell_value($previoussheet, 'D82'));
    $sheet->setCellValue('I82', uwwtd_register_get_cell_value($previoussheet, 'E82'));

    //array 9 Summary distance to compliance expired deadline
    //Target connection pe
    if (isset($context['results']['distance_to_compliance']['expired']['target_connection']['total_load_pe'])) {
    	$sheet->getCell('C88')->setValue(round($context['results']['distance_to_compliance']['expired']['target_connection']['total_load_pe']));
    }
    //not connected
    if (isset($context['results']['distance_to_compliance']['expired']['target_connection']['distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['target_connection']['distance_pe'] == 0) ? $sheet->getCell('C89')->setValue(0) : $sheet->getCell('B89')->setValue(round($context['results']['distance_to_compliance']['expired']['target_connection']['distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['target_connection']['distance_pe'])) {
    	$sheet->getCell('C89')->setValue(round($context['results']['distance_to_compliance']['expired']['target_connection']['distance_pe']));
    }
    //2nd treatment target
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['total_load_pe'])) {
    	$sheet->getCell('C90')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['total_load_pe']));
    }
    //2nd treatment not in place
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_pe'] == 0) ? $sheet->getCell('B91')->setValue(0) : $sheet->getCell('B91')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_pe'])) {
    	$sheet->getCell('C91')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_pe']));
    }
    //2nd treatment performance not met
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B92')->setValue(0) : $sheet->getCell('B92')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C92')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_pe']));
    }
    //Tertiary treatment target
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['total_load_pe'])) {
    	$sheet->getCell('C93')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['total_load_pe']));
    }
    //Tertiary treatment not in place
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_pe'] == 0) ? $sheet->getCell('B94')->setValue(0) : $sheet->getCell('B94')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_pe'])) {
    	$sheet->getCell('C94')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_pe']));
    }
    //Tertiary stringent treatment performance not met
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B95')->setValue(0) : $sheet->getCell('B95')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C95')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_pe']));
    }

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
    if (isset($context['results']['distance_to_compliance']['pending']['target_connection']['total_load_pe'])) {
    	$sheet->getCell('C104')->setValue(round($context['results']['distance_to_compliance']['pending']['target_connection']['total_load_pe']));
    }
    //not connected
    if (isset($context['results']['distance_to_compliance']['pending']['target_connection']['distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['target_connection']['distance_pe'] == 0) ? $sheet->getCell('B105')->setValue(0) : $sheet->getCell('B105')->setValue(round($context['results']['distance_to_compliance']['pending']['target_connection']['distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['target_connection']['distance_pe'])) {
    	$sheet->getCell('C105')->setValue(round($context['results']['distance_to_compliance']['pending']['target_connection']['distance_pe']));
    }
    //2nd treatment target
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['total_load_pe'])) {
    	$sheet->getCell('C106')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['total_load_pe']));
    }
    //2nd treatment not in place
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_pe'] == 0) ? $sheet->getCell('B107')->setValue(0) : $sheet->getCell('B107')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_pe'])) {
    	$sheet->getCell('C107')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_pe']));
    }
    //2nd treatment performance not met
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B108')->setValue(0) : $sheet->getCell('B108')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C108')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_pe']));
    }
    //Tertiary treatment target
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['total_load_pe'])) {
    	$sheet->getCell('C109')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['total_load_pe']));
    }
    //Tertiary treatment not in place
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_pe'] == 0) ? $sheet->getCell('B110')->setValue(0) : $sheet->getCell('B110')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_pe'])) {
    	$sheet->getCell('C110')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_pe']));
    }
    //Tertiary stringent treatment performance not met
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B111')->setValue(0) : $sheet->getCell('B111')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C111')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_pe']));
    }

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
    if (isset($context['results']['distance_to_compliance']['total']['target_connection']['total_load_pe'])) {
    	$sheet->getCell('C119')->setValue(round($context['results']['distance_to_compliance']['total']['target_connection']['total_load_pe']));
    }
    //not connected
    if (isset($context['results']['distance_to_compliance']['total']['target_connection']['distance_%'])) {
    	$sheet->getCell('B120')->setValue(round($context['results']['distance_to_compliance']['total']['target_connection']['distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['target_connection']['distance_pe'])) {
    	$sheet->getCell('C120')->setValue(round($context['results']['distance_to_compliance']['total']['target_connection']['distance_pe']));
    }
    //2nd treatment target
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['total_load_pe'])) {
    	$sheet->getCell('C121')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['total_load_pe']));
    }
    //2nd treatment not in place
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_%'])) {
    	$sheet->getCell('B122')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_pe'])) {
    	$sheet->getCell('C122')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_pe']));
    }
    //2nd treatment performance not met
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_%'])) {
    	$sheet->getCell('B123')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C123')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_pe']));
    }
    //Tertiary treatment target
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['total_load_pe'])) {
    	$sheet->getCell('C124')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['total_load_pe']));
    }
    //Tertiary treatment not in place
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_%'])) {
    	$sheet->getCell('B125')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_pe'])) {
    	$sheet->getCell('C125')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_pe']));
    }
    //Tertiary stringent treatment performance not met
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_%'])) {
    	$sheet->getCell('B126')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C126')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_pe']));
    }

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
    // Parameters   reference year
    //  %   p.e.    number of plants
    // Total load generated 100,00% 5 164 016
    // No treatment (not connected + no treatment after connection) 5,11%   264097      sum of ((neither collecting system nor IAS)+(collected in collecting system-entering load)+(entering load for all stations with compliance article 4 =NC))
    // IAS  0,00%   0       sum of adressed through IAS
    // primary treatment as a maximum   0,23%   11779       sum of entering load for all stations with treatment type in place = 1
    // secondary treatment only as a maximum (performance met)  25,22%  1302236     sum of entering load for all stations with treatment type in place = 2
    // N removal without P removal (performance met)    6,91%   356636      sum of entering load for all stations with treatment type in place = 3N and performance Ntot=pass or NR
    // P removal without N removal (performance met)    1,44%   74542       sum of entering load for all stations with treatment type in place = 3P and performance Ptot=pass or NR
    // N and P removal (performance met)    0,40%   20813       sum of entering load for all stations with treatment type in place = 3NP and performance Ntot=pass or NR & Ptot=pass or NR
    // Other more stringent treatments  60,69%  3133913     sum of entering load for all stations with treatment type in place = 3O only (no 3N, no 3NP, no P)
    //todo
//         $context['results']['uwwtp_total_load_generated_pe'] = 0;
//         $context['results']['uwwtp_notreatment_pe'] = 0;
//         $context['results']['uwwtp_ias_pe'] = 0;
//         $context['results']['uwwtp_p_as_max_pe'] = 0;
//         $context['results']['uwwtp_s_as_max_pe'] = 0;
//         $context['results']['uwwtp_n_removal_without_p_pe'] = 0;
//         $context['results']['uwwtp_p_removal_without_n_pe'] = 0;
//         $context['results']['uwwtp_np_removal_pe'] = 0;
//         $context['results']['uwwtp_other_morestringent_pe'] = 0;

    $sheet->setCellValue('B131', '100.00');
    if (isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('C131', round($context['results']['uwwtp_total_load_generated_pe']));
    }
    if (isset($context['results']['uwwtp_total_load_generated_nb'])) {
    	$sheet->setCellValue('D131', $context['results']['uwwtp_total_load_generated_nb']);
    }
    if (isset($context['results']['uwwtp_notreatment_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B132', round($context['results']['uwwtp_notreatment_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_notreatment_pe'])) {
    	$sheet->setCellValue('C132', round($context['results']['uwwtp_notreatment_pe']));
    }
    if (isset($context['results']['uwwtp_notreatment_nb'])) {
    	$sheet->setCellValue('D132', $context['results']['uwwtp_notreatment_nb']);
    }
    



    
    
        



    if (isset($context['results']['uwwtp_ias_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B133', round($context['results']['uwwtp_ias_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_ias_pe'])) {
    	$sheet->setCellValue('C133', round($context['results']['uwwtp_ias_pe']));
    }
    if (isset($context['results']['uwwtp_ias_nb'])) {
    	$sheet->setCellValue('D133', $context['results']['uwwtp_ias_nb']);
    }
    if (isset($context['results']['uwwtp_p_as_max_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B134', round($context['results']['uwwtp_p_as_max_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_p_as_max_pe'])) {
    	$sheet->setCellValue('C134', round($context['results']['uwwtp_p_as_max_pe']));
    }
    if (isset($context['results']['uwwtp_p_as_max_nb'])) {
    	$sheet->setCellValue('D134', $context['results']['uwwtp_p_as_max_nb']);
    }
    if (isset($context['results']['uwwtp_s_as_max_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B135', round($context['results']['uwwtp_s_as_max_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_s_as_max_pe'])) {
    	$sheet->setCellValue('C135', round($context['results']['uwwtp_s_as_max_pe']));
    }
    if (isset($context['results']['uwwtp_s_as_max_nb'])) {
    	$sheet->setCellValue('D135', $context['results']['uwwtp_s_as_max_nb']);
    }
    if (isset($context['results']['uwwtp_n_removal_without_p_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B136', round($context['results']['uwwtp_n_removal_without_p_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_n_removal_without_p_pe'])) {
    	$sheet->setCellValue('C136', round($context['results']['uwwtp_n_removal_without_p_pe']));
    }
    if (isset($context['results']['uwwtp_n_removal_without_p_nb'])) {
    	$sheet->setCellValue('D136', $context['results']['uwwtp_n_removal_without_p_nb']);
    }
    if (isset($context['results']['uwwtp_p_removal_without_n_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B137', round($context['results']['uwwtp_p_removal_without_n_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_p_removal_without_n_pe'])) {
    	$sheet->setCellValue('C137', round($context['results']['uwwtp_p_removal_without_n_pe']));
    }
    if (isset($context['results']['uwwtp_p_removal_without_n_nb'])) {
    	$sheet->setCellValue('D137', $context['results']['uwwtp_p_removal_without_n_nb']);
    }
    if (isset($context['results']['uwwtp_np_removal_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B138', round($context['results']['uwwtp_np_removal_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_np_removal_pe'])) {
    	$sheet->setCellValue('C138', round($context['results']['uwwtp_np_removal_pe']));
    }
    if (isset($context['results']['uwwtp_np_removal_nb'])) {
    	$sheet->setCellValue('D138', $context['results']['uwwtp_np_removal_nb']);
    }
    if (isset($context['results']['uwwtp_other_morestringent_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B139', round($context['results']['uwwtp_other_morestringent_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_other_morestringent_pe'])) {
    	$sheet->setCellValue('C139', round($context['results']['uwwtp_other_morestringent_pe']));
    }
    if (isset($context['results']['uwwtp_other_morestringent_nb'])) {
    	$sheet->setCellValue('D139', $context['results']['uwwtp_other_morestringent_nb']);
    }

    //array 13    incoming/discharged
    if (isset($context['results']['graph']['incoming_load_bod'])) {
    	$sheet->getCell('C148')->setValue(round($context['results']['graph']['incoming_load_bod']));
    }
    if (isset($context['results']['graph']['discharge_load_bod'])) {
    	$sheet->getCell('C149')->setValue(round($context['results']['graph']['discharge_load_bod']));
    }
    if (isset($context['results']['graph']['incoming_load_cod'])) {
    	$sheet->getCell('C150')->setValue(round($context['results']['graph']['incoming_load_cod']));
    }
    if (isset($context['results']['graph']['discharge_load_cod'])) {
    	$sheet->getCell('C151')->setValue(round($context['results']['graph']['discharge_load_cod']));
    }
    if (isset($context['results']['graph']['incoming_load_n'])) {
    	$sheet->getCell('C152')->setValue(round($context['results']['graph']['incoming_load_n']));
    }
    if (isset($context['results']['graph']['discharge_load_n'])) {
    	$sheet->getCell('C153')->setValue(round($context['results']['graph']['discharge_load_n']));
    }
    if (isset($context['results']['graph']['incoming_load_p'])) {
    	$sheet->getCell('C154')->setValue(round($context['results']['graph']['incoming_load_p']));
    }
    if (isset($context['results']['graph']['discharge_load_p'])) {
    	$sheet->getCell('C155')->setValue(round($context['results']['graph']['discharge_load_p']));
    }

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
    if (isset($context['results']['graph']['discharge_load_bod']) && isset($context['results']['graph']['incoming_load_bod'])) {
    	$sheet->getCell('C159')->setValue(round(($context['results']['graph']['discharge_load_bod'] / $context['results']['graph']['incoming_load_bod']) *100, 1));
    }
    if (isset($context['results']['graph']['discharge_load_cod']) && isset($context['results']['graph']['incoming_load_cod'])) {
    	$sheet->getCell('C160')->setValue(round(($context['results']['graph']['discharge_load_cod'] / $context['results']['graph']['incoming_load_cod']) *100, 1));
    }
    if (isset($context['results']['graph']['discharge_load_n']) && isset($context['results']['graph']['incoming_load_n'])) {
    	$sheet->getCell('C161')->setValue(round(($context['results']['graph']['discharge_load_n'] / $context['results']['graph']['incoming_load_n']) *100, 1));
    }
    if (isset($context['results']['graph']['discharge_load_p']) && isset($context['results']['graph']['incoming_load_p'])) {
    	$sheet->getCell('C162')->setValue(round(($context['results']['graph']['discharge_load_p'] / $context['results']['graph']['incoming_load_p']) *100, 1));
    }
    //previous
    $sheet->getCell('D159')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C159'));
    $sheet->getCell('D160')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C160'));
    $sheet->getCell('D161')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C161'));
    $sheet->getCell('D162')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C162'));

    //array 15   totla volume
    if (isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C166')->setValue(round($context['results']['country_volume_waste_water']));
    }
    //previous
    $sheet->getCell('D166')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C166'));

    //array 16  average
    if (isset($context['results']['graph']['incoming_load_bod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C169')->setValue(round(($context['results']['graph']['incoming_load_bod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_bod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C170')->setValue(round(($context['results']['graph']['discharge_load_bod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['incoming_load_cod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C171')->setValue(round(($context['results']['graph']['incoming_load_cod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_cod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C172')->setValue(round(($context['results']['graph']['discharge_load_cod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['incoming_load_n']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C173')->setValue(round(($context['results']['graph']['incoming_load_n'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_n']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C174')->setValue(round(($context['results']['graph']['discharge_load_n'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['incoming_load_p']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C175')->setValue(round(($context['results']['graph']['incoming_load_p'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_p']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C176')->setValue(round(($context['results']['graph']['discharge_load_p'] / $context['results']['country_volume_waste_water']) * 1000000));
    }

    //previous
    $sheet->getCell('D169')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C169'));
    $sheet->getCell('D170')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C170'));
    $sheet->getCell('D171')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C171'));
    $sheet->getCell('D172')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C172'));
    $sheet->getCell('D173')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C173'));
    $sheet->getCell('D174')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C174'));
    $sheet->getCell('D175')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C175'));
    $sheet->getCell('D176')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C176'));

    // Récupération des valeurs du tableau "Comparison of Compliance between previous year and current year in p.e." :
    $arrayInPe = array();
    $arrayInPe['Compliant']['Article 3'] = $sheet->getCell('E55')->getValue();
    $arrayInPe['Compliant']['Article 4'] = $sheet->getCell('E58')->getValue();
    $arrayInPe['Compliant']['Article 5'] = $sheet->getCell('E61')->getValue();

    $arrayInPe['Not compliant']['Article 3'] = $sheet->getCell('F55')->getValue();
    $arrayInPe['Not compliant']['Article 4'] = $sheet->getCell('F58')->getValue();
    $arrayInPe['Not compliant']['Article 5'] = $sheet->getCell('F61')->getValue();

    $arrayInPe['Not relevant']['Article 3'] = $sheet->getCell('G55')->getValue();
    $arrayInPe['Not relevant']['Article 4'] = $sheet->getCell('G58')->getValue();
    $arrayInPe['Not relevant']['Article 5'] = $sheet->getCell('G61')->getValue();

    // Récupération des valeurs du tableau "Comparison of Compliance between previous year and current year in number of agglomerations" :
    $arrayInNumOfAgg = array();
    $arrayInNumOfAgg['Compliant']['Article 3'] = $sheet->getCell('B43')->getValue();
    $arrayInNumOfAgg['Compliant']['Article 4'] = $sheet->getCell('B46')->getValue();
    $arrayInNumOfAgg['Compliant']['Article 5'] = $sheet->getCell('B49')->getValue();

    $arrayInNumOfAgg['Not compliant']['Article 3'] = $sheet->getCell('B44')->getValue();
    $arrayInNumOfAgg['Not compliant']['Article 4'] = $sheet->getCell('B47')->getValue();
    $arrayInNumOfAgg['Not compliant']['Article 5'] = $sheet->getCell('B50')->getValue();

    $arrayInNumOfAgg['Not relevant']['Article 3'] = $sheet->getCell('B45')->getValue();
    $arrayInNumOfAgg['Not relevant']['Article 4'] = $sheet->getCell('B48')->getValue();
    $arrayInNumOfAgg['Not relevant']['Article 5'] = $sheet->getCell('B51')->getValue();

    // Récupération des valeurs du tableau "Comparison of distance to compliance expired deadline for previous year and current year" :
    $distancesExpired = array();
    $distancesExpired['Target connection'] = $sheet->getCell('B88')->getValue();
    $distancesExpired['Not connected'] = $sheet->getCell('B89')->getValue();
    $distancesExpired['2nd treatment target'] = $sheet->getCell('B90')->getValue();
    $distancesExpired['2nd treatment not in place'] = $sheet->getCell('B91')->getValue();
    $distancesExpired['2nd treatment performance not met'] = $sheet->getCell('B92')->getValue();
    $distancesExpired['Tertiary treatment target'] = $sheet->getCell('B93')->getValue();
    $distancesExpired['Tertiary treatment not in place'] = $sheet->getCell('B94')->getValue();
    $distancesExpired['Tertiary stringent treatment performance not met'] = $sheet->getCell('B95')->getValue();

    // R   cup   ration des valeurs du tableau "Comparison of distance to compliance pending deadlines for previous year and current year" :
    $distancesPending = array();
    $distancesPending['Target connection'] = $sheet->getCell('B104')->getValue();
    $distancesPending['Not connected'] = $sheet->getCell('B105')->getValue();
    $distancesPending['2nd treatment target'] = $sheet->getCell('B106')->getValue();
    $distancesPending['2nd treatment not in place'] = $sheet->getCell('B107')->getValue();
    $distancesPending['2nd treatment performance not met'] = $sheet->getCell('B108')->getValue();
    $distancesPending['Tertiary treatment target'] = $sheet->getCell('B109')->getValue();
    $distancesPending['Tertiary treatment not in place'] = $sheet->getCell('B110')->getValue();
    $distancesPending['Tertiary stringent treatment performance not met'] = $sheet->getCell('B111')->getValue();

    // R   cup   ration des valeurs du tableau "Distance to compliance total" :
    $distancesTotal = array();
    $distancesTotal['Target connection'] = $sheet->getCell('C119')->getValue();
    $distancesTotal['Not connected'] = $sheet->getCell('C120')->getValue();
    $distancesTotal['2nd treatment target'] = $sheet->getCell('C121')->getValue();
    $distancesTotal['2nd treatment not in place'] = $sheet->getCell('C122')->getValue();
    $distancesTotal['2nd treatment performance not met'] = $sheet->getCell('C123')->getValue();
    $distancesTotal['Tertiary treatment target'] = $sheet->getCell('C124')->getValue();
    $distancesTotal['Tertiary treatment not in place'] = $sheet->getCell('C125')->getValue();
    $distancesTotal['Tertiary stringent treatment performance not met'] = $sheet->getCell('C126')->getValue();

    // R   cup   ration du nid du node MS_LEVEL de cette ann   e :
    $query = db_select('node', 'n');
    $query->leftJoin('field_data_field_anneedata', 'a', 'n.nid = a.entity_id AND a.entity_type = \'node\'');
    $query->fields('n');
    $query->fields('a', array('field_anneedata_value'));
    $query->condition('a.field_anneedata_value', $annee, '=');
    $query->condition('n.type', 'ms_level', '=');
    $query->condition('n.status', 1, '=');
    $nid = $query->execute()->fetchField();

    // Si il n'y a pas encore de node ms_level pour cette année :
    global $language;
	$siteId = $annee.'_mslevel';
    if (empty($nid)) {
		// Création du node pour cette année :
        $node = new stdClass();
    	$node->type = 'ms_level';
    	$node->uid = 1;
     	$node->translate = 0;
        $node->tnid = 0;
        $node->sticky = 0;
        $node->promote = 0;
        $node->comment = 0;
        $node->language = $language->language;
        $node->title = 'Data on MS level - '. $annee;
        $node->field_siteid['und'][0]['value'] = $siteId;
        $node->field_anneedata['und'][0]['value'] = $annee;
        node_save($node);

        // Récupération du nid :
        $query = db_select('node', 'n');
        $query->fields('n');
        $query->condition('n.type', 'ms_level', '=');
        $query->condition('n.title', 'Data on MS level - '. $annee, '=');
        $nid = $query->execute()->fetchField();
    }

    // Renseignement des champs "Comparison of Compliance PE" et "Comparison of Compliance Agg" :
    $node = node_load($nid);
    if (!empty($node)) {
    	$node->field_comparison_of_compliance_p[LANGUAGE_NONE][0]['value'] = json_encode($arrayInPe);
    	$node->field_comparison_of_compliance_a[LANGUAGE_NONE][0]['value'] = json_encode($arrayInNumOfAgg);
    	$node->field_compliance_expired[LANGUAGE_NONE][0]['value'] = json_encode($distancesExpired);
    	$node->field_compliance_pending[LANGUAGE_NONE][0]['value'] = json_encode($distancesPending);
    	$node->field_compliance_total[LANGUAGE_NONE][0]['value'] = json_encode($distancesTotal);
    }
    node_save($node);

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
//     $objWriter->setPreCalculateFormulas(true);
    $objWriter->setIncludeCharts(TRUE);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);

    $context['sandbox']['progress']++;

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name']));

//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
	if (isset($context['sandbox']['progress']) && isset($context['sandbox']['max'])) {
    	$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
//     }
}

function uwwtd_create_register_2020_file_graph_sheet11($annee, &$context)
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

    $model_path = uwwtd_getregister_modelfile_path($annee);
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
    $previous_year_n2 = uwwtd_get_previous_year($previous_year);
    $previoussheet = null;
    $previoussheet_n2 = null;

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
    if (false !== $previous_year_n2) {
        $previous_model_path_n2 = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['graph']['xlsx_name'], $previous_year_n2);
        if (true === file_exists($previous_model_path_n2)) {
            $previousobjReader_n2 = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
            $previousobjReader_n2->setIncludeCharts(false);
            $previousobjReader_n2->setLoadSheetsOnly( array(
                                                 $GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name'],
                                                ) );
            $previousobjPHPExcel_n2 = $previousobjReader_n2->load(drupal_realpath($previous_model_path_n2));
            $previoussheet_n2 = $previousobjPHPExcel_n2->getSheetByName($GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name']);
        }
    }

    //size of agglo/pe per class
    //=========================================array1
// uwwtd_trace('Graph : context result = ');
// uwwtd_trace(var_export($context['results'], true));

    if (isset($context['results']['graph']['0_10000']['number'])) {
    	$sheet->setCellValue('B5', $context['results']['graph']['0_10000']['number']);
    }
    if (isset($context['results']['graph']['0_10000']['pe'])) {
    	$sheet->setCellValue('C5', round($context['results']['graph']['0_10000']['pe'], 0));
    }
    if (isset($context['results']['graph']['10000_100000']['number'])) {
    	$sheet->setCellValue('B6', $context['results']['graph']['10000_100000']['number']);
    }
    if (isset($context['results']['graph']['10000_100000']['pe'])) {
    	$sheet->setCellValue('C6', round($context['results']['graph']['10000_100000']['pe'], 0));
    }
    if (isset($context['results']['graph']['100000_n']['number'])) {
    	$sheet->setCellValue('B7', $context['results']['graph']['100000_n']['number']);
    }
    if (isset($context['results']['graph']['100000_n']['pe'])) {
	    $sheet->setCellValue('C7', round($context['results']['graph']['100000_n']['pe'], 0));
    } 

    //previous
    $sheet->setCellValue('D5', uwwtd_register_get_cell_value($previoussheet, 'B5'));
    $sheet->setCellValue('E5', uwwtd_register_get_cell_value($previoussheet, 'C5'));
    $sheet->setCellValue('D6', uwwtd_register_get_cell_value($previoussheet, 'B6'));
    $sheet->setCellValue('E6', uwwtd_register_get_cell_value($previoussheet, 'C6'));
    $sheet->setCellValue('D7', uwwtd_register_get_cell_value($previoussheet, 'B7'));
    $sheet->setCellValue('E7', uwwtd_register_get_cell_value($previoussheet, 'C7'));
    //previous_n2
    $sheet->setCellValue('F5', uwwtd_register_get_cell_value($previoussheet, 'B5'));
    $sheet->setCellValue('G5', uwwtd_register_get_cell_value($previoussheet, 'C5'));
    $sheet->setCellValue('F6', uwwtd_register_get_cell_value($previoussheet, 'B6'));
    $sheet->setCellValue('G6', uwwtd_register_get_cell_value($previoussheet, 'C6'));
    $sheet->setCellValue('F7', uwwtd_register_get_cell_value($previoussheet, 'B7'));
    $sheet->setCellValue('G7', uwwtd_register_get_cell_value($previoussheet, 'C7'));

    //=============================================================array2
    if (isset($context['results']['graph']['total_generated']['pe'])) {
    	$sheet->setCellValue('B15', round($context['results']['graph']['total_generated']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_collected']['pe'])) {
    	$sheet->setCellValue('B16', round($context['results']['graph']['total_collected']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_load_ias']['pe'])) {
    	$sheet->setCellValue('B17', round($context['results']['graph']['total_load_ias']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_entering']['pe'])) {
    	$sheet->setCellValue('B18', round($context['results']['graph']['total_entering']['pe'], 0));
    }
    if (isset($context['results']['graph']['total_capacity']['pe'])) {
    	$sheet->setCellValue('B19', round($context['results']['graph']['total_capacity']['pe'], 0));
    }

    //previous
    $previousB15 = uwwtd_register_get_cell_value($previoussheet, 'B15'); 
    $sheet->setCellValue('C15', $previousB15);
    $sheet->setCellValue('C16', uwwtd_register_get_cell_value($previoussheet, 'B16'));
    $sheet->setCellValue('C17', uwwtd_register_get_cell_value($previoussheet, 'B17'));
    $sheet->setCellValue('C18', uwwtd_register_get_cell_value($previoussheet, 'B18'));
    $sheet->setCellValue('C19', uwwtd_register_get_cell_value($previoussheet, 'B19'));
    //previous_n2
    $previousB15_n2 = uwwtd_register_get_cell_value($previoussheet_n2, 'B15'); 
    $sheet->setCellValue('D15', $previousB15_n2);
    $sheet->setCellValue('D16', uwwtd_register_get_cell_value($previoussheet, 'B16'));
    $sheet->setCellValue('D17', uwwtd_register_get_cell_value($previoussheet, 'B17'));
    $sheet->setCellValue('D18', uwwtd_register_get_cell_value($previoussheet, 'B18'));
    $sheet->setCellValue('D19', uwwtd_register_get_cell_value($previoussheet, 'B19'));

         
    $context['results']['auto_report']['COUNTRY']  = $country_name;
    $context['results']['auto_report']['CURRENT_REPORT_YEAR']  = $annee;
    $context['results']['auto_report']['PREVIOUS_REPORT_YEAR']  = $previous_year;
    $context['results']['auto_report']['PREVIOUS_REPORT_NUMBER'] = $GLOBALS['report_number'][$previous_year];  
    $context['results']['auto_report']['PREVIOUS_REPORT_YEAR_N2']  = $previous_year_n2;
    $context['results']['auto_report']['PREVIOUS_REPORT_NUMBER_N2'] = $GLOBALS['report_number'][$previous_year_n2];  
    $context['results']['auto_report']['CURRENT_REPORT_NUMBER'] = $GLOBALS['report_number'][$annee];
    $context['results']['auto_report']['TOTAL_GENERATED_LOAD'] = $context['results']['graph']['total_generated']['pe'];
    //Cell B15 �� cellule C15 auto generation report
    $context['results']['auto_report']['WASTER_WATER_LOAD_INCREASED'] = round($context['results']['graph']['total_generated']['pe'], 0)  -  $previousB15; 

    //================================================== Array 3
    //Array on load by global compliance (pink array)
    module_load_include('inc','uwwtd','uwwtd.stats');
    $years = [$annee,  $previous_year, $previous_year_n2];
    $data = uwwtd_stat_national_compliance('allyears', $years);
    unset($years);
    if(isset($data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['NC']])){
        $a = $data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['NC']]['data'];
        if(isset($a[$annee])){
            $sheet->setCellValue('K3', $a[$annee]['pe']);
            $sheet->setCellValue('L3', $a[$annee]['nb']);
        }
        if(isset($a[$previous_year])){
            $sheet->setCellValue('K8', $a[$previous_year]['pe']);
            $sheet->setCellValue('L8', $a[$previous_year]['nb']);
        }
        if(isset($a[$previous_year_n2])){
            $sheet->setCellValue('K13', $a[$previous_year_n2]['pe']);
            $sheet->setCellValue('L13', $a[$previous_year_n2]['nb']);
        }
    }
    if(isset($data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['C']])){
        $a = $data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['C']]['data'];
        if(isset($a[$annee])){
            $sheet->setCellValue('K4', $a[$annee]['pe']);
            $sheet->setCellValue('L4', $a[$annee]['nb']);
        }
        if(isset($a[$previous_year])){
            $sheet->setCellValue('K9', $a[$previous_year]['pe']);
            $sheet->setCellValue('L9', $a[$previous_year]['nb']);
        }
        if(isset($a[$previous_year_n2])){
            $sheet->setCellValue('K14', $a[$previous_year_n2]['pe']);
            $sheet->setCellValue('L14', $a[$previous_year_n2]['nb']);
        }
    }
    if(isset($data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['NR']])){
        $a = $data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['NR']]['data'];
        if(isset($a[$annee])){
            $sheet->setCellValue('K5', $a[$annee]['pe']);
            $sheet->setCellValue('L5', $a[$annee]['nb']);
        }
        if(isset($a[$previous_year])){
            $sheet->setCellValue('K10', $a[$previous_year]['pe']);
            $sheet->setCellValue('L10', $a[$previous_year]['nb']);
        }
        if(isset($a[$previous_year_n2])){
            $sheet->setCellValue('K15', $a[$previous_year_n2]['pe']);
            $sheet->setCellValue('L15', $a[$previous_year_n2]['nb']);
        }
    }
    if(isset($data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['NI']])){
        $a = $data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['NI']]['data'];
        if(isset($a[$annee])){
            $sheet->setCellValue('K6', $a[$annee]['pe']);
            $sheet->setCellValue('L6', $a[$annee]['nb']);
        }
        if(isset($a[$previous_year])){
            $sheet->setCellValue('K11', $a[$previous_year]['pe']);
            $sheet->setCellValue('L11', $a[$previous_year]['nb']);
        }
        if(isset($a[$previous_year_n2])){
            $sheet->setCellValue('K16', $a[$previous_year_n2]['pe']);
            $sheet->setCellValue('L16', $a[$previous_year_n2]['nb']);
        }
    }
    if(isset($data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['?']])){
        $a = $data[$GLOBALS['uwwtd']['ui']['field']['compliance_label']['?']]['data'];
        if(isset($a[$annee])){
            $sheet->setCellValue('K7', $a[$annee]['pe']);
            $sheet->setCellValue('L7', $a[$annee]['nb']);
        }
        if(isset($a[$previous_year])){
            $sheet->setCellValue('K12', $a[$previous_year]['pe']);
            $sheet->setCellValue('L12', $a[$previous_year]['nb']);
        }
        if(isset($a[$previous_year_n2])){
            $sheet->setCellValue('K17', $a[$previous_year_n2]['pe']);
            $sheet->setCellValue('L17', $a[$previous_year_n2]['nb']);
        }
    }
    unset($a, $data);
    
    //==================================================array 5 6 7
//     $previous_year = uwwtd_get_previous_year($annee);
    $replaceyearby = $previous_year;
    if (false === $previous_year) {
        $replaceyearby = 'N/A';
    }
    $replaceyearby_n2 = $previous_year_n2;
    if (false === $previous_year_n2) {
        $replaceyearby_n2 = 'N/A';
    }

    $cell_to_repalce = array(
        'B3','D3','F3',
        'B21',
        'B75',
        'A54','A55','A56','A57','A58','A59','A60','A61','A62',
         'A64','A65','A66','A67','A68','A69','A70','A71','A72',
         'M41','N41','O41','P41','Q41','R41','S41','T41','U41',
         'F53',
         //'M50', 'M51','M52','M53','M54','M55','M56','M57','M58',
         'F63',
         //'M65','M66','M67','M68','M69','M70','M71','M72','M73',
         'B74',
         /*
         'AA31',
         'Z33','Z34','Z35',
        'AB48','AB62',
         'AA77','AB77','AC77','AD77','AE77','AF77','AG77',
         'AA86','AB86','AC86',
         'AA103','AB103','AC103',
         'AA118','AB118','AC118',
         */
     );
    foreach ($cell_to_repalce as $x){
        $cell = $sheet->getCell($x);
        $cell->setValue(str_replace(array('[#previous_year#]','[#previous_year_n2#]', '[#current_year#]'), array($replaceyearby,$replaceyearby_n2, $annee), $cell->getValue()));
    }

	if (isset($context['results']['graph']['art3_C']['number'])) {
    	$sheet->setCellValue('B43', $context['results']['graph']['art3_C']['number']);
	}
	if (isset($context['results']['graph']['art3_C']['pe'])) {
    	$sheet->setCellValue('C43', round($context['results']['graph']['art3_C']['pe']));
	}
	if (isset($context['results']['graph']['art3_NC']['number'])) {
    	$sheet->setCellValue('B44', $context['results']['graph']['art3_NC']['number']);
	}
	if (isset($context['results']['graph']['art3_NC']['pe'])) {
    	$sheet->setCellValue('C44', round($context['results']['graph']['art3_NC']['pe']));
	}
	if (isset($context['results']['graph']['art3_NR']['number'])) {
    	$sheet->setCellValue('B45', $context['results']['graph']['art3_NR']['number']);
	}
	if (isset($context['results']['graph']['art3_NR']['pe'])) {
    	$sheet->setCellValue('C45', round($context['results']['graph']['art3_NR']['pe']));
	}
	if (isset($context['results']['graph']['art4_C']['number'])) {
    	$sheet->setCellValue('B46', $context['results']['graph']['art4_C']['number']);
	}
	if (isset($context['results']['graph']['art4_C']['pe'])) {
    	$sheet->setCellValue('C46', round($context['results']['graph']['art4_C']['pe']));
	}
	if (isset($context['results']['graph']['art4_NC']['number'])) {
    	$sheet->setCellValue('B47', $context['results']['graph']['art4_NC']['number']);
	}
	if (isset($context['results']['graph']['art4_NC']['pe'])) {
    	$sheet->setCellValue('C47', round($context['results']['graph']['art4_NC']['pe']));
	}
	if (isset($context['results']['graph']['art4_NR']['number'])) {
    	$sheet->setCellValue('B48', $context['results']['graph']['art4_NR']['number']);
	}
	if (isset($context['results']['graph']['art4_NR']['pe'])) {
    	$sheet->setCellValue('C48', round($context['results']['graph']['art4_NR']['pe']));
	}
	if (isset($context['results']['graph']['art5_C']['number'])) {
    	$sheet->setCellValue('B49', $context['results']['graph']['art5_C']['number']);
	}
	if (isset($context['results']['graph']['art5_C']['pe'])) {
    	$sheet->setCellValue('C49', round($context['results']['graph']['art5_C']['pe']));
	}
	if (isset($context['results']['graph']['art5_NC']['number'])) {
    	$sheet->setCellValue('B50', $context['results']['graph']['art5_NC']['number']);
	}
	if (isset($context['results']['graph']['art5_NC']['pe'])) {
    	$sheet->setCellValue('C50', round($context['results']['graph']['art5_NC']['pe']));
	}
	if (isset($context['results']['graph']['art5_NR']['number'])) {
    	$sheet->setCellValue('B51', $context['results']['graph']['art5_NR']['number']);
	}
	if (isset($context['results']['graph']['art5_NR']['pe'])) {
    	$sheet->setCellValue('C51', round($context['results']['graph']['art5_NR']['pe']));
	}

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
    $sheet->setCellValue('D49', uwwtd_register_get_cell_value($previoussheet, 'B49'));
    $sheet->setCellValue('E49', uwwtd_register_get_cell_value($previoussheet, 'C49'));
    $sheet->setCellValue('D50', uwwtd_register_get_cell_value($previoussheet, 'B50'));
    $sheet->setCellValue('E50', uwwtd_register_get_cell_value($previoussheet, 'C50'));
    $sheet->setCellValue('D51', uwwtd_register_get_cell_value($previoussheet, 'B51'));
    $sheet->setCellValue('E51', uwwtd_register_get_cell_value($previoussheet, 'C51'));

    //previous_n2
    $sheet->setCellValue('F43', uwwtd_register_get_cell_value($previoussheet_n2, 'B43'));
    $sheet->setCellValue('G43', uwwtd_register_get_cell_value($previoussheet_n2, 'C43'));
    $sheet->setCellValue('F44', uwwtd_register_get_cell_value($previoussheet_n2, 'B44'));
    $sheet->setCellValue('G44', uwwtd_register_get_cell_value($previoussheet_n2, 'C44'));
    $sheet->setCellValue('F45', uwwtd_register_get_cell_value($previoussheet_n2, 'B45'));
    $sheet->setCellValue('G45', uwwtd_register_get_cell_value($previoussheet_n2, 'C45'));
    $sheet->setCellValue('F46', uwwtd_register_get_cell_value($previoussheet_n2, 'B46'));
    $sheet->setCellValue('G46', uwwtd_register_get_cell_value($previoussheet_n2, 'C46'));
    $sheet->setCellValue('F47', uwwtd_register_get_cell_value($previoussheet_n2, 'B47'));
    $sheet->setCellValue('G47', uwwtd_register_get_cell_value($previoussheet_n2, 'C47'));
    $sheet->setCellValue('F48', uwwtd_register_get_cell_value($previoussheet_n2, 'B48'));
    $sheet->setCellValue('G48', uwwtd_register_get_cell_value($previoussheet_n2, 'C48'));
    $sheet->setCellValue('F49', uwwtd_register_get_cell_value($previoussheet_n2, 'B49'));
    $sheet->setCellValue('G49', uwwtd_register_get_cell_value($previoussheet_n2, 'C49'));
    $sheet->setCellValue('F50', uwwtd_register_get_cell_value($previoussheet_n2, 'B50'));
    $sheet->setCellValue('G50', uwwtd_register_get_cell_value($previoussheet_n2, 'C50'));
    $sheet->setCellValue('F51', uwwtd_register_get_cell_value($previoussheet_n2, 'B51'));
    $sheet->setCellValue('G51', uwwtd_register_get_cell_value($previoussheet_n2, 'C51'));


    //array 6
    if (isset($context['results']['graph']['art3_C']['pe'])) {
    	$sheet->setCellValue('B54', round($context['results']['graph']['art3_C']['pe']));
    }
    if (isset($context['results']['graph']['art3_NC']['pe'])) {
    	$sheet->setCellValue('C54', round($context['results']['graph']['art3_NC']['pe']));
    }
    if (isset($context['results']['graph']['art3_NR']['pe'])) {
    	$sheet->setCellValue('D54', round($context['results']['graph']['art3_NR']['pe']));
    }
    if (isset($context['results']['graph']['art4_C']['pe'])) {
    	$sheet->setCellValue('B57', round($context['results']['graph']['art4_C']['pe']));
    }
    if (isset($context['results']['graph']['art4_NC']['pe'])) {
    	$sheet->setCellValue('C57', round($context['results']['graph']['art4_NC']['pe']));
    }
    if (isset($context['results']['graph']['art4_NR']['pe'])) {
    	$sheet->setCellValue('D57', round($context['results']['graph']['art4_NR']['pe']));
    }
    if (isset($context['results']['graph']['art5_C']['pe'])) {
    	$sheet->setCellValue('B60', round($context['results']['graph']['art5_C']['pe']));
    }
    if (isset($context['results']['graph']['art5_NC']['pe'])) {
    	$sheet->setCellValue('C60', round($context['results']['graph']['art5_NC']['pe']));
    }
    if (isset($context['results']['graph']['art5_NR']['pe'])) {
    	$sheet->setCellValue('D60', round($context['results']['graph']['art5_NR']['pe']));
    }

    //previous
    $sheet->setCellValue('B55', uwwtd_register_get_cell_value($previoussheet, 'E55'));
    $sheet->setCellValue('C55', uwwtd_register_get_cell_value($previoussheet, 'F55'));
    $sheet->setCellValue('D55', uwwtd_register_get_cell_value($previoussheet, 'G55'));
    $sheet->setCellValue('B58', uwwtd_register_get_cell_value($previoussheet, 'E58'));
    $sheet->setCellValue('C58', uwwtd_register_get_cell_value($previoussheet, 'F58'));
    $sheet->setCellValue('D58', uwwtd_register_get_cell_value($previoussheet, 'G58'));
    $sheet->setCellValue('B61', uwwtd_register_get_cell_value($previoussheet, 'E61'));
    $sheet->setCellValue('C61', uwwtd_register_get_cell_value($previoussheet, 'F61'));
    $sheet->setCellValue('D61', uwwtd_register_get_cell_value($previoussheet, 'G61'));

    //previous_n2
    //=========nd@oieau.fr===17/12/2020========== FIX ME
    $sheet->setCellValue('B56', uwwtd_register_get_cell_value($previoussheet_n2, 'E55'));
    $sheet->setCellValue('C56', uwwtd_register_get_cell_value($previoussheet_n2, 'F55'));
    $sheet->setCellValue('D56', uwwtd_register_get_cell_value($previoussheet_n2, 'G55'));
    $sheet->setCellValue('B59', uwwtd_register_get_cell_value($previoussheet_n2, 'E58'));
    $sheet->setCellValue('C59', uwwtd_register_get_cell_value($previoussheet_n2, 'F58'));
    $sheet->setCellValue('D59', uwwtd_register_get_cell_value($previoussheet_n2, 'G58'));
    $sheet->setCellValue('B62', uwwtd_register_get_cell_value($previoussheet_n2, 'E61'));
    $sheet->setCellValue('C62', uwwtd_register_get_cell_value($previoussheet_n2, 'F61'));
    $sheet->setCellValue('D62', uwwtd_register_get_cell_value($previoussheet_n2, 'G61'));


    //array 7
    if (isset($context['results']['graph']['art3_C']['number'])) {
    	$sheet->setCellValue('B64', $context['results']['graph']['art3_C']['number']);
    }
    if (isset($context['results']['graph']['art3_NC']['number'])) {
    	$sheet->setCellValue('C64', $context['results']['graph']['art3_NC']['number']);
	}
	if (isset($context['results']['graph']['art3_NR']['number'])) {
    	$sheet->setCellValue('D64', $context['results']['graph']['art3_NR']['number']);
	}
	if (isset($context['results']['graph']['art4_C']['number'])) {
	    $sheet->setCellValue('B67', $context['results']['graph']['art4_C']['number']);
	}
	if (isset($context['results']['graph']['art4_NC']['number'])) {
    	$sheet->setCellValue('C67', $context['results']['graph']['art4_NC']['number']);
	}
	if (isset($context['results']['graph']['art4_NR']['number'])) {
    	$sheet->setCellValue('D67', $context['results']['graph']['art4_NR']['number']);
	}
	if (isset($context['results']['graph']['art5_C']['number'])) {
    	$sheet->setCellValue('B70', $context['results']['graph']['art5_C']['number']);
	}
	if (isset($context['results']['graph']['art5_NC']['number'])) {
    	$sheet->setCellValue('C70', $context['results']['graph']['art5_NC']['number']);
	}
	if (isset($context['results']['graph']['art5_NR']['number'])) {
    	$sheet->setCellValue('D70', $context['results']['graph']['art5_NR']['number']);
	}

    //previous
    $sheet->setCellValue('B65', uwwtd_register_get_cell_value($previoussheet, 'E65'));
    $sheet->setCellValue('C65', uwwtd_register_get_cell_value($previoussheet, 'F65'));
    $sheet->setCellValue('D65', uwwtd_register_get_cell_value($previoussheet, 'G65'));
    $sheet->setCellValue('B68', uwwtd_register_get_cell_value($previoussheet, 'E68'));
    $sheet->setCellValue('C68', uwwtd_register_get_cell_value($previoussheet, 'F68'));
    $sheet->setCellValue('D68', uwwtd_register_get_cell_value($previoussheet, 'G68'));
    $sheet->setCellValue('B71', uwwtd_register_get_cell_value($previoussheet, 'E71'));
    $sheet->setCellValue('C71', uwwtd_register_get_cell_value($previoussheet, 'F71'));
    $sheet->setCellValue('D71', uwwtd_register_get_cell_value($previoussheet, 'G71'));
    //previous_n2
    $sheet->setCellValue('B66', uwwtd_register_get_cell_value($previoussheet_n2, 'E65'));
    $sheet->setCellValue('C66', uwwtd_register_get_cell_value($previoussheet_n2, 'F65'));
    $sheet->setCellValue('D66', uwwtd_register_get_cell_value($previoussheet_n2, 'G65'));
    $sheet->setCellValue('B69', uwwtd_register_get_cell_value($previoussheet_n2, 'E68'));
    $sheet->setCellValue('C69', uwwtd_register_get_cell_value($previoussheet_n2, 'F68'));
    $sheet->setCellValue('D69', uwwtd_register_get_cell_value($previoussheet_n2, 'G68'));
    $sheet->setCellValue('B72', uwwtd_register_get_cell_value($previoussheet_n2, 'E71'));
    $sheet->setCellValue('C72', uwwtd_register_get_cell_value($previoussheet_n2, 'F71'));
    $sheet->setCellValue('D72', uwwtd_register_get_cell_value($previoussheet_n2, 'G71'));


    //array 3
    $nidms = uwwtd_check_exist(uwwtd_get_siteid('ms_level', array('year' => $annee)));
    if ('' != $nidms) {
        $node = node_load($nidms);

        //Yearly production of sludge
        $field_mslsludgeproduction = isset($node->field_mslsludgeproduction['und'][0]['value']) ? $node->field_mslsludgeproduction['und'][0]['value'] : 0;
        // Discharged into surface waters : Pipelines
        $field_msldischargepipelines = isset($node->field_msldischargepipelines['und'][0]['value']) ? $node->field_msldischargepipelines['und'][0]['value'] : 0;
        // Discharged into surface waters : Ships
        $field_msldischargeships = isset($node->field_msldischargeships['und'][0]['value']) ? $node->field_msldischargeships['und'][0]['value'] : 0;
        // Discharged into surface waters : Others
        $field_msldischargeothers = isset($node->field_msldischargeothers['und'][0]['value']) ? $node->field_msldischargeothers['und'][0]['value'] : 0;
        //Of which: re-used: Soil and agriculture
        $field_mslreusesoilagriculture = isset($node->field_mslreusesoilagriculture['und'][0]['value']) ? $node->field_mslreusesoilagriculture['und'][0]['value'] : 0;
        //Of which: re-used: Others
        $field_mslreuseothers = isset($node->field_mslreuseothers['und'][0]['value']) ? $node->field_mslreuseothers['und'][0]['value'] : 0;
        //Of which: disposed: Landfill
        $field_msldisposallandfill = isset($node->field_msldisposallandfill['und'][0]['value']) ? $node->field_msldisposallandfill['und'][0]['value'] : 0;
        //Of which: disposed: Incineration
        $field_msldisposalincineration = isset($node->field_msldisposalincineration['und'][0]['value']) ? $node->field_msldisposalincineration['und'][0]['value'] : 0;
        //Of which: disposed: Others
        $field_msldisposalothers = isset($node->field_msldisposalothers['und'][0]['value']) ? $node->field_msldisposalothers['und'][0]['value'] : 0;
        //reuse percent
        $field_mslwwreuseperc = isset($node->field_mslwwreuseperc['und'][0]['value']) ? $node->field_mslwwreuseperc['und'][0]['value'] : 0;
        unset($node);

        $field_others = $field_msldischargepipelines + $field_msldischargeships + $field_msldischargeothers;

        $sheet->setCellValue('B24', round($field_mslsludgeproduction));
        $sheet->setCellValue('B25', round($field_mslreusesoilagriculture));
        $sheet->setCellValue('B26', round($field_mslreuseothers));
        $sheet->setCellValue('B27', round($field_msldisposallandfill));
        $sheet->setCellValue('B28', round($field_msldisposalincineration));
        $sheet->setCellValue('B29', round($field_msldisposalothers));
        $sheet->setCellValue('B30', round($field_others));

//debug to comment
// dsm('delete all debug value B24 ...');
// $sheet->setCellValue('B24', 603142);
//debug to comment

        if (!empty($field_mslsludgeproduction)) {
            if (!empty($field_mslreusesoilagriculture)) {
                $sheet->setCellValue('C25', round($field_mslreusesoilagriculture * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_mslreuseothers)) {
                $sheet->setCellValue('C26', round($field_mslreuseothers * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_msldisposallandfill)) {
                $sheet->setCellValue('C27', round($field_msldisposallandfill * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_msldisposalincineration)) {
                $sheet->setCellValue('C28', round($field_msldisposalincineration * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_msldisposalothers)) {
                $sheet->setCellValue('C29', round($field_msldisposalothers * 100 / $field_mslsludgeproduction, 1));
            }
            if (!empty($field_others)) {
                $sheet->setCellValue('C30', round($field_others * 100 / $field_mslsludgeproduction, 1));
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
        
        //previous_2
        $sheet->setCellValue('F24', uwwtd_register_get_cell_value($previoussheet_n2, 'B24'));
        $sheet->setCellValue('F25', uwwtd_register_get_cell_value($previoussheet_n2, 'B25'));
        $sheet->setCellValue('F26', uwwtd_register_get_cell_value($previoussheet_n2, 'B26'));
        $sheet->setCellValue('F27', uwwtd_register_get_cell_value($previoussheet_n2, 'B27'));
        $sheet->setCellValue('F28', uwwtd_register_get_cell_value($previoussheet_n2, 'B28'));
        $sheet->setCellValue('F29', uwwtd_register_get_cell_value($previoussheet_n2, 'B29'));

        $sheet->setCellValue('G25', uwwtd_register_get_cell_value($previoussheet_n2, 'C25'));
        $sheet->setCellValue('G26', uwwtd_register_get_cell_value($previoussheet_n2, 'C26'));
        $sheet->setCellValue('G27', uwwtd_register_get_cell_value($previoussheet_n2, 'C27'));
        $sheet->setCellValue('G28', uwwtd_register_get_cell_value($previoussheet_n2, 'C28'));
        $sheet->setCellValue('G29', uwwtd_register_get_cell_value($previoussheet_n2, 'C29'));

        //array 4
        if (isset($context['results']['country_volume_waste_water'])) {
        	$sheet->setCellValue('C36', $context['results']['country_volume_waste_water']);
        }
        $sheet->setCellValue('C37', $field_mslwwreuseperc);

        //previous
        $sheet->setCellValue('D36', uwwtd_register_get_cell_value($previoussheet, 'C36'));
        $sheet->setCellValue('D37', uwwtd_register_get_cell_value($previoussheet, 'C37'));

        //previous_n2
        $sheet->setCellValue('E36', uwwtd_register_get_cell_value($previoussheet_n2, 'C36'));
        $sheet->setCellValue('E37', uwwtd_register_get_cell_value($previoussheet_n2, 'C37'));


    }

    $cell = $sheet->getCell('A75');
    $cell->setValue(str_replace(array('[#COUNTRY#]', '[#current_year#]'), array($country_name, $annee), $cell->getValue()));

    if (isset($context['results']['graph']['summary_installation']['total']['number'])) {
    	$sheet->getCell('B77')->setValue($context['results']['graph']['summary_installation']['total']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['total']['percent'])) {
    	$sheet->getCell('C77')->setValue(round($context['results']['graph']['summary_installation']['total']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['waste_pe'])) {
    	$sheet->getCell('D77')->setValue(round($context['results']['graph']['summary_installation']['total']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['waste_percent'])) {
    	$sheet->getCell('E77')->setValue(round($context['results']['graph']['summary_installation']['total']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['number'])) {
    	$sheet->getCell('B78')->setValue($context['results']['graph']['summary_installation']['total']['collecting_system']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['percent'])) {
    	$sheet->getCell('C78')->setValue(round($context['results']['graph']['summary_installation']['total']['collecting_system']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_pe'])) {
    	$sheet->getCell('D78')->setValue(round($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_percent'])) {
    	$sheet->getCell('E78')->setValue(round($context['results']['graph']['summary_installation']['total']['collecting_system']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['number'])) {
    	$sheet->getCell('B79')->setValue($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['percent'])) {
    	$sheet->getCell('C79')->setValue(round($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_pe'])) {
    	$sheet->getCell('D79')->setValue(round($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_percent'])) {
    	$sheet->getCell('E79')->setValue(round($context['results']['graph']['summary_installation']['secondary']['installation_in_place']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['number'])) {
    	$sheet->getCell('B80')->setValue($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['percent'])) {
    	$sheet->getCell('C80')->setValue(round($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_pe'])) {
    	$sheet->getCell('D80')->setValue(round($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_percent'])) {
    	$sheet->getCell('E80')->setValue(round($context['results']['graph']['summary_installation']['secondary']['meet_requirement']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['number'])) {
    	$sheet->getCell('B81')->setValue($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['percent'])) {
    	$sheet->getCell('C81')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_pe'])) {
    	$sheet->getCell('D81')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_percent'])) {
    	$sheet->getCell('E81')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['installation_in_place']['waste_percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['number'])) {
    	$sheet->getCell('B82')->setValue($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['number']);
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['percent'])) {
    	$sheet->getCell('C82')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['percent'], 1));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_pe'])) {
    	$sheet->getCell('D82')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_pe']));
    }
    if (isset($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_percent'])) {
    	$sheet->getCell('E82')->setValue(round($context['results']['graph']['summary_installation']['more_stringent']['meet_requirement']['waste_percent'], 1));
    }

    //previous
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

    $sheet->setCellValue('F81', uwwtd_register_get_cell_value($previoussheet, 'B81'));
    $sheet->setCellValue('G81', uwwtd_register_get_cell_value($previoussheet, 'C81'));
    $sheet->setCellValue('H81', uwwtd_register_get_cell_value($previoussheet, 'D81'));
    $sheet->setCellValue('I81', uwwtd_register_get_cell_value($previoussheet, 'E81'));

    $sheet->setCellValue('F82', uwwtd_register_get_cell_value($previoussheet, 'B82'));
    $sheet->setCellValue('G82', uwwtd_register_get_cell_value($previoussheet, 'C82'));
    $sheet->setCellValue('H82', uwwtd_register_get_cell_value($previoussheet, 'D82'));
    $sheet->setCellValue('I82', uwwtd_register_get_cell_value($previoussheet, 'E82'));
    /*
    //previous_n2
    $sheet->setCellValue('F77', uwwtd_register_get_cell_value($previoussheet_n2, 'B77'));
    $sheet->setCellValue('G77', uwwtd_register_get_cell_value($previoussheet_n2, 'C77'));
    $sheet->setCellValue('H77', uwwtd_register_get_cell_value($previoussheet_n2, 'D77'));
    $sheet->setCellValue('I77', uwwtd_register_get_cell_value($previoussheet_n2, 'E77'));

    $sheet->setCellValue('F78', uwwtd_register_get_cell_value($previoussheet_n2, 'B78'));
    $sheet->setCellValue('G78', uwwtd_register_get_cell_value($previoussheet_n2, 'C78'));
    $sheet->setCellValue('H78', uwwtd_register_get_cell_value($previoussheet_n2, 'D78'));
    $sheet->setCellValue('I78', uwwtd_register_get_cell_value($previoussheet_n2, 'E78'));

    $sheet->setCellValue('F79', uwwtd_register_get_cell_value($previoussheet_n2, 'B79'));
    $sheet->setCellValue('G79', uwwtd_register_get_cell_value($previoussheet_n2, 'C79'));
    $sheet->setCellValue('H79', uwwtd_register_get_cell_value($previoussheet_n2, 'D79'));
    $sheet->setCellValue('I79', uwwtd_register_get_cell_value($previoussheet_n2, 'E79'));

    $sheet->setCellValue('F80', uwwtd_register_get_cell_value($previoussheet_n2, 'B80'));
    $sheet->setCellValue('G80', uwwtd_register_get_cell_value($previoussheet_n2, 'C80'));
    $sheet->setCellValue('H80', uwwtd_register_get_cell_value($previoussheet_n2, 'D80'));
    $sheet->setCellValue('I80', uwwtd_register_get_cell_value($previoussheet_n2, 'E80'));

    $sheet->setCellValue('F81', uwwtd_register_get_cell_value($previoussheet_n2, 'B81'));
    $sheet->setCellValue('G81', uwwtd_register_get_cell_value($previoussheet_n2, 'C81'));
    $sheet->setCellValue('H81', uwwtd_register_get_cell_value($previoussheet_n2, 'D81'));
    $sheet->setCellValue('I81', uwwtd_register_get_cell_value($previoussheet_n2, 'E81'));

    $sheet->setCellValue('F82', uwwtd_register_get_cell_value($previoussheet_n2, 'B82'));
    $sheet->setCellValue('G82', uwwtd_register_get_cell_value($previoussheet_n2, 'C82'));
    $sheet->setCellValue('H82', uwwtd_register_get_cell_value($previoussheet_n2, 'D82'));
    $sheet->setCellValue('I82', uwwtd_register_get_cell_value($previoussheet_n2, 'E82'));
*/
    //array 9 Summary distance to compliance expired deadline
    //Target connection pe
    if (isset($context['results']['distance_to_compliance']['expired']['target_connection']['total_load_pe'])) {
    	$sheet->getCell('C88')->setValue(round($context['results']['distance_to_compliance']['expired']['target_connection']['total_load_pe']));
    }
    //not connected
    if (isset($context['results']['distance_to_compliance']['expired']['target_connection']['distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['target_connection']['distance_pe'] == 0) ? $sheet->getCell('C89')->setValue(0) : $sheet->getCell('B89')->setValue(round($context['results']['distance_to_compliance']['expired']['target_connection']['distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['target_connection']['distance_pe'])) {
    	$sheet->getCell('C89')->setValue(round($context['results']['distance_to_compliance']['expired']['target_connection']['distance_pe']));
    }
    //2nd treatment target
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['total_load_pe'])) {
    	$sheet->getCell('C90')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['total_load_pe']));
    }
    //2nd treatment not in place
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_pe'] == 0) ? $sheet->getCell('B91')->setValue(0) : $sheet->getCell('B91')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_pe'])) {
    	$sheet->getCell('C91')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['treatement_inplace_distance_pe']));
    }
    //2nd treatment performance not met
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B92')->setValue(0) : $sheet->getCell('B92')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C92')->setValue(round($context['results']['distance_to_compliance']['expired']['secondary_target']['monitoring_result_distance_pe']));
    }
    //Tertiary treatment target
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['total_load_pe'])) {
    	$sheet->getCell('C93')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['total_load_pe']));
    }
    //Tertiary treatment not in place
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_pe'] == 0) ? $sheet->getCell('B94')->setValue(0) : $sheet->getCell('B94')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_pe'])) {
    	$sheet->getCell('C94')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['installation_inplace_distance_pe']));
    }
    //Tertiary stringent treatment performance not met
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B95')->setValue(0) : $sheet->getCell('B95')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C95')->setValue(round($context['results']['distance_to_compliance']['expired']['morestringent_target']['monitoring_result_distance_pe']));
    }

    //previous
    //FIX ME =======nd@oieau.fr
    $sheet->setCellValue('E88', uwwtd_register_get_cell_value($previoussheet, 'C88'));
    //not connected
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

    //previous_n2
    $sheet->setCellValue('G88', uwwtd_register_get_cell_value($previoussheet_n2, 'C88'));
    //not EonneEted
    $sheet->setCellValue('F89', uwwtd_register_get_cell_value($previoussheet_n2, 'B89'));
    $sheet->setCellValue('G89', uwwtd_register_get_cell_value($previoussheet_n2, 'C89'));
    //2nd treatment targe                                                      arg
    $sheet->setCellValue('G90', uwwtd_register_get_cell_value($previoussheet_n2, 'C90'));
    //2nd treatment not i                                                      ot
    $sheet->setCellValue('F91', uwwtd_register_get_cell_value($previoussheet_n2, 'B91'));
    $sheet->setCellValue('G91', uwwtd_register_get_cell_value($previoussheet_n2, 'C91'));
    //2nd treatment perfo                                                      erf
    $sheet->setCellValue('F92', uwwtd_register_get_cell_value($previoussheet_n2, 'B92'));
    $sheet->setCellValue('G92', uwwtd_register_get_cell_value($previoussheet_n2, 'C92'));
    //Tertiary treatment                                                       ent
    $sheet->setCellValue('G93', uwwtd_register_get_cell_value($previoussheet_n2, 'C93'));
    //Tertiary treatment                                                       ent
    $sheet->setCellValue('F94', uwwtd_register_get_cell_value($previoussheet_n2, 'B94'));
    $sheet->setCellValue('G94', uwwtd_register_get_cell_value($previoussheet_n2, 'C94'));
    //Tertiary stringent                                                       ent
    $sheet->setCellValue('F95', uwwtd_register_get_cell_value($previoussheet_n2, 'B95'));
    $sheet->setCellValue('G95', uwwtd_register_get_cell_value($previoussheet_n2, 'C95'));


    //array 10 summary distance to compliance pending deadlines
    if (isset($context['results']['distance_to_compliance']['pending']['target_connection']['total_load_pe'])) {
    	$sheet->getCell('C104')->setValue(round($context['results']['distance_to_compliance']['pending']['target_connection']['total_load_pe']));
    }
    //not connected
    if (isset($context['results']['distance_to_compliance']['pending']['target_connection']['distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['target_connection']['distance_pe'] == 0) ? $sheet->getCell('B105')->setValue(0) : $sheet->getCell('B105')->setValue(round($context['results']['distance_to_compliance']['pending']['target_connection']['distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['target_connection']['distance_pe'])) {
    	$sheet->getCell('C105')->setValue(round($context['results']['distance_to_compliance']['pending']['target_connection']['distance_pe']));
    }
    //2nd treatment target
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['total_load_pe'])) {
    	$sheet->getCell('C106')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['total_load_pe']));
    }
    //2nd treatment not in place
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_pe'] == 0) ? $sheet->getCell('B107')->setValue(0) : $sheet->getCell('B107')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_pe'])) {
    	$sheet->getCell('C107')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['treatement_inplace_distance_pe']));
    }
    //2nd treatment performance not met
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B108')->setValue(0) : $sheet->getCell('B108')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C108')->setValue(round($context['results']['distance_to_compliance']['pending']['secondary_target']['monitoring_result_distance_pe']));
    }
    //Tertiary treatment target
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['total_load_pe'])) {
    	$sheet->getCell('C109')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['total_load_pe']));
    }
    //Tertiary treatment not in place
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_pe'] == 0) ? $sheet->getCell('B110')->setValue(0) : $sheet->getCell('B110')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_pe'])) {
    	$sheet->getCell('C110')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['installation_inplace_distance_pe']));
    }
    //Tertiary stringent treatment performance not met
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_%'])) {
    	($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_pe'] == 0) ? $sheet->getCell('B111')->setValue(0) : $sheet->getCell('B111')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C111')->setValue(round($context['results']['distance_to_compliance']['pending']['morestringent_target']['monitoring_result_distance_pe']));
    }

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
   
   //previous_n2
    $sheet->setCellValue('G104', uwwtd_register_get_cell_value($previoussheet_n2, 'C104'));
    //not connected
    $sheet->setCellValue('F105', uwwtd_register_get_cell_value($previoussheet_n2, 'B105'));
    $sheet->setCellValue('G105', uwwtd_register_get_cell_value($previoussheet_n2, 'C105'));
    //2nd treatment target                                                      arge
    $sheet->setCellValue('F106', uwwtd_register_get_cell_value($previoussheet_n2, 'C106'));
    //2nd treatment not in                                                      ot i
    $sheet->setCellValue('F107', uwwtd_register_get_cell_value($previoussheet_n2, 'B107'));
    $sheet->setCellValue('G107', uwwtd_register_get_cell_value($previoussheet_n2, 'C107'));
    //2nd treatment perfor                                                      erfo
    $sheet->setCellValue('F108', uwwtd_register_get_cell_value($previoussheet_n2, 'B108'));
    $sheet->setCellValue('G108', uwwtd_register_get_cell_value($previoussheet_n2, 'C108'));
    //Tertiary treatment t                                                      ent
    $sheet->setCellValue('G109', uwwtd_register_get_cell_value($previoussheet_n2, 'C109'));
    //Tertiary treatment n                                                      ent
    $sheet->setCellValue('F110', uwwtd_register_get_cell_value($previoussheet_n2, 'B110'));
    $sheet->setCellValue('G110', uwwtd_register_get_cell_value($previoussheet, 'C110'));
    //Tertiary stringent t                                                      ent
    $sheet->setCellValue('F111', uwwtd_register_get_cell_value($previoussheet_n2, 'B111'));
    $sheet->setCellValue('G111', uwwtd_register_get_cell_value($previoussheet_n2, 'C111'));

    //array 11 summary distance to compliance total
    if (isset($context['results']['distance_to_compliance']['total']['target_connection']['total_load_pe'])) {
    	$sheet->getCell('C119')->setValue(round($context['results']['distance_to_compliance']['total']['target_connection']['total_load_pe']));
    }
    //not connected
    //nd@oieau.fr : FIX ME this cell is empty
    if (isset($context['results']['distance_to_compliance']['total']['target_connection']['distance_%'])) {
    	$sheet->getCell('B120')->setValue(round($context['results']['distance_to_compliance']['total']['target_connection']['distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['target_connection']['distance_pe'])) {
    	$sheet->getCell('C120')->setValue(round($context['results']['distance_to_compliance']['total']['target_connection']['distance_pe']));
    }
    //2nd treatment target
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['total_load_pe'])) {
    	$sheet->getCell('C121')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['total_load_pe']));
    }
    //2nd treatment not in place
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_%'])) {
    	$sheet->getCell('B122')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_pe'])) {
    	$sheet->getCell('C122')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['treatement_inplace_distance_pe']));
    }
    //2nd treatment performance not met
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_%'])) {
    	$sheet->getCell('B123')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C123')->setValue(round($context['results']['distance_to_compliance']['total']['secondary_target']['monitoring_result_distance_pe']));
    }
    //Tertiary treatment target
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['total_load_pe'])) {
    	$sheet->getCell('C124')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['total_load_pe']));
    }
    //Tertiary treatment not in place
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_%'])) {
    	$sheet->getCell('B125')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_pe'])) {
    	$sheet->getCell('C125')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['installation_inplace_distance_pe']));
    }
    //Tertiary stringent treatment performance not met
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_%'])) {
    	$sheet->getCell('B126')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_%'], 1));
    }
    if (isset($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_pe'])) {
    	$sheet->getCell('C126')->setValue(round($context['results']['distance_to_compliance']['total']['morestringent_target']['monitoring_result_distance_pe']));
    }

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
    
   //previous_n2
    $sheet->setCellValue('G119', uwwtd_register_get_cell_value($previoussheet_n2, 'C119'));
    //not connected
    $sheet->setCellValue('F120', uwwtd_register_get_cell_value($previoussheet_n2, 'B120'));
    $sheet->setCellValue('G120', uwwtd_register_get_cell_value($previoussheet_n2, 'C120'));
    //2nd treatment target                                                      arge
    $sheet->setCellValue('G121', uwwtd_register_get_cell_value($previoussheet_n2, 'C121'));
    //2nd treatment not in                                                      ot i
    $sheet->setCellValue('F122', uwwtd_register_get_cell_value($previoussheet_n2, 'B122'));
    $sheet->setCellValue('G122', uwwtd_register_get_cell_value($previoussheet_n2, 'C122'));
    //2nd treatment perfor                                                      erfo
    $sheet->setCellValue('F123', uwwtd_register_get_cell_value($previoussheet_n2, 'B123'));
    $sheet->setCellValue('G123', uwwtd_register_get_cell_value($previoussheet_n2, 'C123'));
    //Tertiary treatment t                                                      ent
    $sheet->setCellValue('G124', uwwtd_register_get_cell_value($previoussheet_n2, 'C124'));
    //Tertiary treatment n                                                      ent
    $sheet->setCellValue('F125', uwwtd_register_get_cell_value($previoussheet_n2, 'B125'));
    $sheet->setCellValue('G125', uwwtd_register_get_cell_value($previoussheet_n2, 'C125'));
    //Tertiary stringent t                                                      ent
    $sheet->setCellValue('F126', uwwtd_register_get_cell_value($previoussheet_n2, 'B126'));
    $sheet->setCellValue('G126', uwwtd_register_get_cell_value($previoussheet_n2, 'C126'));

    //array 12  treatment details
    // Parameters   reference year
    //  %   p.e.    number of plants
    // Total load generated 100,00% 5 164 016
    // No treatment (not connected + no treatment after connection) 5,11%   264097      sum of ((neither collecting system nor IAS)+(collected in collecting system-entering load)+(entering load for all stations with compliance article 4 =NC))
    // IAS  0,00%   0       sum of adressed through IAS
    // primary treatment as a maximum   0,23%   11779       sum of entering load for all stations with treatment type in place = 1
    // secondary treatment only as a maximum (performance met)  25,22%  1302236     sum of entering load for all stations with treatment type in place = 2
    // N removal without P removal (performance met)    6,91%   356636      sum of entering load for all stations with treatment type in place = 3N and performance Ntot=pass or NR
    // P removal without N removal (performance met)    1,44%   74542       sum of entering load for all stations with treatment type in place = 3P and performance Ptot=pass or NR
    // N and P removal (performance met)    0,40%   20813       sum of entering load for all stations with treatment type in place = 3NP and performance Ntot=pass or NR & Ptot=pass or NR
    // Other more stringent treatments  60,69%  3133913     sum of entering load for all stations with treatment type in place = 3O only (no 3N, no 3NP, no P)
    //todo
//         $context['results']['uwwtp_total_load_generated_pe'] = 0;
//         $context['results']['uwwtp_notreatment_pe'] = 0;
//         $context['results']['uwwtp_ias_pe'] = 0;
//         $context['results']['uwwtp_p_as_max_pe'] = 0;
//         $context['results']['uwwtp_s_as_max_pe'] = 0;
//         $context['results']['uwwtp_n_removal_without_p_pe'] = 0;
//         $context['results']['uwwtp_p_removal_without_n_pe'] = 0;
//         $context['results']['uwwtp_np_removal_pe'] = 0;
//         $context['results']['uwwtp_other_morestringent_pe'] = 0;

    $sheet->setCellValue('B131', '100.00');
    if (isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('C131', round($context['results']['uwwtp_total_load_generated_pe']));
    }
    if (isset($context['results']['uwwtp_total_load_generated_nb'])) {
    	$sheet->setCellValue('D131', $context['results']['uwwtp_total_load_generated_nb']);
    }
    if (isset($context['results']['uwwtp_notreatment_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B132', round($context['results']['uwwtp_notreatment_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_notreatment_pe'])) {
    	$sheet->setCellValue('C132', round($context['results']['uwwtp_notreatment_pe']));
    }
    if (isset($context['results']['uwwtp_notreatment_nb'])) {
    	$sheet->setCellValue('D132', $context['results']['uwwtp_notreatment_nb']);
    }
    



    
    
        



    if (isset($context['results']['uwwtp_ias_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B133', round($context['results']['uwwtp_ias_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_ias_pe'])) {
    	$sheet->setCellValue('C133', round($context['results']['uwwtp_ias_pe']));
    }
    if (isset($context['results']['uwwtp_ias_nb'])) {
    	$sheet->setCellValue('D133', $context['results']['uwwtp_ias_nb']);
    }
    if (isset($context['results']['uwwtp_p_as_max_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B134', round($context['results']['uwwtp_p_as_max_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_p_as_max_pe'])) {
    	$sheet->setCellValue('C134', round($context['results']['uwwtp_p_as_max_pe']));
    }
    if (isset($context['results']['uwwtp_p_as_max_nb'])) {
    	$sheet->setCellValue('D134', $context['results']['uwwtp_p_as_max_nb']);
    }
    if (isset($context['results']['uwwtp_s_as_max_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B135', round($context['results']['uwwtp_s_as_max_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_s_as_max_pe'])) {
    	$sheet->setCellValue('C135', round($context['results']['uwwtp_s_as_max_pe']));
    }
    if (isset($context['results']['uwwtp_s_as_max_nb'])) {
    	$sheet->setCellValue('D135', $context['results']['uwwtp_s_as_max_nb']);
    }
    if (isset($context['results']['uwwtp_n_removal_without_p_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B136', round($context['results']['uwwtp_n_removal_without_p_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_n_removal_without_p_pe'])) {
    	$sheet->setCellValue('C136', round($context['results']['uwwtp_n_removal_without_p_pe']));
    }
    if (isset($context['results']['uwwtp_n_removal_without_p_nb'])) {
    	$sheet->setCellValue('D136', $context['results']['uwwtp_n_removal_without_p_nb']);
    }
    if (isset($context['results']['uwwtp_p_removal_without_n_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B137', round($context['results']['uwwtp_p_removal_without_n_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_p_removal_without_n_pe'])) {
    	$sheet->setCellValue('C137', round($context['results']['uwwtp_p_removal_without_n_pe']));
    }
    if (isset($context['results']['uwwtp_p_removal_without_n_nb'])) {
    	$sheet->setCellValue('D137', $context['results']['uwwtp_p_removal_without_n_nb']);
    }
    if (isset($context['results']['uwwtp_np_removal_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B138', round($context['results']['uwwtp_np_removal_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_np_removal_pe'])) {
    	$sheet->setCellValue('C138', round($context['results']['uwwtp_np_removal_pe']));
    }
    if (isset($context['results']['uwwtp_np_removal_nb'])) {
    	$sheet->setCellValue('D138', $context['results']['uwwtp_np_removal_nb']);
    }
    if (isset($context['results']['uwwtp_other_morestringent_pe']) && isset($context['results']['uwwtp_total_load_generated_pe'])) {
    	$sheet->setCellValue('B139', round($context['results']['uwwtp_other_morestringent_pe'] * 100 / $context['results']['uwwtp_total_load_generated_pe'], 1));
    }
    if (isset($context['results']['uwwtp_other_morestringent_pe'])) {
    	$sheet->setCellValue('C139', round($context['results']['uwwtp_other_morestringent_pe']));
    }
    if (isset($context['results']['uwwtp_other_morestringent_nb'])) {
    	$sheet->setCellValue('D139', $context['results']['uwwtp_other_morestringent_nb']);
    }

    //array 13    incoming/discharged
    if (isset($context['results']['graph']['incoming_load_bod'])) {
    	$sheet->getCell('C148')->setValue(round($context['results']['graph']['incoming_load_bod']));
    }
    if (isset($context['results']['graph']['discharge_load_bod'])) {
    	$sheet->getCell('C149')->setValue(round($context['results']['graph']['discharge_load_bod']));
    }
    if (isset($context['results']['graph']['incoming_load_cod'])) {
    	$sheet->getCell('C150')->setValue(round($context['results']['graph']['incoming_load_cod']));
    }
    if (isset($context['results']['graph']['discharge_load_cod'])) {
    	$sheet->getCell('C151')->setValue(round($context['results']['graph']['discharge_load_cod']));
    }
    if (isset($context['results']['graph']['incoming_load_n'])) {
    	$sheet->getCell('C152')->setValue(round($context['results']['graph']['incoming_load_n']));
    }
    if (isset($context['results']['graph']['discharge_load_n'])) {
    	$sheet->getCell('C153')->setValue(round($context['results']['graph']['discharge_load_n']));
    }
    if (isset($context['results']['graph']['incoming_load_p'])) {
    	$sheet->getCell('C154')->setValue(round($context['results']['graph']['incoming_load_p']));
    }
    if (isset($context['results']['graph']['discharge_load_p'])) {
    	$sheet->getCell('C155')->setValue(round($context['results']['graph']['discharge_load_p']));
    }

    //previous
    $sheet->getCell('D148')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C148'));
    $sheet->getCell('D149')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C149'));
    $sheet->getCell('D150')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C150'));
    $sheet->getCell('D151')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C151'));
    $sheet->getCell('D152')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C152'));
    $sheet->getCell('D153')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C153'));
    $sheet->getCell('D154')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C154'));
    $sheet->getCell('D155')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C155'));
    //previous_n2
    $sheet->getCell('E148')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C148'));
    $sheet->getCell('E149')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C149'));
    $sheet->getCell('E150')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C150'));
    $sheet->getCell('E151')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C151'));
    $sheet->getCell('E152')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C152'));
    $sheet->getCell('E153')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C153'));
    $sheet->getCell('E154')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C154'));
    $sheet->getCell('E155')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C155'));

    //array 14    rate
    if (isset($context['results']['graph']['discharge_load_bod']) && isset($context['results']['graph']['incoming_load_bod'])) {
    	$sheet->getCell('C159')->setValue(round(($context['results']['graph']['discharge_load_bod'] / $context['results']['graph']['incoming_load_bod']) *100, 1));
    }
    if (isset($context['results']['graph']['discharge_load_cod']) && isset($context['results']['graph']['incoming_load_cod'])) {
    	$sheet->getCell('C160')->setValue(round(($context['results']['graph']['discharge_load_cod'] / $context['results']['graph']['incoming_load_cod']) *100, 1));
    }
    if (isset($context['results']['graph']['discharge_load_n']) && isset($context['results']['graph']['incoming_load_n'])) {
    	$sheet->getCell('C161')->setValue(round(($context['results']['graph']['discharge_load_n'] / $context['results']['graph']['incoming_load_n']) *100, 1));
    }
    if (isset($context['results']['graph']['discharge_load_p']) && isset($context['results']['graph']['incoming_load_p'])) {
    	$sheet->getCell('C162')->setValue(round(($context['results']['graph']['discharge_load_p'] / $context['results']['graph']['incoming_load_p']) *100, 1));
    }
    //previous
    $sheet->getCell('D159')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C159'));
    $sheet->getCell('D160')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C160'));
    $sheet->getCell('D161')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C161'));
    $sheet->getCell('D162')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C162'));
    //previous_n2
    $sheet->getCell('E159')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C159'));
    $sheet->getCell('E160')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C160'));
    $sheet->getCell('E161')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C161'));
    $sheet->getCell('E162')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C162'));

    //array 15   totla volume
    if (isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C166')->setValue(round($context['results']['country_volume_waste_water']));
    }
    //previous
    $sheet->getCell('D166')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C166'));
    //previous_n2
    $sheet->getCell('E166')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C166'));

    //array 16  average
    if (isset($context['results']['graph']['incoming_load_bod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C169')->setValue(round(($context['results']['graph']['incoming_load_bod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_bod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C170')->setValue(round(($context['results']['graph']['discharge_load_bod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['incoming_load_cod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C171')->setValue(round(($context['results']['graph']['incoming_load_cod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_cod']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C172')->setValue(round(($context['results']['graph']['discharge_load_cod'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['incoming_load_n']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C173')->setValue(round(($context['results']['graph']['incoming_load_n'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_n']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C174')->setValue(round(($context['results']['graph']['discharge_load_n'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['incoming_load_p']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C175')->setValue(round(($context['results']['graph']['incoming_load_p'] / $context['results']['country_volume_waste_water']) * 1000000));
    }
    if (isset($context['results']['graph']['discharge_load_p']) && isset($context['results']['country_volume_waste_water'])) {
    	$sheet->getCell('C176')->setValue(round(($context['results']['graph']['discharge_load_p'] / $context['results']['country_volume_waste_water']) * 1000000));
    }

    //previous
    $sheet->getCell('D169')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C169'));
    $sheet->getCell('D170')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C170'));
    $sheet->getCell('D171')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C171'));
    $sheet->getCell('D172')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C172'));
    $sheet->getCell('D173')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C173'));
    $sheet->getCell('D174')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C174'));
    $sheet->getCell('D175')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C175'));
    $sheet->getCell('D176')->setValue(uwwtd_register_get_cell_value($previoussheet, 'C176'));
    //previous_n2
    $sheet->getCell('E169')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C169'));
    $sheet->getCell('E170')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C170'));
    $sheet->getCell('E171')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C171'));
    $sheet->getCell('E172')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C172'));
    $sheet->getCell('E173')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C173'));
    $sheet->getCell('E174')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C174'));
    $sheet->getCell('E175')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C175'));
    $sheet->getCell('E176')->setValue(uwwtd_register_get_cell_value($previoussheet_n2, 'C176'));

    // Récupération des valeurs du tableau "Comparison of Compliance between previous year and current year in p.e." :
    $arrayInPe = array();
    $arrayInPe['Compliant']['Article 3'] = $sheet->getCell('E55')->getValue();
    $arrayInPe['Compliant']['Article 4'] = $sheet->getCell('E58')->getValue();
    $arrayInPe['Compliant']['Article 5'] = $sheet->getCell('E61')->getValue();

    $arrayInPe['Not compliant']['Article 3'] = $sheet->getCell('F55')->getValue();
    $arrayInPe['Not compliant']['Article 4'] = $sheet->getCell('F58')->getValue();
    $arrayInPe['Not compliant']['Article 5'] = $sheet->getCell('F61')->getValue();

    $arrayInPe['Not relevant']['Article 3'] = $sheet->getCell('G55')->getValue();
    $arrayInPe['Not relevant']['Article 4'] = $sheet->getCell('G58')->getValue();
    $arrayInPe['Not relevant']['Article 5'] = $sheet->getCell('G61')->getValue();

    // Récupération des valeurs du tableau "Comparison of Compliance between previous year and current year in number of agglomerations" :
    $arrayInNumOfAgg = array();
    $arrayInNumOfAgg['Compliant']['Article 3'] = $sheet->getCell('B43')->getValue();
    $arrayInNumOfAgg['Compliant']['Article 4'] = $sheet->getCell('B46')->getValue();
    $arrayInNumOfAgg['Compliant']['Article 5'] = $sheet->getCell('B49')->getValue();

    $arrayInNumOfAgg['Not compliant']['Article 3'] = $sheet->getCell('B44')->getValue();
    $arrayInNumOfAgg['Not compliant']['Article 4'] = $sheet->getCell('B47')->getValue();
    $arrayInNumOfAgg['Not compliant']['Article 5'] = $sheet->getCell('B50')->getValue();

    $arrayInNumOfAgg['Not relevant']['Article 3'] = $sheet->getCell('B45')->getValue();
    $arrayInNumOfAgg['Not relevant']['Article 4'] = $sheet->getCell('B48')->getValue();
    $arrayInNumOfAgg['Not relevant']['Article 5'] = $sheet->getCell('B51')->getValue();

    // Récupération des valeurs du tableau "Comparison of distance to compliance expired deadline for previous year and current year" :
    $distancesExpired = array();
    $distancesExpired['Target connection'] = $sheet->getCell('B88')->getValue();
    $distancesExpired['Not connected'] = $sheet->getCell('B89')->getValue();
    $distancesExpired['2nd treatment target'] = $sheet->getCell('B90')->getValue();
    $distancesExpired['2nd treatment not in place'] = $sheet->getCell('B91')->getValue();
    $distancesExpired['2nd treatment performance not met'] = $sheet->getCell('B92')->getValue();
    $distancesExpired['Tertiary treatment target'] = $sheet->getCell('B93')->getValue();
    $distancesExpired['Tertiary treatment not in place'] = $sheet->getCell('B94')->getValue();
    $distancesExpired['Tertiary stringent treatment performance not met'] = $sheet->getCell('B95')->getValue();

    // R   cup   ration des valeurs du tableau "Comparison of distance to compliance pending deadlines for previous year and current year" :
    $distancesPending = array();
    $distancesPending['Target connection'] = $sheet->getCell('B104')->getValue();
    $distancesPending['Not connected'] = $sheet->getCell('B105')->getValue();
    $distancesPending['2nd treatment target'] = $sheet->getCell('B106')->getValue();
    $distancesPending['2nd treatment not in place'] = $sheet->getCell('B107')->getValue();
    $distancesPending['2nd treatment performance not met'] = $sheet->getCell('B108')->getValue();
    $distancesPending['Tertiary treatment target'] = $sheet->getCell('B109')->getValue();
    $distancesPending['Tertiary treatment not in place'] = $sheet->getCell('B110')->getValue();
    $distancesPending['Tertiary stringent treatment performance not met'] = $sheet->getCell('B111')->getValue();

    // R   cup   ration des valeurs du tableau "Distance to compliance total" :
    $distancesTotal = array();
    $distancesTotal['Target connection'] = $sheet->getCell('C119')->getValue();
    $distancesTotal['Not connected'] = $sheet->getCell('C120')->getValue();
    $distancesTotal['2nd treatment target'] = $sheet->getCell('C121')->getValue();
    $distancesTotal['2nd treatment not in place'] = $sheet->getCell('C122')->getValue();
    $distancesTotal['2nd treatment performance not met'] = $sheet->getCell('C123')->getValue();
    $distancesTotal['Tertiary treatment target'] = $sheet->getCell('C124')->getValue();
    $distancesTotal['Tertiary treatment not in place'] = $sheet->getCell('C125')->getValue();
    $distancesTotal['Tertiary stringent treatment performance not met'] = $sheet->getCell('C126')->getValue();

    // R   cup   ration du nid du node MS_LEVEL de cette ann   e :
    $query = db_select('node', 'n');
    $query->leftJoin('field_data_field_anneedata', 'a', 'n.nid = a.entity_id AND a.entity_type = \'node\'');
    $query->fields('n');
    $query->fields('a', array('field_anneedata_value'));
    $query->condition('a.field_anneedata_value', $annee, '=');
    $query->condition('n.type', 'ms_level', '=');
    $query->condition('n.status', 1, '=');
    $nid = $query->execute()->fetchField();

    // Si il n'y a pas encore de node ms_level pour cette année :
    global $language;
	$siteId = $annee.'_mslevel';
    if (empty($nid)) {
		// Création du node pour cette année :
        $node = new stdClass();
    	$node->type = 'ms_level';
    	$node->uid = 1;
     	$node->translate = 0;
        $node->tnid = 0;
        $node->sticky = 0;
        $node->promote = 0;
        $node->comment = 0;
        $node->language = $language->language;
        $node->title = 'Data on MS level - '. $annee;
        $node->field_siteid['und'][0]['value'] = $siteId;
        $node->field_anneedata['und'][0]['value'] = $annee;
        node_save($node);

        // Récupération du nid :
        $query = db_select('node', 'n');
        $query->fields('n');
        $query->condition('n.type', 'ms_level', '=');
        $query->condition('n.title', 'Data on MS level - '. $annee, '=');
        $nid = $query->execute()->fetchField();
    }

    // Renseignement des champs "Comparison of Compliance PE" et "Comparison of Compliance Agg" :
    $node = node_load($nid);
    if (!empty($node)) {
    	$node->field_comparison_of_compliance_p[LANGUAGE_NONE][0]['value'] = json_encode($arrayInPe);
    	$node->field_comparison_of_compliance_a[LANGUAGE_NONE][0]['value'] = json_encode($arrayInNumOfAgg);
    	$node->field_compliance_expired[LANGUAGE_NONE][0]['value'] = json_encode($distancesExpired);
    	$node->field_compliance_pending[LANGUAGE_NONE][0]['value'] = json_encode($distancesPending);
    	$node->field_compliance_total[LANGUAGE_NONE][0]['value'] = json_encode($distancesTotal);
    }
    node_save($node);

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
//     $objWriter->setPreCalculateFormulas(true);
    $objWriter->setIncludeCharts(TRUE);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);

    $context['sandbox']['progress']++;

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['graph']['xlsx_model_sheet_name']));

//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
	if (isset($context['sandbox']['progress']) && isset($context['sandbox']['max'])) {
    	$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
//     }
}


/**
 *
 */
function uwwtd_create_register_2015_file_graph2_sheet11_1($annee, &$context)
{
    $model_path = uwwtd_getregister_modelfile_path($annee);
    //change to public://
//     $target_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['graph']['xlsx_name'] .'.xlsx';
    $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['graph_2']['xlsx_name'], $annee);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
    $objReader->setIncludeCharts(TRUE);
    $objReader->setLoadSheetsOnly( array(
                                         $GLOBALS['uwwtd']['sheet']['graph_2']['xlsx_model_sheet_name'],
                                        ) );
    $objPHPExcel = $objReader->load(drupal_realpath($model_path));
//     $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['graph_2']['xlsx_model_sheet_name']);

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
//     $objWriter->setPreCalculateFormulas(true);
    $objWriter->setIncludeCharts(TRUE);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);
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
    $indagglo = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
	

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
        order by f_agggenerated.field_agggenerated_value DESC NULLS LAST
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
    $induwwtp = uwwtd_get_ind($GLOBALS['uwwtd']['sheet2_uwwtp']);
    

    $j = 0;
    while ($j < 20 && $uwwtp = fgetcsv($ruwwtpfile, 5000, ';')) {
//     get all uwwtp from csv file
//         check if exist in previous year
//             if no add a line
        //If the line is a AGGLo without uwwtp, we don't treat
        //correction ev 20180322 for bfb
        if (isset($uwwtp[$induwwtp['uww_code']])
            && !empty($uwwtp[$induwwtp['uww_code']])
            && '' == uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $context['sandbox']['previous_year'], 'uwwCode' => $uwwtp[$induwwtp['uww_code']])))) {
            $line = array();
            if (isset($uwwtp[$induwwtp['uww_code']])) {
                $id_of_uwwtp = $uwwtp[$induwwtp['uww_code']];
            } else {
                $id_of_uwwtp = '';
            }
            $line[$ind['id_of_uwwtp']] = $id_of_uwwtp;

            // $line[$ind['name_of_uwwtp']] = $uwwtp[$induwwtp['uww_name']];
            if (isset($uwwtp[$induwwtp['uww_name']])) {
                $name_of_uwwtp = $uwwtp[$induwwtp['uww_name']];
            } else {
                $name_of_uwwtp = '';
            }
            $line[$ind['name_of_uwwtp']] = $name_of_uwwtp;

            	
            // if (isset($uwwtp[$induwwtp['aggcode']]))
            //     $line[$ind['id_of_agglomeration']] = $uwwtp[$induwwtp['aggcode']];
            if (isset($uwwtp[$induwwtp['aggcode']])) {
                $id_of_agglomeration = $uwwtp[$induwwtp['aggcode']];
            } else {
                $id_of_agglomeration = '';
            }
            $line[$ind['id_of_agglomeration']] = $id_of_agglomeration;
            
            // if (isset($uwwtp[$induwwtp['aggname']]))
            //     $line[$ind['name_of_agglomeration']] = $uwwtp[$induwwtp['aggname']];
            if (isset($uwwtp[$induwwtp['aggname']])) {
                $name_of_agglomeration = $uwwtp[$induwwtp['aggname']];
            } else {
                $name_of_agglomeration = '';
            }
            $line[$ind['name_of_agglomeration']] = $name_of_agglomeration;
                            
            // if (isset($uwwtp[$induwwtp['uww_capacity p. e.']]))
            //     $line[$ind['size_of_uwwtp']] = $uwwtp[$induwwtp['uww_capacity p. e.']];                
            if (isset($uwwtp[$induwwtp['uww_capacity p. e.']])) {
                $size_of_uwwtp = $uwwtp[$induwwtp['uww_capacity p. e.']];
            } else {
                $size_of_uwwtp = '';
            }
            $line[$ind['size_of_uwwtp']] = $size_of_uwwtp;
            
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
            left join 
                {field_data_field_physicalcapacityactivity} f_capacity
                ON n.nid = f_capacity.entity_id

        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
            and f_status.field_status_value = :status
        order by f_capacity.field_physicalcapacityactivity_value DESC NULLS LAST
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

//Load previous UWWTP info from previous register
//load data from previous report 
$context['sandbox']['previous_data'] = [];
if (false !== $context['sandbox']['previous_year']) {
    // $previous_model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'], $context['sandbox']['previous_year']);
    $previous_model_path = uwwtd_getregister_finalfile_path($context['sandbox']['previous_year']);
    //dsm($previous_model_path);
    if (true === file_exists($previous_model_path)) {
        $previousobjReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
        $previousobjReader->setIncludeCharts(false);
        $previousobjReader->setLoadSheetsOnly( array(
                                                $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'],
                                            ) );
        $previousobjPHPExcel = $previousobjReader->load(drupal_realpath($previous_model_path));
        $previoussheet = $previousobjPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name']);

        //load all col A(agglo code) B(agglo name) N(uwwtp code) O(uwwtp name) U (capacity) from previous 
        $prevMaxRow = $previoussheet->getHighestRow();
        for ($i = 1; $i < $prevMaxRow; $i++) {
            $uwwtpCode = $previoussheet->getCell('N'.$i)->getValue();
            // $uwwtpName = $previoussheet->getCell('O'.$i)->getValue();
            $aggloCode = $previoussheet->getCell('A'.$i)->getValue();
            $aggloName = $previoussheet->getCell('B'.$i)->getValue();
            $uwwtpcapacity = $previoussheet->getCell('U'.$i)->getValue();
            
            
            if (!isset($context['sandbox']['previous_data'][$uwwtpCode])) {
                $context['sandbox']['previous_data'][$uwwtpCode] = [
                    'aggloCode' => [],
                    'aggloName' => [],
                    'uwwtpcapacity' => $uwwtpcapacity,
                ];
            }
                
            $context['sandbox']['previous_data'][$uwwtpCode]['aggloCode'][] = $aggloCode;
            $context['sandbox']['previous_data'][$uwwtpCode]['aggloName'][] = $aggloName;
            
        }
        
        // $previousobjReader->disconnectWorksheets();
        unset($previousobjReader);
    }
}
// dsm(array_keys($context['sandbox']['previous_data']));  
        
    } else {
        $r = fopen($filename, 'ab');
    }

    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {

        $previousuwwtp = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);

        if ('' != ($uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('uwwtp', array('year' => $annee, 'uwwCode' => $previousuwwtp->field_inspireidlocalid['und'][0]['value'] ))))) {
            $uwwtp = node_load($uwwtpnid);
            if ($uwwtp->field_status['und'][0]['value'] === '0') {
                $uwwtpCode = $previousuwwtp->field_inspireidlocalid['und'][0]['value'];
                // $list_id_agglo = '';
                // $list_name_agglo = '';
                // if(isset($uwwtp->field_uwwaggliste['und'])){
                //     foreach($uwwtp->field_uwwaggliste['und'] as $nidagg) {
                //         $agg = node_load($nidagg['nid']);
                //         $list_id_agglo = ($list_id_agglo == '' ? '' : ';') . $agg->field_inspireidlocalid['und'][0]['value'];
                //         $list_name_agglo = ($list_name_agglo == '' ? '' : ';') . $agg->title;
                //         unset($agg);
                //     }
                // }
                // $line = array(
                //     $ind['id_of_uwwtp'] => $previousuwwtp->field_inspireidlocalid['und'][0]['value'],
                //     $ind['name_of_uwwtp'] => $previousuwwtp->title,
                //     $ind['id_of_agglomeration'] => $list_id_agglo,
                //     $ind['name_of_agglomeration'] => $list_name_agglo,
                //     $ind['size_of_uwwtp'] => $previousuwwtp->field_physicalcapacityactivity['und'][0]['value'],
                //     $ind['remarks'] => '',
                // );
                $id_of_agglomeration = '';
                if (isset($context['sandbox']['previous_data'][$uwwtpCode]['aggloCode'])) {
                    $id_of_agglomeration = implode(';', $context['sandbox']['previous_data'][$uwwtpCode]['aggloCode']); 
                }
                $name_of_agglomeration = '';
                if (isset($context['sandbox']['previous_data'][$uwwtpCode]['aggloName'])) {
                    $name_of_agglomeration = implode(';', $context['sandbox']['previous_data'][$uwwtpCode]['aggloName']); 
                }
                $size_of_uwwtp = 0; 
                if (isset($context['sandbox']['previous_data'][$uwwtpCode]['uwwtpcapacity'])) {
                    $size_of_uwwtp = $context['sandbox']['previous_data'][$uwwtpCode]['uwwtpcapacity']; 
                } else {
                    $size_of_uwwtp = $previousuwwtp->field_physicalcapacityactivity['und'][0]['value'];
                }
                $line = array(
                    $ind['id_of_uwwtp'] => $previousuwwtp->field_inspireidlocalid['und'][0]['value'],
                    $ind['name_of_uwwtp'] => $previousuwwtp->title,
                    $ind['id_of_agglomeration'] => $id_of_agglomeration,
                    $ind['name_of_agglomeration'] => $name_of_agglomeration,
                    $ind['size_of_uwwtp'] => $size_of_uwwtp,
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
    } else {
        unset($context['sandbox']['previous_data']);
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

        $model_path = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_name'], $annee);
        $target_path = $model_path;
    }

    fseek($ragglofile, $context['sandbox']['file_position']);
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

//     $model_path = uwwtd_getregister_modelfile_path($annee);
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
    while ($j < 1000 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
    	if (isset($ind['id_of_agglomeration'])&& $agglo[$ind['id_of_agglomeration']])
        	$sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['id_of_agglomeration']]);
    	if (isset($ind['name_of_agglomeration'])&& $agglo[$ind['name_of_agglomeration']])
        	$sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['name_of_agglomeration']]);
    	if (isset($ind['size_of_agglomeration'])&& $agglo[$ind['size_of_agglomeration']])
        	$sheet->setCellValueByColumnAndRow($ind['size_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['size_of_agglomeration']]);
    	if (isset($ind['reason'])&& $agglo[$ind['reason']])
        	$sheet->setCellValueByColumnAndRow($ind['reason'], $context['sandbox']['line'], $agglo[$ind['reason']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, 1);   //0 -> +1

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
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);

//     $context['sandbox']['progress']++;

//     $context['message'] = t('new_agglomerations_sheet12 finish');
//
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     }
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['new_agglo']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';

    if (isset($context['sandbox']['progress']) && isset($context['sandbox']['max'])) {
	    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
	        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	    }
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

        $model_path = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_out']['xlsx_name'], $annee);
        $target_path = $model_path;
    }

    fseek($ragglofile, $context['sandbox']['file_position']);

//  if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

//     $model_path = uwwtd_getregister_modelfile_path($annee);
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
    while ($j < 1000 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
    	if (isset($ind['id_of_agglomeration'])&& isset($agglo[$ind['id_of_agglomeration']]))
        	$sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['id_of_agglomeration']]);
    	if (isset($ind['name_of_agglomeration'])&& isset($agglo[$ind['name_of_agglomeration']]))
        	$sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['name_of_agglomeration']]);
    	if (isset($ind['size_of_agglomeration'])&& isset($agglo[$ind['size_of_agglomeration']]))
       		$sheet->setCellValueByColumnAndRow($ind['size_of_agglomeration'], $context['sandbox']['line'], $agglo[$ind['size_of_agglomeration']]);
    	if (isset($ind['remarks'])&& isset($agglo[$ind['remarks']]))
        	$sheet->setCellValueByColumnAndRow($ind['remarks'], $context['sandbox']['line'], $agglo[$ind['remarks']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, 1); //0 -> +1

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
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
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

        $model_path = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['new_uwwtp']['xlsx_name'], $annee);
        $target_path = $model_path;
    }

    fseek($rfile, $context['sandbox']['file_position']);

//  if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

//     $model_path = uwwtd_getregister_modelfile_path($annee);
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
    while ($j < 1000 && $line = fgetcsv($rfile, 5000, ';')) {
    	if (isset($ind['id_of_uwwtp'])&& isset($line[$ind['id_of_uwwtp']]))
        	$sheet->setCellValueByColumnAndRow($ind['id_of_uwwtp'], $context['sandbox']['line'], $line[$ind['id_of_uwwtp']]);
    	if (isset($ind['name_of_uwwtp']) && isset($line[$ind['name_of_uwwtp']]))
        	$sheet->setCellValueByColumnAndRow($ind['name_of_uwwtp'], $context['sandbox']['line'], $line[$ind['name_of_uwwtp']]);
    	if (isset($ind['id_of_agglomeration']) && isset($line[$ind['id_of_agglomeration']]))
        	$sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $line[$ind['id_of_agglomeration']]);
    	if (isset($ind['name_of_agglomeration']) && isset($line[$ind['name_of_agglomeration']]))
        	$sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $line[$ind['name_of_agglomeration']]);
    	if (isset($ind['size_of_uwwtp']) && isset($line[$ind['size_of_uwwtp']]))
        	$sheet->setCellValueByColumnAndRow($ind['size_of_uwwtp'], $context['sandbox']['line'], $line[$ind['size_of_uwwtp']]);
    	if (isset($ind['reason']) && isset($line[$ind['reason']]))
        	$sheet->setCellValueByColumnAndRow($ind['reason'], $context['sandbox']['line'], $line[$ind['reason']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, 1);  //0 -> +1

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
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
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

        $model_path = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_name'], $annee);
    } else {
        $model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp_out']['xlsx_name'], $annee);
        $target_path = $model_path;
    }

    fseek($rfile, $context['sandbox']['file_position']);


//  if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

//     $model_path = uwwtd_getregister_modelfile_path($annee);
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
    while ($j < 1000 && $line = fgetcsv($rfile, 5000, ';')) {
    	if (isset($ind['id_of_uwwtp']) && isset($line[$ind['id_of_uwwtp']]))
        	$sheet->setCellValueByColumnAndRow($ind['id_of_uwwtp'], $context['sandbox']['line'], $line[$ind['id_of_uwwtp']]);
    	if (isset($ind['name_of_uwwtp']) && isset($line[$ind['name_of_uwwtp']]))
        	$sheet->setCellValueByColumnAndRow($ind['name_of_uwwtp'], $context['sandbox']['line'], $line[$ind['name_of_uwwtp']]);
    	if (isset($ind['id_of_agglomeration']) && isset($line[$ind['id_of_agglomeration']]))
        	$sheet->setCellValueByColumnAndRow($ind['id_of_agglomeration'], $context['sandbox']['line'], $line[$ind['id_of_agglomeration']]);
    	if (isset($ind['name_of_agglomeration']) && isset($line[$ind['name_of_agglomeration']]))
        	$sheet->setCellValueByColumnAndRow($ind['name_of_agglomeration'], $context['sandbox']['line'], $line[$ind['name_of_agglomeration']]);
    	if (isset($ind['size_of_uwwtp']) && isset($line[$ind['size_of_uwwtp']]))
        	$sheet->setCellValueByColumnAndRow($ind['size_of_uwwtp'], $context['sandbox']['line'], $line[$ind['size_of_uwwtp']]);
    	if (isset($ind['remarks']) && isset($line[$ind['remarks']]))
        	$sheet->setCellValueByColumnAndRow($ind['remarks'], $context['sandbox']['line'], $line[$ind['remarks']]);

        $context['sandbox']['line']++;
        $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, 1);  // 0 -> +1

        $j++;
        $context['sandbox']['file_position'] = ftell($rfile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
    }
    fclose($rfile);

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    // $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
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
	$ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
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
		if (isset($ind['art3_compliance']) &&
			isset($ind['art4_compliance_art._4_(hierarchical_approach)']) &&
			isset($ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']) &&
			isset($agglo[$ind['art3_compliance']]) &&
			isset($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]) &&
			isset($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']]) &&
        	$agglo[$ind['art3_compliance']] != 'NC'
            && $agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] != 'NC'
            && $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] != 'NC'
            ) {
            continue;
        }

        if (isset($ind['art3_compliance']) &&
			isset($ind['generated_load_p.e.']) &&
			isset($ind['connected_to_collecting_system_p._e.']) &&
        	isset($agglo[$ind['art3_compliance']]) &&
        	isset($agglo[$ind['generated_load_p.e.']]) &&
        	isset($agglo[$ind['connected_to_collecting_system_p._e.']]) &&
        	$agglo[$ind['art3_compliance']] == 'NC') {
            $art3_count          ++;
            $art3_generated_load += $agglo[$ind['generated_load_p.e.']];
            $art3_collected_load += $agglo[$ind['connected_to_collecting_system_p._e.']];
        }

        if (isset($ind['art4_compliance_art._4_(hierarchical_approach)']) &&
			isset($ind['generated_load_p.e.']) &&
			isset($ind['connected_to_collecting_system_p._e.']) &&
        	isset($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]) &&
        	isset($agglo[$ind['generated_load_p.e.']]) &&
        	isset($agglo[$ind['connected_to_collecting_system_p._e.']]) &&
        	$agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NC') {
            $art4_count          ++;
            $art4_generated_load += $agglo[$ind['generated_load_p.e.']];
            $art4_collected_load += $agglo[$ind['connected_to_collecting_system_p._e.']];
        }

        if (isset($ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']) &&
			isset($ind['generated_load_p.e.']) &&
			isset($ind['connected_to_collecting_system_p._e.']) &&
        	isset($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']]) &&
        	isset($agglo[$ind['generated_load_p.e.']]) &&
        	isset($agglo[$ind['connected_to_collecting_system_p._e.']]) &&
        	$agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC') {
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
//         uwwtd_trace($agglo_breach[$i]['type_of_receiving_water']);
//         uwwtd_trace($agglo_breach[$i]['compliance_art._5_(whole_agglo)_(hierarchical_approach)']);
//         uwwtd_trace($agglo_breach[$i]['art4_compliance_art._4_(hierarchical_approach)']);
//         uwwtd_trace($agglo_breach[$i]['art3_compliance']);
        $agglo_breach[$i]['type_of_receiving_water'] = uwwtd_register_get_glossary_label($GLOBALS['uwwtd']['sheet3_agglo']['type_of_receiving_water'], $agglo_breach[$i]['type_of_receiving_water']);
        $agglo_breach[$i]['compliance_art._5_(whole_agglo)_(hierarchical_approach)'] = uwwtd_register_get_glossary_label($GLOBALS['uwwtd']['sheet3_agglo']['compliance_art._5_(whole_agglo)_(hierarchical_approach)'], $agglo_breach[$i]['compliance_art._5_(whole_agglo)_(hierarchical_approach)']);
        $agglo_breach[$i]['art4_compliance_art._4_(hierarchical_approach)'] = uwwtd_register_get_glossary_label($GLOBALS['uwwtd']['sheet3_agglo']['art4_compliance_art._4_(hierarchical_approach)'], $agglo_breach[$i]['art4_compliance_art._4_(hierarchical_approach)']);
        $agglo_breach[$i]['art3_compliance'] = uwwtd_register_get_glossary_label($GLOBALS['uwwtd']['sheet3_agglo']['art3_compliance'], $agglo_breach[$i]['art3_compliance']);
//         uwwtd_trace($agglo_breach[$i]['type_of_receiving_water']);
//         uwwtd_trace($agglo_breach[$i]['compliance_art._5_(whole_agglo)_(hierarchical_approach)']);
//         uwwtd_trace($agglo_breach[$i]['art4_compliance_art._4_(hierarchical_approach)']);
//         uwwtd_trace($agglo_breach[$i]['art3_compliance']);
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

    $model_path = uwwtd_getregister_modelfile_path($annee);
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

    $start_line = 8;   //7 -> 8
    $sheet->insertNewRowBefore(10, count($agglo_breach));  // 8 -> 10

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
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
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
//  if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }


//     $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
	$ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
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
        if (isset($agglo[$ind['generated_load_p.e.']]))
        	$context['sandbox']['summary']['total']['waste_pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load

        if (isset($agglo[$ind['addressed_through_ias_p._e.']]) && $agglo[$ind['addressed_through_ias_p._e.']] > 0) {
            $context['sandbox']['summary']['total']['ias_in_place']['number']++;
            $context['sandbox']['summary']['total']['ias_in_place']['waste_pe'] += $agglo[$ind['addressed_through_ias_p._e.']];  //addressed through IAS pe
        }
        if (isset($agglo[$ind['connected_to_collecting_system_p._e.']]) && $agglo[$ind['connected_to_collecting_system_p._e.']] > 0) {
            $context['sandbox']['summary']['total']['collecting_system']['number']++;
            $context['sandbox']['summary']['total']['collecting_system']['waste_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']]; //connected to collecting system pe
        }

        //secondary treatment
        //  �   Number summarized from sheet   �  Agglomerations  � �, hidden column   �  Installations for wastewater treatment in place, secondary treatment installed, p.e.  � �, where parameter is  >0. (Excel-function: COUNTIF(  �  p.e. for which installation for secondary treatment is in place  � �;  � �>0  � �)
        //  �   p.e. summarized from sheet   �  Agglomerations  � �, hidden column   �  Installations for wastewater treatment in place, secondary treatment installed, p.e.  � �: sum of the entire column Excel-function: SUM (  �  p.e. for which installation for secondary treatment is in place  � �)
        if (isset($agglo[$ind['secondary_treatment_installed_(p._e.)']]) && $agglo[$ind['secondary_treatment_installed_(p._e.)']] > 0) {
            $context['sandbox']['summary']['secondary']['installation_in_place']['number']++;
            $context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
        }

        //  �   Number summarized from sheet   �  Agglomerations  � �, hidden column   �  Installations for wastewater treatment in place, monitoring results f. secondary OK, p.e.  � �, where parameter is  >0 (Excel-function: COUNTIF(  �  p.e. for which monitoring results meet the requirements for discharge concerning secondary treatment  � �;  � �>0  � �)
        //  �   p.e. summarized from sheet   �  Agglomerations  � �, hidden column   �  Installations for wastewater treatment in place, monitoring results f. secondary OK, p.e.  � �: sum of the entire column Excel-function: SUM (  �  p.e. for which monitoring results meet the requirements for discharge concerning secondary treatment  � �)
        if (isset($agglo[$ind['monitoring_results_f._secondary_ok']]) && $agglo[$ind['monitoring_results_f._secondary_ok']] > 0) {
            $context['sandbox']['summary']['secondary']['meet_requirement']['number']++;
            $context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe'] += $agglo[$ind['monitoring_results_f._secondary_ok']];
        }

        //more stringent treatment
        if (isset($agglo[$ind['more_str._treatment_installed_(p._e.)']]) && $agglo[$ind['more_str._treatment_installed_(p._e.)']] > 0) {
            $context['sandbox']['summary']['more_stringent']['installation_in_place']['number']++;
            $context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe'] += $agglo[$ind['more_str._treatment_installed_(p._e.)']];
        }

        if (isset($agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']]) && $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']] > 0) {
            $context['sandbox']['summary']['more_stringent']['meet_requirement']['number']++;
            $context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe'] += $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']];
        }
        $j++;
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
    }
    fclose($ragglofile);

    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
        //calculate percentage
        //MS total
        if (isset($context['sandbox']['summary']['total']['ias_in_place']['number']) && isset($context['sandbox']['summary']['total']['number'])) {
        	$context['sandbox']['summary']['total']['ias_in_place']['percent'] = ($context['sandbox']['summary']['total']['number'] == 0) ? 0 : $context['sandbox']['summary']['total']['ias_in_place']['number'] * 100 / $context['sandbox']['summary']['total']['number'];
        }
        if (isset($context['sandbox']['summary']['total']['ias_in_place']['waste_pe']) && isset($context['sandbox']['summary']['total']['waste_pe'])) {
        	$context['sandbox']['summary']['total']['ias_in_place']['waste_percent'] = ($context['sandbox']['summary']['total']['waste_pe'] == 0) ? 0 : $context['sandbox']['summary']['total']['ias_in_place']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
        }
    	if (isset($context['sandbox']['summary']['total']['collecting_system']['number']) && isset( $context['sandbox']['summary']['total']['number'])) {
        	$context['sandbox']['summary']['total']['collecting_system']['percent'] = ($context['sandbox']['summary']['total']['number'] == 0) ? 0 : $context['sandbox']['summary']['total']['collecting_system']['number'] * 100 / $context['sandbox']['summary']['total']['number'];
    	}
    	if (isset($context['sandbox']['summary']['total']['collecting_system']['waste_pe']) && isset($context['sandbox']['summary']['total']['waste_pe'])) {
        	$context['sandbox']['summary']['total']['collecting_system']['waste_percent'] = ($context['sandbox']['summary']['total']['waste_pe'] == 0) ? 0 : $context['sandbox']['summary']['total']['collecting_system']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
    	}

        //secondary treatment
        if (isset($context['sandbox']['summary']['secondary']['installation_in_place']['number']) && isset($context['sandbox']['summary']['total']['number'])) {
        	$context['sandbox']['summary']['secondary']['installation_in_place']['percent'] = ($context['sandbox']['summary']['total']['number'] == 0) ? 0 : $context['sandbox']['summary']['secondary']['installation_in_place']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        }
        if (isset($context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe']) && isset($context['sandbox']['summary']['total']['waste_pe'])) {
        	$context['sandbox']['summary']['secondary']['installation_in_place']['waste_percent'] = ($context['sandbox']['summary']['total']['waste_pe'] == 0) ? 0 : $context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
        }
        if (isset($context['sandbox']['summary']['secondary']['meet_requirement']['number']) && isset($context['sandbox']['summary']['total']['number'])) {
        	$context['sandbox']['summary']['secondary']['meet_requirement']['percent'] = ($context['sandbox']['summary']['total']['number'] == 0) ? 0 : $context['sandbox']['summary']['secondary']['meet_requirement']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        }
        if (isset($context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe']) && isset($context['sandbox']['summary']['total']['waste_pe'])) {
        	$context['sandbox']['summary']['secondary']['meet_requirement']['waste_percent'] = ($context['sandbox']['summary']['total']['waste_pe'] == 0) ? 0 : $context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
        }

        //more stringent treatment
        if (isset($context['sandbox']['summary']['more_stringent']['installation_in_place']['number']) && isset($context['sandbox']['summary']['total']['number'])) {
        	$context['sandbox']['summary']['more_stringent']['installation_in_place']['percent'] = ($context['sandbox']['summary']['total']['number'] == 0) ? 0 : $context['sandbox']['summary']['more_stringent']['installation_in_place']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        }
        if (isset($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe']) && isset($context['sandbox']['summary']['total']['waste_pe'])) {
        	$context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_percent'] = ($context['sandbox']['summary']['total']['waste_pe'] == 0) ? 0 : $context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
        }
        if (isset($context['sandbox']['summary']['more_stringent']['meet_requirement']['number']) && isset($context['sandbox']['summary']['total']['number'])) {
        	$context['sandbox']['summary']['more_stringent']['meet_requirement']['percent'] = ($context['sandbox']['summary']['total']['number'] == 0) ? 0 : $context['sandbox']['summary']['more_stringent']['meet_requirement']['number'] * 100 /  $context['sandbox']['summary']['total']['number'];
        }
        if (isset($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe']) && isset($context['sandbox']['summary']['total']['waste_pe'])) {
        	$context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_percent'] = ($context['sandbox']['summary']['total']['waste_pe'] == 0) ? 0 : $context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe'] * 100 / $context['sandbox']['summary']['total']['waste_pe'];
        }

    //     return $summary;
    //     dsm($summary);

    //     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');

        $model_path = uwwtd_getregister_modelfile_path($annee);
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

        if (isset($context['sandbox']['summary']['total']['number'])) {
        	$sheet->getCell('C10')->setValue($context['sandbox']['summary']['total']['number']);
        }
        if (isset($context['sandbox']['summary']['total']['percentage'])) {
        	$sheet->getCell('D10')->setValue($context['sandbox']['summary']['total']['percentage']);
        }
        if (isset($context['sandbox']['summary']['total']['waste_pe'])) {
        	$sheet->getCell('E10')->setValue($context['sandbox']['summary']['total']['waste_pe']);
        }
        if (isset($context['sandbox']['summary']['total']['waste_percent'])) {
        	$sheet->getCell('F10')->setValue($context['sandbox']['summary']['total']['waste_percent']);
        }
        if (isset($context['sandbox']['summary']['total']['ias_in_place']['number'])) {
        	$sheet->getCell('C11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['number']);
        }
        if (isset($context['sandbox']['summary']['total']['ias_in_place']['percent'])) {
        	$sheet->getCell('D11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['percent']);
        }
        if (isset($context['sandbox']['summary']['total']['ias_in_place']['waste_pe'])) {
        	$sheet->getCell('E11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['waste_pe']);
        }
        if (isset($context['sandbox']['summary']['total']['ias_in_place']['waste_percent'])) {
        	$sheet->getCell('F11')->setValue($context['sandbox']['summary']['total']['ias_in_place']['waste_percent']);
        }
        if (isset($context['sandbox']['summary']['total']['collecting_system']['number'])) {
        	$sheet->getCell('C12')->setValue($context['sandbox']['summary']['total']['collecting_system']['number']);
        }
        if (isset($context['sandbox']['summary']['total']['collecting_system']['percent'])) {
        	$sheet->getCell('D12')->setValue($context['sandbox']['summary']['total']['collecting_system']['percent']);
        }
        if (isset($context['sandbox']['summary']['total']['collecting_system']['waste_pe'])) {
        	$sheet->getCell('E12')->setValue($context['sandbox']['summary']['total']['collecting_system']['waste_pe']);
        }
        if (isset($context['sandbox']['summary']['total']['collecting_system']['waste_percent'])) {
        	$sheet->getCell('F12')->setValue($context['sandbox']['summary']['total']['collecting_system']['waste_percent']);
        }

        $context['results']['auto_report']['TOTAL_COLLECTING_SYSTEM_IN_PLACE'] = $context['sandbox']['summary']['total']['collecting_system']['waste_percent'];

        //secondary treatment
        if (isset($context['sandbox']['summary']['secondary']['installation_in_place']['number'])) {
        	$sheet->getCell('C15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['number']);
        }
        $context['results']['auto_report']['SECONDARY_INSTALLATION_IN_PLACE'] = $context['sandbox']['summary']['secondary']['installation_in_place']['number'];

        if (isset($context['sandbox']['summary']['secondary']['installation_in_place']['percent'])) {
        	$sheet->getCell('D15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['percent']);
        }
        if (isset($context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe'])) {
        	$sheet->getCell('E15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['waste_pe']);
        }
        if (isset($context['sandbox']['summary']['secondary']['installation_in_place']['waste_percent'])) {
        	$sheet->getCell('F15')->setValue($context['sandbox']['summary']['secondary']['installation_in_place']['waste_percent']);
        }
        if (isset($context['sandbox']['summary']['secondary']['meet_requirement']['number'])) {
        	$sheet->getCell('C16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['number']);
        }
        if (isset($context['sandbox']['summary']['secondary']['meet_requirement']['percent'])) {
        	$sheet->getCell('D16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['percent']);
        }
        if (isset($context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe'])) {
        	$sheet->getCell('E16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['waste_pe']);
        }
        if (isset($context['sandbox']['summary']['secondary']['meet_requirement']['waste_percent'])) {
        	$sheet->getCell('F16')->setValue($context['sandbox']['summary']['secondary']['meet_requirement']['waste_percent']);
        }
        //more stringent treatment
        if (isset($context['sandbox']['summary']['more_stringent']['installation_in_place']['number'])) {
        	$sheet->getCell('C19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['number']);
        }

        $context['results']['auto_report']['TERTIARY_INSTALLATION_IN_PLACE'] = $context['sandbox']['summary']['more_stringent']['installation_in_place']['number']; 

        if (isset($context['sandbox']['summary']['more_stringent']['installation_in_place']['percent'])) {
        	$sheet->getCell('D19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['percent']);
        }
        if (isset($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe'])) {
        	$sheet->getCell('E19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_pe']);
        }
        if (isset($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_percent'])) {
        	$sheet->getCell('F19')->setValue($context['sandbox']['summary']['more_stringent']['installation_in_place']['waste_percent']);
        }
        if (isset($context['sandbox']['summary']['more_stringent']['meet_requirement']['number'])) {
        	$sheet->getCell('C20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['number']);
        }
        if (isset($context['sandbox']['summary']['more_stringent']['meet_requirement']['percent'])) {
        	$sheet->getCell('D20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['percent']);
        }
        if (isset($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe'])) {
        	$sheet->getCell('E20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_pe']);
        }
        if (isset($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_percent'])) {
        	$sheet->getCell('F20')->setValue($context['sandbox']['summary']['more_stringent']['meet_requirement']['waste_percent']);
        }

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
//  if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }

//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
// dsm(__FUNCTION__);
    $ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
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
        $context['sandbox']['data']['expired']['target_connection']['total_load_pe'] = 0;
        $context['sandbox']['data']['expired']['target_connection']['collected_system_inplace'] = 0;
        $context['sandbox']['data']['expired']['target_connection']['ias'] = 0;

        $context['sandbox']['data']['expired']['secondary_target']['total_load_pe'] = 0;
        $context['sandbox']['data']['expired']['secondary_target']['treatement_inplace'] = 0;
        $context['sandbox']['data']['expired']['secondary_target']['monitoring_result'] = 0;

        $context['sandbox']['data']['expired']['morestringent_target']['total_load_pe'] = 0;
        $context['sandbox']['data']['expired']['morestringent_target']['installation_inplace'] = 0;
        $context['sandbox']['data']['expired']['morestringent_target']['monitoring_result'] = 0;
		$context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_pe'] = 0;
		
		$context['sandbox']['data']['pending']['target_connection']['total_load_pe'] = 0;
        $context['sandbox']['data']['pending']['target_connection']['collected_system_inplace'] = 0;
        $context['sandbox']['data']['pending']['target_connection']['ias'] = 0;

        $context['sandbox']['data']['pending']['secondary_target']['total_load_pe'] = 0;
        $context['sandbox']['data']['pending']['secondary_target']['treatement_inplace'] = 0;
        $context['sandbox']['data']['pending']['secondary_target']['monitoring_result'] = 0;

        $context['sandbox']['data']['pending']['morestringent_target']['total_load_pe'] = 0;
        $context['sandbox']['data']['pending']['morestringent_target']['installation_inplace'] = 0;
        $context['sandbox']['data']['pending']['morestringent_target']['monitoring_result'] = 0;
		$context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_pe'] = 0;
        unset($listtypestattmp);
    }

    fseek($ragglofile, $context['sandbox']['file_position']);

    $j = 0;
    while ($j < 20 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
      
        // AGGLO CODE/NUTS2 code
        // $agglo[$ind['nuts_2_code']
        // $sheet->getCell('F51')->setValue($context['sandbox']['data']['total']['target_connection']['distance_pe']);
        // $sheet->getCell('F57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_pe']);
        // $sheet->getCell('F61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_pe']);        
        
        //NUTS 2 distance
        if (!isset($context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]])) {
            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['target_connection']['total_load_pe'] = 0;
            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['target_connection']['distance_pe'] = 0;

            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['total_load_pe'] = 0;
            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['monitoring_result'] = 0;
            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['monitoring_result_distance_pe'] = 0;

            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['total_load_pe'] = 0;           
            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['monitoring_result'] = 0;           
            $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['monitoring_result_distance_pe'] = 0;  
        }

        //AGGLO distance $agglo[$ind['aggcode']]
        if (!isset($context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]])) {
            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['target_connection']['total_load_pe'] = 0;
            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['target_connection']['distance_pe'] = 0;

            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['total_load_pe'] = 0;
            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['monitoring_result'] = 0;
            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['monitoring_result_distance_pe'] = 0;

            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['total_load_pe'] = 0;           
            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['monitoring_result'] = 0;   
            $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['monitoring_result_distance_pe'] = 0;  
        }

        if (isset($agglo[$ind['art3_compliance']]) &&
            isset($agglo[$ind['connected_to_collecting_system_p._e.']]) &&
            isset($agglo[$ind['addressed_through_ias_p._e.']])){
            if(true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC', 'NC'))) {
                $context['sandbox']['data']['expired']['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']] + $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                $context['sandbox']['data']['expired']['target_connection']['distance_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                
                //nuts2
                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']] + $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['target_connection']['distance_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];

                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']] + $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['target_connection']['distance_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
            }
            elseif($agglo[$ind['art3_compliance']]=='PD'){
                $context['sandbox']['data']['pending']['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']] + $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                $context['sandbox']['data']['pending']['target_connection']['distance_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                    
                //nuts2
                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']] + $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['target_connection']['distance_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];

                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['target_connection']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']] + $agglo[$ind['addressed_through_ias_p._e.']] + $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['target_connection']['distance_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
            }
        }	
            // Distance connection p.e.
            

        // Distance connection p.e.
        if (isset($agglo[$ind['art3_compliance']]) &&
            isset($agglo[$ind['connected_to_collecting_system_p._e.']]) &&
            isset($agglo[$ind['addressed_through_ias_p._e.']])){
            if(true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC', 'NC'))) {
                $context['sandbox']['data']['expired']['target_connection']['collected_system_inplace'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                $context['sandbox']['data']['expired']['target_connection']['ias'] += $agglo[$ind['addressed_through_ias_p._e.']];
            }
            elseif($agglo[$ind['art3_compliance']]=='PD'){
                    $context['sandbox']['data']['pending']['target_connection']['collected_system_inplace'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                    $context['sandbox']['data']['pending']['target_connection']['ias'] += $agglo[$ind['addressed_through_ias_p._e.']];
            }
        }

        if (isset($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]) &&
            isset($agglo[$ind['connected_to_collecting_system_p._e.']])){
            if(true === in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('C', 'NC'))) {
                //FIX ME : shall we also add dicharge without treatment values in targeted values?
                //$agglo[$ind['neither_collecting_system_nor_ias_p._e.']]
                $context['sandbox']['data']['expired']['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                
                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                if($agglo[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                    $context['sandbox']['data']['expired']['secondary_target']['total_load_pe'] +=$agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                }
                
            }
            elseif($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]=='PD'){
                    $context['sandbox']['data']['pending']['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                    
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                    if($agglo[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                        $context['sandbox']['data']['pending']['secondary_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                        $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                        $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                    }
            }
        }

        if (isset($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]) &&
            isset($agglo[$ind['secondary_treatment_installed_(p._e.)']]) &&
            isset($agglo[$ind['monitoring_results_f._secondary_ok']])){
            if($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C' ||
                $agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NC'){
                $context['sandbox']['data']['expired']['secondary_target']['treatement_inplace'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
                $context['sandbox']['data']['expired']['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];

                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];
                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];
            }
            elseif($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'PD'){
                $context['sandbox']['data']['pending']['secondary_target']['treatement_inplace'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
                $context['sandbox']['data']['pending']['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];

                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];
                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['secondary_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._secondary_ok']];
            }
            
        }

        if (isset($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']]) &&
            isset($agglo[$ind['connected_to_collecting_system_p._e.']])){
            if(true === in_array($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']], array('C', 'NC'))) {            
                $context['sandbox']['data']['expired']['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                
                
                $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                if($agglo[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                    $context['sandbox']['data']['expired']['morestringent_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                }
            }
            elseif($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'PD'){
                    $context['sandbox']['data']['pending']['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                    
                    
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['total_load_pe'] += $agglo[$ind['connected_to_collecting_system_p._e.']];
                    if($agglo[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $agglo[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                        $context['sandbox']['data']['pending']['morestringent_target']['total_load_pe'] +=$agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                        $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['total_load_pe'] +=$agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                        $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['total_load_pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                    }
                    
            }
        }

        if (isset($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']]) &&
            isset($agglo[$ind['more_str._treatment_installed_(p._e.)']]) &&
            isset($agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']])){
            if($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C' ||
                $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC'){
                if(false !== strpos($agglo[$ind['agg_art_applied_from_rca']],'A523')){
                    //$context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'] += $agglo[$ind['more_str._treatment_installed_(p._e.)']];
                    $context['sandbox']['data']['expired']['morestringent_target']['installation_inplace'] += $agglo[$ind['more_str._treatment_installed_treatment_met_(p._e.)']];
                }
                else{
                    $context['sandbox']['data']['expired']['morestringent_target']['installation_inplace'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
                }
                
            }
            elseif($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'PD'){
                if(false !== strpos($agglo[$ind['agg_art_applied_from_rca']],'A523')){
                    $context['sandbox']['data']['pending']['morestringent_target']['installation_inplace'] += $agglo[$ind['more_str._treatment_installed_treatment_met_(p._e.)']];
                }
                else{
                    $context['sandbox']['data']['pending']['morestringent_target']['installation_inplace'] += $agglo[$ind['secondary_treatment_installed_(p._e.)']];
                }
            }
            
            
            // $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'] += $agglo[$ind['monitoring_results_f._more_string._ok_(p._e.)']];
            
        }

        // BB - 07/11 - New to calculate Art 5 monitoring DTC
        if (isset($agglo[$ind['distance_art5_perf_in_p.e._(7)']]) &&
            is_numeric($agglo[$ind['distance_art5_perf_in_p.e._(7)']])){
            if($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C' ||
                $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC'){
                if(false !== strpos($agglo[$ind['agg_art_applied_from_rca']],'A523')){
                    $context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['distance_art5_perf_in_p.e._(7)']];
                    
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['distance_art5_perf_in_p.e._(7)']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['distance_art5_perf_in_p.e._(7)']];
                }
                else{
                    $context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['dist_art4_perf_in_p.e._(5)']];
                    
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['monitoring_result_distance_pe']  += $agglo[$ind['dist_art4_perf_in_p.e._(5)']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['monitoring_result_distance_pe']  += $agglo[$ind['dist_art4_perf_in_p.e._(5)']];
                }
            }
            elseif($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'PD'){
                if(false !== strpos($agglo[$ind['agg_art_applied_from_rca']],'A523')){
                    $context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['distance_art5_perf_in_p.e._(7)']];
                    
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['distance_art5_perf_in_p.e._(7)']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['distance_art5_perf_in_p.e._(7)']];
                }
                else{
                    $context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_pe'] += $agglo[$ind['dist_art4_perf_in_p.e._(5)']];
                    
                    $context['sandbox']['data']['nuts2'][$agglo[$ind['nuts_2_code']]]['morestringent_target']['monitoring_result_distance_pe']  += $agglo[$ind['dist_art4_perf_in_p.e._(5)']];
                    $context['sandbox']['data']['agg'][$agglo[$ind['aggcode']]]['morestringent_target']['monitoring_result_distance_pe']  += $agglo[$ind['dist_art4_perf_in_p.e._(5)']];
                }
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
        	if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'])) {
            	$context['sandbox']['data']['total']['target_connection']['total_load_pe'] += $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'];
        	}
        	if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'])) {
            	$context['sandbox']['data']['total']['target_connection']['collected_system_inplace'] += $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'];
        	}
        	if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['ias'])) {
            	$context['sandbox']['data']['total']['target_connection']['ias'] += $context['sandbox']['data'][$typedeadline]['target_connection']['ias'];
        	}
            if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['distance_pe'])) {
            	$context['sandbox']['data']['total']['target_connection']['distance_pe'] += $context['sandbox']['data'][$typedeadline]['target_connection']['distance_pe'];
        	}
            
        	if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'])) {
            	$context['sandbox']['data']['total']['secondary_target']['total_load_pe'] += $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'];
        	}
        	if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'])) {
            	$context['sandbox']['data']['total']['secondary_target']['treatement_inplace'] += $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'];
        	}
        	if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'])) {
            	$context['sandbox']['data']['total']['secondary_target']['monitoring_result'] += $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'];
        	}

        	if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'])) {
            	$context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] += $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'];
        	}
        	if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'])) {
            	$context['sandbox']['data']['total']['morestringent_target']['installation_inplace'] += $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'];
        	}
        	//if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'])) {
            //	$context['sandbox']['data']['total']['morestringent_target']['monitoring_result'] += $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'];
        	//}
			if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_distance_pe'])) {
            	$context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_pe'] += $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_distance_pe'];
        	}
        }

        /////////////////////////////////////////////////
        //calcul % NUTS2
        foreach ($context['sandbox']['data']['nuts2'] as $nuts2code => $tmp) {
            //target_connection
            if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['distance_pe']) && isset($context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['total_load_pe'])) {
                $context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['distance_%']  =   
                    ($context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['total_load_pe'] == 0) ? 0 : 
                    (100 * $context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['distance_pe']) / $context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['total_load_pe'];
            }

            //secondary treatment
            if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result']) && isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['total_load_pe'])) {
                $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_%'] = ($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result']* 100 / $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['total_load_pe'];
            }
            if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_%'])) {
                $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_distance_%'] = ($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['total_load_pe'] == 0) ? 0 : 100 - $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_%'];
            }
            if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['total_load_pe']) && isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result'])) {
                $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_distance_pe'] = $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['total_load_pe'] - $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result'];
            }


            
            // if (isset($context['sandbox']['data']['total']['secondary_target']['total_load_pe']) && isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result'])) {
            //     $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_distance_pe'] = 
            //         $context['sandbox']['data']['total']['secondary_target']['total_load_pe'] - $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result'];
            // }

            // if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result']) && isset($context['sandbox']['data']['total']['secondary_target']['total_load_pe'])) {
            //     $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_%'] = 
            //         ($context['sandbox']['data']['total']['secondary_target']['total_load_pe'] == 0) ? 0 : 
            //         $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result']* 100 / $context['sandbox']['data']['total']['secondary_target']['total_load_pe'];
            // }

            // if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_%'])) {
            //     $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_distance_%'] = 
            //         ($context['sandbox']['data']['total']['secondary_target']['total_load_pe'] == 0) ? 0 : 
            //             100 - $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_%'];
            // }

            //more stringent
            if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['total_load_pe'])) {
                $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result'] = $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['total_load_pe'] - $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_pe'];
            }
            
            if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result']) && isset($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['total_load_pe'])) {
                $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_%'] = ($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['total_load_pe'] == 0) ? 0 :  $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result']* 100 / $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['total_load_pe'];
            }
            
            if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_%'])) {
                $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_%'] = ($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_pe'] * 100 / $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['total_load_pe'];
            }


            
            // if (isset($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'])) {
            //     $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result'] = 
            //         $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] - $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_pe'];
            // }
            
            // if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result']) && isset($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'])) {
            //     $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_%'] = 
            //         ($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] == 0) ? 0 :  
            //         $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result']* 100 / $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'];
            // }

            // if (isset($context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_%'])) {
            //     // Changing this - See above
            //     $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_%'] = 
            //         ($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] == 0) ? 0 : 
            //             $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_pe'] * 100 / $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'];
            // }

        }

        /////////////////////////////////////////////////
        //calcul % AGG
        foreach ($context['sandbox']['data']['agg'] as $aggcode => $tmp) {
            //target_connection
            if (isset($context['sandbox']['data']['agg'][$aggcode]['target_connection']['distance_pe']) && isset($context['sandbox']['data']['agg'][$aggcode]['target_connection']['total_load_pe'])) {
                $context['sandbox']['data']['agg'][$aggcode]['target_connection']['distance_%']  =   
                    ($context['sandbox']['data']['agg'][$aggcode]['target_connection']['total_load_pe'] == 0) ? 0 : 
                    (100 * $context['sandbox']['data']['agg'][$aggcode]['target_connection']['distance_pe']) / $context['sandbox']['data']['agg'][$aggcode]['target_connection']['total_load_pe'];
            }

            //secondary treatment

// $sheet->getCell('C57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result']);
// $sheet->getCell('D57')->setValue(round($context['sandbox']['data']['total']['secondary_target']['monitoring_result_%'], 1));
// $sheet->getCell('E57')->setValue(round($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_%'], 1));
// $sheet->getCell('F57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_pe']);

// if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result']) && isset($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'])) {
//     $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'] = ($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result']* 100 / $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'];
// }
// if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'])) {
//     $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_distance_%'] = ($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] == 0) ? 0 : 100 - $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'];
// }
// if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe']) && isset($context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'])) {
//     $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_distance_pe'] = $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'];
// }

if (isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result']) && isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['total_load_pe'])) {
    $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_%'] = ($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result']* 100 / $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['total_load_pe'];
}
if (isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_%'])) {
    $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_distance_%'] = ($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['total_load_pe'] == 0) ? 0 : 100 - $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_%'];
}
if (isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['total_load_pe']) && isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result'])) {
    $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_distance_pe'] = $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['total_load_pe'] - $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result'];
}

            // if (isset($context['sandbox']['data']['total']['secondary_target']['total_load_pe']) && isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result'])) {
            //     $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_distance_pe'] = 
            //         $context['sandbox']['data']['total']['secondary_target']['total_load_pe'] - $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result'];
            // }

            // if (isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result']) && isset($context['sandbox']['data']['total']['secondary_target']['total_load_pe'])) {
            //     $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_%'] = 
            //         ($context['sandbox']['data']['total']['secondary_target']['total_load_pe'] == 0) ? 0 : 
            //         $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result']* 100 / $context['sandbox']['data']['total']['secondary_target']['total_load_pe'];
            // }

            // if (isset($context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_%'])) {
            //     $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_distance_%'] = 
            //         ($context['sandbox']['data']['total']['secondary_target']['total_load_pe'] == 0) ? 0 : 
            //             100 - $context['sandbox']['data']['agg'][$aggcode]['secondary_target']['monitoring_result_%'];
            // }

            //more stringent

// $sheet->getCell('C61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result']);
// $sheet->getCell('D61')->setValue(round($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_%'], 1));
// $sheet->getCell('E61')->setValue(round($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_%'], 1));
// $sheet->getCell('F61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_pe']);

if (isset($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['total_load_pe'])) {
    $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result'] = $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['total_load_pe'] - $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_distance_pe'];
}

if (isset($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result']) && isset($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['total_load_pe'])) {
    $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_%'] = ($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['total_load_pe'] == 0) ? 0 :  $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result']* 100 / $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['total_load_pe'];
}

if (isset($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_%'])) {
    $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_distance_%'] = ($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_distance_pe'] * 100 / $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['total_load_pe'];
}
            // if (isset($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'])) {
            //     $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result'] = 
            //         $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] - $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_distance_pe'];
            // }
            
            // if (isset($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result']) && isset($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'])) {
            //     $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_%'] = 
            //         ($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] == 0) ? 0 :  
            //         $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result']* 100 / $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'];
            // }

            // if (isset($context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_%'])) {
            //     // Changing this - See above
            //     $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_distance_%'] = 
            //         ($context['sandbox']['data']['total']['morestringent_target']['total_load_pe'] == 0) ? 0 : 
            //             $context['sandbox']['data']['agg'][$aggcode]['morestringent_target']['monitoring_result_distance_pe'] * 100 / $context['sandbox']['data']['total']['morestringent_target']['total_load_pe'];
            // }

        }

        //Write csv file for NUTS 2 distance
        // $nuts2code
        // $context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['distance_pe']
        // $context['sandbox']['data']['nuts2'][$nuts2code]['target_connection']['distance_%']

        // $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_distance_pe']
        // $context['sandbox']['data']['nuts2'][$nuts2code]['secondary_target']['monitoring_result_distance_%']

        // $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_pe']
        // $context['sandbox']['data']['nuts2'][$nuts2code]['morestringent_target']['monitoring_result_distance_%']
        $nuts2_filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['nuts_level2']['csv_distance_name'], $annee);

        $rnuts2file = fopen($nuts2_filename, 'wb');

        fputcsv($rnuts2file, ['nuts2code', 
                              'target_connection_total_load_pe',
                              'target_connection_distance_pe', 
                              'target_connection_distance_%', 
                              'secondary_target_total_load_pe',
                              'secondary_target_monitoring_result_distance_pe', 
                              'secondary_target_monitoring_result_distance_%',
                              'morestringent_total_load_pe',
                              'morestringent_target_monitoring_result_distance_pe', 
                              'morestringent_target_monitoring_result_distance_%', 
                            ], ';');
        foreach($context['sandbox']['data']['nuts2'] as $nuts2code => $tmp) {
            fputcsv($rnuts2file, [$nuts2code, 
                                $tmp['target_connection']['total_load_pe'],
                                $tmp['target_connection']['distance_pe'], 
                                $tmp['target_connection']['distance_%'],
                                $tmp['secondary_target']['total_load_pe'],//$context['sandbox']['data']['total']['secondary_target']['monitoring_result']
                                $tmp['secondary_target']['monitoring_result_distance_pe'],
                                $tmp['secondary_target']['monitoring_result_distance_%'],
                                $tmp['morestringent_target']['total_load_pe'],
                                $tmp['morestringent_target']['monitoring_result_distance_pe'],
                                $tmp['morestringent_target']['monitoring_result_distance_%'],
                            ], ';');
        }
        unset($context['sandbox']['data']['nuts2']);
        fclose($rnuts2file);

        //Write aggg distance file
        $aggdistance_filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_distance_name'], $annee);

        $raggdistancefile = fopen($aggdistance_filename, 'wb');

        fputcsv($raggdistancefile, ['aggcode', 
                              'target_connection_total_load_pe',
                              'target_connection_distance_pe', 
                              'target_connection_distance_%', 
                              'secondary_target_total_load_pe',
                              'secondary_target_monitoring_result_distance_pe', 
                              'secondary_target_monitoring_result_distance_%',
                              'morestringent_total_load_pe',
                              'morestringent_target_monitoring_result_distance_pe', 
                              'morestringent_target_monitoring_result_distance_%', 
                            ], ';');
        foreach($context['sandbox']['data']['agg'] as $aggcode => $tmp) {
            fputcsv($raggdistancefile, [$aggcode, 
                                $tmp['target_connection']['total_load_pe'],
                                $tmp['target_connection']['distance_pe'], 
                                $tmp['target_connection']['distance_%'],
                                $tmp['secondary_target']['total_load_pe'],
                                $tmp['secondary_target']['monitoring_result_distance_pe'],
                                $tmp['secondary_target']['monitoring_result_distance_%'],
                                $tmp['morestringent_target']['total_load_pe'],
                                $tmp['morestringent_target']['monitoring_result_distance_pe'],
                                $tmp['morestringent_target']['monitoring_result_distance_%'],
                            ], ';');
        }
        unset($context['sandbox']['data']['agg']);
        fclose($raggdistancefile);


        //calcul % and distance
        $listtypestattmp = $listtypestat;
        $listtypestattmp[] = 'total';
        foreach($listtypestattmp as $typedeadline) {
        	if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace']) && isset($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'])) {
            	$context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace_%'] =  ($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'] * 100 / $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'];
        	} 
        	if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['ias']) && isset($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'])) {
            	$context['sandbox']['data'][$typedeadline]['target_connection']['ias_%'] =   ($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data'][$typedeadline]['target_connection']['ias'] * 100 / $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'];
        	}
            
        	if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['distance_pe']) && isset($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'])) {
                $context['sandbox']['data'][$typedeadline]['target_connection']['distance_%']  =   ($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'] == 0) ? 0 : (100 * $context['sandbox']['data'][$typedeadline]['target_connection']['distance_pe']) / $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'];
            }
      
            if (isset($context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe']) && isset($context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace']) && isset($context['sandbox']['data'][$typedeadline]['target_connection']['ias'])) {
                $context['sandbox']['data'][$typedeadline]['target_connection']['distance_pe'] =  $context['sandbox']['data'][$typedeadline]['target_connection']['total_load_pe'] - ($context['sandbox']['data'][$typedeadline]['target_connection']['collected_system_inplace'] + $context['sandbox']['data'][$typedeadline]['target_connection']['ias']);
            }

            //secondary % and distance
            if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace']) && isset($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'])) {
            	$context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_%'] =  ($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace']* 100 / $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'];
            }
            if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result']) && isset($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'])) {
            	$context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'] = ($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result']* 100 / $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'];
            }

            if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe']) && isset($context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'])) {
            	$context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_distance_pe'] = $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace'];
            }
            if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe']) && isset($context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'])) {
            	$context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_distance_pe'] = $context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result'];
            }

            if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_%'])) {
            	$context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_distance_%'] = ($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] == 0) ? 0 : 100 - $context['sandbox']['data'][$typedeadline]['secondary_target']['treatement_inplace_%'];
            }
            if (isset($context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'])) {
            	$context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_distance_%'] = ($context['sandbox']['data'][$typedeadline]['secondary_target']['total_load_pe'] == 0) ? 0 : 100 - $context['sandbox']['data'][$typedeadline]['secondary_target']['monitoring_result_%'];
            }

            //more stringent % and distance
            if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace']) && isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'])) {
            	$context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_%'] = ($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] == 0) ? 0 :  $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'] * 100 / $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'];
            }
			if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe']) && isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'])) {
            	$context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_distance_pe'] = $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace'];
			}

            // BB - 07/11 - Removing this as the DTC is calculated before
			if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'])) {
            	$context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result'] = $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] - $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_distance_pe'];
			}

			if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result']) && isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'])) {
            	$context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_%'] = ($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] == 0) ? 0 :  $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result']* 100 / $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'];
            }

            if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_%'])) {
            	$context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_distance_%'] = ($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] == 0) ? 0 : 100 - $context['sandbox']['data'][$typedeadline]['morestringent_target']['installation_inplace_%'];
            }
            if (isset($context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_%'])) {
                // Changing this - See above
            	$context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_distance_%'] = ($context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'] == 0) ? 0 : $context['sandbox']['data'][$typedeadline]['morestringent_target']['monitoring_result_distance_pe'] * 100 / $context['sandbox']['data'][$typedeadline]['morestringent_target']['total_load_pe'];
            }
        }

        //build excel sheet
        $model_path = uwwtd_getregister_modelfile_path($annee);
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
        if (isset($context['sandbox']['data']['expired']['target_connection']['total_load_pe']))
        	$sheet->getCell('C12')->setValue($context['sandbox']['data']['expired']['target_connection']['total_load_pe']);
        if (isset($context['sandbox']['data']['expired']['target_connection']['distance_%']))
        	$sheet->getCell('E12')->setValue(round($context['sandbox']['data']['expired']['target_connection']['distance_%'], 1));
        if (isset($context['sandbox']['data']['expired']['target_connection']['distance_pe']))
        	$sheet->getCell('F12')->setValue($context['sandbox']['data']['expired']['target_connection']['distance_pe']);

        if (isset($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace']))
        	$sheet->getCell('C13')->setValue($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace']);
        if (isset($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace_%']))
        	$sheet->getCell('D13')->setValue(round($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace_%'], 1));
        if (isset($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace_distance%']))
        	$sheet->getCell('E13')->setValue(round($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace_distance%'], 1));
        if (isset($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace_distance_pe']))
        	$sheet->getCell('F13')->setValue(round($context['sandbox']['data']['expired']['target_connection']['collected_system_inplace_distance_pe'], 1));
        if (isset($context['sandbox']['data']['expired']['target_connection']['ias']))
        	$sheet->getCell('C14')->setValue($context['sandbox']['data']['expired']['target_connection']['ias']);
        if (isset($context['sandbox']['data']['expired']['target_connection']['ias_%']))
        	$sheet->getCell('D14')->setValue(round($context['sandbox']['data']['expired']['target_connection']['ias_%'], 1));

        //secondary treatement
        if (isset($context['sandbox']['data']['expired']['secondary_target']['total_load_pe']))
        	$sheet->getCell('C16')->setValue($context['sandbox']['data']['expired']['secondary_target']['total_load_pe']);
        if (isset($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace']))
        	$sheet->getCell('C17')->setValue($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace']);
        if (isset($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_%']))
        	$sheet->getCell('D17')->setValue(round($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_%'], 1));
        if (isset($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_distance_%']))
        	$sheet->getCell('E17')->setValue(round($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_distance_%'], 1));
        if (isset($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_distance_pe']))
        	$sheet->getCell('F17')->setValue($context['sandbox']['data']['expired']['secondary_target']['treatement_inplace_distance_pe']);
        if (isset($context['sandbox']['data']['expired']['secondary_target']['monitoring_result']))
        	$sheet->getCell('C18')->setValue($context['sandbox']['data']['expired']['secondary_target']['monitoring_result']);
        if (isset($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_%']))
        	$sheet->getCell('D18')->setValue(round($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_%'], 1));
        if (isset($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_distance_%']))
        	$sheet->getCell('E18')->setValue(round($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_distance_%'], 1));
        if (isset($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_distance_pe']))
        	$sheet->getCell('F18')->setValue($context['sandbox']['data']['expired']['secondary_target']['monitoring_result_distance_pe']);

        //more stringent treatment
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['total_load_pe']))
        	$sheet->getCell('C20')->setValue($context['sandbox']['data']['expired']['morestringent_target']['total_load_pe']);
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace']))
        	$sheet->getCell('C21')->setValue($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace']);
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_%']))
        	$sheet->getCell('D21')->setValue(round($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_%'], 1));
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_distance_%']))
        	$sheet->getCell('E21')->setValue(round($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_distance_%'], 1));
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_distance_pe']))
        	$sheet->getCell('F21')->setValue($context['sandbox']['data']['expired']['morestringent_target']['installation_inplace_distance_pe']);
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result']))
        	$sheet->getCell('C22')->setValue($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result']);
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_%']))
        	$sheet->getCell('D22')->setValue(round($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_%'], 1));
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_%']))
        	$sheet->getCell('E22')->setValue(round($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_%'], 1));
        if (isset($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_pe']))
        	$sheet->getCell('F22')->setValue($context['sandbox']['data']['expired']['morestringent_target']['monitoring_result_distance_pe']);

        //pending
        //target connection
        if (isset($context['sandbox']['data']['pending']['target_connection']['total_load_pe']))
        	$sheet->getCell('C32')->setValue($context['sandbox']['data']['pending']['target_connection']['total_load_pe']);

        if (isset($context['sandbox']['data']['pending']['target_connection']['distance_%']))
        	$sheet->getCell('E32')->setValue(round($context['sandbox']['data']['pending']['target_connection']['distance_%'], 1));
        if (isset($context['sandbox']['data']['pending']['target_connection']['distance_pe']))
        	$sheet->getCell('F32')->setValue($context['sandbox']['data']['pending']['target_connection']['distance_pe']);

        if (isset($context['sandbox']['data']['pending']['target_connection']['collected_system_inplace']))
        	$sheet->getCell('C33')->setValue($context['sandbox']['data']['pending']['target_connection']['collected_system_inplace']);
        if (isset($context['sandbox']['data']['pending']['target_connection']['collected_system_inplace_%']))
        	$sheet->getCell('D33')->setValue(round($context['sandbox']['data']['pending']['target_connection']['collected_system_inplace_%'], 1));
        if (isset($context['sandbox']['data']['pending']['target_connection']['collected_system_inplace_%']))
        	$sheet->getCell('C34')->setValue($context['sandbox']['data']['pending']['target_connection']['ias']);
        if (isset($context['sandbox']['data']['pending']['target_connection']['ias_%']))
        	$sheet->getCell('D34')->setValue(round($context['sandbox']['data']['pending']['target_connection']['ias_%'], 1));

        //secondary treatement
        if (isset($context['sandbox']['data']['pending']['secondary_target']['total_load_pe']))
        	$sheet->getCell('C36')->setValue($context['sandbox']['data']['pending']['secondary_target']['total_load_pe']);
        if (isset($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace']))
	        $sheet->getCell('C37')->setValue($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace']);
        if (isset($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_%']))
	        $sheet->getCell('D37')->setValue(round($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_%'], 1));
        if (isset($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_distance_%']))
	        $sheet->getCell('E37')->setValue(round($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_distance_%'], 1));
        if (isset($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_distance_pe']))
	        $sheet->getCell('F37')->setValue($context['sandbox']['data']['pending']['secondary_target']['treatement_inplace_distance_pe']);
        if (isset($context['sandbox']['data']['pending']['secondary_target']['monitoring_result']))
	        $sheet->getCell('C38')->setValue($context['sandbox']['data']['pending']['secondary_target']['monitoring_result']);
        if (isset($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_%']))
	        $sheet->getCell('D38')->setValue(round($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_%'], 1));
        if (isset($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_distance_%']))
	        $sheet->getCell('E38')->setValue(round($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_distance_%'], 1));
        if (isset($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_distance_pe']))
	        $sheet->getCell('F38')->setValue($context['sandbox']['data']['pending']['secondary_target']['monitoring_result_distance_pe']);

        //more stringent treatment
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['total_load_pe']))
	        $sheet->getCell('C40')->setValue($context['sandbox']['data']['pending']['morestringent_target']['total_load_pe']);
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace']))
	        $sheet->getCell('C41')->setValue($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace']);
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_%']))
	        $sheet->getCell('D41')->setValue(round($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_%'], 1));
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_distance_%']))
	        $sheet->getCell('E41')->setValue(round($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_distance_%'], 1));
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_distance_pe']))
	        $sheet->getCell('F41')->setValue($context['sandbox']['data']['pending']['morestringent_target']['installation_inplace_distance_pe']);
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result']))
	        $sheet->getCell('C42')->setValue($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result']);
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_%']))
	        $sheet->getCell('D42')->setValue(round($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_%'], 1));
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_%']))
	        $sheet->getCell('E42')->setValue(round($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_%'], 1));
        if (isset($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_pe']))
	        $sheet->getCell('F42')->setValue($context['sandbox']['data']['pending']['morestringent_target']['monitoring_result_distance_pe']);

        //total
        //target connection
        if (isset($context['sandbox']['data']['total']['target_connection']['total_load_pe']))
	        $sheet->getCell('C51')->setValue($context['sandbox']['data']['total']['target_connection']['total_load_pe']);

	    if (isset($context['sandbox']['data']['total']['target_connection']['distance_%']))
	        $sheet->getCell('E51')->setValue(round($context['sandbox']['data']['total']['target_connection']['distance_%'], 1));

        $context['results']['auto_report']['ART3_DISTANCE_TO_COMPLIANCE'] = round($context['sandbox']['data']['total']['target_connection']['distance_%'], 1);         

	    if (isset($context['sandbox']['data']['total']['target_connection']['distance_pe']))
	        $sheet->getCell('F51')->setValue($context['sandbox']['data']['total']['target_connection']['distance_pe']);

	    if (isset($context['sandbox']['data']['total']['target_connection']['collected_system_inplace']))
	        $sheet->getCell('C52')->setValue($context['sandbox']['data']['total']['target_connection']['collected_system_inplace']);
	    if (isset($context['sandbox']['data']['total']['target_connection']['collected_system_inplace_%']))
	        $sheet->getCell('D52')->setValue(round($context['sandbox']['data']['total']['target_connection']['collected_system_inplace_%'], 1));
	    if (isset($context['sandbox']['data']['total']['target_connection']['ias']))
	        $sheet->getCell('C53')->setValue($context['sandbox']['data']['total']['target_connection']['ias']);
	    if (isset($context['sandbox']['data']['total']['target_connection']['ias_%']))
	        $sheet->getCell('D53')->setValue(round($context['sandbox']['data']['total']['target_connection']['ias_%'], 1));

        //secondary treatement
        if (isset($context['sandbox']['data']['total']['secondary_target']['total_load_pe']))
	        $sheet->getCell('C55')->setValue($context['sandbox']['data']['total']['secondary_target']['total_load_pe']);
        if (isset($context['sandbox']['data']['total']['secondary_target']['treatement_inplace']))
	        $sheet->getCell('C56')->setValue($context['sandbox']['data']['total']['secondary_target']['treatement_inplace']);
        if (isset($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_%']))
	        $sheet->getCell('D56')->setValue(round($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_%'], 1));
        if (isset($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_distance_%']))
	        $sheet->getCell('E56')->setValue(round($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_distance_%'], 1));
        if (isset($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_distance_pe']))
	        $sheet->getCell('F56')->setValue($context['sandbox']['data']['total']['secondary_target']['treatement_inplace_distance_pe']);
        if (isset($context['sandbox']['data']['total']['secondary_target']['monitoring_result']))
	        $sheet->getCell('C57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result']);
        if (isset($context['sandbox']['data']['total']['secondary_target']['monitoring_result_%']))
	        $sheet->getCell('D57')->setValue(round($context['sandbox']['data']['total']['secondary_target']['monitoring_result_%'], 1));
        if (isset($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_%']))
	        $sheet->getCell('E57')->setValue(round($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_%'], 1));

         $context['results']['auto_report']['ART4_DISTANCE_TO_COMPLIANCE'] = round($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_%'], 1);         

        if (isset($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_pe']))
	        $sheet->getCell('F57')->setValue($context['sandbox']['data']['total']['secondary_target']['monitoring_result_distance_pe']);

        //more stringent treatment
        if (isset($context['sandbox']['data']['total']['morestringent_target']['total_load_pe']))
	        $sheet->getCell('C59')->setValue($context['sandbox']['data']['total']['morestringent_target']['total_load_pe']);
        if (isset($context['sandbox']['data']['total']['morestringent_target']['installation_inplace']))
	        $sheet->getCell('C60')->setValue($context['sandbox']['data']['total']['morestringent_target']['installation_inplace']);
        if (isset($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_%']))
	        $sheet->getCell('D60')->setValue(round($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_%'], 1));
        if (isset($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_distance_%']))
	        $sheet->getCell('E60')->setValue(round($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_distance_%'], 1));
        if (isset($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_distance_pe']))
	        $sheet->getCell('F60')->setValue($context['sandbox']['data']['total']['morestringent_target']['installation_inplace_distance_pe']);
        if (isset($context['sandbox']['data']['total']['morestringent_target']['monitoring_result']))
	        $sheet->getCell('C61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result']);
        if (isset($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_%']))
	        $sheet->getCell('D61')->setValue(round($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_%'], 1));
        if (isset($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_%']))
	        $sheet->getCell('E61')->setValue(round($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_%'], 1));

         $context['results']['auto_report']['ART5_DISTANCE_TO_COMPLIANCE'] = round($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_%'], 1);

        if (isset($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_pe']))
	        $sheet->getCell('F61')->setValue($context['sandbox']['data']['total']['morestringent_target']['monitoring_result_distance_pe']);
	    if (isset($context['sandbox']['data']))
        	$context['results']['distance_to_compliance'] = $context['sandbox']['data'];

        //write
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
        $objWriter->setPreCalculateFormulas(false);
        //$objWriter->setOffice2003Compatibility(true);
        $objWriter->save(drupal_realpath($target_path));
        $objPHPExcel->__destruct();
        unset($objPHPExcel);
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
    uwwtd_trace(__FUNCTION__);
//     $ind = $GLOBALS['uwwtd']['sheet3_agglo'];
//
//  if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }
	$ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['csv_name'], $annee);
    $ragglofile = fopen($filename, 'rb');

    //copy info for debug in a csv file
    $filename_agglo_debug = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_debug']['csv_name'], $annee);

    // uwwtd_trace(__LINE__);
    $list_type = array('all', 'NA', 'CSA/SA');
    //pass header line
    if (!isset($context['sandbox']['progress'])) {
        $ragglodebugfile = fopen($filename_agglo_debug, 'wb');
        // uwwtd_trace('wb');
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
            $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['percentage'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['number'] = 0;
            $context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['percentage'] = 0;
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
            // uwwtd_trace(__LINE__);
            $agglodebugheader = array_keys($ind);
            // uwwtd_trace(__LINE__);
            $agglodebugheader[] = 'TYPE' ;
            $agglodebugheader[] = 'NA' ;
            $agglodebugheader[] = 'CSA';
            $agglodebugheader[] = 'LSA';
            $agglodebugheader[] = 'SA' ;
            $agglodebugheader[] = 'A58';
            $agglodebugheader[] = 'A54';
            $agglodebugheader[] = 'rcac';
            $agglodebugheader[] = 'test1_<=1000';
            $agglodebugheader[] = 'test2_NA';
            $agglodebugheader[] = 'test3_CSA_rcac';
            $agglodebugheader[] = 'test4_A54';
            $agglodebugheader[] = 'test5_connected_collected_system_0';
            $agglodebugheader[] = 'test6_LSA';
            $agglodebugheader[] = 'test7_compliance_PD';
            fputcsv($ragglodebugfile, $agglodebugheader, ';');
            // uwwtd_trace('wb');
        }
    //     $path_xlsx = uwwtd_getregister_modelfile_path($annee);
    //     $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo']['xlsx_name'], $annee);
    } else {
        // uwwtd_trace('ab');
        $ragglodebugfile = fopen($filename_agglo_debug, 'ab');
    }
    fseek($ragglofile, $context['sandbox']['file_position']);
    // uwwtd_trace(__LINE__);
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
        
        // uwwtd_trace(__LINE__);            
//     foreach ($listagglo as $agglo) {
//         dsm($agglo);
        foreach($list_type as $type) {
//             dsm($type);$
            $agglodebug = $agglo;
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $agglodebug[] = '';
            $indSup = 108;
            $agglodebug[$indSup+0] = $type;

            $listrcatype = explode(';', $agglo[$ind['type_of_receiving_water']]);
            if ($type == 'NA') {
                    //type of receiving water
                if (false === in_array('NA', $listrcatype)) {
                    fputcsv($ragglodebugfile, $agglodebug, ';');
                    continue;
                }
            } elseif ($type == 'CSA/SA') {
                //todo A58 ?
                if (false === in_array('SA', $listrcatype) && false === in_array('CSA', $listrcatype)) {
                    fputcsv($ragglodebugfile, $agglodebug, ';');
                    continue;
                }
            } 
            //compliance art3
            
            $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number']++;
            if (isset($agglo[$ind['generated_load_p.e.']])){
                $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
            }
            
            //compliance art3
            if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC', 'NC'))) {
                $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number']++;
                if (isset($agglo[$ind['generated_load_p.e.']])){
                    $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
                }
                
            }
            if (true === in_array($agglo[$ind['art3_compliance']], array('C', 'QC'))) {
                $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['number']++;
                if (isset($agglo[$ind['generated_load_p.e.']]))
                	$context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
              	if (isset($agglo[$ind['connected_to_collecting_system_p._e.']]) && $agglo[$ind['connected_to_collecting_system_p._e.']] > 0) {
                  $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                  $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['number']++;
                }
              	
              	if (isset($agglo[$ind['addressed_through_ias_p._e.']]) && $agglo[$ind['addressed_through_ias_p._e.']] > 0) {
                	$context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe'] += $agglo[$ind['addressed_through_ias_p._e.']];
                    $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['number']++;
                }
              	
                //Manage not connected load
                if (isset($agglo[$ind['neither_collecting_system_nor_ias_p._e.']]) && $agglo[$ind['neither_collecting_system_nor_ias_p._e.']] > 0) {
                      $context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['number']++;
                  $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['pe'] += $agglo[$ind['neither_collecting_system_nor_ias_p._e.']];
                }
                
            }
            
           //compliance art3
           if (true === in_array($agglo[$ind['art3_compliance']], array('NR', 'PD'))) {
                $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['number']++;
                if (isset($agglo[$ind['generated_load_p.e.']])){
                    $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[$ind['generated_load_p.e.']]; //generated load p.e.
                }
                	
            }
            $NA = false !== (strpos($agglo[$ind['type_of_receiving_water']], 'NA'));
            $CSA = false !== (strpos($agglo[$ind['type_of_receiving_water']], 'CSA'));
            $LSA = false !== (strpos($agglo[$ind['type_of_receiving_water']], 'LSA'));
            $SA = false !== (strpos($agglo[$ind['type_of_receiving_water']], 'SA') === 0 && !$CSA && !$LSA);
            $A58 = false !== (strpos($agglo[$ind['type_of_receiving_water']], 'A58'));
            $A54 = false !== (strpos($agglo[$ind['type_of_receiving_water']], 'A54'));
            $rcac = (1 == $agglo[$ind['required_rcac']]);


            $agglodebug[$indSup+1] = $NA === true ? '1' : '0';
            $agglodebug[$indSup+2] = $CSA === true ? '1' : '0';
            $agglodebug[$indSup+3] = $LSA === true ? '1' : '0';
            $agglodebug[$indSup+4] = $SA === true ? '1' : '0';
            $agglodebug[$indSup+5] = $A58 === true ? '1' : '0';
            $agglodebug[$indSup+6] = $A54 === true ? '1' : '0';
            $agglodebug[$indSup+7] = $rcac === true ? '1' : '0';
            
//             dsm('art4');
//             dsm($ind['art4_compliance_art._4_(hierarchical_approach)']);
            //art 4
                //Compliance Art. 4 (hierarchical approach)
            if ($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {
                $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['number']++;
                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                	$context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                }
            }
            if (isset($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]) && true === in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('C', 'NC'))) {
                $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number']++;
                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                    $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                }
                //Report discharge without treatment in not compliant case in Art 4
                if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                    $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'] += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                }
                	
            }
            
            if (isset($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']]) && in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('NR', 'PD'))) {
                $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['number']++;
                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                	$context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                    //Report discharge without treatment in not compliant case in Art 4
                    if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                        $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['pe'] += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                    }
                }
                //            - due to a size of 2000p.e. - 10000 p.e.       ..and discharge into coastal water
                if (isset($agglo[$ind['generated_load_p.e.']]) && $agglo[$ind['generated_load_p.e.']] >= 2000 && $agglo[$ind['generated_load_p.e.']] <= 10000
                    && $agglo[$ind['entirely_discharging_into_coastal_waters']] == 'yes') {
                    $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['number']++;
                	if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']]))
                    	$context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                        //Report discharge without treatment in not compliant case in Art 4
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['pe'] += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                }
    //            - due to 0% collection in collecting   ...system
                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']]) && $agglo[ $ind['connected_to_collecting_system_p._e.']] == 0) {
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['number']++;
                	if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']]))
                    	$context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                }
    //            - due to discharge into LSA
    			if (isset($agglo[ $ind['type_of_receiving_water']]))
                $listrcatype = explode(';', $agglo[ $ind['type_of_receiving_water']]);
                if ($LSA && !$SA && !$CSA && !$A58 && !$A54 && !$NA) {
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['number']++;
                	if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']]))
                    	$context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                        //Report discharge without treatment in not compliant case in Art 4
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['pe'] += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                }
                unset($listrcatype);
    //            - due to transition period
                if (in_array($agglo[$ind['art4_compliance_art._4_(hierarchical_approach)']], array('PD'))) {
                    $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['number']++;
	                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                        $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                        //Report discharge without treatment in not compliant case in Art 4
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
	                    
                }
            }
            $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number']++;
            if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']]))
            	$context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']]; //connected to collecting system p. e.
                //Report discharge without treatment in not compliant case in Art 4
                if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                    $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                }
            //art 5
//             dsm('art5');
//             dsm($ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']);
                //Compliance Art. 5 (whole agglo) (hierarchical approach)
            if ($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {
                $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['number']++;
                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']]))
                	$context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];  //connected to collecting system p. e.
            }
            if (true === in_array($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']], array('C', 'NC'))) {
                $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number']++;
                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                	$context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                    //Report discharge without treatment in not compliant case in Art 5
                    if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                        $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                    }
                }
            }
            if (in_array($agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']], array('NR', 'PD'))) {
                $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['number']++;
                if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                	$context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                    //Report discharge without treatment in not compliant case in Art 5
                    if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                        $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                    }
                }
                //- due to a size of   �   10000 p.e.       
                if ($agglo[$ind['generated_load_p.e.']] >= 2000 && $agglo[$ind['generated_load_p.e.']] <= 10000) {
                    $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['number']++;
                    $agglodebug[$indSup+8] = 'test1=1';
                    if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                		$context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                        //Report discharge without treatment in not compliant case in Art 5
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
                } else {
                    $agglodebug[$indSup+8] = 'test1=0'; 
                }
                //   - size > 10000 p.e., but discharge          ..into NA
                if ($agglo[$ind['generated_load_p.e.']] > 10000
                    && $NA && !$LSA && !$SA && !$CSA && !$A58) {
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['number']++;
                    $agglodebug[$indSup+9] = 'test2=1';
                    if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                		$context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                        //Report discharge without treatment in not compliant case in Art 5
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
                } else {
                    $agglodebug[$indSup+9] = 'test2=0'; 
                }
                //   - size > 10000 p.e. and discharge into   ..CSA designated for criterion c
                if ($agglo[$ind['generated_load_p.e.']] > 10000
                    && $CSA
                    && $rcac) {
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['number']++;
                    $agglodebug[$indSup+10] = 'test3=1';
                    if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                		$context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                        //Report discharge without treatment in not compliant case in Art 5
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
                } else {
                    $agglodebug[$indSup+10] = 'test3=0'; 
                }
                //   - size > 10000 p.e., but discharge into   ...Art. 5(4)- area
                if ($agglo[$ind['generated_load_p.e.']] > 10000
                    && $A54) {
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['number']++;
                    $agglodebug[$indSup+11] = 'test4=1';
                    if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                		$context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                        //Report discharge without treatment in not compliant case in Art 5
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
                } else {
                    $agglodebug[$indSup+11] = 'test4=0'; 
                }
                //   - due to 0% collection in collecting   ...system
                if ($agglo[ $ind['connected_to_collecting_system_p._e.']] == 0) {
                    $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['number']++;
                    $agglodebug[$indSup+12] = 'test5=1';
                    if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                		$context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                        //Report discharge without treatment in not compliant case in Art 5
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe']  += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
                } else {
                    $agglodebug[$indSup+12] = 'test5=0'; 
                }
                //   - size > 10000 p.e.  and discharge into     ...LSA
                if ($agglo[$ind['generated_load_p.e.']] > 10000
                    && $LSA && !$SA && !$CSA && !$A58) {
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['number']++;
                    $agglodebug[$indSup+13] = 'test6=1';
                    if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                		$context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                        //Report discharge without treatment in not compliant case in Art 5
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['pe']   += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
                } else {
                    $agglodebug[$indSup+13] = 'test6=0'; 
                }
                //   - size > 10000 p.e. and discharge into   ..SA/CSA, but transition period pending
                if ($agglo[$ind['generated_load_p.e.']] > 10000
                    && ($SA  || $CSA)
                    &&  $agglo[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'PD') {
                    $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['number']++;
                    $agglodebug[$indSup+14] = 'test7=1';
                    if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                		$context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                        //Report discharge without treatment in not compliant case in Art 5
                        if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                            $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe']   += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                        }
                    }
                } else {
                    $agglodebug[$indSup+14] = 'test7=0'; 
                }
            }
            $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number']++;
            if (isset($agglo[ $ind['connected_to_collecting_system_p._e.']])){
                $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] += $agglo[ $ind['connected_to_collecting_system_p._e.']];
                //Report discharge without treatment in not compliant case in Art 5
                if ($agglo[ $ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2 || $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']] > 2000){
                    $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe']   += $agglo[ $ind['neither_collecting_system_nor_ias_p._e.']];
                }
            }

            fputcsv($ragglodebugfile, $agglodebug, ';');
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
            if (isset($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['number']) && isset($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'])) {
                $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] == 0) ? 0 :  $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'];
            }
            
            if (isset($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe']) && isset($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'])) {
                $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] == 0) ? 0 :  $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe'] * 100
                            /  $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'];
            }
            if (isset($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['number']) && isset($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'])) {
                $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number']; 
            }
            if (isset($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['number']) && isset($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'])) {
                $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'];
            }
            
            if (isset($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe']) && isset($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'])) {
                $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'];
            }
            
            if (isset($context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['number']) && isset($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'])) {
                $context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'];
            }
            if (isset($context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['pe']) && isset($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'])) {
                $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['not_connected']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'];
            }
            if ($context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'] != 0) {
                $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['total_actual']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['target']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['agglo']['number'];
            }
            if ($context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'] != 0) {
                $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['total_actual']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'] == 0) ? 0 :  $context['sandbox']['compliance'][$type]['art3']['target']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art3']['exempt_due_to_transition']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art3']['country_total']['waste_water']['pe'];
            }
            //calcul percentage art4
            if ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] != 0) {
                $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['total_actual']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['target']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_total']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['agglo']['number'];
            }
            if ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] != 0) {
                $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['target']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_total']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_size_2000_10000']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_due_nocollecting']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_due_dischargeLSA']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
                $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art4']['exempt_due_to_transition']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art4']['country_total']['waste_water']['pe'];
            }
            //calcul percentage art5
            if ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] != 0) {
                $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['target']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['exempt_total']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['number'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number'];
            }
            if ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] != 0) {
                $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['target']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['exempt_total']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['size_<=_10000']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_<=_10000']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_NA']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];

                $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['percentage'] = ($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'] == 0) ? 0 : $context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe'] * 100 / $context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe'];
            }
        }
//     dsm($compliance);
//     dsm($compliance);
//     return $compliance;
//     require_once(drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php');
        $model_path = uwwtd_getregister_modelfile_path($annee);
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
            $sheet->getCell($col['C'].'12')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['number']);
            if ($col['C'].'12' == 'C12') {
              $context['results']['auto_report']['COMPLIANCE_ART3_NUMBER'] = $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['number'];
            }
            $sheet->getCell($col['D'].'12')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['percentage']);
            if ($col['D'].'12' == 'D12') {
              $context['results']['auto_report']['COMPLIANCE_ART3'] = $context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['agglo']['percentage'];
            }
            $sheet->getCell($col['E'].'12')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['pe']);
            $sheet->getCell($col['F'].'12')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_collected_sytem']['waste_water']['percentage']);
            $sheet->getCell($col['C'].'13')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['number']);
            $sheet->getCell($col['D'].'13')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['agglo']['percentage']);
            $sheet->getCell($col['E'].'13')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['pe']);
            $sheet->getCell($col['F'].'13')->setValue($context['sandbox']['compliance'][$type]['art3']['connected_to_ias']['waste_water']['percentage']);
            $sheet->getCell($col['C'].'14')->setValue($context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['number']);
            $sheet->getCell($col['D'].'14')->setValue($context['sandbox']['compliance'][$type]['art3']['not_connected']['agglo']['percentage']);
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
            if ($col['F'].'21' == 'F21') {
              $context['results']['auto_report']['COMPLIANCE_ART4'] = $context['sandbox']['compliance'][$type]['art4']['total_actual']['waste_water']['percentage'];
            }
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
            if ($type != 'NA') {
                $sheet->getCell($col['C'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['number']);
                $sheet->getCell($col['D'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['agglo']['percentage']);
                $sheet->getCell($col['E'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['pe']);
                $sheet->getCell($col['F'].'31')->setValue($context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['percentage']);
                if ($col['F'].'31' == 'F31') {
                  $context['results']['auto_report']['COMPLIANCE_ART5'] = $context['sandbox']['compliance'][$type]['art5']['total_actual']['waste_water']['percentage'];
                }
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
                $sheet->getCell($col['C'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['number']);
                $sheet->getCell($col['D'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['agglo']['percentage']);
                $sheet->getCell($col['E'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['pe']);
                $sheet->getCell($col['F'].'36')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_CSA_crit_c']['waste_water']['percentage']);
    //             $sheet->getCell($col['C'].'36')->setValue('TODO');
    //             $sheet->getCell($col['D'].'36')->setValue('TODO');
    //             $sheet->getCell($col['E'].'36')->setValue('TODO');
    //             $sheet->getCell($col['F'].'36')->setValue('TODO');
                $sheet->getCell($col['C'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['number']);
                $sheet->getCell($col['D'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['agglo']['percentage']);
                $sheet->getCell($col['E'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['pe']);
                $sheet->getCell($col['F'].'37')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_art54']['waste_water']['percentage']);
    //             $sheet->getCell($col['C'].'37')->setValue('TODO');
    //             $sheet->getCell($col['D'].'37')->setValue('TODO');
    //             $sheet->getCell($col['E'].'37')->setValue('TODO');
    //             $sheet->getCell($col['F'].'37')->setValue('TODO');
                $sheet->getCell($col['C'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['number']);
                $sheet->getCell($col['D'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['agglo']['percentage']);
                $sheet->getCell($col['E'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['pe']);
                $sheet->getCell($col['F'].'38')->setValue($context['sandbox']['compliance'][$type]['art5']['no_collecting']['waste_water']['percentage']);
                $sheet->getCell($col['C'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['number']);
                $sheet->getCell($col['D'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['agglo']['percentage']);
                $sheet->getCell($col['E'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['pe']);
                $sheet->getCell($col['F'].'39')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_LSA']['waste_water']['percentage']);
                $sheet->getCell($col['C'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['number']);
                $sheet->getCell($col['D'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['agglo']['percentage']);
                $sheet->getCell($col['E'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['pe']);
                $sheet->getCell($col['F'].'40')->setValue($context['sandbox']['compliance'][$type]['art5']['size_>_10000_discharge_SA/CSA_pending']['waste_water']['percentage']);
    //             $sheet->getCell($col['C'].'40')->setValue('TODO (A58 ?, pending ?)');
    //             $sheet->getCell($col['D'].'40')->setValue('TODO (A58 ?, pending ?)');
    //             $sheet->getCell($col['E'].'40')->setValue('TODO (A58 ?, pending ?)');
    //             $sheet->getCell($col['F'].'40')->setValue('TODO (A58 ?, pending ?)');
                $sheet->getCell($col['C'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['number']);
                $sheet->getCell($col['D'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['agglo']['percentage']);
                $sheet->getCell($col['E'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['pe']);
                $sheet->getCell($col['F'].'41')->setValue($context['sandbox']['compliance'][$type]['art5']['country_total']['waste_water']['percentage']);
            }
    //         break;
            unset($context['sandbox']['compliance'][$type]);
        }
        unset($context['sandbox']['compliance']);
        //write
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
        $objWriter->setPreCalculateFormulas(false);
        //$objWriter->setOffice2003Compatibility(true);
        $objWriter->save(drupal_realpath($target_path));
        $objPHPExcel->__destruct();
        unset($objPHPExcel);
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
	global $conf;
    $ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet3_agglo']);
    $induww = uwwtd_get_ind($GLOBALS['uwwtd']['sheet2_uwwtp']);
    $listtypestat = array('expired', 'pending');
    $list_type_rca = array('all', 'na', 'sa_csa');
    if (!isset($context['sandbox']['progress'])) {

        $context['sandbox']['progress'] = 0;

        $context['results']['uwwtp_total_load_generated_pe'] = 0;
        $context['results']['uwwtp_notreatment_pe'] = 0;
        $context['results']['uwwtp_ias_pe'] = 0;
        $context['results']['uwwtp_p_as_max_pe'] = 0;
        $context['results']['uwwtp_s_as_max_pe'] = 0;
        $context['results']['uwwtp_n_removal_without_p_pe'] = 0;
        $context['results']['uwwtp_p_removal_without_n_pe'] = 0;
        $context['results']['uwwtp_np_removal_pe'] = 0;
        $context['results']['uwwtp_other_morestringent_pe'] = 0;

        $context['results']['uwwtp_total_load_generated_nb'] = 0;
        $context['results']['uwwtp_notreatment_nb'] = 0;
        $context['results']['uwwtp_ias_nb'] = 0;
        $context['results']['uwwtp_p_as_max_nb'] = 0;
        $context['results']['uwwtp_s_as_max_nb'] = 0;
        $context['results']['uwwtp_n_removal_without_p_nb'] = 0;
        $context['results']['uwwtp_p_removal_without_n_nb'] = 0;
        $context['results']['uwwtp_np_removal_nb'] = 0;
        $context['results']['uwwtp_other_morestringent_nb'] = 0;

        $context['sandbox']['id'] = array();

        $query = 'select n.nid
        from {node} n
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id
            left join
                {field_data_field_agggenerated} f_agggenerated
                    ON n.nid = f_agggenerated.entity_id
            left join
                {field_data_field_status} status
                    ON n.nid = status.entity_id
                    
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
            and f_agggenerated.field_agggenerated_value >= 2000
            and status.field_status_value::integer = 1
        order by f_agggenerated.field_agggenerated_value desc
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
//      dsm($context['sandbox']['max']);

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
            $context['sandbox']['uwwtpinfos_graph'][$uwwtp[$induww['uww_code']]]['design_capacity'] = $uwwtp[$induww['uww_capacity p. e.']];


            $uww_aggcode = $uwwtp[$induww['aggcode']];
//             dsm($uww_aggcode);
            if (false === isset($context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode])) {
//                  dsm('la');
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['capacity'] = 0;

//                 gen load = entering pe * 100 / generated pe
//
//                             %entering / 100 * (colledted pe)
//                                             ( %colledted / 100 * generated pe)
//         $line[$ind['uww_entering this UWWTP % of gen. load']] =
// //              $entering_load_pe * 100 /  ($context['sandbox']['expired']['generated_pe'] + $context['sandbox']['pending']['generated_pe']);
//
//         $line[$ind['uww_entering load  p. e.']] = $entering_load_pe;


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

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_collected_%'] = 0;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_%'] = 0;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_collected_%'] = 0;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_%'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_%'] = 0;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_collected_%'] = 0;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5_C_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5_NC_pe'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5_NR_pe'] = 0;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcaaP'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcaaN'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcab'] = 0;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcac'] = 0;
            }
//             else {
//                 $context['sandbox']['uwwtpinfo_for_agglo'] &= $context['sandbox']['uwwtpinfo_for_agglo'];
//             }
//             dsm($context['sandbox']['uwwtpinfo_for_agglo']);
            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['entering'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['capacity'] += $uwwtp[$induww['uww_capacity p. e.']];


            if ($uwwtp[$induww['art4_final']] == 'C') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['percent']  =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['C']['pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];

            }
            if ($uwwtp[$induww['art4_final']] == 'NC' || $uwwtp[$induww['art4_final']] == 'PD') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['percent']  =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NC']['pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];

                //(entering load for all stations with compliance article 4 =NC))
                $context['results']['uwwtp_notreatment_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];    //eric : desactivate ? todo ?
                $context['results']['uwwtp_notreatment_nb']++;
            }
            if ($uwwtp[$induww['art4_final']] == 'NR') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['percent']  =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NR']['pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            if ($uwwtp[$induww['art4_final']] == 'NI') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['pe']      += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['percent']  =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['NI']['pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            if ($uwwtp[$induww['art4_final']] == 'PD') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['pe']      += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['percent']  =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['PD']['pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            if ($uwwtp[$induww['art4_final']] == '?') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['pe']      += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['percent']  =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['?']['pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            //Secondary: installed?
            if ($uwwtp[$induww['Secondary: installed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_installed'] = 1;
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_percent'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_treatement_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            if ($uwwtp[$induww['Secondary:  monitoring passed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_passed'] = 1;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_percent'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['secondary_monitoring_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            //tertiary install
            if ($uwwtp[$induww['More stringent:  installed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_installed'] = 1;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_percent'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }
            //tertiary install && is the require treatment
            if ($uwwtp[$induww['More stringent:  installed?']] == '1' && $uwwtp[$induww['treatment_met']]=='1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_met_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_met_percent'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_treatement_met_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }


            if ($uwwtp[$induww['More stringent: monitoring passed?']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_passed'] = 1;

                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_percent'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['morestringent_monitoring_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            if ($uwwtp[$induww['uwwtp_collecting_without_treatment']] == '1') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['uwwtp_collecting_without_treatment']++;
            }

            if ($uwwtp[$induww['art4_treatment type']] == 'NC' && false === strpos($uwwtp[$induww['treatment type in place (4)']], '2')) {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment']   += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment_%'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_treatment'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }
            if ($uwwtp[$induww['art4_monitoring']] == 'NC') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring']   += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring_%'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart4_monitoring'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }
            if ($uwwtp[$induww['art5_treatment type']] == 'NC') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment']   += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment_%'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_treatment'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }
            if ($uwwtp[$induww['art5_monitoring']] == 'NC') {
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring']   += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring_%'] =
                            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['distart5_monitoring'] * 100 / $uwwtp[$induww['generated load  p. e.']];
            }

            //for all uwwtp with more stringent treatment, check art5

            if (false !== strpos($uwwtp[$induww['treatment required']], '3')) {
                //5a
                if ($uwwtp[$induww['compliance5a']] == 'C' &&
                    ($uwwtp[$induww['required_rcaaP']] == 1 || $uwwtp[$induww['required_rcaaN']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_C_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];

                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_C_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_C_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
                }

                if ($uwwtp[$induww['compliance5a']] == 'NC' &&
                    ($uwwtp[$induww['required_rcaaP']] == 1 || $uwwtp[$induww['required_rcaaN']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];

                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NC_collected_%'] +=  $uwwtp[$induww['field_agglo_uww_perc_ent_uw']];

                }

                if (($uwwtp[$induww['compliance5a']] == 'NR') &&
                    ($uwwtp[$induww['required_rcaaP']] == 1 || $uwwtp[$induww['required_rcaaN']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NR_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NR_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5a_NR_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
                }

                //5b
                if ($uwwtp[$induww['compliance5b']] == 'C' &&
                    ($uwwtp[$induww['required_rcab']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_C_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
                }

                if ($uwwtp[$induww['compliance5b']] == 'NC' &&
                    ($uwwtp[$induww['required_rcab']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];

                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NC_collected_%'] +=  $uwwtp[$induww['field_agglo_uww_perc_ent_uw']];
                }

                if (($uwwtp[$induww['compliance5b']] == 'NR') &&
                    ($uwwtp[$induww['required_rcab']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5b_NR_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
                }

                //5c
                if ($uwwtp[$induww['compliance5c']] == 'C' &&
                    ($uwwtp[$induww['required_rcac']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_C_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
                }

                if ($uwwtp[$induww['compliance5c']] == 'NC' &&
                    ($uwwtp[$induww['required_rcac']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];

                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NC_collected_%'] +=  $uwwtp[$induww['field_agglo_uww_perc_ent_uw']];
                }

                if (($uwwtp[$induww['compliance5c']] == 'NR') &&
                    ($uwwtp[$induww['required_rcac']] == 1)) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_%']  =
                                $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5c_NR_pe'] * 100 / $uwwtp[$induww['generated load  p. e.']];
                }

                //5
                if ($uwwtp[$induww['art5_final']] == 'C') {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5_C_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                }
                if ($uwwtp[$induww['art5_final']] == 'NC') {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5_NC_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                }
                if ($uwwtp[$induww['art5_final']] == 'NR') {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['art5_NR_pe']  += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                }


                if ($uwwtp[$induww['required_rcaaP']] == 1) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcaaP'] = 1;
                }
                if ($uwwtp[$induww['required_rcaaN']] == 1) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcaaN'] = 1;
                }
                if ($uwwtp[$induww['required_rcab']] == 1) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcab'] = 1;
                }
                if ($uwwtp[$induww['required_rcac']] == 1) {
                    $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['required_rcac'] = 1;
                }
                
                
            }
            $context['sandbox']['uwwtpinfo_for_agglo'][$uww_aggcode]['treatment_met'] = $uwwtp[$induww['treatment_met']];
            //for graph sheet
            $treatmenttypeinplace_tmp = array_flip(explode(';', $uwwtp[$induww['treatment type in place (4)']]));
//             dsm($uwwtp[$induww['treatment type in place (4)']]);
//             dsm($treatmenttypeinplace_tmp);
            if ($uwwtp[$induww['treatment type in place (4)']] == '1') {
                $context['results']['uwwtp_p_as_max_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['results']['uwwtp_p_as_max_nb']++;
            }
            if (isset($treatmenttypeinplace_tmp['2'])
                && !isset($treatmenttypeinplace_tmp['3N'])
                && !isset($treatmenttypeinplace_tmp['3P'])
                && !isset($treatmenttypeinplace_tmp['3NP'])
                && !isset($treatmenttypeinplace_tmp['3other'])
                ) {
                $context['results']['uwwtp_s_as_max_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['results']['uwwtp_s_as_max_nb']++;
            }
            if (isset($treatmenttypeinplace_tmp['3N'])
                && !isset($treatmenttypeinplace_tmp['3P'])
                && !isset($treatmenttypeinplace_tmp['3NP'])
                && ($uwwtp[$induww['Monitoring results -Ntot']] == 'Pass' || $uwwtp[$induww['Monitoring results -Ntot']] == '')
                ) {
                $context['results']['uwwtp_n_removal_without_p_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['results']['uwwtp_n_removal_without_p_nb']++;
            }
            if (isset($treatmenttypeinplace_tmp['3P'])
                && !isset($treatmenttypeinplace_tmp['3N'])
                && !isset($treatmenttypeinplace_tmp['3NP'])
                &&( $uwwtp[$induww['Monitoring results -Ptot']]== 'Pass' || $uwwtp[$induww['Monitoring results -Ptot']] == '')
                ) {
                $context['results']['uwwtp_p_removal_without_n_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['results']['uwwtp_p_removal_without_n_nb']++;
            }
            if (isset($treatmenttypeinplace_tmp['3NP'])
                && !isset($treatmenttypeinplace_tmp['3P'])
                && !isset($treatmenttypeinplace_tmp['3N'])
                && ($uwwtp[$induww['Monitoring results -Ntot']] == 'Pass' || $uwwtp[$induww['Monitoring results -Ntot']] == '')
                && ($uwwtp[$induww['Monitoring results -Ptot']] == 'Pass' || $uwwtp[$induww['Monitoring results -Ptot']] == '')
                ) {
                $context['results']['uwwtp_np_removal_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['results']['uwwtp_np_removal_nb']++;
            }
            if (isset($treatmenttypeinplace_tmp['3other'])
                && !isset($treatmenttypeinplace_tmp['3P'])
                && !isset($treatmenttypeinplace_tmp['3N'])
                && !isset($treatmenttypeinplace_tmp['3NP'])
                ) {
                $context['results']['uwwtp_other_morestringent_pe'] += $uwwtp[$induww['uww_entering_load_calculated_UwwtpAgglo_p._e.']];
                $context['results']['uwwtp_other_morestringent_nb']++;
            }
//             }
        }
        fclose($ruwwtpfile);
//         dsm($context['sandbox']['uwwtpinfo_for_agglo']);

        foreach ($context['sandbox']['uwwtpinfos_graph'] as $uwwtp_info_graph) {
            $context['results']['graph']['total_capacity']['pe'] += $uwwtp_info_graph['design_capacity'];
        }
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
            $ind['connected_to_collecting_system'] => 'collected by collecting system',
            $ind['connected_to_collecting_system_p._e.'] => 'collected by collecting system p. e.',
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
            $ind['ratio_connected/enentering_%'] => 'ratio collected/enentering %',
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
            $ind['dist_art4_treat__%_of_connected_load'] => '% of collected Load',
            $ind['dist_art4_treat_in_p.e._(4)'] => 'in p.e. (4)',
            //performance'] => //Performance',
            $ind['dist_art4_perf_%_of_connected_load'] => '% of collected Load',
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
            
            $ind['global_compliance'] => 'Global compliance',
            
            //current_rca_details'] => //current RCA details',
            $ind['water_body_type'] => 'water body type',
            $ind['id_code_of_the_water_body_wfd'] => 'Id code of the water body WFD',
            $ind['id_of_wfd_river_basin_district'] => 'ID of WFD river basin district',
            $ind['agg_art_applied_from_rca'] => 'agg article applied from rca',
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
            $ind['more_str._treatment_installed_treatment_met_(%_of_gen._load)'] => 'more str. treatment installed treatment met(%_of_gen._load)',
            $ind['more_str._treatment_installed_treatment_met_(p._e.)'] => 'more str. treatment installed treatment met(p. e.)',
           
            $ind['monitoring_results_f._more_string._ok_(%_of_gen._load)'] => 'monitoring results f. more string. OK (% of gen. load)',
            $ind['monitoring_results_f._more_string._ok_(p._e.)'] => 'monitoring results f. more string. OK (p. e.)',

            //data needed fort later sheet
            $ind['required_rcaaP'] => 'required_rcaaP',
            $ind['required_rcaaN'] => 'required_rcaaN',
            $ind['required_rcab'] => 'required_rcab',
            $ind['required_rcac'] => 'required_rcac',
            $ind['agg_art_applied_from_rca'] => 'agg_art_applied_from_rca',

            //debug value
            $ind['compliance_agglo_global'] => 'compliance_agglo_global',
        );
        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');
    }

    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {
        $line = array();
        $fieldsAndValues_EUFeed = array();


		if (isset($context['sandbox']['id'][$context['sandbox']['progress']]))
        	$agg = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
            //Case of art 5.4 applied in RCA 
            /*
            if($agg->isInRca54){
                $agg->field_aggart5compliance['und'][0]['value'] = 'NR';
            }*/
        $uwwtpinfo_for_agglo = &$context['sandbox']['uwwtpinfo_for_agglo'][$agg->field_inspireidlocalid['und'][0]['value']];

// uwwtd_trace($agg->field_inspireidlocalid['und'][0]['value'] . '=');

        //for next sheet
        if (isset($agg->field_agggenerated['und'][0]['value'])) {
        	$context['results']['uwwtp_total_load_generated_pe'] += $agg->field_agggenerated['und'][0]['value'];
        	$context['results']['uwwtp_total_load_generated_nb']++;
        }

        //Agglomeration info',
        //'code',
        $line[$ind['aggcode']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_inspireidlocalid['und'][0]['value']))
        	$line[$ind['aggcode']] = $agg->field_inspireidlocalid['und'][0]['value'];

        //'name',
        $line[$ind['aggname']] = $agg->title;

        //'generated load p.e.',
        $line[$ind['generated_load_p.e.']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $field_agggenerated = 0;
        if (isset($agg->field_agggenerated['und'][0]['value'])) {
        	$line[$ind['generated_load_p.e.']] = $agg->field_agggenerated['und'][0]['value'];
            $field_agggenerated = $agg->field_agggenerated['und'][0]['value'];
        }

        //'deadline for Art. 3 compliance',
        $line[$ind['deadline_for_art._3_compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggperiodover3['und'][0]['value']))
        	$line[$ind['deadline_for_art._3_compliance']] = substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10);

        //'deadline for Art. 4 compliance',
        $line[$ind['deadline_for_art._4_compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggperiodover4['und'][0]['value']))
        	$line[$ind['deadline_for_art._4_compliance']] = substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10);

        //'deadline for Art. 5 compliance',
        $line[$ind['deadline_for_art._5_compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggperiodover5['und'][0]['value']))
        	$line[$ind['deadline_for_art._5_compliance']] = substr($agg->field_aggperiodover5['und'][0]['value'], 0, 10);

        //'Pending or expired deadline EXP/PD/EXP-PD',
        $pending_or_expired = '';

//         if ((substr($line[$ind['deadline_for_art._3_compliance']], 0, 4) < $annee
//              || substr($line[$ind['deadline_for_art._4_compliance']], 0, 4) < $annee
//              || substr($line[$ind['deadline_for_art._5_compliance']], 0, 4) < $annee
//             )
        if (isset($line[$ind['deadline_for_art._3_compliance']]) 
        	|| isset($line[$ind['deadline_for_art._4_compliance']]) 
        	|| isset($line[$ind['deadline_for_art._5_compliance']])) {
        	$deadline_before = (deadline_beforeorequal_to_referenceyear($line[$ind['deadline_for_art._3_compliance']], $annee)
                               || deadline_beforeorequal_to_referenceyear($line[$ind['deadline_for_art._4_compliance']], $annee)
                               || deadline_beforeorequal_to_referenceyear($line[$ind['deadline_for_art._5_compliance']], $annee)
                               );
        }

        if (isset($agg->field_aggcompliance['und'][0]['value'])) {
            if ($annee >= 2018) {$line[$ind['global_compliance']] = $agg->field_aggcompliance['und'][0]['value'];}
	        if ( true === $deadline_before){
	            $pending_or_expired = 'EXP';
	        }else{
	            $pending_or_expired = 'PD';
	        }
        }
        
		if (isset($pending_or_expired))
        	$line[$ind['pending_or_expired_deadline_exp/pd/exp-pd']] = $pending_or_expired;

        //'NUTS 1 code',
        $nuts1 = '';
        if (isset($agg->field_regionnuts['und'][0]['value']) && strlen($agg->field_regionnuts['und'][0]['value']) >= 3) {
            if($nuts = uwwtd_get_nuts(substr($agg->field_regionnuts['und'][0]['value'], 0, 3), 'n1cd', $annee)){
                $line[$ind['nuts_1_code']] = $nuts['n1nm'];
            }
            else{
                $line[$ind['nuts_1_code']] = substr($agg->field_regionnuts['und'][0]['value'], 0, 3);
            }
        } else {
            $line[$ind['nuts_1_code']] = $GLOBALS['uwwtd']['register']['message']['info_not_provided'];
        }
        //'NUTS 2 code',
        $nuts2 = '';
        if (isset($agg->field_regionnuts['und'][0]['value']) && strlen($agg->field_regionnuts['und'][0]['value']) >= 4) {
            if( $nuts = uwwtd_get_nuts(substr($agg->field_regionnuts['und'][0]['value'], 0, 4), 'n2cd', $annee)){
                $line[$ind['nuts_2_code']] = $nuts['n2nm'];
            }
            else{
                $line[$ind['nuts_2_code']] = substr($agg->field_regionnuts['und'][0]['value'], 0, 4);
            }
        } else {
            $line[$ind['nuts_2_code']] = $GLOBALS['uwwtd']['register']['message']['info_not_provided'];
        }
        //'NUTS 3 code',
        $nuts3 = '';
        if (isset($agg->field_regionnuts['und'][0]['value']) && strlen($agg->field_regionnuts['und'][0]['value']) >= 5) {
            if( $nuts = uwwtd_get_nuts(substr($agg->field_regionnuts['und'][0]['value'], 0, 5), 'n3cd', $annee)){
                $line[$ind['nuts_3_code']] = $nuts['n3nm'];
            }
            else{
                $line[$ind['nuts_3_code']] = substr($agg->field_regionnuts['und'][0]['value'], 0, 5);
            }
        } else {
            $line[$ind['nuts_3_code']] = $GLOBALS['uwwtd']['register']['message']['info_not_provided'];
        }

        //Big City info',
        $bc_code = '';
        $bc_name = '';
        if(isset($agg->field_aggbigid['und']) && $agg->field_aggbigid['und'][0]['value']!=''){
            $bc_code = $agg->field_aggbigid['und'][0]['value'];
            $bc_name = $GLOBALS['big_cities'][$bc_code]['name'];
        }
        if ($bc_code == '' && $agg->field_agggenerated['und'][0]['value']>=150000) {
          $bc_code = $GLOBALS['uwwtd']['register']['message']['info_not_provided'];
        }
        $line[$ind['bccode']] = $bc_code;

        //'name',
        if ($bc_name == '') {
          $bc_name = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        }
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
            /*
            if($previousagg->isInRca54){
                $previousagg->field_aggart5compliance['und'][0]['value'] = 'NR';
            }*/
            $previous_reported = 'yes';
//             dsm($previousagg);
            $previous_aggcode =  $previousagg->field_inspireidlocalid['und'][0]['value'];
            $previous_gen = $previousagg->field_agggenerated['und'][0]['value'];

            $previous_percentage = (($agg->field_agggenerated['und'][0]['value'] / $previousagg->field_agggenerated['und'][0]['value']) - 1) * 100;
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
        $connection_total = $agg->field_aggc1['und'][0]['value'] + $agg->field_aggc2['und'][0]['value'] + $agg->field_aggpercwithouttreatment['und'][0]['value'];
        $connection_aggc1 =  $agg->field_aggc1['und'][0]['value'];
        $connection_aggc2 =  $agg->field_aggc2['und'][0]['value'];
        $connection_aggpercwithouttreatment =  $agg->field_aggpercwithouttreatment['und'][0]['value'];
        $line[$ind['total_%_(2)']] = $connection_total;

//         uwwtd_trace('$connection_total = ' . $connection_total);
//         uwwtd_trace('$connection_aggc1 = ' . $connection_aggc1);
//         uwwtd_trace('$connection_aggc2 = ' . $connection_aggc2);
//         uwwtd_trace('$connection_aggpercwithouttreatment = ' . $connection_aggpercwithouttreatment);

        //'connected to collecting system % of gen. load',
        //e.v 2018/10/22 we fill empty cell with value when not in XML
        if (empty($connection_aggc1)) {
          $connection_aggc1 = $connection_total - $connection_aggc2 - $connection_aggpercwithouttreatment;
        }
        if (empty($connection_aggc2)) {
          $connection_aggc2 = $connection_total - $connection_aggc1 - $connection_aggpercwithouttreatment;
        }
        if (empty($connection_aggpercwithouttreatment)) {
          $connection_aggpercwithouttreatment = $connection_total - $connection_aggc1 - $connection_aggc2;
        }

//         uwwtd_trace('after: ' );
//         uwwtd_trace('$connection_aggc1 = ' . $connection_aggc1);
//         uwwtd_trace('$connection_aggc2 = ' . $connection_aggc2);
//         uwwtd_trace('$connection_aggpercwithouttreatment = ' . $connection_aggpercwithouttreatment);

        $line[$ind['connected_to_collecting_system']] = $connection_aggc1;
        //'connected to collecting system p. e.',
        $line[$ind['connected_to_collecting_system_p._e.']] = $connection_aggc1 / 100 * $agg->field_agggenerated['und'][0]['value'];
        //'addressed through IAS % of gen. load',
        $line[$ind['addressed_through_ias_%_of_gen._load']] = $connection_aggc2;
        //'addressed through IAS p. e.',
        $line[$ind['addressed_through_ias_p._e.']] = $connection_aggc2 / 100 * $agg->field_agggenerated['und'][0]['value'];
        //'neither collecting system nor IAS % of gen. load',
        $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] = $connection_aggpercwithouttreatment;
        //'neither collecting system nor IAS p. e.',
        $line[$ind['neither_collecting_system_nor_ias_p._e.']] = $connection_aggpercwithouttreatment / 100 * $agg->field_agggenerated['und'][0]['value'];

        //for next sheet
        $context['results']['uwwtp_ias_pe'] += $line[$ind['addressed_through_ias_p._e.']];
        $context['results']['uwwtp_ias_nb'] += count($agg->field_agguwwliste['und']);
        //(neither collecting system nor IAS)
        $context['results']['uwwtp_notreatment_pe'] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];

        $context['results']['uwwtp_notreatment_pe'] += ($line[$ind['connected_to_collecting_system_p._e.']] - $uwwtpinfo_for_agglo['entering']);


        $line[$ind['dist_art3_%_of_gen._load_(3)']] = $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
        //'in p.e.',
        $line[$ind['dist_art3_in_p.e.']] = $line[$ind['neither_collecting_system_nor_ias_p._e.']];

        //Compliance Art. 3',
        //'Compliance',
        if (deadline_beforeorequal_to_referenceyear($line[$ind['deadline_for_art._3_compliance']], $annee)) {
        	$line[$ind['art3_compliance']] = $agg->field_aggart3compliance['und'][0]['value'];
        } else {
        	$line[$ind['art3_compliance']] = 'PD';
        }

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
		//cn@oieau 19/01/2017 addition in order to add the leak to the C/NC/NR load : leak = connected load - total entering load
		$agg_leak = ($line[$ind['connected_to_collecting_system_p._e.']] - $line[$ind['total_entering_load_p._e.']]);
        //In 2016 for CY, we need to add $line[$ind['neither_collecting_system_nor_ias_p._e.']] in leak value 
        if($annee >= 2016 && $conf['siif_eru_country_code'] == 'cy'){
            $agg_leak+=$line[$ind['neither_collecting_system_nor_ias_p._e.']];
        }
		if ($agg_leak > 0){
			if($agg->field_aggart4compliance['und'][0]['value'] == 'NR'){
				$uwwtpinfo_for_agglo['NR']['pe'] += $agg_leak;
				$uwwtpinfo_for_agglo['NR']['percent'] += ($agg_leak * 100 / $field_agggenerated);
			}	
			elseif ($agg_leak > 2000 ||
				($agg_leak/$line[$ind['connected_to_collecting_system_p._e.']]) > 0.01){
				$uwwtpinfo_for_agglo['NC']['pe'] += $agg_leak;
				$uwwtpinfo_for_agglo['NC']['percent'] += ($agg_leak * 100 / $field_agggenerated);
				if($agg->field_aggart4compliance['und'][0]['value'] == 'C'){
					//dsm('art4 is compliant on the web site but a part is not compliant in the register : '. $agg->nid);
					//dsm($agg);
				}
			}
			else{
				$uwwtpinfo_for_agglo['C']['pe'] += $agg_leak;
				$uwwtpinfo_for_agglo['NC']['percent'] += ($agg_leak * 100 / $field_agggenerated);
			}
		}

		if ($agg_leak > 0){
			if($agg->field_aggart5compliance['und'][0]['value'] == 'NR'){
				$uwwtpinfo_for_agglo['art5_NR_pe'] += $agg_leak;
			}	
			elseif ($agg_leak > 2000 ||
				($agg_leak/$line[$ind['connected_to_collecting_system_p._e.']]) > 0.01){
				$uwwtpinfo_for_agglo['art5_NC_pe'] += $agg_leak;
				if($agg->field_aggart5compliance['und'][0]['value'] == 'C'){
					//dsm('art5 is compliant on the web site but  a part is not compliant in the register '.$agg->nid);
					//dsm($agg);
				}
			}
			else{
				$uwwtpinfo_for_agglo['art5_C_pe'] += $agg_leak;;
			}
		}
		$agg_leak = NULL;
        //'compliant % of gen. load',
        $line[$ind['art4_compliant_%_of_gen._load']] = $uwwtpinfo_for_agglo['C']['percent'];
        //'compliant p. e.',
        $line[$ind['art4_compliant_p._e.']] = $uwwtpinfo_for_agglo['C']['pe'];
        //'not compliant % of gen. load',
        $line[$ind['art4_not_compliant_%_of_gen._load']] = $uwwtpinfo_for_agglo['NC']['percent'];
        //'not compliant',
        $line[$ind['art4_not_compliant']] = $uwwtpinfo_for_agglo['NC']['pe'];
        if($line[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] >= 2){
            $line[$ind['art4_not_compliant_%_of_gen._load']]  +=$line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
            $line[$ind['art4_not_compliant']]+= $line[$ind['neither_collecting_system_nor_ias_p._e.']];
        }
        //'not relevant % of gen. load',
        $line[$ind['art4_not_relevant_%_of_gen._load']] = $uwwtpinfo_for_agglo['NR']['percent'];
        //'not relevant',
        $line[$ind['art4_not_relevant']] = $uwwtpinfo_for_agglo['NR']['pe'];
        //'Compliance Art. 4 (only)',
		//cn@oieau 18/01/2017 modified in order to have the individual compliance and not the hierarchical one
        if (deadline_beforeorequal_to_referenceyear($line[$ind['deadline_for_art._4_compliance']], $annee)) {
			if($agg->field_aggart4compliance['und'][0]['value'] == 'NC' && $line[$ind['art4_not_compliant']] == 0){
			   $line[$ind['art4_compliance_art._4_(only)']] = 'C';
			}
			else {
        	$line[$ind['art4_compliance_art._4_(only)']] = $agg->field_aggart4compliance['und'][0]['value'];
			}
        }elseif( $agg->field_aggart4compliance['und'][0]['value'] == 'NR'){
			$line[$ind['art4_compliance_art._4_(only)']] = 'NR';	
		}else {
        	$line[$ind['art4_compliance_art._4_(only)']] = 'PD';
        }
        //'Compliance Art. 4 (hierarchical approach)',
        $compart4hierarchical = '';
        if ($line[$ind['art4_compliance_art._4_(only)']] == 'NR' ||
            $line[$ind['art4_compliance_art._4_(only)']] == 'NI' ||
            $line[$ind['art4_compliance_art._4_(only)']] == '?' ||
            $line[$ind['art3_compliance']] == 'NR' ||
            $line[$ind['art3_compliance']] == 'NI' ||
            $line[$ind['art3_compliance']] == '?'
            ) {
            $compart4hierarchical = $line[$ind['art4_compliance_art._4_(only)']];
        } elseif (($line[$ind['art3_compliance']] == 'C'
                  || $line[$ind['art3_compliance']] == 'QC')
                  && $line[$ind['art4_compliance_art._4_(only)']] == 'C'
                  ) {
            $compart4hierarchical = 'C';
        } elseif ($line[$ind['art3_compliance']] == 'PD') {
            $compart4hierarchical = 'PD';
        } elseif ($line[$ind['art4_compliance_art._4_(only)']] == 'PD') {
            $compart4hierarchical = 'PD';
        }else {
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

        if ($line[$ind['art4_compliance_art._4_(only)']] == 'NR') {
            $uwwtpinfo_for_agglo['distart4_treatment_%'] = 0;
            $uwwtpinfo_for_agglo['distart4_treatment'] = 0;
            $uwwtpinfo_for_agglo['distart4_monitoring_%'] = 0;
            $uwwtpinfo_for_agglo['distart4_monitoring'] = 0;
        }

		if($agg->field_aggart4compliance['und'][0]['value'] == 'NR'){
			$compliance = 'NR';
		}else{
			$compliance = 'NC';
		}
        //Distance to compliance art. 4',
        //Treatment',
        //'% of connected Load',
        if($line[$ind['generated load  p. e.']]>0) {
            $line[$ind['dist_art4_treat__%_of_connected_load']] =  $uwwtpinfo_for_agglo['distart4_treatment_%'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]) * 100/$line[$ind['generated load  p. e.']];
        }
        else {
            $line[$ind['dist_art4_treat__%_of_connected_load']] ='';
        }
        //'in p.e. (4)',
        $line[$ind['dist_art4_treat_in_p.e._(4)']] = $uwwtpinfo_for_agglo['distart4_treatment'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]);
                
        //Performance',
        //'% of connected Load',
        if($line[$ind['generated load  p. e.']]>0) $line[$ind['dist_art4_perf_%_of_connected_load']] = $uwwtpinfo_for_agglo['distart4_monitoring_%'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]) * 100/$line[$ind['generated load  p. e.']];
        else $line[$ind['dist_art4_perf_%_of_connected_load']] = '';
        //'in p.e. (5)',
        $line[$ind['dist_art4_perf_in_p.e._(5)']] = $uwwtpinfo_for_agglo['distart4_monitoring'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]);
        
        /*nd@oieau.fr, 01/04/2019, block disabled  see : https://github.com/OIEau/uwwtd/issues/499 for more detail
        //If dicharge without treament > 2% and 2000 eq the load is reported in art 4
        if($line[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] >= 2){
            //Treatment',
            $line[$ind['dist_art4_treat__%_of_connected_load']] +=$line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
            $line[$ind['dist_art4_treat_in_p.e._(4)']] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            //Performance',
            $line[$ind['dist_art4_perf_%_of_connected_load']] += $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
            $line[$ind['dist_art4_perf_in_p.e._(5)']]+=$line[$ind['neither_collecting_system_nor_ias_p._e.']];  
        }
        */
        $compliance = NULL;

        //Compliance Art. 5',
        //'compliant 5 a  (% of gen. load)',
        $line[$ind['compliant_5_a_(%_of_gen._load)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5a_C_%'];
        //'compliant 5 a  (p. e.)',
        $line[$ind['compliant_5_a_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5a_C_pe'];
        //'not compliant 5 a  (% of gen. load)',
        $line[$ind['not_compliant_5_a_(%_of_gen._load)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5a_NC_%'];
        //'not compliant 5 a  (p. e.)',
        $line[$ind['not_compliant_5_a_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5a_NC_pe'];
        //'not relevant 5 a   (% of gen. load)',
        $line[$ind['not_relevant_5_a_(%_of_gen._load)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5a_NR_%'];
        //'not relevant 5 a   (p. e.)',
        $line[$ind['not_relevant_5_a_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5a_NR_pe'];

        //'Compliance Art. 5a (only)',


        $line[$ind['compliance_art._5a_(only)']] = uwwtd_register_getcompliance_art5x_only('a', $ind, $line, $uwwtpinfo_for_agglo['art5a_NC_collected_%'], $annee, substr($agg->field_aggperiodover5['und'][0]['value'], 0, 4));


        $line[$ind['compliance_art._5a_(hierarchical_approach)']] = uwwtd_register_getcompliance_art5x_hierarchical('a', $ind, $line);

        //'compliant 5 b    (% of gen. load)',
        $line[$ind['compliant_5_b_(%_of_gen._load)']] =  ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5b_C_%'];
        //'compliant 5 b    (p. e.)',
        $line[$ind['compliant_5_b_(p._e.)']] =  ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5b_C_pe'];
        //'not compliant 5 b  (% of gen. load)',
        $line[$ind['not_compliant_5_b_(%_of_gen._load)']] =  ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5b_NC_%'];
        //'not compliant 5 b   (p. e.)',
        $line[$ind['not_compliant_5_b_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5b_NC_pe'];
        //'not relevant 5 b   (% of gen. load)',
        $line[$ind['not_relevant_5_b_(%_of_gen._load)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5b_NR_%'];
        //'not relevant 5 b   (p. e.)',
        $line[$ind['not_relevant_5_b_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5b_NR_pe'];

        //'Compliance Art. 5b (only)',
        $line[$ind['compliance_art._5b_(only)']] = uwwtd_register_getcompliance_art5x_only('b', $ind, $line, $uwwtpinfo_for_agglo['art5b_NC_collected_%'], $annee, substr($agg->field_aggperiodover5['und'][0]['value'], 0, 4));
        //'Compliance Art. 5b (hierarchical approach)',
        $line[$ind['compliance_art._5b_(hierarchical_approach)']] =  uwwtd_register_getcompliance_art5x_hierarchical('b', $ind, $line);

        //'compliant 5 c      (% of gen. load)',
        $line[$ind['compliant_5_c_(%_of_gen._load)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5c_C_%'];
        //'compliant 5 c       (p. e.)',
        $line[$ind['compliant_5_c__(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5c_C_pe'];
        //'not compliant 5 c   (% of gen. load)',
        $line[$ind['not_compliant_5_c_(%_of_gen._load)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5c_NC_%'];
        //'not compliant 5 c   (p. e.)',
        $line[$ind['not_compliant_5_c_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5c_NC_pe'];
        //'not relevant 5 c   (% of gen. load)',
        $line[$ind['not_relevant_5_c_(%_of_gen._load)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5c_NR_%'];
        //'not relevant 5 c  (p. e.)',
        $line[$ind['not_relevant_5_c_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5c_NR_pe'];

        //'Compliance Art. 5c (only)',
        $line[$ind['compliance_art._5c_(only)']] = uwwtd_register_getcompliance_art5x_only('c', $ind, $line, $uwwtpinfo_for_agglo['art5c_NC_collected_%'], $annee, substr($agg->field_aggperiodover5['und'][0]['value'], 0, 4));
        //'Compliance Art. 5c (hierarchical approach)',
        $line[$ind['compliance_art._5c_(hierarchical_approach)']] =  uwwtd_register_getcompliance_art5x_hierarchical('c', $ind, $line);

        //'compliant Art. 5 (p. e.)',
        $line[$ind['compliant_art._5_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5_C_pe'];
        //'not compliant Art. 5 (p. e.)',
        $line[$ind['not_compliant_art._5_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5_NC_pe'];
        //'Art. 5 not relevant  (p. e.)',
        $line[$ind['art._5_not_relevant_(p._e.)']] = ($agg->field_agggenerated['und'][0]['value'] <= 10000) ? 0 : $uwwtpinfo_for_agglo['art5_NR_pe'];

        //'Compliance Art. 5 (whole agglo)',
		//cn@oieau 18/01/2017 modified in order to have the individual compliance and not the hierarchical one
        if (deadline_beforeorequal_to_referenceyear($line[$ind['deadline_for_art._5_compliance']], $annee)) {
			if($agg->field_aggart5compliance['und'][0]['value'] == 'NC' && $line[$ind['not_compliant_art._5_(p._e.)']] == 0){
			   $line[$ind['compliance_art._5_(whole_agglo)']] = 'C';
			}
			else {
        	   $line[$ind['compliance_art._5_(whole_agglo)']] = $agg->field_aggart5compliance['und'][0]['value'];
			}
        }elseif( $agg->field_aggart5compliance['und'][0]['value'] == 'NR'){
			$line[$ind['compliance_art._5_(whole_agglo)']] = 'NR';	
		}else {
        	$line[$ind['compliance_art._5_(whole_agglo)']] = 'PD';
        }
        //'Compliance Art. 5  (whole agglo) (hierarchical approach)',
        $compart5hierarchical = '';
        if ($line[$ind['compliance_art._5_(whole_agglo)']] == 'NR') {
            $compart5hierarchical = $line[$ind['compliance_art._5_(whole_agglo)']];
        } elseif ($agg->field_agggenerated['und'][0]['value'] <= 10000) {
            $compart5hierarchical = 'NR';
        } elseif ( ($line[$ind['art3_compliance']] == 'C' || $line[$ind['art3_compliance']] == 'QC') &&
                        $line[$ind['art4_compliance_art._4_(only)']] == 'C' &&
                        $line[$ind['compliance_art._5_(whole_agglo)']] == 'C'
                  ) {
            $compart5hierarchical = 'C';
        } elseif ($line[$ind['art3_compliance']] == 'PD' ||
                        $line[$ind['art4_compliance_art._4_(only)']] == 'PD' ||
                        $line[$ind['compliance_art._5_(whole_agglo)']] == 'PD'
                  ) {
            $compart5hierarchical = 'PD';
        } elseif ( $line[$ind['art3_compliance']] == 'NC' ||
                        $line[$ind['art4_compliance_art._4_(only)']] == 'NC' ||
                        $line[$ind['compliance_art._5_(whole_agglo)']] == 'NC'
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
        if ($line[$ind['compliance_art._5_(whole_agglo)']] == 'NR') {
            $uwwtpinfo_for_agglo['distart5_treatment_%'] = 0;
            $uwwtpinfo_for_agglo['distart5_treatment'] = 0;
            $uwwtpinfo_for_agglo['distart5_monitoring_%'] = 0;
            $uwwtpinfo_for_agglo['distart5_monitoring'] = 0;
        }

		if($agg->field_aggart5compliance['und'][0]['value'] == 'NR'){
			$compliance = 'NR';
		}else{
			$compliance = 'NC';
		}
        if($line[$ind['generated load  p. e.']]>0) $line[$ind['distance_art5_treat_%_of_gen._load']] = $uwwtpinfo_for_agglo['distart5_treatment_%'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]) * 100/$line[$ind['generated load  p. e.']];
        else $line[$ind['distance_art5_treat_%_of_gen._load']] = '';
        //'in p.e. (6)',
        $line[$ind['distance_art5_treat_in_p.e._(6)']] = $uwwtpinfo_for_agglo['distart5_treatment'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]);
        //Performance',
        //'% of gen. Load',
        if($line[$ind['generated load  p. e.']]>0) $line[$ind['distance_art5_perf_%_of_gen._load']] = $uwwtpinfo_for_agglo['distart5_monitoring_%'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]) * 100/$line[$ind['generated load  p. e.']];
        else $line[$ind['distance_art5_perf_%_of_gen._load']] = '';
        //'in p.e. (7)',
        $line[$ind['distance_art5_perf_in_p.e._(7)']] = $uwwtpinfo_for_agglo['distart5_monitoring'] + agglo_leak_pe($compliance,$line[$ind['connected_to_collecting_system_p._e.']],$line[$ind['total_entering_load_p._e.']]);
        
        /*/*nd@oieau.fr, 01/04/2019, block disabled  see : https://github.com/OIEau/uwwtd/issues/499 for more detail
        //If dicharge without treament > 2% and 2000 eq the load is reported in art 5
        //Set DTT but if no more stringent treatment is required => this value will be empty later
        if( $line[$ind['compliance_art._5_(whole_agglo)']] != 'NR' && ($line[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] >= 2)){
            if($line[$ind['compliance_art._5_(whole_agglo)']] == 'C')  $line[$ind['compliance_art._5_(whole_agglo)']] = 'NC';
            
            //Treatment',
            $line[$ind['distance_art5_treat_%_of_gen._load']] +=$line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
            $line[$ind['distance_art5_treat_in_p.e._(6)']] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            //Performance',
            $line[$ind['distance_art5_perf_in_p.e._(7)']]+= $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            $line[$ind['distance_art5_perf_%_of_gen._load']] +=$line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']]; 
        }
        */
        
		$compliance = NULL;

        //current RCA details',
        $dcpWaterBodyType = '';
        $dcpWaterBodyTypeCode = '';
        $dcpWaterbodyID = '';
        $dcpWFDRBD = '';
        $dcpTypeOfReceivingArea = '';
        $rcaID = '';
        $rcaName = '';
        $rcaName = '';
        $rcaDateArt54 = '';
        $rcaDateArt58 = '';
        $rcaDateAN = '';
        $rcaDateAP = '';
        $rcaDateB = '';
        $rcaDateC = '';

        $dcpWaterBodyType = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $dcpWaterBodyTypeCode = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $dcpWaterbodyID = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $dcpWFDRBD = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $dcpTypeOfReceivingArea = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaID = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaName = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaDateArt54 = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaDateArt58 = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaDateAN = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaDateAP = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaDateB = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        $rcaDateC = $GLOBALS['uwwtd']['register']['message']['info_empty'];

        $rcatreated = array();
        if (isset($agg->field_agguwwliste['und'])) {  
	        foreach($agg->field_agguwwliste['und'] as $uwwtpnid) {
	            $uwwtp = node_load($uwwtpnid['nid']);
	            if (isset($uwwtp->field_uwwdcpliste['und'])) {
		            foreach($uwwtp->field_uwwdcpliste['und'] as $dcpnid) {
		                $dcp = node_load($dcpnid['nid']);

                        //TODO  CAS NA !!!!!!!!!!!!!!!!!
				        if (isset($dcp->field_dcprcaliste['und']) || $dcp->field_rcatype['und'][0]['value'] == 'NA') {
                            if (isset($dcp->field_dcpwaterbodytype['und'][0]['value'])) {
                              if ($dcpWaterBodyTypeCode == $GLOBALS['uwwtd']['register']['message']['info_empty'] ) {
                                $dcpWaterBodyTypeCode = '';
                              }
                              $dcpWaterBodyTypeCode .= ($dcpWaterBodyTypeCode == '' ? '' : ';') . $dcp->field_dcpwaterbodytype['und'][0]['value'];
                            }
                            switch ($dcp->field_dcpwaterbodytype['und'][0]['value']) {
                                case 'CW':
                                    $field_dcpwaterbodytype = "To coastal water";
                                    break;
                                case 'ES':
                                    $field_dcpwaterbodytype = "To estuary";
                                    break;
                                case 'FW':
                                    $field_dcpwaterbodytype = "To freshwater";
                                    break;
                                case 'LC':
                                    $field_dcpwaterbodytype = "On land (catchment of coastal water)";
                                    break;
                                case 'LF':
                                    $field_dcpwaterbodytype = "On land (catchment of freshwater and/or estuary)";
                                    break;
                                default:
                                    $field_dcpwaterbodytype = "";
                                    break;
                            }


                            // Not used ? BB - 08/03/2016
                            switch ($dcp->field_dcpirrigation['und'][0]['value']) {
                                case 'IR':
                                    $field_dcpirrigation = 'Irrigation';
                                    break;
                                case 'IN':
                                    $field_dcpirrigation = 'Infiltration';
                                    break;
                                default:
                                    $field_dcpirrigation = '';
                                    break;
                            }

                            if ($field_dcpwaterbodytype != '' && !isStringPresentIn($field_dcpwaterbodytype, $dcpWaterBodyType)) {
                                if ($dcpWaterBodyType == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                  $dcpWaterBodyType = '';
                                }
                                $dcpWaterBodyType .= ($dcpWaterBodyType == '' ? '' : ';') . $field_dcpwaterbodytype;
                            }
                            if (isset($dcp->field_dcpwaterbodyid['und'][0]['value']) && !isStringPresentIn($dcp->field_dcpwaterbodyid['und'][0]['value'], $dcpWaterbodyID)) {
                                if ($dcpWaterbodyID == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                  $dcpWaterbodyID = '';
                                }
                                $dcpWaterbodyID .= ($dcpWaterbodyID == '' ? '' : ';') . $dcp->field_dcpwaterbodyid['und'][0]['value'];
                            }
                            if (isset($dcp->field_dcpwfdrbd['und'][0]['value']) && !isStringPresentIn($dcp->field_dcpwfdrbd['und'][0]['value'], $dcpWFDRBD)) {
                                if ($dcpWFDRBD == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                  $dcpWFDRBD = '';
                                }
                                $dcpWFDRBD .= ($dcpWFDRBD == '' ? '' : ';') . $dcp->field_dcpwfdrbd['und'][0]['value'];
                            }
                            if (isset($dcp->field_rcatype['und'][0]['value']) && !isStringPresentIn($dcp->field_rcatype['und'][0]['value'], $dcpTypeOfReceivingArea)) {
                                if ($dcpTypeOfReceivingArea == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                  $dcpTypeOfReceivingArea = '';
                                }
                                $dcpTypeOfReceivingArea .= ($dcpTypeOfReceivingArea == '' ? '' : ';') . $dcp->field_rcatype['und'][0]['value'];
                                
                            }
                            //Manage case of art 5.4 receiving area
                            if($dcp->isInRca54){
                                $dcpTypeOfReceivingArea .= ($dcpTypeOfReceivingArea == '' ? '' : ';') . 'A54';
                            }
                            
                            if(isset($dcp->field_dcprcaliste['und'])){
			                    foreach($dcp->field_dcprcaliste['und'] as $rcanid) {
			                        $rca = node_load($rcanid['nid']);

			                        if (isset($rcatreated[$rca->field_inspireidlocalid['und'][0]['value']])) {
			                            continue;
			                        }

			                        $rcatreated[$rca->field_inspireidlocalid['und'][0]['value']] = true;

                                    if (isset($rca->field_inspireidlocalid['und'][0]['value'])) {
                                      if ($rcaID == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                        $rcaID = '';
                                      }
                                      $rcaID .= ($rcaID == '' ? '' : ';') . $rca->field_inspireidlocalid['und'][0]['value'];
                                    }
                                    
                                    if ($rcaName == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                      $rcaName = '';
                                    }
                                    $rcaName .= ($rcaName == '' ? '' : ';') . $rca->title;

                                    if (isset($rca->field_rcadateart54['und'][0]['value'])) {
                                      if ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                        $rcaDateArt54 = '';
                                      }
                                      $rcaDateArt54 .= ($rcaDateArt54 == '' ? '' : ';') . substr($rca->field_rcadateart54['und'][0]['value'], 0, 10);
                                    }
//                                     $rcaDateArt54 .= ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcadateart54['und'][0]['value'], 0, 10);


                                    if (isset($rca->field_rcadateart5854['und'][0]['value'])) {
                                      if ($rcaDateArt58 == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                        $rcaDateArt58 = '';
                                      }
                                      $rcaDateArt58 .= ($rcaDateArt58 == '' ? '' : ';') . substr($rca->field_rcadateart5854['und'][0]['value'], 0, 10);
                                    }
//                                     $rcaDateArt58 .= ($rcaDateArt58 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcadateart5854['und'][0]['value'], 0, 10);


                                    if (isset($rca->field_rcaanstartdate['und'][0]['value'])) {
                                      if ($rcaDateAN == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                        $rcaDateAN = '';
                                      }
                                      $rcaDateAN .= ($rcaDateAN == '' ? '' : ';') . substr($rca->field_rcaanstartdate['und'][0]['value'], 0, 10);
                                    }
//                                     $rcaDateAN .= ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcaanstartdate['und'][0]['value'], 0, 10);


                                    if (isset($rca->field_rcaapstartdate['und'][0]['value'])) {
                                      if ($rcaDateAP == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                        $rcaDateAP = '';
                                      }
                                      $rcaDateAP .= ($rcaDateAP == '' ? '' : ';') . substr($rca->field_rcaapstartdate['und'][0]['value'], 0, 10);
                                    }
//                                     $rcaDateAP .= ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcaapstartdate['und'][0]['value'], 0, 10);

                                    if (isset($rca->field_rcabstartdate['und'][0]['value'])) {
                                      if ($rcaDateB == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                        $rcaDateB = '';
                                      }
                                      $rcaDateB .= ($rcaDateB == '' ? '' : ';') . substr($rca->field_rcabstartdate['und'][0]['value'], 0, 10);
                                    }
//                                     $rcaDateB .= ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcabstartdate['und'][0]['value'], 0, 10);

                                    if (isset($rca->field_rcacstardate['und'][0]['value'])) {
                                      if ($rcaDateC == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                        $rcaDateC = '';
                                      }
                                      $rcaDateC .= ($rcaDateC == '' ? '' : ';') . substr($rca->field_rcacstardate['und'][0]['value'], 0, 10);
                                    }
//                                     $rcaDateC .= ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcacstardate['und'][0]['value'], 0, 10);


                                    if ($dcp->field_rcatype['und'][0]['value'] == 'NA' || $dcp->field_rcatype['und'][0]['value'] == 'LSA') {
                                        if ($rcaArtApplied == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                          $rcaArtApplied = '';
                                        }
                                        $rcaArtApplied .= ($rcaArtApplied == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . 'NA/LSA';
                                    } else {
                                        if ($rca->field_rca52applied['und'][0]['value'] == '1') {
                                            if ($rcaArtApplied == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                              $rcaArtApplied = '';
                                            }
                                            $rcaArtApplied .= ($rcaArtApplied == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . 'A523';
                                        }
                                        if ($rca->field_rca54applied['und'][0]['value'] == '1') {
                                            if ($rcaArtApplied == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                                              $rcaArtApplied = '';
                                            }                                        
                                            $rcaArtApplied .= ($rcaArtApplied == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . 'A54';
                                        }
                                    }

                                    unset($rca);
			                    }
                            }
				        }
		                unset($dcp);
		            }
	            }
	            unset($uwwtp);
            }
        }
        unset($rcatreated);
        //'water body type',   ?dcpWaterBodyType
        $line[$ind['water_body_type']] = $dcpWaterBodyType;
        //'Id code of the water body WFD',    ?dcpWaterbodyID
        $line[$ind['id_code_of_the_water_body_wfd']] = $dcpWaterbodyID;
        //'ID of WFD river basin district',    ?dcpWFDRBD
        $line[$ind['id_of_wfd_river_basin_district']] = $dcpWFDRBD;

        //var_dump($ind['id_of_wfd_river_basin_district'], $ind['entirely_discharging_into_coastal_waters'], $ind['agg_art_applied_from_rca']) ; die ;

        $line[$ind['agg_art_applied_from_rca']] = $rcaArtApplied;
        $rcaArtApplied = null ;
        //'entirely discharging into coastal waters?',


        $WBT = explode(';', $dcpWaterBodyTypeCode);
        $entirely_discharge_in_CW = (false !== strpos($dcpWaterBodyTypeCode, 'CW') ? 'yes' : 'no');
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
        $line[$ind['starting_date_of_application_5.8.']] = (is_null($rcaDateArt58) ? '-' : $rcaDateArt58);
        //'Starting date of application 5.4.',
        $line[$ind['starting_date_of_application_5.4.']] = (is_null($rcaDateArt54) ? '-' : $rcaDateArt54);
        //'Starting date of application a N',
        $line[$ind['starting_date_of_application_a_n']] = (is_null($rcaDateAN) ? '-' : $rcaDateAN);
        //'Starting date of application a P',
        $line[$ind['starting_date_of_application_a_p']] = (is_null($rcaDateAP) ? '-' : $rcaDateAP);
        //'Starting date of application b',
        $line[$ind['starting_date_of_application_b']] = (is_null($rcaDateB) ? '-' : $rcaDateB);
        //'Starting date of application c',
        $line[$ind['starting_date_of_application_c']] = (is_null($rcaDateC) ? '-' : $rcaDateC);

        //Installations for wastewater treatment in place',
        //'secondary treatment installed (% of gen. load)',
        //Aggregated from sheet   �  UWWTPs  � � via the Excel function of Pivot-tables  (based on parameters   �  UWWTP info, entering this UWWTP  � � and   �  Installations for wastewater treatment in place, Secondary: installed?  � �
        //Calculated on basis of parameters   �  Secondary treatment installed -  %of gen. load  � � and the generated load of the agglomeration (p.e.)
        $secondary_install_pe      = '';
        $secondary_install_percent = '';
        if ($uwwtpinfo_for_agglo['secondary_installed'] == 1) {
            $secondary_install_pe = $uwwtpinfo_for_agglo['secondary_treatement_pe'];
            $secondary_install_percent = $uwwtpinfo_for_agglo['secondary_treatement_percent'];
        }
        $line[$ind['secondary_treatment_installed_(%_of_gen._load)']] = $secondary_install_percent;
        //'secondary treatment installed (p. e.)',
        $line[$ind['secondary_treatment_installed_(p._e.)']] = $secondary_install_pe;

        //'monitoring results f. secondary OK (% of gen. load)',
        $secondary_passed_pe      = '';
        $secondary_passed_percent = '';
        if ($uwwtpinfo_for_agglo['secondary_passed'] == 1) {
            $secondary_passed_pe = $uwwtpinfo_for_agglo['secondary_monitoring_pe'];
            $secondary_passed_percent = $uwwtpinfo_for_agglo['secondary_monitoring_percent'];
        }
        $line[$ind['monitoring_results_f._secondary_ok_(%_of_gen._load)']] = $secondary_passed_percent;
        //'monitoring results f. secondary OK (p. e.)',
        $line[$ind['monitoring_results_f._secondary_ok']] = $secondary_passed_pe;

        //'more str. treatment installed (%_of_gen._load',
        $morestringent_install_pe      = '';
        $morestringent_install_percent = '';
        if ($uwwtpinfo_for_agglo['morestringent_installed'] == 1) {
            $morestringent_install_pe = $uwwtpinfo_for_agglo['morestringent_treatement_pe'];
            $morestringent_install_percent = $uwwtpinfo_for_agglo['morestringent_treatement_percent'];
        }
        
        $line[$ind['more_str._treatment_installed_(%_of_gen._load)']] = $morestringent_install_percent;
        //'more str. treatment installed (p. e.)',
        $line[$ind['more_str._treatment_installed_(p._e.)']] = $morestringent_install_pe;
        
        //cn@oireau 20/10/2016 'more str. treatment installed_treatment_met (%_of_gen._load),
        $morestringent_install_treatment_met_pe      = 0;
        $morestringent_install_treatment_met_percent = 0;
        if ($uwwtpinfo_for_agglo['morestringent_installed'] == 1) {
            $morestringent_install_treatment_met_pe = $uwwtpinfo_for_agglo['morestringent_treatement_met_pe'];
            $morestringent_install_treatment_met_percent = $uwwtpinfo_for_agglo['morestringent_treatement_met_percent'];
        }
        //dsm($uwwtpinfo_for_agglo);

        $line[$ind['more_str._treatment_installed_treatment_met_(%_of_gen._load)']] = $morestringent_install_treatment_met_percent;
        //'more str. treatment installed (p. e.)',
        
        $line[$ind['more_str._treatment_installed_treatment_met_(p._e.)']] = $morestringent_install_treatment_met_pe;


        //'monitoring results f. more string. OK (% of gen. load)',
        $morestringent_passed_pe      = 0;
        $morestringent_passed_percent = 0;
        if ($uwwtpinfo_for_agglo['morestringent_passed'] == 1) {
            $morestringent_passed_pe = $uwwtpinfo_for_agglo['morestringent_monitoring_pe'];
            $morestringent_passed_percent = $uwwtpinfo_for_agglo['morestringent_monitoring_percent'];
        }
        $line[$ind['monitoring_results_f._more_string._ok_(%_of_gen._load)']] = $morestringent_passed_percent;
        //'monitoring results f. more string. OK (p. e.)',
        $line[$ind['monitoring_results_f._more_string._ok_(p._e.)']] =$morestringent_passed_pe;


        if (isset($agg->field_inspireidlocalid['und']) && isset($context['sandbox']['uwwtpinfo_for_agglo'][$agg->field_inspireidlocalid['und'][0]['value']])) {
            unset($context['sandbox']['uwwtpinfo_for_agglo'][$agg->field_inspireidlocalid['und']]);
        }

        //data needed fort later sheet
        $line[$ind['required_rcaaP']] = $uwwtpinfo_for_agglo['required_rcaaP'];
        $line[$ind['required_rcaaN']] = $uwwtpinfo_for_agglo['required_rcaaN'];
        $line[$ind['required_rcab']]  = $uwwtpinfo_for_agglo['required_rcab'];
        $line[$ind['required_rcac']]  = $uwwtpinfo_for_agglo['required_rcac'];

        //data info addied to csv but not xls (only for agglo_compliance sheet 16 )
        $line[$ind['compliance_agglo_global']] = $agg->field_aggcompliance['und'][0]['value'];


        //ev 20190322 ameeliratio,n on col F 'deadline_for_art._5_compliance' 
        //algo bfb "si CF=NA OU si (CF= SA ou CSA et C<10000) OU si (CF= SA ou CSA et CJ=date rempli),  F=NR"
        // $line[$ind['deadline_for_art._5_compliance']]
        // $line[$ind['type_of_receiving_water']]
        // $line[$ind['starting_date_of_application_5.4']]
        // $line[$ind['generated_load_p.e.']]
        if ($line[$ind['deadline_for_art._5_compliance']] == 'NI') {
            $tmp = explode(';', $line[$ind['type_of_receiving_water']]);
            $sa_or_csa = in_array('SA', $tmp) || in_array('CSA', $tmp);
            if (in_array('NA', $tmp)
                || ($sa_or_csa && $line[$ind['generated_load_p.e.']] < 10000)
                || ($sa_or_csa && 1 === preg_match('/[0-9]{4}-[0-9]{2}-[0-9]{2}/', $line[$ind['starting_date_of_application_5.4']]))
                ) {
                    $line[$ind['deadline_for_art._5_compliance']] = 'NR';
            }
            unset($tmp);
        }

        //Before insert the line we need t sort table by key
        ksort($line);
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
            //expired deadline   (toutes les NC +  les C qui ont date d   pass   e)
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

            //pending deadline    PD + les C avec date pas d   pass   e
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

            $context['sandbox'][$typestat]['generated_pe'] += $field_agggenerated;
            $context['sandbox'][$typestat]['previous_generated_pe'] += $line[$ind['previous_gen._load_p.e.']];
//             $context['sandbox'][$typestat]['total_percent'] += $line[$ind['total_%_(2)']];
//             $context['sandbox'][$typestat]['collected_percent'] += $line[$ind['connected_to_collecting_system']];
            $context['sandbox'][$typestat]['collected_pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
//             $context['sandbox'][$typestat]['ias_percent'] += $line[$ind['addressed_through_ias_%_of_gen._load']];
            $context['sandbox'][$typestat]['ias_pe'] += $line[$ind['addressed_through_ias_p._e.']];
//             $context['sandbox'][$typestat]['not_collected_percent'] += $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
            $context['sandbox'][$typestat]['not_collected_pe'] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];

//             $context['sandbox'][$typestat]['distance_art3_percent'] += $line[$ind['dist_art3_%_of_gen._load_(3)']];
            $context['sandbox'][$typestat]['distance_art3_pe'] += $line[$ind['dist_art3_in_p.e.']];

            $context['sandbox'][$typestat]['ratio_entering'] += $line[$ind['total_entering_load_p._e.']];
//             $context['sandbox'][$typestat]['ratio_connected_entering'] += $line[$ind['ratio_connected/enentering_%']];
            $context['sandbox'][$typestat]['ratio_capacity'] += $line[$ind['total_treatment_plant_design_capacity_p._e.']];
//             $context['sandbox'][$typestat]['ratio_entering_capacity'] += $line[$ind['ratio_total_entering_load/total_design_capacity_%']];

//             $context['sandbox'][$typestat]['art4_c_percent'] += $line[$ind['art4_compliant_%_of_gen._load']];
            $context['sandbox'][$typestat]['art4_c_pe'] += $line[$ind['art4_compliant_p._e.']];
//             $context['sandbox'][$typestat]['art4_nc_percent'] += $line[$ind['art4_not_compliant_%_of_gen._load']];
            $context['sandbox'][$typestat]['art4_nc_pe'] += $line[$ind['art4_not_compliant']];
//             $context['sandbox'][$typestat]['art4_nr_percent'] += $line[$ind['art4_not_relevant_%_of_gen._load']];
            $context['sandbox'][$typestat]['art4_nr_pe'] += $line[$ind['art4_not_relevant']];

//             $context['sandbox'][$typestat]['distance_art4_treatment_percent'] += $line[$ind['dist_art4_treat__%_of_connected_load']];
            $context['sandbox'][$typestat]['distance_art4_treatment_pe'] += $line[$ind['dist_art4_treat_in_p.e._(4)']];
//             $context['sandbox'][$typestat]['distance_art4_performance_percent'] += $line[$ind['dist_art4_perf_%_of_connected_load']];
            $context['sandbox'][$typestat]['distance_art4_performance_pe'] += $line[$ind['dist_art4_perf_in_p.e._(5)']];


//             $context['sandbox'][$typestat]['distance_art5_treatment_percent'] += $line[$ind['distance_art5_treat_%_of_gen._load']];
            $context['sandbox'][$typestat]['distance_art5_treatment_pe'] += $line[$ind['distance_art5_treat_in_p.e._(6)']];
//             $context['sandbox'][$typestat]['distance_art5_performance_percent'] += $line[$ind['distance_art5_perf_%_of_gen._load']];
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
            $context['sandbox'][$type_rca]['subjected_load']['generated'] += $field_agggenerated;
            $context['sandbox'][$type_rca]['subjected_load']['collected'] += $line[$ind['connected_to_collecting_system_p._e.']];

            if ($agg->field_aggcompliance['und'][0]['value'] == 'C') {
                $context['sandbox'][$type_rca]['compliant_agglo']++;
                $context['sandbox'][$type_rca]['compliant_load']['generated'] += $field_agggenerated;
                $context['sandbox'][$type_rca]['compliant_load']['collected'] += $line[$ind['connected_to_collecting_system_p._e.']];
            }
            if ($agg->field_aggcompliance['und'][0]['value'] == 'NC') {
                $context['sandbox'][$type_rca]['notcompliant_agglo']++;
                $context['sandbox'][$type_rca]['notcompliant_load']['generated'] += $field_agggenerated;
                $context['sandbox'][$type_rca]['notcompliant_load']['collected'] += $line[$ind['connected_to_collecting_system_p._e.']];
            }
            if ($pending_or_expired == 'PD') {
                $context['sandbox'][$type_rca]['exempted']++;
                $context['sandbox'][$type_rca]['exempted_load']['generated'] += $field_agggenerated;
                $context['sandbox'][$type_rca]['exempted_load']['collected'] += $line[$ind['connected_to_collecting_system_p._e.']];
            }
        }

        ////////////////////////////////////////////////////////////////////////
        //get data for graph sheet
        ////////////////////////////////////////////////////////////////////////

        if ($field_agggenerated >= 2000 && $field_agggenerated < 10001) {
            $context['results']['graph']['0_10000']['pe']  += $field_agggenerated;
            $context['results']['graph']['0_10000']['number']++;
        } elseif ($field_agggenerated >= 10001 && $field_agggenerated < 100001) {
            $context['results']['graph']['10000_100000']['pe']  += $field_agggenerated;
            $context['results']['graph']['10000_100000']['number']++;
        } elseif ($field_agggenerated >= 100001) {
            $context['results']['graph']['100000_n']['pe']  += $field_agggenerated;
            $context['results']['graph']['100000_n']['number']++;
        }

        //array2
        $context['results']['graph']['total_generated']['pe'] += $field_agggenerated;
        $context['results']['graph']['total_collected']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
        $context['results']['graph']['total_load_ias']['pe'] += $line[$ind['addressed_through_ias_p._e.']];
        $context['results']['graph']['total_entering']['pe'] += $line[$ind['total_entering_load_p._e.']];

        //array 5 6 7
        if ($line[$ind['art3_compliance']] == 'C' || $line[$ind['art3_compliance']] == 'QC') {
            $context['results']['graph']['art3_C']['number']++;
            $context['results']['graph']['art3_C']['pe'] += $field_agggenerated;
        }
        if ($line[$ind['art3_compliance']] == 'NC') {
            $context['results']['graph']['art3_NC']['number']++;
            $context['results']['graph']['art3_NC']['pe'] += $field_agggenerated;
        }
        if ($line[$ind['art3_compliance']] == 'NR' || $line[$ind['art3_compliance']] == 'PD') {
            $context['results']['graph']['art3_NR']['number']++;
            $context['results']['graph']['art3_NR']['pe'] += $field_agggenerated;
        }
        //manage report of dwt values at art 4 level
        
        if ($line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'C') {
            $context['results']['graph']['art4_C']['number']++;
            $context['results']['graph']['art4_C']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
            if($line[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                $context['results']['graph']['art4_NC']['pe'] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            }
        }
        if ($line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NC') {
            $context['results']['graph']['art4_NC']['number']++;
            $context['results']['graph']['art4_NC']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
            if($line[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                $context['results']['graph']['art4_NC']['pe'] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            }
        }
        if ($line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NR' || $line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'PD') {
            $context['results']['graph']['art4_NR']['number']++;
            $context['results']['graph']['art4_NR']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
        }
        if ($line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'C') {
            $context['results']['graph']['art5_C']['number']++;
            $context['results']['graph']['art5_C']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
            //manage report of dwt values at art 5 level
            if($line[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                $context['results']['graph']['art5_NC']['pe'] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            }
        }
        if ($line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NC') {
            $context['results']['graph']['art5_NC']['number']++;
            $context['results']['graph']['art5_NC']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
            //manage report of dwt values at art 5 level
            if($line[$ind['neither_collecting_system_nor_ias_p._e.']] > 2000 || $line[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] > 2) {
                $context['results']['graph']['art5_NC']['pe'] += $line[$ind['neither_collecting_system_nor_ias_p._e.']];
            }
        }
        if ($line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'NR' || $line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']] == 'PD') {
            $context['results']['graph']['art5_NR']['number']++;
            $context['results']['graph']['art5_NR']['pe'] += $line[$ind['connected_to_collecting_system_p._e.']];
        }
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////


        // SAVING FOR EU PORTION
        // Search for this string to find all the fields

        if (deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover6['und'][0]['value'], $annee)) {
            $art6_compliance_EU_feed = $agg->field_aggart6compliance['und'][0]['value'];
        } else {
            $art6_compliance_EU_feed = 'PD';
        }

        if ($line[$ind['pending_or_expired_deadline_exp/pd/exp-pd']] == 'EXP') {
            $agg_compliance_EU_feed = $agg->field_aggcompliance['und'][0]['value'];
        } else {
            $agg_compliance_EU_feed = 'PD';
        }

        $agg->field_region_nuts1['und'][0]['value'] = $nuts1;
        $agg->field_region_nuts2['und'][0]['value'] = $nuts2;
        $agg->field_region_nuts3['und'][0]['value'] = $nuts3;
        $agg->field_agg_connected_load_pe['und'][0]['value'] = number_format($line[$ind['connected_to_collecting_system_p._e.']], 0,'.','');
        $agg->field_agg_ias_load_pe['und'][0]['value'] = number_format($line[$ind['addressed_through_ias_p._e.']], 0,'.','');
        $agg->field_agg_discharge_pe['und'][0]['value'] = number_format($line[$ind['neither_collecting_system_nor_ias_p._e.']], 0,'.','');

        $agg->field_agg_art3_comp_register['und'][0]['value'] = $line[$ind['art3_compliance']];
        $agg->field_agg_art4_comp_register['und'][0]['value'] = $line[$ind['art4_compliance_art._4_(hierarchical_approach)']];
        $agg->field_agg_art5_comp_register['und'][0]['value'] = $line[$ind['compliance_art._5_(whole_agglo)_(hierarchical_approach)']];
        $agg->field_agg_art6_comp_register['und'][0]['value'] = $art6_compliance_EU_feed;
        $agg->field_agg_comp_register['und'][0]['value'] = $agg_compliance_EU_feed;

        $agg->field_agg_equip_s['und'][0]['value'] = number_format($line[$ind['secondary_treatment_installed_(p._e.)']], 0,'.','');
        $agg->field_agg_perf_s['und'][0]['value'] = number_format($line[$ind['monitoring_results_f._secondary_ok']], 0,'.','');
        $agg->field_agg_equip_met_ms['und'][0]['value'] = number_format($line[$ind['more_str._treatment_installed_treatment_met_(p._e.)']], 0,'.','');
        $agg->field_agg_perf_met_ms['und'][0]['value'] = number_format($line[$ind['connected_to_collecting_system_p._e.']] - $line[$ind['distance_art5_perf_in_p.e._(7)']], 0,'.','');
        $agg->field_agg_equip_ms['und'][0]['value'] = number_format($line[$ind['more_str._treatment_installed_(p._e.)']], 0,'.','');
        $agg->field_agg_perf_ms['und'][0]['value'] = number_format($line[$ind['monitoring_results_f._more_string._ok_(p._e.)']], 0,'.','');

        //Remove double values
        $torw = implode(';', array_unique (explode(';', $line[$ind['type_of_receiving_water']])));
        $agg->field_agg_typeof_sa['und'][0]['value'] = $torw;
        $agg->field_agg_art_applied_sa['und'][0]['value'] = $line[$ind['agg_art_applied_from_rca']];
        $agg->field_agg_rbd_code['und'][0]['value'] = $line[$ind['id_of_wfd_river_basin_district']];


        node_save($agg);

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

//             ${$typestat}[$ind['total_%_(2)']] = $context['sandbox'][$typestat]['total_percent'] ;

            ${$typestat}[$ind['total_%_(2)']] = ($context['sandbox'][$typestat]['collected_pe'] + $context['sandbox'][$typestat]['ias_pe'] + $context['sandbox'][$typestat]['not_collected_pe']) * 100 / $context['sandbox'][$typestat]['generated_pe'];

//             ${$typestat}[$ind['connected_to_collecting_system']] = $context['sandbox'][$typestat]['collected_percent'] ;
            ${$typestat}[$ind['connected_to_collecting_system']] = $context['sandbox'][$typestat]['collected_pe'] * 100 / $context['sandbox'][$typestat]['generated_pe'];

            ${$typestat}[$ind['connected_to_collecting_system_p._e.']] = $context['sandbox'][$typestat]['collected_pe'] ;

//             ${$typestat}[$ind['addressed_through_ias_%_of_gen._load']] = $context['sandbox'][$typestat]['ias_percent'] ;
            ${$typestat}[$ind['addressed_through_ias_%_of_gen._load']] =  $context['sandbox'][$typestat]['ias_pe'] * 100 / $context['sandbox'][$typestat]['generated_pe'];

            ${$typestat}[$ind['addressed_through_ias_p._e.']] = $context['sandbox'][$typestat]['ias_pe'] ;

//             ${$typestat}[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] = $context['sandbox'][$typestat]['not_collected_percent'] ;
            ${$typestat}[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] = $context['sandbox'][$typestat]['not_collected_pe'] * 100 / $context['sandbox'][$typestat]['generated_pe'];

            ${$typestat}[$ind['neither_collecting_system_nor_ias_p._e.']] = $context['sandbox'][$typestat]['not_collected_pe'] ;

//             ${$typestat}[$ind['dist_art3_%_of_gen._load_(3)']] = $context['sandbox'][$typestat]['distance_art3_percent'] ;
            ${$typestat}[$ind['dist_art3_%_of_gen._load_(3)']] = ${$typestat}[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];

            ${$typestat}[$ind['dist_art3_in_p.e.']] = $context['sandbox'][$typestat]['distance_art3_pe']         ;

            ${$typestat}[$ind['total_entering_load_p._e.']] = $context['sandbox'][$typestat]['ratio_entering'] ;
//             ${$typestat}[$ind['ratio_connected/enentering_%']] = $context['sandbox'][$typestat]['ratio_connected_entering'] ;
            //$line[$ind['total_entering_load_p._e.']] * 100 / $line[$ind['connected_to_collecting_system_p._e.']];
            ${$typestat}[$ind['ratio_connected/enentering_%']] = ${$typestat}[$ind['total_entering_load_p._e.']] * 100 / ${$typestat}[$ind['connected_to_collecting_system_p._e.']];

            ${$typestat}[$ind['total_treatment_plant_design_capacity_p._e.']] = $context['sandbox'][$typestat]['ratio_capacity'] ;
//             ${$typestat}[$ind['ratio_total_entering_load/total_design_capacity_%']] = $context['sandbox'][$typestat]['ratio_entering_capacity'] ;
            //$line[$ind['total_entering_load_p._e.']] * 100 / $line[$ind['total_treatment_plant_design_capacity_p._e.']];
            ${$typestat}[$ind['ratio_total_entering_load/total_design_capacity_%']] =  ${$typestat}[$ind['total_entering_load_p._e.']] * 100 / ${$typestat}[$ind['total_treatment_plant_design_capacity_p._e.']];

//             ${$typestat}[$ind['art4_compliant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_c_percent'] ;
            ${$typestat}[$ind['art4_compliant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_c_pe'] * 100 / $context['sandbox'][$typestat]['generated_pe'];
            ${$typestat}[$ind['art4_compliant_p._e.']] = $context['sandbox'][$typestat]['art4_c_pe']  ;

//             ${$typestat}[$ind['art4_not_compliant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_nc_percent'] ;
            ${$typestat}[$ind['art4_not_compliant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_nc_pe'] * 100 / $context['sandbox'][$typestat]['generated_pe'];
            ${$typestat}[$ind['art4_not_compliant']] = $context['sandbox'][$typestat]['art4_nc_pe'] ;

//             ${$typestat}[$ind['art4_not_relevant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_nr_percent'] ;
            ${$typestat}[$ind['art4_not_relevant_%_of_gen._load']] = $context['sandbox'][$typestat]['art4_nr_pe']  * 100 / $context['sandbox'][$typestat]['generated_pe'];
            ${$typestat}[$ind['art4_not_relevant']] = $context['sandbox'][$typestat]['art4_nr_pe'] ;

//             ${$typestat}[$ind['dist_art4_treat__%_of_connected_load']] = $context['sandbox'][$typestat]['distance_art4_treatment_percent'] ;
            ${$typestat}[$ind['dist_art4_treat__%_of_connected_load']] = $context['sandbox'][$typestat]['distance_art4_treatment_pe'] * 100 / $context['sandbox'][$typestat]['generated_pe'];
            ${$typestat}[$ind['dist_art4_treat_in_p.e._(4)']] = $context['sandbox'][$typestat]['distance_art4_treatment_pe'];

//             ${$typestat}[$ind['dist_art4_perf_%_of_connected_load']] = $context['sandbox'][$typestat]['distance_art4_performance_percent'] ;
            ${$typestat}[$ind['dist_art4_perf_%_of_connected_load']] =   $context['sandbox'][$typestat]['distance_art4_performance_pe']  * 100 / $context['sandbox'][$typestat]['generated_pe'];
            ${$typestat}[$ind['dist_art4_perf_in_p.e._(5)']] = $context['sandbox'][$typestat]['distance_art4_performance_pe'] ;

//             ${$typestat}[$ind['distance_art5_treat_%_of_gen._load']] = $context['sandbox'][$typestat]['distance_art5_treatment_percent'] ;
            ${$typestat}[$ind['distance_art5_treat_%_of_gen._load']] = $context['sandbox'][$typestat]['distance_art5_treatment_pe'] * 100 / $context['sandbox'][$typestat]['generated_pe'];
            ${$typestat}[$ind['distance_art5_treat_in_p.e._(6)']] = $context['sandbox'][$typestat]['distance_art5_treatment_pe'];

//             ${$typestat}[$ind['distance_art5_perf_%_of_gen._load']] = $context['sandbox'][$typestat]['distance_art5_performance_percent'] ;
            ${$typestat}[$ind['distance_art5_perf_%_of_gen._load']] = $context['sandbox'][$typestat]['distance_art5_performance_pe']  * 100 / $context['sandbox'][$typestat]['generated_pe'];
            ${$typestat}[$ind['distance_art5_perf_in_p.e._(7)']] = $context['sandbox'][$typestat]['distance_art5_performance_pe']         ;

            ${$typestat}[$ind['secondary_treatment_installed_(p._e.)']] = $context['sandbox'][$typestat]['secondary_installed'] ;
            ${$typestat}[$ind['monitoring_results_f._secondary_ok']] = $context['sandbox'][$typestat]['secondary_monitoring'] ;
            ${$typestat}[$ind['more_str._treatment_installed_(p._e.)']] = $context['sandbox'][$typestat]['morestringent_installed'] ;
            ${$typestat}[$ind['monitoring_results_f._more_string._ok_(p._e.)']] = $context['sandbox'][$typestat]['morestringent_monitoring'] ;

//             dsm($$typestat);
            for ($j = 1, $max = count($ind); $j < $max; $j++) {
                if ('' !== (${$typestat}[$j])) {
                    //here % col are added but recalculate after the loop
                    $totalline[$j] += ${$typestat}[$j];
                }
            }
            //calcul the %
            $totalline[$ind['total_%_(2)']] = ($totalline[$ind['connected_to_collecting_system_p._e.']] + $totalline[$ind['addressed_through_ias_p._e.']] + $totalline[$ind['neither_collecting_system_nor_ias_p._e.']]) * 100 / $totalline[$ind['generated_load_p.e.']];

            $totalline[$ind['connected_to_collecting_system']] = $totalline[$ind['connected_to_collecting_system_p._e.']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['addressed_through_ias_%_of_gen._load']] = $totalline[$ind['addressed_through_ias_p._e.']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['neither_collecting_system_nor_ias_%_of_gen._load']] = $totalline[$ind['neither_collecting_system_nor_ias_p._e.']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['dist_art3_%_of_gen._load_(3)']] = $totalline[$ind['neither_collecting_system_nor_ias_%_of_gen._load']];
            $totalline[$ind['ratio_connected/enentering_%']] = $totalline[$ind['total_entering_load_p._e.']] * 100 / $totalline[$ind['connected_to_collecting_system_p._e.']];
            $totalline[$ind['ratio_total_entering_load/total_design_capacity_%']] = $totalline[$ind['total_entering_load_p._e.']] * 100 / $totalline[$ind['total_treatment_plant_design_capacity_p._e.']];
            $totalline[$ind['art4_compliant_%_of_gen._load']] = $totalline[$ind['art4_compliant_p._e.']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['art4_not_compliant_%_of_gen._load']] = $totalline[$ind['art4_not_compliant']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['art4_not_relevant_%_of_gen._load']] = $totalline[$ind['art4_not_relevant']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['dist_art4_treat__%_of_connected_load']] = $totalline[$ind['dist_art4_treat_in_p.e._(4)']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['dist_art4_perf_%_of_connected_load']] = $totalline[$ind['dist_art4_perf_in_p.e._(5)']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['distance_art5_treat_%_of_gen._load']] = $totalline[$ind['distance_art5_treat_in_p.e._(6)']] * 100 / $totalline[$ind['generated_load_p.e.']];
            $totalline[$ind['distance_art5_perf_%_of_gen._load']] = $totalline[$ind['distance_art5_perf_in_p.e._(7)']] * 100 / $totalline[$ind['generated_load_p.e.']];
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
 * Return value of Compliant Article 4 no compliant in p.e..
 */
function _get_compliance_art_4_no_compliant_pe($totalEnteringLoad, $ratioConnectedEntering, $defaultAmount) {
    $noCompliancePe = 'Cannot be computed';
    $amount = ($totalEnteringLoad * (100 - $ratioConnectedEntering)) / 100.0;
    if ($amount > 2000) {
        $noCompliancePe = $amount - 2000;
    } else {
        $noCompliancePe = $defaultAmount;
    }
    return $noCompliancePe;
}


/*
 * Return value of Compliant Article 4 no compliant in percent
 */
function _get_compliance_art_4_no_compliant_perc($totalEnteringLoad, $ratioConnectedEntering, $generatedPe, $defaultAmount) {
    $noCompliancePerc = 'Cannot be computed';
    $amount = ($totalEnteringLoad * (100 - $ratioConnectedEntering)) / 100.0;
    if ($amount > 2000) {
        $noCompliancePe = $amount - 2000;
        $noCompliancePerc = ($noCompliancePe * 100) / $generatedPe;
    } else {
        $noCompliancePerc = $defaultAmount;
    }
    return $noCompliancePerc;
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
        //we need to sort rows by generated load of agglomeration
        $query = '
            SELECT n.nid, max(f_agggenerated.field_agggenerated_value) as agggenerated
            FROM {node} n
                LEFT JOIN {field_data_field_anneedata} f_annee ON n.nid = f_annee.entity_id
                LEFT JOIN {field_data_field_uwwaggliste} agg ON n.nid = agg.entity_id
                LEFT JOIN {field_data_field_agggenerated} f_agggenerated ON agg.field_uwwaggliste_nid = f_agggenerated.entity_id
            WHERE n.type = :typenode
                AND f_annee.field_anneedata_value = :annee
            GROUP BY n.nid
            ORDER BY agggenerated desc NULLS LAST
            ;
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

        //load data from previous report 
        $context['sandbox']['previous_data'] = [];
        if (false !== $context['sandbox']['previous_year']) {
            // $previous_model_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_name'], $context['sandbox']['previous_year']);
            $previous_model_path = uwwtd_getregister_finalfile_path($context['sandbox']['previous_year']);
            //dsm($previous_model_path);
            if (true === file_exists($previous_model_path)) {
                $previousobjReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
                $previousobjReader->setIncludeCharts(false);
                $previousobjReader->setLoadSheetsOnly( array(
                                                     $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'],
                                                    ) );
                $previousobjPHPExcel = $previousobjReader->load(drupal_realpath($previous_model_path));
                $previoussheet = $previousobjPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name']);

                //load all col N BD BE BG BH from previous 
                $prevMaxRow = $previoussheet->getHighestRow();
                for ($i = 1; $i < $prevMaxRow; $i++) {
                    $prevCode = $previoussheet->getCell('N'.$i)->getValue();
                    $prevArt4Treatment = $previoussheet->getCell('BD'.$i)->getValue();
                    $prevArt4Monitoring = $previoussheet->getCell('BE'.$i)->getValue();
                    $compArt4 = $previoussheet->getCell('BF'.$i)->getValue();
                    $prevArt5Treatment = $previoussheet->getCell('BG'.$i)->getValue();
                    $prevArt5Monitoring = $previoussheet->getCell('BH'.$i)->getValue();
                    $compArt5 = $previoussheet->getCell('BI'.$i)->getValue();
                    if (!isset($context['sandbox']['previous_data'][$prevCode])) {
                        $context['sandbox']['previous_data'][$prevCode] = [
                            'prevCode' => $prevCode,
                            'prevArt4Treatment' => $prevArt4Treatment,  
                            'prevArt4Monitoring' => $prevArt4Monitoring,
                            'compArt4' => $compArt4,
                            'prevArt5Treatment' => $prevArt5Treatment,
                            'prevArt5Monitoring' => $prevArt5Monitoring,
                            'compArt5' => $compArt5,
                        ];
                    } 
                }
                
                // $previousobjReader->disconnectWorksheets();
                unset($previousobjReader);
            }
        }
        // dsm(array_keys($context['sandbox']['previous_data']));       

    }

    $ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet2_uwwtp']);

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

            $ind['uww_entering_this_UWWTP_UwwtpAgglo_%_of_gen._load'] => 'uww_entering_this_UWWTP_UwwtpAgglo_%_of_gen._load',
            $ind['uww_entering_load_calculated_UwwtpAgglo_p._e.'] => 'uww_entering_load_calculated_UwwtpAgglo_p._e.',

            $ind['uww_entering this UWWTP % of gen. load'] => 'entering this UWWTP % of gen. load',
            $ind['uww_entering load  p. e.'] => 'entering load  p. e.',
            $ind['uww_capacity p. e.'] => 'capacity p. e.',
//             $ind['uww2_status'] => 'status',
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
            $ind['agg article applied from rca'] => 'agg article applied from rca',
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
            $ind['Incoming concentration COD-tot   t/y'] => 'Incoming concentration COD-tot   t/y',
            $ind['Discharged concentration COD-tot  t/y'] => 'Discharged concentration COD-tot  t/y',
            $ind['COD rate of removal %'] => 'COD rate of removal %',
            $ind['average Incoming concentration COD mg/l'] => 'average Incoming concentration COD mg/l',
            $ind['average Discharge concentration COD mg/l'] => 'average Discharge concentration COD mg/l',
            $ind['Incoming loads N-tot   t/y'] => 'Incoming loads N-tot   t/y',
            $ind['Discharged loads N-tot  t/y'] => 'Discharged loads N-tot  t/y',
            $ind['N rate of removal %'] => 'N rate of removal %',
            $ind['average Incoming concentration N mg/l'] => 'average Incoming concentration N mg/l',
            $ind['average Discharge concentration N mg/l'] => 'average Discharge concentration N mg/l',
            $ind['Incoming concentration P-tot   t/y'] => 'Incoming concentration P-tot   t/y',
            $ind['Discharged concentration P-tot  t/y'] => 'Discharged concentration P-tot  t/y',
            $ind['P rate of removal %'] => 'P rate of removal %',
            $ind['average Incoming concentration P mg/l'] => 'average Incoming concentration P mg/l',
            $ind['average Discharge concentration P mg/l'] => 'average Discharge concentration P mg/l',
            // info needed for next sheets
            $ind['uwwtp_collecting_without_treatment'] => 'uwwtp_collecting_without_treatment',
            $ind['required_rcaaP'] => 'required_rcaaP',
            $ind['required_rcaaN'] => 'required_rcaaN',
            $ind['required_rcab'] => 'required_rcab',
            $ind['required_rcac'] => 'required_rcac',
            $ind['field_agglo_uww_perc_ent_uw'] => 'field_agglo_uww_perc_ent_uw',
            $ind['compliance5a'] => 'compliance5a',
            $ind['compliance5b'] => 'compliance5b',
            $ind['compliance5c'] => 'compliance5c',
            $ind['treatment_met'] => 'treatment_met',
            
            
            
            /////debug info
            $ind['debug_pending_or_expired'] => 'debug_pending_or_expired',
            $ind['debug_art4_final'] => 'debug_art4_final',
        );

        fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
        fputcsv($r, $header, ';');
    } else {
        $r = fopen($filename, 'ab');
    }
//     dsm($header);


    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {

//         if ($context['sandbox']['id'][$context['sandbox']['progress']] != 173583 || $context['sandbox']['id'][$context['sandbox']['progress']] != 173557) {
//             $context['sandbox']['progress']++;
//             $j++;
//             continue;
//         }
//         $uwwtp = node_load($niduwwtp);
        $uwwtp = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);
//         dsm($uwwtp->field_uwwaggliste['und']);
        //a uwwtp can be attach to multiple agglo
        $uwwtpCompliance5x = uwwtd_get_compliance_5abc_uwwtp($uwwtp);

        if (!isset($context['sandbox']['list_uwwtp_treated'][$agg->field_inspireidlocalid['und'][0]['value']])) {
            if(isset($uwwtp->field_uwwwastewatertreated['und'])){
                $context['results']['country_volume_waste_water'] += $uwwtp->field_uwwwastewatertreated['und'][0]['value'];
            }
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
            $line[$ind['aggstatus']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_status['und'])) {
                switch ($agg->field_status['und'][0]['value']) {
                    case (0) :
                        $line[$ind['aggstatus']] = 'inactive';
                        break;
                    case (1) :
                        $line[$ind['aggstatus']] = 'active';
                        break;
                    case (2) :
                        $line[$ind['aggstatus']] = 'inactive';
                        break;
                }
            }
            //'generated load  p. e.',
            $line[$ind['generated load  p. e.']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_agggenerated['und'][0]['value'])) {
              $line[$ind['generated load  p. e.']] = $agg->field_agggenerated['und'][0]['value'];
            }

            //'deadline for Art. 3 compliance',
            $line[$ind['deadline for Art. 3 compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_aggperiodover3['und'][0]['value'])) {
              $line[$ind['deadline for Art. 3 compliance']] = substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10);
            }
            
            //'deadline for Art. 4 compliance',
            $line[$ind['deadline for Art. 4 compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_aggperiodover4['und'][0]['value'])) {
              $line[$ind['deadline for Art. 4 compliance']] = substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10);
            }

            //'deadline for Art. 5 compliance',
            $line[$ind['deadline for Art. 5 compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_aggperiodover5['und'][0]['value'])) {
              $line[$ind['deadline for Art. 5 compliance']] = substr($agg->field_aggperiodover5['und'][0]['value'], 0, 10);
            }

            //Collection
            //'collected in collecting system % of gen. load',
            $line[$ind['collected in collecting system % of gen. load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_aggc1['und'][0]['value'])) {
              $line[$ind['collected in collecting system % of gen. load']] = $agg->field_aggc1['und'][0]['value']; //%
            }

            //'collected in collecting system  p. e.',
            $line[$ind['collected in collecting system  p. e.']] = $agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe

            //'addressed through IAS % of gen. load',
            $line[$ind['addressed through IAS % of gen. load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_aggc2['und'][0]['value'])) {
              $line[$ind['addressed through IAS % of gen. load']] = $agg->field_aggc2['und'][0]['value']; //%
            }

            //'addressed through IAS p. e.',
            $line[$ind['addressed through IAS p. e.']] = $agg->field_aggc2['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe

            //'neither collecting system nor IAS % of gen. load',
            $line[$ind['neither collecting system nor IAS % of gen. load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($agg->field_aggpercwithouttreatment['und'][0]['value'])) {
              $line[$ind['neither collecting system nor IAS % of gen. load']] = $agg->field_aggpercwithouttreatment['und'][0]['value']; //%
            }

            //'neither collecting system nor IAS p. e.',
            $line[$ind['neither collecting system nor IAS p. e.']] = $agg->field_aggpercwithouttreatment['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe

            //UWWTP general info
            //'code',
            $line[$ind['uww_code']] = $uwwtp->field_inspireidlocalid['und'][0]['value'];
            //'name',
            $line[$ind['uww_name']] = $uwwtp->title;
            //'status',
            $line[$ind['uww_status']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($uwwtp->field_status['und'])) {
                switch ($uwwtp->field_status['und'][0]['value']) {
                    case (0) :
                        $line[$ind['uww_status']] = 'inactive';
                        break;
                    case (1) :
                        $line[$ind['uww_status']] = 'active';
                        break;
                    case (2) :
                        $line[$ind['uww_status']] = 'inactive';
                        break;
                }
            }
            //load the agg_uwwtp node to get the percentage enering the uwwtp for this agglo
//             dsm($agg);
//             dsm(array('year' => $annee, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value']));
            $siteid = uwwtd_get_siteid('agglo_uww', array('year' => $annee, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value']));
//             dsm($siteid);
            $nidagglouwwtp = uwwtd_check_exist($siteid);
//             dsm($siteid . '/' . $uwwtp->field_inspireidlocalid['und'][0]['value'] . '/' . $uwwtp->nid . '/' . $nidagglouwwtp);

            if ('' != $nidagglouwwtp) {
//                 dsm('ici');
                $agglo_uwwtp = node_load($nidagglouwwtp);
    //             dsm($agglo_uwwtp);
                //'entering this UWWTP % of gen. load',
                //$uww_entering_load_pe = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * ($agg->field_aggc1['und'][0]['value']/ 100 * $agg->field_agggenerated['und'][0]['value']);
//                 $uww_entering_load_pe = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];
                $line[$ind['uww_entering_this_UWWTP_UwwtpAgglo_%_of_gen._load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
                if (isset($agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'])) {
                  $line[$ind['uww_entering_this_UWWTP_UwwtpAgglo_%_of_gen._load']] = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];//$uww_entering_load_pe * 100 / $agg->field_agggenerated['und'][0]['value'];
                }

                //'entering load  p. e.',
                $line[$ind['uww_entering_load_calculated_UwwtpAgglo_p._e.']] = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value'];

//                 $context['results']['uwwtp_notreatment_pe_eric_fuite_detail'][] = [
//                   $agg->field_inspireidlocalid['und'][0]['value'],
//                   $uwwtp->field_inspireidlocalid['und'][0]['value'],
//                   $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'],
//                   $agg->field_agggenerated['und'][0]['value'],
//                   $line[$ind['uww_entering_load_calculated_UwwtpAgglo_p._e.']],
//                 ];

                $field_agglo_uww_perc_ent_uw = $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];
                unset($agglo_uwwtp, $siteid);
            } else {
                $line[$ind['uww_entering_this_UWWTP_UwwtpAgglo_%_of_gen._load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
                //'entering load  p. e.',
                $line[$ind['uww_entering_load_calculated_UwwtpAgglo_p._e.']] = '';
                $field_agglo_uww_perc_ent_uw = 0;
            }

            $line[$ind['uww_entering this UWWTP % of gen. load']] = $uwwtp->field_uwwloadenteringuwwtp['und'][0]['value'] * 100 / $agg->field_agggenerated['und'][0]['value'];

            $line[$ind['uww_entering load  p. e.']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($uwwtp->field_uwwloadenteringuwwtp['und'][0]['value'])) {
              $line[$ind['uww_entering load  p. e.']] = $uwwtp->field_uwwloadenteringuwwtp['und'][0]['value'];
            }

//             dsm($line[$ind['uww_entering_this_UWWTP_UwwtpAgglo_%_of_gen._load']]);
//             dsm($line[$ind['uww_entering_load_calculated_UwwtpAgglo_p._e.']]);
//             dsm($line[$ind['uww_entering this UWWTP % of gen. load']]);
//             dsm($line[$ind['uww_entering load  p. e.']]);


            //'capacity p. e.',
            $line[$ind['uww_capacity p. e.']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($uwwtp->field_physicalcapacityactivity['und'][0]['value'])) {
              $line[$ind['uww_capacity p. e.']] = $uwwtp->field_physicalcapacityactivity['und'][0]['value'];
            }
            //'status',
//             $line[$ind['uww2_status']] = $uwwtp->field_status['und'][0]['value'] == 1 ? 'active' : 'inactive'; //...again...



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
                $previous_entering_load_pe = $previousagglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100 * $previousagglo->field_agggenerated['und'][0]['value'];
                $previous_capacity_pe = $previousuwwtp->field_physicalcapacityactivity['und'][0]['value'];
                
                if (isset($line[$ind['uww_entering_load_calculated_UwwtpAgglo_p._e.']]) && $previous_entering_load_pe > 0) {
                    $previous_percentage_of_change_of_entering_load =  ($line[$ind['uww_entering_load_calculated_UwwtpAgglo_p._e.']] - $previous_entering_load_pe) / $previous_entering_load_pe * 100;
                } else {
                    $previous_percentage_of_change_of_entering_load = 'NI';
                }

                if (isset($line[$ind['uww_capacity p. e.']]) && $line[$ind['uww_capacity p. e.']] != $GLOBALS['uwwtd']['register']['message']['info_empty'] && $previous_capacity_pe > 0) {
                    $previous_percentage_of_change_of_capacity = ($line[$ind['uww_capacity p. e.']] - $previous_capacity_pe) / $previous_capacity_pe* 100;
                } else {
                    $previous_percentage_of_change_of_capacity = 'NI';
                }
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
            $dcpWaterBodyTypeCode = '';
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

            $dcpWaterBodyType = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $dcpWaterBodyTypeCode = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $dcpIrrigation = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $dcpWaterbodyID = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $dcpWFDRBD = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $dcpTypeOfReceivingArea = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $rcaID = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $rcaName = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $rcaName = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $rcaDateArt54 = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $rcaDateArt58 = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $startingDateOfApplication_an = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $startingDateOfApplication_ap = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $startingDateOfApplication_b = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            $startingDateOfApplication_c = $GLOBALS['uwwtd']['register']['message']['info_empty'];

            if(!empty($uwwtp->field_uwwdcpliste)){
             foreach($uwwtp->field_uwwdcpliste['und'] as $dcpnid) {
               $dcp = node_load($dcpnid['nid']);
              
                if (isset($dcp->field_dcpwaterbodytype['und'][0]['value'])) {
                  if ($dcpWaterBodyTypeCode == $GLOBALS['uwwtd']['register']['message']['info_empty'] ) {
                    $dcpWaterBodyTypeCode = '';
                  }
                  $dcpWaterBodyTypeCode .= ($dcpWaterBodyTypeCode == '' ? '' : ';') . $dcp->field_dcpwaterbodytype['und'][0]['value'];
                }
                switch ($dcp->field_dcpwaterbodytype['und'][0]['value']) {
                    case 'CW':
                        $field_dcpwaterbodytype = "To coastal water";
                        break;
                    case 'ES':
                        $field_dcpwaterbodytype = "To estuary";
                        break;
                    case 'FW':
                        $field_dcpwaterbodytype = "To freshwater";
                        break;
                    case 'LC':
                        $field_dcpwaterbodytype = "On land (catchment of coastal water)";
                        break;
                    case 'LF':
                        $field_dcpwaterbodytype = "On land (catchment of freshwater and/or estuary)";
                        break;
                    default:
                        $field_dcpwaterbodytype = "";
                        break;
                }
                if(isset($dcp->field_dcpirrigation['und'])){
                    switch ($dcp->field_dcpirrigation['und'][0]['value']) {
                        case 'IR':
                            $field_dcpirrigation = 'Irrigation';
                            break;
                        case 'IN':
                            $field_dcpirrigation = 'Infiltration';
                            break;
                        default:
                            $field_dcpirrigation = '';
                            break;
                    }
                }else{$field_dcpirrigation = '';}
                if (isset($dcp->field_dcpwaterbodytype['und'][0]['value'])) {       
                  if ($dcpWaterBodyType == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                    $dcpWaterBodyType = '';
                  }                      
                  $dcpWaterBodyType .= ($dcpWaterBodyType == '' ? '' : ';') . $dcp->field_dcpwaterbodytype['und'][0]['value'];
                }
                if (isset($dcp->field_dcpwaterbodyid['und'][0]['value'])) {
                  if ($dcpWaterbodyID == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                    $dcpWaterbodyID = '';
                  }  
                  $dcpWaterbodyID .= ($dcpWaterbodyID == '' ? '' : ';') . $dcp->field_dcpwaterbodyid['und'][0]['value']; 
                }
                if (isset($dcp->field_dcpwfdrbd['und'][0]['value'])) {
                  if ($dcpWFDRBD == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                    $dcpWFDRBD = '';
                  }  
                  $dcpWFDRBD .= ($dcpWFDRBD == ''? '' : ';') . $dcp->field_dcpwfdrbd['und'][0]['value'];   
                }
                
                if (isset($dcp->field_rcatype['und'][0]['value'])) {
                  if ($dcpTypeOfReceivingArea == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                    $dcpTypeOfReceivingArea = '';
                  }  
                  $dcpTypeOfReceivingArea .= ($dcpTypeOfReceivingArea == '' ? '' : ';') . $dcp->field_rcatype['und'][0]['value'];  
                }
                if($dcp->isInRca54){
                    $dcpTypeOfReceivingArea .= ($dcpTypeOfReceivingArea == '' ? '' : ';') . 'A54';  
                }
                    
              
              
              
              if(!empty($dcp->field_dcprcaliste) || $dcp->field_rcatype['und'][0]['value'] == 'NA'){
                foreach($dcp->field_dcprcaliste['und'] as $rcanid) {
                    
                    $rca = node_load($rcanid['nid']);
                    $rcatreated[$rca->field_inspireidlocalid['und'][0]['value']] = true;
                    
                    if (!empty($field_dcpirrigation)) {
                      if ($dcpIrrigation == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $dcpIrrigation = '';
                      }   
                      $dcpIrrigation .= ($dcpIrrigation == '' ? '' : ';') . $field_dcpirrigation;
                    }

                    

                    

                    
                    if (isset($rca->field_inspireidlocalid['und'][0]['value'])) {
                      if ($rcaID == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $rcaID = '';
                      } 
                      $rcaID .= ($rcaID == '' ? '' : ';') . $rca->field_inspireidlocalid['und'][0]['value'];
                    }
                    

                    if ($rcaName == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                      $rcaName = '';
                    }
                    $rcaName .= ($rcaName == '' ? '' : ';') . $rca->title;

                    if (isset($rca->field_rcadateart54['und'][0]['value'])) {
                      if ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $rcaDateArt54 = '';
                      } 
                      $rcaDateArt54 .= ($rcaDateArt54 == '' ? '' : ';') . substr($rca->field_rcadateart54['und'][0]['value'], 0, 10);
                    }
//                     $rcaDateArt54 .= ($rcaDateArt54 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcadateart54['und'][0]['value'], 0, 10);

                    if (isset($rca->field_rcadateart5854['und'][0]['value'])) {
                      if ($rcaDateArt58 == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $rcaDateArt58 = '';
                      } 
                      $rcaDateArt58 .= ($rcaDateArt58 == '' ? '' : ';') . substr($rca->field_rcadateart5854['und'][0]['value'], 0, 10);
                    }
//                     $rcaDateArt58 .= ($rcaDateArt58 == $GLOBALS['uwwtd']['register']['message']['info_empty'] ? '' : ';') . substr($rca->field_rcadateart5854['und'][0]['value'], 0, 10);

                    if (isset($rca->field_rcaanstartdate['und'][0]['value'])) {
                      if ($startingDateOfApplication_an == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $startingDateOfApplication_an = '';
                      } 
                      $startingDateOfApplication_an .= ($startingDateOfApplication_an == '' ? '' : ';') . uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcaanstartdate[LANGUAGE_NONE][0]['value']);
                    }    
//                     $startingDateOfApplication_an = ($rca->field_rcaanstartdate[LANGUAGE_NONE][0]['value'] ? uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcaanstartdate[LANGUAGE_NONE][0]['value']) : '-');

                    if (isset($rca->field_rcaapstartdate['und'][0]['value'])) {
                      if ($startingDateOfApplication_ap == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $startingDateOfApplication_ap = '';
                      } 
                      $startingDateOfApplication_ap .= ($startingDateOfApplication_ap == '' ? '' : ';') . uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcaapstartdate[LANGUAGE_NONE][0]['value']);
                    } 
//                     $startingDateOfApplication_ap = ($rca->field_rcaapstartdate[LANGUAGE_NONE][0]['value'] ? uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcaapstartdate[LANGUAGE_NONE][0]['value']) : '-');

                    if (isset($rca->field_rcabdatedesignation['und'][0]['value'])) {
                      if ($startingDateOfApplication_b == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $startingDateOfApplication_b = '';
                      } 
                      $startingDateOfApplication_b .= ($startingDateOfApplication_b == '' ? '' : ';') . uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcabdatedesignation[LANGUAGE_NONE][0]['value']);
                    } 
//                     $startingDateOfApplication_b = ($rca->field_rcabdatedesignation[LANGUAGE_NONE][0]['value'] ? uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcabdatedesignation[LANGUAGE_NONE][0]['value']) : '-');

                    if (isset($rca->field_rcacdatedesignation['und'][0]['value'])) {
                      if ($startingDateOfApplication_c == $GLOBALS['uwwtd']['register']['message']['info_empty']) {
                        $startingDateOfApplication_c = '';
                      } 
                      $startingDateOfApplication_c .= ($startingDateOfApplication_c == '' ? '' : ';') . uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcacdatedesignation[LANGUAGE_NONE][0]['value']);
                    } 
//                     $startingDateOfApplication_c = ($rca->field_rcacdatedesignation[LANGUAGE_NONE][0]['value'] ? uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($rca->field_rcacdatedesignation[LANGUAGE_NONE][0]['value']) : '-');

                    unset($rca);
                }
              }
              unset($dcp);
             }
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
            $treatment_required = uwwtd_get_register_treatment_required_with_deadline(
                $uwwtp->field_uwwtreatmentrequired['und'][0]['value'],
                substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10),
                substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10),
                substr($agg->field_aggperiodover5['und'][0]['value'], 0, 10),
                $annee
            );


            $line[$ind['treatment required']] = $treatment_required;//$uwwtp->field_uwwtreatmentrequired['und'][0]['value'];

            $previous_treatment_required = '';
            if (isset($previousuwwtp)) {
                $previous_treatment_required = uwwtd_get_register_treatment_required_with_deadline(
                    $previousuwwtp->field_uwwtreatmentrequired['und'][0]['value'],
                    substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10),
                    substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10),
                    substr($agg->field_aggperiodover5['und'][0]['value'], 0, 10),
                    $previous_reference_year
                );
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
            $line[$ind['Monitoring results - BOD5']] = ($uwwtp->field_uwwbod5perf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwbod5perf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
            //'Monitoring results -COD',
            $line[$ind['Monitoring results -COD']] = ($uwwtp->field_uwwcodperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwcodperf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
            //'Monitoring results -Ntot',
            $line[$ind['Monitoring results -Ntot']] = ($uwwtp->field_uwwntotperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwntotperf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
            //'Monitoring results -Ptot',
            $line[$ind['Monitoring results -Ptot']] = ($uwwtp->field_uwwptotperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwptotperf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
            //'Monitoring results -other',
            $line[$ind['Monitoring results -other']] = ($uwwtp->field_uwwotherperf['und'][0]['value'] == 'P' ? 'Pass' : ($uwwtp->field_uwwotherperf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));

            if (isset($previousuwwtp)) {
                $bod5monitoring = ($previousuwwtp->field_uwwbod5perf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwbod5perf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
                //'Monitoring results -COD',
                $codmonitoring = ($previousuwwtp->field_uwwcodperf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwcodperf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
                //'Monitoring results -Ntot',
                $monitoring_Ntot = ($previousuwwtp->field_uwwntotperf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwntotperf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
                //'Monitoring results -Ptot',
                $monitoring_Ptot = ($previousuwwtp->field_uwwptotperf['und'][0]['value'] == 'P' ? 'Pass' : ($previousuwwtp->field_uwwptotperf['und'][0]['value'] == 'F' ? 'Fail' : $GLOBALS['uwwtd']['register']['message']['info_empty']));
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
                                                                              $line[$ind['deadline for Art. 3 compliance']],
                                                                              $line[$ind['deadline for Art. 4 compliance']],
                                                                              $line[$ind['deadline for Art. 5 compliance']],
                                                                              $uwwtp);


            $previous_pending_or_expired = '';
            if (isset($previousuwwtp)) {
                $previous_pending_or_expired = uwwtd_get_register_pending_or_expired_uwwtp($annee,
                                                                                  $line[$ind['deadline for Art. 3 compliance']],
                                                                                  $line[$ind['deadline for Art. 4 compliance']],
                                                                                  $line[$ind['deadline for Art. 5 compliance']],
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
//     //pending deadline    PD + les C avec date pas d   pass   e
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

            //ev 2019/03/20 previous
            // $previous_uwwtp_compliance = array();
            // if (isset($previousagglo) && isset($previousuwwtp)) {
            //     $previous_uwwtp_compliance = uwwtd_article4_5_6_station_compliance($previousagglo, $previousuwwtp);
            // }

            //compliance Art. 4    //secondary treatment in place
            //'treatment type', //Treatment met

            // =IF(
            //     OR(  �  generated load   �  <2000;
            //           �  neither collecting system nor IAS  � � =100;
            //           �  entering this UWWTP  � �=0;
            //           �  deadline for Art. 4 compliance  � � >  � �reference date  � �;
            //           �  UWWTP status  � �=  � �inactive  � �;
            //         AND(
            //             OR(  �  water body type  � �="to coastal water";
            //                  �  water body type  � �="on land (catchment of coastal water)"
            //                );
            //               �  generated load  � � <=10000
            //             )
            //         );
            //         "NR";
            //         IFERROR(IF(SEARCH("1";  � �treatment type  � �)>0;
            //                     "NC"
            //                   );
            //                     IFERROR(IF(SEARCH("0";  � �treatment type  � �)>0;
            //                                 "NC"
            //                                );
            //                                 "C"
            //                             )
            //                 )
            //     )

            $art4_treatment_type = uwwtd_get_register_art4_treatement_type_compliance($treatment_required, $treatmenttypeinplace);
            //ev 2019/03/20 previous
            // $previous_art4_treatment_type = '';
            // if (isset($previousuwwtp)) {
            //     $previous_art4_treatment_type = uwwtd_get_register_art4_treatement_type_compliance($previous_treatment_required, $previous_treatmenttypeinplace);
            // }

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

            // =IF(OR(  �  generated load  � � <2000;
            //          �  neither collecting system nor IAS  � � =100;
            //          �  entering this UWWTP  � �=0;
            //          � �deadline for Art. 4 compliance  � � >  � �reference date  � �;
            //          � � Monitoring results - BOD5  � �="reported as NR";
            //          �  Monitoring results   �  COD  � �="reported as NR";
            //          �  Compliance Art. 4   �   treatment type  � �="NR");
            //        "NR";
            //        IF(OR(;   � �Monitoring results - BOD5  � �="fail  � �;
            //                  �  Monitoring results - BOD5  � �=  � � not reported";
            //                  �  Monitoring results   �  COD  � �="fail";
            //                  �  Monitoring results   �  COD  � �="not reported";
            //                  �  Monitoring results - BOD5  � �="--";
            //                  �  Monitoring results   �  COD  � �="--");
            //                "NC";
            //                IF(AND(  �  Monitoring results - BOD5  � �="pass";
            //                         �  Monitoring results   �  COD  � �="pass");
            //                       "C";
            //                       "NC")
            //          )
            //    )


            $art4_monitoring = uwwtd_get_register_art4_monitoring_compliance($treatment_required, $line[$ind['Monitoring results - BOD5']], $line[$ind['Monitoring results -COD']]);
            //ev 2019/03/20 previous
            // $previous_art4_monitoring = '';
            // if (isset($previousuwwtp)) {
            //     $previous_art4_monitoring = uwwtd_get_register_art4_monitoring_compliance($previous_treatment_required, $bod5monitoring, $codmonitoring);
            // }

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
            // =IF(OR(  �  Compliance Art. 4   �   treatment type  � �="NC";
            //          � �Compliance Art. 4   �   monitoring results  � �="NC");
            //        "NC";
            //        IF(OR(  �  Compliance Art. 4   �   treatment type  � �="NR";
            //                � �Compliance Art. 4   �   monitoring results  � �="NR");
            //              "NR";
            //              IF(AND(  �  Compliance Art. 4   �   treatment type  � �="C";
            //                       � �Compliance Art. 4   �   monitoring results  � �="C");
            //                     "C";
            //                     "EXIT")))
            $art4_final = '';
            if(isset($line[$ind['art4_treatment type']]) || $line[$ind['art4_monitoring']]){
                if ($line[$ind['art4_treatment type']] == 'NC' || $line[$ind['art4_monitoring']] == 'NC') {
                    $art4_final = 'NC';
                } elseif ($line[$ind['art4_treatment type']] == 'NR' || $line[$ind['art4_monitoring']] == 'NR') {
                    $art4_final = 'NR';
                } elseif ($line[$ind['art4_treatment type']] == 'C' && $line[$ind['art4_monitoring']] == 'C') {
                    $art4_final = 'C';
                }
            }

            //compliance Art. 5   //more stringent treatment in place
            //'treatment type',
            $art5_treatment = uwwtd_get_register_art5_treatement_type_compliance($treatment_required, $treatmenttypeinplace);
            //ev 2019/03/20 previous
            // $previous_art5_treatment = '';
            // if (isset($previousuwwtp)) {
            //     $previous_art5_treatment = uwwtd_get_register_art5_treatement_type_compliance($previous_treatment_required, $previous_treatmenttypeinplace);
            // }

            //'monitoring',
            $art5_monitoring = uwwtd_get_register_art5_monitoring_compliance($treatment_required, $line[$ind['Monitoring results -COD']], $line[$ind['Monitoring results - BOD5']], $line[$ind['Monitoring results -Ntot']], $line[$ind['Monitoring results -Ptot']], $line[$ind['Monitoring results -other']], $uwwtp->field_uwww_required_rcab['und'][0]['value'], $uwwtp->field_uwww_required_rcac['und'][0]['value'],$uwwtp->field_uwww_required_rcaap['und'][0]['value'], $uwwtp->field_uwww_required_rcaan['und'][0]['value']);
            //ev 2019/03/20 previous
            // $previous_art5_monitoring = '';
            // if (isset($previousuwwtp)) {
            //     if(!isset($ind['Monitoring results -other']) && !isset($line[$ind['Monitoring results -other']])){
            //         $monit = null;
            //     }
            //     else{
            //         $monit = $line[$ind['Monitoring results -other']];
            //     }
            //     $previous_art5_monitoring = uwwtd_get_register_art5_monitoring_compliance(
            //         $previous_treatment_required,
			// 		$codmonitoring,
			// 		$bod5monitoring,
            //         $monitoring_Ntot, 
            //         $monitoring_Ptot, 
            //         $monit, 
            //         $previousuwwtp->field_uwww_required_rcab['und'][0]['value'], 
            //         $previousuwwtp->field_uwww_required_rcac['und'][0]['value'], 
            //         $uwwtp->field_uwww_required_rcaap['und'][0]['value'], 
            //         $uwwtp->field_uwww_required_rcaan['und'][0]['value']
            //     );
            // }

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
                $line[$ind['art4_treatment type']] = $pending_or_expired;
                $line[$ind['art4_monitoring']] = $pending_or_expired;
                $line[$ind['art4_final']] = $pending_or_expired;
                $line[$ind['art5_treatment type']] = $pending_or_expired;
                $line[$ind['art5_monitoring']] = $pending_or_expired;
                $line[$ind['art5_final']] = $pending_or_expired;
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
                    // $line[$ind['previous_art4_expired_treatment']] = $previous_art4_treatment_type;
                    // $line[$ind['previous_art4_expired_monitoring']] = $previous_art4_monitoring;
                    // $line[$ind['previous_art4_expired_final']] = $previous_uwwtp_compliance['compArt4'];
                    // $line[$ind['previous_art5_expired_treatment type']] = $previous_art5_treatment;
                    // $line[$ind['previous_art5_expired_monitoring']] = $previous_art5_monitoring;
                    // $line[$ind['previous_art5_expired_final']] = $previous_uwwtp_compliance['compArt5'];

                    //ev 2019/03/20 take the data from the last report year instead of calculated another time all the data
                    if (isset($context['sandbox']['previous_data'][$line[$ind['uww_code']]])) {
                        $line[$ind['previous_art4_expired_treatment']] =$context['sandbox']['previous_data'][$line[$ind['uww_code']]]['prevArt4Treatment'];
                        $line[$ind['previous_art4_expired_monitoring']] = $context['sandbox']['previous_data'][$line[$ind['uww_code']]]['prevArt4Treatment'];
                        $line[$ind['previous_art4_expired_final']] = $context['sandbox']['previous_data'][$line[$ind['uww_code']]]['compArt4'];
                        $line[$ind['previous_art5_expired_treatment type']] = $context['sandbox']['previous_data'][$line[$ind['uww_code']]]['prevArt4Treatment'];
                        $line[$ind['previous_art5_expired_monitoring']] = $context['sandbox']['previous_data'][$line[$ind['uww_code']]]['prevArt4Treatment'];
                        $line[$ind['previous_art5_expired_final']] = $context['sandbox']['previous_data'][$line[$ind['uww_code']]]['compArt5'];
                    }

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
            =IF((  �  generated load  � � <2000);
                    "NR";
                    IFERROR (
                            IF(SEARCH("1";  � �treatment type  � �)>0;
                                "0"
                            );
                            IFERROR(
                                    IF(SEARCH("0";  � �treatment type  � �)>0;
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
            =IF(  �  generated load  � � <2000;
                "NR";
                IF(  �  Secondary   �   installed  � � =0;
                    0;
                    IF(AND(  �  Monitoring results - BOD5  � �="pass";
                             �  Monitoring results   �  COD  � �);
                        1;
                        0)))
            */
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $secondarypass = 'NR';
            //  BB - 09/01
            // Commented to fit with condition : Monitoring OK even if equipment missing (as long as bod5 and cod5 OK)
            // } elseif ($secondaryinstalled == '0') {
            //     $secondarypass = '0';
            } elseif (isset($uwwtp->field_uwwbod5perf['und']) && isset($uwwtp->field_uwwcodperf['und']) && $uwwtp->field_uwwbod5perf['und'][0]['value'] == 'P' && $uwwtp->field_uwwcodperf['und'][0]['value'] == 'P') {
                $secondarypass = '1';
            } else {
                $secondarypass = '0';
            }
            $line[$ind['Secondary:  monitoring passed?']] = $secondarypass;
            //'Secondary:  FINAL',
            /*
            =IF(  �  generated load  � � <2000;
                "NR";
                  �  Secondary   �   installed  � � *   �  Secondary - monitoring passed?  � �)
            */
            $secondaryfinal = ($secondaryinstalled == '1' && $secondarypass == '1' ? '1': '0');
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $secondaryfinal = 'NR';
            }
            $line[$ind['Secondary:  FINAL']] = $secondaryfinal;
            //'More stringent:  installed?',
            /*
            =IF(OR(  �  neither collecting system nor IAS  � � =100;
                     �  entering this UWWTP  � � =0);
                   0;
                   IF(  �  generated load  � � <2000;
                    "NR";
                    IFERROR(IF(SEARCH("3";  � �treatment type  � �)>0;
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
            // =IF(  �  generated load  � � <2000;
            //     "NR";
            //     IFERROR(IF(AND(
            //                     FIND("3NP";  � �treatment type  � �)>0;
            //                     OR(AND(  �  Monitoring results   �  Ntot  � �="pass";
            //                              � �Monitoring results   �  Ptot  � �="pass");
            //                        AND(  �  Monitoring results   �  Ntot  � �="pass";
            //                              � �Monitoring results   �  Ptot  � �="not relevant";
            //                              � �SECONDARY:  MONITORING PASSED  � �=1);
            //                        AND(  �  Monitoring results   �  Ntot  � �="not relevant";
            //                              � �Monitoring results   �  Ptot  � �="not relevant";
            //                              � �SECONDARY:  MONITORING PASSED  � �=1);
            //                        AND(  �  Monitoring results   �  Ntot  � �="not relevant";
            //                              � �Monitoring results   �  Ptot  � �="pass";
            //                              � �SECONDARY:  MONITORING PASSED  � �=1)
            //                       )
            //                     ) ;
            //                     1;
            //                     0
            //                 );
            //             IFERROR(IF(AND(
            //                             FIND("3N";  � �treatment type  � �)>0;
            //                             OR(  �  Monitoring results   �  Ntot  � �="pass";
            //                                AND(  �  Monitoring results   �  Ntot  � �="not relevant";
            //                                      � �SECONDARY:  MONITORING PASSED  � �=1)
            //                                )
            //                           ) ;
            //                         1;
            //                         0);
            //                       IFERROR(IF(AND(
            //                                      FIND("3P";  � �treatment type  � �)>0;
            //                                      OR(  �  Monitoring results   �  Ptot  � �="pass";
            //                                         AND(  �  Monitoring results   �  Ptot  � �="not relevant";
            //                                               � �SECONDARY:  MONITORING PASSED  � �=1)
            //                                         )
            //                                      );
            //                                 1;
            //                                 0);
            //                             "EXIT")
            //                     )
            //          ))
            // Part 2
            // =IF(NOT(  �  Part 1  � �="EXIT");  � �Part 1  � �;
            //     IFERROR(IF(AND(FIND("3other";  � �treatment type  � �)>0;
            //                      � �SECONDARY:  MONITORING PASSED  � �=1);
            //                1;
            //                0);
            //             IFERROR(IF(AND(FIND("2";  � �treatment type  � �)>0;
            //                            OR(  �  Monitoring results   �  Ntot  � �="pass";
            //                                  � �Monitoring results   �  Ptot  � �="pass");
            //                                  � �SECONDARY:  MONITORING PASSED  � �=1
            //                            );
            //                         1;
            //                         0);
            //                       IF(OR(  �  treatment type  � �=0;
            //                               � �treatment type  � �=1
            //                             );
            //                         0;
            //                         0)
            //                       )
            //               )
            //     )
//             */
//cn@oieau 20/10/2016 modified 
            // $morestringentpassed = '';
             
			if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $morestringentpassed = 'NR';
            } elseif ($art5_monitoring == 'NC') {
				$morestringentpassed = '0';
            } else {
            	if (false !== strpos($treatmenttypeinplace, '3NP')) {
                	if ($line[$ind['Monitoring results -Ntot']] == 'Pass' &&
                    	$line[$ind['Monitoring results -Ptot']] == 'Pass' &&
                    	$secondarypass == '1') {
                    		$morestringentpassed = '1';
	                } else {
	                    $morestringentpassed = '0';
	                }
	            } elseif (false !== strpos($treatmenttypeinplace, '3P')) {
	                if ($line[$ind['Monitoring results -Ptot']] == 'Pass' &&
	                    $secondarypass == '1') {
	                    $morestringentpassed = '1';
	                } else {
	                    $morestringentpassed = '0';
	                }
	            } elseif (false !== strpos($treatmenttypeinplace, '3N')) {
	                if ($line[$ind['Monitoring results -Ntot']] == 'Pass' &&
	                    $secondarypass == '1') {
	                    $morestringentpassed = '1';
	                } else {
	                    $morestringentpassed = '0';
	                }
	            } elseif (false !== strpos($treatmenttypeinplace, '3other')) {
	                if ($secondarypass == '1') {
	                    $morestringentpassed = '1';
	                } else {
	                    $morestringentpassed = '0';
	                }
	            } elseif ($secondarypass == '1') {
	                if ($line[$ind['Monitoring results -Ntot']] == 'Pass' ||
	                    $line[$ind['Monitoring results -Ptot']] == 'Pass') {
	                    $morestringentpassed = '1';
	                } else {
	                    $morestringentpassed = '0';
	                }
	            } else {
	                $morestringentpassed = '0';
	            }
            }

            $line[$ind['More stringent: monitoring passed?']] = $morestringentpassed;
            //'More stringent:  FINAL',
            /*
            =IF(  �  generated load  � � <2000;
                "NR";   �  More stringent  - installed  � � *   �  More stringent - monitoring passed  � �)
            */
            $morestringentfinal = ($morestringentinstalled == '1' && $morestringentpassed == '1' ? '1': '0');
            if ($agg->field_agggenerated['und'][0]['value'] < 2000) {
                $morestringentfinal = 'NR';
            }
            $line[$ind['More stringent:  FINAL']] = $morestringentfinal;

            //UWWTP incoming and discharged load
            //'Volume of waste water treated' 'm3/y',     field_uwwwastewatertreated
            $line[$ind['Volume of waste water treated m3/y']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            if (isset($uwwtp->field_uwwwastewatertreated['und'][0]['value'])) {
              $line[$ind['Volume of waste water treated m3/y']] = $uwwtp->field_uwwwastewatertreated['und'][0]['value'];
            }
            

            //'Incoming loads BOD-tot  t/y',       field_uwwbodincoming
            $bodincoming = (float) uwwtd_get_multiple_field_value($uwwtp, ['field_uwwbodincoming',  'field_uwwbodincomingcalculated', 'field_uwwbodincomingestimated'], -1);
            if ($bodincoming > 0) {
                $line[$ind['Incoming loads BOD-tot  t/y']] = $bodincoming;
                $context['results']['graph']['incoming_load_bod'] += $line[$ind['Incoming loads BOD-tot  t/y']];
            } else {
                $line[$ind['Incoming loads BOD-tot  t/y']] =  $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }

            //'Discharged loads BOD-tot  t/y',    field_uwwboddischarge
            $boddischarge = (float) uwwtd_get_multiple_field_value($uwwtp, ['field_uwwboddischarge',  'field_uwwboddischargecalculated', 'field_uwwboddischargeestimated'], -1);
            if ($boddischarge > 0) {
                $line[$ind['Discharged loads BOD-tot  t/y']] = $boddischarge;
                $context['results']['graph']['discharge_load_bod'] += $line[$ind['Discharged loads BOD-tot  t/y']] ;
            } else {
                $line[$ind['Discharged loads BOD-tot  t/y']] =  $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }

            //'BOD rate of removal %',      round(((1- ($uwwtp->field_uwwboddischarge['und'][0]['value'] / $uwwtp->field_uwwbodincoming['und'][0]['value']))*100), 2)
            if ($bodincoming > 0 && $boddischarge > 0) {
                $line[$ind['BOD rate of removal %']] = (1- ($boddischarge / $bodincoming))*100;
            } else {
                $line[$ind['BOD rate of removal %']] = '';
            }

            //'average Incoming concentration BOD mg/l', ($uwwtp->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($bodincoming >0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])) {
                $line[$ind['average Incoming concentration BOD mg/l']] = ($bodincoming / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Incoming concentration BOD mg/l']] = '';
            }

            //'average Discharge concentration BOD mg/l', ($uwwtp->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($boddischarge > 0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])) {
                $line[$ind['average Discharge concentration BOD mg/l']] = ($boddischarge / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Discharge concentration BOD mg/l']] = '';
            }

            //'Incoming concentration COD-tot   t/y',   field_uwwcodincoming
            $codincoming = (float) uwwtd_get_multiple_field_value($uwwtp, ['field_uwwbodincoming',  'field_uwwbodincomingcalculated', 'field_uwwbodincomingestimated'], -1);
            if ( $codincoming > -1) {
                $line[$ind['Incoming concentration COD-tot   t/y']] =  $codincoming;
                $context['results']['graph']['incoming_load_cod'] += $line[$ind['Incoming concentration COD-tot   t/y']];
            } else {
                $line[$ind['Incoming concentration COD-tot   t/y']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }

            //'Discharged concentration COD-tot  t/y',    field_uwwcoddischarge
            $coddischarge = (float) uwwtd_get_multiple_field_value($uwwtp, ['field_uwwcoddischarge',  'field_uwwcoddischargecalculated', 'field_uwwcoddischargeestimated'], -1);
            if ( $coddischarge > -1) {
                $line[$ind['Discharged concentration COD-tot  t/y']] = $coddischarge;
                $context['results']['graph']['discharge_load_cod'] += $line[$ind['Discharged concentration COD-tot  t/y']];
            } else {
                $line[$ind['Discharged concentration COD-tot  t/y']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }

            //'COD rate of removal %',   round(( (1-($uwwtp->field_uwwcoddischarge['und'][0]['value'] / $uwwtp->field_uwwcodincoming['und'][0]['value'])) *100), 2) . '%'
            if ($coddischarge > 0 && $codincoming > 0) {
                $line[$ind['COD rate of removal %']] =  ( (1-($coddischarge / $codincoming)) *100);
            } else {
                $line[$ind['COD rate of removal %']] = '';
            }

            //'average Incoming concentration COD mg/l',  ($uwwtp->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($codincoming >0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])) {
                $line[$ind['average Incoming concentration COD mg/l']] = ($codincoming / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Incoming concentration COD mg/l']] = '';
            }

            //'average Discharge concentration COD mg/l',  ($uwwtp->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($coddischarge >0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])) {
                $line[$ind['average Discharge concentration COD mg/l']] = ($coddischarge / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Discharge concentration COD mg/l']] = '';
            }

            //'Incoming loads N-tot   t/y',   field_uwwnincoming
            $nincoming = uwwtd_get_multiple_field_value($uwwtp, ['field_uwwnincoming',  'field_uwwnincomingcalculated', 'field_uwwnincomingestimated'], -1);
            if ($nincoming>0) {
                $line[$ind['Incoming loads N-tot   t/y']] = $nincoming;
                $context['results']['graph']['incoming_load_n'] += $line[$ind['Incoming loads N-tot   t/y']];
            } else {
                $line[$ind['Incoming loads N-tot   t/y']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }
            //'Discharged loads N-tot  t/y',   field_uwwndischarge
            $ndischarge = uwwtd_get_multiple_field_value($uwwtp, ['field_uwwndischarge', 'field_uwwndischargecalculated', 'field_uwwndischargeestimated'], -1);
            if ($ndischarge >0) {
                $line[$ind['Discharged loads N-tot  t/y']] = $ndischarge ;
                $context['results']['graph']['discharge_load_n'] += $context['results']['graph']['discharge_load_n'];
            } else {
                $line[$ind['Discharged loads N-tot  t/y']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }

            //'N rate of removal %',   round(( (1- ($uwwtp->field_uwwndischarge['und'][0]['value'] / $uwwtp->field_uwwnincoming['und'][0]['value'])) *100), 2) . '%'
            if ($ndischarge  > 0 && $nincoming > 0) {
                $line[$ind['N rate of removal %']] = ( 1- ($ndischarge  / $nincoming) *100);
            } else {
                $line[$ind['N rate of removal %']] = '';
            }


            //'average Incoming concentration N mg/l',         ($uwwtp->field_uwwnincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($nincoming>0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])) {
                $line[$ind['average Incoming concentration N mg/l']] = ($nincoming / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Incoming concentration N mg/l']] = '';
            }

            //'average Discharge concentration N mg/l',    ($uwwtp->field_uwwndischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($ndischarge >0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])) {
                $line[$ind['average Discharge concentration N mg/l']] = ($ndischarge  / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Discharge concentration N mg/l']] = '';
            }

            //'Incoming concentration P-tot   t/y',    field_uwwpincoming
            $pincoming = uwwtd_get_multiple_field_value($uwwtp, ['field_uwwpincoming',  'field_uwwpincomingcalculated', 'field_uwwpincomingestimated'], -1);
            if ($pincoming>0) {
                $line[$ind['Incoming concentration P-tot   t/y']] = $pincoming;
                $context['results']['graph']['incoming_load_p'] += $line[$ind['Incoming concentration P-tot   t/y']];
            } else {
                $line[$ind['Incoming concentration P-tot   t/y']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }

            //'Discharged concentration P-tot  t/y',    field_uwwpdischarge
            $pdischarge = uwwtd_get_multiple_field_value($uwwtp, ['field_uwwpdischarge', 'field_uwwpdischargecalculated', 'field_uwwpdischargeestimated'], -1);
            if ($pdischarge>0) {
                $line[$ind['Discharged concentration P-tot  t/y']] = $pdischarge;
                $context['results']['graph']['discharge_load_p'] += $line[$ind['Discharged concentration P-tot  t/y']];
            } else {
                $line[$ind['Discharged concentration P-tot  t/y']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
            }

            //'P rate of removal %',   round(( (1-($uwwtp->field_uwwpdischarge['und'][0]['value'] / $uwwtp->field_uwwpincoming['und'][0]['value'])) *100), 2) . '%'
            if ($pdischarge > 0 && $pincoming > 0) {
                $line[$ind['P rate of removal %']] = ( (1-($pdischarge / $pincoming)) *100);
            } else {
                $line[$ind['P rate of removal %']] = '';
            }


            //'average Incoming concentration P mg/l',  ($uwwtp->field_uwwpincoming[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($pincoming>0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])) {
                $line[$ind['average Incoming concentration P mg/l']] = ($pincoming / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Incoming concentration P mg/l']] = '';
            }

            //'average Discharge concentration P mg/l', ($uwwtp->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000;
            if ($pdischarge>0 && isset($uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) && $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']>0) {
                $line[$ind['average Discharge concentration P mg/l']] = ($pdischarge / $uwwtp->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
            } else {
                $line[$ind['average Discharge concentration P mg/l']] = '';
            }

            //needed on next sheets :
            //here same test as in tpl node--uwwtp.tpl.php
            // if((!isset($uwwtp->field_uwwcollectingsystem['und'][0]['value'])
                // || $uwwtp->field_uwwcollectingsystem['und'][0]['value'] == 'NOTCON')
               // ||
               // (!isset($uwwtp->field_physicalcapacityactivity['und'][0]['value'])
                // || $uwwtp->field_physicalcapacityactivity['und'][0]['value'] == '0')){
                // $line[$ind['uwwtp_collecting_without_treatment']] = 1;
            // } else {
                // $line[$ind['uwwtp_collecting_without_treatment']] = 0;
            // }

            // Ugly fix for #166 but hey! It works!
            $line[$ind['uwwtp_collecting_without_treatment']]++;

            //here field for next sheet
            $line[$ind['required_rcaaP']] = $uwwtp->field_uwww_required_rcaap['und'][0]['value'];
            $line[$ind['required_rcaaN']] = $uwwtp->field_uwww_required_rcaan['und'][0]['value'];
            $line[$ind['required_rcab']]  = $uwwtp->field_uwww_required_rcab['und'][0]['value'];
            $line[$ind['required_rcac']]  = $uwwtp->field_uwww_required_rcac['und'][0]['value'];
            $line[$ind['field_agglo_uww_perc_ent_uw']]  = $field_agglo_uww_perc_ent_uw;
            $line[$ind['compliance5a']] = $uwwtpCompliance5x['5a'];
            $line[$ind['compliance5b']] = $uwwtpCompliance5x['5b'];
            $line[$ind['compliance5c']] = $uwwtpCompliance5x['5c'];
            $line[$ind['treatment_met']] = $uwwtp->field_uwwtreatment_met['und'][0]['value'];
            
            
            
            
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

                //expired deadline   (toutes les NC +  les C qui ont date d   pass   e)
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

                //pending deadline    PD + les C avec date pas d   pass   e
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
                    $context['sandbox'][$typestat]['generated_pe'] += ($line[$ind['generated load  p. e.']] == $GLOBALS['uwwtd']['register']['message']['info_empty']) ? 0 : $line[$ind['generated load  p. e.']];

                    $context['sandbox'][$typestat]['list_agglo_treated'][$line[$ind['aggcode']]] = 1;
                }
                $context['sandbox'][$typestat]['entering_pe'] +=  ($line[$ind['uww_entering load  p. e.']] != $GLOBALS['uwwtd']['register']['message']['info_empty']) ? $line[$ind['uww_entering load  p. e.']] : 0;
                $context['sandbox'][$typestat]['capacity_pe'] +=  ($line[$ind['uww_capacity p. e.']] != $GLOBALS['uwwtd']['register']['message']['info_empty']) ? $line[$ind['uww_capacity p. e.']] : 0;
                $context['sandbox'][$typestat]['previous_entering_pe'] +=  (is_numeric($line[$ind['previous_entering load p. e.']])?$line[$ind['previous_entering load p. e.']]:0);
                $context['sandbox'][$typestat]['previous_capacity_pe'] +=  (is_numeric($line[$ind['previous_capacity p. e.']])?$line[$ind['previous_capacity p. e.']]:0);
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
             $entering_load_pe * 100 / ($context['sandbox']['expired']['generated_pe'] + $context['sandbox']['pending']['generated_pe']);

        $line[$ind['uww_entering load  p. e.']] = $entering_load_pe;
        $line[$ind['uww_capacity p. e.']] = $context['sandbox']['expired']['capacity_pe'] + $context['sandbox']['pending']['capacity_pe'];
        $line[$ind['previous_entering load p. e.']] = $context['sandbox']['expired']['previous_entering_pe'] + $context['sandbox']['pending']['previous_entering_pe'];
        $line[$ind['previous_capacity p. e.']] = $context['sandbox']['expired']['previous_capacity_pe'] + $context['sandbox']['pending']['previous_capacity_pe'];

//         $previous_percentage_of_change_of_entering_load =
//             ($line[$ind['uww_entering load  p. e.']] -  $previous_entering_load_pe)
//             / $previous_entering_load_pe * 100;

        if ($line[$ind['uww_entering load  p. e.']] > 0 && $line[$ind['previous_entering load p. e.']] > 0) {
            $line[$ind['previous_Percentage of change of entering load (1) %']] =
                //(($line[$ind['uww_entering load  p. e.']] /$line[$ind['previous_capacity p. e.']]) - 1 ) * 100;
                ($line[$ind['uww_entering load  p. e.']] - $line[$ind['previous_entering load p. e.']]) / $line[$ind['previous_entering load p. e.']] * 100;
        } else {
            $line[$ind['previous_Percentage of change of entering load (1) %']] = 'NI';
        }

//         if ($line[$ind['previous_Percentage of change of entering load (1) %']] == -100) {
//             $line[$ind['previous_Percentage of change of entering load (1) %']] = '0';
//         }


        if ($line[$ind['previous_capacity p. e.']] > 0 && $line[$ind['uww_capacity p. e.']] > 0) {
    //                     $previous_percentage_of_change_of_capacity = $previous_capacity_pe * 100 / $line[$ind['uww_capacity p. e.']];
            $line[$ind['previous_Percentage of change of capacity (2) %']] = ($line[$ind['uww_capacity p. e.']] - $line[$ind['previous_capacity p. e.']]) / $line[$ind['previous_capacity p. e.']]* 100;
        } else {
            $line[$ind['previous_Percentage of change of capacity (2) %']] = 'NI';
        }

//         $line[$ind['previous_Percentage of change of capacity (2) %']] =
//                     (($line[$ind['uww_capacity p. e.']] / $line[$ind['previous_capacity p. e.']]) - 1 ) * 100;
//         );

//         if ($line[$ind['previous_Percentage of change of capacity (2) %']] == -100) {
//             $line[$ind['previous_Percentage of change of capacity (2) %']] = '0';
//         }

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
function uwwtd_create_register_2015_agglo_without_uwwtp_sheet2($annee, &$context)
{
    if (!isset($context['sandbox']['progress'])) {

        $context['sandbox']['progress'] = 0;
        $context['sandbox']['id'] = array();

        //get all agglomeration without station (= no trace of the agglo nid inn agglo_uwwtp content)
        $query = 'select n.nid
        from {node} n
            left join
                {field_data_field_anneedata} f_annee
                    ON n.nid = f_annee.entity_id
            left join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        where n.type = :typenode
            and f_annee.field_anneedata_value = :annee
            and f_agggenerated.field_agggenerated_value >= 2000
            and not EXISTS (select 1 from {field_data_field_agglo_uww_agglo} where field_agglo_uww_agglo_nid = n.nid)
        ';

// select n.nid
//         from drupal_node n
//             left join
//                 drupal_field_data_field_anneedata f_annee
//                     ON n.nid = f_annee.entity_id
//         where n.type = 'agglomeration'
//             and f_annee.field_anneedata_value = 2011
//             and n.nid not EXISTS (select 1 from drupal_field_data_field_agglo_uww_agglo where field_agglo_uww_agglo_nid = n.nid)

        $param = array(
            ':typenode' => 'agglomeration',
            ':annee' => $annee,
            );

        try {
            $result = db_query($query, $param);
            while($row = $result->fetchAssoc()) {
                $context['sandbox']['id'][] = $row['nid'];
            }
        }catch(Exception $e) {
            //dsm($e->getMessage());
        }

        $context['sandbox']['max'] = count($context['sandbox']['id']);

    }
    $ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet2_uwwtp']);
    

    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['uwwtp']['csv_name'], $annee);
    $r = fopen($filename, 'ab');

    $j = 0;
    while ($j < 20 && $context['sandbox']['progress'] < $context['sandbox']['max']) {

        $agg = node_load($context['sandbox']['id'][$context['sandbox']['progress']]);

        $line = array();

        $line[$ind['aggcode']] = $agg->field_inspireidlocalid['und'][0]['value'];
        //'name',
        $line[$ind['aggname']] = $agg->title;
        //'status',
        $line[$ind['aggstatus']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_status['und'])) {
            switch ($agg->field_status['und'][0]['value']) {
                case (0) :
                    $line[$ind['aggstatus']] = 'inactive';
                    break;
                case (1) :
                    $line[$ind['aggstatus']] = 'active';
                    break;
                case (2) :
                    $line[$ind['aggstatus']] = 'inactive';
                    break;
            }
        }
        //'generated load  p. e.',
        $line[$ind['generated load  p. e.']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_agggenerated['und'][0]['value'])) {
          $line[$ind['generated load  p. e.']] = $agg->field_agggenerated['und'][0]['value'];
        }
        
        //'deadline for Art. 3 compliance',
        $line[$ind['deadline for Art. 3 compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_agggenerated['und'][0]['value'])) {
          $line[$ind['deadline for Art. 3 compliance']] = substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10);
        }
//         $line[$ind['deadline for Art. 3 compliance']] = substr($agg->field_aggperiodover3['und'][0]['value'], 0, 10);

        //'deadline for Art. 4 compliance',
        $line[$ind['deadline for Art. 4 compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggperiodover4['und'][0]['value'])) {
          $line[$ind['deadline for Art. 4 compliance']] = substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10);
        }
//         $line[$ind['deadline for Art. 4 compliance']] = substr($agg->field_aggperiodover4['und'][0]['value'], 0, 10);

        //'deadline for Art. 5 compliance',
        $line[$ind['deadline for Art. 5 compliance']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggperiodover5['und'][0]['value'])) {
          $line[$ind['deadline for Art. 5 compliance']] = substr($agg->field_aggperiodover5['und'][0]['value'], 0, 10);
        }
//         $line[$ind['deadline for Art. 5 compliance']] = substr($agg->field_aggperiodover5['und'][0]['value'], 0, 10);

        //Collection
        //'collected in collecting system % of gen. load',
        $line[$ind['collected in collecting system % of gen. load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggc1['und'][0]['value'])) {
          $line[$ind['collected in collecting system % of gen. load']] = $agg->field_aggc1['und'][0]['value'];
        }
//         $line[$ind['collected in collecting system % of gen. load']] = $agg->field_aggc1['und'][0]['value']; //%

        //'collected in collecting system  p. e.',
        $line[$ind['collected in collecting system  p. e.']] = $agg->field_aggc1['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe

        //'addressed through IAS % of gen. load',
        $line[$ind['addressed through IAS % of gen. load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggc2['und'][0]['value'])) {
          $line[$ind['addressed through IAS % of gen. load']] = $agg->field_aggc2['und'][0]['value']; //%
        }
//         $line[$ind['addressed through IAS % of gen. load']] = $agg->field_aggc2['und'][0]['value']; //%

        //'addressed through IAS p. e.',
        $line[$ind['addressed through IAS p. e.']] = $agg->field_aggc2['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe

        //'neither collecting system nor IAS % of gen. load',
        $line[$ind['neither collecting system nor IAS % of gen. load']] = $GLOBALS['uwwtd']['register']['message']['info_empty'];
        if (isset($agg->field_aggpercwithouttreatment['und'][0]['value'])) {
          $line[$ind['neither collecting system nor IAS % of gen. load']] = $agg->field_aggpercwithouttreatment['und'][0]['value']; //%
        }                                                                                                                               
//         $line[$ind['neither collecting system nor IAS % of gen. load']] = $agg->field_aggpercwithouttreatment['und'][0]['value']; //%

        //'neither collecting system nor IAS p. e.',
        $line[$ind['neither collecting system nor IAS p. e.']] = $agg->field_aggpercwithouttreatment['und'][0]['value'] / 100 * $agg->field_agggenerated['und'][0]['value']; //pe

        fputcsv($r, $line, ';');

        unset($agg);

        $j++;
        $context['sandbox']['progress']++;
    }

    fclose($r);

    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['uwwtp']['xlsx_model_sheet_name'])) . ' (second pass for agglomeration without uwwtp) ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 *
 */
function uwwtd_create_register_2015_rca_sheet1($annee, &$context)
{
    if (!isset($context['sandbox']['progress'])) {
        $context['sandbox']['max'] = 1;
        $context['sandbox']['progress'] = 0;
    }

    $ind = $GLOBALS['uwwtd']['sheet1_rca'];

    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rca']['csv_name'], $annee);

    $r = fopen($filename, 'wb');

    // Create sensitive area register sheet
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
        $ind['rate_of_removal_n-tot'] => 'Rate of Removal N-tot (%)', //; ?
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

    //we must sort on $rca[$i]->title
    usort($rca, "sort_rca_by_title");    

    fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
    fputcsv($r, $header, ';');

    for ($i = 0, $max = count($rca); $i < $max; $i++) {

        if (false === in_array($rca[$i]->field_specialisedzonetype['und'][0]['value'], array('A51523', 'A523', 'SA', 'CSA', 'A54', 'A5854', 'A5154', 'A58', 'A58523', 'A54523', 'A5854523', 'LSA'))) {
            continue;
        }

        // Incoming loads (t/y) N-tot (1) :
        $incomingLoadsNtot = $rca[$i]->field_rcanincomingmeasured['und'][0]['value'] + $rca[$i]->field_rcanincomingcalculated['und'][0]['value'] + $rca[$i]->field_rcanincomingestimated['und'][0]['value'];

        // Discharged loads (t/y) N-tot :
        $dischargedLoadsNtot = $rca[$i]->field_rcandischargedmeasured['und'][0]['value'] + $rca[$i]->field_rcandischargedcalculated['und'][0]['value'] + $rca[$i]->field_rcandischargedestimated['und'][0]['value'];

        // Incoming loads (t/y) P-tot (1) :
        $incomingLoadsPtot = $rca[$i]->field_rcapincomingmeasured['und'][0]['value'] + $rca[$i]->field_rcapincomingcalculated['und'][0]['value'] + $rca[$i]->field_rcapincomingestimated['und'][0]['value'];

        // Discharged loads (t/y) P-tot :
        $dischargedLoadsPtot = $rca[$i]->field_rcapdischargedmeasured['und'][0]['value'] + $rca[$i]->field_rcapdischargedcalculated['und'][0]['value'] + $rca[$i]->field_rcapdischargedestimated['und'][0]['value'];

        $field_rcacodepredecessor = '';
        if (isset($rca[$i]->field_rcacodepredecessor['und'][0]['value'])) {
          foreach ($rca[$i]->field_rcacodepredecessor['und'] as $rcacodepredecessor) {
            $field_rcacodepredecessor .= ($field_rcacodepredecessor == '' ? '' : ', ') . $rcacodepredecessor['value'];         
          }
        }
        
        $field_rcasalsaremark = '';
        if (isset($rca[$i]->field_rcasalsaremark['und'][0]['value'])) {
          foreach ($rca[$i]->field_rcasalsaremark['und'] as $rcasalsaremark) {
            $field_rcasalsaremark .= ($field_rcasalsaremark == '' ? '' : ', ') . $rcasalsaremark['value'];         
          }
        }
        
        $field_rca_sensitive_area = '';   
        //todo test ? $rca[$i]->field_specialisedzonetype['und'][0]['value'] != 'CSA' ? ''  
        if (isset($rca[$i]->field_rca_sensitive_area['und'][0]['value'])) {
          foreach ($rca[$i]->field_rca_sensitive_area['und'] as $rca_sensitive_area) {
            $field_rca_sensitive_area .= ($field_rca_sensitive_area == '' ? '' : ', ') . $rca_sensitive_area['value'];         
          }
        }
        
        $field_rcarelatedsaremark = '';
        if (isset($rca[$i]->field_rcarelatedsaremark['und'][0]['value'])) {
          foreach ($rca[$i]->field_rcarelatedsaremark['und'] as $rcarelatedsaremark) {
            $field_rcarelatedsaremark .= ($field_rcarelatedsaremark == '' ? '' : ', ') . $rcarelatedsaremark['value'];         
          }
        }

        $line = array(
            //'ID of Sensitive Area', //;       rcaCode
            $ind['id_of_sensitive_area'] => $rca[$i]->field_inspireidlocalid['und'][0]['value'],
            //'Name of Sensitive Area', //;     rcaName
            $ind['name_of_sensitive_area'] => $rca[$i]->title,
//             $ind['name_of_sensitive_area'] => htmlspecialchars_decode($rca[$i]->title),
            
            //'Zone Type Code', //;             rcaType
            $ind['zone_type_code'] => $GLOBALS['uwwtd_value']['rcatype_labels'][$rca[$i]->field_zonetype['und'][0]['value']] .' ['.$rca[$i]->field_zonetype['und'][0]['value'].']',
            //'Specialised Zone type Code', //;   ?rcaType
            $ind['specialised_zone_type_code'] => $GLOBALS['uwwtd_value']['rca_spe_type_labels'][$rca[$i]->field_specialisedzonetype['und'][0]['value']].' ['.$rca[$i]->field_specialisedzonetype['und'][0]['value'].']',
            //'Parameters subject to More Stringent Treatment: N', //;      rcaParameterN
            $ind['parameters_subject_to_more_stringent_treatment:_n'] => $rca[$i]->field_rca_parameter_n['und'][0]['value'],
            //'Parameters subject to More Stringent Treatment: P', //;      rcaParameterP
            $ind['parameters_subject_to_more_stringent_treatment:_p'] => $rca[$i]->field_rca_parameter_p['und'][0]['value'],
           // 'Parameters subject to More Stringent Treatment: m', //;      ?
            $ind['parameters_subject_to_more_stringent_treatment:_m'] => $rca[$i]->field_rca_parameter_m['und'][0]['value'],  //add value e.v 2018/10/22
            //'Parameters subject to More Stringent Treatment: other', //;   rcaParameterOther
            $ind['parameters_subject_to_more_stringent_treatment:_other'] => $rca[$i]->field_rca_parameter_other['und'][0]['value'],
            //'Date of designation of Art. 5(8)', //;                  ?
            $ind['date_of_designation_of_art._5(8)'] => substr($rca[$i]->field_rcaart58datedesign['und'][0]['value'], 0, 10),
            //'Starting date of application of Art. 5(8)', //;         rcaDateArt58
            $ind['starting_date_of_application_of_art._5(8)'] => substr($rca[$i]->field_rcadateart5854['und'][0]['value'], 0, 10),
            //'Starting date of application of Art. 5(4)', //;         rcaDateArt54
            $ind['starting_date_of_application_of_art._5(4)'] => substr($rca[$i]->field_rcadateart54['und'][0]['value'], 0, 10),
            //'Designation criteria  a N', //;                         rcaANitro
            $ind['designation_criteria_a_n'] => ($rca[$i]->field_rcaanitro['und'][0]['value'] == 0) ? '' : $rca[$i]->field_rcaanitro['und'][0]['value'],
            //'Starting date of designation a N', //;                  ?
            $ind['starting_date_of_designation_a_n'] => ($rca[$i]->field_rcaanitro['und'][0]['value'] != 0) ? substr($rca[$i]->field_rcaapdatedesignation['und'][0]['value'], 0, 10) : '',
            //'Starting date of application a N', //;                  ?
            $ind['starting_date_of_application_a_n'] => ($rca[$i]->field_rcaanitro['und'][0]['value'] != 0) ? substr($rca[$i]->field_rcaanstartdate['und'][0]['value'], 0, 10) : '', // =rcaDateDesignation + 7 years
            //'Designation criteria  a P', //;                         rcaAPhos
            $ind['designation_criteria_a_p'] => ($rca[$i]->field_rcaaphos['und'][0]['value'] == 0) ? '' : $rca[$i]->field_rcaaphos['und'][0]['value'],
            //'Starting date of designation a P', //;                  ?
            $ind['starting_date_of_designation_a_p'] => ($rca[$i]->field_rcaaphos['und'][0]['value'] != 0) ?  substr($rca[$i]->field_rcaapdatedesignation['und'][0]['value'], 0, 10) : '',
            //'Starting date of application a P', //;                  ?
            $ind['starting_date_of_application_a_p'] => ($rca[$i]->field_rcaaphos['und'][0]['value'] != 0) ? substr($rca[$i]->field_rcaapstartdate['und'][0]['value'], 0, 10) : '', // =rcaDateDesignation + 7 years
            //'Designation criteria  b', //;                           rcaB
            $ind['designation_criteria_b'] => ($rca[$i]->field_rcab['und'][0]['value'] == 0) ? '' : $rca[$i]->field_rcab['und'][0]['value'],
            //'Starting date of designation', //;                      ?
            $ind['starting_date_of_designation_b'] => ($rca[$i]->field_rcab['und'][0]['value'] != 0) ? substr($rca[$i]->field_rcabdatedesignation['und'][0]['value'], 0, 10) : '',
            //'Starting date of application', //;                      ?
            $ind['starting_date_of_application_b'] => ($rca[$i]->field_rcab['und'][0]['value'] != 0) ? substr($rca[$i]->field_rcabstartdate['und'][0]['value'], 0, 10) : '',     // =rcaDateDesignation + 7 years
            //'Designation criteria  c', //;                           rcaC
            $ind['designation_criteria_c'] => ($rca[$i]->field_rcac['und'][0]['value'] == 0) ? '' : $rca[$i]->field_rcac['und'][0]['value'],
            //'Starting date of designation', //;                      ?
            $ind['starting_date_of_designation_c'] => ($rca[$i]->field_rcac['und'][0]['value'] != 0) ?  substr($rca[$i]->field_rcacdatedesignation['und'][0]['value'], 0, 10) : '',
            //'Starting date of application', //;                      ?
            $ind['starting_date_of_application_c'] => ($rca[$i]->field_rcac['und'][0]['value'] != 0) ?  substr($rca[$i]->field_rcacstardate['und'][0]['value'], 0, 10) : '',   // =rcaCDateOtherDirective + 7 years
            //'Designation criteria c - relevant EU-Directives and related parameters', //;            ?
//             $ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'] => 'TODO',    rcaCRelevantDirective        field_rcacrelevantdirective
            $ind['designation_criteria_c_-_relevant_eu-directives_and_related_parameters'] => $rca[$i]->field_rcacrelevantdirective['und'][0]['value'],
            //'Designation criteria c - ID applied to the area according to this Directive', //;       ?
//             $ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'] => 'TODO',  rcaCIDOtherDirective    field_rcacidotherdirective
            $ind['designation_criteria_c_-_id_applied_to_the_area_according_to_this_directive'] => $rca[$i]->field_rcacidotherdirective['und'][0]['value'],
            //'Designation criteria c - reference date of the area according to this Directive', //;   ?
//             $ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'] => 'TODO',  rcaCDateOtherDirective   field_rcacdateotherdirective
            $ind['designation_criteria_c_-_reference_date_of_the_area_according_to_this_directive'] => ('' != $rca[$i]->field_rcacdateotherdirective['und'][0]['value'] ?  substr($rca[$i]->field_rcacdateotherdirective['und'][0]['value'], 0, 10) : ''),
            //'Number of UWWTPs of the article 5.4 area (1)', //;                                      ?rcaPlants
            $ind['number_of_uwwtps_of_the_article_5.4_area_(1)'] => $rca[$i]->field_rcaplants['und'][0]['value'],
            //'Total organic design capacity of the UWWTPs (p.e.) of the article 5.4 area (1)', //;    ?rcaPlantsCapacity
            $ind['total_organic_design_capacity_of_the_uwwtps_(p.e.)_of_the_article_5.4_area_(1)'] => $rca[$i]->field_rcaplantscapacity['und'][0]['value'],

            //'Incoming loads (t/y) N-tot (1)', //;    rcaNIncomingMeasured + rcaNIncomingCalculated + rcaNIncomingEstimated
            $ind['incoming_loads_(t/y)_n-tot_(1)'] => $incomingLoadsNtot,
            //'Rate of Removal N-tot', //;              ?
            //$ind['rate_of_removal_n-tot'] => 'TODO',         =discharged loads Ntot/incoming loads Ntot
            $ind['rate_of_removal_n-tot'] =>  ($incomingLoadsNtot == 0) ? 0 : ($incomingLoadsNtot - $dischargedLoadsNtot) / $incomingLoadsNtot,
            //'Discharged loads (t/y) N-tot (1)', //; rcaNDischargedMeasured + rcaNDischargedCalculated + rcaNDischargedEstimated
            $ind['discharged_loads_(t/y)_n-tot_(1)'] => $dischargedLoadsNtot,

            //'Incoming loads (t/y) P-tot (1)', //;  rcaPIncomingMeasured + rcaPIncomingCalculated + rcaPIncomingEstimated
            $ind['incoming_loads_(t/y)_p-tot_(1)'] => $incomingLoadsPtot,
            //'Rate of Removal P-tot', //;              ?
            $ind['rate_of_removal_p-tot'] => ($incomingLoadsPtot == 0) ? 0 : ($incomingLoadsPtot - $dischargedLoadsPtot) / $incomingLoadsPtot,
            //'Discharged loads (t/y) P-tot: ', //; rcaPDischargedMeasured+ rcaPDischargedCalculated + rcaPDischargedEstimated
            $ind['discharged_loads_(t/y)_p-tot:_'] => $dischargedLoadsPtot,

            //'general Remark', //;                   rcaRemarks
            $ind['general_remark'] => $rca[$i]->field_rcaremarks['und'][0]['value'],
//             $ind['general_remark'] => htmlspecialchars_decode($rca[$i]->field_rcaremarks['und'][0]['value']),
            //

            //'rcaBeginLife', //;                        ?
            $ind['rcabeginlife'] => substr($rca[$i]->field_rcabeginlife['und'][0]['value'], 0, 10),     //new reporting format?
            //'rcaEndLife', //;                          ?
            $ind['rcaendlife'] => substr($rca[$i]->field_rcaendlife['und'][0]['value'], 0, 10),     //new reporting format?
            //'ID code of successor (2)', //;            ?
            $ind['id_code_of_successor_(2)'] => $rca[$i]->field_rcasuccessor['und'][0]['value'],       //new reporting format?
            //'ID code of predecessor (2)', //;          ?
            $ind['id_code_of_predecessor_(2)'] => $field_rcacodepredecessor, //$rca[$i]->field_rcacodepredecessor['und'][0]['value'],   //new reporting format?
            //'remark Successor/Predecessor SA', //;     ?
            $ind['remark_successor/predecessor_sa'] => $field_rcasalsaremark,   //new reporting format?
            //'Related sensitive area (2)', //;          ?
//             $ind['related_sensitive_area_(2)'] => 'TODO',       si CSA, voir rcaSensitiveArea field_rca_sensitive_area
            $ind['related_sensitive_area_(2)'] => $field_rca_sensitive_area, //($rca[$i]->field_specialisedzonetype['und'][0]['value'] != 'CSA' ? '' : $rca[$i]->field_rca_sensitive_area['und'][0]['value']),
            //'Remark Related sensitive area (2)', //;   ?
            $ind['remark_related_sensitive_area_(2)'] => $field_rcarelatedsaremark, //$rca[$i]->field_rcarelatedsaremark['und'][0]['value'],
        );



        fputcsv($r, $line, ';');
    }

    fclose($r);

    $context['sandbox']['progress']++;
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rca']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
}

/**
 *
 */
function uwwtd_create_register_2015_rca54_sheet1_1($annee, &$context) {
    if (!isset($context['sandbox']['progress'])) {
        $context['sandbox']['max'] = 1;
        $context['sandbox']['progress'] = 0;
    }


    $flag =false; //if this var is still false at the end of this function, remove the filename
    $ind = $GLOBALS['uwwtd']['sheet1_rcaA54'];

    $filename = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['rcaA54']['csv_name'], $annee);

    $r = fopen($filename, 'wb');

    // Create sensitive area register sheet
    $header = array(
      'code' => 'Code',
      'name' => 'Name',
      'designation_date' => 'Designation date',
      'deadline_n' => 'deadline N',
      'deadline_p' => 'deadline P',
      'application_art_5_4' => 'Application Art. 5(4)',
      'uwwtp_number' => 'UWWTP number',
      'uwwtp_capacity_total' => 'UWWTP capacity total',
      'n_in' => 'N in',
      'n_out' => 'N out',
      'p_in' => 'Pin',
      'p_out' => 'Pout',
      'n_reduction' => 'N reduction',
      'p_reduction' => 'P reduction',
      'compliant_5_4' => 'compliant 5(4)?',
      'transition_period_pending' => 'transition period pending?',
      'remarks_by_ms' => 'Remarks by MS',
    );

    $rca = uwwtd_get_all_rca($annee);
    //wee must sort on $rca[$i]->field_rcaplantscapacity['und'][0]['value'] if exists
    usort($rca, "sort_rca_by_field_rcaplantscapacity");

    fwrite($r, $GLOBALS['uwwtd']['sheet']['BOM']);
    fputcsv($r, $header, ';');

    for ($i = 0, $max = count($rca); $i < $max; $i++) {
        if (false === in_array($rca[$i]->field_specialisedzonetype['und'][0]['value'], array('A54', 'A5854', 'A5154', 'A54523', 'A5854523'))) {
            continue;
        }

        $nIn = 0;
        $nOut = 0;
        $pIn = 0;
        $pOut = 0;
        $n_reduction = 0;
        $p_reduction = 0;
        $compliant_5_4 = '';

        switch ($rca[$i]->field_rcamethod54['und'][0]['value']) {
          case 'M':
            $nIn = $rca[$i]->field_rcanincomingmeasured['und'][0]['value'];
            $nOut = $rca[$i]->field_rcandischargedmeasured['und'][0]['value'];
            $pIn = $rca[$i]->field_rcapincomingmeasured['und'][0]['value'];
            $pOut = $rca[$i]->field_rcapdischargedmeasured['und'][0]['value'];
            break;
          case 'C':
            $nIn = $rca[$i]->field_rcanincomingcalculated['und'][0]['value'];
            $nOut = $rca[$i]->field_rcandischargedcalculated['und'][0]['value'];
            $pIn = $rca[$i]->field_rcapincomingcalculated['und'][0]['value'];
            $pOut = $rca[$i]->field_rcapdischargedcalculated['und'][0]['value'];
            break;
          case 'E':
            $nIn = $rca[$i]->field_rcanincomingestimated['und'][0]['value'];
            $nOut = $rca[$i]->field_rcandischargedestimated['und'][0]['value'];
            $pIn = $rca[$i]->field_rcapincomingestimated['und'][0]['value'];
            $pOut = $rca[$i]->field_rcapdischargedestimated['und'][0]['value'];
            break;
        }
        //100- (N out * 100 / N In)
        $n_reduction = ($nOut > 0 && $nIn > 0) ? 1 - ($nOut / $nIn) : 0;
        //100- (P out * 100 / P In)
        $p_reduction = ($pOut > 0 && $pIn > 0) ? 1 - ($pOut / $pIn) : 0;

        if ($rca[$i]->field_rcaanitro['und'][0]['value'] == 1 && $rca[$i]->field_rcaaphos['und'][0]['value'] == 1) {
            $compliant_5_4 = $n_reduction >= 0.75 && $p_reduction >= 0.75 ? 'C' : 'NC';
        } elseif ($rca[$i]->field_rcaanitro['und'][0]['value'] == 1) {
           $compliant_5_4 = $n_reduction >= 0.75 ? 'C' : 'NC';
        } elseif ($rca[$i]->field_rcaaphos['und'][0]['value'] == 1) {
           $compliant_5_4 = $p_reduction >= 0.75 ? 'C' : 'NC';
        } else {
           $compliant_5_4 = 'NR';
        }

        $compliant_5_4 = $GLOBALS['glossary'][$compliant_5_4];

        $line = array(              
            $ind['code'] => $rca[$i]->field_inspireidlocalid['und'][0]['value'],
            $ind['name'] => $rca[$i]->title,
            $ind['designation_date'] => substr($rca[$i]->field_rcabeginlife['und'][0]['value'], 0, 10),
            $ind['deadline_n'] => substr($rca[$i]->field_rcaanstartdate['und'][0]['value'], 0, 10),
            $ind['deadline_p'] => substr($rca[$i]->field_rcaapstartdate['und'][0]['value'], 0, 10),
            $ind['application_art_5_4'] => substr($rca[$i]->field_rcadateart54['und'][0]['value'], 0, 10),
            $ind['uwwtp_number'] => isset($rca[$i]->field_rcaPlants['und'][0]['value']) ? $rca[$i]->field_rcaPlants['und'][0]['value'] : '',  
            $ind['uwwtp_capacity_total'] => isset($rca[$i]->field_rcaplantscapacity['und'][0]['value']) ? $rca[$i]->field_rcaplantscapacity['und'][0]['value'] : '',  
            $ind['n_in'] => $nIn,
            $ind['n_out'] => $nOut,
            $ind['p_in'] => $pIn,
            $ind['p_out'] => $pOut,
            $ind['n_reduction'] => $n_reduction,
            $ind['p_reduction'] => $p_reduction,
            $ind['compliant_5_4'] => $compliant_5_4, 
            $ind['transition_period_pending'] => '',
            $ind['remarks_by_ms'] => '',
        );
        fputcsv($r, $line, ';');
        $flag = true;
    }

    fclose($r);
    //Remove the file if the country doesn't have RCA under Art 5.4
    if(!$flag){
        unlink ($filename);
    }
    $context['sandbox']['progress']++;
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['rcaA54']['xlsx_model_sheet_name'])) . ' ' . $context['sandbox']['progress'] . '/' . $context['sandbox']['max'];
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
}


/**
 * build a sheet from agglo csv data
 * 
 */
function uwwtd_create_register_2015_file_agglo_compliance_sheet16($annee, &$context)
{
    //$debug_times=array();
    // dsm(__FUNCTION__);
	// uwwtd_trace(__FUNCTION__);
    $indConf = $GLOBALS['uwwtd']['sheet16_agglo_compliance'];
	//Keep compatibility
    $ind = uwwtd_get_ind($GLOBALS['uwwtd']['sheet16_agglo_compliance']);
    //dsm($ind);
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
        $context['sandbox']['line'] = 5; //start line

        $path_xlsx = uwwtd_getregister_modelfile_path($annee);
        $target_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_compliance']['xlsx_name'], $annee);
        //Remove the old file, if needed
        drupal_unlink($target_path);
    } else {
        $path_xlsx = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet']['agglo_compliance']['xlsx_name'], $annee);
        $target_path = $path_xlsx;
    }

    fseek($ragglofile, $context['sandbox']['file_position']);

    $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);

    $objReader->setLoadSheetsOnly( array(
		$GLOBALS['uwwtd']['sheet']['agglo_compliance']['xlsx_model_sheet_name'],
    ));

    $objPHPExcel = $objReader->load(drupal_realpath($path_xlsx));
    $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet']['agglo_compliance']['xlsx_model_sheet_name']);



	// uwwtd_trace('init xlsx sheet');
//     $start_line = 4;

	
	$j = 0;
    $lines = array();
	//Warning if the csv file contain more than 3000 lines => the system can probably never re-open the excel file
    while ($j < 10000 && $agglo = fgetcsv($ragglofile, 5000, ';')) {
        $line = array();
        
        foreach($indConf as $k=>$conf){
            if($conf['xls']){
                if(isset($agglo[$conf['k']])){
                    $val = $agglo[$conf['k']];
                }else{
                    $val = null;
                }
                //Other condition on field ?
                $line[$conf['k']] = $val;
                // $line[$conf['k']] = uwwtd_register_get_glossary_label($conf, $line[$conf['k']]);
            }

            
        }

        $lines[$j] = $line;
        // uwwtd_trace(var_export($line, true)); 
        $j++;
        $context['sandbox']['file_position'] = ftell($ragglofile);
        $context['sandbox']['progress'] = $context['sandbox']['file_position'];
        //$sheet->insertNewRowBefore($context['sandbox']['line']+$j, 1);
    }
//     $sheet->insertNewRowBefore($context['sandbox']['line'], $j+1);
    $sheet->insertNewRowBefore($context['sandbox']['line'] + 1, $j+1);
    $sheet->fromArray($lines, null, 'A'.$context['sandbox']['line'], true);

    $context['sandbox']['line']=$context['sandbox']['line']+$j;
	
    fclose($ragglofile);
    unset($ragglofile);

    //$debug_times['end_loop2'] = time()-$debug_times['start'];
    //write
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $objWriter->save(drupal_realpath($target_path));
/*    
    $objWriter->__destruct();
    $objReader->__destruct();
*/     
    $objPHPExcel->__destruct();
    unset($objPHPExcel);
    unset($objWriter);
    unset($objReader);
    gc_collect_cycles();

	$context['sandbox']['progress']++;
	
    $context['message'] = '('.$annee.') ' . t('Processing  @name', array('@name' => $GLOBALS['uwwtd']['sheet']['agglo_compliance']['xlsx_model_sheet_name'])) . ' ' . ' ('. $context['sandbox']['progress'] .'/'. $context['sandbox']['max'] .')';

    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}


/**
 *
 */
function uwwtd_get_all_rca($annee)
{
    $query = '
        SELECT n.nid
        FROM {node} n
        LEFT JOIN {field_data_field_anneedata} f_annee
        ON n.nid = f_annee.entity_id
        WHERE n.type = :typenode
        AND f_annee.field_anneedata_value = :annee
    ';

    $param = array(
        ':typenode' => 'receiving_area',
        ':annee' => $annee,
    );

    $data = array();
    try {
        $result = db_query($query, $param);
        while ($row = $result->fetchAssoc()) {
            $data[] = node_load($row['nid']);
        }
    } catch (Exception $e) {
        dsm($e->getMessage());
    }

    return $data;
}  


// function uwwtd_create_register_2015_final_file_testspreadsheet($annee, &$context)
// {
//     if (!isset($context['sandbox']['progress'])) {
//         $context['sandbox']['progress'] = 0;
//         $context['sandbox']['max'] = 1;
//     }
// 
// //     $configsheet = $GLOBALS['uwwtd']['sheet'];
// //     $listesheet = array(
// //                         'glossary',
// //                         'rca',
// //                         'rcaA54',
// //                         'uwwtp',
// //                         'agglo',
// //                         'summary_legal_comp',
// //                         'distance_to_comp',
// //                         'summary_installation_in_place',
// //                         'breach_list',
// //                         'summary_big_cities',
// //                         'nuts_level2',
// //                         'sewage_sludge',
// //                         'graph',
// //                         'graph_2',
// // //                         'graph_test',
// //                         'new_agglo',
// //                         'agglo_out',
// //                         'new_uwwtp',
// //                         'uwwtp_out',
// //                         );
// 
// // use PhpOffice\PhpSpreadsheet\Spreadsheet;
// // use PhpOffice\PhpSpreadsheet\Writer\Xlsx;
// 
// // $spreadsheet = new Spreadsheet();
// // $sheet = $spreadsheet->getActiveSheet();
// // $sheet->setCellValue('A1', 'Hello World !');
// $target_path = uwwtd_getregister_finalfile_path($annee);
// $model = drupal_realpath(uwwtd_getregister_modelfile_path($annee));
//     $reader = IOFactory::createReader('Xlsx');
//     $reader->setIncludeCharts(true);
//     $spreadsheet = $reader->load(drupal_realpath($model));
// 
//     $writer = IOFactory::createWriter($spreadsheet, 'Xlsx');
//     $writer->setIncludeCharts(true);
// 
//     $writer->save(drupal_realpath($target_path));
// 
//     //$GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['final_register'];
// // //     dsm($target_path);
// //     $objWriter->setIncludeCharts(TRUE);
// //     $objWriter->save(drupal_realpath($target_path));
// 
// // $writer->save(drupal_realpath($target_path));
// 
// //     $finalPHPExcel = new PHPExcel();
// //     $sheetIndex = $finalPHPExcel->getIndex($finalPHPExcel->getSheetByName('Worksheet')); //default worksheet
// //     $finalPHPExcel->removeSheetByIndex($sheetIndex);
// // 
// //     foreach($listesheet as $sheet) {
// // //         dsm($sheet);
// //         //load sheet
// // //         $sheet_path = $GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_name'] .'.xlsx';
// //         $sheet_path = uwwtd_getregister_file_path($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_name'], $annee);
// // 
// //         if (!file_exists($sheet_path)) {
// //             continue;
// //         }
// // //         dsm($sheet_path);
// //     //     PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_phpTemp);
// // //         PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_sqlite);
// //         $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
// //         $objReader->setIncludeCharts(TRUE);
// //         $objReader->setLoadSheetsOnly(array($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']));
// //         $objPHPExcel = $objReader->load(drupal_realpath($sheet_path));
// // //         $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']);
// // //         dsm(1);
// //         $finalPHPExcel->addExternalSheet(clone $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']));
// //         
// //         $objPHPExcel->__destruct();
// //         unset($objPHPExcel, $objReader);
// //     }
// // 
// //     //write
// //     $objWriter = PHPExcel_IOFactory::createWriter($finalPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
// //     $objWriter->setPreCalculateFormulas(false);
// //     //$objWriter->setOffice2003Compatibility(true);
// //     $target_path = uwwtd_getregister_finalfile_path($annee);//$GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['final_register'];
// // //     dsm($target_path);
// //     $objWriter->setIncludeCharts(TRUE);
// //     $objWriter->save(drupal_realpath($target_path));
// //     unset($finalPHPExcel);
// //     unset($objWriter);
// 
// 
// // $zip = new ZipArchive;
// // dsm($target_path);
// // if (($er = $zip->open(drupal_realpath($target_path))) === TRUE) {
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/mytheme1.xml', 'xl/theme/theme1.xml');
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/[Content_Types].xml', '[Content_Types].xml');
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/sharedStrings.xml', 'xl/sharedStrings.xml');
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/styles.xml', 'xl/styles.xml');
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/workbook.xml', 'xl/workbook.xml');
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/calcChain.xml', 'xl/calcChain.xml');
// // 
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/_rels/workbook.xml.rels', 'xl/_rels/workbook.xml.rels');
// // 
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/drawings/drawing1.xml', 'xl/drawings/drawing1.xml');
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/drawings/drawing2.xml', 'xl/drawings/drawing2.xml');
// // 
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/drawings/_rels/drawing1.xml.rels', 'xl/drawings/_rels/drawing1.xml.rels');
// //     $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/drawings/_rels/drawing2.xml.rels', 'xl/drawings/_rels/drawing2.xml.rels');
// // 
// //     foreach(range(1,29) as $key) {
// // //        dsm('add style'.$key.'.xml');
// //        $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/charts/style'.$key.'.xml', 'xl/charts/style'.$key.'.xml');
// // //        $zip->addFile('/var/www/html/evincent/uwwtd_at/sites/all/modules/uwwtd/model/model_file/xl/charts/colors'.$key.'.xml', 'xl/charts/colors'.$key.'.xml');
// //     }
// //     $zip->close();
// //     dsm( 'ok');
// // } else {
// //     dsm($er);
// //     dsm(ZipArchive::ER_EXISTS);
// //     dsm(ZipArchive::ER_INCONS);
// //     dsm(ZipArchive::ER_INVAL);
// //     dsm(ZipArchive::ER_MEMORY);
// //     dsm(ZipArchive::ER_NOENT);
// //     dsm(ZipArchive::ER_NOZIP);
// //     dsm(ZipArchive::ER_OPEN);
// //     dsm(ZipArchive::ER_READ);
// //     dsm(ZipArchive::ER_SEEK);
// //     dsm('err');
// // }
// 
// 
//     $context['sandbox']['progress']++;
// 
//     $context['message'] = '('.$annee.') ' . t('Finalization');
// 
// //     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
//         $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
// //     }
// 
// 
//     variable_set('registre_'.$annee, time());
//     drupal_set_message(uwwtd_registre_create_html_link($annee));
// }



function uwwtd_create_register_2015_final_file($annee, &$context){
    if (!isset($context['sandbox']['progress'])) {
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['max'] = 1;
    }

    $configsheet = $GLOBALS['uwwtd']['sheet'];
    $listesheet = array(
                        'glossary',
                        'rca',
                        'rcaA54', //don't add this sheet if country is not subjected to Art 5.4
                        'rca_history', //don't add this sheet if country is not subjected to Art 5.4
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
                        'graph_2',
//                         'graph_test',
                        'new_agglo',
                        'agglo_out',
                        'new_uwwtp',
                        'uwwtp_out',
                        'agglo_compliance',
                        // 'agglo_debug',
                        );
    if($annee >=2018){
        $listesheet = array_flip($listesheet);
        unset($listesheet['agglo_compliance']);
        unset($listesheet['graph_2']);
        $listesheet = array_flip($listesheet);
    }
    if($annee <= 2018){
        $listesheet = array_flip($listesheet);
        unset($listesheet['rca_history']);
        $listesheet = array_flip($listesheet);
    }
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
        $objReader = PHPExcel_IOFactory::createReader($GLOBALS['uwwtd']['sheet']['model_type']);
        $objReader->setIncludeCharts(TRUE);
        $objReader->setLoadSheetsOnly(array($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']));
        $objPHPExcel = $objReader->load(drupal_realpath($sheet_path));
//         $sheet = $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']);
         
         if($objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name'])){
            $finalPHPExcel->addExternalSheet(clone $objPHPExcel->getSheetByName($GLOBALS['uwwtd']['sheet'][$sheet]['xlsx_model_sheet_name']));
         }
        $objPHPExcel->__destruct();
        unset($objPHPExcel, $objReader);
    }

    //write
    $objWriter = PHPExcel_IOFactory::createWriter($finalPHPExcel, $GLOBALS['uwwtd']['sheet']['target_type']);
    $objWriter->setPreCalculateFormulas(false);
    //$objWriter->setOffice2003Compatibility(true);
    $target_path = uwwtd_getregister_finalfile_path($annee);//$GLOBALS['uwwtd']['sheet']['path_created_file'] . $GLOBALS['uwwtd']['sheet']['final_register'];
//     dsm($target_path);
    $objWriter->setIncludeCharts(TRUE);
    $objWriter->save(drupal_realpath($target_path));
    unset($finalPHPExcel);
    unset($objWriter);

if($annee <= 2016){
    $zip = new ZipArchive;
    if (($er = $zip->open(drupal_realpath($target_path))) === TRUE) {
        //we modify the xml file for each graph we need to change color (use Excel 2007 to modify the excel file and use the xml in the xlsx generated)
        //sheet Graphs
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart2.xml', 'xl/charts/chart2.xml');    //Graph: Load in p.e. per agglomeration size class
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart4.xml', 'xl/charts/chart4.xml');    //GRAph:Sewage sludge in t DS/y
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart5.xml', 'xl/charts/chart5.xml');    //GRAph:Comparison of compliance between 8th, 9th and 10th reporting: Load in p.e
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart6.xml', 'xl/charts/chart6.xml');    //GRAph:Comparison of compliance between 8th, 9th and 10th reporting: number of agglomerations
        //sheet National report Graphs 
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart23.xml', 'xl/charts/chart23.xml');
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart24.xml', 'xl/charts/chart24.xml');
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart25.xml', 'xl/charts/chart25.xml');
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart26.xml', 'xl/charts/chart26.xml');
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart27.xml', 'xl/charts/chart27.xml');
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart28.xml', 'xl/charts/chart28.xml');
//         $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart29.xml', 'xl/charts/chart29.xml');


           /////////////////////////////////////////////////////////////////////
           //sheet Graphs
           //chart 6        Number of agglomeration per agglomeration size class
           //thousand separator
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart6.xml', 'xl/charts/chart6.xml');
//            dsm($r);

           // chart4     Loadin p.e. per agglomeration size class
           //thousand separator , divide by 1000 bar label, 
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart4.xml', 'xl/charts/chart4.xml');
//            dsm($r);

           // chart3     Sewage sludge in t DS/y
           //thousand separator , ihde axe X title
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart3.xml', 'xl/charts/chart3.xml');
//            dsm($r);

           //chart 21 : Comparison of compliance between 8th, 9th and 10th reporting: Load in p.e.
           //thousand separator  + color           
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart21.xml', 'xl/charts/chart21.xml');
//            dsm($r);

           //chart 1 : Comparison of compliance between 8th, 9th and 10th reporting: number of agglomerations
           //thousand separator  + color           
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart1.xml', 'xl/charts/chart1.xml');
//            dsm($r);



           /////////////////////////////////////////////////////////////////////
           //sheet National report Graphs 

           //chart 29 Number of agglomeration per agglomeration size class
           //change label bar to percent and color bar , separator vertical axes for thousand valuz
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart29.xml', 'xl/charts/chart29.xml');
//            dsm($r);

           //chart 27 Loadin p.e. per agglomeration size class
           //change label bar to percent and color bar , separator vertical axes for thousand valuz
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart27.xml', 'xl/charts/chart27.xml');
//            dsm($r);
                  
           //chart 22 Size of agglomerations 
           //change label bar to percent and color bar , separator vertical axes for thousand valuz
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart22.xml', 'xl/charts/chart22.xml');
//            dsm($r);

           //chart 30 Load per agglomeration size   
           //change label bar to percent and color bar , separator vertical axes for thousand valuz
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart30.xml', 'xl/charts/chart30.xml');
//            dsm($r);

           //chart 28 Comparison of compliance between 8th, 9th and 10th reporting: Load in p.e.
           //change color and , separator vertical axes for thousand valuz 
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart28.xml', 'xl/charts/chart28.xml');
//            dsm($r);

           //chart 25 Comparison of compliance between 8th, 9th and 10th reporting: number of agglomerations
           //change color and , separator vertical axes for thousand valuz 
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart25.xml', 'xl/charts/chart25.xml');
//            dsm($r);

           //chart 26 Comparison of installation in place for previous year (light colour) and current year (dark colour) loadin p.e.
           //change color and , separator vertical axes for thousand valuz 
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart26.xml', 'xl/charts/chart26.xml');
//            dsm($r);

           //chart 23 Production of sludge
           //change color and , separator vertical axes for thousand valuz 
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart23.xml', 'xl/charts/chart23.xml');
//            dsm($r);

           //chart 24 : Comparison of sewage sludge re-use and disposal routes between previous year and reference year
           $r = $zip->addFile(DRUPAL_ROOT . '/sites/all/modules/uwwtd/model/model_file/chart24.xml', 'xl/charts/chart24.xml');
//            dsm($r);


        $zip->close();
    } else {
        // uwwtd_trace(var_export($er, true));
    }
}

    $context['sandbox']['progress']++;

    $context['message'] = '('.$annee.') ' . t('Finalization');

//     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
//     }


    variable_set('registre_'.$annee, time());
    drupal_set_message(uwwtd_registre_create_html_link($annee));
}

//Get data on distance to target (distance to compliance) foreach agglomeration and set it in agglomeration nodes
function uwwtd_retreive_agg_dtt($annee, &$context){
    
        if (!isset($context['sandbox'][$annee]['progress'])) {

            $context['sandbox'][$annee]['progress'] = 0;
            $context['sandbox'][$annee]['dtt_data'] = [];

           $uri = 'public://register/'.$annee.'_agglo.csv';
           $filepath = drupal_realpath($uri);
           //=========Get DTT data
            if (($handle = fopen($filepath, "r")) !== FALSE) {
                $headers = [];
                /*
                'deadline for Art. 3 compliance'=>3, 
                'deadline for Art. 4 compliance'=>4, 
                'deadline for Art. 5 compliance'=>5,
                //'dtt_art3_pc'=>24,
                    //'dtt_art4_treatment_pc'=>41,
                    // 'dtt_art4_perf_pc'=>43,
                    // 'dtt_art5_treatment_pc'=>75,
                    //'dtt_art5_perf_pc'=>77,
                */
                
                //List relevant keys
                $keys = [
                    'code'=>0,
                    'generated_load'=>2,
                    'dwt_pc'=>22,
                    'dwt_pe'=>23,
                    'dtt_art3_pe'=>25,
                    'dtt_art4_treatment_pe'=>42,
                    'dtt_art4_perf_pe'=>44,
                    'dtt_art5_treatment_pe'=>76,
                    'dtt_art5_perf_pe'=>78,
                    'comp_art5'=>72
                ];
                while (($row = fgetcsv($handle, 0, ";")) !== FALSE) {
                    if(empty($headers)) {$headers = $row;}
                    else{
                        $line = [];
                        foreach($keys as $k=>$idx){
                            $line[$k] = $row[$idx];
                        }
                        $context['sandbox'][$annee]['dtt_data'][] = $line;
                    } 
                }
                fclose($handle);
            }

        $context['sandbox'][$annee]['max'] = count($context['sandbox'][$annee]['dtt_data']);

    }
    
    $j = 0;
    while ($j < 20 && $context['sandbox'][$annee]['progress'] < $context['sandbox'][$annee]['max']) {
        //Save data in agglomeration nodes
        if(!empty(isset($context['sandbox'][$annee]['dtt_data'][$context['sandbox'][$annee]['progress']]))){
            $info = $context['sandbox'][$annee]['dtt_data'][$context['sandbox'][$annee]['progress']];
            $siteId = $siteId = $annee.'_agg_'.$info['code'];
            if($nid = uwwtd_check_exist($siteId)){
                if($node=node_load($nid)){
                    $dtt_art3_pe = $info['dtt_art3_pe'];
                    
                    
                    $dtt_art_4_t = $info['dtt_art4_treatment_pe'];
                    $dtt_art_4_p = $info['dtt_art4_perf_pe'];
                    $dtt_art_5_t = $info['dtt_art5_treatment_pe'];
                    $dtt_art_5_p = $info['dtt_art5_perf_pe'];
                    
                    //FIX ME : coment the followings lines if Art 3 DTT is reported before
                    if($info['dwt_pc'] > 2 ||  $info['dwt_pe'] > 2000){
                        $dtt_art_4_t +=$dtt_art3_pe;
                        $dtt_art_4_p +=$dtt_art3_pe;
                        //If agglo is subjected to Art 5
                        if($info['comp_art5']!='NR' ){
                            $dtt_art_5_t +=$dtt_art3_pe;
                            $dtt_art_5_p +=$dtt_art3_pe;
                        }
                    }
                    $node->field_dtt_art3['und'][0]['value'] = round($dtt_art3_pe);
                    $node->field_dtt_art_4_treatment['und'][0]['value'] = round($dtt_art_4_t);
                    $node->field_dtt_art_4_performance['und'][0]['value'] = round($dtt_art_4_p);
                    $node->field_dtt_art_5_treatment['und'][0]['value'] = round($dtt_art_5_t);
                    $node->field_dtt_art_5_performance['und'][0]['value'] = round($dtt_art_5_p);
                    node_save($node);
                }
            }
        }
        $j++;
        $context['sandbox'][$annee]['progress']++;
    }
    $context['message'] = '('.$annee.')  Fill Distance To Target fields in Agglomeration nodes  ' . $context['sandbox'][$annee]['progress'] . '/' . $context['sandbox'][$annee]['max'];

    if ($context['sandbox'][$annee]['progress'] < $context['sandbox'][$annee]['max']) {
        $context['finished'] = $context['sandbox'][$annee]['progress'] / $context['sandbox'][$annee]['max'];
    }
    
}

/**
 *
 */
function uwwtd_create_report($annee, &$context)
{
  $text = "
  1	[#COUNTRY#]
  1.1	General information on data
  
  For reference date 31/12/[#CURRENT_REPORT_YEAR#], [#COUNTRY#] reported [#NB_AGGLO#] agglomerations  �� 2 000 population equivalent (p.e.) with a generated load of [#TOTAL_GENERATED_LOAD#] p.e.. 
  Figures 1 and 2 show a comparison of the number of agglomerations and the amount of generated waste water load from the last UWWTD reporting exercise in [#PREVIOUS_YEAR_REPORT#] ([#PREVIOUS_REPORT_NUMBER#] Report) and [#CURRENT_YEAR_REPORT#] ([#CURRENT_REPORT_NUMBER#] Report). 
  Between [#PREVIOUS_YEAR_REPORT#] and [#CURRENT_YEAR_REPORT#] the waste water load increased by [#WASTER_WATER_LOAD_INCREASED#] p.e., the share of waste water load has increased by ?% in the ?  �� ? size class, decreased by ? % in the ?-? size class and is stable in the >? size class.
  
  1.2	Receiving areas  �� application of Article 5(8) and Article 5(4)
  
  1.3	Results of the legal compliance check
  
  This section presents the results of the legal compliance check.
  
  [All [#NB_AGGLO#] agglomerations comply with requirements of Article 3 (collection) of the Directive.] 
  The compliance rate for Article 3 (collection) of the Directive is [#COMPLIANCE_ART3#]%.
  The compliance rate for Article 4 is [#COMPLIANCE_ART4#]% of the waste water load (secondary treatment), and the compliance rate for Article 5 is [#COMPLIANCE_ART5#]% of the waste water load (more stringent treatment).
  
  1.4	Distance to compliance
  
  This section provides an evaluation of the waste water load in p.e. that is responsible for non-compliance. This analysis can give an accurate idea of the efforts needed to meet compliance.
  
  [#COUNTRY#] has a [#ART3_DISTANCE_TO_COMPLIANCE#] distance to compliance for Article 3, but the distance to compliance is [#ART4_DISTANCE_TO_COMPLIANCE#]% for Article 4 and  [#ART5_DISTANCE_TO_COMPLIANCE#]% for Article 5.
      
  1.5	Results of the legal compliance check at the regional level
  
  1.6	Waste water installations in place
  
  This section assesses the information concerning the reported waste water installations in place.
  
  [#COUNTRY#] has reported [#NB_AGGLO#] agglomerations  �� 2,000 p.e. with a total generated load of [#TOTAL_GENERATED_LOAD#] p.e.. 
  The share of the total generated load of all agglomerations  �� 2,000 p.e. that is reported to be collected in a collecting system is [#TOTAL_COLLECTING_SYSTEM_IN_PLACE#]%. 
  There are [#SECONDARY_INSTALLATION_IN_PLACE#] agglomerations that have installations for secondary treatment in place, of these there are [#TERTIARY_INSTALLATION_IN_PLACE#] agglomerations reported to have more stringent treatment installations.
  
  
  1.7	Waste water treatment in  ��big cities ��
  
  1.8	Production of sludge, sludge re-use and disposal
  
  1.9	National Implementation Programme
  
  1.10	Storm water overflows
  
  1.11	Waste Water reuse
  ";

//   dsm($text);
//   dsm($context['results']['auto_report']);
  $text = str_replace(
                 array(
                   '[#COUNTRY#]',
                   '[#CURRENT_REPORT_YEAR#]', 
                   '[#NB_AGGLO#]',
                   '[#TOTAL_GENERATED_LOAD#]',
                   '[#PREVIOUS_YEAR_REPORT#]',
                   '[#PREVIOUS_REPORT_NUMBER#]',
                   '[#CURRENT_YEAR_REPORT#]',
                   '[#CURRENT_REPORT_NUMBER#]',
                   '[#PREVIOUS_YEAR_REPORT#]',
                   '[#CURRENT_YEAR_REPORT#]',
                   '[#WASTER_WATER_LOAD_INCREASED#]',
                   '[#COMPLIANCE_ART3_NUMBER#]',
                   '[#COMPLIANCE_ART3#]',
                   '[#COMPLIANCE_ART4#]',
                   '[#COMPLIANCE_ART5#]', 
                   '[#ART3_DISTANCE_TO_COMPLIANCE#]',
                   '[#ART4_DISTANCE_TO_COMPLIANCE#]',
                   '[#ART5_DISTANCE_TO_COMPLIANCE#]',
                   '[#TOTAL_COLLECTING_SYSTEM_IN_PLACE#]',
                   '[#SECONDARY_INSTALLATION_IN_PLACE#]',
                   '[#TERTIARY_INSTALLATION_IN_PLACE#]',
                 ),
                 array(
                     $context['results']['auto_report']['COUNTRY'],
                     $context['results']['auto_report']['CURRENT_REPORT_YEAR'], 
                     $context['results']['auto_report']['NB_AGGLO'],
                     number_format($context['results']['auto_report']['TOTAL_GENERATED_LOAD'], 0),
                     $context['results']['auto_report']['PREVIOUS_REPORT_YEAR'],
                     $context['results']['auto_report']['PREVIOUS_REPORT_NUMBER'],
                     $context['results']['auto_report']['CURRENT_REPORT_YEAR'],
                     $context['results']['auto_report']['CURRENT_REPORT_NUMBER'],
                     $context['results']['auto_report']['PREVIOUS_REPORT_YEAR'],
                     $context['results']['auto_report']['CURRENT_REPORT_YEAR'],
                     number_format($context['results']['auto_report']['WASTER_WATER_LOAD_INCREASED'], 0),
                     number_format($context['results']['auto_report']['COMPLIANCE_ART3_NUMBER'], 0),
                     number_format(round($context['results']['auto_report']['COMPLIANCE_ART3'], 2), 1),
                     number_format(round($context['results']['auto_report']['COMPLIANCE_ART4'], 2), 1),
                     number_format(round($context['results']['auto_report']['COMPLIANCE_ART5'], 2), 1),
                     $context['results']['auto_report']['ART3_DISTANCE_TO_COMPLIANCE'],
                     $context['results']['auto_report']['ART4_DISTANCE_TO_COMPLIANCE'],
                     $context['results']['auto_report']['ART5_DISTANCE_TO_COMPLIANCE'],
                     number_format(round($context['results']['auto_report']['TOTAL_COLLECTING_SYSTEM_IN_PLACE'], 2), 1),
                     $context['results']['auto_report']['SECONDARY_INSTALLATION_IN_PLACE'],
                     $context['results']['auto_report']['TERTIARY_INSTALLATION_IN_PLACE'],
                 ),
                 $text
              );

  $path = 'public://register/' . $annee . '_report.txt';
  $url = file_create_url($path);              
  file_put_contents($path, $text);
//   dsm($text);
//   dsm($context['results']['auto_report']);
//   drupal_set_message('<a href="'.$url.'?d='.date("YmdHis").'">Rapport</a>');
}


function uwwtd_getregister_file_path($filename, $annee)
{
    return $GLOBALS['uwwtd']['sheet']['path_created_file'] . $annee . $filename;
}

/**
 * From 2016 reporting, the model xlsx has some changed 
 */
function uwwtd_getregister_modelfile_path($annee)
{
//     uwwtd_trace($annee);
    if ($annee >= 2018) {
      return $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model_reporting_2018'];
    }elseif ($annee = 2016) {
//       uwwtd_trace($GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model_reporting_2016']);
//       return $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet'][];
      return $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model_reporting_2016'];
    } else {
//       uwwtd_trace($GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model_reporting_2014']);
//       return $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model'];
      return $GLOBALS['uwwtd']['sheet']['path_model_file'] . $GLOBALS['uwwtd']['sheet']['model_reporting_2014'];
    }
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
        case 'MP':
            $treatment_required = '3P';
            break;
        case 'NPO':
            $treatment_required = '3NPother';
            break;
        case 'NO':
            $treatment_required = '3Nother';
            break;
        case 'PO':
            $treatment_required = '3Pother';
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

function uwwtd_get_register_treatment_required_with_deadline($required_calculated, $deadlineart3, $deadlineart4, $deadlineart5, $referenceyear) {
    $deadline_over_art3 = deadline_beforeorequal_to_referenceyear($deadlineart3, $referenceyear);
    $deadline_over_art4 = deadline_beforeorequal_to_referenceyear($deadlineart4, $referenceyear);
    $deadline_over_art5 = deadline_beforeorequal_to_referenceyear($deadlineart5, $referenceyear);
    $treatment_required = '';

    switch($required_calculated) {
        case 'S':
            $treatment_required = '2';
            break;
        case 'N':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3N';
            }
            break;
        case 'NP':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3NP';
            }
            break;
        case 'P':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3P';
            }
            break;
        case 'MP':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3P';
            }
            break;
        case 'NPO':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3NPother';
            }
            break;
        case 'NO':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3Nother';
            }
            break;
        case 'PO':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3Pother';
            }
            break;
        case 'O':
            if ($deadlineart5 == false) {
                $treatment_required = '2';
            } else {
                $treatment_required = '3other';
            }
            break;
        case 'NI':
            $treatment_required = '0';
            break;
        default:
            $treatment_required = '';
    }

    if ($deadlineart3 === true && $deadlineart4 === true && $deadlineart5 === true) {
        $treatment_required = 'NR';
    }

    if ($required_calculated == 'P' ) {
        $treatment_required = '1';
    }

    if ($required_calculated == 'Appropriate') {
        $treatment_required = 'Appropriate';
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
    if ($uwwtp->field_uwwsandfiltration['und'][0]['value'] == '1' ||
        $uwwtp->field_uwwothertreat['und'][0]['value'] == '1' ||
        $uwwtp->field_uwwozonation['und'][0]['value'] == '1' ||
        $uwwtp->field_uwwuv['und'][0]['value'] == '1' ||
        $uwwtp->field_uwwchlorination['und'][0]['value'] == '1' ||
        $uwwtp->field_uwwmicrofiltration['und'][0]['value'] == '1') {
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

    $deadline_before = (deadline_beforeorequal_to_referenceyear($deadlineart3, $annee)
         || deadline_beforeorequal_to_referenceyear($deadlineart4, $annee)
         || deadline_beforeorequal_to_referenceyear($deadlineart5, $annee)
        );
	//cn@oieau 14-02-2017 add the condition  """$uwwtp->field_uwwcompliance['und'][0]['value'] == 'PD'""" in order to force the value of the result
    if (
        true === $deadline_before
         && ($uwwtp->field_uwwcompliance['und'][0]['value'] == 'NC' || $uwwtp->field_uwwcompliance['und'][0]['value'] == 'C' ||$uwwtp->field_uwwcompliance['und'][0]['value'] == 'PD')
        ) {
        $pending_or_expired = 'EXP';
    }

    //pending deadline    PD + les C avec date pas d   pass   e
    // 2016-03-03 - BB - Added NC for stations connected to multiple agg, where deadlines might be different
    if (
        false === $deadline_before
        && ($uwwtp->field_uwwcompliance['und'][0]['value'] == 'PD' || $uwwtp->field_uwwcompliance['und'][0]['value'] == 'C' || $uwwtp->field_uwwcompliance['und'][0]['value'] == 'NC')
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
        case '3N':
        case '3P':
        case '3NP':
        case '3other':
		case '3Pother':
		case '3Nother':
		case '3NPother':
            if (false !== strpos($treatmenttypeinplace, '2')) {
                $art4_treatment_type = 'C';
            } else {
                $art4_treatment_type = 'NC';
            }
            break;
        // case '3N':
        //     if (false !== strpos($treatmenttypeinplace, '3N')) {
        //         $art4_treatment_type = 'C';
        //     } else {
        //         $art4_treatment_type = 'NC';
        //     }
        //     break;
        // case '3P':
        //     if (false !== strpos($treatmenttypeinplace, '3P')) {
        //         $art4_treatment_type = 'C';
        //     } else {
        //         $art4_treatment_type = 'NC';
        //     }
        //     break;
        // case '3NP':
        //     if (false !== strpos($treatmenttypeinplace, '3NP')) {
        //         $art4_treatment_type = 'C';
        //     } else {
        //         $art4_treatment_type = 'NC';
        //     }
        //     break;
        // case '3other':
        //     if (false !== strpos($treatmenttypeinplace, '3other')) {
        //         $art4_treatment_type = 'C';
        //     } else {
        //         $art4_treatment_type = 'NC';
        //     }
        //     break;
        case 'Appropriate':
        case 'NR':
		case '1':
            $art4_treatment_type = 'NR';
			break;
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
		case '3Pother':
		case '3Nother':
		case '3NPother':
            if ($bod5monitoring ==  'Pass' && $codmonitoring == 'Pass') {
                $art4_monitoring = 'C';
            } else {
                $art4_monitoring = 'NC';
            }
            break;
        case 'Appropriate':
		case '1':
        case 'NR':
            $art4_monitoring = 'NR';
            break;
    }
//             }
    return $art4_monitoring;
}

function uwwtd_get_register_art5_treatement_type_compliance($treatment_required, $treatmenttypeinplace)
{
    // dsm($treatment_required);
    // dsm($treatmenttypeinplace);
    //if treatment required is 3N, NP P Other (if not NR), check if treatment in place is the same 3N, NP, P Other if yes=C if no NC
    $art5_treatment = '';
    switch($treatment_required) {
        case '3N':
            if ((false !== strpos($treatmenttypeinplace, '3N') || false !== strpos($treatmenttypeinplace, '3NP')) &&
				false !== strpos($treatmenttypeinplace, '2')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }
            break;
        case '3P':
            if ((false !== strpos($treatmenttypeinplace, '3P') || false !== strpos($treatmenttypeinplace, '3NP')) &&
				false !== strpos($treatmenttypeinplace, '2')){
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }
            break;
        case '3NP':
            if (false !== strpos($treatmenttypeinplace, '3NP') &&
				false !== strpos($treatmenttypeinplace, '2')){
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }
            break;
        case '3other':
            if (false !== strpos($treatmenttypeinplace, '3other')&&
				false !== strpos($treatmenttypeinplace, '2')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }
            break;
        case '3NPother':
            if (false !== strpos($treatmenttypeinplace, '3other') && false !== strpos($treatmenttypeinplace, '3NP') && false !== strpos($treatmenttypeinplace, '2')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }
            break;
        case '3Nother':
            if (false !== strpos($treatmenttypeinplace, '3other') && false !== strpos($treatmenttypeinplace, '3N') && false !== strpos($treatmenttypeinplace, '2')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }
            break;
        case '3Pother':
            if (false !== strpos($treatmenttypeinplace, '3other') && (false !== strpos($treatmenttypeinplace, '3P') || false !== strpos($treatmenttypeinplace, '3NP')) && false !== strpos($treatmenttypeinplace, '2')) {
                $art5_treatment = 'C';
            } else {
                $art5_treatment = 'NC';
            }
            break;
        case 'Appropriate':
            $art5_treatment = 'NR';
			break;
        case '1':
			$art5_treatment = 'NR';
			break;
        case '2':
			$art5_treatment = 'NR';
            break;
        case 'NR':
            $art5_treatment = 'NR';
            break;
    }
//             }
    return $art5_treatment;
}
//cn@oieau modified the 31/01/2017 in order to take into account COD and BOD performance and skip the performance for "other" treatment
function uwwtd_get_register_art5_monitoring_compliance(
    $treatment_required,
	$monitoring_COD,
	$monitoring_BOD,
    $monitoring_Ntot,
    $monitoring_Ptot,
    $monitoring_other,
    $required_rcaB,
    $required_rcaC,
    $required_rcaaP,
    $required_rcaaN)
{
    //if treatment required is 3N, NP P Other (if not NR)then if 3n 'NtotPerf=pass = C if not =NC, if 3np NtotPerf=pass+PtotPerf=pass = C if not = NC, if 3p PtotPerf=pass = C if not = NC
    $art5_monitoring = '';
    // dsm($treatment_required);
    // dsm($required_rcaB);
    // dsm($required_rcaC);
    switch($treatment_required) {
        case '3N':
            if (($monitoring_Ntot == 'Pass' || $monitoring_Ntot == 'P') &&
				($monitoring_COD == 'Pass' || $monitoring_COD == 'P') &&
				($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P')) {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }
            if (is_null($monitoring_Ntot)) {
                $art5_monitoring = 'NC';
            }
            break;
        case '3P':
            if (($monitoring_Ptot == 'Pass' || $monitoring_Ptot == 'P')  &&
				($monitoring_COD == 'Pass' || $monitoring_COD == 'P') &&
				($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P')) {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }
            if (is_null($monitoring_Ptot)) {
                $art5_monitoring = 'NC';
            }
            break;
        case '3NP':
            if (($monitoring_Ntot == 'Pass' || $monitoring_Ntot == 'P') && 
				($monitoring_Ptot == 'Pass' || $monitoring_Ptot == 'P')  &&
				($monitoring_COD == 'Pass' || $monitoring_COD == 'P') &&
				($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P')) {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }
            if (is_null($monitoring_Ntot) || is_null($monitoring_Ptot)) {
                $art5_monitoring = 'NC';
            }
            break;
        case '3other':
            if(/*($monitoring_other == 'Pass' || $monitoring_other == 'P')  &&*/
				($monitoring_COD == 'Pass' || $monitoring_COD == 'P') &&
				($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P')) {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }
            if (is_null($monitoring_other)) {
                $art5_monitoring = 'NC';
            }
            break;
        case '3NPother':
            if (($monitoring_Ntot == 'Pass' || $monitoring_Ntot == 'P') && 
				//($monitoring_other == 'Pass' || $monitoring_other == 'P') &&
				($monitoring_Ptot == 'Pass' || $monitoring_Ptot == 'P' )  &&
				($monitoring_COD == 'Pass' || $monitoring_COD == 'P') &&
				($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P')) {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }
            if (is_null($monitoring_Ntot) || is_null($monitoring_Ptot) || is_null($monitoring_other)) {
                $art5_monitoring = 'NC';
            }
            break;
        case '3Nother':
            if (($monitoring_Ntot == 'Pass' || $monitoring_Ntot == 'P') &&
				//($monitoring_other == 'Pass' || $monitoring_other == 'P')  &&
				($monitoring_COD == 'Pass' || $monitoring_COD == 'P') &&
				($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P')) {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }
            if (is_null($monitoring_Ntot) || is_null($monitoring_other)) {
                $art5_monitoring = 'NC';
            }
            break;
        case '3Pother':
            if (/*($monitoring_other == 'Pass' || $monitoring_other == 'P') &&*/
				($monitoring_Ptot == 'Pass' || $monitoring_Ptot == 'P')  &&
				($monitoring_COD == 'Pass' || $monitoring_COD == 'P') &&
				($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P')) {
                $art5_monitoring = 'C';
            } else {
                $art5_monitoring = 'NC';
            }
            if (is_null($monitoring_Ptot) || is_null($monitoring_other)) {
                $art5_monitoring = 'NC';
            }
            break;
        case 'Appropriate':
			$art5_monitoring = 'NR';
			break;
        case '1':
			$art5_monitoring = 'NR';
			break;
        case '2':
			$art5_monitoring = 'NR';
			break;
        case 'NR':
            $art5_monitoring = 'NR';
            break;
    }

    if (($required_rcaB == '1' || $required_rcaC == '1') && ($required_rcaaN != '1' || $required_rcaaP != '1') && ($monitoring_COD == 'Pass' || $monitoring_COD == 'P') && ($monitoring_BOD == 'Pass' || $monitoring_BOD == 'P') ) {
        $art5_monitoring = 'C';
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
    return '<a href="'.file_create_url(uwwtd_getregister_finalfile_path($annee)).'?v='.time().'">'. t('Get the register for year @year', array('@year' => $annee)) .'</a>';
}

//'Compliance Art. 5a (hierarchical approach)',
// =IF(  �  Compliance Art.5  � �="NR"
//       �  Compliance Art.5  � �;
//     IF($C4<=10000;  //$C4 = agglo->agggenerated
//        "NR";
//        IF(AND(  �  Compliance Art.4  � �="C";
//                 �  COMPLIANCE ART.5  �  ="C");
//             "C";
//             IF(OR(  �  Compliance Art.4  � �="NC";
//                     �  Compliance Art.5  � �="NC");
//                 "NC";
//                 IF(  �  Compliance Art.5  � �="NR";
//                    "NR";
//                    "EXIT")))))
function uwwtd_register_getcompliance_art5x_hierarchical($type, $ind, $line)
{
    switch($type) {
        case 'a':
            $compart5x = $line[$ind['compliance_art._5a_(only)']];
            break;
        case 'b':
            $compart5x = $line[$ind['compliance_art._5b_(only)']];
            break;
        case 'c':
            $compart5x = $line[$ind['compliance_art._5c_(only)']];
            break;
    }


   if ($line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NI'
        || $line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == '?'
        || $line[$ind['art4_compliance_art._4_(hierarchical_approach)']] == 'NR') {
            return $line[$ind['art4_compliance_art._4_(hierarchical_approach)']];
    }

    if ($compart5x == 'PD') {
        return 'PD';
    }

    if ($line[$ind['generated_load_p.e.']] <= 10000) {
        return 'NR';
    }

    if ($compart5x == 'NR') {
        return 'NR';
    }

    if ($line[$ind['art4_compliance_art._4_(only)']] == 'C'
        && $compart5x == 'C') {
        return 'C';
    }

    if ($line[$ind['art4_compliance_art._4_(only)']] == 'NC'
        || $compart5x == 'NC') {
        return 'NC';
    }
    return '';
}

// =IF(  � �% of generated load for which compliance with Art. 5a/5b/5c is not relevant  � �>0;
//     "NR";
//     IF(AND(  �  deadline for agglomeration  � �<=reference date of reporting;
//              � �% of generated load not compliant with Art. 5a/5b/5c  � �=0;
//              � �% of generated load for which compliance with Art. 5a/5b/5c  is not relevant  � �=0);
//         "C";
//         IF(AND(  �  deadline for agglomeration  � �<=reference date of reporting;
//                  � �% of generated load for which compliance with Art. 5a/5b/5c  is not relevant  � �=0;
//                (AND(
//                      AND(  � �% of generated load not compliant with Art. 5a/5b/5c  � �>0;
//                           � �% of collected load not compliant with Art. 5a/5b/5c  � �<=1
//                         );
//                        � �p.e. not compliant with Art. 5a/5b/5c  � �<2000))
//                );
//                "C";
//                IF(AND(  �  deadline for agglomeration  � �<=reference date of reporting;
//                         � �% of generated load for which compliance with Art. 5a/5b/5c  is not relevant  � �=0;
//                       (OR(  � �% of generated load not compliant with Art. 5a/5b/5c  � �>1;
//                             � �p.e. not compliant with Art. 5a/5b/5c  � �>=2000))
//                      );
//                      "NC";
//                      7))))
function uwwtd_register_getcompliance_art5x_only($type, $ind, $line, $art5x_NC_collected, $reporting_year, $deadline_agglo) {
    switch($type) {
        case 'a':
            $nr = $line[$ind['not_relevant_5_a_(%_of_gen._load)']];
            $c = $line[$ind['compliant_5_a_(%_of_gen._load)']];
            $nc = $line[$ind['not_compliant_5_a_(%_of_gen._load)']];
            $nc_pe = $line[$ind['not_compliant_5_a_(p._e.)']];
            break;
        case 'b':
            $nr = $line[$ind['not_relevant_5_b_(%_of_gen._load)']];
            $c = $line[$ind['compliant_5_b_(%_of_gen._load)']];
            $nc = $line[$ind['not_compliant_5_b_(%_of_gen._load)']];
            $nc_pe = $line[$ind['not_compliant_5_b_(p._e.)']];
            break;
        case 'c':
            $nr = $line[$ind['not_relevant_5_c_(%_of_gen._load)']];
            $c = $line[$ind['compliant_5_c_(%_of_gen._load)']];
            $nc = $line[$ind['not_compliant_5_c_(%_of_gen._load)']];
            $nc_pe = $line[$ind['not_compliant_5_c_(p._e.)']];
            break;
    }

    if ($line[$ind['generated_load_p.e.']] <= 10000) {
        return 'NR';
    }
    elseif ($nr >= 0 && $c == 0 && $nc == 0) {
        return 'NR';
    }
    elseif ($deadline_agglo > $reporting_year) {
        return 'PD';
    }
    elseif ($nc == 0 && $c > 0) {
        return 'C';
    }
    elseif ($deadline_agglo < $reporting_year
        && $nc > 0
        && $art5x_NC_collected <= 1
        && $nc_pe < 2000
        ) {
        return 'C';
    }
    //nd@oieau.fr : 09/08/2019 : change the equality operator
    elseif ($deadline_agglo <= $reporting_year
        &&
        ($nc > 1 || $nc_pe >= 2000)
        ) {
        return 'NC';
    }
    //nd@oieau.fr => why do we return 7 ?
    return 7;
}

//cn@oieau 23/01/2017 modified in order to take into account the equipement and performance of secondary treatment
function uwwtd_get_compliance_5abc_uwwtp($uwwtp)
{
    $compliance5x = array(
            '5a' => 'NR',
            '5b' => 'NR',
            '5c' => 'NR'
        );
    // Compliance 5a

    if ($uwwtp->field_uwww_required_rcaap['und'][0]['value'] == 1 && $uwwtp->field_uwww_required_rcaan['und'][0]['value'] == 0) {
           if(  uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, array('premoval','secondary')) === true &&
                uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('ptotperf','codperf','bod5perf')) === true
            ) {
                $compliance5x['5a'] = 'C';
           } else {
                $compliance5x['5a'] = 'NC';
           }
    }

    if ($uwwtp->field_uwww_required_rcaan['und'][0]['value'] == 1 && $uwwtp->field_uwww_required_rcaap['und'][0]['value'] == 0) {
           if(  uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, array('nremoval','secondary')) === true &&
                uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('ntotperf','codperf','bod5perf')) === true
            ) {
                $compliance5x['5a'] = 'C';
           } else {
                $compliance5x['5a'] = 'NC';
           }
    }

    if ($uwwtp->field_uwww_required_rcaan['und'][0]['value'] == 1 && $uwwtp->field_uwww_required_rcaap['und'][0]['value'] == 1) {
           if(  uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, array('nremoval', 'premoval','secondary')) === true &&
                uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('ntotperf', 'ptotperf','codperf','bod5perf')) === true
            ) {
                $compliance5x['5a'] = 'C';
           } else {
                $compliance5x['5a'] = 'NC';
           }
    }

    // Compliance 5b
    if ($uwwtp->field_uwww_required_rcab['und'][0]['value'] == 1) {
        if (uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, array('nremoval','secondary')) === true &&
			uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('codperf','bod5perf')) === true
		) {
            $compliance5x['5b'] = 'C';
        } else {
            $compliance5x['5b'] = 'NC';
        }
    }

    // Compliance 5c
    if ($uwwtp->field_uwww_required_rcac['und'][0]['value'] == 1) {
        if (uwwtd_atleastone_uwwtp_treatement_inplace_in($uwwtp, array('uv', 'chlorination', 'ozonation', 'sandfiltration', 'microfiltration', 'other')) === true &&
			uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, array('secondary')) === true &&
			uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('codperf','bod5perf')) === true
		) {
            $compliance5x['5c'] = 'C';
        } else {
            $compliance5x['5c'] = 'NC';
        }
    }

    return $compliance5x;
}

function save_for_EU_feed ($node, $fieldsAndValues) {



    // node_save($node)

    return $node->nid;
}


function isStringPresentIn($value, $variable) {
    if (preg_match('/'.$value.'/', $variable)) {
        return true ;
    }
    return false ;
}

//created 23/01/2017 by cn@oieau
// allows to calculate the load of a leak between an agglomeration and its treatment plant
//param : - $complinace (string) : 'NC' return the load of the leak if it is more than 2000 p.e. or 1% of the connected load
//                                 'C' return the load of the leak if it is less than 2000 p.e. and less than 1% of the connected load
//                                 'NR' return zero
//        - connectd_load (float) : load connected to collectiong system
//        - entering_load (float) : total load entering in the WWTP linked to the agglomeration
//value : numerical value (load in p.e.)
function agglo_leak_pe($compliance, $connected_load, $entering_load){
	$agg_leak = ($connected_load - $entering_load);
	if ($compliance == 'NC'){
		if ($agg_leak > 2000 ||
		   ($agg_leak/$connected_load) > 0.01){
			return $agg_leak;
		}else{
			return 0;
		}
	}elseif ($compliance == 'C'){
		if ($agg_leak <= 2000 &&
		   ($agg_leak/$connected_load) <= 0.01){
			return $agg_leak;
		}else{
			return 0;
		}
	}elseif ($compliance == 'NR'){
		return 0;
	}
}

/**
 * Format the Acronym in the register
 */
function uwwtd_register_get_glossary_label($conf, $val) {
//   uwwtd_trace('-->'.var_export($conf, true));
//   uwwtd_trace('-->'.$val);           
  if (isset($conf['glossary']) && true === $conf['glossary'] && isset($GLOBALS['glossary'][ $val ])) {              
    $val = $val . ' [' . $GLOBALS['glossary'][ $val ] . ']';
  }    

  if (isset($conf['glossary_composed']) && true === $conf['glossary_composed']) {              
    $explode = explode(';', $val);
    $implode = '';
    foreach($explode as $element) {
      if (isset($GLOBALS['glossary'][ $element ])) {              
        $implode .= (empty($implode) ? '' : ';') . $element . ' [' . $GLOBALS['glossary'][ $element ] . ']';
      }
    }
    
    $val = $implode;
  }
//   uwwtd_trace('----->'.$val);
  return $val;
}

/**
 * Sort RCA
 */
function sort_rca_by_field_rcaplantscapacity($a, $b) {
    if (!isset($a->field_rcaplantscapacity['und'][0]['value']) && !isset($b->field_rcaplantscapacity['und'][0]['value'])) {
        return 0;
    }        
    if (!isset($a->field_rcaplantscapacity['und'][0]['value'])) {
        return -1;
    }
    if (!isset($b->field_rcaplantscapacity['und'][0]['value'])) {
        return 1;
    }        
    if ($a->field_rcaplantscapacity['und'][0]['value'] == $b->field_rcaplantscapacity['und'][0]['value']) {
        return 0;
    }
    return ($a->field_rcaplantscapacity['und'][0]['value'] < $b->field_rcaplantscapacity['und'][0]['value']) ? -1 : 1;
}


/**
 * Sort RCA
 */
function sort_rca_by_title($a, $b) {
    if ($a->title == $b->title) {
        return 0;
    }
    return ($a->title < $b->title) ? -1 : 1;
}