<?php

function construct_extlinks_form($form, $form_state){

	$form['form_extlinks']['form_extlinks_agglomeration_url'] = array(
			'#type' => 'textfield',
			'#title' => t('Agglomeration url'),
			'#description' => t ('The url of the agglomeration'),
			'#default_value'    => variable_get('form_extlinks_agglomeration_url', '')
	);
	$form['form_extlinks']['form_extlinks_discharge_point_url'] = array(
			'#type' => 'textfield',
			'#title' => t('Discharge point url'),
			'#description' => t ('The url of the discharge point'),
			'#default_value'    => variable_get('form_extlinks_discharge_point_url', '')
	);
	$form['form_extlinks']['form_extlinks_receiving_area_url'] = array(
			'#type' => 'textfield',
			'#title' => t('Receiving area url'),
			'#description' => t ('The url of the receiving area'),
			'#default_value'    => variable_get('form_extlinks_receiving_area_url', '')
	);
	$form['form_extlinks']['form_extlinks_uwwtp_url'] = array(
			'#type' => 'textfield',
			'#title' => t('Uwwtp url'),
			'#description' => t ('The url of the uwwtp'),
			'#default_value'    => variable_get('form_extlinks_uwwtp_url', '')
	);


	if (module_exists('token')) {
		$form['view']['token_help'] = array(
				'#title' => t('Replacement patterns'),
				'#type' => 'fieldset',
				'#collapsible' => FALSE,
				'#collapsed' => FALSE,
				//'#description' => t('Prefer raw-text replacements for text to avoid problems with HTML entities!'),
		);
		$form['view']['token_help']['help'] = array(
				'#theme' => 'token_tree',
				'#token_types' => array(),
				//"#global_types" => FALSE
		);
	}

	return system_settings_form($form);
}


?>