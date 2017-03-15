<?php
	global $base_root; 
	global $theme_key;
?>
<div class="region col-sm-2">
    <div class="uwwtd_footer_div uwwtd_logo">
        <img class="uwwtd_logo_commission" src="<?php echo url(drupal_get_path('theme', $theme_key).'/images/LOGO-CE_Vertical_EN_quadri_LR-150.png'); ?>" alt="Commission European logo" />
    </div>
</div>

<div class="region col-sm-8">
    <div class="uwwtd_footer_div uwwtd_credits">
        <p><em>Project funded by the European Commission (<a href="//ec.europa.eu/dgs/environment/" title="European Commission - Directorate General Environment" target="_blank">Directorate General Environment</a>)</em></p>
        <p><em>Urban Wastewater Treatment Directive Structured Implementation and Information Framework (UWWTD SIIF)</em></p>
        <div class="geospatial-award">
            <div class="geospatial-award-img">
                <a href="//www.geospatialworld.net/news-posts/geospatial-media-communications-announces-winners-geospatial-excellence-award" title="Geospatial Media and Communications announces winners for Geospatial Excellence Award"/>
                <img class="uwwtd_logo_commission" src="<?php echo url(drupal_get_path('theme', $theme_key).'/images/geospatial_world.png'); ?>" alt="Geospatial World" />
                </a>
            </div>
            <div class="geospatial-award-txt">
                UWWTD-SIIF: <a href="//www.oieau.org/oieau/notre-actualite-et-avancement-de/article/la-commission-europeenne-remporte" title="UWWTD-SIIF : winner of the geospatial policy implementation excellence award 2017">winner of the geospatial policy implementation excellence award 2017</a>

            </div>
            
        </div>
        <p>This website is managed by <a href="//www.oieau.org" title="Office International de l'Eau ">Office International de l'Eau</a> © Copyright DGENV 2013-2017</p>
        <?php echo l(t('Credits'), 'content/credits', array('attributes' => array('class' => array('uwwtd_credits_link')))); ?> | 
        <?php echo l(t('Disclaimer'), 'content/disclaimer', array('attributes' => array('class' => array('uwwtd_credits_link')))); ?>
    </div>
</div>
<div class="region col-sm-2">
    <div class="uwwtd_footer_div uwwtd_logo"></div>
</div>