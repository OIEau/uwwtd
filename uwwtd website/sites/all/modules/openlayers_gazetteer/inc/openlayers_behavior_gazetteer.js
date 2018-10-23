/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Gazetteer Control. Implements Location Search Control for Openlayers.
 */
Drupal.openlayers.addBehavior('openlayers_behavior_gazetteer', function (data, options) {
  (function ($) {
    if(Drupal.settings.openlayers_gazetteer.geonames_username == '') {
      console.log('Configure your GeoNames username to activate the Gazetteer Behavior.')
    } else {
      var serviceURL = 'http://api.geonames.org/searchJSON';

      // Create text input.
      var input = $('<input/>',
        {
          type:'text',
          placeholder:'Search for a location',
          name:'openlayers-gazetteer-search-for-location',
          class:'openlayers-gazetteer-search-textfield'
        }
      );

      // Add it to DOM.
      var map = $('#' + data.openlayers.div.id);
      map.append(input);

      // If a there's a restriction query.
      var query = '';
      if(options.restriction_query != '') {
        var restrictions = options.restriction_query.split('&');
        for (var key in restrictions) {
          var param = restrictions[key].split('=');
          query += '&' + encodeURIComponent(param[0]) + '=' + encodeURIComponent(param[1]);
        }
      }

      // Autocomplete handler.
      $('.openlayers-gazetteer-search-textfield').autocomplete({
        source: function (request, response) {
          $.ajax({
            url: serviceURL,
            data: 'name_startsWith=' + encodeURIComponent(request.term) + query + '&maxRows=10&username=' + encodeURIComponent(Drupal.settings.openlayers_gazetteer.geonames_username),
            success: function (data) {
              response($.map(data.geonames, function(item) {
                return {
                  label: item.name + (item.adminName1 ? ", " + item.adminName1 : "") + ", " + item.countryName,
                  value: item.name,
                  lat: item.lat,
                  lon: item.lng
                }
              }));
            }
          })
        },
        minLength: 2,
        delay: 200,
        select: function (event, ui) {
          data.openlayers.setCenter(
            new OpenLayers.LonLat(ui.item.lon , ui.item.lat).transform(
              new OpenLayers.Projection('EPSG:4326'),
              data.map.projection
            ), 13);
        },
        open: function () {
          $(this).removeClass("ui-corner-all").addClass("ui-corner-top");
        },
        close: function () {
          $(this).removeClass("ui-corner-top").addClass("ui-corner-all");
        }
      });
    }
  })(jQuery);
});
