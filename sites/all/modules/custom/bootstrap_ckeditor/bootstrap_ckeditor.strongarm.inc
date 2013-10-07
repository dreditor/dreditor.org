<?php
/**
 * @file
 * bootstrap_ckeditor.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function bootstrap_ckeditor_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'picture_ckeditor_groups';
  $strongarm->value = array(
    'bootstrap' => array(
      'enabled' => 1,
      'weight' => '1',
      'fallback' => 'large_1x',
    ),
  );
  $export['picture_ckeditor_groups'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'picture_ckeditor_label';
  $strongarm->value = 'Image size (required)';
  $export['picture_ckeditor_label'] = $strongarm;

  return $export;
}
