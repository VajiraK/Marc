Dim $bat,$map,$step,$smgdelay,$count
Dim $i,$m,$tile,$batch,$file

;ATTRIBUTES***
$bat 	= 100000
$count	= 20
$batch 	= 12
$map 	= 5
$step	= 5000
$smgdelay = 6
;ATTRIBUTES***

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
$file = FileOpen ( "D:\Vajira\Desktop\ResutsG.txt", 1 )
FileWrite($file, "BR" & $batch & "-" & $map & " Gradient Run" & @CRLF)
FileWrite($file, "------------------------------" & @CRLF)
FileClose($file)
;Open Map
  Send("^o")
  WinWaitActive("Open")
  Send("!n") 
  $tile = "b" & $batch & "_m" & $map & ".mmmf"
  Send("D:\Vajira\Marc\maps\" & $batch & "\" & $tile)
  Send("{ENTER}")

;-----------------------------------------------------------------
 For $i = 1 To $count
  Send("s")

  WinWaitActive("Settings")
  Send("{TAB}")
  Send($bat) 
  ;Check for errors
  if ControlGetText("", "", "Edit2")<>$bat then
	MsgBox(4096, "Script", "Unable to set Bat!")
	ExitLoop 
  endif
  Send("{ENTER}")
  Run("D:\Vajira\Desktop\Ver 10 Release.exe", "D:\Vajira\Desktop")
  ;Promt
  MsgBox(4096, "Script", "Run Next " & $i,$smgdelay)
  
  ;Misson copleted
dim $rem_bat,$score,$hel

  WinActivate("Spaceship")
  $scoret = ControlGetText("", "", "Static3")
  $rem_bat = ControlGetText("", "", "Static1")
  $hel = ControlGetText("", "", "Static2")
  
  ;Write to the file
  $file = FileOpen ( "D:\Vajira\Desktop\ResutsG.txt", 1 )
  FileWrite($file, $scoret & @TAB & $rem_bat & @TAB & $hel  & @TAB & $bat & @TAB & $i & @CRLF)
  FileClose($file)
  ;Changed Bat
  $bat = $bat - $step
 Next
 ;-----------------------------------------------------------------
 
 ;Close file
 FileWrite($file,@CRLF)
 FileClose($file)
