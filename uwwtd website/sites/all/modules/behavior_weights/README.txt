ABOUT
  This is an API module.
  Only enable it if another module requires it, or if you want to use the API
  with your own custom code.

INSTALLATION
  Enable, that's it.

API USAGE
  In your own module's javascript, you can write something like this:

(function(){
  // at the moment this script runs, we do not know if behavior_weights.js has
  // already run or not. Lucky for us, this does not matter.

  // Let this run before other behaviors
  Drupal.behaviors['mymodule_early.weight'] = -10;

  // Let this run after other behaviors
  Drupal.behaviors['mymodule_late.weight'] = 10;

  Drupal.behaviors.mymodule_early = {attach: function(context){
    .. // your stuff to happen.
  }};

  Drupal.behaviors.mymodule_late = {attach: function(context){
    .. // your stuff to happen.
  }};
})();

  The default weight is 0. Anything with a smaller weight will run earlier.
  Anything with a higher weight will run later.

  A note about order of javascript object properties:
  In the javascript spec, there is no guarantee that a "for (var k in x)" loop
  will iterate the attributes of x in the same sequence as they were attached.
  Luckily, almost (?) all browsers are conservative enough, so we don't have to
  worry about this.
  The module tries to behave like a stable sort algorithm: Behaviors with the
  same weight should retain their order.
  If you ever see this fail, you should post a bug report!

  A note on Drupal.behaviors:
  If you want your behavior to run only once on page load, you need to check if
  the context element is the document root.
  For instance, check for $('body', context).length, or wrap your script inside
  $('body', context).each(function(){...});
