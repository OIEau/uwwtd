SELECT siteid.field_siteid_value AS "siteId",
    localid.field_inspireidlocalid_value AS "dcpCode",
    repcode.field_repcode_value AS "repCode",
    n.title AS "dcpName",
    status.field_status_value AS "dcpState",
    station.field_utilityfacilityreference_value AS "uwwID",
    region.field_regionnuts_value AS "dcpNuts",
    type.field_dcpwaterbodytype_value AS "dcpWaterBodyType",
    typename.field_dcpwaterbodyname_value AS "dcpWaterbodyName",
    irrig.field_dcpirrigation_value AS "dcpIrrigation",
    waterb.field_dcpwaterbodyid_value AS "dcpWaterbodyID",
    wfd.field_dcpwfdrbd_value AS "dcpWFDRBD",
    sensi.field_rcaid_value AS "rcaID",
    donot.field_dcpnotaffect_value AS "dcpNotAffect",
    provide.field_dcpmsprovide_value AS "dcpMSProvide",
    accept.field_dcpcomaccept_value AS "dcpCOMAccept",
    remarks.field_dcpremarks_value AS "dcpRemarks",
    rcatype.field_rcatype_value AS "dcpTypeOfReceivingArea",
    wbrefdate.field_dcpwaterbodyreferencedate_value AS "dcpWaterBodyReferenceDate",
    dcplat.field_dcplatitude_value AS "dcpLatitude",
    dcplong.field_dcplongitude_value AS "dcpLongitude",
    geo.the_geom,
    groundwater.field_dcpgroundwater_value AS "dcpGroundWater",
    groundwaterdate.field_dcpdcpgroundwaterreference_value AS "dcpGroundWaterReferenceDate",
    recwater.field_dcpreceivingwater_value AS "dcpReceivingWater",
    recdate.field_dcpreceivingwaterreference_value AS "dcpReceivingWaterReferenceDate",
    surf.field_dcpsurfacewaters_value AS "dcpSurfaceWaters",
    wfdrefdate.field_dcpwfdrbdreferencedate_value AS "dcpWFDRBDReferenceDate",
    wfdsub.field_dcpwfdsubunit_value AS "dcpWFDSubUnit",
    wfdsubdate.field_dcp_wfdsubunitrefdate_value AS "dcpWFDSubUnitReferenceDate",
    annee.field_anneedata_value AS "repReportedPerdiod"
   FROM drupal_node n
     LEFT JOIN drupal_field_data_field_siteid siteid ON n.nid = siteid.entity_id
     LEFT JOIN drupal_field_data_field_anneedata annee ON n.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON n.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid localid ON n.nid = localid.entity_id
     LEFT JOIN drupal_field_data_field_utilityfacilityreference station ON n.nid = station.entity_id
     LEFT JOIN drupal_field_data_field_regionnuts region ON n.nid = region.entity_id
     LEFT JOIN drupal_field_data_field_dcpwaterbodytype type ON n.nid = type.entity_id
     LEFT JOIN drupal_field_data_field_dcpwaterbodyname typename ON n.nid = typename.entity_id
     LEFT JOIN drupal_field_data_field_dcpirrigation irrig ON n.nid = irrig.entity_id
     LEFT JOIN drupal_field_data_field_dcpwaterbodyid waterb ON n.nid = waterb.entity_id
     LEFT JOIN drupal_field_data_field_dcpwfdrbd wfd ON n.nid = wfd.entity_id
     LEFT JOIN drupal_field_data_field_rcaid sensi ON n.nid = sensi.entity_id
     LEFT JOIN drupal_field_data_field_dcpnotaffect donot ON n.nid = donot.entity_id
     LEFT JOIN drupal_field_data_field_dcpmsprovide provide ON n.nid = donot.entity_id
     LEFT JOIN drupal_field_data_field_dcpcomaccept accept ON n.nid = accept.entity_id
     LEFT JOIN drupal_field_data_field_dcpremarks remarks ON n.nid = remarks.entity_id
     LEFT JOIN drupal_field_data_field_rcatype rcatype ON n.nid = rcatype.entity_id
     LEFT JOIN drupal_field_data_field_position_geo geo ON n.nid = geo.entity_id
     LEFT JOIN drupal_field_data_field_status status ON n.nid = status.entity_id
     LEFT JOIN drupal_field_data_field_dcpreceivingwater recwater ON n.nid = recwater.entity_id
     LEFT JOIN drupal_field_data_field_dcpsurfacewaters surf ON n.nid = surf.entity_id
     LEFT JOIN drupal_field_data_field_dcplatitude dcplat ON n.nid = dcplat.entity_id
     LEFT JOIN drupal_field_data_field_dcplongitude dcplong ON n.nid = dcplong.entity_id
     LEFT JOIN drupal_field_data_field_dcpwaterbodyreferencedate wbrefdate ON n.nid = wbrefdate.entity_id
     LEFT JOIN drupal_field_data_field_dcpgroundwater groundwater ON n.nid = groundwater.entity_id
     LEFT JOIN drupal_field_data_field_dcpdcpgroundwaterreference groundwaterdate ON n.nid = groundwaterdate.entity_id
     LEFT JOIN drupal_field_data_field_dcpreceivingwaterreference recdate ON n.nid = recdate.entity_id
     LEFT JOIN drupal_field_data_field_dcpwfdrbdreferencedate wfdrefdate ON n.nid = wfdrefdate.entity_id
     LEFT JOIN drupal_field_data_field_dcpwfdsubunit wfdsub ON n.nid = wfdsub.entity_id
     LEFT JOIN drupal_field_data_field_dcp_wfdsubunitrefdate wfdsubdate ON n.nid = wfdsubdate.entity_id
     LEFT JOIN drupal_field_data_field_anneedata year ON n.nid = year.entity_id
  WHERE n.type::text = 'discharge_point'::text AND n.status = 1;