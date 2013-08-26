(function($){

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
    prodTag = '1.1.7';
  })
  .complete(function () {
    $(document).ready(function () {
      $('body').once('dreditor-install', function () {
        var $button = $('#install-dreditor').attr({
          'data-loading-text': 'Installing Dreditor ' + prodTag + ' ...',
          'data-install-text': 'Install Dreditor ' + prodTag,
          'data-installed-text': 'Dreditor is installed. Click to re-install or update to Dreditor ' + prodTag,
          'data-success-text': 'Successfully installed Dreditor ' + prodTag + '!',
          'data-fail-text': 'Installing Dreditor ' + prodTag + ' failed. Try Again!'
        });
        var icon = '<i aria-hidden="true" class="icon fontello"></i>';
        
        Drupal.behaviors.dreditorInstall = {
          attach: function(context, settings) {
            if ($('#dreditor-is-installed').length) {
              $button
                .button('installed')
                .removeClass('btn-primary btn-success btn-danger')
                .addClass('btn-success')
                .prepend($(icon).addClass('dreditor-ok'));
            }
            else {
              $button
                .button('install')
                .prepend($(icon).addClass('dreditor-download'));
            }
          }
        }

        $button.on('click', function (e) {
          $button
            .button('loading')
            .removeClass('btn-success btn-danger')
            .addClass('btn-primary');
            
          // Chrome webstore.
          if (typeof chrome !== 'undefined') {
            chrome.webstore.install('https://chrome.google.com/webstore/detail/dhdpoembhlojpmehepeadblhglloobao', function () {
              $button
                .button('success')
                .removeClass('btn-primary btn-success btn-danger')
                .addClass('btn-success')
                .prepend($(icon).addClass('dreditor-ok'));
            },
            function () {
              $button
                .button('fail')
                .removeClass('btn-primary btn-success btn-danger')
                .addClass('btn-danger')
                .prepend($(icon).addClass('dreditor-cancel'));
            });
          }
          // Other browsers navigate to raw user-script.
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