<!DOCTYPE html>
<html>
<head>
  <title>Enter Values</title>
</head>
<body bgcolor='#FBEC5D'>
    <?php
            if(isset($_POST['N']) && is_numeric($_POST['N'])){
                $DIM = $_POST['N'];
            }
            else {
                $DIM = 4;
            }
            if($DIM > 100) {
              $DIM=100;
              echo "max Dimension is 100";
            }
            if(isset($_POST['solve'])) {
              include 'solve.php';
            }
    ?>
  <center>
    <form action='/gauss.php' method='POST'>
      <table>
        <tr>
          <td>
            <table border='1'>
              <?php
                for ($i = 0; $i < $DIM; $i++) 
                {
                   echo "\t\t<tr>\n";
                   for ($j = 0; $j < $DIM; $j++) 
                   {
                     $val = 0;
                     if($i == $j){
                       $val = 1;
                     }
                     echo "\t\t\t<td height='25'><input type='text' name='a", $i*$DIM+$j, "' SIZE='4' align='right' value='",$val,"'></td>\n";
                   }
                   echo "\t\t</tr>\n";
                }
              ?>
            </table>
          </td>
          <td align="CENTER">
            <b>x</b>
          </td>
          <td>
            <table border="1">
              <?php
                for ($i = 0; $i < $DIM; $i++) 
                   {
                     echo "\t\t<tr><td height='25'><B><I>x<SUB>", $i, "</SUB></I></B></td></tr>\n";
                   }
              ?>
            </table>
          </td>
          <td align="center">
            <B>=</B>
          </td>
          <td>
            <table border="1">
            <?php
                for ($i = 0; $i < $DIM; $i++) 
                   {
                     echo "\t\t<tr><td height='25'><input type='text' name='b", $i, "' SIZE='4' ALIGN='RIGHT' VALUE='0'></td></tr>\n";
                   }
              ?>
            </table>
          </td>
        </tr>
      <tr>
        <td COLSPAN="6" align="center"><input TYPE="submit" name="solve"></td>
        <input type="hidden" value="<?php echo $DIM; ?>" name="N">
      </tr>
      </table>
    </form>
  </center>
    
</body>
</html>