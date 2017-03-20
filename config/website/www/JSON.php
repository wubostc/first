<?php
/**
 * Created by PhpStorm.
 * User: ub2
 * Date: 2016/1/20
 * Time: 15:01
 */
require_once ("Config.php");

/*
 * json编码与解码
 *
 * author ub
 *
 * since 2016/1/20
 * */
class JSON{

    /*
     * author   ub
     *
     * since    2016/2/11
     */
    static public function encode_2($code = 0,$message = ''){
        if(!is_numeric($code)){
            return null;
        }
        $value = array(
            JSON_CODE=>$code,
            JSON_MSG=>$message
        );
        return json_encode($value);
    }

    /*
     * author   ub
     *
     * since    2016/2/11
     */
    static public function encode_3($code = 0,$message = '',$obj = array()){
        if(!is_numeric($code) || !is_array($obj)){
            return null;
        }
        $value = array(
            JSON_CODE=>$code,
            JSON_MSG=>$message,
            JSON_EXTE=>$obj
        );
        return json_encode($value);// out   {"code":0, "message":"", exte:{ }}
    }


}


?>