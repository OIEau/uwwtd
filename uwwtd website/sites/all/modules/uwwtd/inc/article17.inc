<?php
/**
 * Infos: 
 *     - To reorder fields : check fields_to_reorder in article.17.config.inc file 
 *     - To remove fields :  check fields_to_remove article.17.config.inc file
 *     - To add a new tab with a new content type : check article.17.config.inc file and create a new uwwtd_article17_get_[NAME OF THE CONTENT TYPE]_nids function
 *     - To transform a field content create a new  uwwtd_article17_transform_[FIELD NAME] function
 * 
 */
require_once drupal_get_path('module', 'uwwtd') . '/inc/uwwtd.config.inc';
require_once drupal_get_path('module', 'uwwtd') . '/inc/article17.config.inc';
require_once drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel.php';
require_once drupal_get_path('module', 'uwwtd') . '/lib/PHPExcel-1.8.1/Classes/PHPExcel/IOFactory.php';

/**
 * Export an excel file containing article 17 data
 * for the year $year
 */
function uwwtd_article17_export($year)
{
    $content_types = $GLOBALS['uwwtd']['article17']['content_types'];
    
    try {
        // Load de excel model
        $objReader = PHPExcel_IOFactory::createReader("Excel2007");
        $objPHPExcel = $objReader->load(drupal_get_path('module', 'uwwtd') . '/model/'."Article_17_model.xlsx");

        // Foreach article 17 content type
        foreach ($content_types as $content_type) {
            // Get content type nids for the year
            $nids = array();
            if (function_exists("uwwtd_article17_get_" . $content_type . "_nids")) {
                $nids = call_user_func("uwwtd_article17_get_" . $content_type . "_nids", array($year));
            } else {
                drupal_set_message("Aucune fonction d'export de $content_type n'a été trouvée.", "error");
            }
            // Convert nodes to phparray
            $csvarray = uwwtd_article_17_csvarray($content_type, $nids); 
            // Export to excel sheets
            $sheet = $objPHPExcel->getSheetByName($content_type);
            foreach ($csvarray as $line => $row) {
                foreach ($row as $col => $val) {
                    $sheet->setCellValueByColumnAndRow($col, $line+1, $val);
                }
            }
        }

        // Write the excel output to browser
        $writer = new PHPExcel_Writer_Excel2007($objPHPExcel);
        header('Content-type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
        header('Content-Disposition:inline;filename=Article_17_'.variable_get('siif_eru_country_name').'_'.$year.'.xlsx ');
        $writer->save('php://output');
        exit;
    } catch (Exception $e) {
        dsm($e->getMessage());
        return "";
    }
    return "";
}

/**
 * Return the nids for article 17 ms level nodes of year $year
 */
function uwwtd_article17_get_article_17_ms_level_nids($year)
{
    $nids = array();

    try {
        $query = "SELECT node.nid AS nid
                    FROM   {node} node
                           LEFT JOIN {field_data_field_year_of_datas} field_data_field_year_of_datas
                                  ON node.nid = field_data_field_year_of_datas.entity_id
                                     AND ( field_data_field_year_of_datas.entity_type = 'node'
                                           AND field_data_field_year_of_datas.deleted = '0' )
                    WHERE  (( ( node.status = '1' )
                              AND ( node.type IN ( :type ) )
                              AND ( field_data_field_year_of_datas.field_year_of_datas_value = :year ) )) ";

        $param = array(
            ':type' => "article_17_ms_level",
            ':year' => $year,
        );

        $result = db_query($query, $param);
        while ($row = $result->fetchAssoc()) {
            $nids[] = $row['nid'];
        }
    } catch (Exception $e) {
        dsm($e->getMessage());
    }

    return $nids;
}

/**
 * Return the nids for article 17 agglomeration nodes of year $year
 */
function uwwtd_article17_get_article_17_agglomeration_nids($year)
{
    $nids = array();

    try {
        $query = "SELECT node.nid AS nid
                    FROM   {node} node
                           INNER JOIN {field_data_field_anneedata} field_data_field_anneedata
                                   ON node.nid = field_data_field_anneedata.entity_id
                                      AND ( field_data_field_anneedata.entity_type = 'node'
                                            AND field_data_field_anneedata.deleted = '0' )
                    WHERE  (( ( node.status = '1' )
                              AND ( node.type IN ( :type ) )
                              AND ( field_data_field_anneedata.field_anneedata_value = :year ) )) ";

        $param = array(
            ':type' => "article_17_agglomeration",
            ':year' => $year,

        );

        $result = db_query($query, $param);
        while ($row = $result->fetchAssoc()) {
            $nids[] = $row['nid'];
        }
    } catch (Exception $e) {
        dsm($e->getMessage());
    }

    return $nids;
}

/**
 * Return the nids for article 17 uwwtp nodes of year $year
 */
function uwwtd_article17_get_article_17_uwwtp_nids($year)
{
    $nids = array();

    try {
        $query = "SELECT node.nid AS nid
                    FROM   {node} node
                           INNER JOIN {field_data_field_anneedata} field_data_field_anneedata
                                   ON node.nid = field_data_field_anneedata.entity_id
                                      AND ( field_data_field_anneedata.entity_type = 'node'
                                            AND field_data_field_anneedata.deleted = '0' )
                    WHERE  (( ( node.status = '1' )
                              AND ( node.type IN ( :type ) )
                              AND ( field_data_field_anneedata.field_anneedata_value = :year ) )) ";

        $param = array(
            ':type' => "article_17_uwwtp",
            ':year' => $year,

        );

        $result = db_query($query, $param);
        while ($row = $result->fetchAssoc()) {
            $nids[] = $row['nid'];
        }
    } catch (Exception $e) {
        dsm($e->getMessage());
    }

    return $nids;
}

/**
 * Return a php array containing a CSV description of all nodes
 * contained in the nids array
 */
function uwwtd_article_17_csvarray($content_type, $nids)
{
    //  Get Fields configuration
    $fields = field_info_instances('node', $content_type);

    // Remove fields to hide in CSV and reorganized accordingly
    $fields = uwwtd_article17_reorganize_fields($content_type, $fields);

    // Get labels for 1st row
    $csvheader = array();
    foreach ($fields as $fieldname => $values) {
        if($fieldname =='field_art17agg_link_agglo'){
            continue;
        }
        $csvheader[0][] = $values['label'];
    }

    // Export every node
    $nodes    = node_load_multiple($nids);
    $csvnodes = array();

    foreach ($nodes as $node) {
        $csvnodes[] = uwwtd_article17_export_node($node, $fields);
    }
    return array_merge($csvheader, $csvnodes);
}

/**
 * Converts any node of any type into a php array CSV representation
 */
function uwwtd_article17_export_node($node, $fields)
{
    $csv = array();

    // Foreach field
    foreach ($fields as $fieldname => $values) {
        $csvvalue = "";
        if($fieldname =='field_art17agg_link_agglo'){
            continue;
        }
        // Plugg in functions that will transform node content here :
        if(function_exists("uwwtd_article17_transform_".$fieldname)) {
            $csvvalue = call_user_func("uwwtd_article17_transform_".$fieldname, array($node->{$fieldname}));
        }
        // Default :
        else if (is_array($node->{$fieldname})) {
            $csvvalue = $node->{$fieldname}['und'][0]['value'];
        }
        $csv[] = $csvvalue;
    }
    // link to node
    global $base_url;
    $csv[] = $base_url . '/node/'.$node->nid;


    return $csv;
}

/**
 * Reorganize the fields of a content type accordingt to the configuration file
 */
function uwwtd_article17_reorganize_fields($content_type, $fields) {
    // Fields to remove
    $fields_to_remove = $GLOBALS['uwwtd']['article17'][$content_type]['fields_to_remove'];
    foreach ($fields_to_remove as $fieldname) {
        unset($fields[$fieldname]);
    }

    // Fields to reorder
    $fields_reordered = array();
    $fields_to_reorder = $GLOBALS['uwwtd']['article17'][$content_type]['fields_to_reorder'];

    $i = 1;
    foreach ($fields as $fieldname => $values) {
        while (isset($fields_to_reorder[$i])) {
            $fields_reordered[$fields_to_reorder[$i]] = $values;
            unset($fields[$fields_to_reorder[$i]]);
            $i++;
        }
        $fields_reordered[$fieldname] = $values;
        $i++;
    }
    return $fields_reordered;
}

/**
 * Transform field_art17_flaggstatus values
 */
function uwwtd_article17_transform_field_art17_flaggstatus($field_values) {
    return $GLOBALS['glossary'][$field_values[0]['und'][0]['value']];
}

/**
 * Transform field_art17_flatpstatus values
 */
function uwwtd_article17_transform_field_art17_flatpstatus($field_values) {
    if (isset($GLOBALS['glossary'][$field_values[0]['und'][0]['value']])) {
        return $GLOBALS['glossary'][$field_values[0]['und'][0]['value']];
    }
    else {
        return $field_values[0]['und'][0]['value'];
    }
}

/**
 * Transform field_soecperiod values
 */
function uwwtd_article17_transform_field_soecperiod($field_values) {
    if (isset($GLOBALS['article17']['glossary'][$field_values[0]['und'][0]['value']])) {
        return $GLOBALS['article17']['glossary'][$field_values[0]['und'][0]['value']];
    }
    else {
        return $field_values[0]['und'][0]['value'];
    }
}

/**
 * Transform field_art17_flaggeufundname values
 */
function uwwtd_article17_transform_field_art17_flaggeufundname($field_values) {
    if (isset($GLOBALS['article17']['glossary'][$field_values[0]['und'][0]['value']])) {
        return $GLOBALS['article17']['glossary'][$field_values[0]['und'][0]['value']];
    }
    else {
        return $field_values[0]['und'][0]['value'];
    }
}
