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
