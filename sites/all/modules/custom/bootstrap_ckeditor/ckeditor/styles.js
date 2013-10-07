// Add default styles to CKEditor.
CKEDITOR.stylesSet.add('default', [
	// Block Styles
	{ name : 'Normal'		, element : 'p' },
	{ name : 'Block'			, element : 'div' },
	{ name : 'Header'			, element : 'header' },
	{ name : 'Footer'			, element : 'footer' },
	{ name : 'Heading 2'		, element : 'h2' },
	{ name : 'Heading 3'		, element : 'h3' },
	{ name : 'Heading 4'		, element : 'h4' },
	{ name : 'Heading 5'		, element : 'h5' },
	{ name : 'Heading 6'		, element : 'h6' },
	{ name : 'Preformatted Text', element : 'pre' },
	{ name : 'Address'			, element : 'address' },
  // Inline Styles
	{ name : 'Big'				, element : 'big' },
	{ name : 'Small'			, element : 'small' },
	{ name : 'Typewriter'		, element : 'tt' },
	{ name : 'Computer Code'	, element : 'code' },
	{ name : 'Cited Work'		, element : 'cite' },
	// Object Styles
	{
		name : 'Image Left',
		element : 'img',
		attributes :
		{
			'class' : 'image-left',
		}
	},
	{
		name : 'Image Right',
		element : 'img',
		attributes :
		{
			'class' : 'image-right',
		}
	},
	{
		name : 'Styled Image',
		element : 'img',
		attributes :
		{
			'class' : 'image-style',
		}
	},
	{
		name : 'Styled Image Left',
		element : 'img',
		attributes :
		{
			'class' : 'image-left-style',
		}
	},
	{
		name : 'Styled Image Right',
		element : 'img',
		attributes :
		{
			'class' : 'image-right-style',
		}
	}
]);
