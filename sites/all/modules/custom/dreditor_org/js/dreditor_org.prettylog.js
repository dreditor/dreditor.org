if (typeof Array.prototype.regExpIndexOf === 'undefined') {
  Array.prototype.regExpIndexOf = function (regExp) {
    for (var i in this) {
      if (this[i].toString().match(regExp)) {
        return i;
      }
    }
    return -1;
  };
}

(function ($, Drupal, JS) {
  "use strict";

  Drupal.behaviors.dreditorOrgPrettyLog = {
    attach: function (context, settings) {
      $('#build-log').once('build-log', function () {
        var $wrapper = $(this);
        var $toolbar = $wrapper.find('> .btn-toolbar');

        // View Modes.
        var $viewModes = $toolbar.find('.view-modes');
        var $pretty = $wrapper.find('> .pretty');
        var $raw = $wrapper.find('> .raw');
        if (!$pretty.length) {
          $viewModes.remove();
          return;
        }
        $viewModes.find('#pretty').parent().button('toggle');
        $raw.hide();
        $viewModes.find('.btn :input').on('change', function () {
          var raw = $toolbar.find(':input[name=view_mode]:checked').attr('id') === 'raw';
          $raw[raw ? 'show' : 'hide']();
          $pretty[raw ? 'hide' : 'show']();
        });
        // Parse the log.
        $pretty.load(function () {
          var $dom = $pretty.contents();
          var $html = $dom.find('body > pre');
          $('<link type="text/css" rel="stylesheet" href="' + settings.basePath + settings.prettyLogStyles + '" media="all">').appendTo($dom.find('head'));
          var log = $html.html().split(/--- drush dreditor-build: executing command ---\n/gmi).filter(function(v){return $.trim(v)!==''});
          for (var i = 0; i < log.length; i++) {
            var error = '';

            var lines = log[i].split("\n");
            var errLine = lines.regExpIndexOf(new RegExp('^--- drush dreditor-build: executing command failed ---'));
            if (errLine !== -1) {
              error = ' error active';
              lines.splice(errLine, 1);
            }
            log[i] = '<div class="command-output' + error + '"><div class="command">' + lines.shift() + '</div><div class="output"><p><span class="ln"></span>' + lines.join('</p><p><span class="ln"></span>') + '</p><div class="collapse">[collapse]</div></div></div>';
          }
          $html.html(log.join(''));
          $html.find('.command-output').not('.active').find('.output').hide();
          $html
            .on('click', '.command', function () {
              var $command = $(this);
              var $wrapper = $command.parent();
              var $output = $wrapper.find('.output');
              var active = $wrapper.hasClass('active');
              $wrapper[active ? 'removeClass' : 'addClass']('active');
              $output[active ? 'slideUp' : 'slideDown']();
            })
            .on('click', '.collapse', function () {
              var $wrapper = $(this).parent().parent().removeClass('active');
              $wrapper.find('.output').slideUp();
            });
        });
      });

    }
  };

})(window.jQuery, window.Drupal, window.JS);
