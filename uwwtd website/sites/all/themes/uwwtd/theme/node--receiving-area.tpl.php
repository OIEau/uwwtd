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
//   $admin = 'administrator';
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
  
 
  

  $rcaType = $node->field_specialisedzonetype['und'][0]['value'];
  $nodetype = '';
  switch ($rcaType) {
    case 'LSA':
      $nodetype = t('Less Sensitive Area');
      break;
    case 'NA':
      $nodetype = t('Normal Area');
      break;
    case 'CSA':
      $nodetype = t('Catchment of Sensitive Area');
      break;
    case 'SA':
      $nodetype = t('Sensitive Area');
      break;
    case 'A54':
      $nodetype = t('A54 Sensitive Area');
      break;
    case 'A58':
      $nodetype = t('A58 Sensitive Area');
      break;
    default:
      $nodetype = t('Area');
      break;
  }

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ((!$page && !empty($title)) || !empty($title_prefix) || !empty($title_suffix) || $display_submitted): ?>
  <header>
    <?php if ($view_mode != 'full' && !empty($title)): ?>
    <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
  </header>
  <?php endif; 
  
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_tags']);
    if ($view_mode == 'full' && !empty($title)){
        $fieldstat = field_view_field('node', $node, 'field_status');
        ?>
        <div class="google-map-banner">
        	<h1>
	            <span class="white-title"><?php echo $nodetype; ?> : </span><?php echo $node->title; ?>
	            <span class="white-title"> - Identifier : </span><?php echo $node->field_inspireidlocalid['und'][0]['value']; ?>
	            <span class="white-title"> - Status : </span><?php echo $fieldstat[0]['#markup']; ?>
	            <span class="white-title"> - Reporting year : </span><?php echo $node->field_anneedata['und'][0]['value']; ?>
            </h1>
        </div>
        <?php 
    }
	
    //print render($content);
    $printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');

    // characteristics
    if($rcaType != 'LSA' && $rcaType != 'NA'){
    print '<div class="uwwcontainer">';
      echo render($printy);  
//       print '<div class="uwwhalf">';
//         echo uwwtd_timeline_output($node);
//       print '</div>';
    print '</div>';
    print '<div class="uwwcontainer">';
      print '<fieldset class="uwwfull group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
        print '<legend class="panel-heading">';
          print '<div class="panel-title fieldset-legend">'.t('Characteristics').' '.$node->field_anneedata['und'][0]['value'].'</div>';
        print '</legend>';
        print '<div class="panel-body">';
          print '<div class="uwwrealquart">';
            print render($content['field_rca52applied']);
            print render($content['field_rca_parameter_n']);
            print render($content['field_rcacdatedesignation']);
            print render($content['field_rcaapstartdate']);
          print '</div>';
          print '<div class="uwwrealquart">';
            print render($content['field_rca54applied']);
            print render($content['field_rcaapstartdate']);
            print render($content['field_rca_parameter_p']);
            print render($content['field_rcacdatedesignation']);
            print render($content['field_rcaapstartdate']);
          print '</div>';
          print '<div class="uwwrealquart">';
            print render($content['field_rca58applied']);
            print render($content['field_rcaapstartdate']);
            print render($content['field_rca_parameter_n']);
            print render($content['field_rcacdatedesignation']);
            print render($content['field_rcaapstartdate']);
	        print '</div>';
          print '<div class="uwwrealquart">';
            print render($content['field_rca_parameter_other']);;
            print render($content['field_rcacdatedesignation']);
            print render($content['field_rcaapstartdate']);
          print '</div>';
        print '</div>';
      print '</fieldset>';
    print '</div>';
    }
    // END characteristics

    print '<div class="uwwcontainer">';
      // description
      if($node->field_rca54applied == '1'){
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Description').' '.$node->field_anneedata['und'][0]['value'].'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_rca_nb_uwwtps']);
            print render($content['field_rca_total_capacity_uwwtps']);
            print render($content['field_rca_total_p_entering']);
            print render($content['field_rca_total_p_discharged']);
            $rateP = ($node->field_rca_total_p_discharged['und'][0]['value'] / $node->field_rca_total_p_entering['und'][0]['value']) * 100;
            print'<div class="field field-name-field-rca54applied field-type-list-boolean field-label-inline clearfix">
              <div class="field-label">
                '.t('Rate of P removal:').' 
              </div>
              <div class="field-items">
                <div class="field-item even">
                    '.$rateP.' %
                </div>
              </div>
            </div>';
            print render($content['field_rca_total_n_entering']);
            print render($content['field_rca_total_n_discharged']);
            $rateN = ($node->field_rca_total_n_discharged['und'][0]['value'] / $node->field_rca_total_n_entering['und'][0]['value']) * 100;
            print'<div class="field field-name-field-rca54applied field-type-list-boolean field-label-inline clearfix">
              <div class="field-label">
                '.t('Rate of N removal:').' 
              </div>
              <div class="field-items">
                <div class="field-item even">
                    '.$rateN.' %
                </div>
              </div>
            </div>';
          print '</div>';
        print '</fieldset>';
      print '</div>';
      }
      // END description

      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Waste Water Network Connexions').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_linked_agglomerations']);
            print render($content['field_linked_treatment_plants']);
            print render($content['field_linked_discharge_points']);
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
        if(isset($node->field_article17['und'][0]['nid'])){
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Forward looking aspect').'</div>';
          print '</legend>';
          print '<div class="panel-body">';

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