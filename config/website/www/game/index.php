<html>
	<body>
		<h1>\www\game</h1>
		<?php 
			//phpinfo();
		
			$connect = mysql_connect("127.0.0.1:3306","ub","123456");
			if(!$connect)
				echo "connect error";
			else
				echo "connect success!";
			
			mysql_close();
		?>
	</body>
</html>
