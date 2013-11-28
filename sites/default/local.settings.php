<?php

$databases['default']['default'] = array(
  'driver' => 'mysql',
  'database' => 'dreditor.sandbox',
  'username' => 'root',
  'password' => 'root',
  'host' => 'localhost',
);

if (file_exists('/usr/local/sclone/sites/configuration.inc')) {
  include_once '/usr/local/sclone/sites/configuration.inc';
}

$conf['stage_file_proxy_origin'] = 'http://dreditor.org';
$conf['stage_file_proxy_hotlink'] = TRUE;
