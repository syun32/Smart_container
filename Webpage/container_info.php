
<?php
	include('dbcon.php');
	include('check.php');

	if(is_login()){
		;
	}else
		header("Location: index.php");

	include('head.php');

	$_SESSION['con_num'] = $_GET["num"];
	$con_num = $_SESSION['con_num'];
?>
<?php

	$stmt = $con->prepare("SELECT * FROM containers WHERE ID=:con_num ORDER BY containers.update_time DESC LIMIT 1");
	$stmt -> bindParam(":con_num",$con_num);
	$stmt -> execute();

	$row = $stmt -> fetch(PDO::FETCH_ASSOC);

?>

<html>
<head>
	<title>Container 0001</title>
	<style>
	body{
		font-family: Consolas, monospace;
		font-family: 12px;
	}
	table{
		width: 60%;
	}
	th, td{
		padding: 10px;
		border-bottom: 1px solid #dadada;
	}
	</style>
</head>

<body>
	<table id='Test' align=center><col bgcolor=#eeffcc>
	<tr bgcolor=#ffffbb>
	<td> </td>
	<td colspan="2">VALUES</td>
	<td>SELECT</td></tr>
	<tr>
	<td>container ID</td>
	<td colspan="3"><?php echo $con_num;?></td></tr>
	<tr>
	<td>temperature</td>
	<td colspan="2"><?php echo $row['temp'];?> C</td>
	<td><a href="temp.php">select</a></td></tr>
	<tr>
	<td>cooler</td>
	<td colspan="2"><?php echo $row['cool'];?></td>
	<td><a href="temp.php">select</a></td></tr>
	<tr>
	<td>humidity</td>
	<td colspan="2"><?php echo $row['humi'];?>%</td>
        <td><a href="humi.php">select</a></td></tr>
	<tr>
	<td>security</td>
	<td colspan="2"><?php echo $row['security'];?></td>
        <td><a href="security.php">select</a></td></tr>
	<tr>
	<td rowspan="2">location</td>
	<td>longitude</td>
	<td><?php echo $row['longitude'];?></td>
        <td rowspan="2"><a href="location.php">select</a></td></tr>
	<tr>
	<td>latitude</td>
	<td><?php echo $row['latitude'];?></td></tr>

	</table>


<script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyBNAdajza9d0Gwqn7K1-uWcG2X4AQiMDlU&callback=initialize"></script>
<script>
  function initialize() {
  var myLatlng = new google.maps.LatLng(<?php echo $row['longitude'].",".$row['latitude'];?>);
  var mapOptions = {
        zoom: 14,
        center: myLatlng,
        mapTypeId: google.maps.MapTypeId.ROADMAP
  }
  var map = new google.maps.Map(document.getElementById('map_canvas'), mapOptions);
  var marker = new google.maps.Marker({
position: myLatlng,
map: map,
title: "선택 위치"
});
  }
window.onload = initialize;
</script>
<center>
<div id="map_canvas" style="width: 50%; height: 400px; margin:50px;"></div>
</center>


</body>
</html>
