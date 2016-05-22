(function($) {
$("#tab-9-content").markdown({
	language: 'zh',
	fullscreen: {
		enable: true,
		debounce: 500
	},
	resize: 'vertical',
	localStorage: 'md',
	imgurl: '/c/imgUpload',
	base64url: '/c/imgUpload',
	flowChart : true
});

$("#tab-9-brief").markdown({
	language: 'zh',
	fullscreen: {
		enable: true
	},
	resize: 'vertical',
	localStorage: 'md',
	imgurl: '/c/imgUpload',
	base64url: '/c/imgUpload'
});
})(jQuery);