<div class="openlayers_plus-blockswitcher">
  <h3><a onclick="Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.toggleLayerSwitcher();" class="toggle-button-layerswitcher">[ - ]</a> <?php print t('Legend'); ?></h3>
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
