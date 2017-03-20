<?php
/**
 * Created by PhpStorm.
 * User: ub2
 * Date: 2016/1/20
 * Time: 14:56
 */

require_once("Config.php");

/*
 * 管理与mysql之间的连接
 *
 * author ub
 *
 * since 2016/1/19
 * */
class MySQLConnector{

    // 连接到数据库的资源
    static private $link = null;

    // 单例
    static private $instance = null;

    /*
     * 初始化并连接至数据库
     *
     * since
     * \2016/2/8
     * */
    private function __construct() {
        self::$link = new mysqli(SER_HOST,SER_USERNAME,SER_PASSWORD);
        if(!self::$link){
            throw new Exception(MSG_FATAL.'connection failed! '.__LINE__,CODE_FATAL);
        }
    }

    /*
     * 释放资源
     *
     * since
     * \2016/2/8
     * */
    public function __destruct() {
        if(self::$link){
            self::$link->close();
        }
    }

    /*
     * MySQL连接实例(单例模式)
     *
     * return
     * \instance
     * since
     * \2016/2/11
     * */
    static public function getInstance(){
        if(!self::$link){
            self::$instance = new self();
        }
        return self::$instance;
    }

    /*
     * 获得该类的mysqli唯一的指针
     *
     * return
     * \$link;
     * since
     * \2016/2/9
     * */
    static public function link(){
        return self::$link ? self::$link : null;
    }


    /*
     * 查看最高分(POST)
     *
     * since    2016/2/20
     */
    static public function tell($username,$password){

        self::$link->select_db(DB_EXTRA);

        $query = "select * from tb_ranking order by user_score desc limit 0,1";
        $result = self::$link->query($query);

        if($result->num_rows == 1){
            $arr = $result->fetch_assoc();  // user_nickname=>13012341234, user_score=>1000
            $result->free();
            $arr = array(JSON_NICKNAME=>$arr['user_nickname'],JSON_SCORE=>intval($arr['user_score'])); // wrong: intval 带符号的int
            return JSON::encode_3(CODE_SUCCESS,MSG_SUCCESS,$arr);
        }
        $result->free();
        return JSON::encode_2(CODE_FAILED,MSG_FAILED.'查找失败！');
    }


    /*
     * 请求登录 (POST)
     *
     * param
     * \username
     * \password
     * return
     * \json格式的字符串
     * since
     * \2016/2/11
     * */
    static public function signOn($username, $password){

        self::$link->select_db(DB_BASE);
        $query = "select user_phone, user_pwd from tb_user where user_phone = '{$username}' and user_pwd = '{$password}'";
        $result = self::$link->query($query);

        if($result->num_rows == 1){
            $result->free();
            return JSON::encode_2(CODE_SUCCESS,MSG_SUCCESS);
        };
        $result->free();
        return JSON::encode_2(CODE_FAILED,MSG_FAILED.'用户名或密码已经存在！');
    }

    /*
     * 请求注册 (POST)
     *
     * param
     * \username
     * \password
     * return
     * \json格式的字符串
     * since
     * \2016/2/17
     * */
    static public function login($username, $password){

        if(empty(self::getUID($username,$password))) {
            return JSON::encode_2(CODE_FAILED, MSG_FAILED.'用户名或手机号已经存在！');
        }

        self::$link->select_db(DB_BASE);
        $datetime = date('Y-m-d H:i:s');
        $query = "insert into tb_user values (null, '{$username}', '{$password}', '{$datetime}')";

        if(self::$link->query($query)){
            return JSON::encode_2(CODE_SUCCESS, MSG_SUCCESS);
        }
        return JSON::encode_2(CODE_FAILED,MSG_FAILED.'无法添加用户！');
    }

    /*
     * 提交分数
     *
     * param
     * \username
     * \password
     * \score
     * return
     * \json格式的字符串
     * since
     * \2016/2/11
     */
    static public function upScore($username,$password,$score = 0){

        self::$link->select_db(DB_BASE);
        $query = "update tb_user_custom set user_score = $score where user_id = (select user_id from tb_user where user_phone = '$username') and user_score < $score";

        if(self::$link->query($query)){
            return JSON::encode_2(CODE_SUCCESS,MSG_SUCCESS);
        }
        return JSON::encode_2(CODE_FAILED,MSG_FAILED.'提交失败！');
    }

    /*
     * 获取user_id
     *
     * param
     * \有效的username
     * \有效的password
     * return 如果是无效的用户名和密码，将返回 ''
     * \user_id
     * \''
     * since
     * \2016/2/11
     */
    static public function getUID($username = '',$password = ''){

        self::$link->select_db(DB_BASE);
        $query = "select user_id from tb_user where user_phone = '$username' and user_pwd = '$password'";
        $result = self::$link->query($query);

        if($result->num_rows == 1){
            $data = $result->fetch_assoc();  // var_dump($data) -> array(1) { ["user_id"]=> string(1) "1" }
            $result->free();
            return $data['user_id'];
        }
        $result->free();
        return '';
    }
}

?>