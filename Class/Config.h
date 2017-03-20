/*
author	ub
since	2016/2/18
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

//#define URL_LOGIN				"www/ok/"
//#define URL_SIGN_ON				"www/ok/"
//#define URL_UP_SCORE			"www/ok/"
#define URL_LOGIN				"first/ok/index.php"
#define URL_SIGN_ON				"first/ok/index.php"
#define URL_UP_SCORE			"first/ok/index.php"
//#define SER_HOST(url)			std::string("http://192.168.1.150/" + std::string(url))
#define SER_HOST(url)			std::string("http://121.42.141.113/" + std::string(url))
#define SER_PORT				"3306"



//-----------tools--------------


//#define toString(val)		#val

//-----------tools--------------

//---------plist file----------


// 存放用户信息的文件，该文件用于读取uid和pwd
#define FILE_PLIST_USERINFO			"user.plist"

/*
.plist文件根键
\e.g.
\<?xml version="1.0" encoding="UTF-8"?>
\<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"/>
\
\<plist version="1.0">
\    <dict>
\        <key>xxx</key>
\		 <type>xxx</type>
\    </dict>
\</plist>
@return "user_info"
*/
#define KEYROOT_USERINFO_1			"user_info"

/*
plist文件里的一个键
\e.g.
\<?xml version="1.0" encoding="UTF-8"?>
\<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"/>
\
\<plist version="1.0">
\    <dict>
\        <key>xxx</key>
\		 <type>xxx</type>
\    </dict>
\</plist>
@return "username"
*/
#define KEY_USERINFO_UID			"username"


/*
plist文件里的一个键
\e.g.
\<?xml version="1.0" encoding="UTF-8"?>
\<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"/>
\
\<plist version="1.0">
\    <dict>
\        <key>xxx</key>
\		 <type>xxx</type>
\    </dict>
\</plist>
@return "password"
*/
#define	KEY_USERINFO_PWD			"password"

/*
存储在UserDefault.xml
@return "score"
*/
#define	USERDEFAULT_KEY_SCORE		"score"

//---------plist file----------



//#define DEBUG_FIRST


//------------------------common------------------------
//---------json modules-----------
#define JSON_CODE				"code"
#define JSON_MSG				"message"
#define JSON_EXTE				"exte"	//{...,"exte":obj or arr}
#define JSON_NICKNAME			"nickname"
#define JSON_SCORE				"score"

//致命错误
#define CODE_FATAL				0
#define MSG_FATAL				std::string("fatal error: ")

//操作成功
#define CODE_SUCCESS			1
//操作失败
#define CODE_FAILED				0

//操作成功
#define MSG_SUCCESS				std::string("")
//操作失败
#define MSG_FAILED				std::string("failed: ")


//---------json modules-----------

//---------post modules-----------


#define	TODO					std::string("do")
// 注册
#define LOGIN					std::string("login")
// 登录
#define SIGN_ON					std::string("sign_on")
// 提交分数
#define UP_SCORE				std::string("up_score")
// 查看最高分
#define SEEK_SCORE				std::string("seek_score")


//用户信息
#define USERNAME				std::string("name")
#define PASSWORD				std::string("pwd")

//分数
#define SCORE					std::string("score")



//---------post modules-----------

//---------header modules-----------

// session key
#define HEADER_SESSION_KEY		std::string("Session: ")

// session val
#define HEADER_SIGN_ON_VAL		std::string("null")
#define HEADER_LOGIN_VAL		std::string("require")

//---------header modules-----------


//---------regex modules-----------

// 手机号：11位
#define REGEX_PHONE			"((13[0123569]{1}\\d{8})|(15[1235689]\\d{8})|(188\\d{8}))"

// 账户：字母开头，一共6-11位
#define REGEX_ACCOUNT		"([a-zA-Z][a-zA-Z0-9]{5,10})"

// 密码：6-18位数字字母
#define REGEX_PWD			"([a-zA-Z0-9]{6,18})"

//---------regex modules-----------




#endif //__CONFIG_H__
