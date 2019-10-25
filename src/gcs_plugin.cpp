#include "safedrone_gui/gcs_plugin.h"
// #include <inspector_gcs/gcs_services.h>


namespace safedrone_gui
{

GcsPlugin::GcsPlugin()
    : rqt_gui_cpp::Plugin(), widget_(0)
{
  // Constructor is called first before initPlugin function, needless to say.

  // give QObjects reasonable names
  setObjectName("GcsPlugin");
}

void GcsPlugin::initPlugin(qt_gui_cpp::PluginContext &context)
{
  ROS_INFO("GUI running on Ground Control Station");

  // access standalone command line arguments
  QStringList argv = context.argv();
  // create QWidget
  widget_ = new QWidget();
  // extend the widget with all attributes and children from UI file
  ui_.setupUi(widget_);
  // cloudUpdate = new cloudSignal(ui_);

  // add widget to the user interface
  context.addWidget(widget_);

  //
  // QGuiApplication app(argc, argv);

  // QQmlApplicationEngine engine;
  // engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  // return app.exec();

  //////////////////////////////////////////////
  // Inspector GCS //  
  ros::start();

  // initialize variables
  mission_sent = false;
  mission_created = false;
  
    // start thread
  gui_thread = std::thread(&GcsPlugin::guiThread, this);

    // CONNECT
  connect(ui_.pushButton_SetHome_1, SIGNAL(pressed()), this, SLOT(press_SetHome_1()));
  connect(ui_.pushButton_SetHome_2, SIGNAL(pressed()), this, SLOT(press_SetHome_2()));
  connect(ui_.pushButton_SetHome_3, SIGNAL(pressed()), this, SLOT(press_SetHome_3()));
  connect(ui_.pushButton_TakeOff_1, SIGNAL(pressed()), this, SLOT(press_TakeOff_1()));
  connect(ui_.pushButton_TakeOff_2, SIGNAL(pressed()), this, SLOT(press_TakeOff_2()));
  connect(ui_.pushButton_TakeOff_3, SIGNAL(pressed()), this, SLOT(press_TakeOff_3()));
  connect(ui_.pushButton_StartMission, SIGNAL(pressed()), this, SLOT(press_StartMission()));
  connect(ui_.pushButton_UniflyReady, SIGNAL(pressed()), this, SLOT(press_UniflyReady()));

  
  // connect(ui_.uav_selection_Box, SIGNAL(currentIndexChanged(int index)), this, SLOT(on_uav_selection_Box_currentIndexChanged(int index)));
    
  //uavServices
  // mission_srv = n_.serviceClient<inspector_gcs::MissionService>("/uav_1/mission_service");
  // stby_action_srv = n_.serviceClient<inspector_gcs::StbyActionService>("/uav_1/stby_action_service");
  // paused_st_action_srv = n_.serviceClient<inspector_gcs::PausedStActionService>("/uav_1/paused_state_action_service");
  // stop_srv = n_.serviceClient<inspector_gcs::StopService>("/uav_1/stop_service");
  
  //GCS Services
  // createMission_srv =n_.serviceClient<inspector_gcs::gcsCreateMission>("create_mission_service");
  // sendMission_srv =n_.serviceClient<inspector_gcs::gcsSendMission>("send_mission_service");

  //GCS Subscribers
  // uav_list_sub = n_.subscribe("uav_list", 0, &GcsPlugin::uav_list_cb,this);

  // Inspector GCS // 
  ////////////////////////////////////////////////

  // ros::start();
  // CONNECT
  // connect(ui_.pushButton, SIGNAL(pressed()), this, SLOT(click_pushButton()));
  // connect(cloudUpdate, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

  // connect(ui_.pushButton_takeOff, SIGNAL(pressed()), this, SLOT(press_takeOff()));
  // connect(ui_.pushButton_land, SIGNAL(pressed()), this, SLOT(press_land()));
  // connect(ui_.pushButton_goToWaypoint, SIGNAL(pressed()), this, SLOT(press_goToWaypoint()));
  // connect(ui_.pushButton_setVelocity, SIGNAL(pressed()), this, SLOT(press_setVelocity()));
  // SUBSCRIBERS
  // UAL //
  // srvTakeOff = n_.serviceClient<uav_abstraction_layer::TakeOff>("/uav_1/ual/take_off");
  // srvLand = n_.serviceClient<uav_abstraction_layer::Land>("/uav_1/ual/land");
  // srvGoToWaypoint = n_.serviceClient<uav_abstraction_layer::GoToWaypoint>("/uav_1/ual/go_to_waypoint");
  // srvSetVelocity = n_.serviceClient<uav_abstraction_layer::SetVelocity>("/uav_1/ual/set_velocity");
  // state_sub = n_.subscribe("/uav_1/ual/state", 0, &GcsPlugin::state_callback, this);
  // pose_sub = n_.subscribe("/uav_1/ual/pose", 0, &GcsPlugin::pose_callback, this);
  // velocity_sub = n_.subscribe("/uav_1/ual/velocity", 0, &GcsPlugin::velocity_callback, this);
  // UAL //
  // PUBLISHERS
}



void GcsPlugin::shutdownPlugin()
{
  // unregister all publishers here
  n_.shutdown();
}

void GcsPlugin::saveSettings(qt_gui_cpp::Settings &plugin_settings,
                             qt_gui_cpp::Settings &instance_settings) const
{
  // instance_settings.setValue(k, v)
}

void GcsPlugin::restoreSettings(const qt_gui_cpp::Settings &plugin_settings,
                                const qt_gui_cpp::Settings &instance_settings)
{
  // v = instance_settings.value(k)
}


// void cloudSignal::setValue(int value)
// {
//   emit valueChanged(value);

//   /*if ((ros::Time::now().toSec() - 1.0) >= updateItem)
//   {
//     ROS_INFO("1.0");
//     for (int i = 0; i < ui_.list_movil->count(); ++i)
//     {
//       //QListWidgetItem *item = ui_.list_movil->item(i);
//       //Do stuff!
//       //item->setText(QString("-"));
//       //ui_.list_movil->item(i)->setText(QString::number(updateItem, 'f', 0));
//     }
//     //ui_.list_movil->item(0)->setText(QString::number(ros::Time::now().toSec()));
//     updateItem = ros::Time::now().toSec();
//   }*/
//   //ui_.list_movil->update();
//   //QApplication::processEvents();
// }

//--------
// Inspector GCS //

void GcsPlugin::guiThread()
{
  // ROS_INFO("guiThread");
  std::string uav_id;

  instruction_srv = n_.serviceClient<safedrone_gui::InstructionCommand>("/magna/GS/external_input");
  // set_home_srv = n_.serviceClient<safedrone_gui::UalSetHome>("/magna/GS/external_input");

  while (ros::ok()){
    uav_id = ui_.uav_selection_Box->currentText().toStdString();

    pose_1_sub = n_.subscribe("uav_1/ual/pose", 0, &GcsPlugin::pose_1_cb, this);
    pose_2_sub = n_.subscribe("uav_2/ual/pose", 0, &GcsPlugin::pose_2_cb, this);
    pose_3_sub = n_.subscribe("uav_3/ual/pose", 0, &GcsPlugin::pose_3_cb, this);
    // pose_sub = n_.subscribe(uav_id + "/ual/pose", 0, &GcsPlugin::pose_callback, this);
    // gps_pos_sub = n_.subscribe(uav_id + "/dji_sdk/gps_position", 0, &GcsPlugin::gps_pos_cb, this);
    gps_pos_1_sub = n_.subscribe("uav_1/mavros/global_position/global", 0, &GcsPlugin::gps_pos_1_cb, this);
    gps_pos_2_sub = n_.subscribe("uav_2/mavros/global_position/global", 0, &GcsPlugin::gps_pos_2_cb, this);
    gps_pos_3_sub = n_.subscribe("uav_3/mavros/global_position/global", 0, &GcsPlugin::gps_pos_3_cb, this);

    ual_state_1_sub = n_.subscribe("uav_1/ual/state", 0, &GcsPlugin::ual_state_1_cb, this);
    ual_state_2_sub = n_.subscribe("uav_2/ual/state", 0, &GcsPlugin::ual_state_2_cb, this);
    ual_state_3_sub = n_.subscribe("uav_3/ual/state", 0, &GcsPlugin::ual_state_3_cb, this);
    // adl_state_sub = n_.subscribe(uav_id + "/adl_state", 0, &GcsPlugin::adl_state_cb, this);
    magna_state_1_sub = n_.subscribe("magna/status/agent_1", 0, &GcsPlugin::magna_state_1_cb, this);
    magna_state_2_sub = n_.subscribe("magna/status/agent_2", 0, &GcsPlugin::magna_state_2_cb, this);
    magna_state_3_sub = n_.subscribe("magna/status/agent_3", 0, &GcsPlugin::magna_state_3_cb, this);

    // if (uav_list.count() == 0) {
    //   ui_.pushButton_CreateMission->setVisible(false);
    // } else {
    //   ui_.pushButton_CreateMission->setVisible(true);
    // }
    // if (mission_created) {
    //   ui_.pushButton_SendMission->setVisible(true);
    // } else {
    //   ui_.pushButton_SendMission->setVisible(false);
    // }

    if (uav_id == ""){
      // ui_.pushButton_StartMission_2->setVisible(true);
      // ui_.pushButton_StopMission_2->setVisible(true);
      // ui_.pushButton_ResumeMission_2->setVisible(true);
      // ui_.pushButton_AbortMission_2->setVisible(true);

      // ui_.getUalState->del();
      // ui_.getAdlState->del();
      // ui_.getLat->del();
      // ui_.getLong->del();
      // ui_.getAlt->del();
      // ui_.getPosePx->del();
      // ui_.getPosePy->del();
      // ui_.getPosePz->del();
    } 
    // else {
    //   if (mission_sent && ros::service::exists(uav_id + "/stby_action_service", false)) {
    //     ui_.pushButton_StartMission_2->setVisible(true);
    //   } else {
    //     ui_.pushButton_StartMission_2->setVisible(false);
    //   }

    //   if (mission_sent && ros::service::exists(uav_id + "/stop_service", false)) {
    //     ui_.pushButton_StopMission_2->setVisible(true);
    //   } else {
    //     ui_.pushButton_StopMission_2->setVisible(false);
    //   }

    //   if (ros::service::exists(uav_id + "/paused_state_action_service", false)) {
    //     ui_.pushButton_ResumeMission_2->setVisible(true);
    //     ui_.pushButton_AbortMission_2->setVisible(true);
    //   } else {
    //     ui_.pushButton_ResumeMission_2->setVisible(false);
    //     ui_.pushButton_AbortMission_2->setVisible(false);
    //   }


    //   stby_action_service_count = 0;
    //   stop_service_count = 0;
    //   paused_state_action_service_count = 0;
    //   for (int i = 0; i < uav_list.count(); ++i) {
    //     uav_id = uav_list[i].toStdString();
    //     if (ros::service::exists(uav_id + "/stby_action_service", false)) {stby_action_service_count ++;}
    //     if (ros::service::exists(uav_id + "/stop_service", false)) {stop_service_count ++;}
    //     if (ros::service::exists(uav_id + "/paused_state_action_service", false)) {paused_state_action_service_count ++;}
    //   }

    //   if (mission_sent && (stby_action_service_count == uav_list.count()) ) {
    //     ui_.pushButton_StartMission->setVisible(true);
    //   } else {
    //     ui_.pushButton_StartMission->setVisible(false);
    //   }      
      
    //   if (mission_sent && (stop_service_count == uav_list.count()) ) {
    //     ui_.pushButton_StopMission->setVisible(true);
    //   } else {
    //     ui_.pushButton_StopMission->setVisible(false);
    //   }
      
    //   if (paused_state_action_service_count == uav_list.count()) {
    //     ui_.pushButton_ResumeMission->setVisible(true);
    //     ui_.pushButton_AbortMission->setVisible(true);
    //   } else {
    //     ui_.pushButton_ResumeMission->setVisible(false);
    //     ui_.pushButton_AbortMission->setVisible(false);
    //   }
    // }
    ros::Duration(0.5).sleep();
  }
}

// void GcsPlugin::uav_list_cb(const inspector_gcs::UavList msg)
// {
//   std::vector<std::string> vector = msg.uavs;
//   bool update_uav_list_view = false;

//   // ui_.uav_list_view->clear();
  
//   QStringList uav_list_2;
//   uav_list_2.reserve(vector.size());
//   for(int i = 0; i < vector.size(); ++i) {
//     uav_list_2 << vector[i].c_str();
//   }

//   for (int i = 0; i < uav_list_2.count(); ++i) { //add new items to selection Box
//     if (!uav_list.contains(uav_list_2[i])) {
//       ui_.uav_selection_Box->addItem(uav_list_2[i]);
//       update_uav_list_view = true;
//     }
//   }
//   for (int i = 0; i < uav_list.count(); ++i) { //remove items from selection Box
//     if (!uav_list_2.contains(uav_list[i])) {
//       int index = ui_.uav_selection_Box->findText(uav_list[i]);
//       ui_.uav_selection_Box->removeItem(index);
//       update_uav_list_view = true;
//     }
//   }
//   // vector.clear();
//   uav_list = uav_list_2;
//   uav_list_2.clear();

//   if (update_uav_list_view) {
//     ui_.uav_list_view->clear();
//     ui_.uav_list_view->addItems(uav_list);
//   }
// }

void GcsPlugin::press_SetHome_1()
{
  ROS_INFO("GUI: Set Home 1 clicked");
  set_home_srv = n_.serviceClient<safedrone_gui::UalSetHome>("uav_1/ual/set_home");
  set_home_service.request.set_z = true;
  set_home_srv.call(set_home_service);
}

void GcsPlugin::press_SetHome_2()
{
  ROS_INFO("GUI: Set Home 2 clicked");
  set_home_srv = n_.serviceClient<safedrone_gui::UalSetHome>("uav_2/ual/set_home");
  set_home_service.request.set_z = true;
  set_home_srv.call(set_home_service);
}

void GcsPlugin::press_SetHome_3()
{
  ROS_INFO("GUI: Set Home 3 clicked");
  set_home_srv = n_.serviceClient<safedrone_gui::UalSetHome>("uav_3/ual/set_home");
  set_home_service.request.set_z = true;
  set_home_srv.call(set_home_service);
}

void GcsPlugin::press_TakeOff_1()
{
  ROS_INFO("GUI: Take Off 1 clicked");
  instruction_service.request.instruction = "1_take_off";
  // instruction_srv = n_.serviceClient<safedrone_gui::InstructionCommand>("/magna/gs/external_input");
  instruction_srv.call(instruction_service);
}

void GcsPlugin::press_TakeOff_2()
{
  ROS_INFO("GUI: Take Off 2 clicked");
  instruction_service.request.instruction = "2_take_off";
  // instruction_srv = n_.serviceClient<safedrone_gui::InstructionCommand>("/magna/gs/external_input");
  instruction_srv.call(instruction_service);
}

void GcsPlugin::press_TakeOff_3()
{
  ROS_INFO("GUI: Take Off 3 clicked");
  instruction_service.request.instruction = "3_take_off";
  // instruction_srv = n_.serviceClient<safedrone_gui::InstructionCommand>("/magna/gs/external_input");
  instruction_srv.call(instruction_service);
}

void GcsPlugin::press_StartMission()
{
  ROS_INFO("GUI: Start Mission clicked");
  instruction_service.request.instruction = "start_mission";
  instruction_srv.call(instruction_service);
}

void GcsPlugin::press_UniflyReady()
{
  ROS_INFO("GUI: Unifly Ready clicked");
  instruction_service.request.instruction = "unifly_ready";
  instruction_srv.call(instruction_service);
}


// void GcsPlugin::on_uav_selection_Box_currentIndexChanged(int index)
// {
//   std::string uav_id = ui_.uav_selection_Box->currentText().toStdString();
//   ROS_INFO("selected %s", uav_id.c_str());

// }

// Inspector GCS //

// --------------------------------------------------------------------------
// UAL //
void GcsPlugin::ual_state_1_cb(const safedrone_gui::UalState msg)
{
  QString txt = QString::fromStdString(ual_states[msg.state]);
  ui_.getUalState_1->setText(txt);
}

void GcsPlugin::ual_state_2_cb(const safedrone_gui::UalState msg)
{
  QString txt = QString::fromStdString(ual_states[msg.state]);
  ui_.getUalState_2->setText(txt);
}

void GcsPlugin::ual_state_3_cb(const safedrone_gui::UalState msg)
{
  QString txt = QString::fromStdString(ual_states[msg.state]);
  ui_.getUalState_3->setText(txt);
}

void GcsPlugin::magna_state_1_cb(const std_msgs::String msg)
{
  QString txt = QString::fromStdString(msg.data);
  ui_.getMagnaState_1->setText(txt);
}

void GcsPlugin::magna_state_2_cb(const std_msgs::String msg)
{
  QString txt = QString::fromStdString(msg.data);
  ui_.getMagnaState_2->setText(txt);
}

void GcsPlugin::magna_state_3_cb(const std_msgs::String msg)
{
  QString txt = QString::fromStdString(msg.data);
  ui_.getMagnaState_3->setText(txt);
}

void GcsPlugin::gps_pos_1_cb(const sensor_msgs::NavSatFix msg)
{
  ui_.getLat_1->setText(QString::number(msg.latitude, 'f', 5));
  ui_.getLong_1->setText(QString::number(msg.longitude, 'f', 5));
  ui_.getAlt_1->setText(QString::number(msg.altitude, 'f', 2));
}

void GcsPlugin::gps_pos_2_cb(const sensor_msgs::NavSatFix msg)
{
  ui_.getLat_2->setText(QString::number(msg.latitude, 'f', 5));
  ui_.getLong_2->setText(QString::number(msg.longitude, 'f', 5));
  ui_.getAlt_2->setText(QString::number(msg.altitude, 'f', 2));
}

void GcsPlugin::gps_pos_3_cb(const sensor_msgs::NavSatFix msg)
{
  ui_.getLat_3->setText(QString::number(msg.latitude, 'f', 5));
  ui_.getLong_3->setText(QString::number(msg.longitude, 'f', 5));
  ui_.getAlt_3->setText(QString::number(msg.altitude, 'f', 2));
}

void GcsPlugin::pose_1_cb(const geometry_msgs::PoseStamped msg)
{
  ui_.getPosePx_1->setText(QString::number(msg.pose.position.x, 'f', 2));
  ui_.getPosePy_1->setText(QString::number(msg.pose.position.y, 'f', 2));
  ui_.getPosePz_1->setText(QString::number(msg.pose.position.z, 'f', 2));
  // ui_.getPoseOx->setText(QString::number(msg.pose.orientation.x, 'f', 2));
  // ui_.getPoseOy->setText(QString::number(msg.pose.orientation.y, 'f', 2));
  // ui_.getPoseOz->setText(QString::number(msg.pose.orientation.z, 'f', 2));
  // ui_.getPoseOw->setText(QString::number(msg.pose.orientation.w, 'f', 2));
}

void GcsPlugin::pose_2_cb(const geometry_msgs::PoseStamped msg)
{
  ui_.getPosePx_2->setText(QString::number(msg.pose.position.x, 'f', 2));
  ui_.getPosePy_2->setText(QString::number(msg.pose.position.y, 'f', 2));
  ui_.getPosePz_2->setText(QString::number(msg.pose.position.z, 'f', 2));
}

void GcsPlugin::pose_3_cb(const geometry_msgs::PoseStamped msg)
{
  ui_.getPosePx_3->setText(QString::number(msg.pose.position.x, 'f', 2));
  ui_.getPosePy_3->setText(QString::number(msg.pose.position.y, 'f', 2));
  ui_.getPosePz_3->setText(QString::number(msg.pose.position.z, 'f', 2));
}

// void GcsPlugin::velocity_callback(const geometry_msgs::TwistStamped msg)
// {
//   ui_.getVelLx->setText(QString::number(msg.twist.linear.x, 'f', 2));
//   ui_.getVelLy->setText(QString::number(msg.twist.linear.y, 'f', 2));
//   ui_.getVelLz->setText(QString::number(msg.twist.linear.z, 'f', 2));
//   ui_.getVelAx->setText(QString::number(msg.twist.angular.x, 'f', 2));
//   ui_.getVelAy->setText(QString::number(msg.twist.angular.y, 'f', 2));
//   ui_.getVelAz->setText(QString::number(msg.twist.angular.z, 'f', 2));
// }

// void GcsPlugin::press_takeOff()
// {
// ROS_INFO("take_off clicked" );
// qDebug() << "take_off clicked" << endl;
// std::cout << "take_off clicked" << endl;

//   double height = ui_.setTakeOffHeight->text().toDouble();
//   take_off.request.height = height;
//   take_off.request.blocking = false;
//   srvTakeOff.call(take_off);
// }

// void GcsPlugin::press_land()
// {
//   // land.request.twist.linear.x = 0.0;
//   // land.request.twist.linear.y = 0.0;
//   // land.request.twist.linear.z = 0.0;
//   // land.request.twist.angular.x = 0.0;
//   // land.request.twist.angular.y = 0.0;
//   // land.request.twist.angular.z = 0.0;
//   land.request.blocking = false;
//   srvLand.call(land);
// }

// void GcsPlugin::press_goToWaypoint()
// {
//   double pPx = ui_.setPosePx->text().toDouble();
//   double pPy = ui_.setPosePy->text().toDouble();
//   double pPz = ui_.setPosePz->text().toDouble();
//   double pOx = ui_.setPoseOx->text().toDouble();
//   double pOy = ui_.setPoseOy->text().toDouble();
//   double pOz = ui_.setPoseOz->text().toDouble();
//   double pOw = ui_.setPoseOw->text().toDouble();

//   wp.header.frame_id = "map";
//   wp.pose.position.x = pPx;
//   wp.pose.position.y = pPy;
//   wp.pose.position.z = pPz;
//   wp.pose.orientation.x = pOx;
//   wp.pose.orientation.y = pOy;
//   wp.pose.orientation.z = pOz;
//   wp.pose.orientation.w = pOw;

//   go_to_waypoint.request.waypoint = wp;
//   srvGoToWaypoint.call(go_to_waypoint);
// }

// void GcsPlugin::press_setVelocity()
// {
//   double vLx = ui_.setVelLx->text().toDouble();
//   double vLy = ui_.setVelLy->text().toDouble();
//   double vLz = ui_.setVelLz->text().toDouble();
//   double vAx = ui_.setVelAx->text().toDouble();
//   double vAy = ui_.setVelAy->text().toDouble();
//   double vAz = ui_.setVelAz->text().toDouble();

//   vel.header.frame_id = "map";
//   vel.twist.linear.x = vLx;
//   vel.twist.linear.y = vLy;
//   vel.twist.linear.z = vLz;
//   vel.twist.angular.x = vAx;
//   vel.twist.angular.y = vAy;
//   vel.twist.angular.z = vAz;

//   set_velocity.request.velocity = vel;
//   srvSetVelocity.call(set_velocity);
// }
// UAL //

// --------------------------------------------------------------------------

/*bool hasConfiguration() const
{
  return true;
}

void triggerConfiguration()
{
  // Usually used to open a dialog to offer the user a set of configuration
}*/

} // namespace safedrone_gui
//PLUGINLIB_DECLARE_CLASS(inspector_gcs, GcsPlugin, inspector_gcs::GcsPlugin, rqt_gui_cpp::Plugin)
PLUGINLIB_EXPORT_CLASS(safedrone_gui::GcsPlugin, rqt_gui_cpp::Plugin)
