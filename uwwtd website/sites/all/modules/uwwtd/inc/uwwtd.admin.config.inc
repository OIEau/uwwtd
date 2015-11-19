<?php

function uwwtd_general_config($form, $form_state){

// 	dsm('ok');

	$form['UI']['min_aggenerated_display'] = array(
			'#type' => 'textfield',
			'#title' => t('Minimum agglomeration generated load'),
			'#description' => t('Display agglomeration with generated load superior to this value'),
			'#default_value'    => variable_get('min_aggenerated_display', '0')
	);
    
	$form['UI']['display_only_active'] = array(
			'#type' => 'checkbox',
			'#title' => t('Display only active agglomeration and uwwtp'),
			//'#description' => t('Display only active agglomeration and uwwtp'),
			'#default_value' => variable_get('display_only_active', '1')
	); 
    
// 	$form['UI']['test'] = array(
// 			'#type' => 'textarea',
// 			'#title' => t('Display only active agglomeration and uwwtp'),
// 			//'#description' => t('Display only active agglomeration and uwwtp'),
// 			'#default_value' => variable_get('test', '')
// 	);      
//      dsm(variable_get('test', ''));
//      dsm(uwwtd_get_array_from_conf((variable_get('test', ''))));
//      dsm(uwwtd_get_keyarray_from_conf((variable_get('test', ''))));
	return system_settings_form($form);
}
