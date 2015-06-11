jQuery(document).ready(function() {
    (function($){
		$('.field').hover(function(){
		    $(this).find('.custom_tooltip').show();
		    }, function(){
		    $(this).find('.custom_tooltip').hide();
		});
	})(jQuery);
});