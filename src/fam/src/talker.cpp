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

#define WIDTH   1000
#define HEIGHT  150
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
unsigned char imageBuffer[HEIGHT][WIDTH];
std_msgs::Float64MultiArray axisAngles;
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

Recognize	  recognizer;
LinkQueue<Point>  travelQueue;

static uint8_t s_buffer[6];
static uint8_t r_buffer[6]={0 ,0 ,0 ,0 ,0 ,0};
serial::Serial ser; //声明串口对象

visualization_msgs::Marker points;
visualization_msgs::Marker pointsOrigin;
geometry_msgs::Point p;

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
//话题回调
void write_callback(const std_msgs::Float64MultiArray angleArray)
{
//        ROS_INFO("Target Angles:\n-------------------\n "
//                 "Axis_one %.1f Axis_two %.1f Axis_three %.1f Axis_four %.1f Axis_five %.1f Axis_six %.1f\n",
//                 angleArray.data.at(0),angleArray.data.at(1),angleArray.data.at(2),angleArray.data.at(3),angleArray.data.at(4),angleArray.data.at(5));
        axisAngles.data.at(0) =  angleArray.data.at(0);
        axisAngles.data.at(1) =  angleArray.data.at(1);
        axisAngles.data.at(2) =  angleArray.data.at(2);
        axisAngles.data.at(3) =  angleArray.data.at(3);
        axisAngles.data.at(4) =  angleArray.data.at(4);
        axisAngles.data.at(5) =  angleArray.data.at(5);
}

//---------------------------------------------------
//更新文字在RVIZ中的显示
void updatePoints(visualization_msgs::Marker &points,visualization_msgs::Marker &pointsOrigin)
{
    points.points.clear();
    travelQueue.ClearQueue();
    pointsOrigin.points.clear();

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
    points.color.g = 1.0f;
    points.color.a = 1.0;

    pointsOrigin.points.clear();

    pointsOrigin.header.frame_id  = "/textFrame";
    pointsOrigin.header.stamp = ros::Time::now();
    pointsOrigin.ns = "textPoints";
    pointsOrigin.action = visualization_msgs::Marker::ADD;
    pointsOrigin.pose.orientation.w = 1.0;
    pointsOrigin.id = 0;
    pointsOrigin.type = visualization_msgs::Marker::POINTS;
    // POINTS markers use x and y scale for width/height respectively
    pointsOrigin.scale.x = 0.0032;
    pointsOrigin.scale.y = 0.0032;
    pointsOrigin.color.b = 1.0f;
    pointsOrigin.color.a = 1.0;

    travelQueue = recognizer.Analyse(image);
    recognizer.pointQueue.ClearQueue();

    for (int i = 0; i < WIDTH; ++i)
    {
        for(int j =0;j<HEIGHT;j++)
        {
            if(image[j][i] == 3){
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

void readTextString_callback(std_msgs::String textString)
{
        //std_msgs::String  ---->  std::string   ------> std::wstring -------> const wchar_t*
        //ROS_INFO("I heard %s\n",textString.data);
        string testStr(textString.data);
        wstring wstrData;
        wstrData = s2ws(testStr);
        const wchar_t* pWstrData = wstrData.c_str();

        //draw the text to bitmap
        num_chars = wcslen(pWstrData);

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
        updatePoints(points,pointsOrigin);

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
        pointsOrigin.points.push_back(p);
        travelQueue.pop();
    }else {
        travelQueue.ClearQueue();
    }
}

void updateAngles(const ros::TimerEvent& e)
{
    if(ser.available()){
            ser.read(r_buffer,6);
            //read_pub.publish(number);
    }
    //定义报文头,用于底层判断轴角顺序
    s_buffer[0] = 0;
    ser.write(s_buffer,1);
    //发送报文内容
    s_buffer[0] = (uint8_t)axisAngles.data.at(0);
    s_buffer[1] = (uint8_t)axisAngles.data.at(1);
    s_buffer[2] = (uint8_t)axisAngles.data.at(2);
    s_buffer[3] = (uint8_t)axisAngles.data.at(3);
    s_buffer[4] = (uint8_t)axisAngles.data.at(4);
    s_buffer[5] = (uint8_t)axisAngles.data.at(5);
    ser.write(s_buffer,6);
}

//---------------------------------------------------------------------------------------------

int main (int argc, char** argv)
{
    //------------------------------------------------------------------
    //Init the ROS node
    ros::init(argc, argv, "SerialTalker");
    ros::NodeHandle nh;
    //订阅话题，将角度命令发送至驱动板执行
    ros::Subscriber write_sub = nh.subscribe("Qt_Msg", 1000, write_callback);
    //订阅QT话题，接收字符串
    ros::Subscriber textString_sub = nh.subscribe("Qt_textString", 1000, readTextString_callback);
    //发布话题
    ros::Publisher read_pub = nh.advertise<std_msgs::Int32>("stm_publish", 1000);
    //Init the Marker(textPoint) in RVIZ
    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    //添加RVIZ默认坐标系和Maker坐标系textFrame的坐标系转换关系
    tf::TransformBroadcaster broadcaster;

    ros::Timer timer_1 = nh.createTimer(ros::Duration(0.2), timerCallback);
    axisAngles.data.push_back(210);
    axisAngles.data.push_back(150);
    axisAngles.data.push_back(80);
    axisAngles.data.push_back(60);
    axisAngles.data.push_back(80);
    axisAngles.data.push_back(100);
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
            ROS_INFO_STREAM("Serial Port initialized");
    }
    else
    {
            ROS_ERROR_STREAM("Serial Port did not open");
    }

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
            marker_pub.publish(pointsOrigin);
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
