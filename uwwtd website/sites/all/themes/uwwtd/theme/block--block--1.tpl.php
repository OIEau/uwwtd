<?php 
  drupal_add_library('system', 'ui.dialog');
  drupal_add_library ('system', 'effects.transfer');
  drupal_add_library ('system', 'effects.puff');
  drupal_add_library ('system', 'effects.scale');
?>

<script>
(function($){
	$(document).ready(function(){
    $.fn.bootstrapBtn = $.fn.button.noConflict();

		$( "#popup_content" ).dialog({
      title: "About the content of this website",
      autoOpen: false,
      width: 950,
  		modal: true,
  		closeText: "Close",
      show: {
        effect: "puff",
        duration: 250
      },
      hide: {
        effect: "puff",
        duration: 250
      }
    });
 
    $( "a.btnContent" ).click(function() {
      $( "#popup_content" ).dialog( "open" );
    });

		$( "#popup_project" ).dialog({
      title: "About the SIIF project at the origin of this IT development",
      autoOpen: false,
      width: 950,
  		modal: true,
      show: {
        effect: "puff",
        duration: 250
      },
      hide: {
        effect: "puff",
        duration: 250
      }
    });
 
    $( "a.btnProject" ).click(function() {
      $( "#popup_project" ).dialog( "open" );
    });	
	});
})(jQuery);
</script>

<a class="markup" name="<?php print $block_html_id; ?>"></a>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
  <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
  <?php print render($title_suffix); ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
    $nat_stat_str = '';
    if(function_exists("uwwtd_get_max_annee")){
        $year = uwwtd_get_max_annee();
        $nat_stat_str = uwwtd_get_national_stat_str($year);
    }
	
	?>
	<div class ="txtHomePage"><?php print $nat_stat_str; ?></div>
	<div class="btnHomePage">
		<a class="btnContent" rel="popup_content">About the content of this website</a>
		<a class="btnProject" rel="popup_project">About the SIIF project at the origin of this IT development</a>
	</div>
	
	<div id="popup_content" class="popup_content_block">
		<p>This website provides detailed information about the urban waste water agglomerations, 
		treatment plants, discharged points and sensitive areas. 
		It also provides information about the compliance of each agglomeration and treatment plant, 
		the connection to collecting systems or individual and appropriate systems, the treatment in place and pressure on the environment. </p>
		<p>Through view and discovery services it allows to select data and display them depending from the criteria chosen.
		Download and invoke service are also implemented to ease the access to the databases or to ease the reuse of this information by other IT systems. </p>
		<p>It provides an overview of the urban waste water regulation at EU and national level. </p>
		<p>Statistic can be created or are automatically generated in the statistic webpage giving aggregated result of the database reported.</p>
		<p>Through a useful links web page you can also have access to other information related to this policy.</p>
		<p>From the Home webpage you can also have access to different layers and you can click on and off on these layers to make them appear or disappear.</p>
	</div>
	<div id="popup_project" class="popup_project_block">
		<div>
			<span class="ui-helper-hidden-accessible"><input type="text"/></span>
			<p>The Structured Implementation and Information Framework (SIIF) concept is a ongoing European Commission (Environment Directorate General) 
			pilot project essentially focused on the organisation and management of data in order to enhance the generation of information for policy makers, 
			interested parties and the public at all levels on how legislation is practically implemented. 
			The concept developed is in line with the provisions of the Public Access to Environmental Information  and INSPIRE  Directives. </p>
			<p>The SIIF concept is composed of a governance approach and an IT system.</p>
			<p>The SIIF implies a governance focused on compliance, good environmental status, environmental pressure and impact, economic activities and jobs creation. 
			It has also an objective to reduce administrative burden, provide more up-to-date data, and increase efficiency to all potential users. 
			The existence of an implementation programme on how to reach or maintain the compliance for a policy is also part of the SIIF governance approach.</p>
			<p>As regards the IT components of the SIIF it has three components: (1) the content for data community (information needs and how it is organised);
			(2) the process for data providers (who does what and how); (3) the dissemination for data users (how to display) 
			All these components have a national and a European dimension. </p>
			
			<p id="imgProjectA"><img src="<?php print base_path(); ?>sites/all/themes/uwwtd/images/pageaccueil.png" contenteditable="false"/></p>

			<p>This IT development is related to the process and the dissemination of the existing content of the European urban waste water data model (see <a href="http://rod.eionet.europa.eu/obligations/613" title="http://rod.eionet.europa.eu/obligations/613">http://rod.eionet.europa.eu/obligations/613</a>).</p>
			<p>The SIIF project opted for the implementation of an open source dissemination IT toolbox. The tool was designed to disseminate environmental data and information at national level but also to facilitate the reporting process. It provides EU MS a cheap way to implement the Article 11 of the INSPIRE directive and thus to develop their national waste water website. The framework of this website can also be adapted to the use of other European Policies.</p>
		</div>
	</div>
	
  </div>
</div>
