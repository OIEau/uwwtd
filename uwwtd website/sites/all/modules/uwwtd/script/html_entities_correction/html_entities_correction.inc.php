"<?php

$request = array(
/* test eric */
// "update testeric  set title = replace(title, '&#039;', '''')  where (title like '%&\#039;%' )",
// "update testeric  set title = replace(title, '&amp;', '&')  where ( title like '%&amp;%'  )  ",
// "update testeric  set title = replace(title, '&gt;', '>') where ( title like '%&gt;%'  )   ",
// "update testeric  set title = replace(title, '&lt;', '<') where ( title like '%&lt;%' )   ",
// "update testeric  set title = replace(title, '&quot;', '\"')  where (title like '%&quot;%' )",

/*drupal node*/          
"update drupal_node  set title = replace(title, '&#039;', '''') where (title like '%&\#039;%' )",
"update drupal_node  set title = replace(title, '&amp;', '&') where ( title like '%&amp;%'  )  ",
"update drupal_node  set title = replace(title, '&gt;', '>') where ( title like '%&gt;%'  )   ",
"update drupal_node  set title = replace(title, '&lt;', '<') where ( title like '%&lt;%' )   ",
"update drupal_node  set title = replace(title, '&quot;', '\"')  where (title like '%&quot;%' )",

 
"update drupal_node_revision  set title = replace(title, '&#039;', '''') where (title like '%&\#039;%' )",
"update drupal_node_revision  set title = replace(title, '&amp;', '&') where ( title like '%&amp;%'  )  ",
"update drupal_node_revision  set title = replace(title, '&gt;', '>') where ( title like '%&gt;%'  )   ",
"update drupal_node_revision  set title = replace(title, '&lt;', '<') where ( title like '%&lt;%' )   ",
"update drupal_node_revision  set title = replace(title, '&quot;', '\"')  where (title like '%&quot;%' )",

/* drupal_field_data_field_aggremarks field_aggremarks_value */
"update drupal_field_data_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&#039;', '''') where (field_aggremarks_value like '%&\#039;%' )",
"update drupal_field_data_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&amp;', '&') where ( field_aggremarks_value like '%&amp;%'  )  ",
"update drupal_field_data_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&gt;', '>') where ( field_aggremarks_value like '%&gt;%'  )   ",
"update drupal_field_data_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&lt;', '<') where ( field_aggremarks_value like '%&lt;%' )   ",
"update drupal_field_data_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&quot;', '\"')  where (field_aggremarks_value like '%&quot;%' )",


"update drupal_field_revision_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&#039;', '''') where (field_aggremarks_value like '%&\#039;%' )",
"update drupal_field_revision_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&amp;', '&') where ( field_aggremarks_value like '%&amp;%'  )  ",
"update drupal_field_revision_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&gt;', '>') where ( field_aggremarks_value like '%&gt;%'  )   ",
"update drupal_field_revision_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&lt;', '<') where ( field_aggremarks_value like '%&lt;%' )   ",
"update drupal_field_revision_field_aggremarks  set field_aggremarks_value = replace(field_aggremarks_value, '&quot;', '\"')  where (field_aggremarks_value like '%&quot;%' )",

/* drupal_field_data_field_dcpremarks field_dcpremarks_value */
"update drupal_field_data_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&#039;', '''') where (field_dcpremarks_value like '%&\#039;%' )",
"update drupal_field_data_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&amp;', '&') where ( field_dcpremarks_value like '%&amp;%'  )  ",
"update drupal_field_data_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&gt;', '>') where ( field_dcpremarks_value like '%&gt;%'  )   ",
"update drupal_field_data_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&lt;', '<') where ( field_dcpremarks_value like '%&lt;%' )   ",
"update drupal_field_data_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&quot;', '\"')  where (field_dcpremarks_value like '%&quot;%' )",


"update drupal_field_revision_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&#039;', '''') where (field_dcpremarks_value like '%&\#039;%' )",
"update drupal_field_revision_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&amp;', '&') where ( field_dcpremarks_value like '%&amp;%'  )  ",
"update drupal_field_revision_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&gt;', '>') where ( field_dcpremarks_value like '%&gt;%'  )   ",
"update drupal_field_revision_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&lt;', '<') where ( field_dcpremarks_value like '%&lt;%' )   ",
"update drupal_field_revision_field_dcpremarks  set field_dcpremarks_value = replace(field_dcpremarks_value, '&quot;', '\"')  where (field_dcpremarks_value like '%&quot;%' )",

/* drupal_field_data_field_mslremarks field_mslremarks_value */
"update drupal_field_data_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&#039;', '''') where (field_mslremarks_value like '%&\#039;%' )",
"update drupal_field_data_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&amp;', '&') where ( field_mslremarks_value like '%&amp;%'  )  ",
"update drupal_field_data_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&gt;', '>') where ( field_mslremarks_value like '%&gt;%'  )   ",
"update drupal_field_data_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&lt;', '<') where ( field_mslremarks_value like '%&lt;%' )   ",
"update drupal_field_data_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&quot;', '\"')  where (field_mslremarks_value like '%&quot;%' )",

"update drupal_field_revision_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&#039;', '''') where (field_mslremarks_value like '%&\#039;%' )",
"update drupal_field_revision_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&amp;', '&') where ( field_mslremarks_value like '%&amp;%'  )  ",
"update drupal_field_revision_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&gt;', '>') where ( field_mslremarks_value like '%&gt;%'  )   ",
"update drupal_field_revision_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&lt;', '<') where ( field_mslremarks_value like '%&lt;%' )   ",
"update drupal_field_revision_field_mslremarks  set field_mslremarks_value = replace(field_mslremarks_value, '&quot;', '\"')  where (field_mslremarks_value like '%&quot;%' )",

/* drupal_field_data_field_rcarelatedsaremark field_rcarelatedsaremark_value */
"update drupal_field_data_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&#039;', '''') where (field_rcarelatedsaremark_value like '%&\#039;%' )",
"update drupal_field_data_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&amp;', '&') where ( field_rcarelatedsaremark_value like '%&amp;%'  )  ",
"update drupal_field_data_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&gt;', '>') where ( field_rcarelatedsaremark_value like '%&gt;%'  )   ",
"update drupal_field_data_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&lt;', '<') where ( field_rcarelatedsaremark_value like '%&lt;%' )   ",
"update drupal_field_data_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&quot;', '\"')  where (field_rcarelatedsaremark_value like '%&quot;%' )",

"update drupal_field_revision_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&#039;', '''') where (field_rcarelatedsaremark_value like '%&\#039;%' )",
"update drupal_field_revision_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&amp;', '&') where ( field_rcarelatedsaremark_value like '%&amp;%'  )  ",
"update drupal_field_revision_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&gt;', '>') where ( field_rcarelatedsaremark_value like '%&gt;%'  )   ",
"update drupal_field_revision_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&lt;', '<') where ( field_rcarelatedsaremark_value like '%&lt;%' )   ",
"update drupal_field_revision_field_rcarelatedsaremark  set field_rcarelatedsaremark_value = replace(field_rcarelatedsaremark_value, '&quot;', '\"')  where (field_rcarelatedsaremark_value like '%&quot;%' )",


/* drupal_field_data_field_rcaremarks field_rcaremarks_value */
"update drupal_field_data_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&#039;', '''') where (field_rcaremarks_value like '%&\#039;%' )",
"update drupal_field_data_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&amp;', '&') where ( field_rcaremarks_value like '%&amp;%'  )  ",
"update drupal_field_data_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&gt;', '>') where ( field_rcaremarks_value like '%&gt;%'  )   ",
"update drupal_field_data_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&lt;', '<') where ( field_rcaremarks_value like '%&lt;%' )   ",
"update drupal_field_data_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&quot;', '\"')  where (field_rcaremarks_value like '%&quot;%' )",

"update drupal_field_revision_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&#039;', '''') where (field_rcaremarks_value like '%&\#039;%' )",
"update drupal_field_revision_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&amp;', '&') where ( field_rcaremarks_value like '%&amp;%'  )  ",
"update drupal_field_revision_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&gt;', '>') where ( field_rcaremarks_value like '%&gt;%'  )   ",
"update drupal_field_revision_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&lt;', '<') where ( field_rcaremarks_value like '%&lt;%' )   ",
"update drupal_field_revision_field_rcaremarks  set field_rcaremarks_value = replace(field_rcaremarks_value, '&quot;', '\"')  where (field_rcaremarks_value like '%&quot;%' )",



/* drupal_field_data_field_rcasalsaremark field_rcasalsaremark_value */
"update drupal_field_data_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&#039;', '''') where (field_rcasalsaremark_value like '%&\#039;%' )",
"update drupal_field_data_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&amp;', '&') where ( field_rcasalsaremark_value like '%&amp;%'  )  ",
"update drupal_field_data_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&gt;', '>') where ( field_rcasalsaremark_value like '%&gt;%'  )   ",
"update drupal_field_data_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&lt;', '<') where ( field_rcasalsaremark_value like '%&lt;%' )   ",
"update drupal_field_data_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&quot;', '\"')  where (field_rcasalsaremark_value like '%&quot;%' )",

"update drupal_field_revision_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&#039;', '''') where (field_rcasalsaremark_value like '%&\#039;%' )",
"update drupal_field_revision_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&amp;', '&') where ( field_rcasalsaremark_value like '%&amp;%'  )  ",
"update drupal_field_revision_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&gt;', '>') where ( field_rcasalsaremark_value like '%&gt;%'  )   ",
"update drupal_field_revision_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&lt;', '<') where ( field_rcasalsaremark_value like '%&lt;%' )   ",
"update drupal_field_revision_field_rcasalsaremark  set field_rcasalsaremark_value = replace(field_rcasalsaremark_value, '&quot;', '\"')  where (field_rcasalsaremark_value like '%&quot;%' )",


/* drupal_field_data_field_uwwremarks field_uwwremarks_value */
"update drupal_field_data_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&#039;', '''') where (field_uwwremarks_value like '%&\#039;%' )",
"update drupal_field_data_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&amp;', '&') where ( field_uwwremarks_value like '%&amp;%'  )  ",
"update drupal_field_data_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&gt;', '>') where ( field_uwwremarks_value like '%&gt;%'  )   ",
"update drupal_field_data_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&lt;', '<') where ( field_uwwremarks_value like '%&lt;%' )   ",
"update drupal_field_data_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&quot;', '\"')  where (field_uwwremarks_value like '%&quot;%' )",

"update drupal_field_revision_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&#039;', '''') where (field_uwwremarks_value like '%&\#039;%' )",
"update drupal_field_revision_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&amp;', '&') where ( field_uwwremarks_value like '%&amp;%'  )  ",
"update drupal_field_revision_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&gt;', '>') where ( field_uwwremarks_value like '%&gt;%'  )   ",
"update drupal_field_revision_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&lt;', '<') where ( field_uwwremarks_value like '%&lt;%' )   ",
"update drupal_field_revision_field_uwwremarks  set field_uwwremarks_value = replace(field_uwwremarks_value, '&quot;', '\"')  where (field_uwwremarks_value like '%&quot;%' )",


);
