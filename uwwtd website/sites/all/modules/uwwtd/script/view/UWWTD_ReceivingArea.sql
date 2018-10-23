WITH sum_load AS (
         SELECT rca.field_dcprcaliste_nid AS entity_id,
            sum(cap.field_uwwloadenteringuwwtp_value) AS sum_load
           FROM drupal_field_data_field_dcpuwwliste uww
             LEFT JOIN drupal_field_data_field_dcprcaliste rca ON uww.entity_id = rca.entity_id
             LEFT JOIN drupal_field_data_field_uwwloadenteringuwwtp cap ON uww.field_dcpuwwliste_nid = cap.entity_id
          WHERE rca.field_dcprcaliste_nid IS NOT NULL
          GROUP BY rca.field_dcprcaliste_nid
        ), count_station AS (
         SELECT rca.field_dcprcaliste_nid AS entity_id,
            count(uww.field_dcpuwwliste_nid) AS count_station
           FROM drupal_field_data_field_dcpuwwliste uww
             LEFT JOIN drupal_field_data_field_dcprcaliste rca ON uww.entity_id = rca.entity_id
          WHERE rca.field_dcprcaliste_nid IS NOT NULL
          GROUP BY rca.field_dcprcaliste_nid
        )
 SELECT siteid.field_siteid_value AS "siteId",
    localid.field_inspireidlocalid_value AS "rcaCode",
    repcode.field_repcode_value AS "repCode",
    n.title AS "rcaName",
    type.field_specialisedzonetype_value AS "rcaType",
    status.field_status_value AS "rcaState",
    reldir.field_rcacrelevantdirective_value AS "rcaCRelevantDirective",
    othreldir.field_rcacidotherdirective_value AS "rcaCIDOtherDirective",
    dateothreldir.field_rcacdateotherdirective_value AS "rcaCIDOtherDirectiveDate",
    COALESCE(count_station.count_station, 0::bigint) AS "rcaPlants",
    sum_load.sum_load AS "rcaPlantsCapacity",
    nincomm.field_rcanincomingmeasured_value AS "rcaNIncomingMeasured",
    nincomc.field_rcanincomingcalculated_value AS "rcaNIncomingCalculated",
    nincome.field_rcanincomingestimated_value AS "rcaNIncomingEstimated",
    pincomm.field_rcapincomingmeasured_value AS "rcaPIncomingMeasured",
    pincomc.field_rcapincomingcalculated_value AS "rcaPIncomingCalculated",
    pincome.field_rcapincomingestimated_value AS "rcaPIncomingEstimated",
    ndism.field_rcandischargedmeasured_value AS "rcaNDischargedMeasured",
    ndisc.field_rcandischargedcalculated_value AS "rcaNDischargedCalculated",
    ndise.field_rcandischargedestimated_value AS "rcaNDischargedEstimated",
    pdism.field_rcapdischargedmeasured_value AS "rcaPDischargedMeasured",
    pdisc.field_rcapdischargedcalculated_value AS "rcaPDischargedCalculated",
    pdise.field_rcapdischargedestimated_value AS "rcaPDischargedEstimated",
    remark.field_rcaremarks_value AS "rcaRemarks",
        CASE
            WHEN type.field_specialisedzonetype_value::text = 'SA'::text THEN 'SensitiveArea'::text
            WHEN type.field_specialisedzonetype_value::text = 'CSA'::text THEN 'SensitiveArea'::text
            WHEN type.field_specialisedzonetype_value::text = 'NA'::text THEN 'NormalArea'::text
            WHEN type.field_specialisedzonetype_value::text = 'LSA'::text THEN 'LessSensitiveArea'::text
            ELSE NULL::text
        END AS "rcaSensitiveArea",
    datedes.field_rcabdatedesignation_value AS "rcaDateDesignation",
    date5854.field_rcadateart5854_value AS "rcaDateArt54",
    dateex.field_rcadateexported_value AS "rcaDateexported",
    nitro.field_rcaanitro_value AS "rcaANitro",
    phos.field_rcaaphos_value AS "rcaAPhos",
    rcab.field_rcab_value AS "rcaB",
    rcac.field_rcac_value AS "rcaC",
    morph.field_rcamorphology_value AS "rcaMorphology",
    hydro.field_rcahydrologie_value AS "rcaHydrologie",
    hydra.field_rcahydraulic_value AS "rcaHydraulic",
    absrisk.field_rcaabsencerisk_value AS "rcaAbsenceRisk",
    date5854.field_rcadateart5854_value AS "rcaDateArt58",
    app54.field_rca54applied_value AS "rcaArt54Applied",
    paramn.field_rca_parameter_n_value AS "rcaParameterN",
    paramp.field_rca_parameter_p_value AS "rcaParameterOther",
    paramother.field_rca_parameter_other_value AS "rcaParameterP",
    annee.field_anneedata_value AS "repReportedPerdiod",
    geo.the_geom
   FROM drupal_node n
     LEFT JOIN sum_load ON n.nid = sum_load.entity_id
     LEFT JOIN count_station ON n.nid = count_station.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON n.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid localid ON n.nid = localid.entity_id
     LEFT JOIN drupal_field_data_field_specialisedzonetype type ON n.nid = type.entity_id
     LEFT JOIN drupal_field_data_field_status status ON n.nid = status.entity_id
     LEFT JOIN drupal_field_data_field_position_geo geo ON n.nid = geo.entity_id
     LEFT JOIN drupal_field_data_field_rcanincomingcalculated nincomc ON n.nid = nincomc.entity_id
     LEFT JOIN drupal_field_data_field_rcanincomingestimated nincome ON n.nid = nincome.entity_id
     LEFT JOIN drupal_field_data_field_rcanincomingmeasured nincomm ON n.nid = nincomm.entity_id
     LEFT JOIN drupal_field_data_field_rcapincomingcalculated pincomc ON n.nid = pincomc.entity_id
     LEFT JOIN drupal_field_data_field_rcapincomingestimated pincome ON n.nid = pincome.entity_id
     LEFT JOIN drupal_field_data_field_rcapincomingmeasured pincomm ON n.nid = pincomm.entity_id
     LEFT JOIN drupal_field_data_field_rcandischargedcalculated ndisc ON n.nid = ndisc.entity_id
     LEFT JOIN drupal_field_data_field_rcandischargedestimated ndise ON n.nid = ndise.entity_id
     LEFT JOIN drupal_field_data_field_rcandischargedmeasured ndism ON n.nid = ndism.entity_id
     LEFT JOIN drupal_field_data_field_rcapdischargedcalculated pdisc ON n.nid = pdisc.entity_id
     LEFT JOIN drupal_field_data_field_rcapdischargedestimated pdise ON n.nid = pdise.entity_id
     LEFT JOIN drupal_field_data_field_rcapdischargedmeasured pdism ON n.nid = pdism.entity_id
     LEFT JOIN drupal_field_data_field_rcaremarks remark ON n.nid = remark.entity_id
     LEFT JOIN drupal_field_data_field_rcabdatedesignation datedes ON n.nid = datedes.entity_id
     LEFT JOIN drupal_field_data_field_rcadateart5854 date5854 ON n.nid = date5854.entity_id
     LEFT JOIN drupal_field_data_field_rcadateexported dateex ON n.nid = dateex.entity_id
     LEFT JOIN drupal_field_data_field_rcaanitro nitro ON n.nid = nitro.entity_id
     LEFT JOIN drupal_field_data_field_rcaaphos phos ON n.nid = phos.entity_id
     LEFT JOIN drupal_field_data_field_rcab rcab ON n.nid = rcab.entity_id
     LEFT JOIN drupal_field_data_field_rcac rcac ON n.nid = rcac.entity_id
     LEFT JOIN drupal_field_data_field_rcamorphology morph ON n.nid = morph.entity_id
     LEFT JOIN drupal_field_data_field_rcahydrologie hydro ON n.nid = hydro.entity_id
     LEFT JOIN drupal_field_data_field_rcahydraulic hydra ON n.nid = hydra.entity_id
     LEFT JOIN drupal_field_data_field_rcaabsencerisk absrisk ON n.nid = absrisk.entity_id
     LEFT JOIN drupal_field_data_field_rca54applied app54 ON n.nid = app54.entity_id
     LEFT JOIN drupal_field_data_field_anneedata annee ON n.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_siteid siteid ON n.nid = siteid.entity_id
     LEFT JOIN drupal_field_data_field_rcacrelevantdirective reldir ON n.nid = reldir.entity_id
     LEFT JOIN drupal_field_data_field_rcacidotherdirective othreldir ON n.nid = othreldir.entity_id
     LEFT JOIN drupal_field_data_field_rcacdateotherdirective dateothreldir ON n.nid = dateothreldir.entity_id
     LEFT JOIN drupal_field_data_field_rca_parameter_n paramn ON n.nid = paramn.entity_id
     LEFT JOIN drupal_field_data_field_rca_parameter_p paramp ON n.nid = paramp.entity_id
     LEFT JOIN drupal_field_data_field_rca_parameter_other paramother ON n.nid = paramother.entity_id
     LEFT JOIN drupal_field_data_field_anneedata year ON n.nid = year.entity_id
  WHERE n.type::text = 'receiving_area'::text AND n.status = 1;