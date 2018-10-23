<?php
/**
 * @file
 * Provides a grouped form exposed form plugin for View 3.x.
 */
class views_exposed_groups_plugin extends views_plugin_exposed_form_basic {

  function summary_title() {
    return t('Grouped form');
  }

  function option_definition() {
    $options = parent::option_definition();
    
    $options['views_exposed_groups'] = array('default' => array());
    
	return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $groups = explode("\n", $this->options['views_exposed_groups']['groups']);
    $groups['no-group'] = t('- No group -');
    array_walk($groups, 'trim');
    $form['views_exposed_groups']['format_groups'] = array(
    	'#type' => 'select',
    	'#title' => 'Render groups as',
    	'#options' => array('vertical_tabs' => 'Vertical tabs',
    						'fieldsets_collapsed' => 'Fieldset (collapsed)',
    						'fieldsets' => 'Fieldsets(not collapsed)'),
    	'#default_value' => $this->options['views_exposed_groups']['format_groups'],
    );
    
    $form['views_exposed_groups']['groups'] = array(
    	'#type' => 'textarea',
    	'#title' => 'Groups',
    	'#description' => 'Enter a list of groups to include in this form',
    	'#default_value' => $this->options['views_exposed_groups']['groups'],
    );
    
    $weight_delta = count($this->display->handler->get_handlers('filter'));
    
    foreach ($this->display->handler->get_handlers('filter') as $filter_name => $filter) {
      if (!$filter->options['exposed']) {
        continue;
      }
      $label = $filter->options['expose']['identifier'];
      $field_label = ($filter->options['expose']['label']) ? $filter->options['expose']['label'] : $label;
      $form['views_exposed_groups']['group-'. $label]['group'] = array(
      	'#type' => 'select',
      	'#title' => 'Group for '. $field_label,
      	'#filter_field' => $filter,
      	'#options' => $groups,
        '#title_display' => 'invisible',
      	'#default_value' => $this->options['views_exposed_groups']['group-'. $label]['group']
      );
      $form['views_exposed_groups']['group-'. $label]['filter_name'] = array(
        '#type' => 'value',
        '#value' => $filter_name,
      );
      $form['views_exposed_groups']['group-'. $label]['weight'] = array(
        '#type' => 'weight',
      	'#title' => $field_label,
      	'#filter_field' => $filter,
      	'#delta' => $weight_delta,
        '#title_display' => 'invisible',
      	'#default_value' => $this->options['views_exposed_groups']['group-'. $label]['weight']
      );
    }
    $form['views_exposed_groups']['#theme'][] = 'views_exposed_groups_reorder_filter_form';
  }

  /*
   * Tweak the exposed filter form to show grouped form options.
   */
  function exposed_form_alter(&$form, &$form_state) {
    $form['#theme'] = '';//array(); //nd@oieau.fr =>why ?
    unset($form['#theme']);
    if($this->options['views_exposed_groups']['format_groups'] == 'vertical_tabs') {
  	  $form['filters'] = array(
  	      '#type' => 'vertical_tabs',
  	      '#weight' => -10,
  	  );
    }
  	else {
  	  $form['filters'] = array(
  	      '#weight' => -10,
  	  );
  	}
    $groups = explode("\n", $this->options['views_exposed_groups']['groups']);
    array_walk($groups, 'trim');
    $form['filters']['no-group'] = array( );
    foreach($groups as $key => $group) {
  		$form['filters'][$key] = array(
  			'#type' => 'fieldset',
  			'#title' => $group,
  	        '#collapsible' => TRUE,
  	        '#collapsed' => ($this->options['views_exposed_groups']['format_groups'] == 'fieldsets_collapsed') ? TRUE : FALSE,
  		);
  	}
  	$fields = $this->options['views_exposed_groups'];
  	unset($fields['groups']);
  	foreach($fields as $field => $group) {
  		$field = str_replace('group-', '', $field);

  		if(isset($form[$field]) && is_array($form[$field])) {
  			if($group['group'] == 'no-group') {
    			$form['filters']['no-group'][$field] = $form[$field] + 
    				array('#weight' => $group['weight'],
    				      '#title' => $form['#info']['filter-'. $group['filter_name']]['label']);
    			unset($form[$field]);
  			}
  			else {
    			$form['filters'][$group['group']][$field] = $form[$field] + 
    				array(
                        '#weight' => $group['weight'],
                        '#title' => $form['#info']['filter-'. $group['filter_name']]['label'],
                        '#description' => $form['#info']['filter-'. $group['filter_name']]['description']
                    );
    			unset($form[$field], $form['#info']['filter-'.$field]);
    		}
  		}
  		if(isset($form[$field .'_op']) && is_array($form[$field .'_op'])) {
  		  $form['filters'][$group['group']][$field .'_group'] = array(
  		    '#type' => 'fieldset',
  		    '#title' => check_plain($form['#info']['filter-'. $field]['label']),
  		  );
    		$form['filters'][$group['group']][$field .'_group'][$field] = $form['filters'][$group['group']][$field];
    		$form['filters'][$group['group']][$field .'_group'][$field .'_op'] = $form[$field .'_op'];
    		$form['filters'][$group['group']][$field .'_group'][$field]['#title_display'] = 'invisible';
    		$form['filters'][$group['group']][$field .'_group'][$field]['#weight'] = 10;
    		unset($form['filters'][$group['group']][$field]);
  			unset($form[$field .'_op']);
  		}
  	}
  	if (!empty($this->options['reset_button'])) {
      $form['reset'] = array(
        '#value' => $this->options['reset_button_label'],
        '#type' => 'submit',
      );
    }
  } 
}