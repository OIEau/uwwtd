/**
 * @file
 * EU Cookie Compliance admin script.
 */

(function ($) {
  function showHideThankYouFields(showHide) {
    if (showHide) {
      $('.form-item-eu-cookie-compliance-popup-find-more-button-message, .form-item-eu-cookie-compliance-popup-hide-button-message, .form-item-eu-cookie-compliance-popup-hide-agreed').show();
      $('.form-item-eu-cookie-compliance-popup-agreed-value').parent().show();

      $('#edit-eu-cookie-compliance-popup-agreed-value').prop('required','required');
      $('#edit-eu-cookie-compliance-popup-find-more-button-message').prop('required','required');
      $('#edit-eu-cookie-compliance-popup-hide-button-message').prop('required','required');

      $('.form-item-eu-cookie-compliance-popup-agreed-value label, .form-item-eu-cookie-compliance-popup-find-more-button-message label, .form-item-eu-cookie-compliance-popup-hide-button-message label').append('<span class="form-required">*</span>');
    }
    else {
      $('.form-item-eu-cookie-compliance-popup-find-more-button-message, .form-item-eu-cookie-compliance-popup-hide-button-message, .form-item-eu-cookie-compliance-popup-hide-agreed').hide();
      $('.form-item-eu-cookie-compliance-popup-agreed-value').parent().hide();

      $('#edit-eu-cookie-compliance-popup-agreed-value').prop('required','');
      $('#edit-eu-cookie-compliance-popup-find-more-button-message').prop('required','');
      $('#edit-eu-cookie-compliance-popup-hide-button-message').prop('required','');

      $('.form-item-eu-cookie-compliance-popup-agreed-value label span, .form-item-eu-cookie-compliance-popup-find-more-button-message label span, .form-item-eu-cookie-compliance-popup-hide-button-message label span').remove();
    }
  }

  $(function () {
    showHideThankYouFields($('#edit-eu-cookie-compliance-popup-agreed-enabled').prop('checked'));

    $('#edit-eu-cookie-compliance-popup-agreed-enabled').click(function () {
      showHideThankYouFields($(this).prop('checked'));
    });

  });

} (jQuery))
