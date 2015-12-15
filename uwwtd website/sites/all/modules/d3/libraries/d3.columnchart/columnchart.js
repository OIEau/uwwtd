/**
 * @file
 * Adds a function to generate a column chart to the `Drupal` object.
 */

(function($) {

  Drupal.d3.columnchart = function (select, settings) {

    var rows = settings.rows,
      // Use first value in each row as the label.
      xLabels = rows.map(function(d) { return d.shift(); })
      key = settings.legend,
      // From inside out:
      // - Convert all values to numeric numbers.
      // - Merge all sub-arrays into one flat array.
      // - Return the highest (numeric) value from flat array.
      max = d3.max(d3.merge(settings.rows).map(function(d) { return + d; })),
      // Padding is top, right, bottom, left as in css padding.
      p = [20, 50, 30, 60],
      w = settings.width?settings.width:800,
      h = settings.height?settings.height:400,
	  chart =(w>400? {w: w * .65, h: h * .70}:{w: w*0.8, h: h * .70}),
	  legend =(w>400? {w: w * .35, h:h}:{w: w, h:h*0.3}),
      // bar width is calculated based on chart width, and amount of data
      // items - will resize if there is more or less
      barWidth = ((.90 * chart.w) / (rows.length * key.length)),
      // each cluster of bars - makes coding later easier
      barGroupWidth = (key.length * barWidth),
      // space in between each set
      barSpacing = (.10 * chart.w) / rows.length,
      x = d3.scale.linear().domain([0,rows.length]).range([0,chart.w]),
      y = d3.scale.linear().domain([0,max]).range([chart.h, 0]),
      z = d3.scale.ordinal().range(["blue", "red", "orange", "green"]),
      div = (settings.id) ? settings.id : 'visualization';
	console.log(w);
    var svg = d3.select('#' + div).append("svg")
      .attr("width", w)
      .attr("height", h)
      .append("g")
      .attr("transform", "translate(" + p[3] + "," + p[0] + ")");

    var graph = svg.append("g")
      .attr("class", "chart");

    /* X AXIS  */
    var xTicks = graph.selectAll("g.ticks")
      .data(rows)
      .enter().append("g")
      .attr("class","ticks")
      .attr('transform', function(d,i) { return 'translate(' + (x(i) + (barGroupWidth / 2)) + ',' + (chart.h) + ')'})
      .append("text")
      .attr("dy", ".71em")
      .attr("text-anchor", "end")
      .attr('transform', function(d,i) { return "rotate(-15)"; })
      .text(function(d,i){ return xLabels[i]; });

    /* LINES */
    var rule = graph.selectAll("g.rule")
      .data(y.ticks(4))
      .enter().append("g")
      .attr("class", "rule")
      .attr("transform", function(d) { return "translate(0," + y(d) + ")"; });

    rule.append("line")
      .attr("x2", chart.w)
      .style("stroke", function(d) { return d ? "#ccc" : "#000"; })
      .style("stroke-opacity", function(d) { return d ? .7 : null; });

    /* Y AXIS */
    rule.append("text")
      .attr("x", -15)
      .attr("dy", ".35em")
      .attr("text-anchor", "end")
      .text(d3.format(",d"));

    var bar = graph.selectAll('g.bars')
      .data(rows)
      .enter().append('g')
      .attr('class', 'bargroup')
      .attr('transform', function(d,i) { return "translate(" + i * (barGroupWidth + barSpacing) + ", 0)"; });

    bar.selectAll('rect')
      .data(function(d) { return d; })
      .enter().append('rect')
      .attr("width", barWidth)
      .attr("height", function(d) { return chart.h - y(d); })
      .attr('x', function (d,i) { return i * barWidth; })
      .attr('y', function (d,i) { return y(d); })
      .attr('fill', function(d,i) { return d3.rgb(z(i)); })
      .on('mouseover', function(d, i) { showToolTip(d, i, this); })
      .on('mouseout', function(d, i) { hideToolTip(d, i, this); });

    /* LEGEND */
    var legend = svg.append("g")
      .attr("class", "legend");
	if(w>400){legend.attr("transform", "translate(" + (chart.w + 10) + "," + 0 + ")");}
    else{legend.attr("transform", "translate( 10," + (chart.h+20) + ")");}

	
    var keys = legend.selectAll("g")
      .data(key)
      .enter().append("g")
      .attr("transform", function(d,i) { return "translate(0," + d3.tileText(d,15) + ")"});

    keys.append("rect")
      .attr("fill", function(d,i) { return d3.rgb(z(i)); })
      .attr("width", 16)
      .attr("height", 16)
      .attr("y", 0)
      .attr("x", 0);

    var labelWrapper = keys.append("g");

    labelWrapper.selectAll("text")
      .data(function(d,i) { return d3.splitString(key[i], 15); })
      .enter().append("text")
      .text(function(d,i) { return d})
      .attr("x", 20)
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
        .attr('transform', function(data) { return 'translate(' + (Number(bar.attr('x')) + barWidth) + ',' + y(d) + ')'; });
      d3.tooltip(tooltip, d);
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
