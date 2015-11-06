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
echo uwwtd_insert_errors_tab($node);  
//  $admin = 'administrator';
//   $editor = 'editor';
//   $roles = $GLOBALS['user']->roles;
//   //dsm($roles);
//   foreach($roles as $role){
// 	  if(($role == $admin)|| ($role == $editor)){
// 	   $errors = uwwtd_insert_errors_tab($node);
// 		if($errors !== false){
//     print $errors;
// 	}
//   }    
//   }   
  $nodetype = t('UWWTP');

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ((!$page && !empty($title)) || !empty($title_prefix) || !empty($title_suffix) || $display_submitted): ?>
  <header>
    <?php //print render($title_prefix); ?>
    <?php if ($view_mode != 'full' && !empty($title)): ?>
    <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
  </header>
  <?php endif; ?>
  <?php
    // Hide comments, tags, and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_tags']);
    if ($view_mode == 'full' && !empty($title)){
        $fieldstat = field_view_field('node', $node, 'field_status');
        $fieldnuts = field_view_field('node', $node, 'field_regionnuts');
        print '<h1><span class="black-title">'.$nodetype.'</span>'.t(' : ').$node->title.'<span class="black-title">'.' - '.t('Identifier').t(' : ').'</span>'.$node->field_inspireidlocalid['und'][0]['value'].'<span class="black-title">'.' - '.t('Status').t(' : ').'</span>'.$fieldstat[0]['#markup'].'<span class="black-title">'.' - '.t('Reporting year').t(' : ').'</span>'.$node->field_anneedata['und'][0]['value'];
        if(isset($node->field_regionnuts['und'][0]['value'])) print '<br><small>'.t('Region (NUTS) Code : ').$node->field_regionnuts['und'][0]['value'].'</small>';
        if(isset($fieldnuts[0]['#markup'])) print '<small> - '.t('Region (NUTS) Name : ').$fieldnuts[0]['#markup'].'</small>';
        print '<br>';
        if((!isset($node->field_uwwcollectingsystem['und'][0]['value']) || $node->field_uwwcollectingsystem['und'][0]['value'] == 'NOTCON') || (!isset($node->field_physicalcapacityactivity['und'][0]['value']) || $node->field_physicalcapacityactivity['und'][0]['value'] == '0')){
          print t('COLLECTING SYSTEM WITHOUT TREATMENT PLANT');
        }
        print '</h1><br>';
    }

    $printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');

    print '<div class="uwwcontainer">';
      echo render($printy);
      print '<div class="uwwhalf">';
        echo uwwtd_timeline_output($node);
      print '</div>';
    print '</div>';
    print '<div class="uwwcontainer" style="float:left;overflow:visible;">';
      print '<fieldset class="uwwfull group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper" style="min-height:430px;">';
        print '<legend class="panel-heading">';
          print '<div class="panel-title fieldset-legend">'.t('Description').' '.$node->field_anneedata['und'][0]['value'].'</div>';
            print '</legend>';
          print '<div class="panel-body">';
            print '<div class="uwwrealthird">';
              print render($content['field_uwwloadenteringuwwtp']);
              print render($content['field_physicalcapacityactivity']);
              print render($content['field_uwwwastewatertreated']);
              print render($content['field_uwwbod5perf']);
              print render($content['field_uwwcodperf']);
              print render($content['field_uwwtssperf']);
              print render($content['field_uwwntotperf']);
              print render($content['field_uwwptotperf']);
              print render($content['field_uwwotherperf']);
            print '</div>';
            print '<div class="uwwrealthird">';
              print '<div class="flip" id="uwwtp_stackedbar" style="position:relative;">
                       <div class="front">
                          <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper table-to-chart" title="See diagram" alt="See diagram">';
                        print render($content['field_uwwbodincoming']);
						if(isset($node->field_uwwbodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwbodincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							print "<b>".t('Incoming concentration BOD (mg/l): ')."</b>";
							if(isset($node->field_uwwbodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingBod = ($node->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($incomingBod,2);
							}
						}
													
						print render($content['field_uwwboddischarge']);
						if(isset($node->field_uwwboddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwboddischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							print "<b>".t('Discharged concentration BOD (mg/l): ')."</b>";
							if(isset($node->field_uwwboddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeBod = ($node->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($dischargeBod,2);
							}
						}
						
						if(isset($node->field_uwwboddischarge['und'][0]['value']) && isset($node->field_uwwbodincoming['und'][0]['value']) && $node->field_uwwbodincoming['und'][0]['value']!=0){
                            print '<div class="field field-type-number-decimal field-label-inline clearfix">';
                              print '<div class="field-label">'.t('Rate for BOD:').'</div>';
                              print '<div class="field-items">';
                                print '<div class="field-item">';
                                  print '&nbsp;'.round(((1- ($node->field_uwwboddischarge['und'][0]['value'] / $node->field_uwwbodincoming['und'][0]['value']))*100), 1) . '%';
                                print '</div>';  
                              print '</div>';
                            print '</div>';
                          }
						print render($content['field_uwwcodincoming']);
						if(isset($node->field_uwwcodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwcodincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							print "<b>".t('Incoming concentration COD (mg/l): ')."</b>";
							if(isset($node->field_uwwcodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingCod = ($node->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($incomingCod,2);
							}
						}
							
                          print render($content['field_uwwcoddischarge']);
						  if(isset($node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							print "<b>".t('Discharged concentration COD (mg/l): ')."</b>";
							if(isset($node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeCod = ($node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($dischargeCod,2);
							}
						  }
						  
                          if(isset($node->field_uwwcoddischarge['und'][0]['value']) && isset($node->field_uwwcodincoming['und'][0]['value']) && $node->field_uwwcodincoming['und'][0]['value']!=0){
                            print '<div class="field field-type-number-decimal field-label-inline clearfix">';
                              print '<div class="field-label">'.t('Rate for COD:').'</div>';
                              print '<div class="field-items">';
                                print '<div class="field-item">';
                                  print '&nbsp;'.round(( (1-($node->field_uwwcoddischarge['und'][0]['value'] / $node->field_uwwcodincoming['und'][0]['value'])) *100), 1) . '%';
                                print '</div>';  
                              print '</div>';
                            print '</div>';
                          }
						  
                          print render($content['field_uwwnincoming']);
						  if(isset($node->field_uwwnincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwnincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwnincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							print "<b>".t('Incoming concentration N (mg/l): ')."</b>";
							if(isset($node->field_uwwnincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingN = ($node->field_uwwnincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($incomingN,2);
							}
						  }
						
                          print render($content['field_uwwndischarge']);
						   if(isset($node->field_uwwndischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwndischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwndischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							print "<b>".t('Discharged concentration N (mg/l): ')."</b>";
							if(isset($node->field_uwwndischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeN = ($node->field_uwwndischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($dischargeN,2);
							}
						  }
						
                          if(isset($node->field_uwwndischarge['und'][0]['value']) && isset($node->field_uwwnincoming['und'][0]['value']) && $node->field_uwwnincoming['und'][0]['value']!=0){
                            print '<div class="field field-type-number-decimal field-label-inline clearfix">';
                              print '<div class="field-label">'.t('Rate for Nitrogen:').'</div>';
                              print '<div class="field-items">';
                                print '<div class="field-item">';
                                  print '&nbsp;'.round(( (1- ($node->field_uwwndischarge['und'][0]['value'] / $node->field_uwwnincoming['und'][0]['value'])) *100), 1) . '%';
                                print '</div>';  
                              print '</div>';
                            print '</div>';
                          }
							print render($content['field_uwwpincoming']);
							if(isset($node->field_uwwpincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwpincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwpincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							print "<b>".t('Incoming concentration P (mg/l): ')."</b>";
							if(isset($node->field_uwwpincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingP = ($node->field_uwwpincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($incomingP,2);
							}
						  }
						  
                          print render($content['field_uwwpdischarge']);
						  if(isset($node->field_uwwpdischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwpdischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							 print "<b>".t('Discharged concentration P (mg/l): ')."</b>";
							if(isset($node->field_uwwpdischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeP = ($node->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								print round($dischargeP,2);
							}
						  }
						 
                          if(isset($node->field_uwwpdischarge['und'][0]['value']) && isset($node->field_uwwpincoming['und'][0]['value']) && $node->field_uwwpincoming['und'][0]['value'] != 0){
                            print '<div class="field field-type-number-decimal field-label-inline clearfix">';
                              print '<div class="field-label">'.t('Rate for Phosphorus:').'</div>';
                              print '<div class="field-items">';
                                print '<div class="field-item">';
                                  print '&nbsp;'.round(( (1-($node->field_uwwpdischarge['und'][0]['value'] / $node->field_uwwpincoming['und'][0]['value'])) *100), 1) . '%';
                                print '</div>';  
                              print '</div>';
                            print '</div>';
                          }                      
              print'   </div>
                       <div class="back" style="position:absolute;">
                          <img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper chart-to-table" title="See the data table" alt="See the data table">
                            <svg id="uwwtp_stackedbar_back"></svg>';
                            echo uwwtd_stackedbar_uwwtpnode($node);
              print'   </div>       
                    </div>';
                                
//             print '<div id="uwwtp_stackedbar">
//                     <div class="front">';
//               print render($content['field_uwwbodincoming']);
//               print render($content['field_uwwboddischarge']);
//               if(isset($node->field_uwwboddischarge['und'][0]['value']) && isset($node->field_uwwbodincoming['und'][0]['value']) && $node->field_uwwbodincoming['und'][0]['value']!=0){
//                 print '<div class="field field-type-number-decimal field-label-inline clearfix">';
//                   print '<div class="field-label">'.t('Rate for BOD:').'</div>';
//                   print '<div class="field-items">';
//                     print '<div class="field-item">';
//                       print '&nbsp;'.round(((1- ($node->field_uwwboddischarge['und'][0]['value'] / $node->field_uwwbodincoming['und'][0]['value']))*100), 2) . '%';
//                     print '</div>';  
//                   print '</div>';
//                 print '</div>';
//               }
//               print render($content['field_uwwcodincoming']);
//               print render($content['field_uwwcoddischarge']);
//               if(isset($node->field_uwwcoddischarge['und'][0]['value']) && isset($node->field_uwwcodincoming['und'][0]['value']) && $node->field_uwwcodincoming['und'][0]['value']!=0){
//                 print '<div class="field field-type-number-decimal field-label-inline clearfix">';
//                   print '<div class="field-label">'.t('Rate for COD:').'</div>';
//                   print '<div class="field-items">';
//                     print '<div class="field-item">';
//                       print '&nbsp;'.round(( (1-($node->field_uwwcoddischarge['und'][0]['value'] / $node->field_uwwcodincoming['und'][0]['value'])) *100), 2) . '%';
//                     print '</div>';  
//                   print '</div>';
//                 print '</div>';
//               }
//               print render($content['field_uwwnincoming']);
//               print render($content['field_uwwndischarge']);
//               if(isset($node->field_uwwndischarge['und'][0]['value']) && isset($node->field_uwwnincoming['und'][0]['value']) && $node->field_uwwnincoming['und'][0]['value']!=0){
//                 print '<div class="field field-type-number-decimal field-label-inline clearfix">';
//                   print '<div class="field-label">'.t('Rate for Nitrogen:').'</div>';
//                   print '<div class="field-items">';
//                     print '<div class="field-item">';
//                       print '&nbsp;'.round(( (1- ($node->field_uwwndischarge['und'][0]['value'] / $node->field_uwwnincoming['und'][0]['value'])) *100), 2) . '%';
//                     print '</div>';  
//                   print '</div>';
//                 print '</div>';
//               }
//               print render($content['field_uwwpincoming']);
//               print render($content['field_uwwpdischarge']);
//               if(isset($node->field_uwwpdischarge['und'][0]['value']) && isset($node->field_uwwpincoming['und'][0]['value']) && $node->field_uwwpincoming['und'][0]['value'] != 0){
//                 print '<div class="field field-type-number-decimal field-label-inline clearfix">';
//                   print '<div class="field-label">'.t('Rate for Phosphorus:').'</div>';
//                   print '<div class="field-items">';
//                     print '<div class="field-item">';
//                       print '&nbsp;'.round(( (1-($node->field_uwwpdischarge['und'][0]['value'] / $node->field_uwwpincoming['und'][0]['value'])) *100), 2) . '%';
//                     print '</div>';  
//                   print '</div>';
//                 print '</div>';
//               }
//               print '</div>
//                     <div class="back">';
//                     print '<svg id="uwwtp_stackedbar_back"></svg>';
//                     echo uwwtd_stackedbar_uwwtpnode($node);
//                     print '</div>
//                 </div>';                                                   
            print '</div>';  
            print '<div class="uwwrealthird" style="float:right;">';
              print render($content['field_uwwcompliance']);
              print render($content['field_uwwtreatmenttype']);
              print render($content['field_uwwtreatmentrequired']);
              print render($content['field_uwwtreatment_met']);
              print render($content['field_uwwperformance_met']);
              print '<br><br><br><br><br><br><br><br>';
            print '</div>';
        print '</div>';
      print '</fieldset>';
    print '</div>';
/* 2015/08/31 / e.vincent / add dev from alexander */
// /* 
    print '<div class="uwwcontainer" style="overflow:visible">';
      print '<fieldset class="uwwfull group-agggraphic field-group-fieldset group-description panel panel-default form-wrapper">';
        print '<legend class="panel-heading">';
          print '<div class="panel-title fieldset-legend">'.t('Graphical network summary').'</div>';
            print '</legend>';
          print '<div class="panel-body">';
              
            echo uwwtd_get_uww_graphic($node);

          print '</div>';
      print '</fieldset>';
    print '</div>';
// */
    print '<div class="uwwcontainer">';
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Characteristics').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_uwwlatitude']);
            print render($content['field_uwwlongitude']);
            //print render($content['field_uwwtreatmentrequired']);
            print '<h4>'.t('Treatment in place :').'</h4>';
            print '<span class="bump"></span>'.render($content['field_uwwprimarytreatment']);
            print '<span class="bump"></span>'.render($content['field_uwwsecondarytreatment']);
            print '<h4>'.t('More stringent treatment :').'</h4>';
            print '<span class="bump"></span>'.render($content['field_uwwnremoval']);
            print '<span class="bump"></span>'.render($content['field_uwwpremoval']);
            print '<span class="bump"></span>'.render($content['field_uwwuv']);
            print '<span class="bump"></span>'.render($content['field_uwwchlorination']);
            print '<span class="bump"></span>'.render($content['field_uwwozonation']);
            print '<span class="bump"></span>'.render($content['field_uwwsandfiltration']);
            print '<span class="bump"></span>'.render($content['field_uwwmicrofiltration']);
            print '<span class="bump"></span>'.render(field_view_field('node', $node, 'field_uwwothertreat', array('label' => 'inline')));
          print '</div>';
        print '</fieldset>';
      print '</div>';
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Waste Water Network Connexions').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_linked_agglomerations']);
            print render($content['field_linked_discharge_points']);
            print render($content['field_linked_receiving_areas']); 
            //print render($content['field_uwwaggliste']);
            //print render($content['field_uwwdcpliste']);
          print '</div>';
        print '</fieldset>';
      print '</div>';
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Site information').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_anneedata']);
            $content['field_sourcefile'][0]['#file']->filename = 'See sourcefile';
            print render($content['field_sourcefile']);
          print '</div>';
        print '</fieldset>';
        if(isset($node->field_article17_uwwtp['und'][0]['nid'])){
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Forward looking aspect').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print uwwtd_render_article17_uwwtp($node->field_article17_uwwtp['und'][0]['nid']);
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
