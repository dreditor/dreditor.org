<?php

/**
 * @file
 * Administrative settings forms for Dreditor.org.
 */

/**
 * Administrative settings form for application keys.
 */
function dreditor_org_github_settings($form, &$form_state) {
  $form['dreditor_org_github_access_token'] = array(
    '#title' => t('Personal Access Token'),
    '#type' => 'textfield',
    '#default_value' => variable_get('dreditor_org_github_access_token', ''),
    '#description' => t('The access token of an individual account with access to the dreditor/dreditor repository on GitHub. NOTE: There is a GitHub user, "DreditorServer", that is typically used for this. This way no personal accounts are tied to this server.'),
  );
  return system_settings_form($form);
}

/**
 * Menu callback for 'admin/dreditor/logs'.
 */
function dreditor_org_build_logs() {
  $build = array();
  $rows = array();
  foreach (file_scan_directory('public://logs', '/log$/') as $file) {
    $row = array();
    $row[] = array(
      'data' => array(
        'view' => array(
          '#type' => 'link',
          '#title' => $file->filename,
          '#href' => 'admin/dreditor/logs/' . $file->filename,
        ),
        'download' => array(
          '#type' => 'link',
          '#title' => t('download'),
          '#href' => file_create_url($file->uri),
          '#prefix' => ' (',
          '#suffix' => ')',
        ),
      ),
    );
    $row[] = format_size(filesize($file->uri));
    $row[] = format_date(filemtime($file->uri));
    $rows[] = $row;
  }
  $build['logs'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Filename'),
      t('Size'),
      t('Last Modified'),
    ),
    '#rows' => $rows,
  );
  return $build;
}

/**
 * Menu callback for 'admin/dreditor/logs/%'.
 */
function dreditor_org_build_log_view($file) {
  if (file_exists('public://logs/' . $file)) {
    return array(
      '#theme' => 'html_tag',
      '#tag' => 'pre',
      '#value' => file_get_contents('public://logs/' . $file),
    );
  }
  else {
    drupal_not_found();
  }

}
