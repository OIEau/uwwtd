<div class='openlayers_plus-blockswitcher clear-block'>
  <a onclick="Drupal.behaviors.OpenLayersPlusBlockswitcherPlus.toggleLayerSwitcher();" class="toggle-button-layerswitcher">[ - ] Hide</a>
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
              array('#title' => t('Data layers'),
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
