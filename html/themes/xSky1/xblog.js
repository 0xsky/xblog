
$(document).ready(function(){
    console.log("xblog main");
    config();
    post_list(0);
    classify();
    links();
    postview();
});


function post_list(pageid)
{
    pageid = (pageid<=0)?1:pageid;
    var index  = "/postlist?page="+pageid;
    $.ajax({
        type: "get",
        url: index,
        dataType: "json", 
        success: function(data){
        $('#postlist').html("");
        
        for(var i = 0; i<data.length; i++) {
            var it = data[i];
            var html ='';
            html += '<DIV id="post-'+it.ID+'" pid="'+it.ID+'" class="post hentry category-dev category-event">';
            html += '<H2><A target="_blank" title="'+it.post_title+'" href="javascript:onPost('+it.ID+', \'\');" rel="bookmark">'+it.post_title+'</A></H2>';
            html += '<SMALL>'+it.post_date+' </SMALL>';
            html += '<DIV class="entry">';
            html += it.post_brief;
            html += '<p class="read-more"><a href="javascript:onPost('+it.ID+', \'\');">More...</a></p>';
            html += '</DIV>';
            html += '</DIV>';
            
            $('#postlist').append(html);
        }  
        $("p.read-more a").html("\u003Cstrong style=\"font-size:15px;\"\u003E阅读全文...\u003C/strong\u003E") 
        var last_pageid = ((pageid-1)<=0)?1:(pageid-1);
        var next_pageid = pageid+1;
        
        $('.alignleft').html('<a href="javascript:post_list('+ last_pageid + ');">上一页</a>');
        $('.alignright').html('<a href="javascript:post_list('+ next_pageid + ');">下一页</a>');
        scroll(0,0);
    }
    });
    //获取分页数据
    $.getJSON("/paging", function (data) {
        console.log(data[0].page+"|"+data[0].total);
        var page = data[0].page;
        var total = data[0].total;
        var cnt = Math.ceil(total/page);
        $('.page').html("");
        for(var i = 1; i<cnt+1; i++) {
            $('.page').append('<div><a href="javascript:post_list('+ i + ');">'+i+'</a></div>');
        }
    });
}

function showPost(it)
{
    var html ='';
    html += '<DIV id="post-'+it.ID+'" pid="'+it.ID+'" class="post hentry category-dev category-event">';
    html += '<H2><A target="_blank" title="'+it.post_title+'" href="javascript:onPost('+it.ID+',\'\');" rel="bookmark">'+it.post_title+'</A></H2>';
    html += '<SMALL>'+it.post_date+' </SMALL>';
    html += '<DIV class="entry">';
    html += it.post_content;
    html += '</DIV>';
    html += '<P class="postmetadata">作者:xsky | '+it.classify_name+' | </P>';
    html += '</DIV>';

    $('#postlist').html(html);
}

function onPost(id, pos)
{
    console.log('onPost '+id);
    var url  = "/post?id="+id+"&pos="+pos;
    $.ajax({
        type: "get",
        url: url,
        dataType: "json", 
        success: function(data){
        var it = data[0];
        showPost(it);
        
        $('.alignleft').html('<a href="javascript:onPost('+it.ID+', \'last\');">上一篇</a>');
        $('.alignright').html('<a href="javascript:onPost('+it.ID+', \'next\');">下一篇</a>');
        
    }
    });
}

function classify()
{
    var url  = "/classify";
    $.ajax({
        type: "get",
        url: url,
        dataType: "json", 
        success: function(data){
        var html ='';
        for(var i = 0; i<data.length; i++) {
            var it = data[i];
            html += '<li><a href="javascript:catalog('+it.id+')";>'+it.classify_name+'<span class="article-nums"> ('+it.cnt+')</span></a></li>';
            $('#categories').html(html);
        }  
    }
    });
}

function links()
{
    var url  = "/links";
    $.ajax({
        type: "get",
        url: url,
        dataType: "json", 
        success: function(data){
        var html ='';
        $('#widget_link').html("");
        for(var i = 0; i<data.length; i++) {
            var it = data[i];
            html = '<li><a href="'+it.link_url+'" target="_blank">'+it.link_name+'</a></li>';
            $('#widget_link').append(html);
        }  
    }
    });
}

function config()
{
    var url  = "/config";
    $.ajax({
        type: "get",
        url: url,
        dataType: "json", 
        success: function(data){
            var config = data[0];
            var html ='';
            // 这里的逻辑需要实现 
            $('#blog_title').html(config.title);
            $('#blog_description').html(config.description);
        }
    });
}

function postview()
{
    var url  = "/postview";
    $.ajax({
        type: "get",
        url: url,
        dataType: "json", 
        success: function(data){
        var html ="";
        $('#postview').html("");
        for(var i = 0; i<data.length; i++) {
            var it = data[i];
            html = '<li><a href="javascript:onPost('+it.ID+',\'\');" target="_blank">'+it.post_title+'</a></li>';
            $('#postview').append(html);
        }  
    }
    });
}

function catalog(id)
{
    
    $.ajax({
        type: "get",
        url: "/catalog?id="+id,
        dataType: "json", 
        success: function(data){
        $('#postlist').html("");
        
        for(var i = 0; i<data.length; i++) {
            var html ='';
            var it = data[i];
            html += '<DIV id="post-'+it.ID+'" pid="'+it.ID+'" class="post hentry category-dev category-event">';
            html += '<H2><A target="_blank" title="'+it.post_title+'" href="javascript:onPost('+it.ID+',\'\');" rel="bookmark">'+it.post_title+'</A></H2>';
            html += '<SMALL>'+it.post_date+' </SMALL>';
            html += '<DIV class="entry">';
            html += it.post_brief;
            html += '<p class="read-more"><a href="javascript:onPost('+it.ID+', \'\');">More...</a></p>';
            html += '</DIV>';
            html += '</DIV>';
            $('#postlist').append(html);
        }  
        
        $("p.read-more a").html("\u003Cstrong style=\"font-size:15px;\"\u003E阅读全文...\u003C/strong\u003E") 
        $('.alignleft').html('<a href="javascript:onPost_last('+it.ID+');">上一页</a>');
        $('.alignright').html('<a href="javascript:onPost_next('+it.ID+');">下一页</a>');
    }
    });
}
