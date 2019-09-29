@echo OFF
REM '待处理的Proto文件'
@echo "-----------Proto-file------------------"
for /R ".\" %%i in (*.proto) do (E:\Qt_PRJ\goodPrj\QtProject\SuperGame\base\network\protobuffer\proto\protoc.exe -I=%~dp0 --cpp_out=..\include  %%i
@echo "exec->file:%%i ")
REM '生成的golang文件'
@echo "------------Go-file--------------------"
for /R "..\go" %%s in (*.cpp) do (@echo "creating->file:%%s")

REM '区分文件夹和文件'
REM   timeout 3
    REM for /f "delims=" %%i in ('dir /a/b/s') do pushd "%%i" 2>nul && (call :folder "%%i" & popd) || call :file "%%i"
    REM pause
REM goto :eof
REM :file
	REM echo creating: %~1 file is OK!
REM goto :eof
REM :folder
	REM REM echo %~1 is Folder!
REM goto :eof
PAUSE
REM timeout 3
Exit