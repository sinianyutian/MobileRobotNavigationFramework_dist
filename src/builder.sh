# Builder.sh

build_cpp_rtc() {
  cd $1
  mkdir -p build-linux
  cd build-linux
  cmake ../
  make
  cp src/$1.so ../../../bin/
  cd ../../
}
build_cpp_rtc Localization_MRPT
build_cpp_rtc SpurRTC
build_cpp_rtc Mapper_MRPT
build_cpp_rtc SimplePathFollower
build_cpp_rtc UrgRTC
build_cpp_rtc PathPlanner_MRPT
build_cpp_rtc SFMLJoystick
build_cpp_rtc SFMLJoystickToVelocity
