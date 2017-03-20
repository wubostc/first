<?php
/**
 * author   ub
 * since    2016/2/3
 *
 */

define("SER_HOST",'127.0.0.1:3306');
define("SER_PORT",'3306');
define("SER_USERNAME",'ub');
define("SER_PASSWORD",'20160111c');
define("SER_DBNAME",'db_base');

//数据库
define("DB_BASE",'db_base');
define("DB_EXTRA",'db_extra');
define("DB_SESS","db_sess");

//
define("VERIFY_KEY","verify");

//----------------------unique--------------------------
// header 是否包括这一项：$_SERVER["HTTP_SESSION"]
define("HTTP_SESSION","HTTP_SESSION");


//----------------------common--------------------------
//-------json modules---------
define("JSON_CODE",'code');
define('JSON_MSG','message');
define('JSON_EXTE','exte');
define('JSON_NICKNAME','nickname');
define('JSON_SCORE','score');

//致命错误
define("MSG_FATAL","fatal error: ");
define("CODE_FATAL",0);

//操作正常
define("CODE_SUCCESS",1);
//操作失败
define("CODE_FAILED",0);

//操作成功
define("MSG_SUCCESS",'');
//操作失败
define("MSG_FAILED",'failed: ');

//-------json modules---------


//--------post modules-----------
//
define('TODO','do');
// 注册
define("LOGIN",'login');
// 登录
define("SIGN_ON","sign_on");
// 提交分数
define("UP_SCORE","up_score");
// 查看最高分
define("SEEK_SCORE","seek_score");

//用户信息
define("USERNAME",'name');
define("PASSWORD",'pwd');

//分数
define('SCORE','score');
//--------post modules-----------


//--------header modules----------

// sessin key
define("HEADER_SESSION_KEY",    "Session: ");

// session val
define('HEADER_SING_ON_VAL','null');
define('HEADER_LOGIN_VAL','require');

//--------header modules----------


//--------regex modules--------
// 手机号：11位
define("REGEX_PHONE","/13[0123569]{1}\d{8}|15[1235689]\d{8}|188\d{8}/");

// 账户：字母开头，一共６-11位
define("REGEX_ACCOUNT","/[a-zA-Z][a-zA-Z0-9]{5,10}/");

// 密码：6-18位数字子母
define("REGEX_PWD","/[a-zA-Z0-9]{6,18}/");
//--------regex modules--------

?>


