<?php

require_once(drupal_get_path('module', 'uwwtd') . '/inc/data_center.inc');

function uwwtd_correction_slovenia_date() {
    if ('si' != variable_get('siif_eru_country_code', '-1')) {
        return '';
    }
    $r = fopen(drupal_get_path('module', 'uwwtd') . '/script/correction/SI_register_deadlines-agglo.csv', 'rb');
    dsm($r);
    //header line
    $line = fgetcsv($r, 1000, ';');
    dsm($line);
    //data line
    while ($line = fgetcsv($r, 1000, ';')) {
        dsm($line);
        $siteId = uwwtd_get_siteid('agglomeration', array('year' => '2012', 'aggCode' => $line[0]));
        dsm($siteId);
        $nidagglo = uwwtd_check_exist($siteId);
        dsm($nidagglo);
        if ($nidagglo == '') {
            dsm('Error agglo ' . $line[0] . ' don\'t exists in database');
            continue;
        }
        $line[3] = uwwtd_check_date($line[3]);
        $line[4] = uwwtd_check_date($line[4]);
        $line[5] = uwwtd_check_date($line[5]);
        
        dsm($line);
        
        $agg = node_load($nidagglo);
        
        dsm('before correction');
        dsm('field_aggperiodover3 : ' . $agg->field_aggperiodover3['und'][0]['value']);
        dsm('field_aggperiodover4 : ' . $agg->field_aggperiodover4['und'][0]['value']);
        dsm('field_aggperiodover5 : ' . $agg->field_aggperiodover5['und'][0]['value']);
        
        $agg->field_aggperiodover3['und'][0]['value'] = $line[3];
        $agg->field_aggperiodover4['und'][0]['value'] = $line[4]; 
        $agg->field_aggperiodover5['und'][0]['value'] = $line[5];
        
        dsm('after correction');
        dsm('field_aggperiodover3 : ' . $agg->field_aggperiodover3['und'][0]['value']);
        dsm('field_aggperiodover4 : ' . $agg->field_aggperiodover4['und'][0]['value']);
        dsm('field_aggperiodover5 : ' . $agg->field_aggperiodover5['und'][0]['value']);
                
          
        node_save($agg);
    }
    fclose($r);
    return 'END'; 
}
