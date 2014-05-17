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
  if (JS) {
    JS.behaviors.dreditorOrg = {
      beforeSend: function (event, jqXHR, options) {
        if (options.automatedBuilds) {
          var icon = '<i aria-hidden="true" class="icon glyphicon glyphicon-refresh glyphicon-spin"></i>';

          // Use original trigger button-text data attribute value, if set.
          var buttonText = options.$trigger.data('button-text');

          // Determine if trigger is a dropdown menu-link.
          var $button = options.$trigger;
          if ($button.is('a') && $button.parents('.dropdown-menu').length) {
            // Redirect trigger to main button.
            $button = $button.parents('.btn-group').find('> .btn:first');

            // Use button's button-text data attribute value, if set.
            if (!buttonText) {
              buttonText = $button.data('button-text');
            }
          }

          // Change the button's value.
          if (buttonText) {
            $button.html(Drupal.t('!icon @text', { '!icon': icon, '@text': buttonText }));
          }
          else {
            $button.html(Drupal.t('!icon', { '!icon': icon }));
          }

          if (options.$buttons) {
            options.$buttons.prop('disabled', true);
          }
        }
      },
      complete: function (event, jqXHR, options, json) {
        if (options.automatedBuilds) {
          // Determine if trigger is a dropdown menu-link.
          var $button = options.$trigger;
          if ($button.is('a') && $button.parents('.dropdown-menu').length) {
            $button = $button.parents('.btn-group').find('> .btn:first');
          }

          if (json.buttonText) {
            $button.html(json.buttonText);
          }
          if (json.building) {
            if (options.$buttons) {
              options.$buttons.prop('disabled', true);
            }
            // If the download is currently building, keep creating timeouts
            // calling this method until it returns a download URL.
            setTimeout(function () {
              // Use original trigger here.
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
  }

})(window.jQuery, window.Drupal, window.JS);
