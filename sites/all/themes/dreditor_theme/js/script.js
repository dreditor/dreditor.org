/**
 * Compares two software version numbers (e.g. "1.7.1" or "1.2b").
 *
 * This function was born in http://stackoverflow.com/a/6832721.
 *
 * @param {string} v1 The first version to be compared.
 * @param {string} v2 The second version to be compared.
 * @param {object} [options] Optional flags that affect comparison behavior:
 * <ul>
 *     <li>
 *         <tt>lexicographical: true</tt> compares each part of the version strings lexicographically instead of
 *         naturally; this allows suffixes such as "b" or "dev" but will cause "1.10" to be considered smaller than
 *         "1.2".
 *     </li>
 *     <li>
 *         <tt>zeroExtend: true</tt> changes the result if one version string has less parts than the other. In
 *         this case the shorter string will be padded with "zero" parts instead of being considered smaller.
 *     </li>
 * </ul>
 * @returns {number|NaN}
 * <ul>
 *    <li>0 if the versions are equal</li>
 *    <li>a negative integer iff v1 < v2</li>
 *    <li>a positive integer iff v1 > v2</li>
 *    <li>NaN if either version string is in the wrong format</li>
 * </ul>
 *
 * @copyright by Jon Papaioannou (["john", "papaioannou"].join(".") + "@gmail.com")
 * @license This function is in the public domain. Do what you want with it, no strings attached.
 */
function versionCompare(v1, v2, options) {
  var lexicographical = options && options.lexicographical,
    zeroExtend = options && options.zeroExtend,
    v1parts = v1.split('.'),
    v2parts = v2.split('.');

  function isValidPart(x) {
    return (lexicographical ? /^\d+[A-Za-z]*$/ : /^\d+$/).test(x);
  }

  if (!v1parts.every(isValidPart) || !v2parts.every(isValidPart)) {
    return NaN;
  }

  if (zeroExtend) {
    while (v1parts.length < v2parts.length) {
      v1parts.push("0");
    }
    while (v2parts.length < v1parts.length) {
      v2parts.push("0");
    }
  }

  if (!lexicographical) {
    v1parts = v1parts.map(Number);
    v2parts = v2parts.map(Number);
  }

  for (var i = 0; i < v1parts.length; ++i) {
    if (v2parts.length == i) {
      return 1;
    }

    if (v1parts[i] == v2parts[i]) {
      continue;
    }
    else {
      if (v1parts[i] > v2parts[i]) {
        return 1;
      }
      else {
        return -1;
      }
    }
  }

  if (v1parts.length != v2parts.length) {
    return -1;
  }

  return 0;
}

(function(window, $, Drupal, JS){

  var DreditorTheme = window.DreditorTheme || {

    callbacks: {},

    /**
     * DOM object for modal.
     */
    $modal: $(),

    /**
     * DreditorTheme modal handler.
     */
    Modal: function (e) {
      var $this   = $(this);
      var href    = $this.attr('href');
      href = href ? href.replace(/.*(?=#[^\s]+$)/, '') : false;
      var $target = $($this.attr('data-target') || /^#/.test(href) && href || DreditorTheme.$modal);
      var defaults = {
        remote: !/#/.test(href) && href,
        title: Drupal.t('Loading...'),
        buttons: [
          {
            type: 'default',
            title: Drupal.t('Close')
          }
        ]
      };
      var option  = !$target.is(DreditorTheme.$modal) && $target.data('bs.modal') ? 'toggle' : $.extend(defaults, $target.data(), $this.data());

      // Prevent element default behavior if link or input.
      if ($this.is('a,:input')) {
        e.preventDefault();
      }

      if ($target.is(DreditorTheme.$modal)) {
        var $title = $target.find('.modal-title');
        var $body = $target.find('.modal-body');
        var $footer = $target.find('.modal-footer');
        var throbber = '<div class="ajax-progress ajax-progress-throbber"><span class="icon glyphicon glyphicon-refresh glyphicon-spin" aria-hidden="true"></span></div>';
        var $throbber = $();
        // Test to ensure content being requested is on same server, otherwise
        // let Bootstrap's modal handle it.
        if (!JS.urlIsExternal(option.remote)) {
          option.remote = false;
          var modalLoading = function () {
            $title.html(option.title);
            $body.html('');
            $footer.html('');
            $throbber = $(throbber).appendTo($footer);
          };
          modalLoading();
          var jsDefaluts = {
            // Don't send this data attribute.
            jsIgnoreData: ['toggle'],
            // @todo abstract these into JS.behaviors.
            beforeSend: modalLoading,
            complete: function (jqXHR) {
              if (this.dataType === 'json') {
                // Older versions of jQuery do not have jqXHR.responseJSON, we must parse
                // the responseText manually.
                var json = this.dataType === 'json' && jqXHR.responseText && $.parseJSON(jqXHR.responseText) || {};

                // Modal is redirecting, don't process anything.
                if (json.response && json.response.code && json.response.url && $.inArray(json.response.code, [301, 302, 303, 307]) !== -1) {
                  return;
                }
                $throbber.remove();
                // Get any messages processed already.
                var content = JS.messages.html();
                JS.messages.html('');
                if (json.title) {
                  $title.html(json.title);
                }
                if (json.content) {
                  content += json.content;
                }
                $body.html(content);

                // Bind any links inside the content to load using the same modal.
                $body.find('a').bind('click', function (e) {
                  e.preventDefault();
                  $(this).jsGet(jsDefaluts);
                });

                // Handle form submissions via JS module.
                var $form = $body.find('form').jsForm(jsDefaluts);

                // Focus first input available.
                $form.find(':input').first().focus();

                // Hide form actions and clone them into the footer of the modal.
                // They cannot be moved because their actions would no longer be
                // tied to the form element. Instead, bind the cloned elements so
                // they trigger the real elements.
                var $formActions = $form.find('.form-actions').hide();
                if ($formActions.length === 1) {
                  $footer.html('');
                  $formActions.find('*').each(function () {
                    var $element = $(this);
                    var callback = $element.data('callback');
                    $element.clone().appendTo($footer).not('[data-dismiss=modal]').on('click mousedown mouseup mouseenter keypress keydown keyup', function (e) {
                      if (callback && (e.type === 'click' || e.type === 'keypress')) {
                        if (typeof DreditorTheme.callbacks[callback] === 'function') {
                          DreditorTheme.callbacks[callback].apply(this, [e, $form]);
                        }
                      }
                      else {
                        $element.trigger(e.type);
                      }
                    });
                  });
                }
                Drupal.attachBehaviors($body);
              }
            }
          };
          $this.jsGet(jsDefaluts);
        }
        var footer = '';
        for (var i = 0, l = option.buttons.length; i < l; i++) {
          $footer.append('<button type="button" class="btn ' + (option.buttons[i].type ? 'btn-' + option.buttons[i].type : 'btn-default')  + '" data-dismiss="modal" aria-hidden="true">' + (option.buttons[i].title ? option.buttons[i].title : Drupal.t('Close')) + '</button>');
        }
      }

      $target
        .modal(option, this)
        .one('hide', function () {
          $this.is(':visible') && $this.trigger('focus')
        });
    }
  };

  Drupal.behaviors.dreditorTheme = {
    attach: function (context, settings) {
      $(document.body).once('dreditorTheme', function () {
        DreditorTheme.$modal = $('#dreditor-modal');

        // Handle events on the document DOM.
        $(document)
          // Remove existing Bootstrap modal handler.
          .off('click.bs.modal.data-api')

          // Add our own modal handler.
          .on('click.bs.modal.data-api', '[data-toggle="modal"]', DreditorTheme.Modal);
      });
    }
  };

  $(document).ready(function () {
    var $button = $('#install-dreditor');
    if (JS && $button.length) {
      var userAgent = navigator.userAgent.toLowerCase();
      $.browser.chrome = /chrome/.test(navigator.userAgent.toLowerCase())  && chrome && chrome.webstore;

      // Detect Chrome browsers.
      if ($.browser.chrome) {
        userAgent = userAgent.substring(userAgent.indexOf('chrome/') +7);
        userAgent = userAgent.substring(0,userAgent.indexOf('.'));
        $.browser.version = userAgent;
        $.browser.safari = false;
      }
      // Detect Safari browsers.
      else if ($.browser.safari) {
        userAgent = userAgent.substring(userAgent.indexOf('version/') +8);
        userAgent = userAgent.substring(0,userAgent.indexOf('.'));
        $.browser.version = userAgent;
      }

      var prodTag, prodBranch = '1';

      // Get the latest tag.
      JS.ajax({
        type: 'POST',
        data: {
          js_module: 'dreditor_org',
          js_callback: 'tags'
        },
        success: function (json) {
          if (json.tags) {
            for (var i = 0; i < json.tags.length; i++) {
              if (json.tags[i][0] === prodBranch && !json.tags[i].match(/beta|alpha|dev|rc/)) {
                prodTag = json.tags[i];
                break;
              }
            }
          }
        },
        complete: function () {
          setTimeout(function () {
            $('body').once('dreditor-install', function () {
              Drupal.behaviors.dreditorInstall = {
                attach: function(context, settings) {
                  var autoupdate = false;
                  var browser = '';
                  var browserIcon = 'arrow-down';
                  var disabled = false;
                  var error = false;
                  var installedVersion = '0.0.0';

                  // Manually set a fall back production tag.
                  if (!prodTag) {
                    prodTag = '1.2.10';
                  }

                  // Determine installed version.
                  if (Drupal.dreditor && Drupal.dreditor.version) {
                    installedVersion = Drupal.dreditor.version;
                  }
                  else if (Drupal.dreditor) {
                    installedVersion = '0.0.1';
                  }
                  if ($.browser.mozilla && installedVersion <= '1.2.3') {
                    installedVersion = '0.0.1';
                  }

                  // Determine the browser.
                  if ($.browser.chrome) {
                    browser = 'Chrome';
                    autoupdate = installedVersion !== '0.0.0';
                  }
                  else if ($.browser.mozilla) {
                    browser = 'Firefox';
                  }
                  else if ($.browser.safari) {
                    browser = 'Safari';
                    autoupdate = installedVersion !== '0.0.0';
                  }
                  if (browser.length) {
                    browserIcon = browser.toLowerCase();
                  }

                  var installText = 'Install Dreditor ' + prodTag + (browser ? ' for ' + browser : '');
                  var loadingText = 'Downloading Dreditor ' + prodTag + ' ...';
                  var updateText = 'A new version is available: Dreditor ' + prodTag;

                  if (autoupdate) {
                    disabled = true;
                    updateText += '<br /><small>(Update in ' + browser + ' extension preferences)</small>';
                  }

                  if (installedVersion === "0.0.1") {
                    installText = 'An old GreaseMonkey user script version of Dreditor is installed.<br /><small>(Remove the GreaseMonkey user script and reload this page)</small>.';
                    disabled = true;
                    error = true;
                    browserIcon = 'blocked';
                  }
                  else if (installedVersion !== '0.0.0' && versionCompare(installedVersion, prodTag) >= 0) {
                    updateText = 'Dreditor ' + installedVersion + ' is installed and up to date.';
                    disabled = true;
                  }
                  var successText = 'Successfully installed Dreditor ' + prodTag + '!';
                  if ($.browser.safari) {
                    successText = 'Downloaded Dreditor ' + prodTag + ' ' + browser + ' extension<br /><small>(double-click extension downloaded to install)</small>';
                  }

                  $button.attr({
                    'data-install-text': installText,
                    'data-loading-text': loadingText,
                    'data-update-text': updateText,
                    'data-success-text': successText,
                    'data-fail-text': 'Installing Dreditor ' + prodTag + ' failed. Try Again!'
                  });
                  var icon = '<i aria-hidden="true" class="icon fontello"></i>';

                  if ($button.length) {
                    if (installedVersion) {
                      if (installedVersion === '0.0.0' || installedVersion === '0.0.1') {
                        $button.html($button.data('install-text'));
                      }
                      else {
                        $button.html($button.data('update-text'));
                      }
                      $button
                        .removeClass('btn-primary btn-success btn-danger')
                        .prepend($(icon).addClass('dreditor-' + browserIcon));

                      if (disabled) {
                        $button.addClass('disabled').prop('disabled', true);
                      }
                      else {
                        $button.removeClass('disabled').prop('disabled', false);
                      }
                      if (error) {
                        $button.addClass('btn-danger');
                      }
                      else {
                        $button.addClass('btn-success');
                      }
                    }
                    else {
                      $button
                        .button('install')
                        .removeClass('disabled')
                        .prop('disabled', false)
                        .prepend($(icon).addClass('dreditor-' + browserIcon));
                    }
                    $button.on('click', function (e) {
                      $button
                        .button('loading')
                        .removeClass('btn-success btn-danger')
                        .addClass('btn-primary');

                      // Chrome extension.
                      if ($.browser.chrome) {
                        chrome.webstore.install('https://chrome.google.com/webstore/detail/dhdpoembhlojpmehepeadblhglloobao', function () {
                            $button
                              .button('success')
                              .removeClass('btn-primary btn-danger')
                              .addClass('btn-success disabled')
                              .prepend($(icon).addClass('dreditor-checkmark'));
                          },
                          function () {
                            $button
                              .button('fail')
                              .removeClass('btn-primary btn-success')
                              .addClass('btn-danger')
                              .prepend($(icon).addClass('dreditor-blocked'));
                          });
                      }
                      // Firefox extension.
                      else if ($.browser.mozilla) {
                        if (InstallTrigger) {
                          JS.ajax({
                            type: 'POST',
                            data: {
                              js_module: 'dreditor_org',
                              js_callback: 'ff_hash'
                            },
                            success: function (json) {
                              if (json && json.hash) {
                                var params = {
                                  'dreditor': {
                                    URL: '/dreditor.xpi?' + prodTag,
                                    IconURL: '/sites/all/themes/dreditor_theme/logo.png',
                                    hash: json.hash,
                                    toString: function () { return '/dreditor.xpi?' + prodTag; }
                                  }
                                };
                                InstallTrigger.install(params, function xpinstallCallback(url, status){
                                  if (status === 0) {
                                    $button
                                      .html($button.data('success-text'))
                                      .removeClass('btn-primary btn-danger')
                                      .addClass('btn-success disabled')
                                      .attr('disabled', true)
                                      .prepend($(icon).addClass('dreditor-checkmark'));
                                  }
                                  else {
                                    $button
                                      .button('fail')
                                      .removeClass('btn-primary btn-success')
                                      .addClass('btn-danger')
                                      .prepend($(icon).addClass('dreditor-blocked'));
                                  }
                                });
                              }
                            },
                            error: function () {
                              window.location = '/dreditor.xpi';
                            }
                          });
                        }
                        else {
                          window.location = '/dreditor.xpi';
                        }
                      }
                      // Safari extension.
                      else if ($.browser.safari) {
                          window.location = '/dreditor.safariextz';
                          $button
                            .html($button.data('success-text'))
                            .removeClass('btn-primary btn-danger')
                            .addClass('btn-success disabled')
                            .attr('disabled', true)
                            .prepend($(icon).addClass('dreditor-checkmark'));
                        }
                      e.preventDefault();
                    });
                  }
                }
              }
              Drupal.attachBehaviors($(this));
            });
          }, 1000);
        }
      });
    }
  });

  /**
   * Overrides the prototype "statusMessages" theme implementation.
   */
  Drupal.theme.statusMessages = function (messages) {
    var output = '';
    var status_heading = {
      status: Drupal.t('Status message'),
      error: Drupal.t('Error message'),
      warning: Drupal.t('Warning message')
    };
    // Map Drupal message types to their corresponding Bootstrap classes.
    // @see http://twitter.github.com/bootstrap/components.html#alerts
    var status_class = {
      status: 'success',
      error: 'danger',
      warning: 'warning',
      info: 'info'
    };
    for (var type in messages) {
      if (!messages.hasOwnProperty(type)) {
        continue;
      }
      if (messages[type].length > 0) {
        var messageClass = (typeof(status_class[type]) !== 'undefined' ? ' alert-' + status_class[type] : '');
        output += "<div class=\"alert alert-block" + messageClass + " messages " + type +"\">\n";
        output += "  <a class=\"close\" data-dismiss=\"alert\" href=\"#\">&times;</a>\n";
        if (typeof(status_heading[type]) !== 'undefined') {
          output += '<h4 class="element-invisible">' + status_heading[type] + "</h4>\n";
        }
        if (messages[type].length > 1) {
          output += " <ul>\n";
          for (var i in messages[type]) {
            output += '  <li>' + messages[type][i] + "</li>\n";
            delete messages[type][i];
          }
          output += " </ul>\n";
        }
        else {
          output += messages[type][0];
        }
        output += "</div>\n";
      }
    }
    return output;
  };

})(window, window.jQuery, window.Drupal, window.JS);
