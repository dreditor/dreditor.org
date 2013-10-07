function cke_tpl_text(lines) {
  var output = '';
  var lines = lines || 4;
  if (lines > 4) {
    lines = 4;
  }
  var lipsumContent = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque eu magna orci, sollicitudin ultrices purus. Pellentesque dignissim laoreet sem sed viverra. Sed elementum dolor vitae erat vestibulum ultricies. Vivamus mollis euismod erat, ut porttitor ipsum placerat non. Fusce ligula ante, ultrices quis rutrum ut, euismod et eros. Curabitur vitae iaculis neque. Maecenas id urna velit, vel molestie libero. Nunc neque urna, fermentum quis malesuada non, facilisis et mauris. Nunc sollicitudin sollicitudin ipsum, consectetur hendrerit urna pellentesque ut.\r" +
    "Nulla sapien nisi, fermentum eget lobortis eu, volutpat ac purus. Duis molestie dolor nec dui rutrum mollis. Duis rutrum malesuada augue, vel facilisis est aliquam ut. Pellentesque semper commodo ipsum, a porta erat ultrices nec. Nullam ut scelerisque nulla. Nullam scelerisque sapien eu augue fermentum feugiat. Sed quis nisl erat, rutrum feugiat odio. Nulla facilisi. Vestibulum tempor eleifend dignissim. Mauris tellus purus, cursus in vestibulum at, auctor at dolor. Phasellus vitae felis eget massa consectetur pretium. Donec elementum lacus at elit mollis vehicula.\r" +
    "Quisque quis risus et lectus rutrum imperdiet non quis orci. Cras a neque risus. Mauris eros lorem, rhoncus quis facilisis quis, semper et eros. Ut facilisis, magna at egestas auctor, enim tellus malesuada est, eget iaculis tellus velit a odio. Duis porttitor malesuada ligula, sit amet imperdiet nisl dapibus sed. Etiam viverra dapibus rutrum. Cras ultrices, augue ac rutrum laoreet, est lectus ultricies sapien, at elementum enim lectus id dolor. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque lectus ipsum, venenatis a consectetur eget, hendrerit sed metus. Cras sodales adipiscing nibh eu pharetra. Aenean sed eleifend diam. Suspendisse nec ullamcorper ipsum. Aliquam sodales turpis nec risus sagittis porta. Morbi ac turpis vel nulla cursus placerat at nec tellus. Suspendisse potenti.\r" +
    "Fusce malesuada turpis quis urna tempor imperdiet. Aliquam sodales facilisis lorem, id aliquam ante consequat id. Aenean ac odio eu erat euismod faucibus. In eget metus velit, in vehicula nunc. Nulla id augue dolor. Mauris mattis justo et sapien gravida dapibus eget ut justo. Cras velit felis, commodo vitae gravida id, iaculis a nulla.\r" +
    "In aliquam, eros sit amet tempus fringilla, mi nunc sodales erat, sed commodo nibh dui eget sapien. Aenean egestas vulputate eros, ut auctor libero pellentesque eu. Suspendisse lobortis sollicitudin rutrum. Etiam aliquet erat sit amet enim consequat nec tincidunt ipsum feugiat. In bibendum augue at sapien pulvinar venenatis eget quis nulla. Fusce nec felis eros. Maecenas tempus, dolor vel dignissim interdum, leo nisi gravida velit, sed semper mauris diam nec nisl. Sed hendrerit mi ut velit luctus consequat. Proin egestas enim placerat velit scelerisque dictum. Vivamus sodales mi felis.\r" +
    "Nam elit metus, bibendum dignissim dignissim id, lobortis ut metus. Ut feugiat elit sit amet magna porttitor semper. Suspendisse ultricies turpis nisl, vitae sollicitudin lectus. Phasellus urna neque, lacinia ac pellentesque ac, viverra eu enim. Integer aliquet erat et mauris volutpat blandit. Duis in odio vitae magna sollicitudin hendrerit id eu augue. Phasellus sit amet gravida velit. Maecenas vitae dolor sed nisl rutrum sodales.\r" +
    "Suspendisse massa erat, hendrerit sit amet porttitor id, scelerisque sed leo. Duis non nisi tortor. Sed porta felis ac sem consequat eget accumsan neque laoreet. Etiam viverra leo vitae orci vehicula molestie tincidunt dolor consectetur. Duis feugiat auctor tincidunt. Curabitur eget vehicula quam. Cras non elit at ligula pretium mollis condimentum nec lorem. Praesent id orci turpis, congue imperdiet quam. Nullam eleifend sagittis sapien, eu ullamcorper massa accumsan eget. Curabitur sed turpis ut diam mollis tincidunt eu vitae lacus. In et arcu convallis nisi hendrerit luctus. Suspendisse tristique neque iaculis mauris dictum non rutrum tellus egestas. Nullam venenatis pellentesque nisl nec dapibus. Donec vestibulum magna nec est euismod at malesuada elit pretium.\r" +
    "Nullam aliquam, ipsum vel lacinia rutrum, lorem orci dictum lorem, nec auctor tellus nisi eu ligula. Pellentesque pellentesque elit mauris. In vulputate lacinia ipsum, id venenatis purus lobortis quis. Nam ac tempus augue. In scelerisque eleifend fringilla. Proin elit purus, vestibulum non tincidunt a, luctus quis dui. Ut suscipit aliquet magna id egestas. Mauris ac mollis eros.\r" +
    "Sed lectus metus, consequat a porttitor id, lobortis eu diam. Pellentesque accumsan lectus eu lectus consectetur laoreet. Nunc nec enim vitae metus dignissim consectetur faucibus ac urna. Mauris vel erat nisl. Sed pharetra pharetra enim nec imperdiet. Pellentesque imperdiet, turpis eget porttitor dignissim, elit diam porta leo, sed egestas tellus dui vitae dui. Ut volutpat varius turpis. Duis bibendum rutrum massa, ut vehicula dolor accumsan sit amet.\r" +
    "Maecenas tempor risus vehicula dolor consequat pulvinar sodales nunc aliquet. Nunc sit amet eros in velit semper malesuada in et nisi. Nunc nec justo ipsum. Curabitur nisl metus, fringilla quis condimentum vitae, imperdiet quis justo. Nam leo lectus, accumsan sit amet viverra at, bibendum et dui. Morbi lacinia tortor a nisl rutrum lacinia. Donec sed odio est, at facilisis ante. Etiam vehicula nulla sit amet felis molestie euismod. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Morbi viverra ligula eget leo tincidunt a hendrerit ligula fermentum. Mauris non neque adipiscing urna lobortis sollicitudin. Etiam tempor quam vel velit rhoncus eu aliquam erat cursus. Phasellus aliquet, lorem id lacinia varius, eros nulla ultricies enim, ac tempor nibh augue a eros. Aenean vel magna odio, ut imperdiet augue. Fusce eget lacus a velit consectetur tincidunt nec et dui. Lorem ipsum dolor sit amet, consectetur adipiscing elit. ";
  var sentences = lipsumContent.split("\r");
  for(var i = 0; i < lines; i++){
    output += '<p>' + sentences[Math.floor((Math.random()*sentences.length))] + '</p>';
  }
  return output;
}
function cke_tpl_guid() {
  var S4 = function() {
    return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
  };
  return (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
}
function cke_tpl_img(url, classes) {
  var url = url || 'http://placehold.it/900x450&text=IMAGE';
  var classes = classes || 'image-style';
  return '<figure><img class="' + classes + '" src="' + url + '" /><figcaption>Image Caption</figcaption></figure>';
}
function cke_tpl_section(content) {
  return '<section class="row margin-top margin-bottom">' + content + '</section>';
}
function cke_tpl_span(content, span) {
  var content = content || '<p>Insert content here</p>';
  var span = span || 12;
  return '<div class="col-sm-' + span + '">' + content + '</div>';
}
function cke_tpl_header(content) {
  var content = content || '<h3>Insert title here</h3>';
  return '<header>' + content + '</header>';
}
function cke_tpl_footer(content, span) {
  var content = content || '<p>Insert content here</p>';
  var span = span || 12;
  return '<footer class="col-sm-' + span + '">' + content + '</footer>';
}
function cke_tpl_aside(content, span) {
  var content = content || '<p>Insert content here</p>';
  var span = span || 12;
  return '<aside class="col-sm-' + span + '">' + content + '</aside>';
}
function cke_tpl_tabs(position) {
  if (position === undefined || position === null) {
    position = '';
  }
  var tabs = parseInt(prompt('Number of tabs you wish to create:','3'), 10);
  if (tabs === undefined || tabs === null || tabs === 0) {
    tabs = 3;
  }
  // Generate tab markup.
  var tabOutput = '';
  tabOutput = tabOutput + '  <ul class="nav nav-tabs" id="' + cke_tpl_guid() +'">';
  var tabIds = [];
  for (var i = 0; i < tabs; i++) {
    var id = cke_tpl_guid();
    tabIds.push(id);
    tabOutput = tabOutput + '    <li' + (i === 0 ? ' class="active"' : '') + '><a data-toggle="tab" href="#' + id +'">Tab ' + (i + 1) + '</a></li>';
  }
  tabOutput = tabOutput + '  </ul>';
  // Generate pane markup.
  var paneOutput = '';
  paneOutput = paneOutput + '  <div class="tab-content" id="myTabContent">';
  for (i = 0; i < tabs; i++) {
    paneOutput = paneOutput + '    <div class="tab-pane' + (i === 0 ? ' active' : '') + '" id="' + tabIds[i] + '">' + cke_tpl_text() + '</div>';
  }
  paneOutput = paneOutput + '  </div>';
  // Construct the full markup.
  var output = '<div class="style-tabs tabbable' + (position.length ? ' tabs-' + position : '') +'">';
  if (position === 'below') {
    output = output + paneOutput + tabOutput;
  }
  else {
    output = output + tabOutput + paneOutput;
  }
  output = output + '</div>';
  return output;
}
function cke_tpl_collapse(accordion) {
  accordion = accordion ? true : false;
  var parentId = cke_tpl_guid();
  var sections = parseInt(prompt('Number of collapsible sections you wish to create:','3'), 10);
  if (sections === undefined || sections === null || sections === 0) {
    sections = 3;
  }
  var output = '<div class="panel-group" id="' + parentId + '" class="accordion">';
  for (i = 0; i < sections; i++) {
    var sectionId = cke_tpl_guid();
    output = output + '  <section class="panel panel-default">';
    output = output + '    <header class="panel-heading">';
    output = output + '      <h4 class="panel-title">';
    output = output + '        <a href="#' + sectionId + '"' + (accordion ? ' data-parent="#' + parentId +'"' : '') + ' data-toggle="collapse" class="accordion-toggle">Section ' + (i + 1) + '</a>';
    output = output + '      </h4>';
    output = output + '    </header>';
    output = output + '    <div id="' + sectionId + '" class="panel-collapse collapse' + (i === 0 ? ' in' : '') + '"><div class="panel-body">' + cke_tpl_text() + '</div></div>';
    output = output + '  </section>';
  }
  output = output + '</div>';
  return output;
}
// Adds default templates to choose from.
CKEDITOR.addTemplates('default', {
  imagesPath: Drupal.settings.bootstrap_ckeditor.path + '/ckeditor/images/',
  templates: [
    {
      title: 'Section',
      image: 'section.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A section of content containing a heading with title.',
      html: cke_tpl_section( cke_tpl_header() + cke_tpl_text(3) )
    },
    {
      title: 'Section - Left Image',
      image: 'section-left.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A section of content containing a heading with title. An image floats to the left of the content. Content wraps around the image.',
      html: cke_tpl_section( cke_tpl_img(null, 'image-left-style') + cke_tpl_header() + cke_tpl_text(3) )
    },
    {
      title: 'Section - Right Image',
      image: 'section-right.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A section of content containing a heading with title. An image floats to the right of the content. Content wraps around the image.',
      html: cke_tpl_section( cke_tpl_img(null, 'image-right-style') + cke_tpl_header() + cke_tpl_text(3) )
    },
    {
      title: 'Section - Left Image (no wrap)',
      image: 'section-left-no-wrap.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A section of content containing a heading with title. An image floats to the left of the content. Content does not wrap around the image.',
      html: cke_tpl_section( cke_tpl_img(null, 'image-left-style') + '<div class="no-wrap">' + cke_tpl_header() + cke_tpl_text(3) + '</div>' )
    },
    {
      title: 'Section - Right Image (no wrap)',
      image: 'section-right-no-wrap.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A section of content containing a heading with title. An image floats to the right of the content. Content does not wrap around the image.',
      html: cke_tpl_section( cke_tpl_img(null, 'image-right-style') + '<div class="no-wrap">' + cke_tpl_header() + cke_tpl_text(3) + '</div>' )
    },
    {
      title: 'Row',
      image: 'row.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A container row.',
      html: cke_tpl_section( cke_tpl_text(4) )
    },
    {
      title: 'Row - Two Columns (50/50)',
      image: 'row-50-50.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A row of two equally sized columns with a margin in-between them. NOTE: Columns are responsive. Widths, margins and position will vary upon the device it is viewed on.',
      html: cke_tpl_section( cke_tpl_span(null, 6) + cke_tpl_span(null, 6) )
    },
    {
      title: 'Row - Two columns (33/66)',
      image: 'row-33-66.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A row of two columns smaller one on the left with a margin in-between them. NOTE: Columns are responsive. Widths, margins and position will vary upon the device it is viewed on.',
      html: cke_tpl_section( cke_tpl_span(null, 4) + cke_tpl_span(null, 8) )
    },
    {
      title: 'Row - Two columns (66/33)',
      image: 'row-66-33.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A row of two columns smaller one on the right with a margin in-between them. NOTE: Columns are responsive. Widths, margins and position will vary upon the device it is viewed on.',
      html: cke_tpl_section( cke_tpl_span(null, 8) + cke_tpl_span(null, 4) )
    },
    {
      title: 'Row - Three columns (33/33/33)',
      image: 'row-33-33-33.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A row of three equally sized columns with margins in-between them. NOTE: Columns are responsive. Widths, margins and position will vary upon the device it is viewed on.',
      html: cke_tpl_section( cke_tpl_span(null, 4) + cke_tpl_span(null, 4) + cke_tpl_span(null, 4) )
    },
    {
      title: 'Row - Four columns (25/25/25/25)',
      image: 'row-25-25-25-25.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'A row of four equally sized columns with margins in-between them. NOTE: Columns are responsive. Widths, margins and position will vary upon the device it is viewed on.',
      html: cke_tpl_section( cke_tpl_span(null, 3) + cke_tpl_span(null, 3) + cke_tpl_span(null, 3) + cke_tpl_span(null, 3) )
    },
    {
      title: 'Tabs - top',
      image: 'tabs-top.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'Bring life to your content with toggable tabs.',
      html: function() { return cke_tpl_tabs(); }
    },
    {
      title: 'Tabs - bottom',
      image: 'tabs-bottom.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'Bring life to your content with toggable tabs.',
      html: function() { return cke_tpl_tabs('below'); }
    },
    {
      title: 'Tabs - left',
      image: 'tabs-left.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'Bring life to your content with toggable tabs.',
      html: function() { return cke_tpl_tabs('left'); }
    },
    {
      title: 'Tabs - right',
      image: 'tabs-right.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'Bring life to your content with toggable tabs.',
      html: function() { return cke_tpl_tabs('right'); }
    },
    {
      title: 'Collapsible Sections',
      image: 'collapse.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'The collapsible state of each section can be toggled independently.',
      html: function() { return cke_tpl_collapse(); }
    },
    {
      title: 'Collapsible Sections (accordion style)',
      image: 'collapse-accordion.png?' + Drupal.settings.bootstrap_ckeditor.query,
      description: 'The collapsible state of each section is determined by the currently viewed section. Only one section can be visible at a time.',
      html: function() { return cke_tpl_collapse(true); }
    }
  ]
});
