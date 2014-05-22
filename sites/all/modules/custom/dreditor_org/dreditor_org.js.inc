<?php
/**
 * @file
 * dreditor_org.js.inc
 */

/**
 * Implements hook_js_callback_CALLBACK().
 */
function dreditor_org_js_callback_ff_hash() {
  $json['hash'] = 'sha1:' . sha1_file(DRUPAL_ROOT . '/dreditor.xpi');
  return $json;
}

/**
 * Implements hook_js_callback_CALLBACK().
 */
function dreditor_org_js_callback_tags() {
  $json['tags'] = array_keys(dreditor_org_tags());
  return $json;
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
