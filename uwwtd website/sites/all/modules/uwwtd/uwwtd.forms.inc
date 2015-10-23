<?php

function construct_extlinks_form($form, $form_state){

	dsm("
			BLOCK CODE ( DISPLAY EXTERNAL LINKS ):
		if (arg(0) == 'node' && is_numeric(arg(1))){
		   \$nodeid = arg(1);
		   \$node = node_load(\$nodeid);
		   \$data = array('node' => \$node);

			switch(\$node->type){
			   case 'agglomeration':
			      print_r('<a  href=\"' .  token_replace(variable_get('form_extlinks_agglomeration_url', ''),\$data) .'\">' . token_replace(variable_get('form_extlinks_agglomeration_url', ''),\$data) . '</a>');
			      break;
			   case 'uwwtp':
			      print_r('<a  href=\"' .  token_replace(variable_get('form_extlinks_uwwtp_url', ''),\$data) .'\">' . token_replace(variable_get('form_extlinks_uwwtp_url', ''),\$data) . '</a>');
			      break;
			   case 'discharge_point':
			      print_r('<a  href=\"' .  token_replace(variable_get('form_extlinks_discharge_point_url', ''),\$data) .'\">' . token_replace(variable_get('form_extlinks_discharge_point_url', ''),\$data) . '</a>');
			      break;
			   case 'receiving_area':
			      print_r('<a  href=\"' .  token_replace(variable_get('form_extlinks_receiving_area_url', ''),\$data) .'\">' . token_replace(variable_get('form_extlinks_receiving_area_url', ''),\$data) . '</a>');
			      break;
			   default:
			}
		}

	");

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