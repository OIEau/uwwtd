function uwwtd_labelformaterpie(label, series) {
    return "<div class=\"pie_slice\">" + Math.round(series.percent) + "%</div>";
}     

function uwwtd_labelformaterpie_legend(label, series) {
    return "<div class=\"pie_legend\">" + label + " (" + Math.round(series.percent) + "%)</div>";  
}     
