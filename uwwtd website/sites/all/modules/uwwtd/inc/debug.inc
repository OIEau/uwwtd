<?php 

function uwwtd_debug(){
    $year = 2018;
    //=== Load libs
    module_load_include('inc','uwwtd','uwwtd.stats');
    //return uwwtd_stats_summary_chapter($year);

    /*
    //fix link between agglomeration and inactive uwwtp
    //In inactive UWWTP  case the tag "UwwtpAgglos" is not defined and the link between agglomeration-uwwtp is not created
    //but we can build the link using the "field_uwwtdaggid" (aggCode give in the UWWTP node) 
    //field_uwwtdaggid
    //Launch a request in order to find orphan UWWTP 
    
    module_load_include('inc','uwwtd','inc/data_center');
    $annee = '2018';
    $ids = [];
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
         //$ids[] = $row;
         //dsm($row);
         $siteAggId = $annee.'_agg_'.uwwtd_check_text($row['aggid']);
         $aggNid = uwwtd_check_exist($siteAggId);
         uwwtd_link_uww_agglo_node($annee, $aggNid, $row['nid']);
         //break;
     }
    //dsm(count($ids));
    */
    
    
    
   /* 
   module_load_include('inc','uwwtd','inc/data_center');
    //module_load_include('php','uwwtd','inc/ShapeFileAutoloader');
    module_load_include('php','uwwtd','inc/ShapeFile');
    module_load_include('php','uwwtd','inc/ShapeFileException');

    // importing the downloaded shapefile
    $uri = "public://data_shapes/UWWTDSensitiveAreaCatchments_IE.shp";
    $path = drupal_realpath($uri);
    $shp = new ShapeFile($path, ShapeFile::FLAG_SUPPRESS_Z);
    $annee = 2016;
    
    while ($record = $shp->getRecord(ShapeFile::GEOMETRY_WKT)) {
        $node = rca_set_geo_obj($record, $annee);
    }
    */
    
    /*
    module_load_include('inc','uwwtd','inc/data_center');
    $uri = 'public://data_sources/uwwtdart15fi2016_1554997752.xml';
    $datas = uwwtd_xml_to_array($uri);
    $data = $datas['Agglomerations']['Row'][3];
    dsm($data);
    $repCode =  uwwtd_check_text($datas['MSLevel']['Row']['repCode']);
    $timestamp = time();
    $annee = 2016;
    
    uwwtd_update_agg_node($data, $newFile, $repCode, $annee, $timestamp, $i);
    */
    
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
