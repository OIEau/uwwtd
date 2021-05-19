 SELECT siteid.field_siteid_value AS "siteId",
    localid.field_inspireidlocalid_value AS "aggCode",
    repcode.field_repcode_value AS "repCode",
    comp.field_aggcompliance_value AS caggtreated,
    art3.field_aggart3compliance_value AS cagg3,
    art3add.field_aggart3addcompliance_value AS cagg3,
    art4.field_aggart4compliance_value AS cagg4,
    art5.field_aggart5compliance_value AS cagg5,
    art6.field_aggart6compliance_value AS cagg6,
    annee.field_anneedata_value AS "repReportedPerdiod"
   FROM drupal_node n
     LEFT JOIN drupal_field_data_field_siteid siteid ON n.nid = siteid.entity_id
     LEFT JOIN drupal_field_data_field_anneedata annee ON n.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON n.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid localid ON n.nid = localid.entity_id
     LEFT JOIN drupal_field_data_field_aggcompliance comp ON n.nid = comp.entity_id
     LEFT JOIN drupal_field_data_field_aggart3compliance art3 ON n.nid = art3.entity_id
     LEFT JOIN drupal_field_data_field_aggart3addcompliance art3add ON n.nid = art3add.entity_id
     LEFT JOIN drupal_field_data_field_aggart4compliance art4 ON n.nid = art4.entity_id
     LEFT JOIN drupal_field_data_field_aggart5compliance art5 ON n.nid = art5.entity_id
     LEFT JOIN drupal_field_data_field_aggart6compliance art6 ON n.nid = art6.entity_id

  WHERE n.type::text = 'agglomeration'::text AND n.status = 1;