/****************************************************************************
 * Project:Recognize the Chinese stroke
 * Author:Qi
 * Created Time:2019/12/12
 * *****************************/

#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include "linkqueue.hpp"
#include "tinyxml2.hpp"
#include <sqlite3.h>

using namespace tinyxml2;

/* @Parama defination
 * The point's 'X' position
 * The point's 'Y' position
 */
struct Point
{
    int x;
    int y;

    //在4个方向上定义距离轮廓的长度,100表示无穷大不采用
    int len_12=100;
    int len_6=100;
    int len_3=100;
    int len_9=100;

};

const int WIDTH = 1000;
const int HEIGHT = 150;

//-------------------------------------------------------------------------------------
//Stroke recognizer
class Recognize
{
public:
	Recognize();
	~Recognize();

        //构造点缓存队列
        LinkQueue<Point> pointQueue;

        void Analyse(unsigned char (&image)[HEIGHT][WIDTH]);

        unsigned char imageTemp[HEIGHT][WIDTH];

        //判断点是否为轮廓点
        bool isOutlinePoint(unsigned char imageBuffer[HEIGHT][WIDTH],int i,int j);

        //寻找路径点
        void findPath(unsigned char imageBuffer[HEIGHT][WIDTH]);

        //删除原有的实心汉字,构造由点构成的新汉字
        void buildNewImageBuffer(unsigned char (&image)[HEIGHT][WIDTH]);

        //按照汉字笔画,给点队列排序构成路径
        void sortPointQueue(int i, int j, bool is_firstLayer);

        //读取笔画顺序
        bool GetNodePointerByName(XMLElement* pRootEle, const char* strNodeName,XMLElement** destNode);
	
private:
	
};


bool Recognize::GetNodePointerByName(XMLElement* pRootEle, const char* strNodeName,XMLElement** destNode)
{
    setlocale(LC_ALL,"zh_CN.UTF-8");

    //查找某一汉字,得到对应的笔画数据
    // if equal root node then return
    if (0 == strcmp(strNodeName, pRootEle->Value()))
    {
        *destNode = pRootEle;
        return true;
    }

    XMLElement* pEle = pRootEle;
    for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
    {
        // recursive find sub node return node pointer
        if (0 != strcmp(pEle->Value(), strNodeName))
        {
            GetNodePointerByName(pEle,strNodeName,destNode);
        }
        else
        {
            *destNode = pEle;
            ROS_INFO("destination node name: %s\n", pEle->Value());
            return true;
        }
    }

    return false;


//    XMLDocument doc;
//    if(doc.LoadFile( "/home/pi/catkin_qi/src/fam/stroke_data/handwriting-zh_CN-gb2312.xml" ) == 0)
//        ROS_INFO("Load XML succesfully..");
//    else
//        ROS_INFO("Can not find this XML file..");
//    const char* id = doc.FirstChildElement( "dictionary" )->FirstChildElement( "character" )->FirstChildElement( "utf8" )->GetText();
//    ROS_INFO( "Name of Chinese: %s", id );
//    tinyxml2::XMLText* textNode = doc.FirstChildElement( "dictionary" )->FirstChildElement( "character" )->FirstChildElement( "utf8" )->FirstChild()->ToText();
//    ROS_INFO( "Name of Chinese: %s", textNode->Value() );
}



bool Recognize::isOutlinePoint(unsigned char imageBuffer[HEIGHT][WIDTH],int i,int j)
{
    if(imageBuffer[j-1][i] == 0 || imageBuffer[j+1][i] == 0 || imageBuffer[j][i-1] == 0 || imageBuffer[j][i+1] == 0 ){
        return 1;
    }else{
        return 0;
    }
}

void Recognize::findPath(unsigned char imageBuffer[HEIGHT][WIDTH])
{
    Point point;
    for (int i = 0; i < WIDTH; ++i)
    {
        for(int j =0;j < HEIGHT;j++)
        {
            if(imageBuffer[j][i] == 1 && isOutlinePoint(imageBuffer,i,j) == 0){
                //该点向4个方向延长直到轮廓点,存下长度
                int len = 0;
                for(len = 0;isOutlinePoint(imageBuffer,i,j-len) == 0;len++);
                point.len_12 = len;
                for(len = 0;isOutlinePoint(imageBuffer,i,j+len) == 0;len++);
                point.len_6 = len;
                for(len = 0;isOutlinePoint(imageBuffer,i-len,j) == 0;len++);
                point.len_9 = len;
                for(len = 0;isOutlinePoint(imageBuffer,i+len,j) == 0;len++);
                point.len_3 = len;

                if(point.len_3 -point.len_9<=1 && point.len_3 -point.len_9 >=0){
                    if(point.len_6 -point.len_12<=1 && point.len_6 -point.len_12>=0)
                    {
                        point.x = i;
                        point.y = j;
                        pointQueue.push(point);
                    }else if(point.len_3<point.len_6 && point.len_3<point.len_12)
                    {
                        point.x = i;
                        point.y = j;
                        pointQueue.push(point);
                    }
                }else if (point.len_6 -point.len_12<=1 && point.len_6 -point.len_12>=0 && point.len_6<point.len_3 && point.len_6<point.len_9) {
                    point.x = i;
                    point.y = j;
                    pointQueue.push(point);
                }
            }
        }
    }
}


void Recognize::buildNewImageBuffer(unsigned char (&image)[HEIGHT][WIDTH])
{
    Node<Point>* pNode=new Node<Point>();
    pNode=pointQueue.phead->next;
    while(pNode!=nullptr)
    {
        image[pNode->value.y][pNode->value.x] = 2;
        pNode=pNode->next;
    }
    pointQueue.ClearQueue();
}

//递归排序
void Recognize::sortPointQueue(int i, int j, bool is_firstLayer)
{
    Point point;
    if(imageTemp[j][i] == 2 || !is_firstLayer){
        point.x = i;
        point.y = j;
        if(is_firstLayer)
            pointQueue.push(point);
        //按照汉字书写习惯,从当前点的下半平面(不包括X轴反方向)寻找相邻点
        //优先级:右方>左下方>下方>右下方

        //使用插补,解决笔画中断问题, 在优先联接当前方向的前提下使用贪心算法

        if(imageTemp[j][i+1] == 2){         //右方
            int n=1;
            int breakFlag = 0;
            for (;breakFlag == 0;n++){
                point.x = i+n;
                point.y = j;
                pointQueue.push(point);
                imageTemp[j][i+n] = 3;
                if(imageTemp[j][i+n+1] != 2 && imageTemp[j][i+n+2] != 2 && imageTemp[j][i+n+3] != 2 && imageTemp[j][i+n+4] != 2 &&
                        imageTemp[j][i+n+5] != 2 && imageTemp[j][i+n+6] != 2 && imageTemp[j][i+n+7] != 2 && imageTemp[j][i+n+8] != 2 &&
                        imageTemp[j][i+n+9] != 2 && imageTemp[j][i+n+10] != 2 && imageTemp[j][i+n+11] != 2 && imageTemp[j][i+n+12] != 2){
                    breakFlag = 1;
                }
            }
            sortPointQueue(i+n-1,j,0);
        }else if (imageTemp[j+1][i-1] == 2) {        //左下
            int n=1,m=1;
            int breakFlag = 0;
            for (;breakFlag == 0;n++,m++) {
                point.x = i-n;
                point.y = j+m;
                pointQueue.push(point);
                imageTemp[j+m][i-n] = 3;
                breakFlag = 1;
                if(imageTemp[j+m+1][i-n] != 2)
                    for (int x=1;x<20 && breakFlag == 1;x++) {
                        for (int y=1;y<20 && breakFlag == 1;y++) {
                            if(imageTemp[j+m+y][i-n-x] ==2){
                                breakFlag = 0;
                                m = m+y-1;
                                n = n+x-1;
                            }
                        }
                    }
            }
            sortPointQueue(i-n+1,j+m-1,0);
        }else if (imageTemp[j+1][i] == 2) {         //下方
            int m=1;
            int breakFlag = 0;
            for (;breakFlag == 0;m++) {
                point.x = i;
                point.y = j+m;
                pointQueue.push(point);
                imageTemp[j+m][i] = 3;
                if(imageTemp[j+m+1][i] != 2 && imageTemp[j+m+2][i] != 2 && imageTemp[j+m+3][i] != 2 && imageTemp[j+m+4][i] != 2 &&
                        imageTemp[j+m+5][i] != 2 && imageTemp[j+m+6][i] != 2 && imageTemp[j+m+7][i] != 2 && imageTemp[j+m+8][i] != 2 &&
                        imageTemp[j+m+9][i] != 2 && imageTemp[j+m+10][i] != 2 && imageTemp[j+m+11][i] != 2 && imageTemp[j+m+12][i] != 2){
                    breakFlag = 1;
                }
            }
            sortPointQueue(i,j+m-1,0);
        }else if (imageTemp[j+1][i+1] == 2) {       //右下
            int n=1,m=1;
            int breakFlag = 0;
            for (;breakFlag == 0;n++,m++) {
                point.x = i+n;
                point.y = j+m;
                pointQueue.push(point);
                imageTemp[j+m][i+n] = 3;
                breakFlag = 1;
                if(imageTemp[j+m+1][i+n] != 2)
                    for (int x=1;x<20 && breakFlag == 1;x++) {
                        for (int y=1;y<20 && breakFlag == 1;y++) {
                            if(imageTemp[j+m+y][i+n+x] ==2){
                                breakFlag = 0;
                                m = m+y-1;
                                n = n+x-1;
                            }
                        }
                    }
            }
            sortPointQueue(i+n-1,j+m-1,0);
        }
    }
}

void Recognize::Analyse(unsigned char (&image)[HEIGHT][WIDTH])
{
    findPath(image);
    buildNewImageBuffer(image);
    for (int j =0;j < HEIGHT;j++)
    {
        for(int i = 0; i < WIDTH; i++)
        {
            imageTemp[j][i] = image[j][i];
            image[j][i] = image[j][i]==2?3:image[j][i];
        }
    }

    //from up to bottom and from left to right to travelse
    for (int j =0;j < HEIGHT;j++)
    {
        for(int i = 0; i < WIDTH; i++)
        {
            sortPointQueue(i,j,1);
        }
    }
}

Recognize::Recognize()
{

}

Recognize::~Recognize()
{
	
}


#endif // RECOGNIZE_H
