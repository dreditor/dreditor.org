<?php
/**
 * @file
 * container.vars.php
 */

/**
 * Implements hook_preprocess_container__SUGGESTION().
 */
function dreditor_theme_preprocess_container__dreditor_build_extensions(&$variables) {
  $attributes = &$variables['element']['#attributes'];
  $attributes['class'][] = 'btn-group';
}
