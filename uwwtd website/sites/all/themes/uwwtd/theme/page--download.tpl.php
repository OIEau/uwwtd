

<?php if ($page['navigation']): ?>
  <?php print render($page['navigation']); ?>
<?php endif; ?>

<div class="main-container container">
  <?php /* region--header.tpl.php */ ?>
  <?php print render($page['header']); ?>
<?php /* if ($tabs): ?><?php print render($tabs); ?><?php endif; */ ?>
  <div class="row">

    <?php /* region--sidebar.tpl.php  ?>
    <?php if ($page['sidebar_first']): ?>
      <?php print render($page['sidebar_first']); ?>
    <?php endif; */?>

    <?php /* region--content.tpl.php */ ?>
    <div class="region region-content col-sm-12">
		<div id="printer">
		<?php
			$options = array();
			//Le titre est le nom du site
			$options['title'] = drupal_get_title();
			//Le sous titre le nom de la page
			//$options['subtitle'] ='Le glossaire librement réutilisable que chacun peut améliorer';
			//Description de la page (en fait le titre)
			//if($title!='') $options['comment'] = str_replace("'", "&#039;", $title);
// 			print wkhtmltopdf_tag(array('.node', '.region-content', '.content', '#content', '#recherche'), $options);
            print uwwtd_wkhtmltopdf_tag(array('.main-container'), $options);			
		?>     
		</div>    
        <?php
		$render_field = '';
		$option['allyear'] = uwwtd_get_all_year();
		$list_field = array(
			'field_anneedata_valueData' => array('title' => 'Select the year', 'type' => 'selectData', 'data' => $option['allyear']),
		);

		if (isset($_GET['field_anneedata_valueData']) && array_key_exists($_GET['field_anneedata_valueData'], $option['allyear'])) {
			$filter['filter']['field_anneedata_valueData'] = $_GET['field_anneedata_valueData'];
			$annee = "UWWTD:repReportedPerdiod=".$filter['filter']['field_anneedata_valueData'];
			$anneeRef = $filter['filter']['field_anneedata_valueData'];
		} else {
			$filter['filter']['field_anneedata_valueData'] = uwwtd_get_max_annee();
			$annee = "UWWTD:repReportedPerdiod=".$filter['filter']['field_anneedata_valueData'];
			$anneeRef = $filter['filter']['field_anneedata_valueData'];
		}
		foreach($list_field as $field_name => $data) {
			$render_field .= '<div class="views-exposed-widget views-widget-filter-'.$field_name.'" id="edit-field-'.$field_name.'-wrapper">
			<label for="edit-'.$field_name.'">'.$data['title'].'</label>';
			 switch($data['type']) {	
				case 'selectData':
                $render_field .= '<div class="views-widget">
                    <div class="form-type-select form-item-'.$field_name.' form-item form-group">
                    <select name="'.$field_name.'" id="edit-'.$field_name.'" class="form-control form-select">
                    ';
                foreach($data['data'] as $key => $value) {
                    $selected = "";
					// dsm($filter['filter'][$field_name]);
                    if(isset($filter['filter'][$field_name]) && $filter['filter'][$field_name] == $key) {
                        $selected = 'selected="selected"';
                    }
                    $render_field .= '<option  value="'.$key.'" '.$selected.'>'.$value.'</option>';
                }
                $render_field .= '</select></div>
                </div>';
                break;
		   }
		$render_field .= ' </div>';
}
		$bundleAgg= "agglomeration";
		$bundleUwwtp= "uwwtp";
		$bundleDscp= "discharge_point";
		$aggs = db_query('SELECT n.entity_id FROM {field_data_field_anneedata} n WHERE field_anneedata_value = :annee AND bundle= :bundle LIMIT(1)', array(':annee' => $anneeRef, ':bundle' => $bundleAgg));
		$uwwtps = db_query('SELECT n.entity_id FROM {field_data_field_anneedata} n WHERE field_anneedata_value = :annee AND bundle= :bundle LIMIT(1)', array(':annee' => $anneeRef, ':bundle' => $bundleUwwtp));
		$dcps = db_query('SELECT n.entity_id FROM {field_data_field_anneedata} n WHERE field_anneedata_value = :annee AND bundle= :bundle LIMIT(1)', array(':annee' => $anneeRef, ':bundle' => $bundleDscp));
		foreach($aggs as $agg){
			$agglo = node_load($agg->entity_id);
		}
		foreach($uwwtps as $uwwtp){
			$uwwtp = node_load($uwwtp->entity_id);
		}
		foreach($dcps as $dcp){
			$dcp = node_load($dcp->entity_id);
		}
		$codeAgglo = $agglo->field_inspireidlocalid['und'][0]['value'];
		$codeUwwtp = $uwwtp->field_inspireidlocalid['und'][0]['value'];
		$codeDcp = $dcp->field_inspireidlocalid['und'][0]['value'];
		$server = $_SERVER['HTTP_HOST'];
		$pays = variable_get('siif_eru_country_name');
        //Case of compound name : UK / CZ
		$pays = str_replace(' ', '-', $pays);
        //nd@oieau.fr : Magnific !!! --> i think we can't do worst
		if($server == "webnuxdev.rnde.tm.fr" ||$server == "dev.oieau.fr"){
			$typeName = "UWWTD:UWWTD_recette_".$pays;
			$name= "UWWTD:<br>UWWTD_recette_".$pays;
		}else{
			$typeName = "UWWTD:UWWTD_".$pays;
			$name = "UWWTD:<br>UWWTD_".$pays;
		}
        
        print render($page['content']); 
?>
		
		<div class="data">
		<div class="title">
			<h1><?php print t('Data'); ?></h1>
			<p><?php print t('In this section you can access to the data used on the platform and download them.'); ?></p>
		</div>
			
			
    <div class="view-filters">
			<form accept-charset="UTF-8" id="views-exposed-form-download-page" method="get" action="">
				<div>
					<div class="views-exposed-form">
						<div class="views-exposed-widgets clearfix"><?php print $render_field; ?>
							<div class="views-exposed-widget views-submit-button">
								<button class="btn btn-info form-submit" type="submit" value="Apply" name="" id="edit-submit-agglomeration-conformity"><?php print t("Apply");?></button>
							</div>
							<div class="views-exposed-widget views-reset-button">
								<button class="btn btn-default form-submit" type="submit" value="Reset" name="op" id="edit-reset"><?php print t("Reset");?></button>
							</div>
						</div>
					</div>
				</div>
			</form>
		</div>
<!-- <iframe id="my_iframe" style="display:none;"></iframe> --> 
 <table class="download">	
	<tr>
			<td style="text-align:center;"><b><?php print t("Title");?></b></td>
			<td style="text-align:center;"><b><?php print t("Coverage");?></b></td>
			<td style="text-align:center;"><b><?php print t("Files available");?></b></td>
		</tr>
		<tr>
			<td><?php print t("Full UWWTD reported data");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<?php 
				if (!empty($agglo->field_sourcefile['und'][0]['uri'])) {
					?>
					<a href="<?php print file_create_url($agglo->field_sourcefile['und'][0]['uri']); ?>" target="_blank"> xml</a>
					<?php 
				} 
				?>
			</td>
		</tr>
		<tr>
			<td><?php print t("Full Article 17 UWWTD reported data");?></td>
			<td><?php print t("Country");?></td>
			<td><a href="<?php print $base_url."article17/".$anneeRef ?>" target="_blank"> xlsx (Excel)</a></td>
		</tr>
		<tr>
			<td><?php print t("Agglomerations");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_Agglomeration&CQL_FILTER=<?php print $annee;?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_Agglomeration&CQL_FILTER=<?php print $annee;?>&outputFormat=csv" target="_blank">csv</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_Agglomeration&CQL_FILTER=<?php print $annee;?>&outputFormat=SHAPE-ZIP" target="_blank">shp</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_Agglomeration&CQL_FILTER=<?php print $annee;?>&outputFormat=KML" target="_blank">kml</a></td>
		</tr>
		<tr>
			<td><?php print t("Urban Waste Water Treatment plants");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_UrbanWasteWaterTreatmentPlant&CQL_FILTER=<?php print $annee;?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_UrbanWasteWaterTreatmentPlant&CQL_FILTER=<?php print $annee;?>&outputFormat=csv" target="_blank">csv</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_UrbanWasteWaterTreatmentPlant&CQL_FILTER=<?php print $annee;?>&outputFormat=SHAPE-ZIP" target="_blank">shp</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_UrbanWasteWaterTreatmentPlant&CQL_FILTER=<?php print $annee;?>&outputFormat=KML" target="_blank">kml</a>
			</td>
		</tr>
		<tr>
			<td><?php print t("Discharge points");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_DischargePoint&CQL_FILTER=<?php print $annee;?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_DischargePoint&CQL_FILTER=<?php print $annee;?>&outputFormat=csv" target="_blank">csv</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_DischargePoint&CQL_FILTER=<?php print $annee;?>&outputFormat=SHAPE-ZIP" target="_blank">shp</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_DischargePoint&CQL_FILTER=<?php print $annee;?>&outputFormat=KML" target="_blank">kml</a>
			</td>
		</tr>
		<tr>
			<td><?php print t("Sensitive areas");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_ReceivingArea&CQL_FILTER=<?php print $annee;?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_ReceivingArea&CQL_FILTER=<?php print $annee;?>&outputFormat=csv" target="_blank">csv</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_ReceivingArea&CQL_FILTER=<?php print $annee;?>&outputFormat=SHAPE-ZIP" target="_blank">shp</a>,
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_ReceivingArea&CQL_FILTER=<?php print $annee;?>&outputFormat=KML" target="_blank">kml</a>
			</td>
		</tr>
        <tr>
			<td><?php print t("Association table between Agglomeration and Urban Waste Water Treatment plants");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_UWWTPs_Agglo&CQL_FILTER=<?php print $annee;?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&srsName=EPSG:4326&typeName=<?php print $typeName;?>_UWWTPs_Agglo&CQL_FILTER=<?php print $annee;?>&outputFormat=csv" target="_blank">csv</a>
			</td>
		</tr>
    </table>
</div>


<div class="metadata">

		<div class="title">
			<h1><?php print t("Metadata");?></h1>
			<p><?php print t('In this section you can access the metadata fiches for each of the four geographical layers covered by UWWTD (91/271/EEC).'); ?></p>
		</div>
		
<table class="download">		
	<tr>
			<td style="text-align:center;"><b><?php print t("Title");?></b></td>
			<td style="text-align:center;"><b><?php print t("Coverage");?></b></td>
			<td style="text-align:center;"><b><?php print t("Download");?></b></td>
			<td style="text-align:center;"><b><?php print t("View");?></b></td>
		</tr>
        <?php if(variable_get('metadataAggloUid')): ?>
		<tr>
			<td><?php print t("Agglomerations");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataAggloUid');?>" download target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataAggloUid');?>" download target="_blank">pdf</a>
			</td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/catalog.search#/metadata/<?php print variable_get('metadataAggloUid');?>" target="_blank">html</a>,
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataAggloUid');?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataAggloUid');?>" target="_blank">pdf</a>
			</td>
		</tr>
        <?php endif; ?>
        <?php if(variable_get('metadataUwwtpUid')): ?>
		<tr>
			<td><?php print t("Urban Waste Water Treatment plants");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataUwwtpUid');?>" download target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataUwwtpUid');?>" download target="_blank">pdf</a>
			</td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/catalog.search#/metadata/<?php print variable_get('metadataUwwtpUid');?>" target="_blank">html</a>,
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataUwwtpUid');?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataUwwtpUid');?>" target="_blank">pdf</a>
			</td>
		</tr>
        <?php endif; ?>
        <?php if(variable_get('metadataDpUid')): ?>
		<tr>
			<td><?php print t("Discharge points");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataDpUid');?>" download target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataDpUid');?>" download target="_blank">pdf</a>
			</td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/catalog.search#/metadata/<?php print variable_get('metadataDpUid');?>" target="_blank">html</a>,
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataDpUid');?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataDpUid');?>" target="_blank">pdf</a>
			</td>
		</tr>
        <?php endif; ?>
        <?php if(variable_get('metadataRcaUid')): ?>
		<tr>
			<td><?php print t("Sensitive areas");?></td>
			<td><?php print t("Country");?></td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataRcaUid');?>" download target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataRcaUid');?>" download target="_blank">pdf</a>
			</td>
			<td>
				<a href="https://uwwtd.eu/catalogue/srv/ows/catalog.search#/metadata/<?php print variable_get('metadataRcaUid');?>" target="_blank">html</a>,
				<a href="https://uwwtd.eu/catalogue/srv/ows/xml.metadata.get?uuid=<?php print variable_get('metadataRcaUid');?>" target="_blank">xml</a>, 
				<a href="https://uwwtd.eu/catalogue/srv/ows/md.format.pdf?xsl=full_view&uuid=<?php print variable_get('metadataRcaUid');?>" target="_blank">pdf</a>
			</td>
		</tr>
        <?php endif; ?>
 </table>
</div>


<div class="services">

		<div class="title">
			<h1><?php print t("Services");?></h1>
			<p><?php print t('In this section you can access the webservices for each of the four geographical layers covered by UWWTD (91/271/EEC).');?></p>
		</div>
			
 <table class="download">		
	<tr>
			<td style="text-align:center;"><b><?php print t("Webservice");?></b></td>
			<td style="text-align:center;"><b><?php print t('DescribeFeatureType</b>: description of the information layers, name and type of fields.');?></td>
			<td style="text-align:center;"><b><?php print t('GetFeature</b>: access to data in GML format');?></td>
		</tr>
		<tr>
			<td style="text-align:center;"><b><?php print t("Agglomeration");?></b></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=DescribeFeatureType&typeName=<?php print $typeName; ?>_Agglomeration" target="_blank">Agglomeration</a></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&typeName=<?php print $typeName; ?>_Agglomeration" target="_blank">Agglomeration</a></td>
		</tr>
		<tr>
			<td style="text-align:center;"><b><?php print t("Treatment plant");?></b></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=DescribeFeatureType&typeName=<?php print $typeName; ?>_UrbanWasteWaterTreatmentPlant" target="_blank">UrbanWasteWaterTreatmentPlant</a></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&typeName=<?php print $typeName; ?>_UrbanWasteWaterTreatmentPlant" target="_blank">UrbanWasteWaterTreatmentPlant</a></td>
		</tr>
		<tr>
			<td style="text-align:center;"><b><?php print t("Discharge point");?></b></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=DescribeFeatureType&typeName=<?php print $typeName; ?>_DischargePoint" target="_blank">DischargePoint</a></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&typeName=<?php print $typeName; ?>_DischargePoint" target="_blank">DischargePoint</a></td>
		</tr>
		<tr>
			<td style="text-align:center;"><b><?php print t("Receiving area");?></b></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=DescribeFeatureType&typeName=<?php print $typeName; ?>_ReceivingArea" target="_blank">ReceivingArea</a></td>
			<td><a href="https://uwwtd.eu/services/ows/?service=WFS&version=1.1.0&request=GetFeature&typeName=<?php print $typeName; ?>_ReceivingArea" target="_blank">ReceivingArea</a></td>
		</tr>
 </table>	
</div>


<div class="references">
 
		<div class="title">
			<h1><?php print t("How to make references to this website?");?></h1>
			<p><?php print t('All the items description page (agglomeration, treatment plant, discharge point and sensitive areas) can be accessed directly, and then referenced on other websites.'); ?></p>
			<p><?php print t('This can be done by using the following URL pattern: <br>
			http://uwwtd.oieau.fr/[country]/[name of the element]/[ID of the element]<br>For instance:'); ?></p>
		</div>
		<ul>
			<li><a href="<?php print url('agglomeration/'.$codeAgglo);?>" target="_blank"><?php print url('agglomeration/'.$codeAgglo);?></a></li>
			<li><a href="<?php print url('treatment-plant/'.$codeUwwtp);?>" target="_blank"><?php print url('treatment-plant/'.$codeUwwtp);?></a></li>
			<li><a href="<?php print url('discharge-point/'.$codeDcp);?>" target="_blank"><?php print url('discharge-point/'.$codeDcp);?></a></li>
		</ul>
		<p><?php print t('If you want to reference to a specific year, you simply need to add the year in the URL as follows: '); ?></p>
		<p><?php print t('http://uwwtd.oieau.fr/[country]/[name of the element]/[ID of the element]/[year]<br>For instance:'); ?></p>
		<ul>
			<li><a href="<?php print url('agglomeration/'.$codeAgglo.'/'.$anneeRef);?>" target="_blank"><?php print url('agglomeration/'.$codeAgglo.'/'.$anneeRef);?></a></li>
			<li><a href="<?php print url('treatment-plant/'.$codeUwwtp.'/'.$anneeRef);?>" target="_blank"><?php print url('treatment-plant/'.$codeUwwtp.'/'.$anneeRef);?></a></li>
		</ul>
	
</div>

    </div>
    <?php /* region--sidebar.tpl.php */ ?>
    <?php if ($page['sidebar_second']): ?>
      <?php print render($page['sidebar_second']); ?>
    <?php endif; ?>

  </div>
</div>
<?php /* region--footer.tpl.php */ ?>
<?php print render($page['footer']); ?>