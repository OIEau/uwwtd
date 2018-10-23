 SELECT siteid.field_siteid_value AS "siteId",
    localid.field_inspireidlocalid_value AS "aggCode",
    repcode.field_repcode_value AS "repCode",
    n.title AS "aggName",
    status.field_status_value AS "aggState",
    nuts.field_regionnuts_value AS "aggNUTS",
    agglat.field_agglatitude_value AS "aggLatitude",
    agglong.field_agglongitude_value AS "aggLongitude",
    gen.field_agggenerated_value AS "aggGenerated",
    big.field_aggbigid_value AS "bigID",
    calcul.field_aggcalculation_value AS "aggCalculation",
    changes.field_aggchanges_value AS "aggChanges",
    ccoms.field_aggchangescomment_value AS "aggChangesComment",
    aggpo3.field_aggperiodover3_value AS "aggPeriodOver3",
    aggpo4.field_aggperiodover4_value AS "aggPeriodOver4",
    aggpo5.field_aggperiodover5_value AS "aggPeriodOver5",
    aggpo6.field_aggperiodover6_value AS "aggPeriodOver6",
    c1.field_aggc1_value AS "aggC1",
    mc1.field_aggmethodc1_value AS "aggMethodC1",
    c2.field_aggc2_value AS "aggC2",
    mc2.field_aggmethodc2_value AS "aggMethodC2",
    percwt.field_aggpercwithouttreatment_value AS "aggPercWithoutTreatment",
    mpercwt.field_aggmethodwithouttreatment_value AS "aggMethodWithoutTreatment",
    geo.the_geom,
    ptreat.field_aggpercprimtreatment_value AS "aggPercPrimTreatment",
    sectreat.field_aggpercsectreatment_value AS "aggPercSecTreatment",
    stringe.field_aggpercstringenttreatment_value AS "aggPerStringentTreatment",
    sewage.field_aggsewagenetwork_value AS "aggSewageNetwork",
    techni.field_aggbesttechnicalknowledge_value AS "aggBestTechnicalKnowledge",
    overflow.field_aggseweroverflows_m3_value AS "aggSewerOverflows_m3",
    overflowe.field_aggseweroverflows_pe_value AS "aggSewerOverflows_pe",
    remarks.field_aggremarks_value AS "aggRemarks",
    regi.field_agghaveregistrationsystem_value AS "aggHaveRegistrationSystem",
    mainte.field_aggexistmaintenanceplan_value AS "aggExistMaintenancePlan",
    explain.field_aggexplanationother_value AS "aggExplanationOther",
    inhab.field_agginhabitant_value AS "aggInhabitants",
    forecast.field_aggforecast_value AS "aggForecast",
    flownum.field_aggaccoverflownumber_value AS "aggAccOverflowNumber",
    dilution.field_agg_dilution_rates_value AS "aggDilutionRates",
    coverflow.field_aggaccoverflows_value AS "aggAccOverflows",
    omesures.field_aggothermeasures_value AS "aggOtherMeasures",
    pressure.field_aggpressuretest_value AS "aggPressureTest",
    video.field_aggvideoinspections_value AS "aggVideoInspections",
    annee.field_anneedata_value AS "repReportedPerdiod"
   FROM drupal_node n
     LEFT JOIN drupal_field_data_field_anneedata annee ON n.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON n.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid localid ON n.nid = localid.entity_id
     LEFT JOIN drupal_field_data_field_status status ON n.nid = status.entity_id
     LEFT JOIN drupal_field_data_field_regionnuts nuts ON n.nid = nuts.entity_id
     LEFT JOIN drupal_field_data_field_agggenerated gen ON n.nid = gen.entity_id
     LEFT JOIN drupal_field_data_field_aggbigid big ON n.nid = big.entity_id
     LEFT JOIN drupal_field_data_field_aggcalculation calcul ON n.nid = calcul.entity_id
     LEFT JOIN drupal_field_data_field_aggchanges changes ON n.nid = changes.entity_id
     LEFT JOIN drupal_field_data_field_aggchangescomment ccoms ON n.nid = ccoms.entity_id
     LEFT JOIN drupal_field_data_field_aggc1 c1 ON n.nid = c1.entity_id
     LEFT JOIN drupal_field_data_field_aggmethodc1 mc1 ON n.nid = mc1.entity_id
     LEFT JOIN drupal_field_data_field_aggc2 c2 ON n.nid = c2.entity_id
     LEFT JOIN drupal_field_data_field_aggmethodc2 mc2 ON n.nid = mc2.entity_id
     LEFT JOIN drupal_field_data_field_aggpercwithouttreatment percwt ON n.nid = percwt.entity_id
     LEFT JOIN drupal_field_data_field_aggmethodwithouttreatment mpercwt ON n.nid = mpercwt.entity_id
     LEFT JOIN drupal_field_data_field_position_geo geo ON n.nid = geo.entity_id
     LEFT JOIN drupal_field_data_field_aggpercprimtreatment ptreat ON n.nid = ptreat.entity_id
     LEFT JOIN drupal_field_data_field_aggpercsectreatment sectreat ON n.nid = sectreat.entity_id
     LEFT JOIN drupal_field_data_field_aggpercstringenttreatment stringe ON n.nid = stringe.entity_id
     LEFT JOIN drupal_field_data_field_aggsewagenetwork sewage ON n.nid = sewage.entity_id
     LEFT JOIN drupal_field_data_field_aggbesttechnicalknowledge techni ON n.nid = techni.entity_id
     LEFT JOIN drupal_field_data_field_aggseweroverflows_m3 overflow ON n.nid = overflow.entity_id
     LEFT JOIN drupal_field_data_field_aggseweroverflows_pe overflowe ON n.nid = overflowe.entity_id
     LEFT JOIN drupal_field_data_field_aggremarks remarks ON n.nid = remarks.entity_id
     LEFT JOIN drupal_field_data_field_agghaveregistrationsystem regi ON n.nid = regi.entity_id
     LEFT JOIN drupal_field_data_field_aggexistmaintenanceplan mainte ON n.nid = mainte.entity_id
     LEFT JOIN drupal_field_data_field_aggexplanationother explain ON n.nid = explain.entity_id
     LEFT JOIN drupal_field_data_field_aggothermeasures omesures ON n.nid = omesures.entity_id
     LEFT JOIN drupal_field_data_field_aggpressuretest pressure ON n.nid = pressure.entity_id
     LEFT JOIN drupal_field_data_field_aggvideoinspections video ON n.nid = video.entity_id
     LEFT JOIN drupal_field_data_field_siteid siteid ON n.nid = siteid.entity_id
     LEFT JOIN drupal_field_data_field_agglatitude agglat ON n.nid = agglat.entity_id
     LEFT JOIN drupal_field_data_field_agglongitude agglong ON n.nid = agglong.entity_id
     LEFT JOIN drupal_field_data_field_aggperiodover3 aggpo3 ON n.nid = aggpo3.entity_id
     LEFT JOIN drupal_field_data_field_aggperiodover4 aggpo4 ON n.nid = aggpo4.entity_id
     LEFT JOIN drupal_field_data_field_aggperiodover5 aggpo5 ON n.nid = aggpo5.entity_id
     LEFT JOIN drupal_field_data_field_aggperiodover6 aggpo6 ON n.nid = aggpo6.entity_id
     LEFT JOIN drupal_field_data_field_agginhabitant inhab ON n.nid = inhab.entity_id
     LEFT JOIN drupal_field_data_field_aggforecast forecast ON n.nid = forecast.entity_id
     LEFT JOIN drupal_field_data_field_aggaccoverflownumber flownum ON n.nid = flownum.entity_id
     LEFT JOIN drupal_field_data_field_agg_dilution_rates dilution ON n.nid = dilution.entity_id
     LEFT JOIN drupal_field_data_field_aggaccoverflows coverflow ON n.nid = coverflow.entity_id
     LEFT JOIN drupal_field_data_field_anneedata year ON n.nid = year.entity_id
  WHERE n.type::text = 'agglomeration'::text AND n.status = 1;