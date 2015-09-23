<?php

function variables_drush_command() {
  return array(
    'reset-variables' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Reset variables with those from the project configuration file',
      'options' => array(
        'verbose' => 'Show the value of each variable set'
      )
    ),
  );
}

function drush_variables_reset_variables() {
  global $project_config;
  $variables = $project_config['aliases']['self']['variables'];
  foreach ($variables as $k => $v) {
    if (drush_get_option('verbose', FALSE)) {
      drush_log(sprintf('Setting %s = %s', $k, $v));
    }
    variable_set($k, $v);
  }
}
