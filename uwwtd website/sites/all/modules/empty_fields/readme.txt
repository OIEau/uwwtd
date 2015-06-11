Empty fields module - http://drupal.org/project/empty_fields
============================================================

DESCRIPTION
------------
This module provides a way to show fields that are empty by new display
formatter settings. These can be either custom text defined by the field
administrator or the return value of a defined callback.

REQUIREMENTS
------------
Drupal 7.x
Field formatter settings - http://drupal.org/project/field_formatter_settings

INSTALLATION
------------
1.  Place the Empty Fields modules into your modules directory.
    This is normally the "sites/all/modules" directory.
    Also download and place the Field formatter settings module here too.
    http://drupal.org/project/field_formatter_settings


2.  Go to admin/build/modules. Enable the modules.
    The Empty Fields modules is found in the Fields section.

Read more about installing modules at http://drupal.org/node/70151

API
---
For specific use-cases, you can define a custom callback to generate dynamic
content.

Firstly, implement hook_empty_fields(). This returns an array indexed by the
class name that implements the handler.

This differs from version 1.x that used hook_empty_field_callbacks().

<?php
/**
 * Implements hook_empty_fields().
 */
function HOOK_empty_fields() {
  $items = array(
    'CurrentTimeEmptyFieldText' => array(
      'title' => t('Display current time if empty'),
    ),
  );
  return $items;
}
?>

Create a new concrete class that extends the abstract class EmptyFieldHandler.

<?php
/**
 * @file
 * Contains the CurrentTimeEmptyFieldText plugin for EmptyFieldHandler.
 */

/**
 * Defines CurrentTimeEmptyFieldText class.
 */
class CurrentTimeEmptyFieldText extends EmptyFieldHandler {

  /**
   * Implementation of EmptyFieldText::react().
   */
  public function react($context) {
    return format_date(time());
  }

  /**
   * Implementation of EmptyFieldText:summaryText().
   */
  public function summaryText() {
    return t('Empty Text: current time');
  }
}

?>

Register this class in your modules info file

<code>
files[] = plugins/empty_fields_handler_current_time.inc
</code>

The context has the entity_type, entity, view_mode as well as the empty field
details, field and instance. 

If the callback is empty or a zero-length string, the empty field will not be
rendered.

ACKNOWLEDGEMENTS
----------------
Core implementation was based of Field Delimiter module by Andrew Macpherson,
(andrewmacpherson) and builds on the base concept started by Everett Zufelt in
a support request at http://drupal.org/node/1283974#comment-5385242.

It is made possible with Dave Reids Field formatter settings module that plugs
a couple of the holes found in the core Drupal Field API.

AUTHORS
-------
Alan D. - http://drupal.org/user/198838
rypit   - http://drupal.org/user/868380

REFERENCES
----------

Andrew Macpherson - http://drupal.org/user/265648
Everett Zufelt - http://drupal.org/user/406552
Dave Reid - http://drupal.org/user/53892
Field Delimiter - http://drupal.org/project/field_delimiter
Field formatter settings - http://drupal.org/project/field_formatter_settings
