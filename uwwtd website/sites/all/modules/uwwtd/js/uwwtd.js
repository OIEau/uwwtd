(function ($) {
    $(document).ready(function() {
        // Gestion des flipcards
        var listFlip = $(".flip");
        //console.log(listFlip);
        if (listFlip.length > 0) {
            listFlip.flip({trigger: 'manual'});

            $(".flip .to-table").click(function() {
              $(this).closest('.flip').flip(true);
            });
            $(".flip .to-chart").click(function() {
              $(this).closest('.flip').flip(false);
            });

            $(".flip .chart-to-table").click(function() {
              $(this).closest('.flip').flip(false);
            });
            $(".flip .table-to-chart").click(function() {
              $(this).closest('.flip').flip(true);
            });
        }
    });
})(jQuery);


function uwwtd_labelformaterpie(label, series) {
    return "<div class=\"pie_slice\">" + Math.round(series.percent) + "%</div>";
}

function uwwtd_labelformaterpie_legend(label, series) {
    return "<div class=\"pie_legend\">" + label + " (" + Math.round(series.percent) + "%)</div>";
}

function display_uwwtp_stackedbar(data, color) {
    var margin = {top: 10, right: 20, bottom: 30, left: 60},
        width = 315 - margin.left - margin.right,
        height = 250 - margin.top - margin.bottom;
        widthsvg = width;

    var x = d3.scale.ordinal()
        .rangeRoundBands([0, width], .1);

    var y = d3.scale.linear()
        .rangeRound([height, 0]);


    var color = d3.scale.ordinal()
    .domain(color.domain)
    .range(color.range);

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
      .attr("width", 40)
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

    var widthsvg = 330;
    var width = 120;
    var height = 200;
    var radius = 50;
	var divid = 'agglo_piechart_back';
    var max_legend_label = 25;
    var innerRadius = 25;
	display_piechart(data,divid,widthsvg,width,height,innerRadius,radius,max_legend_label);
}
//TEST AP GRAPHS
function display_piechart_custom(data,divid) {
	var widthsvg = 500;
	var width = 200;
	var height = 200;
	var radius = 72;
    var innerRadius = 42;
    var max_legend_label = 42;
	display_piechart(data,divid,widthsvg,width,height,radius,innerRadius,max_legend_label);
}



function display_piechart(data,divid,widthsvg,width,height,radius,innerRadius,max_legend_label){
	var color = d3.scale.category10();
	var wedges = data;
	
	var svg = d3.select('#' + divid)
	.attr('width', widthsvg)
	.attr('height', height)
	.append('g')
	.attr('transform', 'translate(' + ((width / 2)+20) +  ',' + ((width / 2)+10) + ')');
	var arc = d3.svg.arc()
	.outerRadius(radius)
	.innerRadius(innerRadius);
	
	var circle = d3.svg.arc()
        .outerRadius(radius - 10)
        .innerRadius(radius - 10);

	var pie = d3.layout.pie()
	.value(function(d) { return d.value; })
	.sort(null);

	var g = svg.selectAll(".arc")
	.data(pie(data))
	.enter().append("g")
	.attr("class", "arc");

	g.append("path")
	.attr("d", arc)
	.style("fill", function(d) {return d.data.color;})
    .attr("title", function(d) {return d.data.label;})
    .attr("alt", function(d) {return d.data.label;})
	.on('mouseover', function(d, i) { interact('over', i); })
    .on('mouseout', function(d, i) { interact('out', i); });

	var pieLabels = g.append("text")
	//.attr("transform", function(d) { return "translate(" + (arc.centroid(d)+20) + ")"; })
	//.attr("transform", function(d) {var c = arc.centroid(d);return "translate(" + c[0]*2.3 +"," + c[1]*2.3 + ")";})
    .attr("transform", function(d) {
        var c = arc.centroid(d),
            x = c[0],
            y = c[1],
            // pythagorean theorem for hypotenuse
            h = Math.sqrt(x*x + y*y);
        return "translate(" + (x/h * (radius+12) ) +  ',' +
           (y/h * (radius+12)) +  ")"; 
    })
    .attr("dy", ".35em")
	.style("font-size", "90%")
	.style("text-anchor", "middle")
	.text(function(d) { return percent(d.data.value) });

	//modif nd@oieau.fr pour systeme d'anti collision des libellés
    var prev;
    var prev_radius = radius;
    pieLabels.each(function(d, i) {
      if(i > 0) {
        var thisbb = this.getBoundingClientRect(),
            prevbb = prev.getBoundingClientRect();
            
        // move if they overlap
        if(!(thisbb.right < prevbb.left || 
                thisbb.left > prevbb.right || 
                thisbb.bottom < prevbb.top || 
                thisbb.top > prevbb.bottom)) {
            d3.select(this).attr(
                "transform",
                "translate(" + Math.cos(((d.startAngle + d.endAngle - Math.PI) / 2)) *(prev_radius - 6.5) + "," +
                               Math.sin((d.startAngle + d.endAngle - Math.PI) / 2) * (prev_radius - 6.5) + ")"
            );
            prev_radius = prev_radius - 9;
        }
        else{
            prev_radius = radius;
        }
      }
      prev = this;
    });
	

	var legendRectSize = 12;
	var legendSpacing = 6;
    var vert_idx = 0;
	var legend = svg.selectAll('.legend')
	.data(data)
	.enter()
	  .append('g')
	    .attr('class', 'legend')
	    .attr('transform', function(d, i) {
            var height = legendRectSize + legendSpacing;
            var horz = radius + 30;
            var vert = vert_idx * height - radius;
            vert_idx ++;
            if(d.label.length >max_legend_label) vert_idx ++;
            return 'translate(' + horz + ',' + vert + ')';
        });
	legend.append('rect')
        .attr('width', legendRectSize)
        .attr('height', legendRectSize)
        .style('fill', function(d) {return d.color; })
        .style('stroke', '#aaaaaa');
    legend.append('text')
        .attr('x', legendRectSize + legendSpacing )
        .attr('y', legendRectSize - legendSpacing + 3)
        .attr('style', ' color: #4f4f4f;font-size: 12px;')
        .each(function (d) {
            if(d.label.length >max_legend_label){
                var str = wordwrap(d.label, max_legend_label, '|');
                var arr = str.split("|");
                if (arr != undefined) {
                    for (i = 0; i < arr.length; i++) {
                        d3.select(this)
                            .append("tspan")
                                .text(arr[i])
                                    .attr("dy", i ? "1.2em" : 0)
                                    .attr("x", legendRectSize + legendSpacing);
                    }
                }
            }
            else{
                d3.select(this).text(d.label);
            }
        });
	
	/**
     * Wrapper function for all rollover functions.
     *
     * @param string text
     *   Current state, 'over', or 'out'.
     * @param int i
     *   Current index of the current data row.
     * @return none
     */
    function interact(state, i) {
      if (state == 'over') {
        showToolTip(i);
        //highlightSlice(i);
      }
      else {
        hideToolTip(i);
        //unhighlightSlice(i);
      }
      return true;
    }

    /**
     * Displays a tooltip on the centroid of a pie slice.
     *
     * @param int i
     *   Index of the current data row.
     * @return none
     */
    function showToolTip(i) {
      var dat = pie(wedges);
      var tooltip = svg.append('g')
        .attr('class', 'd3-tooltip')
        // move to the x position of the parent group
          .append('g')
        // now move to the actual x and y of the bar within that group
        .attr('transform', function(d) { return 'translate(' + circle.centroid(dat[i]) + ')'; });
      d3.tooltip(tooltip, wedges[i].valueformat);
    }

    /**
     * Hides tooltip for a given class. Each slice has a unique class in
     * this chart.
     *
     * @param int i
     *   Index of the current data row.
     * @return none
     */
    function hideToolTip(i) {
      svg.select('g.d3-tooltip').remove();

    }
	function percent(i) {
      var sum = d3.sum(wedges.map(function(d,i) { return Number(d.value); }));

      return ((i / sum) ? Math.round((i / sum) * 100) : 0) + '%';
    }
}


function stackedbar_nat_gen_load(data) {
	var margin = {top: 10, right: 20, bottom: 30, left: 60},
	width = 500 - margin.left - margin.right,
	height = 220 - margin.top - margin.bottom;
	widthsvg = width;

	var x = d3.scale.ordinal()
	.rangeRoundBands([0, width], .1);

	var y = d3.scale.linear()
	.rangeRound([height, 0]);

	// Our color bands
	//var color = d3.scale.ordinal()
	//    .range(["#308fef", "#5fa9f3", "#1176db"]);
	var color = d3.scale.category10();
	// Use our X scale to set a bottom axis
	var xAxis = d3.svg.axis()
	.scale(x)
	.orient("bottom");

	// Same for our left axis
	var yAxis = d3.svg.axis()
	.scale(y)
	.orient("left")
	.tickFormat(d3.format(".2s"));


	var svg = d3.select("#graph_generatedByAgglomeration")
//	.attr("width", width + margin.left + margin.right)
	.attr("width", widthsvg + margin.left + margin.right)
	.attr("height", height + margin.top + margin.bottom)
	.append("g")
	.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

	// Map our columns to our colors
	color.domain(d3.keys(data[0]).filter(function (key) {
		return key !== "type";
	}));

	data.forEach(function (d) {
		var y0 = 0;
		d.types = color.domain().map(function (name) {
			return {
				name: name,
				y0: y0,
				y1: y0 += +d[name]
			};
		});
		d.total = d.types[d.types.length - 1].y1;
	});

	// Our X domain is our set of years
	x.domain(data.map(function (d) {
		return d.type;
	}));

	// Our Y domain is from zero to our highest total
	y.domain([0, d3.max(data, function (d) {
		return d.total;
	})]);

	svg.append("g")
	.attr("class", "x axis")
	.attr("transform", "translate(0," + height + ")")
	.attr('style', ' color: #4f4f4f;font--weight: bold;font-size: 9px;font-family: "Open Sans",sans-serif;')
	.call(xAxis)
    .selectAll("text") 
      .style("text-anchor", "end")
      .attr("transform", function(d) {return "rotate(-45)" ;});

	svg.append("g")
	.attr("class", "y axis")
	.attr('style', ' color: #4f4f4f;font--weight: bold;font-size: 9px;font-family: "Open Sans",sans-serif;')
	.call(yAxis)
	.append("text")
	.attr("transform", "rotate(-90)")
	.attr("y", -50)
	.attr("dy", ".35em")
	.style("text-anchor", "middle")

	var year = svg.selectAll(".type")
	.data(data)
	.enter().append("g")
	.attr("class", "g")
	.attr("transform", function (d) {
		return "translate(" + x(d.type) + ",0)";
	});

	year.selectAll("rect")
	.data(function (d) {
		return d.types;
	})
	.enter().append("rect")
	//.attr("width", x.rangeBand())
	.attr("width", 40)
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
	.attr('style', ' color: #4f4f4f;font--weight: bold;font-size: 9px;font-family: "Open Sans",sans-serif;')
	.style("text-anchor", "end")
	.text(function(d) { return d; });

}



function stackedbar_load_ent_and_dis(data,color,divid) {

	var margin = {top: 10, right: 20, bottom: 30, left: 60},
	width = 500 - margin.left - margin.right,
	height = 200 - margin.top - margin.bottom;
	widthsvg = width;

	var x = d3.scale.ordinal()
	.rangeRoundBands([0, width], .1);

	var y = d3.scale.linear()
	.rangeRound([height, 0]);

	var color = d3.scale.ordinal()
	.domain(color.domain)
	.range(color.range);

	var xAxis = d3.svg.axis()
	.scale(x)
	.orient("bottom");

	var yAxis = d3.svg.axis()
	.scale(y)
	.orient("left")
	.tickFormat(d3.format(".2s"));

	var svg = d3.select("#" + divid)
//	.attr("width", width + margin.left + margin.right)
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
	.attr("transform", "rotate(-45)")
	.attr("y", -50)
	.attr("dy", ".71em")
	.style("text-anchor", "end");

	var typeparam = svg.selectAll(".typeparam")
	.data(data)
	.enter().append("g")
	.attr("class", "g")
	.attr("transform", function(d) { return "translate(" + x(d.type) + ",0)"; });

	typeparam.selectAll("rect")
	.data(function(d) { return d.origin; })
	.enter().append("rect")
	//.attr("width", x.rangeBand())
	.attr("width", 40)
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

function wordwrap(str, int_width, str_break, cut) {
  //  discuss at: http://phpjs.org/functions/wordwrap/
  // original by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
  // improved by: Nick Callen
  // improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // improved by: Sakimori
  //  revised by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
  // bugfixed by: Michael Grier
  // bugfixed by: Feras ALHAEK
  //   example 1: wordwrap('Kevin van Zonneveld', 6, '|', true);
  //   returns 1: 'Kevin |van |Zonnev|eld'
  //   example 2: wordwrap('The quick brown fox jumped over the lazy dog.', 20, '<br />\n');
  //   returns 2: 'The quick brown fox <br />\njumped over the lazy<br />\n dog.'
  //   example 3: wordwrap('Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.');
  //   returns 3: 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod \ntempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim \nveniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea \ncommodo consequat.'

  var m = ((arguments.length >= 2) ? arguments[1] : 75);
  var b = ((arguments.length >= 3) ? arguments[2] : '\n');
  var c = ((arguments.length >= 4) ? arguments[3] : false);

  var i, j, l, s, r;

  str += '';

  if (m < 1) {
    return str;
  }

  for (i = -1, l = (r = str.split(/\r\n|\n|\r/))
    .length; ++i < l; r[i] += s) {
    for (s = r[i], r[i] = ''; s.length > m; r[i] += s.slice(0, j) + ((s = s.slice(j))
      .length ? b : '')) {
      j = c == 2 || (j = s.slice(0, m + 1)
        .match(/\S*(\s)?$/))[1] ? m : j.input.length - j[0].length || c == 1 && m || j.input.length + (j = s.slice(
          m)
        .match(/^\S*/))[0].length;
    }
  }

  return r.join('\n');
}
