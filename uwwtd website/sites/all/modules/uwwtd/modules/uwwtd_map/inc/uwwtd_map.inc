<?php

$GLOBALS['uwwtd_map']['europe'] = array(
  'wkhtmlpath_command' => "/opt/wkhtmltox.0.12.4/bin/wkhtmltoimage --crop-y 10 --crop-w 700 --crop-h 1170 --javascript-delay 5000 '[#SOURCE#]' [#TARGET#]",
  'url_source' => 'https://dev.oieau.fr' . base_path() . 'map/europe',
  'main_map' => 'uwwtd_map_europe',
  'list_minimap' =>   array(
    
    'topleft' => array(
      //Açores  PT
      'Açores (PT)' => array(
        'title' => 'Açores (PT)',
        'map_name' => 'acore',
        'class'    => 'acore',
      ),
      //MAdeira  PT
      'Madeira (PT)' => array(
        'title' => 'Madeira (PT)',
        'map_name' => 'madeira',
        'class'    => 'madeira',
      ),
  
      //Islas Canriass  ES
      'Islas Canariass (ES)' => array(
        'title' => 'Islas Canariass (ES)',
        'map_name' => 'canariass',
        'class'    => 'canariass',
      ),
    ),

    'topright' => array(
      //FR
      'La Réunion (FR)' => array(
        'title' => 'La Réunion (FR)',
        'map_name' => 'reunion',
        'class'    => 'reunion',
      ),
      'Guadeloupe (FR)' => array(
        'title' => 'Guadeloupe (FR)',
        'map_name' => 'guadeloupe',
        'class'    => 'guadeloupe',
      ),
      'Martinique (FR)' => array(
        'title' => 'Martinique (FR)',
        'map_name' => 'martinique',
        'class'    => 'martinique',
      ),
      'Guyane (FR)' => array(
        'title' => 'Guyane (FR)',
        'map_name' => 'guyane',
        'class'    => 'guyane',
      ),
    ),

    'bottomleft' => array(
      //Malta 
      'Malta' => array(
        'title' => 'Malta',
        'map_name' => 'malta',
        'class'    => 'malta',
      ),
    ),
  ),
);