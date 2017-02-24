<?php



$country_list = array(
    'at'=>array('CountryName'=>'Austria', 'centerpoint' => '16.372 48.208'),
    'be'=>array(
        'CountryName'=>'Belgium', 'centerpoint' => '4.352042 50.845200'
    ),
    'bg'=>array('CountryName'=>'Bulgaria'),
    'cy'=>array(
        'CountryName'=>'Cyprus', 'centerpoint' => '33.388320, 35.190170', 
        'metadataAggloUid'=> 'e3dcd08579cd958bcaa5028b7b5fc99c5b62dcec', 'metadataUwwtpUid'=> '0b3d405f94cca841715d2d679a595b0a74bf07d6', 'metadataDpUid'=> '395eab5983b1e859a4493559e90de24d47cae488', 'metadataRcaUid'=> '5227395618eb647493f2a8c68d06d7cf2449455c', 
    ),
    'cz'=>array('CountryName'=>'Czech Republic'),
    'de'=>array('CountryName'=>'Germany'),
    'dk'=>array(
        'CountryName'=>'Denmark', 'centerpoint' => '14.797346, 46.136272',
        'metadataAggloUid' => '8bbb2d949392820fdc973afe131eebd7e29d0bb5',	'metadataUwwtpUid' => '8e17c822fbe18debbc0243bebc2399026f7d40d7',	'metadataDpUid' => 'd136df0a0bc78cb717e13aab577afdaa21f3dccf',	'metadataRcaUid' => '9f0200694092610e51a3a186f79c157a8a14c3fe',
    ),
    'ee'=>array('CountryName'=>'Estonia'),
    'es'=>array('CountryName'=>'Spain'),
    'fi'=>array('CountryName'=>'Finland'),
    'fr'=>array(
        'CountryName'=>'France', 'centerpoint' => '2.4609374564489412, 45.95114971020651',
    ),
    'gr'=>array('CountryName'=>'Greece'),
    'hr'=>array(
        'CountryName'=>'Croatia','centerpoint' => '15.971885, 45.825365',
        'metadataAggloUid' => '5b628e65b4c29115ad48a7213bddfa93f05c3352','metadataUwwtpUid' => 'a5ab0d56d60c781b39856299d5a8baaaed3cd1fc','metadataDpUid' => '912a27a10bff193276a5f4fde6422b5ce90a05d7','metadataRcaUid' => '0f7832c51afc43cac0c89899781a399de0b0e04a',
    ),
    'hu'=>array('CountryName'=>'Hungary'),
    'ie'=>array(
        'CountryName'=>'Ireland','centerpoint' => '-6.261830, 53.348971',
        'metadataAggloUid' => '1c6b1704fd655459b15dde46aa59f9fb9e89d84b',	'metadataUwwtpUid' => 'd15eb23f5bf40de2dea26471b38e68dca0939ade','metadataDpUid' => '891cbde1a01d7e2b4f3c64e7aa04d99f8b92314f','metadataRcaUid' => 'b581c2b264061fc51302685940452b3d436e12bb',
    ),
    'it'=>array('CountryName'=>'Italia'),
    'lt'=>array(
        'CountryName'=>'Lithuania', 'centerpoint' => '25.244692, 54.693822',
    ),
    'lu'=>array('CountryName'=>'Luxembourg'),
    'lv'=>array('CountryName'=>'Latvia'),
    'mt'=>array('CountryName'=>'Malta'),
    'nl'=>array('CountryName'=>'Netherlands'),
    'pl'=>array(
        'CountryName'=>'Poland','centerpoint' => '19.995117147985514, 52.26815738901357',
        'metadataAggloUid' => 'e91553b13416af37cb1c1af4b41df0a69d95af2b','metadataUwwtpUid' => 'fb878c358115854197a77f9771463e8830711aee','metadataDpUid' => '38705496d51f2973e95172657504683dac92580e','metadataRcaUid' => '3b014692338d54a94ee9892afeae6abb90a7d765',
    ),
    'pt'=>array('CountryName'=>'Portugal'),
    'ro'=>array(
        'CountryName'=>'Romania','centerpoint' => '26.27929681280725, 46.012223874093685',
        'metadataAggloUid' => '0279b40fdf6e847eee8795ec343d9f3f3826bf70',	'metadataUwwtpUid' => 'e7cf223294aa2298837fc5c56f139e7f4c4225ae',	'metadataDpUid' => '8995e1257d8df55075c82ec7dc26a18801d96351',	'metadataRcaUid' => '36503f6d796b7e29c7d96b09ef8c14f400f81dd9',
    ),
    'se'=>array('CountryName'=>'Sweden'),
    'si'=>array(
        'CountryName'=>'Slovenia','centerpoint' => '14.797346, 46.136272',
        'metadataAggloUid' => '8bbb2d949392820fdc973afe131eebd7e29d0bb5','metadataUwwtpUid' => '8e17c822fbe18debbc0243bebc2399026f7d40d7','metadataDpUid' => 'd136df0a0bc78cb717e13aab577afdaa21f3dccf','metadataRcaUid' => '9f0200694092610e51a3a186f79c157a8a14c3fe',
    ),
    'sk'=>array('CountryName'=>'Slovakia'),
    'uk'=>array('CountryName'=>'United Kindom'), 
);
define('page_regulations_mask','<h2>European legislation</h2><ul><li><a href="http://eur-lex.europa.eu/LexUriServ/LexUriServ.do?uri=CONSLEG:1991L0271:20081211:EN:PDF" target="_blank">Directive 91/271/EC Consolidated version </a></li><li><a href="http://eur-lex.europa.eu/legal-content/EN/TXT/PDF/?uri=CELEX:31991L0271&amp;from=en" target="_blank">Directive 91/271/EC legal version</a></li><li><a href="http://ec.europa.eu/environment/water/water-urbanwaste/legislation/pdf/transitional_periods_eu10_eu2.pdf" target="_blank">Deadlines of transitional periods for new Member States-EU-12</a></li></ul><h2>National regulations</h2><ul><li>...</li></ul>');
define('page_homepage_mask','Welcome to the Urban Waste Water Treatment Directive (UWWTD) site for :cn !');
define('page_credits_mask','<p>This website was developed by Office International de l’Eau (OIEau) for The <strong>European Commission Directorate General Environment</strong> under Specific contract n°<em>07.0307/2012/638595/D2</em> implementing Framework Service Contract n°ENV.D.2/FRA/2012/0013 It is developed using <em>open source softwares</em>.</p><p>A generic version together with instruction for installation and use can be found here <a href="http://uwwtd.oieau.fr" target="_blank">http://uwwtd.oieau.fr</a></p><p>Copyrights UWWTD-SIIF platform by <strong>Office International de l\'Eau</strong> is licensed under a Creative Commons Attribution-NonCommercial 4.0 International License.</p>');
define('block_homepage_mask','Welcome to the Urban Waste Water Directive (UWWTD) Structured Information and Implementation Framework (SIIF) website for :cn . 
The main aim of UWWTD SIIF is to ensure the production and the online dissemination of information allowing:
- the citizens to know the status of implementation of the Directive,
- water stakeholders to access most up to date datasets.
For more details on the UWWTD SIIF framework, look at [URL to concept paper]
The SIIF concept was initially introduced in 2012 through the Commission Communication COM/2012/095 on “Improving the delivery of benefits from EU environment measures: building confidence through better knowledge and responsiveness”. 
It was further elaborated in 2013 for the specific case of Urban Waste Water Directive through a concept paper and its technical annexes.
It was then implemented in 2014 through the current website that give access to information on agglomerations, Urban Waste Water treatment plants, discharge points and sensitive areas, and the associated datasets.');
define('website_mail','bfb@oieau.fr');
define('contact_mail','bfb@oieau.fr'),
define('metadata_catalog_url','http://www.uwwtd.oieau.fr/catalogue/srv'),

$country_config = array();

foreach($country_list as $cc=>$p){
    $mask =array(
        ':cn'=>$p['CountryName'],
        ':cc'=>$cc
    );
    $search = array_keys($mask);
    $replace = array_values($mask);
    
    $country_config[$cc] = array(      //nicosie
        'website_mail' => website_mail,
        'contact_mail' => contact_mail,
        'name' => $p['CountryName'],
        'openlayers_maps' => array(
              'center' => array(
                'initial' => array(
                  'centerpoint' => isset($p['centerpoint'])?$p['centerpoint']:'4.352042 50.845200',  //longitude, latitude
                  'zoom' => '8',         
                ),
                'restrict' => array(
                  'restrictextent' => 0,
                  'restrictedExtent' => '',
                ),
              ),
              'behaviors' => array(
                'openlayers_behavior_gazetteer' => array(
                  'restriction_query' => 'country='.strtoupper($cc),
                ),              
              ),
            ),        
        'page_regulations' => str_replace($search, $replace, page_regulations_mask),
        'page_homepage' => str_replace($search, $replace, page_homepage_mask),
        'page_credits' => str_replace($search, $replace, page_credits_mask),
        'block_homepage' => str_replace($search, $replace, block_homepage_mask),      
        'metadataAgglo' => metadata_catalog_url,
        'metadataAggloUid' => $p['metadataAggloUid'],
        'metadataUwwtp' => metadata_catalog_url,
        'metadataUwwtpUid' => $p['metadataUwwtpUid'],
        'metadataDp' => metadata_catalog_url,
        'metadataDpUid' => $p['metadataDpUid'],
        'metadataRca' => metadata_catalog_url,
        'metadataRcaUid' => $p['metadataRcaUid'],
	);
}


   