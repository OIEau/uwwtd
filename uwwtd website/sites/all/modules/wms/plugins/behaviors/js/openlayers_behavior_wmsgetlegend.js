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
    var layer;

    /**
     * fetchLegend : Retrieve the legend for the visible layer.
     */
    function fetchLegend() {
      if (data.map.behaviors['openlayers_behavior_wmsgetlegend']) {
        // Get visible layer.
        // THIS ONLY WORKS IF ONE LAYER IS VISIBLE AT ANYTIME (BASE LAYER EXCLUDED).
        // TODO handle multiple layers/legends.
        var layers = map.getLayersBy('visibility', true);
        var wmslayer;
        for(i in layers) {
          if(layers[i].isBaseLayer === false && layers[i].displayInLayerSwitcher !== 0) {
            var wmslayer = layers[i];
          }
        }
        
        if (wmslayer !== undefined) {
          // Get layer name from WMS layer.
          var layername = wmslayer.params.LAYERS[0];

          Drupal.openlayers.openlayers_behavior_wmsgetlegend.getlegend_htmlelement =
            options.getlegend_htmlelement;
          Drupal.openlayers.openlayers_behavior_wmsgetlegend.layers = layers;
            Drupal.openlayers.openlayers_behavior_wmsgetlegend.beforegetlegend();

          // Override/add parameters to request string.
          var params = {
            REQUEST: 'GetLegendGraphic',
            LAYER: layername,
            EXCEPTIONS: '',
            SRS: '',
            LAYERS: '',
            FORMAT: 'image/png'
          };

          var url = wmslayer.getFullRequestString(params);
          $.ajax({
            type: 'POST',
            url: Drupal.settings.basePath + 'openlayers/wms/wmsgetlegend',
            data: {
              ajax: true,
              url: url
            },
            success: Drupal.openlayers.openlayers_behavior_wmsgetlegend.fillHTML,
            fail: Drupal.openlayers.openlayers_behavior_wmsgetlegend.fillHTMLerror
          });
        }
      }
    }

    // Fetch legend for the first time.
    fetchLegend();

    // Add listener to retrieve legend each time a layer is selected.
    map.events.register('changelayer', this, fetchLegend);
  });

  Drupal.openlayers.openlayers_behavior_wmsgetlegend.beforegetlegend = function() {
    $("#" + Drupal.openlayers.openlayers_behavior_wmsgetlegend.getlegend_htmlelement).parent().css("display", "block");
    $("#" + Drupal.openlayers.openlayers_behavior_wmsgetlegend.getlegend_htmlelement).html("<div class='ajax-progress'><div class='throbber'></div></div>");
    return;
  };

  Drupal.openlayers.openlayers_behavior_wmsgetlegend.fillHTML = function(result) {
    $('#' + Drupal.openlayers.openlayers_behavior_wmsgetlegend.getlegend_htmlelement).html(result);
    Drupal.attachBehaviors($('#' + Drupal.openlayers.openlayers_behavior_wmsgetlegend.getlegend_htmlelement));
  };

  Drupal.openlayers.openlayers_behavior_wmsgetlegend.fillHTMLerror = function(result) {
    $('#' + Drupal.openlayers.openlayers_behavior_wmsgetlegend.getlegend_htmlelement).html(result);
    Drupal.attachBehaviors($('#' + Drupal.openlayers.openlayers_behavior_wmsgetlegend.getlegend_htmlelement));
  };
})(jQuery);
