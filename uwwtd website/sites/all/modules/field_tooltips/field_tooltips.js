
/**
 * @file
 * Attaches the behaviors for the Field Tooltips module.
 */

(function ($) {
  Drupal.behaviors.fieldTooltips = {
    attach: function (context, settings) {
      // Initialize the tooltip
      $('.field-tooltips :input').tooltip({
        position: 'center right'
      });
    }
  };
}(jQuery));
