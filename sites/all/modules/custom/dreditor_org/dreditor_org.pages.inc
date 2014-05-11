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
  $master_branch = !empty($repo['master_branch']) ? $repo['master_branch'] : FALSE;

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
    if ($master_branch && $branch === $master_branch) {
      $branch .= ' (master)';
      $master_branch = FALSE;
    }
    $branch_rows[] = array(
      array(
        'data' => $branch,
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
