(function($){

  var buttonText;

  Drupal.behaviors.dreditorTheme = {
    attach: function (context, settings) {
      var $button = $('#install-dreditor');
      if (!buttonText) {
        buttonText = $button.text();
      }
      var $icon = $('<i class="icon-white"></i>');
      var $installed = $('#dreditor-is-installed');
      if ($installed.length) {
        var $icon = $button.find('.icon').clone().removeClass('icon-download').addClass('icon-ok');
        $button.removeClass('btn-primary').addClass('btn-success').text(' Dreditor installed. Click to reinstall or update to ' + buttonText);
        $icon.addClass('icon-ok');
      }
      else {
        $button.text(' Install ' + buttonText);
        $icon.addClass('icon-download');
      }
      $button.prepend($icon);
      if (typeof chrome !== 'undefined') {
        $button
          .attr('href', '#')
          .bind('click', function (e){
            chrome.webstore.install('https://chrome.google.com/webstore/detail/dhdpoembhlojpmehepeadblhglloobao', function () {
              $button.removeClass('btn-primary').addClass('btn-success').text(' Successfully installed ' + buttonText + '!');
              $button.prepend($('<i class="icon-white icon-ok"></i>'));
            },
            function () {
              $button.removeClass('btn-primary').addClass('btn-error').text(' Failed to install ' + buttonText + '!');
              $button.prepend($('<i class="icon-white icon-warning-sign"></i>'));
            });
            e.preventDefault();
          });
      }
    }
  };

})(jQuery);