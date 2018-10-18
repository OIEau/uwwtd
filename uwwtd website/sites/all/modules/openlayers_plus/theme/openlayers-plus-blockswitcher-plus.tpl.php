<div class="openlayers_plus-blockswitcher" style="display:none;">
  <div class="toggle-button-layerswitcher-container"><a class="toggle-button-layerswitcher" title="<?php print t('Layer switcher'); ?>" data-state="-">&nbsp;</a><div class="ol-plus-label"></div></div>
  <div class='layer-switcher'>
    <div class='layers base'>
        <?php 
          $fieldset = theme_fieldset(
            array('element' => 
              array('#title' => t('Base layers'), 
                '#children' => '<div class="layers-content"></div>',
                '#collapsible' => true, 
                '#collapsed' => true)
              )
            );
          print $fieldset;
        ?>
      </div>
      <div class='layers groups'>
        <?php 
          $fieldset = theme_fieldset(
            array('element' => 
              array('#title' => t('Overlay'),
                '#children' => '<div class="layers-content"></div>', 
                '#collapsible' => true
                )
              )
            );
          print $fieldset;
        ?>
      </div>
    
      <div style='display:none' class='factory'>
        <div class='form-item form-option radio'>
          <label class='option'></label>
        </div>
        <div class='form-item form-option checkbox'>
          <label class='option'><span class='key'></span><input class='form-checkbox' type='checkbox'/></label>
        </div>
      </div>
  </div>
</div>
