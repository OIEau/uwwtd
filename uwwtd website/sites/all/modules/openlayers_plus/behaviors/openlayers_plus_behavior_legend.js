(function ($) {

    
    $(document).ready(function() {
        Drupal.OpenLayersPlusLegend.removeDuplicateLegend();
        if($(".legend-item").length > 0 && $(".toggleLegend").data("state") == "+") {
            $(".toggleLegend").data("state", "-");
        }
        else if($(".toggleLegend").data("state") == "-"){
          Drupal.OpenLayersPlusLegend.toggleLegend();
        }
        
        $(".toggleLegend").click(
            function(){
                Drupal.OpenLayersPlusLegend.toggleLegend();
            }
        );
        
        
    });

/**
 * Implementation of Drupal behavior.
 */
  Drupal.OpenLayersPlusLegend = {};

  Drupal.behaviors.openlayers_plus_behavior_legend = {
    attach: function(context, settings) {
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors.openlayers_plus_behavior_legend) {
        var layer, i;
        for (i in data.openlayers.layers) {
          layer = data.openlayers.layers[i];
          if (data.map.behaviors.openlayers_plus_behavior_legend[layer.drupalID]) {
            if (!$('div.openlayers-legends', context).size()) {
              $(context).append("<div class='openlayers-legends'></div>");
            }
            layer.events.register('visibilitychanged', layer, Drupal.OpenLayersPlusLegend.setLegend);

            // Trigger the setLegend() method at attach time. We don't know whether
            // our behavior is being called after the map has already been drawn.
            Drupal.OpenLayersPlusLegend.setLegend(layer);
          }
        }
      }
    }
  };

  Drupal.OpenLayersPlusLegend.setLegend = function(layer) {
      // The layer param may vary based on the context from which we are called.
      layer = layer.object ? layer.object : layer;
        
      var name = layer.drupalID;
      var map = $(layer.map.div);
      var data = map.data('openlayers');
      var legend = data.map.behaviors.openlayers_plus_behavior_legend[name];
      var legends = $('div.openlayers-legends', map);
      if (layer.visibility && $('#openlayers-legend-' + name, legends).size() === 0) {
        legends.append(legend);
      }
      else if (!layer.visibility) {
        $('#openlayers-legend-' + name, legends).remove();
      }
  };
    
  Drupal.OpenLayersPlusLegend.removeDuplicateLegend = function() {
    var i = 0;
    $('.toggleLegend-container').each(function(){
      if(i > 0) {
        $(this).remove();
      }
      i++;        
    });
  };

  Drupal.OpenLayersPlusLegend.toggleLegend = function () {
    Drupal.OpenLayersPlusLegend.removeDuplicateLegend();
    
    if($(".toggleLegend").data("state") == "-") {
      $(".toggleLegend").data("state", "+");
      $(".toggleLegend-container .ol-plus-label").html("");
      $("div.openlayers-legends .legend").hide("slow");
    } else {
      $(".toggleLegend").data("state", "-");
      $(".toggleLegend-container .ol-plus-label").html($(".toggleLegend").attr("title"));
      $("div.openlayers-legends .legend").show("slow");
    }
  };
    
})(jQuery);
