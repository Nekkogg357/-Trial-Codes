<?php
	include("bd.php");
	$id = $_GET['id'];

	if($resBD == true){
		$consulta = "SELECT * FROM rgb WHERE id_rgb = $id";
	
		$resultados = $conexion->query($consulta);
		#print_r($resultados);
		$datos = [];
		
		foreach($resultados as $res){
			$datos[] = [
				"id" => $res["id_rgb"],
				"R" => $res["rojo"],
				"G" => $res["verde"],
				"B" => $res["azul"],
				"nombre" => $res["nombre_color"]
			];
		}
		#print_r($datos);
	
	}

	echo json_encode($datos);
?>