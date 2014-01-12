
var   post_pageid=0;
var   comment_pageid=0;

jQuery(function(){
    $(document).ready(function(){ 
        $(".tdRevert").click(tdRevert); 
        $(".tdDelete").click(tdDelete); 
        $(".tdUpdate").click(tdUpdate); 
        $("#BTCMD").click();
        $("#BTCMD").click(function(){
            $.ajax({
            url: '/shell',
            type: 'post',
            data: 'cmd='+$('#cmd').val(),
            success: function(result) {
            $("#cmdshell").html(result);
            }
            });
        });
        
        $(function(){
            $("#cmd").bind('keypress',function(event){
                if(event.keyCode == "13")    
                {
                    $("#BTCMD").click();
                }
            });
        });

    }); 
    
    // 更新友情链接
    $("#BTlinkupdate").live("click",function(){
        var updateURL  = "/links?action=update";
        var strID      = $(this).parents("tr").find(".id").text();
        var strURL     = $(this).parents("tr").find("input")[0].value;
        var strComment = $(this).parents("tr").find("input")[1].value;
        
        $.ajax({
            type: "POST",
            url: updateURL,
            data: {"id":strID,"url":strURL,"name":strComment},
            success: function(msg){
                alert(msg);
        }
        });
    });
    
    $("#BTlinkupadd").live("click",function(){
        var addURL  = "/links?action=add";
        var strURL     = $(this).parents("tr").find("input")[0].value;
        var strComment = $(this).parents("tr").find("input")[1].value;
        
        $.ajax({
            type: "POST",
            url: addURL,
            data: {"url":strURL,"name":strComment},
            success: function(msg){
                alert(msg);
        }
        });
    });
    
    // 更新分类
    $("#BTcatalogupdate").live("click",function(){
        var updateURL  = "/catalogset?action=update";
        var strID      = $(this).parents("tr").find(".id").text();
        var strClassfyId     = $(this).parents("tr").find("input")[0].value;
        var strClassfyName   = $(this).parents("tr").find("input")[1].value;
        var strClassfyStatus = $(this).parents("tr").find("input")[2].value;
        
        $.ajax({
            type: "POST",
            url: updateURL,
            data: {"id":strID,"classfyid":strClassfyId,
                    "classfyname":strClassfyName,
                    "classfystatus":strClassfyStatus},
            success: function(msg){
                alert(msg);
        }
        });
    });
    
    $("#BTcatalogadd").live("click",function(){
        var updateURL  = "/catalogset?action=add";
        var strID      = $(this).parents("tr").find(".id").text();
        var strClassfyId     = $(this).parents("tr").find("input")[0].value;
        var strClassfyName   = $(this).parents("tr").find("input")[1].value;
        var strClassfyStatus = $(this).parents("tr").find("input")[2].value;
        
        $.ajax({
            type: "POST",
            url: updateURL,
            data: {"id":strID,"classfyid":strClassfyId,
                    "classfyname":strClassfyName,
                    "classfystatus":strClassfyStatus},
            success: function(msg){
                alert(msg);
        }
        });
    });
    // 删除友情链接    
    $("#BTlinkdelete").live("click",function(){
        var URL = "/links?action=delete";
        var strID = $(this).parents("tr").find(".id").text();
        $.ajax({
            type: "POST",
            url: URL,
            data: "id="+strID,
            success: function(msg){
                alert(msg);
        }
        });
    });
    // 还原文章
    $("#BTpostRevert").live("click",function(){
        var updateURL = "/postmanager?action=revert";
        var id = $(this).parents("tr").find(".id").text();
        alert(id);
        $.ajax({
            type: "POST",
            url: updateURL,
            data: "id=" + id,
            success: function(msg){
                alert(msg);
        }
        });
    });
    // 删除文章
    $("#BTpostDelete").live("click",function(){
        var updateURL = "/postmanager?action=delete";
        var id = $(this).parents("tr").find(".id").text();
        $.ajax({
            type: "POST",
            url: updateURL,
            data: "id=" + id,
            success: function(msg){
                alert(msg);
        }
        });
    });
    
    // 删除留言
    $("#BTCommentDelete").live("click",function(){
        var updateURL = "/comments?action=delete";
        var id = $(this).parents("tr").find(".id").text();
        
        $.ajax({
            type: "POST",
            url: updateURL,
            data: "id=" + id,
            success: function(msg){
                alert(msg);
        }
        });
    });
    
    // 站点配置更新
    $("#BTwebconfigUpdate").live("click",function(){
        var updateURL = "/siteconfig?action=update";
        var strKey = $(this).parents("tr").find("input")[0].id;
        var strValue= $(this).parents("tr").find("input")[0].value;
        $.ajax({
            type: "POST",
            url: updateURL,
            data: {"id":strKey,"value":strValue},
            success: function(msg){
                alert(msg);
        }
        });
    });
    
})
  
function tdRevert(){ 
    var id = $(this).parents("tr").find(".id").text();
    $(this).parents("tr").find(".flag").html("publish");
    $.ajax({
        type: "POST",
        url: "/post_manager?action=revert",
        data: "id=" + id ,
        success: function(msg){
            alert(msg);
    }
    });
}

function tdDelete(){ 
    var id = $(this).parents("tr").find(".id").text();
    $(this).parents("tr").find(".flag").html("delete");
    $.ajax({
        type: "POST",
        url: "/post_manager?action=delete",
        data: "id=" + id ,
        success: function(msg){
            alert(msg);
    }
    });
}


function tdUpdate(){ 
    var strKey = $(this).parents("tr").find("input")[0].id;
    var strValue= $(this).parents("tr").find("input")[0].value;
    //$(this).parents("tr").find(".flag").html("delete");
    $.ajax({
        type: "POST",
        url: updateURL,
        //data: "id="+strKey+ "&value="+strValue ,
        data: {"id":strKey,"value":strValue},
        success: function(msg){
            alert(msg);
    }
    });
}

function GetLinkList(){
    $.ajax({
        url: "/links?action=view",
        dataType: "jsonp",
        jsonp:"callbackparam",
        jsonpCallback:"jsonpcallback",
        error: function(jqXHR, textStatus, errorThrown) {
            if (textStatus == "error") {
                alert(textStatus + " : " +errorThrown);
            } else {
                alert(textStatus);
            }
        },
        success: function(json, textStatus, jqXHR) {
            $("#links").html("");
            var n = json.result.xdata.length;
            var strhtml = "<table><tr><td>id</td><td>链接地址</td><td>显示</td><td>操作</td></tr>";
            
            $.each(json.result.xdata ,function(i){
                var js = json.result.xdata[i];
                strhtml += "<tr><td class='id'>"+js[0]+"</td>";
                
                strhtml += "<td><input class='url' size='40' type='text'  value='"+js[1]+"' /></td>";
                strhtml += "<td><input class='comment' size='40' type='text'  value='"+js[2]+"' /></td>";
                strhtml += "<td><input type='submit' value='update' id='BTlinkupdate'></td>";
                strhtml += "<td><input type='submit' value='delete' id='BTlinkdelete'></td></tr>";
             
            });
            strhtml+="</table>";
            $("#links").append(strhtml);
            
            strhtml+="<br>添加链接地址";
            var strhtml = "<table><tr><td>id</td><td>链接地址</td><td>显示</td><td>操作</td></tr>";
            
            strhtml += "<tr><td class='id'></td>";
            strhtml += "<td><input class='url' size='40' type='text'  value='' /></td>";
            strhtml += "<td><input class='comment' size='40' type='text'  value='' /></td>";
            strhtml += "<td><input type='submit' value='添加' id='BTcatalogadd'></td>";
             
            strhtml+="</table>";
            $("#links").append(strhtml);
        }
    });
}

function GetAdminUser(){
    $.ajax({
        url: "/user?action=view",
        dataType: "jsonp",
        jsonp:"callbackparam",
        jsonpCallback:"jsonpcallback",
        error: function(jqXHR, textStatus, errorThrown) {
            if (textStatus == "error") {
                alert(textStatus + " : " +errorThrown);
            } else {
                alert(textStatus);
            }
        },
        success: function(json, textStatus, jqXHR) {
            $("#adminuser").html("");
            var n = json.result.xdata.length;
            var strhtml = "<table><tr><td>id</td><td>登陆名</td><td>密码</td><td>显示昵称</td><td>邮箱地址</td><td>操作</td></tr>";
            
            $.each(json.result.xdata ,function(i){
                var js = json.result.xdata[i];
                strhtml += "<tr><td class='id'>"+js[0]+"</td>";
                
                strhtml += "<td><input class='userlogin' size='20' type='text'  value='"+js[1]+"' /></td>";
                strhtml += "<td><input class='userpass' size='40' type='text'  value='"+js[2]+"' /></td>";
                strhtml += "<td><input class='nickname' size='20' type='text'  value='"+js[3]+"' /></td>";
                strhtml += "<td><input class='email' size='30' type='text'  value='"+js[4]+"' /></td>";
                strhtml += "<td><input type='submit' value='更新' id='BTadminuser'></td>";
             
            });
            strhtml+="</table>";
            $("#adminuser").append(strhtml);
           
        }
    });
}

function GetCatalogList(){
    $.ajax({
        url: "/catalogset?action=view",
        dataType: "jsonp",
        jsonp:"callbackparam",
        jsonpCallback:"jsonpcallback",
        error: function(jqXHR, textStatus, errorThrown) {
            if (textStatus == "error") {
                alert(textStatus + " : " +errorThrown);
            } else {
                alert(textStatus);
            }
        },
        success: function(json, textStatus, jqXHR) {
            $("#catalogset").html("");
            var n = json.result.xdata.length;
            var strhtml = "<table><tr><td>id</td><td>排序ID</td><td>分类名称</td><td>状态</td><td>操作</td></tr>";
            var max_id;
            
            $.each(json.result.xdata ,function(i){
                var js = json.result.xdata[i];
                strhtml += "<tr><td class='id'>"+js[0]+"</td>";
                
                strhtml += "<td><input class='catalogid' size='1' type='text'  value='"+js[1]+"' /></td>";
                strhtml += "<td><input class='catalogname' size='40' type='text'  value='"+js[2]+"' /></td>";
                strhtml += "<td><input class='catalogstatus' size='1' type='text'  value='"+js[3]+"' /></td>";
                strhtml += "<td><input type='submit' value='update' id='BTcatalogupdate'></td>";
                max_id = parseInt(js[1]) +1;
            });
            strhtml+="</table>";
            $("#catalogset").append(strhtml);
 
            strhtml+="<br>添加文章分类";
            var strhtml = "<br><table><tr><td>id</td><td>排序ID</td><td>分类名称</td><td>状态</td><td>操作</td></tr>";
            
            strhtml += "<tr><td class='id'></td>";
            strhtml += "<td><input class='catalogid' size='1' type='text'  value='"+max_id+"' /></td>";
            strhtml += "<td><input class='catalogname' size='40' type='text'  value='分类┊名称' /></td>";
            strhtml += "<td><input class='catalogstatus' size='1' type='text'  value='0' /></td>";
            strhtml += "<td><input type='submit' value='添加' id='BTcatalogadd'></td>";
             
            strhtml+="</table>";
            $("#catalogset").append(strhtml);
        }
    });
}

function GetCatalogSelect(){
    $.ajax({
        url: "/catalogset?action=viewselect",
        dataType: "jsonp",
        jsonp:"callbackparam",
        jsonpCallback:"jsonpcallback",
        error: function(jqXHR, textStatus, errorThrown) {
            if (textStatus == "error") {
                alert(textStatus + " : " +errorThrown);
            } else {
                alert(textStatus);
            }
        },
        success: function(json, textStatus, jqXHR) {
            $("#select-classify").html("");
            var strhtml = "分类: <select name='classify'>";
            $.each(json.result.xdata ,function(i){
                var js = json.result.xdata[i];
                strhtml += "<option value='"+ js[0] +"'>"+ js[1]+"</option>";
            });
            strhtml+="</select>";
            $("#select-classify").append(strhtml);
        }
    });
}

function GetPostList(){
        $.ajax({
            url: "/admin?action=postlist&page="+post_pageid,
            dataType: "jsonp",
            jsonp:"callbackparam",
            jsonpCallback:"jsonpcallback",
            error: function(jqXHR, textStatus, errorThrown) {
                if (textStatus == "error") {
                    alert(textStatus + " : " +errorThrown);
                } else {
                    alert(textStatus);
                }
            },
            success: function(json, textStatus, jqXHR) {
                $("#posttotal").html("");
                $("#postlist").html("");
                var n = json.result.xdata.length;
                var Total = "文章总数:" + n;
                var strhtml = '<table border="1"><tr>'
                +'<td>编号</td>'
                +'<td>分类</td>'
                +'<td>作者</td>'  
                +'<td>时间</td>'  
                +'<td>标题</td>'  
                +'<td>状态</td>'  
                +'<td>编辑</td>'  
                +'<td>操作</td></tr>';
 
                $.each(json.result.xdata ,function(i){
                    var js = json.result.xdata[i];
                    strhtml += "<tr><td class='id'>"+js[0]+"</td>";
                    strhtml += "<td>"+js[1]+"</td>";
                    strhtml += "<td>"+js[2]+"</td>";
                    strhtml += "<td>"+js[3]+"</td>";
                    strhtml += "<td>"+js[4]+"</td>";
                    strhtml += "<td>"+js[5]+"</td>";
                    strhtml += "<td><a href='/admin?action=editpost&id="+js[0]+"' target='mainFrame'>[edit]</a></td>";
                    //strhtml += "<td><a href=javascript:void window.open('/admin?action=editpost&id="+js[0]+ "','newwindow', 'height=100, width=400, top=0, left=0, toolbar=no, menubar=no, scrollbars=no, resizable=no,location=no, location=no, status=no')>[edit]</a></td>";
                    //strhtml += "<td><input type='submit' value='update' id='tdUpdate'></td></tr>";
                    strhtml += "<td><input type='submit' value='revert' id='BTpostRevert'><input type='submit' value='delete' id='BTpostDelete'></td></tr>";
                });
                strhtml+="</table>";
                $("#postlist").append(strhtml);
                $("#posttotal").append(Total);
                
                strhtml = "<div class='pagination'>分页：[" + post_pageid + "]"
                strhtml += " <input type='button' class='btn' value='上一页' onclick='GetCommentListPageLast()' />"
                
                if(n==30)
                {
                    strhtml += " <input type='button' class='btn' value='下一页' onclick='GetCommentListPageNext()' />"
                }

                strhtml += "</div><div><br><br><br></div>"
                $("#postlist").append(strhtml);
            }
        });
}


function GetPostListPageNext(){ 
    post_pageid++;
    GetPostList();
}

function GetPostListPageLast(){ 
    post_pageid--;
    if (post_pageid<=0)
        post_pageid=0;
    GetPostList();
}

function GetCommentListPageNext(){ 
    comment_pageid++;
    GetCommentList();
}

function GetCommentListPageLast(){ 
    comment_pageid--;
    if (comment_pageid<=0)
        comment_pageid=0;
    GetCommentList();
}

// 取得留言与讨论列表
function GetCommentList(){
        $.ajax({
            url: "/comments?action=view&page="+comment_pageid,
            dataType: "jsonp",
            jsonp:"callbackparam",
            jsonpCallback:"jsonpcallback",
            error: function(jqXHR, textStatus, errorThrown) {
                if (textStatus == "error") {
                    alert(textStatus + " : " +errorThrown);
                } else {
                    alert(textStatus);
                }
            },
            success: function(json, textStatus, jqXHR) {
                $("#commentlist").html("");
                $("#commenttotal").html("");
                var n = json.result.xdata.length;
                var Total = "留言统计:" + n;
                var strhtml = '<table border="1"><tr>'
                +'<td>文章ID</td>'
                +'<td>文章标题</td>'
                +'<td>留言ID</td>'  
                +'<td>访客名</td>'  
                +'<td>访客邮箱</td>'  
                +'<td>留言时间</td>'   
                +'<td>留言内容</td>'
                +'<td>操作</td></tr>';
                
                $.each(json.result.xdata ,function(i){
                    var js = json.result.xdata[i];
                    strhtml += "<tr><td>"+js[0]+"</td>";
                    strhtml += "<td>"+js[1]+"</td>";
                    strhtml += "<td  class='id'>"+js[2]+"</td>";
                    strhtml += "<td>"+js[3]+"</td>";
                    strhtml += "<td>"+js[4]+"</td>";
                    strhtml += "<td>"+js[5]+"</td>";
                    strhtml += "<td>"+js[6]+"</td>";
                    strhtml += "<td><input type='submit' value='delete' id='BTCommentDelete'></td></tr>";
                });
                strhtml+="</table>";
                $("#commentlist").append(strhtml);
                $("#commenttotal").append(Total);
                
                strhtml = "<div class='pagination'>分页：[" + comment_pageid + "]"
                strhtml += " <input type='button' class='btn30' value='上一页' onclick='GetPostListPageLast()' />"
                
                if(n==30)
                {
                    strhtml += " <input type='button' class='btn30' value='下一页' onclick='GetPostListPageNext()' />"
                }

                strhtml += "</div><div><br><br><br></div>"
                $("#commentlist").append(strhtml);
            }
        });
}

function GetWebConfig(){

        $.ajax({
            url: "/siteconfig?action=mainpage",
            dataType: "jsonp",
            jsonp:"callbackparam",
            jsonpCallback:"jsonpcallback",
            error: function(jqXHR, textStatus, errorThrown) {
                if (textStatus == "error") {
                    alert(textStatus + " : " +errorThrown);
                } else {
                    alert(textStatus);
                }
            },
            success: function(json, textStatus, jqXHR) {
                $("#webconfig").html("");
                var n = json.result.xdata.length;
				var strhtml = "<table><tr><td>id</td><td>name</td><td>value</td><td>method</td></tr>";
 
                $.each(json.result.xdata ,function(i){
                    var js = json.result.xdata[i];
					strhtml += "<tr><td>"+js[0]+"</td>";
                    strhtml += "<td>"+js[1]+"</td>";
                    strhtml += "<td><input id='"+js[0]+"' size='80' type='text' name='"+js[1]+"' value='"+js[2]+"' /></td>";
                    strhtml += "<td><input type='submit' value='update' id='BTwebconfigUpdate'></td></tr>";
                    $("#debug").html(strhtml);
                });
				strhtml+="</table>";
				$("#webconfig").append(strhtml);
                GetAdminUser();
            }
        });

		
}

function GetSysStatus(){
        $.ajax({
            url: "/status",
            dataType: "jsonp",
            jsonp:"callbackparam",
            jsonpCallback:"jsonpcallback",
            error: function(jqXHR, textStatus, errorThrown) {
                if (textStatus == "error") {
                    alert(textStatus + " : " +errorThrown);
                } else {
                    alert(textStatus);
                }
            },
            success: function(data, textStatus, jqXHR) {
                $("#sysstatus").html("");
                $("#mysqlconfig").html("");
                
                var strMysqlconfig = "";
                var Total = "";
                var HttpdTimeOut = data.HttpdTimeOut;
                var Httpdthreads = data.Httpdthreads;
                var LogFileName = data.LogFileName;
                var LogLevel = data.LogLevel;
                var ServerIp = data.ServerIp;
                var port = data.port;
                var wwwroot = data.wwwroot;
                var StartTime = data.StartTime;
                var myDate = new Date(StartTime);
                
                var Mysql = data.Mysql;
                $.each(Mysql ,function(i){
                    strMysqlconfig += i + "=" + Mysql[i] + "<br>";
                });

                Total += "<table><tr><td>ServerIp</td><td>" + ServerIp + "</td>";
                Total += "<tr><td>port</td><td>" + port + "</td>";
                Total += "<tr><td>wwwroot</td><td>" + wwwroot + "</td>";
                Total += "<tr><td>StartTime</td><td>" + myDate + "</td>";
                Total += "<tr><td>HttpdTimeOut</td><td>" + HttpdTimeOut + "</td>";
                Total += "<tr><td>Httpdthreads</td><td>" + Httpdthreads + "</td>";
                Total += "<tr><td>LogFileName</td><td>" + LogFileName + "</td>";
                Total += "<tr><td>LogLevel</td><td>" + LogLevel + "</td></table>";
                
                //$("#mysqlconfig").append(strMysqlconfig);
                $("#sysstatus").append(Total);
            }
        });
}

function GetCmdshell(){
    $("#cmdshell").html("");
}


