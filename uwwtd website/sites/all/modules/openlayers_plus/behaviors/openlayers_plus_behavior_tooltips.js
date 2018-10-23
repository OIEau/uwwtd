(function ($) {

  /**
   * Implementation of Drupal behavior.
   */

  //Drupal.OpenLayersTooltips = {};

  Drupal.behaviors.openlayers_plus_behavior_tooltips = {
      attach: function(context, settings) {
        var data = $(context).data('openlayers');
        if (data && data.map.behaviors.openlayers_plus_behavior_tooltips) {
          // Options
          var select_method = 'select';
          if (data.map.behaviors.openlayers_plus_behavior_tooltips.positioned) {
            select_method = 'positionedSelect';
          }
          var options = data.map.behaviors.openlayers_plus_behavior_tooltips;
          var vector_layers = [];
          if (typeof options.layers == 'undefined' || options.layers.length == 0) {
            vector_layers = map.getLayersByClass('OpenLayers.Layer.Vector');

          }else {
            // Collect vector layers
            for (var key in options.layers) {
              var selectedLayer = data.openlayers.getLayersBy('drupalID', options.layers[key]);
              if (typeof selectedLayer[0] != 'undefined') {
                vector_layers.push(selectedLayer[0]);
              }
            }
          }

          if (vector_layers.length == 1) {
            vector_layers = vector_layers[0];
          }

          // Add control
          var control = new OpenLayers.Control.SelectFeature(vector_layers, {
            activeByDefault: true,
            highlightOnly: false,
            allowSelection: true,
            selectStyle: {
              fillOpacity: 0.5,
              fillColor: "#FFAD33",
              strokeColor: "#000000",
              cursor: "pointer"
            },
            onSelect: function(feature) {
              Drupal.OpenLayersTooltips.select(feature);
            },
            onUnselect: function(feature) {
              Drupal.OpenLayersTooltips.unselect(feature);
            },
            multiple: true,
            hover: true,
            callbacks: {
              'click': Drupal.OpenLayersTooltips.openPopup,
              'over': Drupal.OpenLayersTooltips.over,
              'out': Drupal.OpenLayersTooltips.out
            }
          });
          data.openlayers.addControl(control);
          control.activate();
        }
      }
  };

  Drupal.OpenLayersTooltips = {
      'openPopup': function(feature) {
        
        var html = '';
        if (feature.attributes.name) {
          html += feature.attributes.name;
        }
        if (feature.attributes.description) {
          html += feature.attributes.description;
        }
        // @TODO: Make this a behavior option and allow interaction with other
        // behaviors like the MN story popup.
        var link;
        if ($(html).is('a')) {
          link = $(html);
        }
        else if ($(html).children('a').size() > 0) {
          link = $(html).children('a')[0];
        }
        if (link) {
          var href = $(link).attr('href');
          if (Drupal.OpenLayersPermalink && Drupal.OpenLayersPermalink.addQuery) {
            href = Drupal.OpenLayersPermalink.addQuery(href);
          }
          window.location = href;
          return false;
        }
        return;
      },

      'out': function(feature) {
         $(feature.layer.map.div).children().each(function(index) {
           if(this.tagName === 'SPAN') {
             $(this).parent().children('span').remove();
           }

         });

       },

      'over': function(feature) {
        var title;
        //for clustered features show the title of the first item.
        if (feature.cluster) {
            for(var key in feature.cluster[0].attributes) {
              if(key.indexOf("title") != -1 && feature.cluster[0].attributes[key].indexOf("<a") == -1) {
                title = feature.cluster[0].attributes[key];
                break;
              }
            }

        }else if (feature.attributes) {
            
          for(var key in feature.attributes) {
            if(key.indexOf("title") != -1 ) {
              title = feature.attributes[key];
              break;
            }
          }
        }

        var tooltip = $('<span class="tooltip">' + title + '</span>');
        var point  = new OpenLayers.LonLat(feature.geometry.x, feature.geometry.y);
        var offset = feature.layer.getViewPortPxFromLonLat(point);
        tooltip.css({zIndex: '1000', position: 'absolute', left: offset.x, top: offset.y});
        $(feature.layer.map.div).css({position:'relative'}).append(tooltip);

      }
  };

})(jQuery);
