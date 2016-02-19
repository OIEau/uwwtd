// $Id$

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * WMSGetFeatureinfo Behavior
 * http://dev.openlayers.org/releases/OpenLayers-2.9/doc/apidocs/files/OpenLayers/Control/WMSGetFeatureInfo-js.html
 */

(function ($) {
  //Initialize settings array.
  Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo = Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo || {};
  
  var wms_layers = [];
  var wms_popup;
  //Add the wmsgetfeatureinfo behavior
  Drupal.openlayers.addBehavior('openlayers_behavior_wmsgetfeatureinfo', function (data, options) {
    var map = data.openlayers;
    var layer;
    var layers = [];

    if (data.map.behaviors['openlayers_behavior_wmsgetfeatureinfo']) {
      if (options.getfeatureinfo_usevisiblelayers == false) { 
        wms_layers = data.openlayers.getLayersBy('drupalID', 
          options.getfeatureinfo_layers);  // TODO Make this multiple select! 
      } else {
        for (layer in data.openlayers.layers) {
          if ((data.openlayers.layers[layer].CLASS_NAME == "OpenLayers.Layer.WMS") &&
              (data.openlayers.layers[layer].isBaseLayer == false)){
            wms_layers.push(data.openlayers.layers[layer]);
          }
        }
      }

        var hlLayer = map.getLayersBy('drupalID', "wms_highlight_layer");
        
        Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.hlLayer = hlLayer;

      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement = 
        options.getfeatureinfo_htmlelement;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_usevisiblelayers = 
        options.getfeatureinfo_usevisiblelayers;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_info_format = 
        options.getfeatureinfo_info_format;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.layers = wms_layers;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_properties =
        options.getfeatureinfo_properties;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_highlight =
        options.getfeatureinfo_highlight;
      Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_feature_count =
        options.getfeatureinfo_feature_count;

      OpenLayers.Control.Click = OpenLayers.Class(OpenLayers.Control, 
        {
          defaultHandlerOptions: {
            'single': true,
            'double': false,
            'pixelTolerance': 0,
            'stopSingle': false,
            'stopDouble': false
          },

          initialize: function(options) {
            this.handlerOptions = OpenLayers.Util.extend(
              {}, this.defaultHandlerOptions
            );
            OpenLayers.Control.prototype.initialize.apply(
              this, arguments
            ); 
            this.handler = new OpenLayers.Handler.Click(
              this, {
                'click': this.onClick,
              }, this.handlerOptions
            );
          }, 

          onClick: function(evt) {
            //map = data.openlayers;
            if(wms_popup){
                try{
                    wms_popup.destroy();
                }
                catch(e){}
            }
            var wmslayers = [];
            var params;
            var sublayers;
            // TODO check if all layers have same projection
            // TODO check if all layers are from same server
            for (layer in Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.layers) {
              if (wms_layers[layer].visibility && wms_layers[layer].displayInLayerSwitcher !== 0 && (wms_layers[layer].CLASS_NAME == "OpenLayers.Layer.WMS") && wms_layers[layer].isBaseLayer == false ) {
                sublayers =  wms_layers[layer].params.LAYERS[0].split(',');
                for(l in sublayers){
                    params = {
                        REQUEST: "GetFeatureInfo",
                        BBOX: wms_layers[layer].getExtent().toBBOX(),
                        I: Math.round(evt.xy.x),
                        J: Math.round(evt.xy.y),
                        INFO_FORMAT: Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_info_format, 
                        QUERY_LAYERS: sublayers[l],
                        LAYERS: wms_layers[layer].params.LAYERS,
                        FEATURE_COUNT: Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_feature_count,
                        CRS: wms_layers[layer].projection['projCode'],
                        WIDTH: data.openlayers.size.w,
                        HEIGHT: data.openlayers.size.h,
                        VERSION : '1.3.0',
                        //Remove unwanted parameters
                        FORMAT:null,
                        SRS:null,
                        TRANSPARENT:null,
                        EXCEPTIONS: null,
                        //LAYERS:null, ==> for mapserver we need this param
                        STYLES:null
                    };
                    wmslayers.push(wms_layers[layer].getFullRequestString(params));
                }
              }
            }
            
            if (wmslayers.length > 0) { 
               $.ajax({ 
                 type: 'POST', 
                 url: Drupal.settings.basePath + 'openlayers/wms/wmsgetfeatureinfo',
                 data: { 
                   ajax : true, 
                   url : wmslayers 
                 },
                 success: function(result) {
                      //Test if we have just an html result or we have to start some js function
                      if(result!=''){
                          var pos = wms_layers[wms_layers.length-1].getExtent().getCenterLonLat();
                          wms_popup = new OpenLayers.Popup.FramedCloud(
                               "ol-wms-pop",
                               map.getLonLatFromViewPortPx(evt.xy),
                               new OpenLayers.Size(300,200),
                               '<div class="ol-content-popup ol-wms-content-popup">'+$(result).html()+'</div>',
                               null,
                               true
                          );
                          data.openlayers.addPopup(wms_popup);
                      }
                 },
                 fail: Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.fillHTMLerror
               });
            }
            else {
              //alert ("No Layer to Query is visible.");
            }
          }
        }
      );
      GetFeatureControl = new OpenLayers.Control.Click(); 

      data.openlayers.addControl(GetFeatureControl);
      GetFeatureControl.activate();
    
      // This is to update the OpenLayers Plus block switcher feature
      // Drupal.OpenLayersPlusBlockswitcher.redraw();  // TODO Fix this for D7
    }
  });

  Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.beforegetfeatureinfo = function(layernames) {
    // $("#" + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).parent().css("display", "block");
    // if (Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_usevisiblelayers == false) { 
    //   document.getElementById(Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).innerHTML = Drupal.t('Searching...');
    //   return; 
    // }
  
    // if (layernames.length == 0 ) {
    //   document.getElementById(Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).innerHTML = Drupal.t('No layer selected');
    //   return;
    // } else {
    //   document.getElementById(Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).innerHTML = Drupal.t('Searching in ' + layernames);
    // }
    return;
  };
 

  Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.fillHTMLerror = function(result) {
    /*
    $('#' + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement).html(result);
    Drupal.attachBehaviors($('#' + Drupal.openlayers.openlayers_behavior_wmsgetfeatureinfo.getfeatureinfo_htmlelement));
    */
  };
})(jQuery);
