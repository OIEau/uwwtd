<?php

/**
 * @file
 * This file provides administration form for the module.
 */

/**
 * Provides form to configure GDPR compliant forms.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   For state array.
 *
 * @return array
 *   Admin form render array.
 */
function gdpr_form_compliance_admin_form($form, $form_state) {
  $form = array();

  $form['gdpr_form_compliance_display_form_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display form id on every form'),
    '#description' => t('This settings helps you to identify the ID of the form you want to add. Remember to deactivate this setting on production.'),
    '#default_value' => variable_get('gdpr_form_compliance_display_form_ids', false),
  );

  $form['gdpr_form_compliance_table'] = array(
    '#markup' => _gdpr_form_compliance_table()
  );

  $form['gdpr_form_compliance_group'] = array(
    '#type' => 'fieldset',
    '#tree' => true,
  );

  $form['gdpr_form_compliance_group']['form_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Form ID'),
    '#description' => t('Provide the ID of the form you want to attach the compliance field to.')
  );

  $form['gdpr_form_compliance_group']['compliance_field_value'] = array(
    '#type' => 'textarea',
    '#title' => t('GDPR compliance text'),
    '#description' => t('Provide a compliance text. You can use the !data_protection_statement token to add a link to your data protection statement.')
  );

  $form['gdpr_form_compliance_group']['data_protection_statement_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to your data protection statement'),
    '#description' => t('Can be a internal or external link.')
  );

  $form['gdpr_form_compliance_group']['data_protection_statement_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for your data protection statement link'),
    '#description' => t('This is used as the link text')
  );

  $form['#validate'][] = 'gdpr_form_compliance_admin_form_validate';
  $form['#submit'][] = 'gdpr_form_compliance_admin_form_submit';

  return system_settings_form($form);
}

function gdpr_form_compliance_edit_form($form, $form_state, $form_id) {
  $settings = gdpr_form_compliance_load_by_form_id($form_id);

  $form['edit_form_id'] = array(
    '#type' => 'value',
    '#value' => $form_id
  );

  $form['gdpr_form_compliance_group'] = array(
    '#type' => 'fieldset',
    '#tree' => true,
  );

  $form['gdpr_form_compliance_group']['form_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Form ID'),
    '#description' => t('Provide the ID of the form you want to attach the compliance field to.'),
    '#default_value' => $settings->form_id,
    '#disabled' => true
  );

  $form['gdpr_form_compliance_group']['compliance_field_value'] = array(
    '#type' => 'textarea',
    '#title' => t('GDPR compliance text'),
    '#description' => t('Provide a compliance text. You can use the @data_protection_statement token to add a link to your data protection statement.'),
    '#default_value' => $settings->compliance_field_value
  );

  $form['gdpr_form_compliance_group']['data_protection_statement_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to your data protection statement'),
    '#description' => t('Can be a internal or external link.'),
    '#default_value' => $settings->data_protection_statement_url
  );

  $form['gdpr_form_compliance_group']['data_protection_statement_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for your data protection statement link'),
    '#description' => t('This is used as the link text'),
    '#default_value' => $settings->data_protection_statement_link_text
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  $form['#validate'][] = 'gdpr_form_compliance_admin_form_validate';
  $form['#submit'][] = 'gdpr_form_compliance_admin_form_submit';

  return $form;
}

/**
 * Implementation of hook_form_validate()
 *
 * @param $form
 * @param $form_state
 */
function gdpr_form_compliance_admin_form_validate($form, $form_state) {
  if($form_state['values']['gdpr_form_compliance_group']['form_id'] != '') {
    if(isset($form_state['values']['gdpr_form_compliance_group']['form_id']) && strpos($form_state['values']['gdpr_form_compliance_group']['form_id'], ' ') !== false) {
      form_set_error('gdpr_form_compliance_group][form_id', t('The Form ID must contain only lowercase letters, numbers, and underscores.'));
    }
    if($form_state['values']['gdpr_form_compliance_group']['compliance_field_value'] == '') {
      form_set_error('gdpr_form_compliance_group][compliance_field_value', t('You must provide a GDPR compliance text'));
    }
  }
}

/**
 * Implementation of hook_form_submit()
 *
 * @param $form
 * @param $form_state
 */
function gdpr_form_compliance_admin_form_submit($form, $form_state) {
  if((isset($form_state['input']['gdpr_form_compliance_group']['form_id']) && $form_state['input']['gdpr_form_compliance_group']['form_id'] != '') || isset($form_state['values']['edit_form_id'])) {
    $record = array(
      'form_id' => (isset($form_state['input']['gdpr_form_compliance_group']['form_id'])) ? $form_state['input']['gdpr_form_compliance_group']['form_id'] : '',
      'compliance_field_value' => $form_state['input']['gdpr_form_compliance_group']['compliance_field_value'],
      'data_protection_statement_url' => $form_state['input']['gdpr_form_compliance_group']['data_protection_statement_url'],
      'data_protection_statement_link_text' => $form_state['input']['gdpr_form_compliance_group']['data_protection_statement_link_text']
    );
    if(isset($form_state['values']['edit_form_id'])) {
      $record['form_id'] = $form_state['values']['edit_form_id'];
      drupal_write_record('gdpr_form_compliance', $record, array('form_id'));
    } else {
      drupal_write_record('gdpr_form_compliance', $record);
    }
  }
}

function _gdpr_form_compliance_table() {
  $result = db_query('SELECT * FROM {gdpr_form_compliance}');

  $rows = array();
  foreach($result as $record) {
    $record->edit = l(t('edit'), 'admin/config/system/gdpr-form-compliance/' . $record->form_id . '/edit', array('query' => array(drupal_get_destination())));
    $record->delete = l(t('delete'), 'admin/config/system/gdpr-form-compliance/' . $record->form_id . '/delete', array('query' => array(drupal_get_destination())));
    $record->compliance_field_value = t($record->compliance_field_value, array('!data_protection_statement' => l(t($record->data_protection_statement_link_text), $record->data_protection_statement_url)));
    $rows[] = (array) $record;
  }

  $table = array(
    'header' => array(t('Form ID'), t('GDPR compliance text'), t('Data protection statement'), t('Link text'), '', ''),
    'rows' => $rows,
    'attributes' => array(),
    'caption' => t('Forms'),
    'colgroups' => array(),
    'sticky' => true,
    'empty' => t('You haven\'t configured a form yet')
  );

  return theme_table($table);
}

function gdpr_form_compliance_delete_form($form, $form_state, $form_id) {
  if(isset($form_state['input']['confirm']) && $form_state['input']['confirm'] == 1) {
    db_query('DELETE FROM {gdpr_form_compliance} WHERE form_id=:form_id', array(':form_id' => $form_id));
    drupal_set_message(t('Form @form_id deleted', array('@form_id' => $form_id)));
  } else {
    return confirm_form(
      array(),
      'Are you sure you want to submit this form?',
      'admin/config/system/gdpr-form-compliance',
      "You are about to submit your answers. Are you sure you want to submit this data?",
      'Yes, Submit the Data',
      'No, discard my answers and Start Over'
    );
  }
}