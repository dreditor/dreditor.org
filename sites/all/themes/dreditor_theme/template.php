<?php

/**
 * @file
 * template.php
 */

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
