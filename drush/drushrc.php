<?php

if (!function_exists('yaml_parse')) {
	die('Please install the Yaml PHP extension');
}

// Load project configuration file and set it as global variable $project_config
global $project_config;

$project_config = array();
$config_yml = dirname(__FILE__) . '/../config.yml';
if (file_exists($config_yml)) {
  $config = yaml_parse(file_get_contents($config_yml));
  $project_config += $config;
}
