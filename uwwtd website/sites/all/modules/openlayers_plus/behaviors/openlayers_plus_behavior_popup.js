(function ($) {

  Drupal.behaviors.openlayers_plus_behavior_popup = {
      attach: function(context, settings) {
        var data = $(context).data('openlayers');
        if (data && data.map.behaviors.openlayers_plus_behavior_popup) {
          var map = data.openlayers;
          var options = data.map.behaviors.openlayers_plus_behavior_popup;
          var layers = [];

          // For backwards compatiability, if layers is not
          // defined, then include all vector layers
          if (typeof options.layers == 'undefined' || options.layers.length == 0) {
            layers = map.getLayersByClass('OpenLayers.Layer.Vector');
          }
          else {
            for (var i in options.layers) {
              var selectedLayer = map.getLayersBy('drupalID', options.layers[i]);
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
          // Add control
          var control = new OpenLayers.Control.SelectFeature(
              layers,
              {
                activeByDefault: true,
                highlightOnly: true,
                multiple: false,
                hover: true,
                callbacks: {
                  // Callback for hover state
                  // Only show tooltips on hover if the story popup is not open.
                  'out' : Drupal.openlayers_plus_behavior_popup.out,
                  'click': Drupal.openlayers_plus_behavior_popup.openPopup
                },
                //Only enable popup if selected in menu
                overFeature : function (feature) {
                  if(settings.popup.hover == 1) {
                    Drupal.openlayers_plus_behavior_popup.over(feature);
                  }
                }
              }
          );

          data.openlayers.addControl(control);
          control.activate();
        }
        else if ($(context).is('.openlayers-popupbox')) {
          // Popup close
          $('a.popup-close', context).click(function() {
            $(this).parents('.openlayers-popupbox').fadeOut('fast', function() { $(this).remove(); });
            return false;
          });

          // Set initial pager state
          Drupal.openlayers_plus_behavior_popup.pager(context, 'set');

          // Next link
          $('ul.popup-links a.next', context).click(function() {
            var context = $(this).parents('div.openlayers-popupbox');
            Drupal.openlayers_plus_behavior_popup.pager(context, 'next');
            return false;
          });

          // Prev link
          $('ul.popup-links a.prev', context).click(function() {
            var context = $(this).parents('div.openlayers-popupbox');
            Drupal.openlayers_plus_behavior_popup.pager(context, 'prev');
            return false;
          });
        }
      }
      };

      Drupal.openlayers_plus_behavior_popup = {

          // Pager actions
          'pager': function(context, op) {
            var active = $('div.item-list li.openlayers-popupbox-active', context);
            var index = $('div.item-list > ul > li', context).index(active);
            var total = $('div.item-list > ul > li', context).size();

            var active_title = $('div.title_item-list li.openlayers-popupbox-active', context);

            switch (op) {
            case 'set':
              if (active.size() === 0) {
                index = 0;

                $('div.item-list > ul > li', context).hide();
                $('div.item-list > ul > li:first', context).addClass('openlayers-popupbox-active').show();
                $('ul.popup-links a.prev', context).addClass('disabled');

                $('div.title_item-list > ul > li', context).hide();
                $('div.title_item-list > ul > li:first', context).addClass('openlayers-popupbox-active').show();
                $('ul.popup-links a.prev', context).addClass('disabled');
                if (total <= 1) {
                  $('ul.popup-links', context).hide();
                }
              }
              else {
                if (index === 0) {
                  $('ul.popup-links a.prev', context).addClass('disabled');
                  $('ul.popup-links a.next', context).removeClass('disabled');
                }
                else if (index == (total - 1)) {
                  $('ul.popup-links a.next', context).addClass('disabled');
                  $('ul.popup-links a.prev', context).removeClass('disabled');
                }
                else {
                  $('ul.popup-links a.next', context).removeClass('disabled');
                  $('ul.popup-links a.prev', context).removeClass('disabled');
                }
              }
              var count = parseInt(index + 1, 10) + ' of ' + parseInt(total, 10);
              $('span.count', context).text(count);
              break;

            case 'next':
              if (index < (total - 1)) {
                active.removeClass('openlayers-popupbox-active').hide()
                .next('li').addClass('openlayers-popupbox-active').show();

                active_title.removeClass('openlayers-popupbox-active').hide()
                .next('li').addClass('openlayers-popupbox-active').show();
                Drupal.openlayers_plus_behavior_popup.pager(context, 'set');
              }
              return false;

              break;

            case 'prev':
              if (index > 0) {
                active.removeClass('openlayers-popupbox-active').hide()
                .prev('li').addClass('openlayers-popupbox-active').show();

                active_title.removeClass('openlayers-popupbox-active').hide()
                .prev('li').addClass('openlayers-popupbox-active').show();
                Drupal.openlayers_plus_behavior_popup.pager(context, 'set');
              }
              return false;

              break;

            }
          },

          // Click state
          'openPopup': function(feature) {
            var context = $(feature.layer.map.div);

            // Initialize popup
            if (!$('.openlayers-popupbox', context).size()) {
              context.append("<div class='openlayers-popupbox popup'></div>");
            }
            else {
              $('.openlayers-popupbox:not(.popup)').addClass('popup');
            }

            // Hide the layer switcher if it's open.
            for (var key in context.data('openlayers').openlayers.controls) {
              if (context.data('openlayers').openlayers.controls[key].CLASS_NAME == "OpenLayers.Control.LayerSwitcherPlus") {
                context.data('openlayers').openlayers.controls[key].minimizeControl();
              }
            }

            var text;

            if(feature.attributes.title) {
              text = '<a href="#" class="popup-close">X</a>';
              text += '<h2 class="popup-title">' + feature.attributes.title + "</h2>";

              if (feature.attributes.field_description != undefined) {
                text += '<div class="popup-content">' + feature.attributes.field_description + "</div>";
              }else if (feature.attributes.description != undefined) {
                text += '<div class="popup-content">' + feature.attributes.description + "</div>";
              }else if (feature.attributes.body != undefined) {
                text += '<div class="popup-content">' + feature.attributes.body + "</div>";
              }

            }else if(feature.attributes.name) {
              text = '<a href="#" class="popup-close">X</a>';

              $.each(feature.attributes, function(index,item) {
                //text += '<h2 class="popup-title">' + item + "</h2>";
                if(index.indexOf("title") != -1 && item.indexOf("<a") == -1) {
                  //alert(index);
                  text += '<h2 class="popup-title">' + feature.attributes[index] + "</h2>";
                }
              });

              if (feature.attributes.field_description != undefined) {
                text += '<div class="popup-content">' + feature.attributes.field_description + "</div>";
              }else if (feature.attributes.description != undefined) {
                text += '<div class="popup-content">' + feature.attributes.description + "</div>";
              }else if (feature.attributes.body != undefined) {
                text += '<div class="popup-content">' + feature.attributes.body + "</div>";
              }

            }
            else if (feature.cluster) {

              text = '<a href="" class="popup-close">X</a>';
              text += '<div class="title_item-list">';
              text += '<ul>';

              for(var y = 0; y < feature.cluster.length; y++) {
                for(var x in feature.cluster[y]['attributes']) {
                  if(x.indexOf("title") != -1 && feature.cluster[y]['attributes'][x].indexOf("<a") == -1) {
                    text += '<li>' + feature.cluster[y]['attributes'][x] + '</li>';
                    break;
                  }
                }
              }

              text += '</ul>';
              text += '</div>';

              text += '<div class="popup-content">';
              text += '<div class="item-list">';
              text += '<ul>';

              for(var y = 0; y < feature.cluster.length; y++) {
                for(var z in feature.cluster[y]['attributes']) {
                  if(z.indexOf("description") != -1 || z.indexOf("body") != -1) {
                    text += '<li>' + feature.cluster[y]['attributes'][z] + '</li>';
                    break;
                  }
                }
              }
            }

            text += "<div class='popup-pager'><ul class='links popup-links'><li><a class='prev' href='#prev'>Prev</a></li><li class='pages'><span class='count'></span></li><li><a class='next' href='#next'>Next</a></li></ul></div>";
            $('.openlayers-popupbox', context).html(text).show();
            Drupal.attachBehaviors($('.openlayers-popupbox', context));
          },

            'over' : function(feature) {

              var context = $(feature.layer.map.div);
              if (!$('.openlayers-popupbox.popup', context).size()) {
                if (feature.attributes.name) {
                  var text = "<div class='openlayers-popupbox'>";
                  text += "<h2 class='popup-title'>" + feature.attributes.name + "</h2>";
                  text += "</div>";
                  context.append(text);
                }else if(feature.cluster[0].attributes.title) {
                  var text = "<div class='openlayers-popupbox'>";
                  text += "<h2 class='popup-title'>" + feature.cluster[0].attributes.title + "</h2>";
                  text += "</div>";
                  context.append(text);
                }
              }

            },

            // Call back for out state.
            'out': function(feature) {
              var context = $(feature.layer.map.div);
              $('.openlayers-popupbox:not(.popup)', context).fadeOut('fast', function() { $(this).remove(); });
            }
          };

      })(jQuery);
