<?php
/**
 * @file
 * Contains the EmptyFieldText plugin for EmptyFieldHandler.
 */

/**
 * Defines EmptyFieldText.
 */
class EmptyFieldText extends EmptyFieldHandler {

  /**
   * Implementation of EmptyFieldText::defaults().
   *
   * @return array
   *   An array of default_values for the form below. Key names must match.
   */
  public function defaults() {
    return array(
      'empty_text' => '',
    );
  }

  /**
   * Implementation of EmptyFieldText::form().
   *
   * @return array
   *   A FAPI array to be used in configuration of this empty text plugin.
   */
  public function form($context) {
    $form['empty_text'] = array(
      '#type' => 'textarea',
      '#title' => t('Display Custom Text'),
      '#default_value' => isset($this->options['empty_text']) ? $this->options['empty_text'] : '',
      '#description' => t('Display text if the field is empty.'),
    );

    return $form;
  }

  /**
   * Implementation of EmptyFieldText::react().
   *
   * @return string
   *   A rendered string of html for display.
   */
  public function react($context) {
    global $user;
    $args = array(
      $context['entity_type'] => $context['entity'],
      'user' => $user,
    );
    $text = token_replace($this->options['empty_text'], $args, array('clear' => TRUE));
    return filter_xss_admin(t('!empty_text', array('!empty_text' => $text)));
  }

  /**
   * Implementation of EmptyFieldText:summaryText().
   *
   * @return string
   *   Text for the field formatter settings summary.
   */
  public function summaryText() {
    return t('Empty Text: @empty_text', array('@empty_text' => $this->options['empty_text']));
  }
}
