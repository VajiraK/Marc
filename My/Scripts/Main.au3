dim $i,$j
dim $msgdelay[15]

$msgdelay[1] = 20
$msgdelay[2] = 20
$msgdelay[3] = 20
$msgdelay[4] = 30
$msgdelay[5] = 30
$msgdelay[6] = 35
$msgdelay[7] = 35
$msgdelay[8] = 35
$msgdelay[9] = 40
$msgdelay[10] = 45
$msgdelay[11] = 45
$msgdelay[12] = 50


 For $i = 1 To 13
	ShellExecute("D:\Vajira\Programing\C++\Marc\My\Scripts\" & $i & ".au3","","","Run")
	$j = $i + 1
	MsgBox(48, "Script", "Run script " & $j,$msgdelay[$i])
 Next
