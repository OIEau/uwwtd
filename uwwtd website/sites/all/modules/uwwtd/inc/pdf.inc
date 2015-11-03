<?php 


function uwwtd_make_pdf_report(){
    $conf =wkhtmltopdf_get_conf();
    if(empty($conf)){
        exit("wkhtmltopdf n'est pas configuré");
    }
    if($conf['header_display']){
        $header=wkhtmltopdf_getHeader($data);
        cache_set($cid.'_header', $header);
    }
    if($conf['footer_display']){
        $footer=wkhtmltopdf_getFooter($data);
        cache_set($cid.'_footer', $footer);
    }
    if($conf['cover_display']){
        $cover=wkhtmltopdf_getCover($data);
        //exit($cover);
        cache_set($cid.'_cover', $cover);
    } 
    
    
    //WkHtmlToPdf.php',
    //      'file path' => drupal_get_path('module', 'wkhtmltopdf').'/lib'
     module_load_include('pdf', 'wkhtmltopdf', 'lib/WkHtmlToPdf');
     
	drupal_add_css ( drupal_get_path ( 'module' , 'openlayers_plus' ) . '/theme/openlayers_plus.css' , array( 'group' => CSS_DEFAULT , 'every_page' => TRUE )); 
    drupal_add_css ( drupal_get_path ( 'module' , 'contextual' ) . '/contextual.css' , array( 'group' => CSS_DEFAULT , 'every_page' => TRUE )); 
	drupal_add_css ( drupal_get_path ( 'module' , 'openlayers_gazetteer' ) . '/css/openlayers_gazetteer.css' , array( 'group' => CSS_DEFAULT , 'every_page' => TRUE )); 

   
	drupal_add_js ( drupal_get_path ( 'module' , 'views' ) . '/js/views-contextual.js'); 
    
	$output = '';
    
    $view = views_get_view('agglomeration_conformity');
    $view->set_display('page');
    $view->pre_execute();
    //krumo($view);
    foreach($view->display as $k=>$display){
        if($display->display_options['style_plugin']=="table"){
            //krumo($display);
            $view->display[$k]->display_options['style_options']['sticky']=0;
            $view->display[$k]->display_options['style_options']['override']=0;
            $view->display[$k]->display_options['style_options']['default']=null;
            
        }
    }
    
    $view->execute();
	
    $variables['css'] = pdf_make_link_absolute(drupal_get_css());
    $variables['js'] = pdf_make_link_absolute(drupal_get_js());
    $variables['js_footer'] = pdf_make_link_absolute(drupal_get_js('footer'));
    
    $output.='<head>';
    $output.= $variables['css'];
	$output.= $variables['js'];
    
    $output.='
	<style>
        body{width:1100px;margin:auto;}
        h1{color:#2a6496;font-size:18px;}
        table.sticky-header{display:none!important;}
        table td {word-wrap: break-word;}
        tr {page-break-inside: avoid;}
        thead {display: table-header-group;}
        tfoot {display: table-row-group;}
        canvas.overlay{display:none;}
        .views-exposed-form, div.feed-icon{display:none!important;}
        .openlayers-gazetteer-search-textfield, .olButton, .olControlPanZoomBar{display:none!important;}
	</style>';
    $output.='</head>';
    $output.='<body>';
    //Generation de la carte, du graph et du tableau avec en plus la modification de la taille de la carte
    $output.= str_replace(
        array('style="width:auto;height:650px;"', 'class="active"'), //,'style="width:500px;height:200px;"'
        array('style="width:auto;height:500px;"', ''), //'style="width:350px;height:250px;"'
        $view->render()
    );
    $output.= $variables['js_footer'];
    $output.='</body>';
    $content = '<html>'.$output.'</html>';
    
    $dir = file_default_scheme().'/wkhtmltopdf'; 
    file_prepare_directory($dir, $mode = FILE_CREATE_DIRECTORY);
    
    $options= array(
        'path'=>$dir.'/',
        'binpath'=>$conf['bin_path'], //'binpath'=>'/home/wkhtmltopdf-i386',
        'html'=>$content,
        //'orientation'=>'Landscape'
    );
    if($conf['disablePdfCompression']){$options['pdfCompression']=false;}
    if($conf['footer_display']){$options['footer_html']=url('wkhtmltopdf/getCache/'.$cid.'_footer', array('absolute'=>true));}
    if($conf['header_display']){$options['header_html']=url('wkhtmltopdf/getCache/'.$cid.'_header', array('absolute'=>true));}
    if($conf['cover_display']){$options['cover_html']=url('wkhtmltopdf/getCache/'.$cid.'_cover', array('absolute'=>true));}
    //if($conf['margins']){$options['margins']=$conf['margins'];}
    if($conf['toc_display']){
        $options['toc']=true;
        if($conf['toc_depth']){$options['toc_depth']=$conf['toc_depth'];}
    }
    $filename = "file_".time().".pdf";
    
    
    $wkhtmltopdf = new Wkhtmltopdf($options);
    $wkhtmltopdf->setTitle("Report"); //pdf are encode in ISO-8859-1
    $wkhtmltopdf->output(Wkhtmltopdf::MODE_SAVE, $filename);
    
    
    print '<p align="center">'.l("PDF", $dir.'/'.$filename)."</p><br></br>";
    
    print $content;
    
    exit();
    
}

function pdf_make_link_absolute($str){

    $base_path = '/';
    $str = str_replace('src="//', 'src="http://' ,$str);
	$str = str_replace('href="//', 'href="http://' ,$str); 
    $str = str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].$base_path ,$str); 
    $str = str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].$base_path ,$str);

    return $str;
}