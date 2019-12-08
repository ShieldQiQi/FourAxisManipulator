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

#define WIDTH   200
#define HEIGHT  30
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
std_msgs::Float64 hipAngle;
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
	
static uint8_t s_buffer[5];
serial::Serial ser; //声明串口对象 

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
                                : image[i][j] < 128 ? '+'
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
	ROS_INFO("I heard: [%f]", angleArray.data.at(0));
	hipAngle.data =  angleArray.data.at(0);
}

void readTextString_callback(std_msgs::String textString) 
{ 		
	//std_msgs::String  ---->  std::string   ------> std::wstring -------> const wchar_t*
	
	string testStr(textString.data);
	wstring wstrData;
	wstrData = s2ws(testStr);
	const wchar_t* pWstrData = wstrData.c_str();
	 
	//draw the text to bitmap
	num_chars = wcslen(pWstrData);
	for ( n = 0; n < num_chars; n++ )
	{
		FT_Set_Transform( face, &matrix, &pen );
		error = FT_Load_Char( face, pWstrData[n], FT_LOAD_RENDER );
		if ( error )
		  continue;
		draw_bitmap( &slot->bitmap,
					 slot->bitmap_left,
					 target_height - slot->bitmap_top );

		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}
	show_image();		
}

//---------------------------------------------------------------------------------------------

int main (int argc, char** argv)
{ 
	
	//---------------------------------------------------
	//Init the FreeType
	
	filename = "/usr/share/fonts/truetype/兰亭黑简.TTF";

	angle         = ( 0 / 360 ) * 3.14159 * 2;        
	target_height = HEIGHT;
	error = FT_Init_FreeType( &library ); 
	error = FT_New_Face( library, filename, 0, &face );
	error = FT_Set_Char_Size( face, 30 * 64, 10*64,
							200, 0 );
	slot = face->glyph;
	
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

	pen.x = 15 * 64;
	pen.y = ( target_height - 22 ) * 64;
	
	//---------------------------------------------------
	//Init the ROS node 
	ros::init(argc, argv, "SerialTalker"); 
	ros::NodeHandle nh; 
	//订阅话题，将角度命令发送至驱动板执行
	ros::Subscriber write_sub = nh.subscribe("Qt_Msg", 1000, write_callback); 
	//订阅QT话题，接收字符串
	ros::Subscriber textString_sub = nh.subscribe("Qt_textString", 1000, readTextString_callback); 
	//发布主题 
	ros::Publisher read_pub = nh.advertise<std_msgs::Int32>("stm_publish", 1000); 
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
		//return -1; 
	}
	
	//---------------------------------------------------
	//指定循环的频率 
	ros::Rate loop_rate(50); 
	while(ros::ok()) 
	{
		if(ser.available()){
			std_msgs::String result; 
			//result.data = ser.read(ser.available()); 
			int number = result.data[0]-' '+32;
			cout<< number << endl;
			//read_pub.publish(number);
	  }
	  s_buffer[0] = (uint8_t)hipAngle.data;
	  //ser.write(s_buffer,1);   //发送串口数据
	  ros::spinOnce();
	}
	
	
	//Exit and delete the Freetype value
	FT_Done_Face    ( face );
	FT_Done_FreeType( library );
}
