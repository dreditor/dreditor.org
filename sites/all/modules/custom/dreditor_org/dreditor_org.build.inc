<?php
/**
 * @file
 * dreditor_org.build.inc
 *
 * Stub that contains functions necessary for the building process.
 */

/**
 * Helper function for executing the Drush dreditor-build command.
 *
 * @param string $hash
 *   The hashed value of the build name. Should be a combination of branch, tag
 *   pull request number or a specific git commit SHA id.
 * @param int|bool $bid
 *   (optional) The build identification number, relative to the $hash. If none
 *   is specified, the last known build ID for the hash is used.
 * @param array $options
 *   The optional parameters to send to the drush command.
 *
 * @return bool|int
 *   Returns TRUE if executing, FALSE if Drush cannot be found or an integer
 *   of the error number returned by executing the Drush command.
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
 * Helper function for build buttons.
 */
function _dreditor_org_build_buttons(array $options = array()) {
  $build = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'class' => array('dreditor-build'),
    ),
  );
  $hash = _dreditor_org_build_hash($options);
  $bid = _dreditor_org_get_bid($hash);
  $paths = _dreditor_org_build_paths($hash, $bid);
  $building = _dreditor_org_process_is_running($paths['pid']);
  $extensions = 0;

  if (isset($bid)) {
    $browsers = array(
      'chrome' => 'Chrome',
      'firefox' => 'Firefox',
      'safari' => 'Safari',
    );
    $build['extensions'] = array(
      '#theme_wrappers' => array('container__dreditor_build_extensions'),
      '#attributes' => array(
        'class' => array('extensions'),
      ),
    );
    foreach ($browsers as $browser => $title) {
      if (file_exists($paths['extensions'][$browser]) && filesize($paths['extensions'][$browser]) > 0) {
        $extensions++;
        $build['extensions'][$browser] = array(
          '#theme' => 'link__dreditor_extension',
          '#text' => $title,
          '#path' => file_create_url($paths['extensions'][$browser]),
          '#options' => array(
            'html' => TRUE,
            'attributes' => array(
              'title' => t('Download @browser extension', array(
                '@browser' => $title,
              )),
            ),
          ),
          '#context' => array(
            'browser' => $browser,
          ),
        );
      }
    }
    // If there are no extensions, it is either currently building or something
    // went wrong in the last build.
    if ($extensions === 0) {
      if ($building) {
        $build['extensions']['building'] = array(
          '#theme' => 'html_tag__dreditor_building',
          '#tag' => 'span',
          '#value' => t('Building'),
        );
      }
      elseif (file_exists($paths['log']) && ($log_contents = file_get_contents($paths['log'])) && strpos($log_contents, '--- drush dreditor-build: executing command failed ---') !== FALSE) {
        $build['extensions']['failed'] = array(
          '#theme' => 'html_tag__dreditor_build_failed',
          '#tag' => 'span',
          '#value' => t('Last build failed'),
        );
      }
      elseif (!file_exists($paths['dir'])) {
        $build['extensions']['no_builds'] = array(
          '#theme' => 'html_tag__dreditor_no_builds',
          '#tag' => 'span',
          '#value' => t('Build not available'),
        );
      }
    }
  }
  else {
    $build['extensions']['no_builds'] = array(
      '#theme' => 'html_tag__dreditor_no_builds',
      '#tag' => 'span',
      '#value' => t('Build not available'),
    );
  }

  if (!$building) {
    $dropdown_links = array();

    // Build log.
    $log_exists = file_exists($paths['log']);
    $dropdown_links['log'] = array(
      'title' => t('Last build log'),
      'href' => $paths['log_url'],
    );

    // Administrative tasks.
    if (user_access('administer dreditor builds')) {
      // Rebuild.
      $rebuild_attributes = array(
        // Because this isn't a link element, we must manually inject the
        // necessary attributes for the JS callback.
        'data-js-module' => 'dreditor_org',
        'data-js-callback' => 'rebuild',
        'data-js-token' => js_get_token('dreditor_org', 'rebuild'),
      );
      foreach ($options as $key => $value) {
        $rebuild_attributes["data-$key"] = $value;
      }
      $dropdown_links['rebuild'] = array(
        'title' => t('Rebuild'),
        'href' => '#',
        'external' => TRUE,
        'attributes' => $rebuild_attributes,
      );
    }

    $build['op_dropdown'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array(
        'class' => array('operations', 'btn-group'),
      ),
      'button' => array(
        '#type' => 'button',
        '#theme_wrappers' => array('button__dreditor_build_op'),
        '#value' => t('Operations'),
      ),
      'menu' => array(
        '#theme' => 'links__dreditor_build_op',
        '#links' => $dropdown_links,
        '#context' => array(
          'log_exists' => $log_exists,
        ),
      ),
    );
  }

  return $build;
}

/**
 * Helper function for returning the current build identification number.
 *
 * @param string $hash
 *   The hashed value of the build name. Should be a combination of branch, tag
 *   pull request number or a specific git commit SHA id.
 *
 * @return int
 *   The current build ID.
 */
function _dreditor_org_get_bid($hash) {
  return _dreditor_org_set_bid($hash);
}

/**
 * Helper function for setting a build identification number.
 *
 * @param string $hash
 *   The hashed value of the build name. Should be a combination of branch, tag
 *   pull request number or a specific git commit SHA id.
 * @param int|bool $bid
 *   The build ID to set. If value is TRUE, it will increase the current build
 *   ID. If value is FALSE, it will decrease the current build value.
 *
 * @return int
 *   The build ID that was set, NULL if no builds.
 */
function _dreditor_org_set_bid($hash, $bid = NULL) {
  $builds = variable_get('dreditor_builds', array());
  if (!isset($builds[$hash])) {
    $builds[$hash] = NULL;
  }
  if ($bid === TRUE) {
    if (!isset($builds[$hash])) {
      $builds[$hash] = 0;
    }
    $builds[$hash]++;
  }
  elseif ($bid === FALSE) {
    if (!isset($builds[$hash])) {
      $builds[$hash] = 0;
    }
    if ($builds[$hash] < 2) {
      $builds[$hash] = 2;
    }
    $builds[$hash]--;
  }
  elseif (is_int($bid)) {
    $builds[$hash] = $bid;
    $bid = NULL;
  }
  if ($builds[$hash] !== $bid) {
    variable_set('dreditor_builds', $builds);
  }
  return $builds[$hash];
}

/**
 * Helper function to create the build hash based on the options provided.
 *
 * @param array $options
 *   An array of options.
 *
 * @return string
 *   The generated hash.
 */
function _dreditor_org_build_hash(array $options) {
  $hashes = variable_get('dreditor_build_hashes', array());
  // Ensure all values are strings.
  foreach ($options as $key => $value) {
    $options[$key] = (string) $value;
  }
  $serialized = serialize($options);
  $hash = drupal_hash_base64($serialized);
  if (!isset($hashes[$hash])) {
    $hashes[$hash] = $serialized;
    variable_set('dreditor_build_hashes', $hashes);
  }
  return $hash;
}

/**
 * Helper function to return the paths of various build components.
 *
 * @param string $hash
 *   The hashed value of the build name. Should be a combination of branch, tag
 *   pull request number or a specific git commit SHA id.
 * @param int|bool $bid
 *   (optional) The build identification number, relative to the $hash. If none
 *   is specified, the last known build ID for the hash is used.
 *
 * @return string
 *   Returns the path to the hashed branch/tag/pr build and it's build ID.
 */
function _dreditor_org_build_paths($hash, $bid = NULL) {
  $dir = "public://dreditor-build/$hash";
  // Attempt to retrieve the last build ID if none was specified.
  if (!isset($bid)) {
    $bid = _dreditor_org_get_bid($hash);
  }
  if (isset($bid)) {
    $dir .= "/$bid";
  }
  return array(
    'dir' => $dir,
    'log' => "$dir/build.log",
    'log_html' => "$dir/build.log.html",
    'log_url' => "development/build/log/$hash/$bid",
    'pid' => "$dir/build.pid",
    'extensions' => array(
      'chrome' => "$dir/releases/chrome/dreditor.zip",
      'firefox' => "$dir/releases/firefox/dreditor.xpi",
      'safari' => "$dir/releases/safari/dreditor.safariextz",
    ),
  );
}

/**
 * Helper function for checking if a process is currently running.
 *
 * @param int|string $pid
 *   An integer of the process ID to check or a string to a file containing
 *   the process ID.
 *
 * @return bool
 *   TRUE if the process is currently running, FALSE otherwise.
 */
function _dreditor_org_process_is_running($pid = 0) {
  $file = FALSE;
  if (is_string($pid) && file_exists($pid)) {
    $file = $pid;
    $pid = (int) trim(file_get_contents($file));
  }
  if (!empty($pid)) {
    try {
      $result = shell_exec(sprintf("ps %d", $pid));
      if (count(preg_split("/\n/", $result)) > 2) {
        return TRUE;
      }
    }
    catch(Exception $e){
    }
  }
  // Delete the PID file since the process ID it contains is no longer running.
  if ($file) {
    file_unmanaged_delete($file);
  }
  return FALSE;
}
