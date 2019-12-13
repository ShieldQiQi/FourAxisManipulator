/****************************************************************************
 * Project:Recognize the Chinese stroke
 * Author:Qi
 * Created Time:2019/12/12
 * *****************************/

#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include "linkqueue.hpp"

/* @Parama defination
 * The stroke's 'X' position
 * The stroke's 'Y' position
 * show which stroke is in Chinese Text
 * [index]:0横线、1竖线、2左竖弯、3右竖弯
 * the scale show how big the stroke e.g/the "一" have different scale
 */
struct Stroke
{
        int x;
        int y;
        int index;

        int scale;
};
/*
 * @Parama defination
 * The point's 'X' position
 * The point's 'Y' position
 */
struct Point
{
        int x;
        int y;
};

//-------------------------------------------------------------------------------------
//Stroke recognizer
class Recognize
{
public:
	Recognize();
	~Recognize();
	
	//构造笔画队列
        LinkQueue<Stroke> strokeQueue;

        //构造点缓存队列
        LinkQueue<Point> pointQueue;
	
        //寻找有效的笔画边缘
        Stroke findPath(unsigned char imageBuffer[100][1000],LinkQueue<Point> pointQueue);
        //删除弹入队列之后的笔画对应的点、对其它点进行笔画识别
        void removeRecognizedPoints(unsigned char image[100][1000],LinkQueue<Point> pointQueue);

	//分析汉字部首，存入队列中
	void Analyse(unsigned char (&imageBuffer)[100][1000]);
	
private:
	
};

void Recognize::removeRecognizedPoints(unsigned char image[100][1000],LinkQueue<Point> pointQueue)
{

}

Stroke Recognize::findPath(unsigned char imageBuffer[100][1000],LinkQueue<Point> pointQueue)
{
    Point point;
    point.x = pointQueue.getFront().x;
    point.y = pointQueue.getFront().y;
    Stroke strokeTemp;

    //judge is there right array, or left array, or left-bottom or right-bottom stroke
    if(imageBuffer[point.y][point.x+1] == 1 && imageBuffer[point.y][point.x+2] == 1
            && imageBuffer[point.y][point.x+3] == 1 && imageBuffer[point.y][point.x+4] == 1)
    {
        int n = 5;
        int breakNum = 0;
        for (; breakNum < 0.2*n ; n++)
        {
            //breakNum防止直线被其它笔画截断
            if(imageBuffer[point.y][point.x+n] != 1)
                breakNum++;
            else
                breakNum = 0;
        }
        n = n - breakNum;
        //n>15则将该笔画归类为 "一横"
        if(n>15)
        {
            strokeTemp.x = point.x;
            strokeTemp.y = point.y;
            strokeTemp.index = 0;
            strokeTemp.scale = n;
            return(strokeTemp);
        }else//否则该笔画为垂直型笔画或者斜线型笔画
        {
            //如果包含3个垂直点则认为笔画为直线或者竖弯钩或者竖弯
            if(imageBuffer[point.y+1][point.x+n] == 1 && imageBuffer[point.y+2][point.x+n] == 1
                    && imageBuffer[point.y+3][point.x+n] == 1)
            {
                int n = 4;
                int breakNum = 0;
                for (; breakNum < 0.2*n ; n++)
                {
                    //breakNum防止直线被其它笔画截断
                    if(imageBuffer[point.y+n][point.x+n] != 1)
                        breakNum++;
                    else
                        breakNum = 0;
                }
                n = n - breakNum;
                //左侧或右侧有点表示该笔画为竖线
                if(imageBuffer[point.y+n][point.x+n+1] == 1 ||imageBuffer[point.y+n][point.x+n-1] == 1 )
                {
                    strokeTemp.x = point.x;
                    strokeTemp.y = point.y;
                    strokeTemp.index = 1;
                    strokeTemp.scale = n;
                    return(strokeTemp);
                }else//否则为竖弯钩或者竖弯
                {
                    if(imageBuffer[point.y+n+1][point.x+n-1] == 1)
                    {
                        strokeTemp.x = point.x;
                        strokeTemp.y = point.y;
                        strokeTemp.index = 2;
                        strokeTemp.scale = n;
                        return(strokeTemp);
                    }else if(imageBuffer[point.y+n+1][point.x+n+1] == 1)
                    {
                        strokeTemp.x = point.x;
                        strokeTemp.y = point.y;
                        strokeTemp.index = 3;
                        strokeTemp.scale = n;
                        return(strokeTemp);
                    }
                }
            }
        }
    }
}

void Recognize::Analyse(unsigned char (&image)[100][1000])
{
    Point point;

    for ( int j = 0; j < 100; j++ ){
        for ( int i = 0; i < 1000; i++ ){
            if(image[j][i] == 1 && pointQueue.isEmpty()){
                //将笔画中的第一个点压入队列、构成一个笔画后存入strokeQueue,开始下一循环
                point.x = i;
                point.y = j;
                pointQueue.push(point);
                findPath(image,pointQueue);
                removeRecognizedPoints(image,pointQueue);
            }else if(imageBuffer[j][i] == 1 ){

            }
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
