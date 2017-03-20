<?php
/**
 * Created by PhpStorm.
 * User: ub2
 * Date: 2016/2/11
 * Time: 13:20
 */
require_once ("Regex.php");
require_once ('MySQLConnector.php');
require_once ('JSON.php');
require_once ('Session.php');
require_once ('Config.php');
class Listener{

    /*
     * 检查是否合法
     *
     * param
     *
     * return   ok      true
     *          failed  false
     *
     * author   ub
     *
     * since    2016/2/11
     */
    static protected function check(){

        if(isset($_POST[USERNAME]) && isset($_POST[PASSWORD])){
            $name = $_POST[USERNAME];
            $pwd = $_POST[PASSWORD];

            // 安全检查，正则匹配
            if(!Regex::match($name,$pwd)){
                return false;
            }

            return true;
        }
        return false;
    }

    /*
     * 登录
     *
     * author	ub
     *
     * since	2016/2/11
     */
    static public function singOn(){

        if(!self::check()){
            echo JSON::encode_2(CODE_FAILED,MSG_FAILED.'输入的用户名或密码有误！');
            exit;
        }



        // true 自动将函数 session_write_close() 注册为 register_shutdown_function() 函数。
        session_set_save_handler(Session::__handler(),true);
        session_start();

        // 设置header
        header("Content-Type: first");
        header(HEADER_SESSION_KEY.session_id());

        // 设置 write($session,$data) 的$data
        // 保存来自客户端的session id，如果是null 则保存当前id
        $_SESSION[VERIFY_KEY] = session_id();

        // 设置content
        $response = MySQLConnector::getInstance()->signOn($_POST[USERNAME],$_POST[PASSWORD]);// ok  {"code":1,"message":""}
        echo $response;
        exit;
    }



    /*
    * 监听更新分数操作
    *
    * author	ub
    *
    * since	2016/2/11
    */
    static public function upScore(){

        if(!self::check()){
            echo JSON::encode_2(CODE_FAILED,MSG_FAILED.'输入的用户名或密码有误！');
            exit;
        }

        // 开始session
        session_set_save_handler(Session::__handler(),true);// true 将函数 session_write_close() 注册为 register_shutdown_function() 函数
        session_start();

        // 设置header
        header("Content-Type: first");
        // 接着调用destory();
        session_regenerate_id(true);
        header(HEADER_SESSION_KEY.session_id());

        // session_id() 被重新生成了
        $_SESSION[VERIFY_KEY] = session_id();


        // 设置content
        $score = intval($_POST[SCORE]);
        if(!is_numeric($score)){
            echo JSON::encode_2(CODE_FAILED,MSG_FAILED);
            exit;
        }
        if($score == 0){
            echo JSON::encode_2(CODE_FAILED,MSG_FAILED.'分数不能为0！');
            exit;
        }
        $response = MySQLConnector::getInstance()->upScore($_POST[USERNAME],$_POST[PASSWORD],$score);
        echo $response;
        exit;
    }

    /*
    * 注册
    *
    * author	ub
    *
    * since	2016/2/11
    */
    static public function login(){

        if(!self::check()){
            echo JSON::encode_2(CODE_FAILED,MSG_FAILED.'输入的用户名或密码有误！');
            exit;
        }

        // 开始session
        session_set_save_handler(Session::__handler(),true);
        session_start();

        // 设置header
        header("Content-Type: first");
        header(HEADER_SESSION_KEY.session_id());
        $_SESSION[VERIFY_KEY] = session_id();

        // 设置content
        $response = MySQLConnector::getInstance()->login($_POST[USERNAME],$_POST[PASSWORD]);
        echo $response;
        exit;
    }

    /*
     * 查找最高分
     *
     * since    2016/2/20
     */
    static public function tell(){

        if(!self::check()){
            echo JSON::encode_2(CODE_FAILED,MSG_FAILED.'输入的用户名或密码有误！');
            exit;
        }

        // 开始session
        session_set_save_handler(Session::__handler(),true);// true 将函数 session_write_close() 注册为 register_shutdown_function() 函数
        session_start();

        // 设置header
        header("Content-Type: first");
        // 接着调用destory();
        session_regenerate_id(true);
        header(HEADER_SESSION_KEY.session_id());

        // session_id() 被重新生成了
        $_SESSION[VERIFY_KEY] = session_id();


        // 设置content
        $response = MySQLConnector::tell($_POST[USERNAME],$_POST[PASSWORD]);
        echo $response;
        exit;
    }

}


?>