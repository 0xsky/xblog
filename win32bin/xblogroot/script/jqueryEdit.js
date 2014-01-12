jQuery(function(){
    $(document).ready(function(){ 
        $(".tdRevert").click(tdRevert); 
        $(".tdDelete").click(tdDelete); 
        $(".tdUpdate").click(tdUpdate); 
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

