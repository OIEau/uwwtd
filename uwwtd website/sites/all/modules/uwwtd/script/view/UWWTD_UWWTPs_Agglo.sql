 SELECT repcode.field_repcode_value AS "repCode",
    uwwsiteid.field_siteid_value AS "uwwSiteId",
    uwwid.field_inspireidlocalid_value AS "uwwCode",
    uww.title AS "uwwName",
    aggsiteid.field_siteid_value AS "aggSiteId",
    aggid.field_inspireidlocalid_value AS "aggCode",
    agg.title AS "aggName",
    annee.field_anneedata_value AS "repReportedPerdiod"
   FROM drupal_field_data_field_uwwaggliste list
     JOIN drupal_node agg ON agg.nid = list.field_uwwaggliste_nid AND agg.type = 'agglomeration'
     JOIN drupal_node uww ON uww.nid = list.entity_id AND uww.type = 'uwwtp'
     LEFT JOIN drupal_field_data_field_anneedata annee ON uww.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON uww.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid uwwid ON uww.nid = uwwid.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid aggid ON agg.nid = aggid.entity_id
     LEFT JOIN drupal_field_data_field_siteid uwwsiteid ON uww.nid = uwwsiteid.entity_id
     LEFT JOIN drupal_field_data_field_siteid aggsiteid ON agg.nid = aggsiteid.entity_id;