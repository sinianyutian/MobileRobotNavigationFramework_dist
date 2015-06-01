set PATH=%PATH%;%~dp0\bin
echo %PATH%
rem start call ypspur_coordinator.bat
bin\rtcd.exe -f conf\mapper_rtc_cpp.conf
rem start UrgRTCComp.exe
rem start SpurRTCComp.exe
rem start java -cp NavigationManager.jar;commons-cli-1.1.jar;OpenRTM-aist-1.1.0.jar; NavigationManagerComp