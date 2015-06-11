<?php

################################ DATA CENTER PAGE ################################
function uwwtd_data_form($form, &$form_state){

	if(isset($_SESSION['uwwtd_annee']))unset($_SESSION['uwwtd_annee']);

	$uwwtd_year = date("Y");
	$uwwtd_years = array(
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
	
	// definition du champ input de type select pour l'annee
	$form['uwwtd_year'] = array(     
		'#type' => 'select',
		'#title' => t('Year'),
		'#default_value' => $uwwtd_year,        
		'#options' => $uwwtd_years,
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

function uwwtd_data_form_submit($form, &$form_state){
	
	$annee = $form['uwwtd_year']['#value'];
	$_SESSION['uwwtd_annee'] = $annee;
	
	$form_state['redirect'] = 'data/import';	
}


################################ DATA INPUT ################################
function uwwtd_inport_form($form, &$form_state){

	if(!isset($_SESSION['uwwtd_annee'])){
		drupal_goto('data');
	}
	else{
		
		// Definition des variables pour le formulaire
		$annee = $_SESSION['uwwtd_annee'];
		
		$form['uwwtd_text'] = array(
			'#markup' => '<h4>'.t('Data import for the year').' : '.$annee.'</h4><p>'.t('To import your dataset please either upload an xml with all the data or a compressed (zipped) file with all the csv files correctly formatted. All the files must be encoded in UTF-8 to avoid confusion with special characters').'.</p>'
		);
		
		// definition du champ input de type file
		$form['uwwtd_file'] = array(
			'#type' => 'managed_file',
			'#title' => t('Data file'),
			'#upload_validators' => array('file_validate_extensions' => array('xml zip')),
			'#required' => TRUE,
		);
		
		// Bouton envoyer
		$form['submit_button'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
		
		$form['uwwtd_text2'] = array(
			'#markup' => '<br><br><p>'.t('Once finished uploading or to skip this step, please click the continue link below').'.</p>'
		);
		
		$form['continue_button'] = array(
			'#markup' => ''.l(t('Continue'), 'data/shapes').''
		);
		
		// Fonction a appeller quand on clique sur envoyer
		$form['#submit'][] = 'uwwtd_import_submit_handler';
		return $form;
	}
}

function uwwtd_import_submit_handler($form, &$form_state) {
	
	// Definition et recup des variables
	if(isset($_SESSION['uwwtd_errors'])) unset($_SESSION['uwwtd_errors']);
	if(isset($_SESSION['uwwtd_warnings'])) unset($_SESSION['uwwtd_warnings']);
	$annee = $_SESSION['uwwtd_annee'];
	$file = file_load($form_state['values']['uwwtd_file']);
	$uri = $file->uri;

	// reset session values
	$_SESSION['newRca'] = 0;
	$_SESSION['upRca'] = 0;
	$_SESSION['newAgg'] = 0;
	$_SESSION['upAgg'] = 0;
	$_SESSION['newUww'] = 0;
	$_SESSION['upUww'] = 0;
	$_SESSION['newDcp'] = 0;
	$_SESSION['upDcp'] = 0;
	$_SESSION['newBc'] = 0;
	$_SESSION['upBc'] = 0;

	switch($file->filemime){
		case 'application/zip':
			$newFile = uwwtd_file_upload($file, '.zip');
			$files = uwwtd_unzip_files($newFile, 'public://data_sources/');
			$datas = uwwtd_zipped_csvs_to_array($files);
		break;

		case 'application/xml':
			$newFile = uwwtd_file_upload($file, '.xml');
			$datas = uwwtd_xml_to_array($newFile->uri);
		break;
	}

	// get current timestamp
	//$time = time();
	$timestamp = strtotime('now');

	//dsm($timestamp);

	//dsm($datas);
	
	uwwtd_setup_batch($newFile, $annee, $datas, $timestamp);	
}

function uwwtd_setup_batch($newFile, $annee, $datas, $timestamp){
	$batch = array(
		'title' => t('Data import'),
		'operations' => array(
			//array('uwwtd_import_rca_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_import_bigcity_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_import_agglo_batch_process', array($newFile, $annee, $datas, $timestamp)),
			array('uwwtd_import_uww_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_import_dcp_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_link_dcp_rca_uww_batch_process', array($newFile, $annee, $datas, $timestamp)),
			array('uwwtd_link_uww_agglo_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_link_bc_agglo_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_bc_position_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_mega_links_batch_process', array($newFile, $annee, $datas, $timestamp)),
			//array('uwwtd_rca_extra_batch_process', array($newFile, $annee, $datas, $timestamp))
		),
		'error_message' => t('Error!'),
		'finished' => 'uwwtd_finished',
		'file' => drupal_get_path('module','uwwtd').'/data_center.inc'
	);

	batch_set($batch);
	batch_process('data/import');
}

function uwwtd_import_rca_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['ReceivingAreas'])){
		if (!isset($context['sandbox']['progress'])) {
		
			//Get all receiving areas from data
			if(isset($datas['UWWTD_Report']['ReceivingAreas']['ReceivingArea']['rcaCode'])) $imports = $datas['UWWTD_Report']['ReceivingAreas'];
			else $imports = $datas['UWWTD_Report']['ReceivingAreas']['ReceivingArea'];

			$context['sandbox']['max'] = count($imports);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $imports;
		}
		$i = $context['sandbox']['progress'];

		if($context['sandbox']['max'] === 1){
			$data = $context['sandbox']['ids']['ReceivingArea'];
		}
		else{
			$data = $context['sandbox']['ids'][$i];
		}

		// Check basic elements exist
		// errors
		$e = 0;
		if(!isset($data['rcaName']) || empty($data['rcaName'])){
			$e = 1;
			$emes = t('The receiving area does not have \'title\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		if(!isset($data['rcaCode']) || empty($data['rcaCode'])){
			$e = 1;
			$emes = t('The receiving area does not have \'code\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		// Check if no errors were found
		if($e == 0){

			// Variables
			$uri = $newFile->uri;
			$filename = $newFile->filename;
			$filename .= ".txt";
			$fid = $newFile->fid;
			$siteId = $annee.'_rca_'.uwwtd_check_text($data['rcaCode']);

			// Check if id exists
			$exists = uwwtd_check_exist($siteId);

			// If the element exists
			if($exists != false){

				// load the node
				$node = node_load($exists);
				$node->type = 'receiving_area';

				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_rcadcpliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);
				
				if(isset($data['rcaState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['rcaState']);}
				else uwwtd_add_error(1, 0, 'No receiving area status found.', $timestamp, $i, $node);
				
				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['rcaCode']);

				$node->title = uwwtd_check_text($data['rcaName']);

				if(isset($data['rcaType'])){
					$node->field_specialisedzonetype['und'][0]['value'] = uwwtd_check_text($data['rcaType']);}
				else uwwtd_add_error(1, 0, 'No receiving area specialised zone type found.', $timestamp, $i, $node);
				
				if(isset($data['rcaSensitiveArea'])){
					$node->field_rcadateart5854['und'][0]['value'] = uwwtd_check_text($data['rcaSensitiveArea']);}
				else uwwtd_add_error(1, 0, 'No receiving area Related Sensitive area found.', $timestamp, $i, $node);
				
				if(isset($data['rcaDateArt58'])){
					if(uwwtd_check_date($data['rcaDateArt58'], $i, $node) !==false){
						$node->field_rcadateart5854['und'][0]['value'] = uwwtd_check_date($data['rcaDateArt58'], $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area article 58 date found.', $timestamp, $i, $node);
				
				if(isset($data['rcaParameterN'])){
					if(uwwtd_check_bool($data['rcaParameterN'], $timestamp, $i, $node) !== false){
						$node->field_rca_parameter_n['und'][0]['value'] = uwwtd_check_bool($data['rcaParameterN'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area nitrogen parameter found.', $timestamp, $i, $node);

				if(isset($data['rcaParameterP'])){
					if(uwwtd_check_bool($data['rcaParameterP'], $timestamp, $i, $node) !== false){
						$node->field_rca_parameter_p['und'][0]['value'] = uwwtd_check_bool($data['rcaParameterP'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area phosphorus parameter found.', $timestamp, $i, $node);
				
				if(isset($data['rcaParameterOther'])){
					if(uwwtd_check_bool($data['rcaParameterOther'], $timestamp, $i, $node) !== false){
						$node->field_rca_parameter_other['und'][0]['value'] = uwwtd_check_bool($data['rcaParameterOther'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area other parameter found.', $timestamp, $i, $node);

				if(isset($data['rcaDateDesignation'])){
					if(uwwtd_check_date($data['rcaDateDesignation'], $i, $node) !== false){
						$node->field_rcadatedesignation61['und'][0]['value'] = uwwtd_check_date($data['rcaDateDesignation'], $i, $node);
						$node->field_rcaapdatedesignation['und'][0]['value'] = uwwtd_check_date($data['rcaDateDesignation'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area designation date found.', $timestamp, $i, $node);

				if(isset($data['rcaANitro'])){
					if(uwwtd_check_bool($data['rcaANitro'], $timestamp, $i, $node) !== false){
						$node->field_rcaanitro['und'][0]['value'] = uwwtd_check_bool($data['rcaANitro'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area nitrogen information found.', $timestamp, $i, $node);

				if(isset($data['rcaAPhos'])){
					if(uwwtd_check_bool($data['rcaAPhos'], $timestamp, $i, $node) !== false){
						$node->field_rcaaphos['und'][0]['value'] = uwwtd_check_bool($data['rcaAPhos'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area phosphorus information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaB'])){
					if( uwwtd_check_bool($data['rcaB'], $timestamp, $i, $node) !== false){
						$node->field_rcab['und'][0]['value'] = uwwtd_check_bool($data['rcaB'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area B information found.', $timestamp, $i, $node);

				if(isset($data['rcaC'])){
					if(uwwtd_check_bool($data['rcaB'], $timestamp, $i, $node) !== false){
						$node->field_rcac['und'][0]['value'] = uwwtd_check_bool($data['rcaC'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area C information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaCRelevantDirective'])){
					$node->field_rcacrelevantdirective['und'][0]['value'] = uwwtd_check_text($data['rcaCRelevantDirective']);}
				else uwwtd_add_error(1, 0, 'No receiving area relevant directive information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaMorphology'])){
					if(uwwtd_check_bool($data['rcaMorphology'], $timestamp, $i, $node) !== false){
						$node->field_rcamorphology['und'][0]['value'] = uwwtd_check_bool($data['rcaMorphology'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area morphology information found.', $timestamp, $i, $node);

				if(isset($data['rcaHydrologie'])){
					if(uwwtd_check_bool($data['rcaHydrologie'], $timestamp, $i, $node) !== false){
						$node->field_rcahydrologie['und'][0]['value'] = uwwtd_check_bool($data['rcaHydrologie'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area hydrology information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaHydraulic'])){
					if(uwwtd_check_bool($data['rcaHydraulic'], $timestamp, $i, $node) !== false){
						$node->field_rcahydraulic['und'][0]['value'] = uwwtd_check_bool($data['rcaHydraulic'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area hydraulic information found.', $timestamp, $i, $node);

				if(isset($data['rcaAbsenceRisk'])){
					if(uwwtd_check_bool($data['rcaAbsenceRisk'], $timestamp, $i, $node) !== false){
						$node->field_rcaabsencerisk['und'][0]['value'] = uwwtd_check_bool($data['rcaAbsenceRisk'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area absence risk information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaDateArt54'])){
					if(uwwtd_check_date($data['rcaDateArt54'], $i, $node) !==false){
						$node->field_rcadateart54['und'][0]['value'] = uwwtd_check_date($data['rcaDateArt54'], $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area article 54 date found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPlants'])){
					if(uwwtd_check_chiffre($data['rcaPlants'], $timestamp, $i, $node) !==false){
						$node->field_rcaPlants['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPlants'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area plants information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPlantsCapacity'])){
					if(uwwtd_check_chiffre($data['rcaPlantsCapacity'], $timestamp, $i, $node) !==false){
						$node->field_rcaplantscapacity['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPlantsCapacity'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area plants capacity information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['rcaNIncomingMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcanincomingmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNIncomingMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming N-tot measured information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNIncomingCalculated'])){
					if(uwwtd_check_chiffre($data['rcaNIncomingCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcanincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNIncomingCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['rcaNIncomingEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcanincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNIncomingEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['rcaPIncomingMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcapincomingmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPIncomingMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming P-tot measured information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPIncomingCalculated'])){
					if(uwwtd_check_chiffre($data['rcaPIncomingCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcapincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPIncomingCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming P-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['rcaPIncomingEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcapincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPIncomingEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNDischargedMeasured'])){
					if(uwwtd_check_chiffre($data['rcaNDischargedMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcandischargedmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNDischargedMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged N-tot measured information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNDischargedCalculated'])){
					if(uwwtd_check_chiffre($data['rcaNDischargedCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcandischargedcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNDischargedCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNDischargedEstimated'])){
					if(uwwtd_check_chiffre($data['rcaNDischargedEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcandischargedestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNDischargedEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPDischargedMeasured'])){
					if(uwwtd_check_chiffre($data['rcaPDischargedMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcapdischargedmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPDischargedMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged P-tot measured information found.', $timestamp, $i, $node);
			
				if(isset($data['rcaPDischargedCalculated'])){
					if(uwwtd_check_chiffre($data['rcaPDischargedCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcapdischargedcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPDischargedCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged P-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPDischargedEstimated'])){
					if(uwwtd_check_chiffre($data['rcaPDischargedEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcapdischargedestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPDischargedEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaGIS'])){
					$node->field_rcagis['und'][0]['value'] = uwwtd_check_text($data['rcaGIS']);}
				else uwwtd_add_error(1, 0, 'No receiving area root of corresponding GIS-data file found.', $timestamp, $i, $node);
				
				if(isset($data['rcaCIDOtherDirective'])){
					$node->field_rcacidotherdirective['und'][0]['value'] = uwwtd_check_text($data['rcaCIDOtherDirective']);}
				else uwwtd_add_error(1, 0, 'No receiving area other directive information found.', $timestamp, $i, $node);

				if(isset($data['rcaCDateOtherDirective'])){
					if(uwwtd_check_date($data['rcaCDateOtherDirective'], $i, $node) !== false){
						$node->field_rcacdateotherdirective['und'][0]['value'] = uwwtd_check_date($data['rcaCDateOtherDirective'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area other directive information date found.', $timestamp, $i, $node);

				if(isset($data['rcaRemarks'])){
					$node->field_rcaremarks['und'][0]['value'] = uwwtd_check_text($data['rcaRemarks']);}
				else uwwtd_add_error(1, 0, 'No receiving area root of correspo GIS-data file found.', $timestamp, $i, $node);
				
				if(isset($data['rcaArt54Applied'])){
					if(uwwtd_check_bool($data['rcaArt54Applied'], $timestamp, $i, $node) !== false){
						$node->field_rca54applied['und'][0]['value'] = uwwtd_check_bool($data['rcaArt54Applied'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area article 54 applied information found.', $timestamp, $i, $node);

				$node->field_rca52applied['und'][0]['value'] = '1';

				if($node->field_specialisedzonetype['und'][0]['value'] == 'A58'){
					$node->field_rca58applied['und'][0]['value'] = '1';
				}
				else{
					$node->field_rca58applied['und'][0]['value'] = '0';
				}
				
				// save node
				node_save($node);
				$_SESSION['upRca']++;

			}
			else{

				// create the node
				$node = new StdClass();
				$node->type = 'receiving_area';
				// save once to get nid
				node_save($node);

				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_rcadcpliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);
				
				if(isset($data['rcaState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['rcaState']);}
				else uwwtd_add_error(1, 0, 'No receiving area status found.', $timestamp, $i, $node);
				
				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['rcaCode']);

				$node->title = uwwtd_check_text($data['rcaName']);

				if(isset($data['rcaType'])){
					$node->field_specialisedzonetype['und'][0]['value'] = uwwtd_check_text($data['rcaType']);}
				else uwwtd_add_error(1, 0, 'No receiving area specialised zone type found.', $timestamp, $i, $node);
				
				if(isset($data['rcaSensitiveArea'])){
					$node->field_rcadateart5854['und'][0]['value'] = uwwtd_check_text($data['rcaSensitiveArea']);}
				else uwwtd_add_error(1, 0, 'No receiving area Related Sensitive area found.', $timestamp, $i, $node);
				
				if(isset($data['rcaDateArt58'])){
					if(uwwtd_check_date($data['rcaDateArt58'], $i, $node) !==false){
						$node->field_rcadateart5854['und'][0]['value'] = uwwtd_check_date($data['rcaDateArt58'], $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area article 58 date found.', $timestamp, $i, $node);
				
				if(isset($data['rcaParameterN'])){
					if(uwwtd_check_bool($data['rcaParameterN'], $timestamp, $i, $node) !== false){
						$node->field_rca_parameter_n['und'][0]['value'] = uwwtd_check_bool($data['rcaParameterN'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area nitrogen parameter found.', $timestamp, $i, $node);

				if(isset($data['rcaParameterP'])){
					if(uwwtd_check_bool($data['rcaParameterP'], $timestamp, $i, $node) !== false){
						$node->field_rca_parameter_p['und'][0]['value'] = uwwtd_check_bool($data['rcaParameterP'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area phosphorus parameter found.', $timestamp, $i, $node);
				
				if(isset($data['rcaParameterOther'])){
					if(uwwtd_check_bool($data['rcaParameterOther'], $timestamp, $i, $node) !== false){
						$node->field_rca_parameter_other['und'][0]['value'] = uwwtd_check_bool($data['rcaParameterOther'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area other parameter found.', $timestamp, $i, $node);

				if(isset($data['rcaDateDesignation'])){
					if(uwwtd_check_date($data['rcaDateDesignation'], $i, $node) !== false){
						$node->field_rcadatedesignation61['und'][0]['value'] = uwwtd_check_date($data['rcaDateDesignation'], $i, $node);
						$node->field_rcaapdatedesignation['und'][0]['value'] = uwwtd_check_date($data['rcaDateDesignation'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area designation date found.', $timestamp, $i, $node);

				if(isset($data['rcaANitro'])){
					if(uwwtd_check_bool($data['rcaANitro'], $timestamp, $i, $node) !== false){
						$node->field_rcaanitro['und'][0]['value'] = uwwtd_check_bool($data['rcaANitro'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area nitrogen information found.', $timestamp, $i, $node);

				if(isset($data['rcaAPhos'])){
					if(uwwtd_check_bool($data['rcaAPhos'], $timestamp, $i, $node) !== false){
						$node->field_rcaaphos['und'][0]['value'] = uwwtd_check_bool($data['rcaAPhos'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area phosphorus information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaB'])){
					if( uwwtd_check_bool($data['rcaB'], $timestamp, $i, $node) !== false){
						$node->field_rcab['und'][0]['value'] = uwwtd_check_bool($data['rcaB'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area B information found.', $timestamp, $i, $node);

				if(isset($data['rcaC'])){
					if(uwwtd_check_bool($data['rcaB'], $timestamp, $i, $node) !== false){
						$node->field_rcac['und'][0]['value'] = uwwtd_check_bool($data['rcaC'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area C information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaCRelevantDirective'])){
					$node->field_rcacrelevantdirective['und'][0]['value'] = uwwtd_check_text($data['rcaCRelevantDirective']);}
				else uwwtd_add_error(1, 0, 'No receiving area relevant directive information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaMorphology'])){
					if(uwwtd_check_bool($data['rcaMorphology'], $timestamp, $i, $node) !== false){
						$node->field_rcamorphology['und'][0]['value'] = uwwtd_check_bool($data['rcaMorphology'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area morphology information found.', $timestamp, $i, $node);

				if(isset($data['rcaHydrologie'])){
					if(uwwtd_check_bool($data['rcaHydrologie'], $timestamp, $i, $node) !== false){
						$node->field_rcahydrologie['und'][0]['value'] = uwwtd_check_bool($data['rcaHydrologie'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area hydrology information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaHydraulic'])){
					if(uwwtd_check_bool($data['rcaHydraulic'], $timestamp, $i, $node) !== false){
						$node->field_rcahydraulic['und'][0]['value'] = uwwtd_check_bool($data['rcaHydraulic'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area hydraulic information found.', $timestamp, $i, $node);

				if(isset($data['rcaAbsenceRisk'])){
					if(uwwtd_check_bool($data['rcaAbsenceRisk'], $timestamp, $i, $node) !== false){
						$node->field_rcaabsencerisk['und'][0]['value'] = uwwtd_check_bool($data['rcaAbsenceRisk'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area absence risk information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaDateArt54'])){
					if(uwwtd_check_date($data['rcaDateArt54'], $i, $node) !==false){
						$node->field_rcadateart54['und'][0]['value'] = uwwtd_check_date($data['rcaDateArt54'], $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area article 54 date found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPlants'])){
					if(uwwtd_check_chiffre($data['rcaPlants'], $timestamp, $i, $node) !==false){
						$node->field_rcaPlants['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPlants'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area plants information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPlantsCapacity'])){
					if(uwwtd_check_chiffre($data['rcaPlantsCapacity'], $timestamp, $i, $node) !==false){
						$node->field_rcaplantscapacity['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPlantsCapacity'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area plants capacity information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['rcaNIncomingMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcanincomingmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNIncomingMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming N-tot measured information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNIncomingCalculated'])){
					if(uwwtd_check_chiffre($data['rcaNIncomingCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcanincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNIncomingCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['rcaNIncomingEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcanincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNIncomingEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['rcaPIncomingMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcapincomingmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPIncomingMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming P-tot measured information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPIncomingCalculated'])){
					if(uwwtd_check_chiffre($data['rcaPIncomingCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcapincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPIncomingCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming P-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['rcaPIncomingEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcapincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPIncomingEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area incoming P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNDischargedMeasured'])){
					if(uwwtd_check_chiffre($data['rcaNDischargedMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcandischargedmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNDischargedMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged N-tot measured information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNDischargedCalculated'])){
					if(uwwtd_check_chiffre($data['rcaNDischargedCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcandischargedcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNDischargedCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaNDischargedEstimated'])){
					if(uwwtd_check_chiffre($data['rcaNDischargedEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcandischargedestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaNDischargedEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPDischargedMeasured'])){
					if(uwwtd_check_chiffre($data['rcaPDischargedMeasured'], $timestamp, $i, $node) !==false){
						$node->field_rcapdischargedmeasured['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPDischargedMeasured'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged P-tot measured information found.', $timestamp, $i, $node);
			
				if(isset($data['rcaPDischargedCalculated'])){
					if(uwwtd_check_chiffre($data['rcaPDischargedCalculated'], $timestamp, $i, $node) !==false){
						$node->field_rcapdischargedcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPDischargedCalculated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged P-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaPDischargedEstimated'])){
					if(uwwtd_check_chiffre($data['rcaPDischargedEstimated'], $timestamp, $i, $node) !==false){
						$node->field_rcapdischargedestimated['und'][0]['value'] = uwwtd_check_chiffre($data['rcaPDischargedEstimated'], $timestamp, $i, $node);
					}
				}	
				else uwwtd_add_error(1, 0, 'No receiving area discharged P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['rcaGIS'])){
					$node->field_rcagis['und'][0]['value'] = uwwtd_check_text($data['rcaGIS']);}
				else uwwtd_add_error(1, 0, 'No receiving area root of corresponding GIS-data file found.', $timestamp, $i, $node);
				
				if(isset($data['rcaCIDOtherDirective'])){
					$node->field_rcacidotherdirective['und'][0]['value'] = uwwtd_check_text($data['rcaCIDOtherDirective']);}
				else uwwtd_add_error(1, 0, 'No receiving area other directive information found.', $timestamp, $i, $node);

				if(isset($data['rcaCDateOtherDirective'])){
					if(uwwtd_check_date($data['rcaCDateOtherDirective'], $i, $node) !== false){
						$node->field_rcacdateotherdirective['und'][0]['value'] = uwwtd_check_date($data['rcaCDateOtherDirective'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area other directive information date found.', $timestamp, $i, $node);

				if(isset($data['rcaRemarks'])){
					$node->field_rcaremarks['und'][0]['value'] = uwwtd_check_text($data['rcaRemarks']);}
				else uwwtd_add_error(1, 0, 'No receiving area root of correspo GIS-data file found.', $timestamp, $i, $node);
				
				if(isset($data['rcaArt54Applied'])){
					if(uwwtd_check_bool($data['rcaArt54Applied'], $timestamp, $i, $node) !== false){
						$node->field_rca54applied['und'][0]['value'] = uwwtd_check_bool($data['rcaArt54Applied'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No receiving area article 54 applied information found.', $timestamp, $i, $node);

				$node->field_rca52applied['und'][0]['value'] = '1';

				if($node->field_specialisedzonetype['und'][0]['value'] == 'A58'){
					$node->field_rca58applied['und'][0]['value'] = '1';
				}
				else{
					$node->field_rca58applied['und'][0]['value'] = '0';
				}
				
				// save node
				node_save($node);
				$_SESSION['newRca']++;

			}
		
		}
		$context['sandbox']['progress']++;

		if(isset($node)){
			$context['message'] = t('Processing receiving area').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid);
			$context['results'][] = $node->nid . ' : ' . check_plain($node->title);
		}
		else{
			$context['message'] = t('Failed to import a receiving area.');
			$context['results'][] = t('Failed to import a receiving area.');
		}
		
		if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
			$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
		}
	}else{
			uwwtd_add_error(1, 0, 'No receiving areas found in the dataset', $timestamp, $i);
	}
}

function uwwtd_import_bigcity_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['BigCityDischargers'])){
		if (!isset($context['sandbox']['progress'])) {
		
			//Get all big cities from data
			$imports = $datas['UWWTD_Report']['BigCityDischargers']['BigCityDischarger'];

			$context['sandbox']['max'] = count($imports);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $imports;
		}
		$i = $context['sandbox']['progress'];
		
		if($context['sandbox']['max'] === 1){
			$data = $context['sandbox']['ids']['BigCityDischarger'];
		}
		else{
			$data = $context['sandbox']['ids'][$i];
		}

		// Check basic elements exist
		// errors
		$e = 0;
		if($data['bigCity'] == '' || !isset($data['bigCity'])){
			$e = 1;
			$emes = t('The big city does not have \'title\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		if($data['bigCityID'] == '' || !isset($data['bigCityID'])){
			$e = 1;
			$emes = t('The big city does not have \'identification\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		if($data['bigCountryCode'] == '' || !isset($data['bigCountryCode'])){
			$e = 1;
			$emes = t('The big city does not have \'code\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		// Check if no errors were found
		if($e == 0){

			// Variables
			$uri = $newFile->uri;
			$filename = $newFile->filename;
			$filename .= ".txt";
			$fid = $newFile->fid;
			$siteId = $annee.'_big_'.uwwtd_check_text($data['bigCityID']);

			// Check if id exists
			$exists = uwwtd_check_exist($siteId);

			// If the element exists
			if($exists != false){

				// load the node
				$node = node_load($exists);
				$node->type = 'big_city';

				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_bcaggliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);

				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['bigCityID']);

				$node->title = uwwtd_check_text($data['bigCity']);

				$node->field_bigcountrycode['und'][0]['value'] = uwwtd_check_text($data['bigCountryCode']);

				// save node
				node_save($node);
				$_SESSION['upBc']++;

			}
			else{

				// create the node
				$node = new StdClass();
				$node->type = 'big_city';
				node_save($node);

				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_bcaggliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);

				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['bigCityID']);

				$node->title = uwwtd_check_text($data['bigCity']);

				$node->field_bigcountrycode['und'][0]['value'] = uwwtd_check_text($data['bigCountryCode']);

				// save node
				node_save($node);
				$_SESSION['newBc']++;

			}
		
		}

		$context['sandbox']['progress']++;

		if(isset($node)){
			$context['message'] = t('Processing big city').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid);
			$context['results'][] = $node->nid . ' : ' . check_plain($node->title);
		}
		else{
			$context['message'] = t('Failed to import a big city.');
			$context['results'][] = t('Failed to import a big city.');
		}
		
		if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
			$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
		}
	}else{
		uwwtd_add_error(1, 0, 'No big cities found in the dataset', $timestamp);
	}
}

function uwwtd_import_agglo_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['Agglomerations'])){
		if (!isset($context['sandbox']['progress'])) {
		
			//Get all Agglomerations from data
			$imports = $datas['UWWTD_Report']['Agglomerations']['Agglomeration'];

			$context['sandbox']['max'] = count($imports);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $imports;
		}
		$i = $context['sandbox']['progress'];
		$data = $context['sandbox']['ids'][$i];

		// Check basic elements exist
		// errors
		$e = 0;
		if(!isset($data['aggName']) || empty($data['aggName'])){
			$e = 1;
			$emes = t('The agglomeration does not have \'title\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		if(!isset($data['aggCode']) || empty($data['aggCode'])){
			$e = 1;
			$emes = t('The agglomeration does not have \'identification\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		if(!isset($data['aggLatitude']) || empty($data['aggLatitude'])){
			$e = 1;
			$emes = t('The agglomeration does not have latitude information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		elseif(uwwtd_check_coordinate($data['aggLatitude'], 'latitude', $timestamp, $i) === false){
			$e = 1;
			$emes = t('The latitude for this agglomeration is invalid');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		if(!isset($data['aggLongitude']) || empty($data['aggLongitude'])){
			// if check_coord == false e=1
			$e = 1;
			$emes = t('The agglomeration does not have longitude information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		elseif(uwwtd_check_coordinate($data['aggLongitude'], 'longitude', $timestamp, $i) === false){
			$e = 1;
			$emes = t('The longitude for this agglomeration is invalid');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		
		// Check if no errors were found
		if($e == 0){

			// Variables
			$uri = $newFile->uri;
			$filename = $newFile->filename;
			$filename .= ".txt";
			$fid = $newFile->fid;
			$siteId = $annee.'_agg_'.uwwtd_check_text($data['aggCode']);

			// Check if id exists
			$exists = uwwtd_check_exist($siteId);

			// If the element exists
			if($exists != false){

				// load the node
				$node = node_load($exists);
				$node->type = 'agglomeration';

				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_aggcritb['und'][0]['value'] = 0;
				$node->field_aggcritca['und'][0]['value'] = 0;
				$node->field_aggcritcb['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$node->field_aggcompliance['und'][0]['value'] = 'NI';
				$node->field_aggart3compliance['und'][0]['value'] = 'NI';
				$node->field_aggart4compliance['und'][0]['value'] = 'NI';
				$node->field_aggart5compliance['und'][0]['value'] = 'NI';
				$geoData = uwwtd_coord_transform(uwwtd_check_coordinate($data['aggLatitude'], 'latitude', $timestamp, $i, $node), uwwtd_check_coordinate($data['aggLongitude'], 'longitude', $timestamp, $i, $node));
				if($geoData != false){
					$node->field_position_geo['und'][0] = array(
						'geom'=>$geoData['bin'],
						'geo_type'=>$geoData['point'],
						'lat'=>$geoData['geoy'],
						'lon'=>$geoData['geox'],
						'left'=>$geoData['geoy'],
						'top'=>$geoData['geox'],
						'bottom'=>$geoData['geoy'],
						'geohash'=>$geoData['geohash']
					);
				}
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_agguwwliste['und']);
				unset($node->field_aggbcliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$repcode = str_replace('_', '-', $datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$boom = explode('-', $repcode);
					$boom = explode(' ', $boom[0]);
					$rep = $boom[0];

					if($rep == 'LT'){
						$node->field_regionnuts['und'][0]['value'] = 'LT00';
					}

					if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
					    $node->field_regionnuts['und'][0]['value'] = 'PL112';
					}
				}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);

				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['aggCode']);

				$node->title = uwwtd_check_text($data['aggName']);

				if(isset($data['aggState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['aggState']);}
				else uwwtd_add_error(1, 0, 'No agglomeration status information found.', $timestamp, $i, $node);

				if(isset($data['aggNUTS'])){
					$node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['aggNUTS']);}
				else uwwtd_add_error(1, 0, 'No agglomeration nut code information found.', $timestamp, $i, $node);
				
				$node->field_agglatitude['und'][0]['value'] = uwwtd_check_coordinate($data['aggLatitude'], 'latitude', $timestamp, $i);
				
				$node->field_agglongitude['und'][0]['value'] = uwwtd_check_coordinate($data['aggLongitude'], 'longitude', $timestamp, $i);
				
				if(isset($data['aggGenerated'])){
					if(uwwtd_check_chiffre($data['aggGenerated'], $timestamp, $i, $node) !== false){
						$node->field_agggenerated['und'][0]['value'] = uwwtd_check_chiffre($data['aggGenerated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration generated load information found.', $timestamp, $i, $node);
					
				if(isset($data['bigID']) || isset($data['aggBigID'])){
					if(isset($data['bigID'])){
					$node->field_aggbigid['und'][0]['value'] = uwwtd_check_text($data['bigID']);}
					if(isset($data['aggBigID'])){
					$node->field_aggbigid['und'][0]['value'] = uwwtd_check_text($data['aggBigID']);}
				}
				else uwwtd_add_error(0, 0, 'No agglomeration big id information found.', $timestamp, $i, $node);

				if(isset($data['aggCalculation'])){
					$node->field_aggcalculation['und'][0]['value'] = uwwtd_check_text($data['aggCalculation']);}
				else uwwtd_add_error(1, 0, 'No agglomeration calculation information found.', $timestamp, $i, $node);

				if(isset($data['aggChanges'])){
					$node->field_aggchanges['und'][0]['value'] = uwwtd_check_text($data['aggChanges']);}
				else uwwtd_add_error(1, 0, 'No agglomeration changes information found.', $timestamp, $i, $node);

				if(isset($data['aggChangesComment'])){
					$node->field_aggchangescomment['und'][0]['value'] = uwwtd_check_text($data['aggChangesComment']);}
				else uwwtd_add_error(1, 0, 'No agglomeration changes comment information found.', $timestamp, $i, $node);

				if(isset($data['aggPeriodOver'])){
					if(uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node) !== false){
						$node->field_aggperiodover3['und'][0] = array(
							'value'=>uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);

						$node->field_aggperiodover4['und'][0] = array(
							'value'=>uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);

						$node->field_aggperiodover5['und'][0] = array(
							'value'=>uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}	
				}
				else uwwtd_add_error(1, 0, 'No agglomeration period over information found.', $timestamp, $i, $node);

				if(isset($data['aggC1'])){
					if(uwwtd_check_pourc($data['aggC1'], $timestamp, $i, $node) !== false){
						$node->field_aggc1['und'][0]['value'] = uwwtd_check_pourc($data['aggC1'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration c1 information found.', $timestamp, $i, $node);

				if(isset($data['aggMethodC1'])){
					$node->field_aggmethodc1['und'][0]['value'] = uwwtd_check_text($data['aggMethodC1']);}
				else uwwtd_add_error(1, 0, 'No agglomeration c1 method information found.', $timestamp, $i, $node);

				if(isset($data['aggC2'])){
					if(uwwtd_check_pourc($data['aggC2'], $timestamp, $i, $node) !== false){
						$node->field_aggc2['und'][0]['value'] = uwwtd_check_pourc($data['aggC2'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration c2 information found.', $timestamp, $i, $node);

				if(isset($data['aggMethodC2'])){
					$node->field_aggmethodc2['und'][0]['value'] = uwwtd_check_text($data['aggMethodC2']);}
				else uwwtd_add_error(1, 0, 'No agglomeration c2 method information found.', $timestamp, $i, $node);

				if(isset($data['aggPercWithoutTreatment'])){
					if(uwwtd_check_pourc($data['aggPercWithoutTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercwithouttreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercWithoutTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration percentage without treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggMethodWithoutTreatment'])){
					$node->field_aggmethodwithouttreatment['und'][0]['value'] = uwwtd_check_text($data['aggMethodWithoutTreatment']);}
				else uwwtd_add_error(1, 0, 'No agglomeration method percentage without treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggPercPrimTreatment'])){
					if(uwwtd_check_pourc($data['aggPercPrimTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercprimtreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercPrimTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration percentage with primary treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggPercSecTreatment'])){
					if(uwwtd_check_pourc($data['aggPercSecTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercsectreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercSecTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration percentage with secondary treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggPercStringentTreatment'])){
					if(uwwtd_check_pourc($data['aggPercStringentTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercstringenttreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercStringentTreatment'], $timestamp, $i, $node);}
					}
					else uwwtd_add_error(1, 0, 'No agglomeration percentage with more stringent treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggHaveRegistrationSystem'])){
					$node->field_agghaveregistrationsystem['und'][0]['value'] = uwwtd_check_text($data['aggHaveRegistrationSystem']);}
				else uwwtd_add_error(1, 0, 'No agglomeration registration system information found.', $timestamp, $i, $node);

				if(isset($data['aggExistMaintenancePlan'])){
					$node->field_aggexistmaintenanceplan['und'][0]['value'] = uwwtd_check_text($data['aggExistMaintenancePlan']);}
				else uwwtd_add_error(1, 0, 'No agglomeration maintenance plan information found.', $timestamp, $i, $node);

				if(isset($data['aggPressureTest'])){
					if(uwwtd_check_bool($data['aggPressureTest'], $timestamp, $i, $node) !== false){
						$node->field_aggpressuretest['und'][0]['value'] = uwwtd_check_bool($data['aggPressureTest'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration pressure test information found.', $timestamp, $i, $node);

				if(isset($data['aggVideoInspections'])){
					if(uwwtd_check_bool($data['aggVideoInspections'], $timestamp, $i, $node) !== false){
						$node->field_aggvideoinspections['und'][0]['value'] = uwwtd_check_bool($data['aggVideoInspections'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration video inspections information found.', $timestamp, $i, $node);

				if(isset($data['aggOtherMeasures'])){
					if(uwwtd_check_bool($data['aggOtherMeasures'], $timestamp, $i, $node) !== false){
						$node->field_aggothermeasures['und'][0]['value'] = uwwtd_check_bool($data['aggOtherMeasures'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration other mesures information found.', $timestamp, $i, $node);

				if(isset($data['aggExplanationOther'])){
					$node->field_aggexplanationother['und'][0]['value'] = uwwtd_check_text($data['aggExplanationOther']);}
				else uwwtd_add_error(1, 0, 'No agglomeration explanation of other measures information found.', $timestamp, $i, $node);
				
				if(isset($data['aggSewageNetwork'])){
					$node->field_aggsewagenetwork['und'][0]['value'] = uwwtd_check_text($data['aggSewageNetwork']);}
				else uwwtd_add_error(1, 0, 'No agglomeration type of collecing system information found.', $timestamp, $i, $node);
				
				if(isset($data['aggBestTechnicalKnowledge'])){
					$node->field_aggbesttechnicalknowledge['und'][0]['value'] = uwwtd_check_text($data['aggBestTechnicalKnowledge']);}
				else uwwtd_add_error(1, 0, 'No agglomeration best technical knowledge information found.', $timestamp, $i, $node);
	
				if(isset($data['aggDilutionRates'])){
					if(uwwtd_check_bool($data['aggDilutionRates'], $timestamp, $i, $node) !== false){
						$node->field_agg_dilution_rates['und'][0]['value'] = uwwtd_check_bool($data['aggDilutionRates'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration the measures based on: Dilution rates information found.', $timestamp, $i, $node);

				if(isset($data['aggCapacity'])){
					if(uwwtd_check_bool($data['aggCapacity'], $timestamp, $i, $node) !== false){
						$node->field_aggcapacity['und'][0]['value'] = uwwtd_check_bool($data['aggCapacity'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration the measures based on: Capacity in relation to dry weather flow information found.', $timestamp, $i, $node);

				if(isset($data['aggAccOverflows'])){
					if(uwwtd_check_bool($data['aggAccOverflows'], $timestamp, $i, $node) !== false){
						$node->field_aggaccoverflows['und'][0]['value'] = uwwtd_check_bool($data['aggAccOverflows'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration the measures based on: Acceptable number of overflows per year information found.', $timestamp, $i, $node);

				if(isset($data['aggAccOverflowNumber'])){
					if(uwwtd_check_chiffre($data['aggAccOverflowNumber'], $timestamp, $i, $node) !== false){
						$node->field_aggaccoverflownumber['und'][0]['value'] = uwwtd_check_chiffre($data['aggAccOverflowNumber'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration indicate number of overflows information found.', $timestamp, $i, $node);

				if(isset($data['aggSewerOverflows_m3'])){
					if(uwwtd_check_int($data['aggSewerOverflows_m3'], $i, $node) !== false){
						$node->field_aggseweroverflows_m3['und'][0]['value'] = uwwtd_check_int($data['aggSewerOverflows_m3'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration number of raw sewage discharged from combined sewer overflows (CSOs) in the reporting year : m3 / year found.', $timestamp, $i, $node);
			
				if(isset($data['aggSewerOverflows_pe'])){
					if(uwwtd_check_int($data['aggSewerOverflows_pe'], $i, $node) !== false){
						$node->field_aggseweroverflows_pe['und'][0]['value'] = uwwtd_check_int($data['aggSewerOverflows_pe'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration number of raw sewage has been discharged through combined sewer overflows (CSOs) in the year reported: p.e. found.', $timestamp, $i, $node);
				
				if(isset($data['aggRemarks'])){
					$node->field_aggremarks['und'][0]['value'] = uwwtd_check_text($data['aggRemarks']);}
				else uwwtd_add_error(1, 0, 'No agglomeration remarks information found.', $timestamp, $i, $node);
				
				if(isset($data['aggForecast'])){
					if(uwwtd_check_date($data['aggForecast'], $i, $node) !== false){
						$node->field_aggforecast['und'][0]['value'] = uwwtd_check_date($data['aggForecast'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration when the total generated load of the agglomeration will be collected by the collection system or sent by IAS found.', $timestamp, $i, $node);
				
				// save node
				node_save($node);
				$_SESSION['upAgg']++;

			}
			else{

				// create the node
				$node = new StdClass();
				$node->type = 'agglomeration';
				node_save($node);

				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_aggcritb['und'][0]['value'] = 0;
				$node->field_aggcritca['und'][0]['value'] = 0;
				$node->field_aggcritcb['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$node->field_aggcompliance['und'][0]['value'] = 'NI';
				$node->field_aggart3compliance['und'][0]['value'] = 'NI';
				$node->field_aggart4compliance['und'][0]['value'] = 'NI';
				$node->field_aggart5compliance['und'][0]['value'] = 'NI';
				$geoData = uwwtd_coord_transform(uwwtd_check_coordinate($data['aggLatitude'], 'latitude', $timestamp, $i, $node), uwwtd_check_coordinate($data['aggLongitude'], 'longitude', $timestamp, $i, $node));
				if($geoData != false){
					$node->field_position_geo['und'][0] = array(
						'geom'=>$geoData['bin'],
						'geo_type'=>$geoData['point'],
						'lat'=>$geoData['geoy'],
						'lon'=>$geoData['geox'],
						'left'=>$geoData['geoy'],
						'top'=>$geoData['geox'],
						'bottom'=>$geoData['geoy'],
						'geohash'=>$geoData['geohash']
					);
				}
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_agguwwliste['und']);
				unset($node->field_aggbcliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$repcode = str_replace('_', '-', $datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$boom = explode('-', $repcode);
					$boom = explode(' ', $boom[0]);
					$rep = $boom[0];

					if($rep == 'LT'){
						$node->field_regionnuts['und'][0]['value'] = 'LT00';
					}

					if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
					    $node->field_regionnuts['und'][0]['value'] = 'PL112';
					}
				}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);

				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['aggCode']);

				$node->title = uwwtd_check_text($data['aggName']);

				if(isset($data['aggState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['aggState']);}
				else uwwtd_add_error(1, 0, 'No agglomeration status information found.', $timestamp, $i, $node);

				if(isset($data['aggNUTS'])){
					$node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['aggNUTS']);}
				else uwwtd_add_error(1, 0, 'No agglomeration nut code information found.', $timestamp, $i, $node);
				
				$node->field_agglatitude['und'][0]['value'] = uwwtd_check_coordinate($data['aggLatitude'], 'latitude', $timestamp, $i);
				
				$node->field_agglongitude['und'][0]['value'] = uwwtd_check_coordinate($data['aggLongitude'], 'longitude', $timestamp, $i);
				
				if(isset($data['aggGenerated'])){
					if(uwwtd_check_chiffre($data['aggGenerated'], $timestamp, $i, $node) !== false){
						$node->field_agggenerated['und'][0]['value'] = uwwtd_check_chiffre($data['aggGenerated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration generated load information found.', $timestamp, $i, $node);
					
				if(isset($data['bigID']) || isset($data['aggBigID'])){
					if(isset($data['bigID'])){
					$node->field_aggbigid['und'][0]['value'] = uwwtd_check_text($data['bigID']);}
					if(isset($data['aggBigID'])){
					$node->field_aggbigid['und'][0]['value'] = uwwtd_check_text($data['aggBigID']);}
				}
				else uwwtd_add_error(0, 0, 'No agglomeration big id information found.', $timestamp, $i, $node);

				if(isset($data['aggCalculation'])){
					$node->field_aggcalculation['und'][0]['value'] = uwwtd_check_text($data['aggCalculation']);}
				else uwwtd_add_error(1, 0, 'No agglomeration calculation information found.', $timestamp, $i, $node);

				if(isset($data['aggChanges'])){
					$node->field_aggchanges['und'][0]['value'] = uwwtd_check_text($data['aggChanges']);}
				else uwwtd_add_error(1, 0, 'No agglomeration changes information found.', $timestamp, $i, $node);

				if(isset($data['aggChangesComment'])){
					$node->field_aggchangescomment['und'][0]['value'] = uwwtd_check_text($data['aggChangesComment']);}
				else uwwtd_add_error(1, 0, 'No agglomeration changes comment information found.', $timestamp, $i, $node);

				if(isset($data['aggPeriodOver'])){
					if(uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node) !== false){
						$node->field_aggperiodover3['und'][0] = array(
							'value'=>uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);

						$node->field_aggperiodover4['und'][0] = array(
							'value'=>uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);

						$node->field_aggperiodover5['und'][0] = array(
							'value'=>uwwtd_check_date($data['aggPeriodOver'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}	
				}
				else uwwtd_add_error(1, 0, 'No agglomeration period over information found.', $timestamp, $i, $node);

				if(isset($data['aggC1'])){
					if(uwwtd_check_pourc($data['aggC1'], $timestamp, $i, $node) !== false){
						$node->field_aggc1['und'][0]['value'] = uwwtd_check_pourc($data['aggC1'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration c1 information found.', $timestamp, $i, $node);

				if(isset($data['aggMethodC1'])){
					$node->field_aggmethodc1['und'][0]['value'] = uwwtd_check_text($data['aggMethodC1']);}
				else uwwtd_add_error(1, 0, 'No agglomeration c1 method information found.', $timestamp, $i, $node);

				if(isset($data['aggC2'])){
					if(uwwtd_check_pourc($data['aggC2'], $timestamp, $i, $node) !== false){
						$node->field_aggc2['und'][0]['value'] = uwwtd_check_pourc($data['aggC2'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration c2 information found.', $timestamp, $i, $node);

				if(isset($data['aggMethodC2'])){
					$node->field_aggmethodc2['und'][0]['value'] = uwwtd_check_text($data['aggMethodC2']);}
				else uwwtd_add_error(1, 0, 'No agglomeration c2 method information found.', $timestamp, $i, $node);

				if(isset($data['aggPercWithoutTreatment'])){
					if(uwwtd_check_pourc($data['aggPercWithoutTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercwithouttreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercWithoutTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration percentage without treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggMethodWithoutTreatment'])){
					$node->field_aggmethodwithouttreatment['und'][0]['value'] = uwwtd_check_text($data['aggMethodWithoutTreatment']);}
				else uwwtd_add_error(1, 0, 'No agglomeration method percentage without treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggPercPrimTreatment'])){
					if(uwwtd_check_pourc($data['aggPercPrimTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercprimtreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercPrimTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration percentage with primary treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggPercSecTreatment'])){
					if(uwwtd_check_pourc($data['aggPercSecTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercsectreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercSecTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration percentage with secondary treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggPercStringentTreatment'])){
					if(uwwtd_check_pourc($data['aggPercStringentTreatment'], $timestamp, $i, $node) !== false){
						$node->field_aggpercstringenttreatment['und'][0]['value'] = uwwtd_check_pourc($data['aggPercStringentTreatment'], $timestamp, $i, $node);}
					}
					else uwwtd_add_error(1, 0, 'No agglomeration percentage with more stringent treatment information found.', $timestamp, $i, $node);

				if(isset($data['aggHaveRegistrationSystem'])){
					$node->field_agghaveregistrationsystem['und'][0]['value'] = uwwtd_check_text($data['aggHaveRegistrationSystem']);}
				else uwwtd_add_error(1, 0, 'No agglomeration registration system information found.', $timestamp, $i, $node);

				if(isset($data['aggExistMaintenancePlan'])){
					$node->field_aggexistmaintenanceplan['und'][0]['value'] = uwwtd_check_text($data['aggExistMaintenancePlan']);}
				else uwwtd_add_error(1, 0, 'No agglomeration maintenance plan information found.', $timestamp, $i, $node);

				if(isset($data['aggPressureTest'])){
					if(uwwtd_check_bool($data['aggPressureTest'], $timestamp, $i, $node) !== false){
						$node->field_aggpressuretest['und'][0]['value'] = uwwtd_check_bool($data['aggPressureTest'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration pressure test information found.', $timestamp, $i, $node);

				if(isset($data['aggVideoInspections'])){
					if(uwwtd_check_bool($data['aggVideoInspections'], $timestamp, $i, $node) !== false){
						$node->field_aggvideoinspections['und'][0]['value'] = uwwtd_check_bool($data['aggVideoInspections'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration video inspections information found.', $timestamp, $i, $node);

				if(isset($data['aggOtherMeasures'])){
					if(uwwtd_check_bool($data['aggOtherMeasures'], $timestamp, $i, $node) !== false){
						$node->field_aggothermeasures['und'][0]['value'] = uwwtd_check_bool($data['aggOtherMeasures'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration other mesures information found.', $timestamp, $i, $node);

				if(isset($data['aggExplanationOther'])){
					$node->field_aggexplanationother['und'][0]['value'] = uwwtd_check_text($data['aggExplanationOther']);}
				else uwwtd_add_error(1, 0, 'No agglomeration explanation of other measures information found.', $timestamp, $i, $node);
				
				if(isset($data['aggSewageNetwork'])){
					$node->field_aggsewagenetwork['und'][0]['value'] = uwwtd_check_text($data['aggSewageNetwork']);}
				else uwwtd_add_error(1, 0, 'No agglomeration type of collecing system information found.', $timestamp, $i, $node);
				
				if(isset($data['aggBestTechnicalKnowledge'])){
					$node->field_aggbesttechnicalknowledge['und'][0]['value'] = uwwtd_check_text($data['aggBestTechnicalKnowledge']);}
				else uwwtd_add_error(1, 0, 'No agglomeration best technical knowledge information found.', $timestamp, $i, $node);
	
				if(isset($data['aggDilutionRates'])){
					if(uwwtd_check_bool($data['aggDilutionRates'], $timestamp, $i, $node) !== false){
						$node->field_agg_dilution_rates['und'][0]['value'] = uwwtd_check_bool($data['aggDilutionRates'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration the measures based on: Dilution rates information found.', $timestamp, $i, $node);

				if(isset($data['aggCapacity'])){
					if(uwwtd_check_bool($data['aggCapacity'], $timestamp, $i, $node) !== false){
						$node->field_aggcapacity['und'][0]['value'] = uwwtd_check_bool($data['aggCapacity'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration the measures based on: Capacity in relation to dry weather flow information found.', $timestamp, $i, $node);

				if(isset($data['aggAccOverflows'])){
					if(uwwtd_check_bool($data['aggAccOverflows'], $timestamp, $i, $node) !== false){
						$node->field_aggaccoverflows['und'][0]['value'] = uwwtd_check_bool($data['aggAccOverflows'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration the measures based on: Acceptable number of overflows per year information found.', $timestamp, $i, $node);

				if(isset($data['aggAccOverflowNumber'])){
					if(uwwtd_check_chiffre($data['aggAccOverflowNumber'], $timestamp, $i, $node) !== false){
						$node->field_aggaccoverflownumber['und'][0]['value'] = uwwtd_check_chiffre($data['aggAccOverflowNumber'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration indicate number of overflows information found.', $timestamp, $i, $node);

				if(isset($data['aggSewerOverflows_m3'])){
					if(uwwtd_check_int($data['aggSewerOverflows_m3'], $i, $node) !== false){
						$node->field_aggseweroverflows_m3['und'][0]['value'] = uwwtd_check_int($data['aggSewerOverflows_m3'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration number of raw sewage discharged from combined sewer overflows (CSOs) in the reporting year : m3 / year found.', $timestamp, $i, $node);
			
				if(isset($data['aggSewerOverflows_pe'])){
					if(uwwtd_check_int($data['aggSewerOverflows_pe'], $i, $node) !== false){
						$node->field_aggseweroverflows_pe['und'][0]['value'] = uwwtd_check_int($data['aggSewerOverflows_pe'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration number of raw sewage has been discharged through combined sewer overflows (CSOs) in the year reported: p.e. found.', $timestamp, $i, $node);
				
				if(isset($data['aggRemarks'])){
					$node->field_aggremarks['und'][0]['value'] = uwwtd_check_text($data['aggRemarks']);}
				else uwwtd_add_error(1, 0, 'No agglomeration remarks information found.', $timestamp, $i, $node);
				
				if(isset($data['aggForecast'])){
					if(uwwtd_check_date($data['aggForecast'], $i, $node) !== false){
						$node->field_aggforecast['und'][0]['value'] = uwwtd_check_date($data['aggForecast'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No agglomeration when the total generated load of the agglomeration will be collected by the collection system or sent by IAS found.', $timestamp, $i, $node);
				
				// save node
				node_save($node);
				$_SESSION['newAgg']++;

			}
		
		}

		$context['sandbox']['progress']++;

		if(isset($node)){
			$context['message'] = t('Processing agglomeration').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid);
			$context['results'][] = $node->nid . ' : ' . check_plain($node->title);
		}
		else{
			$context['message'] = t('Failed to import a agglomeration.');
			$context['results'][] = t('Failed to import a agglomeration.');
		}
		
		if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
			$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
		}
	}else{
		uwwtd_add_error(1, 0, 'No Agglomerations found in the dataset', $timestamp);
	}
}

function uwwtd_import_uww_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['UWWTPs'])){
		if (!isset($context['sandbox']['progress'])) {
		
			//Get all UWWTPs from data
			$imports = $datas['UWWTD_Report']['UWWTPs']['UWWTP'];

			$context['sandbox']['max'] = count($imports);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $imports;
		}
		$i = $context['sandbox']['progress'];
		$data = $context['sandbox']['ids'][$i];

		// Check basic elements exist
		// errors
		$e = 0;
		if($data['uwwName'] == '' || !isset($data['uwwName'])){
			$e = 1;
			$emes = t('The urban waste water treatment plant does not have \'title\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		if($data['uwwCode'] == '' || !isset($data['uwwCode'])){
			$e = 1;
			$emes = t('The urban waste water treatment plant does not have \'identification\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		
		if(!isset($data['uwwLatitude']) || empty($data['uwwLatitude'])){
			$e = 1;
			$emes = t('The urban waste water treatment plan does not have latitude information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		elseif(uwwtd_check_coordinate($data['uwwLatitude'], 'latitude', $timestamp, $i) === false){
			$e = 1;
			$emes = t('The latitude for this urban waste water treatment plant is invalid');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		
		if(!isset($data['uwwLongitude']) || empty($data['uwwLongitude'])){
			$e = 1;
			$emes = t('The urban waste water treatment plan does not have longitude information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		elseif(uwwtd_check_coordinate($data['uwwLongitude'], 'longitude', $timestamp, $i) === false){
			$e = 1;
			$emes = t('The longitude for this urban waste water treatment plan is invalid');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		
		// Check if no errors were found
		if($e == 0){

			// Variables
			$uri = $newFile->uri;
			$filename = $newFile->filename;
			$filename .= ".txt";
			$fid = $newFile->fid;
			$siteId = $annee.'_uww_'.uwwtd_check_text($data['uwwCode']);

			// Check if id exists
			$exists = uwwtd_check_exist($siteId);

			// If the element exists
			if($exists != false){

				// load the node
				$node = node_load($exists);
				$node->type = 'uwwtp';

				// site mapping
				// calculate treatment type
				$treatment = 'NI';
				if(isset($data['uwwPrimaryTreatment']) && uwwtd_check_bool($data['uwwPrimaryTreatment']) == 1) $treatment = 'P';
				if(isset($data['uwwSecondaryTreatment']) && uwwtd_check_bool($data['uwwSecondaryTreatment']) == 1) $treatment = 'S';
				if(isset($data['uwwNRemoval']) && uwwtd_check_bool($data['uwwNRemoval']) == 1) $treatment = 'MS';
				if(isset($data['uwwPRemoval']) && uwwtd_check_bool($data['uwwPRemoval']) == 1) $treatment = 'MS';
				if(isset($data['uwwUV']) && uwwtd_check_bool($data['uwwUV']) == 1) $treatment = 'MS';
				if(isset($data['uwwChlorination']) && uwwtd_check_bool($data['uwwChlorination']) == 1) $treatment = 'MS';
				if(isset($data['uwwOzonation']) && uwwtd_check_bool($data['uwwOzonation']) == 1) $treatment = 'MS';
				if(isset($data['uwwSandFiltration']) && uwwtd_check_bool($data['uwwSandFiltration']) == 1) $treatment = 'MS';
				if(isset($data['uwwMicroFiltration']) && uwwtd_check_bool($data['uwwMicroFiltration']) == 1) $treatment = 'MS';
				if(isset($data['uwwOtherTreatment']) && uwwtd_check_bool($data['uwwOtherTreatment']) == 1) $treatment = 'MS';
				$node->field_uwwtreatmenttype['und'][0]['value'] = $treatment;

				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				//$node->field_uwwcompliance['und'][0]['value'] = 'NI';
				$geoData = uwwtd_coord_transform(uwwtd_check_coordinate($data['uwwLatitude'], 'latitude', $timestamp, $i, $node), uwwtd_check_coordinate($data['uwwLongitude'], 'longitude', $timestamp, $i, $node));
				if($geoData != false){
					$node->field_position_geo['und'][0] = array(
						'geom'=>$geoData['bin'],
						'geo_type'=>$geoData['point'],
						'lat'=>$geoData['geoy'],
						'lon'=>$geoData['geox'],
						'left'=>$geoData['geoy'],
						'top'=>$geoData['geox'],
						'bottom'=>$geoData['geoy'],
						'geohash'=>$geoData['geohash']
					);
				}
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				//unset($node->field_uwwaggliste['und']);
				//unset($node->field_uwwdcpliste['und']);
				
				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$repcode = str_replace('_', '-', $datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$boom = explode('-', $repcode);
					$boom = explode(' ', $boom[0]);
					$rep = $boom[0];

					if($rep == 'LT'){
						$node->field_regionnuts['und'][0]['value'] = 'LT00';
					}

					if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
					    $node->field_regionnuts['und'][0]['value'] = 'PL112';
					}
				}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);

				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['uwwCode']);
				
				$node->title = uwwtd_check_text($data['uwwName']);
				
				if(isset($data['uwwState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['uwwState']);
				}else uwwtd_add_error(1, 0, 'No UWWTP status information found.', $timestamp, $i, $node);

				if(isset($data['aggID'])){
					$node->field_uwwtdaggid['und'][0]['value'] = uwwtd_check_text($data['aggID']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs ID of agglomeration information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCollectingSystem']))$node->field_uwwcollectingsystem['und'][0]['value'] = uwwtd_check_text($data['uwwCollectingSystem']);
				
				if(isset($data['uwwDateClosing'])){
					$node->field_validto['und'][0]['value'] = uwwtd_check_text($data['uwwDateClosing']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs date of closing information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwHistorie'])){
					$node->field_uwwhistorie['und'][0]['value'] = uwwtd_check_text($data['uwwHistorie']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs information found.', $timestamp, $i, $node);
				
				if($rep == 'LT'){
					$node->field_regionnuts['und'][0]['value'] = 'LT00';
				}
				$node->field_uwwlatitude['und'][0]['value'] = uwwtd_check_coordinate($data['uwwLatitude']);
				
				$node->field_uwwlongitude['und'][0]['value'] = uwwtd_check_coordinate($data['uwwLongitude']);
				
				if(isset($data['uwwNUTS']))$node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['uwwNUTS']);
				
				if(isset($data['uwwLoadEnteringUWWTP'])){
					if(uwwtd_check_chiffre($data['uwwLoadEnteringUWWTP'], $timestamp, $i, $node) !== false){
						$node->field_uwwloadenteringuwwtp['und'][0]['value'] = uwwtd_check_int($data['uwwLoadEnteringUWWTP'], $timestamp, $i, $node);
					}	
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs load entering found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCapacity'])){
					if(uwwtd_check_int($data['uwwCapacity'], $i, $node) !== false){
						$node->field_physicalcapacityactivity['und'][0]['value'] = uwwtd_check_int($data['uwwCapacity'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs physical capacity found.', $timestamp, $i, $node);
					
				if(isset($data['uwwPrimaryTreatment'])){
					if(uwwtd_check_bool($data['uwwPrimaryTreatment'], $timestamp, $i, $node) !== false){
						$node->field_uwwprimarytreatment['und'][0]['value'] = uwwtd_check_bool($data['uwwPrimaryTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs Primary treatment information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwSecondaryTreatment'])){
					if(uwwtd_check_bool($data['uwwSecondaryTreatment'], $timestamp, $i, $node) !== false){
						$node->field_uwwsecondarytreatment['und'][0]['value'] = uwwtd_check_bool($data['uwwSecondaryTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs Secondary treatment information found.', $timestamp, $i, $node);
					
				if(isset($data['uwwOtherTreatment'])){
					if(uwwtd_check_bool($data['uwwOtherTreatment'], $timestamp, $i, $node) !== false){
						$node->field_uwwothertreat['und'][0]['value'] = uwwtd_check_bool($data['uwwOtherTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs other treatment found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNRemoval'])){
					if(uwwtd_check_bool($data['uwwNRemoval'], $timestamp, $i, $node) !== false){
						$node->field_uwwnremoval['und'][0]['value'] = uwwtd_check_bool($data['uwwNRemoval'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs N removal information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPRemoval'])){
					if(uwwtd_check_bool($data['uwwPRemoval'], $timestamp, $i, $node) !== false){
						$node->field_uwwpremoval['und'][0]['value'] = uwwtd_check_bool($data['uwwPRemoval'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs P removal information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwUV'])){
					if(uwwtd_check_bool($data['uwwUV'], $timestamp, $i, $node) !== false){
						$node->field_uwwuv['und'][0]['value'] = uwwtd_check_bool($data['uwwUV'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs UV information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwChlorination'])){
					if(uwwtd_check_bool($data['uwwChlorination'], $timestamp, $i, $node) !== false){
						$node->field_uwwchlorination['und'][0]['value'] = uwwtd_check_bool($data['uwwChlorination'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs chlorination information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwOzonation'])){
					if(uwwtd_check_bool($data['uwwOzonation'], $timestamp, $i, $node) !== false){
						$node->field_uwwozonation['und'][0]['value'] = uwwtd_check_bool($data['uwwOzonation'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs ozonation information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwSandFiltration'])){
					if(uwwtd_check_bool($data['uwwSandFiltration'], $timestamp, $i, $node) !==false){
						$node->field_uwwsandfiltration['und'][0]['value'] = uwwtd_check_bool($data['uwwSandFiltration'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs sand filtration information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwMicroFiltration'])){
					if(uwwtd_check_bool($data['uwwMicroFiltration'], $timestamp, $i, $node) !== false){
						$node->field_uwwmicrofiltration['und'][0]['value'] = uwwtd_check_bool($data['uwwMicroFiltration'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs micro filtration information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwOther'])){
					if(uwwtd_check_bool($data['uwwOther'], $timestamp, $i, $node) !== false){
						$node->field_uwwother['und'][0]['value'] = uwwtd_check_bool($data['uwwOther'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs other type information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwSpecification'])){
					$node->field_uwwspecification['und'][0]['value'] = uwwtd_check_text($data['uwwSpecification']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs specify other type of more stringent treatment information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBOD5Perf'])){
					$node->field_uwwbod5perf['und'][0]['value'] = uwwtd_check_text($data['uwwBOD5Perf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: BOD5 information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODPerf'])){
					$node->field_uwwcodperf['und'][0]['value'] = uwwtd_check_text($data['uwwCODPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: COD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwTSSPerf'])){
					$node->field_uwwtssperf['und'][0]['value'] = uwwtd_check_text($data['uwwTSSPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: TSS information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNTotPerf'])){
					$node->field_uwwntotperf['und'][0]['value'] = uwwtd_check_text($data['uwwNTotPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: N information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPTotPerf'])){
					$node->field_uwwptotperf['und'][0]['value'] = uwwtd_check_text($data['uwwPTotPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: P information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwOtherPerf'])){
					$node->field_uwwotherperf['und'][0]['value'] = uwwtd_check_text($data['uwwOtherPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: Others information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBadPerformance'])){
					if(uwwtd_check_bool($data['uwwBadPerformance'], $timestamp, $i, $node) !== false){
						$node->field_uwwbadperfdesign['und'][0]['value'] = uwwtd_check_bool($data['uwwBadPerformance'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs bad performance information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwAccidents'])){
					if(uwwtd_check_bool($data['uwwAccidents'], $timestamp, $i, $node) !== false){
						$node->field_uwwaccidents['und'][0]['value'] = uwwtd_check_bool($data['uwwAccidents'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs accidents information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBadDesign'])){
					if(uwwtd_check_bool($data['uwwBadDesign'], $timestamp, $i, $node) !== false){
						$node->field_uwwbaddesign['und'][0]['value'] = uwwtd_check_bool($data['uwwBadDesign'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs bad design or dimensioning information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwInformation'])){
					$node->field_uwwinformation['und'][0]['value'] = uwwtd_check_text($data['uwwInformation']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODIncomingMeasured'])){
					if( uwwtd_check_chiffre($data['uwwBODIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwbodincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads BOD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwBODIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwbodincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads BOD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwBODIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwbodincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads BOD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['uwwCODIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwcodincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads COD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwCODIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcodincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads COD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwCODIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcodincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads COD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['uwwNIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwnincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads Nto information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwNIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwnincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwNIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwnincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['uwwPIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwpincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads Ptot information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwPIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwpincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads P-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwPIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwpincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwBODDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwboddischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads BOD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODDischargeCalculated'])){
					if( uwwtd_check_chiffre($data['uwwBODDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwboddischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads BOD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwBODDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwboddischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads BOD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwCODDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwcoddischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads COD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODDischargeCalculated'])){
					if( uwwtd_check_chiffre($data['uwwCODDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcoddischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads COD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwCODDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcoddischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads COD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwNDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwndischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads Ntot information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNDischargeCalculated'])){
					if(uwwtd_check_chiffre($data['uwwNDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwndischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwNDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwndischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwPDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwpdischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads Ptot information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPDischargeCalculated'])){
					if(uwwtd_check_chiffre($data['uwwPDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwpdischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads Ptot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwPDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwpdischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwWasteWaterTreated'])){
					if(uwwtd_check_chiffre($data['uwwWasteWaterTreated'], $timestamp, $i, $node) !== false){
						$node->field_uwwwastewatertreated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwWasteWaterTreated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs waste water treated  information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwMethodWasteWaterTreated'])){
					$node->field_uwwmethodwastewatertreated['und'][0]['value'] = uwwtd_check_text($data['uwwMethodWasteWaterTreated']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs method used to determine the volume of waste water treated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwRemarks'])){
					$node->field_uwwremarks['und'][0]['value'] = uwwtd_check_text($data['uwwRemarks']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs remarks information found.', $timestamp, $i, $node);

				// save node
				node_save($node);
				$_SESSION['upUww']++;

			}
			else{

				// create the node
				$node = new StdClass();
				$node->type = 'uwwtp';
				// save once to get nid
				node_save($node);
				
				// site mapping
				// calculate treatment type
				$treatment = 'NI';
				if(isset($data['uwwPrimaryTreatment']) && uwwtd_check_bool($data['uwwPrimaryTreatment']) == 1) $treatment = 'P';
				if(isset($data['uwwSecondaryTreatment']) && uwwtd_check_bool($data['uwwSecondaryTreatment']) == 1) $treatment = 'S';
				if(isset($data['uwwNRemoval']) && uwwtd_check_bool($data['uwwNRemoval']) == 1) $treatment = 'MS';
				if(isset($data['uwwPRemoval']) && uwwtd_check_bool($data['uwwPRemoval']) == 1) $treatment = 'MS';
				if(isset($data['uwwUV']) && uwwtd_check_bool($data['uwwUV']) == 1) $treatment = 'MS';
				if(isset($data['uwwChlorination']) && uwwtd_check_bool($data['uwwChlorination']) == 1) $treatment = 'MS';
				if(isset($data['uwwOzonation']) && uwwtd_check_bool($data['uwwOzonation']) == 1) $treatment = 'MS';
				if(isset($data['uwwSandFiltration']) && uwwtd_check_bool($data['uwwSandFiltration']) == 1) $treatment = 'MS';
				if(isset($data['uwwMicroFiltration']) && uwwtd_check_bool($data['uwwMicroFiltration']) == 1) $treatment = 'MS';
				if(isset($data['uwwOtherTreatment']) && uwwtd_check_bool($data['uwwOtherTreatment']) == 1) $treatment = 'MS';
				$node->field_uwwtreatmenttype['und'][0]['value'] = $treatment;

				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				//$node->field_uwwcompliance['und'][0]['value'] = 'NI';
				$geoData = uwwtd_coord_transform(uwwtd_check_coordinate($data['aggLatitude'], 'latitude', $timestamp, $i, $node), uwwtd_check_coordinate($data['aggLongitude'], 'longitude', $timestamp, $i, $node));if($geoData != false){
					$node->field_position_geo['und'][0] = array(
						'geom'=>$geoData['bin'],
						'geo_type'=>$geoData['point'],
						'lat'=>$geoData['geoy'],
						'lon'=>$geoData['geox'],
						'left'=>$geoData['geoy'],
						'top'=>$geoData['geox'],
						'bottom'=>$geoData['geoy'],
						'geohash'=>$geoData['geohash']
					);
				}
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				//unset($node->field_uwwaggliste['und']);
				//unset($node->field_uwwdcpliste['und']);

				
				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$repcode = str_replace('_', '-', $datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$boom = explode('-', $repcode);
					$boom = explode(' ', $boom[0]);
					$rep = $boom[0];

					if($rep == 'LT'){
						$node->field_regionnuts['und'][0]['value'] = 'LT00';
					}

					if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
					    $node->field_regionnuts['und'][0]['value'] = 'PL112';
					}
				}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);

				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['uwwCode']);
				
				$node->title = uwwtd_check_text($data['uwwName']);
				
				if(isset($data['uwwState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['uwwState']);
				}else uwwtd_add_error(1, 0, 'No UWWTP status information found.', $timestamp, $i, $node);

				if(isset($data['aggID'])){
					$node->field_uwwtdaggid['und'][0]['value'] = uwwtd_check_text($data['aggID']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs ID of agglomeration information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCollectingSystem']))$node->field_uwwcollectingsystem['und'][0]['value'] = uwwtd_check_text($data['uwwCollectingSystem']);
				
				if(isset($data['uwwDateClosing'])){
					$node->field_validto['und'][0]['value'] = uwwtd_check_text($data['uwwDateClosing']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs date of closing information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwHistorie'])){
					$node->field_uwwhistorie['und'][0]['value'] = uwwtd_check_text($data['uwwHistorie']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs information found.', $timestamp, $i, $node);
				
				if($rep == 'LT'){
					$node->field_regionnuts['und'][0]['value'] = 'LT00';
				}
				$node->field_uwwlatitude['und'][0]['value'] = uwwtd_check_coordinate($data['uwwLatitude']);
				
				$node->field_uwwlongitude['und'][0]['value'] = uwwtd_check_coordinate($data['uwwLongitude']);
				
				if(isset($data['uwwNUTS']))$node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['uwwNUTS']);
				
				if(isset($data['uwwLoadEnteringUWWTP'])){
					if(uwwtd_check_int($data['uwwLoadEnteringUWWTP'], $timestamp, $i, $node) !== false){
						$node->field_uwwloadenteringuwwtp['und'][0]['value'] = uwwtd_check_int($data['uwwLoadEnteringUWWTP'], $timestamp, $i, $node);
					}	
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs load entering found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCapacity'])){
					if(uwwtd_check_int($data['uwwCapacity'], $i, $node) !== false){
						$node->field_physicalcapacityactivity['und'][0]['value'] = uwwtd_check_int($data['uwwCapacity'], $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs physical capacity found.', $timestamp, $i, $node);
					
				if(isset($data['uwwPrimaryTreatment'])){
					if(uwwtd_check_bool($data['uwwPrimaryTreatment'], $timestamp, $i, $node) !== false){
						$node->field_uwwprimarytreatment['und'][0]['value'] = uwwtd_check_bool($data['uwwPrimaryTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs Primary treatment information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwSecondaryTreatment'])){
					if(uwwtd_check_bool($data['uwwSecondaryTreatment'], $timestamp, $i, $node) !== false){
						$node->field_uwwsecondarytreatment['und'][0]['value'] = uwwtd_check_bool($data['uwwSecondaryTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs Secondary treatment information found.', $timestamp, $i, $node);
					
				if(isset($data['uwwOtherTreatment'])){
					if(uwwtd_check_bool($data['uwwOtherTreatment'], $timestamp, $i, $node) !== false){
						$node->field_uwwothertreat['und'][0]['value'] = uwwtd_check_bool($data['uwwOtherTreatment'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs other treatment found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNRemoval'])){
					if(uwwtd_check_bool($data['uwwNRemoval'], $timestamp, $i, $node) !== false){
						$node->field_uwwnremoval['und'][0]['value'] = uwwtd_check_bool($data['uwwNRemoval'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs N removal information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPRemoval'])){
					if(uwwtd_check_bool($data['uwwPRemoval'], $timestamp, $i, $node) !== false){
						$node->field_uwwpremoval['und'][0]['value'] = uwwtd_check_bool($data['uwwPRemoval'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs P removal information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwUV'])){
					if(uwwtd_check_bool($data['uwwUV'], $timestamp, $i, $node) !== false){
						$node->field_uwwuv['und'][0]['value'] = uwwtd_check_bool($data['uwwUV'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs UV information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwChlorination'])){
					if(uwwtd_check_bool($data['uwwChlorination'], $timestamp, $i, $node) !== false){
						$node->field_uwwchlorination['und'][0]['value'] = uwwtd_check_bool($data['uwwChlorination'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs chlorination information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwOzonation'])){
					if(uwwtd_check_bool($data['uwwOzonation'], $timestamp, $i, $node) !== false){
						$node->field_uwwozonation['und'][0]['value'] = uwwtd_check_bool($data['uwwOzonation'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs ozonation information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwSandFiltration'])){
					if(uwwtd_check_bool($data['uwwSandFiltration'], $timestamp, $i, $node) !==false){
						$node->field_uwwsandfiltration['und'][0]['value'] = uwwtd_check_bool($data['uwwSandFiltration'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs sand filtration information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwMicroFiltration'])){
					if(uwwtd_check_bool($data['uwwMicroFiltration'], $timestamp, $i, $node) !== false){
						$node->field_uwwmicrofiltration['und'][0]['value'] = uwwtd_check_bool($data['uwwMicroFiltration'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs micro filtration information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwOther'])){
					if(uwwtd_check_bool($data['uwwOther'], $timestamp, $i, $node) !== false){
						$node->field_uwwother['und'][0]['value'] = uwwtd_check_bool($data['uwwOther'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs other type information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwSpecification'])){
					$node->field_uwwspecification['und'][0]['value'] = uwwtd_check_text($data['uwwSpecification']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs specify other type of more stringent treatment information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBOD5Perf'])){
					$node->field_uwwbod5perf['und'][0]['value'] = uwwtd_check_text($data['uwwBOD5Perf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: BOD5 information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODPerf'])){
					$node->field_uwwcodperf['und'][0]['value'] = uwwtd_check_text($data['uwwCODPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: COD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwTSSPerf'])){
					$node->field_uwwtssperf['und'][0]['value'] = uwwtd_check_text($data['uwwTSSPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: TSS information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNTotPerf'])){
					$node->field_uwwntotperf['und'][0]['value'] = uwwtd_check_text($data['uwwNTotPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: N information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPTotPerf'])){
					$node->field_uwwptotperf['und'][0]['value'] = uwwtd_check_text($data['uwwPTotPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: P information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwOtherPerf'])){
					$node->field_uwwotherperf['und'][0]['value'] = uwwtd_check_text($data['uwwOtherPerf']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs treatment performance: Others information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBadPerformance'])){
					if(uwwtd_check_bool($data['uwwBadPerformance'], $timestamp, $i, $node) !== false){
						$node->field_uwwbadperfdesign['und'][0]['value'] = uwwtd_check_bool($data['uwwBadPerformance'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs bad performance information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwAccidents'])){
					if(uwwtd_check_bool($data['uwwAccidents'], $timestamp, $i, $node) !== false){
						$node->field_uwwaccidents['und'][0]['value'] = uwwtd_check_bool($data['uwwAccidents'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs accidents information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBadDesign'])){
					if(uwwtd_check_bool($data['uwwBadDesign'], $timestamp, $i, $node) !== false){
						$node->field_uwwbaddesign['und'][0]['value'] = uwwtd_check_bool($data['uwwBadDesign'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs bad design or dimensioning information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwInformation'])){
					$node->field_uwwinformation['und'][0]['value'] = uwwtd_check_text($data['uwwInformation']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODIncomingMeasured'])){
					if( uwwtd_check_chiffre($data['uwwBODIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwbodincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads BOD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwBODIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwbodincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads BOD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwBODIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwbodincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads BOD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['uwwCODIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwcodincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads COD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwCODIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcodincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads COD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwCODIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcodincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads COD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['uwwNIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwnincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads Nto information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwNIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwnincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwNIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->uwwnincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPIncomingMeasured'])){
					if(uwwtd_check_chiffre($data['uwwPIncomingMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwpincoming['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPIncomingMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads Ptot information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPIncomingCalculated'])){
					if( uwwtd_check_chiffre($data['uwwPIncomingCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwpincomingcalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPIncomingCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads P-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPIncomingEstimated'])){
					if(uwwtd_check_chiffre($data['uwwPIncomingEstimated'], $timestamp, $i, $node) !== false){
						$node->uwwpincomingestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPIncomingEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs incoming loads P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwBODDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwboddischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads BOD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODDischargeCalculated'])){
					if( uwwtd_check_chiffre($data['uwwBODDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwboddischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads BOD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwBODDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwBODDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwboddischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwBODDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads BOD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwCODDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwcoddischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads COD information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODDischargeCalculated'])){
					if( uwwtd_check_chiffre($data['uwwCODDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcoddischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads COD-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwCODDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwCODDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwcoddischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwCODDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads COD-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwNDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwndischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads Ntot information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNDischargeCalculated'])){
					if(uwwtd_check_chiffre($data['uwwNDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwndischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads N-tot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwNDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwNDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwndischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwNDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads N-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPDischargeMeasured'])){
					if(uwwtd_check_chiffre($data['uwwPDischargeMeasured'], $timestamp, $i, $node) !== false){
						$node->field_uwwpdischarge['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPDischargeMeasured'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads Ptot information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPDischargeCalculated'])){
					if(uwwtd_check_chiffre($data['uwwPDischargeCalculated'], $timestamp, $i, $node) !== false){
						$node->field_uwwpdischargecalculated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPDischargeCalculated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads Ptot calculated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwPDischargeEstimated'])){
					if( uwwtd_check_chiffre($data['uwwPDischargeEstimated'], $timestamp, $i, $node) !== false){
						$node->field_uwwpdischargeestimated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwPDischargeEstimated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs discharged loads P-tot estimated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwWasteWaterTreated'])){
					if(uwwtd_check_chiffre($data['uwwWasteWaterTreated'], $timestamp, $i, $node) !== false){
						$node->field_uwwwastewatertreated['und'][0]['value'] = uwwtd_check_chiffre($data['uwwWasteWaterTreated'], $timestamp, $i, $node);
					}
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs waste water treated  information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwMethodWasteWaterTreated'])){
					$node->field_uwwmethodwastewatertreated['und'][0]['value'] = uwwtd_check_text($data['uwwMethodWasteWaterTreated']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs method used to determine the volume of waste water treated information found.', $timestamp, $i, $node);
				
				if(isset($data['uwwRemarks'])){
					$node->field_uwwremarks['und'][0]['value'] = uwwtd_check_text($data['uwwRemarks']);
				}
				else uwwtd_add_error(1, 0, 'No uwwTPs remarks information found.', $timestamp, $i, $node);

				
				// save node
				node_save($node);
				//dsm($node);
				$_SESSION['newUww']++;

			}
		
		}

		$context['sandbox']['progress']++;
		$context['message'] = t('Processing waste water treatment plant').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
		$context['results'][] = $node->nid . ' : ' . check_plain($node->title);
		
		if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
			$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
		}
	}else{
		uwwtd_add_error(1, 0, 'No UWWTPs found in the dataset', $timestamp);
	}
}

function uwwtd_import_dcp_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['DischargePoints'])){
		if (!isset($context['sandbox']['progress'])) {
		
			//Get all discharge points from data
			$imports = $datas['UWWTD_Report']['DischargePoints']['DischargePoint'];

			$context['sandbox']['max'] = count($imports);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $imports;
		}
		$i = $context['sandbox']['progress'];
		$data = $context['sandbox']['ids'][$i];

		// Check basic elements exist
		// errors
		$e = 0;
		if(!isset($data['dcpName']) || empty($data['dcpName'])){
			$e = 1;
			$emes = t('The discharge point does not have \'title\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		if(!isset($data['dcpCode']) || empty($data['dcpCode'])){
			$e = 1;
			$emes = t('The discharge point does not have \'identification\' information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		if(!isset($data['dcpLatitude']) || empty($data['dcpLatitude'])){
			$e = 1;
			$emes = t('The discharge point does not have latitude information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		elseif(uwwtd_check_coordinate($data['dcpLatitude'], 'latitude', $timestamp, $i) === false){
			$e = 1;
			$emes = t('The latitude for this discharge point is invalid');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		if(!isset($data['dcpLongitude']) || empty($data['dcpLongitude'])){
			// if check_coord == false e=1
			$e = 1;
			$emes = t('The discharge point does not have longitude information');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}
		elseif(uwwtd_check_coordinate($data['dcpLongitude'], 'longitude', $timestamp, $i) === false){
			$e = 1;
			$emes = t('The longitude for this discharge point is invalid');
			uwwtd_add_error(2, 0, $emes, $timestamp, $i);
		}

		// Check if no errors were found
		if($e == 0){

			// Variables
			$uri = $newFile->uri;
			$filename = $newFile->filename;
			$filename .= ".xml";
			$fid = $newFile->fid;
			$siteId = $annee.'_dcp_'.uwwtd_check_text($data['dcpCode']);

			// Check if id exists
			$exists = uwwtd_check_exist($siteId);

			// If the element exists
			if($exists != false){

				// load the node
				$node = node_load($exists);
				$node->type = 'discharge_point';

				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$geoData = uwwtd_coord_transform(uwwtd_check_coordinate($data['dcpLatitude'], 'latitude', $timestamp, $i, $node), uwwtd_check_coordinate($data['dcpLongitude'], 'longitude', $timestamp, $i, $node));
				if($geoData != false){
					$node->field_position_geo['und'][0] = array(
						'geom'=>$geoData['bin'],
						'geo_type'=>$geoData['point'],
						'lat'=>$geoData['geoy'],
						'lon'=>$geoData['geox'],
						'left'=>$geoData['geoy'],
						'top'=>$geoData['geox'],
						'bottom'=>$geoData['geoy'],
						'geohash'=>$geoData['geohash']
					);
				}
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_dcprcaliste['und']);
				unset($node->field_dcpuwwliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$repcode = str_replace('_', '-', $datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$boom = explode('-', $repcode);
					$boom = explode(' ', $boom[0]);
					$rep = $boom[0];

					if($rep == 'LT'){
						$node->field_regionnuts['und'][0]['value'] = 'LT00';
					}

					if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
					    $node->field_regionnuts['und'][0]['value'] = 'PL112';
					}
				}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);
				
				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['dcpCode']);

				$node->title = uwwtd_check_text($data['dcpName']);

				if(isset($data['dcpState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['dcpState']);}
				else uwwtd_add_error(1, 0, 'No discharge point status information found.', $timestamp, $i, $node);

				if(isset($data['uwwID'])){
					$node->field_utilityfacilityreference['und'][0]['value'] = uwwtd_check_text($data['uwwID']);}
				else uwwtd_add_error(1, 0, 'No discharge point waste water treatment plant connexion information found.', $timestamp, $i, $node);

				if(isset($data['dcpNUTS'])){
					$node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['dcpNUTS']);}
				else uwwtd_add_error(1, 0, 'No discharge point nut code information found.', $timestamp, $i, $node);

				$node->field_dcplatitude['und'][0]['value'] = uwwtd_check_coordinate($data['dcpLatitude'], 'latitude', $timestamp, $i);
				
				$node->field_dcplongitude['und'][0]['value'] = uwwtd_check_coordinate($data['dcpLongitude'], 'longitude', $timestamp, $i);

				if(isset($data['dcpWaterBodyType'])){
					$node->field_dcpwaterbodytype['und'][0]['value'] = uwwtd_check_text($data['dcpWaterBodyType']);}
				else uwwtd_add_error(1, 0, 'No discharge point water body type information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpIrrigation'])){
					$node->field_dcpirrigation['und'][0]['value'] = uwwtd_check_text($data['dcpIrrigation']);}
				else uwwtd_add_error(1, 0, 'No discharge point no discharge point en case of discharge on land please specify the purpose type information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpTypeOfReceivingArea'])){
					$node->field_rcatype['und'][0]['value'] = uwwtd_check_text($data['dcpTypeOfReceivingArea']);}
				else uwwtd_add_error(1, 0, 'No discharge point type of receiving area information found.', $timestamp, $i, $node);

				if(isset($data['rcaID'])){
					$node->field_rcaid['und'][0]['value'] = uwwtd_check_text($data['rcaID']);}
				else uwwtd_add_error(1, 0, 'No discharge point receiving area connexion information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpSurfaceWaters'])){
					$node->field_dcpsurfacewaters['und'][0]['value'] = uwwtd_check_text($data['dcpSurfaceWaters']);}
				else uwwtd_add_error(1, 0, 'No discharge point surface waters information found.', $timestamp, $i, $node);

				if(isset($data['dcpWaterbodyID'])){
					$node->field_dcpwaterbodyid['und'][0]['value'] = uwwtd_check_text($data['dcpWaterbodyID']);}
				else uwwtd_add_error(1, 0, 'No discharge point water body connexion information found.', $timestamp, $i, $node);

				if(isset($data['dcpNotAffect'])){
					$node->field_dcpnotaffect['und'][0]['value'] = uwwtd_check_text($data['dcpNotAffect']);}
				else uwwtd_add_error(1, 0, 'No discharge point information  do comprehensive studies indicate that the discharge does not adversely affect the environment found.', $timestamp, $i, $node);

				if(isset($data['dcpMSProvide'])){
					$node->field_dcpmsprovide['und'][0]['value'] = uwwtd_check_text($data['dcpMSProvide']);}
				else uwwtd_add_error(1, 0, 'No discharge point has the Member State provided these studies to the Commission information found.', $timestamp, $i, $node);

				if(isset($data['dcpCOMAccept'])){
					$node->field_ddcpcomaccept['und'][0]['value'] = uwwtd_check_text($data['dcpCOMAccept']);}
				else uwwtd_add_error(1, 0, 'No discharge point Has the Commission formally accepts that the conditions of art. 6 (2) are met information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpGroundWater'])){
					$node->field_dcpgroundwater['und'][0]['value'] = uwwtd_check_text($data['dcpGroundWater']);}
				else uwwtd_add_error(1, 0, 'No discharge point ID of WFD groundwater body type information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpReceivingWater'])){
					$node->field_dcpreceivingwater['und'][0]['value'] = uwwtd_check_text($data['dcpReceivingWater']);}
				else uwwtd_add_error(1, 0, 'No discharge point water body connexion information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWFDSubUnit'])){
					$node->field_dcpwfdsubunit['und'][0]['value'] = uwwtd_check_text($data['dcpWFDSubUnit']);}
				else uwwtd_add_error(1, 0, 'No discharge point ID of WFD sub-unit information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWFDRBD'])){
					$node->field_dcpwfdrbd['und'][0]['value'] = uwwtd_check_text($data['dcpWFDRBD']);}
				else uwwtd_add_error(1, 0, 'No discharge point WFD river basin district connexion information found.', $timestamp, $i, $node);

				if(isset($data['dcpRemarks'])){
					$node->field_dcpremarks['und'][0]['value'] = uwwtd_check_text($data['dcpRemarks']);}
				else uwwtd_add_error(1, 0, 'No discharge point remarks information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWaterBodyReferenceDate'])){
					if(uwwtd_check_date($data['dcpWaterBodyReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpwaterbodyreferencedate['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpWaterBodyReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point water body reference date information found.', $timestamp, $i, $node);

				if(isset($data['dcpGroundWaterReferenceDate'])){
					if(uwwtd_check_date($data['dcpGroundWaterReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpdcpgroundwaterreference['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpGroundWaterReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point ground water reference date information found.', $timestamp, $i, $node);

				if(isset($data['dcpReceivingWaterReferenceDate'])){
					if(uwwtd_check_date($data['dcpReceivingWaterReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpreceivingwaterreference['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpReceivingWaterReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point receiving water reference date information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWFDSubUnitReferenceDate'])){
					if(uwwtd_check_date($data['dcpWFDSubUnitReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcp_wfdsubunitrefdate['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpWFDSubUnitReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point WFD sub unit reference date information found.', $timestamp, $i, $node);

				if(isset($data['dcpWFDRBDReferenceDate'])){
					if(uwwtd_check_date($data['dcpWFDRBDReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpwfdrbdreferencedate['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpWFDRBDReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point WFD reference date information found.', $timestamp, $i, $node);

				// save node
				node_save($node);
				$_SESSION['upDcp']++;

			}
			else{

				// create the node
				$node = new StdClass();
				$node->type = 'discharge_point';
				node_save($node);
				
				// site mapping
				$node->field_siteid['und'][0]['value'] = $siteId;
				$node->field_valid['und'][0]['value'] = 0;
				$node->field_anneedata['und'][0]['value'] = $annee;
				$geoData = uwwtd_coord_transform(uwwtd_check_coordinate($data['dcpLatitude'], 'latitude', $timestamp, $i, $node), uwwtd_check_coordinate($data['dcpLongitude'], 'longitude', $timestamp, $i, $node));
				if($geoData != false){
					$node->field_position_geo['und'][0] = array(
						'geom'=>$geoData['bin'],
						'geo_type'=>$geoData['point'],
						'lat'=>$geoData['geoy'],
						'lon'=>$geoData['geox'],
						'left'=>$geoData['geoy'],
						'top'=>$geoData['geox'],
						'bottom'=>$geoData['geoy'],
						'geohash'=>$geoData['geohash']
					);
				}
				$node->field_sourcefile = array(
					'und' => array(
						0 => array(
							'fid' => trim($fid),
							'filename' => trim($filename),
							'uri' => trim($uri),
							'status' => 1,
							'display' => 1
						)
					)
				);
				unset($node->field_dcprcaliste['und']);
				unset($node->field_dcpuwwliste['und']);

				// data mapping
				if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
					$node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$repcode = str_replace('_', '-', $datas['UWWTD_Report']['ReportPeriod']['repCode']);
					$boom = explode('-', $repcode);
					$boom = explode(' ', $boom[0]);
					$rep = $boom[0];

					if($rep == 'LT'){
						$node->field_regionnuts['und'][0]['value'] = 'LT00';
					}

					if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
					    $node->field_regionnuts['und'][0]['value'] = 'PL112';
					}
				}
				else uwwtd_add_error(1, 0, 'No repCode found for this dataset.', $timestamp, $i, $node);
				
				$node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['dcpCode']);

				$node->title = uwwtd_check_text($data['dcpName']);

				if(isset($data['dcpState'])){
					$node->field_status['und'][0]['value'] = uwwtd_check_text($data['dcpState']);}
				else uwwtd_add_error(1, 0, 'No discharge point status information found.', $timestamp, $i, $node);

				if(isset($data['uwwID'])){
					$node->field_utilityfacilityreference['und'][0]['value'] = uwwtd_check_text($data['uwwID']);}
				else uwwtd_add_error(1, 0, 'No discharge point waste water treatment plant connexion information found.', $timestamp, $i, $node);

				if(isset($data['dcpNUTS'])){
					$node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['dcpNUTS']);}
				else uwwtd_add_error(1, 0, 'No discharge point nut code information found.', $timestamp, $i, $node);

				$node->field_dcplatitude['und'][0]['value'] = uwwtd_check_coordinate($data['dcpLatitude'], 'latitude', $timestamp, $i);
				
				$node->field_dcplongitude['und'][0]['value'] = uwwtd_check_coordinate($data['dcpLongitude'], 'longitude', $timestamp, $i);

				if(isset($data['dcpWaterBodyType'])){
					$node->field_dcpwaterbodytype['und'][0]['value'] = uwwtd_check_text($data['dcpWaterBodyType']);}
				else uwwtd_add_error(1, 0, 'No discharge point water body type information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpIrrigation'])){
					$node->field_dcpirrigation['und'][0]['value'] = uwwtd_check_text($data['dcpIrrigation']);}
				else uwwtd_add_error(1, 0, 'No discharge point no discharge point en case of discharge on land please specify the purpose type information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpTypeOfReceivingArea'])){
					$node->field_rcatype['und'][0]['value'] = uwwtd_check_text($data['dcpTypeOfReceivingArea']);}
				else uwwtd_add_error(1, 0, 'No discharge point type of receiving area information found.', $timestamp, $i, $node);

				if(isset($data['rcaID'])){
					$node->field_rcaid['und'][0]['value'] = uwwtd_check_text($data['rcaID']);}
				else uwwtd_add_error(1, 0, 'No discharge point receiving area connexion information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpSurfaceWaters'])){
					$node->field_dcpsurfacewaters['und'][0]['value'] = uwwtd_check_text($data['dcpSurfaceWaters']);}
				else uwwtd_add_error(1, 0, 'No discharge point surface waters information found.', $timestamp, $i, $node);

				if(isset($data['dcpWaterbodyID'])){
					$node->field_dcpwaterbodyid['und'][0]['value'] = uwwtd_check_text($data['dcpWaterbodyID']);}
				else uwwtd_add_error(1, 0, 'No discharge point water body connexion information found.', $timestamp, $i, $node);

				if(isset($data['dcpNotAffect'])){
					$node->field_dcpnotaffect['und'][0]['value'] = uwwtd_check_text($data['dcpNotAffect']);}
				else uwwtd_add_error(1, 0, 'No discharge point information  do comprehensive studies indicate that the discharge does not adversely affect the environment found.', $timestamp, $i, $node);

				if(isset($data['dcpMSProvide'])){
					$node->field_dcpmsprovide['und'][0]['value'] = uwwtd_check_text($data['dcpMSProvide']);}
				else uwwtd_add_error(1, 0, 'No discharge point has the Member State provided these studies to the Commission information found.', $timestamp, $i, $node);

				if(isset($data['dcpCOMAccept'])){
					$node->field_ddcpcomaccept['und'][0]['value'] = uwwtd_check_text($data['dcpCOMAccept']);}
				else uwwtd_add_error(1, 0, 'No discharge point Has the Commission formally accepts that the conditions of art. 6 (2) are met information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpGroundWater'])){
					$node->field_dcpgroundwater['und'][0]['value'] = uwwtd_check_text($data['dcpGroundWater']);}
				else uwwtd_add_error(1, 0, 'No discharge point ID of WFD groundwater body type information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpReceivingWater'])){
					$node->field_dcpreceivingwater['und'][0]['value'] = uwwtd_check_text($data['dcpReceivingWater']);}
				else uwwtd_add_error(1, 0, 'No discharge point water body connexion information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWFDSubUnit'])){
					$node->field_dcpwfdsubunit['und'][0]['value'] = uwwtd_check_text($data['dcpWFDSubUnit']);}
				else uwwtd_add_error(1, 0, 'No discharge point ID of WFD sub-unit information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWFDRBD'])){
					$node->field_dcpwfdrbd['und'][0]['value'] = uwwtd_check_text($data['dcpWFDRBD']);}
				else uwwtd_add_error(1, 0, 'No discharge point WFD river basin district connexion information found.', $timestamp, $i, $node);

				if(isset($data['dcpRemarks'])){
					$node->field_dcpremarks['und'][0]['value'] = uwwtd_check_text($data['dcpRemarks']);}
				else uwwtd_add_error(1, 0, 'No discharge point remarks information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWaterBodyReferenceDate'])){
					if(uwwtd_check_date($data['dcpWaterBodyReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpwaterbodyreferencedate['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpWaterBodyReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point water body reference date information found.', $timestamp, $i, $node);

				if(isset($data['dcpGroundWaterReferenceDate'])){
					if(uwwtd_check_date($data['dcpGroundWaterReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpdcpgroundwaterreference['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpGroundWaterReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point ground water reference date information found.', $timestamp, $i, $node);

				if(isset($data['dcpReceivingWaterReferenceDate'])){
					if(uwwtd_check_date($data['dcpReceivingWaterReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpreceivingwaterreference['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpReceivingWaterReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point receiving water reference date information found.', $timestamp, $i, $node);
				
				if(isset($data['dcpWFDSubUnitReferenceDate'])){
					if(uwwtd_check_date($data['dcpWFDSubUnitReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcp_wfdsubunitrefdate['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpWFDSubUnitReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point WFD sub unit reference date information found.', $timestamp, $i, $node);

				if(isset($data['dcpWFDRBDReferenceDate'])){
					if(uwwtd_check_date($data['dcpWFDRBDReferenceDate'], $timestamp, $i, $node) !== false){
						$node->field_dcpwfdrbdreferencedate['und'][0] = array(
							'value'=>uwwtd_check_date($data['dcpWFDRBDReferenceDate'], $timestamp, $i, $node),
							'timezone'=>'Europe/Berlin',
							'timezone_bd'=>'Europe/Berlin',
							'date_type'=>'date'
						);
					}
				}
				else uwwtd_add_error(1, 0, 'No discharge point WFD reference date information found.', $timestamp, $i, $node);

				unset($node->field_dcprcaliste['und']);
				unset($node->field_dcpuwwliste['und']);

				
				// save node
				node_save($node);
				$_SESSION['newDcp']++;

			}
		
		}

		$context['sandbox']['progress']++;
		$context['message'] = t('Processing discharge point').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
		$context['results'][] = $node->nid . ' : ' . check_plain($node->title);
		
		if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
			$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
		}
	}else{
		uwwtd_add_error(1, 0, 'No Discharge points found in the dataset', $timestamp);
	}
}

function uwwtd_link_dcp_rca_uww_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['DischargePoints'])){
		if (!isset($context['sandbox']['progress'])) {
		
			$ids = array();
		
			//Get all discharge points for the year
			$query = db_select('node', 'n');
			$query->join('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
			$dcps = $query
				->fields('n', array('nid', 'title'))			
				->condition('n.type', 'discharge_point', '=')
				->condition('r.field_siteid_value', db_like($annee).'%', 'LIKE')
				->execute();
			foreach($dcps as $dcp){
				$ids[] = array('nid'=>$dcp->nid, 'title'=>$dcp->title);
			}
			
			$context['sandbox']['max'] = count($ids);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $ids;
		}

		$i = $context['sandbox']['progress'];
		$node = node_load($context['sandbox']['ids'][$i]['nid']);

		// Check and add dcp <-> rca
		if(isset($node->field_rcaid['und'][0]['value'])){
			$rcaExists = uwwtd_check_exist($annee.'_rca_'.uwwtd_check_text($node->field_rcaid['und'][0]['value']));
		
			if($rcaExists != false){
				$node->field_dcprcaliste['und'][]['nid'] = $rcaExists;
				$rca = node_load($rcaExists);
				$rca->field_rcadcpliste['und'][]['nid'] = $node->nid;
				node_save($rca);
			}
			else{
				$emes = t('Cannot connect the discharge point').' \''.$node->field_inspireidlocalid['und'][0]['value'].'\' '.t('with drupal id').' \''.$node->nid.'\' '.t('to the receiving area').' \''.$node->field_rcaid['und'][0]['value'].'\' '.t('because the receiving area does not exist.');
				uwwtd_add_error(2, 0, $emes, $timestamp, $i);
			}
		}

		// Check and add dcp <-> uww
		if(isset($node->field_utilityfacilityreference['und'][0]['value'])){
			$uwwExists = uwwtd_check_exist($annee.'_uww_'.uwwtd_check_text($node->field_utilityfacilityreference['und'][0]['value']));
			if($uwwExists != false){
				$node->field_dcpuwwliste['und'][]['nid'] = $uwwExists;
				$uww = node_load($uwwExists);
				$uww->field_uwwdcpliste['und'][]['nid'] = $node->nid;
				node_save($uww);
			}
			else{
				$emes = t('Cannot connect the discharge point').' \''.$node->field_inspireidlocalid['und'][0]['value'].'\' '.t('with drupal id').' \''.$node->nid.'\' '.t('to the waste water treatment plant').' \''.$node->field_utilityfacilityreference['und'][0]['value'].'\' '.t('because the waste water treatment plant does not exist.');
				uwwtd_add_error(2, 0, $emes, $timestamp, $i);
			}
		}

		node_save($node);

		$context['sandbox']['progress']++;
		$context['message'] = t('Processing links from discharge point').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
		$context['results'][] = $node->nid . ' : ' . check_plain($node->title);
		
		if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
			$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
		}
	}else{
		uwwtd_add_error(1, 1, 'No link because no found discharge points in the dataset', $timestamp);
	}
}

function uwwtd_link_uww_agglo_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['Agglomerations'])){
		if (!isset($context['sandbox']['progress'])) {
		
			//Get all links from uwwtps to agglos from data
			$imports = $datas['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'];

			$context['sandbox']['max'] = count($imports);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $imports;
		}
		$i = $context['sandbox']['progress'];
		$data = $context['sandbox']['ids'][$i];

		// Get data uww id and agg id
		$uwwId = $data['aucUwwCode'];
		$aggId = $data['aucAggCode'];

		$siteUwwId = $annee.'_uww_'.$data['aucUwwCode'];
		$siteAggId = $annee.'_agg_'.$data['aucAggCode'];

		// Check if both exist
		$uwwExists = uwwtd_check_exist($siteUwwId);
		$aggExists = uwwtd_check_exist($siteAggId);

		//dsm($i.': '.$siteUwwId.'('.$uwwExists.')'.', '.$siteAggId.'('.$aggExists.')');

		// If both exist
		if($uwwExists != false && $aggExists != false){
			// add agg id to uww
			$uww = node_load($uwwExists);
			$uww->field_uwwaggliste['und'][]['nid'] = $aggExists;
			node_save($uww);

			// add uww id to agg
			$agg = node_load($aggExists);
			$agg->field_agguwwliste['und'][]['nid'] = $uwwExists;
			node_save($agg);

			// Create linking node with all the data needed for compliance calculation later
			$agglouww = new StdClass();
			$agglouww->type = 'agglo_uww';
			$agglouww->title = $annee.' data '.$agg->title.' - '.$uww->title;
			$agglouww->field_agglo_uww_agglo['und'][0]['nid'] = $agg->nid;
			$agglouww->field_agglo_uww_uww['und'][0]['nid'] = $uww->nid;
			$agglouww->field_agglo_uww_perc_ent_uw['und'][0]['value'] = uwwtd_check_text($data['aucPercEnteringUWWTP']);
			$agglouww->field_agglo_uww_mperc_ent_uw['und'][0]['value'] = uwwtd_check_text($data['aucMethodPercEnteringUWWTP']);
			$agglouww->field_anneedata['und'][0]['value'] = $annee;
			node_save($agglouww);
		}
		else{
		uwwtd_add_error(1, 1, 'No link between agglomerations and waste water treatment plant ', $timestamp, $i);
		}


		$context['sandbox']['progress']++;
		$context['message'] = t('Processing link between agglomeration').' : '.'\''.$aggId.'\''.' '.t('and waste water treatment plant').' '.'\''.$uwwId.'\'';
		$context['results'][] = $agg->nid . ' : ' . check_plain($node->title);
		
		if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
			$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
		}
	}else{
		uwwtd_add_error(1, 1, 'No link between agglomerations and waste water treatment plant because no found agglomerations in the dataset', $timestamp);
	}
}

function uwwtd_link_bc_agglo_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if(isset($datas['UWWTD_Report']['BigCityDischargers'])){
		if(!isset($context['sandbox']['progress'])){
			$ids = array();
			
			//Get all the agglomerations
			$query = db_select('node', 'n');
			$query->join('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
			$query->join('field_data_field_aggbigid', 'b', 'b.entity_id = n.nid');
			$aggs = $query
				->fields('n', array('nid', 'title'))
				->fields('b', array('field_aggbigid_value'))			
				->condition('n.type', 'agglomeration', '=')
				->condition('r.field_siteid_value', db_like($annee).'%', 'LIKE')
				->execute();
			foreach($aggs as $agg){
				$ids[] = array('nid'=>$agg->nid, 'title'=>$agg->title);
			}
			
			$context['sandbox']['max'] = count($ids);
			$context['sandbox']['progress'] = 0;
			$context['sandbox']['ids'] = $ids;
		}
		if($context['sandbox']['max'] === 0){
			uwwtd_add_error(2, 1, 'No link between agglomerations and big cities because no found <bigID> in the dataset', $timestamp);
	
		}else{
			$i = $context['sandbox']['progress'];
			$node = node_load($context['sandbox']['ids'][$i]['nid']);
			// Check bcId exists
			if(isset($node->field_aggbigid['und'][0]['value'])){
				$exists = uwwtd_check_exist($annee.'_big_'.uwwtd_check_text($node->field_aggbigid['und'][0]['value']));
				
				if($exists != false){
					$node->field_aggbcliste['und'][]['nid'] = $exists;
					$bc = node_load($exists);
					$bc->field_bcaggliste['und'][]['nid'] = $node->nid;
					node_save($bc);
				}
				else{
					$emes = t('Cannot connect the big cities').' \''.$node->field_inspireidlocalid['und'][0]['value'].'\' '.t('with drupal id').' \''.$node->nid.'\' '.t('to the agglomerations').' \''.$node->field_rcaid['und'][0]['value'].'\' '.t('because the big cities does not exist.');
					uwwtd_add_error(2, 0, $emes, $timestamp, $i);
				}
			}
			//node_save($node);
		
			$context['sandbox']['progress']++;
			$context['message'] = t('Processing links from big cities').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
			$context['results'][] = $node->nid . ' : ' . check_plain($node->title);
			
			if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
				$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
			}
		}
	}else{
		uwwtd_add_error(1, 1, 'No link between agglomerations and big cities because no found big cities in the dataset', $timestamp);
	}
}

function uwwtd_bc_position_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if (!isset($context['sandbox']['progress'])) {
	
		$ids = array();

		//Get all the agglomerations
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
		$aggs = $query
			->fields('n', array('nid', 'title'))		
			->condition('n.type', 'big_city', '=')
			->condition('r.field_siteid_value', db_like($annee.'_big_').'%', 'LIKE')
			->execute();
		foreach($aggs as $agg){
			$ids[] = array('nid'=>$agg->nid, 'title'=>$agg->title);
		}


		$context['sandbox']['max'] = count($ids);
		$context['sandbox']['progress'] = 0;
		$context['sandbox']['ids'] = $ids;

	}

	if($context['sandbox']['max'] > 0){
		$i = $context['sandbox']['progress'];
		$data = $context['sandbox']['ids'][$i];
		$node = node_load($data['nid']);

		if(isset($node->field_bcaggliste['und'])){

			//variables
			$biggestId = '';
			$biggestPop = 0;

			// get the agglo with the biggest population
			foreach($node->field_bcaggliste['und'] as $aggloId){

				$agglo = node_load($aggloId['nid']);
				if($agglo->field_agggenerated['und'][0]['value'] > $biggestPop){
					$biggestPop = $agglo->field_agggenerated['und'][0]['value'];
					$biggestId = $agglo->nid;
				}

			}

			// get the wkd of the allgo with the biggest population
			$bigAgg = node_load($biggestId);
			$node->field_position_geo['und'][0] = array(
				'geom'=>$bigAgg->field_position_geo['und'][0]['geom'],
				'geo_type'=>$bigAgg->field_position_geo['und'][0]['geo_type'],
				'lat'=>$bigAgg->field_position_geo['und'][0]['lat'],
				'lon'=>$bigAgg->field_position_geo['und'][0]['lon'],
				'left'=>$bigAgg->field_position_geo['und'][0]['left'],
				'top'=>$bigAgg->field_position_geo['und'][0]['top'],
				'bottom'=>$bigAgg->field_position_geo['und'][0]['bottom'],
				'geohash'=>$bigAgg->field_position_geo['und'][0]['geohash']
			);
		}

		node_save($node);

		$context['sandbox']['progress']++;
		$context['message'] = t('Processing position of big city').' : '.'\''.$node->title.'\''.' '.t('with drupal id').' '.'\''.$node->nid.'\'';
		$context['results'][] = $agg->nid . ' : ' . check_plain($node->title);
	}

	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_mega_links_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if (!isset($context['sandbox']['progress'])) {
	
		$ids = array();

		//Get all the agglomerations
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
		$aggs = $query
			->fields('n', array('nid', 'title'))		
			->condition('n.type', 'discharge_point', '=')
			->condition('r.field_siteid_value', db_like($annee.'_dcp_').'%', 'LIKE')
			->execute();
		foreach($aggs as $agg){
			$ids[] = array('nid'=>$agg->nid, 'title'=>$agg->title);
		}


		$context['sandbox']['max'] = count($ids);
		$context['sandbox']['progress'] = 0;
		$context['sandbox']['ids'] = $ids;

	}
	$i = $context['sandbox']['progress'];
	$data = $context['sandbox']['ids'][$i];
	$dcpnid = $data['nid'];
	$dcp = node_load($dcpnid);

	$rcas = array();
	// creat list of rcas
	foreach($dcp->field_dcprcaliste['und'] as $item){
		$rcas[] = $item['nid'];
	}

	// need to save at each step
	// rcas
	foreach($rcas as $rcanid){
		// load rca
		$rca = node_load($rcanid);
		// dcp <-> rca
		if(!uwwtd_in_array_r($rcanid, $dcp->field_linked_receiving_areas)) $dcp->field_linked_receiving_areas['und'][]['nid'] = $rcanid;
		if(!uwwtd_in_array_r($dcpnid, $rca->field_linked_discharge_points)) $rca->field_linked_discharge_points['und'][]['nid'] = $dcpnid;

		// uwwtps
		foreach($dcp->field_dcpuwwliste['und'] as $uwwitem){
			$uwwnid = $uwwitem['nid'];
			$uww = node_load($uwwnid);

			// rca <-> uwwtp
			if(!uwwtd_in_array_r($uwwnid, $rca->field_linked_treatment_plants)) $rca->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
			if(!uwwtd_in_array_r($rcanid, $uww->field_linked_receiving_areas)) $uww->field_linked_receiving_areas['und'][]['nid'] = $rcanid;

			// dcp <-> uwwtp
			if(!uwwtd_in_array_r($uwwnid, $dcp->field_linked_treatment_plants)) $dcp->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
			if(!uwwtd_in_array_r($dcpnid, $uww->field_linked_discharge_points)) $uww->field_linked_discharge_points['und'][]['nid'] = $dcpnid;

			// agglomerations
			foreach($uww->field_uwwaggliste['und'] as $aggitem){
				$aggnid = $aggitem['nid'];
				$agg = node_load($aggnid);

				// uww <-> agg
				if(!uwwtd_in_array_r($aggnid, $uww->field_linked_agglomerations)) $uww->field_linked_agglomerations['und'][]['nid'] = $aggnid;
				if(!uwwtd_in_array_r($uwwnid, $agg->field_linked_treatment_plants)) $agg->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;

				// rca <-> agg
				if(!uwwtd_in_array_r($aggnid, $rca->field_linked_agglomerations)) $rca->field_linked_agglomerations['und'][]['nid'] = $aggnid;
				if(!uwwtd_in_array_r($rcanid, $agg->field_linked_receiving_areas)) $agg->field_linked_receiving_areas['und'][]['nid'] = $rcanid;

				// dcp <-> agg
				if(!uwwtd_in_array_r($aggnid, $dcp->field_linked_agglomerations)) $dcp->field_linked_agglomerations['und'][]['nid'] = $aggnid;
				if(!uwwtd_in_array_r($dcpnid, $agg->field_linked_discharge_points)) $agg->field_linked_discharge_points['und'][]['nid'] = $dcpnid;
				node_save($agg);
			}
			node_save($uww);
		}
		node_save($rca);
	}
	node_save($dcp);

	// Sometimes rcadata is not given or incorrectly inserted so we now go from the treatment plants
	$uwws = array();
	// creat list of uwws
	foreach($dcp->field_dcpuwwliste['und'] as $item){
		$uwws[] = $item['nid'];
	}
	foreach($uwws as $uwwnid){
		$uww = node_load($uwwnid);

		// dcp <-> uwwtp
		if(!uwwtd_in_array_r($uwwnid, $dcp->field_linked_treatment_plants)) $dcp->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
		if(!uwwtd_in_array_r($dcpnid, $uww->field_linked_discharge_points)) $uww->field_linked_discharge_points['und'][]['nid'] = $dcpnid;

		// agglomerations
		foreach($uww->field_uwwaggliste['und'] as $aggitem){
			$aggnid = $aggitem['nid'];
			$agg = node_load($aggnid);

			// uww <-> agg
			if(!uwwtd_in_array_r($aggnid, $uww->field_linked_agglomerations)) $uww->field_linked_agglomerations['und'][]['nid'] = $aggnid;
			if(!uwwtd_in_array_r($uwwnid, $agg->field_linked_treatment_plants)) $agg->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;

			// dcp <-> agg
			if(!uwwtd_in_array_r($aggnid, $dcp->field_linked_agglomerations)) $dcp->field_linked_agglomerations['und'][]['nid'] = $aggnid;
			if(!uwwtd_in_array_r($dcpnid, $agg->field_linked_discharge_points)) $agg->field_linked_discharge_points['und'][]['nid'] = $dcpnid;
			node_save($agg);
		}
		node_save($uww);
	}
	node_save($dcp);

	$context['sandbox']['progress']++;
	$context['message'] = t('Processing the links between all elements...');
	$context['results'][] = $agg->nid . ' : ' . check_plain($node->title);
	
	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_rca_extra_batch_process($newFile, $annee, $datas, $timestamp, &$context){
	if (!isset($context['sandbox']['progress'])) {
	
		$ids = array();

		//Get all the agglomerations
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
		$aggs = $query
			->fields('n', array('nid', 'title'))		
			->condition('n.type', 'receiving_area', '=')
			->condition('r.field_siteid_value', db_like($annee.'_rca_').'%', 'LIKE')
			->execute();
		foreach($aggs as $agg){
			$ids[] = array('nid'=>$agg->nid, 'title'=>$agg->title);
		}


		$context['sandbox']['max'] = count($ids);
		$context['sandbox']['progress'] = 0;
		$context['sandbox']['ids'] = $ids;

	}
	$i = $context['sandbox']['progress'];
	$data = $context['sandbox']['ids'][$i];
	$node = node_load($data['nid']);

	// variables
	$nbStations = 0;
	$designCap = 0;
	$pEnt = 0;
	$pDis = 0;
	$nEnt = 0;
	$nDis = 0;

	//dsm($node);
	// for each station connected to the rca we calculate the variables
	// NEED INFO ON ARTICLE 5.4 and 5.8
	// if art 5.4 or 5.8 ... !!!!!!!!!!!!!
	foreach($node->field_linked_treatment_plants['und'] as $uwwitem){
		// load each uww
		$uww = node_load($uwwitem['nid']);
		//dsm($uww);

		$nbStations++;
		$designCap = $designCap + $uww->field_physicalcapacityactivity['und'][0]['value'];
		$pEnt = $pEnt + $uww->field_uwwpincoming['und'][0]['value'];
		$pDis = $pDis + $uww->field_uwwpdischarge['und'][0]['value'];
		$nEnt = $nEnt + $uww->field_uwwnincoming['und'][0]['value'];
		$nDis = $nDis + $uww->field_uwwndischarge['und'][0]['value'];
	}

	if($nbStations != 0){
		$node->field_rca_nb_uwwtps['und'][0]['value'] = $nbStations;
		$node->field_rca_total_capacity_uwwtps['und'][0]['value'] = $designCap;
		$node->field_rca_total_p_entering['und'][0]['value'] = $pEnt;
		$node->field_rca_total_p_discharged['und'][0]['value'] = $pDis;
		$node->field_rca_total_n_entering['und'][0]['value'] = $nEnt;
		$node->field_rca_total_n_discharged['und'][0]['value'] = $nDis;
	}

	node_save($node);

	$context['sandbox']['progress']++;
	$context['message'] = t('Processing Receiving area extra data...');
	$context['results'][] = $agg->nid . ' : ' . check_plain($node->title);
	
	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_finished($success, $results, $operations){
	if ($success) {
		drupal_set_message(t('Receiving areas').': '.$_SESSION['newRca'].' '.t('new elements added').', '.$_SESSION['upRca'].' '.t('elements updated'));
		drupal_set_message(t('Big cities').': '.$_SESSION['newBc'].' '.t('new elements added').', '.$_SESSION['upBc'].' '.t('elements updated'));
		drupal_set_message(t('Agglomerations').': '.$_SESSION['newAgg'].' '.t('new elements added').', '.$_SESSION['upAgg'].' '.t('elements updated'));
		drupal_set_message(t('Treatment plants').': '.$_SESSION['newUww'].' '.t('new elements added').', '.$_SESSION['upUww'].' '.t('elements updated'));
		drupal_set_message(t('Discharge points').': '.$_SESSION['newDcp'].' '.t('new elements added').', '.$_SESSION['upDcp'].' '.t('elements updated'));
		drupal_set_message(t('Links created between \'discharge points\', \'waste water treatment plants\' and \'receiving areas\'.'));
		drupal_set_message(t('Links created between \'agglomerations\' and \'waste water treatment plants\'.'));
		drupal_set_message(t('Links created between \'big cities\' and \'agglomerations\'.'));
		drupal_set_message(t('\'big cities\' and their coordinates have been set.'));
		if(isset($_SESSION['uwwtd_errors'])){
			//uwwtd_add_error($_SESSION['uwwtd_errors']);
			foreach($_SESSION['uwwtd_errors'] as $error){
				drupal_set_message(t('Error occured on row index : ').$error[0].' (" '.$error[1].' ") '.t(' of your file. This element was not added, please check your input file.'), 'error');
			}
		}
		/*if(isset($_SESSION['uwwtd_warnings'])){
			uwwtd_add_error($_SESSION['uwwtd_warnings']);
			foreach($_SESSION['uwwtd_warnings'] as $warning){
				drupal_set_message(t('Warning occured on row index : ').$warning[0].' (" '.$warning[1].' ") '.t(' of your file. This element was added but please check your input file.'), 'warning');
			}
		}*/
	}
	else {
		// An error occurred.
		// $operations contains the operations that remained unprocessed.
		$error_operation = reset($operations);
		drupal_set_message(
			t('An error occurred while processing @operation with arguments : @args',
				array(
					'@operation' => $error_operation[0],
					'@args' => print_r($error_operation[0], TRUE),
				)
			)
		);
	}
}


################################ SHAPES ZIP MANAGEMENT ################################
function uwwtd_shapes_form($form, &$form_state){
	
	if(!isset($_SESSION['uwwtd_annee'])){
		drupal_goto('data');
	}
	else{
		// Definition des variables pour le formulaire
		$annee = $_SESSION['uwwtd_annee'];
		
		// definition du champ input de type file
		$form['uwwtd_text'] = array(
			'#markup' => '<h4>'.t('Receiving areas shapes upload for the year').' : '.$annee.'</h4><p>'.t('Please upload the files that contain the information on the sensitive areas').'.</p>'
		);
		
		$form['uwwtd_file'] = array(
			'#type' => 'managed_file',
			'#title' => t('Data file'),
			'#upload_validators' => array('file_validate_extensions' => array('zip')),
			'#required' => TRUE,
		);
		
		// Bouton envoyer
		$form['submit_button'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
		
		$form['uwwtd_text2'] = array(
			'#markup' => '<br><br><p>'.t('Once finished uploading your shapes files or to skip this step, please click the continue link below').'.</p>'
		);
		
		$form['continue_button'] = array(
			'#markup' => ''.l(t('Continue'), 'data/conformity').''
		);
		
		// Fonction a appeller quand on clique sur envoyer
		$form['#submit'][] = 'uwwtd_shapes_submit_handler';
		return $form;
	}
}

function uwwtd_shapes_submit_handler($form, &$form_state) {
	
	// Definition et recup des variables
	if(isset($_SESSION['uwwtd_errors'])) unset($_SESSION['uwwtd_errors']);
	if(isset($_SESSION['uwwtd_warnings'])) unset($_SESSION['uwwtd_warnings']);
	$annee = $_SESSION['uwwtd_annee'];
	$file = file_load($form_state['values']['uwwtd_file']);
	
	// dezip the zipped file
	$files = uwwtd_unzip_files($file, 'public://data_shapes/');
	
	uwwtd_shapes_setup_batch($files, $annee);	
}

function uwwtd_shapes_setup_batch($files, $annee){
	$batch = array(
		'title' => t('Data processing for the shapes files'),
		'operations' => array(
			array('uwwtd_shapes_batch_process', array($files, $annee))
		),
		'error_message' => t('Error!'),
		'finished' => 'uwwtd_shapes_finished',
		'file' => drupal_get_path('module','uwwtd').'/data_center.inc'
	);

	batch_set($batch);
	batch_process('data/shapes');
}

function uwwtd_shapes_batch_process($files, $annee, &$context){

	if (!isset($context['sandbox']['progress'])) {
		$shpsFiles = array();
		foreach($files['extracted'] as $file){
			$exploFile = explode('.', $file);
			if(end($exploFile) == 'shp') $shpsFiles[] = $file;
		}
		$context['sandbox']['max'] = count($shpsFiles);
		$context['sandbox']['progress'] = 0;
		$context['sandbox']['ids'] = $shpsFiles;
	}
	$i = $context['sandbox']['progress'];
	$chosen = $context['sandbox']['ids'][$i];

	//dsm($context['sandbox']['ids']);
	//dsm($chosen);

	// including the shapefile reader class
	require_once 'ShapeFile.inc.php';
	 
	// importing the downloaded shapefile
	$shp = new ShapeFile(drupal_realpath($chosen));

	foreach($shp->records as $record){
		// id of shape
		$id = '';
		$shptype = '1';
		if(isset($record->dbf_data['EUCD_SA_CM'])){
			$id = uwwtd_check_text($record->dbf_data['EUCD_SA_CM']);
			$shptype = '1';
		}
		if(isset($record->dbf_data['ISO2'])){
			$shptype = '2';
			$id = uwwtd_check_text($record->dbf_data['ISO2']);
		}

		 // check multipolygon
		$type = 'polygon';
		

		if($record->shp_data['numparts'] > 1){
			$type = 'multipolygon';
			$shpdata = $record->shp_data['parts'];
		}
		else{
			$shpdata = $record->shp_data['parts'][0]['points'];
		}

		//dsm($shpdata);

		// Other possible type

		// find receiving area
		$ids = array();
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
		$query->fields('n', array('nid', 'title'));
		$query->fields('s', array('field_siteid_value'));
		$query->condition('n.type', 'receiving_area', '=');
		$query->condition('s.field_siteid_value', $annee.'_'.$id, '=');
		$rcas = $query->execute();
		foreach($rcas as $rca){
			$ids[] = array('nid'=>$rca->nid, 'title'=>$rca->title, 'siteId'=>$rca->field_siteid_value);
		}

		//dsm($ids);

		if(count($ids) > 0){
			// get node
			$nid = $ids[0]['nid'];
			$node = node_load($nid);

			// get polgon geo data
			if($type == 'polygon') $geodata = uwwtd_coord_transform_polygon($shpdata);
			if($type == 'multipolygon') $geodata = uwwtd_coord_transform_multipolygon($shpdata);

			$node->field_position_geo['und'][0] = array(
				'geom'=>$geodata['bin'],
				'geo_type'=>$geodata['polygon'],
				'geohash'=>$geodata['geohash']
			);

			node_save($node);
			//dsm($node);
		}

	}

	unset($shp);

	$context['sandbox']['progress']++;
	$context['message'] = t('Processing shape files');
	$context['results'][] = $node->nid . ' : ' . check_plain($node->title);	

	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_shapes_finished($success, $results, $operations) {
	if ($success) {
		drupal_set_message(t('All the shapes were successfully added to their receiving areas.'));
	}
	else {
		// An error occurred.
		// $operations contains the operations that remained unprocessed.
		$error_operation = reset($operations);
		drupal_set_message(
			t('An error occurred while processing @operation with arguments : @args',
				array(
					'@operation' => $error_operation[0],
					'@args' => print_r($error_operation[0], TRUE),
				)
			)
		);
	}
}


################################ DATA CONFORMITY ################################
function uwwtd_conformity_form($form, &$form_state){

	if(!isset($_SESSION['uwwtd_annee'])){
		drupal_goto('data');
	}
	else {
		// Definition des variables pour le formulaire
		$annee = $_SESSION['uwwtd_annee'];
		$sas = array();
		$csas = array();

		//Get all allos for the year that are in a SA
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
		$query->join('field_data_field_agggenerated', 'g', 'g.entity_id = n.nid');
		$query->join('field_data_field_agguwwliste', 'u', 'u.entity_id = n.nid');
		$query->join('field_data_field_uwwdcpliste', 'd', 'd.entity_id = u.field_agguwwliste_nid');
		$query->join('field_data_field_rcatype', 't', 't.entity_id = d.field_uwwdcpliste_nid');
		$query->join('field_data_field_inspireidlocalid', 'a', 'a.entity_id = n.nid');
		$query->fields('n', array('nid', 'title'));
		$query->fields('s', array('field_siteid_value'));
		$query->fields('g', array('field_agggenerated_value'));
		$query->fields('u', array('field_agguwwliste_nid'));
		$query->fields('d', array('field_uwwdcpliste_nid'));
		$query->fields('t', array('field_rcatype_value'));
		$query->fields('a', array('field_inspireidlocalid_value'));
		$query->condition('n.type', 'agglomeration', '=');
		$query->condition('s.field_siteid_value', db_like($annee).'%', 'LIKE');
		$query->condition('g.field_agggenerated_value', 10000, '>');
		$query->condition('t.field_rcatype_value', 'SA', '=');
		$query->orderBy('t.field_rcatype_value', 'DESC');
		$query->orderBy('g.field_agggenerated_value', 'DESC');
		$aggs = $query->execute();
		foreach($aggs as $agg){
			$sas[] = array('nid'=>$agg->nid, 'title'=>$agg->title, 'aggSize'=>$agg->field_agggenerated_value, 'stationId'=>$agg->field_agguwwliste_nid, 'dcpId'=>$agg->field_uwwdcpliste_nid, 'rcaType'=>$agg->field_rcatype_value, 'aggId'=>$agg->field_inspireidlocalid_value);
		}

		//Get all allos for the year that are in a CSA
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
		$query->join('field_data_field_agggenerated', 'g', 'g.entity_id = n.nid');
		$query->join('field_data_field_agguwwliste', 'u', 'u.entity_id = n.nid');
		$query->join('field_data_field_uwwdcpliste', 'd', 'd.entity_id = u.field_agguwwliste_nid');
		$query->join('field_data_field_rcatype', 't', 't.entity_id = d.field_uwwdcpliste_nid');
		$query->join('field_data_field_inspireidlocalid', 'a', 'a.entity_id = n.nid');
		$query->fields('n', array('nid', 'title'));
		$query->fields('s', array('field_siteid_value'));
		$query->fields('g', array('field_agggenerated_value'));
		$query->fields('u', array('field_agguwwliste_nid'));
		$query->fields('d', array('field_uwwdcpliste_nid'));
		$query->fields('t', array('field_rcatype_value'));
		$query->fields('a', array('field_inspireidlocalid_value'));
		$query->condition('n.type', 'agglomeration', '=');
		$query->condition('s.field_siteid_value', db_like($annee).'%', 'LIKE');
		$query->condition('g.field_agggenerated_value', 10000, '>');
		$query->condition('t.field_rcatype_value', 'CSA', '=');
		$query->orderBy('t.field_rcatype_value', 'DESC');
		$query->orderBy('g.field_agggenerated_value', 'DESC');
		$aggs = $query->execute();
		foreach($aggs as $agg){
			$csas[] = array('nid'=>$agg->nid, 'title'=>$agg->title, 'aggSize'=>$agg->field_agggenerated_value, 'stationId'=>$agg->field_agguwwliste_nid, 'dcpId'=>$agg->field_uwwdcpliste_nid, 'rcaType'=>$agg->field_rcatype_value, 'aggId'=>$agg->field_inspireidlocalid_value);
		}

		$sas = array_filter($sas);
		$csas = array_filter($csas);
		if (!empty($sas) || !(empty($csas))){

			$form['help'] = array(
				'name' => array(
				'#type' => 'item',
				'#markup' => t('The following agglomerations are eligable for criterion b, ca and cb for the article 5 a element for the calculation of conformity for the agglomerations.').'<br>'.t('If any of the criterions apply to the agglomerations below, please tick the boxes and press the send button.'),
				'#weight' => 0
			));

			$form['Agglo'] = array(
				'name' => array(
				'#type' => 'item',
				'#markup' => '<strong>'.t('Agglomeration').'</strong>',
				'#weight' => 0
			));
			$form['critb'] = array(
				'name' => array(
				'#type' => 'item',
				'#markup' => '<strong>'.t('Criterion b applied').'</strong>',
				'#weight' => 0
			));
			$form['critca'] = array(
				'name' => array(
				'#type' => 'item',
				'#markup' => '<strong>'.t('Criterion ca applied').'</strong>',
				'#weight' => 0
			));
			$form['critcb'] = array(
				'name' => array(
				'#type' => 'item',
				'#markup' => '<strong>'.t('Criterion cb applied').'</strong>',
				'#weight' => 0
			));

			foreach ($sas as $sa){

				$node = node_load($sa['nid']);

				$form['SA']['#tree'] = TRUE;

				$form['SA'][] = array(
					'nid' => array(
						'#type' => 'hidden',
						'#value' => $sa['nid'],
					),
					'name' => array(
						'#type' => 'item',
						'#markup' => check_plain($sa['title']).' : '.$sa['aggId'],
					),
					'art5b' => array(
						'#type' => 'checkbox',
						'#default_value' => $node->field_aggcritb['und'][0]['value'],
					),
					'art5ca' => array(
						'#type' => 'checkbox',
						'#default_value' => $node->field_aggcritca['und'][0]['value'],
					),
					'art5cb' => array(
						'#type' => 'checkbox',
						'#default_value' => $node->field_aggcritcb['und'][0]['value'],
					)
				);
			}

			foreach ($csas as $csa){

				$node = node_load($csa['nid']);

				$form['CSA']['#tree'] = TRUE;

				$form['CSA'][] = array(
					'nid' => array(
						'#type' => 'hidden',
						'#value' => $csa['nid'],
					),
					'name' => array(
						'#type' => 'item',
						'#markup' => check_plain($csa['title']).' : '.$csa['aggId'],
					),
					'art5b' => array(
						'#type' => 'checkbox',
						'#default_value' => $node->field_aggcritb['und'][0]['value'],
					),
					'art5ca' => array(
						'#type' => 'item',
					),
					'art5cb' => array(
						'#type' => 'item',
					),
				);
			}
		}
		else{
			$form['uwwtd_text'] = array(
				'#markup' => '<h3>'.t('No agglomerations that could be concerned with article 5 criterion b, ca or cb could be found. You can continue to the next step').'</h3>'
			);
		}


		
		// definition du champ input de type file
		$form['uwwtd_text'] = array(
			'#markup' => '<h4>'.t('Data verification for the year').' : '.$annee.'</h4><p>'.t('Be sure that your data is correctly uploaded then to proceed press the send button below').'.</p>'
		);
		
		// Bouton envoyer
		$form['submit_button'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
		
		// Fonction a appeller quand on clique sur envoyer
		$form['#submit'][] = 'uwwtd_conformity_submit_handler';
		
		return $form;
	}
}

function uwwtd_conformity_submit_handler($form, &$form_state){

	// Definition et recup des variables
	if(isset($_SESSION['uwwtd_errors'])) unset($_SESSION['uwwtd_errors']);
	if(isset($_SESSION['uwwtd_warnings'])) unset($_SESSION['uwwtd_warnings']);
	$annee = $_SESSION['uwwtd_annee'];

	if(isset($form_state['values']['SA'])) $sas = $form_state['values']['SA'];
	else $sas = array();

	if(isset($form_state['values']['CSA'])) $csas = $form_state['values']['CSA'];
	else $csas = array();

	$timestamp = strtotime('now');

	//Init the batch process
	uwwtd_conformity_setup_batch($sas, $csas, $annee, $timestamp);
}

function uwwtd_conformity_setup_batch($sas, $csas, $annee, $timestamp){
	$batch = array(
		'title' => t('Data processing for setting the conformity'),
		'operations' => array(
			//array('uwwtd_add_sa_criterions_batch_process', array($sas, $timestamp)),
			//array('uwwtd_add_csa_criterions_batch_process', array($csas, $timestamp)),
			//array('uwwtd_agglo_conformity_batch_process', array($annee, $timestamp)),
			array('uwwtd_station_conformity_batch_process', array($annee, $timestamp)),
			//array('uwwtd_agglo_reconformity_batch_process', array($annee, $timestamp))
		),
		'error_message' => t('Error!'),
		'finished' => 'uwwtd_conformity_finished',
		'file' => drupal_get_path('module','uwwtd').'/data_center.inc'
	);

	batch_set($batch);
	batch_process('data/conformity');
}

function uwwtd_add_sa_criterions_batch_process($sas, $timestamp, &$context){

	if (!isset($context['sandbox']['progress'])) {
		
		$context['sandbox']['max'] = count($sas);
		if(count($sas) == 0) $context['sandbox']['progress'] = -1;
		else $context['sandbox']['progress'] = 0;
	}
	$i = $context['sandbox']['progress'];

	if(isset($sas[$i])){
		$data = $sas[$i];
		$node = node_load($data['nid']);

		$node->field_aggcritb['und'][0]['value'] = $data['art5b'];
		$node->field_aggcritca['und'][0]['value'] = $data['art5ca'];
		$node->field_aggcritcb['und'][0]['value'] = $data['art5cb'];
		
		node_save($node);
	}

	$context['sandbox']['progress']++;
	$context['message'] = t('Processing criterion for the agglomeration').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
	$context['results'][] = $node->nid . ' : ' . check_plain($node->title);	

	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_add_csa_criterions_batch_process($csas, $timestamp, &$context){

	if (!isset($context['sandbox']['progress'])) {
		
		$context['sandbox']['max'] = count($csas);
		if(count($csas) == 0) $context['sandbox']['progress'] = -1;
		else $context['sandbox']['progress'] = 0;
	}
	$i = $context['sandbox']['progress'];

	if(isset($csas[$i])){
		$data = $csas[$i];
		$node = node_load($data['nid']);

		$node->field_aggcritb['und'][0]['value'] = $data['art5b'];
		$node->field_aggcritca['und'][0]['value'] = $data['art5ca'];
		$node->field_aggcritcb['und'][0]['value'] = $data['art5cb'];
		
		node_save($node);
	}

	$context['sandbox']['progress']++;
	$context['message'] = t('Processing criterion for the agglomeration').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
	$context['results'][] = $node->nid . ' : ' . check_plain($node->title);	

	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_station_conformity_batch_process($annee, $timestamp, &$context){

	if (!isset($context['sandbox']['progress'])) {
	
		$ids = array();
	
		//Get the uwwtps
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
		$query->fields('n', array('nid', 'title'));
		$query->condition('n.type', 'uwwtp', '=');
		$query->condition('s.field_siteid_value', db_like($annee).'%', 'LIKE');
		$uwwtps = $query->execute();
		foreach($uwwtps as $uwwtp){
			$ids[] = array('nid'=>$uwwtp->nid, 'title'=>$uwwtp->title);
		}
		
		$context['sandbox']['max'] = count($ids);
		$context['sandbox']['progress'] = 0;
		$context['sandbox']['ids'] = $ids;
	}
	$i = $context['sandbox']['progress'];
	
	$data = $context['sandbox']['ids'][$i];
	$node = node_load($data['nid']);

	// Function that checks the required treatment, the compliance and the reason for the compliance in the following form array(requiredTreatment, compliance, reason)
	$results = uwwtd_station_compliance_algo($node, $timestamp);
	
	$required = $results[0];
	$comp = $results[1];
	$reason = $results[2];
	$treatmet = $results[3];
	$uwwperf = $results[4];

	// Set required treatment type
	$node->field_uwwtreatmentrequired['und'][0]['value'] = $required;

	// Now we save the results we obtained above
	$node->field_uwwtreatment_met['und'][0]['value'] = $treatmet;
	$node->field_uwwperformance_met['und'][0]['value'] = $uwwperf;

	// Check to see if one of the agglomerations is NR
	$isNR = 1;
	foreach($node->field_uwwaggliste['und'] as $nragg){
		$nraggli = node_load($nragg['nid']);
		if($nraggli->field_aggcompliance['und'][0]['value'] != 'NR') $isNR = 0;
	}

	// if deadline agglo is not passed compliance = NR else compliance = NC
	$today = date('Y-m-j'); 
	$dateArt3 = $node->field_aggperiodover3['und'][0]['value'];
	$dateArt4 = $node->field_aggperiodover4['und'][0]['value'];
	$dateArt5 = $node->field_aggperiodover5['und'][0]['value'];

	$dateSup = $dateArt3;
	if($dateArt4 > $dateSup) $dateSup = $dateArt4;
	if($dateArt5 > $dateArt4 && $dateArt5 > $dateSup) $dateSup = $dateArt5;
	
	if($dateSup > $today){$comp = 'NR';}	

	// If agglomeration is NR and the station compliance is not C then the station takes the agglomeration's compliance of non relative
	if($isNR == 1 && $comp != 'C'){
		$comp = 'NR';
		$reason = 'The station was found to be non relevant because the agglomeration is non relevant';
	}

	$node->field_uwwcompliance['und'][0]['value'] = $comp;
	$node->field_compliance_explication['und'][0]['value'] = $reason;
	
	node_save($node);

	$context['sandbox']['progress']++;
	$context['message'] = t('Processing conformity for the station').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid);
	$context['results'][] = $node->nid . ' : ' . check_plain($node->title);

	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_agglo_conformity_batch_process($annee, $timestamp, &$context){

	if (!isset($context['sandbox']['progress'])) {
	
		$ids = array();
	
		//Get the agglos
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
		$query->join('field_data_field_agggenerated', 'g', 'g.entity_id = n.nid');
		$query->fields('n', array('nid', 'title'));
		$query->condition('n.type', 'agglomeration', '=');
		$query->condition('s.field_siteid_value', db_like($annee).'%', 'LIKE');
		$query->orderBy('field_agggenerated_value', 'DESC');
		$agglos = $query->execute();
		foreach($agglos as $agglo){
			$ids[] = array('nid'=>$agglo->nid, 'title'=>$agglo->title);
		}
		
		$context['sandbox']['max'] = count($ids);
		$context['sandbox']['progress'] = 0;
		$context['sandbox']['ids'] = $ids;
	}
	$i = $context['sandbox']['progress'];
	
	$data = $context['sandbox']['ids'][$i];
	$node = node_load($data['nid']);
	node_object_prepare($node);

	// need rcaType waterBodyType rcaanitro rcaaphos
	$rcaAPhos = 0;
	$rcaANitro = 0;
	$rcaType = '';
	$waterBodyTypes = array();
	if(isset($node->field_agguwwliste['und'])){
		foreach($node->field_agguwwliste['und'] as $uww){
			$uwwtd = node_load($uww['nid']);
			if(isset($uwwtd->field_uwwdcpliste['und'])){
				$dcpNid = $uwwtd->field_uwwdcpliste['und'][0]['nid'];
				foreach($uwwtd->field_uwwdcpliste['und'] as $dc){
					$dcp = node_load($dc['nid']);

					// add waterBodyType
					$waterBodyTypes[] = $dcp->field_dcpwaterbodytype['und'][0]['value'];

					// different cases for rcaType
					if($rcaType == '' && isset($dcp->field_rcatype['und'][0]['value'])){
						$rcaType = $dcp->field_rcatype['und'][0]['value'];
						$dcpNid = $dcp->nid;
					}
					if(isset($dcp->field_rcatype['und'][0]['value']) && $rcaType == 'LSA'){
						$rcaType = $dcp->field_rcatype['und'][0]['value'];
						$dcpNid = $dcp->nid;
					}
					if(isset($dcp->field_rcatype['und'][0]['value']) && $rcaType == 'NA' && $dcp->field_rcatype['und'][0]['value'] == 'SA'){
						$rcaType = $dcp->field_rcatype['und'][0]['value'];
						$dcpNid = $dcp->nid;
					}
				}
				// time to get rca nitro and phos
				$trueDcp = node_load($dcpNid);
				if(isset($trueDcp->field_dcprcaliste['und'])){
					foreach($trueDcp->field_dcprcaliste['und'] as $dcpRca){
						$trueRca = node_load($dcpRca['nid']);
						if(isset($trueRca->field_rcaaphos['und'][0]['value']) && ($trueRca->field_rcaaphos['und'][0]['value'] == 1 || $trueRca->field_rcaaphos['und'][0]['value'] == '1')){
							$rcaAPhos = 1;
						}
						if(isset($trueRca->field_rcaanitro['und'][0]['value']) && ($trueRca->field_rcaanitro['und'][0]['value'] == 1 || $trueRca->field_rcaanitro['und'][0]['value'] == '1')){
							$rcaANitro = 1;
						}
					}
				}
			}
		}
	}

	// cas ou l'ensemble du pays est en zone sensible
	$repcode = $node->field_repcode['und'][0]['value'];
	$repcode = str_replace('_', '-', $repcode);
	$boom = explode('-', $repcode);
	$boom = explode(' ', $boom[0]);
	$rep = $boom[0];
	if($rep == 'LT'){
		$rcaType = 'SA';
		$rcaANitro = 1;
		$rcaAPhos = 1;
	}

	$art3Compar = uwwtd_article3_compliance($node);
	$art3Comp = $art3Compar[0];
	// if the deadline has not yet been reached and is not compliant
	$today = date('Y-m-j'); 
	
	if($node->field_aggperiodover3['und'][0]['value'] > $today && $art3Comp != 'C'){
		$art3Compar = array('NR', 'Not relevant with article 3 because it is not compliant and the deadline has not been reached');
		$art3Comp = 'NR';
	}

	//dsm('3 '.$art3Comp);
	$art4Compar = uwwtd_article4_compliance($node, $rcaType, $waterBodyTypes, $art3Comp);
	$art4Comp = $art4Compar[0];
	if($node->field_aggperiodover4['und'][0]['value'] > $today && $art4Comp != 'C'){
		$art4Compar = array('NR', 'Not relevant with article 4 because it is not compliant and the deadline has not been reached');
		$art4Comp = 'NR';
	}
	//dsm('4 '.$art4Comp);

	$art5Compar = uwwtd_article5_compliance($node, $rcaType, $rcaANitro, $rcaAPhos, $art3Comp, $art4Comp);
	$art5Comp = $art5Compar[0];
	if($node->field_aggperiodover5['und'][0]['value'] > $today && $art5Comp != 'C'){
		$art5Compar = array('NR', 'Not relevant with article 5 because it is not compliant and the deadline has not been reached');
		$art5Comp = 'NR';
	}
	//dsm('5 '.$art5Comp);

	// each article compliance
	$node->field_aggart3compliance['und'][0]['value'] = $art3Comp;
	$node->field_article_3_compliance_expli['und'][0]['value'] = $art3Compar[1];

	$node->field_aggart4compliance['und'][0]['value'] = $art4Comp;
	$node->field_article_4_compliance_expli['und'][0]['value'] = $art4Compar[1];

	$node->field_aggart5compliance['und'][0]['value'] = $art5Comp;
	$node->field_article_5_compliance_expli['und'][0]['value'] = $art5Compar[1];

	//general compliance
	if($art3Comp == 'AddQC') $art3Comp = 'NC';
	if($art3Comp == 'QC') $art3Comp = 'C';
	if($art4Comp == 'RNC') $art4Comp = 'NC';

	if($art3Comp == 'NR'){
		$comp = 'NR';
		$reason = t('The agglomeration has been found to be non relevant with all of the articles.');
	}
	if($art3Comp == 'NC'){
		$comp = 'NC';
		$reason = t('The agglomeration has been found to be non compliant with article 3.');
	}
	if($art3Comp == 'NI'){
		$comp = 'NI';
		$reason = t('The agglomeration has been found to have no information for article 3.');
	}
	if($art3Comp == 'C'){
		if($art4Comp == 'NC'){
			$comp = 'NC';
			$reason = t('The agglomeration has been found to be non compliant with article 4.');
		}
		if($art4Comp == 'NI'){
			$comp = 'NI';
			$reason = t('The agglomeration has been found to have no information for article 4.');
		}
		if($art4Comp == 'C' || $art4Comp == 'NR'){
			if($art5Comp == 'NC'){
				$comp = 'NC';
				$reason = t('The agglomeration has been found to be non compliant with article 5.');
			}
			if($art5Comp == 'NI'){
				$comp = 'NI';
				$reason = t('The agglomeration has been found to have no information for article 5.');
			}
			if($art5Comp == 'C' || $art5Comp == 'NR'){
				$comp = 'C';
				$reason = t('The agglomeration has been found to be compliant with all the articles.');
			}
		}
	}

	$node->field_aggcompliance['und'][0]['value'] = $comp;
	$node->field_compliance_explication['und'][0]['value'] = $reason;

	//dsm($node->title.':'.$node->nid.': general :'.$comp.'('.$art3Comp.','.$art4Comp.','.$art5Comp.')');

	node_save($node);

	$context['sandbox']['progress']++;
	$context['message'] = t('Processing conformity for the agglomeration').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
	$context['results'][] = $node->nid . ' : ' . check_plain($node->title);	
	

	if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_agglo_reconformity_batch_process($annee, $timestamp, &$context){

	if (!isset($context['sandbox']['progress'])) {
	
		$ids = array();
	
		//Get the agglos
		$query = db_select('node', 'n');
		$query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
		$query->join('field_data_field_agggenerated', 'g', 'g.entity_id = n.nid');
		$query->fields('n', array('nid', 'title'));
		$query->condition('n.type', 'agglomeration', '=');
		$query->condition('s.field_siteid_value', db_like($annee).'%', 'LIKE');
		$query->orderBy('field_agggenerated_value', 'DESC');
		$agglos = $query->execute();
		foreach($agglos as $agglo){
			$ids[] = array('nid'=>$agglo->nid, 'title'=>$agglo->title);
		}
		
		$context['sandbox']['max'] = count($ids);
		$context['sandbox']['progress'] = 0;
		$context['sandbox']['ids'] = $ids;
	}
	$i = $context['sandbox']['progress'];
	
	$data = $context['sandbox']['ids'][$i];
	$node = node_load($data['nid']);
	node_object_prepare($node);

	// We need to get our current compliance
	// LG : we take the compliance info from agglo
	$comp = $node->field_aggcompliance['und'][0]['value'];
	$reason = $node->field_compliance_explication['und'][0]['value'];
	$comp_art3_from_tp = $node->field_aggart3compliance['und'][0]['value'];
	$comp_art4_from_tp = $node->field_aggart4compliance['und'][0]['value'];
	$comp_art5_from_tp = $node->field_aggart5compliance['und'][0]['value'];
	$comp_art4_from_tp_reason = $node->field_article_4_compliance_expli['und'][0]['value'];
	
	
	//LG 1 er avril : on regarde pour toute les agglo
	if($comp_art4_from_tp != 'NR'){
		foreach($node->field_agguwwliste['und'] as $uww){
			$uwwtp = node_load($uww['nid']);
			// LG 1er avril 2015 ici => si le TP n'est pas conforme alors c'est l'article 4 de l'agglo que l'on modifie et pas la compliance totale
			$comp_tp = $uwwtp->field_uwwcompliance['und'][0]['value'];
			if($comp_tp != 'C'){
				if($comp_tp == 'NC'){
					$comp_art4_from_tp = $comp_tp;
					$comp_art4_from_tp_reason = t('The agglomeration article 4 has been found to be not compliant because one of the TP is not.');
				}
				if($comp_tp == 'NR' && $comp_art4_from_tp != 'NC'){
					$comp_art4_from_tp = $comp_tp;
					$comp_art4_from_tp_reason = t('The agglomeration article 4 has been found to be non relevant because one of the TP is not.');
				}
				if($comp_tp == 'NI' && $comp_art4_from_tp != 'NC' && $comp_art4_from_tp != 'NR'){
					$comp_art4_from_tp = $comp_tp;
					$comp_art4_from_tp_reason = t('The agglomeration article 4 has been found to have no information.');
				}
			}
		}
	}



	// LG : on recalcule la compliance totale
	if($comp_art3_from_tp == 'NR'){
		$comp = 'NR';
		$reason = t('The agglomeration has been found to be non relevant with all of the articles.');
	}
	if($comp_art3_from_tp == 'NC'){
		$comp = 'NC';
		$reason = t('The agglomeration has been found to be non compliant with article 3.');
	}
	if($comp_art3_from_tp == 'NI'){
		$comp = 'NI';
		$reason = t('The agglomeration has been found to have no information for article 3.');
	}
	if($comp_art3_from_tp == 'C'){
		if($art4Comp == 'NC'){
			$comp = 'NC';
			$reason = t('The agglomeration has been found to be non compliant with article 4.');
		}
		if($comp_art4_from_tp == 'NI'){
			$comp = 'NI';
			$reason = t('The agglomeration has been found to have no information for article 4.');
		}
		if($comp_art4_from_tp == 'C' || $comp_art4_from_tp == 'NR'){
			if($comp_art5_from_tp == 'NC'){
				$comp = 'NC';
				$reason = t('The agglomeration has been found to be non compliant with article 5.');
			}
			if($comp_art5_from_tp == 'NI'){
				$comp = 'NI';
				$reason = t('The agglomeration has been found to have no information for article 5.');
			}
			if($comp_art5_from_tp == 'C' || $comp_art5_from_tp == 'NR'){
				$comp = 'C';
				$reason = t('The agglomeration has been found to be compliant with all the articles.');
			}
		}
	}

	// Lg 1er avril : on met ࡪour l'article 4 dans l'agglo
	$node->field_aggart4compliance['und'][0]['value'] = $comp_art4_from_tp;
	$node->field_article_4_compliance_expli['und'][0]['value'] = $comp_art4_from_tp_reason;
	
	$node->field_aggcompliance['und'][0]['value'] = $comp;
	$node->field_compliance_explication['und'][0]['value'] = $reason;


	node_save($node);

	$context['sandbox']['progress']++;
	$context['message'] = t('Reprocessing conformity for the agglomeration').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
	$context['results'][] = $node->nid . ' : ' . check_plain($node->title);	
	

	if($context['sandbox']['progress'] != $context['sandbox']['max']){
		$context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
	}
}

function uwwtd_conformity_finished($success, $results, $operations) {
	if ($success) {
		drupal_set_message(t('All the criterions were added to the agglomerations concered.'));
		drupal_set_message(t('All the waste water treatment plants\' conformity was calculated.'));
		drupal_set_message(t('All the agglomerations\' conformity was calculated.'));
		drupal_set_message(t('Congratulations! Your data has been successfully imported to the site you can now view your data on the interactive maps accessible from the home page.'));
		if(isset($_SESSION['uwwtd_errors'])){
			uwwtd_add_error($_SESSION['uwwtd_errors']);
			foreach($_SESSION['uwwtd_errors'] as $error){
				drupal_set_message(t('Error occured on row index : ').$error[0].' (" '.$error[1].' ") '.t(' of your file . This element was not added, please check your input file.'), 'error');
			}
		}
		if(isset($_SESSION['uwwtd_warnings'])){
			uwwtd_add_error($_SESSION['uwwtd_warnings']);
			foreach($_SESSION['uwwtd_warnings'] as $warning){
				drupal_set_message(t('Warning occured on row index : ').$warning[0].' (" '.$warning[1].' ") '.t(' of your file . This element was added but please check your input file.'), 
					'warning');
			}
		}
	}
	else {
		// An error occurred.
		// $operations contains the operations that remained unprocessed.
		$error_operation = reset($operations);
		drupal_set_message(
			t('An error occurred while processing @operation with arguments : @args',
				array(
					'@operation' => $error_operation[0],
					'@args' => print_r($error_operation[0], TRUE),
				)
			)
		);
	}
}


################################ FUNCTIONS ################################
function uwwtd_XYToDrupalPoint($x, $y, $epsg_orig, $epsg_dest = 4326){
	//Define the WKT
	$wkt = 'POINT('.$x.' '.$y.')';
	//Transform to the right projection ==> we need PostGIS function
	$wkt_wgs84 = db_query(
		"SELECT ST_AsText(ST_Transform(ST_GeomFromText('".$wkt."', ".$epsg_orig."), ".$epsg_dest.")) as wkt"
	)->fetchField();
	
	return $wkt_wgs84;
}

function uwwtd_check_exist($siteId){

	$id = '';

	$results = db_query('SELECT c.entity_id, c.field_siteid_value FROM {field_data_field_siteid} c WHERE c.field_siteid_value = :id', array(':id' => $siteId));

	foreach($results as $result){
		$id = $result->entity_id;
	}
	return $id;
}

function uwwtd_file_upload($file, $ext){
	$path = $file->uri;
	$name = explode('.', $file->filename);
	$filename = $name[0];
	$filename = preg_replace("/[^a-zA-Z0-9]/", "", $filename);
	$filename = strtolower($filename.'_'.$file->timestamp).''.$ext;
	$file_temp = file_get_contents($path);
	$file_temp = file_save_data($file_temp, 'public://data_sources/' . $filename, FILE_EXISTS_RENAME);
	return $file_temp;
}

function uwwtd_set_warning($i, $emes){
	$_SESSION['uwwtd_warnings'][] = array($i, $emes);
}

function uwwtd_csv_to_array($filename='', $delimiter=','){
	
	// Remove bom
	$string = file_get_contents($filename);
	$string = uwwtd_removeBOM($string);

	file_put_contents($filename, $string);

    if(!file_exists($filename) || !is_readable($filename)) return FALSE;

    $header = NULL;
    $data = array();
    if (($handle = fopen($filename, 'r')) !== FALSE)
    {
        while (($row = fgetcsv($handle, 10000, $delimiter)) !== FALSE)
        {
            if(!$header)
                $header = $row;
            else
                $data[] = array_combine($header, $row);
        }
        fclose($handle);
    }
    return $data;
}

function uwwtd_xml_to_array($link){
	$string = file_get_contents($link);
	$deXml = simplexml_load_string($string);
    $deJson = json_encode($deXml);
    $xml_array = json_decode($deJson,TRUE);
    return $xml_array;
}

function uwwtd_removeBOM($str=""){
    if(substr($str, 0,3) == pack("CCC",0xef,0xbb,0xbf)) {
            $str=substr($str, 3);
    }
    return $str;
}

function uwwtd_getCodeLov($code, $i){
	$ref = array(
		"1"=>'fail',
		"2"=>'pass',
		"3"=>'Production of alcohol and alcoholic beverages',
		"4"=>'Breweries',
		"5"=>'Manufacture and bottling of soft drinks',
		"6"=>'Manufacture of aninmal feed from plant products',
		"7"=>'Fish-processing industry',
		"8"=>'Manufacture of fruit and vegetable products',
		"9"=>'Manufacture of gelatine and glue from hides, skin and bones',
		"10"=>'Malt-houses',
		"11"=>'Meat industry',
		"12"=>'Milk-processing',
		"13"=>'Potato-processing',
		"14"=>'calculated',
		"15"=>'estimated',
		"16"=>'Document (*.doc)',
		"17"=>'GIS-data',
		"18"=>'Document (*.pdf)',
		"19"=>'Missing Culture Info',
		"20"=>'Data type is not clear',
		"21"=>'Please fill-in at least one value',
		"22"=>'Set to deault value',
		"23"=>'Different Report Period ID (between manual data input and data i',
		"24"=>'Field has to have a value',
		"25"=>'Error whilst inserting normal area',
		"26"=>'Value > 100%',
		"27"=>'Invalid code',
		"28"=>'Invalid data format in the import file.',
		"29"=>'ID  is not correct',
		"30"=>'Invalid link',
		"31"=>'Invalid status',
		"32"=>'No valid value',
		"33"=>'Value is master data',
		"34"=>'No Sensitive Area is linked to this Catchment Area of Sensitive',
		"35"=>'Value is not Y (yes) or N (no)',
		"36"=>'No Catchment Area. Indication of Sensitive Area is not foreseen',
		"37"=>'Field not numeric',
		"38"=>'%-values do not add up to 100%',
		"39"=>'Table does not contain data sets',
		"40"=>'Value is not a valid date',
		"41"=>'No discharge point is linked to this UWWTP',
		"42"=>'Data set is not available',
		"43"=>'The selected Sensitive Area does not exist in the inventory of',
		"44"=>'No UWWTP is linked to this agglomeration',
		"45"=>'ID is not distinct',
		"46"=>'Not a valid date',
		"47"=>'Reference is not valid',
		"48"=>'%-value is out of range 0-100',
		"49"=>'Report period does not correspond to the selected report period',
		"50"=>'Value is required',
		"51"=>'Please select one value',
		"52"=>'Error message of the system',
		"53"=>'Text too long',
		"54"=>'File comprises too many data sets',
		"55"=>'Text too short',
		"56"=>'Infiltration',
		"57"=>'Irrigation',
		"58"=>'other',
		"59"=>'No',
		"60"=>'Yes',
		"61"=>'National Regulation in place',
		"62"=>'calculated',
		"63"=>'estimated',
		"64"=>'measured',
		"65"=>'Austria',
		"66"=>'Belgium',
		"67"=>'Bulgaria',
		"68"=>'Cyprus',
		"69"=>'Czech Republic',
		"70"=>'Germany',
		"71"=>'Denmark',
		"72"=>'Estonia',
		"73"=>'Spain',
		"74"=>'Finland',
		"75"=>'France',
		"76"=>'Greece',
		"77"=>'Hungary',
		"78"=>'Ireland',
		"79"=>'Italy',
		"80"=>'Lithuania',
		"81"=>'Luxembourg',
		"82"=>'Latvia',
		"83"=>'Malta',
		"84"=>'Netherlands',
		"85"=>'Norway',
		"86"=>'Poland',
		"87"=>'Portugal',
		"88"=>'Romania',
		"89"=>'Sweden',
		"90"=>'Slovenia',
		"91"=>'Slovakia',
		"92"=>'United Kingdom',
		"93"=>'fail',
		"94"=>'not relevant',
		"95"=>'pass',
		"96"=>'Art. 5(4) area',
		"97"=>'Art. 5(8) area (entire Member State)',
		"98"=>'Catchment in the sense of Art. 5(5)',
		"99"=>'Less sensitive area',
		"100"=>'Normal area',
		"101"=>'Sensitive area',
		"102"=>'c) both a&b',
		"103"=>'a) combined',
		"104"=>'b) separeted',
		"105"=>'inactive',
		"106"=>'active',
		"107"=>'No',
		"108"=>'Yes',
		"109"=>'No combined system',
		"110"=>'National Regulation in place',
		"111"=>'Collecting system is connected to UWWTP',
		"112"=>'Collecting system is not connected to UWWTP',
		"113"=>'to coastal water',
		"114"=>'to estuary',
		"115"=>'to freshwater',
		"116"=>'on land (catchment of coastal water)',
		"117"=>'on land (catchment of freshwater and / or estuary)',
		"118"=>'to coastal water',
		"119"=>'to estuary',
		"120"=>'on land (catchment of coastal water)',
		"121"=>'on land (catchment of freshwater and / or estuary)',
		"122"=>'No',
		"123"=>'Yes',
		"124"=>'No',
		"125"=>'Yes',
		"126"=>'Not relevant',
		"127"=>'fully treated effluent- dry and wet weather, partially treated s',
		"128"=>'fully treated effluent- dry and wet weather',
		"129"=>'fully treated effluent- dry weather;',
		"130"=>'tempinactive',
		"131"=>''
	);
	if(array_key_exists($code, $ref)){
		return $ref[$code];
	}
	else if($code == ''){
		$wmes = t('WARNING : Lov code not found');
		uwwtd_set_warning($i, $wmes);
		return '';
	}
	else{
		$wmes = t('WARNING : Lov code not found ').': '.$code;
		uwwtd_set_warning($i, $wmes);
		return '';
	}	
}

function uwwtd_coord_transform($lat, $long){

	$lat = str_replace(",",".",$lat);
	$long = str_replace(",",".",$long);

	if($lat == '' || $long == '') return false;

	$sql = "SELECT ST_Transform(ST_GeomFromText('POINT(".$long." ".$lat.")', 4258),4326) AS geo";
	$result = db_query($sql);
	foreach ($result as $record) {
  		$geo = $record->geo;
	}

	// create array
	$table = array();
	$sql = "SELECT ST_AsBinary('".$geo."') as bin, 'point' as pt, ST_Y('".$geo."') as geoy, ST_X('".$geo."') as geox, ST_GeoHash('".$geo."'::geometry, 16) as geohash";
	$result = db_query($sql);
	foreach ($result as $record) {
  		$table['bin'] = $record->bin;
  		$table['point'] = $record->pt;
  		$table['geoy'] = $record->geoy;
  		$table['geox'] = $record->geox;
  		$table['geohash'] = $record->geohash;
	}

	return $table;
	//dsm($table);
}

function uwwtd_coord_transform_polygon($parts){

	$sql = "SELECT ST_Transform(ST_GeomFromText('POLYGON((";

	$numParts = count($parts);
	$count = 0;

	foreach($parts as $part){

		$count++;

		$lat = str_replace(",",".",$part['x']);
		$long = str_replace(",",".",$part['y']);

		$sql .= $lat.' '.$long;
		if($count < $numParts) $sql .= ',';

	}

	$sql .= "))', 4258),4326) AS geo";

	$result = db_query($sql);
	foreach ($result as $record) {
  		$geo = $record->geo;
  		//dsm($geo);
	}

	// create array
	$table = array();
	$sql = "SELECT ST_AsBinary('".$geo."') as bin, ST_GeoHash('".$geo."'::geometry, 16) as geohash";
	$result = db_query($sql);
	foreach ($result as $record) {
  		$table['bin'] = $record->bin;
  		$table['polygon'] = 'polygon';
  		//$table['geoy'] = $record->geoy;
  		//$table['geox'] = $record->geox;
  		$table['geohash'] = $record->geohash;
	}
	return $table;
}

function uwwtd_coord_transform_multipolygon($parts){

	$sql = "SELECT ST_Transform(ST_GeomFromText('MULTIPOLYGON(";

	$numParts = count($parts);
	$count = 0;

	foreach($parts as $part){

		$count++;

		$sql .= "((";
		$innerParts = count($part['points']);
		$innerCount = 0;
		foreach($part['points'] as $inner){
			$innerCount++;
			$lat = str_replace(",",".",$inner['x']);
			$long = str_replace(",",".",$inner['y']);

			$sql .= $lat.' '.$long;
			if($innerCount < $innerParts) $sql .= ',';
		}
		$sql .= ")";
		if($count < $numParts) $sql .= "), ";
		else $sql .= ")";

	}

	$sql .= ")', 4258),4326) AS geo";

	$result = db_query($sql);
	foreach ($result as $record) {
  		$geo = $record->geo;
	}

	// create array
	$table = array();
	$sql = "SELECT ST_AsBinary('".$geo."') as bin, ST_GeoHash('".$geo."'::geometry, 16) as geohash";
	$result = db_query($sql);
	foreach ($result as $record) {
  		$table['bin'] = $record->bin;
  		$table['polygon'] = 'multipolygon';
  		//$table['geoy'] = $record->geoy;
  		//$table['geox'] = $record->geox;
  		$table['geohash'] = $record->geohash;
	}
	return $table;
}

function uwwtd_check_text($text){

	// check format
    $text = trim($text);
    $text = check_plain($text);
    
    //return finished value
    return $text;
}

function uwwtd_check_bool($bool, $timestamp, $index = null, $node = null){
	// check format
    $bool = trim($bool);
	$bool = check_plain($bool);
	
	if(($bool == "False")|| ($bool == "false")|| ($bool == "0"))
	{
		$bool = 0;
		return $bool;
	}else if(($bool === "True")|| ($bool === "true")|| ($bool == "1"))
	{
		$bool = 1;
		return $bool;
	}else{
		uwwtd_add_error(2, 4, 'The input data is not in boolean format (true or false).', $timestamp, $index, $node);
		return false;
		}
}

function uwwtd_check_chiffre($chif, $timestamp, $index = null, $node = null){
	//chiffre > 0
	if(is_numeric($chif))
	{
		$chif = intval($chif);
		return $chif;	
	}else{
		uwwtd_add_error(2, 4, 'The input data is not in digital format.', $timestamp, $index, $node);
		return false;
		}
}

function uwwtd_check_int($int, $timestamp, $index = null, $node = null){
	
	if(is_numeric($int))
	{	
		$int = intval($int);
		if(is_int($int))
		{
			return $int;	
		}else{
			uwwtd_add_error(2, 4, 'The input data is not in integer format.', $timestamp, $i, $node);
			return false;
		}
	}
}

function uwwtd_check_pourc($pourc, $timestamp, $index = null, $node = null){
	if(is_numeric($pourc))
	{
		if(($pourc >= "0") && ($pourc <="100")){
			$pourc = intval($pourc);
			return $pourc;	
		}else{
			uwwtd_add_error(2, 4, 'The number must be between 0 and 100.', $timestamp, $index, $node);
			return false;
		}
	}else{
		uwwtd_add_error(2, 4, 'The input data is not in digital format.', $timestamp, $index, $node);
		return false;
	}
}

function uwwtd_check_date($indate, $timestamp, $index = null, $node = null){
	$indate = trim($indate);

	// if the date is empty return false
	if($indate == ''){
		uwwtd_add_error(1, 4, 'Date found to be empty.', $timestamp, $index, $node);
		return false;
	}

	// remove alternate date spacing
	$indate = str_replace("/", "-", $indate);
	$indate = str_replace(".", "-", $indate);
	if(strpos($indate,'+') !== false){
		$indate = substr($indate, 0, strpos($indate, "+"));
	}

	// check date format correct
	$dateTypeEU = DateTime::createFromFormat('Y-m-d', $indate);
	$dateTypeUS = DateTime::createFromFormat('d-m-Y', $indate);
	if($dateTypeEU == FALSE && $dateTypeUS == FALSE){
		uwwtd_add_error(1, 4, 'Unknown date format: '.$indate, $timestamp, $index, $node);
		return false;
	}

	$fullDate = explode('-', $indate);

	// here we change from eu to us format
	if(strlen($fullDate[0]) != 4){
		if(strlen($fullDate[2]) == 4){
			$temp = $fullDate;
			$fullDate[0] = $temp[2];
			$fullDate[2] = $temp[0];
		}
	}

	// here we check format length and add zeros if needed
	if(strlen($fullDate[0]) == 1) $fullDate[0] = '0'.$fullDate[0];
	if(strlen($fullDate[1]) == 1) $fullDate[1] = '0'.$fullDate[1];

	// here we check the date exists
	if(checkdate($fullDate[1], $fullDate[2], $fullDate[0]) == false){
		uwwtd_add_error(1, 4, 'Entered date doesn\'t exist: '.$fullDate[0].'-'.$fullDate[1].'-'.$fullDate[2], $timestamp, $index, $node);
	}

	// we change to drupal friendly format
	$correctDate = $fullDate[0].'-'.$fullDate[1].'-'.$fullDate[2].'T00:00:00';

	return (string)$correctDate;
}

function uwwtd_check_coordinate($coord, $type, $timestamp, $index = null, $node = null){
	$coord = trim($coord);

	$coord = str_replace(",",".",$coord);

	// In europe a coordinate will never have more than 2 numbers before the decimal. This corrects coordinates that have misplaced decimals
	$first = mb_substr($coord, 0, 1, 'utf-8');
	if($first != '-') $first = '';
	$coord = str_replace("-", "", $coord);
	$exploded = explode('.', $coord);
	if(strlen($exploded[0]) > 2){
		$after = substr($exploded[0], 2);
		$coord = mb_substr($coord, $exploded[0], 2, 'utf-8').'.'.$after.$exploded[1];
		uwwtd_add_error(1, 4, 'Decimal point for this coordinate was inproperly placed', $timestamp, $index, $node);
	}

	// In europe the bounding box for latitude is defined between 24 - 80 
	if($type == 'latitude'){
		if((float)$coord < 24 || (float)$coord > 80){
			uwwtd_add_error(1, 4, 'Latitude not found to be in Europe', $timestamp, $index, $node);
		}
	}

	// In europe the bounding box for longitude is defined between -11 - 34 
	if($type == 'longitude'){
		if((float)$coord < -11 || (float)$coord > 34){
			uwwtd_add_error(1, 4, 'Longitude not found to be in Europe', $timestamp, $index, $node);
		}
	}

	return $coord;
}

function uwwtd_article3_compliance($node){

	// only send nodes of type 'agglomeration' here
	//check if aggGenerated exists
	if(!isset($node->field_agggenerated['und'][0]['value'])){
		return 'NI';
	}

	//============================= CAS DES PAYS
	$country = uwwtd_country_cases($node);
	if($country[0] == 'NR') {
		return array($country[0], $country[1]);
	}
	//============================= FIN CAS DES PAYS

	// check if aggGenerated over 2000
	if($node->field_agggenerated['und'][0]['value'] < 2000){
		$art3comp = 'NR';
		$reason = 'Article 3 compliance found to be non relevant because the agglomeration has a population of less than 2 000.';
		return array($art3comp, $reason);
	}
	else{
		// check variables that we need exist
		if(!isset($node->field_aggpercwithouttreatment['und'][0]['value']) || !isset($node->field_aggc2['und'][0]['value'])){
			$art3comp = 'NI';
			$reason = 'Article 3 compliance found to have no information because no data could be found on the "percentage of waste dischaged without treatment" or "treated through IAS".';
			return array($art3comp, $reason);
		}
		else{
			// get and calculate variables
			// var1 = Sum "Not collected in collecting system and not adressed by IAS"
			$var1 = ($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggpercwithouttreatment['und'][0]['value'];
			// var2 = Sum "Adressed under IAS"
			$var2 = ($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggc2['und'][0]['value'];
			// var3 = % "Not collected in collecting system and not adressed by IAS"
			$var3 = $node->field_aggpercwithouttreatment['und'][0]['value'];
			// var4 = % "Adressed under IAS"
			$var4 = $node->field_aggc2['und'][0]['value'];

			// start calculations
			if(($var1 + $var2) <= 2000 && ($var3 + $var4) <= 2){
				$art3comp = 'C';
				$reason = 'Article 3 compliance found to be compliant because the amount of "waste discharged without treatment" and "treated through IAS" is equal or less than 2 000. Also the percentage of "waste discharged without treatment" and "treated through IAS" is equal or less than 2% of total waste.';
				return array($art3comp, $reason);
			}
			else{
				if($var1 > 2000 || $var3 > 2){
					if($var2 > 1000 || $var4 > 2){
						$art3comp = 'AddQC';
						$reason = 'Article 3 compliance found to be additional questionnable compliance because the amount of "waste discharged without treatment" is more than 2 000 or the percentage of "waste discharged without treatment" is more than 2% of total waste. Also the amount of waste "treated through IAS" is more than 1 000 or the percentage of waste "treated through IAS" is more than 2% of total waste.';
						return array($art3comp, $reason);
					}
					else{
						$art3comp = 'NC';
						$reason = 'Article 3 compliance found to be non compliant because the amount of "waste discharged without treatment" is more than 2 000 or the percentage of "waste discharged without treatment" is more than 2% of total waste. Also the amount of waste "treated through IAS" is equal or less than 1 000 or the percentage of waste "treated through IAS" equal or less than 2% of total waste.';
						return array($art3comp, $reason);
					}
				}
				else{
					$art3comp = 'QC';
					$reason = 'Article 3 compliance found to be questionnable compliance because the amount of "waste discharged without treatment" is equal or less than 2 000 or the percentage of "waste discharged without treatment" equal or less than 2% of total waste.';
					return array($art3comp, $reason);
				}
			}
		}
	}
}

function uwwtd_article4_compliance($node, $rcaType, $waterBodyTypes, $art3Comp){

	//============================= CAS DES PAYS
	$country = uwwtd_country_cases($node);
	if($country[0] == 'NR') {
		return array($country[0], $country[1]);
	}
	//============================= FIN CAS DES PAYS

	// Check population data exists
	if(!isset($node->field_agggenerated['und'][0]['value'])){
		$art4comp = 'NI';
		$reason = 'Article 4 compliance found to have no information because the data on the population is missing.';
		return array($art4comp, $reason);
	}

	// removes all agglos with less than 2000 pe
	if($node->field_agggenerated['und'][0]['value'] < 2000){
		$art4comp = 'NR';
		$reason = 'Article 4 compliance found to be non relevant because the agglomeration has a population of less than 2000 pe.';
		return array($art4comp, $reason);
	}

	// removes all agglos with 2000 to 10000 pe discharging into CW
	if($node->field_agggenerated['und'][0]['value'] >= 2000 && $node->field_agggenerated['und'][0]['value'] < 10000){
		$coastal = 0;
		foreach($waterBodyTypes as $waterBodyType){
			if($waterBodyType == 'CW'){
				$art4comp = 'NR';
				$reason = 'Article 4 compliance found to be non relevant because the agglomeration has a population between 2000 - 10000 pe and discharges into coastal waters.';
				return array($art4comp, $reason);
			}
		}
	}

	// removes all agglos discharging into Less Sensitive Areas
	if($rcaType == 'LSA'){
		$art4comp = 'NR';
		$reason = 'Article 4 compliance found to be non relevant because the agglomeration discharges into a Less Sensitive Area.';
		return array($art4comp, $reason);
	}

	// Check percentage of load collected in collecting system is set
	if(!isset($node->field_aggc1['und'][0]['value'])){
		$art4comp = 'NI';
		$reason = 'Article 4 compliance found to have no information because the data on the percentage of load collected in collecting system is missing.';
		return array($art4comp, $reason);
	}

	// percentage of load collected in collecting system
	$percLoadCollecting = $node->field_aggc1['und'][0]['value'];

	// quantity of load collected in collecting system
	$quantLoadCollecting = ($node->field_agggenerated['und'][0]['value'] * $node->field_aggc1['und'][0]['value']) / 100;

	// Check that there is information on connected treatment plants
	if(!isset($node->field_agguwwliste['und'])){
		$art4comp = 'NI';
		$reason = 'Article 4 compliance found to have no information because the data on the connected treatment plants is missing.';
		return array($art4comp, $reason);
	}

	// Get information on all connected treatment plants
	$listUwws = array();
	foreach($node->field_agguwwliste['und'] as $agguwwlist){
		$uww = node_load($agguwwlist['nid']);
		$uwwsec = $uww->field_uwwsecondarytreatment['und'][0]['value'];
		$uwwbod5 = $uww->field_uwwbod5perf['und'][0]['value'];
		$uwwcod = $uww->field_uwwcodperf['und'][0]['value'];

		// get reste of information from node that collected joint information
		$query = db_select('node', 'n');
	    $query->join('field_data_field_agglo_uww_agglo', 'a', 'a.entity_id = n.nid');
	    $query->join('field_data_field_agglo_uww_uww', 'u', 'u.entity_id = n.nid');
	    $query->fields('n',array('title','created'))
	    ->condition('a.entity_id', $node->nid, '=')
	    ->condition('u.entity_id', $uww->nid, '=')
	    ->fields('n',array('nid'));

	    $result = $query->execute();

	    while($record = $result->fetchAssoc()) {
	        $agguww = node_load($record['nid']);
	        $uwwperccollec = $agguww->field_agglo_uww_perc_ent_uw['und'][0]['value'];
	        $uwwquantcollec = ($quantLoadCollecting * $uwwperccollec) / 100;
	    }
	    $listUwws[] = array(
	    	'node' => $uww,
	    	'secondary' => $uwwsec,
	    	'bod5' => $uwwbod5,
	    	'cod' => $uwwcod,
	    	'perc' => $uwwperccollec,
	    	'quant' => $uwwquantcollec
	    );
	}

	// Cases where art 3 compliance = C or QC
	if($art3Comp == 'C' || $art3Comp == 'QC'){
		// specific case where 1% or less or less than 2000pe of the load collected in a collecting system does not receive secondary treatment
		$totalpercnotsec = '';
		$totalquantnotsec = '';
		foreach($listUwws as $connecteduww){
			if($connecteduww['secondary'] == '0'){
				$totalpercnotsec = $totalpercnotsec + $connecteduww['perc'];
				$totalquantnotsec = $totalquantnotsec + $connecteduww['quant'];
			}
		}
		if($totalpercnotsec != '' && $totalquantnotsec != ''){
			if($totalpercnotsec <= 1 && $totalquantnotsec <= 2000){
				$art4comp = 'RNC';
				$reason = 'Article 4 compliance found to be non relative non compliance because of the specific case in the article 4 where the article 3 was either C or QC and where 1% or less or less than 2000pe of the load collected in a collecting system does not receive secondary treatment.';
				return array($art4comp, $reason);
			}
			else{
				$art4comp = 'NC';
				$reason = 'Article 4 compliance found to be non non compliant because of the specific case in the article 4 where the article 3 was either C or QC and where 1% or more or more than 2000pe of the load collected in a collecting system does not receive secondary treatment.';
				return array($art4comp, $reason);
			}
		}


		// Specific case where percentage of load collected in collecting system = 0 and art3 compliance = QC
		if($percLoadCollecting == '0' && $art3Comp == 'QC'){
			$node->field_aggart5compliance['und'][0]['value'] = 'NR';
			$node->field_article_5_compliance_expli['und'][0]['value'] = 'Article 5 found to be non relevant because of the special case in article 4 where the percentage of load collected in collecting system = 0% and the article 3 compliance was QC.';
			node_save($node);
			$art4comp = 'NR';
			$reason = 'Article 4 compliance found to be non relevant because of the special case in article 4 where the percentage of load collected in collecting system = 0% and the article 3 compliance was QC.';
			return array($art4comp, $reason);
		}
	}

	// general checks
	// check that secondary treatment is applied
	foreach($listUwws as $connecteduww){
		if($connecteduww['secondary'] == '0'){
			$art4comp = 'NC';
			$reason = 'Article 4 compliance found to be not compliant because one or more of the treatment plants connected to the agglomeration does not have secondary treatment.';
			return array($art4comp, $reason);
		}
	}

	// check that bod5 and cod passed their tests in all of the treatment plants connected to the agglomeration
	foreach($listUwws as $connecteduww){
		if($connecteduww['bod5'] == 'F' || $connecteduww['cod'] == 'F'){
			$art4comp = 'NC';
			$reason = 'Article 4 compliance found to be not compliant because one or more of the treatment plants connected to the agglomeration failed in either bod5 or cod performance.';
			return array($art4comp, $reason);
		}
	}

	// default compliance for article 4
	$art4comp = 'C';
	$reason = 'Article 4 compliance found to be compliant.';
	return array($art4comp, $reason);
}

function uwwtd_article5_compliance($node, $rcaType, $rcaANitro, $rcaAPhos, $art3Comp, $art4Comp){
	
	//============================= CAS DES PAYS
	$country = uwwtd_country_cases($node);
	if($country[0] == 'NR') {
		return array($country[0], $country[1]);
	}
	//============================= FIN CAS DES PAYS

	if(!isset($node->field_agggenerated['und'][0]['value']) || $rcaType == ''){
		$art5comp = 'NR';
		$reason = 'Article 5 compliance found to be non relevant because no data was found for the agglomeration population or the sensitive area type.';
		return array($art5comp, $reason);
	}
	else{
		$aggGen = $node->field_agggenerated['und'][0]['value'];
		$comp = 'C';
		$reason = 'Article 5 compliance found to be compliant.';

		// General article 5 compliance
		// The agglomeration needs to be larger than 10000 pe and in a receiving area of type SA or CSA
		if($aggGen >= 10000 && ($rcaType == 'SA' || $rcaType == 'CSA' || $rcaType == 'A58')){
			
			// We need to look at each station connected to the agglomeration
			foreach($node->field_agguwwliste['und'] as $uww){
				$uwwtp = node_load($uww['nid']);
				
				// If the receiving area requires Nitrogen treatment then the station must pass Nitrogen performance
				if($rcaANitro == true){
					if(isset($uwwtp->field_uwwntotperf['und'][0]['value'])){
						if($uwwtp->field_uwwntotperf['und'][0]['value'] != 'P'){
							$comp = 'NC';
							$reason = 'Article 5 compliance found to be non compliant because the nitrogen treatment performance was not passed.';						
						}
					}
					else{
						$comp = 'NR';
						$reason = 'Article 5 compliance found to be NR because nitrogen treatment performance was not provided.';
					}
				}
				
				// If the receiving area requires Phosphorus treatment then the station must pass Phosphorus performance
				if($rcaAPhos == true){
					if(isset($uwwtp->field_uwwptotperf['und'][0]['value'])){
						if($uwwtp->field_uwwptotperf['und'][0]['value'] != 'P'){
							$comp = 'NC';
							$reason = 'Article 5 compliance found to be non compliant because the phosphorus treatment performance was not passed.';						
						}
					}
					else{
						$comp = 'NR';
						$reason = 'Article 5 compliance found to be NR because phosphorus treatment performance was not provided.';
					}			
				}
				
				// If the receiving area apparently requires neither Phosphorus nor Nitrogen treatment then the station must pass other performance
				if($rcaANitro != true && $rcaAPhos != true){
					if(isset($uwwtp->field_uwwotherperf['und'][0]['value'])){
						if($uwwtp->field_uwwotherperf['und'][0]['value'] != 'P'){
							$comp = 'NC';
							$reason = 'Article 5 compliance found to be non compliant because the other treatment performance was not passed.';						
						}
					}
					else{
						$comp = 'NR';
						$reason = 'Article 5 compliance found to be NR because the other treatment performance was not provided and no N or P required.';
					}				
				}
			}
		}
		else{
			$comp = 'NR';
			$reason = 'Article 5 compliance found to be non relevant because the agglomeration population is less than 10 000 or is not situated in a sensitive area.';
		}

		// Criterion b
		if($comp == 'C' && $node->field_aggcritb['und'][0]['value'] == 1 && ($rcaType == 'SA' || $rcaType == 'CSA' || $rcaType == 'A58') && $aggGen > 10000){
			foreach($node->field_agguwwliste['und'] as $uww){
				$uwwtp = node_load($uww['nid']);
				if(
					!isset($uwwtp->field_uwwsecondarytreatment['und'][0]['value']) || 
					!isset($uwwtp->field_uwwbod5perf['und'][0]['value']) ||
					!isset($uwwtp->field_uwwcodperf['und'][0]['value']) ||
					!isset($uwwtp->field_uwwnremoval['und'][0]['value']) ||
					$uwwtp->field_uwwsecondarytreatment['und'][0]['value'] == '0' || 
					$uwwtp->field_uwwbod5perf['und'][0]['value'] == 'F' || 
					$uwwtp->field_uwwcodperf['und'][0]['value'] == 'F' || 
					$uwwtp->field_uwwnremoval['und'][0]['value'] == '0'
				){
					$comp = 'NC';
					$reason = 'Article 5 compliance found to be non compliant because the criterium b is necessary and either the secondary treatment, the BOD5 performance or the COD performance was not met.';
				}
			}
		}
		// Criterion ca
		if($comp == 'C' && $node->field_aggcritca['und'][0]['value'] == 1 && $rcaType == 'SA' && $aggGen > 10000){
			foreach($node->field_agguwwliste['und'] as $uww){
				$uwwtp = node_load($uww['nid']);
				if(
					!isset($uwwtp->field_uwwsecondarytreatment['und'][0]['value']) ||
					!isset($uwwtp->field_uwwbod5perf['und'][0]['value']) || 
					!isset($uwwtp->field_uwwcodperf['und'][0]['value']) ||
					$uwwtp->field_uwwsecondarytreatment['und'][0]['value'] == '0' || 
					$uwwtp->field_uwwbod5perf['und'][0]['value'] == 'F' || 
					$uwwtp->field_uwwcodperf['und'][0]['value'] == 'F'
				){
					$comp = 'NC';
					$reason = 'Article 5 compliance found to be non compliant because the criterium c-a is necessary and either the secondary treatment, the BOD5 performance or the COD performance was not met.';
				}
				else{
					if(
						(!isset($uwwtp->field_uwwchlorination['und'][0]['value']) || $uwwtp->field_uwwchlorination['und'][0]['value'] == '0') &&
						(!isset($uwwtp->field_uwwozonation['und'][0]['value']) || $uwwtp->field_uwwozonation['und'][0]['value'] == '0') &&
						(!isset($uwwtp->field_uwwsandfiltration['und'][0]['value']) || $uwwtp->field_uwwsandfiltration['und'][0]['value'] == '0') &&
						(!isset($uwwtp->field_uwwmicrofiltration['und'][0]['value']) || $uwwtp->field_uwwmicrofiltration['und'][0]['value'] == '0') &&
						(!isset($uwwtp->field_uwwuv['und'][0]['value']) || $uwwtp->field_uwwuv['und'][0]['value'] == '0')
					){
						$comp = 'NC';
						$reason = 'Article 5 compliance found to be non compliant because the criterium c-a is necessary and either the chlorination, ozonation, sand filtration or micro filtration treatment is not in place.';
					}
				}
			}
		}
		// criterion cb
		if($comp == 'C' && $node->field_aggcritcb['und'][0]['value'] == '1' && ($rcaType == 'SA' || $rcaType == 'A58') && $aggGen > 10000){
			foreach($node->field_agguwwliste['und'] as $uww){
				$uwwtp = node_load($uww['nid']);
				if(
					!isset($uwwtp->field_uwwsecondarytreatment['und'][0]['value']) || 
					!isset($uwwtp->field_uwwbod5perf['und'][0]['value']) ||
					!isset($uwwtp->field_uwwcodperf['und'][0]['value']) ||
					$uwwtp->field_uwwsecondarytreatment['und'][0]['value'] == '0' ||  
					$uwwtp->field_uwwbod5perf['und'][0]['value'] == 'F' || 
					$uwwtp->field_uwwcodperf['und'][0]['value'] == 'F' || 
					$uwwtp->field_uwwnremoval['und'][0]['value'] == '0'
				){
					$comp = 'NC';
					$reason = 'Article 5 compliance found to be non compliant because the criterium c-b is necessary and either the secondary treatment, the BOD5 performance or the COD performance was not met.';
				}
			}
		}
	
		return array($comp, $reason);
	}		
}

function uwwtd_station_compliance_algo($node, $timestamp){
	// Start compliance check
	$comps = array();
	// Here we setup our variable that will take all the levels or treatment required and put them into an array
	$reqtreatments = array();
	$typeTreat = 'NR';

	// Get all data needed to calculate compliance
	// Get aglo data first
	$agglos = array();
	if(isset($node->field_uwwaggliste['und'])){
		$maxgen = 0;
		foreach($node->field_uwwaggliste['und'] as $agglist){
			$agg = node_load($agglist['nid']);
			//dsm($agg->field_agggenerated['und'][0]['value']);
			if($agg->field_agggenerated['und'][0]['value'] > $maxgen) $maxgen = $agg->field_agggenerated['und'][0]['value'];
			//dsm($maxgen);
			$agglos[] = array(
				'nid' => $agg->nid,
				'title' => $agg->title,
				'generated' => $agg->field_agggenerated['und'][0]['value']
			);
		}
	}
	if(count($agglos) == 0){
		$comp = 'NI';
		$reason = 'Station found to have no information on agglomerations';
		uwwtd_add_error(1, 3, 'Station found to have no information on agglomerations.', $timestamp, null, $node);
		$truereqtreatment = 'NI';
		$treatment = '0';
		$performance = '0';
		return (array($truereqtreatment, $comp, $reason, $treatment, $performance));
	}

	// Get dcp data
	$dcps = array();
	if(isset($node->field_uwwdcpliste['und'])){
		foreach($node->field_uwwdcpliste['und'] as $dcplist){
			$dcp = node_load($dcplist['nid']);
			// get n and p rca data
			$nrec = '0';
			$prec = '0';
			/* Cas des pays avec l'ensemble du pays en zone sensible */
			$repcode = $node->field_repcode['und'][0]['value'];
			$repcode = str_replace('_', '-', $repcode);
			$boom = explode('-', $repcode);
			$boom = explode(' ', $boom[0]);
			$rep = $boom[0];

			$rcaTypeid = $dcp->field_rcatype['und'][0]['value'];
			if($rep == 'LT'){
				$rcaTypeid = 'SA';
			}
			foreach($dcp->field_dcprcaliste['und'] as $rcalist){
				$rca = node_load($rcalist['nid']);
				if($rca->field_rcaanitro['und'][0]['value'] == '1') $nrec = '1';
				if($rca->field_rcaaphos['und'][0]['value'] == '1') $prec = '1';
				if($rca->field_rca_parameter_n['und'][0]['value'] == '1') $nrec = '1';
				if($rca->field_rca_parameter_p['und'][0]['value'] == '1') $prec = '1';
			}
			$dcps[] = array(
				'nid' => $dcp->nid,
				'title' => $dcp->title,
				'waterbody' => $dcp->field_dcpwaterbodytype['und'][0]['value'],
				'rcatype' => $rcaTypeid,
				'rcan' => $nrec,
				'rcap' => $prec
			);
		}
	}
	
	// If we cant find any discharge points attached to this station then we return no information to the user
	if(count($dcps) == 0){
		$comp = 'NI';
		$reason = 'Station found to have no information on discharge points';
		uwwtd_add_error(1, 3, 'Station found to have no information on discharge points.', $timestamp, null, $node);
		$truereqtreatment = 'NI';
		$treatment = '0';
		$performance = '0';
		return (array($truereqtreatment, $comp, $reason, $treatment, $performance));
	}

	$truereqtreatment = uwwtd_get_required_treatment($dcps, $maxgen);

	if($truereqtreatment == 'NI'){
		$comp = 'NI';
		$treatment = '1';
		$performance = '1';
		$reason = 'Station found to be missing information required to calculate proper compliance';
	}

	if($truereqtreatment == 'NR'){
		$comp = 'NR';
		$treatment = '1';
		$performance = '1';
		$reason = 'Station found to be non relevant';
	}

	if($truereqtreatment == 'P'){
		if(isset($node->field_uwwprimarytreatment['und'][0]['value']) && $node->field_uwwprimarytreatment['und'][0]['value'] == '1'){
			$treatment = '1';
			if(isset($node->field_uwwcodperf['und'][0]['value']) && isset($node->field_uwwbod5perf['und'][0]['value']) && $node->field_uwwcodperf['und'][0]['value'] == 'P' && $node->field_uwwbod5perf['und'][0]['value'] == 'P'){
				$comp = 'C';
				$reason = 'Station found to be compliant because primary treatment is in place and performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because primary treatment is in place but performance is not met';
				$performance = '0';
			}
		}
		else{
			$treatment = '0';
			if(isset($node->field_uwwcodperf['und'][0]['value']) && isset($node->field_uwwbod5perf['und'][0]['value']) && $node->field_uwwcodperf['und'][0]['value'] == 'P' && $node->field_uwwbod5perf['und'][0]['value'] == 'P'){
				$comp = 'C';
				$reason = 'Station found to be compliant because primary performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because primary treatment is not in place and performance is not met';
				$performance = '0';
			}
		}
	}

	if($truereqtreatment == 'S'){
		if(isset($node->field_uwwsecondarytreatment['und'][0]['value']) && $node->field_uwwsecondarytreatment['und'][0]['value'] == '1'){
			$treatment = '1';
			if(isset($node->field_uwwcodperf['und'][0]['value']) && isset($node->field_uwwbod5perf['und'][0]['value']) && $node->field_uwwcodperf['und'][0]['value'] == 'P' && $node->field_uwwbod5perf['und'][0]['value'] == 'P'){
				$comp = 'C';
				$reason = 'Station found to be compliant because secondary treatment is in place and performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because secondary treatment is in place but performance is not met';
				$performance = '0';
			}
		}
		else{
			$treatment = '0';
			if(isset($node->field_uwwcodperf['und'][0]['value']) && isset($node->field_uwwbod5perf['und'][0]['value']) && $node->field_uwwcodperf['und'][0]['value'] == 'P' && $node->field_uwwbod5perf['und'][0]['value'] == 'P'){
				$comp = 'C';
				$reason = 'Station found to be compliant because secondary performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because secondary treatment is not in place and performance is not met';
				$performance = '0';
			}
		}
	}

	if($truereqtreatment == 'N'){
		if(
			isset($node->field_uwwnremoval['und'][0]['value']) &&
			isset($node->field_uwwsecondarytreatment['und'][0]['value']) &&
			$node->field_uwwnremoval['und'][0]['value'] == '1' && 
			$node->field_uwwsecondarytreatment['und'][0]['value'] == '1'
		){
			$treatment = '1';
			if(
				isset($node->field_uwwbod5perf['und'][0]['value']) &&
				isset($node->field_uwwcodperf['und'][0]['value']) &&
				isset($node->field_uwwntotperf['und'][0]['value']) &&
				$node->field_uwwbod5perf['und'][0]['value'] == 'P' &&
				$node->field_uwwcodperf['und'][0]['value'] == 'P' &&
				$node->field_uwwntotperf['und'][0]['value'] == 'P'
			){
				$comp = 'C';
				$reason = 'Station found to be compliant because nitrogen treatment is in place and performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because nitrogen treatment is in place but performance is not met';
				$performance = '0';
			}
		}
		else{
			$treatment = '0';
			if(
				isset($node->field_uwwbod5perf['und'][0]['value']) &&
				isset($node->field_uwwcodperf['und'][0]['value']) &&
				isset($node->field_uwwntotperf['und'][0]['value']) &&
				$node->field_uwwbod5perf['und'][0]['value'] == 'P' &&
				$node->field_uwwcodperf['und'][0]['value'] == 'P' &&
				$node->field_uwwntotperf['und'][0]['value'] == 'P'
			){
				$comp = 'C';
				$reason = 'Station found to be compliant because nitrogen treatment is not in place but performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because nitrogen treatment is not in place and performance is not met';
				$performance = '0';
			}
		}
	}

	if($truereqtreatment == 'MP'){
		if(
			isset($node->field_uwwpremoval['und'][0]['value']) &&
			isset($node->field_uwwsecondarytreatment['und'][0]['value']) &&
			$node->field_uwwpremoval['und'][0]['value'] == '1' && 
			$node->field_uwwsecondarytreatment['und'][0]['value'] == '1'
		){
			$treatment = '1';
			if(
				isset($node->field_uwwbod5perf['und'][0]['value']) &&
				isset($node->field_uwwcodperf['und'][0]['value']) &&
				isset($node->field_uwwptotperf['und'][0]['value']) &&
				$node->field_uwwbod5perf['und'][0]['value'] == 'P' &&
				$node->field_uwwcodperf['und'][0]['value'] == 'P' &&
				$node->field_uwwptotperf['und'][0]['value'] == 'P'
			){
				$comp = 'C';
				$reason = 'Station found to be compliant because phosphorus treatment is in place and performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because phosphorus treatment is in place but performance is not met';
				$performance = '0';
			}
		}
		else{
			$comp = 'NC';
			$reason = 'Station found to be non compliant because phosphorus treatment is not in place';
			$treatment = '0';
		}
	}

	if($truereqtreatment == 'NP'){
		if(
			isset($node->field_uwwpremoval['und'][0]['value']) &&
			isset($node->field_uwwnremoval['und'][0]['value']) &&
			isset($node->field_uwwsecondarytreatment['und'][0]['value']) &&
			$node->field_uwwpremoval['und'][0]['value'] == '1' && 
			$node->field_uwwsecondarytreatment['und'][0]['value'] == '1' &&
			$node->field_uwwnremoval['und'][0]['value'] == '1'
		){
			$treatment = '1';
			if(
				isset($node->field_uwwbod5perf['und'][0]['value']) &&
				isset($node->field_uwwcodperf['und'][0]['value']) &&
				isset($node->field_uwwptotperf['und'][0]['value']) &&
				isset($node->field_uwwntotperf['und'][0]['value']) &&
				$node->field_uwwbod5perf['und'][0]['value'] == 'P' &&
				$node->field_uwwcodperf['und'][0]['value'] == 'P' &&
				$node->field_uwwptotperf['und'][0]['value'] == 'P' &&
				$node->field_uwwntotperf['und'][0]['value'] == 'P'
			){
				$comp = 'C';
				$reason = 'Station found to be compliant because phosphorus and nitrogen treatment are in place and performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because phosphorus and nitrogen treatment are in place but performance is not met';
				$performance = '0';
			}
		}
		else{
			$treatment = '0';
			if(
				isset($node->field_uwwbod5perf['und'][0]['value']) &&
				isset($node->field_uwwcodperf['und'][0]['value']) &&
				isset($node->field_uwwptotperf['und'][0]['value']) &&
				isset($node->field_uwwntotperf['und'][0]['value']) &&
				$node->field_uwwbod5perf['und'][0]['value'] == 'P' &&
				$node->field_uwwcodperf['und'][0]['value'] == 'P' &&
				$node->field_uwwptotperf['und'][0]['value'] == 'P' &&
				$node->field_uwwntotperf['und'][0]['value'] == 'P'
			){
				$comp = 'C';
				$reason = 'Station found to be compliant because phosphorus and nitrogen treatment are not in place but performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because phosphorus and nitrogen treatment are not in place and performance is not met';
				$performance = '0';
			}
		}
	}

	if($truereqtreatment == 'O'){
		if(
			(
				isset($node->field_uwwothertreat['und'][0]['value']) ||
				isset($node->field_uwwuv['und'][0]['value']) ||
				isset($node->field_uwwchlorination['und'][0]['value']) ||
				isset($node->field_uwwozonation['und'][0]['value']) ||
				isset($node->field_uwwsandfiltration['und'][0]['value']) ||
				isset($node->field_uwwmicrofiltration['und'][0]['value']) ||
				isset($node->field_uwwsecondarytreatment['und'][0]['value'])
			) &&
			(
				$node->field_uwwothertreat['und'][0]['value'] == '1' ||
				$node->field_uwwuv['und'][0]['value'] == '1' ||
				$node->field_uwwchlorination['und'][0]['value'] == '1' ||
				$node->field_uwwozonation['und'][0]['value'] == '1' ||
				$node->field_uwwsandfiltration['und'][0]['value'] == '1' ||
				$node->field_uwwmicrofiltration['und'][0]['value'] == '1' ||
				$node->field_uwwsecondarytreatment['und'][0]['value'] == '1'
			)
		){
			$treatment = '1';
			if(isset($node->field_uwwotherperf['und'][0]['value']) && $node->field_uwwotherperf['und'][0]['value'] == 'P'){
				$comp = 'C';
				$reason = 'Station found to be compliant because other treatment is in place and performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because other treatment is in place but performance is not met';
				$performance = '0';
			}
		}
		else{
			$treatment = '0';
			if(isset($node->field_uwwotherperf['und'][0]['value']) && $node->field_uwwotherperf['und'][0]['value'] == 'P'){
				$comp = 'C';
				$reason = 'Station found to be compliant because other treatment is not in place but performance is met';
				$performance = '1';
			}
			else{
				$comp = 'NC';
				$reason = 'Station found to be non compliant because other treatment is not in place and performance is not met';
				$performance = '0';
			}
		}
	}
	
	return (array($truereqtreatment, $comp, $reason, $treatment, $performance));	
}

function uwwtd_unzip_files($file, $target){
	$main_dir = 'public://data_sources';
	file_prepare_directory($main_dir, FILE_CREATE_DIRECTORY);

	// export array
	$newfileslist = array();

	$zip = new ZipArchive;
	$newFile = uwwtd_file_upload($file, '.zip');
	$path = file_create_url($newFile->uri);
	$newfileslist['source'] = $newFile->uri;
	$filename = $newFile->filename;
	$url = drupal_parse_url($file->uri);
	$mydir = $main_dir;
	file_prepare_directory($mydir, FILE_CREATE_DIRECTORY);
	$destination = $mydir.'/'.$filename;

	$opened = $zip->open(drupal_realpath($destination));
	if( $opened !== true ){
       drupal_set_message(t('Cannot open this file'), 'error');
       return false;
    }
    else{
    	$dir = file_create_url("sites/default/data_shapes");
    	for($i = 0; $i < $zip->numFiles; $i++){
    		$zippedfilename = $zip->getNameIndex($i);
    		$newfileslist['extracted'][] = $target.$zippedfilename;
    		$zip->extractTo($target, array($zip->getNameIndex($i)));
    	}
		$zip->extractTo($dir);
		$zip->close();
    }

    if(empty($newfileslist)) return false;
    else return $newfileslist;
}

function uwwtd_add_error($type, $category, $error, $timestamp, $index = null, $node = null){
	$error = trim($error);

	// get label for type
	$field = field_info_field('field_uwwtd_err_type');
	$typeText = $field['settings']['allowed_values'][$type];

	// get label for category
	$field = field_info_field('field_uwwtd_err_category');
	$categoryText = $field['settings']['allowed_values'][$category];

	// get errors file
	$file = drupal_realpath('public://data_errors/uwwtd_errors.txt');
	$content = file_get_contents(drupal_realpath('public://data_errors/uwwtd_errors.txt'));

	// get date
	$dateError = date('m/d/Y h:i:s a');

	$content = $content.$categoryText.' '.$typeText.' '.$dateError.': '.$error;

	// clean sentences
	if(substr($error, -1) != '.') $content .= '. ';
	else $content .=  ' ';
	
	if($node !== null && !empty($node) && $node != ''){
		$content .= $type.' concerns data with a site id of '.$node->field_siteid['und'][0]['value'].' and a drupal id of '.$node->nid.'. ';
	}
	if($index !== null && !empty($index) && $index != ''){
		$content .= 'The corresponding row in the dataset used is at index '.$index.'.';
	}
	$content .= "\r\n";

	$fh = fopen($file, "w");
	fwrite($fh, $content);
	fclose($fh);

	// create error node
	$errorNode = new StdClass();
	$errorNode->type = 'uwwtd_error';
	node_save($errorNode);

	$dateFormat = format_date($timestamp, 'custom', 'Y-m-d H:i:s', 'UTC');
	$dateErrorFormat = format_date(strtotime('now'), 'custom', 'Y-m-d H:i:s', 'UTC');
	$errorNode->title = $category.' '.$type.' '.$dateError;
	$errorNode->field_uwwtd_err_identifier['und'][0]['value'] = $timestamp;
	$errorNode->field_uwwtd_err_type['und'][0]['value'] = $type;
	$errorNode->field_uwwtd_err_category['und'][0]['value'] = $category;

	$errorNode->field_uwwtd_err_timestamp['und'][0]['value'] = $dateFormat;
	$errorNode->field_uwwtd_err_timestamp['und'][0]['timezone'] = 'Europe/Paris';

	$errorNode->field_uwwtd_err_time['und'][0]['value'] = $dateErrorFormat;
	$errorNode->field_uwwtd_err_time['und'][0]['timezone'] = 'Europe/Paris';

	$errorNode->field_uwwtd_err_message['und'][0]['value'] = $error;
	if($node != NULL){
		$errorNode->field_uwwtd_err_concern['und'][0]['nid'] = $node->nid;
		$node->field_uwwtd_error_link['und'][]['nid'] = $errorNode->nid;
		node_save($node);
	}
	if($index != NULL) $errorNode->field_uwwtd_err_index['und'][0]['value'] = $index;

	node_save($errorNode);
}

function uwwtd_check_senisitive($nid){
	$result = false;

	$node = node_load($nid);

	// get node type
	$type = $node->type;

	if($type == 'agglomeration'){
		foreach($node->field_agguwwliste['und'] as $uwwnid){
			$uww = node_load($uwwnid['nid']);
			foreach($uww->field_uwwdcpliste['und'] as $dcpnid){
				$dcp = node_load($dcpnid['nid']);
				if(isset($dcp->field_rcatype['und'][0]['value'])){
					if($dcp->field_rcatype['und'][0]['value'] == 'CSA' || $dcp->field_rcatype['und'][0]['value'] == 'SA') return $dcp->field_rcatype['und'][0]['value'];
				}
			}
		}
	}

	if($type == 'uwwtp'){
		foreach($node->field_uwwdcpliste['und'] as $dcpnid){
			$dcp = node_load($dcpnid['nid']);
			if(isset($dcp->field_rcatype['und'][0]['value'])){
				if($dcp->field_rcatype['und'][0]['value'] == 'CSA' || $dcp->field_rcatype['und'][0]['value'] == 'SA') return $dcp->field_rcatype['und'][0]['value'];
			}
		}
	}

	if($type == 'discharge_point'){
		if(isset($dcp->field_rcatype['und'][0]['value'])){
			if($dcp->field_rcatype['und'][0]['value'] == 'CSA' || $dcp->field_rcatype['und'][0]['value'] == 'SA') return $dcp->field_rcatype['und'][0]['value'];
		}
	}
}

function uwwtd_country_cases($node){
	$repcode = $node->field_repcode['und'][0]['value'];
	$repcode = str_replace('_', '-', $repcode);
	$boom = explode('-', $repcode);
	$boom = explode(' ', $boom[0]);
	$rep = $boom[0];
	if($rep == 'SI' || $rep == 'si'){
		$sens = uwwtd_check_senisitive($node->nid);
		if(new DateTime($node->field_anneedata['und'][0]['value'].'-12'.'-31') <= new DateTime('2015-12-31')){
			if($node->field_agggenerated['und'][0]['value'] <= 10000){
				$art4comp = 'NR';
				$reason = 'Article 4 compliance found to be non relevant because the slovenian agglomeration data is before the 31/12/2015 and the agglomeration has a population equal or less than 10 000.';
				return array($art4comp, $reason);
			}
			if(($sens != 'CSA' && $sens != 'SA') && $node->field_agggenerated['und'][0]['value'] <= 15000){
				$art4comp = 'NR';
				$reason = 'Article 4 compliance found to be non relevant because the slovenian agglomeration data is before the 31/12/2015, the agglomeration is situation in a CSA or SA and the population is equal or less than 15 000.';
				return array($art4comp, $reason);
			}
		}
	}

	if($rep == 'CY' || $rep == 'CYC' || $rep == 'cy' || $rep == 'cyc'){
		$nom = $node->title;
		$curDate = new DateTime($node->field_anneedata['und'][0]['value'].'-12'.'-31');
		if($curDate <= new DateTime('2012-12-31')){
			if($curDate <= new DateTime('2008-12-31') && ($nom == 'Paralimni' || $nom == 'Limassol') && $node->field_agggenerated['und'][0]['value'] <= 15000){
				$art4comp = 'NR';
				$reason = 'Article 4 compliance found to be non relevant because the cyprian agglomeration data is before the 31/12/2008, the agglomeration is named "Paralimni" or "Limassol" and the population is equal or less than 15 000.';
				return array($art4comp, $reason);
			}
			if($curDate <= new DateTime('2009-12-31') && $nom == 'Nicosia' && $node->field_agggenerated['und'][0]['value'] <= 15000){
				$art4comp = 'NR';
				$reason = 'Article 4 compliance found to be non relevant because the cyprian agglomeration data is before the 31/12/2009, the agglomeration is named "Nicosia" and the population is equal or less than 15 000.';
				return array($art4comp, $reason);
			}
			if($curDate <= new DateTime('2011-12-31')){
				if($nom != 'Paralimni' && $nom != 'Limassol' && $nom != 'Nicosia'){
					$art4comp = 'NR';
					$reason = 'Article 4 compliance found to be non relevant because the cyprian agglomeration data is before the 31/12/2011 and the agglomeration is not named "Paralimni", "Limassol" or "Nicosia".';
					return array($art4comp, $reason);
				}
			}
		}
	}
}

function uwwtd_in_array_r($needle, $haystack, $strict = false){
    foreach ($haystack as $item){
        if (($strict ? $item === $needle : $item == $needle) || (is_array($item) && uwwtd_in_array_r($needle, $item, $strict))){
            return true;
        }
    }
    return false;
}

function uwwtd_zipped_csvs_to_array($files){

	$combined = array('UWWTD_Report' => array());

	if(in_array('public://data_sources/t_reporter.csv', $files['extracted'])) $combined['UWWTD_Report']['Reporter'] = uwwtd_csv_to_array('public://data_sources/t_reporter.csv', ';');

	if(in_array('public://data_sources/t_reportperiod.csv', $files['extracted'])) $combined['UWWTD_Report']['ReportPeriod'] = uwwtd_csv_to_array('public://data_sources/t_reportperiod.csv', ';');

	if(in_array('public://data_sources/t_contact.csv', $files['extracted'])) $combined['UWWTD_Report']['Contacts']['Contact'] = uwwtd_csv_to_array('public://data_sources/t_contact.csv', ';');

	if(in_array('public://data_sources/t_receivingAreas.csv', $files['extracted'])){
		$rcas = uwwtd_csv_to_array('public://data_sources/t_receivingAreas.csv', ';');
		if(count($rcas) === 1) $combined['UWWTD_Report']['ReceivingAreas']['ReceivingArea'] = $rcas[0];
		else $combined['UWWTD_Report']['ReceivingAreas']['ReceivingArea']['ReceivingArea'] = $rcas;
	}

	if(in_array('public://data_sources/t_agglomerations.csv', $files['extracted'])) $combined['UWWTD_Report']['Agglomerations']['Agglomeration'] = uwwtd_csv_to_array('public://data_sources/t_agglomerations.csv', ';');

	if(in_array('public://data_sources/t_uwwtps.csv', $files['extracted'])) $combined['UWWTD_Report']['UWWTPs']['UWWTP'] = uwwtd_csv_to_array('public://data_sources/t_uwwtps.csv', ';');

	if(in_array('public://data_sources/t_dischargePoints.csv', $files['extracted'])) $combined['UWWTD_Report']['DischargePoints']['DischargePoint'] = uwwtd_csv_to_array('public://data_sources/t_dischargePoints.csv', ';');

	if(in_array('public://data_sources/t_uwwtpagglo.csv', $files['extracted'])) $combined['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'] = uwwtd_csv_to_array('public://data_sources/t_uwwtpagglo.csv', ';');

	if(in_array('public://data_sources/t_mslevel.csv', $files['extracted'])) $combined['UWWTD_Report']['MSLevel'] = uwwtd_csv_to_array('public://data_sources/t_mslevel.csv', ';');

	return $combined;
}

function uwwtd_get_required_treatment($dcps, $maxgen){
	$reqtreatments = array();

	if(count($dcps) == 0) return 'NI';

	foreach($dcps as $dcp){

		// If the discharge point is in a Less Sensitive Area
		if($dcp['rcatype'] == 'LSA'){

			// For dcps in a LSA with water body type LC or CW
			if($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW'){

				// For dcps in a LSA with water body type LC or CW and having an agg population (p.e) of 2000 - 10000
				if($maxgen < 10000){
					$reqtreatments[] = 'NR';
				}

				// For stations in a LSA with water body type LC or CW and having an agg population (p.e) of > 10000
				if($maxgen >= 10000){
					// secondary treatment must be present
					$reqtreatments[] = 'S';
				}
			}

			// For stations in a LSA with water body type ES
			if($dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'LF' || $dcp['waterbody'] == 'FW'){

				// For stations in a LSA with water body type ES or FW and having an agg population (p.e) of < 2000
				if($maxgen < 2000){
					$reqtreatments[] = 'NR';
				}

				// For stations in a LSA with water body type ES and having an agg population (p.e) of 2000 - 10000
				if($maxgen >= 2000 && $maxgen < 10000){
					// primary treatment must be present
					$reqtreatments[] = 'P';
				}

				// For stations in a LSA with water body type ES and having an agg population (p.e) of > 10000
				if($maxgen >= 10000){
					// secondary treatment must be present
					$reqtreatments[] = 'S';
				}
			}
		}

		// If the discharge point is in a Normal Area
		if($dcp['rcatype'] == 'NA'){

			// For stations in a NA with water body type LC or CW
			if($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW'){

				// For stations in a NA with water body type LC or CW and having an agg population (p.e) of 2000 - 10000
				if($maxgen < 10000){
					$reqtreatments[] = 'NR';
				}

				// For stations in a NA with water body type LC or CW and having an agg population (p.e) of > 10000
				if($maxgen >= 10000){
					// secondary treatment must be present
					// LG pas la bonne variable mise à jour :  $typeTreat = 'S';
					$reqtreatments[] = 'S';					
				}
			}

			// For stations in a NA with water body type ES or FW
			if($dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'FW' || $dcp['waterbody'] == 'LF'){

				// For stations in a NA with water body type ES or FW and having an agg population (p.e) of < 2000
				if($maxgen < 2000){
					$reqtreatments[] = 'NR';
				}

				// For stations in a NA with water body type ES or FW and having an agg population (p.e) of 2000 - 10000
				if($maxgen >= 2000){
					// secondary treatment must be present
					$reqtreatments[] = 'S';
				}
			}
		}

		// If the discharge point is in a Sensitive Area or a Catchment Sensitive Area
		if($dcp['rcatype'] == 'SA' || $dcp['rcatype'] == 'CSA' || $dcp['rcatype'] == 'A58'){

			// For stations in a SA or CSA with an agg population (p.e) of < 2000
			if($maxgen < 2000){
				$reqtreatments[] = 'NR';
			}

			// For stations in a SA or CSA with an agg population (p.e) of 2000 - 10000
			if($maxgen >= 2000 && $maxgen < 10000){
				// secondary treatment must be present
				$reqtreatments[] = 'S';
			}

			// For stations in a SA or CSA with an agg population (p.e) of > 10000
			if($maxgen >= 10000){
				if($nrec == '1') $reqtreatments[] = 'N';
				if($prec == '1') $reqtreatments[] = 'MP';
				if($nrec == '1' && $prec == '1') $reqtreatments[] = 'NP';
				if($nrec != '1' && $prec != '1') $reqtreatments[] = 'O';				
			}
		}
	}

	$truereqtreatment = $reqtreatments[0];

	foreach($reqtreatments as $reqtreatment){
		if($truereqtreatment == 'NR'){
			if($reqtreatment == 'P') $truereqtreatment = 'P';
			if($reqtreatment == 'S') $truereqtreatment = 'S';
			if($reqtreatment == 'N') $truereqtreatment = 'N';
			if($reqtreatment == 'MP') $truereqtreatment = 'MP';
			if($reqtreatment == 'NP') $truereqtreatment = 'NP';
			if($reqtreatment == 'O') $truereqtreatment = 'O';
		}
		if($truereqtreatment == 'P'){
			if($reqtreatment == 'S') $truereqtreatment = 'S';
			if($reqtreatment == 'N') $truereqtreatment = 'N';
			if($reqtreatment == 'MP') $truereqtreatment = 'MP';
			if($reqtreatment == 'NP') $truereqtreatment = 'NP';
			if($reqtreatment == 'O') $truereqtreatment = 'O';
		}
		if($truereqtreatment == 'S'){
			if($reqtreatment == 'N') $truereqtreatment = 'N';
			if($reqtreatment == 'MP') $truereqtreatment = 'MP';
			if($reqtreatment == 'NP') $truereqtreatment = 'NP';
			if($reqtreatment == 'O') $truereqtreatment = 'O';
		}
		if($truereqtreatment == 'N'){
			if($reqtreatment == 'NP') $truereqtreatment = 'NP';
			if($reqtreatment == 'O') $truereqtreatment = 'O';
		}
		if($truereqtreatment == 'MP'){
			if($reqtreatment == 'NP') $truereqtreatment = 'NP';
			if($reqtreatment == 'O') $truereqtreatment = 'O';
		}
		if($truereqtreatment == 'NP'){
			if($reqtreatment == 'O') $truereqtreatment = 'O';
		}
	}

	return $truereqtreatment;
}

// No longer needed but could be useful
/*function uwwtd_FindSensitiveArea($wkt){
	//On boucle sur la liste des tables de zones sensibles afin de determiner laquelle est la bonne
	
	$tables = array(
		'data.eu_sa_catchm',
		'data.eu_sa_coasta',
		'data.eu_sa_coastl',
		'data.eu_sa_lake',
		'data.eu_sa_tw'
	);
	
	$results = array();
	
	foreach($tables as $table){
		
		$nbRows = 0;
	
		if($table == 'data.eu_sa_catchm'){
			$query = "
				WITH geo AS (SELECT ST_Transform(ST_GeomFromText('".$wkt."', 4326), 3035) as the_geom)
				SELECT eucd_sa_cm, name, type
				FROM ".$table." c, geo
				WHERE geo.the_geom && c.the_geom
					AND ST_intersects(geo.the_geom, c.the_geom)
			";
			$rs = db_query($query)->fetchAll();
			$nbRows = count($rs);
			if($nbRows > 0){
				foreach($rs as $r){
					$s = 'NON';
					if($r->type == 'S') $s = 'SA';
					elseif($r->type == 'N') $s = 'NA';
					$results[] = array('id'=>$r->eucd_sa_cm, 'name'=>$r->name, 'type'=>$s);
				}
			}
		}
		else{
			$query = "
				WITH geo AS (SELECT ST_Transform(ST_GeomFromText('".$wkt."', 4326), 3035) as the_geom)
				SELECT eucd_sa_cm, name
				FROM ".$table." c, geo
				WHERE geo.the_geom && c.the_geom
					AND ST_intersects(geo.the_geom, c.the_geom)
			";
			$rs = db_query($query)->fetchAll();
			$nbRows = count($rs);
			if($nbRows > 0){
				$s = 'SA';
				foreach($rs as $r){
					$results[] = array('id'=>$r->eucd_sa_cm, 'name'=>$r->name, 'type'=>$s);
				}
			}
		}
		
		if(count($results) > 0){
			return $results;
		}
		else return false;
	}
	//La louse, nos données sont en LAEA 3035, il faut don refaire une reprojection
	$nbRows = count($r);
	if($nbRows == 0) return 0;
	else return array($r[0]);
}*/