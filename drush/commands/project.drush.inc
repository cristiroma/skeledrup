<?php

function project_drush_command() {
	return array(
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
	);
}

function drush_project_setup() {
	$basedir = dirname(__FILE__) . '/../..';
	if (!file_exists($basedir . '/docroot/index.php')) {
		global $project_config;
		$core = $project_config['core'];
		$version = 'drupal';
		if (!empty($core)) {
			$version .= '-' . $core;
		}
		drush_invoke_process('@self', 'dl', array($version), array('drupal-project-rename' => 'docroot'));
	}
}

function drush_project_install() {
	global $project_config;
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

	// Sanitize database.
	// @see sql_drush_sql_sync_sanitize() to add your sanitize queries.
	drush_sql_sanitize();
	drush_log(dt('Devified!'), 'success');
}
