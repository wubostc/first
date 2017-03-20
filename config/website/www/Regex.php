<?php
/**
 * Created by PhpStorm.
 * User: ub2
 * Date: 2016/2/9
 * Time: 16:19
 */
require_once("Config.php");

class RegexComm{

    /*
     * 对一个表达式字符串进行正则匹配
     *
     * param    expression
     *          regex
     *
     * return   ok      true
     *          failed  false
     *
     * author   ub
     *
     * since    2016/2/10
     */
    static protected function match($expr = '',$rule = ''){

        $arr = array();
        $count = 0;

        // 成功
        if(preg_match($rule,$expr,$arr)){
            foreach($arr as $k=>$v){
                $count+=strlen($v);
            }

            // 匹配的字符串长度是否与expr长度相等
            return $count == strlen($expr) ? true : false;
        }

        return false;
    }
}



class Regex extends RegexComm{

    // 手机号
    static private $REGEX_PHONE      = REGEX_PHONE;

    // 账户：字母开头，一共６-11位
    static private $REGEX_ACCOUNT    = REGEX_ACCOUNT;

    // 密码：6-18位数字子母
    static private $REGEX_PWD        = REGEX_PWD;

    /*
     * 匹配账户和密码，
     * 账户：同时兼容6-11位的账户名或11位的手机号，
     * 密码：6-18位的密码
     *
     * param    username
     *          password
     *
     * return   ok      true
     *          failed  false
     *
     * author   ub
     *
     * since    2016/2/11
     *
     */
    static public function match($username = '',$password = ''){

        // 是否匹配账户
        if(parent::match($username,self::$REGEX_PHONE) || parent::match($username,self::$REGEX_ACCOUNT)){
            // 是否匹配密码
            if(parent::match($password,self::$REGEX_PWD)){
                return true;
            }
        }

        return false;
    }
}



?>