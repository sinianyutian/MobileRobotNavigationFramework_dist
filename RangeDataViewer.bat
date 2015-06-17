set PATH=%~dp0\bin;%PATH%
java -cp bin\rtcd.jar;bin\commons-cli-1.1.jar;bin\OpenRTM-aist-1.1.0.jar;bin\jyaml-1.3.jar rtcd.rtcd -f conf\range_data_viewer.conf
