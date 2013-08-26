(function($){
  
  Drupal.behaviors.dreditorTheme = {
    attach: function (context, settings) {
      var $installed = $('#dreditor-is-installed');
      var $button = $('#install-dreditor');
      if ($installed.length) {
        var $icon = $button.find('.icon-download').clone().removeClass('icon-download').addClass('icon-ok');
        $button.removeClass('btn-primary').addClass('btn-success').text(' Dreditor is Installed!');
        $button.prepend($icon);
      }
      if (typeof chrome !== 'undefined') {
        $button.bind('click', function (e){
          chrome.webstore.install();
          e.preventDefault();
        });
      }
    }
  };
  
})(jQuery);