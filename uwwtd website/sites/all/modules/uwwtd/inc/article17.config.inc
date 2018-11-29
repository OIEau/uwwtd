<?php
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

// Fields not to export in article_17_ms_level
$GLOBALS['uwwtd']['article17']['article_17_ms_level']['fields_to_remove'] = array(
    'body',
    'field_linked_ms_level',
);

// Fields to reorder in article_17_ms_level
$GLOBALS['uwwtd']['article17']['article_17_ms_level']['fields_to_reorder'] = array(
    1 => 'field_year_of_datas', // Should be 1st in excel output
    2 => 'field_soeccode', // Should be seoncde in excel output
);

// Fields not to export in article_17_agglomeration
$GLOBALS['uwwtd']['article17']['article_17_agglomeration']['fields_to_remove'] = array(
    'field_art17agg_link_agglo ',
);

// Fields not to export in article_17_agglomeration
$GLOBALS['uwwtd']['article17']['article_17_agglomeration']['fields_to_reorder'] = array(
    2 => 'field_art17agg_aggcode',
    3 => 'field_art17agg_aggname',
    4 => 'field_art17_flaggstatus'
);

// Fields not to export in article_17_uwwtp
$GLOBALS['uwwtd']['article17']['article_17_uwwtp']['fields_to_remove'] = array(
    'field_art17uwwtp_link_uwwtp',
);

// Fields to reorder in article_17_uwwtp
$GLOBALS['uwwtd']['article17']['article_17_uwwtp']['fields_to_reorder'] = array(
    1 => 'field_anneedata',
    2 => 'field_art17uwwtp_uwwcode',
    3 => 'field_art17uwwtp_uwwname',
    4 => 'field_art17_flatpstatus',
);
