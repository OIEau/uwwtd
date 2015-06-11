<?php
/**
 * @file
 * Inserting all blocks in a region on the map, with toggle buttons for
 * each block
 */
class openlayers_plus_behavior_maptext extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    $options = array(
      'verticalposition' => 'top',
      'verticalposition' => 'right',
    );
    return $options;
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    $regionlist = system_region_list(variable_get('theme_default'), REGIONS_ALL);
    $options = array();
    foreach($regionlist as $key => $val) {
      if (substr($key, 0, 9) == 'olmaptext') {
        $options[$key]['name'] = array(
          '#type' => 'checkbox',
          '#title' => t($val),
          '#description' => t('Enable this region.'),
          '#default_value' => isset($defaults[$key]['name']) ? $defaults[$key]['name'] : NULL
        );

        $options[$key]['fieldset'] = array(
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#type' => 'fieldset',
          '#title' => t('Toggle blocks in region @val', array(
              '@val' => $val
          )),
          '#states' => array(
              // Only show this field when the region is selected.
              'visible' => array(
                  ':input[name="behaviors[openlayers_plus_behavior_maptext][options_set][options][' . $key . '][name]"]' => array(
                      'checked' => TRUE
                  )
              )
          )
        );

        $options[$key]['fieldset']['showpanel'] = array(
          '#type' => 'checkbox',
          '#title' => t('Show Toggle Panel'),
          '#description' => t('Show a panel with toggle buttons for each block.'),
          '#default_value' => isset($defaults[$key]['fieldset']['showpanel']) ? $defaults[$key]['fieldset']['showpanel'] : NULL,
        );

        $options[$key]['fieldset']['panelfieldset'] = array(
          '#collapsible' => FALSE,
          '#collapsed' => TRUE,
          '#type' => 'fieldset',
          '#title' => t('Panel Settings'),
          '#states' => array(
            // Only show this field when the region is selected.
            'visible' => array(
              ':input[name="behaviors[openlayers_plus_behavior_maptext][options_set][options][' . $key . '][fieldset][showpanel]"]' => array(
                'checked' => TRUE
              )
            )
          )
        );

        $options[$key]['fieldset']['panelfieldset']['toggleindividually'] = array(
          '#type' => 'checkbox',
          '#title' => t('Toggle Blocks Indiviually'),
          '#description' => t('When checked blocks will be toggled on and off on by one.'),
          '#default_value' => isset($defaults[$key]['fieldset']['panelfieldset']['toggleindividually']) ? $defaults[$key]['fieldset']['panelfieldset']['toggleindividually'] : FALSE,
          '#states' => array(
              // Only show this field when the region is selected.
              'visible' => array(
                  ':input[name="behaviors[openlayers_plus_behavior_maptext][options_set][options][' . $key . '][fieldset][panelfieldset][showpanel]"]' => array(
                      'checked' => TRUE
                  )
              )
          )
        );

        $options[$key]['fieldset']['panelfieldset']['verticalposition'] = array(
          '#type' => 'select',
          '#title' => t('Vertical position of the panel.'),
          '#default_value' => isset($defaults[$key]['fieldset']['panelfieldset']['verticalposition']) ? $defaults[$key]['fieldset']['panelfieldset']['verticalposition'] : NULL,
          '#options' => array(
            'top' => 'Top',
            'bottom' => 'Bottom'
          )
        );

        $options[$key]['fieldset']['panelfieldset']['horizontalposition'] = array(
          '#type' => 'select',
          '#title' => t('Horizontal position of panel.'),
          '#default_value' => isset($defaults[$key]['fieldset']['panelfieldset']['horizontalposition']) ? $defaults[$key]['fieldset']['panelfieldset']['horizontalposition'] : NULL,
          '#options' => array(
            'left' => 'Left',
            'right' => 'Right'
          )
        );

        $options[$key]['fieldset']['verticalposition'] = array(
          '#type' => 'select',
          '#title' => t('Vertical position of the blocks.'),
          '#default_value' => isset($defaults[$key]['fieldset']['verticalposition']) ? $defaults[$key]['fieldset']['verticalposition'] : NULL,
          '#options' => array(
              'top' => 'Top',
              'bottom' => 'Bottom'
          )
        );

        $options[$key]['fieldset']['horizontalposition'] = array(
          '#type' => 'select',
          '#title' => t('Horizontal position of the blocks.'),
          '#default_value' => isset($defaults[$key]['fieldset']['horizontalposition']) ? $defaults[$key]['fieldset']['horizontalposition'] : NULL,
          '#options' => array(
              'left' => 'Left',
              'right' => 'Right'
          )
        );
        $options[$key]['fieldset']['popup'] = array(
          '#type' => 'checkbox',
          '#title' => t("Add block for Popup"),
          '#description' => t('Add an additional block for the popup that will toggle together with the other blocks.'),
          '#disabled' => false,
          '#default_value' => isset($defaults[$key]['fieldset']['popup']) ? $defaults[$key]['fieldset']['popup'] : NULL
        );

        $options[$key]['fieldset']['popupfieldset'] = array(
          '#collapsible' => FALSE,
          '#collapsed' => TRUE,
          '#type' => 'fieldset',
          '#title' => t('Popup Settings'),
          '#states' => array(
            // Only show this field when the region is selected.
            'visible' => array(
              ':input[name="behaviors[openlayers_plus_behavior_maptext][options_set][options][' . $key . '][fieldset][popup]"]' => array(
                'checked' => TRUE
              )
            )
          )
        );
        // Only prompt for vector layers.
        $vector_layers = array();
        foreach ($this->map['layers'] as $id => $name) {
          $layer = openlayers_layer_load($id);
          if (isset($layer->data['vector']) && $layer->data['vector'] == TRUE) {
            $vector_layers[$id] = $name;
          }
        }

        $options[$key]['fieldset']['popupfieldset']['layers'] = array(
          '#title' => t('Layers'),
          '#type' => 'checkboxes',
          '#options' => $vector_layers,
          '#description' => t('Select layer to apply popups to.'),
          '#default_value' => isset($defaults[$key]['fieldset']['popupfieldset']['layers']) ? $defaults[$key]['fieldset']['popupfieldset']['layers'] : array(),
        );


      }
    }

    return $options;
  }

  /**
   * Render.
   */
  public function render(&$map) {

    $count = 0;
    // you can only use maptext as a popup once
    $usedaspopup = false;
    $options = array();
    foreach($this->options as $region => $active) {
      if ($active['name'] === 1) {
      $options[$region] = $active['fieldset'];

        $blocks = block_list($region);
        if (module_exists('context')) {
          $reaction = context_get_plugin('reaction', 'block');
          $contextblocks = $reaction->block_list($region);
          /*
           * This is a nasty workaround for an issue described here
           * https://drupal.org/comment/4109802#comment-4109802
           * And having its own open issue here
           * https://drupal.org/node/2107877
           */
          drupal_static_reset('context_reaction_block_list');
          $blocks = array_merge($contextblocks, $blocks);
        }
        if (!empty($blocks)) {
          foreach($blocks as $key => $block) {
            // render the block without title, to prevent repetition
            $title = !empty($block -> subject) ? $block -> subject : "None";
            $block->title = "";
            $block->subject = "";

            $blocks_content = _block_render_blocks(array($block));
            $build = _block_get_renderable_array($blocks_content);
            $rendered_blocks = drupal_render($build);

            $options[$region]['blocks'][$key] = array(
              'id' => $key,
              'title' =>  $title,
              'markup' => $rendered_blocks,
              'horizontalposition' => $this->options[$region]['fieldset']['horizontalposition'] ,
              'verticalposition' => $this->options[$region]['fieldset']['verticalposition'],
              'showclosebox' => ($this->options[$region]['fieldset']['showpanel'] == 1) ? 1 : 0,
              'toggle' => ($options[$region]['panelfieldset']['toggleindividually'] == 1),
            );
            $count++;
          }
        }

        if ((!$usedaspopup) && ($this->options[$region]['fieldset']['popup'] == 1)) {
          $options[$region]['blocks']["olplusmaptextpopup"] = array(
            'id' => "olplusmaptextpopup",
            'title' =>  "",
            'markup' => "",
            'horizontalposition' => $this->options[$region]['fieldset']['horizontalposition'] ,
            'verticalposition' => $this->options[$region]['fieldset']['verticalposition'],
            'showclosebox' => 1,
            'toggle' => false,
          );
          $count++;
          $usedaspopup = true;
        }
      }
    }

    if (!$count) {
    	return $this->options;
    }

    drupal_add_library('system', 'jquery.cookie', TRUE);


    drupal_add_library('system', 'ui');
    drupal_add_library('system', 'ui.widget');
    drupal_add_library('system', 'ui.mouse');
    drupal_add_library('system', 'ui.draggable');

    drupal_add_js( array (
        'openlayers_plus' => array (
          'maptext' => $options
        ),
      ),
      'setting'
    );
    drupal_add_css(drupal_get_path('module', 'openlayers_plus') . '/theme/openlayers_plus_behavior_maptext.css');
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_maptext.js');

    return $options;
  }
}
