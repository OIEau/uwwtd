/**
 * Implementation of Drupal behavior.
 */
(function($) {

  Drupal.OpenLayersMapeditor = {};

  Drupal.behaviors.openlayers_plus_behavior_mapeditor = {
      'attach' : function(context, settings) {

        var data = $(context).data('openlayers');
        if (data && data.map.behaviors.openlayers_plus_behavior_mapeditor) {

          //Add control
          var control = new OpenLayers.Control.Mapeditor();

          switch(settings.openlayers_plus.mapeditorposition) {
          case 'nw':
            control.top = 0;
            control.left = 0;

            break;
          case 'ne':
            control.top = 0;
            control.right = 0;

            break;
          case 'sw':
            control.bottom = 0;
            control.left = 0;

            break;
          case 'se':
            control.bottom = 0;
            control.right = 0;

            break;
          default:
            break;
          }

          data.openlayers.addControl(control);
          control.activate();
        }
      }
  };

  OpenLayers.Control.Mapeditor = OpenLayers.Class(OpenLayers.Control.Permalink, {

    top: '',
    bottom: '',
    left: '',
    right: '',

    /**
     * Method: draw
     *
     * Returns:
     * {DOMElement}
     */    
    draw: function() {
      OpenLayers.Control.prototype.draw.apply(this, arguments);

      if (!this.element && !this.anchor) {
        this.element = document.createElement("a");
        this.element.innerHTML = OpenLayers.i18n("OSM Map Editor");
        this.element.href="";
        this.div.appendChild(this.element);
        this.div.classList.add('mapeditor');
        this.div.style.right =  this.right;
        this.div.style.top = this.top;

        this.div.style.left =  this.left;
        this.div.style.bottom = this.bottom;
      }
      this.map.events.on({
        'moveend': this.updateLink,
        'changelayer': this.updateLink,
        'changebaselayer': this.updateLink,
        scope: this
      });

      // Make it so there is at least a link even though the map may not have
      // moved yet.
      this.updateLink();

      return this.div;
    },

    /**
     * Override of updateLink().
     */
    updateLink: function() {
      var href = 'http://www.openstreetmap.org/edit?editor=id#';
      if (href.indexOf('#') != -1) {
        href = href.substring(0, href.indexOf('#'));
      }

      var parameters = this.createParams();   
      if(!parameters.hidebutton){
        href += '#map='+parameters['zoom']+'/'+parameters['lat']+'/'+parameters['lon'];
        this.element.href = href;
        this.element.hidden = false;
      }else {
        this.element.hidden = true;
      }

    },

    /**
     * Override of createParams(). Generates smarter layer/baselayer query string.
     */
    createParams: function(center, zoom, layers) {
      center = center || this.map.getCenter();      
      var params = {},
      lat = center.lat,
      lon = center.lon;

      // If there's still no center, map is not initialized yet.
      // Break out of this function, and simply return the params from the
      // base link.
      if (center) {
        //zoom
        params.zoom = zoom || this.map.getZoom();

        switch(true) {
        case center.lat > 0 && center.lon > 0:
          if (this.displayProjection) {
            var mapPosition = OpenLayers.Projection.transform(
                { x: center.lon, y: center.lat },
                this.map.getProjectionObject(),
                this.displayProjection);
            lon = mapPosition.x;
            lat = mapPosition.y;
          }
          break;

        default:
          lon = -0.0095981955528259;
          lat = 0.0095986388623714; 

          break;
        }

        params.lat = Math.round(lat * 100000) / 100000;
        params.lon = Math.round(lon * 100000) / 100000;

        if(params.lat == 0 && params.lon == 0){
          params.hidebutton = true;
        }

      }
      return params;
    },

    CLASS_NAME: "OpenLayers.Control.Mapeditor"
  });
})(jQuery);
