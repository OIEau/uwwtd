<?php
$GLOBALS['uwwtd']['article17']['model'] = '/model/Article_17_model_2018.xlsx';

// Content types to export
$GLOBALS['uwwtd']['article17']['content_types'] = array(
    'article_17_ms_level',
    'article_17_agglomeration',
    'article_17_uwwtp',
);

// Article 17 glossary
$GLOBALS['article17']['glossary'] = array( 
  "CU" => "current",
  "EXP" => "expected",
  "P" => "past",
  "ERDF" => "European Regional Development Fund (ERDF)",
  "CF" => "Cohesion fund (CF)",
  "EIB" => "European Investment Bank (EIB) Loan",
  "EBRD" => "European Bank for reconstruction and Development (EBRD) loan",
  "EPEEF" => "Environmental Protection and Energy Efficiency Fund (EPEEF)",
  "ESF"=> "European Social Fund (ESF)"
);

$GLOBALS['uwwtd']['article17']['article_17_ms_level']['fields'] = array(
    'field_year_of_datas',
    'field_soeccode',
    'field_soecinvcol',
    'field_soecinvcomments',
    'field_soecopcosts',
    'field_soecperiod',
    'field_soeclenghtcocs',  
    'field_soecstartyear',
    'field_soecinvias',
    'field_soeceufunds',
    'field_soecjobs',
    'field_soecnumbias',
    'field_soecpop',
    'field_soeclenghtsacs',
    'field_soeclenghtstcs',
    'field_soecinvtp',
    'field_soeccapacity',
    'field_soecendyear'
);
$GLOBALS['uwwtd']['article17']['article_17_agglomeration']['fields'] = array(
'field_art17agg_aggcode',
'field_art17agg_aggname',
'field_art17_flaggstatus',
'field_art17_flaggotherfund',
'field_art17_flaggloan',
'field_art17_flaggotherfundname',
'field_art17_flaggloanname',
'field_anneedata',
'field_art17_flaggreasons',
'field_art17_flaggmeasures',
'field_art17_flagginv',
'field_art17_flaggeufund',
'field_art17_flaggeufundname',
'field_art17_flaggexpecdatestartw',
'field_art17_flaggexpecdatecomple',
'field_art17_flaggcomments',
'field_art17_flaggexpload',
'field_art17_flaggexploadcoll',
'field_art17_flaggexploadias',
'field_art17_flaggexpecdatestart'
);
$GLOBALS['uwwtd']['article17']['article_17_uwwtp']['fields'] = array(
    'field_anneedata',
    'field_art17uwwtp_uwwcode',
    'field_art17uwwtp_uwwname',
    'field_art17_flatpstatus',
    'field_art17_flatpinv',
    'field_art17_flatpeufund',
    'field_art17_flatpotherfund',
    'field_art17_flatpmeasures',
    'field_art17_flatpeufundname',
    'field_art17uwwtp_aggcode',
    'field_art17uwwtp_aggname',
    'field_art17_flatpreasons',
    'field_art17_flatpexpload',
    'field_art17_flatpexpcapacity',
    'field_art17_flatptexpectreatment',
    'field_art17_flatpcomments',
    'field_art17_flatpexploadtruck',
    'field_art17_flatpexpecdatecomple',
    'field_art17_flatpexpecdateperfor',
    'field_art17_flatpotherfundname',
    'field_art17_flatploanname',
    'field_art17_flatploan',
    'field_art17_flatpexpecdatestart',
    'field_art17_flatpexpecdatestartw'
);