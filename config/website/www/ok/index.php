<?php
require_once ("../Listener.php");
require_once ("../Config.php");


//require_once ("../JSON.php");
/*
$arr  = array("code"=>1,"message"=>"","exte"=>array(array("nickname"=>"qq","sc"=>88),array(1000),array("jpg")));
var_dump(json_encode($arr));
*/
if(isset($_POST[TODO])) {
	if (strcmp($_POST[TODO], SIGN_ON) == 0) {
		Listener::singOn();
	}
	else if (strcmp($_POST[TODO], LOGIN) == 0) {
		Listener::login();
	}
	else if (strcmp($_POST[TODO], UP_SCORE) == 0) {
		Listener::upScore();
	}
	else if(strcmp($_POST[TODO],SEEK_SCORE) == 0){
		Listener::tell();
	}
	else {
		echo MSG_FAILED."access denied!";
		exit;
	}
}
?>
