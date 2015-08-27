<?php 


//Formulaire de configuration du module glossaire_auto
function wkhtmltopdf_admin_settings() {
	$configurations = wkhtmltopdf_get_conf();
    $form = array();
    
    //= Execution settings
    $form['bin_path'] = array(
		'#type' => 'textfield',
		'#title' => t('wkhtmltopdf bin path'),
		'#required'=>true,
        '#description'=>t("path to wkhtmltopdf patched qt"),
		'#default_value' => (isset($configurations['bin_path'])?$configurations['bin_path']:'/usr/bin/wkhtmltopdf'),
	);
    //Pdf compression
    $form['disablePdfCompression']=array(
		'#type' => 'checkbox',
		'#title' => t('Disable the pdf compression'),
		'#default_value' => (isset($configurations['disablePdfCompression'])?$configurations['disablePdfCompression']:false),
		'#description' => 'Do not use lossless compression on pdf objects. When you disable the pdf compression you produce smaller documents but colors and pictures can be degraded',
	);
    
    //= Table of content
    $form['toc'] = array(
		'#type' => 'fieldset',
		'#title' => t('Table of content'),
		'#collapsible' => TRUE,
	);
    $form['toc']['toc_display'] = array(
		'#type' => 'checkbox',
		'#title' => t('Display the table of content.'),
		'#default_value' => (isset($configurations['toc_display'])?$configurations['toc_display']:false),
        '#description'=>t("Insert a table of content in the beginning of the document. It uses the html tag H1, H2, H3 ..."),
	);
    $form['toc']['toc_depth'] = array(
		'#type' => 'textfield',
		'#title' => t('Table of content depht.'),
		'#default_value' => (isset($configurations['toc_depth'])?$configurations['toc_depth']:3),
        '#description'=>t("Set the depth of the toc (default 3)"),
	);
    
    //= Templates
    $form['templates'] = array(
		'#type' => 'fieldset',
		'#title' => t('Templates'),
		'#collapsible' => TRUE,
	);
    //==Header
    $form['templates']['header_display']=array(
		'#type' => 'checkbox',
		'#title' => t('Display header'),
		'#default_value' => (isset($configurations['header_display'])?$configurations['header_display']:false),
		'#description' => 'Adds a html header (need to set margin-top)',
	);
    //==Footer
    $form['templates']['footer_display']=array(
		'#type' => 'checkbox',
		'#title' => t('Display footer'),
		'#default_value' => (isset($configurations['footer_display'])?$configurations['footer_display']:false),
		'#description' => 'Adds a html footer (need to set margin-top)',
	);
    //==Cover
    $form['templates']['cover_display']=array(
		'#type' => 'checkbox',
		'#title' => t('Display cover'),
		'#default_value' => (isset($configurations['cover_display'])?$configurations['cover_display']:false),
		'#description' => "Add a cover to the document",
	);
    //= Margins
    $form['margins'] = array(
		'#type' => 'fieldset',
		'#title' => t('Margins'),
		'#collapsible' => TRUE,
	);
    $form['margins']['margin_top'] = array(
		'#type' => 'textfield',
		'#title' => t('Top'),
		'#default_value' => (isset($configurations['margins']['top'])?$configurations['margins']['top']:10),
        '#description'=>"Set the page top margin (default 10mm)"
	);
    $form['margins']['margin_bottom'] = array(
		'#type' => 'textfield',
		'#title' => t('Bottom'),
		'#default_value' => (isset($configurations['margins']['bottom'])?$configurations['margins']['bottom']:10),
        '#description'=>"Set the page bottom margin (default 10mm)"
	);
    $form['margins']['margin_left'] = array(
		'#type' => 'textfield',
		'#title' => t('Left'),
		'#default_value' => (isset($configurations['margins']['left'])?$configurations['margins']['left']:10),
        '#description'=>"Set the page left margin (default 10mm)"
	);
    $form['margins']['margin_right'] = array(
		'#type' => 'textfield',
		'#title' => t('Right'),
		'#default_value' => (isset($configurations['margins']['right'])?$configurations['margins']['right']:10),
        '#description'=>"Set the page right margin (default 10mm)"
	);
    //= Metadata
    $form['metadata'] = array(
		'#type' => 'fieldset',
		'#title' => t('Metadata'),
		'#collapsible' => TRUE,
	);
    $form['metadata']['default_metadata'] = array(
		'#type' => 'textarea',
		'#title' => t('Default metadata'),
		'#default_value' => (isset($configurations['default_metadata'])?$configurations['default_metadata']:''),
        '#description'=>"
            <p>Enter default dublin core metadata with this mask : <b>key|label|value</b></p>
            <p>You can use this keys :</p>
                'creator','subject','description','tableOfcontenus','abstract','publisher','contributor',<br/>
                'date','created','valid','available','issued','modified','dateAccepted','dateCopyrighted','dateSubmitted',<br/>
                'type','format','extent','medium', 'source','language',<br/>
                'coverage','spatial','temporal',<br/>
                'relation','isVersionOf','hasVersion','isReplacedBy','replaces','isRequiredBy','requires',<br/>
                'isPartOf','hasPart','isReferencedBy','references',<br/>
                'isFormatOf','hasFormat','conformsTo','rights','audience','mediator','educationLevel'"
	);
   
    $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration') );
	$form['#validate'][] = 'wkhtmltopdf_admin_settings_validate';
	$form['#submit'][] = 'wkhtmltopdf_admin_settings_submit';
	$form['#theme'] = 'system_settings_form';
	return $form;
}
//fonction de validation du formulaire de configuration du module glossaire_auto
function wkhtmltopdf_admin_settings_validate($form, &$form_state) {
	//DO nothing...
}

//fonction de validation du formulaire de configuration du module glossaire_auto
function wkhtmltopdf_admin_settings_submit($form, &$form_state) {	
	$configurations = wkhtmltopdf_get_conf();
	$configurations = array(
       'bin_path' => $form_state['values']['bin_path'],
	   'footer_display' => $form_state['values']['footer_display'],
       'header_display' => $form_state['values']['header_display'],
       'cover_display' => $form_state['values']['cover_display'],
       'toc_display'=>$form_state['values']['toc_display'],
       'toc_depth'=>$form_state['values']['toc_depth'],
       'disablePdfCompression'=>$form_state['values']['disablePdfCompression'],
       'margins'=>array(
        'top'=>$form_state['values']['margin_top'],
        'bottom'=>$form_state['values']['margin_bottom'],
        'right'=>$form_state['values']['margin_right'],
        'left'=>$form_state['values']['margin_left'],
        ),
        'default_metadata'=>$form_state['values']['default_metadata'],
	);
	wkhtmltopdf_set_conf($configurations);
    drupal_set_message("Your wkhtmltopdf configuration has been saved.");
}