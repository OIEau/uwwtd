<?php
/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup themeable
 */
//dsm($node);
echo uwwtd_insert_errors_tab($node);    
//  $admin = 'administrator';
//   $editor = 'editor';
//   $roles = $GLOBALS['user']->roles;
// // dsm($roles);
//   foreach($roles as $role){
// 	  if(($role == $admin)|| ($role == $editor)){
// 	   $errors = uwwtd_insert_errors_tab($node);
// 		if($errors !== false){
//     print $errors;
// 	}        
//   }
//   }     
  
  $nodetype = t('Agglomeration');
  // krumo($content);
  //dsm($node);

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ((!$page && !empty($title)) || !empty($title_prefix) || !empty($title_suffix) || $display_submitted): ?>
  <header>
    <?php if ($view_mode != 'full' && !empty($title)): ?>
    <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
  </header>
  <?php endif; ?>
  <?php
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_tags']);
	
	$printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
	print '<div class="uwwcontainer" style="margin-top:-50px">';
		if ($view_mode == 'full' && !empty($title)){
			$fieldstat = field_view_field('node', $node, 'field_status');
			$fieldnuts = field_view_field('node', $node, 'field_regionnuts');
			//print '<h1>'.$node->title.' <small><i>- '.$nodetype.', '.t('reporting year: ').$node->field_anneedata['und'][0]['value'].'</i></small>';
			print '<h1 style="top:220px;color:white;z-index:4;"><span class="white-title">'.$nodetype.'</span>'.t(' : ').$node->title.'<span class="white-title">'.' - '.t('Identifier').t(' : ').'</span>'.$node->field_inspireidlocalid['und'][0]['value'].'<span class="white-title">'.' - '.t('Status').t(' : ').'</span>'.$fieldstat[0]['#markup'].'<span class="white-title">'.' - '.t('Reporting year').t(' : ').'</span>'.$node->field_anneedata['und'][0]['value'];
			print '<br><small>'.t('Region (NUTS) Code : ').$node->field_regionnuts['und'][0]['value'].' - '.t('Region (NUTS) Name : ').$fieldnuts[0]['#markup'].'</small></h1><br>';
		}

    //print render($content);
    
    
      echo render($printy);
      print '<div class="uwwhalf" style="clear:left;">';
        echo uwwtd_timeline_output($node);
      print '</div>';
    print '</div>';
    print '<div class="uwwcontainer" style="overflow:hidden;">';
      print '<fieldset class="uwwfull group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper" style="min-height:240px;">';
        print '<legend class="panel-heading">';
          print '<div class="panel-title fieldset-legend">'.t('Description').' '.$node->field_anneedata['und'][0]['value'].'</div>';
            print '</legend>';
          print '<div class="panel-body">';
            print '<div class="uwwrealthird">';
              print '<div class="flip">
                       <div class="front">
                          <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper table-to-chart" title="See diagram" alt="See diagram">';
                          
                          print render($content['field_agggenerated']);
                          print uwwtd_render_field_with_pe($content['field_aggc1']);
                          print uwwtd_render_field_with_pe($content['field_aggc2']);
                          print uwwtd_render_field_with_pe($content['field_aggpercwithouttreatment']);                          
              print'   </div>
                       <div class="back">
                          <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper chart-to-table" title="See the data table" alt="See the data table">
                          <div id="agglo_piechart_back"></div>';
                          echo uwwtd_piechart_agglonode($node, $content);
              print'   </div>       
                    </div>';
            print '</div>';
			
            print '<div class="uwwrealthird">';
			print render($content['field_aggcompliance']);
              //print render($content['field_compliance_explication']);
              print render($content['field_aggart3compliance']);
              //print render($content['field_article_3_compliance_expli']);
              print render($content['field_aggart4compliance']);
              //print render($content['field_article_4_compliance_expli']);
              print render($content['field_aggart5compliance']);
              //print render($content['field_article_5_compliance_expli']);
			   // print render($content['field_aggart6compliance']);
           $distanceToCompliance = uuwtd_get_distance_compliance($node);
			if(uwwtd_field_num($content['field_aggpercwithouttreatment']) == 0 && uwwtd_field_pe($content['field_aggpercwithouttreatment']) ==0){
			   $colorart3 = '#4f91e1';
			 }elseif((uwwtd_field_pe($content['field_aggpercwithouttreatment']) >= 2 && uwwtd_field_pe($content['field_aggpercwithouttreatment'] <=2)) &&($distanceToCompliance['field_aggpercwithouttreatment'] >= 0 && $distanceToCompliance['field_aggpercwithouttreatment'] <= 2000)){
				$colorart3 = '#ea8b2e';
			}else{
				$colorart3 = '#d93c3c';
			}
			
			if($distanceToCompliance['distart4_treatment'] == 0 && $distanceToCompliance['distart4_treatment_%'] == 0){
				$colorart4T = '#4f91e1';
			}elseif(($distanceToCompliance['distart4_treatment_%'] >= 1 && $distanceToCompliance['distart4_treatment_%'] <= 1) &&($distanceToCompliance['distart4_monitoring'] >= 0 && $distanceToCompliance['distart4_monitoring'] <= 2000)){
				$colorart4T = '#ea8b2e';
			}else{
				$colorart4T = '#d93c3c';
			}
			if($distanceToCompliance['distart4_monitoring'] == 0  && $distanceToCompliance['distart4_monitoring_%'] == 0){
				$colorart4P = '#4f91e1';
			}elseif(($distanceToCompliance['distart4_treatment_%'] >= 1 && $distanceToCompliance['distart4_treatment_%'] <= 1) &&($distanceToCompliance['distart4_monitoring'] >= 0 && $distanceToCompliance['distart4_monitoring'] <= 2000)){
				$colorart4P = '#ea8b2e';
			}else{
				$colorart4P = '#d93c3c';
			}
			

			if($distanceToCompliance['distart5_treatment'] == 0  && $distanceToCompliance['distart5_treatment_%'] == 0){
				$colorart5T = '#4f91e1';
			}elseif(($distanceToCompliance['distart5_treatment_%'] >= 1 && $distanceToCompliance['distart5_treatment_%'] <= 1) &&($distanceToCompliance['distart5_monitoring'] >= 0 && $distanceToCompliance['distart5_monitoring'] <= 2000)){
				$colorart5T = '#ea8b2e';
			}elseif($distanceToCompliance['distart5_treatment_%'] > 1 && $distanceToCompliance['distart5_treatment'] > 2000){
				$colorart5T = '#d93c3c';
			}

			if($distanceToCompliance['distart5_monitoring'] == 0  && $distanceToCompliance['distart5_monitoring_%'] == 0){
				$colorart5P = '#4f91e1';
			}elseif(($distanceToCompliance['distart5_treatment_%'] >= 1 && $distanceToCompliance['distart5_treatment_%'] <= 1) &&($distanceToCompliance['distart5_monitoring'] >= 0 && $distanceToCompliance['distart5_monitoring'] <= 2000)){
				$colorart5P = '#ea8b2e';
			}elseif($distanceToCompliance['distart5_monitoring_%'] > 1 && $distanceToCompliance['distart5_monitoring'] > 2000){
				$colorart5P = '#d93c3c';
			}
			
            print '</div>';
            print '<div class="uwwrealthird">';
				print '<div class="distance">';
				print '<p><b>Distance to compliance:</b><p>';
					print '<table id="UwwtpDescription">';
					print '<tr><td style="font-weight:bold;border: 1px solid #000;"></td>';
					print '<td style="font-weight:bold;border: 1px solid #000;">Equipment</td>';
					print '<td style="font-weight:bold;border: 1px solid #000;">Performance</td></tr>';
					print '<tr><td class="black" style="font-weight:bold;" rowspan=2>Connection</td>';
					print '<td class="light" style="background-color:'.$colorart3.';color:white;">'.uwwtd_field_num($content['field_aggpercwithouttreatment']).'%</td>';
					print '<td class="black"></td></tr>';
					print '<tr><td class="light" style="background-color:'.$colorart3.';color:white;">'. uwwtd_field_pe($content['field_aggpercwithouttreatment']).' p.e</td>';
					print '<tr><td class="black" style="font-weight:bold;" rowspan=2>2nd treatment</td>';
					print '<td class="light" style="background-color:'.$colorart4T.';color:white;">'.number_format($distanceToCompliance['distart4_treatment_%'],1, ',', ' ').'%</td>';
					print '<td class="black"  style="background-color:'.$colorart4P.';color:white;">'.number_format($distanceToCompliance['distart4_monitoring_%'],1, ',', ' ').'%</td></tr>';
					print '<tr><td class="light" style="background-color:'.$colorart4T.';color:white;">'.number_format($distanceToCompliance['distart4_treatment'],0, ',', ' ').' p.e </td>';
					print '<td class="black" style="background-color:'.$colorart4P.';color:white;">'.number_format($distanceToCompliance['distart4_monitoring'],0, ',', ' ').' p.e </td></tr>';
					print '<tr><td class="black" style="font-weight:bold;" rowspan=2>3rd treatment</td>';
					print '<td class="light" style="background-color:'.$colorart5T.';color:white;">'.number_format($distanceToCompliance['distart5_treatment_%'],1, ',', ' ').'%</td>';
					print '<td class="black"  style="background-color:'.$colorart5P.';color:white;">'.number_format($distanceToCompliance['distart5_monitoring_%'],1, ',', ' ').'%</td></tr>';
					print '<tr><td class="light" style="background-color:'.$colorart5T.';color:white;">'.number_format($distanceToCompliance['distart5_treatment'],0, ',', ' ').' p.e</td>';
					print '<td class="black" style="background-color:'.$colorart5P.';color:white;"> '.number_format($distanceToCompliance['distart5_monitoring'],0, ',', ' ').' p.e</td></tr>';
				print '</table>';
				print '</div;>';
            print '</div>';
        print '</div>';
      print '</fieldset>';
    print '</div>';
/* e.vincent / 2015/08/31 / add dev from alexander */
// /*
    print '<div class="uwwcontainer" style="overflow:visible">';
      print '<fieldset class="uwwfull group-agggraphic field-group-fieldset group-description panel panel-default form-wrapper">';
        print '<legend class="panel-heading">';
          print '<div class="panel-title fieldset-legend">'.t('Graphical network summary').'</div>';
            print '</legend>';
          print '<div class="panel-body">';
              
            echo uwwtd_get_agglo_graphic($node);

          print '</div>';
      print '</fieldset>';
    print '</div>';
	
    print '<div class="uwwcontainer">';
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Characteristics').'</div>';
          print '</legend>';
		  $nameurls=explode('/',$_SERVER['REQUEST_URI']);
		  $nameurl= $nameurls[1];
          print '<div class="panel-body">';
		  print '<div class ="field field-name-field-agglatitude field-type-number-decimal field-label-inline clearfix">';
				print '<div class = "field-label"><a href="/'.$nameurl.'/#zoom=17&lat='.$content['field_agglatitude']['#items'][0]['value'].'&lon='.$content['field_agglongitude']['#items'][0]['value'].'&layers=Agglomerations&baseLayers=Google%20Maps%20Normal" target="_blank"> Latitude : </a></div>';
					print '<div class="field-items">';
						print '<div class="field-item even"> &nbsp;'.$content['field_agglatitude']['#items'][0]['value'].'</div>';
					print '</div>';
			print '</div>';
			print '<div class ="field field-name-field-agglongitude field-type-number-decimal field-label-inline clearfix">';
				print '<div class = "field-label"><a href="/'.$nameurl.'/#zoom=17&lat='.$content['field_agglatitude']['#items'][0]['value'].'&lon='.$content['field_agglongitude']['#items'][0]['value'].'&layers=Agglomerations&baseLayers=Google%20Maps%20Normal" target="_blank"> Longitude : </a></div>';
					print '<div class="field-items">';
						print '<div class="field-item even"> &nbsp;'.$content['field_agglongitude']['#items'][0]['value'].'</div>';
					print '</div>';
			print '</div>';
            // print render($content['field_agglatitude']);
            // print render($content['field_agglongitude']);
            print render($content['field_aggperiodover3']);
            print render($content['field_aggperiodover4']);
            print render($content['field_aggperiodover5']);
            print render($content['field_aggperiodover6']);
            print render($content['field_agghaveregistrationsystem']);
            print render($content['field_aggexistmaintenanceplan']);
			print render($content['field_aggbesttechnicalknowledge']);
			 print render($content['field_aggsewagenetwork']);
          print '</div>';
        print '</fieldset>';
      print '</div>';
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Waste Water Network Connexions').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_linked_treatment_plants']);
            print render($content['field_linked_discharge_points']);
            print render($content['field_linked_receiving_areas']); 
            //print render($content['field_agguwwliste']);
          print '</div>';
        print '</fieldset>';
      print '</div>';
	  $option['field_anneedata_value'] = uwwtd_get_all_year();
	  $nbOptionannee = count($option['field_anneedata_value']);
	  $type = $content['field_sourcefile']['#bundle'];
	   foreach($option['field_anneedata_value'] as $optionAnnee){
			$option['year'] = $optionAnnee;
			$option['aggCode'] = $content['field_inspireidlocalid'][0]['#markup'];
			$siteId = uwwtd_get_siteid($type, $option);
			$entity = uwwtd_check_exist($siteId);
			
			if(isset($entity) && $entity!=""){
				$sourceFile = uwwtd_get_sourcefile($entity);
				$fileManaged = uwwtd_get_filemanaged($sourceFile);
				$arrayFile[] = array(
					"year" => $option['year'],
					"file" => $fileManaged
			 );
			}
		 }
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Site information').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
		  if(isset($arrayFile)){
				foreach($arrayFile as $files){
					if($files['year'] == $content['field_anneedata'][0]['#markup']){
						if($files['file'] == $content['field_sourcefile'][0]['#file']->filename){
						}
					}else{
						$server = explode('/',$_SERVER['REQUEST_URI']);
						
						$outputSiteInfo = '<br><b>Year of data: </b>'.$files['year'].'<br>';
						$outputSiteInfo.= '<b>Source of data: </b><img src="/'.$server[1].'/modules/file/icons/application-octet-stream.png" title="application/xml" alt="file"/>
						<a href=/'.$server[1].'/sites/default/files/data_sources/'.$files['file'].'>See sourcefile</a>';
					}
				} 
			}
            print render($content['field_anneedata']);
            $content['field_sourcefile'][0]['#file']->filename = 'See sourcefile';
            print render($content['field_sourcefile']).'<br>';
			if(isset($outputSiteInfo) && $outputSiteInfo !="")
			{
				print '<b><i>Other year : </i></b>';
			}
			
			print $outputSiteInfo;
          print '</div>';
        print '</fieldset>';
        if(isset($node->field_article17_agglo['und'][0]['nid'])){
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Forward looking aspect').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            //print render($content['article_17_agglomeration']);
            print uwwtd_render_article17_aglo($node->field_article17_agglo['und'][0]['nid']);
          print '</div>';
        print '</fieldset>';
        }
      print '</div>';
    print '</div>';


  ?>
  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
  <footer>
    <?php print render($content['field_tags']); ?>
    <?php print render($content['links']); ?>
  </footer>
  <?php endif; ?>
  <?php print render($content['comments']); ?>
</article>