@echo off
title File Structure Compare
powershell "Function Get-Folder(){[System.Reflection.Assembly]::LoadWithPartialName(\"System.windows.forms\")|Out-Null;$foldername = New-Object System.Windows.Forms.FolderBrowserDialog;$foldername.Description = \"Select folder\";$foldername.rootfolder = \"MyComputer\";$foldername.SelectedPath = \"\";if($foldername.ShowDialog() -eq \"OK\"){$folder += $foldername.SelectedPath;}return $folder;}$a = Get-Folder;$b = Get-Folder;Write-Host $a;%~dp0\fileStructCmp.exe $a $b v | tee outl.txt"
pause
notepad outl.txt
del outl.txt
