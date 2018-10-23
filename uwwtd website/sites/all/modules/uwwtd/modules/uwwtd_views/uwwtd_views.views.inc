<?php

/**
 * Implements hook_views_data()
 *
 */
function uwwtd_views_views_data() {
//       dsm(__FUNCTION__);
 $table = array(
    'uwwtd_import_errors' => array(  // Mydons Custom table
       'table' => array(
          'group' => 'uwwtd_import_errors',      // Groupname Mydons
          'base' => array(
            'field' => 'errid',  //Primary key
            'title' => 'uwwtd_import_errors',
            'help' => 'uwwtd_import_errors database table'
          )
        ),
         //Description of errid
        'errid' => array(  
           'title' => t('errid'),
           'help' => t('The primary identifier for a error.'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE,  
              'handler' => 'views_handler_field_numeric',
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_numeric'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),
         //Description of errid
        'type' => array(  
           'title' => t('type'),
           'help' => t('Type of the error.'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE,  
              'handler' => 'views_handler_field',
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_numeric'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),
         //Description of errid
        'category' => array(  
           'title' => t('category'),
           'help' => t('Category of the error'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE,  
              'handler' => 'views_handler_field',
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_numeric'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),
         //Description of errid
        'year' => array(  
           'title' => t('year'),
           'help' => t('reference year of data'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE,  
              'handler' => 'views_handler_field_numeric',
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_numeric'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),
         //Description of errid
        'error' => array(  
           'title' => t('error'),
           'help' => t('description of the error.'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE, 
              'handler' => 'views_handler_field', 
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_string'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),
         //Description of errid
        'date' => array(  
           'title' => t('date'),
           'help' => t('The Unix timestamp of the error.'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE, 
              'handler' => 'views_handler_field_date', 
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_date'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),
         //Description of errid
        'entity_type' => array(  
           'title' => t('entity_type'),
           'help' => t('The type of entity.'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE,  
              'handler' => 'views_handler_field',
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_string'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),
         //Description of errid
        'entity_id' => array(  
           'title' => t('entity_id'),
           'help' => t('The primary identifier for an entity.'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE,  
              'handler' => 'views_handler_field_node',
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_numeric'     
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           ),
           'relationship' => array(
              'base' => 'node', // The name of the table to join with.
              'base field' => 'nid', // The name of the field on the joined table.
              // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
              'handler' => 'views_handler_relationship',
              'label' => t('relationship to node table'),
              'title' => t('relationship to node table'),
              'help' => t('add a relation to node table'),
            ),           
         ),
         //Description of errid
        'bundle' => array(  
           'title' => t('bundle'),
           'help' => t('The bundle to which this entity belongs.'),
           // is mydons_custom_id field sortable TRUE
           'field' => array(
              'click sortable' => TRUE,  
              'handler' => 'views_handler_field_node_type',
            ),
            //Filter handler for filtering records by mydons_custom_id     
           'filter' => array(
               'handler' => 'views_handler_filter_node_type'   
            ),  

           'sort' => array(
              'handler' => 'views_handler_sort'       
           )
         ),                                                                        
      )
    );
    return $table;
}

function uwwtd_views_entity_info()
{
//     dsm(__FUNCTION__);
    return array(  
        'uwwtderror' => array (
            'label' => t('Uwwtd error'),
            'controller class' => 'EntityAPIController',
            'base table' => 'uwwtd_import_errors',
            'fieldable' => false,
            'entity keys' => array(
                'id' => 'errid',
                ),        
        )        
    );    
}
