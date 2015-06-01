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
	// ���݂̃��{�b�g�̈ʒu���擾
	if (m_currentPoseIn.isNew()) {
		m_currentPoseIn.read(); // m_currentPose�Ƀf�[�^���i�[�����
	}

	// ������R�}���h���擾
	if (m_commandIn.isNew()) {
		m_commandIn.read(); // m_command�Ƀf�[�^���i�[�����
		std::string data = m_command.data; // string�^�ɕϊ�
		std::cout << "[RTC::StringCommander] Receive Command (" << data << ")" << std::endl;
			
		// Goal��ݒ肷��
		Pose2D goal;
		if (data == "GOTO_A") { // string�^��==���Z�q���g����
			goal.position.x = 1.0;
			goal.position.y = 1.0;
			goal.heading = 1.57;
		} else if (data == "GOTO_B") {
			goal.position.x = 1.0;
			goal.position.y = 0.0;
			goal.heading = 0.0;
		} else { // �m��Ȃ��R�}���h�̓G���[���b�Z�[�W��\��
			std::cout << "[RTC::StringCommander] Error. Unknown Command (" << data << ")" << std::endl;
			return RTC::RTC_OK; // onExecute����ʏ�I��
		}
		// ���ݒn�ƖړI�n����܂�
		std::cout << "[RTC::StringCommander] Current Pose = (" << m_currentPose.data.position.x << ", " 
			<< m_currentPose.data.position.y << ", " << m_currentPose.data.heading << ")" << std::endl;
		std::cout << "[RTC::StringCommander] Goal Pose    = (" << goal.position.x << ", " 
			<< goal.position.y << ", " << goal.heading << ")" << std::endl;
			
		// �}�b�v��v������
		RTC::OGMap_var outMap; // �}�b�v���i�[���邽�߂̕ϐ�
		RTC::RETURN_VALUE retval = this->m_OGMapServer._ptr()->requestCurrentBuiltMap(outMap);
		if (retval == RTC::RETURN_VALUE::RETVAL_OK) {
			std::cout << "[RTC::StringCommander] Map Request OK." << std::endl;
		} else {
			std::cout << "[RTC::StringCommander] Map Request Unknown Error Code =" << retval << std::endl;
			return RTC::RTC_ERROR;
		}

		// �O���v��ׂ̈̃p�����[�^�𒲐�
		RTC::PathPlanParameter param;
		param.map = outMap; // �}�b�v
		param.currentPose = this->m_currentPose.data; // �X�^�[�g
		param.targetPose = goal; // �S�[��
		param.distanceTolerance = 1.0; // �O������̈ʒu�̂���̋��e��
		param.headingTolerance = 1.0; // �O������̊p�x�̂���̋��e��
		param.maxSpeed.vx = 5.0; param.maxSpeed.vy = 0.0; param.maxSpeed.va = 1.0; // �ő呬�x
		param.timeLimit.sec = 9999; param.timeLimit.nsec = 0; // ���e����
		RTC::Path2D_var outPath; // �O�����i�[���邽�߂̕ϐ�
		retval = m_pathPlanner._ptr()->planPath(param, outPath);
		if (retval == RTC::RETURN_VALUE::RETVAL_OK) { // ����
			std::cout << "[RTC::StringCommander] Path Plan OK." << std::endl;
		} else if (retval == RTC::RETURN_VALUE::RETVAL_NOT_FOUND) { // ������Ȃ� (�ʏ�I��)
			std::cout << "[RTC::StringCommander] Path Plan No Path Found" << std::endl;
			return RTC::RTC_OK;
		} else {
			std::cout << "[RTC::StringCommander] Path Plan Unknown Error Code =" << retval << std::endl;
			return RTC::RTC_ERROR;
		}

		// �O���Ǐ]��v���D�I������܂�followPath�̓u���b�N����
		retval = m_pathFollower._ptr()->followPath(outPath);
		if (retval == RTC::RETURN_VALUE::RETVAL_OK) { // ����
			std::cout << "[RTC::StringCommander] Follow OK." << std::endl;
		} else { // ���s���ʏ�I��
			std::cout << "[RTC::StringCommander] Follow Unknown Error Code =" << retval << std::endl;
			return RTC::RTC_OK;
		}


	} // if (m_commandIn.isNew()) �ɑΉ�

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


