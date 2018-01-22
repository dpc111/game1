del /f /s /a csv\*.csv

del /f /s /a csv\*.txt

..\tools\xls2csv\xls2csv.exe xls csv default

del /f /s /a csv\*.txt

lua ..\tools\csvRename.lua