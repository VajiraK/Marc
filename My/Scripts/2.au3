Dim $bat[5]
Dim $i,$m,$tile,$batch,$file,$msgdelay
$bat[0] = "5000"
$bat[1] = "7500"
$bat[2] = "10000"
$bat[3] = "15000"
$bat[4] = "20000"
$batch  = 2
$msgdelay = 2

;Match any part
Opt("WinTitleMatchMode", 2)
;Is it load
If WinActivate("MARC Spaceship")<>1 Then
Run("C:\Program Files\Marc\bin\Spaceship.exe", "D:\Vajira\Marc\maps")
EndIf
;Prepaire
WinWaitActive("Spaceship","",1)
WinMove("Spaceship", "", 50, 50, 300, 135)
;Open log file
;$file = FileOpen ( "D:\Vajira\Desktop\Test " & @MON & "-" & @MDAY & " " & @HOUR & "." & @MIN & ".txt", 1 )
$file = FileOpen ( "D:\Vajira\Desktop\Resuts.txt", 1 )
FileWrite($file, $batch & @CRLF)
FileWrite($file, "------------------------------" & @CRLF)

$m = 1
 For $i = 0 To 4
  Send("^o")
  WinWaitActive("Open")
  Send("!n") 
  $tile = "b" & $batch & "_m" & $m & ".mmmf"
  Send("D:\Vajira\Marc\maps\" & $batch & "\" & $tile)
  $m = $m + 1
  Send("{ENTER}")
  Send("s")

  WinWaitActive("Settings")
  Send("{TAB}")
  Send($bat[$i]) 
  ;Check for errors
  if ControlGetText("", "", "Edit2")<>$bat[$i] then
	MsgBox(4096, "Script", "Unable to set Bat!")
  endif
  Send("{ENTER}")
  Run("D:\Vajira\Desktop\Ver 10 Release.exe", "D:\Vajira\Desktop")
  MsgBox(4096, "Script", "Run Next!",$msgdelay)
  
  ;Misson copleted
dim $rem_bat,$score,$hel

  WinActivate("Spaceship")
  $scoret = ControlGetText("", "", "Static3")
  $rem_bat = ControlGetText("", "", "Static1")
  $hel = ControlGetText("", "", "Static2")
  
  ;Write to the file
  FileWrite($file, $scoret & @TAB & $rem_bat & @TAB & $hel  & @TAB & $bat[$i] & @CRLF)
 Next
 
 ;Close file
 FileWrite($file,@CRLF)
 FileClose($file)
