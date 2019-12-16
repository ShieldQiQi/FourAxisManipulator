/****************************************************************************
 * Project:Recognize the Chinese stroke
 * Author:Qi
 * Created Time:2019/12/12
 * *****************************/

#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include "linkqueue.hpp"

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

        LinkQueue<Point> Analyse(unsigned char (&image)[HEIGHT][WIDTH]);

        //判断点是否为轮廓点
        bool isOutlinePoint(unsigned char imageBuffer[HEIGHT][WIDTH],int i,int j);

        //寻找路径点
        void findPath(unsigned char imageBuffer[HEIGHT][WIDTH]);

        //删除原有的实心汉字,构造由点构成的新汉字
        void buildNewImageBuffer(unsigned char (&image)[HEIGHT][WIDTH]);

        //按照汉字笔画,给点队列排序构成路径
        void sortPointQueue(unsigned char (&image)[HEIGHT][WIDTH],int i, int j);
	
private:
	
};

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
    while (!pointQueue.isEmpty()) {
        image[pointQueue.getFront().y][pointQueue.getFront().x] = 2;
        pointQueue.pop();
    }
    pointQueue.ClearQueue();
}

void Recognize::sortPointQueue(unsigned char (&image)[HEIGHT][WIDTH],int i, int j)
{
    Point point;
    if(image[j][i] == 2){
        point.x = i;
        point.y = j;
        pointQueue.push(point);
        //按照汉字书写习惯,从当前点的下半平面(不包括X轴反方向)寻找相邻点
        //优先级:右方>左下方>下方>右下方
        if(image[j][i+1] == 2){
            int n=1;
            for (;image[j][i+n] == 2;n++) {
                point.x = i+n;
                point.y = j;
                pointQueue.push(point);
                image[j][i+n] = 0;
            }
            sortPointQueue(image,i+n-1,j);
        }else if (image[j+1][i-1] == 2) {
            int n=1,m=1;
            for (;image[j+m][i-n] == 2;n++,m++) {
                point.x = i-n;
                point.y = j+m;
                pointQueue.push(point);
                image[j+m][i-n] = 0;
            }
            sortPointQueue(image,i-n+1,j+m-1);
        }else if (image[j+1][i] == 2) {
            int m=1;
            for (;image[j+m][i] == 2;m++) {
                point.x = i;
                point.y = j+m;
                pointQueue.push(point);
                image[j+m][i] = 0;
            }
            sortPointQueue(image,i,j+m-1);
        }else if (image[j+1][i+1] == 2) {
            int n=1,m=1;
            for (;image[j+m][i+n] == 2;n++,m++) {
                point.x = i+n;
                point.y = j+m;
                pointQueue.push(point);
                image[j+m][i+n] = 0;
            }
            sortPointQueue(image,i+n-1,j+m-1);
        }
    }
}

LinkQueue<Point> Recognize::Analyse(unsigned char (&image)[HEIGHT][WIDTH])
{
    findPath(image);
//    buildNewImageBuffer(image);
//    for (int i = 0; i < WIDTH; ++i)
//    {
//        for(int j =0;j < HEIGHT;j++)
//        {
//            sortPointQueue(image,i,j);
//        }
//    }

    return pointQueue;
}

Recognize::Recognize()
{

}

Recognize::~Recognize()
{
	
}


#endif // RECOGNIZE_H
