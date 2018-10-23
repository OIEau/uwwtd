/**
 * @file
 * Adds a function to generate a column chart to the `Drupal` object.
 */

(function($) {

  Drupal.d3.stackedcolumnchart = function (select, settings) {
    var rows = settings.rows;
    //Original data
    var xLabels = rows.map(function(d) { return d.shift(); });
    var dataset = [];
    rows.map(function(d, i){
        d.map(function(d, j){
            if(!dataset[j]) dataset[j] = [];
            dataset[j][i]={"y":d};
        });
        
    });
    var div = (settings.id) ? settings.id : 'visualization';  
      
    //Width and height
    
    var w = settings.width?settings.width:800;
    var h = settings.height?settings.height:400;
    var z = d3.scale.ordinal().range(["blue", "red", "orange", "green"]);
    var p = [20, 50, 30, 60];
    var chart =(w>400? {w: w * .65, h: h * .70}:{w: w*0.8, h: h * .70});
	var legend =(w>400? {w: w * .35, h:h}:{w: w, h:h*0.3});
    var key = settings.legend;
    var colors = (typeof settings.color!= 'undefined'?settings.color:null);
    
    //Set up stack method
    var stack = d3.layout.stack();

    //Data, stacked
    stack(dataset);

    //Set up scales
    var xScale = d3.scale.ordinal()
        .domain(d3.range(dataset[0].length))
        .rangeRoundBands([0, chart.w], 0.05);

    var yScale = d3.scale.linear()
        .domain([0,				
            d3.max(dataset, function(d) {
                return d3.max(d, function(d) {
                    return d.y0 + d.y;
                });
            })
        ])
        .range([0, chart.h]);
        
    var barWidth = xScale.rangeBand();
    if(barWidth>100)barWidth=100;
    //Easy colors accessible via a 10-step ordinal scale
    //var colors = d3.scale.category10();

    //Create SVG element
    var svg = d3.select("#"+div)
                .append("svg")
                .attr("width", w)
                .attr("height", h);
    var graph = svg.append("g")
      .attr("class", "chart")
      .attr("transform", "translate(" + p[3] + "," + p[0] + ")");
    

    /* X AXIS  */
    
    var xTicks = graph.selectAll("g.ticks")
      .data(dataset[0])
      .enter().append("g")
          .attr("class","ticks")
          .attr('transform', function(d,i) { return 'translate(' + (xScale(i) + (barWidth / 2)) + ',' + (chart.h) + ')'})
          .append("text")
          .attr("dy", ".71em")
          .attr("text-anchor", "end")
          .attr('transform', function(d,i) { return "rotate(-15)"; })
          .text(function(d,i){
            var content = '';
            var label = d3.splitString(xLabels[i],20);
            d3.selectAll(label)
              .each(function(d, i) {
                content = content +' '+ label[i];  
                //content = content+'<tspan text-anchor="end" x="0" y="'+ (20 * i) +'">'+label[i]+'</tspan>';
              });
            return content;  
          });
          /*
          .html(function(d,i){ 
           var content = '';
            var label = d3.splitString(xLabels[i],20);
            d3.selectAll(label)
              .each(function(d, i) {
                content = content+'<tspan text-anchor="end" x="0" y="'+ (20 * i) +'">'+label[i]+'</tspan>';
              });
            return content;
        });
        */
    
    /* LINES */
    
    var rule = graph.selectAll("g.rule")
      .data(yScale.ticks(4))
      .enter().append("g")
      .attr("class", "rule")
      .attr("transform", function(d) { return "translate(0," + (chart.h - yScale(d)) + ")"; });

    rule.append("line")
      .attr("x2", chart.w)
      .style("stroke", function(d) { return d ? "#ccc" : "#000"; })
      .style("stroke-opacity", function(d) { return d ? .7 : null; });
    
    /* Y AXIS */
    
    rule.append("text")
      .attr("x", -3)
      .attr("dy", ".3em")
      .attr("text-anchor", "end")
      .text(function(d,i){
            if(d>999) return d3.format(",d")(d);
            else return d3.format("")(d);  
        }
       );
      
    // Add a group for each row of data
    var groups = graph.selectAll("g.bars")
        .data(dataset)
        .enter()
        .append("g")
        .attr('fill', function(d,i) { 
            if(colors!=null && typeof(colors[i]) !='undefined'){
                return colors[i];
            }
            return d3.rgb(z(i)); 
          });
    // Add a rect for each data value
    
    var rects = groups.selectAll("rect")
        .data(function(d) { return d; })
        .enter()
        .append("rect")
        .attr("x", function(d, i) {
            return xScale(i);
        })
        .attr("y", function(d) {
            return chart.h-(yScale(d.y)+yScale(d.y0));
        })
        .attr("height", function(d) {
            return yScale(d.y);
        })
        .attr("width", barWidth)
        .on('mouseover', function(d, i) { showToolTip(d, i, this); })
        .on('mouseout', function(d, i) { hideToolTip(d, i, this); });
        
    
    /* LEGEND */
    d3.y = 0;
    var legend = svg.append("g")
      .attr("class", "legend");
    //.attr("transform", "translate(" + p[3] + "," + p[0] + ")")
	if(w>400){legend.attr("transform", "translate(" + (p[3] + chart.w + 5) + "," + 0 + ")");}
    else{legend.attr("transform", "translate( 10," + (chart.h+20) + ")");}

    var keys = legend.selectAll("g")
      .data(key)
      .enter().append("g")
      .attr("transform", function(d,i) { return "translate(0," + d3.tileText(d,15) + ")"});

    keys.append("rect")
      .attr("fill", function(d,i) { 
        if(colors!=null && typeof(colors[i])!='undefined'){
            return colors[i];
        }
        return d3.rgb(z(i)); 
      })
      .attr("width", 15)
      .attr("height", 15)
      .attr("y", 0)
      .attr("x", 0);

    var labelWrapper = keys.append("g");

    labelWrapper.selectAll("text")
      .data(function(d,i) { return d3.splitString(key[i], 15); })
      .enter().append("text")
      .text(function(d,i) { return d})
      .attr("x", 17)
      .attr("y", function(d,i) { return i * 20})
      .attr("dy", "1em");


    
    function showToolTip(d, i, obj) {
      // Change color and style of the bar.
      var bar = d3.select(obj);
      bar.attr('stroke', '#ccc')
        .attr('stroke-width', '1')
        .attr('opacity', '0.75');

      var group = d3.select(obj.parentNode);

      var tooltip = graph.append('g')
        .attr('class', 'd3-tooltip')
        // move to the x position of the parent group
        .attr('transform', function(data) { return group.attr('transform'); })
          .append('g')
        // now move to the actual x and y of the bar within that group
        .attr('transform', function(data) { return 'translate(' + (Number(bar.attr('x')) + barWidth) + ',' + (chart.h - yScale(d.y)) + ')'; });
      d3.tooltip(tooltip, d.y);
    }

    function hideToolTip(d, i, obj) {
      var group = d3.select(obj.parentNode);
      var bar = d3.select(obj);
      bar.attr('stroke-width', '0')
        .attr('opacity', 1);

      graph.select('g.d3-tooltip').remove();
    }
  }
})(jQuery);
