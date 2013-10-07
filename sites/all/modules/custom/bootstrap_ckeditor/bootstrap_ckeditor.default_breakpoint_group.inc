<?php
/**
 * @file
 * bootstrap_ckeditor.default_breakpoint_group.inc
 */

/**
 * Implements hook_default_breakpoint_group().
 */
function bootstrap_ckeditor_default_breakpoint_group() {
  $export = array();

  // Breakpoints.
  $breakpoints = array();
  $breakpoints[] = 'custom.user.extra_small';
  $breakpoints[] = 'custom.user.small';
  $breakpoints[] = 'custom.user.medium';
  $breakpoints[] = 'custom.user.large';

  // Breakpoint group.
  $breakpoint_group = new stdClass();
  $breakpoint_group->disabled = FALSE; /* Edit this to true to make a default breakpoint_group disabled initially */
  $breakpoint_group->api_version = 1;
  $breakpoint_group->machine_name = 'bootstrap';
  $breakpoint_group->name = 'Automatic (responsive)';
  $breakpoint_group->breakpoints = $breakpoints;
  $breakpoint_group->type = 'custom';
  $breakpoint_group->overridden = 0;
  $export['bootstrap'] = $breakpoint_group;

  return $export;
}
