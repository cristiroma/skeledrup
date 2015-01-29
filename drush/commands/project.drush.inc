<?php

function project_drush_command() {
  return array(
    'project' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'description' => 'Show how to use drush commands to manage the project'
    ),
    'project-setup' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'description' => 'Validate configuration, download & install Drupal core - use to start a new project',
    ),
    'project-install' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'description' => 'Install a clean project instance from scratch',
    ),
    'project-update' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Update instance: database updates, revert features, clear caches',
    ),
    'project-devify' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Use when cloning production enviroment to local devel machine. Disable production configuration & enable development specific items',
    ),
    'project-clean' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'description' => 'Deletes all files on disk',
    ),
  );
}

function drush_project() {
  $commands = project_drush_command();
  unset($commands['project']);
  echo "Project commands:\n";
  foreach($commands as $k => $v) {
    echo " * $k: " . $v['description'] . "\n";
  }
}

function drush_project_install() {
  global $project_config;

  $profile_override = !empty($project_config['profile']['override']);
  if ($profile_override) {
    $root = realpath(dirname(__FILE__) . '/../../');
    $profile = $project_config['profile']['profile'];
    $src = $root . '/etc/profiles/' . $profile;
    $dest = $root . '/docroot/profiles/standard';
    if (is_dir($src)) {
      rrmdir($dest);
      rcopy($root . '/etc/profiles/' . $profile, $root . '/docroot/profiles/standard');
    }
  }

  $db = $project_config['db'];
  $admin = $project_config['users']['admin'];
  $variables = $project_config['variables'];
  $db_url = sprintf('mysql://%s:%s@%s:%s/%s', $db['username'], $db['password'], $db['host'], $db['port'], $db['database']);

  $command_specific = array(
    'db-url' => $db_url,
    'account-mail' => $admin['mail'],
    'account-name' => $admin['user'],
    'account-pass' => $admin['pass'],
    'db-su' => $db['root_username'],
    'db-su-pw' => $db['root_password'],
    'site-mail' => $variables['site_mail']
  );
  drush_invoke_process('@self', 'site-install', array(), $command_specific);
}

function drush_project_update() {
  drush_invoke('updatedb');
  if (module_exists('features')) {
    drush_invoke_process('@self', 'features-revert-all', array(), array(
      'force' => TRUE,
      'yes' => TRUE,
    ));
  }
  drush_invoke('cc', array('type' => 'all'));
  drush_log(dt('Update command finished'), 'success');
}

function drush_project_devify() {
  global $project_config;
  $config = $project_config['devify'];

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
  if (!empty($project_config['solr_servers']) && module_exists('search_api_solr')) {
    foreach ($project_config['solr_servers'] as $machine_name => $cfg) {
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

  // Sanitize database.
  // @see sql_drush_sql_sync_sanitize() to add your sanitize queries.
  drush_sql_sanitize();
  drush_log(dt('Devified!'), 'success');
}

function drush_project_clean() {
  $root = realpath(dirname(__FILE__) . '/../../');
  $docroot = "$root/docroot";

  $default = "$docroot/sites/default";
  $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($default));
  foreach($iterator as $item) {
    chmod($item, 0777);
  }
  rrmdir($docroot);
}
