<?php
/**
 * @file
 * bootstrap_ckeditor.default_picture_mapping.inc
 */

/**
 * Implements hook_default_picture_mapping().
 */
function bootstrap_ckeditor_default_picture_mapping() {
  $export = array();

  $picture_mapping = new stdClass();
  $picture_mapping->disabled = FALSE; /* Edit this to true to make a default picture_mapping disabled initially */
  $picture_mapping->api_version = 1;
  $picture_mapping->machine_name = 'bootstrap';
  $picture_mapping->breakpoint_group = 'bootstrap';
  $picture_mapping->mapping = array(
    'custom.user.extra_small' => array(
      '1x' => 'extra_small_1x',
      '1.5x' => 'extra_small_1_5x',
      '2x' => 'extra_small_2x',
    ),
    'custom.user.small' => array(
      '1x' => 'small_1x',
      '1.5x' => 'small_1_5x',
      '2x' => 'small_2x',
    ),
    'custom.user.medium' => array(
      '1x' => 'medium_1x',
      '1.5x' => 'medium_1_5x',
      '2x' => 'medium_2x',
    ),
    'custom.user.large' => array(
      '1x' => 'large_1x',
      '1.5x' => 'large_1_5x',
      '2x' => 'large_2x',
    ),
  );
  $export['bootstrap'] = $picture_mapping;

  return $export;
}
