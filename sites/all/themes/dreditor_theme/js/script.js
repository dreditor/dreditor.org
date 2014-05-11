(function($){

  $(document).ready(function () {
    var $button = $('#install-dreditor');
    if ($button.length) {
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
      else if ($.browser.mozilla) {
          var FF_HASH;
          $.getJSON('/ajax/dreditor/ff-hash', function (json) {
            FF_HASH = json.hash;
          });
        }

      var prodTag, prodBranch = '1';

      // Get the latest tag.
      $.getJSON('/ajax/dreditor/tags', function (json) {
        if (json.tags) {
          for (var i = 0; i < json.tags.length; i++) {
            if (json.tags[i][0] === prodBranch && !json.tags[i].match(/beta|alpha|dev|rc/)) {
              prodTag = json.tags[i];
              break;
            }
          }
        }
      })
      .fail(function () {
        // Manually set a fall back production tag.
        prodTag = '1.2.4';
      })
      .complete(function () {
          setTimeout(function () {
            $('body').once('dreditor-install', function () {
              Drupal.behaviors.dreditorInstall = {
                attach: function(context, settings) {
                  var autoupdate = false;
                  var browserIcon = 'arrow-down';
                  var browser = '';
                  if ($.browser.chrome) {
                    browser = 'Chrome';
                    autoupdate = true;
                  }
                  else if ($.browser.mozilla) {
                    browser = 'Firefox';
                  }
                  else if ($.browser.safari) {
                      browser = 'Safari';
                      autoupdate = true;
                    }
                  if (browser.length) {
                    browserIcon = browser.toLowerCase();
                  }

                  var installedVersion = false;
                  if (Drupal.dreditor && Drupal.dreditor.version) {
                    installedVersion = Drupal.dreditor.version;
                  }
                  else if (Drupal.dreditor) {
                    installedVersion = '0.0.1';
                  }
                  if ($.browser.mozilla && installedVersion <= '1.2.3') {
                    installedVersion = '0.0.1';
                  }

                  var disabled = false;
                  var error = false;

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
                  else if (installedVersion >= prodTag) {
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
                      if (installedVersion === '0.0.1') {
                        $button.html($button.data('install-text'));
                      }
                      else {
                        $button.html($button.data('update-text'));
                      }
                      $button
                        .removeClass('btn-primary btn-success btn-danger')
                        .prepend($(icon).addClass('dreditor-' + browserIcon));

                      if (disabled) {
                        $button.addClass('disabled').attr('disabled', 'disabled');
                      }
                      else {
                        $button.removeClass('disabled').removeAttr('disabled');
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
                        var params = {
                          'dreditor': {
                            URL: '/dreditor.xpi',
                            IconURL: '/sites/all/themes/dreditor_theme/logo.png',
                            hash: FF_HASH,
                            toString: function () { return '/dreditor.xpi'; }
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

})(jQuery);
