OpenRTM-aist�Ή��@�ړ����{�b�g�i�r�Q�[�V�����p�t���[�����[�N

����̉��

Mapper_ALL.bat�̓��e���݂�ƁC�܂��Crtcd.exe�Ƃ����A�v���P�[�V������
�u-f conf/mapper_rtc_cpp.conf�v�Ƃ��������Ŏ��s���Ă���̂��킩��D
rtcd��RTC Daemon�ƌĂ΂��A�v���P�[�V�����ŁC�ݒ�t�@�C����ύX���邱�ƂŁC
DLL�`����RT�R���|�[�l���g�ǂݍ���ŁC��C�ɋN�����邱�Ƃ��ł���D
java�Ɋւ��Ă����l�ɁC�N���X�p�X��ݒ肵�Ȃ���Crtcd.jar�̒��́Crtcd.rtcd�N���X��
���s���Ă���C���ꂪjava�łɂȂ�D

�������ݒ�t�@�C���ł���Cmapper_rtc_cpp.conf���m�F����ƁC�Ō�̍s�ɁC

manager.modules.preload:UrgRTC.dll,Mapper_MRPT.dll,SpurRTC.dll
manager.components.precreate:UrgRTC,Mapper_MRPT,SpurRTC
manager.modules.load_path:bin

Navigation.Mapper_MRPT0.config_file:conf/Mapper_MRPT0.conf
Sensor.UrgRTC0.config_file:conf/UrgRTC0.conf
MobileRobot.SpurRTC0.config_file:conf/SpurRTC0.conf

�̂悤�ɏ�����Ă���D

