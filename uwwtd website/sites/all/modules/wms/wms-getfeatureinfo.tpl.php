<div class="layers" style="min-width:300px">
<?php
// $Id$

/**
 * @file
 * Theme implementation to display getfeatureinfo result
 *
 */
if (isset($tables)) {
  foreach ($tables as $name => $table) {
    $name = str_replace(":", " ", $name);
    print '<div class="' . drupal_html_class($name) . '">';
    print theme_table(
      Array(
        'header' => $table['header'],
        'rows' => $table['rows'],
        'attributes' => array('class' => array(drupal_html_class($name), 'wms-ol-table-result', 'table-striped')),
        'caption' => $name,
        'empty' => t("No results found")
      )
    );
    print '</div>';
  }
}
else {
  print t("No results found");
}
?>
</div>
