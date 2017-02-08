/**
 * Implementation of Drupal behavior.
 */

(function($) {
   
  $(document).ready(function() {
    //Display the block content
    $('.openlayers_plus-blockswitcher').show();
    
    // Init.
    if($("div.openlayers_plus-blockswitcher .layer-switcher").length > 0 && $(".toggle-button-layerswitcher").html() == "[ + ]") {
       $(".toggle-button-layerswitcher").html("[ - ]");
     }
     $('.layer-switcher fieldset legend').prepend('<span class="fieldset-toggle"> - </span>');

    // Custom fieldset handler.
     $('.layer-switcher fieldset legend').on('click', function(e) {
        var toggleButton = $(this).find('.fieldset-toggle');
        if(toggleButton.html() == ' - ') {
          toggleButton.html(' + ');
          $(this).parent().find('.fieldset-wrapper').hide('slow');
        } else {
          toggleButton.html(' - ');
          $(this).parent().find('.fieldset-wrapper').show('slow');
        }
     });
  });

  /**
   * Blockswitcher is **NOT** an OpenLayers control.
   */
  Drupal.behaviors.OpenLayersPlusBlockswitcherPlus = {};

  /**
   * Initializes the */
  Drupal.behaviors.OpenLayersPlusBlockswitcherPlus = {
      'attach' : function(context, settings) {
        /**
         * Stores layer visibility in a cookie string
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.setLayers  = function() {
          var len = this.map.layers.length;
          //layerVisibility = new Array(len);
          var cookie_string = "";
          for (var i = 0; i < len; i++) {
            var layer = this.map.layers[i];
            if (layer.displayInLayerSwitcher) {
              cookie_string = cookie_string + layer.drupalID + ":" + layer.visibility+ ":" + layer.opacity;
              if(i <= len - 2) {
                cookie_string = cookie_string + ",";
              } else if (i == len - 1) {
                cookie_string = cookie_string;
              }
            }
          }
          //Disable this function for debug
          //$.cookie('blockswitcher', cookie_string);
        }

        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.toggleLayerSwitcher = function() {
            if($(".toggle-button-layerswitcher").html() == "[ - ]") {
              $(".toggle-button-layerswitcher").html("[ + ]");
              $("div.openlayers_plus-blockswitcher .layer-switcher").hide("slow");
              $("div.openlayers_plus-blockswitcher").css('width', 'auto');
              $("div.openlayers_plus-blockswitcher").css('height', 'auto');
            } else {
              $(".toggle-button-layerswitcher").html("[ - ]");
              $("div.openlayers_plus-blockswitcher .layer-switcher").show("slow");
              $("div.openlayers_plus-blockswitcher").css('width', '250px');
              $("div.openlayers_plus-blockswitcher").css('height', '400px');
            }
          };

        /**
         * Retrieves layer visibility from cookie
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.getLayers  = function() { 
          if ($.cookie('blockswitcher') == null) {
            return;
          }
          var cookie_array = $.cookie('blockswitcher').split(',');
          var temp = new Array();

          var layerSettings = new Array();
          if (cookie_array != undefined) {
            for(var i = 0; i < cookie_array.length; i++) {
              temp = cookie_array[i].split(":");
              layerSettings[temp[0]] ={
                'visibility':(temp[1] == "true"),
                'opacity' : temp[2]
              };
            }
            for(var i = 0; i < this.map.layers.length; i++) {
              visible = layerSettings[this.map.layers[i].drupalID].visibility;
              opacity = layerSettings[this.map.layers[i].drupalID].opacity;
              if (this.map.layers[i].isBaseLayer) {
                if ((visible) && (this.map.baseLayer.id != this.map.layers[i].id)) {
                  this.map.setBaseLayer(this.map.layers[i]);
                }
              }
              else {
                if (this.map.layers[i].displayInLayerSwitcher) {
                  if (this.map.layers[i].visibility != visible) {
                    this.map.layers[i].setVisibility(visible);
                    this.map.layers[i].setOpacity(opacity);
                    
                  }
                }
              }
            }
          }
        }

        /**
         * Checks if the layer state has changed since the last redraw() call.
         *
         * Returns:
         * {Boolean} The layer state changed since the last redraw() call.
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.needsRedraw  = function() {
          if (!this.layerStates.length || (this.map.layers.length != this.layerStates.length)) {
            return true;
          }
          for (var i = 0, len = this.layerStates.length; i < len; i++) {
            var layerState = this.layerStates[i];
            var layer = this.map.layers[i];
            if ((layerState.name != layer.name) || (layerState.inRange != layer.inRange) || (layerState.id != layer.id) || (layerState.visibility != layer.visibility)) {
              return true;
            }
          }
          return false;
        };

        /**
         * Redraws the blockswitcher to reflect the current state of layers.
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.redraw = function() {
          if (this.needsRedraw()) {

            // Clear out previous layers
            $('.layers.base .layers-content div', this.blockswitcher).remove();
            $('.layers.groups .layers-content div', this.blockswitcher).remove();
            $('.layers.base', this.blockswitcher).hide();
            $('.layers.groups', this.blockswitcher).hide();

            // Save state -- for checking layer if the map state changed.
            // We save this before redrawing, because in the process of redrawing
            // we will trigger more visibility changes, and we want to not redraw
            // and enter an infinite loop.
            var len = this.map.layers.length;
            this.layerStates = new Array(len);
            for (var i = 0; i < len; i++) {
              var layerState = this.map.layers[i];
              this.layerStates[i] = {'name': layerState.name, 'visibility': layerState.visibility, 'inRange': layerState.inRange, 'id': layerState.id, 'opacity':layerState.opacity};
            }
            //console.log(this.layerStates);
            
            
            // Retrieve groups configuration.
            var groups = Drupal.settings.OpenLayersPlusBlockswitcherPlus;

            var layers = this.map.layers.slice();
            for (i = 0, len = layers.length; i < len; i++) {
              var layer = layers[i];
              var baseLayer = layer.isBaseLayer;
              if (layer.displayInLayerSwitcher) {
                // Only check a baselayer if it is *the* baselayer, check data layers if they are visible
                var checked = baseLayer ? (layer === this.map.baseLayer) : layer.getVisibility();

                // Create input element
                var inputType = (baseLayer) ? "radio" : this.overlay_style;

                var inputElem = $('.factory .' + inputType, this.blockswitcher).clone();

                // Append to container
                var container = baseLayer ? $('.layers.base', this.blockswitcher) : $('.layers.groups', this.blockswitcher);
                var j = 1;
                var groupID = 0;
                for(groupName in groups) {
                  if(groups[groupName].indexOf(layer.drupalID) >= 0) {
                    groupID = j;
                    break;
                  }
                  j++;
                }

                // If group was found, add layer to DOM.
                if(groupID > 0) {
                  // Create group if it doesn't exist.
                  if(!$('.layers.groups .layers-content .group-' + j).length) {
                    $('.layers.groups .layers-content').append('<div class="group group-' + groupID + '">' + groupName + '</div>');
                  }
                  $('.layers-content .group-' + groupID, container).append(inputElem);
                } else {
                  $('.layers-content', container).append(inputElem);
                }

                // Set label text
                $('label', inputElem).append((layer.title !== undefined) ? layer.title : layer.name);
                
                
                

                //Give specific class to layer checkbox or radio
                inputElem.addClass(layer.id);

                // Add states and click handler
                if (baseLayer) {
                  $(inputElem)
                  .click(function() { Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.layerClick(this);})
                  .data('layer', layer);
                  if (checked) {
                    $(inputElem).addClass('activated');
                  }
                }
                //Case of overlay layer
                else {
                    
                  // Set slider element
                  // data-layer-name="'+layer.name+'" data-layer-id="'+layer.id+'"
                    var layer_slider = $('<div class="ol-layer-slider slider"></div>');
                    layer_slider.data('layer', layer);
                    var cursor = $('<div class="ui-slider-handle" id="ui-slider-handle-'+layer.id+'"></div>');
                    layer_slider.append(cursor);    
                    $(inputElem).append(layer_slider);
                    $(layer_slider).slider({
                        'min':0,
                        'max':100,
                        'value': layer.opacity?(layer.opacity*100):100,
                        'change': function(event, ui) {
                            var opacity = ui.value/100;
                            Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.layerOpacity(this, opacity);                            
                        },
                        'create': function() {
                            var l = $(this).data('layer');
                            var handle = $('#ui-slider-handle-'+l.id);
                            handle.text( $( this ).slider( "value" ) );
                        },
                        'slide':function(event, ui){
                            var l = $(this).data('layer');
                            var handle = $('#ui-slider-handle-'+l.id);
                            handle.text( ui.value );
                        }
                    });  
                    
                  if (this.overlay_style == 'checkbox') {
                    $('input', inputElem)
                    .click(function() { Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.layerClick(this); })
                    .data('layer', layer)
                    .attr('disabled', !baseLayer && !layer.inRange)
                    .attr('checked', checked);
                  }
                  else if(this.overlay_style == 'radio') {
                    $(inputElem)
                    .click(function() { Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.layerClick(this); })
                    .data('layer', layer)
                    .attr('disabled', !layer.inRange);
                    if (checked) {
                      $(inputElem).addClass('activated');
                    }
                  }
                  // Set key styles
                  if (layer.styleMap) {
                    css = Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.styleMapToCSS(layer.styleMap);
                    $('span.key', inputElem).css(css);
                  }
                }
              }
            }
            /*
             *  Only show layers when there are more than 1 for baselayers, or 1 for data layers
             */
            if ($('.layers.base .layers-content div', this.blockswitcher).length > 1) {
              $('.layers.base', this.blockswitcher).show();
            }
            if ($('.layers.groups .layers-content div', this.blockswitcher).length > 0) {
              $('.layers.groups', this.blockswitcher).show();
            }
          }
        };
        
        /**
        * Change the opacity of a layer
        */
        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.layerOpacity = function(element, opacity) {            
            var layer = $(element).data('layer');
            layer.setOpacity(opacity);
            Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.setLayers()            
        };
        
        /**
         * Click handler that activates or deactivates a layer.
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.layerClick = function(element) {
          var layer = $(element).data('layer');
          if (layer.isBaseLayer) {
            $('.layers.base .layers-content .activated').removeClass('activated');
            $(element).addClass('activated');
            layer.map.setBaseLayer(layer);
          }
          else if (this.overlay_style == 'radio') {
            $('.layers.groups .layers-content .activated').removeClass('activated');
            $.each(this.map.getLayersBy('isBaseLayer', false),
                function() {
              if(this.CLASS_NAME !== 'OpenLayers.Layer.Vector.RootContainer' &&
                  this.displayInLayerSwitcher) {
                this.setVisibility(false);
              }
            }
            );
            layer.setVisibility(true);
            $(element).addClass('activated');
          }
          else {
            layer.setVisibility($(element).is(':checked'));
          }
          Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.setLayers()
        };

        /**
         * Parameters:
         * styleMap {OpenLayers.StyleMap}
         *
         * Returns:
         * {Object} An object with css properties and values that can be applied to an element
         *
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.styleMapToCSS = function(styleMap) {
          css = {};
          default_style = styleMap.styles['default'].defaultStyle;
          if (default_style.fillColor === 'transparent' && typeof default_style.externalGraphic != 'undefined') {
            css['background-image'] = 'url(' + default_style.externalGraphic + ')';
            css['background-repeat'] = 'no-repeat';
            css['background-color'] = 'transparent';
            css.width = default_style.pointRadius * 2;
            css.height = default_style.pointRadius * 2;
          }
          else {
             css['background-color'] = default_style.fillColor;
          }
          if (default_style.strokeWidth > 0) {
             css['border-color'] = default_style.strokeColor;
          }
          return css;
        };

        /**
         * Now we implement the actual behaviour
         */
        this.layerStates = [];

        var data = $(context).data('openlayers');
        if (data == undefined) {
          for (var prop in settings.openlayers.maps) {
            data = settings.openlayers.maps[prop];
            //always get the first map
            continue;
          }
        }
        if (data && (data.map != undefined) && (data.map.behaviors.openlayers_plus_behavior_blockswitcher_plus != undefined)) {
          this.map = data.openlayers;
          this.overlay_style = (data.map.behaviors.openlayers_plus_behavior_blockswitcher_plus.overlay_style) ?
              data.map.behaviors.openlayers_plus_behavior_blockswitcher_plus.overlay_style : 'checkbox';

          
         this.blockswitcher = $('div.openlayers_plus-blockswitcher');
         
         
          // Don't propagate click events to the map
          // this doesn't catch events that are below the layer list
          $('div.openlayers_plus-blockswitcher').mousedown(function(evt) {
            OpenLayers.Event.stop(evt);
          });

          data.openlayers.events.on({
            "addlayer": this.redraw,
            "changelayer": this.redraw,
            "removelayer": this.redraw,
            "changebaselayer": this.redraw,
            scope: this
          });
          // Get the original layerstates from the cookie
          //disbale cookie
          //this.getLayers();
          this.redraw();
          
          
          //$('#openlayers-map').append(this.blockswitcher); ==> add in the map
          var map = $(this.map.div);
          //$('#openlayers-map').after(this.blockswitcher);
          $(map.parent()).after(this.blockswitcher);
          //this.blockswitcher.show()
          
        }
      }
  };
})(jQuery);
