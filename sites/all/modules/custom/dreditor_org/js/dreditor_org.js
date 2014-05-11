(function ($, Drupal, JS) {
  "use strict";

  Drupal.behaviors.dreditorOrg = {
    attach: function (context, settings) {
      $(context).find('#dreditor-builds [data-js-callback]').once('dreditor-builds', function () {
        var $element = $(this);
        var $parent = $element.parents('td');
        $element.on('click', function (e) {
          e.preventDefault();
          $(this).jsCallback({
            automatedBuilds: true,
            $buttons: $parent.find('> .btn-group > .btn')
          });
        });
      });
    }
  };

  /**
   * JS module behaviors for the dreditor_org module.
   */
  JS.behaviors.dreditorOrg = {
    beforeSend: function (event, jqXHR, options) {
      if (options.automatedBuilds) {
        if (options.$buttons) {
          options.$buttons.prop('disabled', true);
        }
        var $trigger = options.$trigger;
        if ($trigger.is('a')) {
          $trigger = $trigger.parents('.btn-group').find('> .btn:first');
        }
        var buttonText = $trigger.data('button-text');
        if (buttonText) {
          $trigger.html(Drupal.t('!icon @text', {
            '!icon': '<i aria-hidden="true" class="icon glyphicon glyphicon-refresh glyphicon-spin"></i>',
            '@text': buttonText
          }));
        }
      }
    },
    complete: function (event, jqXHR, options, json) {
      if (options.automatedBuilds) {
        var $trigger = options.$trigger;
        if ($trigger.is('a')) {
          $trigger = $trigger.parents('.btn-group').find('> .btn:first');
        }
        if (json.buttonText) {
          $trigger.html(json.buttonText);
        }
        if (json.building) {
          if (options.$buttons) {
            options.$buttons.prop('disabled', true);
          }
          // If the download is currently building, keep creating timeouts
          // calling this method until it returns a download URL.
          setTimeout(function () {
            // Original trigger here.
            options.$trigger.jsCallback({
              automatedBuilds: true,
              $buttons: options.$buttons
            });
          }, 5000);
        }
        else if (json.url) {
          window.location = json.url;
        }
        if ((json.url || json.disabled === false) && options.$buttons) {
          options.$buttons.prop('disabled', false);
        }
      }
    }
  };

})(window.jQuery, window.Drupal, window.JS);
