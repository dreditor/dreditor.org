<?php
/**
 * @file
 * bootstrap_ckeditor.default_breakpoints.inc
 */

/**
 * Implements hook_default_breakpoints().
 */
function bootstrap_ckeditor_default_breakpoints() {
  $export = array();

  $breakpoint = new stdClass();
  $breakpoint->disabled = FALSE; /* Edit this to true to make a default breakpoint disabled initially */
  $breakpoint->api_version = 1;
  $breakpoint->machine_name = 'custom.user.extra_small';
  $breakpoint->name = 'Extra Small';
  $breakpoint->breakpoint = 'min-width(480px)';
  $breakpoint->source = 'user';
  $breakpoint->source_type = 'custom';
  $breakpoint->status = 1;
  $breakpoint->weight = 0;
  $breakpoint->multipliers = array(
    '1x' => '1x',
    '1.5x' => '1.5x',
    '2x' => '2x',
  );
  $export['custom.user.extra_small'] = $breakpoint;

  $breakpoint = new stdClass();
  $breakpoint->disabled = FALSE; /* Edit this to true to make a default breakpoint disabled initially */
  $breakpoint->api_version = 1;
  $breakpoint->machine_name = 'custom.user.large';
  $breakpoint->name = 'Large';
  $breakpoint->breakpoint = 'min-width(1200px)';
  $breakpoint->source = 'user';
  $breakpoint->source_type = 'custom';
  $breakpoint->status = 1;
  $breakpoint->weight = 3;
  $breakpoint->multipliers = array(
    '1x' => '1x',
    '1.5x' => '1.5x',
    '2x' => '2x',
  );
  $export['custom.user.large'] = $breakpoint;

  $breakpoint = new stdClass();
  $breakpoint->disabled = FALSE; /* Edit this to true to make a default breakpoint disabled initially */
  $breakpoint->api_version = 1;
  $breakpoint->machine_name = 'custom.user.medium';
  $breakpoint->name = 'Medium';
  $breakpoint->breakpoint = 'min-width(992px)';
  $breakpoint->source = 'user';
  $breakpoint->source_type = 'custom';
  $breakpoint->status = 1;
  $breakpoint->weight = 2;
  $breakpoint->multipliers = array(
    '1x' => '1x',
    '1.5x' => '1.5x',
    '2x' => '2x',
  );
  $export['custom.user.medium'] = $breakpoint;

  $breakpoint = new stdClass();
  $breakpoint->disabled = FALSE; /* Edit this to true to make a default breakpoint disabled initially */
  $breakpoint->api_version = 1;
  $breakpoint->machine_name = 'custom.user.small';
  $breakpoint->name = 'Small';
  $breakpoint->breakpoint = 'min-width(768px)';
  $breakpoint->source = 'user';
  $breakpoint->source_type = 'custom';
  $breakpoint->status = 1;
  $breakpoint->weight = 1;
  $breakpoint->multipliers = array(
    '1x' => '1x',
    '1.5x' => '1.5x',
    '2x' => '2x',
  );
  $export['custom.user.small'] = $breakpoint;

  return $export;
}
