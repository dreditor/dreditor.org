<?php
/**
 * @file
 * links.vars.php
 */

/**
 * Implements hook_preprocess_links__SUGGESTION().
 */
function dreditor_theme_preprocess_links__dreditor_build_op(&$variables) {
  $variables['attributes']['class'][] = 'dropdown-menu';
  $variables['attributes']['role'] = 'menu';
  $context = $variables['context'];
  $links = array();
  foreach (array_keys($variables['links']) as $key) {
    $link = &$variables['links'][$key];
    switch ($key) {
      case 'log':
        $link['title'] = _bootstrap_icon('list-alt') . ' ' . $link['title'];
        $link['html'] = TRUE;
        if (empty($context['log_exists'])) {
          $link['href'] = '#';
          $link['external'] = TRUE;
          $link['attributes']['data-toggle'] = 'tooltip';
          $link['attributes']['title'] = t('Log unavailable');
          $key .= ' disabled';
        }
        else {
          $link['attributes']['data-toggle'] = 'modal';
          $link['absolute'] = TRUE;
        }
        break;

      case 'rebuild':
        $link['title'] = _bootstrap_icon('refresh') . ' ' . $link['title'];
        $link['html'] = TRUE;
        break;
    }
    $links[$key] = $link;
  }
  $variables['links'] = $links;
}
