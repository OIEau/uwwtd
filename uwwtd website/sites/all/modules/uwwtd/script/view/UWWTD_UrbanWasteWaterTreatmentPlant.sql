SELECT siteid.field_siteid_value AS "siteId",
    localid.field_inspireidlocalid_value AS "uwwCode",
    repcode.field_repcode_value AS "repCode",
    n.title AS "uwwName",
    status.field_status_value AS "uwwState",
    ''::text AS "uwwE-PRTRCode",
    system.field_uwwcollectingsystem_value AS "uwwCollectingSystem",
    nuts.field_regionnuts_value AS "uwwNUTS",
    loadent.field_uwwloadenteringuwwtp_value AS "uwwLoadEnteringUWWTP",
    capacity.field_physicalcapacityactivity_value AS "uwwCapacity",
    prim.field_uwwprimarytreatment_value AS "uwwPrimaryTreatment",
    second.field_uwwsecondarytreatment_value AS "uwwSecondaryTreatment",
    nrem.field_uwwnremoval_value AS "uwwNRemoval",
    prem.field_uwwpremoval_value AS "uwwPRemoval",
    uv.field_uwwuv_value AS "uwwUV",
    chlor.field_uwwchlorination_value AS "uwwChlorination",
    ozon.field_uwwozonation_value AS "uwwOzonation",
    sand.field_uwwsandfiltration_value AS "uwwSandFiltration",
    micro.field_uwwmicrofiltration_value AS "uwwMicroFiltration",
    specif.field_uwwspecification_value AS "uwwSpecification",
    bod5.field_uwwbod5perf_value AS "uwwBOD5Perf",
    cod.field_uwwcodperf_value AS "uwwCODPerf",
    tss.field_uwwtssperf_value AS "uwwTSSPerf",
    ntot.field_uwwntotperf_value AS "uwwNTotPerf",
    ptot.field_uwwptotperf_value AS "uwwPTotPerf",
    operf.field_uwwotherperf_value AS "uwwOtherPerf",
    baddesign.field_uwwbadperfdesign_value AS "uwwBadPerfDesign",
    uwwlat.field_uwwlatitude_value AS "uwwLatitude",
    uwwlong.field_uwwlongitude_value AS "uwwLongitude",
    geo.the_geom,
    bodinc.field_uwwbodincoming_value AS "uwwBODIncomingMeasured",
    codinc.field_uwwcodincoming_value AS "uwwCODIncomingMeasured",
    ninc.field_uwwnincoming_value AS "uwwNIncomingMeasured",
    pinc.field_uwwpincoming_value AS "uwwPIncomingMeasured",
    boddis.field_uwwboddischarge_value AS "uwwBODDischargeMeasured",
    coddis.field_uwwcoddischarge_value AS "uwwCODDischargeMeasured",
    ndis.field_uwwndischarge_value AS "uwwNDischargeMeasured",
    pdis.field_uwwpdischarge_value AS "uwwPDischargeMeasured",
    wastet.field_uwwwastewatertreated_value AS "uwwWasteWaterTreated",
    mwastet.field_uwwmethodwastewatertreated_value AS "uwwMethodWasteWaterTreated",
    remarks.field_uwwremarks_value AS "uwwRemarks",
    dateclose.field_validto_value AS "uwwDateClosing",
    acci.field_uwwaccidents_value AS "uwwAccidents",
    bad.field_uwwbaddesign_value AS "uwwBadPerformance",
    boddiscal.field_uwwboddischargecalculated_value AS "uwwBODDischargeCalculated",
    boddisest.field_uwwboddischargeestimated_value AS "uwwBODDischargeEstimated",
    bodincal.field_uwwbodincomingcalculated_value AS "uwwBODIncomingCalculated",
    bodinest.field_uwwbodincomingestimated_value AS "uwwBODIncomingEstimated",
    coddiscal.field_uwwcoddischargecalculated_value AS "uwwCODDischargeCalculated",
    coddisest.field_uwwcoddischargeestimated_value AS "uwwCODDischargeEstimated",
    codinccal.field_uwwcodincomingcalculated_value AS "uwwCODIncomingCalculated",
    codincest.field_uwwcodincomingestimated_value AS "uwwCODIncomingEstimated",
    hist.field_uwwhistorie_value AS "uwwHistorie",
    info.field_uwwinformation_value AS "uwwInformation",
    ndiscal.field_uwwndischargecalculated_value AS "uwwNDischargeCalculated",
    ndisest.field_uwwndischargeestimated_value AS "uwwNDischargeEstimated",
    ninccal.field_uwwnincomingcalculated_value AS "uwwNIncomingCalculated",
    nincest.field_uwwnincomingestimated_value AS "uwwNIncomingEstimated",
    pdiscal.field_uwwpdischargecalculated_value AS "uwwPDischargeCalculated",
    pdisest.field_uwwpdischargeestimated_value AS "uwwPDischargeEstimated",
    pinccal.field_uwwpincomingcalculated_value AS "uwwPIncomingCalculated",
    pincest.field_uwwpincomingestimated_value AS "uwwPIncomingEstimated",
    other.field_uwwotherperf_value AS "uwwOther",
        CASE
            WHEN ttype.field_uwwtreatmenttype_value::text = 'P'::text THEN 'Primary'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'S'::text THEN 'Secondary'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'MS'::text THEN 'More Stringent: Other'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'O'::text THEN 'More Stringent'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'MP'::text THEN 'More Stringent: Phosphorus'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'PO'::text THEN 'More Stringent: Phosphorus and Other'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'N'::text THEN 'More Stringent: Nitrogen'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'NO'::text THEN 'More Stringent: Nitrogen and Other'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'NP'::text THEN 'More Stringent: Nitrogen and Phosphorus'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'NPO'::text THEN 'More Stringent: Nitrogen, Phosphorus and Other'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'NR'::text THEN 'Not relevant'::text
            WHEN ttype.field_uwwtreatmenttype_value::text = 'Appropriate'::text THEN 'Appropriate'::text
            ELSE NULL::text
        END AS "uwwTreatmentType",
    annee.field_anneedata_value AS "repReportedPerdiod",
    bl.field_uwwbeginlife_value AS "uwwBeginLife",
    el.field_uwwendlife_value AS "uwwEndLife"
   FROM drupal_node n
     LEFT JOIN drupal_field_data_field_anneedata annee ON n.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON n.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid localid ON n.nid = localid.entity_id
     LEFT JOIN drupal_field_data_field_status status ON n.nid = status.entity_id
     LEFT JOIN drupal_field_data_field_uwwcollectingsystem system ON n.nid = system.entity_id
     LEFT JOIN drupal_field_data_field_regionnuts nuts ON n.nid = nuts.entity_id
     LEFT JOIN drupal_field_data_field_uwwloadenteringuwwtp loadent ON n.nid = loadent.entity_id
     LEFT JOIN drupal_field_data_field_physicalcapacityactivity capacity ON n.nid = capacity.entity_id
     LEFT JOIN drupal_field_data_field_uwwprimarytreatment prim ON n.nid = prim.entity_id
     LEFT JOIN drupal_field_data_field_uwwsecondarytreatment second ON n.nid = second.entity_id
     LEFT JOIN drupal_field_data_field_uwwnremoval nrem ON n.nid = nrem.entity_id
     LEFT JOIN drupal_field_data_field_uwwpremoval prem ON n.nid = prem.entity_id
     LEFT JOIN drupal_field_data_field_uwwuv uv ON n.nid = uv.entity_id
     LEFT JOIN drupal_field_data_field_uwwchlorination chlor ON n.nid = chlor.entity_id
     LEFT JOIN drupal_field_data_field_uwwozonation ozon ON n.nid = ozon.entity_id
     LEFT JOIN drupal_field_data_field_uwwsandfiltration sand ON n.nid = sand.entity_id
     LEFT JOIN drupal_field_data_field_uwwmicrofiltration micro ON n.nid = micro.entity_id
     LEFT JOIN drupal_field_data_field_uwwspecification specif ON n.nid = specif.entity_id
     LEFT JOIN drupal_field_data_field_uwwbod5perf bod5 ON n.nid = bod5.entity_id
     LEFT JOIN drupal_field_data_field_uwwcodperf cod ON n.nid = cod.entity_id
     LEFT JOIN drupal_field_data_field_uwwtssperf tss ON n.nid = tss.entity_id
     LEFT JOIN drupal_field_data_field_uwwntotperf ntot ON n.nid = ntot.entity_id
     LEFT JOIN drupal_field_data_field_uwwptotperf ptot ON n.nid = ptot.entity_id
     LEFT JOIN drupal_field_data_field_uwwotherperf operf ON n.nid = operf.entity_id
     LEFT JOIN drupal_field_data_field_position_geo geo ON n.nid = geo.entity_id
     LEFT JOIN drupal_field_data_field_uwwbadperfdesign baddesign ON n.nid = baddesign.entity_id
     LEFT JOIN drupal_field_data_field_uwwbodincoming bodinc ON n.nid = bodinc.entity_id
     LEFT JOIN drupal_field_data_field_uwwcodincoming codinc ON n.nid = codinc.entity_id
     LEFT JOIN drupal_field_data_field_uwwnincoming ninc ON n.nid = ninc.entity_id
     LEFT JOIN drupal_field_data_field_uwwpincoming pinc ON n.nid = pinc.entity_id
     LEFT JOIN drupal_field_data_field_uwwboddischarge boddis ON n.nid = boddis.entity_id
     LEFT JOIN drupal_field_data_field_uwwcoddischarge coddis ON n.nid = coddis.entity_id
     LEFT JOIN drupal_field_data_field_uwwndischarge ndis ON n.nid = ndis.entity_id
     LEFT JOIN drupal_field_data_field_uwwpdischarge pdis ON n.nid = pdis.entity_id
     LEFT JOIN drupal_field_data_field_uwwwastewatertreated wastet ON n.nid = wastet.entity_id
     LEFT JOIN drupal_field_data_field_uwwmethodwastewatertreated mwastet ON n.nid = mwastet.entity_id
     LEFT JOIN drupal_field_data_field_uwwremarks remarks ON n.nid = remarks.entity_id
     LEFT JOIN drupal_field_data_field_uwwotherperf other ON n.nid = other.entity_id
     LEFT JOIN drupal_field_data_field_uwwtreatmenttype ttype ON n.nid = ttype.entity_id
     LEFT JOIN drupal_field_data_field_siteid siteid ON n.nid = siteid.entity_id
     LEFT JOIN drupal_field_data_field_uwwlatitude uwwlat ON n.nid = uwwlat.entity_id
     LEFT JOIN drupal_field_data_field_uwwlongitude uwwlong ON n.nid = uwwlong.entity_id
     LEFT JOIN drupal_field_data_field_validto dateclose ON n.nid = dateclose.entity_id
     LEFT JOIN drupal_field_data_field_uwwaccidents acci ON n.nid = acci.entity_id
     LEFT JOIN drupal_field_data_field_uwwboddischargecalculated boddiscal ON n.nid = boddiscal.entity_id
     LEFT JOIN drupal_field_data_field_uwwboddischargeestimated boddisest ON n.nid = boddisest.entity_id
     LEFT JOIN drupal_field_data_field_uwwbodincomingcalculated bodincal ON n.nid = bodincal.entity_id
     LEFT JOIN drupal_field_data_field_uwwbodincomingestimated bodinest ON n.nid = bodinest.entity_id
     LEFT JOIN drupal_field_data_field_uwwcoddischargecalculated coddiscal ON n.nid = coddiscal.entity_id
     LEFT JOIN drupal_field_data_field_uwwcoddischargeestimated coddisest ON n.nid = coddisest.entity_id
     LEFT JOIN drupal_field_data_field_uwwcodincomingcalculated codinccal ON n.nid = codinccal.entity_id
     LEFT JOIN drupal_field_data_field_uwwcodincomingestimated codincest ON n.nid = codincest.entity_id
     LEFT JOIN drupal_field_data_field_uwwhistorie hist ON n.nid = hist.entity_id
     LEFT JOIN drupal_field_data_field_uwwinformation info ON n.nid = info.entity_id
     LEFT JOIN drupal_field_data_field_uwwndischargecalculated ndiscal ON n.nid = ndiscal.entity_id
     LEFT JOIN drupal_field_data_field_uwwndischargeestimated ndisest ON n.nid = ndisest.entity_id
     LEFT JOIN drupal_field_data_field_uwwnincomingcalculated ninccal ON n.nid = ninccal.entity_id
     LEFT JOIN drupal_field_data_field_uwwnincomingestimated nincest ON n.nid = nincest.entity_id
     LEFT JOIN drupal_field_data_field_uwwpdischargecalculated pdiscal ON n.nid = pdiscal.entity_id
     LEFT JOIN drupal_field_data_field_uwwpdischargeestimated pdisest ON n.nid = pdisest.entity_id
     LEFT JOIN drupal_field_data_field_uwwpincomingcalculated pinccal ON n.nid = pinccal.entity_id
     LEFT JOIN drupal_field_data_field_uwwpincomingestimated pincest ON n.nid = pincest.entity_id
     LEFT JOIN drupal_field_data_field_uwwbaddesign bad ON n.nid = bad.entity_id
     
     LEFT JOIN drupal_field_data_field_anneedata year ON n.nid = year.entity_id
     LEFT JOIN drupal_field_data_field_uwwbeginlife bl ON n.nid = bl.entity_id
     LEFT JOIN drupal_field_data_field_uwwendlife el ON n.nid = el.entity_id
  WHERE n.type::text = 'uwwtp'::text AND n.status = 1;