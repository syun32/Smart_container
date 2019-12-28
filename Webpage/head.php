<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; 
    charset=UTF-8" />
<title>CONTAINER</title>
</head>

<body>
<nav class="navbar navbar-default navbar-static-top">
    <div class="container-fluid">
        <div class="navbar-header">
            <a class="navbar-brand"><h2 align="center">SMART CONTAINER</h2></a>
			<ul class="nav navbar-nav">
            <li class="active"><a href="index.php">HOME</a></li>
            <?php if (isset($_SESSION['user_id'])) { ?>
                <li><a href="welcome.php">USER ID : <?php echo $_SESSION['user_id']; ?></a></li>
                <li><a href="logout.php">LOGOUT</a></li><hr>
            <?php } else { ?>
                <li><a href="index.php">Login</a></li>
             <?php } ?>
			</ul>
        </div>
    </div>
</nav>
