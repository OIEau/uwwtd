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
  <p style="font-weight:bold;"><?php print t("Load and concentration per parameter");?> :</p>	


  <?php print $content; ?>
</div>
