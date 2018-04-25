<?php

function GetMatr($mas, $i, $j, $m) {
    $p[0][0] = 0;
    $ki;
    $kj;
    $di;
    $dj;
    $di = 0;
    for ($ki = 0; $ki<$m-1; $ki++) { // проверка индекса строки
      if ($ki == $i) $di = 1;
      $dj = 0;
      for ($kj = 0; $kj<$m-1; $kj++) { // проверка индекса столбца
        if ($kj == $j) $dj = 1;
        $p[$ki][$kj] = $mas[$ki + $di][$kj + $dj];
      }
    }
    return $p;
  }

function Determinant($mas, $m) {
    $p[0][0] = 0;
    $j = 0;
    $d = 0;
    $k = 1; //(-1) в степени i
    $n = $m-1;
    if ($m<1) 
        return 0;
    if ($m == 1) {
        $d = $mas[0][0];
      return($d);
    }
    if ($m == 2) {
        $d = $mas[0][0] * $mas[1][1]-($mas[1][0] * $mas[0][1]);
      return($d);
    }
    if ($m>2) {
      for ($i = 0; $i<$m; $i++) {
        $p = GetMatr($mas, $i, 0, $m);
        $d = $d + $k * $mas[$i][0] * Determinant($p, $n);
        $k = -1*$k;
      }
    }
    return($d);
  }

    for($i = 0; $i < $DIM; $i++){
        for($j = 0; $j < $DIM; $j++){
            $index = $i*$DIM + $j;
            $str = "a{$index}";
            $a[$i][$j] = $_POST[$str];
            if(!is_numeric($a[$i][$j])){
                $a[$i][$j] = 0;
            }               
        }
    }


    $DET = Determinant($a, $DIM);

    if(!$DET)
    {
        echo "Determinant is 0";
    }
    else
    {
        for($j = 0; $j < $DIM; $j++){
            $str = "b{$j}";
            $b[$j] = $_POST[$str];
            if(!is_numeric($b[$j])){
                $b[$j] = 0;
            }               
        }
        for($j = 0; $j < $DIM; $j++){
            $x[$j] = 0;              
        }

        for ($k = 0; $k < $DIM; $k++) // прямой ход
        {
            for ($j = $k+1; $j < $DIM; $j++)
            {
                $d = $a[$j][$k] / $a[$k][$k]; // формула (1)
                for ($i = $k; $i < $DIM; $i++)
                {
                    $a[$j][$i] = $a[$j][$i] - $d * $a[$k][$i]; // формула (2)
                }
                $b[$j] = $b[$j] - $d * $b[$k]; // формула (3)
            }
        }

        for ($k = $DIM - 1; $k >= 0; $k--) // обратный ход
        {
            $d = 0;
            for ($j = $k; $j < $DIM; $j++)
            {
                $s = $a[$k][$j] * $x[$j]; // формула (4)
                $d = $d + $s; // формула (4)
            }
            $x[$k] = ($b[$k] - $d) / $a[$k][$k]; // формула (4)
        }

        echo "<center><h4>Result:<br>";
        for($j = 0; $j < $DIM; $j++){
            echo "x{$j} = ", $x[$j], "<br>";              
        }
        echo "</h4></center>";
    }
?>
