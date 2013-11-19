(function($){
  
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
  
  // Dyanmically determine the latest tagged release.
  $.getJSON('https://api.github.com/repos/dreditor/dreditor/tags', function (json) {
    for (var i = 0; i < json.length; i++) {
      if (json[i].name[0] === prodBranch) {
        prodTag = json[i].name;
        break;
      }
    }
  })
  .fail(function () {
    // Manually set a fall back production tag.
    prodTag = '1.2.1';
  })
  .complete(function () {
    $(document).ready(function () {
      $('body').once('dreditor-install', function () {
        var loadingText = 'Downloading Dreditor ' + prodTag + ' ...';
        var installText = 'Download Dreditor ' + prodTag;
        var installedText = 'Dreditor is installed. Click to re-install or update to Dreditor ' + prodTag;
        var successText = 'Successfully downloaded Dreditor ' + prodTag + '!';
        if ($.browser.chrome) {
          installText = 'Install the Dreditor ' + prodTag + ' Chrome extension';
          installedText = 'Dreditor Chrome extension is installed <small>(updates managed via Chrome)</small>';
          loadingText = 'Installing the Dreditor ' + prodTag + ' Chrome extension...';
          successText = 'Successfully installed the Dreditor ' + prodTag + ' Chrome extension!';
        }
        else if ($.browser.safari) {
          installText = 'Download the Dreditor ' + prodTag + ' Safari extension';
          installedText = 'Dreditor Safari extension is installed <small>(updates managed via Safari)</small>';
          loadingText = 'Downloading the Dreditor ' + prodTag + ' Safari extension ...';
          successText = 'Successfully downloaded Dreditor ' + prodTag + ' Safari extension!<br /><small>(Double-click the Safari extension to install it)</small>';
        }
        var $button = $('#install-dreditor').attr({
          'data-loading-text': loadingText,
          'data-install-text': installText,
          'data-installed-text': installedText,
          'data-success-text': successText,
          'data-fail-text': 'Installing Dreditor ' + prodTag + ' failed. Try Again!'
        });
        var icon = '<i aria-hidden="true" class="icon fontello"></i>';
        
        Drupal.behaviors.dreditorInstall = {
          attach: function(context, settings) {
            if ($('#dreditor-is-installed').length) {
              if ($button.length && ($.browser.chrome || $.browser.safari)) {
                $button
                  .html($button.data('installed-text'))
                  .removeClass('btn-primary btn-success btn-danger')
                  .addClass('btn-success disabled')
                  .attr('disabled', true)
                  .prepend($(icon).addClass('dreditor-checkmark'));
              }
              else if ($button.length) {
                $button
                  .button('installed')
                  .removeClass('btn-primary btn-success btn-danger')
                  .addClass('btn-success')
                  .prepend($(icon).addClass('dreditor-checkmark'));
              }
            }
            else if ($button.length) {
              $button
                .button('install')
                .prepend($(icon).addClass('dreditor-arrow-down'));
            }
          }
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
                .removeClass('btn-primary btn-success btn-danger')
                .addClass('btn-success')
                .prepend($(icon).addClass('dreditor-checkmark'));
            },
            function () {
              $button
                .button('fail')
                .removeClass('btn-primary btn-success btn-danger')
                .addClass('btn-danger')
                .prepend($(icon).addClass('dreditor-blocked'));
            });
          }
          // Safari extension.
          else if ($.browser.safari) {
            window.location = '/Dreditor.safariextz';
            $button
              .html($button.data('success-text'))
              .addClass('disabled')
              .attr('disabled', true)
              .prepend($(icon).addClass('dreditor-checkmark'));
          }
          // The rest of browsers must navigate to the raw user-script.
          else {
            window.location = 'https://github.com/dreditor/dreditor/raw/' + prodTag + '/dreditor.user.js';
          }
          e.preventDefault();
        });
        
        Drupal.attachBehaviors($(this));
      });
    });
  });
  
})(jQuery);