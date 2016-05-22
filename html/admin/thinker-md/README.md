![logo](http://static.oschina.net/uploads/space/2015/0410/192411_CgHK_1028150.jpg)

### Thinker-md是什么

Thinker-md是一个支持markdown的在线文档编辑器，在线演示：http://thinkermd.oschina.mopaas.com/produce.html

### Thinker-md的特性

- 支持Markdown标准和Github风格；
- 编辑内容实时保存，刷新页面不会丢失；
- 支持实时预览、图片（跨域）上传；
- 支持MarkdownToHtml,HtmlToMarkdown双向解析；
- 支持emoji表情；
- 支持流程图与序列图；
- 多语言语法高亮；
- 可全屏可缩小，全屏编辑体验佳；
- 国际化支持；
- 极致的在线代码编写体验

### Thinker-md的由来

在开发`Team@OSC`的过程中，一直在寻找一款好用的Markdown编辑器，纵观所有开源的Markdown编辑器，要么就是外观简陋，要么就是集成繁琐，最终找了一款比较符合要求的产品[https://github.com/toopay/bootstrap-markdown](https://github.com/toopay/bootstrap-markdown) ,但由于在使用过程中仍然遇到一些不够完善的地方，于是我们决定自己开发完善一个，并且开源出来，方便大家。

### License

The MIT License.

Copyright (c) 2016 OSChina.net

### 我们正在用

`Team@OSC` , `Git@OSC`


# 1. 开发者指南

### 1.1 安装Grunt插件
参考:
    [Nodejs 文档](https://nodejs.org/documentation/)
    [Grunt 中文社区](http://www.gruntjs.org/)

进入thinker-md根目录，执行如下命令(**2016.03.30更新加入了新依赖，请更新后再执行一次**)：
```Nodejs
npm install
```

### 1.2 编译
```Nodejs
grunt watch
```
编译后会生成如下目录

dist

----javascript

----|----user

----|----|----thinker-md-user.js

----|----|----thinker-md-user.min.js

----|----|----thinker-md-user.min.map

----|----vendor

----|----|----jquery-2.1.3.js

----|----|----thinker-md.js

----|----|----thinker-md.min.js

----|----|----thinker-md.min.map

----stylesheets

----|----fonts

----|----|----fontawesome-webfont.eot

----|----|----glyphicons-halflings-regular.eot

----|----|----...

----|----img

----|----user

----|----|----thinker-md.user.min.css

----|----vendor

----|----|----thinker-md.min.css


# 2. 用户指南

> thinker-md 依赖jquery库。推荐使用最新版jquery 1.x或者2.x

### 页面引入
_参照/view/index.html和/view/develop.html_
- index.html默认引用编译之后的js和css资源。
- 如果需要定制thinker-md，你需要按照 **开发者指南** 步骤执行命令，切换到develop.html。所有资源的修改会自动生成到dist目录下。
- **推广ES6(ES2015)**：我们强烈建议您修改bootstrap-markdown-es6.js而非bootstrap-markdown.js，您可以在里面使用ES6标准的JavaScript来进行编码。代码将被编译为ES5标准代码的bootstrap-markdown.js，因此您无需担心兼容性问题。([了解ES6](https://babeljs.io/docs/learn-es2015/))

# 3. 使用

### 3.1 使用案例
```html
<!DOCTYPE html>
<html>
<head lang="zh">
    <meta charset="UTF-8">
    <title>Thinker-md</title>
</head>
<body>
    <textarea id="md" data-provide="markdown"></textarea>
<link href="../dist/stylesheets/user/thinker-md.user.min.css" rel="stylesheet">
<link href="../dist/stylesheets/vendor/thinker-md.min.css" rel="stylesheet">
<script src="../dist/javascripts/vendor/jquery-2.1.3.js" type="text/javascript"></script>
<script src="../dist/javascripts/vendor/thinker-md.js" type="text/javascript"></script>
<script>
    $("#md").markdown({
    language: 'zh',
    fullscreen: {
        enable: true
    },
    resize:'vertical',
    localStorage:'md',
    imgurl: 'http://192.168.1.142:8080/upload',
    base64url: 'http://192.168.1.142:8080/base64'
});
</script>
</body>
</html>
```



### 3.2 配置参数说明
|参数名称|类型|说明|
|:----| :---- | :----|
|`autofocus`|`boolean`|编辑器初始后是否默认获取焦点。 默认 `false`|
|`savable`|`boolean`|编辑器是否显示并激活保存按钮。 默认 `false`|
|`hideable`|`boolean`|如果设置 `true` ，编辑器在 `blur` 事件后自动隐藏。 默认 `false`|
|`width`|`mixed`|编辑器宽度。 默认 `inherit` 支持数字类型 (在`css`充许范围), 或bootstrap样式 (如 `span2`)|
|`height`|`mixed`|编辑器高度。 默认 `inherit`|
|`resize`|`string`|禁用或改变 `resize` 属性, 可能的值 `none`,`both`,`horizontal`,`vertical`。 默认 `none` 如果此属性未被禁止, 用户可以预览时改变编辑器高度。|
|`language`|`string`|本地语言设置。 默认 `zh`|
|`footer`|`mixed`|编辑器底部栏. 可能的值 `string`,`callback`。 默认为空|
|`fullscreen`|`object`| `enable` (`boolean`)：是否启用全屏；`debounce` (`number`)：全屏状态下由按键触发的解析函数两次执行最小间隔，用来防止快速输入时的卡顿，单位毫秒，默认为0，不限制。|
|`hiddenButtons`|`mixed`|按钮名字数组或字符串。 默认为空字符串|
|`disabledButtons`|`mixed`|按钮名字数组或字符串。 默认为空字符串|
|`localStorage`|`string`|HTML5本地存储，全局唯一|
|`imgurl`|`url`|图像上传地址|
|`base64url`|`url`|base64编码格式图像上传地址|
|`flowChart`|`boolean`|是否启用流程图、时序图|

### 3.2.1 java版上传代码实现

普通/base64图像上传案例(代码要求jdk8, 可自行修改以适应低版本jdk)
```java
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebInitParam;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.util.ArrayList;
import java.util.Base64;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Created by 艺 on 16/03/31.
 */
@WebServlet(name = "upload", urlPatterns = "/c/imgUpload", initParams = {@WebInitParam(name = "upload_path", value = "/ImageDir/")})
public class UploadServlet extends javax.servlet.http.HttpServlet {

    @Override
    protected void doOptions(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.addHeader("Access-Control-Allow-Origin", "*");
        resp.addHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
        resp.addHeader("Access-Control-Allow-Headers", "Cache-Control,X-Requested-With,Content-Type");
        super.doOptions(req,resp);
    }


    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        this.doPost(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        req.setCharacterEncoding("UTF-8");
        resp.addHeader("Access-Control-Allow-Origin", "*");
        String imageDir = getServletConfig().getInitParameter("upload_path");
        String directory = req.getSession().getServletContext().getRealPath(imageDir);
        StringBuilder path = new StringBuilder(directory);
        path.append(File.separator);

        StringBuffer url = req.getRequestURL();
        String urlBase = url.substring(0, url.indexOf(req.getRequestURI())) + imageDir;

        boolean isMultipart = ServletFileUpload.isMultipartContent(req);
        if (isMultipart) {
            FileItemFactory fileItemFactory = new DiskFileItemFactory();
            ServletFileUpload servletFileUpload = new ServletFileUpload(fileItemFactory);
            List<String> imgFileNames = new ArrayList<>();

            try {
                for (FileItem item : servletFileUpload.parseRequest(req)) {
                    if (!item.isFormField()) { // file upload
                        //文件名称
                        String name = item.getName();
                        String fileName = new Date().getTime() + name.substring(name.lastIndexOf('\\') + 1, name.length());

                        //上传文件
                        File file = new File(path + fileName);
                        File dir = file.getParentFile();
                        if (!dir.exists() && !dir.isDirectory()) {
                            dir.mkdir();
                        }
                        item.write(file);

                        imgFileNames.add(fileName);
                    } else if (item.getFieldName().equals("base64Date")) { // base64 upload
                        String fileName = new Date().getTime() + ".png";
                        StringBuilder base64Data = new StringBuilder();
                        Reader reader = new InputStreamReader(item.getInputStream(), "UTF-8");
                        char[] buffer = new char[1024];
                        int read;
                        while ((read = reader.read(buffer)) != -1) {
                            base64Data.append(buffer, 0, read);
                        }
                        reader.close();
                        String base64 = base64Data.toString();
                        if (!"".equals(base64.trim())) {
                            base64 = base64.substring(base64.lastIndexOf(',') + 1);
                            byte[] b = Base64.getDecoder().decode(base64.getBytes());
                            BufferedOutputStream bos;
                            FileOutputStream fos;
                            File file = new File(path + fileName);
                            fos = new FileOutputStream(file);
                            bos = new BufferedOutputStream(fos);
                            bos.write(b);
                            fos.flush();
                            fos.close();
                            bos.flush();
                            bos.close();

                            imgFileNames.add(fileName);
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            resp.addHeader("Access-Control-Allow-Origin", "*");
            resp.setContentType("text/json; charset=UTF-8");
            OutputStream outputStream = resp.getOutputStream();
            outputStream.write(imgFileNames.stream()
                    .map(fileName -> (urlBase + fileName))
                    .collect(Collectors.joining("\n"))
                    .getBytes("UTF-8"));

            outputStream.flush();
            outputStream.close();
            return ;
        }
    }
}
```