xblog
=====
xBlog is a simple, high-speed, customizable blog app developed by C++
<p>
<b>Simple:</b> Simple and compact design, the main program environment to support only a single executable file without HTTPD, PHP, etc.. <p>
<b>High speed:</b> pure C + + development, support for multi-threading, MYSQL connection pooling support, and support for hot data cache. <p>
<b>Flexible:</b> rich customization features and flexible page module based xhtml + css template structure, giving full rights to user-defined. <p>
<b>Multi-platform:</b> C / C + + development, the perfect support for LINUX, WINDOWS, Raspberry Pi and other platforms. . 

====
<p>欢迎使用xBlog系统
<p>C++打造的简单高速可定制博客
<p><p>
<p>简单 : 设计简单小巧，主程序仅一个可执行文件.无需HTTPD，PHP等环境支持。
<p>高速 : 纯C++开发，支持多线程，支持MYSQL连接池，并支持热点数据缓存。
<p>灵活：丰富的定制特性，灵活的页面模块、基于xhtml+css的模板架构，充分给予用户自定义的权利。.
<p>多平台: C/C++开发，完美支持LINUX，WINDOWS，Raspberry Pi 等平台。.
<p>
<p>demo: <a href="http://www.0xsky.com/"  target="_blank" >xBlog </a>
<p>xBlog交流QQ群:  202115979
<p><p>作者: xSky        
<p>博客: <a href="http://www.0xsky.com/">xSky's Blog</a>
<p>捐赠作者:[支付宝账号] guozhw@gmail.com
<hr>

### 安装 xBlog
    1.安装 libevent
      yum install mysql-devel libevent 
      apt-get install libevent-dev libmysqlclient-dev
    2.编译 xblog
      svn co https://github.com/0xsky/xblog.git xblog
      make
    3.配置&运行
      将 /doc 目录内的 xblog_dev.sql 导入到MYSQL数据库
      修改 xblog.ini 内配置
      ./xBlog.sh start

