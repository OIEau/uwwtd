<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<?php 
    global $base_url; 
?>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
    <style>
        body *{
            font: 9px Verdana,Arial,Helvetica,sans-serif;
            color:#aaaaaa;
        }
        table{
            width:100%;
        }
    </style>
    <script src="<?php print $base_url .'/'.drupal_get_path('module', 'wkhtmltopdf').'/js/wkhtmltopdf_utils.js'; ?>"></script>
</head>
<body onload="subst();">
    <table>
          <tr>
            <td style="text-align:center;">
              -<span class="page"></span>-
            </td>
          </tr>
          <tr>
            <td style="text-align:right;">
              <?php print $variables['base_url']; ?>
            </td>
          </tr>          
        </table>
</body>
</html>
