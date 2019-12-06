#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <robot_state_publisher/robot_state_publisher.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Float64MultiArray.h>

using namespace std;
std_msgs::Float64 hipAngle;

//订阅Qt话题回调函数、修改发送到RVIZ的角度值
void chatterCallback(const std_msgs::Float64MultiArray angleArray)
{
    ROS_INFO("I heard: [%f]", angleArray.data.at(0));
    hipAngle.data = angleArray.data.at(0);
}

int main(int argc, char** argv)
 {
    ros::init(argc, argv, "joint_pub"); //节点的名称
    ros::NodeHandle n;
    //发送关节角话题
    ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);    
    //订阅来自Qt的消息，发送至RVIZ可视化
    ros::Subscriber sub = n.subscribe("Qt_Msg", 1000, chatterCallback);
    ros::Rate loop_rate(10);   
    const double degree = M_PI/180;

    // robot state
    double angle= 0;
    // message declarations
    sensor_msgs::JointState joint_state;

    while (ros::ok()) {
        //update joint_state
        joint_state.header.stamp = ros::Time::now();
        joint_state.name.resize(4);
        joint_state.position.resize(4);
        joint_state.name[0]="hip";
        joint_state.position[0] = hipAngle.data*degree;
        joint_state.name[1] ="shoulder";
        joint_state.position[1] = 0;
        joint_state.name[2] ="elbow";
        joint_state.position[2] = 0;
        joint_state.name[3] ="wrist";
        joint_state.position[3] = 0;

        joint_pub.publish(joint_state);

        loop_rate.sleep();
        ros::spinOnce();
    }

    return 0;
}
