#ifndef inspector_gcs_plugin_H
#define inspector_gcs_plugin_H

// C++
#include <string>
#include <thread>
// ROS
#include <ros/ros.h>
#include <ros/package.h>
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"
#include <std_msgs/Int8.h>
#include "std_srvs/Empty.h"
#include "geometry_msgs/Twist.h"

#include <nav_msgs/Path.h>

// QT & RQT
#include <rqt_gui_cpp/plugin.h>
#include <safedrone_gui/ui_gcs_plugin.h>
#include <QWidget>

#include <QBasicTimer>
#include <QString>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QButtonGroup>
// #include <QComboBox>
#include <QList>
#include <QVector> 
#include <pluginlib/class_list_macros.h>

#include <QGuiApplication>

// Services

#include <safedrone_gui/UalState.h>

#include <safedrone_gui/UalSetHome.h>
#include <safedrone_gui/InstructionCommand.h>

// TEST
#include "sensor_msgs/PointCloud2.h"
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/NavSatFix.h"
#include "sensor_msgs/BatteryState.h"


namespace safedrone_gui
{

class GcsPlugin : public rqt_gui_cpp::Plugin
{
  Q_OBJECT

public:
  GcsPlugin();
  virtual void initPlugin(qt_gui_cpp::PluginContext &context);
  virtual void shutdownPlugin();
  virtual void saveSettings(qt_gui_cpp::Settings &plugin_settings, qt_gui_cpp::Settings &instance_settings) const;
  virtual void restoreSettings(const qt_gui_cpp::Settings &plugin_settings, const qt_gui_cpp::Settings &instance_settings);
  Ui::GcsPluginWidget ui_;
  // --------------------------------------------------------------------------
protected slots:

  virtual void press_TakeOff_1();
  virtual void press_TakeOff_2();
  virtual void press_TakeOff_3();
  virtual void press_SetHome_1();
  virtual void press_SetHome_2();
  virtual void press_SetHome_3();

  virtual void press_StartMission();
  virtual void press_UniflyReady();


protected:

  // UAL //
  virtual void ual_state_1_cb(const safedrone_gui::UalState msg);
  virtual void ual_state_2_cb(const safedrone_gui::UalState msg);
  virtual void ual_state_3_cb(const safedrone_gui::UalState msg);
  virtual void pose_1_cb(const geometry_msgs::PoseStamped);
  virtual void pose_2_cb(const geometry_msgs::PoseStamped);
  virtual void pose_3_cb(const geometry_msgs::PoseStamped);
  //

  // MAGNA //
  virtual void magna_state_1_cb(const std_msgs::String msg);
  virtual void magna_state_2_cb(const std_msgs::String msg);
  virtual void magna_state_3_cb(const std_msgs::String msg);
  //

  // MAVROS //
  virtual void gps_pos_1_cb(const sensor_msgs::NavSatFix);
  virtual void gps_pos_2_cb(const sensor_msgs::NavSatFix);
  virtual void gps_pos_3_cb(const sensor_msgs::NavSatFix);
  virtual void battery_1_cb(const sensor_msgs::BatteryState);
  virtual void battery_2_cb(const sensor_msgs::BatteryState);
  virtual void battery_3_cb(const sensor_msgs::BatteryState);
  //
  

  // Comment in to signal that the plugin has a way to configure it
  // bool hasConfiguration() const;
  // void triggerConfiguration();

private slots:

private:
  QWidget *widget_;
  // Node
  ros::NodeHandle n_;

  // Private variables
  QStringList uav_list;
  bool mission_created;
  bool mission_sent;

  // Thread
  std::thread gui_thread;
  void guiThread();


  ///////////////////////
  // SafeDrone GUI //

  ros::ServiceClient instruction_srv;
  ros::ServiceClient set_home_srv;

  safedrone_gui::InstructionCommand instruction_service;
  safedrone_gui::UalSetHome set_home_service;
  
  ////////////////////////

 
  //////////////////////////////////////////

  // Subscribers
    // Mavros
  ros::Subscriber gps_pos_1_sub, gps_pos_2_sub, gps_pos_3_sub;
  ros::Subscriber battery_1_sub, battery_2_sub, battery_3_sub;
    // ADL
  ros::Subscriber magna_state_1_sub, magna_state_2_sub, magna_state_3_sub;
    // UAL
  ros::Subscriber ual_state_1_sub, ual_state_2_sub, ual_state_3_sub, pose_1_sub, pose_2_sub, pose_3_sub, velocity_sub;
  

  int stby_action_service_count;
  int stop_service_count;
  int paused_state_action_service_count;

  std::vector<std::string> ual_states = { "Uninitialized", 
                                        "Landed Disarmed", 
                                        "Landed Armed", 
                                        "Taking Off",
                                        "Flying Auto",
                                        "Flying Manual",
                                        "Landed" };
  // ros::Publisher resolution;
  // --------------------------------------------------------------------------
};
} // namespace inspector_gcs
#endif // inspector_gcs_plugin_H
