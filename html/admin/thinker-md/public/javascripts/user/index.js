/**
 * Created by ling on 2015/3/31.
 */
$("textarea[data-provide='markdown']").markdown({
    language: 'zh',
    fullscreen: {
        enable: true
    },
    resize: 'vertical',
    localStorage: 'md',
    imgurl: '/c/imgUpload',
    base64url: '/c/imgUpload'
});
