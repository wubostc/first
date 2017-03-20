<?php
/**
 * Created by PhpStorm.
 * User: ub2
 * Date: 2016/2/2
 * Time: 15:37
 */

require_once("Config.php");
require_once("MySQLConnector.php");
/*
 * 手动管理session，需要调用session_start()才能生效，open->read->write->close
 * 在脚本结束后可能会出现析构异常，因此调用register_shutdown_function($sess_obj)进行注册
 *
 * author   ub
 *
 * since    2016/2/7
 *
 */
class Session implements SessionHandlerInterface{

    // 连接到数据库的资源
    private $link_;

    // 用户标识符
    private $uid_;

    // 单例
    static private $instance_;

    /*
     * 获得单例
     *
     * param
     *
     * return   instance
     *
     * author   ub
     *
     * since    2016/2/8
     */
    static public function __handler(){
        if(!self::$instance_) {
            self::$instance_ = new self;
        }
        return self::$instance_;
    }

    /*
     * author   ub
     *
     * since    2016/2/8
     */
    private function __construct(){
        $this->link_ = null;
        $this->uid_ = null;
    }

    /*
     * 这是自动开始会话或者通过调用 session_start() 手动开始会话 之后第一个被调用的回调函数。 此回调函数操作成功返回 TRUE，反之返回 FALSE。
     *
     * param    str
     *          str
     *
     * return   ok      true
     *          failed  false
     *
     * author   ub
     *
     * since    2016/2/8
     */
    public function open($save_path, $session_id){  // string(0) "" string(3) "UID"

        // 常规检查：用户是否合法
        if(!isset($_SERVER[HTTP_SESSION])){
            echo MSG_FAILED."messing param!";
            exit;
        }

        $link = mysqli_connect(SER_HOST,SER_USERNAME,SER_PASSWORD,DB_SESS);
        if($link == true){
            $this->link_ = $link;
            return true;
        }
        else{
            echo MSG_FATAL."connect failed!";
            exit;
        }
    }

    /*
     * 在 write 回调函数调用之后调用。 当调用 session_write_close() 函数之后，也会调用 close 回调函数。 此回调函数操作成功返回 TRUE，反之返回 FALSE。
     *
     * param
     *
     * return   ok  true
     *
     * author   ub
     *
     * since    2016/2/8
     */
    public function close(){
        return mysqli_close($this->link_) ? true : false;
    }

    /*
     * PHP 会自动反序列化返回的字符串并填充 $_SESSION 超级全局变量。 虽然数据看起来和 serialize() 函数很相似， 但是需要提醒的是，它们是不同的。
     *
     * param    session_id
     *
     * return   str(null)
     *
     * author   ub
     *
     * since    2016/2/12
     */
    public function read($new_session_id){              // string(26) "m1d36cgie8ibemstk77gufv692"

        // 请求注册
        if(strcmp($_SERVER[HTTP_SESSION],"require") == 0){
            return "";
        }

        // uid
        $this->uid_ = MySQLConnector::getInstance()->getUID($_POST[USERNAME],$_POST[PASSWORD]);

        // 检查用户是否合法
        if(!empty( $this->uid_)){
            // 检查client 发送给server 的header
            if(strlen($_SERVER[HTTP_SESSION]) == 26){

                $result = mysqli_query($this->link_,"select `session_data` from tb_session where session_id = '". $this->uid_."' and session_expires > '".date('Y-m-d H:i:s')."'");

                // 还在生存期限内
                if(mysqli_num_rows($result) == 1){

                    $data = mysqli_fetch_assoc($result);    //var_dump($data) -> array(1) { ["user_id"]=> string(1) "1" }

                    // string -> verify|s:26:"12345678901234567890abcdef";
                    $list = explode("\"",$data["session_data"]);    // 分成三份 list[0] -> "verify|s:26:"  list[1] -> "12345678901234567890abcdef"   list[2] -> ";"

                    // 通过对比在数据库的sess
                    if(strcmp($list[1], $_SERVER[HTTP_SESSION]) == 0){
                        return "";
                    }
                    // 数据异常
                    else{
                        echo MSG_FAILED."access denied! ".__LINE__;
                        exit;
                    }
                }
                // 超过生存期限
                else{
                    echo MSG_FAILED."time out!";
                    exit;
                }
            }
            // 请求登录
            else if(strcmp($_SERVER[HTTP_SESSION], "null") == 0){
                // 如果是null，那么需要手动指定 $_SESSION["k"] = "v";
                return "";
            }
            else{
                echo MSG_FAILED."incorrect session".$_SERVER[HTTP_SESSION];
                exit;
            }
        }
        else{
            echo MSG_FAILED."access denied!";
            exit;
        }
    }

    /*
     * PHP 会在脚本执行完毕或调用 session_write_close() 函数之后调用此回调函数。
     * 在会话保存数据时会调用 write 回调函数。
     * $new_session_id 接收当前会话 ID 或 session_regeneral_id()后生成的ID)，$data 接收 $_SESSION 中数据序列化之后的字符串作为参数。
     *
     * param    session_id
     *          serialize_str
     *
     * return   ok      true
     *          failed  false
     *
     * author   ub
     *
     * since    2016/2/8
     */
    public function write($new_session_id, $data){               // "21ssbg4091udb7oirtcb13etj6"     $data 实为$_SESSION 例："name|s:26:"21ssbg4091udb7oirtcb13etj6";"

        $lifetime  = date('Y-m-d H:i:s',strtotime(date('Y-m-d H:i:s').' + 30 minutes'));

        $result = mysqli_query($this->link_,"replace into tb_session set session_id = '$this->uid_', session_expires = '$lifetime', session_data = '$data'");

        if($result){
            return true;
        }
        else{
            echo MSG_FATAL."write failed! ".__LINE__;
            return false;
        }
    }


    /*
     * 当调用 session_destroy() 函数， 或者调用 session_regenerate_id() 函数并且设置 destroy 参数为 TRUE 时，会调用此回调函数。
     * 会删除旧会话数据
     *
     * param    上一次session id
     *
     * return   ok      true
     *          failed  false
     *
     * author   ub
     *
     * since    2016/2/8
     */
    public function destroy($old_session_id){
        // 直接返回
        return true;
    }

    /*
     * 垃圾回收，会话开始后(read()之后)会调用
     * 在php.ini 中已经把maxlifetime 改为0，所以对maxlifetime 不作判断
     *
     * parma    max life time
     *
     * return   ok      true
     *          failed  false
     *
     * author   ub
     *
     * since    2016/2/8
     */
    public function gc($maxlifetime){

        $now = date('Y-m-d H:i:s');
        $result = mysqli_query( $this->link_,"delete from tb_session where session_expires > '".$now."'" );
        if($result){
            return true;
        }
        else{
            return false;
        }
    }
}


?>