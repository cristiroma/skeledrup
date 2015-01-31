<?php

if (!function_exists('yaml_parse')) {
	die('Please install the Yaml PHP extension');
}

// Load project configuration file and set it as global variable $project_config
global $project_config;

$project_config = array();
$config_yml = realpath(dirname(__FILE__) . '/../etc/project.yml');
if (file_exists($config_yml)) {
  $config = yaml_parse(file_get_contents($config_yml));
  $project_config += $config;
}

// Utility functions

function rrmdir($dir) {
  if (is_dir($dir)) {
    $objects = scandir($dir);
    foreach ($objects as $object) {
      if ($object != "." && $object != "..") {
        if (filetype($dir."/".$object) == "dir") rrmdir($dir."/".$object); else unlink($dir."/".$object);
      }
    }
    reset($objects);
    rmdir($dir);
  }
}

function rcopy($src, $dst) {
  $dir = opendir($src);
  @mkdir($dst);
  while(false !== ( $file = readdir($dir)) ) {
    if (( $file != '.' ) && ( $file != '..' )) {
      if ( is_dir($src . '/' . $file) ) {
        rcopy($src . '/' . $file,$dst . '/' . $file);
      }
      else {
        copy($src . '/' . $file,$dst . '/' . $file);
      }
    }
  }
  closedir($dir);
}
