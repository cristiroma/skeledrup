<?php

function sk_drush_command() {
  return array(
    'sk' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'description' => 'Show how to use drush commands to manage the project'
    ),
    'sk-setup' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'description' => 'Validate configuration, download & install Drupal core - use to start a new project',
    ),
    'sk-build' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Rebuild the instance: apply database updates and force-revert the features',
    ),
    'sk-reset-variables' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Reset variables with those from the project configuration file',
      'options' => array(
        'verbose' => 'Show the value of each variable set'
      )
    ),
    'sk-devify' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Use when cloning production enviroment to local devel machine. Disable production configuration & enable development specific items',
    ),
    'sk-clean' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'description' => 'Deletes all files on disk',
    ),
  );
}

function drush_sk() {
  $commands = sk_drush_command();
  unset($commands['sk']);
  echo "Project commands:\n";
  foreach($commands as $k => $v) {
    echo " * $k: " . $v['description'] . "\n";
  }
}


function drush_sk_setup() {
  global $project_config;
  $local = $project_config['aliases']['self'];
  $makefile = !empty($local['makefile']) ? $local['makefile'] : '';
  if (!empty($makefile)) {
    $makefile = $makefile[0] == '/' ? $makefile : PROJECT_ROOT . '/' . $makefile;
    drush_invoke_process('@self', 'make', array($makefile, 'docroot'));
  }
  else {
    // Run drush make to make the Drupal instance
    drush_invoke_process('@self', 'dl', array('drupal', '--drupal-project-rename=docroot'));
  }
}

function drush_sk_build() {
  // Read the current profile and enable the modules
  $profile = drupal_get_profile();
  $profile_info = sprintf('%s/profiles/%s/%s.info', DRUPAL_ROOT, $profile, $profile);
  if ($profile_config = parse_ini_file($profile_info)) {
    foreach ($profile_config['dependencies'] as $module) {
      if (!module_exists($module)) {
        drupal_set_message(dt('Enabling previously disabled module: !module', array('!module' => $module)));
        module_enable(array($module));
      }
    }
  }
  // Check and apply pending database updates
  drush_invoke('updatedb');
  // Force-reverts the features
  if (module_exists('features')) {
    drush_invoke_process('@self', 'features-revert-all', array(), array(
      'force' => TRUE,
      'yes' => TRUE,
    ));
  }
  drush_invoke('cc', array('type' => 'all'));
  drush_log(dt('Rebuild process finished'), 'success');
}

function drush_sk_devify() {
  global $project_config;

  $self = $project_config['aliases']['self'];
  $config = $self['devify'];

  $modules = array_unique(array_merge($config['disable-modules'], array('update')));
  module_disable($modules);

  $modules = array_unique(array_merge($config['enable-modules'], array()));
  module_disable($modules);

  $variables = array_unique(array_merge($config['delete-variables'], array('googleanalytics_account')));
  foreach ($variables as $variable) {
    variable_del($variable);
  }

  $variables = array_merge(
    $config['reset-variables'], array(
      'preprocess_css' => 0,
      'preprocess_js' => 0,
      'cache' => 0,
      'page_compression' => 0,
    )
  );
  foreach ($variables as $key => $value) {
    variable_set($key, $value);
  }

  // Devify Solr servers
  if (!empty($self['solr-servers']) && module_exists('search_api_solr')) {
    foreach ($self['solr-servers'] as $machine_name => $cfg) {
      if ($server = search_api_server_load($machine_name)) {
        $server->name = $cfg['name'];
        $server->enabled = $cfg['enabled'];
        $server->description = $cfg['description'];
        $server->options = array(
          'scheme' => $cfg['scheme'],
          'host' => $cfg['host'],
          'port' => $cfg['port'],
          'path' => $cfg['path'],
          'http' => array(
            'http_user' => $cfg['http_user'],
            'http_pass' => $cfg['http_pass'],
          ),
          'excerpt' => $cfg['excerpt'],
          'retrieve_data' => $cfg['retrieve_data'],
          'highlight_data' => $cfg['highlight_data'],
          'skip_schema_check' => $cfg['skip_schema_check'],
          'solr_version' => $cfg['solr_version'],
          'http_method' => $cfg['http_method'],
        );
        $server->save();
      }
    }
  }
  drush_log(dt('Devified!'), 'success');
}

function drush_sk_clean() {
  $default = PROJECT_ROOT . '/docroot/sites/default';
  if (is_dir($default)) {
    $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($default));
    foreach ($iterator as $item) {
      chmod($item, 0777);
    }
  }
  $docroot = PROJECT_ROOT . '/docroot';
  if (is_dir($docroot)) {
    sk_rrmdir($docroot);
  }
}

function drush_sk_reset_variables() {
  global $project_config;
  $variables = $project_config['aliases']['self']['variables'];
  foreach ($variables as $k => $v) {
    if (drush_get_option('verbose', FALSE)) {
      drush_log(sprintf('Setting %s = %s', $k, $v));
    }
    variable_set($k, $v);
  }
}
