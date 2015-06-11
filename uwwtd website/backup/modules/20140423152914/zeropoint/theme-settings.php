<?php

// Create theme settings form widgets using Forms API

// Layout Settings
  $form['tnt_container']['layout_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['tnt_container']['layout_settings']['style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#default_value' => theme_get_setting('style'),
    '#options' => array(
      'grey' => t('0 Point'),
      'sky' => t('Sky'),
      'nature' => t('Nature'),
      'ivy' => t('Ivy'),
      'ink' => t('Ink'),
      'orange' => t('Orange'),
      'sangue' => t('Sangue'),
      'lime' => t('Lime'),
      'themer' => t('- Themer -'),
    ),
  );

  $form['tnt_container']['layout_settings']['layout-width'] = array(
    '#type' => 'select',
    '#title' => t('Layout width'),
    '#default_value' => theme_get_setting('layout-width'),
    '#description' => t('<em>Fluid width</em> and <em>Fixed width</em> can be customized in _custom/custom-style.css.'),
    '#options' => array(
      0 => 'Adaptive width',
      1 => 'Fluid width (custom)',
      2 => 'Fixed width (custom)',
    ),
  );

  $form['tnt_container']['layout_settings']['sidebarslayout'] = array(
    '#type' => 'select',
    '#title' => t('Sidebars layout'),
    '#default_value' => theme_get_setting('sidebarslayout'),
    '#description' => t('<b>Variable width sidebars (wide)</b>: If only one sidebar is enabled, content width is 250px for left sidebar and 300px for right sidebar. If both sidebars are enabled, content width is 160px for left sidebar and 234px for right sidebar. <br /> <b>Fixed width sidebars (wide)</b>: Content width is 160px for left sidebar and 234px for right sidebar. <br /> <em>Equal width sidebars</em> ca be customized in _custom/custom-style.css. For other details, please refer to readme.txt.'),
    '#options' => array(
      0 => 'Variable asyimmetrical sidebars (wide)',
      1 => 'Fixed asyimmetrical sidebars (wide)',
      2 => 'Variable asyimmetrical sidebars (narrow)',
      3 => 'Fixed asyimmetrical sidebars (narrow)',
      4 => 'Equal width sidebars (custom)',
    )
  );

  $form['tnt_container']['layout_settings']['themedblocks'] = array(
    '#type' => 'select',
    '#title' => t('Themed blocks'),
    '#default_value' => theme_get_setting('themedblocks'),
    '#options' => array(
      0 => 'Sidebars only',
      1 => 'Sidebars + User regions',
    )
  );

  $form['tnt_container']['layout_settings']['blockicons'] = array(
    '#type' => 'select',
    '#title' => t('Block icons'),
    '#default_value' => theme_get_setting('blockicons'),
    '#options' => array(
      0 => 'No',
      1 => 'Yes (32x32 pixels)',
      2 => 'Yes (48x48 pixels)',
    )
  );

  $form['tnt_container']['layout_settings']['pageicons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Page icons'),
    '#default_value' => theme_get_setting('pageicons'),
  );

  $form['tnt_container']['layout_settings']['navpos'] = array(
    '#type' => 'select',
    '#title' => t('Menu style and position'),
    '#default_value' => theme_get_setting('navpos'),
    '#description' => t('"Out of the box" 0 Point will display a static menu. To activate the drop-down menu put the <a href="?q=/admin/structure/block/manage/system/main-menu/configure">Main menu block</a> in the "Drop Down menu" region and set the correct levels to "expanded" (the parent item). <br /> NOTE: Only the static menu can be properly centered. <br /> NOTE for RTL sites: IE6 will accept only left positioned Drop-Down menu.'),
    '#options' => array(
      0 => 'Left',
      1 => 'Center',
      2 => 'Right',
    )
  );

  $form['tnt_container']['layout_settings']['roundcorners'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rounded corners'),
    '#description' => t('Some page elements (comments, search, blocks) and primary menu will have rounded corners in all browsers but IE. <br /> NOTE: With this option enabled 0 Point will not validate CSS2.'),
    '#default_value' => theme_get_setting('roundcorners'),
  );

  $form['tnt_container']['layout_settings']['headerimg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Header image rotator'),
    '#description' => t('Rotates images in the _custom/headerimg folder.'),
    '#default_value' => theme_get_setting('headerimg'),
  );

  $form['tnt_container']['layout_settings']['cssPreload'] = array(
    '#type' => 'checkbox',
    '#title' => t('jQuery CSS image preload'),
    '#description' => t('Automatically Preload images from CSS.'),
    '#default_value' => theme_get_setting('cssPreload'),
  );

  $form['tnt_container']['layout_settings']['loginlinks'] = array(
    '#type' => 'checkbox',
    '#title' => t('0 Point login/register links'),
    '#default_value' => theme_get_setting('loginlinks'),
  );

// Breadcrumb
  $form['tnt_container']['general_settings']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['general_settings']['breadcrumb']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
  );

// Search Settings
  if (module_exists('search')) {
    $form['tnt_container']['general_settings']['search_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search results'),
      '#description' => t('What additional information should be displayed on your search results page?'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_snippet'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display text snippet'),
      '#default_value' => theme_get_setting('search_snippet'),
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display content type'),
      '#default_value' => theme_get_setting('search_info_type'),
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_user'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display author name'),
      '#default_value' => theme_get_setting('search_info_user'),
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_date'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display posted date'),
      '#default_value' => theme_get_setting('search_info_date'),
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_comment'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display comment count'),
      '#default_value' => theme_get_setting('search_info_comment'),
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_upload'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display attachment count'),
      '#default_value' => theme_get_setting('search_info_upload'),
    );

// Development settings
  $form['tnt_container']['themedev'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme development settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['themedev']['siteid'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID bodyclass.'),
   	'#description' => t('In order to have different styles of 0 Point in a multisite environment you may find usefull to choose a "one word" site ID and customize the look of each site in custom-style.css file.'),
    '#default_value' => theme_get_setting('siteid'),
    '#size' => 10,
	);

// Return theme settings form
  return $form;
}  
