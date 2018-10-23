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
            color:#000000;
            font-weight:bold; 
            font-size:25px;
            line-height:100%;
            
        }
        div.wkhtmltopdf_creator{
        }
        
        div.wkhtmltopdf_title{
        }
        
        div.wkhtmltopdf_sitename{
        }         
        
        div.wkhtmltopdf_date{
            font-size:12px;
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
        
        div.disclaimer{
            color:#000000;    
        }
        div.disclaimer p {
            text-decoration:underline;
        }
    </style>
</head>
<body>
  <div id="main">  

    <div id="header">
        <table>
          <tr>
            <td style="text-align:justify;">
            <?php print $variables['nota']; ?>
            </td>
          </tr>
        </table>
    </div>
    <p</p>
    <p</p>
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
        <div class="disclaimer">
            <?php print $variables['disclaimer']; ?>
        </div>        
        

        
    </div>
  </div>
    
</body>
</html>