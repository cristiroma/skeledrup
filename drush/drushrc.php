<?php

use Drush\Make\Parser\ParserYaml;

// Load project configuration file and set it as global variable $project_config
global $project_config;
define('PROJECT_ROOT', realpath(dirname(__FILE__) . '/..'));

$project_config = sk_setup_aliases();

// Configuring commands based on config.yml
$admin = $project_config['aliases']['self']['admin'];
$db = $project_config['aliases']['self']['db'];
$db_url = sprintf('mysql://%s:%s@%s:%s/%s', $db['username'], $db['password'], $db['host'], $db['port'], $db['database']);
$options['command-specific']['site-install']['db-url'] = $db_url;
$options['command-specific']['site-install']['account-mail'] = $admin['mail'];
$options['command-specific']['site-install']['account-name'] = $admin['name'];
$options['command-specific']['site-install']['account-pass'] = $admin['pass'];

$makefile = !empty($project_config['aliases']['self']['makefile']) ? $project_config['aliases']['self']['makefile'] : '';
if (!empty($makefile)) {
  $makefile = $makefile[0] == '/' ? $makefile : PROJECT_ROOT . '/' . $makefile;
}


// Always show release notes when running pm-update or pm-updatecode
# $command_specific['pm-update'] = array('notes' => TRUE);
# $command_specific['pm-updatecode'] = array('notes' => TRUE);

/**
 * @return mixed
 */
function sk_get_config($profile = 'self') {
  global $project_config;
  $ret = array();
  if (!empty($project_config['aliases'][$profile])
      && is_array($project_config['aliases'][$profile])) {
    $ret = $project_config['aliases'][$profile];
  }
  return $ret;
}


function sk_setup_aliases() {
  $ret = array();
  $config_yml = realpath(PROJECT_ROOT . '/etc/config.yml');
  if (file_exists($config_yml)) {
    $config = ParserYaml::parse(file_get_contents($config_yml));
    $ret = $config;
  }
  $local_yml = PROJECT_ROOT . '/etc/local.yml';
  if (file_exists($local_yml)) {
    $local = ParserYaml::parse(file_get_contents($local_yml));
    $ret = array_replace_recursive($ret, $local);
    // Check for overrides in specific positions of the array
    // @todo beautify to something ... recursive
    foreach(array('users', 'variables', 'solr-servers') as $key) {
      if (!empty($local['aliases']['self'][$key]['override'])) {
        $ret['aliases']['self'][$key] = $local['aliases']['self'][$key];
        unset($ret['aliases']['self'][$key]['override']);
      }
    }
  }
  return $ret;
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

function sk_parse_drush_get_option_array($option) {
  $ret = array();
  if ($value = drush_get_option($option)) {
    $ret = explode(',', $value);
    $ret = array_map('trim', $ret);
  }
  return $ret;
}
