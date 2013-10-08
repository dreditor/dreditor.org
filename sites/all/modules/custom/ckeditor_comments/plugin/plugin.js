/**
 * @file
 * Plugin to support inline commenting in CKEditor.
 */
(function ($, Drupal) {

  CKEDITOR.addCss(
    'html { background: #ebebeb; height: 100%; margin: 0 auto; max-width: 960px; }' +
    'body.cke_editable { box-shadow: 0 0 0 1px #d1d1d1,0 0 4px 1px #ccc; min-height: 100%; min-width: 800px; }' +
    '@media (min-width: 800px) { html { padding: 10px; } body.cke_editable { padding: .5in 1in; } }' +
    'span[data-cid] { background: rgb(255, 240, 179); padding: 1px 0 1px; border-bottom: 2px solid rgb(255, 240, 179); }'
  );

  CKEDITOR.plugins.add('inline_comment', {
    init : function(editor) {
      // Add Button
      editor.ui.addButton('inline_comment', {
        label: 'Comment',
        command: 'inline_comment',
        icon: this.path + 'inline-comment.png'
      });

      // Add Command
      editor.addCommand('inline_comment', {
        exec : function () {
//          var selection = editor.getSelection(), ranges = selection.getRanges();
        }
      });
    },
    afterInit: function(editor) {
      if (Drupal && Drupal.settings.CKEditor.inline_comments) {
        for (var id in Drupal.settings.CKEditor.inline_comments) {
          if (Drupal.settings.CKEditor.inline_comments.hasOwnProperty(id) && editor.name === id) {
            $('#' + editor.name).once('inline-comments', function () {
              console.log(Drupal.settings.CKEditor.inline_comments[id]);
            });
          }
        }
      }
    }
  });
})(jQuery, Drupal);
