<?php
if(isset($_GET["idcart"])||isset($_GET["barcode1"])||isset($_GET["barcode2"])||isset($_GET["barcode3"])||isset($_GET["barcode4"])||isset($_GET["barcode5"])||isset($_GET["weight"])) {
    $idcart = $_GET["idcart"];
    $barcode1 = $_GET["barcode1"];
    $barcode2 = $_GET["barcode2"];
    $barcode3 = $_GET["barcode3"];
    $barcode4 = $_GET["barcode4"];
    $barcode5 = $_GET["barcode5"];
    $weight = $_GET["weight"];
    $line=$idcart." ".$barcode1." ".$barcode2." ".$barcode3." ".$barcode4." ".$barcode5." ".$weight." ";
    file_put_contents("data.txt", $line,0,null);
}
?>
<!doctype html>
<html lang="en">
<head>
</head>
<body>
<h1 style="text-align: center; color: #7d007d">Select The Cart..</h1>
<a href="billings.php" style="color: #ff0084; text-align: center; margin-left: 25%; font-size: 30px">Cart 1</a>
<a href="billings.php" style="color: #ff0084; text-align: center; margin-left: 10%; font-size: 30px">Cart 2</a>
<a href="billings.php" style="color: #ff0084; text-align: center; margin-left: 10%; font-size: 30px">Cart 3</a>
<a href="billings.php" style="color: #ff0084; text-align: center; margin-left: 10%; font-size: 30px">Cart 4</a>
<img src="313136.jpg" alt="" style="width: 100%; height: 500px">
</body>
</html>