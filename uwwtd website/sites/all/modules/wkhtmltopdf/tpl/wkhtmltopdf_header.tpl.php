<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<?php 
    global $base_url; 
?>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <?php print $variables['html_head'];?>
    <?php print $variables['css'];?>
    <?php print $variables['js'];?>
	<style>
	    body *{
            font: 9px Verdana,Arial,Helvetica,sans-serif;
            color:#aaaaaa;
        }
        table{
            width:100%;
        }
	
    </style>
</head>
<body>
    <table>
      <tr>
        <td><!--LOGO--></td>
        <td style="text-align:justify;"><?php print trim(variable_get('site_name', 'http://'.$_SERVER['SERVER_NAME'])); ?></td>
      </tr>
    </table>
</body>
</html>