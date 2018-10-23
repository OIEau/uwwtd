 Proxy Description
====================================
Proxy module for Drupal.  Creates an API to
proxy heep requests (web sites).

http://drupal.org/project/proxy


 Installation
====================================
1) Regular Drupal module installation.
2) Go to admin/settings/proxy
3) Add domains to whitelist as needed (or turn off
   the whitelist feature if you must)


 API
====================================
$content_object = proxy($params);

This is the core proxy API function.  The basic
param is the "request_uri".

The module also provides a menu call back.  This
can be helpful when trying to do cross-domain
scripting in JS.  Example path:

proxy?request=http://drupal.org


 Status
====================================
The bulk of the functionality of this module
is working, but it is ALPHA, so don't have
high expectations.



 Credits
====================================
zzolo (Alan Palazzolo): http://drupal.org/user/147331
