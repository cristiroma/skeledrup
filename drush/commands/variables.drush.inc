<?php

function variables_drush_command() {
  return array(
    'variables-reset' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Reset variables with those from the project configuration file',
      'options' => array(
        'verbose' => 'Show the value of each variable set'
      )
    ),
  );
}

function drush_variables_reset() {
  global $project_config;
  $variables = $project_config['aliases']['local']['variables'];
  foreach ($variables as $k => $v) {
    drush_log("Setting variable $k: $v", 'status');
    variable_set($k, $v);
  }
}
