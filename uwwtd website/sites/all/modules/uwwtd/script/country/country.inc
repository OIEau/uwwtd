<?php

$country_config = array(
    'cy' => array(      //nicosie
        'openlayers_maps' => array(
              'center' => array(
                'initial' => array(
                  'centerpoint' => '33.388320, 35.190170',  //longitude, latitude
                  'zoom' => '8',         
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country=CY',
                ),              
              ),
            ),              
    ),
    'hr' => array(      //zagreb
        'openlayers_maps' => array(
              'center' => array(
                'initial' => array(
                  'centerpoint' => '15.971885, 45.825365',
                  'zoom' => '7',        
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country=HR',
                ),              
              ),
            ),             
    ),
    'ie' => array(    //Dublin
        'openlayers_maps' => array(
              'center' => array(
                'initial' => array(
                  'centerpoint' => '-6.261830, 53.348971', //'-6.32812500605299, 53.33087298053941',
                  'zoom' => '6',      
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country=IE',
                ),              
              ),
            ),             
    ),    
    'lt' => array(    //Vilnius
        'openlayers_maps' => array(
              'center' => array(
                'initial' => array(
                  'centerpoint' => '25.244692, 54.693822',
                  'zoom' => '6',      
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country=LT',
                ),              
              ),
            ),             
    ),
    'pl' => array(    // 	Varsovie
        'openlayers_maps' => array(
              'center' => array(
                'initial' => array(
                  'centerpoint' => '19.995117147985514, 52.26815738901357',
                  'zoom' => '5',           
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country=PL',
                ),              
              ),
            ),             
    ),
    'ro' => array(     //Bucarest 
        'openlayers_maps' => array(
              'center' => array(
                'initial' => array(
                  'centerpoint' => '26.27929681280725, 46.012223874093685',
                  'zoom' => '6',          
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country=RO',
                ),              
              ),
            ),             
    ),
    'si' => array(   //Ljubljana
        'openlayers_maps' => array(
              'center' => array(                        
                'initial' => array(
                  'centerpoint' => '14.797346, 46.136272',
                  'zoom' => '8',           
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country=SI',
                ),              
              ),
            ),             
    ),                               
);      