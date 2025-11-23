@echo off
setlocal enabledelayedexpansion

REM ==============================
set "TARGET_SCRIPT=keilkill.bat"
REM ==============================

echo 正在全盘搜索并执行 %TARGET_SCRIPT% ...
echo 如果遇到 "Could Not Find" 请忽略，那说明已经清理干净了。
echo.

for /r %%f in (%TARGET_SCRIPT%) do (
    if exist "%%f" (
        echo.
        echo [正在处理] %%f
        
        REM 进入目录
        pushd "%%~dpf"
        
        REM ========================================================
        REM 【核心修改】
        REM 使用 cmd /c 来启动一个新的子进程执行脚本
        REM 这样即使 keilkill.bat 里有 exit，也只会关闭这个子进程
        REM 主循环依然活着
        REM ========================================================
        cmd /c "%%~nxf"
        
        REM 恢复目录
        popd
    )
)

echo.
echo ===================================================
echo 全部清理完成！
pause
