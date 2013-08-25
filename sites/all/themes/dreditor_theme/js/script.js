(function($){
  
  Drupal.behaviors.dreditorTheme = {
    attach: function (context, settings) {
      var $button = $('#install-dreditor');
      if (chrome.app.isInstalled) {
        $button.removeClass('btn-primary').addClass('btn-success').text('Dreditor Installed!');
        $button.find('.icon-download').removeClass('icon-download');
      }
    }
  };
  
})(jQuery);