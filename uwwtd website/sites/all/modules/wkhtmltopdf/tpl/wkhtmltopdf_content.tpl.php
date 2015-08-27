<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<?php 
    global $base_url; 
?>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
	<?php print $variables['css'];?>
    <?php print $variables['js'];?>
    <style>
        .view-agglomeration-conformity{margin-left:auto;margin-right:auto;padding-left:15px;padding-right:15px;width:1170px;}
		.view-agglomeration-conformity h1{margin-top:15px;font-family:lato;font-size:18px;font-weight:bold;padding:2px;color:#2A6496;}
		.views-exposed-form, div.feed-icon{display:none!important;}
        .openlayers-gazetteer-search-textfield, .olButton, .olControlPanZoomBar{display:none!important;}
	
    </style>
</head>
<body id="wkhtmltopdf-print">
    <div id="content" class="m_01">
        <div id="wkhtmltopdf-print-content">
			<?php print  (isset($variables['title'])? '<h1>'.$variables['title'].'</h1>':'');?>
            <?php print $variables['content'];
			print $variables['footer'];?>
        </div>
		
    </div>
    <?php 
	 //print $variables['js_footer']; 
    ?>
</body>
</html>