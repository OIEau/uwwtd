(function ($) {

  Drupal.behaviors['extruders.weight'] = -7;
  Drupal.behaviors['openlayers_plus_behavior_blockpanel.weight'] = -98; 
 
 Drupal.behaviors.extruders = {
    attach : function(context, setting) {
      $(".olControlblockpanel", context).once("extruder-done",function () {
        /*
         * Figure out which blockpanel we are loading, get the settings, initiate
         */

        id = $(this).context.firstElementChild.id;
        
        id = id.substr(8, 100);
        settings = setting.openlayers_plus.blockpanels[id];

        options = {
          positionFixed:false,
          width:285,
          sensibility:800,
          position:settings.position, // left, right, bottom
          flapDim:100,
          textOrientation:"bt", // or "tb" (top-bottom or bottom-top)
          onExtOpen:function(){},
          onExtContentLoad:function(){},
          onExtClose:function(){},  
          hidePanelsOnClose:true,
          textOrientation:"bt",
          autoCloseTime:0, // 0=never
          slideTimer:0, // very fast
          top:200,
          extruderOpacity:1
        };

        $(this).context.firstElementChild.innerHTML = settings.markup;
        $(this).addClass("{title:'" + settings.label + "'}");
        $(this).buildMbExtruder(options);
        if (settings.startstatus == 'open') {
          $(this).openMbExtruder(true);
        }
        // And now set the setting
        $(this).context['options'].slideTimer = settings.slidetimer;
        // Don't propagate click events to the map
        // this doesn't catch events that are below the layer list
       this.mousedown = function(evt) {
          OpenLayers.Event.stop(evt);
        };

      });
    }
  };

  Drupal.behaviors.openlayers_plus_behavior_blockpanel = {
    attach : function(context, setting) {
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors.openlayers_plus_behavior_blockpanel) {
        var ctrl;
        var map = data.openlayers;

        blockpanels = setting.openlayers_plus.blockpanels;
        controls = new Array();
        for (region in blockpanels) {
          panel = new OpenLayers.Control.BlockPanel('top');
          panel.html = blockpanels[region]['markup'];//<div class="text">Reinier and a lot of other info</div>';
          panel.label = blockpanels[region]['label']; //'<a href="http://www.google.com">Open Me int he page</a>';
          panel.position = blockpanels[region]['position'];
          panel.id = "Extruder" + region;
          map.addControl(panel);
        }
      }
    }
  };

})(jQuery);

OpenLayers.Control.BlockPanel = OpenLayers.Class(OpenLayers.Control, {

  html: "",
  label: "",
  position: "",
  id: "",
  
  draw: function() {
    OpenLayers.Control.prototype.draw.apply(this, arguments);

    /*
     * We create the div as an empty div, so as to not trigger any behavior on the content
     * before our own behavior is fired. (e.g. collapsiblock)
     * The html is added inside our behavior.
     */
    this.div.innerHTML = '<div id="' + this.id + '"></div>';
    jQuery(this.div).addClass(this.position);
    this.registerEvents();
    return this.div;
  },

  
  destroy: function() {redraw
      OpenLayers.Control.prototype.destroy.apply(this, arguments);
    },
  
    /** 
     * Method: registerEvents
     * Registers events on the popup.
     *
     * Do this in a separate function so that subclasses can 
     *   choose to override it if they wish to deal differently
     *   with mouse events
     * 
     *   Note in the following handler functions that some special
     *    care is needed to deal correctly with mousing and popups. 
     *   
     *   Because the user might select the zoom-rectangle option and
     *    then drag it over a popup, we need a safe way to allow the
     *    mousemove and mouseup events to pass through the popup when
     *    they are initiated from outside. The same procedure is needed for
     *    touchmove and touchend events.
     * 
     *   Otherwise, we want to essentially kill the event propagation
     *    for all other events, though we have to do so carefully, 
     *    without disabling basic html functionality, like clicking on 
     *    hyperlinks or drag-selecting text.
     */
     registerEvents:function() {
        this.events = new OpenLayers.Events(this, this.div, null, true);

        function onTouchstart(evt) {
            OpenLayers.Event.stop(evt, true);
        }
        this.events.on({
            "mousedown": this.onmousedown,
            "mousemove": this.onmousemove,
            "mouseup": this.onmouseup,
            "click": this.onclick,
            "mouseout": this.onmouseout,
            "dblclick": this.ondblclick,
            "touchstart": onTouchstart,
            "activate": this.onActivate,
            "deactivate": this.onDeactivate,
            
            scope: this
        });
        
     },

    /** 
     * Method: onmousedown 
     * When mouse goes down within the popup, make a note of
     *   it locally, and then do not propagate the mousedown 
     *   (but do so safely so that user can select text inside)
     * 
     * Parameters:
     * evt - {Event} 
     */
    onmousedown: function (evt) {
        this.mousedown = true;
        OpenLayers.Event.stop(evt, true);
    },

    /** 
     * Method: onmousemove
     * If the drag was started within the popup, then 
     *   do not propagate the mousemove (but do so safely
     *   so that user can select text inside)
     * 
     * Parameters:
     * evt - {Event} 
     */
    onmousemove: function (evt) {
        if (this.mousedown) {
            OpenLayers.Event.stop(evt, true);
        }
    },

    /** 
     * Method: onmouseup
     * When mouse comes up within the popup, after going down 
     *   in it, reset the flag, and then (once again) do not 
     *   propagate the event, but do so safely so that user can 
     *   select text inside
     * 
     * Parameters:
     * evt - {Event} 
     */
    onmouseup: function (evt) {
        if (this.mousedown) {
            this.mousedown = false;
            OpenLayers.Event.stop(evt, true);
        }
    },

    /**
     * Method: onclick
     * Ignore clicks, but allowing default browser handling
     * 
     * Parameters:
     * evt - {Event} 
     */
    onclick: function (evt) {
        OpenLayers.Event.stop(evt, true);
    },

    /** 
     * Method: onmouseout
     * When mouse goes out of the popup set the flag to false so that
     *   if they let go and then drag back in, we won't be confused.
     * 
     * Parameters:
     * evt - {Event} 
     */
    onmouseout: function (evt) {
        this.mousedown = false;
    },
    
    /** 
     * Method: ondblclick
     * Ignore double-clicks, but allowing default browser handling
     * 
     * Parameters:
     * evt - {Event} 
     */
    ondblclick: function (evt) {
        OpenLayers.Event.stop(evt, true);
    },
  CLASS_NAME: "OpenLayers.Control.blockpanel"
});
