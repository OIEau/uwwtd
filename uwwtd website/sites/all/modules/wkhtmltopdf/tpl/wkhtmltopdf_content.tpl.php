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
    body {background-color:#FFFFFF;}  
    .sticky-header {display:none;}
    .well {display:none;}
    .button-flipper {display:none;}
    .form-submit {display:none;}
    .openlayers-gazetteer-search-textfield{display:none;}
    #block-menu-menu-editorial-menu{display:none;}
    #printer{display:none;}
    .hideprint{display:none;} 
    /*div.data-table-row table-responsive table{width:50%;}*/   
    /*table{min-width:1500px;}
    table{max-width:1500px;}*/
    #block-system-main  > h1 {display:none;}
    
    /*chart position*/
    div.charts-row div.view-content {
        border: 0px;
    } 
    .flip-title-back{display:none;}
    .agglo_connexion_pie{margin:auto}  
    
    .front, .back {
        backface-visibility : none !important;
        position : relative !important;
        transform : none !important;
        transform-style : none !important;
        transition : none !important;
    }
    
    .front .legend > table{
        width:250px !important;        
    }
    
    .olAlphaImg, .olAlphaImg {
        display:none;
    }
    div.charts-row{
        page-break-after: always;
    }
    </style>
    <!--
    <style>
        .view-agglomeration-conformity{margin-left:auto;margin-right:auto;padding-left:15px;padding-right:15px;width:1170px;}
		.view-agglomeration-conformity h1{margin-top:15px;font-family:lato;font-size:18px;font-weight:bold;padding:2px;color:#2A6496;}
		.views-exposed-form, div.feed-icon{display:none!important;}
        .openlayers-gazetteer-search-textfield, .olButton, .olControlPanZoomBar{display:none!important;}
	
    </style>
    -->
</head>
<body id="wkhtmltopdf-print">
    <div id="content" class="m_01" style="padding-top:15px;">
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