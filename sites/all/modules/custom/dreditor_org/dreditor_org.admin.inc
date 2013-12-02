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

