(function ($) {

  Drupal.behaviors['openlayers.weight'] = -100;
  Drupal.behaviors['openlayers_plus_behavior_maptext.weight'] = -99; 
	
  Drupal.behaviors.openlayers_plus_behavior_maptext = {
    attach : function(context, setting) {
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors.openlayers_plus_behavior_maptext) {
        var map = data.openlayers;
          
        regions = setting.openlayers_plus.maptext;
          
        for (region in regions) {
          i = 0;    
          var controls = new Array();

          for (block in regions[region]['blocks']) {
            curblock = regions[region]['blocks'][block];
            maptext =  new OpenLayers.Control.mtext(curblock);
            if (regions[region]['showpanel'] == 1) {
              controls[i] = maptext;
            }
            else {
              map.addControl(maptext);
              maptext.activate();
            }
            i++;
          }
          // If we want to show the panel
          if (regions[region]['showpanel'] == 1) {
            panel = new OpenLayers.Control.Panel();
            panel.addControls(controls);
            panel.saveState = true;
            panel.activeState = true;
            panel.allowDepress = true;
            map.addControl(panel);
            panel.div.classList.add(regions[region]['panelfieldset']['horizontalposition']);
            panel.div.classList.add(regions[region]['panelfieldset']['verticalposition']);
          }
          /**
           * The next section is entirely for the scenario where the panel group
           * should also contain the popup. (and popup only)
           */
          if (regions[region]['popup']) {
              // First, lets hide the button for the popup in the panel
              control = map.getControl("olplusmaptextpopup");
              panel = control.panel_div.classList.add("olplusmaptextpopupbutton");

              // Now determine the layers we are a popup for
              var layers = [];

              // For backwards compatiability, if layers is not
              // defined, then include all vector layers
              if (typeof regions[region]['popupfieldset']['layers'] == 'undefined' || regions[region]['popupfieldset']['layers'] == 0) {
                layers = map.getLayersByClass('OpenLayers.Layer.Vector');
              }
              else {
                for (var i in regions[region]['popupfieldset']['layers']) {
                  var selectedLayer = map.getLayersBy('drupalID', regions[region]['popupfieldset']['layers'][i]);
                  if (typeof selectedLayer[0] != 'undefined') {
                    layers.push(selectedLayer[0]);
                  }
                }
              }

              /*
               *  Due to a bug in OpenLayers,
               *  if you pass 1 layer to the SelectFeature control as an array
               *  the onClick event is not cancelled after the first layer.
               *  Some people might need that.
               */
              if (layers.length == 1) {
                layers = layers[0];
              }
              
              // Add the select control. We dont deal with hovers.
              var control = new OpenLayers.Control.SelectFeature(
                  layers,
                  {
                    activeByDefault: true,
                    highlightOnly: true,
                    multiple: false,
                    hover: false,
                    callbacks: {
                      'click': Drupal.openlayers_plus_behavior_maptext.popup
                    },
                  }
              );

              map.addControl(control);
              control.activate();

          }
          
        }
      }
    }
  };

  Drupal.openlayers_plus_behavior_maptext = {
    'popup': function(feature) {
      control = feature.layer.map.getControl("olplusmaptextpopup");
      control.title = feature.data.name;
      control.blockhtml = feature.data.description;
      control.draw();
      panel_id = control.panel_div.parentNode.id;
      panel = feature.layer.map.getControl(panel_id);
      if (!control.active) {
        panel.activateControl(control);
      }
    }
  };
})(jQuery);

OpenLayers.Control.mtext = OpenLayers.Class(OpenLayers.Control, {
  
  type: OpenLayers.Control.TYPE_TOOL,
  title: "",
  contentHTML: null,
  horizontalposition: "",
  verticalposition: "",

  /** 
   * Property: contentDiv 
   * {DOMElement} a reference to the element that holds the content of
   *              the div.
   */
  contentDiv: null,
  
  /** 
   * Property: groupDiv 
   * {DOMElement} First and only child of 'div'. The group Div contains the
   *     'contentDiv' and the 'closeDiv'.
   */
  groupDiv: null,

  /** 
   * Property: closeDiv
   * {DOMElement} the optional closer image, in the headerDiv.
   */

  closeDiv: null,
  
  /** 
   * Property: groupDiv 
   * {DOMElement} Contains the title of the block.
   */
  titleDiv: null,

  /** 
   * Property: closeDiv
   * {DOMElement} contains the title and closeDiv.
   */
  headerDiv: null,
  
  /** 
   * Property: map 
   * {<OpenLayers.Map>} this gets set in Map.js when the popup is added to the map
   */
  map: null,
  
  /**
   * Initializes a maptext. We do not add this.div here 
   * because that will prevent the control from being added to the map.
   * So, the control is first added to the map, and then in the draw
   * the html is created. (and that works together with behaviours, too.
   */
  initialize: function(options) {
      OpenLayers.Control.prototype.initialize.apply(
          this, arguments
      );
      this.title = options.title;
      this.blockhtml = options.markup;
      this.id = options.id;
      this.active = false;
      this.horizontalposition = options.horizontalposition;
      this.verticalposition = options.verticalposition;
      this.showclosebox = options.showclosebox;
      if (options.toggle) {
          this.type = OpenLayers.Control.TYPE_TOGGLE
      }
      else {
          this.type = OpenLayers.Control.TYPE_TOOL;          
      }
  },
  
  
  destroy: function() {redraw
    OpenLayers.Control.prototype.destroy.apply(this, arguments);
  },

  draw: function() {
    OpenLayers.Control.prototype.draw.apply(this, arguments);
    if (this.groupDiv == null) {
      this.div.className = this.displayClass;
      this.div.classList.add(this.horizontalposition);
      this.div.classList.add(this.verticalposition);
      this.groupDiv = OpenLayers.Util.createDiv(
            this.id + "_block", 
                       null, null, null, "relative"
      );

     this.titleDiv = OpenLayers.Util.createDiv(
           this.id + "_title", 
           null, null, null, "relative"
     );
   
     this.headerDiv = OpenLayers.Util.createDiv(
           this.id + "_header", 
           null, null, null, "relative"
     );
   
     this.contentDiv = OpenLayers.Util.createDiv(
           this.id + "_content",
           null, null, null, "relative"
     );

     this.titleDiv.appendChild(this.headerDiv);
     this.groupDiv.appendChild(this.titleDiv);
     this.groupDiv.appendChild(this.contentDiv);

     this.hide();
   
     this.div.appendChild(this.groupDiv);
     if (this.showclosebox) {
       this.addCloseBox();
     }  
      this.registerEvents();
    }

    if (this.title === '') {
        title = '<h2 class="popup-title none"><a></a></h2>';
      }
      else {
        title = '<h2 class="popup-title"><a>' + this.title + '</a></h2>';
      }
    
      this.headerDiv.innerHTML = title;
      this.contentHTML = this.blockhtml;

    
    this.setContentHTML();
    
    return this.div;
  },
  
  setContentHTML: function (contentHTML) {
      if (contentHTML != null) {
          this.contentHTML = contentHTML;
      }
     
      if ((this.contentDiv != null) && 
          (this.contentHTML != null) &&
          (this.contentHTML != this.contentDiv.innerHTML)) {
     
          this.contentDiv.innerHTML = this.contentHTML;
      }
  },

  onActivate: function() {
     this.show();
  },
  
  onDeactivate: function() {
      this.hide();      
   },
  
   /**
    * Method: show
    * Makes the popup visible.
    */
   show: function() {
       this.div.style.display = 'block';
   },

   /**
    * Method: hide
    * Makes the popup invisible.
    */
   hide: function() {
       this.div.style.display = 'none';
   },
   
  /**
   * Method: addCloseBox
   * 
   * Parameters:
   * callback - {Function} The callback to be called when the close button
   *     is clicked.
   */
  addCloseBox: function(callback) {

      this.closeDiv = OpenLayers.Util.createDiv(
          this.id + "_close", null, new OpenLayers.Size(17, 17)
      );
      this.closeDiv.className = "olPopupCloseBox"; 
      this.closeDiv.style.right = "0px";
      this.closeDiv.style.top = "0px";

      this.titleDiv.appendChild(this.closeDiv);

      var closePopup = callback || function(e) {
          this.deactivate();
          OpenLayers.Event.stop(e);
      };+
      OpenLayers.Event.observe(this.closeDiv, "touchend", 
              OpenLayers.Function.bindAsEventListener(closePopup, this));
      OpenLayers.Event.observe(this.closeDiv, "click", 
              OpenLayers.Function.bindAsEventListener(closePopup, this));
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

  CLASS_NAME: "OpenLayers.Control.Maptext"
});
