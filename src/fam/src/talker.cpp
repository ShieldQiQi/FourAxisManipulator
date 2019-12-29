/****************************************************************************
 * Project:Six axis manipulator
 * Author:Qi
 * Created Time:2019/12/11
 * *****************************/

#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64MultiArray.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <visualization_msgs/Marker.h>
#include <cmath>
#include <tf/transform_broadcaster.h>
#include "./include/recognize.hpp"
#include "./include/inverseSolutonKiller.hpp"

#define WIDTH   1000
#define HEIGHT  150
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
unsigned char imageBuffer[HEIGHT][WIDTH];
std_msgs::Float64MultiArray axisAngles;
ros::Publisher read_pub;
std_msgs::Int32 temp;
FT_Library    library;
FT_Face       face;
FT_GlyphSlot  slot;
FT_Matrix     matrix;
FT_Vector     pen;
FT_Error      error;
char*         filename;
wchar_t       *wcharText = L"齐";
double        angle;
int           target_height;
int           n, num_chars;
int           fowardcount = 0;
bool          is_angleArrayUpdated = 0;
double        x_before = 0;
double        y_before = 0;

Recognize	  recognizer;
LinkQueue<Point>  travelQueue;
LinkQueue<Point>  travelQueueIdeal;

static uint8_t s_buffer[12];
static uint8_t r_buffer[1]={0};
serial::Serial ser; //声明串口对象

visualization_msgs::Marker points;
visualization_msgs::Marker pointWork;
visualization_msgs::Marker pointIdeal;
geometry_msgs::Point p;

inverseSolutionKiller soluKiller;

XMLDocument doc;

using namespace std;

void draw_bitmap( FT_Bitmap*  bitmap, FT_Int x, FT_Int y)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0 || j < 0|| i >= WIDTH || j >= HEIGHT )
        continue;

      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}

void show_image( void )
{
  int  i, j;

  for ( i = 0; i < HEIGHT; i++ )
  {
    for ( j = 0; j < WIDTH; j++ )
      putchar( image[i][j] == 0 ? ' '
                                : image[i][j] < 128 ? '*'
                                                    : '*' );
    putchar( '\n' );
  }
}

std::wstring s2ws(const std::string& s)
{
        setlocale(LC_ALL,"zh_CN.UTF-8");

        const char* _Source = s.c_str();
        size_t _Dsize = s.size() + 1;

        wchar_t *_Dest = new wchar_t[_Dsize];
        wmemset(_Dest, 0, _Dsize);
        mbstowcs(_Dest, _Source, _Dsize);
        std::wstring result = _Dest;
        delete[]_Dest;
        setlocale(LC_ALL, "C");
        return result;

}

//---------------------------------------------------
//更新文字在RVIZ中的显示
void updatePoints(visualization_msgs::Marker &points,visualization_msgs::Marker &pointWork)
{
    setlocale(LC_ALL,"zh_CN.UTF-8");

    fowardcount = 0;
    points.points.clear();
    travelQueue.ClearQueue();
    travelQueueIdeal.ClearQueue();
    pointWork.points.clear();
    pointIdeal.points.clear();

    points.header.frame_id  = "/textFrame";
    points.header.stamp = ros::Time::now();
    points.ns = "textPoints2";
    points.action = visualization_msgs::Marker::ADD;
    points.pose.orientation.w = 1.0;
    points.id = 0;
    points.type = visualization_msgs::Marker::POINTS;
    // POINTS markers use x and y scale for width/height respectively
    points.scale.x = 0.0032;
    points.scale.y = 0.0032;
    // Points are green
    points.color.b = 1.0f;
    points.color.a = 1.0;

    pointWork.header.frame_id  = "/textFrame";
    pointWork.header.stamp = ros::Time::now();
    pointWork.ns = "textPoints";
    pointWork.action = visualization_msgs::Marker::ADD;
    pointWork.pose.orientation.w = 1.0;
    pointWork.id = 0;
    pointWork.type = visualization_msgs::Marker::POINTS;
    // POINTS markers use x and y scale for width/height respectively
    pointWork.scale.x = 0.0032;
    pointWork.scale.y = 0.0032;
    pointWork.color.g = 1.0f;
    pointWork.color.a = 1.0;

    pointIdeal.header.frame_id  = "/textFrame";
    pointIdeal.header.stamp = ros::Time::now();
    pointIdeal.ns = "textPoints3";
    pointIdeal.action = visualization_msgs::Marker::ADD;
    pointIdeal.pose.orientation.w = 1.0;
    pointIdeal.id = 0;
    pointIdeal.type = visualization_msgs::Marker::POINTS;
    // POINTS markers use x and y scale for width/height respectively
    pointIdeal.scale.x = 0.0032;
    pointIdeal.scale.y = 0.0032;
    pointIdeal.color.g = 1.0f;
    pointIdeal.color.r = 1.0;
    pointIdeal.color.a = 1.0f;

    recognizer.Analyse(image,0);
    recognizer.pointQueue.Assignment(travelQueue,travelQueueIdeal);

    Node<Stroke>* pStrokeNode = recognizer.strokeQueue.phead->next;
    for (;pStrokeNode !=nullptr;pStrokeNode = pStrokeNode->next) {
        ROS_INFO("num %d:the stroke is '%s' which have %d point(s)",pStrokeNode->value.orderNum,pStrokeNode->value.strokeName,pStrokeNode->value.strokePointNum);
        Node<Point>* pPointNode = pStrokeNode->value.strokePointQueue.phead->next;

        for (;pPointNode!=nullptr;pPointNode = pPointNode->next) {
            ROS_INFO("   the point(s) are x = %d y = %d",pPointNode->value.x,pPointNode->value.y);
        }
    }

//    ROS_INFO("-------------------------------------------\nThere are %d point in travelQueueIdeal"
//             ,travelQueueIdeal.getSize());
    recognizer.pointQueue.ClearQueue();

    for (int i = 0; i < WIDTH; i++)
    {
        for(int j =0; j< HEIGHT;j++)
        {
            if(image[j][i] == 1 || i == 130 || (j == HEIGHT-1 && i<=130)|| i == 0 || (j == 0 && i<=130)){
                p.x = 0.012*(j-0);
                p.y = 0.012*i;
                p.z = 0;
                points.points.push_back(p);
            }
        }
    }
}

void getOutline()
{
        //舍去buffer边缘的点
        for (int i = 1; i < WIDTH-1; i++)
        {
                for(int j = 1;j<HEIGHT-1;j++)
                {
                        if(image[j][i] != 0 && (image[j-1][i] == 0 || image[j+1][i] == 0 || image[j][i-1] == 0 || image[j][i+1] == 0 )){
                                imageBuffer[j][i] = 1;
                        }else{
                                imageBuffer[j][i] = 0;
                        }
                }
        }
        for (int i = 0; i < WIDTH; i++)
        {
                for(int j =0;j<HEIGHT;j++)
                {
                        image[j][i] = imageBuffer[j][i];
                }
        }
}

void imageBinarization()
{
    for (int i = 0; i < WIDTH; i++)
    {
        for(int j = 0;j<HEIGHT;j++)
        {
            if(image[j][i] != 0){
                image[j][i] = 1;
            }
        }
    }
}

//---------------------------------------------------
//话题回调
void write_callback(const std_msgs::Float64MultiArray angleArray)
{
//        ROS_INFO("Target Angles:\n-------------------\n "
//                 "Axis_one %.1f Axis_two %.1f Axis_three %.1f Axis_four %.1f Axis_five %.1f Axis_six %.1f\n",
//                 angleArray.data.at(0),angleArray.data.at(1),angleArray.data.at(2),angleArray.data.at(3),angleArray.data.at(4),angleArray.data.at(5));
        if(angleArray.data.at(7) == 1)
            is_angleArrayUpdated = 1;
        axisAngles.data.at(0) =  angleArray.data.at(0);
        axisAngles.data.at(1) =  angleArray.data.at(1);
        axisAngles.data.at(2) =  angleArray.data.at(2);
        axisAngles.data.at(3) =  angleArray.data.at(3);
        axisAngles.data.at(4) =  angleArray.data.at(4);
        axisAngles.data.at(5) =  angleArray.data.at(5);
        axisAngles.data.at(6) =  angleArray.data.at(6);
}


void readTextString_callback(std_msgs::String textString)
{
    setlocale(LC_ALL,"zh_CN.UTF-8");

    x_before = y_before = 0;
    //std_msgs::String  ---->  std::string   ------> std::wstring -------> const wchar_t*
    //ROS_INFO("I heard %s\n",textString.data);
    string testStr(textString.data);

    XMLElement* pRootEle = doc.FirstChildElement("dictionary");
    XMLElement* destNode;
    const char* strNodeName = testStr.c_str();
    bool Error = recognizer.GetNodePointerByName(pRootEle,strNodeName,destNode);
    if(Error == 0)
        ROS_ERROR_STREAM("Could not find it in this XML...");
    else {
        ROS_WARN("Going to write Chinese '%s' ",destNode->GetText());
    }
    recognizer.GetStrokeMsg(destNode);
    ROS_INFO("There are %d stroke(s) in '%s', and They are:",recognizer.strokeQueue.getSize(),destNode->GetText());

    wstring wstrData;
    wstrData = s2ws(testStr);
    const wchar_t* pWstrData = wstrData.c_str();

    //draw the text to bitmap
    num_chars = wcslen(pWstrData);
//        ROS_INFO(" num_chars = %d",num_chars);

    FT_New_Face( library, filename, 0, &face );
    //FT_Set_Char_Size( face, 30 * 64, 10*64, 400, 0 );
    FT_Set_Char_Size( face, 0, 15*64, 400, 550 );
    slot = face->glyph;

    for ( n = 0; n < num_chars; n++ )
    {
            FT_Set_Transform( face, &matrix, &pen );
            error = FT_Load_Char( face, pWstrData[n], FT_LOAD_RENDER );
            //error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
            if ( error )
              continue;
            draw_bitmap( &slot->bitmap,
                                     slot->bitmap_left,
                                     target_height - slot->bitmap_top );

            pen.x += slot->advance.x;
            pen.y += slot->advance.y;
    }
    //show_image();
    //getOutline();
    imageBinarization();
    updatePoints(points,pointWork);

    pen.x = 15 * 64;
    pen.y = ( target_height - 100 ) * 64;
    FT_Done_Face(face);
    for(int i = 0;i<WIDTH; i++)
            for(int j =0;j<HEIGHT;j++)
                    image[j][i] = 0;
}

void timerCallback(const ros::TimerEvent& e)
{
    if (!travelQueue.isEmpty()) {
        p.x = 0.012*travelQueue.getFront().y;
        p.y = 0.012*travelQueue.getFront().x;
        p.z = 0;
        pointWork.points.push_back(p);
        travelQueue.pop();
    }else {
        travelQueue.ClearQueue();
    }
}

void fowardSolution(float theta[6])
{
    // define D-H parameters and related...
    double a1 = 0.016,a2 = 0.103,a3 = 0.097,x1 = 0.018;

    p.y = -1+0.012*150/0.1*cos(theta[0])*(x1+a1+a2*cos(theta[1])+a3*cos(theta[1]+theta[2]))+0.6;
    p.x = +1.1+0.012*150/0.1*sin(theta[0])*(x1+a1+a2*cos(theta[1])+a3*cos(theta[1]+theta[2]))-0.6;
//    ROS_INFO("------------\nOUTPUT:X = %f Y = %f foward count = %d",cos(theta[0])*(x1+a1+a2*cos(theta[1])+a3*cos(theta[1]+theta[2])),
//            sin(theta[0])*(x1+a1+a2*cos(theta[1])+a3*cos(theta[1]+theta[2])),fowardcount);
    pointIdeal.points.push_back(p);
    fowardcount++;
}

void inverseSolution()
{
    if (!travelQueueIdeal.isEmpty()) {

        // get four axis angles throuth inverse manipulator kinematic
        if(x_before != 0 && y_before !=0 && (pow(x_before - travelQueueIdeal.getFront().x,2) + pow(y_before - travelQueueIdeal.getFront().y,2) > 100)){
            soluKiller.getThetaArray((100-travelQueueIdeal.getFront().x)*0.1/150+0.134/*+0.173*/, travelQueueIdeal.getFront().y*0.1/150-0.03);
            soluKiller.angleArray[1] -= 0.4;
//            soluKiller.angleArray[0] = 0;
//            soluKiller.angleArray[1] = -1.92;
//            soluKiller.angleArray[2] = 1.57;
//            soluKiller.angleArray[3] = -1.57;
//            soluKiller.angleArray[4] = 0.4;
//            soluKiller.angleArray[5] = 0.4;
        }else {
//            soluKiller.angleArray[0] = 0;
//            soluKiller.angleArray[1] = -1.92;
//            soluKiller.angleArray[2] = 1.57;
//            soluKiller.angleArray[3] = -1.57;
//            soluKiller.angleArray[4] = 0.4;
//            soluKiller.angleArray[5] = 0.4;
            soluKiller.getThetaArray((100-travelQueueIdeal.getFront().x)*0.1/150+0.134/*+0.173*/, travelQueueIdeal.getFront().y*0.1/150-0.03);
        }
        x_before = travelQueueIdeal.getFront().x;
        y_before = travelQueueIdeal.getFront().y;
//        ROS_INFO("INPUT:X = %f Y = %f",(100-travelQueueIdeal.getFront().x)*0.1/150+0.134,travelQueueIdeal.getFront().y*0.1/150-0.03);
        travelQueueIdeal.pop();

    }else {
        travelQueueIdeal.ClearQueue();
    }
}

void updateAngles(const ros::TimerEvent& e)
{
    if(ser.available()){
            ser.read(r_buffer,1);
            temp.data = 1;
            read_pub.publish(temp);
    }
    temp.data = 1;
    read_pub.publish(temp);
    //发送报文内容
    if(axisAngles.data.at(6) == 0 && !travelQueueIdeal.isEmpty()){

        inverseSolution();
        fowardSolution(soluKiller.angleArray);
        //定义报文头,用于底层判断轴角顺序
        s_buffer[0] = 255;
        s_buffer[1] = 255;
//        ser.write(s_buffer,2);

        s_buffer[0] = (uint8_t)(soluKiller.angleArray[0]/3.14159*180+180);
        s_buffer[1] = (uint8_t)((uint16_t(soluKiller.angleArray[0]/3.14159*180+180)) >> 8);
        s_buffer[2] = (uint8_t)(soluKiller.angleArray[1]/3.14159*180+180);
        s_buffer[3] = (uint8_t)((uint16_t(soluKiller.angleArray[1]/3.14159*180+180)) >> 8);
        s_buffer[4] = (uint8_t)(soluKiller.angleArray[2]/3.14159*180+180);
        s_buffer[5] = (uint8_t)((uint16_t(soluKiller.angleArray[2]/3.14159*180+180)) >> 8);
        s_buffer[6] = (uint8_t)(soluKiller.angleArray[3]/3.14159*180+180);
        s_buffer[7] = (uint8_t)((uint16_t(soluKiller.angleArray[3]/3.14159*180+180)) >> 8);
        s_buffer[8] = (uint8_t)(soluKiller.angleArray[4]/3.14159*180+180);
        s_buffer[9] = (uint8_t)((uint16_t(soluKiller.angleArray[4]/3.14159*180+180)) >> 8);
        s_buffer[10] = (uint8_t)(soluKiller.angleArray[5]/3.14159*180+180);
        s_buffer[11] = (uint8_t)((uint16_t(soluKiller.angleArray[5]/3.14159*180+180)) >> 8);
//        ser.write(s_buffer,12);

//        ROS_INFO("-----------\nI Send:theta1 %f theta2 %f theta3 %f theta4 %f",
//                 soluKiller.angleArray[0]/3.14159*180,soluKiller.angleArray[1]/3.14159*180,soluKiller.angleArray[2]/3.14159*180,
//                soluKiller.angleArray[3]/3.14159*180);

    }else if(axisAngles.data.at(6) == 1 && is_angleArrayUpdated == 1){
        //定义报文头,用于底层判断轴角顺序
        s_buffer[0] = 255;
        s_buffer[1] = 255;
        ser.write(s_buffer,2);

        s_buffer[0] = (uint8_t)(axisAngles.data.at(0)/3.14159*180+180);
        s_buffer[1] = (uint8_t)((uint16_t(axisAngles.data.at(0)/3.14159*180+180)) >> 8);
        s_buffer[2] = (uint8_t)(axisAngles.data.at(1)/3.14159*180+180);
        s_buffer[3] = (uint8_t)((uint16_t(axisAngles.data.at(1)/3.14159*180+180)) >> 8);
        s_buffer[4] = (uint8_t)(axisAngles.data.at(2)/3.14159*180+180);
        s_buffer[5] = (uint8_t)((uint16_t(axisAngles.data.at(2)/3.14159*180+180)) >> 8);
        s_buffer[6] = (uint8_t)(axisAngles.data.at(3)/3.14159*180+180);
        s_buffer[7] = (uint8_t)((uint16_t(axisAngles.data.at(3)/3.14159*180+180)) >> 8);
        s_buffer[8] = (uint8_t)(axisAngles.data.at(4)/3.14159*180+180);
        s_buffer[9] = (uint8_t)((uint16_t(axisAngles.data.at(4)/3.14159*180+180)) >> 8);
        s_buffer[10] = (uint8_t)(axisAngles.data.at(5)/3.14159*180+180);
        s_buffer[11] = (uint8_t)((uint16_t(axisAngles.data.at(5)/3.14159*180+180)) >> 8);
        ser.write(s_buffer,12);

//        ROS_INFO("-----------\nI Send:theta1 %f theta2 %f theta3 %f theta4 %f theta5 %f theta6 %f",
//                 axisAngles.data.at(0)/3.14159*180,axisAngles.data.at(1)/3.14159*180,axisAngles.data.at(2)/3.14159*180,
//                axisAngles.data.at(3)/3.14159*180,axisAngles.data.at(4)/3.14159*180,axisAngles.data.at(5)/3.14159*180);

        is_angleArrayUpdated = 0;
    }
}

//---------------------------------------------------------------------------------------------

int main (int argc, char** argv)
{
    setlocale(LC_ALL,"zh_CN.UTF-8");
    //------------------------------------------------------------------
    //Init the ROS node
    ros::init(argc, argv, "SerialTalker");
    ros::NodeHandle nh;
    //订阅话题，将角度命令发送至驱动板执行
    ros::Subscriber write_sub = nh.subscribe("Qt_Msg", 1000, write_callback);
    //订阅QT话题，接收字符串
    ros::Subscriber textString_sub = nh.subscribe("Qt_textString", 1000, readTextString_callback);
    //发布话题
    read_pub = nh.advertise<std_msgs::Int32>("stm_publish", 1000);
    //Init the Marker(textPoint) in RVIZ
    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    //添加RVIZ默认坐标系和Maker坐标系textFrame的坐标系转换关系
    tf::TransformBroadcaster broadcaster;

    ros::Timer timer_1 = nh.createTimer(ros::Duration(0.04), timerCallback);
    axisAngles.data.push_back(210);
    axisAngles.data.push_back(150);
    axisAngles.data.push_back(80);
    axisAngles.data.push_back(60);
    axisAngles.data.push_back(80);
    axisAngles.data.push_back(100);
    axisAngles.data.push_back(0);
    //串口更新控制频率 == 50HZ
    ros::Timer timer_2 = nh.createTimer(ros::Duration(0.02), updateAngles);

    //------------------------------------------------------------------
    //Init the FreeType

    filename = "/usr/share/fonts/truetype/兰亭黑简.TTF";
    angle         = ( 0 / 360 ) * 3.14159 * 2;
    target_height = HEIGHT;
    FT_Init_FreeType( &library );

    matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
    matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
    matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
    matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
    pen.x = 15 * 64;
    pen.y = ( target_height - 100 ) * 64;
    //------------------------------------------------------------------
    //Init Serial Port communicating with stm32
    try
    {
            //设置串口属性，并打开串口
            ser.setPort("/dev/ttyUSB0");
            ser.setBaudrate(115200);
            serial::Timeout to = serial::Timeout::simpleTimeout(1000);
            ser.setTimeout(to);
            ser.open();
    }
    catch (serial::IOException& e)
    {
            ROS_ERROR_STREAM("Unable to open port ");
            //return -1;
    }
    //检测串口是否已经打开，并给出提示信息
    if(ser.isOpen())
    {
            ROS_WARN_STREAM("Serial Port initialized");
    }
    else
    {
            ROS_ERROR_STREAM("Serial Port did not open");
    }
    //加载笔画XML文件
    if(doc.LoadFile( "/home/pi/catkin_qi/src/fam/stroke_data/handwriting-zh_CN-gb2312.xml" ) == 0)
        ROS_WARN_STREAM("Load XML succesfully..");
    else
        ROS_ERROR_STREAM("Can not find this XML file..");

    //------------------------------------------------------------------
    //指定循环的频率

    ros::Rate loop_rate(50);
    int index = 0;
    while(ros::ok())
    {
        //定义坐标系与世界坐标系的转化关系
        broadcaster.sendTransform(tf::StampedTransform( tf::Transform(tf::Quaternion(0, 0, 0, 1),
                                                                tf::Vector3(0.0, 0.0, 0.0)),ros::Time::now(),"map", "textFrame"));
        //定时更新汉字在RVIZ中的显示
        if(index==15)
            marker_pub.publish(points);
        else if (index ==30) {
            marker_pub.publish(pointWork);
        }else if (index ==45) {
            marker_pub.publish(pointIdeal);
            index = 0;
        }
        index++;

        //loop_rate.sleep();
        ros::spinOnce();
    }

    //------------------------------------------------------------------
    //Exit and delete the Freetype value
    FT_Done_FreeType( library );
}


/*******************************************************************************************************************
 * End Of File
 * ************************************/
