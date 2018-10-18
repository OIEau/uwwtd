<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
<title><?php print $head_title ?></title>
<meta http-equiv="X-UA-Compatible" content="edge" />
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
</head>

<body class="<?php print $classes; ?>">

<div id="bg1"><div id="bg2">

<div id="top_bg" class="page0">
<div class="sizer0">
<div id="topex" class="expander0">
<div id="top_left">
<div id="top_right">
<div id="headimg">

<div id="above" class="clearfix">
</div>

<div id="header" class="clearfix">
  <div id="top-elements">
  </div><!-- /top-elements -->
  <div id="logo">
  <?php if ($logo): ?>
    <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
      <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
    </a>
  <?php endif; ?>
  </div> <!-- /logo -->
  <div id="name-and-slogan">
<?php if ($site_name) : ?>
    <h1 id="site-name"><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><?php print filter_xss_admin($site_name); ?></a></h1>   
<?php endif; ?>
  <?php if ($site_slogan): ?>
    <div id="site-slogan"><?php print filter_xss_admin($site_slogan); ?></div>
  <?php endif; ?>
  </div> <!-- /name-and-slogan -->

<div class="brclear"></div>

</div> <!-- /header -->

</div>
</div><!-- /top_right -->
</div><!-- /top_left -->
</div><!-- /expander0 -->
</div><!-- /sizer0 -->
</div><!-- /page0 -->

<div id="body_bg" class="page0">
<div class="sizer0">
<div class="expander0">
<div id="body_left">
<div id="body_right">

<div id="middlecontainer">
  <div id="wrapper">
    <div class="outer">
      <div class="float-wrap">
        <div class="colmain">
          <div id="main">
            <h2 class="title"><?php print $title ?></h2>
            <?php print $content; ?>
          </div>
        </div> <!-- /colmain -->
        <br class="brclear" />
      </div> <!-- /float-wrap -->
      <br class="brclear" />
    </div><!-- /outer -->
  </div><!-- /wrapper -->
</div>

<div id="bar"></div>

</div><!-- /body_right -->
</div><!-- /body_left -->
</div><!-- /expander0 -->
</div><!-- /sizer0 -->
</div><!-- /page0 -->

<div class="eopage">
<div id="bottom_bg" class="page0">
<div class="sizer0">
<div class="expander0">
<div id="bottom_left">
<div id="bottom_right">

<div id="footer" class="clearfix">
  <div class="legal">
    <div id="brand"></div>
  </div>
</div>

<div id="belowme">
</div>

</div><!-- /bottom_right -->
</div><!-- /bottom_left -->
</div><!-- /expander0 -->
</div><!-- /sizer0 -->
</div><!-- /page0 -->
</div>

</div></div><!-- /bg# -->

</body>
</html>
