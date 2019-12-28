<?php
	include('dbcon.php');
	include('check.php');

	include('head.php');

	$_SESSION['update_time']=$_GET["value"];
	$con_time = $_SESSION['update_time'];

	$stmt = $con->prepare('select longitude, latitude from containers where update_time=:con_time AND ID=:con_num');
	$stmt -> bindParam(":con_num", $_SESSION['con_num']);
	$stmt -> bindParam(":con_time", $con_time);
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
		width: 70%;
	}
	th, td{
		padding: 10px;
		border-bottom: 1px solid #dadada;
	}

	</style>
</head>

<body>

<div align = "center">

<table>
<thead>
	<tr bgcolor=#ffffbb>
	<th>CONTAINER ID</th>
	<th>TIME</th>
	<th>LONGITUDE</th>
	<th>LATITUDE</th>
	</tr>
</thead>
<tbody>
	<tr>
	<td><?php echo $_SESSION['con_num'];?></td>
	<td><?php echo $con_time."</td><td>".$row['longitude']."</td><td>".$row['latitude'];?>
	</td><tr>
</tbody>
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
<div id="map_canvas" style="width: 70%; height: 400px; margin:50px;"></div>
</center>


</body>
</html>
