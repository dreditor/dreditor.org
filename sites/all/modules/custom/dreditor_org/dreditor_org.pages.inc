<?php
/**
 * @file
 * Pages for Dreditor.org.
 */

/**
 * Menu callback for "ajax/dreditor/%".
 *
 * @todo turn this in to a JS module callback.
 */
function dreditor_org_ajax($action) {
  $json = array();
  if ($action === 'tags') {
    $json['tags'] = array_keys(dreditor_org_tags());
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
 * Page callback for "development/builds".
 */
function dreditor_org_builds() {
  $module_path = drupal_get_path('module', 'dreditor_org');
  $build = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'id' => 'dreditor-builds',
      'class' => array(
        'dreditor-builds',
      ),
    ),
    '#attached' => array(
      'library' => array(
        array('js', 'js'),
        array('system', 'ui'),
      ),
      'js' => array(
        $module_path . '/js/dreditor_org.js',
        array(
          'data' => array(
            'prettyLogStyles' => $module_path . '/css/dreditor_org.prettylog.css',
          ),
          'type' => 'setting',
        ),
      ),
    ),
  );

  $repo = dreditor_org_repo();
  $default_branch = !empty($repo['default_branch']) ? $repo['default_branch'] : FALSE;

  // Pull requests.
  $pull_request_rows = array();
  $build['types']['pull_requests'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pull Requests'),
    '#attributes' => array(
      'id' => 'pull-requests',
    ),
  );
  foreach (dreditor_org_pull_requests() as $pr) {
    $mergable = (bool) $pr['mergeable'];
    $pull_request_rows[] = array(
      array(
        'data' => array(
          'number' => array(
            '#theme' => 'html_tag',
            '#tag' => 'span',
            '#value' => "#" . $pr['number'],
            '#attributes' => array(
              'class' => array(
                'label',
                'label-primary',
              ),
            ),
          ),
          'title' => array(
            '#type' => 'link',
            '#title' => $pr['title'],
            '#href' => $pr['html_url'],
            '#attributes' => array(
              'target' => '_blank',
            ),
          ),
        ),
      ),
      array(
        'data' => $mergable ? _dreditor_org_build_buttons(array(
          'pr' => $pr['number'],
        )) : array(
          '#theme' => 'html_tag__dreditor_build_rebase',
          '#tag' => 'span',
          '#value' => t('Rebase needed'),
        ),
        'style' => 'text-align: right; width: 0;',
      ),
    );
  }
  $build['types']['pull_requests']['table'] = array(
    '#theme' => 'table',
    '#rows' => $pull_request_rows,
    '#empty' => t('There are currently no pull request builds.'),
    '#context' => array(
      'striped' => FALSE,
      'hover' => FALSE,
    ),
  );

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
    if (empty($branch['autobuild'])) {
      continue;
    }
    $branch_title = $branch['name'];
    if ($default_branch && $branch['name'] === $default_branch) {
      $branch_title .= ' (HEAD)';
      $default_branch = FALSE;
    }
    $branch_rows[] = array(
      array(
        'data' => $branch_title,
      ),
      array(
        'data' => _dreditor_org_build_buttons(array(
          'branch' => $branch['name'],
        )),
        'style' => 'text-align: right; width: 0;',
      ),
    );
  }
  $build['types']['branches']['table'] = array(
    '#theme' => 'table',
    '#rows' => $branch_rows,
    '#empty' => t('There are currently no branch builds.'),
    '#context' => array(
      'striped' => FALSE,
      'hover' => FALSE,
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
    if (empty($tag['autobuild'])) {
      continue;
    }
    $tag_rows[] = array(
      array(
        'data' => $tag['name'],
      ),
      array(
        'data' => _dreditor_org_build_buttons(array(
          'branch' => $tag['name'],
          'checkout' => $tag['commit']['sha'],
        )),
        'style' => 'text-align: right; width: 0;',
      ),
    );
  }
  $build['types']['tags']['table'] = array(
    '#theme' => 'table',
    '#rows' => $tag_rows,
    '#empty' => t('There are currently no tagged builds.'),
    '#context' => array(
      'striped' => FALSE,
      'hover' => FALSE,
    ),
  );

  return $build;
}

/**
 * Menu callback for 'development/build/log'.
 */
function dreditor_org_build_log($hash = NULL, $bid = NULL) {
  $paths = _dreditor_org_build_paths($hash, $bid);
  $hashes = variable_get('dreditor_build_hashes', array());
  $title = t('Build Log');
  if (isset($hashes[$hash])) {
    $options = unserialize($hashes[$hash]);
    if (isset($options['branch'])) {
      $title = t('Build log: @ref', array(
        '@ref' => $options['branch'],
      ));
    }
    elseif (isset($options['pr'])) {
      $title = t('Build log: Pull #@pr', array(
        '@pr' => $options['pr'],
      ));
    }
  }
  if (file_exists($paths['log']) && filesize($paths['log']) > 0) {
    drupal_set_title($title, PASS_THROUGH);
    $build = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array(
        'id' => 'dreditor-build-log',
        'class' => array('dreditor-build-log'),
      ),
    );
    if ($ansi2html = trim(shell_exec('which ansi2html'))) {
      if (file_exists($paths['log_html'])) {
        file_unmanaged_delete($paths['log_html']);
      }
      exec(sprintf('%s < %s > %s', $ansi2html, _dreditor_org_uri_realpath($paths['log']), _dreditor_org_uri_realpath($paths['log_html'])));
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
      '#markup' => '<h4>' . format_date(filemtime($paths['log']), 'custom', 'l, M j, Y - g:i A (T)') . '</h4>',
    );
    $build['raw'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'pre',
      '#value' => file_get_contents($paths['log']),
      '#attributes' => array(
        'class' => array('raw'),
      ),
    );
    // Show the parsed HTML version.
    if (file_exists($paths['log_html']) && filesize($paths['log_html']) > 0) {
      $build['pretty'] = array(
        '#markup' => '<iframe class="pretty" src="' . file_create_url($paths['log_html']) . '"></iframe>',
      );
    }
  }
  else {
    return MENU_NOT_FOUND;
  }
  return $build;
}
