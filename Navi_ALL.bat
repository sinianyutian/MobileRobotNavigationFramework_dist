set PATH=%~dp0\bin;%PATH%
start bin\rtcd.exe -f conf\navi_rtc_cpp.conf
java -cp bin\rtcd.jar;bin\commons-cli-1.1.jar;bin\OpenRTM-aist-1.1.0.jar;bin\jyaml-1.3.jar rtcd.rtcd -f conf\navi_rtc_java.conf
