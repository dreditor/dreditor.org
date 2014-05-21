<?php
/**
 * @file
 * button.vars.php
 */

/**
 * Implements hook_preprocess_button__SUGGESTION().
 */
function dreditor_theme_preprocess_button__dreditor_build_op(&$variables) {
  $variables['element']['#attributes']['class'][] = 'btn-xs';
  $variables['element']['#attributes']['class'][] = 'dropdown-toggle';
  $variables['element']['#attributes']['data-toggle'] = 'dropdown';
  $variables['element']['#icon'] = _bootstrap_icon('cog') . ' <span class="caret"></span><span class="sr-only">' . t('Toggle Dropdown') . '</span>';
  $variables['element']['#value'] = '';
}
