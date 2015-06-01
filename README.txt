OpenRTM-aist対応　移動ロボットナビゲーション用フレームワーク

動作の解説

Mapper_ALL.batの内容をみると，まず，rtcd.exeというアプリケーションを
「-f conf/mapper_rtc_cpp.conf」という引数で実行しているのがわかる．
rtcdはRTC Daemonと呼ばれるアプリケーションで，設定ファイルを変更することで，
DLL形式のRTコンポーネント読み込んで，一気に起動することができる．
javaに関しても同様に，クラスパスを設定しながら，rtcd.jarの中の，rtcd.rtcdクラスを
実行しており，これがjava版になる．

引数が設定ファイルであり，mapper_rtc_cpp.confを確認すると，最後の行に，

manager.modules.preload:UrgRTC.dll,Mapper_MRPT.dll,SpurRTC.dll
manager.components.precreate:UrgRTC,Mapper_MRPT,SpurRTC
manager.modules.load_path:bin

Navigation.Mapper_MRPT0.config_file:conf/Mapper_MRPT0.conf
Sensor.UrgRTC0.config_file:conf/UrgRTC0.conf
MobileRobot.SpurRTC0.config_file:conf/SpurRTC0.conf

のように書かれている．

