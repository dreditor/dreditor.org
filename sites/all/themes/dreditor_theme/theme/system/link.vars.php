<?php
/**
 * @file
 * link.vars.php
 */

/**
 * Implements hook_preprocess_link__SUGGESTION().
 */
function dreditor_theme_preprocess_link__dreditor_extension(&$variables) {
  $browser = $variables['context']['browser'];
  $attributes = &$variables['options']['attributes'];
  $attributes['class'][] = 'btn';
  $attributes['class'][] = 'btn-xs';
  $attributes['class'][] = 'btn-group';
  $attributes['data-toggle'] = 'tooltip';
  $attributes['data-placement'] = 'bottom';

  // Browser specific variables.
  $variables['options']['html'] = TRUE;
  $variables['text'] = theme('icon', array(
    'bundle' => 'dreditor',
    'icon' => 'dreditor-' . $browser,
  )) . ' ' . $variables['text'];
  switch ($browser) {
    case 'chrome':
      $attributes['class'][] = 'btn-warning';
      break;

    case 'firefox':
      $attributes['class'][] = 'btn-danger';
      break;

    case 'safari':
      $attributes['class'][] = 'btn-primary';
      break;
  }
}
