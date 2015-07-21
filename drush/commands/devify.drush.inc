<?php

function devify_drush_command() {
  return array(
    'devify' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Disable production configuration & enable development specific settings',
    ),
    'sql-sync-production' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Disable production configuration & enable development specific settings',
    ),
  );
}


function drush_devify() {
  global $project_config;

  $local = $project_config['aliases']['local'];
  $config = $local['devify'];

  $modules = array_unique(array_merge($config['disable-modules'], array('update')));
  drush_log('Disabling modules: ' . implode(', ', $modules), 'status');
  module_disable($modules);

  $modules = array_unique(array_merge($config['enable-modules'], array()));
  drush_log('Enabling modules: ' . implode(', ', $modules), 'status');
  module_disable($modules);

  $variables = array_unique(array_merge($config['delete-variables'], array('googleanalytics_account')));
  foreach ($variables as $variable) {
    variable_del($variable);
  }

  $variables = array_merge(
    $config['reset-variables'], array()
  );
  foreach ($variables as $key => $value) {
    drush_log("Setting variable $key: $value", 'status');
    variable_set($key, $value);
  }

  // Devify Solr servers
  if (!empty($local['solr-servers']) && module_exists('search_api_solr')) {
    foreach ($local['solr-servers'] as $machine_name => $cfg) {
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
  drush_log(dt('Instance devified!'), 'success');
}

function drush_devify_sql_sync_production() {
  drush_invoke('sql-sync', array('source' => '@prod', 'target' => '@self'));
  drush_invoke('devify');
  drush_invoke('variables-reset');
}
