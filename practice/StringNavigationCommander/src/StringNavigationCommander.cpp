// -*- C++ -*-
/*!
 * @file  StringNavigationCommander.cpp
 * @brief Navigation Manager RTC
 * @date $Date$
 *
 * $Id$
 */

#include "StringNavigationCommander.h"

// Module specification
// <rtc-template block="module_spec">
static const char* stringnavigationcommander_spec[] =
  {
    "implementation_id", "StringNavigationCommander",
    "type_name",         "StringNavigationCommander",
    "description",       "Navigation Manager RTC",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Test",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    // Widget
    "conf.__widget__.debug", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
StringNavigationCommander::StringNavigationCommander(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_commandIn("command", m_command),
    m_currentPoseIn("currentPose", m_currentPose),
    m_mapServerPort("mapServer"),
    m_PathPlannerPort("PathPlanner"),
    m_PathFollowerPort("PathFollower")

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
StringNavigationCommander::~StringNavigationCommander()
{
}



RTC::ReturnCode_t StringNavigationCommander::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("command", m_commandIn);
  addInPort("currentPose", m_currentPoseIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  m_mapServerPort.registerConsumer("mapServer", "RTC::OGMapServer", m_OGMapServer);
  m_PathPlannerPort.registerConsumer("pathPlanner", "RTC::PathPlanner", m_pathPlanner);
  m_PathFollowerPort.registerConsumer("PathFollower", "RTC::PathFollower", m_pathFollower);
  
  // Set CORBA Service Ports
  addPort(m_mapServerPort);
  addPort(m_PathPlannerPort);
  addPort(m_PathFollowerPort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t StringNavigationCommander::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StringNavigationCommander::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StringNavigationCommander::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t StringNavigationCommander::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t StringNavigationCommander::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t StringNavigationCommander::onExecute(RTC::UniqueId ec_id)
{
	// 現在のロボットの位置を取得
	if (m_currentPoseIn.isNew()) {
		m_currentPoseIn.read(); // m_currentPoseにデータが格納される
	}

	// 文字列コマンドを取得
	if (m_commandIn.isNew()) {
		m_commandIn.read(); // m_commandにデータが格納される
		std::string data = m_command.data; // string型に変換
		std::cout << "[RTC::StringCommander] Receive Command (" << data << ")" << std::endl;
			
		// Goalを設定する
		Pose2D goal;
		if (data == "GOTO_A") { // string型は==演算子が使える
			goal.position.x = 1.0;
			goal.position.y = 1.0;
			goal.heading = 1.57;
		} else if (data == "GOTO_B") {
			goal.position.x = 1.0;
			goal.position.y = 0.0;
			goal.heading = 0.0;
		} else { // 知らないコマンドはエラーメッセージを表示
			std::cout << "[RTC::StringCommander] Error. Unknown Command (" << data << ")" << std::endl;
			return RTC::RTC_OK; // onExecuteから通常終了
		}
		// 現在地と目的地が定まる
		std::cout << "[RTC::StringCommander] Current Pose = (" << m_currentPose.data.position.x << ", " 
			<< m_currentPose.data.position.y << ", " << m_currentPose.data.heading << ")" << std::endl;
		std::cout << "[RTC::StringCommander] Goal Pose    = (" << goal.position.x << ", " 
			<< goal.position.y << ", " << goal.heading << ")" << std::endl;
			
		// マップを要求する
		RTC::OGMap_var outMap; // マップを格納するための変数
		RTC::RETURN_VALUE retval = this->m_OGMapServer._ptr()->requestCurrentBuiltMap(outMap);
		if (retval == RTC::RETURN_VALUE::RETVAL_OK) {
			std::cout << "[RTC::StringCommander] Map Request OK." << std::endl;
		} else {
			std::cout << "[RTC::StringCommander] Map Request Unknown Error Code =" << retval << std::endl;
			return RTC::RTC_ERROR;
		}

		// 軌道計画の為のパラメータを調整
		RTC::PathPlanParameter param;
		param.map = outMap; // マップ
		param.currentPose = this->m_currentPose.data; // スタート
		param.targetPose = goal; // ゴール
		param.distanceTolerance = 1.0; // 軌道からの位置のずれの許容差
		param.headingTolerance = 1.0; // 軌道からの角度のずれの許容差
		param.maxSpeed.vx = 5.0; param.maxSpeed.vy = 0.0; param.maxSpeed.va = 1.0; // 最大速度
		param.timeLimit.sec = 9999; param.timeLimit.nsec = 0; // 許容時間
		RTC::Path2D_var outPath; // 軌道を格納するための変数
		retval = m_pathPlanner._ptr()->planPath(param, outPath);
		if (retval == RTC::RETURN_VALUE::RETVAL_OK) { // 成功
			std::cout << "[RTC::StringCommander] Path Plan OK." << std::endl;
		} else if (retval == RTC::RETURN_VALUE::RETVAL_NOT_FOUND) { // 見つからない (通常終了)
			std::cout << "[RTC::StringCommander] Path Plan No Path Found" << std::endl;
			return RTC::RTC_OK;
		} else {
			std::cout << "[RTC::StringCommander] Path Plan Unknown Error Code =" << retval << std::endl;
			return RTC::RTC_ERROR;
		}

		// 軌道追従を要求．終了するまでfollowPathはブロックする
		retval = m_pathFollower._ptr()->followPath(outPath);
		if (retval == RTC::RETURN_VALUE::RETVAL_OK) { // 成功
			std::cout << "[RTC::StringCommander] Follow OK." << std::endl;
		} else { // 失敗も通常終了
			std::cout << "[RTC::StringCommander] Follow Unknown Error Code =" << retval << std::endl;
			return RTC::RTC_OK;
		}


	} // if (m_commandIn.isNew()) に対応

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t StringNavigationCommander::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StringNavigationCommander::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t StringNavigationCommander::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t StringNavigationCommander::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StringNavigationCommander::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void StringNavigationCommanderInit(RTC::Manager* manager)
  {
    coil::Properties profile(stringnavigationcommander_spec);
    manager->registerFactory(profile,
                             RTC::Create<StringNavigationCommander>,
                             RTC::Delete<StringNavigationCommander>);
  }
  
};


