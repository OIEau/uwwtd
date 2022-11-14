<?php 

function uwwtd_debug(){
    
    //replay a data file import
    require_once(drupal_get_path('module', 'uwwtd') . '/inc/data_center.inc');
    
    //Get the last imported file
    $sourcefiles = uwwtd_get_all_sourcefile();
    $fid = array_shift($sourcefiles);
    $file = file_load($fid);
    $datas = uwwtd_xml_to_array($file->uri);
    
    $timestamp = strtotime('now');
    $annee = $datas['ReportPeriod']['Row']['repReportedPeriod'];
    $repCode = '';
    if(isset($datas['MSLevel']['Row']['repCode'])){
        $repCode =  uwwtd_check_text($datas['MSLevel']['Row']['repCode']);
    }
    
    //Replay import for RCA
    if(isset($datas['ReceivingAreas_SAMain'])){
        if(!isset($datas['ReceivingAreas_SAMain']['Row']['rcaCode'])){
            foreach($datas['ReceivingAreas_SAMain']['Row'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        //nd@oieau.fr 17/07/2018 : check how this tag appear in $datas when we have only one item
        else{
            foreach($datas['ReceivingAreas_SAMain'] as $rca){
                $imports[$rca['rcaCode']] = $rca;
            }
        }
        $subkeys = array(
            'ReceivingAreas_SAParameter'=>'Row',
            'ReceivingAreas_SA54'=>'Row',
            'ReceivingAreas_SASA'=>'Row',
            'ReceivingAreas_SALSAPredecessor'=>'Row'
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
   
    //dpm($imports);
    $i=0;
    foreach($imports as $data){
        dpm($data);
        $node = uwwtd_update_rca_node($data, $file, $repCode, $annee, $timestamp, $i);
        dpm($node);
        $i++;
    }
    
    return 'debug';  
}
