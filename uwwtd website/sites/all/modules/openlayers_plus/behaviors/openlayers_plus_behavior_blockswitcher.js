/**
 * Implementation of Drupal behavior.
 */

(function($) {
  /**
   * Blockswitcher is **NOT** an OpenLayers control.
   */
  Drupal.behaviors.OpenLayersPlusBlockswitcher = {};

  /**
   * Initializes the */
  Drupal.behaviors.OpenLayersPlusBlockswitcher = {
      'attach' : function(context, settings) {
        /**
         * Stores layer visibility in a cookie string
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcher.setLayers  = function() {
          var len = this.map.layers.length;
          //layerVisibility = new Array(len);
          var cookie_string = "";
          for (var i = 0; i < len; i++) {
            var layer = this.map.layers[i];
            if (layer.displayInLayerSwitcher) {
              cookie_string = cookie_string + layer.drupalID + ":" + layer.visibility;
              if(i <= len - 2) {
                cookie_string = cookie_string + ",";
              } else if (i == len - 1) {
                cookie_string = cookie_string;
              }
            }
          }
          $.cookie('blockswitcher', cookie_string);
        }

        /**
         * Retrieves layer visibility from cookie
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcher.getLayers  = function() {
          if ($.cookie('blockswitcher') == null) {
            return;
          }
          var cookie_array = $.cookie('blockswitcher').split(',');
          var temp = new Array();

          var layerSettings = new Array();
          if (cookie_array != undefined) {
            for(var i = 0; i < cookie_array.length; i++) {
              temp = cookie_array[i].split(":");
              layerSettings[temp[0]] = (temp[1] == "true");
            }
            for(var i = 0; i < this.map.layers.length; i++) {
              visible = layerSettings[this.map.layers[i].drupalID];
              if (this.map.layers[i].isBaseLayer) {
                if ((visible) && (this.map.baseLayer.id != this.map.layers[i].id)) {
                  this.map.setBaseLayer(this.map.layers[i]);
                }
              }
              else {
                if (this.map.layers[i].displayInLayerSwitcher) {
                  if (this.map.layers[i].visibility != visible) {
                    this.map.layers[i].setVisibility(visible);
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
        Drupal.behaviors.OpenLayersPlusBlockswitcher.needsRedraw  = function() {
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
        Drupal.behaviors.OpenLayersPlusBlockswitcher.redraw = function() {
          if (this.needsRedraw()) {

            // Clear out previous layers
            $('.layers.base .layers-content div', this.blockswitcher).remove();
            $('.layers.data .layers-content div', this.blockswitcher).remove();
            $('.layers.base', this.blockswitcher).hide();
            $('.layers.data', this.blockswitcher).hide();

            // Save state -- for checking layer if the map state changed.
            // We save this before redrawing, because in the process of redrawing
            // we will trigger more visibility changes, and we want to not redraw
            // and enter an infinite loop.
            var len = this.map.layers.length;
            this.layerStates = new Array(len);
            for (var i = 0; i < len; i++) {
              var layerState = this.map.layers[i];
              this.layerStates[i] = {'name': layerState.name, 'visibility': layerState.visibility, 'inRange': layerState.inRange, 'id': layerState.id};
            }

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
                var container = baseLayer ? $('.layers.base', this.blockswitcher) : $('.layers.data', this.blockswitcher);
                $('.layers-content', container).append(inputElem);

                // Set label text
                $('label', inputElem).append((layer.title !== undefined) ? layer.title : layer.name);

                //Give specific class to layer checkbox or radio
                inputElem.addClass(layer.id);

                // Add states and click handler
                if (baseLayer) {
                  $(inputElem)
                  .click(function() { Drupal.behaviors.OpenLayersPlusBlockswitcher.layerClick(this);})
                  .data('layer', layer);
                  if (checked) {
                    $(inputElem).addClass('activated');
                  }
                }
                else {
                  if (this.overlay_style == 'checkbox') {
                    $('input', inputElem)
                    .click(function() { Drupal.behaviors.OpenLayersPlusBlockswitcher.layerClick(this); })
                    .data('layer', layer)
                    .attr('disabled', !baseLayer && !layer.inRange)
                    .attr('checked', checked);
                  }
                  else if(this.overlay_style == 'radio') {
                    $(inputElem)
                    .click(function() { Drupal.behaviors.OpenLayersPlusBlockswitcher.layerClick(this); })
                    .data('layer', layer)
                    .attr('disabled', !layer.inRange);
                    if (checked) {
                      $(inputElem).addClass('activated');
                    }
                  }
                  // Set key styles
                  if (layer.styleMap) {
                    css = Drupal.behaviors.OpenLayersPlusBlockswitcher.styleMapToCSS(layer.styleMap);
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
            if ($('.layers.data .layers-content div', this.blockswitcher).length > 0) {
              $('.layers.data', this.blockswitcher).show();
            }
          }
        };

        /**
         * Click handler that activates or deactivates a layer.
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcher.layerClick = function(element) {
          var layer = $(element).data('layer');
          if (layer.isBaseLayer) {
            $('.layers.base .layers-content .activated').removeClass('activated');
            $(element).addClass('activated');
            layer.map.setBaseLayer(layer);
          }
          else if (this.overlay_style == 'radio') {
            $('.layers.data .layers-content .activated').removeClass('activated');
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
          Drupal.behaviors.OpenLayersPlusBlockswitcher.setLayers()
        };

        /**
         * Parameters:
         * styleMap {OpenLayers.StyleMap}
         *
         * Returns:
         * {Object} An object with css properties and values that can be applied to an element
         *
         */
        Drupal.behaviors.OpenLayersPlusBlockswitcher.styleMapToCSS = function(styleMap) {
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
        if (data && (data.map != undefined) && (data.map.behaviors.openlayers_plus_behavior_blockswitcher != undefined)) {
          this.map = data.openlayers;
          this.overlay_style = (data.map.behaviors.openlayers_plus_behavior_blockswitcher.overlay_style) ?
              data.map.behaviors.openlayers_plus_behavior_blockswitcher.overlay_style : 'checkbox';

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
          this.getLayers();
          this.redraw();
        }
      }
  };
})(jQuery);
