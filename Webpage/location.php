<?php
	include ('dbcon.php');
	include ('check.php');
	include ('head.php');

        $stmt = $con->prepare('SELECT update_time, longitude, latitude FROM containers WHERE ID=:con_num');
        $stmt -> bindParam(":con_num",$_SESSION['con_num']);
        $stmt -> execute();


	$count = $stmt->rowCount();
	$stmt->setFetchMode(PDO::FETCH_ASSOC);

	$i=0;
	while($row = $stmt->fetch()){
		$con_time[$i]=$row['update_time'];
		$con_longitude[$i]=$row['longitude'];
		$con_latitude[$i]=$row['latitude'];
		$i++;
	}


?>
<!DOCTYPE html>
<html lang="ko">
    <head> 
        <meta http-equiv="Content-Type"  content="text/html; charset=UTF-8" /> 
        <title>container information</title>
        <style>
        body{
		font-family: Consolas, monospace;
		font-family: 12px;
	}
	table{
		width: 70%;
	}
	th, td{
		padding: 10px;
		border-bottom: 1px solid #dadada;
	}
        </style>
    </head>
    <body>

	<div align="center">

        <h3>CONTAINER ID : <?php echo $_SESSION['con_num'];?></h3>
        <table>
	<thead>
		<tr bgcolor=#ffffbb>
		<th>TIME</th>
		<th>TEMPERATURE</th>
		<th>COOLER</th>
		<th>SELECT</th>
		</tr>
	<thead>
	<tbody>
<?php
	for($i=0;$i<$count;$i++){
		echo "<tr><td>". $con_time[$i]."</td><td>".$con_longitude[$i]."</td><td>".$con_latitude[$i]."</td>";
		echo "<td><a href=\"map.php?value=$con_time[$i]\">select</a></td></tr>";
	}
?>

	</tbody>
        </table>
    </body>
</html>
