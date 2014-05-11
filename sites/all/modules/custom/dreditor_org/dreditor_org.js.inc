<?php
/**
 * @file
 * dreditor_org.js.inc
 */

/**
 * Helper function to return the build path and prepare it.
 *
 * @param string $build
 *   The build type. Should be one of:
 *   - pr
 *   - branch
 *   - tag
 * @param string $ref
 *   The value of the reference associated with the build type.
 *
 * @return string
 *   Returns the build path.
 */
function _dreditor_org_build_path($build, $ref) {
  return "public://builds/$build/$ref";
}

/**
 * Helper function for checking if a process is currently running.
 *
 * @param int $pid
 *   The process ID.
 *
 * @return bool
 *   TRUE if process is currently running, FALSE otherwise.
 */
function _dreditor_org_check_pid($pid) {
  try {
    $result = shell_exec(sprintf("ps %d", $pid));
    if (count(preg_split("/\n/", $result)) > 2) {
      return TRUE;
    }
  }
  catch(Exception $e){
  }
  return FALSE;
}

/**
 * Helper functionf for building an extension.
 *
 * @param string $build
 *   The build type. Should be one of:
 *   - pr
 *   - branch
 *   - tag
 * @param string $ref
 *   The value of the reference associated with the build type.
 */
function _dreditor_org_build_extension($build, $ref) {
  $pid = FALSE;
  $build_path = _dreditor_org_build_path($build, $ref);
  file_prepare_directory($build_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  $stream_wrapper = file_stream_wrapper_get_instance_by_uri($build_path);

  // Retrieve the currently running build processes.
  $pids = array();
  if (($cache = cache_get('dreditor:build:pids')) && isset($cache->data)) {
    $pids = $cache->data;
  }
  if (isset($pids[$build_path])) {
    $pid = _dreditor_org_check_pid($pids[$build_path]);
  }

  // Only execute the build command if there's no process currently running.
  if (!$pid) {
    // Prepare the current log file.
    $log_path = "public://logs";
    file_prepare_directory($log_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
    $log_file = "dreditor-$build-$ref";
    $i = 0;
    while (TRUE) {
      if (!file_exists("$log_path/$log_file.log")) {
        // Update the latest log file.
        $logs = array();
        if (($cache = cache_get('dreditor:build:logs')) && isset($cache->data)) {
          $logs = $cache->data;
        }
        $logs[$build_path] = "$log_file.log";
        cache_set('dreditor:build:logs', $logs);

        // Convert it into a real path for CLI commands.
        touch("$log_path/$log_file.log");
        $log_stream_wrapper = file_stream_wrapper_get_instance_by_uri("$log_path/$log_file.log");
        break;
      }
      $log_file = "dreditor-$build-$ref-$i";
    }

    // Create a temporary file to for the PID dump.
    $pid_file = variable_get('file_temporary_path', '/tmp') . '/' . drupal_hash_base64($build_path) . '.pid';

    // Create the Drush command.
    $drush_cmd = sprintf('%s dreditor-build %s %s', variable_get('drush_path', '/usr/local/bin/drush'), $stream_wrapper->realpath(), $ref);

    // Execute the Drush command, log the output and save the PID.
    exec(sprintf('%s > %s 2>&1 & echo $! > %s', $drush_cmd, $log_stream_wrapper->realpath(), $pid_file));

    // Retrieve the PID.
    $pid = (int) trim(file_get_contents($pid_file));

    // Set the PID in the PIDs array.
    $pids[$build_path] = $pid;

    // Remove the temporary PID file.
    file_unmanaged_delete($pid_file);

    // Save the current cache of PIDs.
    cache_set('dreditor:build:pids', $pids);
  }

  return $pid;
}

/**
 * Implements hook_js_callback_CALLBACK().
 */
function dreditor_org_js_callback_download($build, $ref, $extension, $rebuild = FALSE, $download = TRUE) {
  $json = array(
    'buttonText' => '<i class="icon fontello dreditor-' . $extension . '" aria-hidden="true"></i>' . ucwords($extension),
  );
  $extensions = array(
    'chrome' => 'zip',
    'firefox' => 'xpi',
    'safari' => 'safariextz',
  );
  $build_path = _dreditor_org_build_path($build, $ref);
  $file = FALSE;
  if ($files = file_scan_directory($build_path, "/{$extensions[$extension]}$/")) {
    $file = reset($files);
  }
  if ($rebuild || !file_exists($build_path) || !$file || !filesize($file->uri) > 0) {
    _dreditor_org_build_extension($build, $ref);
    $json['buttonText'] = t('!icon Building...', array(
      '!icon' => '<i aria-hidden="true" class="icon glyphicon glyphicon-refresh glyphicon-spin"></i>',
    ));
    $json['building'] = TRUE;
    $json['rebuild'] = $rebuild;
  }
  elseif ($download) {
    $json['url'] = file_create_url($file->uri);
    $cid = 'dreditor:build:pids';
    $pids = array();
    if (($cache = cache_get($cid)) && isset($cache->data)) {
      $pids = $cache->data;
    }
    unset($pids[$build_path]);
    cache_set($cid, $pids);
  }
  else {
    $json['disabled'] = !$rebuild;
  }
  return $json;
}
