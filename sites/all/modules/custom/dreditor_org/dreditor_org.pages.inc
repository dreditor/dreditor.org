<?php
/**
 * @file
 * Pages for Dreditor.org.
 */

/**
 * Menu callback for "ajax/dreditor/%".
 */
function dreditor_org_ajax($action) {
  $json = array();
  if ($action === 'tags') {
    $json['tags'] = dreditor_org_tags();
  }
  if ($action === 'ff-hash') {
    $json['hash'] = 'sha1:' . sha1_file(DRUPAL_ROOT . '/dreditor.xpi');
  }
  drupal_json_output($json);
  ajax_footer();
}

/**
 * Page callback for "development/contributing".
 *
 * Pulls directly from the repository's CONTRIBUTING.md file.
 */
function dreditor_org_contributing() {
  return array(
    '#title_hidden' => TRUE,
    '#markup' => dreditor_org_repo_content('CONTRIBUTING.md'),
  );
}

/**
 * Helper function for build buttons.
 */
function _dreditor_org_build_buttons($attributes = array()) {
  $buttons = array();
  $browsers = array(
    'chrome' => 'Chrome',
    'firefox' => 'Firefox',
    'safari' => 'Safari',
  );
  $admin = user_access('administer dreditor builds');
  foreach ($browsers as $browser => $title) {
    $button_classes = array('btn-xs');
    switch ($browser) {
      case 'chrome':
        $button_classes[] = 'btn-warning';
        break;

      case 'firefox':
        $button_classes[] = 'btn-danger';
        break;

      case 'safari':
        $button_classes[] = 'btn-primary';
        break;
    }
    $button_attributes = drupal_array_merge_deep(array(
      'class' => $button_classes + array('dreditor-build'),
      'data-extension' => $browser,
      'data-toggle' => 'tooltip',
      'data-placement' => 'bottom',
      'data-button-text' => t('Checking build...'),
      'title' => t('Download @browser extension', array(
        '@browser' => $title,
      )),
    ), $attributes);
    $button = array(
      '#type' => 'button',
      '#value' => $title,
      '#icon' => theme('icon', array('bundle' => 'dreditor', 'icon' => 'dreditor-' . $browser)),
      '#js_callback' => array('dreditor_org' => 'download'),
      '#attributes' => $button_attributes,
    );
    if ($admin) {
      $dropdown_links = array();

      // Last log link.
      if (isset($attributes['data-build']) && $attributes['data-ref']) {
        $build_path = "public://builds/{$attributes['data-build']}/{$attributes['data-ref']}";
        static $logs;
        if (!isset($logs)) {
          $logs = array();
          // Retrieve the latest logs.
          if (($cache = cache_get('dreditor:build:logs')) && isset($cache->data)) {
            $logs = $cache->data;
          }
        }
        if (!empty($logs[$build_path])) {
          $dropdown_links[] = array(
            'data' => array(
              '#type' => 'link',
              '#title' => _bootstrap_icon('list-alt') . ' ' . t('Last build log'),
              '#href' => 'admin/dreditor/logs/' . $logs[$build_path],
              '#attributes' => array(
                'target' => '_blank',
              ),
              '#options' => array(
                
                'html' => TRUE,
              ),
            ),
          );
        }
      }

      // Force rebuild.
      $dropdown_links[] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => _bootstrap_icon('refresh') . ' ' . t('Force rebuild'),
          '#href' => 'javascript:void(0);',
          // @todo add this back in once re-enabled.
//          '#js_callback' => array('dreditor_org' => 'download'),
          '#disabled' => TRUE,
          '#attributes' => array(
            'data-rebuild' => TRUE,
            'data-download' => FALSE,
            'data-toggle' => 'tooltip',
            'title' => t('Currently disabled due to performance and recursions.'),
          ) + $attributes,
          '#options' => array(
            'html' => TRUE,
            'external' => TRUE,
          ),
        ),
      );


      $buttons[$browser] = array(
        '#theme_wrappers' => array('container'),
        '#attributes' => array(
          'class' => array('btn-group'),
        ),
        'button' => $button,
        'dropdown-toggle' => array(
          '#type' => 'button',
          '#value' => '',
          '#icon' => '<span class="caret"></span><span class="sr-only">' . t('Toggle Dropdown') . '</span>',
          '#attributes' => array(
            'class' => $button_classes + array('dropdown-toggle'),
            'data-toggle' => 'dropdown',
          ),
        ),
        'menu' => array(
          '#theme' => 'item_list',
          '#attributes' => array(
            'class' => array('dropdown-menu'),
            'role' => 'menu',
          ),
          '#items' => $dropdown_links,
        ),
      );
    }
    else {
      $buttons[$browser] = $button;
    }
  }
  return $buttons;
}

/**
 * Page callback for "development/builds".
 */
function dreditor_org_builds() {
//  dpm(var_export(_dreditor_org_process_is_running(418), TRUE));
  $build = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'id' => 'dreditor-builds',
    ),
    '#attached' => array(
      'library' => array(
        array('js', 'js'),
      ),
      'js' => array(
        drupal_get_path('module', 'dreditor_org') . '/js/dreditor_org.js',
      ),
    ),
  );

  $repo = dreditor_org_repo();
  $default_branch = !empty($repo['default_branch']) ? $repo['default_branch'] : FALSE;

  // Pull requests.
  // @todo re-enable these once proper events are in place.
//  $pull_request_rows = array();
//  $build['types']['pull_requests'] = array(
//    '#type' => 'fieldset',
//    '#title' => t('Pull Requests'),
//    '#attributes' => array(
//      'id' => 'pull-requests',
//    ),
//  );
//  foreach (dreditor_org_pull_requests() as $pull_request) {
//    $pull_request_rows[] = array(
//      array(
//        'data' => '<strong>#' . $pull_request['number'] . '</strong> ' . l($pull_request['title'], $pull_request['html_url'], array('attributes' => array('target' => '_blank'))),
//      ),
//      array(
//        'data' => _dreditor_org_build_buttons(array(
//          'data-build' => 'pr',
//          'data-ref' => $pull_request['number'],
//        )),
//        'style' => 'text-align: right; width: 0;',
//      ),
//    );
//  }
//  $build['types']['pull_requests']['table'] = array(
//    '#theme' => 'table',
//    '#rows' => $pull_request_rows,
//    '#empty' => t('There are currently no pull request builds.'),
//    '#attributes' => array(
//      'class' => array('table-hover'),
//    ),
//  );

  // Branches.
  $branch_rows = array();
  $build['types']['branches'] = array(
    '#type' => 'fieldset',
    '#title' => t('Branches'),
    '#attributes' => array(
      'id' => 'branches',
    ),
  );
  foreach (dreditor_org_branches() as $branch) {
    $branch_title = $branch;
    if ($default_branch && $branch === $default_branch) {
      $branch_title .= ' (HEAD)';
      $default_branch = FALSE;
    }
    $branch_rows[] = array(
      array(
        'data' => $branch_title,
      ),
      array(
        'data' => _dreditor_org_build_buttons(array(
          'data-build' => 'branch',
          'data-ref' => $branch,
        )),
        'style' => 'text-align: right; width: 0;',
      ),
    );
  }
  $build['types']['branches']['table'] = array(
    '#theme' => 'table',
    '#rows' => $branch_rows,
    '#empty' => t('There are currently no branch builds.'),
    '#attributes' => array(
      'class' => array('table-hover'),
    ),
  );

  // Tags.
  $tag_rows = array();
  $build['types']['tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tags'),
    '#attributes' => array(
      'id' => 'tags',
    ),
  );
  foreach (dreditor_org_tags() as $tag) {
    $tag_rows[] = array(
      array(
        'data' => $tag,
      ),
      array(
        'data' => _dreditor_org_build_buttons(array(
          'data-build' => 'tag',
          'data-ref' => $tag,
        )),
        'style' => 'text-align: right; width: 0;',
      ),
    );
  }
  $build['types']['tags']['table'] = array(
    '#theme' => 'table',
    '#rows' => $tag_rows,
    '#empty' => t('There are currently no tagged builds.'),
    '#attributes' => array(
      'class' => array('table-hover'),
    ),
  );

  return $build;
}

/**
 * Menu callback for 'development/build/logs'.
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
          '#href' => 'development/build/logs/' . $file->filename,
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
 * Menu callback for 'development/build/logs/%'.
 */
function dreditor_org_build_log_view($file) {
  $module_path = drupal_get_path('module', 'dreditor_org');
  $build = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'id' => 'build-log',
      'class' => array('build-log'),
    ),
    '#attached' => array(
      'library' => array(
        array('system', 'ui'),
      ),
      'js' => array(
        $module_path . '/js/dreditor_org.js',
      ),
    ),
  );

  $raw_log = "public://logs/$file";
  $html_log = "public://logs/$file.html";
  if (file_exists($raw_log) || file_exists($html_log)) {
    drupal_set_title($file);
    if (!file_exists($html_log)) {
      if ($ansi2html = trim(shell_exec('which ansi2html'))) {
        file_put_contents($html_log, '');
        $stream_wrapper = file_stream_wrapper_get_instance_by_uri($raw_log);
        $raw_log_realpath = $stream_wrapper->realpath();
        $stream_wrapper = file_stream_wrapper_get_instance_by_uri($html_log);
        $html_log_realpath = $stream_wrapper->realpath();
        if ($raw_log_realpath && $html_log_realpath) {
          exec(sprintf('%s < %s > %s', $ansi2html, $raw_log_realpath, $html_log_realpath));
        }
      }
    }
    $build['toolbar'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array(
        'class' => array('btn-toolbar', 'pull-right'),
      ),
    );
    $build['toolbar']['view_modes'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array(
        'class' => array(
          'btn-group',
          'btn-group-sm',
          'pull-right',
          'view-modes',
        ),
        'data-toggle' => 'buttons',
      ),
      'pretty' => array(
        '#theme' => 'radio',
        '#prefix' => '<label class="btn btn-default">',
        '#suffix' => t('Pretty') . '</label>',
        '#attributes' => array(
          'class' => array('pretty'),
          'id' => 'pretty',
          'name' => 'view_mode',
        ),
      ),
      'raw' => array(
        '#theme' => 'radio',
        '#prefix' => '<label class="btn btn-default">',
        '#suffix' => t('Raw') . '</label>',
        '#attributes' => array(
          'class' => array('raw'),
          'id' => 'raw',
          'name' => 'view_mode',
        ),
      ),
    );

    $build['time'] = array(
      '#markup' => '<h4>' . format_date(filemtime($raw_log)) . '</h4>',
    );
    $build['raw'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'pre',
      '#value' => file_get_contents($raw_log),
      '#attributes' => array(
        'class' => array('raw'),
      ),
    );
    // Show the parsed HTML version.
    if (file_exists($html_log) && filesize($html_log) > 0) {
      $build['#attached']['js'][] = $module_path . '/js/dreditor_org.prettylog.js';
      $build['#attached']['js'][] = array(
        'data' => array(
          'prettyLogStyles' => $module_path . '/css/dreditor_org.prettylog.css',
        ),
        'type' => 'setting',
      );
      $build['html'] = array(
        '#markup' => '<iframe class="pretty" src="' . file_create_url($html_log) . '" height="500" width="100%" /></iframe>',
      );
    }
  }
  else {
    drupal_not_found();
  }
  return $build;
}
