function uwwtd_labelformaterpie(label, series) {
    return "<div class=\"pie_slice\">" + Math.round(series.percent) + "%</div>";
}     

function uwwtd_labelformaterpie_legend(label, series) {
    return "<div class=\"pie_legend\">" + label + " (" + Math.round(series.percent) + "%)</div>";  
}   

function display_uwwtp_stackedbar(data) {
    var margin = {top: 0, right: 20, bottom: 30, left: 60},
        width = 200 - margin.left - margin.right,
        height = 250 - margin.top - margin.bottom;
        widthsvg = 220;
    
    var x = d3.scale.ordinal()
        .rangeRoundBands([0, width], .1);
    
    var y = d3.scale.linear()
        .rangeRound([height, 0]);
                  
    // var color = d3.scale.ordinal()
    //     .range(["#98abc5", "#8a89a6", "#7b6888", "#6b486b", "#a05d56", "#d0743c", "#ff8c00"]);
    
    var color = d3.scale.category10();
    
    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom");
    
    var yAxis = d3.svg.axis()
        .scale(y)
        .orient("left")
        .tickFormat(d3.format(".2s"));
    
    var svg = d3.select("#uwwtp_stackedbar_back")
//         .attr("width", width + margin.left + margin.right)
        .attr("width", widthsvg + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
      .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
            
  color.domain(d3.keys(data[0]).filter(function(key) { return key !== "type"; }));

  data.forEach(function(d) {
    var y0 = 0;  
    d.origin = color.domain().map(function(name) { return {name: name, y0: y0, y1: y0 += +d[name]}; });
    d.total = d.origin[d.origin.length - 1].y1;
  });

  data.sort(function(a, b) { return b.total - a.total; });

  x.domain(data.map(function(d) { return d.type; }));
  y.domain([0, d3.max(data, function(d) { return d.total; })]);

  svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(xAxis);

  svg.append("g")
      .attr("class", "y axis")
      .call(yAxis)
    .append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", -50)
      .attr("dy", ".71em")
      .style("text-anchor", "end")
      .text("t per year");

  var typeparam = svg.selectAll(".typeparam")
      .data(data)
    .enter().append("g")
      .attr("class", "g")
      .attr("transform", function(d) { return "translate(" + x(d.type) + ",0)"; });

  typeparam.selectAll("rect")
      .data(function(d) { return d.origin; })
    .enter().append("rect")
      //.attr("width", x.rangeBand())
      .attr("width", 25)
      .attr("y", function(d) { return y(d.y1); })
      .attr("height", function(d) { return y(d.y0) - y(d.y1); })
      .style("fill", function(d) { return color(d.name); });
      
    var legend = svg.selectAll(".legend")
      .data(color.domain().slice().reverse())
    .enter().append("g")
      .attr("class", "legend")
      .attr("transform", function(d, i) { return "translate(0," + i * 20 + ")"; });
    
    legend.append("rect")
      .attr("x", widthsvg - 18)
      .attr("width", 18)
      .attr("height", 18)
      .style("fill", color);
    
    legend.append("text")
      .attr("x", widthsvg - 24)
      .attr("y", 9)
      .attr("dy", ".35em")
      .style("text-anchor", "end")
      .text(function(d) { return d; });
      
         
}

function display_agglo_piechart(data) {    
    var widthsvg = 400;
    var width = 120;  
    var height = 120;
    var radius = 60;   
    var color = d3.scale.category10();
    var svg = d3.select('#agglo_piechart_back')   
      .append('svg')
      .attr('width', widthsvg)
      .attr('height', height)
      .append('g')
      .attr('transform', 'translate(' + (width / 2) +  ',' + (width / 2) + ')');
    var arc = d3.svg.arc()
      .outerRadius(radius);
    var pie = d3.layout.pie()
      .value(function(d) { return d.value; })
      .sort(null);
    var path = svg.selectAll('path')
      .data(pie(data))
      .enter()
      .append('path')
      .attr('d', arc)
      .attr('fill', function(d, i) { 
        return color(d.data.label);
      });      
    var legendRectSize = 12;
    var legendSpacing = 4;
    var legend = svg.selectAll('.legend')
      .data(color.domain())
      .enter()
      .append('g')
      .attr('class', 'legend')
      .attr('transform', function(d, i) {
        var height = legendRectSize + legendSpacing;
    //     var offset =  0;
    //     var horz = -2 * legendRectSize;
//         var horz = -radius;
        var horz = radius + 5;        
    //     var vert = i * height - offset;
        //var vert = i * height + radius;
//         console.log(i);
        var vert = i * height - radius;
        return 'translate(' + horz + ',' + vert + ')';
      });
    legend.append('rect')
      .attr('width', legendRectSize)
      .attr('height', legendRectSize)
      .style('fill', color)
      .style('stroke', color);
    legend.append('text')              
      .attr('x', legendRectSize + legendSpacing )
      .attr('y', legendRectSize - legendSpacing + 3)
      .attr('style', ' color: #4f4f4f;font--weight: bold;font-size: 12px;font-family: "Open Sans",sans-serif;')
      .text(function(d) { return d; });      
}
// 
// (function($){	
//     $(document).ready(function(){        
//         $("#agglo_piechart").flip(); 
//         $("#uwwtp_stackedbar").flip();    
//     })
// })(jQuery);      
