/**
 * Created by ling on 2015/3/3.
 */
hljs.configure({useBR: false});
hljs.configure({tabReplace: '    '});
hljs.initHighlightingOnLoad();

marked.setOptions({
    renderer: new marked.Renderer(),
    gfm: true,
    emoji: true,
    tables: true,
    breaks: false,
    pedantic: false,
    sanitize: true,
    smartLists: true,
    smartypants: false,
    highlight: function (code, lang) {
        try {
            if (lang)
                return hljs.highlight(lang, code).value;
        } catch (e) {
            return hljs.highlightAuto(code).value;
        }
        return hljs.highlightAuto(code).value;
    }
});
