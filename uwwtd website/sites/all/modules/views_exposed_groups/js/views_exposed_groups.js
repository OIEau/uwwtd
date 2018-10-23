(function ($) {

Drupal.behaviors.viewsExposedGroups = {
  attach: function (context) {
    // Provide the vertical tab summaries.
    $('.view-filters .vertical-tabs-panes fieldset').each(function() {
      $(this).drupalSetSummary(function(context) {
        var vals = [];
        $(context).find(':input').each(function() {
          var $label = $('label[for="' + $(this).attr('id') + '"]');
          if(Drupal.checkPlain($(this).val()).length && $label.html().length) {
            vals.push(Drupal.checkPlain($label.html() + ': ' + Drupal.checkPlain($(this).val())));
          }
          
        });
        return vals.join('<br/>');
      });
    });
  }
};

})(jQuery);
