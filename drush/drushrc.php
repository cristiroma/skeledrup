<?php

use Drush\Make\Parser\ParserYaml;

// Load project configuration file and set it as global variable $project_config
global $project_config;
$project_config = sk_setup_aliases();
define('PROJECT_ROOT', realpath(dirname(__FILE__) . '/..'));

function sk_setup_aliases() {
  $ret = array();
  $config_yml = realpath(PROJECT_ROOT . '/etc/project.yml');
  if (file_exists($config_yml)) {
    $config = ParserYaml::parse(file_get_contents($config_yml));
    $ret = $config;
  }
  $local_yml = PROJECT_ROOT . '/etc/local.yml';
  if (file_exists($local_yml)) {
    $config = ParserYaml::parse(file_get_contents($local_yml));
    $ret = array_replace_recursive($ret, $config);
  }
  return $ret;
}

// Utility functions
function sk_rrmdir($dir) {
  if (is_dir($dir)) {
    $objects = scandir($dir);
    foreach ($objects as $object) {
      if ($object != "." && $object != "..") {
        if (filetype($dir."/".$object) == "dir") {
          sk_rrmdir($dir."/".$object);
        }
        else {
          unlink($dir."/".$object);
        }
      }
    }
    reset($objects);
    rmdir($dir);
  }
}

function sk_rcopy($src, $dst) {
  $dir = opendir($src);
  @mkdir($dst);
  while(false !== ( $file = readdir($dir)) ) {
    if (( $file != '.' ) && ( $file != '..' )) {
      if ( is_dir($src . '/' . $file) ) {
        sk_rcopy($src . '/' . $file,$dst . '/' . $file);
      }
      else {
        copy($src . '/' . $file,$dst . '/' . $file);
      }
    }
  }
  closedir($dir);
}
