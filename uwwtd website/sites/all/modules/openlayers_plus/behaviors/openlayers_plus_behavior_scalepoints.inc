<?php
/**
 * @file
 * Dynamic styling, changing point radii based on a certain value.
 */

class openlayers_plus_behavior_scalepoints extends openlayers_behavior {
  /**
   * Override of options_init().
   */
  public function options_init() {
    $options['min'] = array(
      'pointRadius' => 5,
      'value' => 0,
    );
    $options['max'] = array(
      'pointRadius' => 20,
      'value' => '50',
    );
    $options['distinct'] = 5;
    $options['min_label'] = -1;
    $options['method'] = 'area';
    // @TODO: This sucks. Get a submission/validation handling into the
    // behaviors base class and fix this.
    $options['fields'] = array('count, weight');
    return $options;
  }

  /**
   * Override of options_form().
   */
  public function options_form($defaults = array()) {
  
	$options = array(
      'min' => array(
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#type' => 'fieldset',
        '#title' => t('Minimum values'),
      ),
      'max' => array(
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#type' => 'fieldset',
        '#title' => t('Maximum values'),
      ),
    );
    foreach (array('min', 'max') as $key) {
      $options[$key]['value'] = array(
        '#title' => t('Value'),
        '#description' => t('Enter a fixed value for this field or leave empty to autoscale points.'),
        '#type' => 'textfield',
        '#size' => 4,
        '#default_value' => isset($defaults[$key]['value']) ? $defaults[$key]['value'] : NULL,
      );
      $options[$key]['pointRadius'] = array(
        '#title' => t('Radius'),
        '#type' => 'textfield',
        '#size' => 4,
        '#default_value' => isset($defaults[$key]['pointRadius']) ? $defaults[$key]['pointRadius'] : NULL,
      );
      $options[$key]['fillOpacity'] = array(
        '#title' => t('Fill opacity'),
        '#type' => 'textfield',
        '#size' => 4,
        '#maxlength' => 4,
        '#default_value' => isset($defaults[$key]['fillOpacity']) ? $defaults[$key]['fillOpacity'] : NULL,
      );
      $options[$key]['strokeWidth'] = array(
        '#title' => t('Stroke width'),
        '#type' => 'textfield',
        '#size' => 4,
        '#maxlength' => 4,
        '#default_value' => isset($defaults[$key]['strokeWidth']) ? $defaults[$key]['strokeWidth'] : NULL,
      );

    };
    $options['distinct'] = array(
      '#title' => t('Number of distinct sizes'),
      '#type' => 'textfield',
      '#default_value' => isset($defaults['distinct']) ? $defaults['distinct'] : NULL,
    );
	$options['arbitrary_classes']=array(
      '#title' => t('Arbitrary classes'),
      '#description' => t('Enter list of arbitrary classes with this format min|max. Enter one class per line. You can let blank the last max value for unbounded the series'),
      '#type' => 'textarea',
      '#default_value' => isset($defaults['arbitrary_classes']) ? $defaults['arbitrary_classes'] : NULL,
    );
	
	
	$options['method'] = array(
      '#title' => t('Scaling method'),
      '#type' => 'select',
      '#options' => array(
        'area' => t('Circle area'),
        'radius' => t('Circle radius'),
      ),
      '#default_value' => isset($defaults['method']) ? $defaults['method'] : NULL,
    );
	
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
    $options['fields'] = array(
      '#title' => t('Fields'),
      '#description' => t('Enter a comma separated list of attribute fields that can be used for scaling points. The first found attribute will be used.'),
      '#type' => 'textfield',
      '#default_value' => isset($defaults['fields']) ? $defaults['fields'] : NULL,
    );
    $options['min_label'] = array(
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#type' => 'textfield',
      '#title' => t('Minimum weight point to label'),
      '#default_value' => isset($defaults['min_label']) ? $defaults['min_label'] : NULL,
      '#description' => t('Don"t show labels below a certain point weight'),
    );
	$options['fields_colors'] = array(
      '#title' => t('Fields colors'),
      '#description' => t('Enter a comma separated list of attribute fields that can be used for filling the circle. The first found attribute will be used. You can use "+" for aggregate values of two distinct field : field_1+field_2,field_3'),
      '#type' => 'textfield',
      '#default_value' => isset($defaults['fields_colors']) ? $defaults['fields_colors'] : NULL,
    );
	$options['fields_colors_values'] = array(
      '#title' => t('Fields colors'),
      '#description' => t('Enter list of value with color code eg: "class value|label|#000000" for filling the circle. The label will be use in the legend. Enter on class per line. Note : the class value is the rendered value of the field without space : "My rendered value" become "Myrenderendvalue"'),
      '#type' => 'textarea',
      '#default_value' => isset($defaults['fields_colors_values']) ? $defaults['fields_colors_values'] : NULL,
    );
    return $options;
  }

  /**
   * Generate weight segments from the number of distinct values.
   */
  public function get_weights() {
    if(trim($this->options['arbitrary_classes'])!=''){
		$arbitrary_classes = $this->get_classes($this->options['arbitrary_classes']);
		$this->options['distinct'] = count($arbitrary_classes);
	}
	elseif (!is_numeric($this->options['distinct']) || $this->options['distinct'] < 1) {
      $this->options['distinct'] = 5;
    }
    $size = number_format(1 / $this->options['distinct'], 2);
    $weights = array();
    for ($i = 1; $i < $this->options['distinct']; $i++) {
      $key = (string) ($i * $size);
      $weights[$key] = $i;
    }
    $weights[1] = $this->options['distinct'];
    return $weights;
  }

  /**
   * Generate a weight => pointRadius mapping.
   */
  public function get_styles() {
    $weights = array_values($this->get_weights());
    $styles = array();
    foreach (array_keys($this->options['min']) as $key) {
      if ($key != 'value' && (!empty($this->options['min'][$key]) ||  !empty($this->options['max'][$key]))) {
        $pointdiff = ($this->options['max'][$key] - $this->options['min'][$key]) / $this->options['distinct'];
        $pointdiff = number_format($pointdiff, 2);

        foreach ($weights as $weight) {
          $styles[$weight][$key] = $this->options['min'][$key] + ($weight - 1) * $pointdiff;
          if ($weight <= $this->options['min_label']) {
            $styles[$weight]['label'] = '';
          }
        }
        $styles[$this->options['distinct']][$key] = $this->options['max'][$key];
      }
    }
    return $styles;
  }
  public function get_colors(){
	$color_values = $this->fields_colors_values();
	$colors = array();
	if(!empty($color_values)){
		foreach($color_values as $value){
			$colors[$value[2]] = array('fillColor'=>$value[2]);
		}
	}
    if(isset($this->options['addColors'])){
        foreach($this->options['addColors'] as $value){
			$colors[$value] = array('fillColor'=>$value);
		}
    }
    return $colors;
  }
  
  public function get_layers(){
	$layers = array();
	if(!empty($this->options['layers'])){
		foreach($this->options['layers'] as $key => $layer_name){
			if($layer_name) $layers[] = $layer_name;
		}
	}
	return $layers;
  }
  
  protected function get_classes(){
	$classes = array();
	if(trim($this->options['arbitrary_classes'])!=''){
		$arbitrary_classes=explode("\n", trim($this->options['arbitrary_classes']));
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
  /**
   * Retrieve the first found usable field from a set of features.
   */
  protected function get_field($features) {
    $fields = explode(',', $this->options['fields']);
    foreach ($fields as $k => $v) {
      $fields[$k] = trim($v);
    }
    foreach ($features as $feature) {
      foreach ($fields as $field) {
        if (isset($feature['attributes'][$field])) {
          return $field;
        }
      }
    }
    return FALSE;
  }

  protected function get_fields_colors($features) {
    $fields = explode(',', $this->options['fields_colors']);
    foreach ($fields as $k => $v) {
      $fields[$k] = explode('+',trim($v));
    }
    foreach ($features as $feature) {
      foreach ($fields as $field) {
		//If field is an array ==> we need to check if each value are in feature
		if(is_array($field)){
			$flag_match=true;
			foreach($field as $f){
				if(!isset($feature['attributes'][$f])){$flag_match=false;}
			}
			if($flag_match) return $field;
		}
        else{
			if (isset($feature['attributes'][$field])) {
			  return $field;
			}
		}
      }
    }
    return FALSE;
  }
  
  protected function fields_colors_values(){
	
	$options=array();
	if(trim($this->options['fields_colors_values'])!=''){
		$values = explode("\n", $this->options['fields_colors_values']);
		foreach($values as $value){
			$tmp = explode("|",$value);
			$options[$tmp[0]]=$tmp;
		}
	}
	return $options;
  }
  
  
  /**
   * Retrieve map layers.
   */
  protected function get_vector_layers($layers) {
    $vector_layers = array();
    foreach ($layers as $key => $layer) {
        
      // Get type == Vector for backwards-compatibility.
      // TODO: After OpenLayers alpha8 it should be removed.
      if (((isset($layer['vector']) && $layer['vector'] == 1)) && !empty($layer['features'])) {
        $vector_layers[$key] = $layer;
      }
    }
    return $vector_layers;
  }

  /**
   * Process weights.
   */
  protected function process_weight($ratio, $method) {
    switch ($method) {
      case 'radius':
        return $ratio;

      case 'area':
      default:
        return sqrt($ratio);

    }
  }
  protected function get_field_value($string){
	$value ='';
	//The tag can be a div or a span
	$start_pos = strpos($string, 'class="field-content">');
	if($start_pos>-1){
		$string = substr($string, $start_pos+22);
		$end_pos = strpos($string, '</');
		if($end_pos>-1){$string = substr($string, 0, $end_pos);}
	}
	$value = preg_replace('/\s+/', '', strip_tags($string));
	return $value;
  }

  /**
   * Render.
   */
  public function render(&$map) {
    // Get the layers we are going to use.
    $layers = $this->get_vector_layers($map['layers']);
    $get_min = $this->options['min']['value'] === '' || !isset($this->options['min']['value']);
    $get_max = $this->options['max']['value'] === '' || !isset($this->options['max']['value']);
    $weights = $this->get_weights();
  	if(trim($this->options['arbitrary_classes'])!=''){
  		$arbitrary_classes = $this->get_classes($this->options['arbitrary_classes']);
  	}
    foreach ($layers as $k => $layer) {
      // Get the field we are going to use.
      if ($field = $this->get_field($layer['features'])) {        
    		$fields_colors = $this->get_fields_colors($layer['features']);
    		if($fields_colors){
    			$fields_colors_values = $this->fields_colors_values();
    		}

        // Get min/max per layer.
    		if(!empty($arbitrary_classes)){
    			$min = $arbitrary_classes[0]['min'];
    			$max = $arbitrary_classes[count($arbitrary_classes)-1]['min'];
    		}
    		else{
    			$min = isset($this->options['min']['value']) && is_numeric($this->options['min']['value']) ? $this->options['min']['value'] : 1000000;
          $max = isset($this->options['max']['value']) && is_numeric($this->options['max']['value']) ? $this->options['max']['value'] : 0;
        }
        $count_min = 1000000;
        $count_max = 0;
        if ($get_min || $get_max) {
          foreach ($layer['features'] as $j => $feature) {
            if ($field && isset($feature['attributes'][$field]) && $count = $feature['attributes'][$field]) {
              // Replace the count attribute with the selected one.
              $count = (float)trim(strip_tags($count));
              if ($field !== 'count') {
                $map['layers'][$k]['features'][$j]['attributes']['count'] = $count;
              }
              $max = ($count > $max) && $get_max ? $count : $max;
              $min = ($count < $min) && $get_min ? $count : $min;
              $count_max = ($count > $count_max) ? $count : $count_max;
              $count_min = ($count < $count_min) ? $count : $count_min;
            }
          }
        }

        // Sensible defaults code right here: If count_max &
        // count_min are both under 1, assume data source is
        // doing a ratio calc for us. Set max to 1.
        if ($count_max < 1 && $count_min < 1) {
          $max = 1;
        }
        foreach ($layer['features'] as $j => $feature) {
          if ($field && isset($feature['attributes'][$field]) && $count = $feature['attributes'][$field]) {
            // For layers with only a single value always use the
            // smallest weight.
            //------------> modif nd@oieau.fr : we need to strip html tag when data comes from views
            $count = (float)$this->get_field_value($count);
            if ($count_max === $count_min) {
              $map['layers'][$k]['features'][$j]['attributes']['weight'] = 1;
            }
            // Otherwise iterate through to find the right weight.
            else {
    				  if(!empty($arbitrary_classes)){
      					$flag_class = false;
      					foreach ($arbitrary_classes as  $weight => $class){
      						if($count>=$class['min'] && $count<$class['max']){
      							$map['layers'][$k]['features'][$j]['attributes']['weight'] = $weight+1;
      							$flag_class = true;
      						}
    					}
    					if(!$flag_class){
    						$map['layers'][$k]['features'][$j]['attributes']['weight'] = count($arbitrary_classes);
    					}
			  }
				else{
				  $calculated = $this->process_weight(($count - $min) / ($max - $min), $this->options['method']);
				  $flag_percentile = false;
				  foreach ($weights as $percentile => $weight) {
					if ($calculated <= $percentile) {
					  $map['layers'][$k]['features'][$j]['attributes']['weight'] = $weight;
					  $flag_percentile = true;
					  break;
					}
				  }
				  //Si la valeur est plus grande que le max des classes alors on considère que la valeur se trouve dans la classe la plus grande
				  if(!$flag_percentile){
					$map['layers'][$k]['features'][$j]['attributes']['weight'] = $weight;
				  }
				}
            }
			$color_value = '';
			if($fields_colors && !empty($fields_colors)){
                
				foreach($fields_colors as $field_color){
					if(isset($feature['attributes'][$field_color])) $color_value.= (string)$this->get_field_value($feature['attributes'][$field_color]);
				}
			}
			if(isset($fields_colors_values[$color_value])){
				$col = $fields_colors_values[$color_value][2];
			}
			elseif($this->options['default_color']){
				$col=$this->options['default_color'];
			}else{
				$col='#888888';
			}
            $map['layers'][$k]['features'][$j]['attributes']['color'] = $col;
          }
        }
      }
    }
    drupal_add_js(drupal_get_path('module', 'openlayers_plus') . '/behaviors/openlayers_plus_behavior_scalepoints.js');
    $options = array();
    $options['styles'] = $this->get_styles();
    $options['colors'] = $this->get_colors();
	$options['layers'] = $this->get_layers();
    return $options;
  }
}
