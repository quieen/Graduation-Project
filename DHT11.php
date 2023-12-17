<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <link rel="stylesheet" href="style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.0/css/all.min.css">
    <link rel="stylesheet" href="C:\xampp\htdocs\FINAL02\CSS\fontawesome-free-6.5.1-web\css\all.min.css">
    
</head>
<body>
    <div class="container">
        <div class="menu">
            <div class="logo">
                Smart<span>-Home</span>
            </div>
            
            <ul>
                <li class="active"><i class="fa-solid fa-droplet"></i><a style="color:black; text-decoration:none" href="">Humd & Temp</a></li>
                <li><i class="fa-regular fa-user"></i><a style="color:black; text-decoration:none" href="\FINAL02\Finger.php">Finger List</a></li>
            </ul>
        </div>
        <div class="center">
            <h2>DATABASE DHT11</h2>

            <table class="styled-table" id= "table_id">
                <thead>
                    <tr>
                    <th>NO</th>
                    <th>ID</th>
                    <th>BOARD</th>
                    <th>TEMPERATURE (°C)</th>
                    <th>HUMIDITY (%)</th>
                    <th>STATUS READ SENSOR DHT11</th>
                    <th>TIME</th>
                    <th>DATE (dd-mm-yyyy)</th>
                    </tr>
                </thead>
                <tbody id="tbody_table_record">
                        <?php
                        include 'database.php';
                        $num = 0;
                        //------------------------------------------------------------ The process for displaying a record table containing the DHT11 sensor data and the state of the LEDs.
                        $pdo = Database::connect();
                        // replace_with_your_table_name, on this project I use the table name 'esp32_table_dht11_leds_record'.
                        // This table is used to store and record DHT11 sensor data updated by ESP32. 
                        // This table is also used to store and record the state of the LEDs, the state of the LEDs is controlled from the "home.php" page. 
                        // To store data, this table is operated with the "INSERT" command, so this table will contain many rows.
                        $sql = 'SELECT * FROM `esp32_table_dht11_leds_record` ORDER BY `esp32_table_dht11_leds_record`.`time` DESC';
                        foreach ($pdo->query($sql) as $row) {
                            $date = date_create($row['date']);
                            $dateFormat = date_format($date,"d-m-Y");
                            $num++;
                            echo '<tr>';
                            echo '<td>'. $num . '</td>';
                            echo '<td class="bdr">'. $row['id'] . '</td>';
                            echo '<td class="bdr">'. $row['board'] . '</td>';
                            echo '<td class="bdr">'. $row['temperature'] . '</td>';
                            echo '<td class="bdr">'. $row['humidity'] . '</td>';
                            echo '<td class="bdr">'. $row['status_read_sensor_dht11'] . '</td>';
                            echo '<td class="bdr">'. $row['time'] . '</td>';
                            echo '<td>'. $dateFormat . '</td>';
                            echo '</tr>';
                        }
                        Database::disconnect();
                        //------------------------------------------------------------
                        ?>
                </tbody>
            </table>

            <div class="btn-group">
                <button class="button" id="btn_prev" onclick="prevPage()">Prev</button>
                <button class="button" id="btn_next" onclick="nextPage()">Next</button>
                <div style="display: inline-block; position:relative; border: 0px solid #e3e3e3;  margin-left: 2px;;">
                    <p style="position:relative; font-size: 14px;"> Table : <span id="page"></span></p>
            </div>
                <select name="number_of_rows" id="number_of_rows">
                    <option value="10">10</option>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                </select>
                <button class="button" id="btn_apply" onclick="apply_Number_of_Rows()">Apply</button>
            </div>
        </div>
    </div>
    <br>
    
    

    <br>
    
    <script>
      //------------------------------------------------------------
      var current_page = 1;
      var records_per_page = 10;
      var l = document.getElementById("table_id").rows.length
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function apply_Number_of_Rows() {
        var x = document.getElementById("number_of_rows").value;
        records_per_page = x;
        changePage(current_page);
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function prevPage() {
        if (current_page > 1) {
            current_page--;
            changePage(current_page);
        }
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function nextPage() {
        if (current_page < numPages()) {
            current_page++;
            changePage(current_page);
        }
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function changePage(page) {
        var btn_next = document.getElementById("btn_next");
        var btn_prev = document.getElementById("btn_prev");
        var listing_table = document.getElementById("table_id");
        var page_span = document.getElementById("page");
       
        // Validate page
        if (page < 1) page = 1;
        if (page > numPages()) page = numPages();

        [...listing_table.getElementsByTagName('tr')].forEach((tr)=>{
            tr.style.display='none'; // reset all to not display
        });
        listing_table.rows[0].style.display = ""; // display the title row

        for (var i = (page-1) * records_per_page + 1; i < (page * records_per_page) + 1; i++) {
          if (listing_table.rows[i]) {
            listing_table.rows[i].style.display = ""
          } else {
            continue;
          }
        }
          
        page_span.innerHTML = page + "/" + numPages() + " (Total Number of Rows = " + (l-1) + ") | Number of Rows : ";
        
        if (page == 0 && numPages() == 0) {
          btn_prev.disabled = true;
          btn_next.disabled = true;
          return;
        }

        if (page == 1) {
          btn_prev.disabled = true;
        } else {
          btn_prev.disabled = false;
        }

        if (page == numPages()) {
          btn_next.disabled = true;
        } else {
          btn_next.disabled = false;
        }
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function numPages() {
        return Math.ceil((l - 1) / records_per_page);
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      window.onload = function() {
        var x = document.getElementById("number_of_rows").value;
        records_per_page = x;
        changePage(current_page);
      };
      //------------------------------------------------------------
    </script>

    <script src="script.js"></script>
</body>
</html>