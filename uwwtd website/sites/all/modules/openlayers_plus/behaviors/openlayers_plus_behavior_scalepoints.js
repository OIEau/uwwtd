/**
 * Implementation of Drupal behavior.
 */
(function ($) {

  Drupal.behaviors.openlayers_plus_behavior_scalepoints = {
      attach: function(context) {
        var data = $(context).data('openlayers');
        if (data && data.map.behaviors.openlayers_plus_behavior_scalepoints) {
          var styles = data.map.behaviors.openlayers_plus_behavior_scalepoints.styles;
          var colors = data.map.behaviors.openlayers_plus_behavior_scalepoints.colors;
		  var scalepointsLayers = data.map.behaviors.openlayers_plus_behavior_scalepoints.layers;
          // Collect vector layers
          var vector_layers = [];
          for (var key in data.openlayers.layers) {
            var layer = data.openlayers.layers[key];
            var styleMap = layer.styleMap;
			
            if(styleMap != undefined) {
				if(scalepointsLayers.indexOf(layer.drupalID) != -1){
				  styleMap.addUniqueValueRules("default", "weight", styles);
				  styleMap.addUniqueValueRules("default", "color", colors);
				}
              layer.redraw();
              vector_layers.push(layer);
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
