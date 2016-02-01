<?php
/**
 * @file
 * Behaviour that shows a layerswitcher that is slightly more stylable
 * than the default openlayers one and is able to show itself next to the
 * map as a standard Drupal block
 */

class openlayers_plus_behavior_blockswitcher_plus extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    $options = array(
      'enabled' => FALSE,
      'position' => 'se',
    );
    return $options;
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    // Get non-base layers.
    $layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if ($layer->data['isBaseLayer'] !== true) {
        $layers[] = $layer->name;
      }
    }

    $configuration_form = array(
      'enabled' => array(
        '#type' => 'checkbox',
        '#title' => t('Show blockswitcher in maps'),
        '#default_value' => isset($defaults['enabled']) ? $defaults['enabled'] : NULL,
      ),
      'open' => array(
        '#type' => 'checkbox',
        '#title' => t('Show blockswitcher open when the map loads'),
        '#default_value' => isset($defaults['open']) ? $defaults['open'] : NULL,
      ),
      'overlay_style' => array(
        '#type' => 'select',
        '#title' => t('Show overlay layers as checkboxes or radio buttons'),
        '#options' => array(
          'checkbox' => t('Checkboxes'),
          'radio' => t('Radio Buttons'),
        ),
        '#default_value' => isset($defaults['overlay_style']) ? $defaults['overlay_style'] : NULL,
      ),
      'sort_layers' => array(
        '#title' => t('Sort layers by groups'),
        '#type' => 'checkbox',
        '#default_value' => isset($defaults['sort_layers']) ? $defaults['sort_layers'] : array(),
      )
    );

    foreach ($layers as $key => $layer_name) {
      $configuration_form['layers_' . $layer_name] = array(
        '#title' => $layer_name,
        '#type' => 'checkbox',
        '#options' => $layer_name,
        '#default_value' =>  isset($defaults['layers_' . $layer_name]) ? $defaults['layers_' . $layer_name] : array(),
        '#states' => array(
          'visible' => array(':input[name="behaviors[openlayers_plus_behavior_blockswitcher_plus][options_set][options][sort_layers]"]' => array('checked' => TRUE)),
        ),
      );
      $configuration_form['layers_' . $layer_name . '_groupname'] = array(
        '#title' => t('Group name for') . ' : ' . $layer_name,
        '#type' => 'textfield',
        '#default_value' => isset($defaults['layers_' . $layer_name . '_groupname']) ? $defaults['layers_' . $layer_name . '_groupname'] : array(),
        '#states' => array(
          'visible' => array(':input[name="behaviors[openlayers_plus_behavior_blockswitcher_plus][options_set][options][layers_' . $layer_name . ']"]' => array('checked' => TRUE)),
        ),
      );
    }

    return $configuration_form;
  }

  /**
   * Render.
   */
  public function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers_plus')
      . '/behaviors/openlayers_plus_behavior_blockswitcher_plus.js');
    drupal_add_js('misc/jquery.cookie.js', 'file');drupal_add_library('system', 'drupal.collapse');

    // Format groups.
    $groups = array();
    foreach ($map['behaviors']['openlayers_plus_behavior_blockswitcher_plus'] as $key => $group_name) {
      if(strpos($key, '_groupname') !== false) {
        $layer_name = str_replace('_groupname', '', str_replace('layers_', '', $key));
        if($map['behaviors']['openlayers_plus_behavior_blockswitcher_plus']['layers_' . $layer_name] == 1) {
          $groups[$group_name][] = $layer_name;
        }
      }
    }

    drupal_add_js(array('OpenLayersPlusBlockswitcherPlus' => $groups), 'setting');
    if ($this->options['enabled']) {
      $block = module_invoke('openlayers_plus', 'block_view', 'blockswitcher_plus');
      $this->options['block'] = drupal_render($block['content']);
    }
    return $this->options;
  }
}
