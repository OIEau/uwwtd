 SELECT siteid.field_siteid_value AS "siteId",
    localid.field_inspireidlocalid_value AS "uwwCode",
    repcode.field_repcode_value AS "repCode",
    comp.field_uwwcompliance_value AS cuwwtreated,
    annee.field_anneedata_value AS "repReportedPerdiod"
   FROM drupal_node n
     LEFT JOIN drupal_field_data_field_siteid siteid ON n.nid = siteid.entity_id
     LEFT JOIN drupal_field_data_field_anneedata annee ON n.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON n.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid localid ON n.nid = localid.entity_id
     LEFT JOIN drupal_field_data_field_uwwcompliance comp ON n.nid = comp.entity_id
  WHERE n.type::text = 'uwwtp'::text AND n.status = 1;