<?php
/**************************************************************
 ********************** Agglomerations ************************
 **************************************************************/

/*
 * Get compliance for agglomerations
*/
function uwwtd_statistics_agglomeration_compliance($year = 'all', $groupby = '') {
  $stats = array();
  $query = "SELECT ";

  if($groupby == 'basin') {
    $query .= " rb.rbdname as name,";
  } elseif($groupby == 'regions') {
    $query .= " rn.n3nm as name,";
  }

  $query .= " COUNT(fdfa.field_aggcompliance_value), fdfa.field_aggcompliance_value as label
  FROM {node} n
  JOIN {field_data_field_aggcompliance} fdfa ON fdfa.entity_id = n.nid
  JOIN {field_data_field_position_geo} fdfpg ON fdfpg.entity_id = n.nid";

  if($groupby == 'basin') {
    $query .= " JOIN river_basin rb ON ST_Intersects(rb.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  } elseif($groupby == 'regions') {
    $query .= " JOIN regions_nuts rn ON ST_Intersects(rn.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  }

  if($year != 'all') {
    $query .= " JOIN {field_data_field_anneedata} fdfan ON fdfan.entity_id = n.nid";
  }

  $query .= " WHERE n.type = 'agglomeration'";

  if($year != 'all') {
    $query .= " AND fdfan.field_anneedata_value = :year";
  }

  $query .= " GROUP BY name, fdfa.field_aggcompliance_value";
  $query .= " ORDER BY name";

  if($year != 'all') {
    $results = db_query($query, array(':year' => $year));
  } else {
    $results = db_query($query);
  }

  $resultats = $results->fetchAll();
  foreach ($resultats as $resultat) {
    //override PD and QC value, we don't want to display QC and PD (for now) 
    if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $resultat->label ])) {
        $resultat->label = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $resultat->label ];    
    }      
    $stats[$resultat->name][$resultat->label] += $resultat->count;
  }

  return $stats;
}

/*
 * Get treatment data for agglomerations
*/
function uwwtd_statistics_agglomeration_treatment($year = 'all', $groupby = '') {
  $stats = array();
  $query = "SELECT ";

  if($groupby == 'basin') {
    $query .= " rb.rbdname as name,";
  } elseif($groupby == 'regions') {
    $query .= " rn.n3nm as name,";
  }

  $query .= " ROUND(AVG(fdfa1.field_aggc1_value),2) as count1, 'Rate of generated load of agglomeration collected through collecting system (% of p.e.)' as label1, 
              ROUND(AVG(fdfa2.field_aggc2_value),2) as count2, 'Rate of generated load of agglomeration addressed through IAS (% of p.e.)' as label2, 
              ROUND(AVG(fdfanot.field_aggpercwithouttreatment_value),2) as count3, 'Rate of generated load of agglomeration not collected through collecting systems and not addressed through IAS (% of p.e.)' as label3
  FROM {node} n
  JOIN {field_data_field_aggc1} fdfa1 ON fdfa1.entity_id = n.nid
  JOIN {field_data_field_aggc2} fdfa2 ON fdfa2.entity_id = n.nid
  JOIN {field_data_field_aggpercwithouttreatment} fdfanot ON fdfanot.entity_id = n.nid
  JOIN {field_data_field_position_geo} fdfpg ON fdfpg.entity_id = n.nid";

  if($groupby == 'basin') {
    $query .= " JOIN river_basin rb ON ST_Intersects(rb.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  } elseif($groupby == 'regions') {
    $query .= " JOIN regions_nuts rn ON ST_Intersects(rn.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  }

  if($year != 'all') {
    $query .= " JOIN {field_data_field_anneedata} fdfan ON fdfan.entity_id = n.nid";
  }

  $query .= " WHERE n.type = 'agglomeration'";

  if($year != 'all') {
    $query .= " AND fdfan.field_anneedata_value = :year";
  }

  $query .= " GROUP BY name";
  $query .= " ORDER BY name";

  if($year != 'all') {
    $results = db_query($query, array(':year' => $year));
  } else {
    $results = db_query($query);
  }

  $resultats = $results->fetchAll();
  foreach ($resultats as $resultat) {
    $stats[$resultat->name][$resultat->label1] = $resultat->count1;
    $stats[$resultat->name][$resultat->label2] = $resultat->count2;
    $stats[$resultat->name][$resultat->label3] = $resultat->count3;
  }

  return $stats;
}

/**************************************************************
 ************************* Stations ***************************
 **************************************************************/

/*
 * Get compliance for stations
*/
function uwwtd_statistics_uwwtp_compliance($year = 'all', $groupby = '') {
  $stats = array();
  $query = "SELECT ";

  if($groupby == 'basin') {
    $query .= " rb.rbdname as name,";
  } elseif($groupby == 'regions') {
    $query .= " rn.n3nm as name,";
  }

  $query .= " COUNT(fdfa.field_uwwcompliance_value), fdfa.field_uwwcompliance_value as label
  FROM {node} n
  JOIN {field_data_field_uwwcompliance} fdfa ON fdfa.entity_id = n.nid
  JOIN {field_data_field_position_geo} fdfpg ON fdfpg.entity_id = n.nid";

  if($groupby == 'basin') {
    $query .= " JOIN river_basin rb ON ST_Intersects(rb.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  } elseif($groupby == 'regions') {
    $query .= " JOIN regions_nuts rn ON ST_Intersects(rn.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  }

  if($year != 'all') {
    $query .= " JOIN {field_data_field_anneedata} fdfan ON fdfan.entity_id = n.nid";
  }

  $query .= " WHERE n.type = 'uwwtp'";

  if($year != 'all') {
    $query .= " AND fdfan.field_anneedata_value = :year";
  }

  $query .= " GROUP BY name, fdfa.field_uwwcompliance_value";
  $query .= " ORDER BY name";

  if($year != 'all') {
    $results = db_query($query, array(':year' => $year));
  } else {
    $results = db_query($query);
  }

  $resultats = $results->fetchAll();
  foreach ($resultats as $resultat) {
    //override PD and QC value, we don't want to display QC and PD (for now) 
    if (isset($GLOBALS['uwwtd']['ui']['compliance_connection'][ $resultat->label ])) {
        $resultat->label = $GLOBALS['uwwtd']['ui']['compliance_connection'][ $resultat->label ];    
    }   
    $stats[$resultat->name][$resultat->label] += $resultat->count;
  }

  return $stats;
}

/*
 * Get treatment data for stations
*/
function uwwtd_statistics_uwwtp_treatment($year = 'all', $groupby = '') {
  $stats = array();
  $query = "SELECT ";

  if($groupby == 'basin') {
    $query .= " rb.rbdname as name,";
  } elseif($groupby == 'regions') {
    $query .= " rn.n3nm as name,";
  }

  $query .= " COUNT(fdfu.field_uwwtreatmenttype_value) as count1, fdfu.field_uwwtreatmenttype_value as label1
    FROM {node} n
    JOIN {field_data_field_uwwtreatmenttype} fdfu ON fdfu.entity_id = n.nid
    JOIN {field_data_field_position_geo} fdfpg ON fdfpg.entity_id = n.nid";

  if($groupby == 'basin') {
    $query .= " JOIN river_basin rb ON ST_Intersects(rb.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  } elseif($groupby == 'regions') {
    $query .= " JOIN regions_nuts rn ON ST_Intersects(rn.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  }

  if($year != 'all') {
    $query .= " JOIN {field_data_field_anneedata} fdfan ON fdfan.entity_id = n.nid";
  }

  $query .= " WHERE n.type = 'uwwtp'";

  if($year != 'all') {
    $query .= " AND fdfan.field_anneedata_value = :year";
  }

  $query .= " GROUP BY name, label1";
  $query .= " ORDER BY name";

  if($year != 'all') {
    $results = db_query($query, array(':year' => $year));
  } else {
    $results = db_query($query);
  }

  $resultats = $results->fetchAll();
  foreach ($resultats as $resultat) {
    $stats[$resultat->name][$resultat->label1] = $resultat->count1;
  }

  return $stats;
}

/**************************************************************
 ********************** Discharge points **********************
 **************************************************************/

/*
 * Get quantity of discharge points
*/
function uwwtd_statistics_discharge_point($year = 'all', $groupby = '') {
  $stats = array();
  $query = "SELECT ";

  if($groupby == 'basin') {
    $query .= " rb.rbdname as name,";
  } elseif($groupby == 'regions') {
    $query .= " rn.n3nm as name,";
  }

  $query .= " COUNT(*), 'Quantity' as label
  FROM {node} n
  JOIN {field_data_field_position_geo} fdfpg ON fdfpg.entity_id = n.nid";

  if($groupby == 'basin') {
    $query .= " JOIN river_basin rb ON ST_Intersects(rb.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  } elseif($groupby == 'regions') {
    $query .= " JOIN regions_nuts rn ON ST_Intersects(rn.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  }

  if($year != 'all') {
    $query .= " JOIN {field_data_field_anneedata} fdfan ON fdfan.entity_id = n.nid";
  }

  $query .= " WHERE n.type = 'discharge_point'";

  if($year != 'all') {
    $query .= " AND fdfan.field_anneedata_value = :year";
  }

  $query .= " GROUP BY name";
  $query .= " ORDER BY name";

  if($year != 'all') {
    $results = db_query($query, array(':year' => $year));
  } else {
    $results = db_query($query);
  }

  $resultats = $results->fetchAll();
  foreach ($resultats as $resultat) {
    $stats[$resultat->name][$resultat->label] = $resultat->count;
  }

  return $stats;
}

/**************************************************************
 ********************** Sensitive areas ***********************
 **************************************************************/

/*
 * Get list of sensitive areas
*/
function uwwtd_statistics_sensitive_area($year = 'all', $groupby = '') {
  $stats = array();
  $query = "SELECT ";

  if($groupby == 'basin') {
    $query .= " rb.rbdname as name,";
  } elseif($groupby == 'regions') {
    $query .= " rn.n3nm as name,";
  }

  $query .= " COUNT(*), 'Quantity' as label
  FROM {node} n
  JOIN {field_data_field_position_geo} fdfpg ON fdfpg.entity_id = n.nid";

  if($groupby == 'basin') {
    $query .= " JOIN river_basin rb ON ST_Intersects(rb.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  } elseif($groupby == 'regions') {
    $query .= " JOIN regions_nuts rn ON ST_Intersects(rn.the_geom4326, ST_GeomFromWKB(fdfpg.field_position_geo_geom, 4326))";
  }

  if($year != 'all') {
    $query .= " JOIN {field_data_field_anneedata} fdfan ON fdfan.entity_id = n.nid";
  }

  $query .= " WHERE n.type = 'receiving_area'";

  if($year != 'all') {
    $query .= " AND fdfan.field_anneedata_value = :year";
  }

  $query .= " GROUP BY name";
  $query .= " ORDER BY name";

  if($year != 'all') {
    $results = db_query($query, array(':year' => $year));
  } else {
    $results = db_query($query);
  }

  $resultats = $results->fetchAll();
  foreach ($resultats as $resultat) {
    $stats[$resultat->name][$resultat->label] = $resultat->count;
  }

  return $stats;
}

