<?php
	/*------------------------------------------------------------------*/
	function connectDB(){
		///Database variables
		$bdServer = 'Localhost'; 					//'127.0.0.1';
		$bdUser = 'id6767071_benitez';				//'root';
		$bdPassword = '142130leo';					//'';
		$bdName = 'id6767071_receiveddata';			//'receiveddata';
	
		///Connection
		$_connection = mysqli_connect($bdServer, $bdUser, $bdPassword, $bdName);
		if (mysqli_connect_errno($_connection)) {
			echo "Problemas para conectar no banco. Verifique os dados!";
			die();
		}
		return $_connection;
	}
	
	/*------------------------------------------------------------------*/
	/// Devolve os dados contidos na tabela 
	function searchData($_connection, $_limit) {
		$sqlSelect = 'SELECT * FROM `raw_data` ORDER BY id DESC LIMIT ' . $_limit . ' ';
		$result = mysqli_query($_connection, $sqlSelect);
		$data = array();
		while ($line = mysqli_fetch_assoc($result)) {
			$data[] = $line;
		}
		return $data;
	}
	
	/*------------------------------------------------------------------*/
	// Insere os dados na tabela
	function insertData($_connection, $_data){
		$sqlInsert = "
			INSERT INTO raw_data
			(team, instaled, volume, color, received)
			VALUES
			(
				'{$_data['team']}',
				'{$_data['instaled']}',
				'{$_data['volume']}',
				'{$_data['color']}',
				'{$_data['date']}'
			)
		";

		echo "Insert result: ";
		//mysqli_query($_connection, $sqlInsert);
		if (mysqli_query($_connection, $sqlInsert)) return "sucess";
		else return "fail";
	}
	
	/*------------------------------------------------------------------*/
	function closeDB($_conexão){
		mySQLi_close ($_conexão);
		//TODO: verify error
	}
?>