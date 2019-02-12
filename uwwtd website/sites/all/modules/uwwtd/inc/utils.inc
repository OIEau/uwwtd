<?php

/**
 * Return years which defines 8th report.
 */
function _get_years_8th_report() {
    return array(2011, 2012, 2013);
}

/**
 * Return years which defines 9th report.
 */
function _get_years_9th_report() {
    return array(2014, 2015);
}

/**
 * Return years which defines 10th report.
 */
function _get_years_10th_report() {
    return array(2016);
}


if (!function_exists('array_column')) {
    /**
     * Returns the values from a single column of the input array, identified by
     * the $columnKey.
     *
     * Optionally, you may provide an $indexKey to index the values in the returned
     * array by the values from the $indexKey column in the input array.
     *
     * @param array $input A multi-dimensional array (record set) from which to pull
     *                     a column of values.
     * @param mixed $columnKey The column of values to return. This value may be the
     *                         integer key of the column you wish to retrieve, or it
     *                         may be the string key name for an associative array.
     * @param mixed $indexKey (Optional.) The column to use as the index/keys for
     *                        the returned array. This value may be the integer key
     *                        of the column, or it may be the string key name.
     * @return array
     */
    function array_column($input = null, $columnKey = null, $indexKey = null)
    {
        // Using func_get_args() in order to check for proper number of
        // parameters and trigger errors exactly as the built-in array_column()
        // does in PHP 5.5.
        $argc = func_num_args();
        $params = func_get_args();
        if ($argc < 2) {
            trigger_error("array_column() expects at least 2 parameters, {$argc} given", E_USER_WARNING);
            return null;
        }
        if (!is_array($params[0])) {
            trigger_error(
                'array_column() expects parameter 1 to be array, ' . gettype($params[0]) . ' given',
                E_USER_WARNING
            );
            return null;
        }
        if (!is_int($params[1])
            && !is_float($params[1])
            && !is_string($params[1])
            && $params[1] !== null
            && !(is_object($params[1]) && method_exists($params[1], '__toString'))
        ) {
            trigger_error('array_column(): The column key should be either a string or an integer', E_USER_WARNING);
            return false;
        }
        if (isset($params[2])
            && !is_int($params[2])
            && !is_float($params[2])
            && !is_string($params[2])
            && !(is_object($params[2]) && method_exists($params[2], '__toString'))
        ) {
            trigger_error('array_column(): The index key should be either a string or an integer', E_USER_WARNING);
            return false;
        }
        $paramsInput = $params[0];
        $paramsColumnKey = ($params[1] !== null) ? (string) $params[1] : null;
        $paramsIndexKey = null;
        if (isset($params[2])) {
            if (is_float($params[2]) || is_int($params[2])) {
                $paramsIndexKey = (int) $params[2];
            } else {
                $paramsIndexKey = (string) $params[2];
            }
        }
        $resultArray = array();
        foreach ($paramsInput as $row) {
            $key = $value = null;
            $keySet = $valueSet = false;
            if ($paramsIndexKey !== null && array_key_exists($paramsIndexKey, $row)) {
                $keySet = true;
                $key = (string) $row[$paramsIndexKey];
            }
            if ($paramsColumnKey === null) {
                $valueSet = true;
                $value = $row;
            } elseif (is_array($row) && array_key_exists($paramsColumnKey, $row)) {
                $valueSet = true;
                $value = $row[$paramsColumnKey];
            }
            if ($valueSet) {
                if ($keySet) {
                    $resultArray[$key] = $value;
                } else {
                    $resultArray[] = $value;
                }
            }
        }
        return $resultArray;
    }
}


function uwwtd_debug(){

    return "debug";
    /*
    $years = uwwtd_get_all_year();
    //uwwtd_retreive_agg_dtt(2016, $context);
    foreach($years as $year){
        //dsm($year);
        unset($context['sandbox']['progress']);
        uwwtd_retreive_agg_dtt($year, $context);
    }
    return 'aaaa';
   */
    /*
    $img = uwwtd_get_gg_snapshot('public://test.png', '37.20776245', '-7.41892334', 'hybrid', 16, '640x640');
    dsm($img);
    */
    
    //Loading Art 17 xml file
    /*
    module_load_include('inc','uwwtd','inc/data_center');
    //$uri = 'public://data_sources/uwwtdart17tablesfinal190118final200718_1538643292.xml';
    $uri = 'public://data_sources/hruwwtdart17ref_1539000349.xml';
    $datas = uwwtd_xml_to_array($uri);
    dsm($datas);
    
    
    $newFile = new stdclass();
    $newFile->uri = $uri;
    $newFile->fid = 925;
    //dsm($datas);
    foreach($datas['FLAInvestments']['Row'] as $investment){
        uwwtd_art17_import_FLAInvestment($investment, $newFile, 2016);
    }
    foreach($datas['FLAAgglomerations']['Row'] as $agglomeration){
        uwwtd_art17_import_FLAAgglomeration($agglomeration, $newFile, 2016);
    }

    foreach($datas['FLAUWWTPs']['Row'] as $uwwtp){
        uwwtd_art17_import_FLAUWWTP($uwwtp, $newFile, 2016);
    }*/
    
   
//dsm(array_keys(openlayers_styles()));
    //require_once(drupal_get_path('module', 'uwwtd') . '/inc/ShapeFile.inc.php');
    /*
    module_load_include('php','uwwtd','inc/ShapeFile.inc');
    module_load_include('inc','uwwtd','inc/data_center');
    $shp = new ShapeFile(drupal_realpath('sites/default/files/data_shapes/SI_SA_river.shp'));
    foreach($shp->records as $record){
        //dsm($record);
        $shpdata = $record->shp_data['parts'][0]['points'];
        $geodata = uwwtd_coord_transform_polygon($shpdata);
        dsm($geodata);
        break;
    }
    */
    /*
    $maps = openlayers_maps(true);
    foreach($maps as $name=>$old_map){
        $map = clone $old_map;
        if($name!='default' && substr($name, 0, 7)!="example" && substr($name, 0, 8)!="geofield"){
            foreach($map->data['layer_activated'] as $layer_name=>$activated){
                if($activated){
                    if(!isset($map->data['behaviors']['openlayers_plus_behavior_blockswitcher_plus']['layers_'.$layer_name])){
                        $map->data['behaviors']['openlayers_plus_behavior_blockswitcher_plus']['layers_'.$layer_name] = 1;
                        $map->data['behaviors']['openlayers_plus_behavior_blockswitcher_plus']['layers_'.$layer_name.'_groupname'] = 'UWWTD';
                    }
                }
            }
        }
    }
    */
    return 'debug';
}

function uwwtd_get_gg_snapshot($dest_uri, $lat, $lon, $maptype, $zoom = 15, $size = '640x640', $format = 'png'){
    $url = url(
        'https://maps.googleapis.com/maps/api/staticmap',
        array(
            'external' => true,
            'query'    => array(
                'format'  => $format,
                'zoom'    => $zoom,
                'size'    => $size,
                'maptype' => $maptype,
                //'key'     => 'AIzaSyCQGC-fIeuFfBp84oW7AooJFQTZewQZgq0',
                'markers' => 'color:red|' . $lat . ',' . $lon,
            ),
        )
    );
    dsm($url);
    $img           = file_get_contents($url);
    $file          = file_save_data($img, $dest_uri, FILE_EXISTS_REPLACE);
    $file          = (array) $file;
    $file['title'] = 'Google snapshot : ' . $maptype;
    return $file;
}


/**
 *
 */
function uwwtd_get_max_annee($bundle = 'agglomeration')
{
	$query = "SELECT MAX(field_anneedata_value) FROM {field_data_field_anneedata} WHERE bundle=:bundle ";
	$result = db_query($query, [':bundle'=>$bundle]);
	$annee = $result->fetchField();
    return $annee;
}

function uwwtd_get_all_nuts($type)
{
	$regionnuts = '';
	
$results = db_query(
		'SELECT r.field_regionnuts_value, nut.n3nm 
		FROM {field_data_field_regionnuts} r 
			LEFT JOIN regions_nuts nut ON r.field_regionnuts_value = nut.n3cd
		WHERE r.bundle = :type 
		GROUP BY r.field_regionnuts_value, nut.n3nm 
		ORDER BY n3nm', 
		array(':type' => $type)
	);
	$regionnuts = array("All" =>'- Any -');
	foreach($results as $result){
		$regionnuts[$result->field_regionnuts_value] = $result->n3nm;
	}
	return $regionnuts;
}

function uwwtd_get_all_rbd($type){
	$rbd = array('All' => t('- Any -'));
	/*
	$results = db_query(
		'SELECT dc.field_dcpreceivingwater_value,  rb.rbdname
		FROM {field_data_field_linked_discharge_points} d 
			LEFT JOIN {field_data_field_dcpreceivingwater} dc ON d.field_linked_discharge_points_nid = dc.entity_id
			LEFT JOIN river_basin rb ON dc.field_dcpreceivingwater_value = rb.eucd_rbd
		WHERE d.bundle = :type 
		GROUP BY dc.field_dcpreceivingwater_value, rb.rbdname
		ORDER BY rb.rbdname', 
		array(':type' => $type)
	);
	foreach($results as $result){
		$rbd[$result->field_dcpreceivingwater_value] = $result->rbdname.' '. $result->field_dcpreceivingwater_value;
	}*/
    
    
    $results = db_query(
        '
            SELECT rb.eurbdcode,  rb.rbdname
            FROM river_basin rb
            WHERE lower(c_cd) = :cc
            GROUP BY rb.eurbdcode,  rb.rbdname
            ORDER BY rb.rbdname ASC
        ',
        array(':cc' => variable_get('siif_eru_country_code', '%'))
    );
    foreach($results as $result){
		$rbd[$result->eurbdcode] = $result->rbdname.' '. $result->eurbdcode;
	}
    
    
	return $rbd;
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
	$query = "SELECT field_anneedata_value FROM {field_data_field_anneedata} WHERE bundle='agglomeration' GROUP BY field_anneedata_value ORDER BY field_anneedata_value DESC";
    $result = db_query($query);
	while($record = $result->fetchAssoc()) {
		$year[$record['field_anneedata_value']] = $record['field_anneedata_value'];
	}
    return $year;
}

function uwwtd_get_all_sourcefile()
{
    $sourcefile = array();
	$query = "SELECT field_sourcefile_fid FROM {field_data_field_sourcefile} group by field_sourcefile_fid order by field_sourcefile_fid desc";
    $result = db_query($query);
	while($record = $result->fetchAssoc()) {
		$sourcefile[$record['field_sourcefile_fid']] = $record['field_sourcefile_fid'];
	}
    return $sourcefile;
}

function uwwtd_get_sourcefile($entity){

	$fid = '';

	$results = db_query('SELECT s.field_sourcefile_fid FROM {field_data_field_sourcefile} s WHERE s.entity_id = :fid', array(':fid' => $entity));

	foreach($results as $result){
		$fid = $result->field_sourcefile_fid;
	}
	return $fid;
}

function uwwtd_get_filemanaged($fid){

	$filename = '';

	$results = db_query('SELECT f.filename FROM {file_managed} f WHERE f.fid = :filename', array(':filename' => $fid));

	foreach($results as $result){
		$filename = $result->filename;
	}
	return $filename;
}

/**
 *
 */
function uwwtd_check_exist($siteId){

	$id = '';
    if(in_array(strtolower(variable_get('siif_eru_country_code')), ['gr', 'el'])){
        $siteIds=[$siteId];
        $siteIds[]=str_replace('_EL', '_GR',$siteId);
        $siteIds[]=str_replace('_GR', '_EL',$siteId);
        $results = db_query("SELECT c.entity_id, c.field_siteid_value FROM {field_data_field_siteid} c WHERE c.field_siteid_value IN ('". implode("','",$siteIds) ."')");
    }
    else{
        $results = db_query('SELECT c.entity_id, c.field_siteid_value FROM {field_data_field_siteid} c WHERE c.field_siteid_value = :id', array(':id' => $siteId));
    }
	

	foreach($results as $result){
		$id = $result->entity_id;
        break;
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


/**
 * Return list of errors messages only for 8th report.
 */
function _get_8th_report_errors() {
    return array(
        'No receiving area status found',
        'No receiving area lsa article 61 date found',
        'No receiving area Related Sensitive area found',
        'No receiving area designation date found',
        'No receiving area nitrogen information found',
        'No receiving area phosphorus information found',
        'No receiving area B information found',
        'No receiving area C information found',
        'No receiving area relevant directive information found',
        'No receiving area morphology information found',
        'No receiving area hydrology information found',
        'No receiving area hydraulic information found',
        'No receiving area absence risk information found',
        'No receiving area plants information found',
        'No receiving area plants capacity information found',
        'No receiving area incoming N-tot measured information found',
        'No receiving area incoming N-tot calculated information found',
        'No receiving area incoming N-tot estimated information found',
        'No receiving area incoming P-tot measured information found',
        'No receiving area incoming P-tot calculated information found',
        'No receiving area incoming P-tot estimated information found',
        'No receiving area discharged N-tot measured information found',
        'No receiving area discharged N-tot calculated information found',
        'No receiving area discharged N-tot estimated information found',
        'No receiving area discharged P-tot measured information found',
        'No receiving area discharged P-tot calculated information found',
        'No receiving area discharged P-tot estimated information found',
        'No receiving area root of corresponding GIS-data file found',
        'No receiving area other directive information found',
        'No receiving area other directive information date found',
        'No receiving area root of correspo GIS-data file found',
        'No receiving area article 54 applied information found',
        'No receiving area name parameter found',
        'No receiving area starting date of application found',
        'No receiving area lsa hyperlink found',
        'No receiving area lsa begin life date found',
        'No receiving area lsa end life date found',
        'No big cities found in the dataset',
        'No agglomeration big id information found',
    );
}


/**
 * Return xml field name which exists only in 9th report.
 */
function _get_xml_fields_names_only_in_9th_report() {
    return array(
        'ReceivingAreaSAParameter',
        'ReceivingAreaSAMain',
        'ReceivingAreaSA54',
        'ReceivingAreaLSA',
        'ReceivingAreaSASA',
        'ReceivingAreasSALSAPredecessor',
    );
}


/**
 * Return true if one of 8th report errors contains the the given text.
 */
function _is_like_a_8th_report_errors($text) {
    $isLike8thReportError = FALSE;
    foreach (_get_8th_report_errors() as $errorText) {
        if (strpos($errorText, $text) !== FALSE || strpos($text, $errorText) !== FALSE) {
            $isLike8thReportError = TRUE;
            break;
        }
    }
    return $isLike8thReportError;
}


function uwwtd_add_error($year, $type, $category, $error, $timestamp, $index = null, $node = null, $xmlFieldName = '') {
    $isShowed = TRUE;

    // If this is a 8th report :
    if (in_array($year, _get_years_8th_report())) {
        // If this field is about rca links :
        if (!empty($xmlFieldName) && in_array($xmlFieldName, _get_xml_fields_names_only_in_9th_report())) {
            $isShowed = FALSE;
        }

    // If this is a 9th report and the message is about:
    } elseif (in_array($year, _get_years_9th_report()) && _is_like_a_8th_report_errors($error)) {
        $isShowed = FALSE;
    }

    if ($isShowed) {
        if ($node) {
            $key = $node->nid;
            //Don't raise error related to inactive element
            if(isset($node->field_status) && $node->field_status['und'][0]['value']!=1){
                return null;
            }
        } else {
            $key = 'all';
        }
        $GLOBALS['uwwtd_errors'][$key][]=array(
            'year'=>$year,
            'type'=>$type,
            'category'=>$category,
            'error'=>$error,
            'timestamp'=>$timestamp,
            'index'=>$index,
            'field'=>$xmlFieldName
        );
    }
}

function uwwtd_flush_errors(){
    if(!empty($GLOBALS['uwwtd_errors'])){
        // get label for type
//         $field = field_info_field('field_uwwtd_err_type');
//         $err_type = $field['settings']['allowed_values'];
        //$field['settings']['allowed_values'][$type];

        // get label for category
//         $field = field_info_field('field_uwwtd_err_category');
//         $err_category = $field['settings']['allowed_values'];
        //[$category];
        $err_type = array_flip($GLOBALS['uwwtd_errors_type']['type']);
        $err_category = array_flip($GLOBALS['uwwtd_errors_type']['category']);

        // get errors file
        $file = drupal_realpath('public://data_errors/uwwtd_errors.txt');
        $fh = fopen($file, "a");
        foreach($GLOBALS['uwwtd_errors'] as $nid=>$errors){
            if(!empty($errors)){
                
                if($nid!='all'){
                    $node=node_load($nid);
//                     if(isset($node->field_anneedata)) $year = $node->field_anneedata['und'][0]['value'];
                }
                else $node =null;
                foreach($errors as $error){
                    $year = $error['year'];
                    $log_msg = '';
                    $log_msg =$err_category[$error['category']].' '.$err_type[$error['type']].' '.$error['timestamp'].': '.$error['error'];
//                     dsm($log_msg);
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

function uwwtd_check_text($text){

	// check format
    //FIX ME : why sometime we have array in uwwtp import
    if(is_array($text)){
//         dsm('uwwtd_check_text ARRAY $text = ');
//         dsm($text);
        if (empty($text)) {
//           watchdog('uwwtd_checktext_notice', 'uwwtd_check_text array empty for value', array(), WATCHDOG_NOTICE);
          return '';      
        } else {
          watchdog('uwwtd_checktext', 'uwwtd_check_text array not empty for value : ' . var_export($text, true), array(), WATCHDOG_WARNING);
          //TODO delete
          file_put_contents(DRUPAL_ROOT . '/sites/all/modules/uwwtd/uwwtd_check_text.log', 'uwwtd_check_text array not empty for value : ' . var_export($text, true) . "\n", FILE_APPEND);
        }
        return $text;
    }
    $text = trim($text);
//     $text = check_plain($text); //on ne veut pas tranformer en entité les caractère, sinon dans le registre et les export csv/xls, le title on les voit
    
    //return finished value
    return $text;
}

function uwwtd_check_bool($bool){
    //nd@oieau.fr : 02/10/2018 : fix bug on value "0" with "empty" function
    if (!empty($bool) || $bool == '0') {
        // check format
        $bool = trim($bool);
        $bool = check_plain($bool);
        $bool = strtolower($bool);
        if(($bool == "false") || ($bool == "0")) {
            $bool = 0;
            return $bool;
        } elseif(($bool == "true")|| ($bool == "1")) {
            $bool = 1;
            return $bool;
        }
//     else{
// 		uwwtd_add_error(2, 4, 'The input data is not in boolean format (true or false).', $timestamp, $index, $node);
        return 0;
// 	}
    } else {
        return 0;
    }
}

function uwwtd_check_numeric($value) 
{
    $value = str_replace(",",".",trim($value));
	if(true === ctype_digit($value)) {
		return (int)$value;
	}
//     else{
// 		uwwtd_add_error(2, 4, 'The input data is not in numeric format.', $timestamp, $index, $node);
		return false;
// 	}
}

function uwwtd_check_decimal($value){
    $value = str_replace(",",".",trim($value));
	//in fact what is the max value for a real type in postgresql ?
	//if(is_numeric($value) && $value < bcpow(10,9)) {
        //nd@oieau.fr 17/07/2018  : bcpow is not include by default in php7
	if(is_numeric($value) && $value < 10000000000000) {
		return (float)$value;	
	}
//     else{
// 		uwwtd_add_error(2, 4, 'The input data is not in numeric or decimal format.', $timestamp, $index, $node);
		return false;
// 	}
}

function uwwtd_check_date($indate, &$message){
	$indate = trim($indate);
    $message = '';
	// if the date is empty return false
	if($indate == ''){
// 		uwwtd_add_error(1, 4, 'Date found to be empty.', $timestamp, $index, $node);
//         $message .= 'Date found to be empty.';
        $message .= $GLOBALS['uwwtd']['message']['date_empty_error'];
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
// 		uwwtd_add_error(1, 4, 'Unknown date format: '.$indate, $timestamp, $index, $node);
//         $message .= 'Unknown date format: '.$indate;
        $message .= str_replace('%', $indate, $GLOBALS['uwwtd']['message']['date_format_error']);
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
// 		uwwtd_add_error(1, 4, 'Entered date doesn\'t exist: '.$fullDate[0].'-'.$fullDate[1].'-'.$fullDate[2], $timestamp, $index, $node);
//         $message .= 'Entered date doesn\'t exist: '.$fullDate[0].'-'.$fullDate[1].'-'.$fullDate[2] . '.';
        $message .= str_replace('%', ($fullDate[0].'-'.$fullDate[1].'-'.$fullDate[2]), $GLOBALS['uwwtd']['message']['date_exist_error']);
	}

	// we change to drupal friendly format
	$correctDate = $fullDate[0].'-'.$fullDate[1].'-'.$fullDate[2].'T00:00:00';

	return (string)$correctDate;
}

function uwwtd_check_coordinate($coord, $type, &$message){
	$coord = trim($coord);
    $message = '';
    //exemple in PL 2010 csv file, ',' instead of '.'
	$coord = str_replace(",",".",$coord);

	// In europe a coordinate will never have more than 2 numbers before the decimal. This corrects coordinates that have misplaced decimals
	$first = mb_substr($coord, 0, 1, 'utf-8');
	if($first != '-') $first = '';
	$coord = str_replace("-", "", $coord);
	$exploded = explode('.', $coord);
	if(strlen($exploded[0]) > 2){
		$after = substr($exploded[0], 2);
// 		$coord = mb_substr($coord, $exploded[0], 2, 'utf-8').'.'.$after.$exploded[1];
// 		uwwtd_add_error(1, 4, 'Decimal point for this coordinate was inproperly placed', $timestamp, $index, $node);
        $message .= $GLOBALS['uwwtd']['message']['coord_decimal_error'];
        $coord = mb_substr($coord, 0, 2, 'utf-8') . '.' . $after . $exploded[1];
	}

	// In europe the bounding box for latitude is defined between 24 - 80 
	if($type == 'latitude'){
		if((float)$coord < 24 || (float)$coord > 80){
// 			uwwtd_add_error(1, 4, 'Latitude not found to be in Europe', $timestamp, $index, $node);
            $message .= $GLOBALS['uwwtd']['message']['coord_latitude_europe_error'];
		}
	}

	// In europe the bounding box for longitude is defined between -11 - 34 
	if($type == 'longitude'){
		if((float)$coord < -11 || (float)$coord > 34){
// 			uwwtd_add_error(1, 4, 'Longitude not found to be in Europe', $timestamp, $index, $node);
            $message .= $GLOBALS['uwwtd']['message']['coord_longitude_europe_error'];
		}
	}

	return $first.$coord;
}

function uwwtd_check_pourc($pourc, &$message){
    $pourc = str_replace(",",".",trim($pourc));
    $message = '';
	if(is_numeric($pourc))
	{
		if(($pourc >= "0") && ($pourc <="100")){
            $check = uwwtd_check_decimal($pourc);
            if (false === $check) {
                $message .= $GLOBALS['uwwtd']['message']['decimal_error'];
            }
            return $check;
		}else{
            $message .= $GLOBALS['uwwtd']['message']['percent_interval_error'];
// 			uwwtd_add_error($GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], 'The number must be between 0 and 100.', $timestamp, $index, $node);
			return false;
		}
	}else{
        $message .= $GLOBALS['uwwtd']['message']['percent_not_digit_error'];
// 		uwwtd_add_error($GLOBALS['uwwtd_errors_type']['type']['error'], $GLOBALS['uwwtd_errors_type']['category']['import data format'], 'The input data is not in digital format.', $timestamp, $index, $node);
		return false;
	}
}

function uwwtd_trace($message) {
    if (DEBUG_ALGO === true) {
        $file = drupal_realpath('public://data_errors/'.date("Ymd").'_trace.log');
        $r = fopen($file, 'a');
        fputs($r, date("[Y-m-d H:i:s] ") . $message . "\n");
        fclose($r);
    }
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

function uwwtd_sendfile($path)
{
    $basename = basename($path);
    ini_set("auto_detect_line_endings", false); //for Mac user    
    $aHeaders = array(
          'Content-Type'        => 'application/octet-stream',   
          'Content-Disposition' => 'attachment; filename="' . $basename . '"',
          'Expires'             => '0',
          'Cache-Control'       => 'must-revalidate',
          'Pragma'              =>'public',
          'Content-Length'      => filesize($path),
      );            
    foreach ($aHeaders as $sName => $sValue) {
        drupal_add_http_header($sName, $sValue);
    }
    readfile($path); 
    exit();     
}  

/**
 *
 */ 
function uwwtd_get_keyarray_from_conf($value){
    $result = array();
    $lines = explode("\n", $value);
    foreach($lines as $line){
        $l = explode('|', trim($line));
        $result[trim($l[0])] = trim($l[1]);
    }
    return $result;
}

/**
 *
 */ 
function uwwtd_get_array_from_conf($value){
    $result = array();
    $lines = explode("\n", $value);
    foreach($lines as $line){
        $result[] = trim($line);
    }

    return $result;
}

function uwwtd_get_distance_compliance($agg){
    require_once(drupal_get_path('module', 'uwwtd') . '/inc/register.inc');
    $annee = $agg->field_anneedata['und'][0]['value'];
    $dist_art4_treatment_pe = 0;
    $dist_art4_treatment_percent = 0;
    $dist_art4_perf_pe = 0;
    $dist_art4_perf_percent = 0;
    $dist_art5_treatment_pe = 0;
    $dist_art5_treatment_percent = 0;
    $dist_art5_perf_pe = 0;
    $dist_art5_perf_percent = 0;

    $totalLoadenteringPE = 0;
    $totalLoadenteringPercent = 0;
    $totalLoadenteringPE_P_NI = 0;
    $totalLoadenteringPercent_P_NI = 0;

    $art4complianceTreat = '';
    $art4compliancePerf = '';
    $art5complianceTreat = '';
    $art5compliancePerf = '';

    foreach ($agg->field_agguwwliste['und'] as $uwwtpnid) {
        $uwwtp = node_load($uwwtpnid['nid']);

        $treatment_required = uwwtd_get_register_treatment_required($uwwtp->field_uwwtreatmentrequired['und'][0]['value']);
        $treatmenttypeinplace = uwwtd_get_register_treatmenttype_inplace($uwwtp);
        $agglo_uwwtpnid = uwwtd_check_exist(uwwtd_get_siteid('agglo_uww', array('year' => $annee, 'aggCode' => $agg->field_inspireidlocalid['und'][0]['value'], 'uwwCode' => $uwwtp->field_inspireidlocalid['und'][0]['value'])));
        $agglo_uwwtp = node_load($agglo_uwwtpnid);

        $totalLoadenteringPE += ($agg->field_agggenerated['und'][0]['value'] * $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100);
        $totalLoadenteringPercent += $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];

        $collected_by_uww_for_this_agglo = $agg->field_agggenerated['und'][0]['value'] * ($agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'] / 100);

        // if($agg->field_aggart4compliance['und'][0]['value'] == 'NC') {
            // $art4_treatment_compliance = uwwtd_get_register_art4_treatement_type_compliance($treatment_required, $treatmenttypeinplace);
        // dsm($treatment_required);
        if (false === strpos($treatmenttypeinplace, '2')) {
            $dist_art4_treatment_pe += $collected_by_uww_for_this_agglo;
            $dist_art4_treatment_percent += $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];
            $art4complianceTreat = 'NC';
        }

        if ($uwwtp->field_uwwbod5perf['und'][0]['value'] != 'P' || $uwwtp->field_uwwcodperf['und'][0]['value'] != 'P') {
            $dist_art4_perf_pe += $collected_by_uww_for_this_agglo;
            $dist_art4_perf_percent += $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];
            $art4compliancePerf = 'NC';
        }
        // }

        // if($agg->field_aggart5compliance['und'][0]['value'] == 'NC') {
        $art5_treatment_compliance = uwwtd_get_register_art5_treatement_type_compliance($treatment_required, $treatmenttypeinplace);
        $art5_perf_compliance = '';
        if (!empty($treatment_required)) {
            $art5_perf_compliance = uwwtd_get_register_art5_monitoring_compliance(
                $treatment_required,
                $uwwtp->field_uwwcodperf['und'][0]['value'],
                $uwwtp->field_uwwbod5perf['und'][0]['value'],
                $uwwtp->field_uwwntotperf['und'][0]['value'],
                $uwwtp->field_uwwptotperf['und'][0]['value'],
                $uwwtp->field_uwwotherperf['und'][0]['value'],
                $uwwtp->field_uwww_required_rcab['und'][0]['value'],
                $uwwtp->field_uwww_required_rcac['und'][0]['value'],
                $uwwtp->field_uwww_required_rcaap['und'][0]['value'],
                $uwwtp->field_uwww_required_rcaan['und'][0]['value']);
        }
        if ($art5_treatment_compliance == 'NC') {
            $dist_art5_treatment_pe += $collected_by_uww_for_this_agglo;
            $dist_art5_treatment_percent += $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];
            $art5complianceTreat = 'NC';
        }
        if ($art5_perf_compliance == 'NC') {
            $dist_art5_perf_pe += $collected_by_uww_for_this_agglo;
            $dist_art5_perf_percent += $agglo_uwwtp->field_agglo_uww_perc_ent_uw['und'][0]['value'];
            $art5compliancePerf = 'NC';
        }
        if ($art5_perf_compliance == 'NR') {
            $dist_art5_perf_pe = '-';
            $dist_art5_perf_percent = '-';
        }
        // }
    }

    // dsm($totalLoadenteringPE);
    // dsm($totalLoadenteringPercent);

    $percentage_lost = $agg->field_aggc1['und'][0]['value'] - $totalLoadenteringPercent;
    $pe_lost = ($percentage_lost * ($agg->field_aggc1['und'][0]['value'] * $agg->field_agggenerated['und'][0]['value'])) / 10000;


    $dist_art4_treatment_pe +=  $pe_lost;
    $dist_art4_treatment_percent += $percentage_lost;
    $dist_art4_perf_pe += $pe_lost;
    $dist_art4_perf_percent += $percentage_lost;
    if ($percentage_lost > 1 || $pe_lost >= 2000) {
        $art4complianceTreat = 'NC';
        $art4compliancePerf = 'NC'; 
    }
    //$dist_art3_treatment_percent = 100 * $agg->field_aggpercwithouttreatment['und'][0]['value']/$agg->field_agggenerated['und'][0]['value'];

    // Set the colors of "Distance to compliance" table
                // Article 3
                if ($agg->field_aggart3compliance['und'][0]['value'] == 'C' || $agg->field_aggart3compliance['und'][0]['value'] == 'QC') {
                    $colorart3 = '#4f91e1';
                } elseif ($agg->field_aggart3compliance['und'][0]['value'] == 'NC') {
                    $colorart3 = '#d93c3c';
                } elseif ($agg->field_aggart3compliance['und'][0]['value'] == 'PD' 
                    && $agg->field_aggpercwithouttreatment['und'][0]['value'] > 0) 
                {
                    $colorart3 = '#FF5200';
                } else {
                    $colorart3 = '#a2a2a2';
                }
                // Article 4
                if ($art4complianceTreat == 'NC' &&
                    deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) === true &&
                    $dist_art4_perf_pe > 0) {
                    $colorart4T = '#d93c3c';
                } elseif ($art4complianceTreat == 'NC' && 
                    deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) === false) {
                    $colorart4T = '#FF5200';
                } else {
                    $colorart4T = '#4f91e1';
                }
                
               if ($art4compliancePerf == 'NC' &&
                    deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) == true &&
                    $dist_art4_perf_pe > 0) {
                    $colorart4P = '#d93c3c';
                } elseif ($art4compliancePerf == 'NC' && 
                        deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover4['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) == false) {
                    $colorart4P = '#FF5200';
                } else {
                    $colorart4P = '#4f91e1';
                }

                if ($agg->field_aggart4compliance['und'][0]['value'] == 'NR') {
                    $colorart4T = '#a2a2a2';
                    $colorart4P = '#a2a2a2';
                } 

                // Article 5
                if ($art5complianceTreat == 'NC' &&
                    deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) === true &&
                    $dist_art5_treatment_pe > 0) {
                    $colorart5T = '#d93c3c';
                } elseif ($art5complianceTreat == 'NC' &&
                    deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) === false) {
                    $colorart5T = '#FF5200';
                } else {
                    $colorart5T = '#4f91e1';
                }

                if ($art5compliancePerf == 'NC' &&
                    deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) === true &&
                    $dist_art5_perf_pe > 0) {
                    $colorart5P = '#d93c3c';
                } elseif ($art5compliancePerf == 'NC' &&
                    deadline_beforeorequal_to_referenceyear($agg->field_aggperiodover5['und'][0]['value'], $agg->field_anneedata['und'][0]['value']) === false) {
                    $colorart5P = '#FF5200';
                } else {
                    $colorart5P = '#4f91e1';
                }

                if ($agg->field_aggart5compliance['und'][0]['value'] == 'NR') {
                    $colorart5T = '#a2a2a2';
                    $colorart5P = '#a2a2a2';
                }

                if ($agg->field_aggart5compliance['und'][0]['value'] == 'PD' && $dist_art5_treatment_pe > 0) {
                    $colorart5P = '#FF5200';
                }

                if ($agg->field_aggart5compliance['und'][0]['value'] == 'PD' && $dist_art5_treatment_pe > 0) {
                    $colorart5T = '#FF5200';
                }

                if ($dist_art5_treatment_pe == '-') {
                    $colorart5P = '#a2a2a2';
                }
    
    
    
    $dtc = array(
        'art3'=>[
            'Equipment'=>[
                'pe'=>$agg->field_aggpercwithouttreatment['und'][0]['value']*$agg->field_agggenerated['und'][0]['value'], 
                'percent'=>$agg->field_aggpercwithouttreatment['und'][0]['value'], 
                'compliance'=>$agg->field_aggart3compliance['und'][0]['value'], 
                'color'=>$colorart3
            ],
        ],
        'art4'=>[
            'Equipment'=>['pe'=>$dist_art4_treatment_pe, 'percent'=>$dist_art4_treatment_percent, 'compliance'=>$art4complianceTreat, 'color'=>$colorart4T],
            'Performance'=>['pe'=>$dist_art4_perf_pe, 'percent'=>$dist_art4_perf_percent, 'compliance'=>$art4compliancePerf, 'color'=>$colorart4P],
        ],
        'art5'=>[
            'Equipment'=>['pe'=>$dist_art5_treatment_pe, 'percent'=>$dist_art5_treatment_percent, 'compliance'=>$art5complianceTreat, 'color'=>$colorart5T],
            'Performance'=>['pe'=>$dist_art5_perf_pe, 'percent'=>$dist_art5_perf_percent, 'compliance'=>$art5compliancePerf, 'color'=>$colorart5P],
        ],

        'art4_treat_pe' => $dist_art4_treatment_pe,
        'art4_treat_percent' => $dist_art4_treatment_percent,
        'art4_treatment_compliance' => $art4complianceTreat,
        'art4_perf_pe' => $dist_art4_perf_pe,
        'art4_perf_percent' => $dist_art4_perf_percent,
        'art4_perf_compliance' => $art4compliancePerf,
        'art5_treat_pe' => $dist_art5_treatment_pe,
        'art5_treat_percent' => $dist_art5_treatment_percent,
        'art5_treat_compliance' => $art5complianceTreat,
        'art5_perf_pe' => $dist_art5_perf_pe,
        'art5_perf_percent' => $dist_art5_perf_percent,
        'art5_perf_compliance' => $art5compliancePerf,
        
    );

    return $dtc;	
}


/**
 * Check if the first parameter year is before or equal the second parameter year
 * 
 * @param   string   $deadlinedate      a date formated YYYY-mm-dd
 * @param   string   $referenceyear     the reference year YYYY to compare with
 * @return  boolean   
 */ 
function deadline_beforeorequal_to_referenceyear($deadlinedate, $referenceyear) 
{
	$date = explode('-', $deadlinedate);
	return $date[0] <= $referenceyear;
}

//Permet de retourne un tableau de conf id/val pour les variable de conf afin d'assurer la compatibilité
function uwwtd_get_ind($conf){
    $ind=array();
    if(!empty($conf)){
        foreach($conf as $k=>$v){
            $ind[$k]=$v['k'];
        }
    }
    return $ind;
}

function uwwtd_get_percent($val1, $val2){
    if(is_null($val2) || $val2==0) return false;
    $pc = round(($val1-$val2)/$val2 *100, 1);
    return  ($val1>$val2?'+':'').$pc.' %';
}
