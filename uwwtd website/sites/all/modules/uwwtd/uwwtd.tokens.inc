<?php


/**
 * Implements hook_token_info
 * @return multitype:string Ambigous <The, string, A, Optional>
 */
function uwwtd_token_info(){

  $info = array();

  $info['types']['agglomeration'] = array(
  		'name' => t('Agglomeration'),
  		'description' => t('Tokens for agglomerations.'),
  		//'needs-data' => 'node'
  );
  $info['tokens']['agglomeration']['Agg_code'] = array(
  		'name' => t('code of an agglomeration'),
  		'description' => t('code of an agglomeration.'),
  		'type' => 'uwwtd'

  );
  $info['tokens']['agglomeration']['Agg_name'] = array(
  		'name' => t('name of an agglomeration'),
  		'description' => t('name of an agglomeration.'),
  		'type' => 'uwwtd'

  );
  $info['tokens']['agglomeration']['Agg_yeardata'] = array(
  		'name' => t('year of data'),
  		'description' => t('year of data.'),
  		'type' => 'uwwtd'

  );
  $info['types']['Discharge_point'] = array(
  		'name' => t('discharge point'),
  		'description' => t('Tokens for discharge points.'),
  		//'needs-data' => 'node'
  );
  $info['tokens']['Discharge_point']['Dp_name'] = array(
  		'name' => t('name of a discharge point'),
  		'description' => t('name of a discharge point.'),
  		'type' => 'uwwtd'

  );
  $info['types']['receiving_area'] = array(
  		'name' => t('Receiving area'),
  		'description' => t('Tokens for receiving areas.'),
  		//'needs-data' => 'node'
  );
  $info['tokens']['receiving_area']['Ra_name'] = array(
  		'name' => t('name of a receiving area'),
  		'description' => t('name of a receiving area.'),
  		'type' => 'uwwtd'

  );
  $info['types']['uwwtp'] = array(
  		'name' => t('uwwtp'),
  		'description' => t('Tokens for uwwtp.'),
  		//'needs-data' => 'node'
  );
  $info['tokens']['uwwtp']['Uww_name'] = array(
  		'name' => t('name of an uwwtp'),
  		'description' => t('name of an uwwtp.'),
  		'type' => 'uwwtd'

  );

  return $info;
}

function uwwtd_tokens($type, $tokens, array $data = array(), array $options = array()){

  $replacements = array();
  if (false === isset($data['node'])) {
    return $replacements;
  }
	$type = $data['node']->type;

  // Text format tokens.
  if ($type == 'agglomeration' && !empty($data['node'])) {
  	$format = $data['node'];

  	foreach ($tokens as $name => $original) {
  		switch ($name) {
  			case 'Agg_code':
  				$replacements[$original] = check_plain($format->field_inspireidlocalid['und'][0]['value']);
  				break;
  			case 'Agg_name':
  				$replacements[$original] = check_plain($format->title);
  				break;
  			case 'Agg_yeardata':
  				$replacements[$original] = $format->field_anneedata['und'][0]['value'];
  				break;

  		}
  	}

  }
  if ($type == 'uwwtp' && !empty($data['node'])) {

  	$format = $data['node'];

  	foreach ($tokens as $name => $original) {
  		switch ($name) {
  			case 'Uww_name':
  				$replacements[$original] = check_plain($format->title);
  				break;

  		}
  	}
  }
  if ($type == 'discharge_point' && !empty($data['node'])) {
  	$format = $data['node'];

  	foreach ($tokens as $name => $original) {
  		switch ($name) {
  			case 'Dp_name':
  				$replacements[$original] = check_plain($format->title);
  				break;

  		}
  	}
  }

  if ($type == 'receiving_area' && !empty($data['node'])) {
  	$format = $data['node'];

  	foreach ($tokens as $name => $original) {
  		switch ($name) {
  			case 'Ra_name':
  				$replacements[$original] = check_plain($format->title);
  				break;

  		}
  	}
  }

  return $replacements;

}