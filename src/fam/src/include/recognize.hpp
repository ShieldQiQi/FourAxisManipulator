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
#include <stdlib.h>
#include <map>
#include <vector>

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

/* @Parama defination
 * which stroke is (stroke name)
 * ----------------------------------------------------
 * 横: -->右
 * 竖: -->下
 * 点: -->右下
 * 提: -->右上
 *
 * 撇: -->左下-->左下
 * 撇点: -->左下-->右下
 * 撇折: -->左下-->右(右上)
 *
 * 竖提: -->下-->右上
 * 竖弯: -->下-->右
 * 竖钩: -->下-->左上
 * 竖折: -->下-->右
 * 竖弯钩: -->下-->右-->上
 * 竖折撇: -->下-->右-->左下
 * 竖折折: -->下-->右-->下
 * 竖折折钩: -->下-->右-->左下-->上
 *
 * 捺: -->右下-->右下
 * 横撇: -->右-->左下-->左下
 * 横勾: -->右-->左下
 * 横折: -->右-->下
 * 斜钩: -->右下-->右下-->右下-->上
 * 弯钩: -->右下-->下-->下-->上
 * 卧钩: -->右下-->右下-->左上
 * 横斜钩: -->右-->下-->右下-->上
 * 横折提: -->右上-->下-->右上
 * 横折弯: -->右-->下-->右
 * 横折钩: -->右-->下-->左上
 * 横折折: -->右-->下-->右
 * 横撇弯钩: -->右-->左下-->右下-->左上
 * 横折弯钩: -->右-->下-->右-->上
 * 横折折撇: -->右-->左下-->右-->左下
 * 横折折折: -->右-->下-->右-->下
 * 横折折折钩: -->右-->左下-->右-->左下-->上
 * ----------------------------------------------------
 * the order num of the stroke in current Chinese
 * how many point in this stroke
 * the points queue in this stroke which include X and Y data message..
 */
struct Stroke
{
    const char*               strokeName="未知";
    uint8_t             orderNum;
    int                 strokePointNum;
    LinkQueue<Point>    strokePointQueue;
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

        map<const char*,uint8_t> strokeMap;

        //构造点缓存队列
        LinkQueue<Point> pointQueue;
        //笔画队列
        LinkQueue<Stroke> strokeQueue;

        vector<uint8_t> strokeNumberArray;
        vector<uint8_t> serveStrokeNumberArray;

        void Analyse(unsigned char (&image)[HEIGHT][WIDTH],bool is_UseXML);
        unsigned char imageTemp[HEIGHT][WIDTH];
        //判断点是否为轮廓点
        bool isOutlinePoint(unsigned char imageBuffer[HEIGHT][WIDTH],int i,int j);
        //寻找路径点
        void findPath(unsigned char imageBuffer[HEIGHT][WIDTH]);
        //删除原有的实心汉字,构造由点构成的新汉字
        void buildNewImageBuffer(unsigned char (&image)[HEIGHT][WIDTH]);
        //按照汉字笔画,给点队列排序构成路径
        void sortPointQueue(int i, int j, bool is_firstLayer);
        void sortPointQueueByXML(const char* strokeName);
        //读取笔画顺序
        bool GetNodePointerByName(XMLElement* pRootEle, const char* strNodeName,XMLElement* &destNode);
        bool GetStrokeMsg(XMLElement* destNode);
	
private:
	
};


bool Recognize::GetStrokeMsg(XMLElement* destNode)
{
    strokeQueue.ClearQueue();

    XMLElement* pEle = destNode;
    XMLElement* pEleDeeper;
    int strokecount = 0;
    int pointcount = 0;

    for(pEle = destNode->NextSiblingElement()->FirstChildElement();pEle;pEle = pEle->NextSiblingElement())
    {
        Stroke strokeTemp;
        strokeQueue.push(strokeTemp);
    }

    Node<Stroke>* pStroke =  strokeQueue.phead->next;

    for(pEle = destNode->NextSiblingElement()->FirstChildElement();pEle;pEle = pEle->NextSiblingElement())
    {
        Point pointTemp;

        for(pointcount = 0,pEleDeeper = pEle->FirstChildElement();pEleDeeper;pEleDeeper = pEleDeeper->NextSiblingElement())
        {
            pointTemp.x = atoi(pEleDeeper->Attribute("x"));
            pointTemp.y = atoi(pEleDeeper->Attribute("y"));
            pStroke->value.strokePointQueue.push(pointTemp);
            pointcount++;
        }
        Node<Point>* pPoint =  pStroke->value.strokePointQueue.phead->next;
        //先判断两个点的笔画\有4个
        if (pointcount == 2) {
            if(1.0*abs(pPoint->value.x-pPoint->next->value.x)/abs(pPoint->value.y-pPoint->next->value.y) < 0.4){
                pStroke->value.strokeName = "竖";
            }else if(1.0*abs(pPoint->value.x-pPoint->next->value.x)/abs(pPoint->value.y-pPoint->next->value.y) > 2.5){
                pStroke->value.strokeName = "横";
            }else if (pPoint->value.y-pPoint->next->value.y > 0) {
                pStroke->value.strokeName = "提";
            }else{
                pStroke->value.strokeName = "点";
            }
        }//再依据方向判断剩下的笔画
        else if(pPoint->value.x-pPoint->next->value.x > 0 && 1.0*abs(pPoint->value.x-pPoint->next->value.x)/abs(pPoint->value.y-pPoint->next->value.y) > 0.15){
            //第一次向左下走\可能为撇\撇点\撇折
            if(pPoint->next->value.x-pPoint->next->next->value.x > 0){
                pStroke->value.strokeName = "撇";
            }else if(pPoint->next->value.y-pPoint->next->next->value.y > 0){
                if(pointcount == 3){
                    pStroke->value.strokeName = "撇折";
                }else if(pointcount == 4) {
                    pStroke->value.strokeName = "竖折撇";
                }else{
                    pStroke->value.strokeName = "竖折折钩";
                }
            }else {
                pStroke->value.strokeName = "撇点";
            }
        }//可能为 竖提\竖弯\竖钩\竖折\竖弯钩\竖折撇\竖折折\竖折折钩
        else if(1.0*abs(pPoint->value.x-pPoint->next->value.x)/abs(pPoint->value.y-pPoint->next->value.y) < 0.15) {
            if(pointcount == 3){
                if(pPoint->next->value.x-pPoint->next->next->value.x > 0 && pPoint->next->value.y-pPoint->next->next->value.y > 0){
                    pStroke->value.strokeName = "竖钩";
                }else if (pPoint->next->value.x-pPoint->next->next->value.x < 0 && 1.0*abs(pPoint->next->next->value.x-pPoint->next->value.x)/abs(pPoint->next->next->value.y-pPoint->next->value.y) < 1.5) {
                    pStroke->value.strokeName = "竖提";
                }else if(pPoint->next->value.x-pPoint->next->next->value.x < 0){
                    pStroke->value.strokeName = "竖折";
                }else {
                    pStroke->value.strokeName = "竖撇";
                }
            }else if(pointcount == 4){
                if((1.0*abs(pPoint->next->next->value.x-pPoint->next->next->next->value.x)/abs(pPoint->next->next->value.y-pPoint->next->next->next->value.y) < 0.3)){
                    pStroke->value.strokeName = "竖折折";
                }else{
                    pStroke->value.strokeName = "竖弯";
                }
            }else{
                if(pPoint->next->value.x-pPoint->next->next->value.x > 0){
                    pStroke->value.strokeName = "竖撇";
                }else if(pPoint->next->next->value.x-pPoint->next->next->next->value.x > 0){
                    pStroke->value.strokeName = "竖折折钩";
                }else {
                    pStroke->value.strokeName = "竖弯钩";
                }
            }
        }else if(1.0*abs(pPoint->value.x-pPoint->next->value.x)/abs(pPoint->value.y-pPoint->next->value.y) < 2){
            if(pointcount == 3 || pointcount == 4){
                pStroke->value.strokeName = "捺";
            }else if(pPoint->next->next->next->value.x-pPoint->next->next->value.x < 0){
                pStroke->value.strokeName = "弯钩";
            }else if(pPoint->next->next->next->value.y-pPoint->next->next->value.y < 0){
                pStroke->value.strokeName = "卧钩";
            }else{
                pStroke->value.strokeName = "斜钩";
            }
        }else if(1.0*abs(pPoint->value.x-pPoint->next->value.x)/abs(pPoint->value.y-pPoint->next->value.y) > 2) {
            if(pointcount == 3){
                if(1.0*abs(pPoint->next->value.x-pPoint->next->next->value.x)/abs(pPoint->next->value.y-pPoint->next->next->value.y) < 0.7){
                    pStroke->value.strokeName = "横折";
                }else{
                    pStroke->value.strokeName = "横钩";
                }
            }else if(pPoint->next->value.x-pPoint->next->next->value.x > 0 && pPoint->next->next->value.x-pPoint->next->next->next->value.x > 0
                     && pPoint->next->next->value.y-pPoint->next->next->next->value.y < 0){
                pStroke->value.strokeName = "横撇";
            }else if(pointcount == 4){
                if(pPoint->next->next->value.x-pPoint->next->next->next->value.x > 0){
                    pStroke->value.strokeName = "横折钩";
                }else if (abs(pPoint->next->next->value.x-pPoint->next->next->next->value.x)/abs(pPoint->next->next->value.y-pPoint->next->next->next->value.y) > 1.8) {
                    pStroke->value.strokeName = "横折折";
                }else{
                    pStroke->value.strokeName = "横折提";
                }
            }else if(pointcount == 5){
                if(pPoint->next->next->next->value.x-pPoint->next->next->next->next->value.x > 0){
                    if(pPoint->next->next->next->value.x-pPoint->next->next->next->next->value.x < 0){
                        pStroke->value.strokeName = "横折折折";
                    }else {
                        pStroke->value.strokeName = "横撇弯钩";
                    }
                }else{
                    pStroke->value.strokeName = "横折弯";
                }
            }else if(pointcount == 6){
                if(pPoint->next->next->next->value.x-pPoint->next->next->next->next->value.x > 0){
                    if(pPoint->next->next->next->next->value.y-pPoint->next->next->next->next->next->value.y > 0){
                        pStroke->value.strokeName = "横折折折钩";
                    }else {
                        pStroke->value.strokeName = "横折折撇";
                    }
                }else if(pPoint->next->next->next->value.y-pPoint->next->next->next->next->value.y > 0){
                    pStroke->value.strokeName = "横折弯钩";
                }else {
                    pStroke->value.strokeName = "横斜钩";
                }
            }
        }

        pStroke->value.orderNum = strokecount+1;
        pStroke->value.strokePointNum = pointcount;
        pStroke = pStroke->next;
        strokecount++;
    }
}

bool Recognize::GetNodePointerByName(XMLElement* pRootEle, const char* strNodeName,XMLElement* &destNode)
{
    setlocale(LC_ALL,"zh_CN.UTF-8");

    //查找某一汉字,得到对应的笔画数据
    // if equal root node then return
    if(pRootEle->GetText()!=nullptr){
        if (0 == strcmp(strNodeName, pRootEle->GetText()))
        {
//            ROS_INFO("1destination node name: %s\n", pRootEle->GetText());
            destNode = pRootEle;
            return true;
        }
    }

    XMLElement* pEle = pRootEle;
    for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
    {
        if(pEle->GetText()!=nullptr){
            // recursive find sub node return node pointer
            if (0 != strcmp(pEle->GetText(), strNodeName))
            {
                if(GetNodePointerByName(pEle,strNodeName,destNode) == true)
                    return true;
            }
            else
            {
//                ROS_INFO("2destination node name: %s\n", pEle->GetText());
                destNode = pEle;
                return true;
            }
        }else {
            if(GetNodePointerByName(pEle,strNodeName,destNode) == true)
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

void Recognize::sortPointQueueByXML(const char *strokeName)
{
    ROS_INFO("%s",strokeName);
}
void Recognize::Analyse(unsigned char (&image)[HEIGHT][WIDTH],bool is_UseXML)
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
    if(is_UseXML){
        //排序前对笔画按照复杂到简单的排序
        while (strokeQueue.phead->next) {
            ROS_INFO("num %d:the stroke is '%s' which have %d point(s) the stroke number is %d",
                     strokeQueue.phead->next->value.orderNum,strokeQueue.phead->next->value.strokeName,strokeQueue.phead->next->value.strokePointNum
                     ,strokeMap[(char*)strokeQueue.phead->next->value.strokeName]);
            strokeNumberArray.push_back(strokeMap[(char*)strokeQueue.phead->next->value.strokeName]);
            serveStrokeNumberArray.push_back(strokeMap[(char*)strokeQueue.phead->next->value.strokeName]);
            strokeQueue.pop();
        }
        for (int i = 0,temp = 0;i<serveStrokeNumberArray.size();i++) {
            temp = serveStrokeNumberArray[i];
            for (int j =i;j<serveStrokeNumberArray.size();j++) {
                if(temp < serveStrokeNumberArray[j]){
                    serveStrokeNumberArray[i] = serveStrokeNumberArray[j];
                    serveStrokeNumberArray[j] = temp;
                    temp = serveStrokeNumberArray[i];
                }
            }
        }

        Node<Stroke> *pStroke = strokeQueue.phead->next;
        //按照笔画复杂程度顺序分别在点数组中匹配相应的笔画
        for (int i = 0;i<serveStrokeNumberArray.size();i++) {

            for(map<const char*,uint8_t>::iterator it = strokeMap.begin();it!=strokeMap.end();it++)
            {
                if(it->second==serveStrokeNumberArray[i])
                    sortPointQueueByXML(it->first);
            }
        }

        serveStrokeNumberArray.clear();
        strokeNumberArray.clear();
        strokeQueue.ClearQueue();
    }else {
        for (int j =0;j < HEIGHT;j++)
        {
            for(int i = 0; i < WIDTH; i++)
            {
                sortPointQueue(i,j,1);
            }
        }
    }
}

Recognize::Recognize()
{
    strokeMap["点"] = 0;
    strokeMap["提"] = 1;
    strokeMap["横"] = 2;
    strokeMap["竖"] = 3;
    strokeMap["撇"] = 4;
    strokeMap["捺"] = 5;

    strokeMap["横勾"] = 6;
    strokeMap["横撇"] = 7;
    strokeMap["横折"] = 8;
    strokeMap["撇点"] = 9;
    strokeMap["撇折"] = 10;
    strokeMap["竖提"] = 11;
    strokeMap["竖弯"] = 12;
    strokeMap["竖钩"] = 13;
    strokeMap["竖折"] = 14;

    strokeMap["斜钩"] = 15;
    strokeMap["弯钩"] = 16;
    strokeMap["卧钩"] = 17;

    strokeMap["竖弯钩"] = 18;
    strokeMap["竖折撇"] = 19;
    strokeMap["竖折折"] = 20;

    strokeMap["横斜钩"] = 21;
    strokeMap["横折提"] = 22;
    strokeMap["横折弯"] = 23;
    strokeMap["横折钩"] = 24;
    strokeMap["横折折"] = 25;


    strokeMap["横撇弯钩"] = 26;
    strokeMap["横折弯钩"] = 27;
    strokeMap["横折折撇"] = 28;
    strokeMap["横折折折"] = 29;
    strokeMap["竖折折钩"] = 30;
    strokeMap["横折折折钩"] = 31;
}

Recognize::~Recognize()
{
	
}


#endif // RECOGNIZE_H
