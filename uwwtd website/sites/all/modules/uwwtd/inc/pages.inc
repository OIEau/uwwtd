<?php 

/**
 * Generates the sensitive areas stats table.
 */

 //============================================================================= DATA
 
/**
 * Get all agglomeration data for a year
 */
function uwwtd_get_data_agglomeration($option){
    //        dsm(__FUNCTION__);
    $cache_name = uwwtd_get_cache_key_agglomeration($option);
//     dsm($cache_name);
    $cache = '';
	$putincache =true;
    /*
	if(isset($option['rbd']) && $option['rbd'] !="All"){
		$putincache =false;
	}*/
    if ($putincache && UWWTD_CACHE_ACTIVE === true) {
        $cache = cache_get($cache_name, 'cache');
    }
//     dsm($cache);
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return  $cache->data;
    }

    $data = array();
//field_agguwwliste_nid
    $query = 'SELECT 
            n.nid,
            n.title,
            f_annee.field_anneedata_value,
            f_aggcode.field_inspireidlocalid_value,
            f_agggenerated.field_agggenerated_value,
            f_art3.field_aggart3compliance_value,
            f_art4.field_aggart4compliance_value,
            f_art5.field_aggart5compliance_value,
            f_art6.field_aggart6compliance_value,
            f_comp.field_aggcompliance_value,
            f_nuts.field_regionnuts_value,
            f_aggc2.field_aggc2_value,
            f_aggc1.field_aggc1_value,
            f_aggpercwithouttreatment.field_aggpercwithouttreatment_value,
            f_status.field_status_value,
            st_astext(st_transform(f_geo.the_geom, 4326)) wkt,
            f_article17_agglo.field_article17_agglo_nid,
            rbd.eurbdcode as rbd,
            rbd.rbdname,
            COUNT(f_uwwtp_agglo.entity_id) as nb_uwwtp,
            SUM(uwwtp_capa.field_physicalcapacityactivity_value) as sum_uwwtp,
            '.($option['field_anneedata_value']>=2012?'nut.nuts_id':'nut.n3nm').' as n3nm
    FROM {node} n
        LEFT join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT join
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        LEFT join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        LEFT join
            {field_data_field_aggart3compliance} f_art3
                ON n.nid = f_art3.entity_id
        LEFT join
            {field_data_field_aggart4compliance} f_art4
                ON n.nid = f_art4.entity_id
        LEFT join
            {field_data_field_aggart5compliance} f_art5
                ON n.nid = f_art5.entity_id
        LEFT join
            {field_data_field_aggart6compliance} f_art6
                ON n.nid = f_art6.entity_id
        LEFT join
            {field_data_field_aggcompliance} f_comp
                ON n.nid = f_comp.entity_id
        LEFT join
            {field_data_field_regionnuts} f_nuts
                ON n.nid = f_nuts.entity_id
        LEFT join
            {field_data_field_aggc2} f_aggc2
                ON n.nid = f_aggc2.entity_id
        LEFT join
            {field_data_field_aggc1} f_aggc1
                ON n.nid = f_aggc1.entity_id
        LEFT join
            {field_data_field_aggpercwithouttreatment} f_aggpercwithouttreatment
                ON n.nid = f_aggpercwithouttreatment.entity_id
        LEFT join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
        LEFT join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		LEFT join
            {field_data_field_article17_agglo} f_article17_agglo
                ON n.nid = f_article17_agglo.entity_id
		LEFT join
            {field_data_field_agguwwliste} f_uwwtp_agglo
                ON n.nid = f_uwwtp_agglo.entity_id
        LEFT JOIN
            {field_data_field_physicalcapacityactivity} uwwtp_capa
                ON f_uwwtp_agglo.field_agguwwliste_nid=uwwtp_capa.entity_id
        LEFT join 
            river_basin rbd
                ON ST_Intersects(ST_Transform(f_geo.the_geom,4326), rbd.the_geom4326)
        '.(
            $option['field_anneedata_value']>=2012?
            'LEFT JOIN nuts_rg_v2013 nut ON f_nuts.field_regionnuts_value = nut.nuts_id'
            :'LEFT JOIN regions_nuts nut ON f_nuts.field_regionnuts_value = nut.n3cd'
        ).'        
    WHERE n.type = :typenode
        AND f_annee.field_anneedata_value = :annee
    GROUP BY n.nid,
            n.title,
            f_annee.field_anneedata_value,
            f_aggcode.field_inspireidlocalid_value,
            f_agggenerated.field_agggenerated_value,
            f_art3.field_aggart3compliance_value,
            f_art4.field_aggart4compliance_value,
            f_art5.field_aggart5compliance_value,
            f_art6.field_aggart6compliance_value,
            f_comp.field_aggcompliance_value,
            f_nuts.field_regionnuts_value,
            n3nm,
            f_aggc2.field_aggc2_value,
            f_aggc1.field_aggc1_value,
            f_aggpercwithouttreatment.field_aggpercwithouttreatment_value,
            f_status.field_status_value,
            f_geo.the_geom,
            f_article17_agglo.field_article17_agglo_nid,
            rbd.eurbdcode,
            rbd.rbdname 
	ORDER BY n.title 	
    ';
    $param = array(
        ':typenode' => 'agglomeration',
        ':annee' => $option['field_anneedata_value'],
    );
    
   // dsm($param);
    try {
        
        $result = db_query($query, $param);
        while($row = $result->fetchAssoc()) {
            //override PD and QC value, we don't want to display QC and PD (for now)
            if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggcompliance_value'] ])) {
                $row['field_aggcompliance_value'] = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggcompliance_value'] ];
            }
            if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart3compliance_value'] ])) {
                $row['field_aggart3compliance_value'] = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart3compliance_value'] ];
            }
            if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart4compliance_value'] ])) {
                $row['field_aggart4compliance_value'] = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart4compliance_value'] ];
            }
            if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart5compliance_value'] ])) {
                $row['field_aggart5compliance_value'] = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart5compliance_value'] ];
            }
            if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart6compliance_value'] ])) {
                $row['field_aggart6compliance_value'] = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_aggart6compliance_value'] ];
            }

            $row['field_agggenerated_value_format'] = is_null($row['field_agggenerated_value']) ? '' : uwwtd_format_number($row['field_agggenerated_value']);
            $row['sum_uwwtp_format'] = is_null($row['sum_uwwtp']) ? '' : uwwtd_format_number($row['sum_uwwtp']);
            $row['field_aggcompliance_value_format'] = is_null($row['field_aggcompliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggcompliance_value']];
            
            //Alter compliance value for display legal compliance (hierachical)
            $row['field_aggart3compliance_value_format'] = is_null($row['field_aggart3compliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggart3compliance_value']];
            if($row['field_aggart3compliance_value'] == 'NC' && $row['field_aggart4compliance_value']== 'C'){
                $row['field_aggart4compliance_value_format'] = $GLOBALS['uwwtd']['ui']['field']['compliance_label']['NC'];
                $row['field_aggart4compliance_value'] = 'NC';
            }
            else{
                $row['field_aggart4compliance_value_format'] = is_null($row['field_aggart4compliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggart4compliance_value']];
            }
            if($row['field_aggart4compliance_value'] == 'NC' && $row['field_aggart5compliance_value']== 'C'){
                $row['field_aggart5compliance_value_format'] = $GLOBALS['uwwtd']['ui']['field']['compliance_label']['NC'];
                $row['field_aggart5compliance_value'] = 'NC';
            }
            else{
               $row['field_aggart5compliance_value_format'] = is_null($row['field_aggart5compliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggart5compliance_value']];
            }
            
            //Only for Portugal case, not use for legal compliance.
            $row['field_aggart6compliance_value_format'] = is_null($row['field_aggart6compliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggart6compliance_value']];
            
            
            $row['field_aggc1_value_format'] = is_null($row['field_aggc1_value']) ? '' : uwwtd_format_number($row['field_aggc1_value'], 1);
            $row['field_aggc2_value_format'] = is_null($row['field_aggc2_value']) ? '' : uwwtd_format_number($row['field_aggc2_value'], 1);
            $row['field_aggpercwithouttreatment_value_format'] = is_null($row['field_aggpercwithouttreatment_value']) ? '' : uwwtd_format_number($row['field_aggpercwithouttreatment_value'], 1);

			if($row['field_article17_agglo_nid'] == "" || is_null($row['field_article17_agglo_nid'])){
				$row['field_article17_agglo_value_format'] =  'No';
			}
			else{
				$row['field_article17_agglo_value_format'] = 'Yes';
			}
			
			if (isset($row['field_regionnuts_value']) && isset($GLOBALS['uwwtd']['ui']['field']['agglomeration'][$row['field_regionnuts_value']])) {
                $row['field_regionnuts_value'] = $GLOBALS['uwwtd']['ui']['agglomeration'][ $row['field_regionnuts_value'] ];
            }
			if (isset($row['field_dcpreceivingwater_value']) && isset($GLOBALS['uwwtd']['ui']['field']['agglomeration'][$row['field_dcpreceivingwater_value']])) {
                $row['field_dcpreceivingwater_value'] = $GLOBALS['uwwtd']['ui']['agglomeration'][ $row['field_dcpreceivingwater_value'] ];
            }
            $data[] = $row;
        }
		if($putincache){
			cache_set($cache_name, $data, 'cache', CACHE_PERMANENT);
		}
        
    }catch(Exception $e) {
        dsm($e->getMessage());
    }
    return $data;
}

function uwwtd_get_data_uwwtp($option)
{
    $cache_name = uwwtd_get_cache_key_uwwtp($option);
    $cache  = '';
    $putincache =true;
	
    /*if(isset($option['rbd']) && $option['rbd'] !="All"){
		$putincache =false;
	}*/
    if ($putincache && UWWTD_CACHE_ACTIVE === true) {
        $cache = cache_get($cache_name, 'cache');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return  $cache->data;
    }

    $data = array();
    $query = '
    WITH uwwtp_min_agg AS(
        SELECT n.nid, n.type, n.title 
        FROM {node} n
            LEFT join
              {field_data_field_anneedata f_annee}
                ON n.nid = f_annee.entity_id
            left join
              {field_data_field_status f_status}
                ON n.nid = f_status.entity_id
            LEFT join
              {field_data_field_agguwwliste f_uwwtp_agglo}
                ON f_uwwtp_agglo.field_agguwwliste_nid = n.nid
            LEFT join
            {field_data_field_agggenerated f_agggenerated}
                ON f_uwwtp_agglo.entity_id = f_agggenerated.entity_id
        WHERE n.type = :typenode
            AND f_agggenerated.field_agggenerated_value >= 2000
             AND f_annee.field_anneedata_value = :annee
        GROUP BY n.nid
    )
    select distinct on (n.nid) n.nid,
        n.title,
		f_annee.field_anneedata_value,
		f_uwwcode.field_inspireidlocalid_value,
		f_comp.field_uwwcompliance_value,
		f_primarytreatment.field_uwwprimarytreatment_value,
		f_secondarytreatment.field_uwwsecondarytreatment_value,
		f_nremoval.field_uwwnremoval_value,
		f_premoval.field_uwwpremoval_value,
		f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value,
		f_regionnuts.field_regionnuts_value,
		f_uwwbod5perf.field_uwwbod5perf_value,
		f_uwwcodperf.field_uwwcodperf_value,
		f_uwwntotperf.field_uwwntotperf_value,
		f_uwwptotperf.field_uwwptotperf_value,
		f_uwwtreatmenttype.field_uwwtreatmenttype_value,
		f_physicalcapacityactivity.field_physicalcapacityactivity_value,
		f_uwwspecification.field_uwwspecification_value,
		f_uwwboddischarge.field_uwwboddischarge_value,
		f_uwwcoddischarge.field_uwwcoddischarge_value,
		f_uwwndischarge.field_uwwndischarge_value,
		f_uwwpdischarge.field_uwwpdischarge_value,
        f_status.field_status_value,
		st_astext(st_transform(f_geo.the_geom, 4326)) wkt,
		f_article17_uwwtp.field_article17_uwwtp_nid,
		f_dcpre_uwwtp.field_dcpreceivingwater_value,
        rbd.eurbdcode as rbd,
        rbd.rbdname,
        '.($option['field_anneedata_value']>=2012?'nut.nuts_id':'nut.n3nm').' as n3nm
    from {node} n
        JOIN uwwtp_min_agg
            ON n.nid = uwwtp_min_agg.nid
        LEFT join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT join
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        LEFT join
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        LEFT join
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        LEFT join
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        LEFT join
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        LEFT join
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        LEFT join
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        LEFT join
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        LEFT join
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        LEFT join
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        LEFT join
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        LEFT join
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        LEFT join
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        LEFT join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        LEFT join
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        LEFT join
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        LEFT join
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        LEFT join
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        LEFT join
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        LEFT join
            {field_data_field_status} f_status
                ON n.nid = f_status.entity_id
        LEFT join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		LEFT join
            {field_data_field_article17_uwwtp} f_article17_uwwtp
                ON n.nid = f_article17_uwwtp.entity_id
		LEFT join
            {field_data_field_uwwdcpliste} f_dcp_uwwtp
                ON n.nid = f_dcp_uwwtp.entity_id
		LEFT join
            {field_data_field_dcpreceivingwater} f_dcpre_uwwtp
                ON f_dcp_uwwtp.field_uwwdcpliste_nid = f_dcpre_uwwtp.entity_id
        LEFT join 
            river_basin rbd
                ON ST_Intersects(ST_Transform(f_geo.the_geom,4326), rbd.the_geom4326)
        '.(
            $option['field_anneedata_value']>=2012?
            'LEFT JOIN nuts_rg_v2013 nut ON f_regionnuts.field_regionnuts_value = nut.nuts_id'
            :'LEFT JOIN regions_nuts nut ON f_regionnuts.field_regionnuts_value = nut.n3cd'
        ).' 
    WHERE n.type = :typenode
        and f_annee.field_anneedata_value = :annee
    ORDER BY n.nid
    ';
 // dsm($query);

 $nb = strlen($option['field_anneedata_value']);
  if($nb == 1){
	   $param = array(
        ':typenode' => 'uwwtp',
        ':annee' => $option,
        );
  }else{
	  $param = array(
        ':typenode' => 'uwwtp',
        ':annee' => $option['field_anneedata_value'],
        );
  }
  
    
    try {
        $result = db_query($query, $param);

        while($row = $result->fetchAssoc()) {
            //override PD and QC value, we don't want to display QC and PD (for now)
            if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_uwwcompliance_value'] ])) {
                $row['field_uwwcompliance_value'] = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $row['field_uwwcompliance_value'] ];
            }

            $row['field_uwwloadenteringuwwtp_value_format'] = is_null($row['field_uwwloadenteringuwwtp_value']) ? '' : uwwtd_format_number($row['field_uwwloadenteringuwwtp_value']);

            $row['field_uwwcompliance_value_format'] = is_null($row['field_uwwcompliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_uwwcompliance_value']];

            $row['field_uwwprimarytreatment_value_format'] = is_null($row['field_uwwprimarytreatment_value']) ? '' : $GLOBALS['uwwtd']['ui']['yes_no'][$row['field_uwwprimarytreatment_value']];
            $row['field_uwwsecondarytreatment_value_format'] = is_null($row['field_uwwsecondarytreatment_value']) ? '' : $GLOBALS['uwwtd']['ui']['yes_no'][$row['field_uwwsecondarytreatment_value']];
            $row['field_uwwnremoval_value_format'] = is_null($row['field_uwwnremoval_value']) ? '' : $GLOBALS['uwwtd']['ui']['yes_no'][$row['field_uwwnremoval_value']];
            $row['field_uwwpremoval_value_format'] = is_null($row['field_uwwpremoval_value']) ? '' : $GLOBALS['uwwtd']['ui']['yes_no'][$row['field_uwwpremoval_value']];

            $row['field_uwwbod5perf_value_format'] = is_null($row['field_uwwbod5perf_value']) ? '' : $GLOBALS['uwwtd']['ui']['test_pass'][$row['field_uwwbod5perf_value']];
            $row['field_uwwcodperf_value_format'] = is_null($row['field_uwwcodperf_value']) ? '' : $GLOBALS['uwwtd']['ui']['test_pass'][$row['field_uwwcodperf_value']];
            $row['field_uwwntotperf_value_format'] = is_null($row['field_uwwntotperf_value']) ? '' : $GLOBALS['uwwtd']['ui']['test_pass'][$row['field_uwwntotperf_value']];
            $row['field_uwwptotperf_value_format'] = is_null($row['field_uwwptotperf_value']) ? '' : $GLOBALS['uwwtd']['ui']['test_pass'][$row['field_uwwptotperf_value']];

            $row['field_uwwtreatmenttype_value_format'] = is_null($row['field_uwwtreatmenttype_value']) ? '' : $GLOBALS['uwwtd']['ui']['treatment_type_in_place'][$row['field_uwwtreatmenttype_value']];

            $row['field_physicalcapacityactivity_value_format'] = is_null($row['field_physicalcapacityactivity_value']) ? '' : uwwtd_format_number($row['field_physicalcapacityactivity_value']);

            $row['field_uwwboddischarge_value_format'] = is_null($row['field_uwwboddischarge_value']) ? '' : uwwtd_format_number($row['field_uwwboddischarge_value'], 2);
            $row['field_uwwcoddischarge_value_format'] = is_null($row['field_uwwcoddischarge_value']) ? '' : uwwtd_format_number($row['field_uwwcoddischarge_value'], 2);
            $row['field_uwwndischarge_value_format']   = is_null($row['field_uwwndischarge_value']) ? '' : uwwtd_format_number($row['field_uwwndischarge_value'], 2);
            $row['field_uwwpdischarge_value_format']   = is_null($row['field_uwwpdischarge_value']) ? '' : uwwtd_format_number($row['field_uwwpdischarge_value'], 2);

			if($row['field_article17_uwwtp_nid'] == "" || is_null($row['field_article17_uwwtp_nid'])){
				$row['field_article17_uwwtp_value_format'] =  'No';
			}
			else{$row['field_article17_uwwtp_value_format'] = 'Yes';}
			if (isset($GLOBALS['uwwtd']['ui']['field']['uwwtp'][$row['field_regionnuts_value']])) {
                $row['field_regionnuts_value'] = $GLOBALS['uwwtd']['ui']['uwwtp'][ $row['field_regionnuts_value'] ];
            }
			if (isset($GLOBALS['uwwtd']['ui']['field']['uwwtp'][$row['field_dcpreceivingwater_value']])) {
                $row['field_dcpreceivingwater_value'] = $GLOBALS['uwwtd']['ui']['uwwtp'][ $row['field_dcpreceivingwater_value'] ];
            }
			
			$data[] = $row;
        }
        cache_set($cache_name, $data, 'cache', CACHE_PERMANENT);
    }catch(Exception $e) {
        dsm($e->getMessage());
    }

     // dsm($data);
    return $data;
}

function uwwtd_get_data_dcp($option){
    $cache_name = uwwtd_get_cache_key_dcp($option);
    $cache  ='';
    if (UWWTD_CACHE_ACTIVE === false) {
        $cache = cache_get($cache_name, 'cache');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return  $cache->data;
    }
    $data = array();

    $query = 'select n.title,
                     n.nid,
                     f_annee.field_anneedata_value,
                     f_dcpcode.field_inspireidlocalid_value,
                     zt.field_rcatype_value as zonetype,
                     wbt.field_dcpwaterbodytype_value as rcacat,
                     uwwtp.title as uwwtp_name,
                     uwwtp.nid as uwwtp_id,
                     f_comp.field_uwwcompliance_value,
                     f_physicalcapacityactivity.field_physicalcapacityactivity_value,
                     wwt.field_uwwwastewatertreated_value,
                     rca.title as rca_name,
                     rca.nid as rca_id,
                     ir.field_dcpirrigation_value,
                     st_astext(st_transform(f_geo.the_geom, 4326)) wkt
    from {node} n
        LEFT join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        LEFT join
            {field_data_field_inspireidlocalid} f_dcpcode
                ON n.nid = f_dcpcode.entity_id
        LEFT join
            {field_data_field_dcpwaterbodytype} wbt
                ON n.nid = wbt.entity_id        
        LEFT join
            {field_data_field_rcatype} zt
                ON n.nid = zt.entity_id        
        LEFT join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
        LEFT join
            {field_data_field_dcpirrigation} ir
                ON n.nid = ir.entity_id
        LEFT join 
            {field_data_field_linked_treatment_plants} tp
                ON n.nid = tp.entity_id
        LEFT join
            {node} uwwtp
                ON uwwtp.nid = tp.field_linked_treatment_plants_nid
        LEFT join
            {field_data_field_uwwcompliance} f_comp
                ON uwwtp.nid = f_comp.entity_id
        LEFT join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON uwwtp.nid = f_physicalcapacityactivity.entity_id
        LEFT join
            {field_data_field_uwwwastewatertreated} wwt
                ON uwwtp.nid = wwt.entity_id
        LEFT join 
            {field_data_field_linked_receiving_areas} rc
                ON n.nid = rc.entity_id
        LEFT join
            {node} rca
                ON rca.nid = rc.field_linked_receiving_areas_nid
                
    where n.type = :typenode
    and f_annee.field_anneedata_value = :annee
    ';
//     dsm($query);

//     $param = array(
//         ':typenode' => 'discharge_point',
// //         ':annee' => '(select max(field_anneedata_value) from drupal_field_data_field_anneedata)',
//     );
    $param = array(
        ':typenode' => 'discharge_point',
        ':annee' => $option['field_anneedata_value'],
        );
    try {
        $result = db_query($query, $param);
        while($row = $result->fetchAssoc()) {
            
            $row['zonetype_format'] =  (isset($GLOBALS['uwwtd_value']['rcatype_labels'][$row['zonetype']])? $GLOBALS['uwwtd_value']['rcatype_labels'][$row['zonetype']] :$GLOBALS['uwwtd_value']['rcatype_labels']['default']);
            $row['rcacat_format'] =  (isset($row['rcacat']) && isset($GLOBALS['uwwtd_value']['rcacat_labels'][$row['rcacat']])? $GLOBALS['uwwtd_value']['rcacat_labels'][$row['rcacat']] :'');
            $row['rca_format'] =  l($row['rca_name'], 'node/'.$row['rca_id']);
            $row['uwwtp_format'] =  l($row['uwwtp_name'], 'node/'.$row['uwwtp_id']);
            $row['field_uwwcompliance_value_format'] =  $GLOBALS['uwwtd']['all_compliance'][$row['field_uwwcompliance_value']];
            $row['field_physicalcapacityactivity_value_format'] =  uwwtd_format_number($row['field_physicalcapacityactivity_value']);
             $row['field_uwwwastewatertreated_value_format'] =  uwwtd_format_number($row['field_uwwwastewatertreated_value']);
            $row['field_dcpirrigation_value_format'] =  (isset($GLOBALS['uwwtd']['reuse_type'][$row['field_dcpirrigation_value']])?$GLOBALS['uwwtd']['reuse_type'][$row['field_dcpirrigation_value']]:'');
            
            
            $data[] = $row;
        }
        cache_set($cache_name, $data, 'cache', CACHE_PERMANENT);
    }catch(Exception $e) {
        dsm($e->getMessage());
    }
    return $data;
}

 
function uwwtd_get_data_sensitive_areas($option){
    //Note : Receving area = sensitive area
    
    //        dsm(__FUNCTION__);
    $cache_name = uwwtd_get_cache_key_sensitive_area($option);
    //     dsm($cache_name);
    $cache = '';
	$putincache =false;
    /*
	if(isset($option['rbd']) && $option['rbd'] !="All"){
		$putincache =false;
	}*/
    if ($putincache && UWWTD_CACHE_ACTIVE === true) {
        $cache = cache_get($cache_name, 'cache');
    }
    //     dsm($cache);
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return  $cache->data;
    }

    $data = array();
    
    //field_rca_sensitive_area
    $query = uwwtd_get_query_data_sensitive_areas();

    // add rca state
    
    $param = array(
        ':typenode' => 'receiving_area',
        ':annee' => $option['field_anneedata_value'],
    );
    try {
        $result = db_query($query, $param);
        /*
        Article 52 applied : traitement plus poussé sur station > 10000.
        Article 54 applied : 75% phosphore + 75 % nitrates de la somme des entering load
        Article 58 applied : état membre complet : 75% phosphore + 75 % nitrates de la somme des entering load
        */
        
        $rcatype = array_flip($GLOBALS['uwwtd_value']['rcatype']);
        while($row = $result->fetchAssoc()) {
            
            if($node = node_load($row['nid'])){
                $row['aggs'] = count($node->field_linked_agglomerations['und']);
            }
            
            $row['tot_design_capacity_format'] =  ($row['tot_design_capacity'] ? uwwtd_format_number($row['tot_design_capacity']) : '-');
            $row['tot_entering_load_format'] =  ($row['tot_entering_load'] ? uwwtd_format_number($row['tot_entering_load']) : '-');

            $zonetype_formatA = '';
            if (!empty($row['zonetype']) && !empty($GLOBALS['uwwtd_value']['rcatype_labels'][$row['zonetype']])) {
                //TODO : test if values mapping is needed between  10th report values and old ones 
                $row['zonetype_raw'] = $row['zonetype'];
                $zonetype_formatA = $GLOBALS['uwwtd_value']['rcatype_labels'][$row['zonetype']];
            }
            $zonetype_formatB = '';
            if (!empty($GLOBALS['uwwtd_value']['rcatype_labels']['default'])) {
                $zonetype_formatB = $GLOBALS['uwwtd_value']['rcatype_labels']['default'];
            }
            $row['zonetype_format'] =  (!empty($row['zonetype']) && isset($GLOBALS['uwwtd_value']['rcatype_labels'][$row['zonetype']]) ? $zonetype_formatA : $zonetype_formatB);
            $row['spezonetype_format'] =  (!empty($row['zonetype_spe']) && isset($GLOBALS['uwwtd_value']['rca_spe_type_labels'][$row['zonetype_spe']]) ? $zonetype_formatA : $zonetype_formatB);
            
            if($row['rca52']=='') $row['rca52'] = 0; 
            if($row['rca54']=='') $row['rca54'] = 0; 
            if($row['rca58']=='') $row['rca58'] = 0; 
            
            $row['rca52_format'] = $GLOBALS['uwwtd']['ui']['yes_no'][$row['rca52']];
            $row['rca54_format'] = $GLOBALS['uwwtd']['ui']['yes_no'][$row['rca54']];
            $row['rca58_format'] = $GLOBALS['uwwtd']['ui']['yes_no'][$row['rca58']];
            $row['area_format'] =  ($row['area']?uwwtd_format_number($row['area']/1000000).' km²':'-');
            $data[] = $row;
        }
		if($putincache){
			cache_set($cache_name, $data, 'cache', CACHE_PERMANENT);
		}
        
    }catch(Exception $e) {
        dsm($e->getMessage());
    }
    return $data;
    
}

//======================================================================PAGES ====================================================

function uwwtd_page_agglomeration_compliance($type_export = ''){
    $title = 'UWWTD Agglomerations- Compliance map';
    drupal_set_title($title);
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    libraries_load('d3.drupal');
    
//     drupal_add_js(drupal_get_path('module', 'uwwtd') . '/../../libraries/d3/d3.v3.min.js')
	$type = "agglomeration";
    $option = array();
    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['agglomeration'];
    $option['default']['order'] = $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_agggenerated_value_format']['sort_field'];
    $option['default']['sort_mod'] = $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_agggenerated_value_format']['sort_mod'];
    $option['default']['sort'] = 'desc';
    $year = null;
    if(isset($_REQUEST['field_anneedata_value']) && $_REQUEST['field_anneedata_value']!=""){
        $year = $_REQUEST['field_anneedata_value'];
    }
    $option['allyear'] = uwwtd_get_all_year();
	$option['allnuts'] = uwwtd_get_all_nuts($type, $year);
	$option['allrbd'] = uwwtd_get_all_rbd($type);
	
    $filter = uwwtd_get_form_filter($option);
    $preset_name = 'agg_compliance_map';
    $preset = openlayers_preset_load($preset_name);
    $map    = openlayers_build_map($preset->data);

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_agglo_compliance($filter['filter']);

    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }
	$data = uwwtd_get_data_agglomeration($filter['filter']);
    if(uwwtd_data_contain_art17($data, 'field_article17_agglo_nid')){
    //if(isset($data[0]['field_article17_agglo_nid']) || ($data[0]['field_article17_agglo_nid']!= '')){
        $list_field = array(
            'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_agggenerated_value'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_agggenerated_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggcompliance_value'     => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggcompliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_aggart3compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart3compliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_aggart4compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart4compliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_aggart5compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart5compliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            //'field_aggart6compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart6compliance_value'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_article17_agglo_value'  	=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_article17_agglo_value_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
        );
    }
    else{
       $list_field = array(
            'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_agggenerated_value'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_agggenerated_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggcompliance_value'     => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggcompliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_aggart3compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart3compliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_aggart4compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart4compliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_aggart5compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart5compliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            //'field_aggart6compliance_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_aggart6compliance_value'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
            );
    }
// dsm($list_field);
    $form = uwwtd_filter_form($list_field, $filter);


//     dsm($data);
    //e.vincent / bruno want to show only agglo > 2000
    $filter_force = array();
    $filter_force['filter']['field_agggenerated_value_op'] = '>';
    $filter_force['filter']['field_agggenerated_value']['value'] = variable_get('min_aggenerated_display', '1999');
    if (variable_get('display_only_active', '1') == 1) {
        $filter_force['filter']['field_status_value'] = '1';
    }
	// dsm($filter);
    uwwtd_filter_data($filter, $data, $filter_force); 
    
//     uwwtd_filter_data($filter, $data);
    $exists = array();
	foreach($data as $art17){
		if(isset($art17['field_article17_agglo_value_format']) && ($art17['field_article17_agglo_value_format']== 'Yes')){
			$exists[] = array('Yes' => $art17['field_article17_agglo_nid']);
			break;
		}else{
			$exists[] = array('No' => "");
		}
	}
	foreach($exists as $exist){
		 if(array_key_exists('Yes', $exist)){
			 $art17 = "Yes";
			 break;
		}elseif(!isset($filter['filter']['field_article17_agglo_value_art'])){
			 $art17 = "No";
		}
	}
    /*
	  if($art17 == "No"){
		  unset($header[9]);
	  }
	  unset($header[8]);
    */
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['agglomeration'], $data);
//     dsm($data);

    $optionstyle = array();
    //get the color for the point
    $optionstyle['color']['list'] = uwwtd_get_point_color_from_preset($preset);
    $optionstyle['color']['field'] = 'field_aggcompliance_value_format';
    //get the size of the point
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['field'] = 'field_agggenerated_value';
    $optionstyle['popup_title_type'] = 'agglomeration';
	
    $optionstyle['popup'] = array(
        'field_agggenerated_value_format' => array('label' => 'Generated load (p.e.)'),
        'field_aggcompliance_value_format' => array('label' => 'Compliance'),
        'field_aggart3compliance_value_format' => array('label' => 'Collection compliance'),
        'field_aggart4compliance_value_format' => array('label' => '2nd treatment compliance'),
        'field_aggart5compliance_value_format' => array('label' => '3rd treatment compliance'),
//         'field_aggart6compliance_value' => array('label' => 'Article 6 compliance'),
//         'field_aggc1_value' => array('label' => 'Collecting system'),
//         'field_aggc2_value' => array('label' => 'Individual and Appropriate Systems (IAS)'),
//         'field_aggpercwithouttreatment_value' => array('label' => 'Discharge without treatment'),
    );


    //todo format data in table
    $graph = array();
    foreach($data as $row) {
        if (false === isset($graph[$row['field_aggcompliance_value_format']])) {
            $graph[$row['field_aggcompliance_value_format']]['nb'] = 0;
            $graph[$row['field_aggcompliance_value_format']]['field_agggenerated_value'] = 0;
        }
        $graph[$row['field_aggcompliance_value_format']]['nb']++;
        $graph[$row['field_aggcompliance_value_format']]['field_agggenerated_value'] += $row['field_agggenerated_value'];

		if(uwwtd_data_contain_art17($data, 'field_article17_agglo_nid')){
			$titleV = truncate_utf8($row['title'], 20, $wordsafe = TRUE, $add_ellipsis = TRUE, $min_wordsafe_length = 10);
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($titleV, uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($titleV, 'node/'.$row['nid'], array('html'=>true))),
				'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_agggenerated_value_format'=> (empty($row['field_agggenerated_value']) ? '0' : $row['field_agggenerated_value']),
                        'sum_uwwtp_format'=> $row['sum_uwwtp'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
                'rbd'=>$row['rbd'].' '.$row['rbdname'],
				'field_aggcompliance_value_format'=>$row['field_aggcompliance_value_format'],
				'field_aggart3compliance_value_format'=>$row['field_aggart3compliance_value_format'],
				'field_aggart4compliance_value_format'=>$row['field_aggart4compliance_value_format'],
				'field_aggart5compliance_value_format'=>$row['field_aggart5compliance_value_format'],
				'field_article17_agglo_value_format'=>$row['field_article17_agglo_value_format'],                
			));
		}else{
			 $rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_agggenerated_value_format'=> $row['field_agggenerated_value'],
                'sum_uwwtp_format'=> $row['sum_uwwtp'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
                'rbd'=>$row['rbd'].' '.$row['rbdname'],
				'field_aggcompliance_value_format'=>$row['field_aggcompliance_value_format'],
				'field_aggart3compliance_value_format'=>$row['field_aggart3compliance_value_format'],
				'field_aggart4compliance_value_format'=>$row['field_aggart4compliance_value_format'],
				'field_aggart5compliance_value_format'=>$row['field_aggart5compliance_value_format'],
			));
		}

    }

    $header = array();
    $header_export = array();


    $keys = array_keys($rows[0]['data']);
    foreach($GLOBALS['uwwtd']['ui']['field']['agglomeration'] as $field => $info) {
        if (!empty($field) && !empty($keys)) {
            if(in_array($field, $keys)){
                $header[$field] = array(
                    'data' => $info['label'],
                    'field' => $field,
                    'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                    'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
                );
                $header_export[] = $info['label'];
            }
        }
    }

    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
	    	// Restructuration des tableaux :
	    	$rowsToBeExported = array();
	    	foreach ($header_export as $header) {
	    		$rowsToBeExported[0][] = $header;
	    	}
	    	foreach ($rows as $row) {
	    		if (!empty($row['data'])) {
	    			$rowsToBeExported[] = $row['data'];
	    		}
	    	}
	    	
	    	// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
	    	$filename = 'agglomeration_compliance_'. $filter['filter']['field_anneedata_value']. '.xls';

	    	uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
        	uwwtd_export_data($type_export, 'agglomeration_compliance_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    	}
    }


    $features['compliance_map'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
//     dsm($features);
    $rendermap = uwwtd_build_map($map, $features);
    unset($features);
    

    //first graph number agglo
    $data_pie_1 = array();
    $data_table_1 = array();
    $data_pie_2 = array();
    $data_table_2 = array();
    $total_1 = 0;
    $total_2 = 0;
    
//     dsm($optionstyle);
    foreach($graph as $compliance_format => $detail) {
        $seg = array();
        $seg['value'] = $detail['nb'];
        $seg['valueformat'] = number_format($detail['nb']);
        $seg['label'] = $compliance_format;
        $seg['color'] = $optionstyle['color']['list'][$compliance_format];
        $data_pie_1[] = $seg;
        $data_table_1[]=array(
            uwwtd_format_number($detail['nb']),
            $compliance_format
        );
        $total_1 += $detail['nb'];
        
        
        $seg2 = array();
        $seg2['value'] = (float)$detail['field_agggenerated_value'];
        $seg2['valueformat'] = number_format($detail['field_agggenerated_value']).' pe';
        $seg2['label'] = $compliance_format;
        $seg2['color'] = $optionstyle['color']['list'][$compliance_format];
        $data_pie_2[] = $seg2;
        
        $data_table_2[]=array(
            uwwtd_format_number($detail['field_agggenerated_value']).' pe',
            $compliance_format
        );
        $total_2 += $detail['field_agggenerated_value'];
    }
    $data_table_1[] = array( uwwtd_format_number($total_1), '');
    $data_table_2[] = array( uwwtd_format_number($total_2), '');
    
    //Graph 1
    $graph1Id = 'flot-auto-identifier-1';
    $front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
    $front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($data_pie_1).",  '".$graph1Id."');});</script>";
    $render_graph_nb = $front;
    $header_1 = array(t('Number of agglomeration'),t('Compliance'));
    $render_table_graph_nb = theme('table', array('rows'=>$data_table_1, 'header'=>$header_1));
    
    //Graph 2
    $graph2Id = 'flot-auto-identifier-2';
    $front2 = '<svg id="'. $graph2Id .'" style="margin:10px;"/>';
    $front2 .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($data_pie_2).",  '".$graph2Id."');});</script>";
    $render_graph = $front2;
    
    
    
    $header_2 = array(t('Generated load (p.e.)'),t('Compliance'));
    $render_table_graph = theme('table', array('rows'=>$data_table_2, 'header'=>$header_2));

    unset($data_pie_1,$data_pie_2, $data_table_1, $data_table_2);

    $build['tablesort_table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
    );

   //============================Render the page ==> TODO use a true template/theme function 
    require_once(drupal_get_path('module', 'uwwtd') . '/uwwtd.graphs.inc');
    
    // Récupération des années possibles :
    $yearsPossibles = getYearsPossibles();
    
    // Récupération des données des graphs :
    $results = uwwtd_stat_national_compliance(ALL_YEARS, $yearsPossibles);
    $frontAndBackGraph1And2 = getRow2Graph1and2($results, ALL_YEARS);
    
    // Création du code html pour ces graphs :
    $htmlGraphsMultiYears = '';
    $htmlGraphsMultiYears = getHtmlPlusHtmlChartsPies($htmlGraphsMultiYears, t("Agglomeration : Generated load by compliance"),
    	t("Total generated load by compliance (population equivalent – p.e.)"), 'graph_generatedByCompliance', $frontAndBackGraph1And2[0],  $frontAndBackGraph1And2[1],
    	t("Number of agglomerations by compliance (number of agglomerations)"), 'graph_agglomerationByCompliance', $frontAndBackGraph1And2[2], $frontAndBackGraph1And2[3],
    	false, $yearsPossibles);

    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
        $page.=  '<div class="rows map-row" >'.$rendermap.'</div>';
        // Todo : affichage des graph multi ou single année :
        $displayGraphsMultiYears = false;
        if ($displayGraphsMultiYears) {
            $page .= '<div class="rows charts-row" >'. $htmlGraphsMultiYears .'</div>';
        } else {
            $page .= '<div class="rows charts-row" >'
                    .'<div class="cell charts-rows-cell">'
                      .'<div class="cell-content">' 
                        .uwwtd_page_render_flipcard(t("Global compliance by number of agglomeration (&gt;2000 pe)"), $render_graph_nb, $render_table_graph_nb,'')
                      .'</div>'
                    .'</div>'
                    .'<div class="cell charts-rows-cell">'
                      .'<div class="cell-content">'
                        .uwwtd_page_render_flipcard(t("Global compliance by Generated load (p.e.)"), $render_graph, $render_table_graph,'')
                      .'</div>'
                    .'</div>';
            }
        $page .=  '</div>'
                . '<div class="rows data-table-row">'
                 . '<div class="data-table-header">'
                  . '<b>'.count($rows) ." results</b>"		
                    . '<div class="hideprint export-result"> | Download results: '
                        .l('csv', 'agglomerations/compliance/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
                        .l('xls', 'agglomerations/compliance/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
                    .'</div>'
                 .'</div>'
                    
                    . '<div class="view-content content-table">'. drupal_render($build) . '</div>'
                
                    . '<!--markerprinttable-->'  //use for print wkhtmltopdf
                .'</div>'
         ;
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}


function uwwtd_page_agglomeration_connection($type_export = ''){
    $title = 'Agglomeration collection map and list';
    drupal_set_title($title);
    
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    libraries_load('d3.drupal');
    
    d3_draw(array(
        'type'=>'ColumnChart',
        'id'=>'default-empty-d3'
    ));
    
    $type = "agglomeration";
	$option = array();
    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment'];   
    $option['default']['order'] = $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_agggenerated_value_format']['sort_field'];
    $option['default']['sort_mod'] = $GLOBALS['uwwtd']['ui']['field']['agglomeration']['field_agggenerated_value_format']['sort_mod'];
    $option['default']['sort'] = 'desc';
    if(isset($_REQUEST['field_anneedata_value']) && $_REQUEST['field_anneedata_value']!=""){
        $year = $_REQUEST['field_anneedata_value'];
    }
    $option['allyear'] = uwwtd_get_all_year();
	$option['allnuts'] = uwwtd_get_all_nuts($type, $year);
	$option['allrbd'] = uwwtd_get_all_rbd($type);
    $filter = uwwtd_get_form_filter($option);

    $preset_name = 'agglomeration';   //layer agglomeration_treatment 'Agglomeration connection map and list'
    $preset = openlayers_preset_load($preset_name);
    $map    = openlayers_build_map($preset->data);

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_agglo_treatment($filter['filter']);
    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }

	$data = uwwtd_get_data_agglomeration($filter['filter']);
	if(uwwtd_data_contain_art17($data, 'field_article17_agglo_nid')){
       $list_field = array(
            'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_agggenerated_value'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_agggenerated_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggc2_value'             => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggc2_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggc1_value'             => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggc1_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggpercwithouttreatment_value'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggpercwithouttreatment_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_article17_agglo_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_article17_agglo_value_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
        );
   }else{
       $list_field = array(
            'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_agggenerated_value'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_agggenerated_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggc2_value'             => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggc2_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggc1_value'             => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggc1_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_aggpercwithouttreatment_value'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggpercwithouttreatment_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
            );
    }

    $form = uwwtd_filter_form($list_field, $filter, 'views-exposed-form-agglomeration-connection-page');



//     uwwtd_filter_data($filter, $data);
    // dsm($data);
    //e.vincent / bruno want to show only agglo > 2000
    $filter_force = array();
    $filter_force['filter']['field_agggenerated_value_op'] = '>';
    $filter_force['filter']['field_agggenerated_value']['value'] = variable_get('min_aggenerated_display', '1999');
    if (variable_get('display_only_active', '1') == 1) {
        $filter_force['filter']['field_status_value'] = '1';
    }    
    uwwtd_filter_data($filter, $data, $filter_force);
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment'], $data);

    $optionstyle = array();
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_piechart');
    $optionstyle['weight']['field'] = 'field_agggenerated_value';
    $optionstyle['piechart'] = uwwtd_get_piechart_from_preset($preset, 'openlayers_plus_behavior_piechart');
    $optionstyle['piechart_weight_style'] = $map['behaviors']['openlayers_plus_behavior_piechart']['styles'];

    $optionstyle['popup_title_type'] = 'agglomeration';
    $optionstyle['popup'] = array(
        'field_agggenerated_value_format' => array('label' => 'Generated load (p.e.)'),
        'field_aggcompliance_value_format' => array('label' => 'Compliance'),
        'field_aggart3compliance_value_format' => array('label' => 'Collection compliance'),
        'field_aggart4compliance_value_format' => array('label' => '2nd treatment compliance'),
        'field_aggart5compliance_value_format' => array('label' => '3rd treatment compliance'),
//         'field_aggart6compliance_value' => array('label' => 'Article 6 compliance'),
        'field_aggc1_value_format' => array('label' => 'Collecting system'),
        'field_aggc2_value_format' => array('label' => 'Individual and Appropriate Systems (IAS)'),
        'field_aggpercwithouttreatment_value_format' => array('label' => 'Discharge without treatment'),
    );
    $graph = array();
        $field_agggenerated_value = 0;
        $graph['field_aggc1_value']=array('label'=>$GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggc1_value_format']['label'],'value' => 0);
        $graph['field_aggc2_value']=array('label'=>$GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggc2_value_format']['label'],'value' => 0);
        $graph['field_aggpercwithouttreatment_value']=array('label'=>$GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment']['field_aggpercwithouttreatment_value_format']['label'],'value' => 0);
    foreach($data as $row) {
        $field_agggenerated_value   += $row['field_agggenerated_value'];
        $graph['field_aggc1_value']['value'] += $row['field_aggc1_value'] / 100  * $row['field_agggenerated_value'];
        $graph['field_aggc2_value']['value'] += $row['field_aggc2_value'] / 100  * $row['field_agggenerated_value'];
        $graph['field_aggpercwithouttreatment_value']['value'] += $row['field_aggpercwithouttreatment_value'] / 100  * $row['field_agggenerated_value'];

		if(uwwtd_data_contain_art17($data, 'field_article17_agglo_nid')){
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_agggenerated_value_format'=>$row['field_agggenerated_value_format'],
                'sum_uwwtp_format'=>$row['sum_uwwtp_format'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
				'rbd'=>$row['rbd'].' '.$row['rbdname'],
				'field_aggc1_value_format'=>$row['field_aggc1_value_format'],
                'field_aggc2_value_format'=>$row['field_aggc2_value_format'],
				'field_aggpercwithouttreatment_value_format'=>$row['field_aggpercwithouttreatment_value_format'],
				'field_article17_agglo_value_format'=>$row['field_article17_agglo_value_format'],
                
			));
		}else{
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_agggenerated_value_format'=>$row['field_agggenerated_value_format'],
                'sum_uwwtp_format'=>$row['sum_uwwtp_format'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
                'rbd'=>$row['rbd'].' '.$row['rbdname'],
                'field_aggc1_value_format'=>$row['field_aggc1_value_format'],
				'field_aggc2_value_format'=>$row['field_aggc2_value_format'],
				'field_aggpercwithouttreatment_value_format'=>$row['field_aggpercwithouttreatment_value_format'],
			));
		}
    }
    
    $graph['field_aggc2_value']['percent'] = $graph['field_aggc2_value']['value'] * 100 / $field_agggenerated_value;
    $graph['field_aggc1_value']['percent'] = $graph['field_aggc1_value']['value'] * 100 / $field_agggenerated_value;
    $graph['field_aggpercwithouttreatment_value']['percent'] = $graph['field_aggpercwithouttreatment_value']['value'] * 100 / $field_agggenerated_value;
    $graph['total'] = array('label'=>t("Total"), 'value'=>$field_agggenerated_value, 'percent'=>100);
    
    $header = array();
    $header_export = array();
    $keys = array_keys($rows[0]['data']);
    foreach($GLOBALS['uwwtd']['ui']['field']['agglomeration_treatment'] as $field => $info) {
        if(in_array($field, $keys)){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }
    
    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
    		// Restructuration des tableaux :
    		$rowsToBeExported = array();
    		foreach ($header_export as $header) {
    			$rowsToBeExported[0][] = $header;
    		}
    		foreach ($rows as $row) {
    			if (!empty($row['data'])) {
    				$rowsToBeExported[] = $row['data'];
    			}
    		}
    	
    		// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
    		$filename = 'agglomeration_connection_'. $filter['filter']['field_anneedata_value']. '.xls';
    		uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
    		uwwtd_export_data($type_export, 'agglomeration_connection_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    	}
    }
    
    $features['agglomeration_treatment'] = uwwtd_build_feature($data, $optionstyle, $piechart);

    $rendermap = uwwtd_build_map($map, $features);
    unset($features);
    $data_pie = array();
    $xaxis = array();
    $i = 0;
    foreach($graph as $field => $detail) {
        if($field!='total'){
            $seg = array();
            $seg['value'] = $detail['value'];
            $seg['valueformat'] = number_format($detail['value']);
            $seg['label'] = $detail['label'];
            $seg['color'] = $optionstyle['piechart'][$field]['color'];
            $data_pie_1[] = $seg;
            
        }
    }
    if(!empty($graph)){
        foreach($graph as $k=>$item){
            $graph[$k]['label'] = substr($item['label'], 0,strpos($item['label'],'('));
            $graph[$k]['value'] = uwwtd_format_number($item['value']);
            $graph[$k]['percent'] = uwwtd_format_number($item['percent'],1);
            
        }
    }
    $render_table_graph = theme('table', array('rows'=>$graph, 'header'=>array(t('Treatment'),t('Total in p.e.'),t('Rate in %'))));

    $build['tablesort_table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
    );
    //build export data on ODT,XML,CSV,DOC...

    //todo save view and map and .install update map/style/overlay
    //$render_graph = theme('flot_graph', uwwtd_page_build_flot_piechart($data_pie, 'flot-auto-identifier-1'));
    //Graph 1
    $graph1Id = 'flot-auto-identifier-1';
    $front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
    $js = "jQuery(document).ready(function(){display_piechart_custom(".json_encode($data_pie_1).",  '".$graph1Id."');})";
    //drupal_add_js(array('inline'=>$js), array('defer'=>true));
    $front .= "<script>".$js."</script>";
    $render_graph = $front;

    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
            $page.= $piechart                  //style="page-break-after:always;"
            . '<div class="rows map-row" >'.$rendermap.'</div>'
            
            
            . '<div class="rows charts-row">'
                .'<div>'
                  .'<div class="cell-content">'
                    .uwwtd_page_render_flipcard(t("Generated load (p.e.) distribution"), $render_graph, $render_table_graph,'')
                  .'</div>'
                .'</div>'
                .'</div>'
            .'</div>'
            . '<div class="rows data-table-row">'
                . '<div class="rows data-table-row">'
			 . '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'agglomerations/connection/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'agglomerations/connection/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
                . '<div class="view-content content-table">'. drupal_render($build) . '</div>'
                . '<!--markerprinttable-->' //use for print wjhtmltopdf
            .'</div>'
     ;
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}

function uwwtd_page_uwwtps_compliance($type_export = ''){
    $title = 'UWWTD Treatment Plants- Compliance map';
    drupal_set_title($title);  //todo
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    libraries_load('d3.drupal');
    
    $type = "uwwtp";
	$option = array();
    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp'];
    $option['default']['order'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['sort_field'];
    $option['default']['sort_mod'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['sort_mod'];
    $option['default']['sort'] = 'desc';
    if(isset($_REQUEST['field_anneedata_value']) && $_REQUEST['field_anneedata_value']!=""){
        $year = $_REQUEST['field_anneedata_value'];
    }
    $option['allyear'] = uwwtd_get_all_year();
	$option['allnuts'] = uwwtd_get_all_nuts($type, $year);
	$option['allrbd'] = uwwtd_get_all_rbd($type);
    $filter = uwwtd_get_form_filter($option);

    $preset_name = 'uwwtp_compliance_map';
    $preset = openlayers_preset_load($preset_name);
    $map    = openlayers_build_map($preset->data);

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_uwwtp_compliance($filter['filter']);
    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }
//     dsm('not in cache');    
	$data = uwwtd_get_data_uwwtp($filter['filter']);      
	if(uwwtd_data_contain_art17($data, 'field_article17_uwwtp_nid')){
         $list_field = array(
            'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_uwwloadenteringuwwtp_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwtreatmenttype_value'  => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwtreatmenttype_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['treatment_type_in_place']),
            'field_uwwcompliance_value'     => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwcompliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
            'field_uwwbod5perf_value'       => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwbod5perf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            'field_uwwcodperf_value'        => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwcodperf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            'field_uwwntotperf_value'       => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwntotperf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            'field_uwwptotperf_value'       => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwptotperf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            'field_article17_uwwtp_value'  	=> array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_article17_uwwtp_value_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            
        );
   }else{
        $list_field = array(
            'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_uwwloadenteringuwwtp_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwtreatmenttype_value'  => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwtreatmenttype_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['treatment_type_in_place']),
            'field_uwwcompliance_value'     => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwcompliance_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['compliance']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
            'field_uwwbod5perf_value'       => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwbod5perf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            'field_uwwcodperf_value'        => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwcodperf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            'field_uwwntotperf_value'       => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwntotperf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            'field_uwwptotperf_value'       => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwptotperf_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['test_pass']),
            
        );
   }

    $form = uwwtd_filter_form($list_field, $filter, 'views-exposed-form-uwwtp-compliance-page');
      
    $filter_force['filter'] = array();
    if (variable_get('display_only_active', '1') == 1) {
        $filter_force['filter']['field_status_value'] = '1';
    }
    //Process data
    uwwtd_filter_data($filter, $data, $filter_force); 
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['uwwtp'], $data);

    $optionstyle = array();
    //get the color for the point
    $optionstyle['color']['list'] = uwwtd_get_point_color_from_preset($preset);
    $optionstyle['color']['field'] = 'field_uwwcompliance_value_format';
    //get the size of the point
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['field'] = 'field_physicalcapacityactivity_value';


    $optionstyle['popup_title_type'] = 'treatment-plant';

    $optionstyle['popup'] = array(
        'field_uwwcompliance_value_format' => array('label' => 'Compliance'),
        'field_uwwloadenteringuwwtp_value_format' => array('label' => 'Load entering (p.e.)'),
        'field_physicalcapacityactivity_value_format' => array('label' => 'Capacity'),
        'field_uwwbod5perf_value_format' => array('label' => 'Performance: BOD5'),
        'field_uwwcodperf_value_format' => array('label' => 'Performance: COD'),
        'field_uwwntotperf_value_format' => array('label' => 'Performance: Total nitrogen'),
        'field_uwwptotperf_value_format' => array('label' => 'Performance: Total phosphorus'),
    );

    //todo format data in table
    $graph = array();
    foreach($data as $row) {
        if (false === isset($graph[$row['field_uwwcompliance_value_format']])) {
            $graph[$row['field_uwwcompliance_value_format']]['nb'] = 0;
            $graph[$row['field_uwwcompliance_value_format']]['field_uwwloadenteringuwwtp_value'] = 0;
//             $graph[$row['field_uwwcompliance_value_format']]['label'] = 'TODO';
        }
        $graph[$row['field_uwwcompliance_value_format']]['nb']++;
        $graph[$row['field_uwwcompliance_value_format']]['field_uwwloadenteringuwwtp_value'] += $row['field_uwwloadenteringuwwtp_value'];

        //format data
//         $row['field_uwwloadenteringuwwtp_value'] = uwwtd_format_number($row['field_uwwloadenteringuwwtp_value']);
		if(uwwtd_data_contain_art17($data, 'field_article17_uwwtp_nid')){
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_uwwloadenteringuwwtp_value_format'=>$row['field_uwwloadenteringuwwtp_value_format'],
				'field_physicalcapacityactivity_value_format'=>$row['field_physicalcapacityactivity_value_format'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
                'rbd'=>$row['rbd'].' '.$row['rbdname'],
				'field_uwwtreatmenttype_value_format'=>$row['field_uwwtreatmenttype_value_format'],
				'field_uwwcompliance_value_format'=>$row['field_uwwcompliance_value_format'],
				'field_uwwbod5perf_value_format'=>$row['field_uwwbod5perf_value_format'],
				'field_uwwcodperf_value_format'=>$row['field_uwwcodperf_value_format'],
				'field_uwwntotperf_value_format'=>$row['field_uwwntotperf_value_format'],
				'field_uwwptotperf_value_format'=>$row['field_uwwptotperf_value_format'],
				'field_article17_uwwtp_value_format'=>$row['field_article17_uwwtp_value_format'],
			));
		}else{
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_uwwloadenteringuwwtp_value_format'=>$row['field_uwwloadenteringuwwtp_value_format'],
				'field_physicalcapacityactivity_value_format'=>$row['field_physicalcapacityactivity_value_format'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
                'rbd'=>$row['rbd'].' '.$row['rbdname'],
				'field_uwwtreatmenttype_value_format'=>$row['field_uwwtreatmenttype_value_format'],
				'field_uwwcompliance_value_format'=>$row['field_uwwcompliance_value_format'],
				'field_uwwbod5perf_value_format'=>$row['field_uwwbod5perf_value_format'],
				'field_uwwcodperf_value_format'=>$row['field_uwwcodperf_value_format'],
				'field_uwwntotperf_value_format'=>$row['field_uwwntotperf_value_format'],
				'field_uwwptotperf_value_format'=>$row['field_uwwptotperf_value_format'],
			));
		}
	}

    $header = array();
    $header_export = array();
    $keys = array_keys($rows[0]['data']);
    foreach($GLOBALS['uwwtd']['ui']['field']['uwwtp'] as $field => $info) {
        if(in_array($field, $keys)){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }
    
    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
    		// Restructuration des tableaux :
    		$rowsToBeExported = array();
    		foreach ($header_export as $header) {
    			$rowsToBeExported[0][] = $header;
    		}
    		foreach ($rows as $row) {
    			if (!empty($row['data'])) {
    				$rowsToBeExported[] = $row['data'];
    			}
    		}
    		 
    		// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
    		$filename = 'uwwtp_compliance_'. $filter['filter']['field_anneedata_value']. '.xls';
    		uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
    		uwwtd_export_data($type_export, 'uwwtp_compliance_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    	}
    }

    $features['uwwtp_compliance_map'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
     // dsm($features);


    $rendermap = uwwtd_build_map($map, $features);
    unset($features);
    $total_1 = 0;
    $total_2 = 0;
    
    foreach($graph as $compliance_format => $detail) {       
        $seg = array();
        $seg['value'] = $detail['nb'];
        $seg['valueformat'] = number_format($detail['nb']);
        $seg['label'] = $compliance_format;
        $seg['color'] = $optionstyle['color']['list'][$compliance_format];
        $data_pie_1[] = $seg;
        
        
        $data_table_1[]=array(
            uwwtd_format_number($detail['nb']),
            $compliance_format
        );
        $total_1 += $detail['nb'];
        
        $seg2 = array();
        $seg2['value'] = (float)$detail['field_uwwloadenteringuwwtp_value'];
        $seg2['valueformat'] = number_format($detail['field_uwwloadenteringuwwtp_value']).' pe';
        $seg2['label'] = $compliance_format;
        $seg2['color'] = $optionstyle['color']['list'][$compliance_format];
        $data_pie_2[] = $seg2;
        $data_table_2[]=array(
            uwwtd_format_number($detail['field_uwwloadenteringuwwtp_value']),
            $compliance_format
        );
        $total_2 += $detail['field_uwwloadenteringuwwtp_value'];
    }
    $data_table_1[] = array( uwwtd_format_number($total_1), '');
    $data_table_2[] = array( uwwtd_format_number($total_2), '');
    
    $header_1 = array(t('Number of plants'),t('Compliance'));
    $render_table_graph_nb = theme('table', array('rows'=>$data_table_1, 'header'=>$header_1));
    $header_2 = array(t('Load entering (p.e.)'),t('Compliance'));
    $render_table_graph = theme('table', array('rows'=>$data_table_2, 'header'=>$header_2));

    //Graph 1
    $graph1Id = 'flot-auto-identifier-1';
    $front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
    $front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($data_pie_1).",  '".$graph1Id."');});</script>";
    $render_graph_nb = $front;
    $render_table_graph_nb = theme('table', array('rows'=>$data_table_1, 'header'=>$header_1));
    
    //Graph 2
    $graph2Id = 'flot-auto-identifier-2';
    $front2 = '<svg id="'. $graph2Id .'" style="margin:10px;"/>';
    $front2 .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($data_pie_2).",  '".$graph2Id."');});</script>";
    $render_graph = $front2;
    
    
    unset($data_pie_1,$data_pie_2, $data_table_1, $data_table_2);
    

	$headers = array();
    
    
	$headers[] = array(
		array('data' => 'Description ', 'colspan' => 7, 'class' => 'customTableHeader'),
		array('data' => 'Compliance ', 'class' => 'customTableHeader'),
		array('data' => 'Performance', 'colspan' => 4, 'class' => 'customTableHeader'),   	
	);
    //if article 17, add a column
    /*
    if (count($header) > 11) {
        $headers[0][] = '';
    }*/
    

	$headers[] = $header;
	
    $table = theme('table', array(
            'header' => $headers,
            'rows' => $rows,
			'rows_multiple'=>true,
            'attributes' => array('class' => 'customTable'),     
        ));   

        
    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
    $page.= '<div class="rows map-row" >'.$rendermap.'</div>'
            .'<div class="rows charts-row" >'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content">'
                    .uwwtd_page_render_flipcard(t("Global compliance by number of treament plants"), $render_graph_nb, $render_table_graph_nb,'')
                  .'</div>'
                .'</div>'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content">'
                    .uwwtd_page_render_flipcard(t("Global compliance by load entering (p.e.)"), $render_graph, $render_table_graph,'')
                  .'</div>'
                .'</div>'
            .'</div>'
            .'<div class="rows data-table-row">'
			. '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'uwwtps/compliance/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'uwwtps/compliance/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
                . '<div class="view-content content-table">'. $table . '</div>'
                . '<!--markerprinttable-->'  //use for print wjhtmltopdf
            .'</div>'
     ;
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}

function uwwtd_page_uwwtps_treatment($type_export = ''){
    $title = 'UWWTD Treatment Plants- Treatment map';
    drupal_set_title($title); 
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
    libraries_load('d3.drupal');
    $keys_order = $GLOBALS['uwwtd']['ui']['treatment_type_in_place'];
    unset($keys_order['All']);
    $keys_order = array_reverse(array_values($keys_order));
    
    $type = "uwwtp";
	$option = array();
    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment'];
    $option['default']['order'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['sort_field'];
    $option['default']['sort_mod'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['sort_mod'];
    $option['default']['sort'] = 'desc';
    if(isset($_REQUEST['field_anneedata_value']) && $_REQUEST['field_anneedata_value']!=""){
        $year = $_REQUEST['field_anneedata_value'];
    }
    $option['allyear'] = uwwtd_get_all_year();
	$option['allnuts'] = uwwtd_get_all_nuts($type, $year);
	$option['allrbd'] = uwwtd_get_all_rbd($type);
    $filter = uwwtd_get_form_filter($option);

    $preset_name = 'uwwtp_treatment_map';   //layer agglomeration_treatment 'Agglomeration connection map and list'
    $preset = openlayers_preset_load($preset_name);
    $map    = openlayers_build_map($preset->data);

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_uwwtp_treatment($filter['filter']);
//     dsm($filter);
    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }
//     dsm('not in cache');


//     dsm($filter);
//     $listyear = ();
    // dsm($listyear);
    //     return '';
	$data = uwwtd_get_data_uwwtp($filter['filter']);
	if(uwwtd_data_contain_art17($data, 'field_article17_uwwtp_nid')){
        $list_field = array(
            'field_anneedata_value'             => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_uwwloadenteringuwwtp_value'  => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwloadenteringuwwtp_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwprimarytreatment_value'   => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwprimarytreatment_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_uwwsecondarytreatment_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwsecondarytreatment_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_uwwnremoval_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwnremoval_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_uwwpremoval_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwpremoval_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_article17_uwwtp_value'  			=> array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_article17_uwwtp_value_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
        );
   }else{
       $list_field = array(
            'field_anneedata_value'             => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_uwwloadenteringuwwtp_value'  => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwloadenteringuwwtp_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwprimarytreatment_value'   => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwprimarytreatment_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_uwwsecondarytreatment_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwsecondarytreatment_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_uwwnremoval_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwnremoval_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_uwwpremoval_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment']['field_uwwpremoval_value_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
        );
   }
    $form = uwwtd_filter_form($list_field, $filter, 'views-exposed-form-uwwtp-treatment-page');
              
    $filter_force['filter'] = array();
    if (variable_get('display_only_active', '1') == 1) {
        $filter_force['filter']['field_status_value'] = '1';
    }
    //Process data
    uwwtd_filter_data($filter, $data, $filter_force); 
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment'], $data);


    $optionstyle = array();
    //get the color for the point
    $optionstyle['color']['list'] = uwwtd_get_point_color_from_preset($preset);
    $optionstyle['color']['field'] = 'field_uwwtreatmenttype_value_format';
    //get the size of the point
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['field'] = 'field_physicalcapacityactivity_value';

    $optionstyle['popup_title_type'] = 'treatment-plant';
    $optionstyle['popup'] = array(
        'field_uwwtreatmenttype_value_format' => array('label' => t('Treatment type in place')),
        'field_physicalcapacityactivity_value_format' => array('label' => t('Capacity')),
        'field_uwwcompliance_value_format' => array('label' => t('Compliance')),
        'field_uwwprimarytreatment_value_format' => array('label' => t('Primary treatment')),
        'field_uwwsecondarytreatment_value_format' => array('label' => t('Secondary treatment')),
        'field_uwwnremoval_value_format' => array('label' => t('N-removal')),
        'field_uwwpremoval_value_format' => array('label' => t('P-removal')),
    );

    //todo format data in table
    $graph = array();
    foreach($data as $row) {
        if (false === isset($graph[$row['field_uwwtreatmenttype_value_format']])) {
            $graph[$row['field_uwwtreatmenttype_value_format']]['nb'] = 0;
            $graph[$row['field_uwwtreatmenttype_value_format']]['field_uwwloadenteringuwwtp_value'] = 0;
//             $graph[$row['field_uwwtreatmenttype_value_format']]['label'] = 'TODO';
        }
        $graph[$row['field_uwwtreatmenttype_value_format']]['nb']++;
        $graph[$row['field_uwwtreatmenttype_value_format']]['field_uwwloadenteringuwwtp_value'] += $row['field_uwwloadenteringuwwtp_value'];

		if(uwwtd_data_contain_art17($data, 'field_article17_uwwtp_nid')){
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_uwwloadenteringuwwtp_value_format'=>$row['field_uwwloadenteringuwwtp_value_format'],
				'field_physicalcapacityactivity_value_format'=>$row['field_physicalcapacityactivity_value_format'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
				'rbd'=>$row['rbd'].' '.$row['rbdname'],
                'field_uwwprimarytreatment_value_format'=>$row['field_uwwprimarytreatment_value_format'],
				'field_uwwsecondarytreatment_value_format'=>$row['field_uwwsecondarytreatment_value_format'],
				'field_uwwnremoval_value_format'=>$row['field_uwwnremoval_value_format'],
				'field_uwwpremoval_value_format'=>$row['field_uwwpremoval_value_format'],
				'field_uwwspecification_value'=>$row['field_uwwspecification_value'],
				'field_article17_uwwtp_value_format'=>$row['field_article17_uwwtp_value_format'],
			));
		}else{
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_uwwloadenteringuwwtp_value_format'=>$row['field_uwwloadenteringuwwtp_value_format'],
				'field_physicalcapacityactivity_value_format'=>$row['field_physicalcapacityactivity_value_format'],
				'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
				'rbd'=>$row['rbd'].' '.$row['rbdname'],
                'field_uwwprimarytreatment_value_format'=>$row['field_uwwprimarytreatment_value_format'],
				'field_uwwsecondarytreatment_value_format'=>$row['field_uwwsecondarytreatment_value_format'],
				'field_uwwnremoval_value_format'=>$row['field_uwwnremoval_value_format'],
				'field_uwwpremoval_value_format'=>$row['field_uwwpremoval_value_format'],
				'field_uwwspecification_value'=>$row['field_uwwspecification_value'],
			));
		}
	}

    $header = array();
    $header_export = array();
    $keys = array_keys($rows[0]['data']);
    foreach($GLOBALS['uwwtd']['ui']['field']['uwwtp_treatment'] as $field => $info) {
        if(in_array($field, $keys)){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }

    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
    		// Restructuration des tableaux :
    		$rowsToBeExported = array();
    		foreach ($header_export as $header) {
    			$rowsToBeExported[0][] = $header;
    		}
    		foreach ($rows as $row) {
    			if (!empty($row['data'])) {
    				$rowsToBeExported[] = $row['data'];
    			}
    		}
    		 
    		// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
    		$filename = 'uwwtp_treatment_'. $filter['filter']['field_anneedata_value']. '.xls';
    		uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
    		uwwtd_export_data($type_export, 'uwwtp_treatment_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    	}
    }
    
    
    $features['uwwtp_treatment'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
//     dsm($features);
//     unset($data);
    $rendermap = uwwtd_build_map($map, $features);
    unset($features);
    
    $graph = array_replace(array_flip($keys_order), $graph);
    $total_1 = 0;
    $total_2 = 0;
    foreach($graph as $treatmenttype_format => $detail) {
        
        $seg = array();
        $seg['value'] = $detail['nb'];
        $seg['valueformat'] = number_format($detail['nb']);
        $seg['label'] = $treatmenttype_format;
        $seg['color'] = $optionstyle['color']['list'][$treatmenttype_format];
        $data_pie_1[] = $seg;
        
        $data_table_1[]=array(
            uwwtd_format_number($detail['nb']),
            $treatmenttype_format
        );
        $total_1 += $detail['nb'];
        
        $seg2 = array();
        $seg2['value'] = (float)$detail['field_uwwloadenteringuwwtp_value'];
        $seg2['valueformat'] = number_format($detail['field_uwwloadenteringuwwtp_value']).' pe';
        $seg2['label'] = $treatmenttype_format;
        $seg2['color'] = $optionstyle['color']['list'][$treatmenttype_format];
        $data_pie_2[] = $seg2;
        
        
        
        $data_table_2[]=array(
            uwwtd_format_number($detail['field_uwwloadenteringuwwtp_value']),
            $treatmenttype_format
        );
        $total_2 += $detail['field_uwwloadenteringuwwtp_value'];
    }
    $data_table_1[] = array( uwwtd_format_number($total_1), '');
    $data_table_2[] = array( uwwtd_format_number($total_2), '');
    
    $header_1 = array(t('Number of plants'),t('Treatment type in place'));
    $render_table_graph_nb = theme('table', array('rows'=>$data_table_1, 'header'=>$header_1));
    $header_2 = array(t('Load entering (p.e.)'),t('Treatment type in place'));
    $render_table_graph = theme('table', array('rows'=>$data_table_2, 'header'=>$header_2));

    //Graph 1
    $graph1Id = 'flot-auto-identifier-1';
    $front = '<svg id="'. $graph1Id .'" style="margin:10px;"/>';
    $front .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($data_pie_1).",  '".$graph1Id."');});</script>";
    $render_graph_nb = $front;
    $render_table_graph_nb = theme('table', array('rows'=>$data_table_1, 'header'=>$header_1));
    
    //Graph 2
    $graph2Id = 'flot-auto-identifier-2';
    $front2 = '<svg id="'. $graph2Id .'" style="margin:10px;"/>';
    $front2 .= "<script>jQuery(document).ready(function(){display_piechart_custom(".json_encode($data_pie_2).",  '".$graph2Id."');});</script>";
    $render_graph = $front2;
    
    unset($data_pie_1,$data_pie_2, $data_table_1, $data_table_2);
    

    $build['tablesort_table'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
        );

    
    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
    $page.= '<div class="rows map-row" >'.$rendermap.'</div>'
            . '<div class="rows charts-row" >'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content">'
                    .uwwtd_page_render_flipcard(t("Treatment type in place by number of treatment plants"), $render_graph_nb, $render_table_graph_nb,'')
                    
                  .'</div>'
                .'</div>'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content">'
                    .uwwtd_page_render_flipcard(t("Treatment type in place by Load entering (p.e.)"), $render_graph, $render_table_graph,'')
                  .'</div>'
                .'</div>'
            .'</div>'
            . '<div class="rows data-table-row">'
			. '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'uwwtps/treatment/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'uwwtps/treatment/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
                . '<div class="view-content content-table">'. drupal_render($build) . '</div>'
                . '<!--markerprinttable-->'   //use for print wkhtmltopdf
            .'</div>';
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}


//=======================================TODO : manage style for scale point and color with E-PRT range
function uwwtd_page_uwwtps_pressure($type_export = ''){
    $title = 'UWWTD Treatment Plants- Pressure map';
    drupal_set_title($title);  //todo
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    $type = "uwwtp";
	$option = array();
    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure'];
    $option['default']['order'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['sort_field'];
    $option['default']['sort_mod'] = $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_uwwloadenteringuwwtp_value_format']['sort_mod'];
    $option['default']['sort'] = 'desc';
    if(isset($_REQUEST['field_anneedata_value']) && $_REQUEST['field_anneedata_value']!=""){
        $year = $_REQUEST['field_anneedata_value'];
    }
    $option['allyear'] = uwwtd_get_all_year();
	$option['allnuts'] = uwwtd_get_all_nuts($type, $year);
	$option['allrbd'] = uwwtd_get_all_rbd($type);
//     dsm($option);
    $filter = uwwtd_get_form_filter($option);

    
//     dsm($map);
    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_uwwtp_pressure($filter['filter']);
//     dsm($filter);
    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }
//     dsm('not in cache');


	$data = uwwtd_get_data_uwwtp($filter['filter']);
	if(uwwtd_data_contain_art17($data, 'field_article17_uwwtp_nid')){
         $list_field = array(
            'field_anneedata_value'             => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
            'field_uwwloadenteringuwwtp_value'  => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwloadenteringuwwtp_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwboddischarge_value'   => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwboddischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwcoddischarge_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwcoddischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwndischarge_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwndischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_uwwpdischarge_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwpdischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
            'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
            'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
            'field_article17_uwwtp_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp']['field_article17_uwwtp_value_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
            
        );
   }else{
       $list_field = array(
        'field_anneedata_value'             => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
        
        'field_uwwloadenteringuwwtp_value'  => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwloadenteringuwwtp_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'field_uwwboddischarge_value'   => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwboddischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'field_uwwcoddischarge_value' => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwcoddischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'field_uwwndischarge_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwndischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'field_uwwpdischarge_value'           => array('title' => $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure']['field_uwwpdischarge_value_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'field_regionnuts_value'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['agglomeration']['n3nm']['label'], 'type' => 'selectregion', 'data' => $option['allnuts']),
        'rbd' => array('title' => t('River Basin District'), 'type' => 'select', 'data' => $option['allrbd']),
        
    );
    }

    $form = uwwtd_filter_form($list_field, $filter, 'views-exposed-form-uwwtp-pressure-page');

//     dsm($data);        
    $filter_force['filter'] = array();
    if (variable_get('display_only_active', '1') == 1) {
        $filter_force['filter']['field_status_value'] = '1';
    }
    //Process data
    uwwtd_filter_data($filter, $data, $filter_force);
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure'], $data);

    $optionstyle = array();
    //get the size of the point

    $optionstyle['popup_title_type'] = 'treatment-plant';
    $optionstyle['popup'] = array(
        'field_uwwtreatmenttype_value_format' => array('label' => 'Treatment type in place'),
        'field_physicalcapacityactivity_value_format' => array('label' => 'Capacity'),
        'field_uwwcompliance_value_format' => array('label' => 'Compliance'),
        'field_uwwprimarytreatment_value_format' => array('label' => 'Primary treatment'),
        'field_uwwsecondarytreatment_value_format' => array('label' => 'Secondary treatment'),
        'field_uwwnremoval_value_format' => array('label' => 'N-removal'),
        'field_uwwpremoval_value_format' => array('label' => 'P-removal'),
        'field_uwwboddischarge_value_format' => array('label' => 'Discharged load BOD (t per year)'),
        'field_uwwcoddischarge_value_format' => array('label' => 'Discharged load COD (t per year)'),
        'field_uwwndischarge_value_format' => array('label' => 'Discharged load Ntot (t per year)'),
        'field_uwwpdischarge_value_format' => array('label' => 'Discharged load Ptot (t per year)'),
    );

//     dsm($optionstyle);
//     dsm($data);

    //todo format data in table
//     $graph = array();
    foreach($data as $row) {
		if(uwwtd_data_contain_art17($data, 'field_article17_uwwtp_nid')){
		   $rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_uwwloadenteringuwwtp_value_format'=>$row['field_uwwloadenteringuwwtp_value_format'],
				'field_physicalcapacityactivity_value_format'=>$row['field_physicalcapacityactivity_value_format'],
                'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
				'rbd'=>$row['rbd'].' '.$row['rbdname'],
				'field_uwwboddischarge_value_format'=>$row['field_uwwboddischarge_value_format'],
				'field_uwwcoddischarge_value_format'=>$row['field_uwwcoddischarge_value_format'],
				'field_uwwndischarge_value_format'=>$row['field_uwwndischarge_value_format'],
				'field_uwwpdischarge_value_format'=>$row['field_uwwpdischarge_value_format'],
				'field_article17_uwwtp_value_format'=>$row['field_article17_uwwtp_value_format'],
			));
		}else{
			$rows[] = array('data' => array(
				//'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path($optionstyle['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
				'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
                'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
				'field_uwwloadenteringuwwtp_value_format'=>$row['field_uwwloadenteringuwwtp_value_format'],
				'field_physicalcapacityactivity_value_format'=>$row['field_physicalcapacityactivity_value_format'],
                'n3nm'=>($row['n3nm']!=''?$row['n3nm']:$row['field_regionnuts_value']),
				'rbd'=>$row['rbd'].' '.$row['rbdname'],
				'field_uwwboddischarge_value_format'=>$row['field_uwwboddischarge_value_format'],
				'field_uwwcoddischarge_value_format'=>$row['field_uwwcoddischarge_value_format'],
				'field_uwwndischarge_value_format'=>$row['field_uwwndischarge_value_format'],
				'field_uwwpdischarge_value_format'=>$row['field_uwwpdischarge_value_format'],
			));
		}
    }
    
    $header = array();
    $header_export = array();
    $keys = array_keys($rows[0]['data']);
    foreach($GLOBALS['uwwtd']['ui']['field']['uwwtp_pressure'] as $field => $info) {
        if(in_array($field, $keys)){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }
    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
    		// Restructuration des tableaux :
    		$rowsToBeExported = array();
    		foreach ($header_export as $header) {
    			$rowsToBeExported[0][] = $header;
    		}
    		foreach ($rows as $row) {
    			if (!empty($row['data'])) {
    				$rowsToBeExported[] = $row['data'];
    			}
    		}
    		 
    		// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
    		$filename = 'uwwtp_pressure_'. $filter['filter']['field_anneedata_value']. '.xls';
    		uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
    		uwwtd_export_data($type_export, 'uwwtp_pressure_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    	}
    }
    
    //=============================Create the map
    
    $preset_name = 'uwwtp_pressure_map';   //layer agglomeration_treatment 'Agglomeration connection map and list'
    $preset = openlayers_preset_load($preset_name);
   
    
    /*
        BOD: 100 and 10
        COD 150 and 15
        N: 50 and 5
        P: 5 and 0.5
    */
    
    //=== BOD
    //$optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['list'] = array("1"=>10,"2"=>100,"3"=>0);
    $optionstyle['default_color'] = '#5B3349';
    $optionstyle['weight']['field'] = 'field_uwwboddischarge_value';
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addColors'][] = $optionstyle['default_color'];
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addArbitrary_classes'][] = "0|10\n10|100\n100|";
    $behavior = $preset->data['behaviors']['openlayers_plus_behavior_scalepoints'];
    $behavior['distinct'] = 3;
    $behavior['arbitrary_classes'] = "0|10\n10|100\n100|";
    $behavior['default_color'] =$optionstyle['default_color'];
    $features['uwwtp_bod_pressure_'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    $preset->data['behaviors']['openlayers_plus_behavior_legend']['layers']['uwwtp_bod_pressure_']=array(
        'layer'=>'uwwtp_bod_pressure_',
        'openlayers_plus_behavior_scalepoints'=>$behavior
    );
    
    
    
    //=== COD
    $optionstyle['weight']['list'] = array("1"=>15,"2"=>150,"3"=>0);
    $optionstyle['default_color'] = '#d87600';
    $optionstyle['weight']['field'] = 'field_uwwcoddischarge_value';
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addColors'][] = $optionstyle['default_color'];
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addArbitrary_classes'][] = "0|15\n15|100\n100|";
    $behavior = $preset->data['behaviors']['openlayers_plus_behavior_scalepoints'];
    $behavior['distinct'] = 3;
    $behavior['arbitrary_classes'] = "0|15\n15|150\n150|";
    $behavior['default_color'] =$optionstyle['default_color'];
    $features['uwwtp_cod_pressure_'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    $preset->data['behaviors']['openlayers_plus_behavior_legend']['layers']['uwwtp_cod_pressure_']=array(
        'layer'=>'uwwtp_cod_pressure_',
        'openlayers_plus_behavior_scalepoints'=>$behavior
    );
    
    
    //=== Nitrates
    $optionstyle['weight']['list'] = array("1"=>5,"2"=>50,"3"=>0);
    $optionstyle['default_color'] = '#87ceeb';
    $optionstyle['weight']['field'] = 'field_uwwndischarge_value';
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addColors'][] = $optionstyle['default_color'];
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addArbitrary_classes'][] = "0|5\n5|50\n50|";
    $behavior = $preset->data['behaviors']['openlayers_plus_behavior_scalepoints'];
    $behavior['distinct'] = 3;
    $behavior['arbitrary_classes'] = "0|5\n5|50\n50|";
    $behavior['default_color'] =$optionstyle['default_color'];
    $features['uwwtp_n_pressure_'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    $preset->data['behaviors']['openlayers_plus_behavior_legend']['layers']['uwwtp_n_pressure_']=array(
        'layer'=>'uwwtp_n_pressure_',
        'openlayers_plus_behavior_scalepoints'=>$behavior
    );
    
    //=== Phosphore
    $optionstyle['weight']['list'] = array("1"=>0.5,"2"=>5,"3"=>0);
    $optionstyle['default_color'] = '#ff9900';
    $optionstyle['weight']['field'] = 'field_uwwpdischarge_value';
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addColors'][] = $optionstyle['default_color'];
    $preset->data['behaviors']['openlayers_plus_behavior_scalepoints']['addArbitrary_classes'][] =  "0|0.5\n0.5|5\n5|";
    $behavior = $preset->data['behaviors']['openlayers_plus_behavior_scalepoints'];
    $behavior['distinct'] = 3;
    $behavior['arbitrary_classes'] = "0|0.5\n0.5|5\n5|";
    $behavior['default_color'] =$optionstyle['default_color'];
    $features['uwwtp_p_pressure_'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    $preset->data['behaviors']['openlayers_plus_behavior_legend']['layers']['uwwtp_p_pressure_']=array(
        'layer'=>'uwwtp_p_pressure_',
        'openlayers_plus_behavior_scalepoints'=>$behavior
    );
//     $features = array();
//     dsm($features);
//     unset($data);
    $map    = openlayers_build_map($preset->data);
    $rendermap = uwwtd_build_map($map, $features);
    unset($features);

    $build['tablesort_table'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
        );

    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
    $page.= '<div class="rows map-row" >'.$rendermap.'</div>'
			
            . '<div class="rows data-table-row">'
				. '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'uwwtps/pressure/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'uwwtps/pressure/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
                . '<div class="view-content content-table">'. drupal_render($build) . '</div>'
                . '<!--markerprinttable-->'  //use for print wjhtmltopdf              
            .'</div>'
     ;
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);

     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}

 
function uwwtd_sensitive_areas_page($type_export = ''){
    $title = 'Sensitive areas';
    drupal_set_title($title);
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
//     drupal_add_js(drupal_get_path('module', 'uwwtd') . '/../../libraries/d3/d3.v3.min.js')
	$type = "receiving_area";
    $option = array();

    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['rca'];
    $option['default']['order'] = 'tot_entering_load';//$GLOBALS['uwwtd']['ui']['field']['rca']['_format'];
    $option['default']['sort_mod'] = 'numeric';
    $option['default']['sort'] = 'desc';
    $option['allyear'] = uwwtd_get_all_year();
    $option['zonetype'] = $GLOBALS['uwwtd_value']['rcatype_labels'];

    $filter = uwwtd_get_form_filter($option);
    

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_sensitive_area($filter['filter']);

    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }
	$data = uwwtd_get_data_sensitive_areas($filter['filter']);
    $list_field = array(
        'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
        'tot_design_capacity'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_design_capacity_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'tot_entering_load'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_entering_load_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'rca52'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca52_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca54'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca54_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca58'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca58_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'zonetype'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['zonetype_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd_value']['rcatype_labels']),  
        'zonetype_spe'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['spezonetype_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd_value']['rca_spe_type_labels']),  
        
    );
// dsm($list_field);
    $form = uwwtd_filter_form($list_field, $filter);


//     dsm($data);
    //e.vincent / bruno want to show only agglo > 2000
    $filter_force = array();

    uwwtd_filter_data($filter, $data, $filter_force);        

    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['rca'], $data);
//     dsm($data);

    $optionstyle = array();
    //get the color for the point
    $optionstyle['color']['list'] =$GLOBALS['uwwtd_value']['rcatype_color'];
    $optionstyle['color']['field'] = 'zonetype';
    $optionstyle['style_prefix'] = 'rca_';
    //get the size of the point
/*
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['field'] = 'field_agggenerated_value';
    
*/	
    $optionstyle['popup_title_type'] = 'receiving-area';
    $optionstyle['popup'] = array(
        'field_inspireidlocalid_value' => array('label' => 'ID'),
        'zonetype_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['zonetype_format']['label']),
        'rca52_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca52_format']['label']),
        'rca54_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca54_format']['label']),
        'rca58_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca58_format']['label']),
        'area_format' => array('label' => 'Area'),
        'tot_design_capacity_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_design_capacity_format']['label']),
        'tot_entering_load_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_entering_load_format']['label']),
    );

    //todo format data in table
    $graph = array();
    foreach($data as $row) {
        $dts  = array();
        //Art. 5(2,3) - N : field_rcaanstartdate
        //Art. 5(2,3) -P  : field_rcaapstartdate
        //Art. 5(2,3) - b : field_rcabstartdate
        //Art. 5(2,3) - c  : field_rcacstardate
        //Art. 5(8)          : field_rcadateart5854
        if(!empty($row['dt_art58'])){
            $dts[]  = t('Art. 5(8)').': '.date('d/m/Y',strtotime($row['dt_art58'])).',';
        }
        if(!empty($row['dt_an'])){
            $dts[]  = t('Art. 5(2,3) - N').': '.date('d/m/Y',strtotime($row['dt_an'])).',';
        }
        if(!empty($row['dt_ap'])){
            $dts[]  = t('Art. 5(2,3) - P').': '.date('d/m/Y',strtotime($row['dt_ap'])).',';
        }
        if(!empty($row['dt_b'])){
            $dts[]  = t('Art. 5(2,3) - b').': '.date('d/m/Y',strtotime($row['dt_b'])).',';
        }
         if(!empty($row['dt_c'])){
            $dts[]  = t('Art. 5(2,3) - c').': '.date('d/m/Y',strtotime($row['dt_c'])).',';
        }
        
        $rows[] = array('data' => array(
            //'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path('receiving-area', array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
            'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
            'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
            // 'zonetype_format'=>$row['zonetype_format'],
            'rca52_format'=>$row['rca52_format'],
             'rca54_format'=>$row['rca54_format'],
            'rca58_format'=>$row['rca58_format'],
            'aggs'=>$row['aggs'],
            'uwwtps'=>$row['uwwtps'],
            'tot_design_capacity_format'=>$row['tot_design_capacity_format'],
            'tot_entering_load_format'=>$row['tot_entering_load_format'],
            'starting_dates_application'=>theme('item_list', array('items'=>$dts))
        ));

    }
    $header = array();
    $header_export = array();
    foreach($GLOBALS['uwwtd']['ui']['field']['rca'] as $field => $info) {
        if(isset($rows[0]['data'][$field])){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }
    
    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
    		// Restructuration des tableaux :
    		$rowsToBeExported = array();
    		foreach ($header_export as $header) {
    			$rowsToBeExported[0][] = $header;
    		}
    		foreach ($rows as $row) {
    			if (!empty($row['data'])) {
                    $row['data']['starting_dates_application'] = strip_tags($row['data']['starting_dates_application']);
    				$rowsToBeExported[] = $row['data'];
    			}
    		}
    		 
    		// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
    		$filename = 'rca_'. $filter['filter']['field_anneedata_value']. '.xls';
    		uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
            // Restructuration des tableaux :
    		$rowsToBeExported = array();
            foreach ($rows as $row) {
    			if (!empty($row['data'])) {
                    $row['data']['starting_dates_application'] = strip_tags($row['data']['starting_dates_application']);
    				$rowsToBeExported[] = $row;
    			}
    		}
    		uwwtd_export_data($type_export, 'rca_'.$filter['filter']['field_anneedata_value'], $header_export, $rowsToBeExported);
    	}
    }

    //dsm($map);
    $features['rca'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    if(!empty($features['rca'])){
    $styles = array_unique(array_column($features['rca'], 'style_name'));
    //$map; 
    
    $preset_name = 'rca_map';
    $preset = openlayers_preset_load($preset_name);
    //Fix bug : in some case background layer  is not activated
    if(!isset($preset->data['layers']['osm_mapnik'])){
        $preset->data['layers']['osm_mapnik'] = 'osm_mapnik';
    }
    $preset->data['behaviors']['openlayers_plus_behavior_legend']['layers']['rca']=array(
        'layer'=>'rca',
        //'styles'=>$styles
    );
    $map    = openlayers_build_map($preset->data);
    $rendermap = uwwtd_build_map($map, $features);
    }else{
        $rendermap = '';
    }

    unset($features);

    //================================TODO INSERT GRAPH HERE
    
    
    //dsm($rows[0]['data']);
    $build['tablesort_table'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
        );
    
    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
        $page.= '<div class="rows map-row" >'.$rendermap.'</div>'
            . '<div class="rows data-table-row">'
			 . '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'sensitive-areas/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'sensitive-areas/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
				
                . '<div class="view-content content-table">'. drupal_render($build) . '</div>'
			
                . '<!--markerprinttable-->'  //use for print wkhtmltopdf
            .'</div>' ;
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}

function uwwtd_dcp_page($type_export = ''){
     
    
    $title = 'Discharge points';
    drupal_set_title($title);
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
//     drupal_add_js(drupal_get_path('module', 'uwwtd') . '/../../libraries/d3/d3.v3.min.js')
	$type = "discharge_point";
    $option = array();

    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['dcp'];
    $option['default']['order'] = 'title';//$GLOBALS['uwwtd']['ui']['field']['rca']['_format'];
    //$option['default']['sort_mod'] = 'numeric';
    $option['default']['sort'] = 'asc';
    $option['allyear'] = uwwtd_get_all_year();
    $option['zonetype'] = $GLOBALS['uwwtd_value']['rcatype_labels'];
    $option['rcacat'] = $GLOBALS['uwwtd_value']['rcacat_labels'];

    $filter = uwwtd_get_form_filter($option);
    $preset_name = 'dcp_map';
    $preset = openlayers_preset_load($preset_name);
    $map    = openlayers_build_map($preset->data);

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_dcp($filter['filter']);

    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }
	$data = uwwtd_get_data_dcp($filter['filter']);
    $list_field = array(
    
        'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
        'zonetype'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['dcp']['zonetype_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd_value']['rcatype_labels']),  
        'rcacat'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['dcp']['rcacat_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd_value']['rcacat_labels']),  
        
    /*
        'tot_design_capacity'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_design_capacity_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'tot_entering_load'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_entering_load_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'rca52'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca52_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca54'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca54_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca58'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca58_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        
    */
    );
// dsm($list_field);
    $form = uwwtd_filter_form($list_field, $filter);


//     dsm($data);
    //e.vincent / bruno want to show only agglo > 2000
    $filter_force = array();
    /*
    
    $filter_force['filter']['field_agggenerated_value_op'] = '>';
    $filter_force['filter']['field_agggenerated_value']['value'] = variable_get('min_aggenerated_display', '1999');
    if (variable_get('display_only_active', '1') == 1) {
        $filter_force['filter']['field_status_value'] = '1';
    }
    */
	//dsm($filter);
    uwwtd_filter_data($filter, $data, $filter_force);        
//     uwwtd_filter_data($filter, $data);
    
    /*
	  if($art17 == "No"){
		  unset($header[9]);
	  }
	  unset($header[8]);
    */
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['dcp'], $data);
//     dsm($data);

    $optionstyle = array();
    //get the color for the point
    $optionstyle['color']['list'] = uwwtd_get_point_color_from_preset($preset);
    //$optionstyle['color']['field'] = 'uwwtp_compliance_format';
    $optionstyle['color']['field'] = 'field_uwwcompliance_value_format';
    //get the size of the point
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['field'] = 'field_physicalcapacityactivity_value';
	
    $optionstyle['popup_title_type'] = 'discharge-point';
    $optionstyle['popup'] = array(
        'field_inspireidlocalid_value' => array('label' => 'ID'),
        'rca_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['rca_format']['label']),
        'zonetype_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['zonetype_format']['label']),
        'uwwtp_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['uwwtp_format']['label']),
        'field_uwwcompliance_value_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['field_uwwcompliance_value_format']['label']),
    );
    
    //todo format data in table
    $graph = array();
    $keys = array();
    foreach($data as $row) {
        $rows[] = array('data' => array(
            //'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path('discharge-point', array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
            'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
            'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
            'rca_format'=>$row['rca_format'],
            'zonetype_format'=>$row['zonetype_format'],
            'uwwtp_format'=>$row['uwwtp_format'],
            'field_uwwcompliance_value_format'=>$row['field_uwwcompliance_value_format'],
        ));
        
        if(!in_array($row['field_uwwcompliance_value'], $keys )){ $keys[] = $row['field_uwwcompliance_value'];}
        if(!isset($graph[$row['zonetype']][$row['field_uwwcompliance_value']])){
            $graph[$row['zonetype']][$row['field_uwwcompliance_value']]['nb'] = 0;
            $graph[$row['zonetype']][$row['field_uwwcompliance_value']]['load'] = 0;
            
        }
        $graph[$row['zonetype']][$row['field_uwwcompliance_value']]['nb']++;
        $graph[$row['zonetype']][$row['field_uwwcompliance_value']]['load']+=$row['field_physicalcapacityactivity_value'];
        
    }
    //We have to set values for empty cell of graph matrix
    //Sort keys
    foreach($GLOBALS['uwwtd']['all_compliance'] as $k=>$v){
        if(in_array($k, $keys)){
            $order[] = $k;
        }
    }

    if(!empty($graph) && !empty($order)){
        foreach($graph as $k=>$line){
            $new_line = array();
            foreach($order as $key){
                if(!isset($line[$key])){
                    $new_line[$key] = 0;
                }
                else{
                    $new_line[$key] = $line[$key];
                }
            }
            $graph[$k] = $new_line ;
        } 
    }
    
    $header = array();
    $header_export = array();
    foreach($GLOBALS['uwwtd']['ui']['field']['dcp'] as $field => $info) {
        if(isset($rows[0]['data'][$field])){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }
    
    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
    		// Restructuration des tableaux :
    		$rowsToBeExported = array();
    		foreach ($header_export as $header) {
    			$rowsToBeExported[0][] = $header;
    		}
    		foreach ($rows as $row) {
    			if (!empty($row['data'])) {
    				$rowsToBeExported[] = $row['data'];
    			}
    		}
    		 
    		// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
    		$filename = 'dcp_'. $filter['filter']['field_anneedata_value']. '.xls';
    		uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
    		uwwtd_export_data($type_export, 'dcp_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    	}
    }
    $features['dcp'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    $rendermap = uwwtd_build_map($map, $features);
    
    unset($features);

    $rows_nb = array();
    $rows_load = array();
    $legend = array();
    $color =array();
    $total_nb=array();
    $total_load=array();
    if(!empty($graph)){
        foreach($graph as $type=>$item){
            if(empty($legend)){
                $legend = array_keys($item);
                foreach($legend as $i=>$j){
                    $legend[$i]= $GLOBALS['uwwtd']['all_compliance'][$j];
                    $color[$i]= $GLOBALS['uwwtd']['compliance_color'][$j];
                }
            }
            $line_nb = array();
            $line_load = array();
            
            $line_nb[] = $GLOBALS['uwwtd_value']['rcatype_labels'][$type];
            $line_load[] = $GLOBALS['uwwtd_value']['rcatype_labels'][$type];
            foreach($item as $k=>$v){
                $line_nb[] = $v['nb'];
                $line_load[] = $v['load'];
                if(!isset($total_nb[$k])) $total_nb[$k] = 0;
                if(!isset($total_load[$k])) $total_load[$k] = 0;
                $total_nb[$k]+=$v['nb'];
                $total_load[$k]+=$v['load'];
                
            }
            $rows_nb[] = $line_nb;
            $rows_load[] = $line_load;  
        } 
    }
    

    // Pour afficher ce qui est attendu pour crééer un graph pulti colonnes (voir sur /discharge-points) :
//     dsm($rows_nb);
//     dsm($color);
//     dsm($legend);

    $dcp_graph_nb= uwwtd_page_render_column('graph-1', $legend, $color, $rows_nb, true);
    $dcp_graph=uwwtd_page_render_column('graph-2', $legend, $color, $rows_load, true);
    array_unshift($total_nb, t('Total'));
    array_unshift($total_load, t('Total'));
    //FIX ME nd@oieau.fr
    $rows_nb[]=$total_nb;
    $rows_load[]=$total_load;
    array_unshift($legend, t('Area type'));
    $dcp_table_nb = theme('table', array('rows'=>$rows_nb, 'header'=>$legend));
    $dcp_table = theme('table', array('rows'=>$rows_load, 'header'=>$legend));
    //dsm($rows[0]['data']);
    $build['tablesort_table'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
        );


   //============================Render the page ==> TODO use a true template/theme function 

    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
        $page.= '<div class="rows map-row" >'.$rendermap.'</div>'
            
            . '<div class="rows charts-row" >'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content-extended">'
                    
                    .uwwtd_page_render_flipcard(t("Compliance of the UWWTP linked to the discharge point by number and by receving area type"), $dcp_graph_nb, $dcp_table_nb, t("The compliance of the discharge point is based on the linked waste water treatment plant's compliance"))
                  
                  .'</div>'
                .'</div>'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content-extended">'
                    .uwwtd_page_render_flipcard(t("Compliance of the UWWTP linked to the discharge point by load entering and by receving area type"), $dcp_graph, $dcp_table, t("The compliance and the load entering of the discharge point is based on linked the waste water treatment plant's compliance and load entering"))
                  .'</div>'
                .'</div>'
            .'</div>'
            . '<div class="rows data-table-row">'
			 . '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'discharge-points/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'discharge-points/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
				
                . '<div class="view-content content-table">'. drupal_render($build) . '</div>'
			
                . '<!--markerprinttable-->'  //use for print wkhtmltopdf
            .'</div>' ;
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page; 
 }

function uwwtd_dcp_reuse_page($type_export = ''){
    $title = 'Discharge points - reuse';
    drupal_set_title($title);
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/js/uwwtd.js');
    drupal_add_js(drupal_get_path('module', 'uwwtd') . '/lib/flip/jquery.flip.min.js');
//     drupal_add_js(drupal_get_path('module', 'uwwtd') . '/../../libraries/d3/d3.v3.min.js')
	$type = "discharge_point";
    $option = array();

    $option['order'] = $GLOBALS['uwwtd']['ui']['field']['dcp'];
    $option['default']['order'] = 'title';//$GLOBALS['uwwtd']['ui']['field']['rca']['_format'];
    //$option['default']['sort_mod'] = 'numeric';
    $option['default']['sort'] = 'asc';
    $option['allyear'] = uwwtd_get_all_year();
    $option['zonetype'] = $GLOBALS['uwwtd_value']['rcatype_labels'];
    $option['rcacat'] = $GLOBALS['uwwtd_value']['rcacat_labels'];

    $filter = uwwtd_get_form_filter($option);
    $preset_name = 'dcp_reuse_map';
    $preset = openlayers_preset_load($preset_name);
    $map = null;
    if(isset($preset->data)){
        $map    = openlayers_build_map($preset->data);
    }

    $cache = '';
    $cache_name = uwwtd_get_cache_key_page_dcp($filter['filter']);

    //use cache only for page with no filter , or only the year and default sort
    $put_in_cache = uwwtd_is_filter_default($filter, $option) && uwwtd_is_user_anonymous() && $type_export == '';
    if (UWWTD_CACHE_ACTIVE === true && true === $put_in_cache) {
        $cache = cache_get($cache_name, 'cache_page');
    }
    if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
        return $cache->data['page'];
    }
	$data = uwwtd_get_data_dcp($filter['filter']);
    $list_field = array(
    
        'field_anneedata_value'         => array('title' => 'Year of data', 'type' => 'select', 'data' => $option['allyear']),
        'zonetype'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['dcp']['zonetype_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd_value']['rcatype_labels']),  
        'rcacat'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['dcp']['rcacat_format']['label'], 'type' => 'select', 'data' => $GLOBALS['uwwtd_value']['rcacat_labels']),  
        
    /*
        'tot_design_capacity'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_design_capacity_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'tot_entering_load'      => array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['tot_entering_load_format']['label'], 'type' => 'selectoperator', 'data' => $GLOBALS['uwwtd']['ui']['operator']),
        'rca52'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca52_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca54'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca54_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        'rca58'  		=> array('title' => $GLOBALS['uwwtd']['ui']['field']['rca']['rca58_format']['label'], 'type' => 'selectarticle', 'data' => $GLOBALS['uwwtd']['ui']['yes_no']),
        
    */
    );
// dsm($list_field);
    $form = uwwtd_filter_form($list_field, $filter);


     //dsm($data);
     //Filter all discharge point without a field_dcpirrigation_value value
    foreach($data as $k=>$line){
        if($line['field_dcpirrigation_value']==''){
            unset($data[$k]);
        }
    }
    $filter_force = array();

    uwwtd_filter_data($filter, $data, $filter_force);        
    if(empty($data)){
        return '<h1>'.$title.'</h1>'.$form.'<p>'.t("There is no information about water reuse with yours criterions").'</p>';
    }
    
    uwwtd_sort_data($filter, $GLOBALS['uwwtd']['ui']['field']['dcp'], $data);
//     dsm($data);

    $optionstyle = array();
    //get the color for the point
    $optionstyle['color']['list'] = uwwtd_get_point_color_from_preset($preset);
    $optionstyle['color']['field'] = 'field_dcpirrigation_value_format';
    //get the size of the point
    $optionstyle['weight']['list'] = uwwtd_get_point_size_from_preset($preset, 'openlayers_plus_behavior_scalepoints');
    $optionstyle['weight']['field'] = 'field_uwwwastewatertreated_value';
	
    $optionstyle['popup_title_type'] = 'discharge-point';
    $optionstyle['popup'] = array(
        'field_inspireidlocalid_value' => array('label' => 'ID'),
        'rca_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['rca_format']['label']),
        'zonetype_format' => array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['zonetype_format']['label']),
        'uwwtp_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['uwwtp_format']['label']),
        'field_uwwcompliance_value_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['field_uwwcompliance_value_format']['label']),
        'field_dcpirrigation_value_format'=>array('label' => $GLOBALS['uwwtd']['ui']['field']['dcp']['field_dcpirrigation_value']['label']),
        'field_uwwwastewatertreated_value'=>array('label' =>$GLOBALS['uwwtd']['ui']['field']['dcp']['field_uwwwastewatertreated_value']['label']),
        
    );
    
    //todo format data in table
    $graph = array();
    $keys = array();
    foreach($data as $row) {
        $rows[] = array('data' => array(
            //'title'=>($type_export != '' ? $row['title'] : l($row['title'], uwwtd_get_url_path('discharge-point', array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])))),
            'title'=>($type_export != '' ? $row['title'] : l($row['title'], 'node/'.$row['nid'], array('html'=>true))),
            'field_inspireidlocalid_value'=>$row['field_inspireidlocalid_value'],
            'rca_format'=>$row['rca_format'],
            'zonetype_format'=>$row['zonetype_format'],
            'uwwtp_format'=>$row['uwwtp_format'],
            'field_uwwcompliance_value_format'=>$row['field_uwwcompliance_value_format'],
            'field_dcpirrigation_value'=>$GLOBALS['uwwtd']['reuse_type'][$row['field_dcpirrigation_value']],
            'field_uwwwastewatertreated_value'=>$row['field_uwwwastewatertreated_value_format']
        ));
        
        if(!in_array($row['field_uwwcompliance_value'], $keys )){ $keys[] = $row['field_uwwcompliance_value'];}
        if(!isset($graph[$row['zonetype']])){
            foreach($GLOBALS['uwwtd']['reuse_type'] as $r=>$label){
                $graph[$row['zonetype']][$r]['nb'] = 0;
                $graph[$row['zonetype']][$r]['load'] = 0;  
            }
            
        }
        $graph[$row['zonetype']][$row['field_dcpirrigation_value']]['nb']++;
        $graph[$row['zonetype']][$row['field_dcpirrigation_value']]['load']+=$row['field_uwwwastewatertreated_value'];
        
    }
    
    //We have to set values for empty cell of graph matrix
    //Sort keys
    foreach($GLOBALS['uwwtd']['reuse_type'] as $k=>$v){
        if(in_array($k, $keys)){
            $order[] = $k;
        }
    }

    if(!empty($graph) && !empty($order)){
        foreach($graph as $k=>$line){
            $new_line = array();
            foreach($order as $key){
                if(!isset($line[$key])){
                    $new_line[$key] = 0;
                }
                else{
                    $new_line[$key] = $line[$key];
                }
            }
            $graph[$k] = $new_line ;
        } 
    }
    $header = array();
    $header_export = array();
    foreach($GLOBALS['uwwtd']['ui']['field']['dcp'] as $field => $info) {
        if(isset($rows[0]['data'][$field])){
            $header[$field] = array(
                'data' => $info['label'],
                'field' => $field,
                'sorter'=>isset($info['sort_field'])?$info['sort_field']:$field,
                'sort_mod'=>isset($info['sort_mod'])?$info['sort_mod']:'text',
            );
            $header_export[] = $info['label'];
        }
        
    }
    
    //here export data and exit
    if ($type_export != '') {
    	if ($type_export === 'xls') {
    		// Restructuration des tableaux :
    		$rowsToBeExported = array();
    		foreach ($header_export as $header) {
    			$rowsToBeExported[0][] = $header;
    		}
    		foreach ($rows as $row) {
    			if (!empty($row['data'])) {
    				$rowsToBeExported[] = $row['data'];
    			}
    		}
    		 
    		// Création du fichier xls avec le tableau de valeurs donnés et l'ouvre :
    		$filename = 'dcp_'. $filter['filter']['field_anneedata_value']. '.xls';
    		uwwtd_create_xls($filename, $rowsToBeExported);
    	} else {
    		uwwtd_export_data($type_export, 'dcp_'.$filter['filter']['field_anneedata_value'], $header_export, $rows);
    	}
    }
   $features['dcp'] = uwwtd_build_feature($data, $optionstyle, $returnvalue);
    $rendermap = uwwtd_build_map($map, $features);
    
    unset($features);
    $rows_nb = array();
    $rows_load = array();
    $legend = array();
    $color =array();
    $total_nb=array();
    $total_load=array();
    if(!empty($graph)){
        foreach($graph as $type=>$item){
            if(empty($legend)){
                $legend = array_keys($item);
                foreach($legend as $i=>$j){
                    //dsm($j);
                    $legend[$i]= $GLOBALS['uwwtd']['reuse_type'][$j];
                    $color[$i]= $GLOBALS['uwwtd']['reuse_type_color'][$j];
                }
            }
            $line_nb = array();
            $line_load = array();
            
            $line_nb[] = $GLOBALS['uwwtd_value']['rcatype_labels'][$type];
            $line_load[] = $GLOBALS['uwwtd_value']['rcatype_labels'][$type];
            foreach($item as $k=>$v){
                $line_nb[] = $v['nb'];
                $line_load[] = $v['load'];
                //FIX ME nd@oieau.fr
                $total_nb[$k]+=$v['nb'];
                $total_load[$k]+=$v['load'];
                
            }
            $rows_nb[] = $line_nb;
            $rows_load[] = $line_load;  
        } 
    }
    

    // Pour afficher ce qui est attendu pour crééer un graph pulti colonnes (voir sur /discharge-points) :
//     dsm($rows_nb);
//     dsm($color);
//     dsm($legend);

    $dcp_graph_nb= uwwtd_page_render_column('graph-1', $legend, $color, $rows_nb, true);
    $dcp_graph=uwwtd_page_render_column('graph-2', $legend, $color, $rows_load, true);
    array_unshift($total_nb, t('Total'));
    array_unshift($total_load, t('Total'));
    //FIX ME nd@oieau.fr
    $rows_nb[]=$total_nb;
    $rows_load[]=$total_load;
    array_unshift($legend, t('Area type'));
    $dcp_table_nb = theme('table', array('rows'=>$rows_nb, 'header'=>$legend));
    $dcp_table = theme('table', array('rows'=>$rows_load, 'header'=>$legend));
    //dsm($rows[0]['data']);
    $build['tablesort_table'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
        );


   //============================Render the page ==> TODO use a true template/theme function 

    $page =  '<h1>'.$title.'</h1>';
    $page.=  $form;
    if(!empty($data)){
            $page.=   '<div class="rows map-row" >'.$rendermap.'</div>'
            
            . '<div class="rows charts-row" >'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content-extended">'
                    
                    .uwwtd_page_render_flipcard(t("Water reuse type of discharge point by number and by receving area type"), $dcp_graph_nb, $dcp_table_nb, '')
                  
                  .'</div>'
                .'</div>'
                .'<div class="cell charts-rows-cell">'
                  .'<div class="cell-content-extended">'
                    .uwwtd_page_render_flipcard(t("Water reuse type of discharge point by load entering and by receving area type"), $dcp_graph, $dcp_table, t("Volume of the discharge point is based on the volume entering in the linked waste water treatment plant's"))
                  .'</div>'
                .'</div>'
            .'</div>'
            . '<div class="rows data-table-row">'
			 . '<div class="data-table-header">'
			  . '<b>'.count($rows) ." results</b>"		
				. '<div class="hideprint export-result"> | Download results: '
					.l('csv', 'discharge-points/csv', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters())).' '
					.l('xls', 'discharge-points/xls', array('attributes'=>array('class'=>array("export-result-btn")),'query'=>drupal_get_query_parameters()))
				.'</div>'
			 .'</div>'
				
                . '<div class="view-content content-table">'. drupal_render($build) . '</div>'
			
                . '<!--markerprinttable-->'  //use for print wkhtmltopdf
            .'</div>';
    }
    else{
        $page.="<p><strong>Sorry, your search returned no results</strong></p>";
    }
    unset($data);
     if (true === $put_in_cache) {
        cache_set($cache_name, array('page' => $page), 'cache_page', CACHE_PERMANENT);
     }

     return $page;
}
 
/* 
function uwwtd_sensitive_areas_page() {
  $form = drupal_get_form('uwwtd_sensitive_areas_form');
  //drupal_add_css(drupal_get_path('module', 'uwwtd').'/css/statistics.css');
  return render($form);
}
*/
/**
 * Implements hook_form().
 */
function uwwtd_sensitive_areas_form($form, &$form_state) {
  // Get available years for data
  $query = db_select('field_data_field_anneedata', 'a');
  $query->fields('a', array('field_anneedata_value'));
  $query->orderBy('field_anneedata_value', 'DESC');
  $results = $query->execute();
  while($record = $results->fetchAssoc()) {
    $years[$record['field_anneedata_value']] = $record['field_anneedata_value'];
  }

  $form['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => $years,
    '#default_value' => isset($form_state['values']['year']) ? $form_state['values']['year'] : current($years),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#attributes' => array('class' => array('btn-primary', 'btn')),
    '#value' => t('Filter')
  );

  if(isset($form_state['storage']['result'])) {
    $form['#suffix'] = $form_state['storage']['result'];
  } else {
    $form['#suffix'] = theme_datatable(uwwtd_build_sensitive_areas_table(current($years)));
  }

  return $form;
}

/**
 * Form submit.
 */
function uwwtd_sensitive_areas_form_submit(&$form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $table = uwwtd_build_sensitive_areas_table($form_state['values']['year']);
  dsm($table);
  //$form_state['storage']['result'] = theme_datatable($table);
}

function uwwtd_build_sensitive_areas_table($year = '') {
  $rows = array();
  // LG $header = array('ID', 'Name', 'UWWTPs', 'N-removal', 'P-removal', 'N & P-removal');
  $header = array('ID', 'Name', 'UWWTPs', 'N-removal', 'P-removal', 'Article 5.4 applied');

  $type = 'receiving_area';

  $query = "SELECT nid FROM {node} n";

  if($year != '') {
    $query .= " LEFT JOIN {field_data_field_anneedata} fdfa ON fdfa.entity_id = n.nid WHERE type = :type AND fdfa.field_anneedata_value = :year";
    $result = db_query($query, array(':type' => $type, ':year' => $year));
  } else{
    $query .= " WHERE type = :type";
    $result = db_query($query, array(':type' => $type));
  }

  $nids = array();

  foreach ($result as $row) {
    $nids[] = $row->nid;
  }

  $nodes = node_load_multiple($nids);

  foreach ($nodes as $key => $node) {
    // Initialize counters
    $counter_stations = 0;
    $counter_stations_n = 0;
    $counter_stations_p = 0;
    //$counter_stations_n_and_p = 0;


    // Load discharge points, stations and agglomerations
    if(isset($node->field_rcadcpliste['und'])) {
      foreach ($node->field_rcadcpliste['und'] as $key => $dp_ref) {
        $discharge_point = node_load($dp_ref['nid']);
        if(isset($discharge_point->field_dcpuwwliste['und'])) {
          foreach ($discharge_point->field_dcpuwwliste['und'] as $key => $station_ref) {
            $station = node_load($station_ref['nid']);
            if(isset($station->field_uwwpremoval['und'][0]['value']) && $station->field_uwwpremoval['und'][0]['value'] == 1) {
              $counter_stations_p++;
            }
            if(isset($station->field_uwwnremoval['und'][0]['value']) && $station->field_uwwnremoval['und'][0]['value'] == 1) {
              $counter_stations_n++;
            }

            //LG
			//if((isset($station->field_uwwnremoval['und'][0]['value']) && $station->field_uwwnremoval['und'][0]['value'] == 1) && (isset($station->field_uwwpremoval['und'][0]['value']) && $station->field_uwwpremoval['und'][0]['value'] == 1)) {
              //$counter_stations_n_and_p++;
            //}
            $counter_stations++;
          }
        }
      }
    }
   if ($counter_stations_n != 0) $counter_stations_n = "Yes"; else $counter_stations_n = "No";
   if ($counter_stations_p != 0) $counter_stations_p = "Yes"; else $counter_stations_p = "No";
   if ($node->field_rca54applied['und'][0]['value'] == 1) $display_rca54applied = "Yes"; else $display_rca54applied = "No";

    $rows[] = array(
        array('data' => $node->field_inspireidlocalid['und'][0]['value']),
        array('data' => l($node->title, 'node/'.$node->nid)),
        array('data' => $counter_stations),
        array('data' => $counter_stations_n),
        array('data' => $counter_stations_p),
        array('data' => $display_rca54applied),
      );
  }
  $table = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('class' => 'table', 'id' => 'statsTable', 'datatable_options' => array('bTableTools' => TRUE)),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => '',
    'empty' => '',
  );

  return $table;
}



//====================================================================== Others function for pages rendering
/**
 * Give available years for the country.
 */
function getYearsPossibles() {
	$year = uwwtd_get_max_annee();
	
	// Get available years for data
	$query = db_select('field_data_field_anneedata', 'a');
	$query->fields('a', array('field_anneedata_value'));
	$query->orderBy('field_anneedata_value', 'DESC');
	$results = $query->execute();
	$years = array();
	$count = 0;
	while ($record = $results->fetchAssoc()) {
		$years[$record['field_anneedata_value']] = $record['field_anneedata_value'];
		if ($record['field_anneedata_value']!=$year){$count++;}
	}
	return $years;
}

function uwwtd_page_render_flipcard($title, $front, $back, $msg=''){
    $content = 
        '<div class="container flip">'
          .'<div class="leftgraph front">'
            . '<div class="flot-title">'
                .'<div class="flip-title">'.$title.'</div>'
                .'<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper to-table" title="See the data table" alt="See the data table"></div>'
            . '</div>'
            . '<div class="flip-content view-content"><div class="flip-msg">'.$msg .'</div>'.$front.'</div>'
          .'</div>'

          .'<div class="leftgraph back">'
            . '<div class="flot-title flip-title-back">'
                .'<div class="flip-title">'.$title.'</div>'
                .'<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper to-chart" title="See diagram" alt="See diagram"></div>'
            . '</div>'
            . '<div class="flip-content view-content"><div class="flip-msg">'.$msg .'</div>'. $back .'</div>'
          .'</div>'
        .'</div>';
    return $content;
}

function uwwtd_page_build_flot_piechart($data, $id){
    $options = new flotStylePie();
    $options->legend  = new StdClass();
    $options->legend->show = true;
    $options->legend->position = "ne";
    $options->resize = true;
    $options->legend->labelFormatter = 'uwwtd_labelformaterpie_legend';
    $options->series->pie->label->formatter = 'uwwtd_labelformaterpie';
    $options->replace = new StdClass();
    $options->replace->transform_on_callback = array($options->legend->labelFormatter, $options->series->pie->label->formatter);

    return array(
        'data' => $data,
        'element' => array(
            'id' => $id,
            'style' => 'width: 500px; height: 200px; padding: 0px; position: relative;',
        ),
        'options' => clone $options,
    );
    
}

function uwwtd_page_render_column($id, $legend, $color, $rows, $stacked=false){
  $chart = array(
    'id' => $id,
    'type' => ($stacked?'StackedColumnChart':'ColumnChart'),
    'legend' => $legend,
    'rows' => $rows,
    'width'=>510,
    'height'=>190,
  );
  
  if(!empty($color)){
      $chart['color'] = $color;
  }
  
  
  return d3_draw($chart);
    
    
}

function uwwtd_build_feature($data, $option, &$returnvalue, $type=''){
    //Sort data by area
    if(isset($data[0]['area'])){
        $area = array();
        foreach ($data as $key => $row)
        {
            $area[$key] = (float)$row['area'];
        }
        array_multisort($area, SORT_DESC, $data);
        unset($area);
    }
    //Load openlayers styles 
    $styles = openlayers_styles();
    $feature = array();
    $i = 0;
    $returnvalue = '';
    foreach($data as $row) {
        if(isset($row['wkt'])){
            $i++;
            $description = '';
            if (isset($option['popup'])) {
                foreach($option['popup'] as $field => $detail) {
                    $description .= "  \n  <span class=\"views-field views-field-".$field."\">    <span class=\"views-label views-label-".$field."\">".$detail['label'].": </span>    <span class=\"field-content\">"
                                    . $row[$field] .
                                    "</span>  </span>";
                }
            }

            //$title = l($row['title'], uwwtd_get_url_path($option['popup_title_type'], array(strtolower($row['field_inspireidlocalid_value']), $row['field_anneedata_value'])), array('attributes'=>array('target'=>'_blank')));
            $title = l($row['title'], 'node/'.$row['nid'], array('html'=>true,'attributes'=>array('target'=>'_blank')));
            //Color of the feature
            $color = isset($option['default_color'])?$option['default_color']:'#000000';
            $style = null;
            if (isset($option['color']) && isset($row[ $option['color']['field']])) {
                if(isset($option['color']['list'][$row[ $option['color']['field'] ]])){
                    $color = $option['color']['list'][$row[ $option['color']['field'] ]];
                }
                
                //Style of the feature 
                $style_name = (isset($option['style_prefix'])?$option['style_prefix']:'').strtolower($row[ $option['color']['field'] ]);
                if(isset($styles[$style_name])){
                    $style = $style_name;
                }
                
            }
            //Weigth of the feature (radius of circle)
            $weight = 1;
            if (isset($option['weight'])) {
               foreach($option['weight']['list'] as $key => $value) {
                 if ($row[ $option['weight']['field'] ] < $value) {
                    $weight = $key;
                    break;
                 }
                 if ($value == 0) {
                    $weight = $key;
                 }
               }
            }
            $chart = array();
            //Pie chart def ===> FIX ME : is it really used ?
            if (isset($option['piechart'])) {
                foreach($option['piechart'] as $field => $detail) {
                    $chart[] = array(
                        'label' => $detail['label'],
                        'value' => $row[$field],
                        'color' => $detail['color'],


                    );
                }
                $returnvalue .= uwwtd_get_svg_piechart($i, $row, $option, $option['piechart_weight_style'][$weight]);

            }
            
            $feature[] = array(
                'projection' => 'EPSG:4326',
                'attributes' => array(
                    'name'=> $title,
                    'description'=> $description,
                    'weight' => $weight,
                    'color' => $color,//'#000000',
                    'data' => $chart,
                    'piechartid' => 'pie_'.$type .$i,
                ),
                'style' => ($style?$styles[$style]->data:null),
                'style_name' => $style,
                
                'wkt' => $row['wkt'],
            );
        }
    }
    //dsm($feature);
    return $feature;
}

function uwwtd_build_map($map, $features) {

  foreach($features as $overlay => $feature) {
      if (!isset($map['layers'][$overlay])) {
        drupal_set_message(t('Trying to render a overlay ' . $overlay . ' on a map without the placeholder layer'), 'error');
      }
    $map['layers'][$overlay]['features'] = $feature;
  }
  //FIX ME : manage overridens style in openlayer_plus_legend for layers if style is define on feature (renderinf is make before feature association)
  
  
  // Return themed map if no errors found
  if (empty($map['errors'])) {
      
    libraries_load('proj4js');
    openlayers_add_js_projection_definition(
      openlayers_get_projection_by_identifier('EPSG:3857')
    );  
      
    $js = array('openlayers' => array('maps' => array($map['id'] => $map)));
    //put js in html output in order to put all block in cache
    $output =
    '<script>jQuery.extend(Drupal.settings,'.json_encode($js).');</script>'.
    '<div class="view-content">
    <div class="openlayers-views-map">'.
    theme('openlayers_map', array(
      'map' => $map,
      'map_name' => $map['map_name'],
    ))
    .'</div></div>'
    ;
  }

  return $output;
}

function uwwtd_data_contain_art17($data, $field){
    if(!empty($data)){
        $d = array_unique(array_column($data, $field));
        if(count($d)>1) return true;
    }
    return false;
}

function uwwtd_filter_form($list_field, $filter, $id = 'views-exposed-form-agglomeration-conformity-page')
{
$render_field = '';
foreach($list_field as $field_name => $data) {
    $render_field .= '<div class="views-exposed-widget views-widget-filter-'.$field_name.'" id="edit-field-'.$field_name.'-wrapper">
    <label for="edit-'.$field_name.'">
    '.$data['title'].'
    </label>
    ';
        switch($data['type']) {
            case 'selectoperator':
                 $render_field .= '<div class="views-operator">
                    <div class="form-type-select form-item-'.$field_name.'-op form-item form-group">
                        <select name="'.$field_name.'_op" id="edit-'.$field_name.'-op" class="form-control form-select">';
                        foreach($data['data'] as $operator => $label) {
                            $selected = "";
                            if (isset($filter['filter'][$field_name.'_op']) && $filter['filter'][$field_name.'_op'] == $operator) {
                                $selected = 'selected="selected"';
                            }
                            $render_field .= '<option value="'.htmlspecialchars($operator).'" '. $selected . '>'.$label.'</option>';
                        }
                        $render_field .= '</select>
                    </div>
                </div>
                <div class="views-widget">
                    <div class="form-type-textfield form-item-'.$field_name.'-value form-item form-group" style="display: block;">
                        <input type="text" maxlength="128" size="30" value="';

                        if (isset($filter['filter'][$field_name]['value']) && $filter['filter'][$field_name]['value']) {
                            $render_field .= $filter['filter'][$field_name]['value'];
                        }

                        $render_field .= '" name="'.$field_name.'[value]" id="edit-'.$field_name.'-value" class="form-control form-text">
                    </div>
                    <!--
                    <div class="form-type-textfield form-item-'.$field_name.'-min form-item form-group" style="display: none;">
                        <input type="text" maxlength="128" size="30" value="" name="'.$field_name.'[min]" id="edit-'.$field_name.'-min" class="form-control form-text">
                    </div>
                    <div class="form-type-textfield form-item-'.$field_name.'-max form-item form-group" style="display: none;">
                        <label for="edit-'.$field_name.'-max">And </label>
                        <input type="text" maxlength="128" size="30" value="" name="'.$field_name.'[max]" id="edit-'.$field_name.'-max" class="form-control form-text">
                    </div>
                    -->
                </div>';
                break;
            case 'select':
//             case 'field_aggcompliance_value':
//             case 'field_aggart3compliance_value':
//             case 'field_aggart4compliance_value':
//             case 'field_aggart5compliance_value':
//             case 'field_aggart6compliance_value':
                $render_field .= '<div class="views-widget">
                    <div class="form-type-select form-item-'.$field_name.' form-item form-group">
                    <select name="'.$field_name.'" id="edit-'.$field_name.'" class="form-control form-select">
                    ';
                foreach($data['data'] as $key => $value) {
                    $selected = "";
                    // dsm($filter['filter'][$field_name]);
                    // dsm($key);
                    if (isset($filter['filter'][$field_name]) && $filter['filter'][$field_name] == $key) {
//                         dsm('find');
                        $selected = 'selected="selected"';
                    }
                    $render_field .= '<option  value="'.$key.'" '.$selected.'>'.$value.'</option>';
                }
                $render_field .= '</select></div>
                </div>';
                break;
			case 'selectarticle':
                 $render_field .= '<div class="views-article">
				 <div class="form-type-select form-item-'.$field_name.'-art form-item form-group">
                        <select name="'.$field_name.'_art" id="edit-'.$field_name.'-art" class="form-control form-select">';
						foreach($data['data'] as $operator => $label) {
							$selected = "";
                            if (isset($filter['filter'][$field_name.'_art']['value']) && $filter['filter'][$field_name.'_art']['value'] == $operator) {

                                $selected = 'selected="selected"';
                            }
                            $render_field .= '<option value="'.htmlspecialchars($operator).'" '. $selected . '>'.$label.'</option>';
                        }
                        $render_field .= '</select>
                    </div>
                </div>';
                break;
			case 'selectregion':
                $render_field .= '<div class="views-widget">
                    <div class="form-type-select form-item-'.$field_name.' form-item form-group">
                    <select name="'.$field_name.'" id="edit-'.$field_name.'" class="form-control form-select">
                    ';
                foreach($data['data'] as $key => $value) {
                    $selected = "";
                    // dsm($filter['filter'][$field_name]);
                    // dsm($key);
                    if (isset($filter['filter'][$field_name]) && $filter['filter'][$field_name] == $key) {
//                         dsm('find');
                        $selected = 'selected="selected"';
                    }
                    $render_field .= '<option  value="'.$key.'" '.$selected.'>'.$value.'</option>';
                }
                $render_field .= '</select></div>
                </div>';
                break;		

        }
    $render_field .= '
    </div>';
}

    return '<div class="view-filters">
<form accept-charset="UTF-8" id="'.$id.'" method="get" action="">
<div>
<div class="views-exposed-form">
<div class="views-exposed-widgets clearfix">

'. $render_field . '

<div class="views-exposed-widget views-submit-button">
    <button class="btn btn-info form-submit" type="submit" value="Apply" name="" id="edit-submit-agglomeration-conformity">Apply</button>
</div>
<div class="views-exposed-widget views-reset-button">
    <button class="btn btn-default form-submit" type="submit" value="Reset" name="op" id="edit-reset">Reset</button>
</div>

</div>
</div>
</div>
</form>
</div>';
}

function uwwtd_get_form_filter($option){
    //check if we must reset
    if (isset($_GET['op']) && $_GET['op'] == 'Reset') {
        drupal_goto(request_path());
    }
    $filter = array();

    if (isset($_GET['field_anneedata_value']) && in_array($_GET['field_anneedata_value'], $option['allyear'])) {
        $filter['filter']['field_anneedata_value'] = $_GET['field_anneedata_value'];
    } else {
        $filter['filter']['field_anneedata_value'] = uwwtd_get_max_annee();
    }
    if (isset($_GET['field_agggenerated_value_op']) && in_array($_GET['field_agggenerated_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_agggenerated_value']['value'])
        && '' != ($_GET['field_agggenerated_value']['value'] = trim($_GET['field_agggenerated_value']['value']))
        && ctype_digit($_GET['field_agggenerated_value']['value'])
//         && !empty($_GET['field_agggenerated_value']['value'])
        ) {
        $filter['filter']['field_agggenerated_value_op'] = $_GET['field_agggenerated_value_op'];
        $filter['filter']['field_agggenerated_value']['value'] = (int)$_GET['field_agggenerated_value']['value'];
    }


    if (isset($_GET['field_aggcompliance_value']) && in_array($_GET['field_aggcompliance_value'],  array_keys($GLOBALS['uwwtd']['ui']['compliance']))
        && $_GET['field_aggcompliance_value'] != 'All') {
        $filter['filter']['field_aggcompliance_value'] = $_GET['field_aggcompliance_value'];
    }


    if (isset($_GET['field_aggart3compliance_value']) && in_array($_GET['field_aggart3compliance_value'],  array_keys($GLOBALS['uwwtd']['ui']['compliance']))
        && $_GET['field_aggart3compliance_value'] != 'All') {
        $filter['filter']['field_aggart3compliance_value'] = $_GET['field_aggart3compliance_value'];
    }

    if (isset($_GET['field_aggart4compliance_value']) && in_array($_GET['field_aggart4compliance_value'],  array_keys($GLOBALS['uwwtd']['ui']['compliance']))
        && $_GET['field_aggart4compliance_value'] != 'All') {
        $filter['filter']['field_aggart4compliance_value'] = $_GET['field_aggart4compliance_value'];
    }

    if (isset($_GET['field_aggart5compliance_value'])  && in_array($_GET['field_aggart5compliance_value'],  array_keys($GLOBALS['uwwtd']['ui']['compliance']))
        && $_GET['field_aggart5compliance_value'] != 'All') {
        $filter['filter']['field_aggart5compliance_value'] = $_GET['field_aggart5compliance_value'];
    }

    if (isset($_GET['field_aggart6compliance_value']) && in_array($_GET['field_aggart6compliance_value'],  array_keys($GLOBALS['uwwtd']['ui']['compliance']))
        && $_GET['field_aggart6compliance_value'] != 'All') {
        $filter['filter']['field_aggart6compliance_value'] = $_GET['field_aggart6compliance_value'];
    }

    //agglo/treatment
    if (isset($_GET['field_aggc2_value_op']) && in_array($_GET['field_aggc2_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_aggc2_value']['value'])
        && '' != ($_GET['field_aggc2_value']['value'] = trim($_GET['field_aggc2_value']['value']))
        && ctype_digit($_GET['field_aggc2_value']['value'])
//         && !empty($_GET['field_aggc2_value']['value'])
        ) {
        $filter['filter']['field_aggc2_value_op'] = $_GET['field_aggc2_value_op'];
        $filter['filter']['field_aggc2_value']['value'] = (int)$_GET['field_aggc2_value']['value'];
    }

    if (isset($_GET['field_aggc1_value_op']) && in_array($_GET['field_aggc1_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_aggc1_value']['value'])
        && '' != ($_GET['field_aggc1_value']['value'] = trim($_GET['field_aggc1_value']['value']))
        && ctype_digit($_GET['field_aggc1_value']['value'])
//         && !empty($_GET['field_aggc1_value']['value'])
        ) {
        $filter['filter']['field_aggc1_value_op'] = $_GET['field_aggc1_value_op'];
        $filter['filter']['field_aggc1_value']['value'] = (int)$_GET['field_aggc1_value']['value'];
    }

    if (isset($_GET['field_aggpercwithouttreatment_value_op']) && in_array($_GET['field_aggpercwithouttreatment_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_aggpercwithouttreatment_value']['value'])
        && '' != ($_GET['field_aggpercwithouttreatment_value']['value'] = trim($_GET['field_aggpercwithouttreatment_value']['value']))
        && ctype_digit($_GET['field_aggpercwithouttreatment_value']['value'])
//         && !empty($_GET['field_aggpercwithouttreatment_value']['value'])
        ) {
        $filter['filter']['field_aggpercwithouttreatment_value_op'] = $_GET['field_aggpercwithouttreatment_value_op'];
        $filter['filter']['field_aggpercwithouttreatment_value']['value'] = (int)$_GET['field_aggpercwithouttreatment_value']['value'];
    }


    if (isset($_GET['field_uwwloadenteringuwwtp_value_op']) && in_array($_GET['field_uwwloadenteringuwwtp_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_uwwloadenteringuwwtp_value']['value'])
        && '' != ($_GET['field_uwwloadenteringuwwtp_value']['value'] = trim($_GET['field_uwwloadenteringuwwtp_value']['value']))
        && ctype_digit($_GET['field_uwwloadenteringuwwtp_value']['value'])
//         && !empty($_GET['field_uwwloadenteringuwwtp_value']['value'])
        ) {
        $filter['filter']['field_uwwloadenteringuwwtp_value_op'] = $_GET['field_uwwloadenteringuwwtp_value_op'];
        $filter['filter']['field_uwwloadenteringuwwtp_value']['value'] = (int)$_GET['field_uwwloadenteringuwwtp_value']['value'];
    }

    if (isset($_GET['field_uwwcompliance_value']) && in_array($_GET['field_uwwcompliance_value'],  array_keys($GLOBALS['uwwtd']['ui']['compliance']))
        && $_GET['field_uwwcompliance_value'] != 'All') {
        $filter['filter']['field_uwwcompliance_value'] = $_GET['field_uwwcompliance_value'];
    }

    if (isset($_GET['field_uwwbod5perf_value']) && in_array($_GET['field_uwwbod5perf_value'], array_keys($GLOBALS['uwwtd']['ui']['test_pass']))
        && $_GET['field_uwwbod5perf_value'] != 'All') {
        $filter['filter']['field_uwwbod5perf_value'] = $_GET['field_uwwbod5perf_value'];
    }

    if (isset($_GET['field_uwwcodperf_value']) && in_array($_GET['field_uwwcodperf_value'], array_keys($GLOBALS['uwwtd']['ui']['test_pass']))
        && $_GET['field_uwwcodperf_value'] != 'All') {
        $filter['filter']['field_uwwcodperf_value'] = $_GET['field_uwwcodperf_value'];
    }

    if (isset($_GET['field_uwwntotperf_value']) && in_array($_GET['field_uwwntotperf_value'], array_keys($GLOBALS['uwwtd']['ui']['test_pass']))
        && $_GET['field_uwwntotperf_value'] != 'All') {
        $filter['filter']['field_uwwntotperf_value'] = $_GET['field_uwwntotperf_value'];
    }

    if (isset($_GET['field_uwwptotperf_value']) && in_array($_GET['field_uwwptotperf_value'], array_keys($GLOBALS['uwwtd']['ui']['test_pass']))
        && $_GET['field_uwwptotperf_value'] != 'All') {
        $filter['filter']['field_uwwptotperf_value'] = $_GET['field_uwwptotperf_value'];
    }

    if (isset($_GET['field_uwwtreatmenttype_value']) && in_array($_GET['field_uwwtreatmenttype_value'], array_keys($GLOBALS['uwwtd']['ui']['treatment_type_in_place']))
        && $_GET['field_uwwtreatmenttype_value'] != 'All') {
        $filter['filter']['field_uwwtreatmenttype_value'] = $_GET['field_uwwtreatmenttype_value'];
    }

    //uwwtps/treatment
    if (isset($_GET['field_uwwprimarytreatment_value']) && in_array($_GET['field_uwwprimarytreatment_value'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['field_uwwprimarytreatment_value'] != 'All') {
        $filter['filter']['field_uwwprimarytreatment_value'] = $_GET['field_uwwprimarytreatment_value'];
    }

    if (isset($_GET['field_uwwsecondarytreatment_value']) && in_array($_GET['field_uwwsecondarytreatment_value'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['field_uwwsecondarytreatment_value'] != 'All') {
        $filter['filter']['field_uwwsecondarytreatment_value'] = $_GET['field_uwwsecondarytreatment_value'];
    }

    if (isset($_GET['field_uwwnremoval_value']) && in_array($_GET['field_uwwnremoval_value'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['field_uwwnremoval_value'] != 'All') {
        $filter['filter']['field_uwwnremoval_value'] = $_GET['field_uwwnremoval_value'];
    }

    if (isset($_GET['field_uwwpremoval_value']) && in_array($_GET['field_uwwpremoval_value'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['field_uwwpremoval_value'] != 'All') {
        $filter['filter']['field_uwwpremoval_value'] = $_GET['field_uwwpremoval_value'];
    }

    if (isset($_GET['field_uwwboddischarge_value_op']) && in_array($_GET['field_uwwboddischarge_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_uwwboddischarge_value']['value'])
        && '' != ($_GET['field_uwwboddischarge_value']['value'] = trim($_GET['field_uwwboddischarge_value']['value']))
        && ctype_digit($_GET['field_uwwboddischarge_value']['value'])
//         && !empty($_GET['field_uwwboddischarge_value']['value'])
        ) {
        $filter['filter']['field_uwwboddischarge_value_op'] = $_GET['field_uwwboddischarge_value_op'];
        $filter['filter']['field_uwwboddischarge_value']['value'] = (int)$_GET['field_uwwboddischarge_value']['value'];
    }

    if (isset($_GET['field_uwwcoddischarge_value_op']) && in_array($_GET['field_uwwcoddischarge_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_uwwcoddischarge_value']['value'])
        && '' != ($_GET['field_uwwcoddischarge_value']['value'] = trim($_GET['field_uwwcoddischarge_value']['value']))
        && ctype_digit($_GET['field_uwwcoddischarge_value']['value'])
//         && !empty($_GET['field_uwwcoddischarge_value']['value'])
        ) {
        $filter['filter']['field_uwwcoddischarge_value_op'] = $_GET['field_uwwcoddischarge_value_op'];
        $filter['filter']['field_uwwcoddischarge_value']['value'] = (int)$_GET['field_uwwcoddischarge_value']['value'];
    }

    if (isset($_GET['field_uwwndischarge_value_op']) && in_array($_GET['field_uwwndischarge_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_uwwndischarge_value']['value'])
        && '' != ($_GET['field_uwwndischarge_value']['value'] = trim($_GET['field_uwwndischarge_value']['value']))
        && ctype_digit($_GET['field_uwwndischarge_value']['value'])
//         && !empty($_GET['field_uwwndischarge_value']['value'])
        ) {
        $filter['filter']['field_uwwndischarge_value_op'] = $_GET['field_uwwndischarge_value_op'];
        $filter['filter']['field_uwwndischarge_value']['value'] = (int)$_GET['field_uwwndischarge_value']['value'];
    }

    if (isset($_GET['field_uwwpdischarge_value_op']) && in_array($_GET['field_uwwpdischarge_value_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['field_uwwpdischarge_value']['value'])
        && '' != ($_GET['field_uwwpdischarge_value']['value'] = trim($_GET['field_uwwpdischarge_value']['value']))
        && ctype_digit($_GET['field_uwwpdischarge_value']['value'])
        ) {
        $filter['filter']['field_uwwpdischarge_value_op'] = $_GET['field_uwwpdischarge_value_op'];
        $filter['filter']['field_uwwpdischarge_value']['value'] = (int)$_GET['field_uwwpdischarge_value']['value'];
    }
    
    
    
	if (isset($_GET['field_article17_uwwtp_value_art']) && in_array($_GET['field_article17_uwwtp_value_art'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['field_article17_uwwtp_value_art'] != 'All') {
        $filter['filter']['field_article17_uwwtp_value_art']['value'] = $_GET['field_article17_uwwtp_value_art'];
        $filter['filter']['field_article17_uwwtp_value_format'] = $_GET['field_article17_uwwtp_value_art']==1?'Yes':'No';
        
    }

	if (isset($_GET['field_article17_agglo_value_art']) && in_array($_GET['field_article17_agglo_value_art'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['field_article17_agglo_value_art'] != 'All') {
        $filter['filter']['field_article17_agglo_value_art']['value'] = $_GET['field_article17_agglo_value_art'];
        $filter['filter']['field_article17_agglo_value_format'] = $_GET['field_article17_agglo_value_art']==1?'Yes':'No';
    }

	if (isset($_GET['field_regionnuts_value']) && array_key_exists($_GET['field_regionnuts_value'], $option['allnuts'])  && $_GET['field_regionnuts_value'] != 'All') {
	  $filter['filter']['field_regionnuts_value'] = $_GET['field_regionnuts_value'];
    } 

	if (isset($_GET['rbd']) && array_key_exists($_GET['rbd'], $option['allrbd'])  && $_GET['rbd'] != 'All') {
	  $filter['filter']['rbd'] = $_GET['rbd'];
    }
	
    
    //=======RCA
    
    if (isset($_GET['tot_design_capacity_op']) && in_array($_GET['tot_design_capacity_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['tot_design_capacity']['value'])
        && '' != ($_GET['tot_design_capacity']['value'] = trim($_GET['tot_design_capacity']['value']))
        && ctype_digit($_GET['tot_design_capacity']['value'])
        ) {
        $filter['filter']['tot_design_capacity_op'] = $_GET['tot_design_capacity_op'];
        $filter['filter']['tot_design_capacity']['value'] = (int)$_GET['tot_design_capacity']['value'];
    }
    if (isset($_GET['tot_entering_load_op']) && in_array($_GET['tot_entering_load_op'], array_keys($GLOBALS['uwwtd']['ui']['operator']))
        && isset($_GET['tot_entering_load']['value'])
        && '' != ($_GET['tot_entering_load']['value'] = trim($_GET['tot_entering_load']['value']))
        && ctype_digit($_GET['tot_entering_load']['value'])
        ) {
        $filter['filter']['tot_entering_load_op'] = $_GET['tot_entering_load_op'];
        $filter['filter']['tot_entering_load']['value'] = (int)$_GET['tot_entering_load']['value'];
    }
    if (isset($_GET['zonetype']) && array_key_exists($_GET['zonetype'], $option['zonetype'])  && $_GET['zonetype'] != 'All') {
	  $filter['filter']['zonetype'] = $_GET['zonetype'];
    }
    if (isset($_GET['rcacat']) && array_key_exists($_GET['rcacat'], $option['rcacat'])  && $_GET['rcacat'] != 'All') {
	  $filter['filter']['rcacat'] = $_GET['rcacat'];
    }
    
    if (isset($_GET['rca52_art']) && in_array($_GET['rca52_art'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['rca52_art'] != 'All') {
        $filter['filter']['rca52_art']['value'] = $_GET['rca52_art'];
    }
    if (isset($_GET['rca54_art']) && in_array($_GET['rca52_art'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['rca54_art'] != 'All') {
        $filter['filter']['rca54_art']['value'] = $_GET['rca54_art'];
    }
    if (isset($_GET['rca58_art']) && in_array($_GET['rca58_art'], array_keys($GLOBALS['uwwtd']['ui']['yes_no']))
        && $_GET['rca58_art'] != 'All') {
        $filter['filter']['rca58_art']['value'] = $_GET['rca58_art'];
    }

    
    
    if(!empty($option['order'])){
        foreach($option['order'] as $k=>$v){
            
            if(isset($v['sort_field'])){
                $sort_options[$k] = $v['sort_field'];
            }
            else{
                $sort_options[$k] = $k;
            }
        }
    }
    if (isset($_GET['order']) && in_array($_GET['order'], $sort_options)) {
        $filter['order'] = $_GET['order'];
        if (isset($_GET['sort']) && in_array($_GET['sort'], array('asc', 'desc'))) {
            $filter['sort'] = $_GET['sort'];
        } else {
            $filter['sort'] = 'asc';
        }
        if (isset($_GET['sort_mod'])) {
            $filter['sort_mod'] = $_GET['sort_mod'];
        } else {
            $filter['sort_mod'] = 'text';
        }
        
    } 
    else {
        $filter['order'] = $option['default']['order'];
        $filter['sort'] = $option['default']['sort'];
        $filter['sort_mod'] = isset($option['default']['sort_mod'])?$option['default']['sort_mod']:'';
    }
    return $filter;
}

function uwwtd_filter_data($filter, &$data, $forcefilter = array()){
     //For RBD data are already filtered
     //unset($filter['filter']['rbd']);
//     dsm($forcefilter);
    //no need to treat annee, already filter by request
    if (isset($filter['filter']['field_anneedata_value'])) {
        unset($filter['filter']['field_anneedata_value']);
    }
    //get field with operator
    $list_operator_field = array();
    foreach($filter['filter'] as $field_op => $operator) {
        if (substr($field_op, -3) == '_op') {
            $field_name_value = substr($field_op, 0, strlen($field_op) -3);
            $list_operator_field[] = array(
                'field' => $field_name_value,
                'op' => $operator,
                'value' => $filter['filter'][$field_name_value]['value'],
            );
            unset($filter['filter'][$field_op], $filter['filter'][$field_name_value]);
        }
    }

	$list_operator_field_art = array();
	 foreach($filter['filter'] as $field_art => $article) {
		if (substr($field_art, -4) == '_art') {
			$field_name_value_art = substr($field_art, 0, strlen($field_art) -4);
			$list_operator_field_art[] = array(
				'field' => $field_name_value_art,
				'art' => $article,
				'value' => $filter['filter'][$field_name_value_art]['value'],
			);
			unset($filter['filter'][$field_art], $filter['filter'][$field_name_value_art]);
		}
	}
    //for now treat only forcefilter with _op
    if (!empty($forcefilter)) {
        foreach($forcefilter['filter'] as $field_op => $operator) {
            if (substr($field_op, -3) == '_op') {
                $field_name_value = substr($field_op, 0, strlen($field_op) -3);
                $list_operator_field[] = array(
                    'field' => $field_name_value,
                    'op' => $operator,
                    'value' => $forcefilter['filter'][$field_name_value]['value'],
                );
                unset($forcefilter['filter'][$field_op], $forcefilter['filter'][$field_name_value]);
            }
        }
    }
    $datafilter = array();
    foreach($data as $row) {
        $keepRow = true;
        foreach($filter['filter'] as $field => $value) {
            if ($row[$field] != $value) {
                $keepRow = false;
//                 dsm(__LINE__);
                break;
            }
        }
        if ($keepRow == true) {
            foreach($list_operator_field as $one_operator) {
                $keepRow = $GLOBALS['uwwtd']['ui']['operatorfunction'][ $one_operator['op'] ]($row[ $one_operator['field'] ], $one_operator['value']);
                if ($keepRow == false) {
//                     dsm(__LINE__);
                    break;
                }
            }
        }
        
        //change nd@oieau.fr : not just art 17
		if ($keepRow == true) {
		 foreach($list_operator_field_art as $one_operator_art) {
			if($one_operator_art['art']['value'] == 0){
				if((isset($row[$one_operator_art['field']]) && $row[$one_operator_art['field']] =="1" )){
					$keepRow = false;
						break;
				}
			}
			if($one_operator_art['art']['value'] == 1){
				if((isset($row[$one_operator_art['field']]) && $row[$one_operator_art['field']] =="0" )){
					$keepRow = false;
						break;
				}
			}
		 }
		}
        
        //treat force filter
        if ($keepRow == true) {
            if (!empty($forcefilter)) {
                foreach($forcefilter['filter'] as $field => $value) {
                    if ($row[$field] != $value) {
                        $keepRow = false; 
//                         dsm(__LINE__);
//                         dsm($row);                        
                        break;
                    }
                } 
            }
        } 

        if ($keepRow == true) {
            $datafilter[] = $row;
        }
    }
    $data = $datafilter;
}

function uwwtd_sort_data($filter, $order, &$data){
    //get field to sort
    if (isset($filter['order'])) {
        //dsm('sort : '.$filter['order']);
        //dsm($order);
        //dsm($data);
        $fieldname =  $filter['order'];//array_search($filter['order'], $order);
        if (false === $fieldname) {
            return;
        }
        $GLOBALS['uwwtd']['ui']['sortfield'] = $fieldname;
        $GLOBALS['uwwtd']['ui']['sortmod'] = isset($filter['sort_mod'])?$filter['sort_mod']:'text';
        usort($data, $GLOBALS['uwwtd']['ui']['sort'][$filter['sort']]);
    }
}

function uwwtd_is_filter_default($filter, $option){
    $result = false;
    if (count($filter['filter']) == 1 && isset($filter['filter']['field_anneedata_value'])
        && $filter['order'] == $option['default']['order']
        && $filter['sort'] == $option['default']['sort']) {
        $result = true;
    }
    return $result;
}

