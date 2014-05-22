<?php

/**
 * @file
 * template.php
 */

/**
 * Implements hook_preprocess_page().
 */
function dreditor_theme_preprocess_page(&$variables) {
  drupal_add_library('js', 'js');
}

/**
 * Implements hook_preprocess_menu_link().
 */
function dreditor_theme_preprocess_menu_link(&$variables) {
  global $user;
  $element = &$variables['element'];
  if ($element['#href'] === 'user' && in_array('expanded', $element['#attributes']['class']) && variable_get('user_pictures', 0)) {
    if (module_exists('gravatar')) {
      $picture = _gravatar_get_account_user_picture($user);
    }
    elseif (!empty($user->picture) && file_exists($user->picture)) {
      $picture = file_create_url($user->picture);
    }
    elseif (variable_get('user_picture_default', '')) {
      $picture = variable_get('user_picture_default', '');
    }
    if (isset($picture)) {
      if (module_exists('image') && file_valid_uri($picture) && $style = variable_get('user_picture_style', '')) {
        $user_picture = theme('image_style', array(
          'style_name' => $style,
          'path' => $picture,
          'attributes' => array('class' => array('img-thumbnail')),
        ));
      }
      else {
        $user_picture = theme('image', array('path' => $picture, 'attributes' => array('class' => array('img-thumbnail'))));
      }
      $element['#title'] = $user_picture . format_username($user);
      $element['#localized_options']['html'] = TRUE;
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function dreditor_theme_form_user_login_alter(&$form, &$form_state) {
  $form['#attributes']['class'][] = 'fade collapse';
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function dreditor_theme_form_connector_button_form_alter(&$form, &$form_state) {
  if (user_is_anonymous()) {
    $form['#prefix'] = '<p>&nbsp;</p>';
    $form['manual_login'] = array(
      '#type' => 'button',
      '#value' => t('Login with username/password'),
      '#attributes' => array(
        'class' => array(
          'pull-left',
          'manual-login',
        ),
        'data-toggle' => 'collapse',
        'data-target' => '#user-login',
      ),
      '#prefix' => '<span class="pull-left lead text-muted or"> - ' . t('or') . ' - </span>',
    );
  }
}

/**
 * Overrides theme_connector_buttons().
 */
function dreditor_theme_connector_buttons($variables) {

  $form = $variables['form'];
  if (!$form['#has_buttons']) {
    return NULL;
  }
  foreach (element_children($form) as $key) {
    if ($form[$key]['#type'] === 'submit' || $form[$key]['#type'] === 'button') {
      $class = str_replace('_', '-', $key);
      $form[$key]['#attributes']['class'][] = 'btn-lg';
      $form[$key]['#attributes']['class'][] = 'pull-left';
      $form[$key]['#attributes']['class'][] = $class;
      if ($key === 'oauthconnector_github') {
        $form[$key]['#value'] = theme('icon', array('bundle' => 'dreditor', 'icon' => 'dreditor-github')) . (user_is_anonymous() ? t('Login with GitHub') : t('Connect your GitHub account'));
        $form[$key]['#attributes']['class'][] = 'btn-success';
      }
    }
  }
  return drupal_render_children($form);

}
