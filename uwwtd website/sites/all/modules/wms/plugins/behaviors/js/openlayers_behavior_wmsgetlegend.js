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
  //if(!Drupal.openlayers.openlayers_behavior_wmsgetlegend.memUrl){
   //   Drupal.openlayers.openlayers_behavior_wmsgetlegend.memUrl = [];
  //  }
  // Add the wmsgetlegend behavior.
  Drupal.openlayers.addBehavior('openlayers_behavior_wmsgetlegend', function (data, options) {
     
    var map = data.openlayers;
    var legend = {};
    var selector = '';
    //FIX ME why in some conf we don't have the options.getlegend_htmlelement value but juste an array
    if(options.getlegend_htmlelement){
        selector = options.getlegend_htmlelement;
    }else{
        selector = options[0];
    }
    
    
    var container = $('<div class="legend legend-wms" />').appendTo($(selector));
    container.attr('id','openlayers-behavior-wmsgetlegend-target');
    /**
     * fetchLegend : Retrieve the legend for the visible layer.
     */
    function fetchLegend() {
      container.html('');
      var wms_layers = [];
      var sublayers;
      var url;
      var urls = [];
      if (data.map.behaviors['openlayers_behavior_wmsgetlegend']) {
        for(layer in data.openlayers.layers) {
          if ((data.openlayers.layers[layer].CLASS_NAME == "OpenLayers.Layer.WMS") && (data.openlayers.layers[layer].isBaseLayer == false) && (data.openlayers.layers[layer].visibility == true)){
            wms_layers.push(data.openlayers.layers[layer]);
          }
        }
        if (wms_layers !== undefined) {
            params ={};
            for(layer in wms_layers) {
                sublayers =  wms_layers[layer].params.LAYERS[0].split(',');
                for(l in sublayers){
                    params = {
                        REQUEST: 'GetLegendGraphic',
                        LAYER: sublayers[l],
                        EXCEPTIONS: '',
                        SRS: '',
                        LAYERS: '',
                        FORMAT: 'image/png'
                    };
                    //if(Drupal.openlayers.openlayers_behavior_wmsgetlegend.memUrl.indexOf(wms_layers[layer].getFullRequestString(params))===-1){
                        urls.push( wms_layers[layer].getFullRequestString(params));
                    //    Drupal.openlayers.openlayers_behavior_wmsgetlegend.memUrl.push(wms_layers[layer].getFullRequestString(params));  
                    //}
                                     
                }
                
            }
        }
        if (urls.length > 0) {
            $.ajax({
                type: 'POST',
                url: Drupal.settings.basePath + 'openlayers/wms/wmsgetlegend',
                data: {
                  ajax: true,
                  url: urls
                },
                success: function(result){
                    container.html('');
                    if(result!=''){
                        container.append(result);
                    }
                },
                fail: function(result){
                    
                }
            });  
        }
      }
    }

    // Fetch legend for the first time.
    fetchLegend();

    // Add listener to retrieve legend each time a layer is selected.
    map.events.register('changelayer', this, fetchLegend);
  });
})(jQuery);
