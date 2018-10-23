
# WMS Module

The WMS module implements the WMSGetFeatureInfo control as a behaviour for
the openlayers module.
It allows you to query WMS layers of an Openlayers map for details on displayed
features.
It comes with a block where the results are inserted.
It is implemented as a seperate module because it implements 2 menu's.

http://dev.openlayers.org/docs/files/OpenLayers/Control/WMSGetFeatureInfo-js.html


## Requirements

* OpenLayers
* proxy
  This module is needed for the server-side processing implementation of the getfeaturerequest.
  This is how it should be finally implemented. For now the request is done & processed on the client-side, 
  the dependency is included so you dont spend a day wondering why no data is every returned.
  After installing, make sure:
  * The correct users have the 
  * Your preset has a proxy value of 'proxy?request=' 
  * The security settings on the proxy admin page make sense http://example.com/admin/settings/proxy
* If you enable the proxy_openlayers module, you do not have to worry about setting the proxy value on every map. 
  (but you will still need to make sure to add your domain(s) to the whitelist settings on the proxy module settings page)


## ISSUES!

# TODO

* more options in the UI
* add other WMS functionality (including the layer?) like the WFS module
* make the process link use a POST request instead of GET
* Ajaxify the process link
* more TODOs in the code


## Credits

* [batje](http://drupal.org/user/2696)
