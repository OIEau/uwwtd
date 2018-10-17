<?php

exit(); 
// 
// ini_set('display_errors', 1); 
// error_reporting(E_ALL);
// 
// $fullPath = $_SERVER["SCRIPT_FILENAME"];
// //echo $fullPath;
// $explo = explode('/sites', $fullPath);
// $drupalPath = $explo[0];
// 
// // Shouldn't be required but it is!
// define('DRUPAL_ROOT', $drupalPath);
// 
// chdir($drupalPath);
// $path = getcwd();
// 
// include_once 'includes/bootstrap.inc';
// drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
// 
// if(isset($_POST['action'])){
// 	$action = $_POST['action'];
// 	if($action === 'delete_error'){
// 		echo 'im here';
// 		$id = $_POST['id'];
// 		$attached = $_POST['attached'];
// 
// 		// remove first the error from the attached node
// 		$nodeAt = node_load($attached);
// 		foreach($nodeAt->field_uwwtd_error_link['und'] as $key => $error){
// 			if($error['nid'] === $id){
// 				unset($nodeAt->field_uwwtd_error_link['und'][$key]);
// 				node_save($nodeAt);
// 			}
// 		}
// 
// 		// Now to delete the error node
// 		node_delete($id);
// 	}
// }

?>