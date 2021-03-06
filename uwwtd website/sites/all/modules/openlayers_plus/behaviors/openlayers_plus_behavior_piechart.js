/**
 * Implementation of Drupal behavior.
 */
(function ($) {
	//==========================Override Openlayers constant and function 
	//Add the piechart renderer symbol
	OpenLayers.Renderer.symbol['piechart'] = [0,10, 10,10, 5,0, 0,10]; //add dummy value just for pass some validation test
	
	//Override the setStyle function of OpenLayers.Renderer.SVG
	OpenLayers.Renderer.SVG.prototype.setStyle = function(node, style, options) {

        style = style  || node._style;
        options = options || node._options;
        var r = parseFloat(node.getAttributeNS(null, "r"));
        var widthFactor = 1;
        var pos;
        if (node._geometryClass == "OpenLayers.Geometry.Point" && r) {
            node.style.visibility = "";
            
            if (style.graphic === false) {
                node.style.visibility = "hidden";
            } 
            else if (style.externalGraphic) {
                pos = this.getPosition(node);
                
                if (style.graphicTitle) {
                    node.setAttributeNS(null, "title", style.graphicTitle);
                    //Standards-conformant SVG
                    // Prevent duplicate nodes. See issue https://github.com/openlayers/openlayers/issues/92 
                    var titleNode = node.getElementsByTagName("title");
                    if (titleNode.length > 0) {
                        titleNode[0].firstChild.textContent = style.graphicTitle;
                    } else {
                        var label = this.nodeFactory(null, "title");
                        label.textContent = style.graphicTitle;
                        node.appendChild(label);
                    }
                }
                if (style.graphicWidth && style.graphicHeight) {
                  node.setAttributeNS(null, "preserveAspectRatio", "none");
                }
                var width = style.graphicWidth || style.graphicHeight;
                var height = style.graphicHeight || style.graphicWidth;
                width = width ? width : style.pointRadius*2;
                height = height ? height : style.pointRadius*2;
                var xOffset = (style.graphicXOffset != undefined) ?
                    style.graphicXOffset : -(0.5 * width);
                var yOffset = (style.graphicYOffset != undefined) ?
                    style.graphicYOffset : -(0.5 * height);

                var opacity = style.graphicOpacity || style.fillOpacity;
                
                node.setAttributeNS(null, "x", (pos.x + xOffset).toFixed());
                node.setAttributeNS(null, "y", (pos.y + yOffset).toFixed());
                node.setAttributeNS(null, "width", width);
                node.setAttributeNS(null, "height", height);
                node.setAttributeNS(this.xlinkns, "href", style.externalGraphic);
                node.setAttributeNS(null, "style", "opacity: "+opacity);
                node.onclick = OpenLayers.Renderer.SVG.preventDefault;
            }	
            			//here we add a special case for piechart rendering
			else if(style.graphicName=="piechart" && typeof style.piechartdata.data.data != 'undefined'){
				//Note : for each adding svg element, we need to set the "_featureId"  properties for allow events on feature and for eg fire a "popup"
				var size = style.pointRadius * 2;
				pos = this.getPosition(node);
                if (node.getAttribute("treated") != 1 ) {
                    var data=style.piechartdata.data.data;
                    /////////////////////////////////////////////////////
//     				var arc = d3.svg.arc()
//     					.outerRadius(style.pointRadius)
//     					.innerRadius(0);
//     				var pie = d3.layout.pie()
//     					.sort(null)
//     					.value(function(d) { return d.value; });
//     				var svg = d3.select(node)
//     					.append("g")
//     					.property('_featureId', node._featureId)
//     					.attr("transform", "translate(" + size / 2 + "," + size / 2 + ")");
//     				data.forEach(function(d) {
//     					d.value = +d.value;
//     				});
//     				var g = svg.selectAll(".arc")
//     					  .data(pie(data))
//     					.enter().append("g")
//     					  .property('_featureId', node._featureId)
//     					  .attr("class", "arc");
//     					  
//     				g.append("path")
//     				  .property('_featureId', node._featureId)
//     				  .attr("d", arc)
//     				  .style("fill", function(d) { return d.data.color; })
//     				  .style("stroke", function(d) { return style.strokeColor; })
//     				  .style("opacity", function(d) { return style.fillOpacity; })
//     				  .style("stroke-width", function(d) { return style.strokeWidth; });
                    /////////////////////////////////////////////////////
    				var parent = node.parentNode;
                    var nextSibling = node.nextSibling;
                    if (parent) { 
                        parent.removeChild(node);
                    }
                    var piechartnode = null;
                    //Piechart is provide in feature definition
                    if(typeof data.piechart != 'undefined'){
                        piechartnode = $(data.piechart)[0];
                        piechartnode.setAttributeNS(null, "style", "display:block;");
                    }
                    //Piechart is provide in the DOM of page
                    else{
                        piechartnode = document.getElementById(style.piechartdata.data.piechartid).cloneNode(true);
                        piechartnode.setAttributeNS(null, "style", "display:block;");
                    }
                    
                    //If we have find a piechart
                    if(piechartnode){
                        for (var i = 0; i <  piechartnode.childNodes.length;i++){
                            piechartnode.childNodes[i]._featureId = node._featureId;
                        }   
                                     
                        node.appendChild(piechartnode);
                    }
                    node.setAttributeNS(null, "viewBox", "0 0 "+size+" "+size);
                    node.setAttributeNS(null, "width", size);
                    node.setAttributeNS(null, "height", size);
                    node.setAttributeNS(null, "treated", '1');  
                }    
                /////////////////////////////////////////////////////
				node.setAttributeNS(null, "x", pos.x - style.pointRadius);
				node.setAttributeNS(null, "y", pos.y - style.pointRadius);
                
				// now that the node has all its new properties, insert it
                // back into the dom where it was
                if(nextSibling) {
                    parent.insertBefore(node, nextSibling);
                } else if(parent) {
                    parent.appendChild(node);
                }
			}
			else if (this.isComplexSymbol(style.graphicName)) {
				// the symbol viewBox is three times as large as the symbol
				var offset = style.pointRadius * 3;
				var size = offset * 2;
				pos = this.getPosition(node);
				var src = this.importSymbol(style.graphicName);
				widthFactor = this.symbolMetrics[src.id][0] * 3 / size;
				
                // remove the node from the dom before we modify it. This
                // prevents various rendering issues in Safari and FF
                var parent = node.parentNode;
                var nextSibling = node.nextSibling;
                if(parent) {
                    parent.removeChild(node);
                }
                
                // The more appropriate way to implement this would be use/defs,
                // but due to various issues in several browsers, it is safer to
                // copy the symbols instead of referencing them. 
                // See e.g. ticket http://trac.osgeo.org/openlayers/ticket/2985 
                // and this email thread
                // http://osgeo-org.1803224.n2.nabble.com/Select-Control-Ctrl-click-on-Feature-with-a-graphicName-opens-new-browser-window-tc5846039.html
                node.firstChild && node.removeChild(node.firstChild);
                node.appendChild(src.firstChild.cloneNode(true));
                node.setAttributeNS(null, "viewBox", src.getAttributeNS(null, "viewBox"));
                
                node.setAttributeNS(null, "width", size);
                node.setAttributeNS(null, "height", size);
                node.setAttributeNS(null, "x", pos.x - offset);
                node.setAttributeNS(null, "y", pos.y - offset);
                
                // now that the node has all its new properties, insert it
                // back into the dom where it was
                if(nextSibling) {
                    parent.insertBefore(node, nextSibling);
                } else if(parent) {
                    parent.appendChild(node);
                }
            } else {
                node.setAttributeNS(null, "r", style.pointRadius);
            }

            var rotation = style.rotation;
            
            if ((rotation !== undefined || node._rotation !== undefined) && pos) {
                node._rotation = rotation;
                rotation |= 0;
                if (node.nodeName !== "svg") { 
                    node.setAttributeNS(null, "transform", 
                        "rotate(" + rotation + " " + pos.x + " " + 
                        pos.y + ")"); 
                } else {
                    var metrics = this.symbolMetrics[src.id];
                    node.firstChild.setAttributeNS(null, "transform", "rotate(" 
                        + rotation + " " 
                        + metrics[1] + " "
                        + metrics[2] + ")");
                }
            }
        }
        
        if (options.isFilled) {
            node.setAttributeNS(null, "fill", style.fillColor);
            if (node.getAttribute('cx') == null) {
                node.setAttributeNS(null, "fill-opacity", 0.3);
            }
            else {
                node.setAttributeNS(null, "fill-opacity", style.fillOpacity);
            }
        } else {
            node.setAttributeNS(null, "fill", "none");
        }

        if (options.isStroked) {
            node.setAttributeNS(null, "stroke", style.strokeColor);
            node.setAttributeNS(null, "stroke-opacity", style.strokeOpacity);
            node.setAttributeNS(null, "stroke-width", style.strokeWidth * widthFactor);
            node.setAttributeNS(null, "stroke-linecap", style.strokeLinecap || "round");
            // Hard-coded linejoin for now, to make it look the same as in VML.
            // There is no strokeLinejoin property yet for symbolizers.
            node.setAttributeNS(null, "stroke-linejoin", "round");
            style.strokeDashstyle && node.setAttributeNS(null,
                "stroke-dasharray", this.dashStyle(style, widthFactor));
        } else {
            node.setAttributeNS(null, "stroke", "none");
        }
        
        if (style.pointerEvents) {
            node.setAttributeNS(null, "pointer-events", style.pointerEvents);
        }
                
        if (style.cursor != null) {
            node.setAttributeNS(null, "cursor", style.cursor);
        }
        
        return node;
    };
	
	
  Drupal.behaviors.openlayers_plus_behavior_piechart = {
      attach: function(context) {
        var data = $(context).data('openlayers');
        if (data && data.map.behaviors.openlayers_plus_behavior_piechart) {
          var styles = data.map.behaviors.openlayers_plus_behavior_piechart.styles;
          //var series = data.map.behaviors.openlayers_plus_behavior_piechart.series;
		  
          // Collect vector layers
          var vector_layers = [];
          for (var key in data.openlayers.layers) {
            var layer = data.openlayers.layers[key];
			
			if(layer.CLASS_NAME=="OpenLayers.Layer.Vector"){
				var styleMap = layer.styleMap;
				if(styleMap != undefined) {	
                    
					styleMap.createSymbolizer = function(feature, intent){
                        //console.log(feature);
						if(!feature) {
							feature = new OpenLayers.Feature.Vector();
						}
						if(!this.styles[intent]) {
							intent = "default";
						}
						
						feature.renderIntent = intent;
						var defaultSymbolizer = {};
						if(this.extendDefault && intent != "default") {
							defaultSymbolizer = this.styles["default"].createSymbolizer(feature);
						}
						var symbol = OpenLayers.Util.extend(defaultSymbolizer, this.styles[intent].createSymbolizer(feature));
						symbol.piechartdata = feature;
                        var styles_idx = 1;
                        if(feature.data.weight){styles_idx = feature.data.weight;}
						symbol.pointRadius = styles[styles_idx].pointRadius;
						symbol.fillOpacity = styles[styles_idx].fillOpacity;
						symbol.strokeWidth = styles[styles_idx].strokeWidth;
						symbol.graphic=true;
						symbol.graphicName = "piechart";
						symbol.pointerEvents = 'visiblePainted';
						/* //bad idea
						symbol.label = feature.data.name; 
						symbol.labelSelect =true;
						symbol.fontSize = 6;
						*/
						return symbol;
					};
					
				  layer.redraw();
				  vector_layers.push(layer);
				}
            }
		  }
          /**
           * This attempts to fix a problem in IE7 in which points
           * are not displayed until the map is moved. 
           *
           * Since namespaces is filled neither on window.load nor
           * document.ready, and testing it is unsafe, this renders
           * map layers after 500 milliseconds.
           */
          if($.browser.msie) {
            setTimeout(function() {
              $.each(data.openlayers.getLayersByClass('OpenLayers.Layer.Vector'),
                  function() {
                this.redraw();
              });
            }, 500);
          }
        }
      }
  };
})(jQuery);
