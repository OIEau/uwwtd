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
   		$printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
        $fieldstat = field_view_field('node', $node, 'field_status');
        if (!empty($fieldstat[0]['#markup']) && $fieldstat[0]['#markup'] == '2') {
            $fieldstat[0]['#markup'] = 'Inactive';
        }
        ?>
        <div class="google-map-banner receiving-area <?php print (empty($printy)) ? 'empty' : ''; ?>">
        	<h1>
	            <span class="white-title"><?php echo $nodetype; ?> : </span><?php echo $node->title; ?>
	            <span class="white-title"> - Identifier : </span><?php echo $node->field_inspireidlocalid['und'][0]['value']; ?>
	            <!-- <span class="white-title"> - Status : </span><?php echo $fieldstat[0]['#markup']; ?> -->
	            <span class="white-title"> - Reporting year : </span><?php echo $node->field_anneedata['und'][0]['value']; ?>
            </h1>
        </div>
        <?php
    }
    $validImg = '<img style="position: relative; top: -2px; margin-left: 5px;" height="10px" src="'.url(path_to_theme().'/images/tick.png'). '"/>';
    $noValidImg = '<img style="position: relative; top: -2px; margin-left: 5px;" height="10px" src="'.url(path_to_theme().'/images/cross.png'). '"/>';

    ?>
    <!-- characteristics -->
    <?php if ($rcaType != 'LSA' && $rcaType != 'NA') : ?>
        <div class="uwwcontainer">
            <?php print render($printy); ?>
            <div class="uwwhalf">
                <?php print uwwtd_timeline_receiving_area_output($node); ?>
            </div>
        </div>
        <div class="uwwcontainer">
            <fieldset class="uwwfull group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
                <legend class="panel-heading">
                    <div class="panel-title fieldset-legend"><?php print t('Characteristics').' '.$node->field_anneedata['und'][0]['value']; ?></div>
                </legend>
                <div class="panel-body">
                    <table class="characteristics-sentitive-area-header">
                        <tr>
                            <td><b>Type of sensitive area:</b></td>
                            <td><?php print $variables['typeSensitiveArea']; ?></td>
                        </tr>
                        <tr>
                            <td><b>Total load entering:</b></td>
                            <td><?php print $variables['totalLoadEntering']; ?></td>
                        </tr>
                        <tr>
                            <td><b>Total design capacity:</b></td>
                            <td><?php print $variables['totalDesigncapacity']; ?></td>
                        </tr>
                    </table>
                    <table class="characteristics-sentitive-area">
                        <tr>
                            <td></td>
                            <td><b>Date of designation</b></td>
                            <td><b>Starting date of application</b></td>
                        </tr>
                        <tr>
                            <td ><?php print render($content['field_rca58applied']); ?></td>
                            <td><?php print (isset($node->field_rcaart58datedesign[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcaart58datedesign[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                            <td><?php print (isset($node->field_rcadateart5854[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcadateart5854[LANGUAGE_NONE][0]['value']) : '-'); ?></td>    
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    <b>75% removal nitrogen and phosphorus (5.4):</b>
                                    <?php $img75Percent = $noValidImg; ?>
                                    <?php if (isset($node->field_specialisedzonetype) && !empty($node->field_specialisedzonetype[LANGUAGE_NONE][0]['value']) &&
                                              ($node->field_specialisedzonetype[LANGUAGE_NONE][0]['value'] === 'A5854' ||
                                               $node->field_specialisedzonetype[LANGUAGE_NONE][0]['value'] === 'A54' ||
                                               $node->field_specialisedzonetype[LANGUAGE_NONE][0]['value'] === 'A5854523' ||
                                               $node->field_specialisedzonetype[LANGUAGE_NONE][0]['value'] === 'A54523')) : ?>
                                        <?php $img75Percent = $validImg; ?>
                                    <?php endif; ?>
                                    <?php print $img75Percent; ?>
                                </div>
                            </td>
                            <td> <?php print (isset($node->field_rcadateart54[LANGUAGE_NONE]) && isset($node->field_rcaapdatedesignation[LANGUAGE_NONE])?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcaapdatedesignation[LANGUAGE_NONE][0]['value']) : '-'); ?>
                            
                            </td>
                            <td><?php print (isset($node->field_rcadateart54[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcadateart54[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rca52applied']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><br/></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><b>Criteria</b></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rcaanitro']); ?></td>
                            <td><?php print (isset($node->field_rcaadatedesignation[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcaadatedesignation[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                            <td><?php print (isset($node->field_rcaanstartdate[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcaanstartdate[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rcaaphos']); ?></td>
                            <td><?php print (isset($node->field_rcaapdatedesignation[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcaapdatedesignation[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                            <td><?php print (isset($node->field_rcaapstartdate[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcaapstartdate[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rcab']); ?></td>
                            <td><?php print (isset($node->field_rcabdatedesignation[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcabdatedesignation[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                            <td><?php print (isset($node->field_rcabstartdate[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcabstartdate[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rcac']); ?></td>
                            <td><?php print (isset($node->field_rcacdatedesignation[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcacdatedesignation[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                            <td><?php print (isset($node->field_rcacstardate[LANGUAGE_NONE]) ?
                                    uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcacstardate[LANGUAGE_NONE][0]['value']) : '-'); ?></td>
                        </tr>
                        <tr>
                            <td><div><b>Comment other directive:</b></div></td>
                            <td colspan="2"><?php print (isset($node->field_rcacrelevantdirective[LANGUAGE_NONE])?$node->field_rcacrelevantdirective[LANGUAGE_NONE][0]['value']:'-'); ?></td>
                        </tr>
                        <tr>
                            <td><br/></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><b>Parameter (s) subject to more stringent treatment</b></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rca_parameter_n']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rca_parameter_m']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rca_parameter_p']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rca_parameter_other']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                    </table>
                    <div class="under-characteristics-sentitive-area">
                        <p><b>General comment:</b></p>
                        <?php print (isset($node->field_rcaremarks[LANGUAGE_NONE])?$node->field_rcaremarks[LANGUAGE_NONE][0]['value']:'-'); ?></td>
                    </div>
                    <table class="characteristics-sentitive-area">
                        <tr>
                            <td>
                               <div class="field field-type-text field-label-inline">
                                <div class="field-label"><?php print ("Beginlife of the SA"); ?>: </div>
                                <div class="field-items"><div class="field-item"> <?php print (isset($node->field_rcabeginlife[LANGUAGE_NONE])?uwwtd_get_DD_MM_YYYY_from_YYYY_MM_DD_XXX($node->field_rcabeginlife[LANGUAGE_NONE][0]['value']) : '-'); ?></div></div>
                                </div>
                            </td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rcaevolutiontype']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['rcasalsaRemark']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                        <tr>
                            <td><?php print render($content['field_rcaendlife']); ?></td>
                            <td></td>
                            <td></td>
                        </tr>
                    </table>
                </div>
            </fieldset>
        </div>
    <?php endif; ?>

    <div class="uwwcontainer">
    <?php if ((isset($node->field_rca54applied[LANGUAGE_NONE]) && $node->field_rca54applied[LANGUAGE_NONE][0]['value'] === '1') ||	  ($node->field_rca54applied[LANGUAGE_NONE][0]['value'] === '1' && $node->field_rca58applied[LANGUAGE_NONE][0]['value'] === '1')) : ?>
	      <div class="uwwthird">
	        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
	          <legend class="panel-heading">
	            <div class="panel-title fieldset-legend"><?php print t('Description').' '. $node->field_anneedata[LANGUAGE_NONE][0]['value'] ;?></div>
	            </legend>
	            <div class="panel-body">
                <?php 
	            print render($content['field_rcaplants']);
	            if (!empty($node->field_rca_total_capacity_uwwtps)) {
	            	print render($content['field_rca_total_capacity_uwwtps']);
	            }
                //Manage N & P 
                $n_inc = 0;
                $n_dis = 0;
                $p_inc = 0;
                $p_dis = 0;
                                
                //================P
                if (!empty($node->field_rcapincomingmeasured) && $node->field_rcapincomingmeasured[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_inc = $node->field_rcapincomingmeasured[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcapincomingmeasured']);
	            }
                elseif (!empty($node->field_rcapincomingcalculated) && $node->field_rcapincomingcalculated[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_inc = $node->field_rcapincomingcalculated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcapincomingcalculated']);
	            }
                elseif (!empty($node->field_rcapincomingestimated) && $node->field_rcapincomingestimated[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_inc = $node->field_rcapincomingestimated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcapincomingestimated']);
	            }
	            elseif (!empty($node->field_rca_total_p_entering) && $node->field_rca_total_p_entering[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_inc = $node->field_rca_total_p_entering[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rca_total_p_entering']);
	            }
                
                if (!empty($node->field_rcapdischargedmeasured) && $node->field_rcapdischargedmeasured[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_dis = $node->field_rcapdischargedmeasured[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcapdischargedmeasured']);
	            }
                elseif (!empty($node->field_rcapdischargedcalculated) && $node->field_rcapdischargedcalculated[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_dis = $node->field_rcapdischargedcalculated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcapdischargedcalculated']);
	            }
                elseif (!empty($node->field_rcapdischargedestimated) && $node->field_rcapdischargedestimated[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_dis = $node->field_rcapdischargedestimated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcapdischargedestimated']);
	            }
	            elseif (!empty($node->field_rca_total_p_discharged) && $node->field_rca_total_p_discharged[LANGUAGE_NONE][0]['value'] > 0) {
                    $p_dis = $node->field_rca_total_p_discharged[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rca_total_p_discharged']);
	            }
                print '<strong>'. t('Total Phosphorus removal (t/year):').'</strong> '. ($p_inc - $p_dis).'<br/>';
                if($p_inc > 0 ) {
                    $rate = 100 * ($p_inc - $p_dis)/$p_inc;
                    $compliance = 'c';
                    if($rate<75) $compliance = 'nc';
                    print '<strong>'. t('Rate of Phosphorus removal (t/year):'). '</strong> <span class="'.$compliance.'">'. number_format($rate, 2, ',', ' ') .'% </span><br/>';
                }
                
                //================N
                if (!empty($node->field_rcanincomingmeasured) && $node->field_rcanincomingmeasured[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_inc = $node->field_rcanincomingmeasured[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcanincomingmeasured']);
	            }
                elseif (!empty($node->field_rcanincomingcalculated) && $node->field_rcanincomingcalculated[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_inc = $node->field_rcanincomingcalculated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcanincomingcalculated']);
	            }
                elseif (!empty($node->field_rcanincomingestimated) && $node->field_rcanincomingestimated[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_inc = $node->field_rcanincomingestimated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcanincomingestimated']);
	            }
	            elseif (!empty($node->field_rca_total_n_entering) && $node->field_rca_total_n_entering[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_inc = $node->field_rca_total_n_entering[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rca_total_n_entering']);
	            }
                
                if (!empty($node->field_rcandischargedmeasured) && $node->field_rcandischargedmeasured[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_dis = $node->field_rcandischargedmeasured[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcandischargedmeasured']);
	            }
                elseif (!empty($node->field_rcandischargedcalculated) && $node->field_rcandischargedcalculated[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_dis = $node->field_rcandischargedcalculated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcandischargedcalculated']);
	            }
                elseif (!empty($node->field_rcandischargedestimated) && $node->field_rcandischargedestimated[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_dis = $node->field_rcandischargedestimated[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rcandischargedestimated']);
	            }
	            elseif (!empty($node->field_rca_total_n_entering) && $node->field_rca_total_n_entering[LANGUAGE_NONE][0]['value'] > 0) {
                    $n_dis = $node->field_rca_total_n_entering[LANGUAGE_NONE][0]['value'];
	            	print render($content['field_rca_total_n_discharged']);
	            }
                print '<strong>'. t('Total Nitrogen removal (t/year):').'</strong> '. ($n_inc-$n_dis).'<br/>';
                if($n_inc > 0 ) {
                    $rate = 100 * ($n_inc-$n_dis)/$n_inc;
                    $compliance = 'c';
                    if($rate<75) $compliance = 'nc';
                    print '<strong>'. t('Rate of Nitrogen removal (t/year):'). '</strong> <span class="'.$compliance.'">'. number_format($rate, 2, ',', ' ') .'% </span><br/>';
                }
                ?>
	          </div>
	        </fieldset>
	      </div>
	<?php endif; ?>
    <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Waste Water Network Connexions'); ?></div>
          </legend>
          <div class="panel-body">
          <?php 
            print render($content['field_linked_agglomerations']);
            print render($content['field_linked_treatment_plants']);
            print render($content['field_rcadcpliste']);
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
          // Get all agglomeration with this field_inspireidlocalid :
          $sourcesFilesUri = uwwtd_get_sourcesfilesuri($node->field_inspireidlocalid[LANGUAGE_NONE][0]['value'], $node->type);
          ?>
          
          <?php 
            print render($content['field_anneedata']);
            $content['field_sourcefile'][0]['#file']->filename = 'See sourcefile';
            print render($content['field_sourcefile']);
            ?>
            <br/>
          <b><i><?php print t('Other years'); ?> :</i></b>
          <br/>
          <?php foreach ($sourcesFilesUri as $sourceFileUri) : ?>
              <?php if ($sourceFileUri->field_anneedata_value != $content['field_anneedata'][0]['#markup']) : ?>
                  <b><?php print t('Year of data'); ?>:</b> <?php print $sourceFileUri->field_anneedata_value; ?>
                  <br/>
                  <b><?php print t('Source of data'); ?>:</b> <img src="<?php print url('modules/file/icons/application-octet-stream.png'); ?>" title="application/xml" alt="file"/>
                  <?php print l(t('See sourcefile'), file_create_url($sourceFileUri->uri)); ?>
                  <br/>
                  <br/>
              <?php endif; ?>
          <?php endforeach; ?>
            
          </div>
        </fieldset>
        <?php if(isset($node->field_article17['und'][0]['nid'])) : ?>
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Forward looking aspect'); ?></div>
          </legend>
          <div class="panel-body">
          </div>
        </fieldset>
        <?php endif; ?>
      </div>
      <?php if (!empty($node->field_rca_sensitive_area[LANGUAGE_NONE][0]['value'])) : ?>
	      <div class="uwwthird">
	        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
	          <legend class="panel-heading">
	            <div class="panel-title fieldset-legend"><?php print t('Related Sensitive area'); ?></div>
	            </legend>
	            <div class="panel-body">
	              <?php print render($content['field_rca_sensitive_area']); ?>
	          </div>
	        </fieldset>
	      </div>
	  <?php endif; ?>

    </div>

  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
  <footer>
    <?php print render($content['field_tags']); ?>
    <?php print render($content['links']); ?>
  </footer>
  <?php endif; ?>
  <?php print render($content['comments']); ?>
</article>