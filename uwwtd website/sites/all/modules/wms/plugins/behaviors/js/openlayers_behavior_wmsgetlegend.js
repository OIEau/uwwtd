// $Id$

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * WMSGetLegend Behavior
 */

(function ($) {
  // Initialize settings array.
  Drupal.openlayers.openlayers_behavior_wmsgetlegend = Drupal.openlayers.openlayers_behavior_wmsgetlegend || {};
  // Add the wmsgetlegend behavior.
  Drupal.openlayers.addBehavior('openlayers_behavior_wmsgetlegend', function (data, options) {
     
    var map = data.openlayers;
    var legend = {};
    var container = $('<div class="legend legend-wms" />').appendTo($(options.getlegend_htmlelement));
    container.attr('id','openlayers-behavior-wmsgetlegend-target');
    /**
     * fetchLegend : Retrieve the legend for the visible layer.
     */
    function fetchLegend() {
      container.html('');
      var wms_layers = [];
      if (data.map.behaviors['openlayers_behavior_wmsgetlegend']) {
        for(layer in data.openlayers.layers) {
          if ((data.openlayers.layers[layer].CLASS_NAME == "OpenLayers.Layer.WMS") && (data.openlayers.layers[layer].isBaseLayer == false) && (data.openlayers.layers[layer].visibility == true)){
            wms_layers.push(data.openlayers.layers[layer]);
          }
        }
        if (wms_layers !== undefined) {
            params ={};
            for(layer in wms_layers) {
                if(legend[wms_layers[layer].params.LAYERS[0]]){
                    container.append(legend[wms_layers[layer].params.LAYERS[0]]);
                }
                else{
                    params = {
                        REQUEST: 'GetLegendGraphic',
                        LAYER: wms_layers[layer].params.LAYERS[0],
                        EXCEPTIONS: '',
                        SRS: '',
                        LAYERS: '',
                        FORMAT: 'image/png'
                    };
                    
                    $.ajax({
                        type: 'POST',
                        url: Drupal.settings.basePath + 'openlayers/wms/wmsgetlegend',
                        data: {
                          ajax: true,
                          url: wms_layers[layer].getFullRequestString(params)
                        },
                        success: function(result){
                            if(result!=''){
                                $(result).attr('width', '90%');
                                var content = $('<div class="'+wms_layers[layer].params.LAYERS[0]+'">').append(result);
                                container.append(content);
                                legend[wms_layers[layer].params.LAYERS[0]] = content;
                            }
                        },
                        fail: function(result){
                            
                        }
                    });
                }
                
            }
        }
      }
    }

    // Fetch legend for the first time.
    fetchLegend();

    // Add listener to retrieve legend each time a layer is selected.
    map.events.register('changelayer', this, fetchLegend);
  });
})(jQuery);
