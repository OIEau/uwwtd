<?php


function uwwtd_page_stats_querys()
{
	$title = 'Querys for national stats';
	drupal_set_title($title);  //todo
	$page =  '<h1 align="center">'.$title.'</h1><br/>';
	dsm('function uwwtd_stat_national_compliance');
	dsm(uwwtd_stat_national_compliance());
	dsm('function uwwtd_stat_national_uwwtp(\'compliance\')');
	dsm(uwwtd_stat_national_uwwtp('compliance'));
	dsm('function uwwtd_stat_national_uwwtp(\'treatmenttype\')');
	dsm(uwwtd_stat_national_uwwtp('treatmenttype'));
	dsm('function uwwtd_stat_national_treatmentplant');
	dsm(uwwtd_stat_national_treatmentplant());
	dsm('function uwwtd_stat_national_ms_level');
	dsm(uwwtd_stat_national_ms_level());
	dsm('function uwwtd_stat_national_uwwtp_agglo');
	dsm(uwwtd_stat_national_uwwtp_agglo());
	dsm('function uwwtd_stat_national_generated_by_agglomeration');
	dsm(uwwtd_stat_national_generated_by_agglomeration());


	return $page;
}

function uwwtd_stat_national_ms_level()
{

	$data = array();
	$param = array();
/*
 *
 	field_msldischargepipelines
 	field_msldischargeships
 	field_msldischargeothers
 	field_mslreusesoilagriculture
 	field_mslreuseothers
 	field_msldisposallandfill
 	field_msldisposalincineration
 	field_msldisposalothers
 			 left join
            {field_data_field_uwwpincoming} f_uwwpincoming
                ON n.nid = f_uwwpincoming.entity_id
 *
 */
	$query = 'select
				SUM(f_msldischargepipelines.field_msldischargepipelines_value) AS disc_pipelines_SUM,
				SUM(f_msldischargeships.field_msldischargeships_value) AS disc_ships_SUM,
				SUM(f_msldischargeothers.field_msldischargeothers_value) AS disc_others_SUM,
				SUM(f_mslreusesoilagriculture.field_mslreusesoilagriculture_value) AS ru_agriculture_SUM,
				SUM(f_mslreuseothers.field_mslreuseothers_value) AS ru_others_SUM,
				SUM(f_msldisposallandfill.field_msldisposallandfill_value) AS disp_landfill_SUM,
				SUM(f_msldisposalincineration.field_msldisposalincineration_value) AS disp_incineration_SUM,
				SUM(f_msldisposalothers.field_msldisposalothers_value) AS disp_others_SUM
    from {node} n
    	left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
		 left join
            {field_data_field_msldischargepipelines} f_msldischargepipelines
                ON n.nid = f_msldischargepipelines.entity_id
		left join
            {field_data_field_msldischargeships} f_msldischargeships
                ON n.nid = f_msldischargeships.entity_id
		left join
            {field_data_field_msldischargeothers} f_msldischargeothers
                ON n.nid = f_msldischargeothers.entity_id
		left join
            {field_data_field_mslreusesoilagriculture} f_mslreusesoilagriculture
                ON n.nid = f_mslreusesoilagriculture.entity_id
		left join
            {field_data_field_mslreuseothers} f_mslreuseothers
                ON n.nid = f_mslreuseothers.entity_id
		left join
            {field_data_field_msldisposallandfill} f_msldisposallandfill
                ON n.nid = f_msldisposallandfill.entity_id
		left join
            {field_data_field_msldisposalincineration} f_msldisposalincineration
                ON n.nid = f_msldisposalincineration.entity_id
		left join
            {field_data_field_msldisposalothers} f_msldisposalothers
                ON n.nid = f_msldisposalothers.entity_id
    where n.type = :typenode';


	$param = array(
			':typenode' => 'ms_level',
	);


	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {

			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	//     dsm($data);
	return $data;
}

function uwwtd_stat_national_treatmentplant()
{

	$data = array();
	$param = array();

	$query = 'select
					SUM(f_uwwbodincoming.field_uwwbodincoming_value)AS bodin_sum,
					SUM(f_uwwboddischarge.field_uwwboddischarge_value)AS bodout_sum,
					SUM(f_uwwcodincoming.field_uwwcodincoming_value)AS codin_sum,
					SUM(f_uwwcoddischarge.field_uwwcoddischarge_value)AS codout_sum,
					SUM(f_uwwnincoming.field_uwwnincoming_value)AS nin_sum,
					SUM(f_uwwndischarge.field_uwwndischarge_value)AS nout_sum,
					SUM(f_uwwpincoming.field_uwwpincoming_value)AS pin_sum,
					SUM(f_uwwpdischarge.field_uwwpdischarge_value)AS pout_sum,
					SUM(f_uwwwastewatertreated.field_uwwwastewatertreated_value)AS uwwt_sum
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        left join
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        left join
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        left join
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        left join
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        left join
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        left join
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        left join
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        left join
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        left join
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        left join
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        left join
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        left join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        left join
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        left join
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        left join
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        left join
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        left join
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		 left join
            {field_data_field_uwwbodincoming} f_uwwbodincoming
                ON n.nid = f_uwwbodincoming.entity_id
		 left join
            {field_data_field_uwwcodincoming} f_uwwcodincoming
                ON n.nid = f_uwwcodincoming.entity_id
		 left join
            {field_data_field_uwwnincoming} f_uwwnincoming
                ON n.nid = f_uwwnincoming.entity_id
		 left join
            {field_data_field_uwwpincoming} f_uwwpincoming
                ON n.nid = f_uwwpincoming.entity_id
		left join
            {field_data_field_uwwwastewatertreated} f_uwwwastewatertreated
                ON n.nid = f_uwwwastewatertreated.entity_id
    where n.type = :typenode';


	$param = array(
			':typenode' => 'uwwtp',
				);


	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			$row['bodin_sum_format'] = is_null($row['bodin_sum']) ? '0' : uwwtd_format_number($row['bodin_sum'], 2);
			$row['bodout_sum_format'] = is_null($row['bodout_sum']) ? '0' : uwwtd_format_number($row['bodout_sum'], 2);
			$row['codin_sum_format'] = is_null($row['codin_sum']) ? '0' : uwwtd_format_number($row['codin_sum'], 2);
			$row['codout_sum_format'] = is_null($row['codout_sum']) ? '0' : uwwtd_format_number($row['codout_sum'], 2);
			$row['nin_sum_format'] = is_null($row['nin_sum']) ? '0' : uwwtd_format_number($row['nin_sum'], 2);
			$row['nout_sum_format'] = is_null($row['nout_sum']) ? '0' : uwwtd_format_number($row['nout_sum'], 2);
			$row['pin_sum_format'] = is_null($row['pin_sum']) ? '0' : uwwtd_format_number($row['pin_sum'], 2);
			$row['pout_sum_format'] = is_null($row['pout_sum']) ? '0' : uwwtd_format_number($row['pout_sum'], 2);
			$row['uwwt_sum_format'] = is_null($row['uwwt_sum']) ? '0' : uwwtd_format_number($row['uwwt_sum'], 2);
			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	//     dsm($data);
	return $data;
}

function uwwtd_stat_national_uwwtp($GroupBy)
{

	switch ($GroupBy){
		case 'compliance' :
			$group_by = 'f_comp.field_uwwcompliance_value';
			break;
		case 'treatmenttype' :
			$group_by = 'f_uwwtreatmenttype.field_uwwtreatmenttype_value';
			break;
		default:
			$group_by = 'f_comp.field_uwwcompliance_value';
	}


	$data = array();

	$query = 'select ' . $group_by . ',';
	$query .= ' COUNT(n.title) AS  TOTAL_COUNT,
			SUM(f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value) AS LOADENTERING_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        left join
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        left join
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        left join
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        left join
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        left join
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id

        left join
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        left join
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        left join
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        left join
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        left join
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        left join
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        left join
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        left join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        left join
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        left join
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        left join
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        left join
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        left join
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		where n.type = :typenode
		group by ' . $group_by;

	$param = array(
			':typenode' => 'uwwtp',
	);

	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			switch ($group_by){
				case 'f_comp.field_uwwcompliance_value' :
					$row['field_uwwcompliance_value_format'] = is_null($row['field_uwwcompliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_uwwcompliance_value']];
					break;
				case 'f_uwwtreatmenttype.field_uwwtreatmenttype_value' :
					$row['field_uwwtreatmenttype_value_format'] = is_null($row['field_uwwtreatmenttype_value']) ? '' : $GLOBALS['uwwtd']['ui']['treatment_type_in_place'][$row['field_uwwtreatmenttype_value']];
					break;
			}
			$row['total_count_format'] = is_null($row['total_count']) ? '0' : uwwtd_format_number($row['total_count'], 0);
			$row['loadentering_sum_format'] = is_null($row['loadentering_sum']) ? '0' : uwwtd_format_number($row['loadentering_sum'], 0);
			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	//     dsm($data);
	return $data;
}




function uwwtd_stat_national_compliance()
{

	$data = array();
	$param = array();

	$query = 'select f_comp.field_aggcompliance_value,
					 COUNT(n.title) AS  TOTAL_COUNT,
					 SUM(f_aggc2.field_aggc2_value) AS AGGC2_SUM,
                     SUM(f_aggc1.field_aggc1_value) AS AGGC1_SUM,
                     SUM(f_aggpercwithouttreatment.field_aggpercwithouttreatment_value) AS AGGPCWT_SUM,
                     SUM(f_agggenerated.field_agggenerated_value) AS GENERATED_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        left join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        left join
            {field_data_field_aggart3compliance} f_art3
                ON n.nid = f_art3.entity_id
        left join
            {field_data_field_aggart4compliance} f_art4
                ON n.nid = f_art4.entity_id
        left join
            {field_data_field_aggart5compliance} f_art5
                ON n.nid = f_art5.entity_id
        left join
            {field_data_field_aggart6compliance} f_art6
                ON n.nid = f_art6.entity_id
        left join
            {field_data_field_aggcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_regionnuts} f_nuts
                ON n.nid = f_nuts.entity_id
        left join
            {field_data_field_aggc2} f_aggc2
                ON n.nid = f_aggc2.entity_id
        left join
            {field_data_field_aggc1} f_aggc1
                ON n.nid = f_aggc1.entity_id
        left join
            {field_data_field_aggpercwithouttreatment} f_aggpercwithouttreatment
                ON n.nid = f_aggpercwithouttreatment.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		where n.type = :typenode
	group by f_comp.field_aggcompliance_value
    ';

	$param = array(
			':typenode' => 'agglomeration',
	);

	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {
			$row['field_aggcompliance_value_format'] = is_null($row['field_aggcompliance_value']) ? '' : $GLOBALS['uwwtd']['ui']['field']['compliance_label'][$row['field_aggcompliance_value']];
			$row['total_count_format'] = is_null($row['total_count']) ? '0' : uwwtd_format_number($row['total_count'], 0);
			$row['aggc1_sum_format'] = is_null($row['aggc1_sum']) ? '0' : uwwtd_format_number($row['aggc1_sum'], 2);
			$row['aggc2_sum_format'] = is_null($row['aggc2_sum']) ? '0' : uwwtd_format_number($row['aggc2_sum'], 2);
			$row['aggpcwt_sum_format'] = is_null($row['aggpcwt_sum']) ? '0' : uwwtd_format_number($row['aggpcwt_sum'], 2);
			$row['generated_sum_format'] = is_null($row['generated_sum']) ? '0' : uwwtd_format_number($row['generated_sum'], 0);

			$data[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	//     dsm($data);
	return $data;
}

function uwwtd_stat_national_uwwtp_agglo()
{




	$data = array();

	$query = 'select
			SUM(f_uwwloadenteringuwwtp.field_uwwloadenteringuwwtp_value) AS LOADENTERING_SUM,
			SUM(f_physicalcapacityinput.field_physicalcapacityinput_value) AS CAPACITYPHY_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_uwwcode
                ON n.nid = f_uwwcode.entity_id
        left join
            {field_data_field_uwwprimarytreatment} f_primarytreatment
                ON n.nid = f_primarytreatment.entity_id
        left join
            {field_data_field_uwwsecondarytreatment} f_secondarytreatment
                ON n.nid = f_secondarytreatment.entity_id
        left join
            {field_data_field_uwwnremoval} f_nremoval
                ON n.nid = f_nremoval.entity_id
        left join
            {field_data_field_uwwpremoval} f_premoval
                ON n.nid = f_premoval.entity_id
        left join
            {field_data_field_uwwcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_uwwloadenteringuwwtp} f_uwwloadenteringuwwtp
                ON n.nid = f_uwwloadenteringuwwtp.entity_id
        left join
            {field_data_field_regionnuts} f_regionnuts
                ON n.nid = f_regionnuts.entity_id
        left join
            {field_data_field_uwwbod5perf} f_uwwbod5perf
                ON n.nid = f_uwwbod5perf.entity_id
        left join
            {field_data_field_uwwcodperf} f_uwwcodperf
                ON n.nid = f_uwwcodperf.entity_id
        left join
            {field_data_field_uwwntotperf} f_uwwntotperf
                ON n.nid = f_uwwntotperf.entity_id
        left join
            {field_data_field_uwwptotperf} f_uwwptotperf
                ON n.nid = f_uwwptotperf.entity_id
        left join
            {field_data_field_uwwtreatmenttype} f_uwwtreatmenttype
                ON n.nid = f_uwwtreatmenttype.entity_id
        left join
            {field_data_field_physicalcapacityactivity} f_physicalcapacityactivity
                ON n.nid = f_physicalcapacityactivity.entity_id
        left join
            {field_data_field_uwwspecification} f_uwwspecification
                ON n.nid = f_uwwspecification.entity_id
        left join
            {field_data_field_uwwboddischarge} f_uwwboddischarge
                ON n.nid = f_uwwboddischarge.entity_id
        left join
            {field_data_field_uwwcoddischarge} f_uwwcoddischarge
                ON n.nid = f_uwwcoddischarge.entity_id
        left join
            {field_data_field_uwwndischarge} f_uwwndischarge
                ON n.nid = f_uwwndischarge.entity_id
        left join
            {field_data_field_uwwpdischarge} f_uwwpdischarge
                ON n.nid = f_uwwpdischarge.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		left join
            {field_data_field_physicalcapacityinput} f_physicalcapacityinput
                ON n.nid = f_physicalcapacityinput.entity_id
		where n.type = :typenode';

	$param = array(
			':typenode' => 'uwwtp',
	);

	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {

			$data1[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	$query = 'select SUM(f_agggenerated.field_agggenerated_value) AS GENERATED_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        left join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id
        left join
            {field_data_field_aggart3compliance} f_art3
                ON n.nid = f_art3.entity_id
        left join
            {field_data_field_aggart4compliance} f_art4
                ON n.nid = f_art4.entity_id
        left join
            {field_data_field_aggart5compliance} f_art5
                ON n.nid = f_art5.entity_id
        left join
            {field_data_field_aggart6compliance} f_art6
                ON n.nid = f_art6.entity_id
        left join
            {field_data_field_aggcompliance} f_comp
                ON n.nid = f_comp.entity_id
        left join
            {field_data_field_regionnuts} f_nuts
                ON n.nid = f_nuts.entity_id
        left join
            {field_data_field_aggc2} f_aggc2
                ON n.nid = f_aggc2.entity_id
        left join
            {field_data_field_aggc1} f_aggc1
                ON n.nid = f_aggc1.entity_id
        left join
            {field_data_field_aggpercwithouttreatment} f_aggpercwithouttreatment
                ON n.nid = f_aggpercwithouttreatment.entity_id
        left join
            {field_data_field_position_geo} f_geo
                ON n.nid = f_geo.entity_id
		where n.type = :typenode
    ';

	$param = array(
			':typenode' => 'agglomeration',
	);

	try {
		$result = db_query($query,$param);
		while($row = $result->fetchAssoc()) {

			$data2[] = $row;
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}
	$data[0] = array_merge($data1[0],$data2[0]);
	//     dsm($data);
	return $data;
}

function uwwtd_stat_national_generated_by_agglomeration()
{

	$data = array();
	$param = array();
	$conditions = array(
		'>2000<=10000' => ' and (f_agggenerated.field_agggenerated_value > 2000 and f_agggenerated.field_agggenerated_value <= 10000) ',
		'>10000<=100000' => ' and (f_agggenerated.field_agggenerated_value > 10000 and f_agggenerated.field_agggenerated_value <= 100000) ',
		'>100000' => ' and (f_agggenerated.field_agggenerated_value > 100000) ',
	);

	$query = 'select
                     SUM(f_agggenerated.field_agggenerated_value) AS GENERATED_SUM
    from {node} n
        left join
            {field_data_field_anneedata} f_annee
                ON n.nid = f_annee.entity_id
        left join
            {field_data_field_inspireidlocalid} f_aggcode
                ON n.nid = f_aggcode.entity_id
        left join
            {field_data_field_agggenerated} f_agggenerated
                ON n.nid = f_agggenerated.entity_id

		where n.type = :typenode
    ';

	$param = array(
			':typenode' => 'agglomeration',
	);

	try {
			foreach ($conditions as $libelle => $condition){
				$result = db_query($query . $condition,$param);
				while($row = $result->fetchAssoc()) {

					///$data[0][$libelle] = $row['generated_sum'];
					$row['type'] = $libelle;
					$row['generated_sum_format'] = is_null($row['generated_sum']) ? '0' : uwwtd_format_number($row['generated_sum'], 0);

					$data[] = $row;
				}
		}
	}catch(Exception $e) {
		dsm($e->getMessage());
	}

	//     dsm($data);
	return $data;
}

?>