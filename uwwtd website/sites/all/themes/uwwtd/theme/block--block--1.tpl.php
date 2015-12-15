<script>
(function($){
	$(document).ready(function(){
		var largeur_fenetre = $(window).width();
		$('a.btnContent').click(function() {
			var popID = $(this).attr('rel'); //Trouver la pop-up correspondante
			var popURL = $(this).attr('href'); //Retrouver la largeur dans le href
			var imgUrl = "http://" + location.host+ location.pathname;
			//Récupérer les variables depuis le lien
			var query= popURL.split('?');
			var dim= query[1].split('&amp;');
			var popWidth = dim[0].split('=')[1]; //La première valeur du lien

			//Faire apparaitre la pop-up et ajouter le bouton de fermeture
			$('#' + popID).fadeIn().css({
				'width': Number(popWidth)
			})
			.prepend('<div class="div_btn_close" style="position:fixed;"><a href="#" class="close"><img src='+imgUrl +'sites/all/themes/uwwtd/images/popclose.png class="btn_close" title="Fermer" alt="Fermer" /></a></div>');

			if(largeur_fenetre < 1350){
				$('.popup_content_block .div_btn_close').css('right', '9%');
				$('.popup_content_block .div_btn_close').css('top', '24%');
			}else{
				$('.popup_content_block').css('margin-left', '-10%');
				$('.popup_content_block .div_btn_close').css('right', '8%');
				$('.popup_content_block .div_btn_close').css('top', '27%');
			}
			
			//Récupération du margin, qui permettra de centrer la fenêtre - on ajuste de 80px en conformité avec le CSS
			var popMargTop = ($('#' + popID).height() + 80) / 2;
			var popMargLeft = ($('#' + popID).width() + 80) / 2;

			//Effet fade-in du fond opaque
			$('body').append('<div id="fade"></div>'); //Ajout du fond opaque noir
			//Apparition du fond - .css({'filter' : 'alpha(opacity=80)'}) pour corriger les bogues de IE
			 $('#fade').css({'filter' : 'alpha(opacity=80)'}).fadeIn();

			return false;
		});
		//Fermeture de la pop-up et du fond
		$('a.close, #fade').live('click', function() { //Au clic sur le bouton ou sur le calque...
			$('#fade , .popup_content_block').fadeOut(function() {
				$('#fade, a.close').remove();  //...ils disparaissent ensemble
			});
			return false;
		});
		
		//Btn project
		$('a.btnProject').click(function() {
			
				
			var popID = $(this).attr('rel'); //Trouver la pop-up correspondante
			var popURL = $(this).attr('href'); //Retrouver la largeur dans le href
			var imgUrl = "http://" + location.host+ location.pathname;
			//Récupérer les variables depuis le lien
			var query= popURL.split('?');
			var dim= query[1].split('&amp;');
			var popWidth = dim[0].split('=')[1]; //La première valeur du lien

			//Faire apparaitre la pop-up et ajouter le bouton de fermeture
			$('#' + popID).fadeIn().css({
				'width': Number(popWidth)
			})
			.prepend('<div class="div_btn_close" style="position:fixed;"><a href="#" class="closeProject"><img src='+imgUrl +'sites/all/themes/uwwtd/images/popclose.png class="btn_close" title="Fermer" alt="Fermer" /></a></div>');

			if(largeur_fenetre < 1350){
				$('.popup_project_block').css('margin-left', '5%');
				$('.popup_project_block .div_btn_close').css('right', '16%');
				$('.popup_project_block .div_btn_close').css('top', '2%');
			}else{
				$('.popup_project_block').css('margin-left', '-5%');
				$('.popup_project_block .div_btn_close').css('right', '14%');
				$('.popup_project_block .div_btn_close').css('top', '6%');
			}
			//Récupération du margin, qui permettra de centrer la fenêtre - on ajuste de 80px en conformité avec le CSS
			var popMargTop = ($('#' + popID).height() + 80) / 2;
			var popMargLeft = ($('#' + popID).width() + 80) / 2;

			//Effet fade-in du fond opaque
			$('body').append('<div id="fade"></div>'); //Ajout du fond opaque noir
			//Apparition du fond - .css({'filter' : 'alpha(opacity=80)'}) pour corriger les bogues de IE
			 $('#fade').css({'filter' : 'alpha(opacity=80)'}).fadeIn();

			return false;
		});
		//Fermeture de la pop-up et du fond
		$('a.closeProject, #fade').live('click', function() { //Au clic sur le bouton ou sur le calque...
			$('#fade , .popup_project_block').fadeOut(function() {
				$('#fade, a.closeProject').remove();  //...ils disparaissent ensemble
			});
			return false;
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
	$year = uwwtd_get_max_annee();
	$nat_stat_str = uwwtd_get_national_stat_str($year);
	
	
	
	?>
	<div class ="txtHomePage"><?php print $nat_stat_str; ?></div>
	<div class="btnHomePage">
		<a href="#?w=600" class="btnContent" rel="popup_content">About the content of this website</a>
		<a href="#?w=800" class="btnProject" rel="popup_project">About the SIIF project at the origin of this IT development</a>
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
			<p>The Structured Implementation and Information Framework (SIIF) concept is a ongoing European Commission (Environment Directorate General) 
			pilot project essentially focused on the organisation and management of data in order to enhance the generation of information for policy makers, 
			interested parties and the public at all levels on how legislation is practically implemented. 
			The concept developed is in line with the provisions of the Public Access to Environmental Information  and INSPIRE  Directives. </p>
			<p>The SIIF concept is composed of a governance approach and an IT system.</p>
			<p>The SIIF implies a governance focused on compliance, good environmental status, environmental pressure and impact, economic activities and jobs creation. 
			It has also an objective to reduce administrative burden, provide more up-to-date data, increase efficiency to all potential users. 
			The existence of an implementation programme on how to reach or maintain the compliance for a policy is also part of the SIIF governance approach.</p>
			<p>As regards the IT components of the SIIF it has three components: (1) the content for data community (information needs and how it is organised);
			(2) the process for data providers (who does what and how); (3) the dissemination for data users (how to display) 
			All these components have a national and a European dimension. </p>
			
			<figure tabindex="1" contenteditable="true">
				<img id="imgProjectA" src="sites/all/themes/uwwtd/images/pageaccueil.png" contenteditable="false"/>
			</figure>

			<p>This IT development is related to the process and the dissemination of the existing content of the European urban waste water data model.</p>
			<p>The SIIF project opted for the implementation of an open source dissemination IT toolbox. 
			The tool was designed to disseminate environmental data and information at national level but also to facilitate the reporting process. 
			It provides EU MS a cheap way to implement the Article 11 of the INSPIRE directive and thus to develop their national waste water website. 
			The framework of this website can also be adapted to the use of other European Policies.</p>
			
			<p class="legend"><span>1 </span>Directive 2003/4/CE</p>
			<p class="legend"><span>2 </span>Directive 2007/2/CE</p>
			<p class="legend"><span>3 </span><a href="http://rod.eionet.europa.eu/obligations/613" target="_blank">http://rod.eionet.europa.eu/obligations/613</a></p>
		</div>
	</div>
	
  </div>
</div>
