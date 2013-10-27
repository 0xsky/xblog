/*
Navicat MySQL Data Transfer

Source Server         : MK802-152
Source Server Version : 50531
Source Host           : 192.168.1.152:3306
Source Database       : xblog_dev

Target Server Type    : MYSQL
Target Server Version : 50531
File Encoding         : 65001

Date: 2013-10-27 12:32:04
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `xb_classify`
-- ----------------------------
DROP TABLE IF EXISTS `xb_classify`;
CREATE TABLE `xb_classify` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `classify_id` varchar(60) NOT NULL DEFAULT '',
  `classify_name` varchar(64) NOT NULL DEFAULT '',
  `classify_status` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  KEY `classify_id` (`classify_id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of xb_classify
-- ----------------------------
INSERT INTO `xb_classify` VALUES ('2', '1', '知识┊技术相关', '0');
INSERT INTO `xb_classify` VALUES ('3', '2', '作品┊原创作品', '0');
INSERT INTO `xb_classify` VALUES ('4', '3', '代码┊程序源码 ', '0');
INSERT INTO `xb_classify` VALUES ('5', '4', '随笔┊心情日记', '0');
INSERT INTO `xb_classify` VALUES ('6', '5', '杂谈┊杂七杂八 ', '0');

-- ----------------------------
-- Table structure for `xb_comments`
-- ----------------------------
DROP TABLE IF EXISTS `xb_comments`;
CREATE TABLE `xb_comments` (
  `comment_ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `comment_post_ID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `comment_author` tinytext NOT NULL,
  `comment_author_email` varchar(100) NOT NULL DEFAULT '',
  `comment_author_url` varchar(200) NOT NULL DEFAULT '',
  `comment_author_IP` varchar(100) NOT NULL DEFAULT '',
  `comment_date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `comment_date_gmt` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `comment_content` text NOT NULL,
  `comment_karma` int(11) NOT NULL DEFAULT '0',
  `comment_approved` varchar(20) NOT NULL DEFAULT '1',
  `comment_agent` varchar(255) NOT NULL DEFAULT '',
  `comment_type` varchar(20) NOT NULL DEFAULT '',
  `comment_parent` bigint(20) unsigned NOT NULL DEFAULT '0',
  `user_id` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`comment_ID`),
  KEY `comment_approved` (`comment_approved`),
  KEY `comment_post_ID` (`comment_post_ID`),
  KEY `comment_approved_date_gmt` (`comment_approved`,`comment_date_gmt`),
  KEY `comment_date_gmt` (`comment_date_gmt`),
  KEY `comment_parent` (`comment_parent`)
) ENGINE=MyISAM AUTO_INCREMENT=358 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of xb_comments
-- ----------------------------

-- ----------------------------
-- Table structure for `xb_links`
-- ----------------------------
DROP TABLE IF EXISTS `xb_links`;
CREATE TABLE `xb_links` (
  `link_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `link_url` varchar(255) NOT NULL DEFAULT '',
  `link_name` varchar(255) NOT NULL DEFAULT '',
  `link_image` varchar(255) NOT NULL DEFAULT '',
  `link_target` varchar(25) NOT NULL DEFAULT '',
  `link_description` varchar(255) NOT NULL DEFAULT '',
  `link_visible` varchar(20) NOT NULL DEFAULT 'Y',
  `link_owner` bigint(20) unsigned NOT NULL DEFAULT '1',
  `link_rating` int(11) NOT NULL DEFAULT '0',
  `link_updated` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `link_rel` varchar(255) NOT NULL DEFAULT '',
  `link_notes` mediumtext NOT NULL,
  `link_rss` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`link_id`),
  KEY `link_visible` (`link_visible`)
) ENGINE=MyISAM AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of xb_links
-- ----------------------------
INSERT INTO `xb_links` VALUES ('11', 'http://www.0xsky.com/', 'xSky的Blog', '', '', '', 'Y', '1', '0', '0000-00-00 00:00:00', '', '', '');
INSERT INTO `xb_links` VALUES ('8', 'http://lab.0xsky.com/', 'xSky云实验室', '', '', '', 'Y', '1', '0', '0000-00-00 00:00:00', '', '', '');
INSERT INTO `xb_links` VALUES ('9', 'http://rpi.0xsky.com/', '树莓派实验室', '', '', '', 'Y', '1', '0', '0000-00-00 00:00:00', '', '', '');

-- ----------------------------
-- Table structure for `xb_posts`
-- ----------------------------
DROP TABLE IF EXISTS `xb_posts`;
CREATE TABLE `xb_posts` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `post_author` bigint(20) unsigned NOT NULL DEFAULT '0',
  `post_date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `post_content` longtext NOT NULL,
  `post_brief` longtext NOT NULL,
  `post_title` text NOT NULL,
  `post_status` varchar(20) NOT NULL DEFAULT 'publish',
  `post_classify` int(11) NOT NULL DEFAULT '0',
  `comment_status` varchar(20) NOT NULL DEFAULT 'open',
  `post_modified` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `post_url` varchar(255) NOT NULL DEFAULT '',
  `comment_count` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=931 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of xb_posts
-- ----------------------------
INSERT INTO `xb_posts` VALUES ('930', '1', '2013-10-23 13:28:02', '<p>运行在MK802II迷你PC上的私有云服务器：</p>\r\n<div class=\"entry-content\">\r\n<p><a href=\"http://lab.0xsky.com/\">http://lab.0xsky.com/</a></p>\r\n<p>MK802II迷你PC系统配置：</p>\r\n<p>&nbsp;</p>\r\n<table>\r\n<tbody>\r\n<tr>\r\n<td>系 统</td>\r\n<td>Linux debian 3.4.29+ #3 PREEMPT Mon Mar 25 14:19:15 CST 2013 armv7l GNU/Linux</td>\r\n</tr>\r\n<tr>\r\n<td>处理器</td>\r\n<td>Allwinner A10/ 1GHz Cortex-A8</td>\r\n</tr>\r\n<tr>\r\n<td>内存</td>\r\n<td>1G</td>\r\n</tr>\r\n<tr>\r\n<td>存储空间</td>\r\n<td>4GB</td>\r\n</tr>\r\n<tr>\r\n<td>GPU</td>\r\n<td>2D/ 3D/ OpenGL ES2.0(AMD Z430)/OpenVG1.1(AMD Z160) 27M Tri/sec</td>\r\n</tr>\r\n<tr>\r\n<td>网络</td>\r\n<td>Wireless 802.11b/g, WAPI(Ralink8188)</td>\r\n</tr>\r\n<tr>\r\n<td>扩展</td>\r\n<td>Micro TF卡，最高支持32GB</td>\r\n</tr>\r\n<tr>\r\n<td>\r\n<div>接口</div>\r\n</td>\r\n<td>Micro 5pin USB/ USB2.0 data支持OTG</td>\r\n</tr>\r\n<tr>\r\n<td>输入功率</td>\r\n<td>5V2A</td>\r\n</tr>\r\n<tr>\r\n<td>尺寸</td>\r\n<td>8.8*3.5*1.2cm</td>\r\n</tr>\r\n<tr>\r\n<td>重量</td>\r\n<td>0.2kg</td>\r\n</tr>\r\n</tbody>\r\n</table>\r\n</div>', '<p>运行在MK802II迷你PC上的私有云服务器：</p>\r\n<p>MK802II迷你PC系统配置：</p>', 'xSky实验室', 'publish', '1', 'open', '0000-00-00 00:00:00', '', '0');

-- ----------------------------
-- Table structure for `xb_siteconfig`
-- ----------------------------
DROP TABLE IF EXISTS `xb_siteconfig`;
CREATE TABLE `xb_siteconfig` (
  `Fid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Fkey` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `Fvalue` varchar(1024) CHARACTER SET utf8 DEFAULT '',
  PRIMARY KEY (`Fid`)
) ENGINE=MyISAM AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of xb_siteconfig
-- ----------------------------
INSERT INTO `xb_siteconfig` VALUES ('1', 'XBLOG_HOST', 'http://lab.0xsky.com');
INSERT INTO `xb_siteconfig` VALUES ('2', 'XBLOG_DESCRIPTION', '关注网络编程,高性能计算,分布式系统,');
INSERT INTO `xb_siteconfig` VALUES ('3', 'XBLOG_ADMIN_PASS', '123');
INSERT INTO `xb_siteconfig` VALUES ('4', 'XBLOG_TITLE', 'xSky 实验室');
INSERT INTO `xb_siteconfig` VALUES ('5', 'XBLOG_LANGUAGE', 'zh-CN');
INSERT INTO `xb_siteconfig` VALUES ('6', 'XBLOG_NAME', 'xSky 实验室');
INSERT INTO `xb_siteconfig` VALUES ('7', 'XBLOG_SUB_NAME', '关注网络编程,高性能计算,分布式系统');
INSERT INTO `xb_siteconfig` VALUES ('8', 'XBLOG_SUB_TITLE', '关注网络编程,高性能计算,分布式系统');
INSERT INTO `xb_siteconfig` VALUES ('9', 'XBLOG_THEME', 'xSky');
INSERT INTO `xb_siteconfig` VALUES ('10', 'XBLOG_VERSION', 'xBlog 1.0');
INSERT INTO `xb_siteconfig` VALUES ('11', 'XBLOG_COPYRIGHT', 'Copyright 0xsky.com All Rights Reserved.<script src=\"http://s14.cnzz.com/stat.php?id=5015251&web_id=5015251&show=pic1\" language=\"JavaScript\"></script>');
INSERT INTO `xb_siteconfig` VALUES ('12', 'XBLOG_MAX_VIEW', '10');

-- ----------------------------
-- Table structure for `xb_users`
-- ----------------------------
DROP TABLE IF EXISTS `xb_users`;
CREATE TABLE `xb_users` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `user_login` varchar(60) NOT NULL DEFAULT '',
  `user_pass` varchar(64) NOT NULL DEFAULT '',
  `user_nicename` varchar(50) NOT NULL DEFAULT '',
  `user_email` varchar(100) NOT NULL DEFAULT '',
  `user_url` varchar(100) NOT NULL DEFAULT '',
  `user_registered` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `user_activation_key` varchar(60) NOT NULL DEFAULT '',
  `user_status` int(11) NOT NULL DEFAULT '0',
  `display_name` varchar(250) NOT NULL DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `user_login_key` (`user_login`),
  KEY `user_nicename` (`user_nicename`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of xb_users
-- ----------------------------
INSERT INTO `xb_users` VALUES ('1', 'xSky', '925EB10CFFA23C2CC19235BB0F17001D', 'xSky', '0xsky@live.com', 'http://www.0xsky.com/', '2012-04-21 17:02:28', '', '0', 'xSky');
