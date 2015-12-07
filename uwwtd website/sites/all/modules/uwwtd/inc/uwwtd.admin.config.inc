<?php

function uwwtd_general_config($form, $form_state){

// 	dsm('ok');

// 	$form['UI']['rptFormRA'] = array(
// 			'#type' => 'select',
// 			'#title' => t('Indication of type of receiving area'),
// 			'#description' => t('Please fill-in the relevant number depending on the designation of sensitive areas : 1 = There are no designated Sensitive or Less Sensitive Areas (all Territory is Normal Area), 2 = Application of Art. 5(8) and Art.5(2) + Art.5(3), 3 = Application of Art. 5(8) and Art.5(4), 4 = There are designated Sensitive Areas - Application of Art. 5(1), 5 = There are designated Less Sensitive Areas - Application of Art. 6(1), 6 = There are designated Sensitive Areas - Application of Art. 5(1) and
//                                  There are designated Less Sensitive Areas - Application of Art. 6(1)'),
// 			'#default_value' => variable_get('rptFormRA', '1'),
//             '#options' => array(
//                                     1 => t('1 : There are no designated Sensitive or Less Sensitive Areas (all Territory is Normal Area)'),
//                                     2 => t('2 : Application of Art. 5(8) and Art.5(2) + Art.5(3)'),
//                                     3 => t('3 : Application of Art. 5(8) and Art.5(4)'),
//                                     4 => t('4 : There are designated Sensitive Areas - Application of Art. 5(1)'),
//                                     5 => t('5 : There are designated Less Sensitive Areas - Application of Art. 6(1)'),
//                                     6 => t('6 : There are designated Sensitive Areas - Application of Art. 5(1) and There are designated Less Sensitive Areas - Application of Art. 6(1)'), 
//                                 ),
// 	);
    
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
