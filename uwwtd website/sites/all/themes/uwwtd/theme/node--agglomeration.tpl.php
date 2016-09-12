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
//       if(($role == $admin)|| ($role == $editor)){
//        $errors = uwwtd_insert_errors_tab($node);
//         if($errors !== false){
//     print $errors;
//     }        
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
  
  <div class="uwwcontainer">
      <?php
        $printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');
        
            if ($view_mode == 'full' && !empty($title)){
                $fieldstat = field_view_field('node', $node, 'field_status');
                $fieldnuts = field_view_field('node', $node, 'field_regionnuts');
                ?>
                <div class="google-map-banner">
                	<h1>
                		<span class="white-title"><?php echo $nodetype; ?> : </span><?php echo $node->title; ?>
                		<span class="white-title"> - Identifier : </span><?php echo $node->field_inspireidlocalid['und'][0]['value']; ?>
                		<span class="white-title"> - Status : </span><?php echo $fieldstat[0]['#markup']; ?>
                		<span class="white-title"> - Reporting year : </span><?php echo $node->field_anneedata['und'][0]['value']; ?>
                		<br />
                		<small>
                			Region (NUTS) Code : <?php echo $node->field_regionnuts['und'][0]['value']; ?> - 
                			Region (NUTS) Name : <?php echo $fieldnuts[0]['#markup'];?>
                		</small>
                	</h1>
                </div>
                <?php 
            }
        print render($printy);
      ?>
      <div class="uwwhalf" style="clear:left;"><?php print uwwtd_timeline_output($node);?></div>
    </div>
    <div>
    	<?php
    	// Affichage du graph inter année :
    	print $variables['htmlGraph']; 
    	?>
    </div>
    <div class="uwwcontainer" style="overflow:hidden;">
      <fieldset class="uwwfull group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper" style="min-height:240px;">
        <legend class="panel-heading">
          <div class="panel-title fieldset-legend"><?php print t('Description').' '.$node->field_anneedata['und'][0]['value']; ?></div>
        </legend>
          <div class="panel-body">
            <div class="uwwrealthird">
              <div class="flip">
               <div class="front">
                  <div class="flip-image"><img src="<?php print file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png');?>" class="button-flipper table-to-chart" title="See diagram" alt="See diagram"></div>
                  <?php 
	                  hide($content['comments']);
	                  hide($content['links']);
	                  hide($content['field_tags']);
                      print render($content['field_agggenerated']);
                   ?>
                   <div class="field-group-aggcx">
                    <?php
                      print uwwtd_render_field_with_pe($content['field_aggc1']);
                      print uwwtd_render_field_with_pe($content['field_aggc2']);
                      print uwwtd_render_field_with_pe($content['field_aggpercwithouttreatment']);  
                    ?>
                   </div>
               </div>
               <div class="back">
                  <div class="flip-title"><?php print t("Generated Load by collection type in population equivalent"); ?></div>
                  <div class="flip-image"><img src="<?php print file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png'); ?>" class="button-flipper chart-to-table" title="See the data table" alt="See the data table"></div>
                  
                  <svg id="agglo_piechart_back"></svg>
                  <?php print uwwtd_piechart_agglonode($node, $content); ?>
               </div>       
              </div>
            </div>
            
            <div class="uwwrealthird">
            <?php
            print render($content['field_aggcompliance']);
              //print render($content['field_compliance_explication']);
              print render($content['field_aggart3compliance']);
              //print render($content['field_article_3_compliance_expli']);
              
              //Gestion de la "legal compliance" (hierarchical)
              if($content['field_aggart3compliance']['#items'][0]['value']=='NC' && $content['field_aggart4compliance']['#items'][0]['value']=='C'){
                $content['field_aggart4compliance']['#items'][0]['value']='NC';  
              }
              print render($content['field_aggart4compliance']);
              //print render($content['field_article_4_compliance_expli']);
              
              //Gestion de la "legal compliance" (hierarchical)
              if($content['field_aggart4compliance']['#items'][0]['value']=='NC' && $content['field_aggart5compliance']['#items'][0]['value']=='C'){
                $content['field_aggart5compliance']['#items'][0]['value']='NC';  
              }
              print render($content['field_aggart5compliance']);
              //print render($content['field_article_5_compliance_expli']);
               // print render($content['field_aggart6compliance']);
            ?>
            </div>
            <?php
                $distanceToCompliance = uuwtd_get_distance_compliance($node);
                // Set the colors of "Distance to compliance" table
                // Article 3
                if ($node->field_aggart3compliance['und'][0]['value'] == 'C'
                    || $node->field_aggart3compliance['und'][0]['value'] == 'QC') 
                {
                    $colorart3 = '#4f91e1';
                } elseif ($node->field_aggart3compliance['und'][0]['value'] == 'NC') {
                    $colorart3 = '#d93c3c';
                } elseif ($node->field_aggart3compliance['und'][0]['value'] == 'PD' 
                    && $node->field_aggpercwithouttreatment['und'][0]['value'] > 0) 
                {
                    $colorart3 = '#FF5200';
                } else {
                    $colorart3 = '#a2a2a2';
                }

                // Article 4
                if ($distanceToCompliance['art4_treatment_compliance'] == 'NC' && 
                    deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === true &&
                    $distanceToCompliance['art4_treat_pe'] > 0) {
                    $colorart4T = '#d93c3c';
                } elseif ($distanceToCompliance['art4_treatment_compliance'] == 'NC' && 
                        deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === false) {
                    $colorart4T = '#FF5200';
                } else {
                    $colorart4T = '#4f91e1';
                }

                if ($distanceToCompliance['art4_perf_compliance'] == 'NC' &&
                    deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === true &&
                    $distanceToCompliance['art4_perf_pe'] > 0) {
                    $colorart4P = '#d93c3c';
                } elseif ($distanceToCompliance['art4_perf_compliance'] == 'NC' && 
                        deadline_beforeorequal_to_referenceyear($node->field_aggperiodover4['und'][0]['value'], $node->field_anneedata['und'][0]['value']) === false) {
                    $colorart4P = '#FF5200';
                } else {
                    $colorart4P = '#4f91e1';
                }

                if ($node->field_aggart4compliance['und'][0]['value'] == 'NR') {
                    $colorart4T = '#a2a2a2';
                    $colorart4P = '#a2a2a2';
                } 

                // If art 3 not compliant because discharged without treatment too important
                // we add it to art 4 and 5 if relevant
                if ($node->field_aggpercwithouttreatment['und'][0]['value'] > 0
                  && $node->field_aggart3compliance['und'][0]['value'] == 'NC') {
                    $distanceToCompliance['art4_treat_pe'] += uwwtd_field_pe($content['field_aggpercwithouttreatment'], false);
                    $distanceToCompliance['art4_perf_pe'] += uwwtd_field_pe($content['field_aggpercwithouttreatment'], false);
                    $distanceToCompliance['art4_treat_percent'] += $node->field_aggpercwithouttreatment['und'][0]['value'];
                    $distanceToCompliance['art4_perf_percent'] += $node->field_aggpercwithouttreatment['und'][0]['value'];
                    $colorart4T = '#d93c3c';
                    $colorart4P = '#d93c3c';
                    if ($distanceToCompliance['art5_treat_compliance'] != 'NR') {
                      $distanceToCompliance['art5_treat_pe'] += uwwtd_field_pe($content['field_aggpercwithouttreatment'], false);
                      $distanceToCompliance['art5_perf_pe'] += uwwtd_field_pe($content['field_aggpercwithouttreatment'], false);
                      $distanceToCompliance['art5_treat_percent'] += $node->field_aggpercwithouttreatment['und'][0]['value'];
                      $distanceToCompliance['art5_perf_percent'] += $node->field_aggpercwithouttreatment['und'][0]['value'];
                      $colorart5T = '#d93c3c';
                      $colorart5P = '#d93c3c';
                    }
                }

                // Article 5
                if ($distanceToCompliance['art5_treat_compliance'] == 'NC') {
                    $colorart5T = '#d93c3c';
                } else {
                    $colorart5T = '#4f91e1';
                }

                if ($distanceToCompliance['art5_perf_compliance'] == 'NC') {
                    $colorart5P = '#d93c3c';
                } else {
                    $colorart5P = '#4f91e1';
                }

                if ($node->field_aggart5compliance['und'][0]['value'] == 'NR') {
                    $colorart5T = '#a2a2a2';
                    $colorart5P = '#a2a2a2';
                }

                if ($node->field_aggart5compliance['und'][0]['value'] == 'PD' && $distanceToCompliance['art5_perf_pe'] > 0) {
                    $colorart5P = '#FF5200';
                }

                if ($node->field_aggart5compliance['und'][0]['value'] == 'PD' && $distanceToCompliance['art5_treat_pe'] > 0) {
                    $colorart5T = '#FF5200';
                }

                if ($distanceToCompliance['art5_perf_pe'] === '-') {
                    $colorart5P = '#a2a2a2';
                }
            ?>
            
            
            <div class="uwwrealthird">
                <div class="distance">
                    <div class="field custom_tooltip_wrapper">
                        <div class="field-label"><?php print t('Distance to compliance:'); ?></div>
                            <span class="custom_tooltip"><?php print t('The distance to compliance concept present the rate of waste water load that is:'); ?>
                            <ul>
                                <li><?php print t('adequately connected to a centralised urban waste water collecting system or addressed via Individual or Appropriate System (IAS)'); ?></li>
                            </ul>
                            <?php print t('and then when collected:'); ?>
                            <ul>
                                <li><?php print t('treated at an adequate level (secondary or more stringent treatment) as required by the directive,'); ?></li>
                                <li><?php print t('and with the performance requirements under tables 1 or 2 of the annex I of Directive 91/271/EEC, (UWWTD).'); ?></li>
                            </ul>
                            <?php print t('When countries joined the EU, they have obtained a delay to implement the Directive. 
                            In such case the deadline for implementing the above (see in section characteristics) may be different, 
                            and if the above is not already implemented for the agglomeration it is identified with orange colour in the table.'); ?></span>
                    </div>

                    <table id="UwwtpDescription">
                        <tr>
                            <td style="font-weight:bold;border: 1px solid #000;"></td>
                            <td style="font-weight:bold;border: 1px solid #000;"><?php print t('Equipment'); ?></td>
                            <td style="font-weight:bold;border: 1px solid #000;"><?php print t('Performance'); ?></td>
                        </tr>
                        <tr>
                            <td class="black" style="font-weight:bold;" rowspan="2"><?php print t('Connection'); ?></td>
                            <td class="light" style="background-color:<?php print $colorart3;?>;color:white;"><?php print uwwtd_field_num($content['field_aggpercwithouttreatment']);?>%</td>
                            <td class="black"></td>
                        </tr>
                        <tr>
                            <td class="light" style="background-color:<?php print $colorart3;?>;color:white;"><?php 
                                print uwwtd_field_pe($content['field_aggpercwithouttreatment']);
                            ?> p.e</td>
                        </tr>
                        <tr>
                            <td class="black" style="font-weight:bold;" rowspan="2"><?php print t('2nd treatment'); ?></td>
                            <td class="light" style="background-color:<?php print $colorart4T;?>;color:white;"><?php print number_format($distanceToCompliance['art4_treat_percent'],1, ',', ' ');?>%</td>
                            <td class="black"  style="background-color:<?php print $colorart4P;?>;color:white;"><?php print number_format($distanceToCompliance['art4_perf_percent'],1, ',', ' ');?>%</td>
                        </tr>
                        <tr>
                            <td class="light" style="background-color:<?php print $colorart4T;?>;color:white;"><?php print number_format($distanceToCompliance['art4_treat_pe'],0, ',', ' ');?> p.e </td>
                            <td class="black" style="background-color:<?php print $colorart4P;?>;color:white;"><?php print number_format($distanceToCompliance['art4_perf_pe'],0, ',', ' ');?> p.e </td>
                        </tr>
                        <tr>
                            <td class="black" style="font-weight:bold;" rowspan="2"><?php print t('3rd treatment'); ?></td>
                            <td class="light" style="background-color:<?php print $colorart5T;?>;color:white;"><?php print ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_treat_percent'],1).'%');?></td>
                            <td class="black"  style="background-color:<?php print $colorart5P;?>;color:white;"><?php print ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_perf_percent'],1).'%');?></td>
                        </tr>
                        <tr>
                            <td class="light" style="background-color:<?php print $colorart5T;?>;color:white;"><?php print ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_treat_pe'],0).' p.e');?></td>
                            <td class="black" style="background-color:<?php print $colorart5P;?>;color:white;"><?php print ((integer)$node->field_agggenerated['und'][0]['value'] < 10000 ? '-':uwwtd_format_number($distanceToCompliance['art5_perf_pe'],0).' p.e');?></td>
                        </tr>
                    </table>
                </div>
            </div>
        </div>
      </fieldset>
    </div>

    <div class="uwwcontainer" style="overflow:visible">
      <fieldset class="uwwfull group-agggraphic field-group-fieldset group-description panel panel-default form-wrapper">
        <legend class="panel-heading">
          <div class="panel-title fieldset-legend"><?php print t('Graphical network summary');?></div>
            </legend>
          <div class="panel-body">
              
            <?php print uwwtd_get_agglo_graphic($node); ?>

          </div>
      </fieldset>
    </div>
   
    <div class="uwwcontainer">
      <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Characteristics');?></div>
          </legend>
          <div class="panel-body">
          <div class ="field field-name-field-agglatitude field-type-number-decimal field-label-inline clearfix">
                <div class = "field-label"><a href="<?php print url('<front>');?>#zoom=17&lat=<?php print $content['field_agglatitude']['#items'][0]['value'];?>&lon=<?php print $content['field_agglongitude']['#items'][0]['value'];?>&layers=Agglomerations&baseLayers=Google%20Maps%20Normal" target="_blank"> Latitude : </a></div>
                    <div class="field-items">
                        <div class="field-item even"> &nbsp;<?php print $content['field_agglatitude']['#items'][0]['value'];?></div>
                    </div>
            </div>
            <div class ="field field-name-field-agglongitude field-type-number-decimal field-label-inline clearfix">
                <div class = "field-label"><a href="<?php print url('<front>');?>#zoom=17&lat=<?php print $content['field_agglatitude']['#items'][0]['value'];?>&lon=<?php print $content['field_agglongitude']['#items'][0]['value'];?>&layers=Agglomerations&baseLayers=Google%20Maps%20Normal" target="_blank"> Longitude : </a></div>
                    <div class="field-items">
                        <div class="field-item even"> &nbsp;<?php print $content['field_agglongitude']['#items'][0]['value'];?></div>
                    </div>
            </div>
            <?php 
                print render($content['field_aggperiodover3']);
                print render($content['field_aggperiodover4']);
                //dsm($content['field_aggperiodover5']);
                //
                if((integer)$node->field_agggenerated['und'][0]['value'] < 10000){
                    $content['field_aggperiodover5'][0]['#markup']='<span>Not Relevant</span>';
                }
                 print render($content['field_aggperiodover5']);
                
                print render($content['field_aggperiodover6']);
                print render($content['field_agghaveregistrationsystem']);
                print render($content['field_aggexistmaintenanceplan']);
                print render($content['field_aggbesttechnicalknowledge']);
                print render($content['field_aggsewagenetwork']);
            ?>
          </div>
        </fieldset>
      </div>
      <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Waste Water Network Connexions');?></div>
          </legend>
          <div class="panel-body">
           <?php 
            print render($content['field_linked_treatment_plants']);
            print render($content['field_linked_discharge_points']);
            print render($content['field_linked_receiving_areas']);
            //print render($content['field_agguwwliste']);
           ?>
          </div>
        </fieldset>
      </div>
      <?php 
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
      ?>
      <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Site information');?></div>
          </legend>
          <div class="panel-body">
          <?php 
            if(isset($arrayFile)){
                foreach($arrayFile as $files){
                    if($files['year'] == $content['field_anneedata'][0]['#markup']){
                        if($files['file'] == $content['field_sourcefile'][0]['#file']->filename){
                        }
                    }else{
                        
                        $outputSiteInfo = '<br><b>Year of data: </b>'.$files['year'].'<br>';
                        $outputSiteInfo.= '<b>Source of data: </b><img src="'.url('modules/file/icons/application-octet-stream.png').'" title="application/xml" alt="file"/>';
                        $outputSiteInfo.= l('See sourcefile', 'public://data_sources/'.$files['file']);
                    }
                } 
            }
            print render($content['field_anneedata']);
            $content['field_sourcefile'][0]['#file']->filename = 'See sourcefile';
            print render($content['field_sourcefile']).'<br/>';
            if(isset($outputSiteInfo) && $outputSiteInfo !=""){
                print '<b><i>Other years : </i></b>';
            }
            
            print $outputSiteInfo;
          ?>
          </div>
        </fieldset>
        <?php if(isset($node->field_article17_agglo['und'][0]['nid'])): ?>
            <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
              <legend class="panel-heading">
                <div class="panel-title fieldset-legend"><?php print t('Forward looking aspect'); ?></div>
              </legend>
              <div class="panel-body">
                <?php print uwwtd_render_article17_aglo($node->field_article17_agglo['und'][0]['nid']); ?>
              </div>
            </fieldset>
        <?php endif; ?>
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