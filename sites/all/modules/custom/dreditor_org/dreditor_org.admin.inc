<?php

/**
 * @file
 * Administrative settings forms for Dreditor.org.
 */

/**
 * Administrative settings form for application keys.
 */
function dreditor_org_github_settings($form, &$form_state) {
  $form['dreditor_org_github_username'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => variable_get('dreditor_org_github_username', ''),
    '#description' => t('The GitHub account the repository belongs to.'),
  );
  $form['dreditor_org_github_repository'] = array(
    '#title' => t('Repository'),
    '#type' => 'textfield',
    '#default_value' => variable_get('dreditor_org_github_repository', ''),
    '#description' => t('The repository to use.'),
  );
  $form['dreditor_org_github_access_token'] = array(
    '#title' => t('Access Token'),
    '#type' => 'textfield',
    '#default_value' => variable_get('dreditor_org_github_access_token', ''),
    '#description' => t('The access token associated with the GitHub repository.'),
  );
  return system_settings_form($form);
}
