<?php
    include('dbcon.php');
    include('check.php');

    if (is_login()){
        ;
    }else
        header("Location: index.php");

	include('head.php');
?>

<div align="center">
<?php

	$userid = $_SESSION['user_id'];
	$stmt = $con->prepare('select containerid, memo from user_con where userid=:userid');
	$stmt -> bindParam(":userid", $userid);
	$stmt -> execute();

	$count = $stmt->rowCount();
	$stmt -> setFetchMode(PDO::FETCH_ASSOC);

	$i=0;
	while($row = $stmt->fetch()){
		$container[$i] = $row['containerid'];
		$memo[$i] = $row['memo'];
		$i++;
	}

?>

<!DOCTYPE html>
<html lang="ko">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<title>container select</title>
	<style>
	body{
		font-family: Consolas, monospace;
		font-family: 12px;
	}
	table{
		width: 50%;
	}
	th, td{
		padding: 10px;
		border-bottom: 1px solid #dadada;
	}
	</style>
</head>
<body>

<h3>[AVAILABLE CONTAINER LIST]</h3>
<table>
<thead>
	<tr bgcolor=#eeffcc>
	<th>CONTAINER</th>
	<th>MEMO</th>
	<th>SELECT</th>
	</tr>
</thead>
<tbody>
<?php
	for($i=0;$i<$count;$i++){
		echo "<tr><td>".$container[$i]."</td><td>".$memo[$i]."</td>";
		echo "<td><a href=\"container_info.php?num=$container[$i]\">select</a></td></tr>";
	}
?>
</tbody>
</table>

</div>

</body>
</html>
