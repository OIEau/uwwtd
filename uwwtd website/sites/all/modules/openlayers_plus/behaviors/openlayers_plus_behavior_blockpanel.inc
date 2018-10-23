<?php
/**
 * @file
 * Inserting all blocks in a region on the map, as a foldable panel
 *
 */

class openlayers_plus_behavior_blockpanel extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    $options = array();
    return $options;
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    $regionlist = system_region_list(variable_get('theme_default'), REGIONS_ALL);
    $options = array();

    foreach ($regionlist as $key => $val) {
      if (substr($key, 0, 12) == 'olblockpanel') {
        $options[$key]['name'] = array(
          '#type' => 'checkbox',
          '#title' => t($val),
          '#description' => t('Enable this region.'),
          '#default_value' => isset($defaults[$key]['name']) ? $defaults[$key]['name'] : NULL,
        );
        $options[$key]['fieldset'] = array(
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#type' => 'fieldset',
          '#title' => t('Settings'),
          '#states' => array(
            // Only showstrstr this field when the region is selected.
            'visible' => array(
              ':input[name="behaviors[openlayers_plus_behavior_blockpanel][options_set][options][' . $key . '][name]"]' => array('checked' => TRUE),
            ),
          ),
        );

        if (!(($library = libraries_detect("jquery-mb-extruder")) && !empty($library['installed']))) {
          $options[$key]['fieldset']['libraries'] = array(
            '#type' => 'markup',
            '#markup' => '<messages class="error">For blockpanel to work, you need the mb.extrduder library installed. ' .
              '</messages>' ,
          );
        }
        if ((variable_get('jquery_update_jquery_version', '') != '1.8')) {
          $options[$key]['fieldset']['jquery'] = array(
            '#type' => 'markup',
            '#markup' => '<messages class="error">The mb-extruder libary needs an updated version of jQuery. You should install the jquery_update module and enable jQuery 1.8. ' .
              '</messages>' ,
          );
        }


        $options[$key]['fieldset']['position'] = array(
          '#type' => 'select',
          '#title' => t('Position'),
          '#description' => t('The position of the panel in the map.'),
          '#options' => array('top' => 'Top', 'bottom' => 'Bottom', 'left' => "Left", 'right' => 'Right'),
          '#default_value' => isset($defaults[$key]['fieldset']['position']) ? $defaults[$key]['fieldset']['position'] : 'left',
        );

        $options[$key]['fieldset']['label'] = array(
          '#type' => 'textfield',
          '#title' => t('Handle Label'),
          '#description' => t('The text in the label. Leave empty to use a background image.'),
          '#default_value' => isset($defaults[$key]['fieldset']['label']) ? $defaults[$key]['fieldset']['label'] : NULL,
        );

        $options[$key]['fieldset']['startstatus'] = array(
          '#type' => 'radios',
          '#title' => t('Start status'),
          '#options' => array('open' => 'Open', 'closed' => 'Closed'),
          '#description' => t('The text in the label. Leave empty to use a background image.'),
          '#default_value' => isset($defaults[$key]['fieldset']['startstatus']) ? $defaults[$key]['fieldset']['startstatus'] : 'closed',
        );

        $options[$key]['fieldset']['slidetimer'] = array(
          '#type' => 'textfield',
          '#title' => t('Slide Timer'),
          '#description' => t('The wait time before the panel folds out after you click the Flap. (in milliseconds)'),
        	'#default_value' => isset($defaults[$key]['fieldset']['slidetimer']) ? $defaults[$key]['fieldset']['slidetimer'] : 500,
          '#size' => 5,
          '#maxlength' => 5,
        );
      }
    }
    return $options;
  }

  /**
   * Render.
   */
  public function render(&$map) {
    if (!(($library = libraries_detect("jquery-mb-extruder")) && !empty($library['installed']))) {
      drupal_set_message("For blockpanel to work, you need the mb.extrduder library installed.", 'error');
    }
    $panels = array();
    $count = 0;

    foreach ($this->options as $region => $active) {
      if ($active['name'] === 1) {
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
        $blocks_content = _block_render_blocks($blocks);
        $build = _block_get_renderable_array($blocks_content);

        if (!empty($blocks)) {
          $panels[$region] = array();

          $rendered_blocks = drupal_render($build);

          $panels[$region] = array(
            'startstatus' => $this->options[$region]['fieldset']['startstatus'],
            'position'    => $this->options[$region]['fieldset']['position'],
            'label'       => $this->options[$region]['fieldset']['label'],
            'markup'      => $rendered_blocks,
            'slidetimer'      => $this->options[$region]['fieldset']['slidetimer'],
          );
          $count = $count + 1;
        }
      }
    }

    $options = $this->options;
    if (!$count) {
    	return $options;
    }
    $options['blockpanels'] = $panels;
    unset($options['regions']);
    drupal_add_js(array('openlayers_plus' => $options), 'setting');
    drupal_add_css(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_blockpanel_toggle.css');
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_blockpanel.js');
    libraries_load('jquery-mb-extruder');
    return $options;
  }
}
