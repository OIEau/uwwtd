<script>
(function($){
	$(document).ready(function(){
		$('.field-name-field-physicalcapacityactivity').addClass('clearfix');
		
		var bod = $('.field-name-field-uwwbod5perf .field-label').text();
		var bod5 = bod.split(':');
		$('.field-name-field-uwwbod5perf .field-label').text(bod5[1] +': ');
		$('.field-name-field-uwwbod5perf .field-label').append("&nbsp;");
		var cod = $('.field-name-field-uwwcodperf .field-label').text();
		var cod = cod.split(':');
		$('.field-name-field-uwwcodperf .field-label').text(cod[1] +': ');
		$('.field-name-field-uwwcodperf .field-label').append("&nbsp;");
		var tss = $('.field-name-field-uwwtssperf .field-label').text();
		var tss = tss.split(':');
		$('.field-name-field-uwwtssperf .field-label').text(tss[1] +': ');
		$('.field-name-field-uwwtssperf .field-label').append("&nbsp;");
		var ntot = $('.field-name-field-uwwntotperf .field-label').text();
		var ntot = ntot.split(':');
		$('.field-name-field-uwwntotperf .field-label').text(ntot[1] +': ');
		$('.field-name-field-uwwntotperf .field-label').append("&nbsp;");
		var ptot = $('.field-name-field-uwwptotperf .field-label').text();
		var ptot = ptot.split(':');
		$('.field-name-field-uwwptotperf .field-label').text(ptot[1] +': ');
		$('.field-name-field-uwwptotperf .field-label').append("&nbsp;");
		var other = $('.field-name-field-uwwotherperf .field-label').text();
		var other = other.split(':');
		$('.field-name-field-uwwotherperf .field-label').text(other[1] +': ');
		$('.field-name-field-uwwotherperf .field-label').append("&nbsp;");
	});
})(jQuery);
</script>
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
   
    $printy = field_view_field('node', $node, 'field_position_geo', 'openlayers_map');

    print '<div class="uwwcontainer" style="margin-top:-50px">';
		if ($view_mode == 'full' && !empty($title)){
			$fieldstat = field_view_field('node', $node, 'field_status');
			$fieldnuts = field_view_field('node', $node, 'field_regionnuts');
			print '<h1 style="z-index: 4;top:200px;color:white;"><span class="white-title">'.$nodetype.'</span>'.t(' : ').$node->title.'<span class="white-title">'.' - '.t('Identifier').t(' : ').'</span>'.$node->field_inspireidlocalid['und'][0]['value'].'<span class="white-title">'.' - '.t('Status').t(' : ').'</span>'.$fieldstat[0]['#markup'].'<span class="white-title">'.' - '.t('Reporting year').t(' : ').'</span>'.$node->field_anneedata['und'][0]['value'];
			if(isset($node->field_regionnuts['und'][0]['value'])) print '<br><small>'.t('Region (NUTS) Code : ').$node->field_regionnuts['und'][0]['value'].'</small>';
			if(isset($fieldnuts[0]['#markup'])) print '<small> - '.t('Region (NUTS) Name : ').$fieldnuts[0]['#markup'].'</small>';
			print '<br>';
			if((!isset($node->field_uwwcollectingsystem['und'][0]['value']) || $node->field_uwwcollectingsystem['und'][0]['value'] == 'NOTCON') || (!isset($node->field_physicalcapacityactivity['und'][0]['value']) || $node->field_physicalcapacityactivity['und'][0]['value'] == '0')){
				print t('COLLECTING SYSTEM WITHOUT TREATMENT PLANT');
			}
			print '</h1>';
		}
		echo render($printy);

      print '<div class="uwwhalf" style="clear:left;">';
        echo uwwtd_timeline_output($node);
      print '</div>';
    print '</div>';
    print '<div class="uwwcontainer" style="float:left;overflow:visible;">';
      print '<fieldset class="uwwfull group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper" style="min-height:390px;">';
        print '<legend class="panel-heading">';
          print '<div class="panel-title fieldset-legend">'.t('Description').' '.$node->field_anneedata['und'][0]['value'].'</div>';
            print '</legend>';
          print '<div class="panel-body">';
            print '<div class="uwwrealthird">';
              print render($content['field_uwwloadenteringuwwtp']);
              print render($content['field_physicalcapacityactivity']);
              print render($content['field_uwwwastewatertreated']);
			 
			  print '<div class="treatmentPerformance" style="clear:both;">';
			  
			   print '<div style="font-weight:bold";>'.t('Treatment performance :').'</div>';
			  $list = array(
				render($content['field_uwwbod5perf']),
				render($content['field_uwwcodperf']),
				render($content['field_uwwtssperf']),
				render($content['field_uwwntotperf']),
				render($content['field_uwwptotperf']),
				render($content['field_uwwotherperf'])
			  );
			  print theme('item_list', array('items'=>$list));
			  print '</div>';
			  print '<div class="dataOfClosing" style="clear:both;">';
			  if(isset($content['field_validto']) && $content['field_validto']['#items'][0]['value'] != ""){
				   print render($content['field_validto']);
			  }
			print '</div>';
            print '</div>';
			
            print '<div class="uwwrealthird">';
              print '<div class="flip" id="uwwtp_stackedbar" style="position:relative;">
                       <div class="front">';
                         //BOD
						  if(isset($node->field_uwwbodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwbodincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwbodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingLoadBod= round($node->field_uwwbodincoming[LANGUAGE_NONE][0]['value'],2);
								$incomingBod = ($node->field_uwwbodincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultIncomingBod = round($incomingBod,2);
							}
						  }
						if(isset($node->field_uwwboddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwboddischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwboddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeLoadBod = round($node->field_uwwboddischarge[LANGUAGE_NONE][0]['value'],2);
								$dischargeBod = ($node->field_uwwboddischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultDischargedBod = round($dischargeBod,2);
							}
						}
						if(isset($node->field_uwwboddischarge['und'][0]['value']) && isset($node->field_uwwbodincoming['und'][0]['value']) && $node->field_uwwbodincoming['und'][0]['value']!=0){
                             $rateBod = round(((1- ($node->field_uwwboddischarge['und'][0]['value'] / $node->field_uwwbodincoming['und'][0]['value']))*100), 1) . '%';
						}
						//COD
						if(isset($node->field_uwwcodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwcodincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwcodincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingLoadCod= round($node->field_uwwcodincoming[LANGUAGE_NONE][0]['value'],2);
								$incomingCod = ($node->field_uwwcodincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultIncomingCod = round($incomingCod,2);
							}
						}
						if(isset($node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeLoadCod = round($node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'],2);
								$dischargeCod = ($node->field_uwwcoddischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultDischargedCod = round($dischargeCod,2);
							}
						}
						if(isset($node->field_uwwcoddischarge['und'][0]['value']) && isset($node->field_uwwcodincoming['und'][0]['value']) && $node->field_uwwcodincoming['und'][0]['value']!=0){
                            $rateCod = round(( (1-($node->field_uwwcoddischarge['und'][0]['value'] / $node->field_uwwcodincoming['und'][0]['value'])) *100), 1) . '%';
                        }
						//Nitrogen
						if(isset($node->field_uwwnincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwnincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwnincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwnincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingLoadN= round($node->field_uwwnincoming[LANGUAGE_NONE][0]['value'],2);
								$incomingN = ($node->field_uwwnincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultIncomingN = round($incomingN,2);
							}
						}
						if(isset($node->field_uwwndischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwndischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwndischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwndischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeLoadN = round($node->field_uwwndischarge[LANGUAGE_NONE][0]['value'],2);
								$dischargeN = ($node->field_uwwndischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultDischargedN = round($dischargeN,2);
							}
						}
						if(isset($node->field_uwwndischarge['und'][0]['value']) && isset($node->field_uwwnincoming['und'][0]['value']) && $node->field_uwwnincoming['und'][0]['value']!=0){
                            $rateN =round(( (1- ($node->field_uwwndischarge['und'][0]['value'] / $node->field_uwwnincoming['und'][0]['value'])) *100), 1) . '%';
                        }
						//Phosphorus
						if(isset($node->field_uwwpincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwpincoming[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwpincoming[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwpincoming[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$incomingLoadP= round($node->field_uwwpincoming[LANGUAGE_NONE][0]['value'],2);
								$incomingP = ($node->field_uwwpincoming[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultIncomingP = round($incomingP,2);
							}
						}
						if(isset($node->field_uwwpdischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] != "" && $node->field_uwwpdischarge[LANGUAGE_NONE][0]['value']!= "Not provided"){
							if(isset($node->field_uwwpdischarge[LANGUAGE_NONE][0]['value']) && $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value'])
							{
								$dischargeLoadP = round($node->field_uwwpdischarge[LANGUAGE_NONE][0]['value'],2);
								$dischargeP = ($node->field_uwwpdischarge[LANGUAGE_NONE][0]['value'] / $node->field_uwwwastewatertreated[LANGUAGE_NONE][0]['value']) *1000000;
								$resultDischargedP =  round($dischargeP,2);
							}
						}
						if(isset($node->field_uwwpdischarge['und'][0]['value']) && isset($node->field_uwwpincoming['und'][0]['value']) && $node->field_uwwpincoming['und'][0]['value'] != 0){
                            $rateP = round(( (1-($node->field_uwwpdischarge['und'][0]['value'] / $node->field_uwwpincoming['und'][0]['value'])) *100), 1) . '%';
                        } 
						if(isset($incomingLoadBod)|| isset($resultIncomingBod) || isset($dischargeLoadBod)|| isset($resultDischargedBod)|| 
						   isset($incomingLoadCod)|| isset($resultIncomingCod)|| isset($dischargeLoadCod)|| isset($resultDischargedCod)||
						   isset($incomingLoadN)|| isset($resultIncomingN)|| isset($dischargeLoadN)|| isset($resultDischargedN)||
						   isset($incomingLoadP)|| isset($resultIncomingP)|| isset($dischargeLoadP)|| isset($resultDischargedP)
						)
						{
							  print '<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-chart-off.png').'" class="button-flipper table-to-chart" title="See diagram" alt="See diagram"></div>';
						  $output = '<p style="font-weight:bold;">Load and concentration per parameter :</p>';						  
						$output .='<table id="UwwtpDescription">
								 <tr>
									<td style="border: 1px solid #000;" class="black" ></td>
									<td style="font-weight: bold;border: 1px solid #000;" class="black">Incoming</td>
									<td style="font-weight: bold;border: 1px solid #000;" class="black">Discharged</td>
									<td style="font-weight: bold;border: 1px solid #000;" class="black">Rate</td>
								</tr>';
						$output .='<tr>
									<td rowspan=2 style="font-weight: bold;" class="black">BOD</td>
									<td class="light">'.(isset($incomingLoadBod)?$incomingLoadBod.' t/year':'').'</td>
									<td class="light">'.(isset($dischargeLoadBod)?$dischargeLoadBod.' t/year':'').'</td>
									<td rowspan=2  class="black">'.(isset($rateBod)?$rateBod:'').'</td>
								</tr>';
						$output .='<tr>
									<td class="light">'.(isset($resultIncomingBod)?$resultIncomingBod.' mg/l':'').'</td>
									<td class="light">'.(isset($resultDischargedBod)?$resultDischargedBod.' mg/l':'').'</td>
								</tr>';
						$output .='<tr>
									<td rowspan=2 style="font-weight: bold;" class="black">COD</td>
									<td class="light">'.(isset($incomingLoadCod)?$incomingLoadCod.' t/year':'').'</td>
									<td class="light">'.(isset($dischargeLoadCod)?$dischargeLoadCod.' t/year':'').'</td>
									<td rowspan=2  class="black">'.(isset($rateCod)?$rateCod:'').'</td>
								</tr>';
						$output .='<tr>
									<td class="light">'.(isset($resultIncomingCod)?$resultIncomingCod.' mg/l':'').'</td>
									<td class="light">'.(isset($resultDischargedCod)?$resultDischargedCod.' mg/l':'').'</td>
								</tr>';
						$output .='<tr>
									<td rowspan=2 style="font-weight: bold;" class="black">Nitrogen</td>
									<td class="light">'.(isset($incomingLoadN)?$incomingLoadN.' t/year':'').'</td>
									<td class="light">'.(isset($dischargeLoadN)?$dischargeLoadN.' t/year':'').'</td>
									<td rowspan=2  class="black">'.(isset($rateN)?$rateN:'').'</td>
								</tr>';
						$output .='<tr>
									<td class="light">'.(isset($resultIncomingN)?$resultIncomingN.' mg/l':'').'</td>
									<td class="light">'.(isset($resultDischargedN)?$resultDischargedN.' mg/l':'').'</td>
								</tr>';
						$output .='<tr>
									<td rowspan=2 style="font-weight: bold;" class="black">Phosphorus</td>
									<td class="light">'.(isset($incomingLoadP)?$incomingLoadP.' t/year':'').'</td>
									<td class="light">'.(isset($dischargeLoadP)?$dischargeLoadP.' t/year':'').'</td>
									<td rowspan=2  class="black">'.(isset($rateP)?$rateP:'').'</td>
								</tr>';	
						$output .='<tr>
									<td class="light">'.(isset($resultIncomingP)?$resultIncomingP.' mg/l':'').'</td>
									<td class="light">'.(isset($resultDischargedP)?$resultDischargedP.' mg/l':'').'</td>
								</tr>';		
						$output .= '</table>';
						$output .= '* Concentration calculated using the annual load and the annual volume of wastewater treated.';
						print $output;
						}                     
              print'   </div>
			  
			    <div class="back" style="position:absolute;">';
			  if(isset($incomingLoadBod)|| isset($resultIncomingBod) || isset($dischargeLoadBod)|| isset($resultDischargedBod)|| 
			   isset($incomingLoadCod)|| isset($resultIncomingCod)|| isset($dischargeLoadCod)|| isset($resultDischargedCod)||
			   isset($incomingLoadN)|| isset($resultIncomingN)|| isset($dischargeLoadN)|| isset($resultDischargedN)||
			   isset($incomingLoadP)|| isset($resultIncomingP)|| isset($dischargeLoadP)|| isset($resultDischargedP)
			)
			{
				print '<div class="flip-title">Incoming and discharged loads (t/year)</div>';
                print'<div class="flip-image"><img src="'.file_create_url(drupal_get_path('theme', 'uwwtd').'/images/corner-table-off.png').'" class="button-flipper chart-to-table" title="See the data table" alt="See the data table"></div>';
				echo uwwtd_stackedbar_uwwtpnode($node);
             } 
			 print'   </div> ';
					
			 print'  </div>';
                                                                                  
            print '</div>';  
            print '<div class="uwwrealthird">';
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
?>
    <div class="uwwcontainer" style="overflow:visible">
      <fieldset class="uwwfull group-agggraphic field-group-fieldset group-description panel panel-default form-wrapper">
        <legend class="panel-heading">
          <div class="panel-title fieldset-legend"><?php print t('Graphical network summary'); ?></div>
        </legend>
          <div class="panel-body">
            <?php    print uwwtd_get_uww_graphic($node); ?>

          </div>
      </fieldset>
    </div>

    <div class="uwwcontainer">
      <div class="uwwthird">
        <fieldset class="group-aggdescription field-group-fieldset group-description panel panel-default form-wrapper">
          <legend class="panel-heading">
            <div class="panel-title fieldset-legend"><?php print t('Characteristics'); ?></div>
          </legend>
		  <?php 
		  $nameurls=explode('/',$_SERVER['REQUEST_URI']);
		  $nameurl= $nameurls[1];
		  ?>
          <div class="panel-body">
			<div class ="field field-name-field-uwwlatitude field-type-number-decimal field-label-inline clearfix">
				<div class = "field-label"><?php print '<a href="'.url('<front>').'#zoom=17&lat='.$content['field_uwwlatitude']['#items'][0]['value'].'&lon='.$content['field_uwwlongitude']['#items'][0]['value'].'&layers=Treatment plants&baseLayers=Google%20Maps%20Normal" target="_blank"> Latitude : </a>';?> </div>
					<div class="field-items">
						<div class="field-item even"> &nbsp;<?php print $content['field_uwwlatitude']['#items'][0]['value']; ?></div>
					</div>
			</div>
			<div class ="field field-name-field-uwwlongitude field-type-number-decimal field-label-inline clearfix">
				<div class = "field-label"><?php print '<a href="'.url('<front>').'#zoom=17&lat='.$content['field_uwwlatitude']['#items'][0]['value'].'&lon='.$content['field_uwwlongitude']['#items'][0]['value'].'&layers=Treatment plants&baseLayers=Google%20Maps%20Normal" target="_blank"> Longitude : </a>';?> </div>
					<div class="field-items">
						<div class="field-item even"> &nbsp;<?php print $content['field_uwwlongitude']['#items'][0]['value']; ?></div>
					</div>
			</div>

            <h4><?php print t('Treatment in place :'); ?></h4>
            <span class="bump"></span><?php print render($content['field_uwwprimarytreatment']); ?>
            <span class="bump"></span><?php print render($content['field_uwwsecondarytreatment']); ?>
            <h4><?php print('More stringent treatment :'); ?></h4>
            <span class="bump"></span><?php print render($content['field_uwwnremoval']);?>
            <span class="bump"></span><?php print render($content['field_uwwpremoval']);?>
            <span class="bump"></span><?php print render($content['field_uwwuv']);?>
            <span class="bump"></span><?php print render($content['field_uwwchlorination']);?>
            <span class="bump"></span><?php print render($content['field_uwwozonation']);?>
            <span class="bump"></span><?php print render($content['field_uwwsandfiltration']);?>
            <span class="bump"></span><?php print render($content['field_uwwmicrofiltration']);?>
            <span class="bump"></span><?php print render(field_view_field('node', $node, 'field_uwwothertreat', array('label' => 'inline')));?>
          </div>
        </fieldset>
      </div>
	<?php
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
	  $option['field_anneedata_value'] = uwwtd_get_all_year();
	  $nbOptionannee = count($option['field_anneedata_value']);
	  $type = $content['field_sourcefile']['#bundle'];
	   foreach($option['field_anneedata_value'] as $optionAnnee){
			$option['year'] = $optionAnnee;
			$option['uwwCode'] = $content['field_inspireidlocalid'][0]['#markup'];
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
	   // dsm($arrayFile);
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
