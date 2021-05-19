 SELECT siteid.field_siteid_value AS "siteId",
    localid.field_inspireidlocalid_value AS "uwwCode",
    repcode.field_repcode_value AS "repCode",
    comp.field_uwwcompliance_value AS cuwwtreated,
        CASE
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'P'::text THEN 'Primary'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'S'::text THEN 'Secondary'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'MS'::text THEN 'More Stringent: Other'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'O'::text THEN 'More Stringent'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'MP'::text THEN 'More Stringent: Phosphorus'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'PO'::text THEN 'More Stringent: Phosphorus and Other'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'N'::text THEN 'More Stringent: Nitrogen'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'NO'::text THEN 'More Stringent: Nitrogen and Other'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'NP'::text THEN 'More Stringent: Nitrogen and Phosphorus'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'NPO'::text THEN 'More Stringent: Nitrogen, Phosphorus and Other'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'NR'::text THEN 'Not relevant'::text
            WHEN ttype_r.field_uwwtreatmentrequired_value::text = 'Appropriate'::text THEN 'Appropriate'::text
            ELSE NULL::text
        END AS "uwwTreatmentTypeRequired",
    treat.field_uwwtreatment_met_value as "uwwTreatmentMet",
    perf.field_uwwperformance_met_value as "uwwPerformanceMet",
    
    annee.field_anneedata_value AS "repReportedPerdiod"
   FROM drupal_node n
     LEFT JOIN drupal_field_data_field_siteid siteid ON n.nid = siteid.entity_id
     LEFT JOIN drupal_field_data_field_anneedata annee ON n.nid = annee.entity_id
     LEFT JOIN drupal_field_data_field_repcode repcode ON n.nid = repcode.entity_id
     LEFT JOIN drupal_field_data_field_inspireidlocalid localid ON n.nid = localid.entity_id
     LEFT JOIN drupal_field_data_field_uwwcompliance comp ON n.nid = comp.entity_id
     LEFT JOIN drupal_field_data_field_uwwtreatmentrequired ttype_r ON n.nid = ttype_r.entity_id
     LEFT JOIN drupal_field_data_field_uwwperformance_met  perf ON n.nid = perf.entity_id
     LEFT JOIN drupal_field_data_field_uwwtreatment_met treat ON n.nid = treat.entity_id
  WHERE n.type::text = 'uwwtp'::text AND n.status = 1;