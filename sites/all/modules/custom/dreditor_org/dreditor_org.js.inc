<?php
/**
 * @file
 * dreditor_org.js.inc
 */

/**
 * Helper function for executing the Drush dreditor-build command.
 *
 * @param array $options
 *   The optional parameters to send to the drush command.
 */
function _dreditor_org_drush_dreditor_build($hash, $bid, array $options = array()) {
  $return = FALSE;
  // Only run if the Drush command is located and executable.
  if (($drush = variable_get('drush_path')) && is_executable($drush)) {
    // Retrieve the build paths.
    $paths = _dreditor_org_build_paths($hash, $bid);

    // Only execute the build command if there's no process currently running.
    if (!_dreditor_org_process_is_running($paths['pid'])) {
      // Prepare the build directory.
      file_prepare_directory($paths['dir'], FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

      // Format options for Drush.
      foreach ($options as $name => $value) {
        $options[$name] = "--$name=$value";
      }

      // Build the command to execute.
      $arguments = array(
        '%s %s %s %s >> %s 2>&1 & echo $! > %s',
        $drush,
        'dreditor-build',
        implode(' ', $options),
        _dreditor_org_uri_realpath($paths['dir']),
        _dreditor_org_uri_realpath($paths['log']),
        _dreditor_org_uri_realpath($paths['pid']),
      );
      $command = call_user_func_array('sprintf', $arguments);

      // Execute the Drush dreditor-build command.
      watchdog('dreditor_org', sprintf('Executing drush command: %s', $command));
      exec($command, $output, $result);
      $return = ($result == 0 ? TRUE : $result);
      if ($return !== TRUE) {
        watchdog('dreditor_org', 'Drush command "dreditor-build" returned the following result: %result', array(
          '%result' => $result,
        ), WATCHDOG_ERROR, $paths['log_url']);
      }
    }
  }
  else {
    drupal_set_message(t('Could not locate the path to Drush.'), 'error');
  }
  return $return;
}

/**
 * Helper function for parsing build options from request data.
 *
 * @param array $data
 *   The request data.
 *
 * @return array
 *   The array of parsed options.
 */
function _dreditor_org_js_build_options($data) {
  $options = array();
  foreach (array('branch', 'checkout', 'pr') as $option) {
    if (!empty($data[$option])) {
      $options[$option] = (string) $data[$option];
    }
  }
  return $options;
}

/**
 * Implements hook_js_callback_CALLBACK().
 */
function dreditor_org_js_callback_rebuild() {
  $args = func_get_args();
  $data = array_pop($args);
  $options = _dreditor_org_js_build_options($data);
  $hash = _dreditor_org_build_hash($options);
  $bid = _dreditor_org_get_bid($hash);
  $paths = _dreditor_org_build_paths($hash, $bid);
  $building = _dreditor_org_process_is_running($paths['pid']);
  $result = TRUE;

  // Only execute Drush dreditor-build command if there is no build process
  // currently running.
  if (!$bid || !$building) {
    // Increase the build ID.
    $bid = _dreditor_org_set_bid($hash, TRUE);
    // Execute the Drush dreditor-build command.
    $building = (bool) _dreditor_org_drush_dreditor_build($hash, $bid, $options);
  }
  if ($building === FALSE) {
    drupal_set_message(t('There was an error during the build process. Please check the build log.'), 'error');
  }
  $buttons = _dreditor_org_build_buttons($options);
  return array(
    'building' => $building,
    'content' => drupal_render($buttons),
  );
}

/**
 * Implements hook_js_callback_CALLBACK().
 */
function dreditor_org_js_callback_build_check() {
  $args = func_get_args();
  $data = array_pop($args);
  $options = _dreditor_org_js_build_options($data);
  $hash = _dreditor_org_build_hash($options);
  $bid = _dreditor_org_get_bid($hash);
  $paths = _dreditor_org_build_paths($hash, $bid);
  $buttons = _dreditor_org_build_buttons($options);
  return array(
    'building' => _dreditor_org_process_is_running($paths['pid']),
    'content' => drupal_render($buttons),
  );
}
