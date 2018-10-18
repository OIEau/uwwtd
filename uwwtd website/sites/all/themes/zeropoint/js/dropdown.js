//0 Point jQuery Drop Down Menu 
// uses hoverIntent jquery plugin (http://cherne.net/brian/resources/jquery.hoverIntent.html 


jQuery(document).ready(function(){

// add some markup

jQuery('#navlinks li.expanded > a').addClass("expandfirst");
jQuery('#navlinks li.expanded ul li.expanded > a').addClass('expand');
jQuery('#navlinks li.expanded > ul').addClass('firstsublayer');
jQuery('#navlinks li.expanded ul li a').removeClass('active');
jQuery('#navlinks li.expanded ul li.expanded > ul').removeClass('firstsublayer').addClass('sublayer'); 


// show & hide functions

function show(){
	jQuery(this).children('.firstsublayer, .sublayer').show(); 
	}
	
function hide(){
	jQuery(this).children('.firstsublayer, .sublayer').hide();	
	}
	
// set some options	for the hover effect

var config = {    
     sensitivity: 5, // number = sensitivity threshold (must be 1 or higher)    
     interval: 100,  // number = milliseconds for onMouseOver polling interval    
     over: show,     // function = onMouseOver callback (REQUIRED)    
     timeout: 200,   // number = milliseconds delay before onMouseOut (200)  
     out: hide       // function = onMouseOut callback (REQUIRED)    
};

	 
     jQuery('#navlinks li.expanded').hoverIntent(config);
});