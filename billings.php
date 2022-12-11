<?php
$file=file_get_contents("data.txt",false, null,0, null);
$pieces = explode(" ", $file);
?>
<!doctype html>
<html lang="en">
<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- Bootstrap CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet">
    <title>Cashier</title>
</head>
<body>

<div class="container">
    <div class="row">
        <div class="col-md-12">
            <div class="card mt-4">
                <div class="card-header">
                    <h4>Billings </h4>
                </div>

            </div>
        </div>

        <div class="col-md-12">
            <div class="card mt-4">
                <div class="card-body">
                    <table class="table table-bordered">
                        <thead>
                        <tr>
                            <th style="text-align: center; color: maroon; font-family: Arial; font-weight: bolder"colspan="4">Cart ID: <?php echo $pieces[0];?></th>
                        </tr>
                        <tr>
                            <th>Barcode</th>
                            <th>Product Name</th>
                            <th>Price</th>
                            <th>Weight</th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php
                        $link = mysqli_connect("localhost", "ayaq", "hardwareproj", "cash_system") or die("Error in connecting to server");
                        $result = mysqli_query($link, "select * from prod_info where barcode='".$pieces[1]."' or barcode='".$pieces[2]."'or barcode='".$pieces[3]."'or barcode='".$pieces[4]."'or barcode='".$pieces[5]."'") or die("Error in getting data");
                        $arr=array();
                        $arrbar=array();
                        $arrname=array();
                        $arrprice=array();
                        $arrweight=array();
                        while($row = mysqli_fetch_assoc($result)){
                            foreach ($row as $value) {
                                array_push($arr,$value);
                            }
                        }
                        for ($x = 0; $x < sizeof($arr); $x=$x+4) {
                            array_push($arrbar, $arr[$x]);
                            array_push($arrname, $arr[$x + 1]);
                            array_push($arrprice, $arr[$x + 2]);
                            array_push($arrweight, $arr[$x + 3]);
                        }
                        for($y=0; $y<sizeof($arrbar);$y++){
                            ?>
                            <tr>
                                <td><?php echo $arrbar[$y];?></td>
                                <td><?php echo $arrname[$y];?></td>
                                <td><?php echo $arrprice[$y];?></td>
                                <td><?php echo $arrweight[$y];?></td>
                            </tr>
                            <?php
                        }
                        $sum_price = array_sum($arrprice);
                        $sum_weight = array_sum($arrweight);


                        ?>

                        </tbody>
                    </table>
                    <table class="table table-bordered">
                        <thead>
                        <tr>
                            <th>Total Price:</th>
                            <td><?php echo $sum_price;?>NIS</td>
                        </tr>
                        <tr>
                            <th>Actual Weight:</th>
                            <td><?php echo $sum_weight;?>g</td>
                            <th>Theoretical Weight:</th>
                            <td><?php echo $pieces[6];?>g</td>
                            <?php
                            if($sum_weight!=$pieces[6]){
                                echo '<script>alert("The Weight is not the same..")</script>';
                            }

                            ?>
                        </tr>
                        </thead>
                    </table>
                </div>
            </div>
        </div>
    </div>
</div>

<script src="https://code.jquery.com/jquery-3.5.1.js"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta1/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>

