# sudo add-apt-repository ppa:joseluisblancoc/mrpt
# sudo apt-get update
# sudo apt-get install libmrpt-dev mrpt-apps  

#git clone https://github.com/sugarsweetrobotics/NavigationManager.git
#git clone https://github.com/sugarsweetrobotics/StringCommander.git
git clone https://github.com/sugarsweetrobotics/MapServer.git
git clone https://github.com/sugarsweetrobotics/SpurRTC.git
git clone https://github.com/sugarsweetrobotics/UrgRTC.git
cd UrgRTC
git submodule init
git submodule update
cd ../
git clone -b ubuntu1404 https://github.com/sugarsweetrobotics/Mapper_MRPT.git 
git clone -b ubuntu1404 https://github.com/sugarsweetrobotics/Localization_MRPT.git
git clone -b ubuntu1404 https://github.com/sugarsweetrobotics/PathPlanner_MRPT.git
git clone https://github.com/sugarsweetrobotics/SimplePathFollower.git
git clone https://github.com/sugarsweetrobotics/SFMLJoystick.git
git clone https://github.com/sugarsweetrobotics/SFMLJoystickToVelocity.git

