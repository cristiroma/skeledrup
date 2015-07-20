<?php

function build_drush_command() {
  return array(
    'build' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Apply pending database updates and force-revert the features',
    ),
  );
}

function drush_build() {
  // Read the current profile and enable the modules
  $profile_info = PROJECT_ROOT . '/etc/profiles/standard/standard.info';
  if ($profile_config = parse_ini_file($profile_info)) {
    foreach ($profile_config['dependencies'] as $module) {
      if (!module_exists($module)) {
        drush_log(dt('Enabling module: !module', array('!module' => $module)), 'status');
        module_enable(array($module));
      }
    }
  }
  // Check and apply pending database updates
  drush_invoke('updatedb');
  // Force-revert the features
  if (module_exists('features')) {
    drush_invoke_process('@self', 'features-revert-all', array(), array(
      'force' => TRUE,
      'yes' => TRUE,
    ));
  }
  drush_log(dt('Clearing the caches'), 'success');
  drush_invoke('cc', array('type' => 'all'));
  drush_log(dt('Build process finished'), 'success');
}
