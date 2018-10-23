INTRODUCTION
------------
The OpenLayers Gazetteer module adds a simple new behavior option to 
OpenLayers maps which allows the user to quickly search for a location 
and show it on the map.

The Gazetteer data is provided by GeoNames so you will an account to use 
this module. You can create an account at the following address : 
  http://www.geonames.org/login

REQUIREMENTS
------------
This module requires the following module:
* OpenLayers (https://drupal.org/project/openlayers)

INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

CONFIGURATION
-------------
* Go to "admin/structure/openlayers/gazetteer" and enter your GeoNames username.
  This will be used for each request to the API.
* Go to "admin/structure/openlayers/maps", click "Edit" on a map. 
* Select the "Behavior" tab and enable the "Location search" behavior.
* Save your map and that's it !

TROUBLESHOOTING
---------------
* If the behavior does not appear on the map, try clearing your cache and 
make sure to check you correctly configured your username : 
  "admin/structure/openlayers/gazetteer"

MAINTAINERS
-----------
Current maintainers:
* Damien Chatry (d.chatry) - https://www.drupal.org/user/1839130
