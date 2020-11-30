<?php 

function uwwtd_stats_sum($data, $year, $col){
    $out = [];
    if(!empty($data)){
        $tot  = 0;
        foreach($data as $cat=>$val){
            if(isset($val['data'][$year]) && isset($val['data'][$year][$col])){
                $out[$cat] =$val['data'][$year][$col];
                $tot +=$val['data'][$year][$col]; 
            }
        }
        $out['Total'] = $tot;
    }
    return $out;
}

function uwwtd_debug(){
    //=== Load libs
    module_load_include('inc','uwwtd','uwwtd.stats');
    module_load_include('inc','uwwtd','script/country/country');
    module_load_include('inc','uwwtd','inc/pages');
    
    /*
    module_load_include('inc','uwwtd','inc/register');
    module_load_include('inc','uwwtd','inc/inc/register.config');
    */
    
    
    //===Set options and const
    $year = 2018;
    $option['field_anneedata_value'] = $year;
    $country_code = variable_get('siif_eru_country_code', '-1');
    $country_name = variable_get('siif_eru_country_name');
    if(isset($GLOBALS['pop_ms'][strtoupper($country_code)][$year])){
        $population =$GLOBALS['pop_ms'][strtoupper($country_code)][$year];
    }else{
        $population =$GLOBALS['pop_ms'][strtoupper($country_code)]['2018'];
    }
        
    //===Load data
    //=$national_compliance
    $national_compliance = uwwtd_stat_national_compliance($year, []);
    $national_compliance_nb = uwwtd_stats_sum($national_compliance, $year, 'nb');
    $national_compliance_pe = uwwtd_stats_sum($national_compliance, $year, 'pe');
    
    //=$stat_collect
    $stat_collect = uwwtd_stat_collect_data($year, []);
    $stat_collect_values = array_combine(array_column($stat_collect[$year], 'label'), array_column($stat_collect[$year], 'value'));
    $stat_collect_values['Total'] = array_sum(array_column($stat_collect[$year], 'value'));
    
    //=$treatmenttype
    $treatmenttype = uwwtd_stat_national_uwwtp($year, [],'treatmenttype');
    $treatmenttype_nb = uwwtd_stats_sum($treatmenttype, $year, 'nb');
    $treatmenttype_pe = uwwtd_stats_sum($treatmenttype, $year, 'pe');
    
    //= Design capacity 
    $dataUwwtp = uwwtd_get_data_uwwtp($option);
    $uwwtp_ids = [];
    $nbPhysicalCapacity = 0;
	foreach($dataUwwtp as $uwwtp){        
        if($uwwtp['field_status_value']==1 && !in_array($uwwtp['nid'], $uwwtp_ids)){
            /*
            if($uwwtp['field_uwwtreatmenttype_value'] == "P"){
                $nbPrimaryTreatement = $nbPrimaryTreatement+1;
            }elseif($uwwtp['field_uwwtreatmenttype_value'] == "S"){
                $nbSecondaryTreatement = $nbSecondaryTreatement +1;
            }elseif($uwwtp['field_uwwtreatmenttype_value'] == "MS"){
                $nbOtherTreatement = $nbOtherTreatement+1;
            }*/
            $nbPhysicalCapacity = $uwwtp['field_physicalcapacityactivity_value']+ $nbPhysicalCapacity;
            //Keep id of uwwtp in memory in order to prevent double counting
            $uwwtp_ids[] = $uwwtp['nid'];
        }
	}
    
    //= Legal compliance
    $legal_compliance = uwwtd_stat_national_distance_to_compliance($year, []);
    
    //ms_level
    $ms_level = uwwtd_stat_national_ms_level_data($year, []);
    
    //Reuse
    $dcp_reuse = uwwtd_get_data_dcp(['field_anneedata_value'=>$year]);
    foreach($data as $k=>$line){
        if($line['field_dcpirrigation_value']==''){
            unset($data[$k]);
        }
    }
    
    //$path = drupal_get_path('module', 'uwwtd') . '/script/country/country.inc';
    //include($path);

    
    $output = '<h2>Summary</h2>';

    $output .= '<p><b>'.$country_name.' compliance rate: '. round(100 * ($national_compliance_nb['Compliant']/$national_compliance_nb['Total']),1) .'%</b></p>';
    $output .= '<p>In '.$year.', '.$country_name.' counted '.$national_compliance_nb['Total'].' agglomerations generating ≥2,000 p.e. each, of which:</p>';
    $output .= '<ul>';
    $output .= '    <li>'.format_plural($national_compliance_nb['Compliant'], '1 agglomeration',  '@count agglomerations').', generating '.uwwtd_format_number($national_compliance_pe['Compliant']).' p.e. of waste water, complied with the requirements of the Directive; and</li>';
    $output .= '    <li>'.format_plural($national_compliance_nb['Not Compliant'], '1 agglomeration',  '@count agglomerations').', generating '.uwwtd_format_number($national_compliance_pe['Not Compliant']).' p.e. of waste water, did not comply with the requirements of the Directive.</li>';
    //Manage PD & NR case ?
    $output .= '</ul>';
$output .= '<p>The waste water load generated in '.$country_name.' in '.$year.' was reported to be '.uwwtd_format_number($national_compliance_pe['Total']).' p.e. </p>';

//Display this text only if compliance art 3 is < 100 %
$output .= '<p>In '.$year.', '.$country_name.' had not yet reached the target for collecting and treating waste water to fully comply with the Directive.</p>';
$output .= '<ul>
        <li>'.round(100 * $stat_collect_values['Discharged without treatment']/$stat_collect_values['Total'], 1).' % of the waste water load must still be collected (approximately '.uwwtd_format_number(round($stat_collect_values['Discharged without treatment'], -2)).' p.e.); and</li>
        <li>===TODO===== 0.1% of the collected waste water load from agglomerations generating >10,000 p.e. and discharging into sensitive areas must still undergo more stringent treatment in line with the requirements of the Directive (approximately 1,200 p.e.).====</li>
    </ul>
    <p>All of the collected waste water load underwent secondary treatment in line with the requirements of the Directive.</p>';

$output .='<p>'.round(100 * $stat_collect_values['Individual and Appropriate Systems']/$stat_collect_values['Total'], 1).'% of waste water load is addressed by means of individual or other appropriate systems (IAS) i.e. non-centralised sanitation systems (approximately '.uwwtd_format_number(round($stat_collect_values['Individual and Appropriate Systems'], -2)).' p.e.).  </p>';

$output .='<p>For the '.$national_compliance_nb['Total'].' agglomerations, there are '.$treatmenttype_nb['Total'].' urban waste water treatment plants, with a total design capacity of '.uwwtd_format_number(round($nbPhysicalCapacity, -3)).' p.e., of which:</p>
<ul>
    <li>'.format_plural($treatmenttype_nb['Secondary treatment'], '1 plant' , '@count plants').' equipped with technology for primary and secondary treatment </li>
    <li>'.format_plural($treatmenttype_nb['More stringent treatment'], '1 plant' , '@count plants').' equipped with technology for more stringent treatment than secondary.</li>
</ul>
';
$output.='<h3>Compliance rate by Article</h3>';

$art3_C = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Compliant"}->{"Article 3"} ;
$art3_NC = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Not compliant"}->{"Article 3"};
$art3_nb =  (integer)$art3_C+ $art3_NC;
$art4_C = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Compliant"}->{"Article 4"} ;
$art4_NC = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Not compliant"}->{"Article 4"};
$art4_nb =  (integer)$art4_C+ $art4_NC;
$art5_C = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Compliant"}->{"Article 5"} ;
$art5_NC = (integer)$legal_compliance['comparisonsCompliancesByAgg'][$year]->{"Not compliant"}->{"Article 5"};
$art5_nb =  (integer)$art5_C+ $art5_NC;

$art3_pe_C = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Compliant"}->{"Article 3"} ;
$art3_pe_NC = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Not compliant"}->{"Article 3"};
$art3_pe_nb =  (integer)$art3_pe_C+ $art3_pe_NC;
$art4_pe_C = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Compliant"}->{"Article 4"} ;
$art4_pe_NC = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Not compliant"}->{"Article 4"};
$art4_pe_nb =  (integer)$art4_pe_C+ $art4_pe_NC;
$art5_pe_C = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Compliant"}->{"Article 5"} ;
$art5_pe_NC = (integer)$legal_compliance['comparisonsCompliancesByPe'][$year]->{"Not compliant"}->{"Article 5"};
$art5_pe_nb =  (integer)$art5_pe_C+ $art5_pe_NC;

$output.='
<table class="table">
    <thead>
        <tr>
            <th></th>
            <th colspan="3" align="center">Agglomeration</th>
            <th colspan="3" align="center">Waste water load</th>
        </tr>
        <tr>
            <th>'.$country_name.' in '.$year.'</th>
            <th>Total<br/>[No]</th>
            <th>Comply<br/>[No]</th>
            <th>Comply<br/>[%]</th>
            <th>Total<br/>[p.e.]]</th>
            <th>Comply<br/>[p.e.]]</th>
            <th>Comply<br/>[%]</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><b>Article 3 (collection)</b></td>
            <td>'. $art3_nb .'</td>
            <td>'.$art3_C.'</td>
            <td>'.round(100 * $art3_C/$art3_nb , 1).' %</td>
            <td>'. uwwtd_format_number($art3_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art3_pe_C).'</td>
            <td>'.round(100 * $art3_pe_C/$art3_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>Article 4 (secondary treatment)</b></td>
            <td>'. $art4_nb .'</td>
            <td>'.$art4_C.'</td>
            <td>'.round(100 * $art4_C/$art4_nb , 1).' %</td>
            <td>'. uwwtd_format_number($art4_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art4_pe_C).'</td>
            <td>'.round(100 * $art4_pe_C/$art4_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>Article 5 (more stringent treatment)</b></td>
            <td>'. $art5_nb .'</td>
            <td>'.$art5_C.'</td>
            <td>'.round(100 * $art5_C/$art5_nb , 1).' %</td>
            <td>'. uwwtd_format_number($art5_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art5_pe_C).'</td>
            <td>'.round(100 * $art5_pe_C/$art5_pe_nb , 1).' %</td>
        </tr>
    </tbody>
</table>
';
$output.='
<p><i>*The total [No of agglomerations] includes only those agglomerations that must provide secondary treatment / or more stringent treatment than secondary before discharging waste water from their treatment plants.</i></p>
<p><i>**The total [p.e. waste water load] is the sum of the load of all of the agglomerations that must provide secondary treatment or more stringent treatment than secondary before discharging waste water from their treatment plants. </i></p>
<p align="right"><i>Data source for table: Register, summary of MS data</i></p>
';

$output.='<h3>Distance to target </h3>';
$output.='
<table class="table">
    <thead>
        <tr>
            <th></th>
            <th colspan="3" align="center">Waste water load</th>
        </tr>
        <tr>
            <th>'.$country_name.' in '.$year.'</th>
            <th>Target<br/>[p.e.]</th>
            <th>Distance to target<br/>[p.e.]</th>
            <th>Distance to target<br/>[%]</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><b>Collection</b></td>
            <td>'. uwwtd_format_number($art3_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art3_pe_NC).'</td>
            <td>'.round(100 * $art3_pe_NC/$art3_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>Secondary treatment</b></td>
            <td>'. uwwtd_format_number($art4_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art4_pe_NC).'</td>
            <td>'.round(100 * $art4_pe_NC/$art4_pe_nb , 1).' %</td>
        </tr>
        <tr>
            <td><b>More stringent treatment</b></td>
            <td>'. uwwtd_format_number($art5_pe_nb) .'</td>
            <td>'.uwwtd_format_number($art5_pe_NC).'</td>
            <td>'.round(100 * $art5_pe_NC/$art5_pe_nb , 1).' %</td>
        </tr>
    </tbody>
</table>
';
$output.='
    <p><i>*The target [p.e.] for secondary treatment represents the waste water load (collected and not collected) from all agglomerations that must be subject to secondary treatment, regardless of whether or not the agglomeration complies with Article 3.</i></p>
    <p><i>** The target for more stringent treatment represents the waste water load (collected and not collected) from all agglomerations generating >10,000 p.e. and discharging into sensitive areas (regardless of whether or not the agglomeration complies with Article 3).</i></p>
    <p align="right"><i>Data source for table: Register, summary of MS data</i></p>
';



$art17_mslevel = uwwtd_stat_art17_mslevel(['year'=>$year]);
$art17_mslevel_data = [];
if(!empty($art17_mslevel )){
    foreach($art17_mslevel as $v){
        $v['soeinvtot'] = $v['soecinvtp']+$v['soecinvcol'];
        $v['nb_year'] = 1 + (integer)substr($v['soecendyear'],0,4) - (integer)substr($v['soecstartyear'],0,4);
        $v['yearly_inv'] = $v['soeinvtot']/$v['nb_year'];
        
        $art17_mslevel_data[$v['soecperiod']][substr($v['soecstartyear'],0,4)] = $v;
        
    }
    $cu_invest = array_column($art17_mslevel_data['CU'], 'yearly_inv');
    $cu_investment_inh = (array_sum($cu_invest)*1000000/$population)/count($cu_invest);
    $exp_invest = array_column($art17_mslevel_data['EXP'], 'yearly_inv');
    $exp_invest_inh = (array_sum($exp_invest)*1000000/$population)/count($exp_invest);
    $years = array_keys($art17_mslevel_data['EXP']);
    $min_k = min($years);
    $max_k = max($years);
    $begin_y = (integer)substr($art17_mslevel_data['EXP'][$min_k]['soecstartyear'],0,4);
    $end_y =  (integer)substr($art17_mslevel_data['EXP'][$max_k]['soecendyear'],0,4);
    
    
    $output.='<p>#CURRENT == The expected annual investment costs per capita for installing and renewing waste water collecting systems and treatment plants is '.uwwtd_format_number(round($cu_investment_inh)).' EUR/inhabitant/year.</p>';
    $output.='<p>#EXPECTED == The expected annual investment costs per capita for installing and renewing waste water collecting systems and treatment plants is '.uwwtd_format_number(round($exp_invest_inh)).' EUR/inhabitant/year.</p>';
    $output.='<p>';
    $output.='The investment needs  for ensuring adequate urban waste water collection and treatment, i.e. compliance with the Directive, as estimated by the national authorities and included in their national plan, are at the level of EUR '.array_sum(array_column($art17_mslevel_data['EXP'], 'soeinvtot')).' million for the period '.$begin_y.'-'.$end_y.'.';
    $output.='This covers works on treatment plants, with the forecasted investment cost of EUR '.array_sum(array_column($art17_mslevel_data['EXP'], 'soecinvcol')).' million ('.$begin_y.'-'.$end_y.'), and works on collecting systems and/or IAS, with a forecasted investment cost of EUR '.array_sum(array_column($art17_mslevel_data['EXP'], 'soecinvtp')).' million ('.$begin_y.'-'.$end_y.').';
    $output.='</p>';
    $output.='<p>';    
   $output.='===TODO======However, as indicated in a recent OECD study, '. $country_name.' would need to increase their investments further to reach and maintain compliance.';
    $output.='The OECD projected that the financing needs by 2030 (total cumulative additional expenditures for water supply and sanitation) is EUR 0.6 billion. ========';
    $output.='</p>';
    
}




$no_ms_level = [];
if(isset($ms_level[$year]['sludge']['production']) && $ms_level[$year]['sludge']['production'] > 0){
    $output.='<p>';
    $output.='For the reference year '.$year.', '.$country_name.' reported the production of '.uwwtd_format_number($ms_level[$year]['sludge']['production']).' tonnes of sludge. ';
    if(!empty($ms_level[$year]['sludge']['destination'])){
        arsort($ms_level[$year]['sludge']['destination']);
        $output.='The sludge is mainly ';
        $slude_use = [];
        foreach($ms_level[$year]['sludge']['destination'] as $k=>$v){
            $rate = round(100 * $v /$ms_level[$year]['sludge']['production']);
            if($rate >= 10){
                $slude_use[]= strtolower($GLOBALS['ms_level']['sludge'][$k]['label']).' ('.$rate.' %)';
            }else{
                break;
            }
        }
        $output.= implode(' or ',$slude_use) .'.';
        if($ms_level[$year]['remark']!=''){
            $output.=$ms_level[$year]['remark'];
        }
    }else{
        $no_ms_level[] = 'sludge destination';
    }
     //re-used for soil and agriculture (83%) or disposed in landfills (17%).';
    $output.='</p>';
}
else{
    $no_ms_level[] = 'sludge';
}



if(isset($ms_level[$year]['waste_water']['rate of re-used']) || !empty($dcp_reuse)){
    $output.='<p>';
    
    if(!empty($dcp_reuse)){
        $uses = array_unique(array_column($dcp_reuse, 'field_dcpirrigation_value'));
        if(!empty($uses)){
            foreach($uses as $k=>$v){
                if(!is_null($v) && isset($GLOBALS['uwwtd']['reuse_type'][$v])){
                    $uses[$k] = strtolower($GLOBALS['uwwtd']['reuse_type'][$v]);
                }else{
                    unset($uses[$k]);
                }
            }
        }
        
        $output.='At '.format_plural(count($dcp_reuse), '1 discharge point' , '@count discharge points').' , treated waste water is reused for '.implode(' or ', $uses).'.';
    }
    
    if($ms_level[$year]['waste_water']['rate of re-used'] > 0) {
        $output.= 'In '. $year .' '.$ms_level[$year]['waste_water']['rate of re-used'].' % of treated waste water was reported to be reused';
    }
    if($ms_level[$year]['waste_water']['Re-used']!=''){
        $output.=' ('.$ms_level[$year]['waste_water']['Re-used'].')';
    }
    $output.='.</p>';
}else{
    $no_ms_level[] = 'waste water reuse';
}

$output.='<p>===TODO====== storm water overflows and urban run-off ========</p>';
if(!empty($no_ms_level)){
    $output.='<p>No information was reported on '.implode(', ', $no_ms_level).' by '.$country_name.' because <b>##################</b>.</p>';
}

    return '<div class="view-content">'.$output.'</div>';
    
        
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
