(function($){
	
	$(document).ready(function(){
		$(".uwwtd_errors_tab > h3").click(function(){
		    $(".uwwtd_errors_tab").children(".uwwtd_errors_tab_header").toggle("fast");
		});

		$(".uwwtd_errors_tab_header > h4").click(function(){
		    $(this).parent().find(".uwwtd_errors_tab_type_header").toggle("fast");
		});

		$(".uwwtd_errors_tab_type_header > .uwwtd_error_Input > h5").click(function(){
		    $(this).parent().find(".uwwtd_error").toggle("fast");
			
		});
		
		$(".uwwtd_errors_tab_type_header > .uwwtd_error_Linking > h5").click(function(){
		    $(this).parent().find(".uwwtd_error").toggle("fast");
		});
		
		$(".uwwtd_errors_tab_type_header > .uwwtd_error_Geometry > h5").click(function(){
		    $(this).parent().find(".uwwtd_error").toggle("fast");
		});
		
		$(".uwwtd_errors_tab_type_header > .uwwtd_error_Conformity > h5").click(function(){
		    $(this).parent().find(".uwwtd_error").toggle("fast");
		});
		
		$(".uwwtd_errors_tab_type_header > .uwwtd_error_Format > h5").click(function(){
		    $(this).parent().find(".uwwtd_error").toggle("fast");
		});
		

		$(".uwwtd_ignore").click(function(){
			$(this).closest('[class^="uwwtd_error"]').hide(500, function() { $(this).remove(); });
			
			// update counter Notification
			var generalParent = $(this).closest('.uwwtd_errors_tab');
			var inputParent = $(this).closest('.uwwtd_errors_tab_header');
			
			var generalCounter = generalParent.find('> h3 .uwwtd_tab_header_total');
			var generalValue = generalCounter.text();
			
			var inputCounter = inputParent.find('> h4 .uwwtd_tab_header_total');
			var inputValue = inputCounter.text();
			
			var directParent = $(this).closest('[class^="uwwtd_error"]');
			var directParentType = $(this).closest('[class^="uwwtd_error_"]');
			var heading = directParent.prevAll('h5');
			var newcounter = heading.find('span [class^="uwwtd_tab_header_total"]');
			var newcounterValue = newcounter.text();
			
			// update in order
			generalValue--;
			inputValue--;
			newcounterValue--;
			
			// update counter
			newcounter.fadeOut(100, function(){ 
				$(this).html(newcounterValue);
				$(this).fadeIn(100); 
			});
			if(newcounterValue == 0){
				directParentType.hide(500, function() { $(this).remove(); });
			}
			
			inputCounter.fadeOut(100, function(){ 
				$(this).html(inputValue);
				$(this).fadeIn(100); 
			});
			if(inputValue == 0){
				inputParent.hide(500, function() { $(this).remove(); });
			}
			
			generalCounter.fadeOut(100, function(){ 
				$(this).html(generalValue);
				$(this).fadeIn(100); 
			});
			if(generalValue == 0){
				generalParent.hide(500, function() { $(this).remove(); });
			}

			var id = this.getAttribute("data-id");
			var attached = this.getAttribute("data-attached");
		    var data = 'action=delete_error&id=' + id + '&attached=' + attached;

		    //console.log(data);

			request = $.ajax({
				type: "post",
				url: Drupal.settings.basePath + 'sites/all/themes/uwwtd/inc/ajax.php',
				//data: data,
				data: data
			});

			request.done(function (response, textStatus, jqXHR){
		        // Log a message to the console
		        console.log(response);
		    });

		    request.fail(function (jqXHR, textStatus, errorThrown){
		        // Log the error to the console
		        console.error(
		            "The following error occurred: "+
		            textStatus, errorThrown
		        );
		    });
		});
	});
})(jQuery);