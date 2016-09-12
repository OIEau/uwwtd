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
//   //dsm($roles);
//   foreach($roles as $role){
// 	  if(($role == $admin)|| ($role == $editor)){
// 	   $errors = uwwtd_insert_errors_tab($node);
// 		if($errors !== false){
//     print $errors;
// 	}     
//   }
//   }
  
  $nodetype = t('Discharge point');
  //dsm($content);
  //dsm($node);
	$nameurls=explode('/',$_SERVER['REQUEST_URI']);
	$nameurl= $nameurls[1];
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
    if ($view_mode == 'full' && !empty($title)){
        $fieldstat = field_view_field('node', $node, 'field_status');
        $fieldnuts = field_view_field('node', $node, 'field_regionnuts');
        ?>
        <div class="google-map-banner">
        	<h1>
            	<span class="white-title"><?php echo t($nodetype); ?> : </span><?php echo $node->title; ?>
            	<span class="white-title"> - <?php echo t("Identifier"); ?> : </span><?php echo $node->field_inspireidlocalid['und'][0]['value']; ?>
            	<span class="white-title"> - <?php echo t("Status"); ?> : </span><?php echo $fieldstat[0]['#markup']; ?>
            	<span class="white-title"> - <?php echo t("Reporting year"); ?> : </span><?php echo $node->field_anneedata['und'][0]['value']; ?>
            	<br />
            	<small>
                	<?php echo t("Region (NUTS) Code"); ?> : <?php echo $node->field_regionnuts['und'][0]['value']; ?> - 
                	<?php echo t("Region (NUTS) Name"); ?> : <?php echo $fieldnuts[0]['#markup'];?>
             	</small>
            	</h1>
   		</div>
        <?php 
    }
  ?>
    

    <div class="uwwcontainer">
      <?php 
		$printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
		echo render($printy);
	  ?>
    </div>
    <div class="uwwcontainer">
      <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Characteristics'); ?></div>
          </legend>
		  <div class="panel-body">
		   <div class ="field field-name-field-agglatitude field-type-number-decimal field-label-inline clearfix">
			<div class = "field-label">
				<?php print '<a href="'.url('<front>').'#zoom=17&lat='.$content['field_dcplatitude']['#items'][0]['value'].'&lon='.$content['field_dcplongitude']['#items'][0]['value'].'&layers=Discharge%20points&baseLayers=Google%20Maps%20Normal" target="_blank"> Latitude : </a>';?>
			</div>
			<div class="field-items">
				<div class="field-item even"> &nbsp;<?php print $content['field_dcplatitude']['#items'][0]['value']; ?> </div>
			</div>
		   </div>
		   <div class ="field field-name-field-agglongitude field-type-number-decimal field-label-inline clearfix">
			<div class = "field-label">
				<?php print '<a href="'.url('<front>').'#zoom=17&lat='.$content['field_dcplatitude']['#items'][0]['value'].'&lon='.$content['field_dcplongitude']['#items'][0]['value'].'&layers=Discharge%20points&baseLayers=Google%20Maps%20Normal" target="_blank"> Longitude : </a>';?>
			</div>
			<div class="field-items">
				<div class="field-item even"> &nbsp;<?php print $content['field_dcplongitude']['#items'][0]['value']; ?> </div>
			</div>
		   </div>
		   <?php 
            print render($content['field_dcpwaterbodytype']);
            print render($content['field_rcatype']);
            print render($content['field_dcpsurfacewaters']);
			print render($content['field_dcpirrigation']); //=======> ALter the label
			?>
          </div>
        </fieldset>
      </div>
      <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Waste Water Network Connexions'); ?></div>
          </legend>
          <div class="panel-body">
		  <?php
            //print render($content['field_dcpuwwliste']);
            //print render($content['field_dcprcaliste']);
            print render($content['field_linked_agglomerations']);
            print render($content['field_linked_treatment_plants']);
            if ($content['field_rcatype']['#items'][0]['value'] == 'NA') {
              print('<div class="field field-name-field-linked-receiving-areas field-type-node-reference field-label-above">');
              print('<div class="field-label">'.t("Linked receiving areas").':&nbsp;</div>');
              print('<div class="field-items"><div class="field-item even">'.t("Normal area").'</div></div>');
              print('</div>');
            } else {
              print render($content['field_linked_receiving_areas']);
            }
            print render($content['field_dcpreceivingwater']);
            print render($content['field_dcpwaterbodyid']);
		?>
          </div>
        </fieldset>
      </div>
      <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Site information'); ?></div>
          </legend>
          <div class="panel-body">
			<?php
            print render($content['field_anneedata']);
            $content['field_sourcefile'][0]['#file']->filename = t('See sourcefile');
            print render($content['field_sourcefile']);
			?>
          </div>
        </fieldset>
      </div>
    </div>

  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
  <footer>
    <?php print render($content['field_tags']); ?>
    <?php print render($content['links']); ?>
  </footer>
  <?php endif; ?>
  <?php print render($content['comments']); ?>
</article>