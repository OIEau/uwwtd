<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<?php 
    global $base_url; 
?>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
    <style>
        body{
            font: 11px/14px Verdana,Arial,Helvetica,sans-serif;
        }
        table{
            width:100%;
        }
        div.cartouche table{
            border:1px solid #aaaaaa;
            font-size:9px;
            margin:140px 0px 70px;
        }
        div#header *, div#footer *{
            color:#aaaaaa;
            font-size:9px;
        }
        div#content{
            height:180mm;
        }
        div.doc-info{
            width:100%;
            text-align:center;
            color:#3d6098;
        }
        div.doc-info div{
            margin:50px 5px;
        }
        div.titre{
            font-size:20px;
        }
        div.sous-titre, div.auteurs{
            font-size:18px;
        }
        div.date{
            font-size:12px;
        }
    </style>
</head>
<body>
  <div id="main">  
    <div id="header">
        <table>
          <tr>
            <td><img src="<?php print $base_url .'/'.drupal_get_path('module', 'wkhtmltopdf').'/images/eaufrance.png'; ?>" alt="logo eaufrance"/></td>
            <td style="text-align:justify;">Ce document provient du site <?php print variable_get('site_name', $_SERVER['SERVER_NAME']); ?> , site de la toile Eaufrance rassemblant les informations sur l'eau et les milieux aquatiques.</td>
          </tr>
        </table>
    </div>
    <div id="content">
        <div class="doc-info">
             <?php print $variables['doc-info'];?>
        </div>
        <div class="cartouche">
            <?php print $variables['cartouche']?>
        </div>
        <div class="comment">
             <?php print $variables['comment'];?>
        </div>
    </div>
    <div id="footer">
        <table>
          <tr>
            <td><img src="<?php print $base_url .'/'.drupal_get_path('module', 'wkhtmltopdf').'/images/eaufrance.png'; ?>" alt="logo eaufrance"/></td>
            <td style="text-align:justify;">
                Le portail <?php print variable_get('site_name', $_SERVER['SERVER_NAME']); ?> est le site des outils de gestion intégrée de l'eau, regroupant
                des informations sur les documents de planification qui s'inscrivent dans la Directive Cadre Européenne sur l'Eau (DCE) pour atteindre le bon état des eaux
            </td>
          </tr>
        </table>
    </div>
  </div>
</body>
</html>