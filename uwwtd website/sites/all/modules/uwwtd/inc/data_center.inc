<?php

require_once(drupal_get_path('module', 'uwwtd') . '/inc/uwwtd.config.inc');
require_once(drupal_get_path('module', 'uwwtd') . '/inc/utils.inc');

################################ DATA CENTER PAGE ################################
function uwwtd_data_form($form, &$form_state){

    if(isset($_SESSION['uwwtd_annee']))unset($_SESSION['uwwtd_annee']);

    $uwwtd_year = date("Y");

    $options = $GLOBALS['uwwtd']['uwwtd_years'];
    $all_years = uwwtd_get_all_year();
    foreach ($options as $key => $value) {
        if (isset($all_years[$key])) {
            $options[$key] = "$value *";               
        }
    }
    // definition du champ input de type select pour l'annee
    $form['uwwtd_year'] = array(     
        '#type' => 'select',
        '#title' => t('Year'),
        '#default_value' => $uwwtd_year,        
        '#options' => $options,
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


function uwwtd_run_compliance_page($node){
    if($node->type=='agglomeration'){
        if(!empty($node->field_agguwwliste['und'])){
            foreach($node->field_agguwwliste['und'] as $u){
                if($uww = node_load($u['nid'])){
                    uwwtd_station_set_conformity($uww);
                    drupal_set_message("Processing conformity for the UWWTP ".$uww->title." : Ok");
                    //dsm($uww);
                }else{
                    drupal_set_message("Can't load a linked UWWTP for the agglomeration ".$node->title, 'warning');
                }
            }
        }
        else{
            drupal_set_message("There is no linked UWWTP for the agglomeration ".$node->title, 'warning');
        }
        
        uwwtd_agglo_set_conformity($node, true);
        drupal_set_message("Processing conformity for the agglomeration ".$node->title." : Ok");
    }
    elseif($node->type=='uwwtp'){
        uwwtd_station_set_conformity($node);
        drupal_set_message("Processing conformity for the UWWTP ".$node->title." : Ok");
    }
    //dsm($node);
    drupal_goto('node/'.$node->nid);
}


function uwwtd_get_errors_stats(){
    $output ='';
    //Summary stat on error list
    $query ="
        SELECT 
            e.year AS year,
            e.type AS type,
            e.bundle AS bundle,
            e.category AS category,
            e.error AS error,
            COUNT(*) AS nb
        FROM 
            {uwwtd_import_errors} e
        GROUP BY e.year, e.category, e.type, e.error, e.bundle
        ORDER BY e.year DESC, e.type DESC, e.bundle ASC, e.category ASC, nb DESC
    ";
    $result = db_query($query);
    $data = array();
    $errors_type = array_flip($GLOBALS['uwwtd_errors_type']['type']);
    $errors_cat = array_flip($GLOBALS['uwwtd_errors_type']['category']);
    $node_types = _node_types_build();
    foreach($result as $row){
        
        $line = (array)$row;
        if($line['bundle']=='') {
            $line['bundle'] = 'Other';
        }
        elseif(isset($node_types->names[$line['bundle']])){
            $line['bundle'] = $node_types->names[$line['bundle']];
        }
        $line['category'] =$errors_cat[$line['category']];
        $line['type'] =$errors_type[$line['type']];
        
        $data[$line['bundle']][$line['year']][]=$line;
    }
    
    if(!empty($data)){
        foreach($data as $bundle=>$bundles_err){
            $output.="<h2>".$bundle."</h2>";
            foreach($bundles_err as $year=>$err){
                $output.="<h3>".$year."</h3>";
                $output.=theme('table', [
                    'rows'=>$err, 
                    'header'=>[t('Year'),t('Type'),t('Bundle'),t('Category'),t('Error'),t('Number of message')],
                    'sticky'=>true
                ]);
            }
        }
    }
    //dsm($data);
    
    return $output;
    
}


################################ DATA INPUT ################################
function uwwtd_import_form($form, &$form_state){

    if(!isset($_SESSION['uwwtd_annee'])){
        drupal_goto('data');
    } else {        
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
//  if(isset($_SESSION['uwwtd_errors'])) unset($_SESSION['uwwtd_errors']);
//  if(isset($_SESSION['uwwtd_warnings'])) unset($_SESSION['uwwtd_warnings']);
    $annee = $_SESSION['uwwtd_annee'];
    $file = file_load($form_state['values']['uwwtd_file']);
    $uri = $file->uri;

    // reset session values
    $_SESSION['newRca'] = 0;
    $_SESSION['upRca'] = 0;
    $_SESSION['totRca'] = 0;
    $_SESSION['newAgg'] = 0;
    $_SESSION['upAgg'] = 0;
    $_SESSION['totAgg'] = 0;
    $_SESSION['newUww'] = 0;
    $_SESSION['upUww'] = 0;
    $_SESSION['totUww'] = 0;
    $_SESSION['newDcp'] = 0;
    $_SESSION['upDcp'] = 0;
    $_SESSION['totDcp'] = 0;
    $_SESSION['newBc'] = 0;
    $_SESSION['upBc'] = 0;
    $_SESSION['totBc'] = 0;
    $_SESSION['newAggloUww'] = 0;
    $_SESSION['upAggloUww'] = 0;
    $_SESSION['totAggloUww'] = 0;

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
    $timestamp = strtotime('now');


    //dsm($timestamp);

    if(isset($datas['UWWTD_Report']['ReportPeriod']['repCode'])){
        $repCode =  uwwtd_check_text($datas['UWWTD_Report']['ReportPeriod']['repCode']);
    }
    //10th report
    elseif(isset($datas['MSLevel']['Row']['repCode'])){
        $repCode =  uwwtd_check_text($datas['MSLevel']['Row']['repCode']);
    }
    else{
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No repCode found for this dataset :'.$file->uri, $timestamp, 0);
        $repCode = null;
    }
    
    //On profite de l'occasion pour creer le noeud sur le MSLevel
    if(!empty($datas['UWWTD_Report']['MSLevel'])){
        uwwtd_update_mslevel($newFile, $annee, $datas['UWWTD_Report']['MSLevel'], $repCode, $timestamp);
    }
    //10th report
    elseif(!empty($datas['MSLevel']) && isset($datas['MSLevel']['Row'])){
        uwwtd_update_mslevel($newFile, $annee, $datas['MSLevel']['Row'], $repCode, $timestamp);
    }

//     dsm($datas['MSLevel']);
//     return;
    uwwtd_setup_batch($newFile, $annee, $datas, $repCode, $timestamp);  
}

function uwwtd_update_mslevel($newFile, $annee, $datas, $repCode, $timestamp){
    global $language;
    if(!empty($datas)){

//         dsm($datas);
        // Variables
        $siteId = $annee.'_mslevel';
        uwwtd_trace('import ms_level : ' . $siteId);
        // Check if id exists
        $exists = uwwtd_check_exist($siteId);

        // If the element exists
        
        uwwtd_trace('ms_level ' . $siteId . ' exists ? "' . $exists . '".');
        if(!$exists){
            $node = new stdClass();
            $node->type = 'ms_level';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('ms_level ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('ms_level ' . $siteId . ' nid : ' . $node->nid);
            }            
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'ms_level';

            if (empty($node->nid)) {
                uwwtd_trace('ms_level ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('ms_level ' . $siteId . ' nid : ' . $node->nid);
            }             
        }
        $node->title = 'Data on MS level - '.$annee;

        // site mapping
        $node->field_siteid['und'][0]['value'] = $siteId;
        //$node->field_valid['und'][0]['value'] = 0;
        $node->field_anneedata['und'][0]['value'] = $annee;
        /*
        $node->field_sourcefile = array(
            'und' => array(
                0 => array(
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );*/

        // data mapping
        $node->field_repcode['und'][0]['value'] = $repCode;
        //text
        if(isset($datas['repCode'])) $node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['repCode']);
        //numeric
        //if(isset($datas['mslSludgeProduction'])) $node->field_mslsludgeproduction['und'][0]['value'] = uwwtd_check_numeric($datas['mslSludgeProduction'], $timestamp, 0 , $node);
        if(isset($datas['mslSludgeProduction'])) {
            $check = uwwtd_check_decimal($datas['mslSludgeProduction']); //e.v 209/01/07 change from uwwtd_check_numeric to uwwtd_check_decimal
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, 0 , $node);
            } else {
                $node->field_mslsludgeproduction['und'][0]['value'] = $check;
            }
        }        
        //decimal
        //if(isset($datas['mslDischargePipelines'])) $node->field_msldischargepipelines['und'][0]['value'] = uwwtd_check_decimal($datas['mslDischargePipelines'], $timestamp, 0 , $node);
        if(isset($datas['mslDischargePipelines'])) {
            $check = uwwtd_check_decimal($datas['mslDischargePipelines']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_msldischargepipelines['und'][0]['value'] = $check;
            
        }        
        //decimal
        //if(isset($datas['mslDischargeShips'])) $node->field_msldischargeships['und'][0]['value'] = uwwtd_check_decimal($datas['mslDischargeShips'], $timestamp, 0 , $node);
        if(isset($datas['mslDischargeShips'])) {
            $check = uwwtd_check_decimal($datas['mslDischargeShips']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_msldischargeships['und'][0]['value'] = $check;
            
        }         
        //decimal
//         if(isset($datas['mslDischargeOthers'])) $node->field_msldischargeothers['und'][0]['value'] = uwwtd_check_decimal($datas['mslDischargeOthers'], $timestamp, 0 , $node);
        if(isset($datas['mslDischargeOthers'])) {
            $check = uwwtd_check_decimal($datas['mslDischargeOthers']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_msldischargeothers['und'][0]['value'] = $check;
            
        }        
        //decimal
//         if(isset($datas['mslReuseSoilAgriculture'])) $node->field_mslreusesoilagriculture['und'][0]['value'] = uwwtd_check_decimal($datas['mslReuseSoilAgriculture'], $timestamp, 0 , $node);
        if(isset($datas['mslReuseSoilAgriculture'])) {
            $check = uwwtd_check_decimal($datas['mslReuseSoilAgriculture']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_mslreusesoilagriculture['und'][0]['value'] = $check;
            
        }        
        //decimal
//         if(isset($datas['mslReuseOthers'])) $node->field_mslreuseothers['und'][0]['value'] = uwwtd_check_decimal($datas['mslReuseOthers'], $timestamp, 0 , $node);
        if(isset($datas['mslReuseOthers'])) {
            $check = uwwtd_check_decimal($datas['mslReuseOthers']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_mslreuseothers['und'][0]['value'] = $check;
            
        }        
        //decimal
//         if(isset($datas['mslDisposalLandfill'])) $node->field_msldisposallandfill['und'][0]['value'] = uwwtd_check_decimal($datas['mslDisposalLandfill'], $timestamp, 0 , $node);
        if(isset($datas['mslDisposalLandfill'])) {
            $check = uwwtd_check_decimal($datas['mslDisposalLandfill']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_msldisposallandfill['und'][0]['value'] = $check;
            
        }        
        //decimal
//         if(isset($datas['mslDisposalIncineration'])) $node->field_msldisposalincineration['und'][0]['value'] = uwwtd_check_decimal($datas['mslDisposalIncineration'], $timestamp, 0 , $node);
        if(isset($datas['mslDisposalIncineration'])) {
            $check = uwwtd_check_decimal($datas['mslDisposalIncineration']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_msldisposalincineration['und'][0]['value'] = $check;
            
        }        
        //decimal
//         if(isset($datas['mslDisposalOthers'])) $node->field_msldisposalothers['und'][0]['value'] = uwwtd_check_decimal($datas['mslDisposalOthers'], $timestamp, 0 , $node);
        if(isset($datas['mslDisposalOthers'])) {
            $check = uwwtd_check_decimal($datas['mslDisposalOthers']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, 0 , $node);
            }
            $node->field_msldisposalothers['und'][0]['value'] = $check;
            
        }        
        //text
        if(isset($datas['mslRemarks'])) $node->field_mslremarks['und'][0]['value'] = truncate_utf8(uwwtd_check_text($datas['mslRemarks']), 254);
        //numeric
//         if(isset($datas['mslWWReusePerc'])) $node->field_mslwwreuseperc['und'][0]['value'] = uwwtd_check_numeric($datas['mslWWReusePerc'], $timestamp,  0 , $node);
        if(isset($datas['mslWWReusePerc'])) {
            $check = uwwtd_check_decimal($datas['mslWWReusePerc']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, 0 , $node);
            } else {
                $node->field_mslwwreuseperc['und'][0]['value'] = $check;
            }
        }         
        //bool
        if(isset($datas['mslWWReuseAgri'])) {
            $check = uwwtd_check_bool($datas['mslWWReuseAgri']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node);
            }
            $node->field_mslwwreuseagri['und'][0]['value'] = $check;
            
        }
        //bool
        if(isset($datas['mslWWReuseInd'])) {
            $check = uwwtd_check_bool($datas['mslWWReuseInd']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node);
            }   
            $node->field_mslwwreuseind['und'][0]['value'] = $check;
            
        }
        //bool
        if(isset($datas['mslWWReuseOther'])) {
            $check = uwwtd_check_bool($datas['mslWWReuseOther']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node);
            }  
            $node->field_mslwwreuseother['und'][0]['value'] = $check;
            
        }
        //text
        if(isset($datas['mslWWReuseExplain'])) $node->field_mslwwreuseexplain['und'][0]['value'] = uwwtd_check_text($datas['mslWWReuseExplain']);

        // save node
        node_save($node);
        
        if (empty($node->nid)) {
            uwwtd_trace('ms_level ' . $siteId . ' node_save() error2');
        }  
                
        return $node;
    } else {
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No MsLevel found in the dataset ' . $annee, $timestamp, 0);
    }   
}

/*DG 15/12/2015 Reporter*/
function uwwtd_update_reporter($newFile, $annee, $datas, $repCode, $timestamp){
    global $language;
    
    if(!empty($datas)){
//         dsm($datas);
        // Variables
        $siteId = $annee.'_reporter';
        uwwtd_trace('import reporter : ' . $siteId);
        // Check if id exists
        $exists = uwwtd_check_exist($siteId);

        // If the element exists
        
        uwwtd_trace('reporter ' . $siteId . ' exists ? "' . $exists . '".');
        if(!$exists){
            $node = new stdClass();
            $node->type = 'reporter';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('reporter ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('reporter ' . $siteId . ' nid : ' . $node->nid);
            }            
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'reporter';

            if (empty($node->nid)) {
                uwwtd_trace('reporter ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('reporter ' . $siteId . ' nid : ' . $node->nid);
            }             
        }
        $node->title = 'Data on reporter - '.$annee;

        // site mapping
        $node->field_siteid['und'][0]['value'] = $siteId;
        //$node->field_valid['und'][0]['value'] = 0;
        $node->field_anneedata['und'][0]['value'] = $annee;
        $node->field_sourcefile = array(
            'und' => array(
                0 => array(
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );

        // data mapping
        $node->field_repcode['und'][0]['value'] = $repCode;
        //text
        if(isset($datas['rptMStateKey'])) $node->field_rptmstatekey['und'][0]['value'] = uwwtd_check_text($datas['rptMStateKey']);
        //text
        if(isset($datas['rptMStateValue'])) $node->field_rptmstatevalue['und'][0]['value'] = uwwtd_check_text($datas['rptMStateValue']);
        //text
        if(isset($datas['rptCulture'])) $node->field_rptculture['und'][0]['value'] = uwwtd_check_text($datas['rptCulture']);
        //numeric
        if(isset($datas['rptFormRA'])) {
            $check = uwwtd_check_numeric($datas['rptFormRA']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, 0 , $node);
            } else {
                $node->field_rptformra['und'][0]['value'] = $check;
            }
        } 
        
        // save node
        node_save($node);
        
        if (empty($node->nid)) {
            uwwtd_trace('reporter ' . $siteId . ' node_save() error2');
        }  
                
        return $node;
    } else {
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No reporter found in the dataset ' . $annee, $timestamp, 0);
    }   
}

/*DG 15/12/2015 Report Period*/
function uwwtd_update_reportPeriod($newFile, $annee, $datas, $repCode, $timestamp){
    global $language;
    
    if(!empty($datas)){
//         dsm($datas);
        // Variables
        $siteId = $annee.'_reportperiod';
        uwwtd_trace('import report_period : ' . $siteId);
        // Check if id exists
        $exists = uwwtd_check_exist($siteId);

        // If the element exists
        
        uwwtd_trace('report_period ' . $siteId . ' exists ? "' . $exists . '".');
        if(!$exists){
            $node = new stdClass();
            $node->type = 'report_period';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('report_period ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('report_period ' . $siteId . ' nid : ' . $node->nid);
            }            
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'report_period';

            if (empty($node->nid)) {
                uwwtd_trace('report_period ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('report_period ' . $siteId . ' nid : ' . $node->nid);
            }             
        }
        $node->title = 'Data on report_period - '.$annee;

        // site mapping
        $node->field_siteid['und'][0]['value'] = $siteId;
        //$node->field_valid['und'][0]['value'] = 0;
        $node->field_anneedata['und'][0]['value'] = $annee;
        $node->field_sourcefile = array(
            'und' => array(
                0 => array(
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );

        // data mapping
        $node->field_repcode['und'][0]['value'] = $repCode;
        //text
        if(isset($datas['rptMStateKey'])) $node->field_rptmstatekey['und'][0]['value'] = uwwtd_check_text($datas['rptMStateKey']);
        //text
        if(isset($datas['repCode'])) $node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['repCode']);
        //date
        if(isset($data['repVersion'])){
            $check = uwwtd_check_date($data['repVersion'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_repversion['und'][0]['value'] = $check;
            }            
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No Report period version date found.', $timestamp, $i, $node);
        //date
        if(isset($data['repSituationAt'])){
            $check = uwwtd_check_date($data['repSituationAt'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_repsituationat['und'][0]['value'] = $check;
            }            
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No Report period situation date found.', $timestamp, $i, $node);
        //text
        if(isset($datas['repReportedPeriod'])) $node->field_repreportedperiod['und'][0]['value'] = uwwtd_check_text($datas['repReportedPeriod']);
        
        // save node
        node_save($node);
        
        if (empty($node->nid)) {
            uwwtd_trace('report_period ' . $siteId . ' node_save() error2');
        }  
                
        return $node;
    } else {
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No report period found in the dataset ' . $annee, $timestamp, 0);
    }   
}

function uwwtd_setup_batch($newFile, $annee, $datas, $repCode, $timestamp){
    uwwtd_trace('uwwtd_setup_batch all file');
    uwwtd_trace(var_export($datas, true));
    $batch = array(
        'title' => t('Data import'),
        'operations' => array(),
        'error_message' => t('Error!'),
        'finished' => 'uwwtd_finished',
        'file' => drupal_get_path('module','uwwtd').'/inc/data_center.inc'
    );
    
    //delete all error from previous import
    $batch['operations'][]=array('uwwtd_delete_error', array($annee, 'import'));

    /*DG 15/12/2015 Add type RCA*/
    /*Fix nd@oieau.fr 09/06/2016 : reprocess the ReceivingAreas import ==> the batch operation not working before*/
    //ReceivingAreas
    $imports  = array();
    //Old reporting format (8th)
    if(isset($datas['UWWTD_Report']['ReceivingAreas'])){
        //Case of country with only one RCA

        if(isset($datas['UWWTD_Report']['ReceivingAreas']['ReceivingArea']['rcaCode'])){
            $imports[0] = $datas['UWWTD_Report']['ReceivingAreas']['ReceivingArea'];
        }
        //Case of country with more of one RCA
        else{
            $imports = $datas['UWWTD_Report']['ReceivingAreas']['ReceivingArea'];
        }
    }
    //Old reporting format (9th)
    elseif(isset($datas['UWWTD_Report']['ReceivingAreasSAMain'])){
        //ReceivingAreaSAMain
        //fix nd@oieau.fr le 10/08/2016 : import des rca pour les pays avec plusieurs RCA

        if(!isset($datas['UWWTD_Report']['ReceivingAreasSAMain']['ReceivingAreaSAMain']['rcaCode'])){
            foreach($datas['UWWTD_Report']['ReceivingAreasSAMain']['ReceivingAreaSAMain'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        else{
            foreach($datas['UWWTD_Report']['ReceivingAreasSAMain'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        $subkeys = array(
            'ReceivingAreasSAParameter'=>'ReceivingAreaSAParameter',
            'ReceivingAreasSA54'=>'ReceivingAreaSA54',
            'ReceivingAreasLSA'=>'ReceivingAreaLSA',
            'ReceivingAreasSASA'=>'ReceivingAreaSASA',
            'ReceivingAreasSALSAPredecessor'=>'ReceivingAreaSALSAPredecessor'
        );
        foreach ($subkeys as $root=>$key) {
            if (isset($datas['UWWTD_Report'][$root]) && isset($datas['UWWTD_Report'][$root][$key])) {
                foreach ($datas['UWWTD_Report'][$root][$key] as $baliseName => $rcaValue){
                    if (!is_array($rcaValue)) {
                        // Get rcaCode :
                        $rcaCode = '';
                        foreach($datas['UWWTD_Report'][$root][$key] as $key2 => $rcaValue2) {
                            if ($key2 === 'rcaCode') {
                                $rcaCode = $rcaValue2;
                            }
                        }

                        // if $rcaCode found :
                        if (!empty($rcaCode)) {
                            $imports[$rcaCode][$root][0][$baliseName] = $rcaValue;
                        }
                    } else {
                        if(isset($imports[$rcaValue['rcaCode']])){
                            //ajout d'un tableau
                            $imports[$rcaValue['rcaCode']][$root][] = $rcaValue;
                        }
                    }
                }
            }
        }
    }
    //Case of the 10th report
    elseif(isset($datas['ReceivingAreasSAMain'])){
        if(!isset($datas['ReceivingAreasSAMain']['Row']['rcaCode'])){
            foreach($datas['ReceivingAreasSAMain']['Row'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        //nd@oieau.fr 17/07/2018 : check how this tag appear in $datas when we have only one item
        else{
            foreach($datas['ReceivingAreasSAMain'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        $subkeys = array(
            'ReceivingAreasSAParameter'=>'Row',
            'ReceivingAreasSA54'=>'Row',
            'ReceivingAreasSASA'=>'Row',
            'ReceivingAreasSALSAPredecessor'=>'Row'
        );
        foreach ($subkeys as $root=>$key) {
            if (isset($datas[$root]) && isset($datas[$root][$key])) {
                foreach ($datas[$root][$key] as $baliseName => $rcaValue){
                    if (!is_array($rcaValue)) {
                        // Get rcaCode :
                        $rcaCode = '';
                        foreach($datas[$root][$key] as $key2 => $rcaValue2) {
                            if ($key2 === 'rcaCode') {
                                $rcaCode = $rcaValue2;
                            }
                        }

                        // if $rcaCode found :
                        if (!empty($rcaCode)) {
                            $imports[$rcaCode][$root][0][$baliseName] = $rcaValue;
                        }
                    } else {
                        if(isset($imports[$rcaValue['rcaCode']])){
                            //ajout d'un tableau
                            $imports[$rcaValue['rcaCode']][$root][] = $rcaValue;
                        }
                    }
                }
            }
        }
    }
    if(isset($datas['ReceivingAreasLSA'])){
        if(!isset($datas['ReceivingAreasLSA']['Row']['rcaCode'])){
            foreach($datas['ReceivingAreasLSA']['Row'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        //nd@oieau.fr 17/07/2018 : check how this tag appear in $datas when we have only one item
        else{
            foreach($datas['ReceivingAreasLSA'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        $subkeys = array(
            'ReceivingAreasSALSAPredecessor'=>'Row'
        );
        foreach ($subkeys as $root=>$key) {
            if (isset($datas[$root]) && isset($datas[$root][$key])) {
                foreach ($datas[$root][$key] as $baliseName => $rcaValue){
                    if (!is_array($rcaValue)) {
                        // Get rcaCode :
                        $rcaCode = '';
                        foreach($datas[$root][$key] as $key2 => $rcaValue2) {
                            if ($key2 === 'rcaCode') {
                                $rcaCode = $rcaValue2;
                            }
                        }

                        // if $rcaCode found :
                        if (!empty($rcaCode)) {
                            $imports[$rcaCode][$root][0][$baliseName] = $rcaValue;
                        }
                    } else {
                        if(isset($imports[$rcaValue['rcaCode']])){
                            //ajout d'un tableau
                            $imports[$rcaValue['rcaCode']][$root][] = $rcaValue;
                        }
                    }
                }
            }
        }
    }
    if (!empty($imports)) {

    	$batch['operations'][]=array('uwwtd_import_rca_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));

    }

// return 'ok';
    //BigCityDischargers
    $imports  = array();
    if(isset($datas['UWWTD_Report']['BigCityDischargers'])){
        if(isset($datas['UWWTD_Report']['BigCityDischargers']['BigCityDischarger']['bigCityID'])){
            $imports[0] = $datas['UWWTD_Report']['BigCityDischargers']['BigCityDischarger'];
        }
        else{
            $imports = $datas['UWWTD_Report']['BigCityDischargers']['BigCityDischarger'];
        }
        uwwtd_trace('$imports BigCityDischargers');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_import_bigcity_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }
    //10th report
    elseif(isset($datas['BigCityDischargers'])){
        if(isset($datas['BigCityDischargers']['Row']['bigCityID'])){
            $imports[0] = $datas['BigCityDischargers']['Row'];
        }
        else{
            $imports = $datas['BigCityDischargers']['Row'];
        }
        uwwtd_trace('$imports BigCityDischargers v10th report');
        uwwtd_trace(var_export($imports, true));
        //e.v 2018/10/22 => BigCityDischargers are not big city, but city of p.e > 150 000 without bigCityID
//         $batch['operations'][]=array('uwwtd_import_bigcity_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }

    //Agglomerations
    $imports  = array();
    if(isset($datas['UWWTD_Report']['Agglomerations'])){
        if(isset($datas['UWWTD_Report']['Agglomerations']['Agglomeration']['aggCode'])){
            $imports[0] = $datas['UWWTD_Report']['Agglomerations']['Agglomeration'];
        }
        else{
            $imports = $datas['UWWTD_Report']['Agglomerations']['Agglomeration'];
        }
        uwwtd_trace('$imports Agglomerations');
        uwwtd_trace(var_export($imports, true));

        // Save codes :
        if (!empty($imports)) {
            $codes = array();
            foreach ($imports as $import) {
                if (!empty($import['aggCode'])) {
                    $codes[] = $import['aggCode'];
                }
            }
            variable_set(AGGS_WITHOUT_TP, $codes);
        }
        $batch['operations'][]=array('uwwtd_import_agglo_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }
    //10th report
    elseif(isset($datas['Agglomerations'])){
        if(isset($datas['Agglomerations']['Row']['aggCode'])){
            $imports[0] = $datas['Agglomerations']['Row'];
        }
        else{
            $imports = $datas['Agglomerations']['Row'];
        }
        uwwtd_trace('imports Agglomerations v10th report');
        uwwtd_trace(var_export($imports, true));

        // Save codes :
        if (!empty($imports)) {
            $codes = array();
            foreach ($imports as $import) {
                if (!empty($import['aggCode'])) {
                    $codes[] = $import['aggCode'];
                }
            }
            variable_set(AGGS_WITHOUT_TP, $codes);
        }
        $batch['operations'][]=array('uwwtd_import_agglo_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }

    //UWWTPs
    $imports  = array();
    if(isset($datas['UWWTD_Report']['UWWTPs'])){
        if(isset($datas['UWWTD_Report']['UWWTPs']['UWWTP']['uwwCode'])){
            $imports[0] = $datas['UWWTD_Report']['UWWTPs']['UWWTP'];
        }
        else{
            $imports = $datas['UWWTD_Report']['UWWTPs']['UWWTP'];
        }
        uwwtd_trace('$imports UWWTPs');
        uwwtd_trace(var_export($imports, true));

        // Save codes :
        if (!empty($imports)) {
            $codes = array();
            foreach ($imports as $import) {
                if (!empty($import['uwwCode'])) {
                    $codes[] = $import['uwwCode'];
                }
            }
            variable_set(TPS_WITHOUT_AGG, $codes);
            variable_set(TPS_WITHOUT_DP, $codes);
        }
        $batch['operations'][]=array('uwwtd_import_uww_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }
    elseif(isset($datas['UWWTPs'])){
        if(isset($datas['UWWTPs']['Row']['uwwCode'])){
            $imports[0] = $datas['UWWTPs']['Row'];
        }
        else{
            $imports = $datas['UWWTPs']['Row'];
        }
        uwwtd_trace('$imports UWWTPs v10th report');
        uwwtd_trace(var_export($imports, true));

        // Save codes :
        if (!empty($imports)) {
            $codes = array();
            foreach ($imports as $import) {
                if (!empty($import['uwwCode'])) {
                    $codes[] = $import['uwwCode'];
                }
            }
            variable_set(TPS_WITHOUT_AGG, $codes);
            variable_set(TPS_WITHOUT_DP, $codes);
        }
        $batch['operations'][]=array('uwwtd_import_uww_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }

    //DischargePoints
    $imports  = array();
    if(isset($datas['UWWTD_Report']['DischargePoints'])){
        if(isset($datas['UWWTD_Report']['DischargePoints']['DischargePoint']['dcpCode'])){
            $imports[0] = $datas['UWWTD_Report']['DischargePoints']['DischargePoint'];
        }
        else{
            $imports = $datas['UWWTD_Report']['DischargePoints']['DischargePoint'];
        }
        uwwtd_trace('$imports DischargePoints');
        uwwtd_trace(var_export($imports, true));

        // Save codes :
        if (!empty($imports)) {
            $codes = array();
            foreach ($imports as $import) {
                if (!empty($import['dcpCode'])) {
                    $codes[] = $import['dcpCode'];
                }
            }
            variable_set(DPS_WITHOUT_TP, $codes);
        }
        $batch['operations'][]=array('uwwtd_import_dcp_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }
    //10th report
    elseif(isset($datas['DischargePoints'])){
        if(isset($datas['DischargePoints']['Row']['dcpCode'])){
            $imports[0] = $datas['DischargePoints']['Row'];
        }
        else{
            $imports = $datas['DischargePoints']['Row'];
        }
        uwwtd_trace('imports DischargePoints v10th report');
        uwwtd_trace(var_export($imports, true));

        // Save codes :
        if (!empty($imports)) {
            $codes = array();
            foreach ($imports as $import) {
                if (!empty($import['dcpCode'])) {
                    $codes[] = $import['dcpCode'];
                }
            }
            variable_set(DPS_WITHOUT_TP, $codes);
        }
        $batch['operations'][]=array('uwwtd_import_dcp_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }

    //link_dcp_rca
    $imports  = array();
    if(isset($datas['UWWTD_Report']['DischargePoints'])){
        /*
        if(isset($datas['UWWTD_Report']['DischargePoints']['DischargePoint']['dcpCode'])){
            $imports[0] = $datas['UWWTD_Report']['DischargePoints'];
        }
        else{
            $imports = $datas['UWWTD_Report']['DischargePoints']['DischargePoint'];
        }
        $batch['operations'][]=array('uwwtd_link_dcp_rca_uww_batch_process', array($newFile, $annee, $imports, $timestamp));
        */
        $batch['operations'][]=array('uwwtd_link_dcp_rca_uww_batch_process', array($newFile, $annee, $timestamp));
    }
    //10th report
    elseif(isset($datas['DischargePoints'])){
        /*
        if(isset($datas['DischargePoints']['DischargePoint']['dcpCode'])){
            $imports[0] = $datas['UWWTD_Report']['DischargePoints'];
        }
        else{
            $imports = $datas['DischargePoints']['DischargePoint'];
        }
        $batch['operations'][]=array('uwwtd_link_dcp_rca_uww_batch_process', array($newFile, $annee, $imports, $timestamp));
        */
        $batch['operations'][]=array('uwwtd_link_dcp_rca_uww_batch_process', array($newFile, $annee, $timestamp));
    }

    //UwwtpAgglos
    $imports  = array();
    if(isset($datas['UWWTD_Report']['UwwtpAgglos'])){
        if(isset($datas['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo']['aucUwwCode'])){
            $imports[0] = $datas['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'];
        }
        else{
            $imports = $datas['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'];
        }
        uwwtd_trace('$imports UwwtpAgglos');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_link_uww_agglo_batch_process', array($newFile, $annee, $imports, $timestamp));
    }
    //10th report
    elseif(isset($datas['UwwtpAgglos'])){
        if(isset($datas['UwwtpAgglos']['Row']['aucUwwCode'])){
            $imports[0] = $datas['UwwtpAgglos']['Row'];
        }
        else{
            $imports = $datas['UwwtpAgglos']['Row'];
        }
        uwwtd_trace('imports UwwtpAgglos v10th report');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_link_uww_agglo_batch_process', array($newFile, $annee, $imports, $timestamp));
    }

    //Process missing link between agglo & uwwtp (especially for inactive uwwtp)
    $batch['operations'][]=array('uwwtd_missing_link_uww_agglo_batch_process', array($newFile, $annee, $timestamp));

    //BigCityDischargers
    $imports  = array();
    if(isset($datas['UWWTD_Report']['BigCityDischargers'])){
        if(isset($datas['UWWTD_Report']['BigCityDischargers']['BigCityDischarger']['bigCityID'])){
            $imports[0] = $datas['UWWTD_Report']['BigCityDischargers']['BigCityDischarger'];
        }
        else{
            $imports = $datas['UWWTD_Report']['BigCityDischargers']['BigCityDischarger'];
        }
        uwwtd_trace('$imports BigCityDischargers');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_link_bc_agglo_batch_process', array($newFile, $annee, $imports, $timestamp));
    }
    //10th report
    elseif(isset($datas['BigCityDischargers'])){
        if(isset($datas['BigCityDischargers']['Row']['bigCityID'])){
            $imports[0] = $datas['BigCityDischargers']['Row'];
        }
        else{
            $imports = $datas['BigCityDischargers']['Row'];
        }
        uwwtd_trace('imports BigCityDischargers v10th report');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_link_bc_agglo_batch_process', array($newFile, $annee, $imports, $timestamp));
    }

    /*DG 15/12/2015 Contact*/
    $imports  = array();
    if(isset($datas['UWWTD_Report']['Contacts'])){
        if(isset($datas['UWWTD_Report']['Contacts']['Contact']['repCode'])){
            $imports[0] = $datas['UWWTD_Report']['Contacts']['Contact'];
        }
        else{
            $imports = $datas['UWWTD_Report']['Contacts']['Contact'];
        }
        uwwtd_trace('$imports Contacts');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_import_contact_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }
    //10th report
    elseif(isset($datas['Contacts'])){
        if(isset($datas['Contacts']['Row']['repCode'])){
            $imports[0] = $datas['Contacts']['Row'];
        }
        else{
            $imports = $datas['Contacts']['Row'];
        }
        uwwtd_trace('imports Contacts v10th report');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_import_contact_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }

    /*DG 15/12/2015 Industry*/
    $imports  = array();
    if(isset($datas['UWWTD_Report']['Industries'])){
        if(isset($datas['UWWTD_Report']['Industries']['Industry']['repCode'])){
            $imports[0] = $datas['UWWTD_Report']['Industries']['Industry'];
        }
        else{
            $imports = $datas['UWWTD_Report']['Industries']['Industry'];
        }
        uwwtd_trace('$imports Industries');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_import_contact_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }
    //10th report
    elseif(isset($datas['Industries'])){
        if(isset($datas['Industries']['Row']['repCode'])){
            $imports[0] = $datas['Industries']['Row'];
        }
        else{
            $imports = $datas['Industries']['Row'];
        }
        uwwtd_trace('imports Industries v10th report');
        uwwtd_trace(var_export($imports, true));
        $batch['operations'][]=array('uwwtd_import_contact_batch_process', array($newFile, $annee, $imports, $repCode, $timestamp));
    }

    //bc_position
    $batch['operations'][]= array('uwwtd_bc_position_batch_process', array($newFile, $annee, $timestamp));
    //mega_links
    $batch['operations'][]= array('uwwtd_mega_links_batch_process', array($newFile, $annee, $timestamp));
    //rca_extra
    $batch['operations'][]= array('uwwtd_rca_extra_batch_process', array($newFile, $annee, $timestamp));
    $batch['operations'][]= array('uwwtd_update_cache_data', array(array($annee)));
    $batch['operations'][]= array('uwwtd_import_data_var', array($annee));

    // Create errors for agglomerations without treatment plant :
    $batch['operations'][]= array('uwwtd_set_errors_aggs_without_tp', array($annee));

    // Create errors for treatment plants without agglomeration :
    $batch['operations'][]= array('uwwtd_set_errors_tps_without_agg', array($annee));

    // Create errors for treatment plants without discharge point :
    $batch['operations'][]= array('uwwtd_set_errors_tps_without_dp', array($annee));

    // Create errors for discharging point without treatment plant :
    $batch['operations'][]= array('uwwtd_set_errors_dps_without_tp', array($annee));

    batch_set($batch);
    batch_process('data/import');
}

function uwwtd_import_rca_batch_process($newFile, $annee, $datas, $repCode, $timestamp, &$context){
    if(!empty($datas)){
        if (!isset($context['sandbox']['progress'])) {
            //Get all receiving areas from data
            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = array_keys($datas);
            $_SESSION['totRca'] = $context['sandbox']['max'];
        }
        $i = $context['sandbox']['progress'];
        $j=0;
        while($j<20 && $context['sandbox']['progress']<$context['sandbox']['max']){
            $k=$i+$j;
            $data = $datas[$context['sandbox']['ids'][$k]];
            $node = uwwtd_update_rca_node($data, $newFile, $repCode, $annee, $timestamp, $i);
            if($node){
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
            }
            else{
                $context['results'][] = t('Failed to import a receiving area.');
            }
            $context['sandbox']['progress']++;
            $j++;
        }

        uwwtd_flush_errors();
        if($node){
            $context['message'] = '(' .$annee . ') ' . t('Processing receiving area').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        }
        else{
            $context['message'] = '(' .$annee . ') ' . t('Failed to import a receiving area.');
        }
        
        if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    }else{
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving areas found in the dataset ' . $annee, $timestamp, $i);
    }
}

function uwwtd_update_rca_node($data, $newFile, $repCode, $annee, $timestamp, $i){
    global $language;
 
    // Check basic elements exist
    // errors
    $e = 0;
    if(!isset($data['rcaName']) || empty($data['rcaName'])){
        $e = 1;
        $emes = t('The receiving area does not have \'title\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }
    if(!isset($data['rcaCode']) || empty($data['rcaCode'])){
        $e = 1;
        $emes = t('The receiving area does not have \'code\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }

    // Check if no errors were found
    uwwtd_trace('import receiving_area : ' . $annee.'_rca_'.uwwtd_check_text($data['rcaCode']));
    //dsm($data);
    if($e == 0){
        // Variables
        $siteId = $annee.'_rca_'.uwwtd_check_text($data['rcaCode']);

        // Check if id exists
        $exists = uwwtd_check_exist($siteId);
        uwwtd_trace('receiving_area ' . $siteId . ' exists ? "' . $exists . '".');
        // If the element exists
        if(!$exists){
            $node = new stdClass();
            $node->type = 'receiving_area';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            $_SESSION['newRca']++;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('receiving_area ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('receiving_area ' . $siteId . ' nid : ' . $node->nid);
            }
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'receiving_area';
            $_SESSION['upRca']++;
            
            if (empty($node->nid)) {
                uwwtd_trace('receiving_area ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('receiving_area ' . $siteId . ' nid : ' . $node->nid);
            }             
        }
        //text
        $node->title = uwwtd_check_text($data['rcaName']);

        // site mapping
        $node->field_siteid['und'][0]['value'] = $siteId;
        $node->field_valid['und'][0]['value'] = 0;
        $node->field_anneedata['und'][0]['value'] = $annee;
        $node->field_sourcefile = array(
            'und' => array(
                0 => array(
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );
        //unset liste for case the rca is updated, we recalcultaed it later
        unset($node->field_rcadcpliste['und']);

        // data mapping
        $node->field_repcode['und'][0]['value'] = $repCode;
        
        /* DG 15/12/2015 add rcaEnvDom for RCA LSA, RCA SA MAIN*/
        //text
        if(isset($data['rcaEnvDom'])){
            $node->field_rcaenvdom['und'][0]['value'] = uwwtd_check_text($data['rcaEnvDom']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area lsa environmental domain found.', $timestamp, $i, $node);
        
        //nd@oieau.fr 04/01/2021 the default status is "1" 
        $node->field_status['und'][0]['value'] = '1';
        
        //text
        // BB 22/08 added elseif for 9th report
        // BeginLife date is set and is before report date
        if (isset($data['rcaBeginLife']) && date("Y", strtotime($data['rcaBeginLife'])) >  $annee) {
        	$rcaState9thReport = '0';
        	// If EndLife date before report date, rca is inactive
        	
        }
        if (isset($data['rcaEndLife']) && date("Y", strtotime($data['rcaEndLife'])) < $annee) {
            $rcaState9thReport = '0';
        }

        if(isset($data['rcaState'])){
            $node->field_status['und'][0]['value'] = uwwtd_check_text($data['rcaState']);
        } elseif(isset($rcaState9thReport)) {
        	$node->field_status['und'][0]['value'] = $rcaState9thReport;
        } 
        //else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area status found.', $timestamp, $i, $node);
        
        //text
        $node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['rcaCode']);

        /* DG 15/12/2015 add rcaSpZTyp for RCA LSA, RCA SA MAIN*/
        //text
        if(isset($data['rcaType']) || isset($data['rcaSpZTyp'])){
            if(isset($data['rcaType'])){
                 $node->field_specialisedzonetype['und'][0]['value'] = uwwtd_check_text($data['rcaType']);
            }elseif(isset($data['rcaSpZTyp'])){
                     $node->field_specialisedzonetype['und'][0]['value'] = uwwtd_check_text($data['rcaSpZTyp']);
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area specialised zone type found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaZtype for RCA SA MAIN, RCA LSA*/
        //text
        if(isset($data['rcaZtype'])){
            $node->field_zonetype['und'][0]['value'] = uwwtd_check_text($data['rcaZtype']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area zone type found.', $timestamp, $i, $node);
        
        
        /* DG 15/12/2015 add rca61DateDesignation for RCA LSA*/
        //date
        if(isset($data['rca61DateDesignation'])){
            $check = uwwtd_check_date($data['rca61DateDesignation'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rca61datedesignation['und'][0]['value'] = $check;
            }            
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area lsa article 61 date found.', $timestamp, $i, $node);
        
        
        //text
        if(isset($data['rcaSensitiveArea'])){
            $node->field_rca_sensitive_area['und'][0]['value'] = uwwtd_check_text($data['rcaSensitiveArea']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area Related Sensitive area found.', $timestamp, $i, $node);
        
        //date
        if(isset($data['rcaDateArt58'])){
//             if(uwwtd_check_date($data['rcaDateArt58'], $timestamp, $i, $node) !==false){
//                 $node->field_rcadateart5854['und'][0]['value'] = uwwtd_check_date($data['rcaDateArt58'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_date($data['rcaDateArt58'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcadateart5854['und'][0]['value'] = $check;
            }            
        } elseif (!empty($data['rcaSpZTyp']) && ($data['rcaSpZTyp'] == 'A5854' || $data['rcaSpZTyp'] == 'A58523' || $data['rcaSpZTyp'] == 'A5854523')) {
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area article 58 date found.', $timestamp, $i, $node);
        }

        //bool
        if(isset($data['rcaParameterN'])){
            $check = uwwtd_check_bool($data['rcaParameterN']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rca_parameter_n['und'][0]['value'] = $check;
            }
        }                                
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area nitrogen parameter found.', $timestamp, $i, $node);

        //bool
        if(isset($data['rcaParameterP'])){
            $check = uwwtd_check_bool($data['rcaParameterP']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rca_parameter_p['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area phosphorus parameter found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaParameterM for RCA main*/
        //bool
        if(isset($data['rcaParameterM'])){
            $check = uwwtd_check_bool($data['rcaParameterM']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rca_parameter_m['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area more stringent parameter found.', $timestamp, $i, $node);

        //bool
        if(isset($data['rcaParameterOther'])){
            $check = uwwtd_check_bool($data['rcaParameterOther']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rca_parameter_other['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area other parameter found.', $timestamp, $i, $node);

        /* DG 15/12/2015 add rcaArt58DateDesign for RCA LSA*/
         //date
        if(isset($data['rcaArt58DateDesign'])){

            $check = uwwtd_check_date($data['rcaArt58DateDesign'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcaart58datedesign['und'][0]['value'] = $check;
            }             
        } elseif (!empty($data['rcaSpZTyp']) && ($data['rcaSpZTyp'] == 'A5854' || $data['rcaSpZTyp'] == 'A58523' || $data['rcaSpZTyp'] == 'A5854523')) {
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area designation date art 58 found.', $timestamp, $i, $node);
        }

        //date
        if(isset($data['rcaDateDesignation'])){
//             if(uwwtd_check_date($data['rcaDateDesignation'], $timestamp, $i, $node) !== false){
//                 $node->field_rcadatedesignation61['und'][0]['value'] = uwwtd_check_date($data['rcaDateDesignation'], $timestamp, $i, $node);
//                 $node->field_rcaapdatedesignation['und'][0]['value'] = uwwtd_check_date($data['rcaDateDesignation'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_date($data['rcaDateDesignation'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcadatedesignation61['und'][0]['value'] = $check;
                $node->field_rcaapdatedesignation['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area designation date found.', $timestamp, $i, $node);

        //bool
        if(isset($data['rcaANitro'])){
            $check = uwwtd_check_bool($data['rcaANitro']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcaanitro['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area nitrogen information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['rcaAPhos'])){
            $check = uwwtd_check_bool($data['rcaAPhos']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcaaphos['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area phosphorus information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['rcaB'])){
            $check = uwwtd_check_bool($data['rcaB']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcab['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area B information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['rcaC'])){
            $check = uwwtd_check_bool($data['rcaC']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcac['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area C information found.', $timestamp, $i, $node);
        
        //memo ? text ?
        if(isset($data['rcaCRelevantDirective'])){
            $node->field_rcacrelevantdirective['und'][0]['value'] = uwwtd_check_text($data['rcaCRelevantDirective']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area relevant directive information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['rcaMorphology'])){
            $check = uwwtd_check_bool($data['rcaMorphology']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcamorphology['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area morphology information found.', $timestamp, $i, $node);

        /* DG 15/12/2015 add rcaHydrology for RCA LSA*/
        //bool
        if(isset($data['rcaHydrologie'])|| (isset($data['rcaHydrology']))){
            if(isset($data['rcaHydrologie'])){
                $check = uwwtd_check_bool($data['rcaHydrologie']);
                if (false === $check) {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
                } else {
                    $node->field_rcahydrologie['und'][0]['value'] = $check;
                }
            }elseif(isset($data['rcaHydrology'])){
                $check = uwwtd_check_bool($data['rcaHydrology']);
                if (false === $check) {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
                } else {
                    $node->field_rcahydrologie['und'][0]['value'] = $check;
                }
            }
            
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area hydrology information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['rcaHydraulic'])){
            $check = uwwtd_check_bool($data['rcaHydraulic']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcahydraulic['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area hydraulic information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['rcaAbsenceRisk'])){
            $check = uwwtd_check_bool($data['rcaAbsenceRisk']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcaabsencerisk['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area absence risk information found.', $timestamp, $i, $node);
        
        //date
        if(isset($data['rcaDateArt54'])){
//             if(uwwtd_check_date($data['rcaDateArt54'], $timestamp, $i, $node) !==false){
//                 $node->field_rcadateart54['und'][0]['value'] = uwwtd_check_date($data['rcaDateArt54'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_date($data['rcaDateArt54'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcadateart54['und'][0]['value'] = $check;
            }              
        } elseif (!empty($data['rcaSpZTyp']) && ($data['rcaSpZTyp'] == 'A54' || $data['rcaSpZTyp'] == 'A5854' || $data['rcaSpZTyp'] == 'A5854523')) {
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area article 54 date found.', $timestamp, $i, $node);
        }
        
        /*DG 15/12/2015 Add rcaPlants54 for RCA SA 54*/
        //numeric
        if(isset($data['rcaPlants']) || isset($data['rcaPlants54'])){
            if(isset($data['rcaPlants'])){
                $check = uwwtd_check_numeric($data['rcaPlants']);
                if (false === $check) {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
                } else {
                    $node->field_rcaplants['und'][0]['value'] = $check;
                }      
            }elseif(isset($data['rcaPlants54'])){
                $check = uwwtd_check_numeric($data['rcaPlants54']);
                if (false === $check) {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
                } else {
                    $node->field_rcaplants['und'][0]['value'] = $check;
                }     
            }
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area plants information found.', $timestamp, $i, $node);
        
        /*DG 15/12/2015 Add rcaPlantsCapacity54 for RCA SA 54*/
        //numeric
        if(isset($data['rcaPlantsCapacity']) || isset($data['rcaPlantsCapacity54'])){
            if(isset($data['rcaPlantsCapacity'])){
                $check = uwwtd_check_numeric($data['rcaPlantsCapacity']);
                if (false === $check) {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
                } else {
                    $node->field_rcaplantscapacity['und'][0]['value'] = $check;
                }      
            }elseif(isset($data['rcaPlantsCapacity54'])){
                $check = uwwtd_check_numeric($data['rcaPlantsCapacity54']);
                if (false === $check) {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
                } else {
                    $node->field_rcaplantscapacity['und'][0]['value'] = $check;
                }     
            }
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area plants capacity information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaNIncomingMeasured'])){
//             if(uwwtd_check_decimal($data['rcaNIncomingMeasured'], $timestamp, $i, $node) !==false){
//                 $node->field_rcanincomingmeasured['und'][0]['value'] = uwwtd_check_decimal($data['rcaNIncomingMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaNIncomingMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcanincomingmeasured['und'][0]['value'] = $check;
            }          
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming N-tot measured information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaNIncomingCalculated'])){
//             if(uwwtd_check_decimal($data['rcaNIncomingCalculated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcanincomingcalculated['und'][0]['value'] = uwwtd_check_decimal($data['rcaNIncomingCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaNIncomingCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcanincomingcalculated['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming N-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaNIncomingEstimated'])){
//             if(uwwtd_check_decimal($data['rcaNIncomingEstimated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcanincomingestimated['und'][0]['value'] = uwwtd_check_decimal($data['rcaNIncomingEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaNIncomingEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcanincomingestimated['und'][0]['value'] = $check;
            }              
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming N-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaPIncomingMeasured'])){
//             if(uwwtd_check_decimal($data['rcaPIncomingMeasured'], $timestamp, $i, $node) !==false){
//                 $node->field_rcapincomingmeasured['und'][0]['value'] = uwwtd_check_decimal($data['rcaPIncomingMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaPIncomingMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcapincomingmeasured['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming P-tot measured information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaPIncomingCalculated'])){
//             if(uwwtd_check_decimal($data['rcaPIncomingCalculated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcapincomingcalculated['und'][0]['value'] = uwwtd_check_decimal($data['rcaPIncomingCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaPIncomingCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcapincomingcalculated['und'][0]['value'] = $check;
            }              
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming P-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaPIncomingEstimated'])){
//             if(uwwtd_check_decimal($data['rcaPIncomingEstimated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcapincomingestimated['und'][0]['value'] = uwwtd_check_decimal($data['rcaPIncomingEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaPIncomingEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcapincomingestimated['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming P-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaNDischargedMeasured'])){
//             if(uwwtd_check_decimal($data['rcaNDischargedMeasured'], $timestamp, $i, $node) !==false){
//                 $node->field_rcandischargedmeasured['und'][0]['value'] = uwwtd_check_decimal($data['rcaNDischargedMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaNDischargedMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcandischargedmeasured['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged N-tot measured information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaNDischargedCalculated'])){
//             if(uwwtd_check_decimal($data['rcaNDischargedCalculated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcandischargedcalculated['und'][0]['value'] = uwwtd_check_decimal($data['rcaNDischargedCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaNDischargedCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcandischargedcalculated['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged N-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaNDischargedEstimated'])){
//             if(uwwtd_check_decimal($data['rcaNDischargedEstimated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcandischargedestimated['und'][0]['value'] = uwwtd_check_decimal($data['rcaNDischargedEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaNDischargedEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcandischargedestimated['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged N-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaPDischargedMeasured'])){
//             if(uwwtd_check_decimal($data['rcaPDischargedMeasured'], $timestamp, $i, $node) !==false){
//                 $node->field_rcapdischargedmeasured['und'][0]['value'] = uwwtd_check_decimal($data['rcaPDischargedMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaPDischargedMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcapdischargedmeasured['und'][0]['value'] = $check;
            }              
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged P-tot measured information found.', $timestamp, $i, $node);
    
        //decimal
        if(isset($data['rcaPDischargedCalculated'])){
//             if(uwwtd_check_decimal($data['rcaPDischargedCalculated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcapdischargedcalculated['und'][0]['value'] = uwwtd_check_decimal($data['rcaPDischargedCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaPDischargedCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcapdischargedcalculated['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged P-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['rcaPDischargedEstimated'])){
//             if(uwwtd_check_decimal($data['rcaPDischargedEstimated'], $timestamp, $i, $node) !==false){
//                 $node->field_rcapdischargedestimated['und'][0]['value'] = uwwtd_check_decimal($data['rcaPDischargedEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['rcaPDischargedEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
            } else {
                $node->field_rcapdischargedestimated['und'][0]['value'] = $check;
            }             
        }   
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged P-tot estimated information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['rcaGIS'])){
            $node->field_rcagis['und'][0]['value'] = uwwtd_check_text($data['rcaGIS']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area root of corresponding GIS-data file found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['rcaCIDOtherDirective'])){
            $node->field_rcacidotherdirective['und'][0]['value'] = uwwtd_check_text($data['rcaCIDOtherDirective']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area other directive information found.', $timestamp, $i, $node);

        //date
        if(isset($data['rcaCDateOtherDirective'])){
//             if(uwwtd_check_date($data['rcaCDateOtherDirective'], $timestamp, $i, $node) !== false){
//                 $node->field_rcacdateotherdirective['und'][0]['value'] = uwwtd_check_date($data['rcaCDateOtherDirective'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_date($data['rcaCDateOtherDirective'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcacdateotherdirective['und'][0]['value'] = $check;
            }              
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area other directive information date found.', $timestamp, $i, $node);

        /* DG 15/12/2015 add rcalsaRemarks for RCA LSA*/
        //memo ? text ?
        if(isset($data['rcaRemarks'])|| isset($data['rcalsaRemarks'])){
            if(isset($data['rcaRemarks'])){
                $node->field_rcaremarks['und'][0]['value'] = uwwtd_check_text($data['rcaRemarks']);
            }elseif(isset($data['rcalsaRemarks'])){
                $node->field_rcaremarks['und'][0]['value'] = uwwtd_check_text($data['rcalsaRemarks']);
            }
        }  
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area root of correspo GIS-data file found.', $timestamp, $i, $node);
        
        //bool
        // VGO: improved 2016 12 08
        if (isset($data['rcaArt54Applied'])) {
            $check = uwwtd_check_bool($data['rcaArt54Applied']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_rca54applied['und'][0]['value'] = $check;
            }
        } else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area article 54 applied information found.', $timestamp, $i, $node);


        // liste de type 'A51523', 'A523', 'SA', 'CSA', 'A54', 'A5854', 'A5154', 'A58', 'A58523', 'A54523', 'A5854523', 'LSA'
        //if(false !== strpos(_______,'54')){
        //if(false !== strpos(_______,'523')){
        //if(false !== strpos(_______,'58')){
//         if ($node->field_specialisedzonetype['und'][0]['value'] == 'A54' ||
//             $node->field_specialisedzonetype['und'][0]['value'] == 'A5854'
        if (in_array($node->field_specialisedzonetype['und'][0]['value'], ['A54', 'A5854', 'A5154', 'A54523', 'A5854523'])) {
            $node->field_rca54applied['und'][0]['value'] = TRUE;
        }

        if ($node->field_specialisedzonetype['und'][0]['value'] == 'A523' || $node->field_specialisedzonetype['und'][0]['value'] == 'A54523'
            || $node->field_specialisedzonetype['und'][0]['value'] == 'A58523' || $node->field_specialisedzonetype['und'][0]['value'] == 'A5854523') {
            $node->field_rca52applied['und'][0]['value'] = '1';
        } else {
            $node->field_rca52applied['und'][0]['value'] = '0';
        }

        if ($node->field_specialisedzonetype['und'][0]['value'] == 'A58' || $node->field_specialisedzonetype['und'][0]['value'] == 'A5854'
            || $node->field_specialisedzonetype['und'][0]['value'] == 'A58523' || $node->field_specialisedzonetype['und'][0]['value'] == 'A5854523') {
            $node->field_rca58applied['und'][0]['value'] = '1';
        } else {
            $node->field_rca58applied['und'][0]['value'] = '0';
        }
        
        /* DG 15/12/2015 add rcaParameter for RCA SA PARAMETER*/
        //text
        if(isset($data['rcaParameter'])){
            $node->field_rcaparameter['und'][0]['value'] = uwwtd_check_text($data['rcaParameter']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area name parameter found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaStartDate for RCA SA PARAMETER*/
        //date
        if(isset($data['rcaStartDate'])){
            $check = uwwtd_check_date($data['rcaStartDate'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcastartdate['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area starting date of application found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaHyperlink for RCA LSA*/
        //text
        if(isset($data['rcaHyperlink'])){
            $node->field_rcahyperlink['und'][0]['value'] = uwwtd_check_text($data['rcaHyperlink']);
            //debug IT 10th report : 
            //$node->field_rcaremarks['und'][0]['value'] = uwwtd_check_text($data['rcaHyperlink']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area lsa hyperlink found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaBeginLife for RCA LSA*/
        //date
        if(isset($data['rcaBeginLife'])){
            $check = uwwtd_check_date($data['rcaBeginLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcabeginlife['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area lsa begin life date found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaEndLife for RCA LSA*/
        //date
        if(isset($data['rcaEndLife'])){
            $check = uwwtd_check_date($data['rcaEndLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_rcaendlife['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area lsa end life date found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaMethod54 for RCA SA54*/
        //text
        if(isset($data['rcaMethod54'])){
            $node->field_rcamethod54['und'][0]['value'] = uwwtd_check_text($data['rcaMethod54']);
            if($node->field_rcamethod54['und'][0]['value'] == "Measured"){
                //rcaNIncoming54
                 if(isset($data['rcaNIncoming54'])){
                    $check = uwwtd_check_decimal($data['rcaNIncoming54']);
                    if (false === $check){
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    }else{
                        $node->field_rcanincomingmeasured['und'][0]['value'] = $check;
                    }         
                }
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming N-tot measured information found.', $timestamp, $i, $node);
                //rcaNDischarged54
                 //decimal
                if(isset($data['rcaNDischarged54'])){
                    $check = uwwtd_check_decimal($data['rcaNDischarged54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcandischargedmeasured['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged N-tot measured information found.', $timestamp, $i, $node);
                
                //rcaPIncoming54
                //decimal
                if(isset($data['rcaPIncoming54'])){
                    $check = uwwtd_check_decimal($data['rcaPIncoming54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcapincomingmeasured['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming P-tot measured information found.', $timestamp, $i, $node);
                
                //rcaPDischarged54
                 //decimal
                if(isset($data['rcaPDischarged54'])){
                    $check = uwwtd_check_decimal($data['rcaPDischarged54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcapdischargedmeasured['und'][0]['value'] = $check;
                    }              
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged P-tot measured information found.', $timestamp, $i, $node);
            
            }elseif($node->field_rcamethod54['und'][0]['value'] == "Calculated"){
                //decimal
                if(isset($data['rcaNIncoming54'])){
                    $check = uwwtd_check_decimal($data['rcaNIncoming54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcanincomingcalculated['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming N-tot calculated information found.', $timestamp, $i, $node);
                
                //decimal
                if(isset($data['rcaPIncoming54'])){
                    $check = uwwtd_check_decimal($data['rcaPIncoming54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcapincomingcalculated['und'][0]['value'] = $check;
                    }              
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming P-tot calculated information found.', $timestamp, $i, $node);
                
                 //decimal
                if(isset($data['rcaNDischarged54'])){
                    $check = uwwtd_check_decimal($data['rcaNDischarged54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcandischargedcalculated['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged N-tot calculated information found.', $timestamp, $i, $node);
                
                //decimal
                if(isset($data['rcaPDischarged54'])){
                    $check = uwwtd_check_decimal($data['rcaPDischarged54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcapdischargedcalculated['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged P-tot calculated information found.', $timestamp, $i, $node);
            }elseif($node->field_rcamethod54['und'][0]['value'] == "Estimated"){
                //decimal
                if(isset($data['rcaNIncoming54'])){
                    $check = uwwtd_check_decimal($data['rcaNIncoming54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcanincomingestimated['und'][0]['value'] = $check;
                    }              
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming N-tot estimated information found.', $timestamp, $i, $node);
                
                //decimal
                if(isset($data['rcaPIncoming54'])){
                    $check = uwwtd_check_decimal($data['rcaPIncoming54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcapincomingestimated['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area incoming P-tot estimated information found.', $timestamp, $i, $node);
                
                 //decimal
                if(isset($data['rcaNDischarged54'])){
                    $check = uwwtd_check_decimal($data['rcaNDischarged54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcandischargedestimated['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged N-tot estimated information found.', $timestamp, $i, $node);
                
                //decimal
                if(isset($data['rcaPDischarged54'])){
                    $check = uwwtd_check_decimal($data['rcaPDischarged54']);
                    if (false === $check) {
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node);
                    } else {
                        $node->field_rcapdischargedestimated['und'][0]['value'] = $check;
                    }             
                }   
                else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area discharged P-tot estimated information found.', $timestamp, $i, $node);
                
                
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area method found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaParameter for RCA SASA*/
        //text
        if(isset($data['rcaRelatedSA'])){
            $node->field_rcarelatedsa['und'][0]['value'] = uwwtd_check_text($data['rcaRelatedSA']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area Id of the related sensitive area found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaRelatedSARemark for RCA SASA*/
        //text
        if(isset($data['rcaRelatedSARemark'])){
            $node->field_rcarelatedsaremark['und'][0]['value'] = uwwtd_check_text($data['rcaRelatedSARemark']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SASA remarks area found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaCodePredecessor for RCA SALSAPredecessor*/
        //text
        if(isset($data['rcaCodePredecessor'])){
            $node->field_rcacodepredecessor['und'][0]['value'] = uwwtd_check_text($data['rcaCodePredecessor']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SALSA id code area found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcaEvolutionType for RCA SALSAPredecessor*/
        //text
        if(isset($data['rcaEvolutionType'])){
            $node->field_rcaevolutiontype['und'][0]['value'] = uwwtd_check_text($data['rcaEvolutionType']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SALSA reason area found.', $timestamp, $i, $node);
        
        /* DG 15/12/2015 add rcasalsaRemark for RCA SALSAPredecessor*/
        //text
        if(isset($data['rcasalsaRemark'])){
            $node->field_rcasalsaremark['und'][0]['value'] = uwwtd_check_text($data['rcasalsaRemark']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SALSA remarks area found.', $timestamp, $i, $node);
        
        // dsm($node);

        //BB - 22/08 - 9th report changes
        if(isset($data['ReceivingAreasSAParameter'])) {
        	$node->field_rcaanitro['und'][0]['value'] = '0';
        	$node->field_rcaaphos['und'][0]['value'] = '0';
        	$node->field_rcab['und'][0]['value'] = '0';
        	$node->field_rcac['und'][0]['value'] = '0';
        	foreach ($data['ReceivingAreasSAParameter'] as $rcaSAParameter) {
        		switch ($rcaSAParameter['rcaParameter']) {
        			case 'aN':
        				$node->field_rcaanitro['und'][0]['value'] = '1';
        				$node->field_rcaanstartdate['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaStartDate'], $message);
        				$node->field_rcaadatedesignation['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaDateDesignation'], $message);
        				break;
        			case 'aP':
        				$node->field_rcaaphos['und'][0]['value'] = '1';
        				$node->field_rcaapstartdate['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaStartDate'], $message);
        				$node->field_rcaapdatedesignation['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaDateDesignation'], $message);
        				break;
        			case 'b':
        				$node->field_rcab['und'][0]['value'] = '1';
        				$node->field_rcabstartdate['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaStartDate'], $message);
        				$node->field_rcabdatedesignation['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaDateDesignation'], $message);
        				break;
        			case 'c':
        				$node->field_rcac['und'][0]['value'] = '1';
        				$node->field_rcacstardate['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaStartDate'], $message);
        				$node->field_rcacdatedesignation['und'][0]['value'] = uwwtd_check_date($rcaSAParameter['rcaDateDesignation'], $message);

                        //10th report
                        if(isset($rcaSAParameter['rcaCRelevantDirective'])){
                          $node->field_rcacrelevantdirective['und'][0]['value'] = uwwtd_check_text($rcaSAParameter['rcaCRelevantDirective']);
                        }
                        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area relevant directive information found.', $timestamp, $i, $node);
                        
                        //text
                        if(isset($rcaSAParameter['rcaCIDOtherDirective'])){
                          $node->field_rcacidotherdirective['und'][0]['value'] = uwwtd_check_text($rcaSAParameter['rcaCIDOtherDirective']);
                        }
                        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area other directive information found.', $timestamp, $i, $node);
                        
                        
                        //date
                        if(isset($rcaSAParameter['rcaCDateOtherDirective'])){
                          $check = uwwtd_check_date($rcaSAParameter['rcaCDateOtherDirective'], $message);
                          if ($message != '') {
                            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                          }
                          if($check !== false){
                            $node->field_rcacdateotherdirective['und'][0]['value'] = $check;
                          }              
                        }
                        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area other directive information date found.', $timestamp, $i, $node);

        				break;
        			default:
        				break;
        		}
        	}
        }

        // TODO : improve this test
        //9th report
        //TODO e.v -> voir avec b et nd si on doit boucler , si il peux y avoir plusieurs ReceivingAreasSA54:row pour le même rcaCode
        if(isset($data['ReceivingAreasSA54'])) {
//             foreach ($data['ReceivingAreasSA54'] as $key => $ReceivingAreasSA54) {  //e.v add foreach ==> todo see where we use the field to sum them all in the processing
              switch ($data['ReceivingAreasSA54'][0]['rcaMethod54']) {
              	case 'M':                                          
                    $node->field_rcamethod54['und'][0]['value'] = 'M';  //todo e.v problem here if multiple $key
              		$node->field_rcanincomingmeasured['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaNIncoming54'];
          			$node->field_rcandischargedmeasured['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaNDischarged54'];
          			$node->field_rcapincomingmeasured['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaPIncoming54'];
          			$node->field_rcapdischargedmeasured['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaPDischarged54'];
              		break;
              	case 'C':
                      $node->field_rcamethod54['und'][0]['value'] = 'C';    //todo e.v problem here if multiple $key
              		$node->field_rcanincomingcalculated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaNIncoming54'];
          			$node->field_rcandischargedcalculated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaNDischarged54'];
          			$node->field_rcapincomingcalculated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaPIncoming54'];
          			$node->field_rcapdischargedcalculated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaPDischarged54'];
              		break;
              	case 'E':
                      $node->field_rcamethod54['und'][0]['value'] = 'E';     //todo e.v problem here if multiple $key
              		$node->field_rcanincomingestimated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaNIncoming54'];
          			$node->field_rcandischargedestimated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaNDischarged54'];
          			$node->field_rcapincomingestimated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaPIncoming54'];
          			$node->field_rcapdischargedestimated['und'][0]['value']	= $data['ReceivingAreasSA54'][0]['rcaPDischarged54'];
              		break;
           		default:
           			break;
              }

            //numeric
            if(isset($data['ReceivingAreasSA54'][0]['rcaPlants54'])){
              $check = uwwtd_check_numeric($data['ReceivingAreasSA54'][0]['rcaPlants54']);
              if (false === $check) {
                  uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
              } else {
                  $node->field_rcaplants['und'][0]['value'] = $check;
              }
            }   
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area plants information found.', $timestamp, $i, $node);

            if(isset($data['ReceivingAreasSA54'][0]['rcaPlantsCapacity54'])){
              $check = uwwtd_check_numeric($data['ReceivingAreasSA54'][0]['rcaPlantsCapacity54']);
              if (false === $check) {
                  uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
              } else {
                  $node->field_rcaplantscapacity['und'][0]['value'] = $check;
              }
            }   
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area plants capacity information found.', $timestamp, $i, $node);
            
//             }
       
        }

        //10 th report e.v
        //text TODO test avec 0 1 n value
        if(isset($data['ReceivingAreasSALSAPredecessor'])) { 

          //rcaCodePredecessor
          if(isset($data['ReceivingAreasSALSAPredecessor'][0]['rcaCodePredecessor'])){
              foreach ($data['ReceivingAreasSALSAPredecessor'] as $key => $ReceivingAreasSALSAPredecessor) {
                //multiple value  => todo e.v change field confiuration to accept multiple value !           
                $node->field_rcacodepredecessor['und'][$key]['value'] = uwwtd_check_text($ReceivingAreasSALSAPredecessor['rcaCodePredecessor']);
              }
          }
          else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SALSA id code area found.', $timestamp, $i, $node);

          //rcasalsaRemark
          if(isset($data['ReceivingAreasSALSAPredecessor'][0]['rcasalsaRemark'])){
              foreach ($data['ReceivingAreasSALSAPredecessor'] as $key => $ReceivingAreasSALSAPredecessor) {
                //multiple value  => todo e.v change field confiuration to accept multiple value !
                $node->field_rcasalsaremark['und'][$key]['value'] = uwwtd_check_text($ReceivingAreasSALSAPredecessor['rcasalsaRemark']);
              }
          }
          else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SALSA remarks area found.', $timestamp, $i, $node);
        }

        //10 th report e.v
        //text TODO test avec 0 1 n value
        if(isset($data['ReceivingAreasSASA'])) { 

          //rcaRelatedSA
          if(isset($data['ReceivingAreasSASA'][0]['rcaRelatedSA'])){
              foreach ($data['ReceivingAreasSASA'] as $key => $ReceivingAreasSASA) {
                //multiple value  => todo e.v change field confiuration to accept multiple value !           
                $node->field_rca_sensitive_area['und'][$key]['value'] = uwwtd_check_text($ReceivingAreasSASA['rcaRelatedSA']);
              }
          }
          else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SASA rcaRelatedSA area found.', $timestamp, $i, $node);

          //rcaRelatedSARemark
          if(isset($data['ReceivingAreasSASA'][0]['rcaRelatedSARemark'])){
              foreach ($data['ReceivingAreasSASA'] as $key => $ReceivingAreasSASA) {
                //multiple value  => todo e.v change field confiuration to accept multiple value !
                $node->field_rcarelatedsaremark['und'][$key]['value'] = uwwtd_check_text($ReceivingAreasSASA['rcaRelatedSARemark']);
              }
          }
          else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No receiving area SASA rcaRelatedSARemark area found.', $timestamp, $i, $node);
        }





        // save node

        node_save($node);

        if (empty($node->nid)) {
            uwwtd_trace('receiving_area ' . $siteId . ' node_save() error2');
        }        
       
        return $node;
    }
    
    uwwtd_trace('import receiving_area error ' . var_export($data, true));
    return false;
}

function uwwtd_import_bigcity_batch_process($newFile, $annee, $datas, $repCode, $timestamp, &$context){
    global $language;
    if(!empty($datas)){
        if (!isset($context['sandbox']['progress'])) {
        

            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = $datas;
            $_SESSION['totBc'] = $context['sandbox']['max'];
        }
        $i = $context['sandbox']['progress'];
        $data = $context['sandbox']['ids'][$i];

        // Check basic elements exist
        // errors
        $e = 0;
        if($data['bigCity'] == '' || !isset($data['bigCity'])){
            $e = 1;
            $emes = t('The big city does not have \'title\' information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
        }
        if($data['bigCityID'] == '' || !isset($data['bigCityID'])){
            $e = 1;
            $emes = t('The big city does not have \'identification\' information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
        }
        if($data['bigCountryCode'] == '' || !isset($data['bigCountryCode'])){
            $e = 1;
            $emes = t('The big city does not have \'code\' information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
        }
        uwwtd_trace('import big_city : ' . $annee.'_big_'.uwwtd_check_text($data['bigCityID']));
        // Check if no errors were found
        if($e == 0){

            // Variables  //text
            $siteId = $annee.'_big_'.uwwtd_check_text($data['bigCityID']);

            // Check if id exists
            $exists = uwwtd_check_exist($siteId);
            uwwtd_trace('big_city ' . $siteId . ' exists ? "' . $exists . '".');
            // If the element exists
            if(!$exists){
                $node = new stdClass();
                $node->type = 'big_city';
                $node->uid = 1;
                $node->translate = 0;
                $node->tnid = 0;
                $node->sticky = 0;
                $node->promote = 0;
                $node->comment = 0;
                $node->language = $language->language;
                $_SESSION['newBc']++;
                node_save($node);
                
                if (empty($node->nid)) {
                    uwwtd_trace('big_city ' . $siteId . ' node_save() error');
                } else {
                    uwwtd_trace('big_city ' . $siteId . ' nid : ' . $node->nid);
                }                
            }
            else{
                // load the node
                $node = node_load($exists);
                $node->type = 'big_city';
                $_SESSION['upBc']++;
                
                if (empty($node->nid)) {
                    uwwtd_trace('big_city ' . $siteId . ' node_save() error');
                } else {
                    uwwtd_trace('big_city ' . $siteId . ' nid : ' . $node->nid);
                }                 
            }
            // site mapping
            $node->field_siteid['und'][0]['value'] = $siteId;
            $node->field_valid['und'][0]['value'] = 0;
            $node->field_anneedata['und'][0]['value'] = $annee;
            $node->field_sourcefile = array(
                'und' => array(
                    0 => array(
                        'fid' => trim($newFile->fid),
                        'filename' => trim($newFile->filename .".xml"),
                        'uri' => trim($newFile->uri),
                        'status' => 1,
                        'display' => 1
                    )
                )
            );
            unset($node->field_bcaggliste['und']);

            // data mapping
            $node->field_repcode['und'][0]['value'] = $repCode;

            //text
            $node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['bigCityID']);

            //text
            $node->title = uwwtd_check_text($data['bigCity']);

            //text
            $node->field_bigcountrycode['und'][0]['value'] = uwwtd_check_text($data['bigCountryCode']);

            // save node
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('big_city ' . $siteId . ' node_save() error2');
            }             
        } else {
            uwwtd_trace('import big_city error ' . var_export($data, true));
        }

        $context['sandbox']['progress']++;
        uwwtd_flush_errors();
        if(isset($node)){
            $context['message'] = '(' .$annee . ') ' . t('Processing big city').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
            $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
        }
        else{
            $context['message'] = '(' .$annee . ') ' . t('Failed to import a big city.');
            $context['results'][] = t('Failed to import a big city.');
        }
        
        if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    } else {
       uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No big cities found in the dataset ' . $annee, $timestamp);
    }
}

function uwwtd_import_agglo_batch_process($newFile, $annee, $datas, $repCode, $timestamp, &$context){
    if(!empty($datas)){
        if (!isset($context['sandbox']['progress'])) {
            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = $datas;
            $_SESSION['totAgg'] = $context['sandbox']['max'];
        }
        
        $i = $context['sandbox']['progress'];
        $j=0;
        while($j<10 && $context['sandbox']['progress']<$context['sandbox']['max']){
            $k=$i+$j;
            $data = $context['sandbox']['ids'][$k];
            $node = uwwtd_update_agg_node($data, $newFile, $repCode, $annee, $timestamp, $i);
            if($node){
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
            }
            else{
                $context['results'][] = t('Failed to import an agglomeration.');
            }
            $context['sandbox']['progress']++;
            $j++;
        }
//      $context['sandbox']['progress']++;
        uwwtd_flush_errors();
        if($node){
            $context['message'] = '(' .$annee . ') ' . t('Processing agglomeration').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        }
        else{
            $context['message'] = '(' .$annee . ') ' . t('Failed to import an agglomeration.');
        }
        
        if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    }else{
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No Agglomerations found in the dataset ' . $annee, $timestamp);
    }
}

function uwwtd_update_agg_node($data, $newFile, $repCode, $annee, $timestamp, $i){
    global $language;
    // Check basic elements exist
    // errors
    $e = 0;
    if(!isset($data['aggName']) || empty($data['aggName'])){
        $e = 1;
        $emes = t('The agglomeration does not have \'title\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }

    if(!isset($data['aggCode']) || empty($data['aggCode'])){
        $e = 1;
        $emes = t('The agglomeration does not have \'identification\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }

    // Check if no errors were found
    uwwtd_trace('import agglomeration : ' . $annee.'_agg_'.uwwtd_check_text($data['aggCode']));
    if($e == 0){
        $siteId = $annee.'_agg_'.uwwtd_check_text($data['aggCode']);
        // Check if id exists
        $exists = uwwtd_check_exist($siteId);
        uwwtd_trace('agglomeration ' . $siteId . ' exists ? "' . $exists . '".');
        // If the element exists
        if(!$exists){
            $node = new stdClass();
            $node->type = 'agglomeration';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            $_SESSION['newAgg']++;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('agglomeration ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('agglomeration ' . $siteId . ' nid : ' . $node->nid);
            }            
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'agglomeration';
            $_SESSION['upAgg']++;
            
            if (empty($node->nid)) {
                uwwtd_trace('agglomeration ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('agglomeration ' . $siteId . ' nid : ' . $node->nid);
            }             
        }

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
        $node->field_aggart6compliance['und'][0]['value'] = 'NI';
        $node->field_aggart3addcompliance['und'][0]['value'] = '';

        // Check latitude :
        if(!isset($data['aggLatitude']) || empty($data['aggLatitude'])){
            $emes = t('The agglomeration does not have latitude information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            $checkLatitude = '0';
        } else {
            $checkLatitude = uwwtd_check_coordinate($data['aggLatitude'], 'latitude', $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($checkLatitude === false){
                $checkLatitude = '0';
                $emes = t('The latitude for this agglomeration is invalid');
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            }
        }

        // Check longitude :
        if(!isset($data['aggLongitude']) || empty($data['aggLongitude'])){
            $emes = t('The agglomeration does not have longitude information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            $checkLongitude = '0';
        } else {
            $checkLongitude = uwwtd_check_coordinate($data['aggLongitude'], 'longitude', $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($checkLongitude === false){
                $checkLongitude = '0';
                $emes = t('The longitude for this agglomeration is invalid');
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            }
        }

        $geoData = uwwtd_coord_transform($checkLatitude, $checkLongitude);
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
                
        //decimal
        $node->field_agglatitude['und'][0]['value'] = $checkLatitude;
        
        //decimal
        $node->field_agglongitude['und'][0]['value'] = $checkLongitude;
        
        
        
        $node->field_sourcefile = array(
            'und' => array(
                0 => array(
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );
        unset($node->field_agguwwliste['und']);
        unset($node->field_aggbcliste['und']);

        // data mapping
            $node->field_repcode['und'][0]['value'] = $repCode;
//             $repcode_f = str_replace('_', '-',  $repCode);
//             $boom = explode('-', $repcode_f);
//             $boom = explode(' ', $boom[0]);
//             $rep = $boom[0];
            $rep = uwwtd_get_countrycode_from_repcode($repCode);
            if($rep == 'LT'){
                $node->field_regionnuts['und'][0]['value'] = 'LT00';
            }

            if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
                $node->field_regionnuts['und'][0]['value'] = 'PL112';
            }


        $node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['aggCode']);

        $node->title = uwwtd_check_text($data['aggName']);

        //text
        if(isset($data['aggState'])){
            $node->field_status['und'][0]['value'] = uwwtd_check_text($data['aggState']);
//             //TODO delete!!
//             if ($node->field_status['und'][0]['value'] == 1 ) {
//                 $node->field_status['und'][0]['value'] = 0;
//             } else {
//                 $node->field_status['und'][0]['value'] = 1;
//             }
//             //TODO delete!!
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration status information found.', $timestamp, $i, $node);

        //text
        if(isset($data['aggNUTS'])){
            $node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['aggNUTS']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration nut code information found.', $timestamp, $i, $node);

        
        //numeric
        if(isset($data['aggGenerated'])){
//             if(uwwtd_check_numeric($data['aggGenerated'], $timestamp, $i, $node) !== false){
//                 $node->field_agggenerated['und'][0]['value'] = uwwtd_check_numeric($data['aggGenerated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_numeric($data['aggGenerated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
            } else {
                $node->field_agggenerated['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration generated load information found.', $timestamp, $i, $node);
        
        //text
        //add check for benoit 2015/11/17  
        //DG 15/12/2015 add bigCityID     
        if(isset($data['bigID']) || isset($data['aggBigID']) || isset($data['bigCityID'])){
            if(isset($data['bigID'])){
                $node->field_aggbigid['und'][0]['value'] = uwwtd_check_text($data['bigID']);
            }
            if(isset($data['aggBigID'])){
                $node->field_aggbigid['und'][0]['value'] = uwwtd_check_text($data['aggBigID']);
            }
            if(isset($data['bigCityID'])){
                $node->field_aggbigid['und'][0]['value'] = uwwtd_check_text($data['bigCityID']);
            }
        }
        else {
            if (isset($data['aggGenerated']) && $data['aggGenerated'] > 150000) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration big id information found.', $timestamp, $i, $node);
            }
        }

        //memo
        if(isset($data['aggCalculation'])){
            $node->field_aggcalculation['und'][0]['value'] = uwwtd_check_text($data['aggCalculation']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration calculation information found.', $timestamp, $i, $node);

        //text
        if(isset($data['aggChanges'])){
            $node->field_aggchanges['und'][0]['value'] = uwwtd_check_text($data['aggChanges']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration changes information found.', $timestamp, $i, $node);

        //memo
        if(isset($data['aggChangesComment'])){
            $node->field_aggchangescomment['und'][0]['value'] = uwwtd_check_text($data['aggChangesComment']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration changes comment information found.', $timestamp, $i, $node);

        //date
        if(isset($data['aggDateArt3']) && isset($data['aggDateArt4'])){
            $check3 = uwwtd_check_date($data['aggDateArt3'], $message);
            $check4 = uwwtd_check_date($data['aggDateArt4'], $message);
            $check5 = false;
            if(isset($data['aggDateArt5']) && !empty($data['aggDateArt5'])){
                /*
                    Deadline for the implementation of requirements of article 5 of the UWWTD (provision of more stringent treatment to urban waste water entering collecting system and being discharge after a treatment into sensitive areas or their catchments). 
                    This value is required for all active agglomerations of size ≥ 10 000 p.e., served by UWWTPs discharging into sensitive areas.
                */
                $check5 = uwwtd_check_date($data['aggDateArt5'], $message);
            }
            //Case when aggDateArt5 is not provided, but needed for all compliance processing
            elseif(isset($data['aggPeriodOver']) && $data['aggGenerated'] > 10000){
                $check5 = uwwtd_check_date($data['aggPeriodOver'], $message);
            }
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            } 
            if($check3 !== false){
                $node->field_aggperiodover3['und'][0] = array(
                    'value'=> $check3,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );
                $node->field_aggperiodover6['und'][0] = array(
                    'value'=> $check3,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );
                //field_aggperiodover7 ????
            }
            if($check4 !== false){
                $node->field_aggperiodover4['und'][0] = array(
                    'value'=> $check4,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );
            }
            if($check5 !== false){
                $node->field_aggperiodover5['und'][0] = array(
                    'value'=> $check5,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );
            }elseif (isset($data['aggGenerated']) && $data['aggGenerated'] > 10000){
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], "The aggDateArt5 is required for all active agglomerations of size ≥ 10 000 p.e., served by UWWTPs discharging into sensitive areas.", $timestamp, $i, $node);
                
            }
            
        }
        elseif(isset($data['aggPeriodOver'])){
            $check = uwwtd_check_date($data['aggPeriodOver'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }        
            if($check !== false){
                $node->field_aggperiodover3['und'][0] = array(
                    'value'=> $check,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );

                $node->field_aggperiodover4['und'][0] = array(
                    'value'=> $check,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );

                $node->field_aggperiodover5['und'][0] = array(
                    'value'=> $check,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );
                
                $node->field_aggperiodover6['und'][0] = array(
                    'value'=> $check,
                    'timezone'=>'Europe/Berlin',
                    'timezone_bd'=>'Europe/Berlin',
                    'date_type'=>'date'
                );
            }   
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration period over information found.', $timestamp, $i, $node);

        //decimal
        if(isset($data['aggC1'])){        
            $check = uwwtd_check_pourc($data['aggC1'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggc1['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration c1 information found.', $timestamp, $i, $node);

        //text
        if(isset($data['aggMethodC1'])){
            $node->field_aggmethodc1['und'][0]['value'] = uwwtd_check_text($data['aggMethodC1']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration c1 method information found.', $timestamp, $i, $node);

        //decimal
        if(isset($data['aggC2'])){
            $check = uwwtd_check_pourc($data['aggC2'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggc2['und'][0]['value'] = $check;
            }        
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration c2 information found.', $timestamp, $i, $node);

        //text
        if(isset($data['aggMethodC2'])){
            $node->field_aggmethodc2['und'][0]['value'] = uwwtd_check_text($data['aggMethodC2']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration c2 method information found.', $timestamp, $i, $node);

        //decimal
        if(isset($data['aggPercWithoutTreatment'])){
            $check = uwwtd_check_pourc($data['aggPercWithoutTreatment'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggpercwithouttreatment['und'][0]['value'] = $check;
            }        
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration percentage without treatment information found.', $timestamp, $i, $node);

        //text
        if(isset($data['aggMethodWithoutTreatment'])){
            $node->field_aggmethodwithouttreatment['und'][0]['value'] = uwwtd_check_text($data['aggMethodWithoutTreatment']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration method percentage without treatment information found.', $timestamp, $i, $node);

        //decimal
        if(isset($data['aggPercPrimTreatment'])){
            $check = uwwtd_check_pourc($data['aggPercPrimTreatment'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggpercprimtreatment['und'][0]['value'] = $check;
            }        
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration percentage with primary treatment information found.', $timestamp, $i, $node);

        //decimal
        if(isset($data['aggPercSecTreatment'])){
            $check = uwwtd_check_pourc($data['aggPercSecTreatment'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggpercsectreatment['und'][0]['value'] = $check;
            }        
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration percentage with secondary treatment information found.', $timestamp, $i, $node);

        //decimal
        if(isset($data['aggPercStringentTreatment'])){
            $check = uwwtd_check_pourc($data['aggPercStringentTreatment'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggpercstringenttreatment['und'][0]['value'] = $check;
            }        
        } else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration percentage with more stringent treatment information found.', $timestamp, $i, $node);

        //text
        if(isset($data['aggHaveRegistrationSystem'])){
            $node->field_agghaveregistrationsystem['und'][0]['value'] = uwwtd_check_text($data['aggHaveRegistrationSystem']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration registration system information found.', $timestamp, $i, $node);

        //text
        if(isset($data['aggExistMaintenancePlan'])){
            $node->field_aggexistmaintenanceplan['und'][0]['value'] = uwwtd_check_text($data['aggExistMaintenancePlan']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration maintenance plan information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['aggPressureTest'])){
            $check = uwwtd_check_bool($data['aggPressureTest']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_aggpressuretest['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration pressure test information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['aggVideoInspections'])){
            $check = uwwtd_check_bool($data['aggVideoInspections']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } else {
                $node->field_aggvideoinspections['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration video inspections information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['aggOtherMeasures'])){
            $check = uwwtd_check_bool($data['aggOtherMeasures']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0, $node);
            } else {
                $node->field_aggothermeasures['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration other mesures information found.', $timestamp, $i, $node);

        //memo
        if(isset($data['aggExplanationOther'])){
            $node->field_aggexplanationother['und'][0]['value'] = uwwtd_check_text($data['aggExplanationOther']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration explanation of other measures information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['aggSewageNetwork'])){
            $node->field_aggsewagenetwork['und'][0]['value'] = strtoupper(uwwtd_check_text($data['aggSewageNetwork']));}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration type of collecing system information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['aggBestTechnicalKnowledge'])){
            $node->field_aggbesttechnicalknowledge['und'][0]['value'] = uwwtd_check_text($data['aggBestTechnicalKnowledge']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration best technical knowledge information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['aggDilutionRates'])){
            $check = uwwtd_check_bool($data['aggDilutionRates']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0, $node);
            } else {
                $node->field_agg_dilution_rates['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration the measures based on: Dilution rates information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['aggCapacity'])){
            $check = uwwtd_check_bool($data['aggCapacity']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0, $node);
            } else {
                $node->field_aggcapacity['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration the measures based on: Capacity in relation to dry weather flow information found.', $timestamp, $i, $node);

        //bool
        if(isset($data['aggAccOverflows'])){
            $check = uwwtd_check_bool($data['aggAccOverflows']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0, $node);
            } else {
                $node->field_aggaccoverflows['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration the measures based on: Acceptable number of overflows per year information found.', $timestamp, $i, $node);

        //numeric
        if(isset($data['aggAccOverflowNumber'])){
//             if(uwwtd_check_numeric($data['aggAccOverflowNumber'], $timestamp, $i, $node) !== false){
//                 $node->field_aggaccoverflownumber['und'][0]['value'] = uwwtd_check_numeric($data['aggAccOverflowNumber'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_numeric($data['aggAccOverflowNumber']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
            } else {
                $node->field_aggaccoverflownumber['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration indicate number of overflows information found.', $timestamp, $i, $node);

        //numeric
        if(isset($data['aggSewerOverflows_m3'])){
//             if(uwwtd_check_numeric($data['aggSewerOverflows_m3'], $timestamp, $i, $node) !== false){
//                 $node->field_aggseweroverflows_m3['und'][0]['value'] = uwwtd_check_numeric($data['aggSewerOverflows_m3'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_numeric($data['aggSewerOverflows_m3']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
            } else {
                $node->field_aggseweroverflows_m3['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration number of raw sewage discharged from combined sewer overflows (CSOs) in the reporting year : m3 / year found.', $timestamp, $i, $node);
    
        //numeric
        if(isset($data['aggSewerOverflows_pe'])){
//             if(uwwtd_check_numeric($data['aggSewerOverflows_pe'], $timestamp, $i, $node) !== false){
//                 $node->field_aggseweroverflows_pe['und'][0]['value'] = uwwtd_check_numeric($data['aggSewerOverflows_pe'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_numeric($data['aggSewerOverflows_pe']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
            } else {
                $node->field_aggseweroverflows_pe['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration number of raw sewage has been discharged through combined sewer overflows (CSOs) in the year reported: p.e. found.', $timestamp, $i, $node);
        
        //memo
        if(isset($data['aggRemarks'])){
            $node->field_aggremarks['und'][0]['value'] = uwwtd_check_text($data['aggRemarks']);}
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration remarks information found.', $timestamp, $i, $node);
        
        //date
        if(isset($data['aggForecast'])){
//             if(uwwtd_check_date($data['aggForecast'], $timestamp, $i, $node) !== false){
//                 $node->field_aggforecast['und'][0]['value'] = uwwtd_check_date($data['aggForecast'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_date($data['aggForecast'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggforecast['und'][0]['value'] = $check;

                if ($repCode === 'RO05') {
                    $node->field_aggperiodover3['und'][0] = array(
                        'value'=> $check,
                        'timezone'=>'Europe/Berlin',
                        'timezone_bd'=>'Europe/Berlin',
                        'date_type'=>'date'
                    );
                }
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration when the total generated load of the agglomeration will be collected by the collection system or sent by IAS found.', $timestamp, $i, $node);
        //DG 15/12/2015 add aggBeginLife
        //date
        if(isset($data['aggBeginLife'])){
            $check = uwwtd_check_date($data['aggBeginLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggbeginlife['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration date begin life found.', $timestamp, $i, $node);
        //DG 15/12/2015 add aggEndLife        
        //date
        if(isset($data['aggEndLife'])){
            $check = uwwtd_check_date($data['aggEndLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_aggendlife['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No agglomeration date end life found.', $timestamp, $i, $node);
        
        // save node
        node_save($node);

        if (empty($node->nid)) {
            uwwtd_trace('agglomeration ' . $siteId . ' node_save() error2');
        }   
                
        return $node;
    }
    
    uwwtd_trace('import agglomeration error ' . var_export($data, true));
    return false;
}

/**
 * Traitement de 10 stations d'épurations.
 */
function uwwtd_import_uww_batch_process($newFile, $annee, $datas, $repCode, $timestamp, &$context){
    if(!empty($datas)){
        if (!isset($context['sandbox']['progress'])) {
            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = $datas;
            $_SESSION['totUww'] = $context['sandbox']['max'];
        }
        $i = $context['sandbox']['progress'];
        $j=0;
        while($j<10 && $context['sandbox']['progress']<$context['sandbox']['max']){
            $k=$i+$j;
            $data = $context['sandbox']['ids'][$k];
            $node = uwwtd_update_uww_node($data, $newFile, $repCode, $annee, $timestamp, $k);
            if($node){
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
            }
            else{
                $context['results'][] = t('Failed to import an UWWTP.');
            }
            $context['sandbox']['progress']++;
            $j++;
        }
        uwwtd_flush_errors();
        if($node){
            $context['message'] = '(' .$annee . ') ' . t('Processing waste water treatment plant').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        }
        else{
            $context['message'] = '(' .$annee . ') ' . t('Failed to import an UWWTP.');
        }
        
        if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    }else{
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No UWWTPs found in the dataset ' . $annee, $timestamp);
    }
}

function uwwtd_update_uww_node($data, $newFile, $repCode, $annee, $timestamp, $i){
    global $language;
    $e = 0;
    if($data['uwwName'] == '' || !isset($data['uwwName'])){
        $e = 1;
        $emes = t('The urban waste water treatment plant does not have \'title\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }
    if($data['uwwCode'] == '' || !isset($data['uwwCode'])){
        $e = 1;
        $emes = t('The urban waste water treatment plant does not have \'identification\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }
    
    // Check if no errors were found
    uwwtd_trace('import uwwtp : ' . $annee.'_uww_'.uwwtd_check_text($data['uwwCode']));
    if($e == 0){

        // Variables
        $siteId = $annee.'_uww_'.uwwtd_check_text($data['uwwCode']);

        // Check if id exists
        $exists = uwwtd_check_exist($siteId);
        uwwtd_trace('uwwtp ' . $siteId . ' exists ? "' . $exists . '".');
        // If the element exists
        if(!$exists){
            $node = new stdClass();
            $node->type = 'uwwtp';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            $_SESSION['newUww']++;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('uwwtp ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('uwwtp ' . $siteId . ' nid : ' . $node->nid);
            }            
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'uwwtp';
            $_SESSION['upUww']++;
            
            if (empty($node->nid)) {
                uwwtd_trace('uwwtp ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('uwwtp ' . $siteId . ' nid : ' . $node->nid);
            }             
        }


        // site mapping
        // calculate treatment type
        $treatment = 'NI';
        //bool 
        if(isset($data['uwwPrimaryTreatment'])) {
            $check = uwwtd_check_bool($data['uwwPrimaryTreatment']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'P';
            }
        }
        //bool        
        //if(isset($data['uwwSecondaryTreatment']) && uwwtd_check_bool($data['uwwSecondaryTreatment'], $timestamp, $i, $node) == 1) $treatment = 'S';
        if(isset($data['uwwSecondaryTreatment'])) {
            $check = uwwtd_check_bool($data['uwwSecondaryTreatment']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'S';
            }
        }        
        //bool        
        //if(isset($data['uwwNRemoval']) && uwwtd_check_bool($data['uwwNRemoval'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwNRemoval'])) {
            $check = uwwtd_check_bool($data['uwwNRemoval']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        }        
        //bool         
        //if(isset($data['uwwPRemoval']) && uwwtd_check_bool($data['uwwPRemoval'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwPRemoval'])) {
            $check = uwwtd_check_bool($data['uwwPRemoval']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        }         
        //bool        
        //if(isset($data['uwwUV']) && uwwtd_check_bool($data['uwwUV'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwUV'])) {
            $check = uwwtd_check_bool($data['uwwUV']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        }         
        //bool          
        //if(isset($data['uwwChlorination']) && uwwtd_check_bool($data['uwwChlorination'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwChlorination'])) {
            $check = uwwtd_check_bool($data['uwwChlorination']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        }        
        //bool          
        //if(isset($data['uwwOzonation']) && uwwtd_check_bool($data['uwwOzonation'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwOzonation'])) {
            $check = uwwtd_check_bool($data['uwwOzonation']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        }         
        //bool          
        //if(isset($data['uwwSandFiltration']) && uwwtd_check_bool($data['uwwSandFiltration'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwSandFiltration'])) {
            $check = uwwtd_check_bool($data['uwwSandFiltration']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        }        
        //bool          
        //if(isset($data['uwwMicroFiltration']) && uwwtd_check_bool($data['uwwMicroFiltration'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwMicroFiltration'])) {
            $check = uwwtd_check_bool($data['uwwMicroFiltration']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        }         
        //bool            
        //if(isset($data['uwwOtherTreatment']) && uwwtd_check_bool($data['uwwOtherTreatment'], $timestamp, $i, $node) == 1) $treatment = 'MS';
        if(isset($data['uwwOtherTreatment'])) {
            $check = uwwtd_check_bool($data['uwwOtherTreatment']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, $i, $node);
            } elseif ($check == 1) {           
                $treatment = 'MS';
            }
        } 
                
        $node->field_uwwtreatmenttype['und'][0]['value'] = $treatment;

        $node->field_siteid['und'][0]['value'] = $siteId;
        $node->field_valid['und'][0]['value'] = 0;
        $node->field_anneedata['und'][0]['value'] = $annee;
        //$node->field_uwwcompliance['und'][0]['value'] = 'NI';

        // Check latitude :
        if(!isset($data['uwwLatitude']) || empty($data['uwwLatitude'])){
            $emes = t('The urban waste water treatment plan does not have latitude information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
        } else {
            $checkLatitude = uwwtd_check_coordinate($data['uwwLatitude'], 'latitude', $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($checkLatitude === false){
                $checkLatitude = '0';
                $emes = t('The latitude for this urban waste water treatment plant is invalid');
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            }
        }

        // Check longitude
        if(!isset($data['uwwLongitude']) || empty($data['uwwLongitude'])){
            $emes = t('The urban waste water treatment plan does not have longitude information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
        } else {
            $checkLongitude = uwwtd_check_coordinate($data['uwwLongitude'], 'longitude', $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($checkLongitude === false){
                $checkLongitude = '0';
                $emes = t('The longitude for this urban waste water treatment plan is invalid');
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            }
        }

        $geoData = uwwtd_coord_transform($checkLatitude, $checkLongitude);
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
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );
        //TODO : Why is thse 2 lines were commented ?
        unset($node->field_uwwaggliste['und']);
        unset($node->field_uwwdcpliste['und']);
        
        // data mapping
        $node->field_repcode['und'][0]['value'] =  $repCode;
//             $repcode_f = str_replace('_', '-',  $repCode);
//             $boom = explode('-', $repcode_f);
//             $boom = explode(' ', $boom[0]);
//             $rep = $boom[0];
        $rep = uwwtd_get_countrycode_from_repcode($repCode);
        if($rep == 'LT'){
            $node->field_regionnuts['und'][0]['value'] = 'LT00';
        }

        if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
            $node->field_regionnuts['und'][0]['value'] = 'PL112';
        }

        //text
        $node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['uwwCode']);
        
        //text
        $node->title = uwwtd_check_text($data['uwwName']);
        
        //text
        if(isset($data['uwwState'])){
            $node->field_status['und'][0]['value'] = uwwtd_check_text($data['uwwState']);
//             if ($node->field_status['und'][0]['value'] == 1 ) {
//                 $node->field_status['und'][0]['value'] = 0;
//             } else {
//                 $node->field_status['und'][0]['value'] = 1;
//             }            
        }else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No UWWTP status information found.', $timestamp, $i, $node);
        
        //text
        if(isset($datas['repCode'])) $node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['repCode']);
        
        /*DG 15/12/2015 add aggCode*/   
        //text
        if(isset($data['aggID']) || isset($data['aggCode'])){
            if(isset($data['aggID'])){
                $node->field_uwwtdaggid['und'][0]['value'] = uwwtd_check_text($data['aggID']);
            }elseif(isset($data['aggCode'])){
                $node->field_uwwtdaggid['und'][0]['value'] = uwwtd_check_text($data['aggCode']);
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs ID of agglomeration information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwCollectingSystem']))$node->field_uwwcollectingsystem['und'][0]['value'] = uwwtd_check_text($data['uwwCollectingSystem']);
        
        //text
        if(isset($data['uwwDateClosing']) && $data['uwwDateClosing']!=''){
            $node->field_validto['und'][0]['value'] = uwwtd_check_text($data['uwwDateClosing']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs date of closing information found.', $timestamp, $i, $node);
        
        //memo
        if(isset($data['uwwHistorie']) && $data['uwwHistorie']!=''){
            $node->field_uwwhistorie['und'][0]['value'] = uwwtd_check_text($data['uwwHistorie']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs information found.', $timestamp, $i, $node);
        
        //TODO pourquoi le refaire ?   peut être écraser par PL112 ? dans ce cas changer al condition plus haut non ?
        //text
        if($rep == 'LT'){
            $node->field_regionnuts['und'][0]['value'] = 'LT00';
        }
        //decimal                                     
        $node->field_uwwlatitude['und'][0]['value'] = $checkLatitude;
        
        //decimal
        $node->field_uwwlongitude['und'][0]['value'] = $checkLongitude;
        
        //text
        if(isset($data['uwwNUTS']))$node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['uwwNUTS']);
        
        //numeric
        if(isset($data['uwwLoadEnteringUWWTP'])){
//             if(uwwtd_check_numeric($data['uwwLoadEnteringUWWTP'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwloadenteringuwwtp['und'][0]['value'] = uwwtd_check_numeric($data['uwwLoadEnteringUWWTP'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_numeric($data['uwwLoadEnteringUWWTP']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node, 'uwwLoadEnteringUWWTP');
            } else {
                $node->field_uwwloadenteringuwwtp['und'][0]['value'] = $check;
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs load entering found.', $timestamp, $i, $node);
        
        //numeric
        if(isset($data['uwwCapacity'])){
//             if(uwwtd_check_numeric($data['uwwCapacity'], $timestamp, $i, $node) !== false){
//                 $node->field_physicalcapacityactivity['und'][0]['value'] = uwwtd_check_numeric($data['uwwCapacity'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_numeric($data['uwwCapacity']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node, 'uwwCapacity');
            } else {
                $node->field_physicalcapacityactivity['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No industries status informations found.', $timestamp, $i, $node);
        
        //bool    
        if(isset($data['uwwPrimaryTreatment'])){
            $check = uwwtd_check_bool($data['uwwPrimaryTreatment']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwPrimaryTreatment');
            } else {
                $node->field_uwwprimarytreatment['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs Primary treatment information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwSecondaryTreatment'])){
            $check = uwwtd_check_bool($data['uwwSecondaryTreatment']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwSecondaryTreatment');
            } else {
                $node->field_uwwsecondarytreatment['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs Secondary treatment information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwNRemoval'])){
            $check = uwwtd_check_bool($data['uwwNRemoval']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwNRemoval');
            } else {
                $node->field_uwwnremoval['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs N removal information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwPRemoval'])){
            $check = uwwtd_check_bool($data['uwwPRemoval']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwPRemoval');
            } else {
                $node->field_uwwpremoval['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs P removal information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwUV'])){
            $check = uwwtd_check_bool($data['uwwUV']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwUV');
            } else {
                $node->field_uwwuv['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs UV information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwChlorination'])){
            $check = uwwtd_check_bool($data['uwwChlorination']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwChlorination');
            } else {
                $node->field_uwwchlorination['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs chlorination information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwOzonation'])){
            $check = uwwtd_check_bool($data['uwwOzonation']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwOzonation');
            } else {
                $node->field_uwwozonation['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs ozonation information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwSandFiltration'])){
            $check = uwwtd_check_bool($data['uwwSandFiltration']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwSandFiltration');
            } else {
                $node->field_uwwsandfiltration['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs sand filtration information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwMicroFiltration'])){
            $check = uwwtd_check_bool($data['uwwMicroFiltration']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwMicroFiltration');
            } else {
                $node->field_uwwmicrofiltration['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs micro filtration information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwOther'])){
            $check = uwwtd_check_bool($data['uwwOther']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwOther');
            } else {
                $node->field_uwwothertreat['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs other type information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwSpecification'])){
            $node->field_uwwspecification['und'][0]['value'] = uwwtd_check_text($data['uwwSpecification']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs specify other type of more stringent treatment information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwBOD5Perf'])){
            $node->field_uwwbod5perf['und'][0]['value'] = uwwtd_check_text($data['uwwBOD5Perf']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs treatment performance: BOD5 information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwCODPerf'])){
            $node->field_uwwcodperf['und'][0]['value'] = uwwtd_check_text($data['uwwCODPerf']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs treatment performance: COD information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwTSSPerf'])){
            $node->field_uwwtssperf['und'][0]['value'] = uwwtd_check_text($data['uwwTSSPerf']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs treatment performance: TSS information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwNTotPerf'])){
            $node->field_uwwntotperf['und'][0]['value'] = uwwtd_check_text($data['uwwNTotPerf']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs treatment performance: N information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwPTotPerf'])){
            $node->field_uwwptotperf['und'][0]['value'] = uwwtd_check_text($data['uwwPTotPerf']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs treatment performance: P information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwOtherPerf'])){
            $node->field_uwwotherperf['und'][0]['value'] = uwwtd_check_text($data['uwwOtherPerf']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs treatment performance: Others information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwBadPerformance'])){
            $check = uwwtd_check_bool($data['uwwBadPerformance']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwBadPerformance');
            } else {
                $node->field_uwwbadperfdesign['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs bad performance information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwAccidents'])){ 
            $check = uwwtd_check_bool($data['uwwAccidents']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwAccidents');
            } else {
                $node->field_uwwaccidents['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs accidents information found.', $timestamp, $i, $node);
        
        //bool
        if(isset($data['uwwBadDesign'])){  
            $check = uwwtd_check_bool($data['uwwBadDesign']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node, 'uwwBadDesign');
            } else {
                $node->field_uwwbaddesign['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs bad design or dimensioning information found.', $timestamp, $i, $node);
        
        //memo
        if(isset($data['uwwInformation'])){
            $node->field_uwwinformation['und'][0]['value'] = truncate_utf8(uwwtd_check_text($data['uwwInformation']), 254);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwBODIncomingMeasured'])){
//             if( uwwtd_check_decimal($data['uwwBODIncomingMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwbodincoming['und'][0]['value'] = uwwtd_check_decimal($data['uwwBODIncomingMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwBODIncomingMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwBODIncomingMeasured');
            } else {
                $node->field_uwwbodincoming['und'][0]['value'] = round($check,2);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads BOD information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwBODIncomingCalculated'])){
//             if( uwwtd_check_decimal($data['uwwBODIncomingCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwbodincomingcalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwBODIncomingCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwBODIncomingCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwBODIncomingCalculated');
            } else {
                $node->field_uwwbodincomingcalculated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads BOD-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwBODIncomingEstimated'])){
//             if(uwwtd_check_decimal($data['uwwBODIncomingEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwbodincomingestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwBODIncomingEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwBODIncomingEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwBODIncomingEstimated');
            } else {
                $node->field_uwwbodincomingestimated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads BOD-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwCODIncomingMeasured'])){
//             if(uwwtd_check_decimal($data['uwwCODIncomingMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwcodincoming['und'][0]['value'] = uwwtd_check_decimal($data['uwwCODIncomingMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwCODIncomingMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwCODIncomingMeasured');
            } else {
                $node->field_uwwcodincoming['und'][0]['value'] = round($check,2);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads COD information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwCODIncomingCalculated'])){
//             if( uwwtd_check_decimal($data['uwwCODIncomingCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwcodincomingcalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwCODIncomingCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwCODIncomingCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwCODIncomingCalculated');
            } else {
                $node->field_uwwcodincomingcalculated['und'][0]['value'] = round($check,2);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads COD-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwCODIncomingEstimated'])){
//             if(uwwtd_check_decimal($data['uwwCODIncomingEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwcodincomingestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwCODIncomingEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwCODIncomingEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwCODIncomingEstimated');
            } else {
                $node->field_uwwcodincomingestimated['und'][0]['value'] = round($check,2);
            }              
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads COD-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwNIncomingMeasured'])){
//             if(uwwtd_check_decimal($data['uwwNIncomingMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwnincoming['und'][0]['value'] = uwwtd_check_decimal($data['uwwNIncomingMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwNIncomingMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwNIncomingMeasured');
            } else {
                $node->field_uwwnincoming['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads Nto information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwNIncomingCalculated'])){
//             if( uwwtd_check_decimal($data['uwwNIncomingCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwnincomingcalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwNIncomingCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwNIncomingCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwNIncomingCalculated');
            } else {
                $node->field_uwwnincomingcalculated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads N-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwNIncomingEstimated'])){
//             if(uwwtd_check_decimal($data['uwwNIncomingEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwnincomingestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwNIncomingEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwNIncomingEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwNIncomingEstimated');
            } else {
                $node->field_uwwnincomingestimated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads N-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwPIncomingMeasured'])){
//             if(uwwtd_check_decimal($data['uwwPIncomingMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwpincoming['und'][0]['value'] = uwwtd_check_decimal($data['uwwPIncomingMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwPIncomingMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwPIncomingMeasured');
            } else {
                $node->field_uwwpincoming['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads Ptot information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwPIncomingCalculated'])){
//             if( uwwtd_check_decimal($data['uwwPIncomingCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwpincomingcalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwPIncomingCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwPIncomingCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwPIncomingCalculated');
            } else {
                $node->field_uwwpincomingcalculated['und'][0]['value'] = round($check,2);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads P-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwPIncomingEstimated'])){
//             if(uwwtd_check_decimal($data['uwwPIncomingEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwpincomingestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwPIncomingEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwPIncomingEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwPIncomingEstimated');
            } else {
                $node->field_uwwpincomingestimated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs incoming loads P-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwBODDischargeMeasured'])){
//             if(uwwtd_check_decimal($data['uwwBODDischargeMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwboddischarge['und'][0]['value'] = uwwtd_check_decimal($data['uwwBODDischargeMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwBODDischargeMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwBODDischargeMeasured');
            } else {
                $node->field_uwwboddischarge['und'][0]['value'] = round($check,2);
            }              
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads BOD information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwBODDischargeCalculated'])){
//             if( uwwtd_check_decimal($data['uwwBODDischargeCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwboddischargecalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwBODDischargeCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwBODDischargeCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwBODDischargeCalculated');
            } else {
                $node->field_uwwboddischargecalculated['und'][0]['value'] = round($check,2);
            }              
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads BOD-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwBODDischargeEstimated'])){
//             if( uwwtd_check_decimal($data['uwwBODDischargeEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwboddischargeestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwBODDischargeEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwBODDischargeEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwBODDischargeEstimated');
            } else {
                $node->field_uwwboddischargeestimated['und'][0]['value'] = round($check,2);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads BOD-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwCODDischargeMeasured'])){
//             if(uwwtd_check_decimal($data['uwwCODDischargeMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwcoddischarge['und'][0]['value'] = uwwtd_check_decimal($data['uwwCODDischargeMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwCODDischargeMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwCODDischargeMeasured');
            } else {
                $node->field_uwwcoddischarge['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads COD information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwCODDischargeCalculated'])){
//             if( uwwtd_check_decimal($data['uwwCODDischargeCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwcoddischargecalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwCODDischargeCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwCODDischargeCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwCODDischargeCalculated');
            } else {
                $node->field_uwwcoddischargecalculated['und'][0]['value'] = round($check,2);
            }              
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads COD-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwCODDischargeEstimated'])){
//             if( uwwtd_check_decimal($data['uwwCODDischargeEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwcoddischargeestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwCODDischargeEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwCODDischargeEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwCODDischargeEstimated');
            } else {
                $node->field_uwwcoddischargeestimated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads COD-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwNDischargeMeasured'])){
//             if(uwwtd_check_decimal($data['uwwNDischargeMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwndischarge['und'][0]['value'] = uwwtd_check_decimal($data['uwwNDischargeMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwNDischargeMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwNDischargeMeasured');
            } else {
                $node->field_uwwndischarge['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads Ntot information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwNDischargeCalculated'])){
//             if(uwwtd_check_decimal($data['uwwNDischargeCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwndischargecalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwNDischargeCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwNDischargeCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwNDischargeCalculated');
            } else {
                $node->field_uwwndischargecalculated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads N-tot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwNDischargeEstimated'])){
//             if( uwwtd_check_decimal($data['uwwNDischargeEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwndischargeestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwNDischargeEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwNDischargeEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwNDischargeEstimated');
            } else {
                $node->field_uwwndischargeestimated['und'][0]['value'] = round($check,2);
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads N-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwPDischargeMeasured'])){
//             if(uwwtd_check_decimal($data['uwwPDischargeMeasured'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwpdischarge['und'][0]['value'] = uwwtd_check_decimal($data['uwwPDischargeMeasured'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwPDischargeMeasured']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwPDischargeMeasured');
            } else {
                $node->field_uwwpdischarge['und'][0]['value'] = round($check,2);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads Ptot information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwPDischargeCalculated'])){
//             if(uwwtd_check_decimal($data['uwwPDischargeCalculated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwpdischargecalculated['und'][0]['value'] = uwwtd_check_decimal($data['uwwPDischargeCalculated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwPDischargeCalculated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwPDischargeCalculated');
            } else {
                $node->field_uwwpdischargecalculated['und'][0]['value'] = round($check,2);
            }              
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads Ptot calculated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwPDischargeEstimated'])){
//             if( uwwtd_check_decimal($data['uwwPDischargeEstimated'], $timestamp, $i, $node) !== false){
//                 $node->field_uwwpdischargeestimated['und'][0]['value'] = uwwtd_check_decimal($data['uwwPDischargeEstimated'], $timestamp, $i, $node);
//             }
            $check = uwwtd_check_decimal($data['uwwPDischargeEstimated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwPDischargeEstimated');
            } else {
                $node->field_uwwpdischargeestimated['und'][0]['value'] = round($check,2);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs discharged loads P-tot estimated information found.', $timestamp, $i, $node);
        
        //decimal
        if(isset($data['uwwWasteWaterTreated'])){
            $check = uwwtd_check_decimal($data['uwwWasteWaterTreated']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['decimal_error'], $timestamp, $i, $node, 'uwwWasteWaterTreated');
            } else {
                $node->field_uwwwastewatertreated['und'][0]['value'] = round($check, 0);
            }               
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs waste water treated  information found.', $timestamp, $i, $node);
        
        //text
        if(isset($data['uwwMethodWasteWaterTreated'])){
            $node->field_uwwmethodwastewatertreated['und'][0]['value'] = uwwtd_check_text($data['uwwMethodWasteWaterTreated']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs method used to determine the volume of waste water treated information found.', $timestamp, $i, $node);
        
        //memo
        if(isset($data['uwwRemarks'])){
            $node->field_uwwremarks['und'][0]['value'] = uwwtd_check_text($data['uwwRemarks']);
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs remarks information found.', $timestamp, $i, $node);

         //DG 15/12/2015 add uwwBeginLife
        //date
        if(isset($data['uwwBeginLife'])){
            $check = uwwtd_check_date($data['uwwBeginLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_uwwbeginlife['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs date begin life found.', $timestamp, $i, $node);
        //DG 15/12/2015 add uwwEndLife       
        //date
        if(isset($data['uwwEndLife'])){
            $check = uwwtd_check_date($data['uwwEndLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_uwwendlife['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No uwwTPs date end life found.', $timestamp, $i, $node);

        // save node
        node_save($node);
        
        if (empty($node->nid)) {
            uwwtd_trace('uwwtp ' . $siteId . ' node_save() error2');
        }  
                
        return $node;
    }
    
    uwwtd_trace('import uwwtp error ' . var_export($data, true));
    return false;
    
}

function uwwtd_import_dcp_batch_process($newFile, $annee, $datas, $repCode, $timestamp, &$context){
    global $language;
    if(isset($datas)){
        if (!isset($context['sandbox']['progress'])) {
            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = $datas;
            $_SESSION['totDcp'] = $context['sandbox']['max'];
        }
        $i = $context['sandbox']['progress'];
        $data = $context['sandbox']['ids'][$i];

        // Check basic elements exist
        // errors
        $e = 0;
        if(!isset($data['dcpName']) || empty($data['dcpName'])){
            $e = 1;
            $emes = t('The discharge point does not have \'title\' information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
        }
        if(!isset($data['dcpCode']) || empty($data['dcpCode'])){
            $e = 1;
            $emes = t('The discharge point does not have \'identification\' information');
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
        }

        // Check if no errors were found
        uwwtd_trace('import discharge_point : ' . $annee.'_dcp_'.uwwtd_check_text($data['dcpCode']));
        if($e == 0){

            // Variables
            $siteId = $annee.'_dcp_'.uwwtd_check_text($data['dcpCode']);

            // Check if id exists
            $exists = uwwtd_check_exist($siteId);
            uwwtd_trace('discharge_point ' . $siteId . ' exists ? "' . $exists . '".');
            // If the element exists
            if(!$exists){
                $node = new stdClass();
                $node->type = 'discharge_point';
                $node->uid = 1;
                $node->translate = 0;
                $node->tnid = 0;
                $node->sticky = 0;
                $node->promote = 0;
                $node->comment = 0;
                $node->language = $language->language;
                $_SESSION['newDcp']++;
                node_save($node);
                
                if (empty($node->nid)) {
                    uwwtd_trace('discharge_point ' . $siteId . ' node_save() error');
                } else {
                    uwwtd_trace('discharge_point ' . $siteId . ' nid : ' . $node->nid);
                }                
            }
            else{
                // load the node
                $node = node_load($exists);
                $node->type = 'discharge_point';
                $_SESSION['upDcp']++;
                
                if (empty($node->nid)) {
                    uwwtd_trace('discharge_point ' . $siteId . ' node_save() error');
                } else {
                    uwwtd_trace('discharge_point ' . $siteId . ' nid : ' . $node->nid);
                }                   
            }

            // site mapping
            $node->field_siteid['und'][0]['value'] = $siteId;
            $node->field_valid['und'][0]['value'] = 0;
            $node->field_anneedata['und'][0]['value'] = $annee;

            // Check latitude :
            if(!isset($data['dcpLatitude']) || empty($data['dcpLatitude'])){
                $emes = t('The discharge point does not have latitude information');
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            } else {
                $checkLatitude = uwwtd_check_coordinate($data['dcpLatitude'], 'latitude', $message);
                if ($message != '') {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                }
                if($checkLatitude === false){
                    $checkLatitude = '0';
                    $emes = t('The latitude for this discharge point is invalid');
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
                }
            }

            // Check longitude :
            if(!isset($data['dcpLongitude']) || empty($data['dcpLongitude'])){
                $emes = t('The discharge point does not have longitude information');
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
            } else {
                $checkLongitude = uwwtd_check_coordinate($data['dcpLongitude'], 'longitude', $message);
                if ($message != '') {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                }
                if($checkLongitude === false){
                    $checkLongitude = '0';
                    $emes = t('The longitude for this discharge point is invalid');
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i, $node);
                }
            }

            $geoData = uwwtd_coord_transform($checkLatitude, $checkLongitude);
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
                        'fid' => trim($newFile->fid),
                        'filename' => trim($newFile->filename .".xml"),
                        'uri' => trim($newFile->uri),
                        'status' => 1,
                        'display' => 1
                    )
                )
            );
            unset($node->field_dcprcaliste['und']);
            unset($node->field_dcpuwwliste['und']);

            // data mapping
                $node->field_repcode['und'][0]['value'] =  $repCode;
//                 $repcode_f = str_replace('_', '-',  $repCode);
//                 $boom = explode('-', $repcode_f);
//                 $boom = explode(' ', $boom[0]);
//                 $rep = $boom[0];
                $rep = uwwtd_get_countrycode_from_repcode($repCode);
                if($rep == 'LT'){
                    $node->field_regionnuts['und'][0]['value'] = 'LT00';
                }

                if((strpos($rep, 'PL') !== false) || (strpos($rep, 'pl') !== false)){
                    $node->field_regionnuts['und'][0]['value'] = 'PL112';
                }
            //text
            if(isset($datas['repCode'])) $node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['repCode']);
        
            //text
            $node->field_inspireidlocalid['und'][0]['value'] = uwwtd_check_text($data['dcpCode']);

            //text
            $node->title = uwwtd_check_text($data['dcpName']);

            //text
            if(isset($data['dcpState'])){
                $node->field_status['und'][0]['value'] = uwwtd_check_text($data['dcpState']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point status information found.', $timestamp, $i, $node);

            //DG 15/12/2015 add uwwCode
            //text
            if(isset($data['uwwID']) || isset($data['uwwCode'])){
                if(isset($data['uwwID'])){
                    $node->field_utilityfacilityreference['und'][0]['value'] = uwwtd_check_text($data['uwwID']);
                }elseif(isset($data['uwwCode'])){
                    $node->field_utilityfacilityreference['und'][0]['value'] = uwwtd_check_text($data['uwwCode']);
                }
        }else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point waste water treatment plant connexion information found.', $timestamp, $i, $node);

            //text
            if(isset($data['dcpNUTS'])){
                $node->field_regionnuts['und'][0]['value'] = uwwtd_check_text($data['dcpNUTS']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point nut code information found.', $timestamp, $i, $node);

            //decimal                                 
            $node->field_dcplatitude['und'][0]['value'] = $checkLatitude;
            
            //decimal
            $node->field_dcplongitude['und'][0]['value'] = $checkLongitude;

            //text
            if(isset($data['dcpWaterBodyType'])){
                $node->field_dcpwaterbodytype['und'][0]['value'] = uwwtd_check_text($data['dcpWaterBodyType']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point water body type information found.', $timestamp, $i, $node);
            
            //text
            if(isset($data['dcpIrrigation'])){
                $node->field_dcpirrigation['und'][0]['value'] = uwwtd_check_text($data['dcpIrrigation']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No information about waste water reuse for irrigation found.', $timestamp, $i, $node);
            
            //text
            if(isset($data['dcpTypeOfReceivingArea'])){
                $node->field_rcatype['und'][0]['value'] = uwwtd_check_text($data['dcpTypeOfReceivingArea']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point type of receiving area information found.', $timestamp, $i, $node);

            //DG 15/12/2015 add rcaCode
            //text
            if(isset($data['rcaID']) || isset($data['rcaCode'])){
                if(isset($data['rcaID'])){
                     $node->field_rcaid['und'][0]['value'] = uwwtd_check_text($data['rcaID']);
                }elseif(isset($data['rcaCode'])){
                     $node->field_rcaid['und'][0]['value'] = uwwtd_check_text($data['rcaCode']);
                }
            } elseif (empty($data['dcpTypeOfReceivingArea']) || ($data['dcpTypeOfReceivingArea'] != 'NA' && $data['dcpTypeOfReceivingArea'] != 'na')) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point receiving area connexion information found.', $timestamp, $i, $node);
            }
            
            //text
            if(isset($data['dcpSurfaceWaters'])){
                $node->field_dcpsurfacewaters['und'][0]['value'] = uwwtd_check_text($data['dcpSurfaceWaters']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point surface waters information found.', $timestamp, $i, $node);

            //text
            if(isset($data['dcpWaterbodyID'])){
                $node->field_dcpwaterbodyid['und'][0]['value'] = uwwtd_check_text($data['dcpWaterbodyID']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point water body connexion information found.', $timestamp, $i, $node);

            //text
            if(isset($data['dcpNotAffect'])){
                $node->field_dcpnotaffect['und'][0]['value'] = uwwtd_check_text($data['dcpNotAffect']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point information  do comprehensive studies indicate that the discharge does not adversely affect the environment found.', $timestamp, $i, $node);

            //text
            if(isset($data['dcpMSProvide'])){
                $node->field_dcpmsprovide['und'][0]['value'] = uwwtd_check_text($data['dcpMSProvide']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point has the Member State provided these studies to the Commission information found.', $timestamp, $i, $node);

            //text
            if(isset($data['dcpCOMAccept'])){
                $node->field_ddcpcomaccept['und'][0]['value'] = uwwtd_check_text($data['dcpCOMAccept']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point Has the Commission formally accepts that the conditions of art. 6 (2) are met information found.', $timestamp, $i, $node);
            
            //text
            if(isset($data['dcpGroundWater'])){
                $node->field_dcpgroundwater['und'][0]['value'] = uwwtd_check_text($data['dcpGroundWater']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point ID of WFD groundwater body type information found.', $timestamp, $i, $node);
            
            //text
            if(isset($data['dcpReceivingWater'])){
                $node->field_dcpreceivingwater['und'][0]['value'] = uwwtd_check_text($data['dcpReceivingWater']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point water body connexion information found.', $timestamp, $i, $node);
            
            //text
            if(isset($data['dcpWFDSubUnit'])){
                $node->field_dcpwfdsubunit['und'][0]['value'] = uwwtd_check_text($data['dcpWFDSubUnit']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point ID of WFD sub-unit information found.', $timestamp, $i, $node);
            
            //text
            if(isset($data['dcpWFDRBD'])){
                $node->field_dcpwfdrbd['und'][0]['value'] = uwwtd_check_text($data['dcpWFDRBD']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point WFD river basin district connexion information found.', $timestamp, $i, $node);

            //memo
            if(isset($data['dcpRemarks'])){
                $node->field_dcpremarks['und'][0]['value'] = uwwtd_check_text($data['dcpRemarks']);}
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point remarks information found.', $timestamp, $i, $node);
            
            //date
            if(isset($data['dcpWaterBodyReferenceDate'])){
                $check = uwwtd_check_date($data['dcpWaterBodyReferenceDate'], $message);
                if ($message != '') {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                }            
                if($check !== false){
                    $node->field_dcpwaterbodyreferencedate['und'][0] = array(
                        'value'=> $check,
                        'timezone'=>'Europe/Berlin',
                        'timezone_bd'=>'Europe/Berlin',
                        'date_type'=>'date'
                    );
                }
            }
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point water body reference date information found.', $timestamp, $i, $node);

            //date
            if(isset($data['dcpGroundWaterReferenceDate'])){
                $check = uwwtd_check_date($data['dcpGroundWaterReferenceDate'], $message);
                if ($message != '') {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                }              
                if($check !== false){
                    $node->field_dcpdcpgroundwaterreference['und'][0] = array(
                        'value'=> $check,
                        'timezone'=>'Europe/Berlin',
                        'timezone_bd'=>'Europe/Berlin',
                        'date_type'=>'date'
                    );
                }
            }
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point ground water reference date information found.', $timestamp, $i, $node);

            //date
            if(isset($data['dcpReceivingWaterReferenceDate'])){
                $check = uwwtd_check_date($data['dcpReceivingWaterReferenceDate'], $message);
                if ($message != '') {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                }             
                if($check !== false){
                    $node->field_dcpreceivingwaterreference['und'][0] = array(
                        'value'=> $check,
                        'timezone'=>'Europe/Berlin',
                        'timezone_bd'=>'Europe/Berlin',
                        'date_type'=>'date'
                    );
                }
            }
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point receiving water reference date information found.', $timestamp, $i, $node);
            
            //date
            if(isset($data['dcpWFDSubUnitReferenceDate'])){
                $check = uwwtd_check_date($data['dcpWFDSubUnitReferenceDate'], $message);
                if ($message != '') {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                }             
                if($check !== false){
                    $node->field_dcp_wfdsubunitrefdate['und'][0] = array(
                        'value'=>$check,
                        'timezone'=>'Europe/Berlin',
                        'timezone_bd'=>'Europe/Berlin',
                        'date_type'=>'date'
                    );
                }
            }
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point WFD sub unit reference date information found.', $timestamp, $i, $node);

            //date
            if(isset($data['dcpWFDRBDReferenceDate'])){
                $check = uwwtd_check_date($data['dcpWFDRBDReferenceDate'], $message);
                if ($message != '') {
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
                }               
                if($check !== false){
                    $node->field_dcpwfdrbdreferencedate['und'][0] = array(
                        'value'=> $check,
                        'timezone'=>'Europe/Berlin',
                        'timezone_bd'=>'Europe/Berlin',
                        'date_type'=>'date'
                    );
                }
            }
            else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point WFD reference date information found.', $timestamp, $i, $node);

             //DG 15/12/2015 add dcpBeginLife
        //date
        if(isset($data['dcpBeginLife'])){
            $check = uwwtd_check_date($data['dcpBeginLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_dcpbeginlife['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point date begin life found.', $timestamp, $i, $node);
        //DG 15/12/2015 add dcpEndLife       
        //date
        if(isset($data['dcpEndLife'])){
            $check = uwwtd_check_date($data['dcpEndLife'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_dcpendlife['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No discharge point date end life found.', $timestamp, $i, $node);
        
            // save node
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('discharge_point ' . $siteId . ' node_save() error2');
            }             
        } else {
            uwwtd_trace('import discharge_point error ' . var_export($data, true));
        }
        uwwtd_flush_errors();
        $context['sandbox']['progress']++;
        $context['message'] = '(' .$annee . ') ' . t('Processing discharge point').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
        
        if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    } else {
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No Discharge points found in the dataset ' . $annee, $timestamp);
    }
}

/*DG 15/12/2015 import Contact*/
function uwwtd_import_contact_batch_process($newFile, $annee, $datas, $repCode, $timestamp, &$context){
    if(!empty($datas)){
        if (!isset($context['sandbox']['progress'])) {
        
            //Get all receiving areas from data
            
            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = $datas;
            $_SESSION['totcontact'] = $context['sandbox']['max'];
        }
        $i = $context['sandbox']['progress'];
        $j=0;
        while($j<20 && $context['sandbox']['progress']<$context['sandbox']['max']){
            $k=$i+$j;
            $data = $context['sandbox']['ids'][$k];
            $node = uwwtd_update_contact_node($data, $newFile, $repCode, $annee, $timestamp, $i);
            if($node){
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
            }
            else{
                $context['results'][] = t('Failed to import a contact.');
            }
            $context['sandbox']['progress']++;
            $j++;
        }
        uwwtd_flush_errors();
        if($node){
            $context['message'] = '(' .$annee . ') ' . t('Processing contact').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        }
        else{
            $context['message'] = '(' .$annee . ') ' . t('Failed to import a contact.');
        }
        
        if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    }else{
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No contacts found in the dataset ' . $annee, $timestamp, $i);
    }
}

/*DG 15/12/2015 Update Contact*/
function uwwtd_update_contact_node($data, $newFile, $repCode, $annee, $timestamp, $i){
    global $language;
    // Check basic elements exist
    // errors
    $e = 0;
    if(!isset($data['conName']) || empty($data['conName'])){
        $e = 1;
        $emes = t('The contact does not have \'title\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }

    // Check if no errors were found
    if (!empty($data['conName'])) {
        uwwtd_trace('import contact : ' . $annee.'_contact_'.uwwtd_check_text($data['conName']));
    }
    if($e == 0){

        // Variables
        $siteId = $annee.'_contact_'.uwwtd_check_text($data['conName']);

        // Check if id exists
        $exists = uwwtd_check_exist($siteId);
        uwwtd_trace('contact ' . $siteId . ' exists ? "' . $exists . '".');
        // If the element exists
        if(!$exists){
            $node = new stdClass();
            $node->type = 'contact';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            $_SESSION['newContact']++;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('contact ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('contact ' . $siteId . ' nid : ' . $node->nid);
            }
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'contact';
            $_SESSION['upContact']++;
            
            if (empty($node->nid)) {
                uwwtd_trace('contact ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('contact ' . $siteId . ' nid : ' . $node->nid);
            }             
        }
        //text
        $node->title = uwwtd_check_text($data['conName']);

        // site mapping
        $node->field_siteid['und'][0]['value'] = $siteId;
        $node->field_valid['und'][0]['value'] = 0;
        $node->field_anneedata['und'][0]['value'] = $annee;
        $node->field_sourcefile = array(
            'und' => array(
                0 => array(
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );
        //unset liste for case the rca is updated, we recalcultaed it later
        //unset($node->field_rcadcpliste['und']);

        // data mapping
        $node->field_repcode['und'][0]['value'] = $repCode;
        
        //text
        if(isset($datas['repCode'])) $node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['repCode']);
        //text
        if(isset($datas['conName'])) $node->field_conname['und'][0]['value'] = uwwtd_check_text($datas['conName']);
        //text
        if(isset($datas['conInstitution'])) $node->field_coninstitution['und'][0]['value'] = uwwtd_check_text($datas['conInstitution']);
        //text
        if(isset($datas['conStreet'])) $node->field_constreet['und'][0]['value'] = uwwtd_check_text($datas['conStreet']);
        //text
        if(isset($datas['conZIP'])) $node->field_conzip['und'][0]['value'] = uwwtd_check_text($datas['conZIP']);
        //text
        if(isset($datas['conCity'])) $node->field_concity['und'][0]['value'] = uwwtd_check_text($datas['conCity']);
        //text
        if(isset($datas['conPhone'])) $node->field_conphone['und'][0]['value'] = uwwtd_check_text($datas['conPhone']);
        //text
        if(isset($datas['conFax'])) $node->field_confax['und'][0]['value'] = uwwtd_check_text($datas['conFax']);
        //text
        if(isset($datas['conEmail'])) $node->field_conemail['und'][0]['value'] = uwwtd_check_text($datas['conEmail']);
        //text
        if(isset($datas['conRemarks'])) $node->field_conremarks['und'][0]['value'] = uwwtd_check_text($datas['conRemarks']);
        
        
        // save node
        node_save($node);

        if (empty($node->nid)) {
            uwwtd_trace('contact ' . $siteId . ' node_save() error2');
        }        
       
        return $node;
    }
    
    uwwtd_trace('import contact error ' . var_export($data, true));
    return false;
    
}

/*DG 15/12/2015 import Industry*/
function uwwtd_import_industry_batch_process($newFile, $annee, $datas, $repCode, $timestamp, &$context){
    if(!empty($datas)){
        if (!isset($context['sandbox']['progress'])) {
        
            //Get all receiving areas from data
            
            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = $datas;
            $_SESSION['totcontact'] = $context['sandbox']['max'];
        }
        $i = $context['sandbox']['progress'];
        $j=0;
        while($j<20 && $context['sandbox']['progress']<$context['sandbox']['max']){
            $k=$i+$j;
            $data = $context['sandbox']['ids'][$k];
            $node = uwwtd_update_contact_node($data, $newFile, $repCode, $annee, $timestamp, $i);
            if($node){
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
            }
            else{
                $context['results'][] = t('Failed to import a industry.');
            }
            $context['sandbox']['progress']++;
            $j++;
        }
        uwwtd_flush_errors();
        if($node){
            $context['message'] = '(' .$annee . ') ' . t('Processing industry').': '.check_plain($node->title).' '.t('with a drupal id of: ').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        }
        else{
            $context['message'] = '(' .$annee . ') ' . t('Failed to import a industry.');
        }
        
        if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    }else{
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No industries found in the dataset ' . $annee, $timestamp, $i);
    }
}

/*DG 15/12/2015 Update Industry*/
function uwwtd_update_industry_node($data, $newFile, $repCode, $annee, $timestamp, $i){
    global $language;
    // Check basic elements exist
    // errors
    $e = 0;
    if(!isset($data['indCodePlant']) || empty($data['indCodePlant'])){
        $e = 1;
        $emes = t('The industry does not have \'title\' information');
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $i);
    }

    // Check if no errors were found
    uwwtd_trace('import industry : ' . $annee.'_industry_'.uwwtd_check_text($data['indCodePlant']));
    if($e == 0){

        // Variables
        $siteId = $annee.'_industry_'.uwwtd_check_text($data['indCodePlant']);

        // Check if id exists
        $exists = uwwtd_check_exist($siteId);
        uwwtd_trace('industry ' . $siteId . ' exists ? "' . $exists . '".');
        // If the element exists
        if(!$exists){
            $node = new stdClass();
            $node->type = 'industry';
            $node->uid = 1;
            $node->translate = 0;
            $node->tnid = 0;
            $node->sticky = 0;
            $node->promote = 0;
            $node->comment = 0;
            $node->language = $language->language;
            $_SESSION['newContact']++;
            node_save($node);
            
            if (empty($node->nid)) {
                uwwtd_trace('industry ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('industry ' . $siteId . ' nid : ' . $node->nid);
            }
        }
        else{
            // load the node
            $node = node_load($exists);
            $node->type = 'industry';
            $_SESSION['upIndustry']++;
            
            if (empty($node->nid)) {
                uwwtd_trace('industry ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('industry ' . $siteId . ' nid : ' . $node->nid);
            }             
        }
        //text
        $node->title = uwwtd_check_text($data['indCodePlant']);

        // site mapping
        $node->field_siteid['und'][0]['value'] = $siteId;
        $node->field_valid['und'][0]['value'] = 0;
        $node->field_anneedata['und'][0]['value'] = $annee;
        $node->field_sourcefile = array(
            'und' => array(
                0 => array(
                    'fid' => trim($newFile->fid),
                    'filename' => trim($newFile->filename .".xml"),
                    'uri' => trim($newFile->uri),
                    'status' => 1,
                    'display' => 1
                )
            )
        );
        //unset liste for case the rca is updated, we recalcultaed it later
        //unset($node->field_rcadcpliste['und']);

        // data mapping
        $node->field_repcode['und'][0]['value'] = $repCode;
        
        //text
        if(isset($datas['repCode'])) $node->field_repcode['und'][0]['value'] = uwwtd_check_text($datas['repCode']);
       //numeric
        if(isset($data['indState'])){
            $check = uwwtd_check_numeric($data['indState']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
            } else {
                $node->field_indstate['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No industries status informations found.', $timestamp, $i, $node);
        //text
        if(isset($datas['indCodePlant'])) $node->field_indcodeplant['und'][0]['value'] = uwwtd_check_text($datas['indCodePlant']);
        //text
        if(isset($datas['indNamePlant'])) $node->field_indnameplant['und'][0]['value'] = uwwtd_check_text($datas['indNamePlant']);
        //text
        if(isset($datas['indBranch'])) $node->field_indbranch['und'][0]['value'] = uwwtd_check_text($datas['indBranch']);
        //numeric
        if(isset($data['indOrganicLoad'])){
            $check = uwwtd_check_numeric($data['indOrganicLoad']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['numeric_error'], $timestamp, $i, $node);
            } else {
                $node->field_indorganicload['und'][0]['value'] = $check;
            }             
        }
         //bool
        if(isset($data['indConditions'])){
            $check = uwwtd_check_bool($data['indConditions']);
            if (false === $check) {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $GLOBALS['uwwtd']['message']['boolean_error'], $timestamp, 0 , $node);
            } else {
                $node->field_indconditions['und'][0]['value'] = $check;
            }
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No industries conditions information found.', $timestamp, $i, $node);
        //date
        if(isset($data['indDateCompliance'])){
            $check = uwwtd_check_date($data['indDateCompliance'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $i, $node);
            }
            if($check !== false){
                $node->field_inddatecompliance['und'][0]['value'] = $check;
            }             
        }
        else uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['notification'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], 'No industries date compliance found.', $timestamp, $i, $node);
        
        
        
        // save node
        node_save($node);

        if (empty($node->nid)) {
            uwwtd_trace('industry ' . $siteId . ' node_save() error2');
        }        
       
        return $node;
    }
    
    uwwtd_trace('import industry error ' . var_export($data, true));
    return false;
    
}

//Modif nd@oieau.fr, le but est de faire plusieurs traitement pdt la même boucle
function uwwtd_link_dcp_rca_uww_batch_process($newFile, $annee, $timestamp, &$context){
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
    $j=0;
    while($j<10 && $context['sandbox']['progress']<$context['sandbox']['max']){
        $k=$i+$j;
        if($node = node_load($context['sandbox']['ids'][$k]['nid'])){
            // Check and add dcp <-> rca
            if(isset($node->field_rcaid['und'][0]['value'])){
                $rcaExists = uwwtd_check_exist($annee.'_rca_'.uwwtd_check_text($node->field_rcaid['und'][0]['value']));
            
                if($rcaExists != false){
                    $node->field_dcprcaliste['und'][]['nid'] = $rcaExists;
                    $rca = node_load($rcaExists);
                    $rca->field_rcadcpliste['und'][]['nid'] = $node->nid;
                    node_save($rca);
                }else{
                    $emes = t('Cannot connect the discharge point').' \''.$node->field_inspireidlocalid['und'][0]['value'].'\' '.t('with drupal id').' \''.$node->nid.'\' '.t('to the receiving area').' \''.$node->field_rcaid['und'][0]['value'].'\' '.t('because the receiving area does not exist.');
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $k);
                }
            }

            // Check and add dcp <-> uww
            if(isset($node->field_utilityfacilityreference['und'][0]['value'])){
                $uwwExists = uwwtd_check_exist($annee.'_uww_'.uwwtd_check_text($node->field_utilityfacilityreference['und'][0]['value']));
                if($uwwExists != false){
                    $node->field_dcpuwwliste['und'][]['nid'] = $uwwExists;
                    $uww = node_load($uwwExists);

                    // Remove this discharge point from the list of discharge point without treatment plants :
                    $dpsWithoutTp = variable_get(DPS_WITHOUT_TP, array());
                    _remove_array_element_by_value($node->field_inspireidlocalid['und'][0]['value'], $dpsWithoutTp);
                    variable_set(DPS_WITHOUT_TP, $dpsWithoutTp);

                    $uww->field_uwwdcpliste['und'][]['nid'] = $node->nid;
                    node_save($uww);

                    // Remove this treatment plant from the list of treatment plants without discharge point :
                    $tpsWithoutDp = variable_get(TPS_WITHOUT_DP, array());
                    _remove_array_element_by_value($node->field_utilityfacilityreference['und'][0]['value'], $tpsWithoutDp);
                    variable_set(TPS_WITHOUT_DP, $tpsWithoutDp);
                }
                else{
                    $emes = t('Cannot connect the discharge point').' \''.$node->field_inspireidlocalid['und'][0]['value'].'\' '.t('with drupal id').' \''.$node->nid.'\' '.t('to the waste water treatment plant').' \''.$node->field_utilityfacilityreference['und'][0]['value'].'\' '.t('because the waste water treatment plant does not exist.');
                    uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $k);
                }
            }

            node_save($node);
            $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
        }
        
        $context['sandbox']['progress']++;
        $j++;
    }
    uwwtd_flush_errors();
    
    $context['message'] = '(' .$annee . ') ' . t('Processing links from discharge point').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
    
    
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_link_uww_agglo_batch_process($newFile, $annee, $datas, $timestamp, &$context){
    global $language;
    if(!empty($datas)){
        if (!isset($context['sandbox']['progress'])) {
            $context['sandbox']['max'] = count($datas);
            $context['sandbox']['progress'] = 0;
            $context['sandbox']['ids'] = $datas;
            $_SESSION['totAggloUww'] = $context['sandbox']['max'];
        }
        
        $i = $context['sandbox']['progress'];
        $j=0;
        while($j<10 && $context['sandbox']['progress']<$context['sandbox']['max']){
            $k=$i+$j;
            $data = $context['sandbox']['ids'][$k];

            // Get data uww id and agg id
            //Here we need to build the site id the same way as in agg or uwwtp creation node
            //TODO : a function 
            $uwwId = $data['aucUwwCode'];  //$annee.'_uww_'.uwwtd_check_text($data['uwwCode']);
            $aggId = $data['aucAggCode'];  //$annee.'_agg_'.uwwtd_check_text($data['aggCode']));
            //todo test (ro 1)
            $siteUwwId = $annee.'_uww_'.uwwtd_check_text($data['aucUwwCode']);
            $siteAggId = $annee.'_agg_'.uwwtd_check_text($data['aucAggCode']);
            

            // Check if both exist
            $uwwExists = uwwtd_check_exist($siteUwwId);
            $aggExists = uwwtd_check_exist($siteAggId);
            
            
            uwwtd_trace('uwwtp ' . $siteUwwId . ' exists ? "' . $uwwExists . '".');
            uwwtd_trace('agglo ' . $siteAggId . ' exists ? "' . $aggExists . '".');
            

            // Nids :
            
            $aggNid = $aggExists;
            $tpNid = $uwwExists;

            // If there are agglomeration and treatment plants nids :
            if (!empty($aggNid) && !empty($tpNid)) {
                $aggTpLinkFull = uwwtd_link_uww_agglo_node($annee, $aggNid, $tpNid, $data, $datas['repCode']);
                //=======warning
                $context['results'][] = $aggTpLinkFull->nid . ' : ' . check_plain($aggTpLinkFull->title);
            } else {
                uwwtd_trace('agglo_uww error, agglo or uwwtp doesn\' exits $siteUwwId = ' . $siteUwwId . ' ("'.$uwwExists.'"), $siteAggId = ' . $siteAggId . '("'.$aggExists.'")');
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'], 'No link between agglomeration ' . $siteAggId . ' and waste water treatment plant ' . $siteUwwId . '', $timestamp, $k);
            }

            $context['sandbox']['progress']++;
            $j++;
        }
        
        $context['message'] = '(' .$annee . ') ' . t('Processing link between agglomeration').' : '.'\''.$aggId.'\''.' '.t('and waste water treatment plant').' '.'\''.$uwwId.'\'' . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        
        uwwtd_flush_errors();
        if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
            $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
    } else {
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'], 'No link between agglomerations and waste water treatment plant because no found agglomerations in the dataset', $timestamp);
    }
}

function uwwtd_missing_link_uww_agglo_batch_process($newFile, $annee, $timestamp, &$context){
        if(!isset($context['sandbox']['progress'])){
            $ids = array();
                $result = db_query("
                    SELECT n.nid AS nid, n.title AS title, a.field_uwwtdaggid_value AS aggid
                    FROM  {node} n
                        JOIN {field_data_field_siteid} r ON r.entity_id = n.nid
                        JOIN {field_data_field_uwwtdaggid} a ON a.entity_id = n.nid
                        LEFT JOIN {field_data_field_uwwaggliste} agg ON agg.entity_id = n.nid 
                    WHERE  n.type = 'uwwtp' 
                        AND a.field_uwwtdaggid_value IS NOT NULL
                        AND agg.field_uwwaggliste_nid IS NULL
                        AND r.field_siteid_value LIKE '".$annee."%'
                 ");
                 while($row = $result->fetchAssoc()) {
                     $ids[] = $row;
                }
                $context['sandbox']['max'] = count($ids);
                $context['sandbox']['progress'] = 0;
                $context['sandbox']['ids'] = $ids;
    }
    
    
    if($context['sandbox']['max'] > 0){
        $i = $context['sandbox']['progress'];
        $j=0;
        while($j<5 && $context['sandbox']['progress']<$context['sandbox']['max']){
            $k=$i+$j;
            $item = $context['sandbox']['ids'][$k];
            $siteAggId = $annee.'_agg_'.uwwtd_check_text($item['aggid']);
             if($aggNid = uwwtd_check_exist($siteAggId) && $item['nid'] !=''){
                 uwwtd_link_uww_agglo_node($annee, $aggNid, $item['nid']);
                $context['results'][] = $item['nid'] . ' : ' . check_plain($item['title']);
             }
             $context['sandbox']['progress']++;
             $j++;
        }
        uwwtd_flush_errors();
        $context['message'] = '(' .$annee . ') ' . t('Processing missing links between agglomeration and inactive UWWTP').' : '.'\''.$item['title'].'\''.' '.t('with drupal id').' '.'\''.$item['nid'].'\'' . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
    }
     if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_link_uww_agglo_node($annee, $aggNid, $tpNid, $data = [], $repCode = null){
         // Add agglomeration nid in treament plant :
        $tpFull = node_load($tpNid);
        if ($tpFull !== FALSE) {
            if (isset($tpFull->field_uwwaggliste)) {
                $isFound = FALSE;
                if (!empty($tpFull->field_uwwaggliste[LANGUAGE_NONE])) {
                    // Check if this aggNid already exists in this treament plant :
                    foreach ($tpFull->field_uwwaggliste[LANGUAGE_NONE] as $nidElement) {
                        if (array_key_exists('nid', $nidElement) && $nidElement['nid'] == $aggNid) {
                            $isFound = TRUE;
                            break;
                        }
                    }
                }
                if (!$isFound) {
                    // Add new nid :
                    $tpFull->field_uwwaggliste[LANGUAGE_NONE][] = array('nid' => $aggNid);
                    node_save($tpFull);
                }
            }
        }

        // Add treament plant nid in agglomeration :
        $aggFull = node_load($aggNid);
        if ($aggFull !== FALSE) {
            if (isset($aggFull->field_agguwwliste)) {
                $isFound = FALSE;
                if (!empty($aggFull->field_agguwwliste[LANGUAGE_NONE])) {
                    // Check if this aggNid already exists in this treament plant :
                    foreach ($aggFull->field_agguwwliste[LANGUAGE_NONE] as $nidElement) {
                        if (array_key_exists('nid', $nidElement) && $nidElement['nid'] == $tpNid) {
                            $isFound = TRUE;
                            break;
                        }
                    }
                }
                if (!$isFound) {
                    // Add new nid :
                    $aggFull->field_agguwwliste[LANGUAGE_NONE][] = array('nid' => $tpNid);
                    node_save($aggFull);
                }
            }
            if (isset($aggFull->field_linked_treatment_plants)) {
                $isFound = FALSE;
                if (!empty($aggFull->field_linked_treatment_plants[LANGUAGE_NONE])) {
                    // Check if this aggNid already exists in this treament plant :
                    foreach ($aggFull->field_linked_treatment_plants[LANGUAGE_NONE] as $nidElement) {
                        if (array_key_exists('nid', $nidElement) && $nidElement['nid'] == $tpNid) {
                            $isFound = TRUE;
                            break;
                        }
                    }
                }
                if (!$isFound) {
                    // Add new nid :
                    $aggFull->field_linked_treatment_plants[LANGUAGE_NONE][] = array('nid' => $tpNid);
                    node_save($aggFull);
                }
            }
        }


        $siteId = $aggFull->field_siteid['und'][0]['value'] . '_' . $tpFull->field_siteid['und'][0]['value'];
        $agguwwExists = uwwtd_check_exist($siteId);
        uwwtd_trace('import agglo_uww : ' . $siteId);
        uwwtd_trace('uwwtp ' . $siteId . ' exists ? "' . $agguwwExists . '".');
        
        $aggTpLinkNid = $agguwwExists;


        // Create/update link :
        $aggTpLinkFull = node_load($aggTpLinkNid);
        if ($aggTpLinkFull === FALSE) {
            // Create :
            $aggTpLinkFull = new stdClass();
            $aggTpLinkFull->type = 'agglo_uww';
            $aggTpLinkFull->uid = 1;
            $aggTpLinkFull->translate = 0;
            $aggTpLinkFull->tnid = 0;
            $aggTpLinkFull->sticky = 0;
            $aggTpLinkFull->promote = 0;
            $aggTpLinkFull->comment = 0;
            $aggTpLinkFull->language = $language->language;
            $_SESSION['newAggloUww']++;
            node_save($aggTpLinkFull);

            if (empty($aggTpLinkFull->nid)) {
                uwwtd_trace('agglo_uww ' . $siteId . ' node_save() error');
            } else {
                uwwtd_trace('agglo_uww ' . $siteId . ' nid : ' . $aggTpLinkFull->nid);
            }
        } else {
            // Update :
            $_SESSION['upAggloUww']++;
            uwwtd_trace('agglo_uww ' . $siteId . ' nid : ' . $aggTpLinkFull->nid);

            // Delete current agglomeration and treament plant :
            unset($aggTpLinkFull->field_agglo_uww_agglo['und']);
            unset($aggTpLinkFull->field_agglo_uww_uww['und']);
        }

        // Create linking node with all the data needed for compliance calculation later
        $aggTpLinkFull->title = $annee.' data '.$aggFull->title.' - '.$tpFull->title;
        $aggTpLinkFull->field_siteid['und'][0]['value'] = $siteId;
        $aggTpLinkFull->field_agglo_uww_agglo['und'][0]['nid'] = $aggNid;
        $aggTpLinkFull->field_agglo_uww_uww['und'][0]['nid'] = $tpNid;

        //text
        if ($repCode) $aggTpLinkFull->field_repcode['und'][0]['value'] = uwwtd_check_text($repCode);
        else $aggTpLinkFull->field_repcode['und'][0]['value'] = $aggFull->field_repcode['und'][0]['value'];
        //decimal
        if (isset($data['aucPercEnteringUWWTP'])) {
            $check = uwwtd_check_pourc($data['aucPercEnteringUWWTP'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $k, $aggTpLinkFull, 'aucPercEnteringUWWTP');
            }
        } else {
            $check = 0;
        }

        $aggTpLinkFull->field_agglo_uww_perc_ent_uw['und'][0]['value'] = $check;

        //text
        $aggTpLinkFull->field_agglo_uww_mperc_ent_uw['und'][0]['value'] = (isset($data['aucMethodPercEnteringUWWTP']) ? uwwtd_check_text($data['aucMethodPercEnteringUWWTP']) : 0);
        $aggTpLinkFull->field_anneedata['und'][0]['value'] = $annee;
        
        //FIX ME
        //Rate of generated load of agglomeration transported to this UWWTP by trucks (%).
        //min : 0 max 100% decimal
        //aucPercC2T
        if (isset($data['aucPercC2T'])) {
            $check = uwwtd_check_pourc($data['aucPercC2T'], $message);
            if ($message != '') {
                uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], $message, $timestamp, $k, $aggTpLinkFull, 'aucPercC2T');
            }
        } else {
            $check = 0;
        }
        $aggTpLinkFull->field_aucpercc2t['und'][0]['value'] = $check;
        
        
        node_save($aggTpLinkFull);

        if (empty($aggTpLinkFull->nid)) {
            uwwtd_trace('agglo_uww ' . $siteId . ' node_save() error2');
        } else {
            uwwtd_trace('agglo_uww ' . $siteId . ' nid : ' . $aggTpLinkFull->nid);
        }

        

        // Remove this agglomeration from the list of agglomeration without treatment plants :
        $aggsWithoutTp = variable_get(AGGS_WITHOUT_TP, array());
        _remove_array_element_by_value($aggId, $aggsWithoutTp);
        variable_set(AGGS_WITHOUT_TP, $aggsWithoutTp);

        // Remove this treatment plant from the list of treatment plants without agglomeration :
        $tpsWithoutAgg = variable_get(TPS_WITHOUT_AGG, array());
        _remove_array_element_by_value($uwwId, $tpsWithoutAgg);
        variable_set(TPS_WITHOUT_AGG, $tpsWithoutAgg);
    return $aggTpLinkFull;
}


/**
 * Remove an array element by value.
 */
function _remove_array_element_by_value($value, &$array) {
    if (($key = array_search($value, $array)) !== FALSE) {
        unset($array[$key]);
    }
}


function uwwtd_link_bc_agglo_batch_process($newFile, $annee, $datas, $timestamp, &$context){
//     uwwtd_trace(__FUNCTION__);     
    if(!empty($datas)){
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
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'], 'No link between agglomerations and big cities because no found <bigID> in the dataset ' . $annee, $timestamp);
    
        }
        else{
            $i = $context['sandbox']['progress'];
            $j=0;
            while($j<10 && $context['sandbox']['progress']<$context['sandbox']['max']){
                $k=$i+$j;
                $node = node_load($context['sandbox']['ids'][$k]['nid']);
//                 uwwtd_trace($node->nid);
                // Check bcId exists
                if(isset($node->field_aggbigid['und'][0]['value'])){
//                     uwwtd_trace($node->field_aggbigid['und'][0]['value']);
//                     uwwtd_trace($annee.'_big_'.uwwtd_check_text($node->field_aggbigid['und'][0]['value']));
                    $exists = uwwtd_check_exist($annee.'_big_'.uwwtd_check_text($node->field_aggbigid['und'][0]['value']));
//                     uwwtd_trace($exists);
                    if($exists != false){
                        if (!isset($node->field_aggbcliste['und'])) {
                          $node->field_aggbcliste['und'][]['nid'] = $exists;
                        } else {
                          $find = false;
                          foreach($node->field_aggbcliste['und'] as $field) {
                             if ($field['nid'] == $exists) {
                               $find = true; 
                               break;
                             }
                          }
                          if (!$find) {
                            $node->field_aggbcliste['und'][]['nid'] = $exists;
                          }
                        }

                        $bc = node_load($exists);

                        if (!isset($bc->field_bcaggliste['und'])) {
                          $bc->field_bcaggliste['und'][]['nid'] = $node->nid;
                        } else {
                          $find = false;
                          foreach($bc->field_bcaggliste['und'] as $field) {
                             if ($field['nid'] == $node->nid) {
                               $find = true; 
                               break;
                             }
                          }
                          if (!$find) {
                            $bc->field_bcaggliste['und'][]['nid'] = $node->nid;
                          }
                        }

                        node_save($bc);
                    }
                    else{
                        $emes = t('Cannot connect the big cities').' \''.$node->field_inspireidlocalid['und'][0]['value'].'\' '.t('with drupal id').' \''.$node->nid.'\' '.t('to the agglomerations').' \''.$node->field_rcaid['und'][0]['value'].'\' '.t('because the big cities does not exist.');
                        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data input'], $emes, $timestamp, $k);
                    }
                }
                node_save($node);
            
                $context['sandbox']['progress']++;
                $j++;
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
            }
            uwwtd_flush_errors();
            $context['message'] = '(' .$annee . ') ' . t('Processing links from big cities').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid) . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
                
            if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
                $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
            } else {
               $context['finished'] = 1;
            }
        }
    }else{
        uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'], 'No link between agglomerations and big cities because no found big cities in the dataset ' . $annee, $timestamp);
    }
}

function uwwtd_bc_position_batch_process($newFile, $annee, $timestamp, &$context){
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
        $j=0;
        while($j<10 && $context['sandbox']['progress'] < $context['sandbox']['max']){
            $k=$i+$j;
            //dsm($k);
            //dsm($context['sandbox']['ids']);
            $data = $context['sandbox']['ids'][$k];
            if($node = node_load($data['nid'])){

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
                    if($bigAgg = node_load($biggestId)){
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
                }

                node_save($node);
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
            }
            $context['sandbox']['progress']++;
            $j++;
            
        }
        $context['message'] = '(' .$annee . ') ' . t('Processing position of big city').' : '.'\''.$node->title.'\''.' '.t('with drupal id').' '.'\''.$node->nid.'\'' . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
        
    }
    uwwtd_flush_errors();
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

//nd@oieau.fr : 08/10/2018 : this function is too long with large dataset ==> need to be optimized
function uwwtd_mega_links_batch_process($newFile, $annee, $timestamp, &$context){
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
    $j=0;
    while($j<5 && $context['sandbox']['progress'] < $context['sandbox']['max']){
        $k=$i+$j;
        $data = $context['sandbox']['ids'][$k];
        $dcpnid = $data['nid'];
        $dcp = node_load($dcpnid);
        $flag_dcp = false;
        $rcas = array();
        // creat list of rcas
        //todo normal don't exist ?
        if (isset($dcp->field_dcprcaliste['und'])) {
            foreach($dcp->field_dcprcaliste['und'] as $item){
                $rcas[] = $item['nid'];
            }
        }

        // need to save at each step
        // rcas
        
        foreach($rcas as $rcanid){
            // load rca
            $rca = node_load($rcanid);
            $flag_rca = false;
            // dcp <-> rca
            if(!uwwtd_in_array_r($rcanid, $dcp->field_linked_receiving_areas)){
                $dcp->field_linked_receiving_areas['und'][]['nid'] = $rcanid;
                $flag_dcp = true;
            }
            if(!uwwtd_in_array_r($dcpnid, $rca->field_linked_discharge_points)){
                $rca->field_linked_discharge_points['und'][]['nid'] = $dcpnid;
                $flag_rca = true;
            }

            // uwwtps
            if(isset($dcp->field_dcpuwwliste['und'])){
                foreach($dcp->field_dcpuwwliste['und'] as $uwwitem){
                    $uwwnid = $uwwitem['nid'];
                    if($uww = node_load($uwwnid)){
                    $flag_uww = false;
                    // rca <-> uwwtp
                    if(!uwwtd_in_array_r($uwwnid, $rca->field_linked_treatment_plants)){
                        $rca->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
                        $flag_rca = true;
                    }
                    if(!uwwtd_in_array_r($rcanid, $uww->field_linked_receiving_areas)){
                        $uww->field_linked_receiving_areas['und'][]['nid'] = $rcanid;
                        $flag_uww = true;
                    }
                    // dcp <-> uwwtp
                    if(!uwwtd_in_array_r($uwwnid, $dcp->field_linked_treatment_plants)){
                        $dcp->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
                        $flag_dcp = true;
                    }
                    if(!uwwtd_in_array_r($dcpnid, $uww->field_linked_discharge_points)){
                        $uww->field_linked_discharge_points['und'][]['nid'] = $dcpnid;
                        $flag_uww = true;
                    }

                    // agglomerations
                    if(isset($uww->field_uwwaggliste['und'])){
                        foreach($uww->field_uwwaggliste['und'] as $aggitem){
                            $aggnid = $aggitem['nid'];
                            
                            if($agg = node_load($aggnid)){
                            $flag_agg = false;
                            // uww <-> agg
                            if(!uwwtd_in_array_r($aggnid, $uww->field_linked_agglomerations)){
                                $uww->field_linked_agglomerations['und'][]['nid'] = $aggnid;
                                $flag_uww = true;
                            }
                            if(!uwwtd_in_array_r($uwwnid, $agg->field_agguwwliste)){
                                $agg->field_agguwwliste['und'][]['nid'] = $uwwnid;
                                $flag_agg = true;
                            }
                            if(!uwwtd_in_array_r($uwwnid, $agg->field_linked_treatment_plants)){
                                $agg->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
                                $flag_agg = true;
                            }

                            // rca <-> agg
                            if(!uwwtd_in_array_r($aggnid, $rca->field_linked_agglomerations)){
                                $rca->field_linked_agglomerations['und'][]['nid'] = $aggnid;
                                $flag_rca = true;
                            }
                            if(!uwwtd_in_array_r($rcanid, $agg->field_linked_receiving_areas)){
                                $agg->field_linked_receiving_areas['und'][]['nid'] = $rcanid;
                                $flag_agg = true;
                            }

                            // dcp <-> agg
                            if(!uwwtd_in_array_r($aggnid, $dcp->field_linked_agglomerations)){
                                $dcp->field_linked_agglomerations['und'][]['nid'] = $aggnid;
                                $flag_dcp = true;
                            }
                            if(!uwwtd_in_array_r($dcpnid, $agg->field_linked_discharge_points)){
                                $agg->field_linked_discharge_points['und'][]['nid'] = $dcpnid;
                                $flag_agg = true;
                            }
                            if($flag_agg){
                                node_save($agg);}
                            }
                        }
                    }
                    if($flag_uww){node_save($uww);}
                    }
                }
                if($flag_rca){node_save($rca);}
            }
        }
        if($flag_dcp){ node_save($dcp);}
        $flag_dcp = false;
        // Sometimes rcadata is not given or incorrectly inserted so we now go from the treatment plants
        $uwws = array();
        // creat list of uwws
        foreach($dcp->field_dcpuwwliste['und'] as $item){
            $uwws[] = $item['nid'];
        }
        foreach($uwws as $uwwnid){
            if($uww = node_load($uwwnid)){
            $flag_uww = false;
            // dcp <-> uwwtp
            if(!uwwtd_in_array_r($uwwnid, $dcp->field_linked_treatment_plants)){
                $dcp->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
                $flag_dcp = true;
            }
            if(!uwwtd_in_array_r($dcpnid, $uww->field_linked_discharge_points)){
                $uww->field_linked_discharge_points['und'][]['nid'] = $dcpnid;
                $flag_uww = true;
            } 

            // agglomerations
            if(isset($uww->field_uwwaggliste['und'])){
                foreach($uww->field_uwwaggliste['und'] as $aggitem){
                    $aggnid = $aggitem['nid'];
                    if($agg = node_load($aggnid)){
                    $flag_agg = false;
                    // uww <-> agg
                    if(!uwwtd_in_array_r($aggnid, $uww->field_linked_agglomerations)){
                        $uww->field_linked_agglomerations['und'][]['nid'] = $aggnid;
                        $flag_uww = true;
                    }
                        
                    if(!uwwtd_in_array_r($uwwnid, $agg->field_agguwwliste)){
                        $agg->field_agguwwliste['und'][]['nid'] = $uwwnid;
                        $flag_agg = true;
                    }
                    if(!uwwtd_in_array_r($uwwnid, $agg->field_linked_treatment_plants)){
                        $agg->field_linked_treatment_plants['und'][]['nid'] = $uwwnid;
                        $flag_agg = true;
                    } 

                    // dcp <-> agg
                    if(!uwwtd_in_array_r($aggnid, $dcp->field_linked_agglomerations)){
                        $dcp->field_linked_agglomerations['und'][]['nid'] = $aggnid;
                        $flag_dcp = true;
                    }
                    if(!uwwtd_in_array_r($dcpnid, $agg->field_linked_discharge_points)){
                        $agg->field_linked_discharge_points['und'][]['nid'] = $dcpnid;
                        $flag_agg = true;
                    } 
                    if($flag_agg){node_save($agg);}
                  }
                }
                if($flag_uww) {node_save($uww);}
            }
            }
        }
        if($flag_dcp){node_save($dcp);}

        $context['sandbox']['progress']++;
        $j++;
        $context['results'][] = $dcp->nid . ' : ' . check_plain($dcp->title);
    }
    $context['message'] = '(' .$annee . ') ' . t('Processing the links between all elements...') . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';;
    
    uwwtd_flush_errors();
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_rca_extra_batch_process($newFile, $annee, $timestamp, &$context){
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
    $j=0;
    while($j<10 && $context['sandbox']['progress'] < $context['sandbox']['max']){
        $k=$i+$j;
        $data = $context['sandbox']['ids'][$k];
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
        //todo normal if don't exists?
        if (isset($node->field_linked_treatment_plants['und'])) {
            foreach($node->field_linked_treatment_plants['und'] as $uwwitem){
                // load each uww
                $uww = node_load($uwwitem['nid']);
                //dsm($uww);
    
                $nbStations++;
                $designCap = $designCap + (isset($uww->field_physicalcapacityactivity['und'][0]['value']) ? $uww->field_physicalcapacityactivity['und'][0]['value']: 0);
                $pEnt = $pEnt + (isset($uww->field_uwwpincoming['und'][0]['value']) ? $uww->field_uwwpincoming['und'][0]['value']: 0);
                $pDis = $pDis + (isset($uww->field_uwwpdischarge['und'][0]['value']) ? $uww->field_uwwpdischarge['und'][0]['value']: 0);
                $nEnt = $nEnt + (isset($uww->field_uwwnincoming['und'][0]['value']) ? $uww->field_uwwnincoming['und'][0]['value']: 0);
                $nDis = $nDis + (isset($uww->field_uwwndischarge['und'][0]['value']) ? $uww->field_uwwndischarge['und'][0]['value']: 0);
            }
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
        $j++;
        $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
    }
    $context['message'] = '(' .$annee . ') ' . t('Processing Receiving area extra data...') . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';;
    
    uwwtd_flush_errors();
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_finished($success, $results, $operations){
    
    if ($success) {
        drupal_set_message(t('Receiving areas').': '.$_SESSION['newRca'].' '.t('new elements added').', '.$_SESSION['upRca'].' '.t('elements updated').' ( '.$_SESSION['totRca'].' '.t('total elements').')');
        drupal_set_message(t('Big cities').': '.$_SESSION['newBc'].' '.t('new elements added').', '.$_SESSION['upBc'].' '.t('elements updated').' ( '.$_SESSION['totBc'].' '.t('total elements').')');
        drupal_set_message(t('Agglomerations').': '.$_SESSION['newAgg'].' '.t('new elements added').', '.$_SESSION['upAgg'].' '.t('elements updated').' ( '.$_SESSION['totAgg'].' '.t('total elements').')');
        drupal_set_message(t('Treatment plants').': '.$_SESSION['newUww'].' '.t('new elements added').', '.$_SESSION['upUww'].' '.t('elements updated').' ( '.$_SESSION['totUww'].' '.t('total elements').')');
        drupal_set_message(t('Discharge points').': '.$_SESSION['newDcp'].' '.t('new elements added').', '.$_SESSION['upDcp'].' '.t('elements updated').' ( '.$_SESSION['totDcp'].' '.t('total elements').')');
        drupal_set_message(t('Links created between \'discharge points\', \'waste water treatment plants\' and \'receiving areas\'.'));
        drupal_set_message(t('Links created between \'agglomerations\' and \'waste water treatment plants\'') . ' : '.$_SESSION['newAggloUww'].' '.t('new elements added').', '.$_SESSION['upAggloUww'].' '.t('elements updated').' ( '.$_SESSION['totAggloUww'].' '.t('total elements').')');
        drupal_set_message(t('Links created between \'big cities\' and \'agglomerations\'.'));
        drupal_set_message(t('\'big cities\' and their coordinates have been set.'));
//      if(isset($_SESSION['uwwtd_errors'])){
//          //uwwtd_add_error($annee, $_SESSION['uwwtd_errors']);
//          foreach($_SESSION['uwwtd_errors'] as $error){
//              drupal_set_message(t('Error occured on row index : ').$error[0].' (" '.$error[1].' ") '.t(' of your file. This element was not added, please check your input file.'), 'error');
//          }
//      }
        /*if(isset($_SESSION['uwwtd_warnings'])){
            uwwtd_add_error($annee, $_SESSION['uwwtd_warnings']);
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
        //dsm($_SESSION);
        drupal_goto('data');
    }
    else{
        // Definition des variables pour le formulaire
        $annee = $_SESSION['uwwtd_annee'];
        
        // definition du champ input de type file
        $form['uwwtd_text'] = array(
            '#markup' => '<h4>'.t('Receiving areas shapes upload for the year').' : '.$annee.'</h4><p>'.t('Please upload the files that contain the information on the sensitive areas').'.</p>'
        );
        $form['proj'] = array(
            '#type' => 'select',
            '#title' => t('Projection'),
            '#options'=>['4258'=>'ETRS89 (EPSG:4258)', '4326'=>'WGS84 (EPSG:4326)', '3035'=>'ETRS89 LAEA Europe (EPSG:3035)'],
            '#default_value'=>'4258',
            '#required' => TRUE,
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
//  if(isset($_SESSION['uwwtd_errors'])) unset($_SESSION['uwwtd_errors']);
//  if(isset($_SESSION['uwwtd_warnings'])) unset($_SESSION['uwwtd_warnings']);
    $annee = $_SESSION['uwwtd_annee'];
    $proj = $form_state['values']['proj'];
    $file = file_load($form_state['values']['uwwtd_file']);
    
    // dezip the zipped file
    $files = uwwtd_unzip_files($file, 'public://data_shapes/');
    
    uwwtd_shapes_setup_batch($files, $annee, $proj);   
}

function uwwtd_shapes_setup_batch($files, $annee, $proj='4258'){
    $batch = array(
        'title' => t('Data processing for the shapes files'),
        'operations' => array(),
        'error_message' => t('Error!'),
        'finished' => 'uwwtd_shapes_finished',
        'file' => drupal_get_path('module','uwwtd').'/inc/data_center.inc'
    );
    
    //delete all error from previous import
    $batch['operations'][]=array('uwwtd_delete_error', array($annee, 'shape'));
    $batch['operations'][]=array('uwwtd_shapes_batch_process', array($files, $annee, $proj));

    batch_set($batch);
    batch_process('data/shapes');
}

function uwwtd_shapes_batch_process($files, $annee, $proj, &$context){
//     uwwtd_debug_trace(__FUNCTION__);
    if (!isset($context['sandbox']['progress'])) {
        $shpsFiles = array();
        foreach($files['extracted'] as $file){
            $exploFile = explode('.', $file);
            if(end($exploFile) == 'shp') $shpsFiles[] = $file;
        }
        $context['sandbox']['max'] = count($shpsFiles);
        $context['sandbox']['progress'] = 0;
        $context['sandbox']['ids'] = $shpsFiles;        
        
        $context['results']['nbrecord'] = 0;
        $context['results']['nbrecorderror'] = 0;
        $context['results']['nbrecordok'] = 0;        
    }
    $i = $context['sandbox']['progress'];
    $chosen = $context['sandbox']['ids'][$i];

    module_load_include('php','uwwtd','inc/ShapeFile');
    module_load_include('php','uwwtd','inc/ShapeFileException');

    $shp = new ShapeFile(drupal_realpath($chosen), ShapeFile::FLAG_SUPPRESS_Z);

    while ($record = $shp->getRecord(ShapeFile::GEOMETRY_WKT)) {
        $context['results']['nbrecord']++;
        if($node = rca_set_geo_obj($record, $annee,  $proj)){
            $context['results']['nbrecordok']++;
        }
        else{
            $context['results']['nbrecorderror']++;
        }
    }
    
    uwwtd_flush_errors();

    unset($shp);

    $context['sandbox']['progress']++;
    $context['message'] = '(' .$annee . ') ' . t('Processing shape files') . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';;
//  $context['results'][] = ''; 

    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function rca_set_geo_obj($record, $annee,  $proj='4258'){
    if(!$record['shp']) return false;
    $id = '';
    $shptype = '1';

    $dbf = array_change_key_case ($record['dbf']);
    if(isset($dbf['eucd_sa_cm'])){
        $id = uwwtd_check_text($dbf['eucd_sa_cm']);
        $geoType = 'CSA';
    }        
    elseif(isset($dbf['eucdsa_cm'])){
        $id = uwwtd_check_text($dbf['eucdsa_cm']);
        $geoType = 'CSA';
    }        
    elseif(isset($dbf['eucd_sa_rv'])){
        $id = uwwtd_check_text($dbf['eucd_sa_rv']);
        $geoType = 'RW';
    }        
    elseif(isset($dbf['eucdsa_rv'])){
        $id = uwwtd_check_text($dbf['eucdsa_rv']);
        $geoType = 'RW';
    }        
    elseif(isset($dbf['eucd_sa_lk'])){
        $id = uwwtd_check_text($dbf['eucd_sa_lk']);
        $geoType = 'LW';
    }        
    elseif(isset($dbf['eucdsa_lk'])){
        $id = uwwtd_check_text($dbf['eucdsa_lk']);
        $geoType = 'LW';
    }        
    elseif(isset($dbf['eucd_sa_tw'])){
        $id = uwwtd_check_text($dbf['eucd_sa_tw']);
        $geoType = 'ES';
    }  
    elseif(isset($dbf['eucdsa_tw'])){
        $id = uwwtd_check_text($dbf['eucdsa_tw']);
        $geoType = 'ES';
    }  
    elseif(isset($dbf['eucd_sa_cl'])){
        $id = uwwtd_check_text($dbf['eucd_sa_cl']);
        $geoType = 'SA';
    }      
    elseif(isset($dbf['eucdsa_cl'])){
        $id = uwwtd_check_text($dbf['eucdsa_cl']);
        $geoType = 'SA';
    }      
    elseif(isset($dbf['eucd_lsacl'])){
        $id = uwwtd_check_text($dbf['eucd_lsacl']);
        $geoType = 'LSA';
    } 
    elseif(isset($dbf['eucd_sa_ca'])){
        $id = uwwtd_check_text($dbf['eucd_sa_ca']);
        $geoType = 'CSA';
    }
    elseif(isset($dbf['eucdsa_ca'])){
        $id = uwwtd_check_text($dbf['eucdsa_ca']);
        $geoType = 'CSA';
    }    
//for HU missing  type EUCD_SA_CA here
    elseif(isset($dbf['rcacode'])){
        $id = uwwtd_check_text($dbf['rcacode']);
        $geoType = 'CSA';
    }    
    //for IE missing  type EUCD_SA_CA here
    elseif(isset($dbf['localid'])){
        $id = uwwtd_check_text($dbf['localid']);
        $geoType = 'SA';
    }
    elseif(isset($dbf['iso2'])){
        $id = uwwtd_check_text($dbf['iso2']);
        $geoType = 'SA';
    }
    else return false;
    
    if($id!=''){
        $id = str_replace(' ', '', trim($id));
    }
    else return false;
    $geo = uwwtd_wkt_transform($record['shp'],  $proj, 4326);     
    $ids = array();
    $query = db_select('node', 'n');
        $query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->fields('s', array('field_siteid_value'));
        $query->condition('n.type', 'receiving_area', '=');
        $query->condition('s.field_siteid_value', $annee.'_rca_'.$id, '=');
    $rcas = $query->execute();
    foreach($rcas as $rca){
        $ids[] = array('nid'=>$rca->nid, 'title'=>$rca->title, 'siteId'=>$rca->field_siteid_value);
    }

    if(count($ids) > 0){
        // get node
        $nid = $ids[0]['nid'];
        $node = node_load($nid);

        $node->field_rca_geotype['und'][0]['value'] = $geoType;
        $node->field_position_geo['und'][0] = array(
            'geom'=>$geo->bin,
            'geo_type'=>(strtolower(substr($record['shp'],0,4))=="poly"? 'polygon':'multipolygon'),
            'geohash'=>$geo->geohash
        );

        node_save($node);
    }
    else{
        dsm('fail to find ID:'.$id);
        return false; 
    }        
}

function uwwtd_shapes_finished($success, $results, $operations) {
//     dsm($success);
//     dsm($results);
//     dsm($operations);
    if ($success) {
        if ($results['nbrecordok'] != $results['nbrecorderror']) {
            drupal_set_message(t('Some shapes record were not imported. Check on "List Error" page, the error of category "shape format".') . ' (' . $results['nbrecordok'] . '/' .$results['nbrecord'] . ')');
        } else {
          drupal_set_message(t('All the shapes were successfully added to their receiving areas.') . ' (' . $results['nbrecordok'] . '/' .$results['nbrecord'] . ')');
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


################################ DATA CONFORMITY ################################
function uwwtd_conformity_form($form, &$form_state){

    if(!isset($_SESSION['uwwtd_annee'])){
        drupal_goto('data');
    }
    else {
        // Definition des variables pour le formulaire
        $annee = $_SESSION['uwwtd_annee'];
        
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
//  if(isset($_SESSION['uwwtd_errors'])) unset($_SESSION['uwwtd_errors']);
//  if(isset($_SESSION['uwwtd_warnings'])) unset($_SESSION['uwwtd_warnings']);
    $annee = $_SESSION['uwwtd_annee'];

//  if(isset($form_state['values']['SA'])) $sas = $form_state['values']['SA'];
//  else $sas = array();
// 
//  if(isset($form_state['values']['CSA'])) $csas = $form_state['values']['CSA'];
//  else $csas = array();

    $timestamp = strtotime('now');

    //Init the batch process
//  uwwtd_conformity_setup_batch($sas, $csas, $annee, $timestamp);
    uwwtd_conformity_setup_batch($annee, $timestamp);
}

// function uwwtd_conformity_setup_batch($sas, $csas, $annee, $timestamp){
function uwwtd_conformity_setup_batch($annee, $timestamp){
    $batch = array(
        'title' => t('Data processing for setting the conformity'),
        'operations' => array(
            array('uwwtd_delete_error', array($annee, 'conformity')),
//          array('uwwtd_add_sa_criterions_batch_process', array($sas, $timestamp)),
//          array('uwwtd_add_csa_criterions_batch_process', array($csas, $timestamp)),
            array('uwwtd_station_conformity_batch_process', array($annee, $timestamp)),
            array('uwwtd_agglo_conformity_batch_process', array($annee, $timestamp)),
            array('uwwtd_update_cache_data', array(array($annee))),
            //replace by new algo in uwwtd_agglo_conformity_batch_process
            //array('uwwtd_agglo_reconformity_batch_process', array($annee, $timestamp))
            array('uwwtd_conformity_var', array(array($annee))),
        ),
        'error_message' => t('Error!'),
        'finished' => 'uwwtd_conformity_finished',
        'file' => drupal_get_path('module','uwwtd').'/inc/data_center.inc'
    );
    //calculate the register file
//     uwwtd_register_set_operation($batch, $annee);
//     uwwtd_art17_set_export_operation($batch, $annee);

    batch_set($batch);
    batch_process('data/conformity');
}

function uwwtd_delete_error($annee, $type, &$context)
{
//     dsm(__FUNCTION__ . $annee . $type);
    if (!isset($context['sandbox']['progress'])) {
        $context['sandbox']['max'] = 1;
        $context['sandbox']['progress'] = 0;
    }
// $GLOBALS['uwwtd_errors_type']['category'] = array(
//     'import data input' => 0,
//     'import data linking' => 1,
//     'import data geometry' => 2,
//     'conformity data' => 3,
//     'import data format' => 4,
//     'shape format' => 5,
//     'article17 format' => 6,   //ok
//     'article17 input' => 7,   //ok
// );
    switch ($type) {
        case 'import':
                db_delete('uwwtd_import_errors')
                ->condition('year', $annee, '=')
                ->condition('category', array(
                    $GLOBALS['uwwtd_errors_type']['category']['import data input'],
                    $GLOBALS['uwwtd_errors_type']['category']['import data linking'],
                    $GLOBALS['uwwtd_errors_type']['category']['import data geometry'],
                    $GLOBALS['uwwtd_errors_type']['category']['import data format'],
                    ), 'in'
                )
                ->execute();
            break;
        case 'conformity':
                db_delete('uwwtd_import_errors')
                ->condition('year', $annee, '=')
                ->condition('category', array(
                    $GLOBALS['uwwtd_errors_type']['category']['conformity data'],
                    ), 'in'
                )
                ->execute();
            break;
        case 'shape':
                db_delete('uwwtd_import_errors')
                ->condition('year', $annee, '=')
                ->condition('category', array(
                    $GLOBALS['uwwtd_errors_type']['category']['shape format'],
                    ), 'in'
                )
                ->execute();
            break;
        case 'article17':
                $del = db_delete('uwwtd_import_errors')
                ->condition('year', $annee, '=')
                ->condition('category', array(
                    $GLOBALS['uwwtd_errors_type']['category']['article17 format'],
                    $GLOBALS['uwwtd_errors_type']['category']['article17 input'],
                    ), 'in'
                ) ;
//                 dsm($del->__toString());
                $result = $del->execute();
//                 dsm($result);
            break;
    }

    $context['sandbox']['progress']++;
    $context['message'] = '(' .$annee . ') ' . t('previous error deleted.');
    $context['results']['delete_error'] = 'previous error deleted';

    $context['finished'] = 1;
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

        $node->field_aggcritb['und'][0]['value'] = (true === isset($data['art5b']) ? $data['art5b'] : null);
        $node->field_aggcritca['und'][0]['value'] = (true === isset($data['art5ca']) ? $data['art5ca'] : null);
        $node->field_aggcritcb['und'][0]['value'] = (true === isset($data['art5cb']) ? $data['art5cb'] : null);
        
        node_save($node);
        $message = t('Processing criterion for the agglomeration').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
    } else {
        $message = t('Nothing to Process');
    }

    $context['sandbox']['progress']++;
    $context['message'] = '(' .$annee . ') ' . $message . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
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

        $node->field_aggcritb['und'][0]['value'] = (true === isset($data['art5b']) ? $data['art5b'] : null);
        $node->field_aggcritca['und'][0]['value'] = (true === isset($data['art5ca']) ? $data['art5ca'] : null);
        $node->field_aggcritcb['und'][0]['value'] = (true === isset($data['art5cb']) ? $data['art5cb'] : null);
        
        node_save($node);
        
        $message = t('Processing criterion for the agglomeration').' : '.check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid);
    } else {
        $message = t('Nothing to Process');
    } 

    $context['sandbox']['progress']++;
    $context['message'] = '(' .$annee . ') ' . $message . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')';
    $context['results'][] = $node->nid . ' : ' . check_plain($node->title); 

    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

/**
 *
 */ 
function uwwtd_station_conformity_batch_process($annee, $timestamp, &$context){
    uwwtd_trace(__FUNCTION__);
    if (!isset($context['sandbox']['progress'])) {
    
        $ids = array();
    
        //Get the uwwtps
        $query = db_select('node', 'n');
        $query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
        //$query->join('field_data_field_uwwcompliance', 'c', 'c.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->condition('n.type', 'uwwtp', '=');
        //$query->condition('c.field_uwwcompliance_value', 'NC', '=');
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
    
    $j = 0;
    $nids = [];
    while ($j < 20 && $i < $context['sandbox']['max']) {
        $nids[] = $context['sandbox']['ids'][$i]['nid'];       
        $i++;
        $j++;
    }
    $msg = [];
    if(!empty($nids)){
        $nodes = node_load_multiple($nids);
        if(!empty($nodes)){
            foreach($nodes as $node){
                uwwtd_station_set_conformity($node);
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
                $msg[] = check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid) ;
            }
        }
    }
    $context['sandbox']['progress']=$i;
    $context['message'] =  '<b>(' .$annee . ') ' . t('Processing conformity for UWWTPs') . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')'. ' : </b><br/>'. implode('<br/>', $msg);
    

    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_station_set_conformity(&$node){
    uwwtd_trace('calculate compliance uwwtp ' . $node->field_inspireidlocalid['und'][0]['value']);
    // Function that checks the required treatment, the compliance and the reason for the compliance in the following form array(requiredTreatment, compliance, reason)
    //     $compliance_info = new stdClass();
    //     $compliance_info->required = '';
    //     $compliance_info->required_rcaan = '';
    //     $compliance_info->required_rcaap = '';
    //     $compliance_info->required_rcab  = '';
    //     $compliance_info->required_rcac  = '';  
    //     $compliance_info->compliance = '';
    //     $compliance_info->reason = '';
    //     $compliance_info->treatment_met = '';
    //     $compliance_info->performance_met = '';
    
    //Fix nd@oieau.fr : 09/10/2018, if the UWWTP is inactive, set compliance status to NR
    if(!isset($node->field_status['und']) || $node->field_status['und'][0]['value']!=1){
        $compliance_info = new stdClass();
        
        $compliance_info->required =null;
        $compliance_info->required_rcaan = 0;
        $compliance_info->required_rcaap = 0;
        $compliance_info->required_rcab  = 0;
        $compliance_info->required_rcac  = 0; 
        $compliance_info->treatment_met = 2; //Not Calculable
        $compliance_info->performance_met = 2;  //Not Calculable
        
        $compliance_info->compliance = 'NR';
        $compliance_info->reason = 'UWWTP is inactive';
    }
    else{
        $compliance_info = uwwtd_station_compliance_algo($node, time()); //ALGO2 (et ALGO1)
    }
        uwwtd_trace('calculate compliance uwwtp result : ' . var_export($compliance_info, true));
        
        $node->field_uwwtreatmentrequired['und'][0]['value'] = $compliance_info->required;
        $node->field_uwww_required_rcaan['und'][0]['value'] = $compliance_info->required_rcaan;
        $node->field_uwww_required_rcaap['und'][0]['value'] = $compliance_info->required_rcaap;
        $node->field_uwww_required_rcab['und'][0]['value'] = $compliance_info->required_rcab;
        $node->field_uwww_required_rcac['und'][0]['value'] = $compliance_info->required_rcac;
        
        $node->field_uwwtreatment_met['und'][0]['value'] =  $compliance_info->treatment_met;
        $node->field_uwwperformance_met['und'][0]['value'] =  $compliance_info->performance_met;
        
        $dateReportingReference = $node->field_anneedata['und'][0]['value'].'-12'.'-31';
        $deadLineBeforeReference = false;
        // Station compliance , step B, on of the agglo has at least one date before reference date
        //ALGO3
        if (true === isset($node->field_uwwaggliste['und'])) {
            foreach($node->field_uwwaggliste['und'] as $agg){
                $dateaggl = node_load($agg['nid']);

    //             if ($dateaggl->field_aggperiodover4['und'][0]['value'] < $dateReportingReference
    //                 ||$dateaggl->field_aggperiodover5['und'][0]['value'] < $dateReportingReference
    //                 || $dateaggl->field_aggperiodover6['und'][0]['value'] < $dateReportingReference) {
    //                 $deadLineBeforeReference = true;
    //                 break;
    //             } 
                
                if (deadline_beforeorequal_to_referenceyear($dateaggl->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value'])
                    || deadline_beforeorequal_to_referenceyear($dateaggl->field_aggperiodover5['und'][0]['value'], $node->field_anneedata['und'][0]['value'])
                    || deadline_beforeorequal_to_referenceyear($dateaggl->field_aggperiodover6['und'][0]['value'], $node->field_anneedata['und'][0]['value'])) {
                    $deadLineBeforeReference = true;
                    break;
                }                    
            }
        }
        uwwtd_trace('$deadLineBeforeReference = ' . $deadLineBeforeReference);
        if (false === $deadLineBeforeReference) {
            if($compliance_info->compliance != 'C'){
                $compliance_info->compliance = 'PD';  
            }
        }
    //}
    $node->field_uwwcompliance['und'][0]['value'] = $compliance_info->compliance;
    $node->field_compliance_explication['und'][0]['value'] = $compliance_info->reason;
    
    node_save($node);
}

//Function change by nd@oieau.fr in order to fix errors on not provided data : Collective system + IAS + D without treatement = 100
// and fix infinite loop in uwwtd_article3_compliance()
function uwwtd_agglo_conformity_batch_process($annee, $timestamp, &$context){
    
    uwwtd_trace(__FUNCTION__);        
    if (!isset($context['sandbox']['progress'])) {
    
        $ids = array();
    
        //Get the agglos
        $query = db_select('node', 'n');
        $query->join('field_data_field_siteid', 's', 's.entity_id = n.nid');
        $query->join('field_data_field_agggenerated', 'g', 'g.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->condition('n.type', 'agglomeration', '=');
        $query->condition('s.field_siteid_value', db_like($annee).'%', 'LIKE');
        //$query->join('field_data_field_aggcompliance', 'c', 'c.entity_id = n.nid');
        //$query->condition('c.field_aggcompliance_value', 'NI');
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
    
    $j = 0;
    $nids = [];
    while ($j < 50 && $i < $context['sandbox']['max']) {
        $nids[] = $context['sandbox']['ids'][$i]['nid'];       
        $i++;
        $j++;
    }
    
    $msg = [];
    if(!empty($nids)){
        $nodes = node_load_multiple($nids);
        if(!empty($nodes)){
            foreach($nodes as $node){
                uwwtd_agglo_set_conformity($node);
                $context['results'][] = $node->nid . ' : ' . check_plain($node->title); 
                $msg[] = check_plain($node->title).' '.t('with a drupal id of :').check_plain($node->nid) ;
            }
        }
    }
    

    $context['sandbox']['progress']=$i;
    $context['message'] =  '<b>(' .$annee . ') ' . t('Processing conformity for agglomerations') . ' ('.$context['sandbox']['progress'] .'/'. $context['sandbox']['max'].')'. ' : </b><br/>'. implode('<br/>', $msg);
    
    

    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
}

function uwwtd_agglo_set_conformity(&$node, $verbose = false){
    $verbose_msg = '';
    //We need to fix error due to not provided data : Collective system + IAS + D without treatement = 100
    if(!isset($node->field_aggc1['und']) || !isset($node->field_aggc2['und']) || !isset($node->field_aggpercwithouttreatment['und'])){
        //debug
        
            $verbose_msg.="fix error due to not provided data : Collective system + IAS + D without treatement = 100 \n";
        
        if(isset($node->field_aggc1['und'])){
            $aggc1=$node->field_aggc1['und'][0]['value'];
            $f_aggc1=true;
        }
        else{
            $aggc1=0;
            $f_aggc1=false;
        }
        if(isset($node->field_aggc2['und'])){
            $aggc2=$node->field_aggc2['und'][0]['value'];
            $f_aggc2=true;
        }
        else{
            $aggc2=0;
            $f_aggc2=false;
        }
        if(isset($node->field_aggpercwithouttreatment['und'])){
            $wt=$node->field_aggpercwithouttreatment['und'][0]['value'];
            $f_wt=true;
        }
        else{
            $wt=0;
            $f_wt=false;
        }
        //If the sum = 100 we can fill the lost value with 0 
        $sum = $aggc1+$aggc2+$wt;
        if( $sum >=99.9 && $sum <=100.1){
            if(!$f_wt) $node->field_aggpercwithouttreatment['und'][0]['value'] = 0;
            if(!$f_aggc2) $node->field_aggc2['und'][0]['value'] = 0;
            if(!$f_aggc1) $node->field_aggc1['und'][0]['value'] = 0;
            node_save($node);
        }
        //If sum < 100 but we have at leat 2 values with aggc1, we can make a subtraction
        if($sum < 100 && $f_aggc1){
            if($f_aggc2){
                $node->field_aggpercwithouttreatment['und'][0]['value'] = 100 - ($aggc1+$aggc2);
                node_save($node);
            }
            elseif($f_wt){
                $node->field_aggc2['und'][0]['value'] = 100 - ($aggc1+$wt);
                node_save($node);
            }
        }
    }
    
    node_object_prepare($node);
    
    uwwtd_trace('calculate compliance for agglo : ' . $node->field_inspireidlocalid['und'][0]['value']);        
    
    $yearReporting = $node->field_anneedata['und'][0]['value'];
    //ALGO4
    list($art3comp, $art3compAdditionnal, $art4comp, $art5comp, $art6comp, $reason) = uwwtd_article3_compliance($node, $yearReporting);
    if( $reason!=''){
        $node->field_article_3_compliance_expli['und'][0]=[
            'value'=>$reason,
            'format'=>'plain_text'
        ];
    }
    //debug
    
        $verbose_msg.="uwwtd_article3_compliance \n";
        $verbose_msg.=" - art3comp : $art3comp \n";
        $verbose_msg.=" - art3compAdditionnal : $art3compAdditionnal \n";
        $verbose_msg.=" - art4comp : $art4comp \n";
        $verbose_msg.=" - art5comp : $art5comp \n";
        $verbose_msg.=" - art6comp : $art6comp \n";
        $verbose_msg.=" - reason : $reason \n";
    
    
    
    $infoAllAggUww = uwwtd_get_all_info_agguwww($node);
    $bDone = false; 
    if($art3comp == 'NI'){ //ALGO4 too
        $node->field_aggart3compliance['und'][0]['value'] = 'NI';
        $node->field_aggart4compliance['und'][0]['value'] = 'NI';
        $node->field_aggart5compliance['und'][0]['value'] = 'NI';
        $node->field_aggart6compliance['und'][0]['value'] = 'NI';
        $bDone = true;
        
            $verbose_msg.=" -- art3comp == NI (Algo 4) => bDone = TRUE \n";
    }
    //case < 2000p.e  algo4 too
    if ($bDone === false && $art3comp == 'NR' && true === isset($art4comp) && $art4comp == 'NR') {
        $node->field_aggart3compliance['und'][0]['value'] = $art3comp;
        $node->field_aggart4compliance['und'][0]['value'] = $art4comp;
        $node->field_aggart5compliance['und'][0]['value'] = $art5comp;
        $node->field_aggart6compliance['und'][0]['value'] = $art6comp;
        $node->field_compliance_explication['und'][0]['value'] = $reason;
        $bDone = true;  
        
            $verbose_msg.=" -- case < 2000p.e (Algo 4) => bDone = TRUE \n";
        
    }
    
    if($bDone === false) {
        $node->field_aggart3compliance['und'][0]['value'] = $art3comp;  //NR/NC/C ALGO4 too
        if (false === is_null($art3compAdditionnal)) {
            $node->field_aggart3addcompliance['und'][0]['value'] = $art3compAdditionnal;
            uwwtd_trace('field_aggart3addcompliance for agglo nid ' . $node->nid);
                $verbose_msg.=" -- bDone = False && art3compAdditionnal IS NOT NULL ==> field_aggart3addcompliance = $art3compAdditionnal \n"; 
        } 
        else{
                $verbose_msg.=" -- bDone = False && art3compAdditionnal IS NULL\n";
        }
    } 

    if($bDone === false){
        //ALGO5 too    
        if($node->field_aggc1['und'][0]['value'] > 0){  //algo5  
            
                $verbose_msg.=" --- Algo 5 : field_aggc1 = ".$node->field_aggc1['und'][0]['value']." run uwwtd_article4_5_6_agglomeration_compliance_step_a\n";
            $aArtCompliance = uwwtd_article4_5_6_agglomeration_compliance_step_a($infoAllAggUww, $verbose);  //algo5
        } else {             
           $aArtCompliance = array();  
           $verbose_msg.=" X-- Algo 5 : field_aggc1 = 0 \n";
        }
        if (false === empty($aArtCompliance)) {
            if($aArtCompliance['compArt4'] == $GLOBALS['uwwtd_value']['compliance']['unknown']){
                //================================================Special rules for 10th report under instruction of DGEnv
                $ids = explode('_', $node->field_siteid['und'][0]['value']);
                //IT Case
                if($ids[0]>=2016 && strtoupper(substr($ids[2], 0, 2)) =='IT'){
                    $aArtCompliance['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
                    $aArtCompliance['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
                    if($node->field_agggenerated['und'][0]['value'] >= 10000){
                        $aArtCompliance['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
                    }else{
                        $aArtCompliance['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
                    }
                }
                // DE case
                elseif($ids[0]>=2016 && strtoupper(substr($ids[2], 0, 2)) =='DE'){
                    $aArtCompliance['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];
                    $aArtCompliance['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
                    if($node->field_agggenerated['und'][0]['value'] >= 10000){
                        $aArtCompliance['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];
                    }else{
                        $aArtCompliance['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
                    }
                }
            }
            
            $node->field_aggart4compliance['und'][0]['value'] = $aArtCompliance['compArt4'];
            $node->field_aggart5compliance['und'][0]['value'] = $aArtCompliance['compArt5'];
            $node->field_aggart6compliance['und'][0]['value'] = $aArtCompliance['compArt6'];
            
            
                $verbose_msg.=" --- Algo 5 : field_aggart4compliance = ".$aArtCompliance['compArt4']."\n";
                $verbose_msg.=" --- Algo 5 : field_aggart5compliance = ".$aArtCompliance['compArt5']."\n";
                $verbose_msg.=" --- Algo 5 : field_aggart6compliance = ".$aArtCompliance['compArt6']."\n"; 
            
            
        } 
        else {  
            //e.vincent / 2015/09/02 / new algo station art4,5,6 compliance, then agglomeration art4,5,6 compliance            
            //ALGO6-algo7-algo8-algo9
            
                $verbose_msg.=" X-- Algo 5 : uwwtd_article4_5_6_agglomeration_compliance_step_a fail ==> run uwwtd_article4_5_6_agglomeration_compliance_step_b\n";
            
            //dsm($infoAllAggUww);
            $aggArt456Compliance = uwwtd_article4_5_6_agglomeration_compliance_step_b($node, $infoAllAggUww);
            //dsm($aggArt456Compliance);
            //dsm($aggArt456Compliance);
            //=========ART 4
            $node->field_aggart4compliance['und'][0]['value'] = $aggArt456Compliance['compArt4'];
            if($aggArt456Compliance['compArt4_info']['reason']) {
                $node->field_article_4_compliance_expli['und'][0] = [
                    'value'=>$aggArt456Compliance['compArt4_info']['reason'],
                    'format'=>'plain_text'
                ];
            }
            
            //=======ART 5
            if(isset($aggArt456Compliance['compArt5_info']) && isset($aggArt456Compliance['compArt5_info']['art54applies']) && $aggArt456Compliance['compArt5_info']['art54applies']){
                //nd@oieau.fr : 16/05/2019 (technical workshop)==> if art54applies is apply art5 is NR even if receiving area is not compliant
                $node->field_aggart5compliance['und'][0]['value'] = $aggArt456Compliance['compArt5_info']['rca_compliance'];
                //$node->field_aggart5compliance['und'][0]['value'] = $aggArt456Compliance['compArt5_info']['compliance'];
            }
            else{
                $node->field_aggart5compliance['und'][0]['value'] = $aggArt456Compliance['compArt5'];
            }
            if(isset($aggArt456Compliance['compArt523'])){
                $node->field_aggart523compliance['und'][0]['value'] = $aggArt456Compliance['compArt523'];
            }
            if($aggArt456Compliance['compArt5_info']['reason']) {
                $node->field_article_5_compliance_expli['und'][0] = [
                    'value'=>$aggArt456Compliance['compArt5_info']['reason'],
                    'format'=>'plain_text'
                ];
            }
            
            //======ART 6
            $node->field_aggart6compliance['und'][0]['value'] = $aggArt456Compliance['compArt6'];
            if($aggArt456Compliance['compArt5_info']['reason']) {
                $node->field_article_6_compliance_expli['und'][0] = [
                    'value'=>$aggArt456Compliance['compArt6_info']['reason'],
                    'format'=>'plain_text'
                ];
            }
            
            
                $verbose_msg.=" ---- uwwtd_article4_5_6_agglomeration_compliance_step_b \n";
                $verbose_msg.=" ---- field_aggart4compliance = ".$aggArt456Compliance['compArt4']."\n";
                if(isset($aggArt456Compliance['compArt4_info'])) $verbose_msg.=" -------> reason : ".$aggArt456Compliance['compArt4_info']['reason']."\n";
                if(isset($aggArt456Compliance['compArt5_info'])){
                    if(isset($aggArt456Compliance['compArt5_info']['art54applies']) && $aggArt456Compliance['compArt5_info']['art54applies']){
                        $verbose_msg.=" ---- field_aggart5compliance = ".$aggArt456Compliance['compArt5']." with : \n";
                        $verbose_msg.=" --------------- RCA compliance = ".$aggArt456Compliance['compArt5_info']['rca_compliance']." \n";
                        $verbose_msg.=" --------------- Art5.2,3 = ".$aggArt456Compliance['compArt5_info']['compliance_523']." \n";
                        if(isset($aggArt456Compliance['compArt5_info'])) $verbose_msg.=" -------> reason : ".$aggArt456Compliance['compArt5_info']['reason']."\n";
                    }else{
                        $verbose_msg.=" ---- field_aggart5compliance = ".$aggArt456Compliance['compArt5']." with : \n";
                        if(isset($aggArt456Compliance['compArt5_info'])) $verbose_msg.=" -------> reason : ".$aggArt456Compliance['compArt5_info']['reason']."\n";
                    }
                }
                $verbose_msg.=" ---- field_aggart6compliance = ".$aggArt456Compliance['compArt6']."\n"; 
                    if(isset($aggArt456Compliance['compArt6_info'])) $verbose_msg.=" -------> reason : ".$aggArt456Compliance['compArt6_info']['reason']."\n";
            
        }
    }
    
    //ALGO10    
    $node->field_aggcompliance['und'][0]['value'] = uwwtd_agglomeration_compliance($node,  $infoAllAggUww);
    $verbose_msg.=" -- ALGO10 : field_aggcompliance = ".$node->field_aggcompliance['und'][0]['value']." \n";
    

    if ($node->field_aggart3compliance['und'][0]['value'] == 'NC') {
         $verbose_msg.="===> application of hierarchical compliance because ART 3 is not compliant \n";
        if ($node->field_aggart4compliance['und'][0]['value'] == 'C') {
            if (deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value'])) {
                $node->field_aggart4compliance['und'][0]['value'] = 'NC';
            } else {
                $node->field_aggart4compliance['und'][0]['value'] = 'PD';
            }
        }

        if ($node->field_aggart5compliance['und'][0]['value'] == 'C') {
            if (deadline_beforeorequal_to_referenceyear($node->field_aggperiodover5['und'][0]['value'], $node->field_anneedata['und'][0]['value'])) {
                $node->field_aggart5compliance['und'][0]['value'] = 'NC';
            } else {
                $node->field_aggart5compliance['und'][0]['value'] = 'PD';
            }
        }

        if ($node->field_aggart6compliance['und'][0]['value'] == 'C') {
            if (deadline_beforeorequal_to_referenceyear($node->field_aggperiodover6['und'][0]['value'], $node->field_anneedata['und'][0]['value'])) {
                $node->field_aggart6compliance['und'][0]['value'] = 'NC';
            } else {
                $node->field_aggart6compliance['und'][0]['value'] = 'PD';
            }
        }
    } else {
        if ($node->field_aggart4compliance['und'][0]['value'] == 'NC') {
            if ($node->field_aggart5compliance['und'][0]['value'] == 'C') {
                if (deadline_beforeorequal_to_referenceyear($node->field_aggperiodover5['und'][0]['value'], $node->field_anneedata['und'][0]['value'])) {
                    $node->field_aggart5compliance['und'][0]['value'] = 'NC';
                } else {
                    $node->field_aggart5compliance['und'][0]['value'] = 'PD';
                }
            } 
        }
    }

    $verbose_msg.=" FINAL : \n";
    $verbose_msg.=" - field_aggart3compliance : ".$node->field_aggart3compliance['und'][0]['value']."\n";
    $verbose_msg.=" - field_aggart4compliance : ".$node->field_aggart4compliance['und'][0]['value']."\n";
    $verbose_msg.=" - field_aggart5compliance : ".$node->field_aggart5compliance['und'][0]['value']."\n";
    $verbose_msg.=" - field_aggart6compliance : ".$node->field_aggart6compliance['und'][0]['value']."\n";

    unset($infoAllAggUww);    
    $node->field_compliance_explication['und'][0]=[
        'value' => $verbose_msg,
        'format'=>'plain_text'
    ];
    node_save($node);
    
    if($verbose) {
        drupal_set_message('<pre>'.$verbose_msg.'</pre>');
    }
}



/**
 * Get the agglomeration compliance base on the agglomeration art4,5,6 compliance
 * 
 * @param   object  $node    agglomeration node
 * @param   float   $maxgen  % of load entering the treatment plants 
 * @return  string  afgglomeration compliance (NC,NR,NI,...)   
 */ 
function uwwtd_agglomeration_compliance($node, &$infoAllAggUww)
{
    uwwtd_trace(__FUNCTION__);   
    uwwtd_trace($node->field_inspireidlocalid['und'][0]['value']);
    uwwtd_trace($node->field_aggart3compliance['und'][0]['value']);
    uwwtd_trace($node->field_aggart4compliance['und'][0]['value']);
    uwwtd_trace($node->field_aggart5compliance['und'][0]['value']);
    uwwtd_trace($node->field_aggart6compliance['und'][0]['value']);
    
    if ($node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['unknown']) {
        uwwtd_trace(__LINE__);      
        return $GLOBALS['uwwtd_value']['compliance']['unknown'];
    }
    if (in_array($node->field_aggart3compliance['und'][0]['value'], [$GLOBALS['uwwtd_value']['compliance']['compliant'], $GLOBALS['uwwtd_value']['compliance']['compliantQC']])  && (
        $node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['unknown']
        || $node->field_aggart5compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['unknown']
        || $node->field_aggart6compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['unknown'])
    ) {
        uwwtd_trace(__LINE__);
        if($node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_compliant']){
            return $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
        }
        else{
            return $GLOBALS['uwwtd_value']['compliance']['unknown'];
        }
        
    }
    

    if ($node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['no_information']
        || $node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['no_information']
        || $node->field_aggart5compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['no_information']
        || $node->field_aggart6compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['no_information']
    ) {
        uwwtd_trace(__LINE__);      
        return $GLOBALS['uwwtd_value']['compliance']['no_information'];
    }
    
    if ($node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_compliant']
        || $node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_compliant']
        || $node->field_aggart5compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_compliant']
        || $node->field_aggart6compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_compliant']    
    ) {
        uwwtd_trace(__LINE__);      
        return $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
    }
    
    if ($node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant']
        || (
		   $node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant']
        && $node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant']
        && $node->field_aggart5compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant']
        && $node->field_aggart6compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant']
        )    
    ) {
        uwwtd_trace(__LINE__);      
        return $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
    }
	//cn@oieau 13-01-2017
	if($node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['pending_deadline']
    /* nd@oieau.fr : 04/03/2019
        || $node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['pending_deadline']
        || $node->field_aggart5compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['pending_deadline']
        || $node->field_aggart6compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['pending_deadline']    
    */
    ) {
		uwwtd_trace(__LINE__);
		return $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
    }
    /*cn@oieau 13-01-2017
    if ($node->field_aggart3compliance['und'][0]['value'] != $GLOBALS['uwwtd_value']['compliance']['compliant']
        && $node->field_aggart3compliance['und'][0]['value'] != $GLOBALS['uwwtd_value']['compliance']['compliantQC']) {
        uwwtd_trace(__LINE__);      
        return $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
    } */
	
    $maxgen = $infoAllAggUww['total'];
    //(load collected in collecting system- sum of load entering the treatment plants) is <1% or <=2000p.e.
    $percentage_lost = $node->field_aggc1['und'][0]['value'] - $maxgen;    
    //$percentage_lost = bcsub($node->field_aggc1['und'][0]['value'], $maxgen, 2);
    $pe_lost = $percentage_lost / 100 * $node->field_agggenerated['und'][0]['value'];
    //$pe_lost = bcmul(bcdiv($percentage_lost,100,2),$node->field_agggenerated['und'][0]['value'],2);
    
    uwwtd_trace("maxgen = $maxgen");
    uwwtd_trace("pe_lost = $pe_lost");
    uwwtd_trace("percentage_lost = $percentage_lost");      
    uwwtd_trace(__LINE__);


    // BB - 13/01/2016 - Draft 11 - This condition goes into algo 7   

    // if (false === ($percentage_lost < 1 || $pe_lost <= 2000)) {
        // uwwtd_trace(__LINE__);      
        // return $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
    // }   
    if($node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['compliantQC'] ||$node->field_aggart3compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['compliant']){
        $treat_required = array_column($infoAllAggUww['listUww'], 'required');
        if($node->field_agggenerated['und'][0]['value'] <= 10000 && in_array($GLOBALS['uwwtd_value']['required']['appropriate'], $treat_required)){
            return $GLOBALS['uwwtd_value']['compliance']['compliant'];
        }
        elseif ($node->field_aggart6compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['compliant']
            //cn@oieau.fr || $node->field_aggart6compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['pending_deadline']
        )
        {
            uwwtd_trace(__LINE__);
            return $GLOBALS['uwwtd_value']['compliance']['compliant'];
        }
        else{
            if ($node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['compliant']
                //cn@oieau.fr||$node->field_aggart4compliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['pending_deadline']
            )
            {
                uwwtd_trace(__LINE__);
                return $GLOBALS['uwwtd_value']['compliance']['compliant'];
            }
            else{     
                if ($node->field_aggc2['und'][0]['value'] >= 98) {
                    uwwtd_trace(__LINE__);      
                    return $GLOBALS['uwwtd_value']['compliance']['compliant'];
                }
                else{
                    return $GLOBALS['uwwtd_value']['compliance']['not_relevant'];  
                } 
            }
        }
    }
    else{
        return $GLOBALS['uwwtd_value']['compliance']['unknown'];
    }


    

    

    uwwtd_trace(__LINE__);
    return $GLOBALS['uwwtd_value']['compliance']['not_relevant'];          
}

/**
 * Get the compliance art 4,5,6 of an agglomeration
 * Algo 6
 *
 * @param   object  $agg            the agglomeration node of the uwwtp node
 * @param   array   $infoAllAggUww  list of all uwwtp of the agglomeration with their information    
 * @return array    an array with 3 keys : compArt4, compArt5, compArt6 which contains the compliance calculated  
 */ 
function uwwtd_article4_5_6_agglomeration_compliance_step_b($agg, &$infoAllAggUww)
{   
    uwwtd_trace(__FUNCTION__);    
    $d_wot = ($agg->field_aggpercwithouttreatment['und'][0]['value']/100) *  $agg->field_agggenerated['und'][0]['value'];
      

    //collected in collective system = 0 ==> it's probably an error in data or there is no linked uwwtp or all the load is manage in IAS
    if($agg->field_aggc1['und'][0]['value'] == 0){
        if($d_wot > 2000 || $agg->field_aggpercwithouttreatment['und'][0]['value'] > 2){
            if(true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
                $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
            }
            else{
                $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
            }
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
            $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant']; 
        }
        else{
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
            $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant']; 
        }
         
        return $aResult;
    }    
    
    // all uwwtp treatment required in (appropriate or NR)
    if (true === uwwtd_all_uwwtp_required_in(array($GLOBALS['uwwtd_value']['required']['appropriate'], $GLOBALS['uwwtd_value']['required']['notrelevant']), $infoAllAggUww)) {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
        $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];  
        return $aResult;
    }  
    
    $listStationArticleAndUwwtpCompliance = array();
    if(!empty($infoAllAggUww['listUww'])){
        foreach($infoAllAggUww['listUww'] as $niduwwtp => $uwwtp){
            //ALGO6
            //$agg->field_aggart3compliance['und'][0]['value']
            $listStationArticleAndUwwtpCompliance[$niduwwtp] = [];
            if($uwwtp['connected']==1){
                $listStationArticleAndUwwtpCompliance[$niduwwtp] = uwwtd_article4_5_6_station_compliance($agg, $uwwtp['node'], $infoAllAggUww['rca']);
                //add field necessary for next function call
                $listStationArticleAndUwwtpCompliance[$niduwwtp]['connected'] = $uwwtp['connected'];
                $listStationArticleAndUwwtpCompliance[$niduwwtp]['agglo_uww_perc_ent_uw'] = $uwwtp['agglo_uww_perc_ent_uw'];
                $listStationArticleAndUwwtpCompliance[$niduwwtp]['treatmenttype'] = $uwwtp['treatmenttype'];
            }
            else{
                $listStationArticleAndUwwtpCompliance[$niduwwtp]['required'] = $uwwtp['required'];
            }
            $listStationArticleAndUwwtpCompliance[$niduwwtp]['connected'] = $uwwtp['connected'];
            $listStationArticleAndUwwtpCompliance[$niduwwtp]['agglo_uww_perc_ent_uw'] = $uwwtp['agglo_uww_perc_ent_uw'];
            $listStationArticleAndUwwtpCompliance[$niduwwtp]['treatmenttype'] = $uwwtp['treatmenttype'];
        }
    }
    else{
        
    }
//     uwwtd_trace(var_export($listStationArticleAndUwwtpCompliance, true));
    //ALGO7
    $compArt4 = uwwtd_article4_agglomeration_compliance($agg, 'compArt4', $listStationArticleAndUwwtpCompliance);
    $aResult['compArt4'] = $compArt4['compliance'];
    $aResult['compArt4_info'] = $compArt4;
    //ALGO8
    $compArt6 = uwwtd_article6_agglomeration_compliance($agg, 'compArt6', $listStationArticleAndUwwtpCompliance);
    $aResult['compArt6'] = $compArt6['compliance'];
    $aResult['compArt6_info'] = $compArt6;
    //ALGO9         
//FIX ME case of art 5.4 : keep compliance (NR) or RCA compliance or art 5.2,3   
    $compArt5 = uwwtd_article5_agglomeration_compliance($agg, 'compArt5', $listStationArticleAndUwwtpCompliance, $infoAllAggUww['rca']);
    //dsm($compArt5);
    if(isset($compArt5['compliance_523'])){
        if($compArt4['compliance']==$GLOBALS['uwwtd_value']['compliance']['not_compliant']){
            $aResult['compArt523'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
            $compArt5['compliance_523'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
            $compArt5['reason'] .=' AND Art 4 = '.$GLOBALS['uwwtd_value']['compliance']['not_compliant'].' (application of hierarchical compliance)';
        }else{
            $aResult['compArt523'] = $compArt5['compliance_523'];
        }
    } 
    else $aResult['compArt523'] = $compArt5['compArt5'];
    $aResult['compArt5'] = $compArt5['compliance'];
    $aResult['compArt5_info'] = $compArt5;
//     uwwtd_trace(var_export($aResult, true));
    unset($listStationArticleAndUwwtpCompliance);
    return $aResult;    
}

//Algo 7
function uwwtd_article4_agglomeration_compliance($agg, $article, $listUwwtpCompliance) 
{
    uwwtd_trace(__FUNCTION__);
    uwwtd_trace(var_export($listUwwtpCompliance, true));

    $aTmp['listUww'] = $listUwwtpCompliance;
    if(empty($listUwwtpCompliance)){
        if(true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'], 'reason'=>'there is no linked uwwtp and agglomeration is not in pending deadline for art 4'];
        }else{
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'there is no linked uwwtp but agglomeration is still in pending deadline for art 4'];
        }
    }   
    elseif (false === uwwtd_atleastone_uwwtp_required_in(array(
            $GLOBALS['uwwtd_value']['required']['secondary'],
            $GLOBALS['uwwtd_value']['required']['morestringent'],
            $GLOBALS['uwwtd_value']['required']['nitrogen'],
            $GLOBALS['uwwtd_value']['required']['phosphorus'],
            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus'],
            $GLOBALS['uwwtd_value']['required']['other'],
            
            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus_other'],
            $GLOBALS['uwwtd_value']['required']['nitrogen_other'],
            $GLOBALS['uwwtd_value']['required']['phosphorus_other'],             
            ), $aTmp)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'], 'reason'=>'no secondary or more stringent treatment required'];
    }
    
    //sum of load entering for all station with treatment in place = primary or ni <= 1% et < 2000pe
    $totalLoadenteringPE = 0;
    $totalLoadenteringPercent = 0;
    $totalLoadenteringPE_P_NI = 0;
    $totalLoadenteringPercent_P_NI = 0;  
    foreach($listUwwtpCompliance as $uwwtp) {
        $totalLoadenteringPE += ($agg->field_agggenerated['und'][0]['value'] * $uwwtp['agglo_uww_perc_ent_uw'] / 100);
        $totalLoadenteringPercent += $uwwtp['agglo_uww_perc_ent_uw'];
        if (in_array($uwwtp['treatmenttype'], array($GLOBALS['uwwtd_value']['inplace']['primary'], $GLOBALS['uwwtd_value']['inplace']['noinformation']))) {
            $totalLoadenteringPE_P_NI += ($agg->field_agggenerated['und'][0]['value'] * $uwwtp['agglo_uww_perc_ent_uw'] / 100);
            $totalLoadenteringPercent_P_NI += $uwwtp['agglo_uww_perc_ent_uw'];
        }
    }
    uwwtd_trace($totalLoadenteringPE);
    uwwtd_trace($totalLoadenteringPercent);
    if ($totalLoadenteringPE === 0) {
        if(true !== deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'totalLoadenteringPE === 0 but agglomeration is still in pending deadline for art 4'];
        }
        else{
            uwwtd_add_error($agg->field_anneedata['und'][0]['value'], $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['conformity data'], $GLOBALS['uwwtd']['message']['total_load_entering_uwwtd_null'], time(), null, $agg);
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['no_information'], 'reason'=>'totalLoadenteringPE === 0'];
        }
    }
    if (false === ($totalLoadenteringPE_P_NI < 2000 && $totalLoadenteringPercent_P_NI <= 1)) {
        if(true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'], 'reason'=>'totalLoadenteringPE_P_NI < 2000 && totalLoadenteringPercent_P_NI <= 1'];
        } else {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'totalLoadenteringPE_P_NI < 2000 && totalLoadenteringPercent_P_NI <= 1 but under pending deadline'];
        }  
    } 
    
    
    // BB - 12/01/2016 - Draft 11 - New condition coming from art. 10
    // load collected in collecting system - sum of load entering the tp
    // $percentage_lost = ($pe_lost * 100) / $agg->field_aggc1['und'][0]['value'];
    // $pe_lost = ($agg->field_aggc1['und'][0]['value'] * 100 / $agg->field_agggenerated['und'][0]['value']) - $totalLoadenteringPE;
    $totalLoadenteringPercent = 0;
    $totalLoadentering_NC_Percent = 0;
    foreach ($listUwwtpCompliance as $aUwwtp) {
        $totalLoadenteringPercent += $aUwwtp['agglo_uww_perc_ent_uw'];
        if(in_array($aUwwtp['compArt4'], [$GLOBALS['uwwtd_value']['compliance']['not_compliant'], $GLOBALS['uwwtd_value']['compliance']['no_information']] )){
            $totalLoadentering_NC_Percent += $aUwwtp['agglo_uww_perc_ent_uw'];
        }
    }
    $percentage_lost = $agg->field_aggpercwithouttreatment['und'][0]['value'] + ($agg->field_aggc1['und'][0]['value'] - $totalLoadenteringPercent);
    $pe_lost = ($percentage_lost * ($agg->field_aggc1['und'][0]['value'] * $agg->field_agggenerated['und'][0]['value'])) / 10000;
    $percentage_nc = $totalLoadentering_NC_Percent;
    $pe_nc = ($percentage_nc * ($agg->field_aggc1['und'][0]['value'] * $agg->field_agggenerated['und'][0]['value'])) / 10000;
    
    uwwtd_trace('WOLOLOLO');
    uwwtd_trace($agg->title);
    uwwtd_trace($pe_lost);
    uwwtd_trace($percentage_lost);

    /*
    ([(load collected in collecting system- sum of load entering the treatment plants)] + (Sum of load entering for all stations with treatment in place = primary or NI or compliance for article 4 = NC)) <=1% and < 2000p.e" (herewith updated algorithms)
    */
    if (
        (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['not_compliant']), $listUwwtpCompliance) ) || 
        (true === $percentage_lost > 1 || $pe_lost >= 2000)
    ) {
        if($pe_nc <= 2000 && $percentage_nc <= 1 && $percentage_lost <= 1 && $pe_lost <= 2000){
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['compliant'], 'reason'=>'at least one uwwtp is not compliant but pe_nc< 2000 pe and percentage_nc < 1% so agglo is compliant'];
        }
        elseif(true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'], 'reason'=>'at least one uwwtp is not compliant and pe_nc > 2000 pe or $percentage_nc > 1%'];
        } else {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'at least one uwwtp is not compliant but under pending deadline for art 4'];
        }
    }
    
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['pending_deadline']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'at least one uwwtp is at PD state'];
    } 
        
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['compliant']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['compliant'], 'reason'=>''];
    }
    
    return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'], 'reason'=>''];       
}

/**
 * Return the cagglomeration compliance of the article $article based on the compliance article of all his station
 * Algo 8
 * 
 * @param   string   $article               key of the article to look in the second parameter 
 * @param   array    $listUwwtpCompliance   List of all the sstation with all there article 4,5,6 compliance
 * @return  string      The agglomeration article compliance asked with $article                                              
 */ 
function uwwtd_article6_agglomeration_compliance($agg, $article, $listUwwtpCompliance) 
{
    uwwtd_trace(__FUNCTION__);
    $aTmp['listUww'] = $listUwwtpCompliance;    
       
    if (false === uwwtd_atleastone_uwwtp_required_in(array(
            $GLOBALS['uwwtd_value']['required']['primary'],       
            ), $aTmp)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'], 'reason'=>'no uwwtp require primary treatment'];
    }
    
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['not_compliant']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'], 'reason'=>'at least one uwwtp is not compliant'];
    }


    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['pending_deadline']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'at least one uwwtp is under pending deadline'];
    } 
    
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['compliant']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['compliant'], 'reason'=>''];
    }
             
    return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'], 'reason'=>''];    
}

//Algo 9
function uwwtd_article5_agglomeration_compliance($agg, $article, $listUwwtpCompliance, $rcainfo) 
{
    uwwtd_trace(__FUNCTION__);
    //art5(4) applies ?
    if (true === $rcainfo['art54applies']) {
        uwwtd_trace('art54applies applies to agglo');
        uwwtd_trace(var_export($rcainfo, true));
        $nincoming   = $rcainfo['rca_n_incoming_measured']   + $rcainfo['rca_n_incoming_calculated']   + $rcainfo['rca_n_incoming_estimated'];
        $ndischarged = $rcainfo['rca_n_discharged_measured'] + $rcainfo['rca_n_discharged_calculated'] + $rcainfo['rca_n_discharged_estimated'];

        $pincoming   = $rcainfo['rca_p_incoming_measured']   + $rcainfo['rca_p_incoming_calculated']   + $rcainfo['rca_p_incoming_estimated'];
        $pdischarged = $rcainfo['rca_p_discharged_measured'] + $rcainfo['rca_p_discharged_calculated'] + $rcainfo['rca_p_discharged_estimated'];
        
        uwwtd_trace("nincoming = $nincoming");
        uwwtd_trace("ndischarged = $ndischarged");
        uwwtd_trace("pincoming = $pincoming");
        uwwtd_trace("pdischarged = $pdischarged");
        uwwtd_trace("n0.75 = " . (1 - ($ndischarged / $nincoming)));
        uwwtd_trace("p0.75 = " . (1 - ($pdischarged / $pincoming)));

        if (($nincoming > 0 && (1 - ($ndischarged / $nincoming)) >= 0.75)
            &&
            ($pincoming > 0 && (1 - ($pdischarged / $pincoming)) >= 0.75)
            ) {
            if ($agg->field_agggenerated['und'][0]['value'] < 10000) {
                return [
                    'art54applies'=>true,
                    'compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'rca_compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'compliance_523'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'reason'=>'art54applies AND agggenerated < 10000 AND perf > 75 %'
                ];
            } 
            else {
                $comp_523 = uwwtd_article5_54_as_523_compliance($agg, $article, $listUwwtpCompliance, $rcainfo);
                return [
                    'art54applies'=>true,
                    'compliance'=>$GLOBALS['uwwtd_value']['compliance']['compliant'],
                    //'compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'rca_compliance'=>$GLOBALS['uwwtd_value']['compliance']['compliant'],
                    'compliance_523'=>$comp_523['compliance'],
                    'reason'=>'art54applies AND agggenerated > 10000 AND perf > 75 % and compliance of art5.2,3 give '.$comp_523['compliance'].' ('.$comp_523['reason'].')'
                ];
            }              
        } else {
            if ($agg->field_agggenerated['und'][0]['value'] < 10000) {
                return [
                    'art54applies'=>true,
                    'compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'rca_compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'compliance_523'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'reason'=>'art54applies AND agggenerated < 10000 BUT perf < 75 %'
                ];
            } 
            else {
                //FIX ME : In this case we need to check if perf of UWWTPS are "PASS" for N & P 
                $comp_523 = uwwtd_article5_54_as_523_compliance($agg, $article, $listUwwtpCompliance, $rcainfo);
                return [
                    'art54applies'=>true,
                    'compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'],
                    //'compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'],
                    'rca_compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'],
                    'compliance_523'=>$comp_523['compliance'],
                    'reason'=>'art54applies BUT perf on N&P of RCA < 75 % and compliance of art5.2,3 give '.$comp_523['compliance']
                ];
            }              
        }
    }

    if ($agg->field_agggenerated['und'][0]['value'] < 10000) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'], 'reason'=>'agggenerated<1000'];
    }
    
    $aTmp['listUww'] = $listUwwtpCompliance;
    if (false === uwwtd_atleastone_uwwtp_required_in(array(
                            $GLOBALS['uwwtd_value']['required']['morestringent'],
                            $GLOBALS['uwwtd_value']['required']['nitrogen'],
                            $GLOBALS['uwwtd_value']['required']['phosphorus'],
                            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus'],
                            $GLOBALS['uwwtd_value']['required']['other'],
                            
                            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus_other'],
                            $GLOBALS['uwwtd_value']['required']['nitrogen_other'],
                            $GLOBALS['uwwtd_value']['required']['phosphorus_other'],   
                                                        
                            ), $aTmp)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'], 'reason'=>'not uwwtp require more stringent treatment'];
    }
    //sum of load entering for all station with treatment in place = primary or ni <= 1% et < 2000pe
    $totalLoadenteringPE = 0;
    $totalLoadenteringPercent = 0;      
    $totalLoadentering_NC_Percent = 0;
    foreach($listUwwtpCompliance as $uwwtp) {  
        if (in_array($uwwtp['treatmenttype'], array($GLOBALS['uwwtd_value']['inplace']['primary'], 
                                                    $GLOBALS['uwwtd_value']['inplace']['secondary'],
                                                    $GLOBALS['uwwtd_value']['inplace']['noinformation']))) {

            $totalLoadenteringPE += ($agg->field_agggenerated['und'][0]['value'] * $uwwtp['agglo_uww_perc_ent_uw'] / 100);
            $totalLoadenteringPercent += $uwwtp['agglo_uww_perc_ent_uw'];
        }
        if(in_array($aUwwtp['compArt5'], [$GLOBALS['uwwtd_value']['compliance']['not_compliant'], $GLOBALS['uwwtd_value']['compliance']['no_information']] )){
            $totalLoadentering_NC_Percent += $aUwwtp['agglo_uww_perc_ent_uw'];
        }
    }
    $percentage_nc = $totalLoadentering_NC_Percent;
    $pe_nc = ($percentage_nc * ($agg->field_aggc1['und'][0]['value'] * $agg->field_agggenerated['und'][0]['value'])) / 10000;
    if (false === ($totalLoadenteringPE < 2000 && $totalLoadenteringPercent <= 1)) {
        if(true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'], 'reason'=>'totalLoadenteringPE < 2000 && totalLoadenteringPercent <= 1 AND expired deadline'];
        } else {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'totalLoadenteringPE < 2000 && totalLoadenteringPercent <= 1 AND no expired deadline'];
        }
    }
    
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['not_compliant']), $listUwwtpCompliance)) {
        if($pe_nc <= 2000 && $percentage_nc <= 1 ){
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['compliant'], 'reason'=>'at least one uwwtp is not compliant but pe_nc< 2000 pe and percentage_nc < 1% so agglo is compliant'];
        }
        elseif(true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_compliant'], 'reason'=>'at least one uwwtp not compliant and expired deadline'];
        } else {
            return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'at least one uwwtp not compliant and no expired deadline'];
        }
    }
    
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['pending_deadline']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['pending_deadline'], 'reason'=>'at least one uwwtp with PD compliance state'];
    } 
    
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['compliant']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['compliant'], 'reason'=>''];
    }        
    if (true === uwwtd_atleastone_uwwtp_complianceart_in($article, array($GLOBALS['uwwtd_value']['compliance']['no_information']), $listUwwtpCompliance)) {
        return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['no_information'], 'reason'=>'UWWTP have NI status probably because Nperf & Pperf are unavailable'];
    }        
        
    return ['compliance'=>$GLOBALS['uwwtd_value']['compliance']['not_relevant'], 'reason'=>''];    
}

//Simulate art 5.2,3 compliance with 5.4 RCA 
function uwwtd_article5_54_as_523_compliance($agg, $article, $listUwwtpCompliance, $rcainfo){
    $rcainfo['art54applies'] = false;  
    foreach($listUwwtpCompliance as $k=>$v){
        $listUwwtpCompliance[$k]['required'] = $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus'];
        $listUwwtpCompliance[$k]['compArt5'] = $listUwwtpCompliance[$k]['compArt54-523'];
        //Get compliance status of uwwtp with new input data
    }       
    return uwwtd_article5_agglomeration_compliance($agg, $article, $listUwwtpCompliance, $rcainfo);
}

/**
 *
 */ 
function uwwtd_no_uwwtp_with_required_morestringent_has_complianceart5_notrelevant($listUwwtpCompliance)
{
    uwwtd_trace(__FUNCTION__);
    $bResult = true; 
    
    foreach($listUwwtpCompliance as $aUwwtp) {
        if (true === in_array($aUwwtp['required'], array(
            $GLOBALS['uwwtd_value']['required']['nitrogen'],
            $GLOBALS['uwwtd_value']['required']['phosphorus'],
            $GLOBALS['uwwtd_value']['required']['other'],
            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus'],
            $GLOBALS['uwwtd_value']['required']['notrelevant'],
            $GLOBALS['uwwtd_value']['required']['morestringent'],

            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus_other'],
            $GLOBALS['uwwtd_value']['required']['nitrogen_other'],
            $GLOBALS['uwwtd_value']['required']['phosphorus_other'],  
        ))) {        
            if ($aUwwtp['compArt5'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant']) {
                $bResult = false;
                break;
            }
        }
    }    
    return $bResult;
}

/**
 * Get the compliance art 4,5,6 of an uwwtp
 *
 * @param   object  $agg        the agglomeration node of the uwwtp node
 * @param   object  $uwwtp      the uwwtp node we need to calculate the complaince   
 * @return  array    an array with 3 keys : compArt4, compArt5, compArt6 which contains the compliance calculated  
 */ 
function uwwtd_article4_5_6_station_compliance($agg, $uwwtp, $rca = null)
{        
    uwwtd_trace(__FUNCTION__);
    //Case of transboundary UWWTP in BE 
    /*
    if(in_array($uwwtp->field_inspireidlocalid['und'][0]['value'], array('BERW0000601','BERW5401001','BERW0000201','BERW0000104'))){
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];
        $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant']; 
        if($agg->field_agggenerated['und'][0]['value']>=10000){
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];
            $aResult['required'] = 'N';
        }else{
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
            $aResult['required'] = 'S';
        }
        return $aResult;
    }*/
//     uwwtd_trace(__LINE__);
    $aResult['required'] = $uwwtp->field_uwwtreatmentrequired['und'][0]['value'];
    //treatement required = primary
    if ($uwwtp->field_uwwtreatmentrequired['und'][0]['value'] == $GLOBALS['uwwtd_value']['required']['primary']) {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
        if($rca['art54applies']) $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
        if ($uwwtp->field_uwwcompliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['compliant']) {
            $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];    
        } else {
           if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover6['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
           } else {
            $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
           }                             
        }
//         uwwtd_trace(__LINE__);
        return $aResult;         
    }
    
                       
    $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
    //treatement required = secondary && treatment in place = primary
    if ($uwwtp->field_uwwtreatmentrequired['und'][0]['value'] == $GLOBALS['uwwtd_value']['required']['secondary']
//         && true === uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, array('primary'))
        && true === in_array($uwwtp->field_uwwtreatmenttype['und'][0]['value'], array($GLOBALS['uwwtd_value']['inplace']['primary']))        
        ) {
        
       if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
        if($rca['art54applies']) $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
       } else {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline']; 
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
        if($rca['art54applies']) $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
       }
//         uwwtd_trace(__LINE__);
       return $aResult;          
    }
      
    //treatement required = secondary && treatement in place = secondary
    
    if ($uwwtp->field_uwwtreatmentrequired['und'][0]['value'] == $GLOBALS['uwwtd_value']['required']['secondary']
        && true === in_array($uwwtp->field_uwwtreatmenttype['und'][0]['value'], array($GLOBALS['uwwtd_value']['inplace']['secondary'],$GLOBALS['uwwtd_value']['inplace']['morestringent']))) {  
        //We have to check if perf are ok but also if the secondary treatment is in place
        // uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, $listtreatement) 
        if (true === uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('codperf', 'bod5perf'))
            && true === uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, array('secondary'))
        ) {
            
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['compliant']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];   
            if($rca['art54applies']){
                //Test if agg > 10000 : test if more stringnet treament in place and perf
                if($agg->field_agggenerated['und'][0]['value']>=10000){
                    if($uwwtp->field_uwwtreatmenttype['und'][0]['value'] == $GLOBALS['uwwtd_value']['inplace']['morestringent'] &&
                    true === uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('ntotperf', 'ptotperf'))
                    //TODO check if N & P equipement are in place
                    ){
                         if(isset($uwwtp->field_uwwnremoval['und'][0]['value'])|| isset($uwwtp->field_uwwpremoval['und'][0]['value'])){
                              if($uwwtp->field_uwwnremoval['und'][0]['value'] ==0 || $uwwtp->field_uwwpremoval['und'][0]['value'] == 0){
                                  $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
                              }else{
                                  $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];
                              }
                         }
                         //If information on equipment are not provided
                         else{
                             $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['compliant']; //in this case, we consider the (non) environnemental impact
                         }
                         
                    }
                    elseif($uwwtp->field_uwwtreatmenttype['und'][0]['value'] == $GLOBALS['uwwtd_value']['inplace']['morestringent']){
                        //change nd@oieau.fr on 22/01/2021
                        if($uwwtp->field_uwwntotperf['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant'] && $uwwtp->field_uwwptotperf['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_relevant'] ){
                            $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['no_information'];
                        }elseif($uwwtp->field_uwwntotperf['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_compliant'] || $uwwtp->field_uwwptotperf['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['not_compliant'] ){
                            $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
                        }else{
                            $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];
                        }
                    } 
                    else{
                        $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
                    }
                }
                else{
                    $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
                }
            } 
        } else {
           if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
            if($rca['art54applies']) $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
           } else {
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
            if($rca['art54applies']) $aResult['compArt54-523'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
           }            
        } 
//         uwwtd_trace(__LINE__);
        return $aResult;            
    }
    
//     //treatment required = more stringent
//     if (false === uwwtd_required_treatement_morestingent($uwwtp)) {
//         //TODO missing art 4 compliance here 
//         $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
//         return $aResult;   
//     }
   
    //treatement required = secondary && treatement in place = NI (not provided on treatement inplace)
    if ($uwwtp->field_uwwtreatmentrequired['und'][0]['value'] == $GLOBALS['uwwtd_value']['required']['secondary']
        && $uwwtp->field_uwwtreatmenttype['und'][0]['value'] == $GLOBALS['uwwtd_value']['inplace']['noinformation']) {
       if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
       } else {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline']; 
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_relevant'];
       } 
//        uwwtd_trace(__LINE__);     
       return $aResult;          
    }
    
    //treatement required = tertiary && treatement in place = NI (not provided on treatement inplace)
    if (true === in_array($uwwtp->field_uwwtreatmentrequired['und'][0]['value'],array(
                            $GLOBALS['uwwtd_value']['required']['morestringent'],
                            $GLOBALS['uwwtd_value']['required']['nitrogen'],
                            $GLOBALS['uwwtd_value']['required']['phosphorus'],
                            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus'],
                            $GLOBALS['uwwtd_value']['required']['other'],
                            
                            $GLOBALS['uwwtd_value']['required']['nitrogen_phosphorus_other'],
                            $GLOBALS['uwwtd_value']['required']['nitrogen_other'],
                            $GLOBALS['uwwtd_value']['required']['phosphorus_other'],                             
                        ))
        && $uwwtp->field_uwwtreatmenttype['und'][0]['value'] == $GLOBALS['uwwtd_value']['inplace']['noinformation']) { 
       if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
           if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
           } else {
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
           } 
       } else {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline']; 
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
       }
//        uwwtd_trace(__LINE__);      
       return $aResult;          
    }    
    
    //cod and bod5 pass
//     if(true === uwwtd_codperf_pass($uwwtp) && true === uwwtd_bod5perf_pass($uwwtp)){
    if (true === uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, array('codperf', 'bod5perf'))) {
        if ($uwwtp->field_uwwcompliance['und'][0]['value'] == $GLOBALS['uwwtd_value']['compliance']['compliant']) {
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['compliant']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['compliant'];            
        } else {
           if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['compliant']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
           } else {
            $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['compliant']; 
            $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
           }              
        }
//         uwwtd_trace(__LINE__);
        return $aResult;   
    }
    
    //cod and bod5 not pass
   if (false === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
    $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline']; 
    $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
   } else {
       if (true === deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value'])) {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant'];
       } else {
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['not_compliant']; 
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['pending_deadline'];
       } 
   } 
//    uwwtd_trace(__LINE__);
   return $aResult;   
}




function uwwtd_conformity_finished($success, $results, $operations) {
    if ($success) {
//      drupal_set_message(t('All the criterions were added to the agglomerations concerned.'));
        drupal_set_message(t('All the waste water treatment plants\' conformity was calculated.'));
        drupal_set_message(t('All the agglomerations\' conformity was calculated.'));
//         drupal_set_message(t('Register calculated.'));
//         drupal_set_message(t('Article17 calculated.'));
        drupal_set_message(t('Congratulations! Your data has been successfully imported to the site you can now view your data on the interactive maps accessible from the home page.'));
//      if(isset($_SESSION['uwwtd_errors'])){
//          uwwtd_add_error($annee, $_SESSION['uwwtd_errors']);
//          foreach($_SESSION['uwwtd_errors'] as $error){
//              drupal_set_message(t('Error occured on row index : ').$error[0].' (" '.$error[1].' ") '.t(' of your file . This element was not added, please check your input file.'), 'error');
//          }
//      }
//      if(isset($_SESSION['uwwtd_warnings'])){
//          uwwtd_add_error($annee, $_SESSION['uwwtd_warnings']);
//          foreach($_SESSION['uwwtd_warnings'] as $warning){
//              drupal_set_message(t('Warning occured on row index : ').$warning[0].' (" '.$warning[1].' ") '.t(' of your file . This element was added but please check your input file.'), 
//                  'warning');
//          }
//      }
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

function uwwtd_conformity_var($annee) {
//     dsm(__FUNCTION__);
    variable_set('conformity_'.$annee, time());
}

function uwwtd_import_data_var($annee) {
//     dsm(__FUNCTION__);
    variable_set('import_data_'.$annee, time());
}


/**
 *  Create errors for agglomerations without treatment plants.
 */
function uwwtd_set_errors_aggs_without_tp($annee) {
    $aggsWithoutTp = variable_get(AGGS_WITHOUT_TP, array());
    if (!empty($aggsWithoutTp)) {
        // Get nids of agglomerations :
        $query = db_select('node', 'n');
        $query->leftJoin('field_data_field_inspireidlocalid', 'a', 'a.entity_id = n.nid');
        $query->leftJoin('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->condition('n.type', 'agglomeration', '=');
        $query->condition('a.field_inspireidlocalid_value', $aggsWithoutTp, 'IN');
        $query->condition('r.field_siteid_value', db_like($annee).'%', 'LIKE');
        $nodes = $query->execute();

        // Create errors for each agglo :
        foreach ($nodes as $node) {
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'],
                'Agglomeration without treatment plants', time(), 0, $node);
        }
        uwwtd_flush_errors();
    }
}

/**
 * Create errors for treatment plants without agglomeration.
 */
function uwwtd_set_errors_tps_without_agg($annee) {
    $tpsWithoutAgg = variable_get(TPS_WITHOUT_AGG, array());
    if (!empty($tpsWithoutAgg)) {
        // Get nids of treatment plants :
        $query = db_select('node', 'n');
        $query->leftJoin('field_data_field_inspireidlocalid', 'a', 'a.entity_id = n.nid');
        $query->leftJoin('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->condition('n.type', 'uwwtp', '=');
        $query->condition('a.field_inspireidlocalid_value', $tpsWithoutAgg, 'IN');
        $query->condition('r.field_siteid_value', db_like($annee).'%', 'LIKE');
        $nodes = $query->execute();

        // Create errors for each treatment plants :
        foreach ($nodes as $node) {
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'],
                'Treatment plants without agglomeration', time(), 0, $node);
        }
        uwwtd_flush_errors();
    }
}

/**
 * Create errors for treatment plants without discharge point.
 */
function uwwtd_set_errors_tps_without_dp($annee) {
    $tpsWithoutDp = variable_get(TPS_WITHOUT_DP, array());
    if (!empty($tpsWithoutDp)) {
        // Get nids of treatment plants :
        $query = db_select('node', 'n');
        $query->leftJoin('field_data_field_inspireidlocalid', 'a', 'a.entity_id = n.nid');
        $query->leftJoin('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->condition('n.type', 'uwwtp', '=');
        $query->condition('a.field_inspireidlocalid_value', $tpsWithoutDp, 'IN');
        $query->condition('r.field_siteid_value', db_like($annee).'%', 'LIKE');
        $nodes = $query->execute();

        // Create errors for each treatment plants :
        foreach ($nodes as $node) {
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'],
                'Treatment plants without discharge point', time(), 0, $node);
        }
        uwwtd_flush_errors();
    }
}


/**
 * Create errors for discharge points without treatment plants.
 */
function uwwtd_set_errors_dps_without_tp($annee) {
    $dpsWithoutTp = variable_get(DPS_WITHOUT_TP, array());
    if (!empty($dpsWithoutTp)) {
        // Get nids of treatment plants :
        $query = db_select('node', 'n');
        $query->leftJoin('field_data_field_inspireidlocalid', 'a', 'a.entity_id = n.nid');
        $query->leftJoin('field_data_field_siteid', 'r', 'r.entity_id = n.nid');
        $query->fields('n', array('nid', 'title'));
        $query->condition('n.type', 'discharge_point', '=');
        $query->condition('a.field_inspireidlocalid_value', $dpsWithoutTp, 'IN');
        $query->condition('r.field_siteid_value', db_like($annee).'%', 'LIKE');
        $nodes = $query->execute();

        // Create errors for each agglo :
        foreach ($nodes as $node) {
            uwwtd_add_error($annee, $GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data linking'],
                'Discharge points without treatment plants', time(), 0, $node);
        }
        uwwtd_flush_errors();
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
    $xml = simplexml_load_string($string);
    $namespaces = $xml->getDocNamespaces();
    unset($xml);
    $ns = array_keys($namespaces);
    foreach($ns as $k=>$v){
        $ns[$k]=$v.':';
    }
    $string = str_replace($ns, '', $string);
    $xml = simplexml_load_string($string);
    $deJson = json_encode($xml);
    unset($xml, $string, $ns, $namespaces);
    $xml_array = json_decode($deJson,TRUE);
    return $xml_array;
}

function uwwtd_removeBOM($str=""){
    if(substr($str, 0,3) == pack("CCC",0xef,0xbb,0xbf)) {
            $str=substr($str, 3);
    }
    return $str;
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
    $sql = "SELECT ST_AsBinary('".$geo."'::geometry) as bin, 'point' as pt, ST_Y('".$geo."'::geometry) as geoy, ST_X('".$geo."'::geometry) as geox, ST_GeoHash('".$geo."'::geometry, 16) as geohash";
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

function  uwwtd_coord_transform_line($parts){
    
    $sql = "SELECT ST_Transform(ST_GeomFromText('LINE((";

    $numParts = count($parts);
    $count = 0;

    foreach($parts as $k=>$part){

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
    $sql = "SELECT ST_AsBinary('".$geo."'::geometry) as bin, ST_GeoHash('".$geo."'::geometry, 16) as geohash";
    $result = db_query($sql);
    foreach ($result as $record) {
        $table['bin'] = $record->bin;
        $table['polygon'] = 'line';
        //$table['geoy'] = $record->geoy;
        //$table['geox'] = $record->geox;
        $table['geohash'] = $record->geohash;
    }
    return $table;
}

function uwwtd_coord_transform_polygon($parts){
    $type  = 'POLYGON';
    

    $numParts = count($parts);
    $count = 0;
    $def = '';
    foreach($parts as $k=>$part){
        if($k == 0){
            $firstLat = $part['x'];
            $firstLong = $part['y'];
        }

        $count++;

        $lat = str_replace(",",".",$part['x']);
        $long = str_replace(",",".",$part['y']);

        $def .= $lat.' '.$long;
        if($count < $numParts) $def .= ',';
        else{
            $lastLat = $part['x'];
            $lastLong = $part['y'];
        }
    }

    if($lastLat != $firstLat || $lastLong != $firstLong){
        $type  = 'LINESTRING';
        
        //$sql .= ','.$firstLat.' '.$firstLong;
    }
    if($type  == 'LINESTRING') $sql = "SELECT ST_Transform(ST_GeomFromText('".$type."(".$def.")', 4258),4326) AS geo";
    else $sql = "SELECT ST_Transform(ST_GeomFromText('".$type."((".$def."))', 4258),4326) AS geo";
    $result = db_query($sql);
    foreach ($result as $record) {
        $geo = $record->geo;
    }

    // create array
    $table = array();
    $sql = "SELECT ST_AsBinary('".$geo."'::geometry) as bin, ST_GeoHash('".$geo."'::geometry, 16) as geohash";
    $result = db_query($sql);
    foreach ($result as $record) {
        $table['bin'] = $record->bin;
        $table['polygon'] = strtolower($type);
        $table['geohash'] = $record->geohash;
    }
    return $table;
}

//TODO check if multipolygon is not a multline
function uwwtd_coord_transform_multipolygon($parts){
    $sql = "SELECT ST_Transform(ST_GeomFromText('MULTIPOLYGON(";

    $numParts = count($parts);
    $count = 0;

    foreach($parts as $part){

        $count++;

        $sql .= "((";
        $innerParts = count($part['points']);
        $innerCount = 0;
        foreach($part['points'] as $k=>$inner){
            if($k == 0){
                $firstLat = $inner['x'];
                $firstLong = $inner['y'];
            }

            $innerCount++;
            $lat = str_replace(",",".",$inner['x']);
            $long = str_replace(",",".",$inner['y']);

            $sql .= $lat.' '.$long;
            if($innerCount < $innerParts) $sql .= ',';
            else{
                $lastLat = $inner['x'];
                $lastLong = $inner['y'];
            }
        }
        if($lastLat != $firstLat || $lastLong != $firstLong){
            $sql .= ','.$firstLat.' '.$firstLong;
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
//  $sql = "SELECT ST_AsBinary('".$geo."') as bin, ST_GeoHash('".$geo."'::geometry, 16) as geohash";
    
    $sql = "SELECT ST_AsBinary(:thegeom::geometry) as bin, ST_GeoHash(:thegeom::geometry, 16) as geohash";
    $aParam = array(
                ':thegeom' => $geo,
                    );
    $result = db_query($sql, $aParam);

    foreach ($result as $record) {
        $table['bin'] = $record->bin;
        $table['polygon'] = 'multipolygon';
        //$table['geoy'] = $record->geoy;
        //$table['geox'] = $record->geox;
        $table['geohash'] = $record->geohash;
    }
    return $table;
}

function uwwtd_wkt_transform($wkt, $prj_src, $prj_target){
    $sql = "
        WITH geo AS( SELECT ST_Transform(ST_GeomFromText('".$wkt."', ".$prj_src."),".$prj_target.")::geometry AS obj ) 
        SELECT  
            ST_AsBinary(geo.obj::geometry) as bin,
            ST_GeoHash(geo.obj::geometry, 16) as geohash,
            ST_AsText(geo.obj::geometry) as wkt
        FROM geo
    ";
    $result = db_query($sql);
    foreach ($result as $record) {
        return $record;
    } 
}


/**
 * return art3,4,5,6 compliance and a reason (4,5,6 are optional)
 * 
 * @return  array      (art3compliance, 4 or null,5 or null,6, or nullreason)
 *                      possible value of art3 => NI,NR,C,NC
 *                      possible value for art4,5,6 => NR or null  
 */ 
function uwwtd_article3_compliance($node, $yearReporting){
    uwwtd_trace(__FUNCTION__);
    // only send nodes of type 'agglomeration' here
    //check if aggGenerated exists
    if(!isset($node->field_agggenerated['und'][0]['value'])){
        return array('NI', null, null, null, null, 'Article 3 compliance found to have no information because no data could be found on the "Generated load (p.e.)".');
    }

    // check if aggGenerated over 2000
    if($node->field_agggenerated['und'][0]['value'] < 2000){
        $art3comp = 'NR';
        $art4comp = 'NR';
        $art5comp = 'NR';
        $art6comp = 'NR';
        $reason = 'Article 3 compliance found to be non relevant because the agglomeration has a population of less than 2 000.';
        return array($art3comp, null, $art4comp, $art5comp, $art6comp, $reason);
    }
    
    if(!isset($node->field_aggpercwithouttreatment['und'][0]['value'])){
        return array('NI', null, null, null, null, 'Article 3 compliance found to have no information because no data could be found on the "percentage of waste dischaged without treatment".');
    }
        
    $aggpercwithouttreatment_pe = ($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggpercwithouttreatment['und'][0]['value'];
    $aggc2_pe = ($node->field_agggenerated['und'][0]['value'] / 100) * $node->field_aggc2['und'][0]['value'];

    $aggpercwithouttreatment_percent = $node->field_aggpercwithouttreatment['und'][0]['value'];
    $aggc2_percent = $node->field_aggc2['und'][0]['value'];

    // start calculations
    if (($aggpercwithouttreatment_pe + $aggc2_pe) <= 2000 && ($aggpercwithouttreatment_percent + $aggc2_percent) <= 2){
        $art3comp = 'C';
        $reason = 'Article 3 compliance found to be compliant because the amount of "waste discharged without treatment" and "treated through IAS" is equal or less than 2 000. Also the percentage of "waste discharged without treatment" and "treated through IAS" is equal or less than 2% of total waste.';
        return array($art3comp, null, null, null, null, $reason);
    }


    if ( ! ($aggpercwithouttreatment_pe > 2000 || $aggpercwithouttreatment_percent > 2)) {
        $art3comp = $GLOBALS['uwwtd_value']['compliance']['compliantQC'];
        $reason = 'Article 3 compliance found to be compliantQC because the amount of "waste discharged without treatment" is equal or less than 2 000 or the percentage of "waste discharged without treatment" is equal or less than 2% of total waste.';
        uwwtd_trace('field_aggart3compliance QC for agglo nid ' . $node->nid);
        return array($art3comp, null, null, null, null, $reason);
    }

    if (true === deadline_beforeorequal_to_referenceyear($node->field_aggperiodover3['und'][0]['value'], $node->field_anneedata['und'][0]['value'])) {
        $art3comp = 'NC';
        $reason = 'Not compliant with article 3 because the deadline has been reached';
        $art3compAddictionnal = null;
        if ($aggc2_pe > 1000 || $aggc2_percent > 2) {
            $art3compAddictionnal = $GLOBALS['uwwtd_value']['compliance']['not_compliantQC'];
            uwwtd_trace('field_aggart3addcompliance AddQC for agglo nid ' . $node->nid);
        }
        
        return array($art3comp, $art3compAddictionnal, null, null, null, $reason);
    }

    $art3comp = 'PD';
    $reason = 'Article 3 compliance found to be pending deadline deadline has not been reached';
    return array($art3comp, null, null, null, null, $reason);
}

/**
 * Return art3,4,5,6 agglomeration compliance for some first case
 * @param   object      $node       agglomeration node
 * @param   string      $art3comp   art3 agglomeration compliance
 * @return array        empty array if the cases must be treated later
 *                      array with key compArt3, compArt4, compArt5, compArt6      
 */ 
function uwwtd_article4_5_6_agglomeration_compliance_step_a(&$infoAllAggUww, $verbose=false) 
{
    uwwtd_trace(__FUNCTION__);
    $aResult = array();
    if (count($infoAllAggUww['listUww']) == 0) {
//      $aResult['compArt3'] = $art3comp;
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['unknown'];
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['unknown'];
        $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['unknown'];
    }
    elseif (
        true === uwwtd_atleastone_uwwtp_compliance_in(array($GLOBALS['uwwtd_value']['compliance']['unknown']),$infoAllAggUww)
        && true === uwwtd_all_uwwtp_compliance_in(array(
            $GLOBALS['uwwtd_value']['compliance']['unknown'],
            $GLOBALS['uwwtd_value']['compliance']['compliant'],
            $GLOBALS['uwwtd_value']['compliance']['not_relevant'],
            $GLOBALS['uwwtd_value']['compliance']['pending_deadline'],
        ), $infoAllAggUww)
    ) {
//      $aResult['compArt3'] = $art3comp;
        $aResult['compArt4'] = $GLOBALS['uwwtd_value']['compliance']['unknown'];
        $aResult['compArt5'] = $GLOBALS['uwwtd_value']['compliance']['unknown'];
        $aResult['compArt6'] = $GLOBALS['uwwtd_value']['compliance']['unknown'];                                                    
    }
//     unset($infoAllAggUww); 
    
    return $aResult;
}

function uwwtd_station_compliance_algo($node, $timestamp){
    global $conf;
    uwwtd_trace(__FUNCTION__);
    // Get all data needed to calculate compliance
    // Get aglo data first
    $compliance_info = new stdClass();
    $compliance_info->required = '';
    $compliance_info->required_rcaan = 0;
    $compliance_info->required_rcaap = 0;
    $compliance_info->required_rcab  = 0;
    $compliance_info->required_rcac  = 0;  
    $compliance_info->compliance = '';
    $compliance_info->reason = '';
    $compliance_info->treatment_met = 0;
    $compliance_info->performance_met = 0;
    
    $agglos = array();
    if(isset($node->field_uwwaggliste['und'])){
        $maxgen = 0;
        foreach($node->field_uwwaggliste['und'] as $agglist){
            $agg = node_load($agglist['nid']);
            if($agg->field_agggenerated['und'][0]['value'] > $maxgen) $maxgen = $agg->field_agggenerated['und'][0]['value'];
            $aggCode = $agg->field_inspireidlocalid['und'][0]['value'];
            $agglos[$aggCode] = array(
                'nid' => $agg->nid,
                'title' => $agg->title,
                'generated' => $agg->field_agggenerated['und'][0]['value'],
                'inspireidlocalid'=>$aggCode
            );
        }
    }

    if(count($agglos) == 0){
        $compliance_info->compliance = 'NI';
        $compliance_info->reason = 'Station found to have no information on agglomerations';
        uwwtd_add_error($node->field_anneedata['und'][0]['value'], $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['conformity data'], 'Station found to have no information on agglomerations.', $timestamp, null, $node);
        $compliance_info->required = 'NI';
        $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
        $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
        return $compliance_info;
    }
    
        // Get dcp data
    $dcps = array();
    if(isset($node->field_uwwdcpliste['und'])){
        foreach($node->field_uwwdcpliste['und'] as $dcplist){
            $dcp = node_load($dcplist['nid']);
            $rca54applied = false;
            $rcaANitro = false;
            $rcaAPhos = false;
            $rcab = false;
            $rcac = false;
            $paramN = false;
            $paramP = false;
            $paramO = false;
            $zonetypeIsSet = false;
            $remarks=[];
                            
            /* Cas des pays avec l'ensemble du pays en zone sensible */
            $rcaTypeid = $dcp->field_rcatype['und'][0]['value'];
            // if('LT' == uwwtd_get_countrycode_from_repcode($node->field_repcode['und'][0]['value'])){
            //     $rcaTypeid = 'SA';
            // } 
            if(isset($dcp->field_dcprcaliste['und'])){
                foreach($dcp->field_dcprcaliste['und'] as $rcalist){
                    $rca = node_load($rcalist['nid']);
                    
                    
                    $isrca54applied = (isset($rca->field_rca54applied['und']) ? $rca->field_rca54applied['und'][0]['value'] == 1 : false);
                    $isrcaANitro = (isset($rca->field_rcaanitro['und']) ? $rca->field_rcaanitro['und'][0]['value'] == 1: false);
                    $isrcaAPhos  = (isset($rca->field_rcaaphos['und']) ? $rca->field_rcaaphos['und'][0]['value'] == 1: false);
                    $isrcab      = (isset($rca->field_rcab['und']) ? $rca->field_rcab['und'][0]['value'] == 1: false);
                    $isrcac      = (isset($rca->field_rcac['und']) ? $rca->field_rcac['und'][0]['value'] == 1: false);
                    $hasParamN      = (isset($rca->field_rca_parameter_n['und']) ? $rca->field_rca_parameter_n['und'][0]['value'] == 1: false);
                    $hasParamP      = (isset($rca->field_rca_parameter_p['und']) ? $rca->field_rca_parameter_p['und'][0]['value'] == 1: false);
                    $hasParamO      = (isset($rca->field_rca_parameter_other['und']) ? $rca->field_rca_parameter_other['und'][0]['value'] == 1: false);

                    $zonetypeIsSet = (isset($rca->field_zonetype['und'][0]['value']) ? $rca->field_zonetype['und'][0]['value'] : false);
                    if (isset($rca->field_specialisedzonetype['und'][0]['value'])) {
                        $rcaTypeid = $rca->field_specialisedzonetype['und'][0]['value'];
                    }

                    if($isrca54applied) $rca54applied = true;
                    
                    
                    if($isrcaANitro){
                        if(isset($rca->field_rcaanstartdate['und'])){
                            if(true === deadline_beforeorequal_to_referenceyear($rca->field_rcaanstartdate['und'][0]['value'], $node->field_anneedata['und'][0]['value'])){
                                $rcaANitro = true;
                                 $remarks[]='rcaANitro with an expired date of application';
                            }else{
                                $rcaANitro = false;
                                $remarks[]='rcaANitro with a pending deadline date of application';
                            }
                        }else{
                            $rcaANitro = true;
                            $remarks[]='rcaANitro without starting date of application';
                        }
                    } 
                    if($isrcaAPhos){
                        if(isset($rca->field_rcaapstartdate['und'])){
                            if(true === deadline_beforeorequal_to_referenceyear($rca->field_rcaapstartdate['und'][0]['value'], $node->field_anneedata['und'][0]['value'])){
                                $rcaAPhos = true;
                                 $remarks[]='rcaAPhos with an expired date of application';
                            }else{
                                $rcaAPhos = false;
                                $remarks[]='rcaAPhos with a pending deadline date of application';
                            }
                        }else{
                            $rcaAPhos = true;
                            $remarks[]='rcaAPhos without starting date of application';
                        }
                    } 
                    if($isrcab){
                        if(isset($rca->field_rcabstartdate['und'])){
                            if(true === deadline_beforeorequal_to_referenceyear($rca->field_rcabstartdate['und'][0]['value'], $node->field_anneedata['und'][0]['value'])){
                                $rcab = true;
                                 $remarks[]='rcab with an expired date of application';
                            }else{
                                $rcab = false;
                                $remarks[]='rcab with a pending deadline date of application';
                            }
                        }else{
                            $rcab = true;
                            $remarks[]='rcab without starting date of application';
                        }
                    } 
                    if($isrcac){
                        if(isset($rca->field_rcacstardate['und'])){
                            if(true === deadline_beforeorequal_to_referenceyear($rca->field_rcacstardate['und'][0]['value'], $node->field_anneedata['und'][0]['value'])){
                                $rcac = true;
                                $remarks[]='rcac with an expired date of application';
                            }else{
                                $rcac = false;
                                $remarks[]='rcac with a pending deadline date of application';
                            }
                        }else{
                            $rcac = true;
                            $remarks[]='rcac without starting date of application';
                        }
                    } 
                    if($hasParamN) $paramN = true;
                    if($hasParamP) $paramP = true;
                    if($hasParamO) $paramO = true;
                }
            }
            $dcps[] = array(
                'nid' => $dcp->nid,
                'title' => $dcp->title,
                'status'=>$dcp->field_status['und'][0]['value'],
                'waterbody' => $dcp->field_dcpwaterbodytype['und'][0]['value'],
                'rcatype' => $rcaTypeid,
                'rcazonetype' => $zonetypeIsSet,
                'rca54applied' => $rca54applied,
                'rcaan' => $rcaANitro,
                'rcaap' => $rcaAPhos,
                'rcab' => $rcab,
                'rcac' => $rcac,
                'paramN' => $paramN,
                'paramP' => $paramP,
                'paramO' => $paramO,
                'agglomerations'=>$agglos,
                'remarks'=>$remarks
            );
        }
    }

    uwwtd_trace('dcp and rca info use : ' . var_export($dcps, true));
    // If we cant find any discharge points attached to this station then we return no information to the user
    
    if($node->field_status['und'][0]['value'] == 1 && isset($node->field_uwwcollectingsystem['und']) && $node->field_uwwcollectingsystem['und'][0]['value'] == 'NOTCON'){
        $compliance_info->compliance = 'NR';
        $compliance_info->reason = "Station is not connected to collecting system";
        uwwtd_add_error($node->field_anneedata['und'][0]['value'], $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['conformity data'], 'Station  is active and it is not connected to collecting system.', $timestamp, null, $node);
        
        
        $required_treatment = new stdClass();
        $required_treatment->reason = '';
        $required_treatment->required = '';
        $required_treatment->required_rcaan = 0;
        $required_treatment->required_rcaap = 0;
        $required_treatment->required_rcab = 0;
        $required_treatment->required_rcac = 0;
        //if we don't have dcp, try to fill default information (very low chance of success)
        if(empty($dcps)){
            $dcps = [[
                'rcatype'=>$GLOBALS['uwwtd']['default_by_cc'][$conf['siif_eru_country_code'] ]['rcaType']
            ]];
        }
        
        uwwtd_get_required_treatment($required_treatment, $dcps, $maxgen);
        if(isset($required_treatment->required)){
            $compliance_info->required =$required_treatment->required;
        }else{
            $compliance_info->required = 'NI';
        }
        $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
        $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
        
        return $compliance_info;
    }

    if(count($dcps) == 0){
        $compliance_info->compliance = $GLOBALS['uwwtd_value']['compliance']['unknown'];
        $compliance_info->reason = 'Station found to have no information on discharge points';
        uwwtd_add_error($node->field_anneedata['und'][0]['value'], $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['conformity data'], 'Station found to have no information on discharge points.', $timestamp, null, $node);
        $compliance_info->required = 'NI';
        $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
        $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
        return $compliance_info;
    }

    //$dcps MUST not be empty
    //ALGO1
    $required_treatment = new stdClass();
    $required_treatment->reason = '';
    $required_treatment->required = '';
    $required_treatment->required_rcaan = 0;
    $required_treatment->required_rcaap = 0;
    $required_treatment->required_rcab = 0;
    $required_treatment->required_rcac = 0;

    uwwtd_get_required_treatment($required_treatment, $dcps, $maxgen);
    uwwtd_trace('result of uwwtd_get_required_treatment : ' . var_export($required_treatment, true));

    $compliance_info->required       = $required_treatment->required;
    $compliance_info->reason         = $required_treatment->reason."\n";
    $compliance_info->required_rcaan = $required_treatment->required_rcaan;
    $compliance_info->required_rcaap = $required_treatment->required_rcaap;
    $compliance_info->required_rcab  = $required_treatment->required_rcab;
    $compliance_info->required_rcac  = $required_treatment->required_rcac;
    unset($required_treatment);
    uwwtd_trace('$compliance_info become : ' . var_export($compliance_info, true));

    //same case as below for count($dcp), missing information on dcp
    if($compliance_info->required == 'NI' || $compliance_info->required == '?'){        
        $compliance_info->compliance = $GLOBALS['uwwtd_value']['compliance']['unknown'];
        uwwtd_add_error($node->field_anneedata['und'][0]['value'], $GLOBALS['uwwtd_errors_type']['type']['warning'], $GLOBALS['uwwtd_errors_type']['category']['conformity data'], $compliance_info->reason, $timestamp, null, $node);
        $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
        $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
        return $compliance_info;        
    }
    
    if($compliance_info->required == 'Appropriate' || $compliance_info->required == 'NR'){    
        //e.vincent / 2015/08/27 / ask from b.fribourg-blanc
        $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['not_calculable'];
        $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['not_calculable'];
        $compliance_info->compliance = 'NR';
        $compliance_info->reason .= 'Treatment found to be appropriate, treatment to implement and performance can therefore not be calculated';
    }         

    if($compliance_info->required == 'P'){
        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('primary'))) {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('tssperf', 'bod5perf'))) {
                $compliance_info->compliance = 'C';                            
                $compliance_info->reason .= 'Station found to be compliant because primary treatment is in place and performance is met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{   
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because primary treatment is in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('tssperf', 'bod5perf'))) {            
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because primary performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{                                    
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because primary treatment is not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
    }

    if($compliance_info->required == 'S'){      
        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary'))) {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf'))) {            
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because secondary treatment is in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{                            
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because secondary treatment is in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf'))) {            
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because secondary performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{                                     
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because secondary treatment is not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
    }

    //////////////////////////////////////////////
    //More stringent treatment
    //////////////////////////////////////////////
    //more stringent nitrogen rcab   
    if ($compliance_info->required_rcaan == 0
        && $compliance_info->required_rcaap == 0
        && $compliance_info->required_rcab == 1
        && $compliance_info->required_rcac == 0) {

        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'nremoval'))) {
        
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf'))) {            
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because nitrogen treatment is in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because nitrogen treatment is in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];            
            $compliance_info->compliance = 'NC';
            $compliance_info->reason .= 'Station found to be not compliant because nitrogen treatment is not in place';
            $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['false'];
        }  
    }

    //more stringent nitrogen rcaanitro
    if ($compliance_info->required_rcaan == 1
        && $compliance_info->required_rcaap == 0
        && $compliance_info->required_rcac == 0) {

        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'nremoval'))) {
        
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf'))) {            
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because nitrogen treatment is in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because nitrogen treatment is in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf'))) {                
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because nitrogen treatment is not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because nitrogen treatment is not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }        
    }
    
    //more stringent phosphorus rcaaPhos   
    if ($compliance_info->required_rcaan == 0
        && $compliance_info->required_rcaap == 1
        && $compliance_info->required_rcab == 0
        && $compliance_info->required_rcac == 0) {

        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'premoval'))) {
        
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ptotperf'))) {            
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus treatment is in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus treatment is in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ptotperf'))) {                 
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus treatment is not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus treatment is not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }        
    }
    
    //more stringent nitrogen and phosphorus rcaanitro + rcaapos   
    if ($compliance_info->required_rcaan == 1
        && $compliance_info->required_rcaap == 1
        && $compliance_info->required_rcac == 0) {

        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'premoval', 'nremoval'))) {
              
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf', 'ptotperf'))) {                         
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf', 'ptotperf'))) {                            
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }        
    }
    
    //more stringent other rcac
    if ($compliance_info->required_rcaan == 0
        && $compliance_info->required_rcaap == 0
        && $compliance_info->required_rcab == 0
        && $compliance_info->required_rcac == 1) {
        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary'))
            && true === uwwtd_atleastone_uwwtp_treatement_inplace_in($node, array('uv', 'chlorination', 'ozonation', 'sandfiltration', 'microfiltration', 'other'))) {                   
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf'))) {             
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because other treatment is in place and phosphorus and nitrogen performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because other treatment is in place but phosphorus and nitrogen performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            //dsm("uwwtd_all_uwwtp_treatement_inplace_in : not secondary");
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf'))) {               
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because other treatment is not in place but phosphorus and nitrogen performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because other treatment is not in place and phosphorus and nitrogen performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }        
    }
    
    //more stringent nitrogen and other rcab + rcac
    if ($compliance_info->required_rcaan == 0
        && $compliance_info->required_rcaap == 0
        && $compliance_info->required_rcab == 1
        && $compliance_info->required_rcac == 1) {
        
        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'nremoval'))
            && true === uwwtd_atleastone_uwwtp_treatement_inplace_in($node,
                            array('uv', 'chlorination', 'ozonation', 'sandfiltration', 'microfiltration', 'other')
                        )) {                   
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];
            
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf'))) {             
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because other treatment is in place and phosphorus and nitrogen performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because other treatment is in place but phosphorus and nitrogen performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
        else{
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];

            $compliance_info->compliance = 'NC';
            $compliance_info->reason .= 'Station found to be not compliant because other treatment is not in place and phosphorus and nitrogen performance is not met';
            $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
        }        
    }
    
    //more stringent phophorus and other rcaaphos + rcac
    if ($compliance_info->required_rcaan == 0
        && $compliance_info->required_rcaap == 1
        && $compliance_info->required_rcab == 0
        && $compliance_info->required_rcac == 1) {
        
        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'premoval'))
            && true === uwwtd_atleastone_uwwtp_treatement_inplace_in($node,
                            array('uv', 'chlorination', 'ozonation', 'sandfiltration', 'microfiltration', 'other')
                        )) {                   
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ptotperf'))) {                         
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        } else {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ptotperf'))) {                           
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }        
    }
    
    //more stringent nitrogen an dother rcaanitro +rcac   
    if ($compliance_info->required_rcaan == 1
        && $compliance_info->required_rcaap == 0
        && $compliance_info->required_rcac == 1) {

        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'nremoval'))
            && true === uwwtd_atleastone_uwwtp_treatement_inplace_in($node,
                            array('uv', 'chlorination', 'ozonation', 'sandfiltration', 'microfiltration', 'other')
                        )) {                   
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf'))) {                         
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        } else {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf'))) {                            
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            }
            else{
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }        
    }
    
    //more stringent nitrogen and phosphorus and other rcaanitro = rcaPhos + rcac   
    if ($compliance_info->required_rcaan == 1
        && $compliance_info->required_rcaap == 1
        && $compliance_info->required_rcac == 1) {
        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'nremoval', 'premoval')) &&
           true === uwwtd_atleastone_uwwtp_treatement_inplace_in($node, array('uv', 'chlorination', 'ozonation', 'sandfiltration', 'microfiltration', 'other'))) {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf', 'ptotperf'))) {
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            } else {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        } else {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ntotperf', 'ptotperf'))) {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            } else {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
    }


    //more stringent nitrogen and phosphorus  (rcaPhos + rcab)
    if ($compliance_info->required_rcaan == 0
        && $compliance_info->required_rcaap == 1
        && $compliance_info->required_rcab == 1
        && $compliance_info->required_rcac == 0) {

        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'nremoval', 'premoval'))) {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];

            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ptotperf'))) {
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus and nitrogen treatment are in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            } else {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        } 
        #Fix nd@oieau.fr : 25/01/2019 : change according v22 of algo
        else {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
            $compliance_info->compliance = 'NC';
            $compliance_info->reason .= 'Station found to be not compliant because phosphorus and nitrogen treatment are not in place but performance is always met in this case';
            $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['true'];
            /*
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf',  'ptotperf'))) {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason = 'Station found to be compliant because phosphorus and nitrogen treatment are not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            } else {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason = 'Station found to be not compliant because phosphorus and nitrogen treatment are not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
            */
        }
    }

    //more stringent nitrogen and phosphorus and other (rcaPhos + rcab + rcac)
    if ($compliance_info->required_rcaan == 0
        && $compliance_info->required_rcaap == 1
        && $compliance_info->required_rcab == 1
        && $compliance_info->required_rcac == 1) {
        if (true === uwwtd_all_uwwtp_treatement_inplace_in($node, array('secondary', 'nremoval', 'premoval')) &&
            true === uwwtd_atleastone_uwwtp_treatement_inplace_in($node, array('uv', 'chlorination', 'ozonation', 'sandfiltration', 'microfiltration', 'other'))) {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['true'];
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf', 'ptotperf'))) {
                $compliance_info->compliance = 'C';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus, nitrogen and other treatment are in place and performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            } else {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus, nitrogen and other treatment are in place but performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        } else {
            $compliance_info->treatment_met = $GLOBALS['uwwtd_value']['treatementmet']['false'];
            if (true === uwwtd_all_uwwtp_treatementperf_pass_in($node, array('codperf', 'bod5perf',  'ptotperf'))) {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be compliant because phosphorus, nitrogen or other treatment are not in place but performance is met';
                $compliance_info->performance_met =  $GLOBALS['uwwtd_value']['performancemet']['true'];
            } else {
                $compliance_info->compliance = 'NC';
                $compliance_info->reason .= 'Station found to be not compliant because phosphorus, nitrogen and other treatment are not in place and performance is not met';
                $compliance_info->performance_met = $GLOBALS['uwwtd_value']['performancemet']['false'];
            }
        }
    }

    return $compliance_info;
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

//Modif nd@oieau.fr le : 07/07/2015
//pas besoin de créer le noeud d'erreur, de le sauver puis de le modifier => on fait tout d'un coup
//encore moins besoin d'aller chercher le contenu complet du fichier de log pour ajouter une erreur => au niveau charge serveur c'est ce qu'il y a de pir.
//Il faudrait encore améliorer en envoyant un lot de message au lieu d'un à la fois

function uwwtd_check_sensitive($nid){
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
//  $repcode = $node->field_repcode['und'][0]['value'];
//  $repcode = str_replace('_', '-', $repcode);
//  $boom = explode('-', $repcode);
//  $boom = explode(' ', $boom[0]);
//  $rep = $boom[0];
    $rep = uwwtd_get_countrycode_from_repcode($node->field_repcode['und'][0]['value']);
    if($rep == 'SI' || $rep == 'si'){
        $sens = uwwtd_check_sensitive($node->nid);
        if(new DateTime($node->field_anneedata['und'][0]['value'].'-12'.'-31') <= new DateTime('2008-12-31')){
            if($node->field_agggenerated['und'][0]['value'] <= 10000){
                $art4comp = 'NR';
                $reason = 'Article 4 compliance found to be non relevant because the slovenian agglomeration data is before the 31/12/2008 and the agglomeration has a population equal or less than 10 000.';
                return array($art4comp, $reason);
            }
        }elseif(new DateTime($node->field_anneedata['und'][0]['value'].'-12'.'-31') <= new DateTime('2010-12-31')){
            if(($sens != 'CSA' && $sens != 'SA') && $node->field_agggenerated['und'][0]['value'] <= 15000){
                $art4comp = 'NR';
                $reason = 'Article 4 compliance found to be non relevant because the slovenian agglomeration data is before the 31/12/2010, the agglomeration is situation in a CSA or SA and the population is equal or less than 15 000.';
                return array($art4comp, $reason);
            }
        }else{
            $art4comp = 'NR';
            $reason = 'Article 4 compliance found to be non relevant because the slovenian agglomeration data is before the 31/12/2015.';
            return array($art4comp, $reason);
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

    if(!empty($haystack) && $needle!=''){
        foreach ($haystack as $item){
            if (($strict ? $item === $needle : $item == $needle) || (is_array($item) && uwwtd_in_array_r($needle, $item, $strict))){
                return true;
            }
        }
    }
    return false;
}

function uwwtd_zipped_csvs_to_array($files){
    uwwtd_trace(__FUNCTION__);
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

/**
 * @param   array    $dcps
 * @param   numeric  $maxgen    max pe generated among all agglo attach to the uwwt treated 
 * @return  string              require treatement (NI,NR, ...)
 *                              possible value : Appropriate, S, P, N, NP, MP, O
 *                                               NI if no case found in algorythm  
 */ 

function uwwtd_get_required_treatment(&$required_treatment, $dcps, $maxgen) {
	uwwtd_trace(__FUNCTION__);
    $required_treatment->reason = '';
    if ($maxgen < 2000) {
    	$required_treatment->required = 'Appropriate';
        return $required_treatment;
    }

    // Default :
    $required_treatment->required = 'X';

    //If more than one dcp, take the most contrianing SA > CSA > A58 > NA > LSA
    $typePriority = array(
        'A51523' => 1, //biggest priority
    	'A523' => 1, //biggest priority
        'SA'   => 1, 
        'CSA'  => 2,
        'A54'  => 3,
        'A5854'=> 3,
        'A5154'=> 3,
        'A58'  => 4,
        'A58523' => 4,
        'A54523' => 5,
        'A5854523' => 5,
        'NA'   => 6,
        'LSA'  => 7, //lower priority
    );
    foreach($dcps as $oneDcp){
        if (false === isset($dcp)) {
            $dcp = $oneDcp;
        }  
        //if priority of the current dcp is superior than the priority of the last dcp, we take it  
        if ($typePriority[ $oneDcp['rcatype'] ] < $typePriority[ $dcp['rcatype'] ]) {
            $dcp = $oneDcp;
        }
    }
    
    switch ($dcp['rcatype']) {
		case 'LSA':
            if ($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW') {
                if ($maxgen >= 150000) {
                        $required_treatment->required = 'S';
                }
                //FIx nd@oieau.fr : add distinction between case under 10000
                elseif ($maxgen >= 10000) { 
                    $required_treatment->required = 'P';
                }else {
                    $required_treatment->required = 'Appropriate';
                }
            } elseif ($dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'LF') {
                if ($maxgen >= 2000 && $maxgen <= 10000) {
                    $required_treatment->required = 'P';
                } else {
                    $required_treatment->required = 'S';
                }
            } else {
                // Problem
                $required_treatment->required = 'NI';
                $required_treatment->reason = 'Station found to be missing dcp waterbody information required to calculate proper compliance';
            }
            break;
        case 'NA':
            if ($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW') {
                if ($maxgen < 10000) {
                    $required_treatment->required = 'Appropriate';
                } else {
                    $required_treatment->required = 'S';
                }

            } elseif ($dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'FW' || $dcp['waterbody'] == 'LF') {
                $required_treatment->required = 'S';
            } else {
                // Problem
                $required_treatment->required = 'NI';
                $required_treatment->reason = 'Station found to be missing dcp waterbody information required to calculate proper compliance';
            }
            break;


        // If the discharge point is in a Sensitive Area or a Catchment Sensitive Area
        case 'SA':
        case 'CSA':
            if ($maxgen < 10000) {
        		if ($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW') {
                	$required_treatment->required = 'Appropriate';
                } else {
                    $required_treatment->required = 'S';
                }
            } else {
            	if (true === uwwtd_rca_art54_applies($dcp)) {
                    $required_treatment->required = 'S';
                } elseif ($dcp['rcatype'] == 'SA' || $dcp['rcatype'] == 'A58') {
        			uwwtd_get_required_treatment_sa_or_a58($required_treatment, $dcp, $maxgen);
                } else {
                    uwwtd_get_required_treatment_csa($required_treatment, $dcp, $maxgen);
                }
            }
            break;
        case 'A51523':
        case 'A523':
        case 'A58':
        case 'A54':
        case 'A5154':
        case 'A5854':
        case 'A58523':
        case 'A54523':
        case 'A5854523':
        case 'A5854523':
        	if ($maxgen < 10000) {
        		if ($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW') {
                	$required_treatment->required = 'Appropriate';
                } else {
                    $required_treatment->required = 'S';
                }
            } 
            else {
            	if ($dcp['rcatype'] == 'A54' || $dcp['rcatype'] == 'A5154' || $dcp['rcatype'] == 'A5854') {
            		if (true === uwwtd_rca_art54_applies($dcp)) {
                    	$required_treatment->required = 'S';
                        $required_treatment->reason = 'Treatment required is secondary according to A54 requirements';
                	}
            	} elseif ($dcp['rcatype'] == 'A58523' || $dcp['rcatype'] == 'A58') {
            		uwwtd_get_required_treatment_sa_or_a58($required_treatment, $dcp, $maxgen);
            	} elseif ($dcp['rcatype'] == 'A51523' || $dcp['rcatype'] == 'A523') {
            		if (isset($dcp['rcazonetype']) && $dcp['rcazonetype'] == 'UWWCMSA') {
            			uwwtd_get_required_treatment_csa($required_treatment, $dcp, $maxgen);
            		} else {
            			uwwtd_get_required_treatment_sa_or_a58($required_treatment, $dcp, $maxgen);
            		}
            	} else {
            		$required_treatment->required = '?';
            	}
            }
            break;
        default:
            $required_treatment->required = 'NI';
            $required_treatment->reason = 'Station found to be missing rcatype information required to calculate proper compliance';
            break;
    }
    // BB - 04/11 - Adding a step for 9th report format
    if ($dcp['rcazonetype'] === 'UWWTDLSA') {
        if ($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW') {
            if ($maxgen >= 150000) {
                /**
                 * Costa do Estoril agglomeration (aggID code: PTAGL070, uwwID: pt12058222) 
                 * has a derogation granted according to Article 5(8) of the Urban Waste Water Treatment Directive (Commission Decision 2001/720/EC of 8.10.2001, OJ L269 of 10.10.2001). 
                 * For this agglomerations it is required a less stringent treatment, ie treatment required=primary treatment.
                **/
                
                if( array_key_exists('PTAGL070', $dcp['agglomerations'])){
                    $required_treatment->required = 'P';
                    $required_treatment->reason ="Costa do Estoril agglomeration (aggID code: PTAGL070, uwwID: pt12058222) has a derogation granted according to Article 5(8) of the Urban Waste Water Treatment Directive (Commission Decision 2001/720/EC of 8.10.2001, OJ L269 of 10.10.2001).For this agglomerations it is required a less stringent treatment, ie treatment required=primary treatment.";
                }else{
                    $required_treatment->required = 'S';
                }
            }
            //FIx nd@oieau.fr : add distinction between case under 10000
            elseif ($maxgen >= 10000) { 
                $required_treatment->required = 'P';
            }else {
                $required_treatment->required = 'Appropriate';
            }
        } elseif ($dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'LF') {
            if ($maxgen >= 2000 && $maxgen < 10000) {
                $required_treatment->required = 'P';
            } else {
                $required_treatment->required = 'S';
            }
        } else {
            // Problem
            $required_treatment->required = 'NI';
            $required_treatment->reason = 'Station found to be missing dcp waterbody information required to calculate proper compliance';
        }
    }
}


/**
 * Return a list of all the uwwtp with there infirmation
 * 
 * @param   object   $node    a valid node of an agglomeration 
 * 
 * @return  array    
 *                      $result['total'] = numerical val =  sum of all the uww_agglo data field_agglo_uww_perc_ent_uw
 *                                                          for each uwwtp 
 *                      $result['listUww'][] = array(    
 *                           node  = uwwtp node
 *                           required  =  node value
 *                           bod5      =  node value
 *                           cod       =  node value
 *                           collecting =  node value
 *                           compB      =  node value
 *                           secondary  =  node  value
 *                           treatmenttype = node value 
 *                      )  
 */
function uwwtd_get_all_info_agguwww($node) 
{
    uwwtd_trace(__FUNCTION__);
    $aResult = array();
    $aResult['total']   = 0;

    $aResult['listUww'] = array();
    
    //nd@oieau.fr : 28/11/2016 : Why we need to test between field_agguwwliste and field_linked_treatment_plants in theory field_agguwwliste must be filled
    if(isset($node->field_agguwwliste['und'])){
        $field = 'field_agguwwliste';
    }
    elseif(isset($node->field_linked_treatment_plants['und'])){
        $field = 'field_linked_treatment_plants';
    }
    elseif($node->field_status['und'][0]['value']==1){
        drupal_set_message("There is no linked UWWTP define in field_agguwwliste or in field_linked_treatment_plants", "warning");
    }
    if (!empty($field) && isset($node->{$field}) && !empty($node->{$field}['und'])) {
        foreach($node->{$field}['und'] as $agguwwlist){
            $uww = node_load($agguwwlist['nid']);
            // we check if uwwtp is active (status = 1) and iscon (we check if value !=  notcon)
            if($uww->field_status['und'][0]['value']==1 && $uww->field_uwwcollectingsystem['und'][0]['value']!='NOTCON'){
                $aResult['listUww'][$uww->nid] = array(
                    'connected'=> 1,
                    'required' => $uww->field_uwwtreatmentrequired['und'][0]['value'],
                    'bod5' => $uww->field_uwwbod5perf['und'][0]['value'],
                    'cod' => $uww->field_uwwcodperf['und'][0]['value'],
                    'collecting' => $uww->field_uwwloadenteringuwwtp['und'][0]['value'],
                    'compB' => $uww->field_uwwcompliance['und'][0]['value'],
                    'secondary' => $uww->field_uwwsecondarytreatment['und'][0]['value'],
                    'treatmenttype' => $uww->field_uwwtreatmenttype['und'][0]['value'],
                );

                // get others information from node that collected joint information
                $query = db_select('node', 'n');
                $query->join('field_data_field_agglo_uww_agglo', 'a', 'a.entity_id = n.nid');
                $query->join('field_data_field_agglo_uww_uww', 'u', 'u.entity_id = n.nid');
                $query->fields('n',array('title','created'))
                    ->condition('a.field_agglo_uww_agglo_nid', $node->nid, '=')
                    ->condition('u.field_agglo_uww_uww_nid', $uww->nid, '=')
                    ->fields('n',array('nid'));

                $result = $query->execute();
                $aResult['listUww'][$uww->nid]['agglo_uww_perc_ent_uw'] = 0;
                while($record = $result->fetchAssoc()){
                    $agguww = node_load($record['nid']);
                    $uwwperccollec = $agguww->field_agglo_uww_perc_ent_uw['und'][0]['value'];
                    //$uwwquantcollec = ($quantLoadCollecting * $uwwperccollec) / 100;
                    $aResult['total'] += $uwwperccollec;
                    $aResult['listUww'][$uww->nid]['agglo_uww_perc_ent_uw'] = $agguww->field_agglo_uww_perc_ent_uw['und'][0]['value'];
                }
                //Fix nd@oieau.fr 07/12/2020 : don't reset "rca" values if values are already set
                if(!isset($aResult['rca'])){
                    $aResult['rca']['art54applies'] = false;
                    $aResult['rca']['rca_n_incoming_measured']     = 0;
                    $aResult['rca']['rca_n_incoming_calculated']   = 0;
                    $aResult['rca']['rca_n_incoming_estimated']    = 0;
                    $aResult['rca']['rca_n_discharged_measured']   = 0;
                    $aResult['rca']['rca_n_discharged_calculated'] = 0;
                    $aResult['rca']['rca_n_discharged_estimated']  = 0;

                    $aResult['rca']['rca_p_incoming_measured']     = 0;
                    $aResult['rca']['rca_p_incoming_calculated']   = 0;
                    $aResult['rca']['rca_p_incoming_estimated']    = 0;
                    $aResult['rca']['rca_p_discharged_measured']   = 0;
                    $aResult['rca']['rca_p_discharged_calculated'] = 0;
                    $aResult['rca']['rca_p_discharged_estimated']  = 0;
                }
                foreach($uww->field_uwwdcpliste['und'] as $dcplist){
                    $dcp = node_load($dcplist['nid']);
                    if($dcp->field_status['und'][0]['value']==1){
                        /* Cas des pays avec l'ensemble du pays en zone sensible */
                        $rcaTypeid = $dcp->field_rcatype['und'][0]['value'];
                        // if('LT' == uwwtd_get_countrycode_from_repcode($node->field_repcode['und'][0]['value'])){
                        //     $rcaTypeid = 'SA';
                        // }
                        if(isset($dcp->field_dcprcaliste['und'])){
                            foreach($dcp->field_dcprcaliste['und'] as $rcalist){
                                $rca = node_load($rcalist['nid']);
                                $isrca54applied = (isset($rca->field_rca54applied['und'][0]['value']) ? $rca->field_rca54applied['und'][0]['value'] == 1 : false);
                                if (uwwtd_rca_art54_applies(array('rcatype' => $rcaTypeid, 'rca54applied' => $isrca54applied))) {
                                    $aResult['rca']['art54applies'] = true;
                                    $aResult['rca']['rca_n_incoming_measured']     = $rca->field_rcanincomingmeasured['und'][0]['value'];
                                    $aResult['rca']['rca_n_incoming_calculated']   = $rca->field_rcanincomingcalculated['und'][0]['value'];
                                    $aResult['rca']['rca_n_incoming_estimated']    = $rca->field_rcanincomingestimated['und'][0]['value'];
                                    $aResult['rca']['rca_n_discharged_measured']   = $rca->field_rcandischargedmeasured['und'][0]['value'];
                                    $aResult['rca']['rca_n_discharged_calculated'] = $rca->field_rcandischargedcalculated['und'][0]['value'];
                                    $aResult['rca']['rca_n_discharged_estimated']  = $rca->field_rcandischargedestimated['und'][0]['value'];

                                    $aResult['rca']['rca_p_incoming_measured']     = $rca->field_rcapincomingmeasured['und'][0]['value'];
                                    $aResult['rca']['rca_p_incoming_calculated']   = $rca->field_rcapincomingcalculated['und'][0]['value'];
                                    $aResult['rca']['rca_p_incoming_estimated']    = $rca->field_rcapincomingestimated['und'][0]['value'];
                                    $aResult['rca']['rca_p_discharged_measured']   = $rca->field_rcapdischargedmeasured['und'][0]['value'];
                                    $aResult['rca']['rca_p_discharged_calculated'] = $rca->field_rcapdischargedcalculated['und'][0]['value'];
                                    $aResult['rca']['rca_p_discharged_estimated']  = $rca->field_rcapdischargedestimated['und'][0]['value'];
                                    break 2;
                                }
                                unset($rca);
                            }
                        }
                        unset($dcp);
                    }
                }
                $aResult['listUww'][$uww->nid]['node'] = $uww;
            }
            elseif($uww->field_status['und'][0]['value']==1 && $uww->field_uwwcollectingsystem['und'][0]['value']=='NOTCON'){
                $aResult['listUww'][$uww->nid] = array(
                    'connected'=> 0,
                    'treatmenttype'=> 'NI'
                );
                // get others information from node that collected joint information
                $query = db_select('node', 'n');
                $query->join('field_data_field_agglo_uww_agglo', 'a', 'a.entity_id = n.nid');
                $query->join('field_data_field_agglo_uww_uww', 'u', 'u.entity_id = n.nid');
                $query->fields('n',array('title','created'))
                    ->condition('a.field_agglo_uww_agglo_nid', $node->nid, '=')
                    ->condition('u.field_agglo_uww_uww_nid', $uww->nid, '=')
                    ->fields('n',array('nid'));
                
                $result = $query->execute();
                $aResult['listUww'][$uww->nid]['agglo_uww_perc_ent_uw'] = 0;
                while($record = $result->fetchAssoc()){
                    $agguww = node_load($record['nid']);
                    $uwwperccollec = $agguww->field_agglo_uww_perc_ent_uw['und'][0]['value'];
                    //$uwwquantcollec = ($quantLoadCollecting * $uwwperccollec) / 100;
                    $aResult['total'] += $uwwperccollec;
                    $aResult['listUww'][$uww->nid]['agglo_uww_perc_ent_uw'] = $agguww->field_agglo_uww_perc_ent_uw['und'][0]['value'];
                    $aResult['listUww'][$uww->nid]['required'] =$uww->field_uwwtreatmentrequired['und'][0]['value'];
                    /*
                    if($node->field_agggenerated['und'][0]['value'] * $node->field_aggc1['und'][0]['value'] * $agguww->field_agglo_uww_perc_ent_uw['und'][0]['value'] > 10000){
                        $aResult['listUww'][$uww->nid]['required'] = 'S';
                    }else{
                        $aResult['listUww'][$uww->nid]['required'] = 'P';
                    }*/
                }
                $aResult['listUww'][$uww->nid]['node'] = $uww;
            }
            
            unset($uww);
        }
    }    
    return $aResult;
}

/**
 * Check if at least one uwwtp compliance value are in a list of autorized value
 * return true at the first uwwtp which is in  $authorizeValue
 *  
 * @param   array    $authorizeValue    List of authorize values
 * @param   array    $infoAllAggUww     List of all uwwtp with a key compB with the value "compliance" 
 * 
 * @return  boolean                     if at least one of the uwwtp compliance value are in the $authorizeValue, true is return   
 */        
function uwwtd_atleastone_uwwtp_complianceart_in($article, $authorizeValue, $infoAllAggUww) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = false;

    foreach($infoAllAggUww as $aUwwtp) {
        if (true === in_array($aUwwtp[$article], $authorizeValue)) {
            $bResult = true;
            break;
        }
    }
    
    return $bResult;    
}

function uwwtd_all_uwwtp_complianceart_in($article, $authorizeValue, $infoAllAggUww) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = true;

    foreach($infoAllAggUww as $aUwwtp) {
        if (false === in_array($aUwwtp[$article], $authorizeValue)) {
            $bResult = false;
            break;
        }
    }
    
    return $bResult;    
}

/**
 * Check if at least one uwwtp compliance value are in a list of autorized value
 * return true at the first uwwtp which is in  $authorizeValue
 *  
 * @param   array    $authorizeValue    List of authorize values
 * @param   array    $infoAllAggUww     List of all uwwtp with a key compB with the value "compliance" 
 * 
 * @return  boolean                     if at least one of the uwwtp compliance value are in the $authorizeValue, true is return   
 */        
function uwwtd_atleastone_uwwtp_compliance_in($authorizeValue, &$infoAllAggUww) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = false;
    
    foreach($infoAllAggUww['listUww'] as $aUwwtp) {
        if (true === in_array($aUwwtp['compB'], $authorizeValue)) {
            $bResult = true;
            break;
        }
    }
    
    return $bResult;    
}

/**
 * Check if all uwwtp compliance value are in a list of autorized value
 * return false at the first uwwtp which is not in  $authorizeValue
 * 
 * @param   array    $authorizeValue    List of authorize values
 * @param   array    $infoAllAggUww     List of all uwwtp with a key compB with the value "compliance" 
 * 
 * @return  boolean                     if all the uwwtp compliance value are in the $authorizeValue, true is return   
 */         
function uwwtd_all_uwwtp_compliance_in($authorizeValue, &$infoAllAggUww) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = true;
    
    foreach($infoAllAggUww['listUww'] as $aUwwtp) {
        if (false === in_array($aUwwtp['compB'], $authorizeValue)) {
            $bResult = false;
            break;
        }
    }
    
    return $bResult;    
}

/**
 * Check if all uwwtp required value are in a list of autorized value
 * return false at the first uwwtp which is not in  $authorizeValue
 * 
 * @param   array    $authorizeValue    List of authorize values
 * @param   array    $infoAllAggUww     List of all uwwtp with a key required with the value "required" 
 * 
 * @return  boolean                     if all the uwwtp required value are in the $authorizeValue, true is return   
 */         
function uwwtd_all_uwwtp_required_in($authorizeValue, &$infoAllAggUww) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = true;
    
    foreach($infoAllAggUww['listUww'] as $aUwwtp) {
        if (false === in_array($aUwwtp['required'], $authorizeValue)) {
            $bResult = false;
            break;
        }
    }
    
    return $bResult;    
}


/**
 * Check if all uwwtp required value are in a list of autorized value
 * return false at the first uwwtp which is in  $unauthorizeValue
 *  
 * @param   array    $unauthorizeValue    List of unauthorize values
 * @param   array    $infoAllAggUww     List of all uwwtp with a key required with the value "required" 
 * 
 * @return  boolean                     if all the uwwtp required value are not in the $unauthorizeValue, true is return   
 */         
function uwwtd_all_uwwtp_required_notin($unauthorizeValue, &$infoAllAggUww) 
{   
    uwwtd_trace(__FUNCTION__);
    $bResult = true;
    
    foreach($infoAllAggUww['listUww'] as $aUwwtp) {
        if (true === in_array($aUwwtp['required'], $unauthorizeValue)) {
            $bResult = false;
            break;
        }
    }
    
    return $bResult;    
}


/**
 * Check if at least one uwwtp required value are in a list of autorized value
 * return true at the first uwwtp which is in  $authorizeValue
 *  
 * @param   array    $authorizeValue    List of authorize values
 * @param   array    $infoAllAggUww     List of all uwwtp with a key required with the value "required" 
 * 
 * @return  boolean                     if at least one of the uwwtp required value are in the $authorizeValue, true is return   
 */         
function uwwtd_atleastone_uwwtp_required_in($authorizeValue, &$infoAllAggUww) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = false;
    foreach($infoAllAggUww['listUww'] as $aUwwtp) {
        if (true === in_array($aUwwtp['required'], $authorizeValue)) {
            $bResult = true;
            break;
        }
    }
    
    return $bResult;    
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

/**
 * Return country code (lt, LT, cy, cyc,...) ifrom the repcode
 */             
function uwwtd_get_countrycode_from_repcode($repcode)
{
    uwwtd_trace(__FUNCTION__);
    $repcode = str_replace('_', '-', $repcode);
    $boom = explode('-', $repcode);
    $boom = explode(' ', $boom[0]);
    $rep = $boom[0];
    return $rep; 
}  


function uwwtd_all_uwwtp_treatement_inplace_in($uwwtp, $listtreatement) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = true;
    foreach($listtreatement as $treatement) {
        switch ($treatement) {
            case 'primary':
                if(!isset($uwwtp->field_uwwprimarytreatment['und'][0]['value']) || $uwwtp->field_uwwprimarytreatment['und'][0]['value'] != '1') {
                    return false;
                }
                break;
            case 'secondary':
                if(!isset($uwwtp->field_uwwsecondarytreatment['und'][0]['value']) || $uwwtp->field_uwwsecondarytreatment['und'][0]['value'] != '1'){
                    return false;
                }
                break; 
            case 'premoval':
                if(!isset($uwwtp->field_uwwpremoval['und'][0]['value']) || $uwwtp->field_uwwpremoval['und'][0]['value'] != '1'){
                    return false;
                }    
                break;          
            case 'nremoval':
                if(!isset($uwwtp->field_uwwnremoval['und'][0]['value']) || $uwwtp->field_uwwnremoval['und'][0]['value'] != '1'){
                    return false;
                }
                break; 
            case 'uv':
                if(!isset($uwwtp->field_uwwuv['und'][0]['value']) || $uwwtp->field_uwwuv['und'][0]['value'] != '1'){
                    return false;
                }
                break; 
            case 'chlorination':
                if(!isset($uwwtp->field_uwwchlorination['und'][0]['value']) || $uwwtp->field_uwwchlorination['und'][0]['value'] != '1'){
                    return false;
                }
                break; 
            case 'ozonation':
                if(!isset($uwwtp->field_uwwozonation['und'][0]['value']) || $uwwtp->field_uwwozonation['und'][0]['value'] != '1'){
                    return false;
                }
                break; 
            case 'sandfiltration':
                if(!isset($uwwtp->field_uwwsandfiltration['und'][0]['value']) || $uwwtp->field_uwwsandfiltration['und'][0]['value'] != '1'){
                    return false;
                }
                break; 
            case 'microfiltration':
                if(!isset($uwwtp->field_uwwmicrofiltration['und'][0]['value']) || $uwwtp->field_uwwmicrofiltration['und'][0]['value'] != '1'){
                    return false;
                }
                break; 
            case 'other':
                if(!isset($uwwtp->field_uwwothertreat['und'][0]['value']) || $uwwtp->field_uwwothertreat['und'][0]['value'] != '1'){
                    return false;
                }
                break;                                                                                                                                              
            default:
                return false;                   
        }
    }
    return $bResult;    
}
              
function uwwtd_atleastone_uwwtp_treatement_inplace_in($uwwtp, $listtreatement) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = false;
    
    foreach($listtreatement as $treatement) {
        switch ($treatement) {
            case 'primary':
                if(isset($uwwtp->field_uwwprimarytreatment['und'][0]['value']) && $uwwtp->field_uwwprimarytreatment['und'][0]['value'] == '1') {
                    return true;
                }
                break;
            case 'secondary':
                if(isset($uwwtp->field_uwwsecondarytreatment['und'][0]['value']) && $uwwtp->field_uwwsecondarytreatment['und'][0]['value'] == '1'){
                    return true;
                }
                break; 
            case 'premoval':
                if(isset($uwwtp->field_uwwpremoval['und'][0]['value']) && $uwwtp->field_uwwpremoval['und'][0]['value'] == '1'){
                    return true;
                }    
                break;          
            case 'nremoval':
                if(isset($uwwtp->field_uwwnremoval['und'][0]['value']) && $uwwtp->field_uwwnremoval['und'][0]['value'] == '1'){
                    return true;
                }
                break; 
            case 'uv':
                if(isset($uwwtp->field_uwwuv['und'][0]['value']) && $uwwtp->field_uwwuv['und'][0]['value'] == '1'){
                    return true;
                }
                break; 
            case 'chlorination':
                if(isset($uwwtp->field_uwwchlorination['und'][0]['value']) && $uwwtp->field_uwwchlorination['und'][0]['value'] == '1'){
                    return true;
                }
                break; 
            case 'ozonation':
                if(isset($uwwtp->field_uwwozonation['und'][0]['value']) && $uwwtp->field_uwwozonation['und'][0]['value'] == '1'){
                    return true;
                }
                break; 
            case 'sandfiltration':
                if(isset($uwwtp->field_uwwsandfiltration['und'][0]['value']) && $uwwtp->field_uwwsandfiltration['und'][0]['value'] == '1'){
                    return true;
                }
                break; 
            case 'microfiltration':
                if(isset($uwwtp->field_uwwmicrofiltration['und'][0]['value']) && $uwwtp->field_uwwmicrofiltration['und'][0]['value'] == '1'){
                    return true;
                }
                break; 
            case 'other':
                if(isset($uwwtp->field_uwwothertreat['und'][0]['value']) && $uwwtp->field_uwwothertreat['und'][0]['value'] == '1'){
                    return true;
                }
                break;                                                                                                                                              
            default:
                ;               
        }
    }
    
    return $bResult;    
}

function uwwtd_all_uwwtp_treatementperf_pass_in($uwwtp, $listperf) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = true;
    if(in_array($uwwtp->field_inspireidlocalid['und'][0]['value'], array('BERW0000601','BERW5401001','BERW0000201','BERW0000104'))){
        return true;
    }

    foreach($listperf as $perf) {
        switch($perf) {
            case 'codperf':
                if (!isset($uwwtp->field_uwwcodperf['und'][0]['value']) || $uwwtp->field_uwwcodperf['und'][0]['value'] != 'P') {
                    return false;
                }
                break;
            case 'bod5perf':
                if (!isset($uwwtp->field_uwwbod5perf['und'][0]['value']) || $uwwtp->field_uwwbod5perf['und'][0]['value'] != 'P') {
                    return false;
                }
                break; 
            case 'tssperf':
                if (!isset($uwwtp->field_uwwtssperf['und'][0]['value']) || $uwwtp->field_uwwtssperf['und'][0]['value'] != 'P') {
                    return false;
                }
                break;
            case 'ntotperf':
                if (!isset($uwwtp->field_uwwntotperf['und'][0]['value']) || $uwwtp->field_uwwntotperf['und'][0]['value'] != 'P') {
                    return false;
                }
                break;
            case 'ptotperf':
                if (!isset($uwwtp->field_uwwptotperf['und'][0]['value']) || $uwwtp->field_uwwptotperf['und'][0]['value'] != 'P') {
                    return false;
                }
                break;  
            case 'otherperf':
                if (!isset($uwwtp->field_uwwotherperf['und'][0]['value']) || $uwwtp->field_uwwotherperf['und'][0]['value'] != 'P') {
                    return false;
                }
                break;     
            default:
                return false;                
        }
    }
    
    return $bResult;    
}

/**
 *
 */ 
function uwwtd_all_uwwtp_treatementperf_pass_or_nr_in($uwwtp, $listperf) 
{
    uwwtd_trace(__FUNCTION__);
    $bResult = true;

    foreach($listperf as $perf) {
        switch($perf) {
            case 'codperf':
                if (isset($uwwtp->field_uwwcodperf['und'][0]['value']) 
                    && $uwwtp->field_uwwcodperf['und'][0]['value'] != 'P'
                    && $uwwtp->field_uwwcodperf['und'][0]['value'] != 'NR') {
                    return false;
                }
                break;
            case 'bod5perf':
                if (isset($uwwtp->field_uwwbod5perf['und'][0]['value']) 
                    && $uwwtp->field_uwwbod5perf['und'][0]['value'] != 'P'
                    && $uwwtp->field_uwwbod5perf['und'][0]['value'] != 'NR') {
                    return false;
                }
                break; 
            case 'tssperf':
                if (isset($uwwtp->field_uwwtssperf['und'][0]['value']) 
                    && $uwwtp->field_uwwtssperf['und'][0]['value'] != 'P'
                    && $uwwtp->field_uwwtssperf['und'][0]['value'] != 'NR') {
                    return false;
                }
                break;
            case 'ntotperf':
                if (isset($uwwtp->field_uwwntotperf['und'][0]['value']) 
                    && $uwwtp->field_uwwntotperf['und'][0]['value'] != 'P'
                    && $uwwtp->field_uwwntotperf['und'][0]['value'] != 'NR') {
                    return false;
                }
                break;
            case 'ptotperf':
                if (isset($uwwtp->field_uwwptotperf['und'][0]['value']) 
                    && $uwwtp->field_uwwptotperf['und'][0]['value'] != 'P'
                    && $uwwtp->field_uwwptotperf['und'][0]['value'] != 'NR') {
                    return false;
                }
                break;  
            case 'otherperf':
                if (isset($uwwtp->field_uwwotherperf['und'][0]['value']) 
                    && $uwwtp->field_uwwotherperf['und'][0]['value'] != 'P'
                    && $uwwtp->field_uwwotherperf['und'][0]['value'] != 'NR') {
                    return false;
                }
                break;     
            default:
                return false;                
        }
    }
    
    return $bResult;    
}

/**
 *
 */ 
function uwwtd_rca_art54_applies($dcp) 
{
    uwwtd_trace(__FUNCTION__);
    uwwtd_trace('rcatype = ' . $dcp['rcatype']);
    uwwtd_trace('rca54applied = ' . ($dcp['rca54applied'] === true ? 'true' : 'false'));
    $result = (
        ($dcp['rcatype'] == 'A54' && $dcp['rca54applied'] === true) ||
        ($dcp['rcatype'] == 'SA' && $dcp['rca54applied'] === true) ||
        ($dcp['rcatype'] == 'A58' && $dcp['rca54applied'] === true) ||
        ($dcp['rcatype'] == 'A5854' && $dcp['rca54applied'] === true) ||
        ($dcp['rcatype'] == 'CSA' && $dcp['rca54applied'] === true) ||
        ($dcp['rcatype'] == 'A5154' && $dcp['rca54applied'] === true) ||
        ($dcp['rcatype'] == 'NA' && $dcp['rca54applied'] === true) ||   //nd 2019/06/14 case of HU
        ($dcp['rcatype'] == 'A54523' && $dcp['rca54applied'] === true) ||   //e.v 2018/11/15 mail b.db of 09/11/2018 à 17:26
        ($dcp['rcatype'] == 'A5854523' && $dcp['rca54applied'] === true)    //e.v 2018/11/15 mail b.db of 09/11/2018 à 17:26
        
    );
    uwwtd_trace('uwwtd_rca_art54_applies = ' . ($result === true ? 'true' : 'false'));
    return $result;    
}

/**
 *
 */
function uwwtd_get_required_treatment_sa_or_a58(&$required_treatment, $dcp, $maxgen)
{
    uwwtd_trace(__FUNCTION__);
    // dsm(func_get_args());
    if(($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW') && $maxgen <= 10000){
        $required_treatment->required = 'Appropriate';
    } elseif (($dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'FW' || $dcp['waterbody'] == 'LF') && $maxgen <= 10000 ) {
        $required_treatment->required = 'S';
    } elseif ($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW' || $dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'FW' || $dcp['waterbody'] == 'LF') {
        if($dcp['rcatype'] == 'SA' || $dcp['rcatype'] == 'A51523' || $dcp['rcatype'] == 'A523') {
            //if rca demands nitrogen rcaANitro
            if ($dcp['rcaan']) {
                //if rca demands phosphorus rcaAPhos
                if ($dcp['rcaap']) {
                    //if rca demands Other rcac
                    if ($dcp['rcac']) {
                        $required_treatment->required = 'NPO';  
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 1;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 1;
                        $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);                        
                    } else {
                        $required_treatment->required = 'NP';
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 1;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 0; 
                        $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);                        
                    }                 
                } else {
                    //if rca demands Other rcac
                    if ($dcp['rcac']) {
                        $required_treatment->required = 'NO';   
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 0;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 1; 
                        $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);                        
                    } else {
                        $required_treatment->required = 'N';
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 0;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 0;       
                        $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);
                    }             
                }              
            } else {
                //if rca demands nitrogen rcab
                if ($dcp['rcab']) {
                    //if rca demands Other rcac
                    if ($dcp['rcac']) {
                        $required_treatment->required = 'NO';  
                        $required_treatment->required_rcaan = 0;
                        $required_treatment->required_rcaap = 0;
                        $required_treatment->required_rcab = 1;
                        $required_treatment->required_rcac = 1;
                            $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);
                    } else {
                        //if rca demands phosphorus rcaAPhos
                        if ($dcp['rcaap']) {
                            $required_treatment->required = 'NP'; 
                            $required_treatment->required_rcaan = 0;
                            $required_treatment->required_rcaap = 1;
                            $required_treatment->required_rcab = 1;
                            $required_treatment->required_rcac = 0;
                             $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);
                        } else {
                            $required_treatment->required = 'N';
                            $required_treatment->required_rcaan = 0;
                            $required_treatment->required_rcaap = 0;
                            $required_treatment->required_rcab = 1;
                            $required_treatment->required_rcac = 0;   
                            $required_treatment->reason = '------'.implode(", ", $dcp['remarks']); 
                        }              
                    }
                } else {
                    //if rca demands phosphorus rcaAPhos
                    if ($dcp['rcaap']) {
                        //if rca demands Other rcac
                        if ($dcp['rcac']) {
                            $required_treatment->required = 'PO';
                            $required_treatment->required_rcaan = 0;
                            $required_treatment->required_rcaap = 1;
                            $required_treatment->required_rcab = 0;
                            $required_treatment->required_rcac = 1;
                            $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);
                        } else {
                            $required_treatment->required = 'MP';
                            $required_treatment->required_rcaan = 0;
                            $required_treatment->required_rcaap = 1;
                            $required_treatment->required_rcab = 0;
                            $required_treatment->required_rcac = 0;
                            $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);                            
                        }                       
                    } else {
                        if ($dcp['rcac']) {
                            $required_treatment->required = 'O';
                            $required_treatment->required_rcaan = 0;
                            $required_treatment->required_rcaap = 0;
                            $required_treatment->required_rcab = 0;
                            $required_treatment->required_rcac = 1;
                            $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);
                        }  
                        else {
                            //$maxgen <= 10000
                            //$required_treatment->required = '?';
                            $required_treatment->required = 'S';
                            $required_treatment->required_rcaan = 0;
                            $required_treatment->required_rcaap = 0;
                            $required_treatment->required_rcab = 0;
                            $required_treatment->required_rcac = 0;
                            $required_treatment->reason = '------'.implode(", ", $dcp['remarks']);
                        }                      
                    }  
                }            
            }
        } 
        elseif($dcp['rcatype'] == 'A58' || $dcp['rcatype'] == 'A58523') {
            if($dcp['paramN']) {
                if($dcp['paramP']) {
                    if ($dcp['paramO']) {
                        $required_treatment->required = 'NPO';  
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 1;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 1;
                    } else {
                        $required_treatment->required = 'NP';  
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 1;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 0;     
                    }
                } else {
                    if ($dcp['paramO']) {
                        $required_treatment->required = 'NO';  
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 0;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 1;
                    } else {
                        $required_treatment->required = 'N';  
                        $required_treatment->required_rcaan = 1;
                        $required_treatment->required_rcaap = 0;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 0;     
                    }
                }
            } else {
                if($dcp['paramP']) {
                    if ($dcp['paramO']) {
                        $required_treatment->required = 'PO';  
                        $required_treatment->required_rcaan = 0;
                        $required_treatment->required_rcaap = 1;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 1;
                    } else {
                        $required_treatment->required = 'MP';
                        $required_treatment->required_rcaan = 0;
                        $required_treatment->required_rcaap = 1;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 0;     
                    }
                } else {
                    if ($dcp['paramO']) {
                        $required_treatment->required = 'O';  
                        $required_treatment->required_rcaan = 0;
                        $required_treatment->required_rcaap = 0;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 1;
                    } else {
                        $required_treatment->required = '?';  
                        $required_treatment->required_rcaan = 0;
                        $required_treatment->required_rcaap = 0;
                        $required_treatment->required_rcab = 0;
                        $required_treatment->required_rcac = 0;     
                    }
                }
            }
        }   
    } else {
        $required_treatment->required = 'NI';
        $required_treatment->reason = 'Station found to be missing dcp waterbody information required to calculate proper compliance';    
    }            
}  

/**
 *
 */
function uwwtd_get_required_treatment_csa(&$required_treatment, $dcp, $maxgen)
{
    uwwtd_trace(__FUNCTION__);
    if(($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW') && $maxgen <= 10000){
        $required_treatment->required = 'Appropriate';
    } elseif (($dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'FW' || $dcp['waterbody'] == 'LF') && $maxgen <= 10000 ) {
        $required_treatment->required = 'S';
    } elseif ($dcp['waterbody'] == 'LC' || $dcp['waterbody'] == 'CW' || $dcp['waterbody'] == 'ES' || $dcp['waterbody'] == 'FW' || $dcp['waterbody'] == 'LF') {
        //if rca demands nitrogen rcaANitro
        if ($dcp['rcaan']) {
            //if rca demands phosphorus rcaAPhos
            if ($dcp['rcaap']) {
                $required_treatment->required = 'NP';
                $required_treatment->required_rcaan = 1;
                $required_treatment->required_rcaap = 1;
                $required_treatment->required_rcab = 0;
                $required_treatment->required_rcac = 0;                        
            } else {
                $required_treatment->required = 'N';
                $required_treatment->required_rcaan = 1;
                $required_treatment->required_rcaap = 0;
                $required_treatment->required_rcab = 0;
                $required_treatment->required_rcac = 0;                        
            }              
        } else {
            //if rca demands nitrogen rcab
            if ($dcp['rcab']) {        //if rca demands phosphorus rcaAPhos
                if ($dcp['rcaap']) {
                    $required_treatment->required = 'NP'; 
                    $required_treatment->required_rcaan = 0;
                    $required_treatment->required_rcaap = 1;
                    $required_treatment->required_rcab = 1;
                    $required_treatment->required_rcac = 0;                               
                } else {
                    $required_treatment->required = 'N';
                    $required_treatment->required_rcaan = 0;
                    $required_treatment->required_rcaap = 0;
                    $required_treatment->required_rcab = 1;
                    $required_treatment->required_rcac = 0;                            
                }   
            } else {
                //if rca demands phosphorus rcaAPhos
                // Nota: The case where receiving area demands criterion c (other treatment) is not considered/listed here, because according to the document "UWWTD legal compliance assessment methodology document - Final version from 20 June 2014" (page 10) compliance with Article 5 under criterion c is only assessed for SA, but not for CSA.  Hence if CSA demands criterion c only, the required treatment is only secondary
                if ($dcp['rcaap']) {  
                    $required_treatment->required = 'MP';
                    $required_treatment->required_rcaan = 0;
                    $required_treatment->required_rcaap = 1;
                    $required_treatment->required_rcab = 0;
                    $required_treatment->required_rcac = 0;                                                 
                } else {
                    $required_treatment->required = 'S';
                    $required_treatment->required_rcaan = 0;
                    $required_treatment->required_rcaap = 0;
                    $required_treatment->required_rcab = 0;
                    $required_treatment->required_rcac = 0;                        
                }  
            }            
        }
    } else {
        $required_treatment->required = 'NI';
        $required_treatment->reason = 'Station found to be missing dcp waterbody information required to calculate proper compliance';    
    }     
}
