<?php
	global $base_root; 
	global $theme_key;
?>
<div class="region col-sm-2">
				<div class="uwwtd_footer_div uwwtd_logo">
					<img class="uwwtd_logo_commission" src="<?php echo $base_root.base_path().drupal_get_path('theme', $theme_key); ?>/images/LOGO-CE_Vertical_EN_quadri_LR-150.png" alt="Commission European logo" />
				</div>
			</div>
			
			<div class="region col-sm-8">
				<div class="uwwtd_footer_div uwwtd_credits">
                                        <p><em>Service request funded by the European Commission (<a href="http://ec.europa.eu/dgs/environment/" title="http://ec.europa.eu/dgs/environment/" target="_blank">Directorate General Environment</a>)</em></p>
                                        <p><em>Expanding the Urban Wastewater Treatment Directive Structured Implementation and Information Framework (UWWTD SIIF)</em> – n°: 07.0201/2014/SFRA/698614/ENV.C.2 and n°07.0201/2014/SFRA/698614/ENV.C.2 – date 02/01/2015 to 1/01/2016</p>
                                        <p>This website is managed by Office International de l’Eau © Copyright DGENV 2013-2016</p>
					<?php echo l(t('Credits'), 'content/credits', array('attributes' => array('class' => array('uwwtd_credits_link')))); ?> | 
                                        <?php echo l(t('Disclaimer'), 'content/disclaimer', array('attributes' => array('class' => array('uwwtd_credits_link')))); ?>
				</div>
			</div>
                        <div class="region col-sm-2">
				<div class="uwwtd_footer_div uwwtd_logo">
					<img class="uwwtd_logo_commission" src="<?php echo $base_root.base_path().drupal_get_path('theme', $theme_key); ?>/images/ministry_logo-150.jpg" alt="Ministry logo" />
				</div>
			</div>