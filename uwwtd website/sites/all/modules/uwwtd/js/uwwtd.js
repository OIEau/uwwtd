(function ($) {
    $(document).ready(function() {
        // Gestion des flipcards
        $(".flip").flip({trigger: 'manual'});
        
        $(".flip .plus").click(function() {
          $(this).closest('.flip').flip(true);
        });
        $(".flip .minus").click(function() {
          $(this).closest('.flip').flip(false);
        });
    });    
})(jQuery);


function uwwtd_labelformaterpie(label, series) {
    return "<div class=\"pie_slice\">" + Math.round(series.percent) + "%</div>";
}     

function uwwtd_labelformaterpie_legend(label, series) {
    return "<div class=\"pie_legend\">" + label + " (" + Math.round(series.percent) + "%)</div>";  
}     
