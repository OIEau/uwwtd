<?php
/**
 * @file
 * Legend template.
 */

/**
 * @param $layer_id The layer's id
 * @param $layer layer array
 * @param $legend an array of legend items
 */
?>
<div class="toggleLegend-container"><a class="toggleLegend" data-state="-" title="<?php print t("Legend"); ?>">&nbsp;</a><div class="ol-plus-label"></div></div>

<div class='legend legend-count-<?php print count($legend) ?> clear-block' id='openlayers-legend-<?php print $layer_id ?>'>
  <?php foreach ($legend as $key => $item): ?>
    <?php if (!empty($item['data']['fillColor']) && !empty($item['data']['strokeColor']) && !isset($item['data']['externalGraphic'])): ?>    
        <div class="legend-item clear-block">
          <?php print check_plain($item['title']) ?>
        </div>
    <?php endif; ?>      
    <?php if (!empty($item['data']['externalGraphic'])): ?>    
      <div class="legend-item clear-block">
      <img class="swatch" src='<?php print file_create_url($item['data']['externalGraphic']); ?>' style='height: 15px; width: <?php print 'auto'; ?>'/>
      <?php if (!empty($layer['title'])): ?>
        <?php print check_plain($item['title']) ?>
      <?php endif; ?>
      </div>      
    <?php endif; ?>
  <?php endforeach; ?>
</div>
