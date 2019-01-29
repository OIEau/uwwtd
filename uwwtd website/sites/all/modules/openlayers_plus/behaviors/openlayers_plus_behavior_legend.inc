<?php
/**
 * @file
 * A block in a corner of a map that provides information on layers
 */

class openlayers_plus_behavior_legend extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    return array(
      'layers' => array(),
    );
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
    $vector_layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if (isset($layer->data['vector']) && $layer->data['vector'] == TRUE) {
        $vector_layers[$id] = $name;

      }
    }

    $options['layers'] = array(
      '#title' => t('Layers'),
      '#type' => 'checkboxes',
      '#options' => $vector_layers,
      '#description' => t('Select layers to add legend.'),
      '#default_value' => isset($defaults['layers']) ? $defaults['layers'] : array(),
    );

    return $options;

  }


  /**
   * Render.
   */
  public function render(&$map) {
	//dsm($map);
    $legend_title = null;
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_legend.js');
    drupal_add_css(drupal_get_path('module', 'openlayers_plus') . '/css/openlayers_plus.css');
    $options = array();
    $styles = openlayers_styles();
    uasort($map['layers'], 'openlayers_plus_layer_compare');    
    foreach ($map['layers'] as $layer => $layer_data) {
      if (isset($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]) && $map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]) {
        $selected_layer_style = $map['layer_styles'][$layer];
        if ($selected_layer_style == '0') {
          $selected_layer_style = "default";	  
        }
        else {
          $selected_layer_style = $map['layer_styles'][$layer];
        }
        $legend = array();

        if (!empty($map['layers'][$layer]['title'])) {
          $legend_title = $map['layers'][$layer]['title'];
        }
        else {
          $legend_title = $styles[$selected_layer_style]->title;
        }
        $legend[$layer]['title'] = $legend_title;
        $legend[$layer]['data'] = $styles[$selected_layer_style]->data;
        $options[$layer] = theme('openlayers_plus_legend',
            array(
              'legend' => $legend,
              'layer' => $layer_data,
              'layer_id' => $layer,
            ));
        //dsm($layer);
            
        /*
        dsm($legend_title);
        
        dsm($layer_data);
        dsm($legend[$layer]['data']);
        */
        //dsm($options[$layer]);
		//Modif nd@oieau.fr prise en compte des légendes avec scale point
		if(isset($map['behaviors']['openlayers_plus_behavior_scalepoints']) && !empty($map['behaviors']['openlayers_plus_behavior_scalepoints']) && $map['behaviors']['openlayers_plus_behavior_scalepoints']['layers'][$layer] != '0'){
            $additional_legend = '';
            
            //Gestion des cas on l'on passe un style directement dans la def de la couche
            if(is_array($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]) && isset($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]['openlayers_plus_behavior_scalepoints'])){
                $additional_legend.= $this->get_scale_point_legend($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]['openlayers_plus_behavior_scalepoints']); 
            }
            else{
                $additional_legend.= $this->get_scale_point_legend($map['behaviors']['openlayers_plus_behavior_scalepoints']);
                //Si on a des couleurs 
                // dsm($map['behaviors']['openlayers_plus_behavior_scalepoints']);
                $colors_legend = '';
                if($map['behaviors']['openlayers_plus_behavior_scalepoints']['fields_colors'] !='' && $map['behaviors']['openlayers_plus_behavior_scalepoints']['fields_colors_values'] !=''){
                    
                    $colors = $this->get_textarea_field_values($map['behaviors']['openlayers_plus_behavior_scalepoints']['fields_colors_values']);
                    $colors_legend='';
                    foreach($colors as $color){
                        $colors_legend.='<div><div style="background-color:'.$color[2].';border:1px solid #222222;width:20px;height:12px;float:left;margin-right:7px;">&nbsp;</div> <div>'.$color[1].'</div></div>';
                    }
                }
                $additional_legend.='<div>'.$colors_legend.'</div>';		
			}
			if($additional_legend!='') $options[$layer] = substr(trim($options[$layer]), 0, -6) .$additional_legend.'</div>';
		}

		if(isset($map['behaviors']['openlayers_plus_behavior_piechart']) && !empty($map['behaviors']['openlayers_plus_behavior_piechart'])){
			$additional_legend = '';
			$additional_legend.= $this->get_scale_point_legend($map['behaviors']['openlayers_plus_behavior_piechart']);
			if($map['behaviors']['openlayers_plus_behavior_piechart']['fields_series']!=''){
				$series = $this->get_textarea_field_values($map['behaviors']['openlayers_plus_behavior_piechart']['fields_series']);
				$series_legend = '';
				foreach($series as $serie){
					$series_legend.='<div><div style="background-color:'.$serie[2].';border:1px solid #222222;width:20px;height:12px;float:left;margin-right:7px;">&nbsp;</div> <div>'.$serie[1].'</div></div>';
				}
			}
			$additional_legend.='<div>'.$series_legend.'</div>';
			if($additional_legend!='') $options[$layer] = substr(trim($options[$layer]), 0, -6) .$additional_legend.'</div>';
		}
        
        //Gestion des couches avec un style Openlayers
        if(is_array($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]) && isset($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]['styles'])){
            $additional_legend = '';            
            foreach($map['behaviors']['openlayers_plus_behavior_legend']['layers'][$layer]['styles'] as $style){
                if(isset($styles[$style]) && !empty($styles[$style]->data)){
                    $additional_legend .= '<div><div style="background-color:'.$styles[$style]->data['fillColor'].';border:1px solid #222222;width:20px;height:12px;float:left;margin-right:7px;">&nbsp;</div><div>'.t($styles[$style]->title) .'</div></div>';
                }
            }
            if($additional_legend!='') $options[$layer] = substr(trim($options[$layer]), 0, -6) .$additional_legend.'</div>';
        }
      }
    }
    return $options;
  }
  
  protected function get_scale_point_legend($scalepoints){
	$output = '';
	
	$scalepoints_legend='';
	if(isset($scalepoints['distinct']) && $scalepoints['distinct']!='' && $scalepoints['distinct'] > 0){
		$min_radius = $scalepoints['min']['pointRadius'];
        $max_radius = $scalepoints['max']['pointRadius'];
        $pointdiff = ($max_radius - $min_radius) / $scalepoints['distinct'];
        
        if($scalepoints['min']['value'] && $scalepoints['max']['value']){
            $min_value = $scalepoints['min']['value'];
            $max_value = $scalepoints['max']['value'];
        }
        else{
            $min_value = 0;
            $max_value = 1000;
        }
        $valdiff = ($max_value - $min_value) / $scalepoints['distinct'];
		
		if(trim($scalepoints['arbitrary_classes'])!=''){
			$classes = $this->get_classes($scalepoints['arbitrary_classes']);
		}
		
		for ($i = 0; $i <  $scalepoints['distinct']; $i++) {
			$r= $min_radius +($i*$pointdiff);
			$cy =$max_radius + 4 +($pointdiff*($scalepoints['distinct']-$i));
			
			if(!empty($classes)){
				$class_value = ($classes[$i]['max']!=''?$classes[$i]['max']:'>'.$classes[$i]['min']);
			}
			else{
				$class_value = $min_value + ($valdiff * ($i+1) );
				$class_value = $class_value - $class_value%100;
			}
			//Ajoute le "et +" pour la dernière class 
			$class = $class_value .(  $scalepoints['distinct'] == $i+1 ?' +':'');
			
			
			$scalepoints_legend.='
			<g>
				<circle cx="'.$max_radius.'" cy="'.$cy.'" r="'.$r.'" stroke-width="1" fill-opacity="0.6" fill="'.(isset($scalepoints['default_color'])?$scalepoints['default_color']:'#aaaaaa').'" stroke="#222222" stroke-linecap="round" stroke-linejoin="round"/>
				<text dx="'.($max_radius*2 ).'" dy="'.( ($max_radius*2 +5) - ($r*2) ).'" font-size="7" alignment-baseline="baseline">'. $class .'</text>
			</g>';
		}
	}
	$scalepoints_legend_item =  $scalepoints['fields'];
	$field_label='';
	
	//We try to give a label for the field of scale point
	if($field_info = field_info_field($scalepoints['fields'])){
		//dsm($field_info);
		if($field = field_info_instance('node', $scalepoints['fields'], $field_info['bundles']['node'][0])){
			$field_label = $field['label'];
		}
	}
	$output.='<div>
		<svg xmlns="http://www.w3.org/2000/svg" width="'. ($max_radius*2+35) .'" height="'.($max_radius*2+5).'"><g>'.$scalepoints_legend.'</g></svg><span class="scalepoints-legend-item">'.($field_label!=''?$field_label:$scalepoints_legend_item).'</span>
	</div>';
  
	return $output;
  
  }
  
  protected function get_textarea_field_values($colors_values){
		$values = explode("\n", $colors_values);
		$options=array();
		foreach($values as $value){
			$tmp = explode("|",trim($value));
			$options[$tmp[0]]=$tmp;
		}
		return $options;
	}
  protected function get_classes($classes_values){
	$classes = array();
	if(trim($classes_values)!=''){
		$arbitrary_classes=explode("\n", trim($classes_values));
		foreach($arbitrary_classes as $c){
			$cl=explode('|', trim($c));
			$classes[] =array(
				'min'=>trim($cl[0]),
				'max'=>trim($cl[1])
			);
		}
	}
	return $classes;
  }
}
