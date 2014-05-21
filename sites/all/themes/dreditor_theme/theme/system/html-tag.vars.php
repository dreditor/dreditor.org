<?php
/**
 * @file
 * html-tag.vars.php
 */

/**
 * Implements hook_preprocess_html_tag__SUGGESTION().
 */
function dreditor_theme_preprocess_html_tag__dreditor_build_rebase(&$variables) {
  $element = &$variables['element'];
  $element['#attributes']['class'][] = 'text-warning';
  $element['#value'] = _bootstrap_icon('warning-sign') . ' ' . $element['#value'];
}

/**
 * Implements hook_preprocess_html_tag__SUGGESTION().
 */
function dreditor_theme_preprocess_html_tag__dreditor_build_failed(&$variables) {
  $element = &$variables['element'];
  $element['#attributes']['class'][] = 'btn-group';
  $element['#attributes']['class'][] = 'text-danger';
  $element['#value'] = _bootstrap_icon('remove') . ' ' . $element['#value'];
}

/**
 * Implements hook_preprocess_html_tag__SUGGESTION().
 */
function dreditor_theme_preprocess_html_tag__dreditor_no_builds(&$variables) {
  $element = &$variables['element'];
  $element['#attributes']['class'][] = 'btn-group';
  $element['#attributes']['class'][] = 'text-muted';
}

/**
 * Implements hook_preprocess_html_tag__SUGGESTION().
 */
function dreditor_theme_preprocess_html_tag__dreditor_building(&$variables) {
  $element = &$variables['element'];
  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'btn-xs';
  $element['#attributes']['class'][] = 'btn-default';
  $element['#attributes']['class'][] = 'disabled';
  $icon = theme('icon', array(
    'bundle' => 'bootstrap',
    'icon' => 'glyphicon-refresh',
    'attributes' => array(
      'class' => array('glyphicon-spin'),
    ),
  ));
  $element['#value'] = $icon . ' ' . $element['#value'] . '...';
}
