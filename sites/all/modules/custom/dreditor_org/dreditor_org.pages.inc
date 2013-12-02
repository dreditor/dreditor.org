<?php
/**
 * @file
 * Pages for Dreditor.org.
 */

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
  $repo = dreditor_org_repo();
  $master_branch = !empty($repo['master_branch']) ? $repo['master_branch'] : FALSE;

  // Icons.
  $icons['chrome'] = array(
    '#type' => 'button',
    '#value' => theme('icon', array('bundle' => 'dreditor', 'icon' => 'dreditor-chrome')) . t('ZIP'),
    '#attributes' => array(
      'class' => array('btn-sm', 'chrome'),
    ),
  );
  $icons['firefox'] = array(
    '#type' => 'button',
    '#value' => theme('icon', array('bundle' => 'dreditor', 'icon' => 'dreditor-firefox')) . t('XPI'),
    '#attributes' => array(
      'class' => array('btn-sm', 'firefox'),
    ),
  );
  $icons['safari'] = array(
    '#type' => 'button',
    '#value' => theme('icon', array('bundle' => 'dreditor', 'icon' => 'dreditor-safari')) . t('SAFARIEXTZ'),
    '#attributes' => array(
      'class' => array('btn-sm', 'safari'),
    ),
  );

  // Pull requests.
  $pull_request_rows = array();
  $build['types']['pull_requests'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pull Requests'),
    '#attributes' => array(
      'id' => 'pull-requests',
    ),
  );
  foreach (dreditor_org_pull_requests() as $pull_request) {
    $pull_request_rows[] = array(
      array(
        'data' => '<strong>#' . $pull_request['number'] . '</strong> ' . l($pull_request['title'], $pull_request['html_url'], array('attributes' => array('target' => '_blank'))),
        'style' => 'width: 100%;',
      ),
      array(
        'data' => $icons,
        'style' => 'white-space: nowrap;',
      ),
    );
  }
  $build['types']['pull_requests']['table'] = array(
    '#theme' => 'table',
    '#rows' => $pull_request_rows,
    '#empty' => t('There are currently no pull requests to choose from.'),
    '#attributes' => array(
      'class' => array('table-hover'),
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
    if ($master_branch && $branch === $master_branch) {
      $branch .= ' (master)';
      $master_branch = FALSE;
    }
    $branch_rows[] = array(
      array(
        'data' => $branch,
        'style' => 'width: 100%;',
      ),
      array(
        'data' => $icons,
        'style' => 'white-space: nowrap;',
      ),
    );
  }
  $build['types']['branches']['table'] = array(
    '#theme' => 'table',
    '#rows' => $branch_rows,
    '#empty' => t('There are currently no branches to choose from.'),
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
        'style' => 'width: 100%;',
      ),
      array(
        'data' => $icons,
        'style' => 'white-space: nowrap;',
      ),
    );
  }
  $build['types']['tags']['table'] = array(
    '#theme' => 'table',
    '#rows' => $tag_rows,
    '#empty' => t('There are currently no tags to choose from.'),
    '#attributes' => array(
      'class' => array('table-hover'),
    ),
  );

  return $build;
}
