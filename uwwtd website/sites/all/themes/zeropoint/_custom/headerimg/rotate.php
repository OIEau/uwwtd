<?php

/**
 * Thanks to Marinelli's rotate.php
 */

$file_types = array( 
  'gif'  => 'image/gif',
  'jpg'  => 'image/jpeg',
  'jpeg' => 'image/jpeg',
  'png'  => 'image/png'
) ;

$regex = '/\.(' . implode('|',array_keys($file_types)) . ')$/i' ;
$files = array() ;

$directory = opendir(".");
while ( FALSE !== ($file = readdir( $directory )) ) {
  if ( preg_match( $regex, $file ) ) {
    $files[] = $file ;
  }
}

if ( !empty( $files ) ) {

  $which   = rand(0,sizeof($files)-1) ;

  if ( $file = file_get_contents( $files[$which] ) ) {

    $parts   = explode('.',$files[$which]) ;
    $ext     = strtolower($parts[sizeof($parts)-1]) ;
    
    header( "Content-type: " . $file_types[$ext] ) ;
    header( "Expires: Wed, 29 Jan 1975 04:15:00 GMT" );
    header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
    header( "Cache-Control: no-cache, must-revalidate" );
    header( "Pragma: no-cache" );

    print $file ;
  
  }

}