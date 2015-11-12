<?php

$GLOBALS['uwwtd']['sheet_art17'] = array(
//     'BOM' => "\xEF\xBB\xBF",
    'model' => '2015_article17_model_20151109.xlsx',
    'final_art17' => '2015_article17_model_final.xlsx',
    'model_type' => 'Excel2007',
    'target_type' => 'Excel2007',
    'path_model_file' => drupal_get_path('module', 'uwwtd_art17') . '/model/',
//     'path_created_file' => drupal_get_path('module', 'uwwtd') . '/model/test/',
//     'path_created_file' => drupal_realpath('public://') . '/register/',
    'path_created_file' => 'public://article17/',
    'basic_information' => array(
        'xlsx_name' => 'basic_information.xlsx',
        'xlsx_model_sheet_name' => 'BASIC INFORMATION',
        'sheet_index' => 0,
    ),   
    'uwwtp' => array(
        'xlsx_name' => 'flauwwtp.xlsx',
        'xlsx_model_sheet_name' => 'FLAUWWTP',
        'sheet_index' => 1,
    ),     
    'agglo' => array(
        'xlsx_name' => 'flaagglo.xlsx',
        'xlsx_model_sheet_name' => 'FLAagglo',
        'sheet_index' => 2,
    ),

    'investmentsjobs' => array(
        'xlsx_name' => 'investmentsjobs.xlsx',
        'xlsx_model_sheet_name' => 'InvestmentsJobs',
        'sheet_index' => 3,
    ),    
    
);    