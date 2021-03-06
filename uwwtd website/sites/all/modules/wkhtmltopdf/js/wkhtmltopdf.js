function wkhtmltopdf_print(selectors, origine, base_path, options){
    selector =wkhtmltopdf_get_first_valid_selector(selectors);
    if(selector){
        if(!base_path){
            base_path = '';
        }
        var content = jQuery(selector).clone();
        var content_txt = content.html();
        //En plus du contenu que l'on a choppé avec le selecteur on a beson d'aller chercher, les balise script directement incluse dans le corp de la page
        //cf : pb avec le module geosie
        jQuery('body script').each(function(){
            content_txt = content_txt + '<script type="text/javascript" defer="defer">'+jQuery(this).html()+'</script>';
        });
        var titre = '';
            if(options['title']){titre=options['title'];}
            else{titre = jQuery(document).attr('title').trim();}
        var sous_titre = '';
            if(options['subtitle']){sous_titre=options['subtitle'];}
        var comment ='';
            if(options['comment']){comment=options['comment'];}
        var fichier = encodeURIComponent(
            titre.toLowerCase()
                        .replace(/[éèêë]/g, "e")
                        .replace(/[à]/g, "a")
                        .replace(/[^a-zA-Z ]+/g, '')
                        .replace('/ {2,}/',' ')
                        .replace(/\s/g, "-")

        ); 
        var orientation = '';
        if (options['orientation']) {orientation = options['orientation']};
		var filename = fichier.substr(0,50);
        var dialogContent = '<div class="formulaire-public">'+
                            '<form id="wkhtmltopdf_Form">'+
                            '<input type="hidden" name="wkhtmltopdf_identifier" size="36" id="wkhtmltopdf_identifier" value="'+document.location+'"/>'+
                            '<ul>'+
                                '<li>'+
                                    '<label for="wkhtmltopdf_title">Title : </label>'+
                                    '<input type="text" name="wkhtmltopdf_title" size="36" id="wkhtmltopdf_title" value="'+titre+'"/><br />'+
                                '</li>'+
                                '<li>'+
                                    '<label for="wkhtmltopdf_subtitle">Sub-title : </label>'+
                                    '<input type="text" name="wkhtmltopdf_subtitle" size="36" id="wkhtmltopdf_subtitle" value="'+sous_titre+'"/><br />'+
                                '</li>'+
                                '<li>'+
                                    '<label for="wkhtmltopdf_comment">Add your comment / description document : </label>'+
                                    '<textarea name="wkhtmltopdf_comment" rows="3" id="wkhtmltopdf_comment">'+comment+'</textarea><br />'+
                                '</li>'+
                                '<li>'+
                                    '<label for="wkhtmltopdf_filename"> Name the PDF file : </label>'+
                                    '<input type="text" name="wkhtmltopdf_filename" size="36" id="wkhtmltopdf_filename" value="'+filename+'"/>.pdf<br />'+
                                '</li>'+
                                '<li>'+
	                                '<label for="wkhtmltopdf_orientation">Orientation : </label>'+
	                                '<input type="text" name="wkhtmltopdf_orientation" size="36" id="wkhtmltopdf_orientation" value="'+orientation+'"/><br />'+
	                            '</li>'+
                                '<li>'+
                                    '<div style="text-align: center;">'+
                                        '<input type="submit" style="cursor: pointer;" value="Print" />'+
                                    '</div>'+
                                '</li>'+
                        '</ul>'+
                        '</form>'+
                        '</div>';
                                                
        if(jQuery("#wkhtmltopdf_dialog")){
            jQuery("#wkhtmltopdf_dialog").dialog({
                    title: 'Impression PDF',
                    bgiframe: true,
                    modal: true,
                    autoOpen: false,
                    zIndex: 20000,
                    minHeight : 310,
                    height: 380,
                    width:430
                });
             jQuery("#wkhtmltopdf_dialog").html(dialogContent);  
             
             //desactivate dialog and execute directly the code after the submit
//              jQuery("#wkhtmltopdf_Form").bind("submit", function(){
                var title = jQuery("#wkhtmltopdf_title" ).val();
                var subtitle = jQuery("#wkhtmltopdf_subtitle" ).val();
                var filename = jQuery( "#wkhtmltopdf_filename" ).val();
                var comment = jQuery("#wkhtmltopdf_comment" ).val();
                var identifier = jQuery("#wkhtmltopdf_identifier" ).val();
                var orientation = jQuery("#wkhtmltopdf_orientation" ).val();
                 if(title!='' && filename!=''){
//                     jQuery("#wkhtmltopdf_dialog").dialog("close");
                    // prevent normal submit
                    if(jQuery('#wkhtmltopdf_wait').length>0) jQuery('#wkhtmltopdf_wait').show();
                    else jQuery(origine).html(jQuery(origine).html() + '<div id="wkhtmltopdf_wait">&nbsp;wait...</div>');
                    jQuery.ajax({
                        type: 'POST',
                        url: base_path+"?q=wkhtmltopdf/print",
                        dataType: 'json',
                        data: { 
                            wkhtmltopdf_content:content_txt,
                            wkhtmltopdf_title:title,
                            wkhtmltopdf_subtitle: subtitle,
                            wkhtmltopdf_comment:comment,
                            wkhtmltopdf_filename:filename,
                            wkhtmltopdf_identifier:identifier,
                            wkhtmltopdf_orientation:orientation
                        },
                        success: function(data) {
                                jQuery('#wkhtmltopdf_wait').hide();
                                if(data.file!=''){
                                    jQuery("#wkhtmltopdf_dialogResult").dialog({
                                        title: 'Print PDF - Download',
                                        bgiframe: true,
                                        modal: true,
                                        autoOpen: false,
                                        zIndex: 20000,
                                        minHeight : 60,
                                        width:400,
                                        buttons: { "Close": function() { jQuery(this).dialog("close"); } }
                                });
                                    
                                jQuery("#wkhtmltopdf_dialogResult").html(
                                    '<b>Your file can be downloaded from the following link : </b><br/>'+
                                    '<a href="'+data.file+'" target="_blank">Download PDF</a>'
                                                );
                                jQuery("#wkhtmltopdf_dialogResult").dialog("open");
                            }
                        },
                        error: function(xhr, ajaxOptions, thrownError){
                            jQuery('#wkhtmltopdf_wait').hide();
                            jQuery("#wkhtmltopdf_dialogResult").dialog({
                                title: 'An error has occurred !',
                                bgiframe: true,
                                modal: true,
                                autoOpen: false,
                                zIndex: 20000,
                                minHeight : 100,
                                width:530,
                                buttons: { "Close": function() { jQuery(this).dialog("close"); } }
                            });
                            
                            jQuery("#wkhtmltopdf_dialogResult").html(
                                '<b>An error occured when generating the PDF document.</b><br/>'+
                                '<span>Please contact an administrator.</span><br />' + 
                                '<b>Error :</b><br />' + 
                                '<span>'+ xhr.responseText +'</span>'
                                );
                            jQuery("#wkhtmltopdf_dialogResult").dialog("open");                        
                        }
                    });
                    
                    
                    return false;
                }
//                 else{
//                     jQuery("#wkhtmltopdf_Form").html('<p class="formulaire_erreur">Merci de remplir le titre et le nom du fichier avant de valider.</p>'+jQuery("#wkhtmltopdf_Form").html());
//                     // prevent normal submit
//                     return false;
//                 }
//              });
             
//             jQuery("#wkhtmltopdf_dialog").dialog("open");
        }
    }
    else{
        alert("We can't find a valid selectors in "+selectors);
    }
}
function wkhtmltopdf_get_first_valid_selector(selectors){
    var selector='';
    //On test, si on a un tableau ou juste une chaine de caractère
    jQuery.each(selectors, function(key, val){
        if(jQuery(val, 'body').html()){ 
            selector = val;
            //break the each iteration
            return false;
        }
    });
    if(selector!=''){
        return selector;
    }
    else{
        return false;
    }
}