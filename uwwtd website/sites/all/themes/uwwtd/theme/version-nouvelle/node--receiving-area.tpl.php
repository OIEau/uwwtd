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
 ?>
<?php 

  switch ($node->field_specialisedzonetype['und'][0]['value']) {
    case 'SA':
      $nodetype = t('Sensitive area');
      break;
    case 'CSA':
      $nodetype = t('Catchment of sensitive area');
      break;
    case 'NA':
      $nodetype = t('Normal area');
      break;
    case 'LSA':
      $nodetype = t('Less sensitive area');
      break;
    
    default:
      $nodetype = t('Area');
      break;
  }
  //dsm($content);
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
    if ($view_mode == 'full' && !empty($title)){
        $fieldstat = field_view_field('node', $node, 'field_status');
        //print '<h1>'.$node->title.' <small><i>- '.$nodetype.', '.t('reporting year: ').$node->field_anneedata['und'][0]['value'].'</i></small>';
        print '<h1><span class="black-title">'.$nodetype.'</span>'.t(' : ').$node->title.'<span class="black-title">'.' - '.t('Identifier').t(' : ').'</span>'.$node->field_inspireidlocalid['und'][0]['value'].'<span class="black-title">'.' - '.t('Status').t(' : ').'</span>'.$fieldstat[0]['#markup'].'<span class="black-title">'.' - '.t('Reporting year').t(' : ').'</span>'.$node->field_anneedata['und'][0]['value'];
        print '</h1><br>';
    }

    //print render($content);
    $printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');

    print '<div class="uwwcontainer">';
      echo render($printy);
      print '<div class="uwwhalf">';
        echo uwwtd_timeline_output($node);
      print '</div>';
    print '</div>';
   
    if($node->field_specialisedzonetype['und'][0]['value'] == 'SA' || $node->field_specialisedzonetype['und'][0]['value'] == 'CSA'){
      print '<div class="uwwcontainer">';
      print '<fieldset class="uwwfull group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
        print '<legend class="panel-heading">';
          print '<div class="panel-title fieldset-legend">'.t('Charac').' '.$node->field_anneedata['und'][0]['value'].'</div>';
            print '</legend>';
          print '<div class="panel-body">';
            print '<div class="uwwrealquart">';
              print render(field_view_field('node', $node, 'field_aggpercwithouttreatment', array('label' => 'inline')));
            print '</div>';
            print '<div class="uwwrealquart">';
            print '&nbsp;';
            print '</div>';
            print '<div class="uwwrealquart">';
              print render($content['field_aggcompliance']);
            print '</div>';
            print '<div class="uwwrealquart">';
              print render($content['field_aggcompliance']);
            print '</div>';
        print '</div>';
      print '</fieldset>';
    print '</div>';
    }
    print '<div class="uwwcontainer">';
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Characteristics').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_agglatitude']);
            print render($content['field_agglongitude']);
            print render($content['field_aggperiodover3']);
            print render($content['field_aggperiodover4']);
            print render($content['field_aggperiodover5']);
            print render($content['field_aggperiodover6']);
            print render($content['field_agghaveregistrationsystem']);
            print render($content['field_aggexistmaintenanceplan']);
      print render($content['field_aggbesttechnicalknowledge']);
          print '</div>';
        print '</fieldset>';
      print '</div>';
      print '<div class="uwwthird">';
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Waste Water Network Connexions').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            print render($content['field_agguwwliste']);
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
            print render($content['field_sourcefile']);
          print '</div>';
        print '</fieldset>';
        if(isset($node->field_article17['und'][0]['nid'])){
        print '<fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">';
          print '<legend class="panel-heading">';
            print '<div class="panel-title fieldset-legend">'.t('Forward looking aspect').'</div>';
          print '</legend>';
          print '<div class="panel-body">';
            //print render($content['field_article17']);
            print uwwtd_render_article17($node->field_article17['und'][0]['nid']);
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