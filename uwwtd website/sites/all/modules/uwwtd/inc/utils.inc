<?php

/**
 *
 */
function uwwtd_get_max_annee()
{
	$query = "SELECT MAX(field_anneedata_value) FROM {field_data_field_anneedata}";
	$result = db_query($query);
	$annee = $result->fetchField();
    return $annee;
}

/**
 *
 */
function  uwwtd_get_previous_year($year)
{
    $allyear = uwwtd_get_all_year();
    rsort($allyear);
    foreach($allyear as $one_year) {
        if ($one_year < $year) {
            return $one_year;
        }
    } 
    return false;   
}

/**
 *
 */
function uwwtd_get_all_year()
{
    $year = array();
	$query = "SELECT field_anneedata_value FROM {field_data_field_anneedata} group by field_anneedata_value order by field_anneedata_value desc";
    $result = db_query($query);
	while($record = $result->fetchAssoc()) {
		$year[$record['field_anneedata_value']] = $record['field_anneedata_value'];
	}
    return $year;
}

/**
 *
 */
function uwwtd_check_exist($siteId){

	$id = '';

	$results = db_query('SELECT c.entity_id, c.field_siteid_value FROM {field_data_field_siteid} c WHERE c.field_siteid_value = :id', array(':id' => $siteId));

	foreach($results as $result){
		$id = $result->entity_id;
	}
	return $id;
}
  
/**
 *
 */ 
function uwwtd_get_siteid($type, $option)
{
    $siteId = '';
    switch($type) {
        case 'ms_level':
            $siteId = $option['year'].'_mslevel';
            break;
        case 'receiving_area':  
            $siteId = $option['year'].'_rca_'.uwwtd_check_text($option['rcaCode']);
            break;
        case 'big_city':  
            $siteId = $option['year'].'_big_'.uwwtd_check_text($option['bigCityID']);
            break;   
        case 'agglomeration':
            $siteId = $option['year'].'_agg_'.uwwtd_check_text($option['aggCode']);
            break;
        case 'uwwtp':  
            $siteId = $option['year'].'_uww_'.uwwtd_check_text($option['uwwCode']);
            break;
        case 'discharge_point':  
            $siteId = $option['year'].'_dcp_'.uwwtd_check_text($option['dcpCode']);
            break;  
        case 'agglo_uww':  
            $siteId = uwwtd_get_siteid('agglomeration', $option) . '_' . uwwtd_get_siteid('uwwtp', $option); 
            break;                                           
    }
    return $siteId;
}

/**
 * Extract an xml for only one agglo from a xml source
 */
function debug_export_xml($nid, $type, $value)
{
    if (false === in_array($type, array('aggcode'))) {
        return '';
    }

    $node = node_load($nid);
    $xml = uwwtd_xml_to_array($node->field_sourcefile['und'][0]['uri']);
    unset($node);

    $s = '';
    $listAggCode = explode(',', $value);
    $currentCodeAgglo = array();
    foreach($listAggCode as $aggCode) {
        $currentCodeAgglo[] = strtolower($aggCode);
    }
//     codeagglo in $agg->field_inspireidlocalid['und'][0]['value']
//     copier xml node /Reporter
//                     /ReportPeriod
//                     /Contacts
//                     /BigCityDischargers
//     Agglomerations/Agglomeration/aggCode = codeagglo
//         copier l'agglo
//     UwwtpAgglos/UwwtpAgglo/aucAggCode =  codeagglo
//         copier UwwtpAgglo de l'agglo
//     UwwtpAgglos/UwwtpAgglo/aucUwwCode =  codeuwwtp
//         copier les uwwtp de l'agglo avec contenu de UwwtpAgglos
//     UWWTPs/UWWTP/uwwCode = codeuwwtp
//         !! copier toutes les agglo de ce uwwtp (et agglo et agglo_uwwtp)
//     DischargePoints/DischargePoint/uwwID = codeuwwtp
//         copier les dcp de chaque uwwtp
//     DischargePoints/DischargePoint/rcaID = rcacode
//         copier les rca de chaque dcp
//     ReceivingAreas/ReceivingArea/rcaCode = rcacode
    $xmlTest = array();
    $xmlTest['UWWTD_Report']['Reporter'] = $xml['UWWTD_Report']['Reporter'];
    $xmlTest['UWWTD_Report']['ReportPeriod'] = $xml['UWWTD_Report']['ReportPeriod'];
    $xmlTest['UWWTD_Report']['Contacts'] = $xml['UWWTD_Report']['Contacts'];
    $xmlTest['UWWTD_Report']['BigCityDischargers'] = $xml['UWWTD_Report']['BigCityDischargers'];

    //AGGLO ======================================================
    $imports  = array();
    if(isset($xml['UWWTD_Report']['Agglomerations']['Agglomeration']['aggCode'])){
        $imports[0] = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
    }
    else{
        $imports = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
    }
    $aggloFound = 0;
    foreach($imports as $agglo) {
        if (in_array(strtolower($agglo['aggCode']),$currentCodeAgglo)) {
            $xmlTest['UWWTD_Report']['Agglomerations']['Agglomeration'][] = $agglo;
            $aggloFound++;
//             break; //only one agglo
        }
    }
    if ($aggloFound != count($currentCodeAgglo)) {
        return "error, can't find agglo in xml " . implode(',', $currentCodeAgglo) . "</br>";
    }
    //UwwtpAgglos for agglo ======================================================
    $imports  = array();
    if(isset($xml['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo']['aucUwwCode'])){
        $imports[0] = $xml['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'];
    }
    else{
        $imports = $xml['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'];
    }
    $uwwtd_aggloFound = array();
    foreach($imports as $uwwtp_agglo) {
        if (in_array(strtolower($uwwtp_agglo['aucAggCode']),$currentCodeAgglo)) {
            $xmlTest['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'][] = $uwwtp_agglo;
            $uwwCode[] = $uwwtp_agglo['aucUwwCode'];  //list of uwwtp attach to the agglo
            $uwwtd_aggloFound[$uwwtp_agglo['aucAggCode']]++;;
        }
    }
    if (count($uwwtd_aggloFound) != count($currentCodeAgglo)) {
//         return "error, can't find buwwtd_agglo in xml for agglo " . $currentCodeAgglo . "</br>";
        drupal_set_message("error, can't find buwwtd_agglo in xml for agglo " . implode(',', $currentCodeAgglo));
                $s .= "<!-- error, can't find buwwtd_agglo in xml for agglo " . implode(',', $currentCodeAgglo) . "-->
        ";
    }
    //UwwtpAgglos for uwwtp======================================================
    foreach($imports as $uwwtp_agglo) {
        if (true === in_array($uwwtp_agglo['aucUwwCode'],$uwwCode) && ! in_array(strtolower($uwwtp_agglo['aucAggCode']), $currentCodeAgglo)) {
            $listeAgglo[] = $uwwtp_agglo['aucAggCode']; //all other agglo attach to the uwwtp (need for calculation)
            $xmlTest['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'][] = $uwwtp_agglo;
        }
    }

    //Agglo for uwwtp======================================================
    $imports  = array();
    if (true === isset($listeAgglo)) {
        if(isset($xml['UWWTD_Report']['Agglomerations']['Agglomeration']['aggCode'])){
            $imports[0] = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
        }
        else{
            $imports = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
        }
        $bAggloFound = 0;
        foreach($imports as $agglo) {
            if (true === in_array($agglo['aggCode'], $listeAgglo)) {
                $xmlTest['UWWTD_Report']['Agglomerations']['Agglomeration'][] = $agglo;
                $bAggloFound++;
            }
        }
        if ($bAggloFound != count($listeAgglo)) {
//             return "error, can't find agglo attach to uwwtp in xml " . implode(',', $listeAgglo) . "</br>";
            drupal_set_message("error, can't find agglo attach to uwwtp in xml " . implode(',', $listeAgglo));
                    $s .= "<!-- error, can't find agglo attach to uwwtp in xml " . implode(',', $listeAgglo) . "-->
        ";
        }
    }

    //uwwtp======================================================
    $imports  = array();
    if(isset($xml['UWWTD_Report']['UWWTPs']['UWWTP']['uwwCode'])){
        $imports[0] = $xml['UWWTD_Report']['UWWTPs']['UWWTP'];
    }
    else{
        $imports = $xml['UWWTD_Report']['UWWTPs']['UWWTP'];
    }
    $buwwtpFound = 0;
    foreach($imports as $uwwtp) {
        if (true === in_array($uwwtp['uwwCode'],$uwwCode)) {
            $xmlTest['UWWTD_Report']['UWWTPs']['UWWTP'][] = $uwwtp;
            $buwwtpFound++;
        }
    }
    if ($buwwtpFound != count($uwwCode)) {
//         return "error, can't find uwwtp in xml  " . implode(',', $uwwCode) . "</br>";
        drupal_set_message("error, can't find uwwtp in xml  " . implode(',', $uwwCode));
                $s .= "<!-- error, can't find uwwtp in xml  " . implode(',', $uwwCode) . "-->
        ";
    }

    //DischargePoints/DischargePoint/uwwID = codeuwwtp ======================================================
    $imports  = array();
    if(isset($xml['UWWTD_Report']['DischargePoints']['DischargePoint']['dcpCode'])){
        $imports[0] = $xml['UWWTD_Report']['DischargePoints']['DischargePoint'];
    }
    else{
        $imports = $xml['UWWTD_Report']['DischargePoints']['DischargePoint'];
    }
    $bdcpFound = array();
    $rcaCode = array();
    foreach($imports as $dcp) {
//         dsm($dcp['uwwID']);
//         dsm($dcp['rcaID']);
        if (true === in_array($dcp['uwwID'],$uwwCode)) {
            $xmlTest['UWWTD_Report']['DischargePoints']['DischargePoint'][] = $dcp;
            if (!in_array($dcp['rcaID'], $rcaCode)) {
                $rcaCode[] = $dcp['rcaID'];
            }
            $bdcpFound[$dcp['uwwID']]++;
        }
    }
//     dsm($rcaCode);
    if (count($bdcpFound) != count($uwwCode)) {
//         return "error, can't find dcp in xml for uwwtp " . implode(',', $uwwCode) . "</br>";
        drupal_set_message("error, can't find dcp in xml for uwwtp " . implode(',', $uwwCode));
        $s .= "<!-- error, can't find dcp in xml for uwwtp " . implode(',', $uwwCode) . "-->
        ";
    }
    //RCA======================================================
    $imports  = array();
    if(isset($xml['UWWTD_Report']['ReceivingAreas']['ReceivingArea']['rcaCode'])){
        $imports[0] = $xml['UWWTD_Report']['ReceivingAreas']['ReceivingArea'];
    }
    else{
        $imports = $xml['UWWTD_Report']['ReceivingAreas']['ReceivingArea'];
    }

    $brcaFound = 0;
    foreach($imports as $rca) {
        if (true === in_array($rca['rcaCode'],$rcaCode)) {
            $xmlTest['UWWTD_Report']['ReceivingAreas']['ReceivingArea'][] = $rca;
            $brcaFound++;
        }
    }
    if ($brcaFound != count($rcaCode)) {
//         return "error, can't find rca in xml for rca " . implode(',', $rcaCode) . "</br>";
        drupal_set_message("error, can't find rca in xml for rca " . implode(',', $rcaCode));
        $s .= "<!-- error, can't find rca in xml for rca " . implode(',', $rcaCode) . "-->
        ";
    }

    //CREATE XML======================================================
    $s .= '<'.'?xml version="1.0" encoding="UTF-8"?'.'>
<UWWTD_Data xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xsi:noNamespaceSchemaLocation="http://water.eionet.europa.eu/schemas/dir91271eec/UWWTD2011.xsd">

';

//     if ($uwwtd_aggloFound != count($currentCodeAgglo)) {
//         $s .= "<!-- error, can't find buwwtd_agglo in xml for agglo " . implode(',', $currentCodeAgglo) . "-->
//         ";
//     }
//     if ($bAggloFound != count($listeAgglo)) {
//         $s .= "<!-- error, can't find agglo attach to uwwtp in xml " . implode(',', $listeAgglo) . "-->
//         ";
//     }

//     if ($buwwtpFound != count($uwwCode)) {
//         $s .= "<!-- error, can't find uwwtp in xml  " . implode(',', $uwwCode) . "-->
//         ";
//     }
//     if ($bdcpFound != count($currentCodeAgglo)) {
//         $s .= "<!-- error, can't find dcp in xml for uwwtp " . implode(',', $uwwCode) . "-->
//         ";
//     }
//     if ($brcaFound != count($rcaCode)) {
//         $s .= "<!-- error, can't find rca in xml for rca " . implode(',', $rcaCode) . "-->
//         ";
//     }

    $s .= uwwtd_debug_create_tag_from_array($xmlTest, '');

    $s .= '

</UWWTD_Data>';
    ini_set("auto_detect_line_endings", false); //for Mac user
    $aHeaders = array(
          'Content-Type'        => 'text/xml',
          'Content-Disposition' => 'attachment; filename="agglo'.$value.'.xml' . '"',
          'Expires'             => '0',
          'Cache-Control'       => 'must-revalidate',
          'Pragma'              =>'public',
//           'Content-Length'      => mb_strlen($filename),
      );
    foreach ($aHeaders as $sName => $sValue) {
        drupal_add_http_header($sName, $sValue);
    }
    echo $s;

    exit();
//     dsm($s);
//     return 'qsd';
}



/**
 * Extract an xml for only one agglo from a xml source
 */
// function debug_create_xml_old($type, $value)
// {
//     //todo, get a list of agglo nid (to test mutliple agglo)
//     if ($type != 'nid-agglo') {
//         return '';
//     }
//     $agg = node_load($value);
//     $xml = uwwtd_xml_to_array($agg->field_sourcefile['und'][0]['uri']);
//     $currentCodeAgglo = $agg->field_inspireidlocalid['und'][0]['value'];
// //     codeagglo in $agg->field_inspireidlocalid['und'][0]['value']
// //     copier xml node /Reporter
// //                     /ReportPeriod
// //                     /Contacts
// //                     /BigCityDischargers
// //     Agglomerations/Agglomeration/aggCode = codeagglo
// //         copier l'agglo
// //     UwwtpAgglos/UwwtpAgglo/aucAggCode =  codeagglo
// //         copier UwwtpAgglo de l'agglo
// //     UwwtpAgglos/UwwtpAgglo/aucUwwCode =  codeuwwtp
// //         copier les uwwtp de l'agglo avec contenu de UwwtpAgglos
// //     UWWTPs/UWWTP/uwwCode = codeuwwtp
// //         !! copier toutes les agglo de ce uwwtp (et agglo et agglo_uwwtp)
// //     DischargePoints/DischargePoint/uwwID = codeuwwtp
// //         copier les dcp de chaque uwwtp
// //     DischargePoints/DischargePoint/rcaID = rcacode
// //         copier les rca de chaque dcp
// //     ReceivingAreas/ReceivingArea/rcaCode = rcacode
//     $xmlTest = array();
//     $xmlTest['UWWTD_Report']['Reporter'] = $xml['UWWTD_Report']['Reporter'];
//     $xmlTest['UWWTD_Report']['ReportPeriod'] = $xml['UWWTD_Report']['ReportPeriod'];
//     $xmlTest['UWWTD_Report']['Contacts'] = $xml['UWWTD_Report']['Contacts'];
//     $xmlTest['UWWTD_Report']['BigCityDischargers'] = $xml['UWWTD_Report']['BigCityDischargers'];
//
//     //AGGLO ======================================================
//     if(isset($xml['UWWTD_Report']['Agglomerations']['Agglomeration']['aggCode'])){
//         $imports[0] = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
//     }
//     else{
//         $imports = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
//     }
//     $aggloFound = false;
//     foreach($imports as $agglo) {
//         if ($agglo['aggCode'] == $currentCodeAgglo) {
//             $xmlTest['UWWTD_Report']['Agglomerations']['Agglomeration'][] = $agglo;
//             $aggloFound = true;
//             break; //only one agglo
//         }
//     }
//     if ($aggloFound == false) {
//         return "error, can't find agglo in xml " . $currentCodeAgglo . "</br>";
//     }
//     //UwwtpAgglos for agglo ======================================================
//     if(isset($xml['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo']['aucUwwCode'])){
//         $imports[0] = $xml['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'];
//     }
//     else{
//         $imports = $xml['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'];
//     }
//     $uwwtd_aggloFound = false;
//     foreach($imports as $uwwtp_agglo) {
//         if ($uwwtp_agglo['aucAggCode'] == $currentCodeAgglo) {
//             $xmlTest['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'][] = $uwwtp_agglo;
//             $uwwCode[] = $uwwtp_agglo['aucUwwCode'];  //list of uwwtp attach to the agglo
//             $uwwtd_aggloFound = true;
//         }
//     }
//     if ($uwwtd_aggloFound == false) {
// //         return "error, can't find buwwtd_agglo in xml for agglo " . $currentCodeAgglo . "</br>";
//         drupal_set_message("error, can't find buwwtd_agglo in xml for agglo " . $currentCodeAgglo);
//     }
//     //UwwtpAgglos for uwwtp======================================================
//     foreach($imports as $uwwtp_agglo) {
//         if (true === in_array($uwwtp_agglo['aucUwwCode'],$uwwCode) && $uwwtp_agglo['aucAggCode'] != $currentCodeAgglo) {
//             $listeAgglo[] = $uwwtp_agglo['aucAggCode']; //all other agglo attach to the uwwtp (need for calculation)
//             $xmlTest['UWWTD_Report']['UwwtpAgglos']['UwwtpAgglo'][] = $uwwtp_agglo;
//         }
//     }
//
//     //Agglo for uwwtp======================================================
//     if (true === isset($listeAgglo)) {
//         if(isset($xml['UWWTD_Report']['Agglomerations']['Agglomeration']['aggCode'])){
//             $imports[0] = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
//         }
//         else{
//             $imports = $xml['UWWTD_Report']['Agglomerations']['Agglomeration'];
//         }
//         $bAggloFound = false;
//         foreach($imports as $agglo) {
//             if (true === in_array($agglo['aggCode'], $listeAgglo)) {
//                 $xmlTest['UWWTD_Report']['Agglomerations']['Agglomeration'][] = $agglo;
//                 $bAggloFound = true;
//             }
//         }
//         if ($bAggloFound == false) {
// //             return "error, can't find agglo attach to uwwtp in xml " . implode(',', $listeAgglo) . "</br>";
//             drupal_set_message("error, can't find agglo attach to uwwtp in xml " . implode(',', $listeAgglo));
//         }
//     }
//
//     //uwwtp======================================================
//     if(isset($xml['UWWTD_Report']['UWWTPs']['UWWTP']['uwwCode'])){
//         $imports[0] = $xml['UWWTD_Report']['UWWTPs']['UWWTP'];
//     }
//     else{
//         $imports = $xml['UWWTD_Report']['UWWTPs']['UWWTP'];
//     }
//     $buwwtpFound = false;
//     foreach($imports as $uwwtp) {
//         if (true === in_array($uwwtp['uwwCode'],$uwwCode)) {
//             $xmlTest['UWWTD_Report']['UWWTPs']['UWWTP'][] = $uwwtp;
//             $buwwtpFound = true;
//         }
//     }
//     if ($buwwtpFound == false) {
// //         return "error, can't find uwwtp in xml  " . implode(',', $uwwCode) . "</br>";
//         drupal_set_message("error, can't find uwwtp in xml  " . implode(',', $uwwCode));
//     }
//
//     //DischargePoints/DischargePoint/uwwID = codeuwwtp ======================================================
//     if(isset($xml['UWWTD_Report']['DischargePoints']['DischargePoint']['dcpCode'])){
//         $imports[0] = $xml['UWWTD_Report']['DischargePoints']['DischargePoint'];
//     }
//     else{
//         $imports = $xml['UWWTD_Report']['DischargePoints']['DischargePoint'];
//     }
//     $bdcpFound = false;
//     foreach($imports as $dcp) {
//         if (true === in_array($dcp['uwwID'],$uwwCode)) {
//             $xmlTest['UWWTD_Report']['DischargePoints']['DischargePoint'][] = $dcp;
//             $rcaCode[] = $dcp['rcaID'];
//             $bdcpFound = true;
//         }
//     }
//     if ($bdcpFound == false) {
// //         return "error, can't find dcp in xml for uwwtp " . implode(',', $uwwCode) . "</br>";
//         drupal_set_message("error, can't find dcp in xml for uwwtp " . implode(',', $uwwCode));
//     }
//     //RCA======================================================
//     if(isset($xml['UWWTD_Report']['ReceivingAreas']['ReceivingArea']['rcaCode'])){
//         $imports[0] = $xml['UWWTD_Report']['ReceivingAreas']['ReceivingArea'];
//     }
//     else{
//         $imports = $xml['UWWTD_Report']['ReceivingAreas']['ReceivingArea'];
//     }
//
//     $brcaFound = false;
//     foreach($imports as $rca) {
//         if (true === in_array($rca['rcaCode'],$rcaCode)) {
//             $xmlTest['UWWTD_Report']['ReceivingAreas']['ReceivingArea'][] = $rca;
//             $brcaFound = true;
//         }
//     }
//     if ($brcaFound == false) {
// //         return "error, can't find rca in xml for rca " . implode(',', $rcaCode) . "</br>";
//         drupal_set_message("error, can't find rca in xml for rca " . implode(',', $rcaCode));
//     }
//
//     //CREATE XML======================================================
//     $s = '<'.'?xml version="1.0" encoding="UTF-8"?'.'>
// <UWWTD_Data xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xsi:noNamespaceSchemaLocation="http://water.eionet.europa.eu/schemas/dir91271eec/UWWTD2011.xsd">
//
// ';
//
//     if ($uwwtd_aggloFound == false) {
//         $s .= "<!-- error, can't find buwwtd_agglo in xml for agglo " . $currentCodeAgglo . "-->
//         ";
//     }
//     if ($bAggloFound == false) {
//         $s .= "<!-- error, can't find agglo attach to uwwtp in xml " . implode(',', $listeAgglo) . "-->
//         ";
//     }
//
//     if ($buwwtpFound == false) {
//         $s .= "<!-- error, can't find uwwtp in xml  " . implode(',', $uwwCode) . "-->
//         ";
//     }
//     if ($bdcpFound == false) {
//         $s .= "<!-- error, can't find dcp in xml for uwwtp " . implode(',', $uwwCode) . "-->
//         ";
//     }
//     if ($brcaFound == false) {
//         $s .= "<!-- error, can't find rca in xml for rca " . implode(',', $rcaCode) . "-->
//         ";
//     }
//
//     $s .= uwwtd_debug_create_tag_from_array($xmlTest, '');
//
//     $s .= '
//
// </UWWTD_Data>';
//     ini_set("auto_detect_line_endings", false); //for Mac user
//     $aHeaders = array(
//           'Content-Type'        => 'text/xml',
//           'Content-Disposition' => 'attachment; filename="agglo'.$value.'.xml' . '"',
//           'Expires'             => '0',
//           'Cache-Control'       => 'must-revalidate',
//           'Pragma'              =>'public',
// //           'Content-Length'      => mb_strlen($filename),
//       );
//     foreach ($aHeaders as $sName => $sValue) {
//         drupal_add_http_header($sName, $sValue);
//     }
//     echo $s;
//     exit();
// }

//TODO better !!
function uwwtd_debug_create_tag_from_array($array, $parentkey)
{
    if (false === is_array($array)) {
        return $array;
    }
    $s = '';
    foreach($array as $key => $value) {
        if (true === is_int($key)) {
            $s .= '<'.$parentkey.'>';
        }elseif (false === is_array($value) || false === isset($value[0])) {
            $s .= '<'.$key.'>';
        }

        $s .= uwwtd_debug_create_tag_from_array($value, $key);

        if (true === is_int($key)) {
            $s .= '</'.$parentkey.'>';
        }elseif (false === is_array($value) || false === isset($value[0])) {
            $s .= '</'.$key.'>';
        }
        $s .= "\n";
    }
    return $s;
}

function uwwtd_debug(){
	
    return '';

    return 'debug';
}

function uwwtd_add_error($type, $category, $error, $timestamp, $index = null, $node = null){
    if($node){
        $key = $node->nid;
    }
    else{
        $key = 'all';
    }
	$GLOBALS['uwwtd_errors'][$key][]=array(
        'type'=>$type,
        'category'=>$category,
        'error'=>$error,
        'timestamp'=>$timestamp,
        'index'=>$index,
    );
    
    /*
    $error = trim($error);

	// get label for type
	$field = field_info_field('field_uwwtd_err_type');
	$typeText = $field['settings']['allowed_values'][$type];

	// get label for category
	$field = field_info_field('field_uwwtd_err_category');
	$categoryText = $field['settings']['allowed_values'][$category];

	// get errors file
	$file = drupal_realpath('public://data_errors/uwwtd_errors.txt');
	//$content = file_get_contents($file);

	// get date
	$dateError = date('m/d/Y h:i:s a');

	//$content = $content.$categoryText.' '.$typeText.' '.$dateError.': '.$error;
    $log_msg =$categoryText.' '.$typeText.' '.$dateError.': '.$error;
	// clean sentences
	if(substr($error, -1) != '.') $log_msg .= '. ';
	else $log_msg .=  ' ';
	
	if($node !== null && !empty($node) && $node != ''){
		$log_msg .= $type.' concerns data with a site id of '.$node->field_siteid['und'][0]['value'].' and a drupal id of '.$node->nid.'. ';
	}
	if($index !== null && !empty($index) && $index != ''){
		$log_msg .= 'The corresponding row in the dataset used is at index '.$index.'.';
	}
	$log_msg .= "\r\n";

	$fh = fopen($file, "a");
	fwrite($fh, $log_msg);
	fclose($fh);

	// create error node
	$errorNode = new StdClass();
	$errorNode->type = 'uwwtd_error';
    $errorNode->uid = 1;
    $errorNode->translate = 0;
    $errorNode->tnid = 0;
    $errorNode->sticky = 0;
    $errorNode->promote = 0;
    $errorNode->comment = 0;
	//node_save($errorNode); ===> pas de double insertion en base

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
	}
	if($index != NULL) $errorNode->field_uwwtd_err_index['und'][0]['value'] = $index;

	node_save($errorNode);
    if($node != NULL){
        $node->field_uwwtd_error_link['und'][] = array('nid'=>$errorNode->nid);
		node_save($node);
    }
    */
}

function uwwtd_flush_errors(){
    if(!empty($GLOBALS['uwwtd_errors'])){
        // get label for type
        $field = field_info_field('field_uwwtd_err_type');
        $err_type = $field['settings']['allowed_values'];
        //$field['settings']['allowed_values'][$type];

        // get label for category
        $field = field_info_field('field_uwwtd_err_category');
        $err_category = $field['settings']['allowed_values'];
        //[$category];

        // get errors file
        $file = drupal_realpath('public://data_errors/uwwtd_errors.txt');
        $fh = fopen($file, "a");
        foreach($GLOBALS['uwwtd_errors'] as $nid=>$errors){
            if(!empty($errors)){
                $year = null;
                if($nid!='all'){
                    $node=node_load($nid);
                    if(isset($node->field_anneedata)) $year = $node->field_anneedata['und'][0]['value'];
                }
                else $node =null;
                foreach($errors as $error){
                    $log_msg = '';
                    $log_msg =$err_category[$error['category']].' '.$err_type[$error['type']].' '.$error['timestamp'].': '.$error['error'];
                    // clean sentences
                    if(substr($error['error'], -1) != '.') $log_msg .= '. ';
                    else $log_msg .=  ' ';
                    
                    if($node !== null && !empty($node) && $node != ''){
                        $log_msg .= $error['type'].' concerns data with a site id of '.$node->field_siteid['und'][0]['value'].' and a drupal id of '.$node->nid.'. ';
                    }
                    if($error['index'] !== null && !empty($error['index']) && $error['index'] != ''){
                        $log_msg .= 'The corresponding row in the dataset used is at index '.$error['index'].'.';
                    }
                    $log_msg .= "\r\n";
                    
                    fwrite($fh, $log_msg);
                    
                    /*
                    // create error node
                    $errorNode = new StdClass();
                    $errorNode->type = 'uwwtd_error';
                    $errorNode->uid = 1;
                    $errorNode->translate = 0;
                    $errorNode->tnid = 0;
                    $errorNode->sticky = 0;
                    $errorNode->promote = 0;
                    $errorNode->comment = 0;

                    $dateFormat = format_date($error['timestamp'], 'custom', 'Y-m-d H:i:s', 'UTC');
                    $dateErrorFormat = format_date(strtotime('now'), 'custom', 'Y-m-d H:i:s', 'UTC');
                    $errorNode->title = $error['category'].' '.$error['type'].' '.$error['dateError'];
                    $errorNode->field_uwwtd_err_identifier['und'][0]['value'] = $error['timestamp'];
                    $errorNode->field_uwwtd_err_type['und'][0]['value'] = $error['type'];
                    $errorNode->field_uwwtd_err_category['und'][0]['value'] = $error['category'];

                    $errorNode->field_uwwtd_err_timestamp['und'][0]['value'] = $dateFormat;
                    $errorNode->field_uwwtd_err_timestamp['und'][0]['timezone'] = 'Europe/Paris';

                    $errorNode->field_uwwtd_err_time['und'][0]['value'] = $dateErrorFormat;
                    $errorNode->field_uwwtd_err_time['und'][0]['timezone'] = 'Europe/Paris';

                    $errorNode->field_uwwtd_err_message['und'][0]['value'] = $error['error'];
                    if($node != NULL){
                        $errorNode->field_uwwtd_err_concern['und'][0]['nid'] = $node->nid;
                    }
                    if($index != NULL) $errorNode->field_uwwtd_err_index['und'][0]['value'] = $index;

                    node_save($errorNode);
                    if($node != NULL){
                        $node->field_uwwtd_error_link['und'][] = array('nid'=>$errorNode->nid);
                    }
                    */
                    $record = array(
                        'type'=>$error['type'],
                        'category'=>$error['category'],
                        'year'=>(int)$year,
                        'error'=>$error['error'],
                        'date'=>$error['timestamp'],
                        'entity_type'=>($node?'node':null),
                        'entity_id'=>($node?$node->nid:null), //todo if
                        'bundle'=>($node?$node->type:null), //todo if
                    );
                    drupal_write_record("uwwtd_import_errors", $record);
                }
                /*if($node != NULL){
                    node_save($node);
                }*/
            }
            
        }
        fclose($fh);
        
        $GLOBALS['uwwtd_errors'] = array();
    }
}

function uwwtd_set_warning($i, $emes){
	$_SESSION['uwwtd_warnings'][] = array($i, $emes);
}

function uwwtd_set_error($i, $emes){
	$_SESSION['uwwtd_error'][] = array($i, $emes);
}

function uwwtd_check_numeric($value, $timestamp, $index = null, $node = null) 
{
    $value = str_replace(",",".",trim($value));
	if(true === ctype_digit($value)) {
		return (int)$value;
	}else{
		uwwtd_add_error(2, 4, 'The input data is not in numeric format.', $timestamp, $index, $node);
		return false;
	}
}

function uwwtd_check_decimal($value, $timestamp, $index = null, $node = null){
    $value = str_replace(",",".",trim($value));
	if(is_numeric($value)) {
		return (float)$value;	
	}else{
		uwwtd_add_error(2, 4, 'The input data is not in numeric or decimal format.', $timestamp, $index, $node);
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