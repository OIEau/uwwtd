<?php
/**
 * @file
 * bootstrap-panel.tpl.php
 *
 * Markup for Bootstrap panels ([collapsible] fieldsets).
 */

?>                   


<div class="front">    
  <div class="flip-image"><img src="<?php print file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png');?>" class="button-flipper table-to-chart" title="See diagram" alt="See diagram"></div>
  <?php print $content; ?>
</div>
