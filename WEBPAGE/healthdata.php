<?php
$servername = "localhost";
$username = "root";
$password = ""; 
$database = "mini_project";
$conn = new mysqli($servername, $username, $password, $database);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
$sql = "SELECT 
            AVG(temp) AS avg_temp,
            AVG(co2) AS avg_co2,
            AVG(hr) AS avg_hr,
            AVG(hrv) AS avg_hrv,
            AVG(shr) AS avg_shr,
            AVG(spo2) AS avg_spo2
        FROM (
            SELECT 
                temp,
                co2,
                hr,
                hrv,
                shr,
                spo2
            FROM mini_project_sensor_data
            ORDER BY id DESC
            LIMIT 5
        ) AS top_5_records";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $tt = number_format($row["avg_temp"], 3);
        $cc = number_format($row["avg_co2"], 3);
        $hh = number_format($row["avg_hr"], 3);
        $hrv = number_format($row["avg_hrv"], 3);
        $shr = number_format($row["avg_shr"], 3);
        $spo2 = number_format($row["avg_spo2"], 3);
    }
} else {
}
$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="refresh" content="10">
    <title>UR HEALTH DATA IS HERE</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz" crossorigin="anonymous"></script>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.2/css/all.min.css" integrity="sha512-SnH5WK+bZxgPHs44uWIX+LLJAJ9/2PkPKZ5QiAj6Ta86w+fsb2TkcmfRyVX3pBnMFcV7oQPJkl9QevSCWr3W6A==" crossorigin="anonymous" referrerpolicy="no-referrer" />
    <style>
        * {
            padding: 0;
            margin: 0;
        }
        ::-webkit-scrollbar {
            width: 0;
        }
        body {
            width: 100vw;
            height: 100vh;
            /* background: linear-gradient(#567,#687); */
            background: linear-gradient(270deg, #f08b8b, #ff9900, #6fe56f, #75d1f0, #9933ff);
            background-size: 1000% 1000%;
            animation: colorAnimation 20s ease infinite;
        }
        @keyframes colorAnimation {
            0% {
                background-position: 0% 50%;
            }
            50% {
                background-position: 100% 50%;
            }
            100% {
                background-position: 0% 50%;
            }
        }

        .navbar {
            backdrop-filter: blur(5rem);
            border-radius: 10px; 
            padding: 20px; 
        }

        .data-sen {
            text-align: right;
        }

        .pred-btn {
            max-width: fit-content;
        }

        .data-field-bmi {
            display: flex;
            align-items: center;
            justify-content: center;
        }
    </style>
</head>
<body>
    <div class="container">
        <nav class="navbar navbar-expand-lg navbar-light bg-light m-5 px-5 d-flex justify-content-between align-items-center">
            <a class="navbar-brand" href="https://gmrit.edu.in/index.php">
            <img src="images/logo.png" width="100" height="50" class="d-inline-block align-top" alt="">
            </a>
            <div class="display-6">Vital Sync Plus</div>
            <a href="http://127.0.0.1:5500/home.html"><i class="fa-solid fa-house fs-4"></i></a>
        </nav>

        <!-- DATA -->
        <div class="row m-5">
            <div class="col bg-light rounded">
                <div class="display-3">YOUR HEALTH DATA IS HERE</div>
            </div>
        </div>
        <div class="row m-5 bg-light p-3 abc">
            <div class="col bg-warning p-3">
                <div class="row m-2 display-6">TEMPERATURE</div>
                <div class="row m-2 display-6">CARBON DIOXIDE</div>
                <div class="row m-2 display-6">HEART RATE</div>
                <div class="row m-2 display-6">HEART RATE VARIABILITY</div>
                <!--<div class="display-6">HEART RATE</div>-->
                <div class="row m-2 display-6">BLOOD OXYGEN LEVEL</div>
            </div>
            <div class="col-8 bg-warning p-3 data-sen">
            <div class="col progress m-2" style="height: fit-content;">
                <div class="progress-bar progress-bar-striped display-6" role="progressbar" style="width: <?php echo ($tt / 100) * 100; ?>%;" aria-valuenow="<?php echo $tt; ?>" aria-valuemin="0" aria-valuemax="100">
                    <?php echo $tt; ?>° C
                </div>
            </div>
            <div class="col progress m-2" style="height: fit-content;">
                <div class="progress-bar bg-secondary progress-bar-striped display-6" role="progressbar" style="width: <?php echo ($cc / 70) * 100; ?>%;" aria-valuenow="<?php echo $cc; ?>" aria-valuemin="0" aria-valuemax="70">
                    <?php echo $cc; ?> PPM
                </div>
            </div>
            <div class="col progress m-2" style="height: fit-content;">
                <div class="progress-bar bg-info progress-bar-striped display-6" role="progressbar" style="width: <?php echo ($shr / 130) * 100; ?>%;" aria-valuenow="<?php echo $shr; ?>" aria-valuemin="0" aria-valuemax="130">
                    <?php echo $shr; ?> HR
                </div>
            </div>
            <div class="col progress m-2" style="height: fit-content;">
                <div class="progress-bar bg-danger progress-bar-striped display-6" role="progressbar" style="width: <?php echo ($hrv / 15) * 100; ?>%;" aria-valuenow="<?php echo $hrv; ?>" aria-valuemin="0" aria-valuemax="15">
                    <?php echo $hrv; ?> HRV
                </div>
            </div>
            <div class="col progress m-2" style="height: fit-content;">
                <div class="progress-bar bg-success progress-bar-striped display-6" role="progressbar" style="width: <?php echo ($spo2 / 120) * 100; ?>%;" aria-valuenow="<?php echo $spo2; ?>" aria-valuemin="0" aria-valuemax="120">
                    <?php echo $spo2; ?>%
                </div>
            </div>


            </div>            
        </div>

        <!--    UR DATA IS HERE -->
    <!--
        <div class="row m-4 mb-0">
            <div class="col">
                <div class="display-1 text-warning">KNOW UR BMI</div>
                <div class="row-fluid data-field-bmi bg-light p-4 fs-2">
                    <div class="col-1"></div>
                    <div class="col">
                        <label for="weight">ENTER THE WEIGHT (kg's)</label>
                        <br><br>
                        <label for="height">ENTER THE HEIGHT (mt's)</label>
                    </div>
                    <div class="col text-center">
                        <input type="number" class="border-1 bmi-weight px-4" value="0" required>
                        <br><br>
                        <input type="number" class="border-1 bmi-height px-4" value="0" required>
                    </div>
                    <div class="col-1"></div>
                </div>
                <div class="col text-center m-2">
                    <input type="submit" class="fs-2 p-2 rounded-pill outline-none border-0 bmi-submit">
                    <div class="display-6 text-info bmi-op">View The Results Here</div>
                </div>  
            </div>
        </div>
    -->
        <!-- HEALTH PREDICTION -->
        <div class="row justify-content-evenly mx-5 my-0">
            <div class="col p-2 pred-btn">
                <div class="display-6 w-auto p-2 bg-light pred-btn" style="border-radius: 1.2rem;"><a href="http://127.0.0.1:5000/" class="text-decoration-none">Go & Get Prediction Stats through sms</a></div>
            </div>
        </div>
        <div class="row"></div>
    </div>

    <!--    SCRIPT -->
    <script>
        // Refresh the page every 3 seconds
        setInterval(function() {
            abc.reload();
        }, 3000);
    </script>
</body>
</html>