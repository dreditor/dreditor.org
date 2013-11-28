<?php

/**
 * @file
 * Hooks provided by the GitHub WebHook module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Controls access to the endpoint.
 *
 * Return FALSE to deny access, TRUE to allow access, or NULL to defer to other
 * access callbacks. If no access hooks are implemented then the endpoint will
 * have no access control.
 *
 * @param array $payload
 *   The parsed JSON that was sent by GitHub.
 * @param array $args
 *   The arguments that are passed through wildcards in the path.
 *
 * @return bool|NULL
 */
function hook_github_webhook_access(array $payload, array $args) {
  return FALSE; // Deny access for some reason.
}

/**
 * Hook fired when a payload is received from a GitHub Service hook.
 *
 * @param array $payload
 *   The parsed JSON that was sent by GitHub.
 * @param array &$response
 *   An array containing the JSON response returned to the server that invoked
 *   the webhook. The array is structured as follows:
 *   - status: Either "ok" or "error".
 *   - messages: An array of messages describing what happened.
 *   - ...: Any additional data that you want to pass to the invoking server.
 * @param array $args
 *   The arguments that are passed through wildcards in the path.
 *
 * @see github_webhook_payload()
 * @see https://help.github.com/articles/post-receive-hooks
 */
function hook_github_webhook_event(array $payload, array &$response, array $args) {
  $commits = 0;
  foreach ($payload['commits'] as $commit) {
    $commits++;
    watchdog('github_webhook', t('Commit @id pushed to repo @repo'), array(
      '@id' => $commit['id'],
      '@repo' => $commit['url'],
    ));
  }
  $response['messages'][] = format_plural($commits, 'Processed @count commit', 'Processed @count commits');
}

/**
 * @} End of "addtogroup hooks".
 */
