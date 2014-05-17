<?php
/**
 * @file
 * dreditor_org.drush.inc
 *
 * Drush custom commands.
 */

/**
 * Implements hook_drush_command().
 */
function dreditor_org_drush_command() {
  $items['dreditor-build'] = array(
    'description' => dt('Builds the Dreditor extension for all browsers.'),
    'arguments' => array(
      'dest' => dt('The destination where the extensions are to be placed.'),
      'ref' => dt('The repository checkout branch/tag/reference to use.'),
    ),
  );
  return $items;
}

/**
 * Validates the 'dreditor-build' command.
 */
function drush_dreditor_org_dreditor_build_validate($dest = NULL, $ref = NULL) {
  if (is_null($dest)) {
    return drush_set_error('DREDITOR_ARGUMENT_MISSING', dt('You must specify a destination path where the extensions are to be placed.'));
  }
  if (is_null($ref)) {
    return drush_set_error('DREDITOR_ARGUMENT_MISSING', dt('You must specify the repository checkout branch/tag/reference to use before building.'));
  }
}

/**
 * Executes the 'dreditor-build' command.
 */
function drush_dreditor_org_dreditor_build($dest, $ref) {
  // Create a temporary directory to use for building the extension (the entire
  // directory is automatically deleted after the Drush command finishes
  // execution.
  $temp_dir = drush_tempdir();

  // Ensure the destination directory exists.
  drush_mkdir($dest, TRUE);

  $commands = array(
    // Clone the repo.
    'git clone --progress https://github.com/dreditor/dreditor.git .',
    // Checkout the ref.
    "git checkout $ref",
  );

  if (_drush_dreditor_org_exec($commands, $temp_dir)) {
    // Only continue if there is an "autobuild.json" file to indicate how this
    // particular build is to be ran.
    if (file_exists("$temp_dir/autobuild.json") && ($autobuild_commands = json_decode(file_get_contents("$temp_dir/autobuild.json"))) && _drush_dreditor_org_exec($autobuild_commands, $temp_dir)) {
      // Ensure files have been built before trying to copy them.
      $i = 0;
      while ($i < 10) {
        $files = array();

        // Scan the release directory for extensions.
        foreach (drush_scan_directory("$temp_dir/release", '/(zip|xpi|safariextz)$/') as $file) {
          // Ensure the extension isn't just a touched file (placeholder before
          // being copied from build process).
          if (filesize($file->filename) > 0) {
            $files[] = $file->filename;
          }
        }

        // There should be three extensions built.
        if (count($files) >= 3) {
          break;
        }

        // Increase the iteration count.
        $i++;

        // Wait one second before attempting again.
        sleep(1);
      }

      // Copy the release directory of the build to the final destination.
      drush_copy_dir($temp_dir . '/release', $dest, FILE_EXISTS_OVERWRITE);
    }
    elseif (empty($autobuild_commands)) {
      return drush_set_error('DREDITOR_NO_AUTOBUILD_COMMANDS', dt('Repository does not contain any autobuild commands to execute.'));
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Helper function for executing system commands.
 *
 * @param array $commands
 *   A simple array of commands to execute.
 * @param string $cwd
 *   The path to set as the current working directory before executing command.
 *
 * @return bool
 *   TRUE if all commands executed successfully, FALSE otherwise.
 */
function _drush_dreditor_org_exec(array $commands = array(), $cwd = NULL) {
  $cwd = $cwd ? "cd $cwd; " : '';

  // Iterate over commands.
  foreach ($commands as $command) {
    drush_print(dt('--- drush dreditor-build: executing command ---'));
    drush_print("$ $command");
    if (!drush_shell_exec($cwd . $command)) {
      $error = TRUE;
    }
    $output = drush_shell_exec_output();
    if (is_array($output)) {
      foreach ($output as $line) {
        drush_print($line);
      }
    }
    if (isset($error)) {
      break;
    }
  }

  if (isset($error)) {
    return drush_set_error('DREDITOR_EXEC_FAILED', dt('--- drush dreditor-build: executing command failed ---'));
  }
  return TRUE;
}
