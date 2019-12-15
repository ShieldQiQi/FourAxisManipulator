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
/* @Parama defination
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
        Stroke findStroke(unsigned char (&imageBuffer)[100][1000]);
        //删除弹入队列之后的笔画对应的点、对其它点进行笔画识别
        void removeRecognizedPoints(unsigned char (&image)[100][1000]);

	//分析汉字部首，存入队列中
        void Analyse(unsigned char (&image)[100][1000]);
	
private:
	
};

void Recognize::removeRecognizedPoints(unsigned char (&image)[100][1000])
{
    int queueSize = pointQueue.getSize();
    for (int i =0 ;i<queueSize;i++) {
        //不同笔画赋予不同颜色index，便于查看仿真效果
        //0横线、1竖线、2左竖弯、3右竖弯
        switch (strokeQueue.getRear().index) {
        case 0:
            image[pointQueue.getFront().y][pointQueue.getFront().x] = 2;
            break;
        case 1:
            image[pointQueue.getFront().y][pointQueue.getFront().x] = 3;
            break;
        case 2:
            image[pointQueue.getFront().y][pointQueue.getFront().x] = 4;
            break;
        case 3:
            image[pointQueue.getFront().y][pointQueue.getFront().x] = 5;
            break;
        }
        //ROS_INFO("y:%d x:%d\n",pointQueue.getFront().y,pointQueue.getFront().x);
        pointQueue.pop();
    }
    pointQueue.ClearQueue();
}

Stroke Recognize::findStroke(unsigned char  (&imageBuffer)[100][1000])
{
    //ROS_INFO("e\n");
    Point point,pointTemp;
    point.x = pointQueue.getFront().x;
    point.y = pointQueue.getFront().y;
    Stroke strokeTemp;
    ROS_INFO("0\n");
    //judge is there right array, or left array, or left-bottom or right-bottom stroke
    if(imageBuffer[point.y][point.x+1] == 1 && imageBuffer[point.y][point.x+2] == 1
            && imageBuffer[point.y][point.x+3] == 1 && imageBuffer[point.y][point.x+4] == 1)
    {
        ROS_INFO("1\n");
        //find in x Axis
        int n = 5;
        int breakNum = 0;
        for (; breakNum <=0.2*n ; n++)
        {
            //breakNum防止直线被其它笔画截断
            if(imageBuffer[point.y][point.x+n] == 0){
                breakNum++;
                //ROS_INFO("%d\n",n);
            }
            else
                breakNum = 0;
        }
        n = n-breakNum-1;
        //find in y Axis
        int m = 0;
        breakNum = 0;
        for (; breakNum <=0.2*m ; m++)
        {
            //breakNum防止直线被其它笔画截断
            if(imageBuffer[point.y+m][point.x+n] ==  0)
                breakNum++;
            else
                breakNum = 0;
        }
        m = m - breakNum-1;

        //n>15则将该笔画归类为 "一横"
        if(n>15)
        {
            ROS_INFO("2\n");
            //ROS_INFO("%d %d\n",n,m);
            //将识别过的点存入点队列
            for (int i=0;i<=n;i++) {
                pointTemp.x = point.x+i;
                pointTemp.y = point.y;
                pointQueue.push(pointTemp);
                pointTemp.y = point.y+m;
                pointQueue.push(pointTemp);
            }
            for (int i=1;i<m;i++) {
                pointTemp.x = point.x;
                pointTemp.y = point.y+i;
                pointQueue.push(pointTemp);
                pointTemp.x = point.x+n;
                pointQueue.push(pointTemp);
            }
            //输出笔画为"一横"
            strokeTemp.x = point.x;
            strokeTemp.y = point.y;
            strokeTemp.index = 0;
            strokeTemp.scale = n;
            return(strokeTemp);
        }else//否则该笔画为垂直型笔画或者斜线型笔画
        {
            ROS_INFO("3\n");
            //如果包含3个垂直点则认为笔画为直线或者竖弯钩或者竖弯
            if(m>=3)
            {
                ROS_INFO("4\n");
                //左侧有点表示该笔画为竖线
                if(imageBuffer[point.y+m][point.x+n-1] == 1 && imageBuffer[point.y+m][point.x+n-2] == 1)
                {
                    //将识别过的点存入点队列
                    for (int i=0;i<=n;i++) {
                        pointTemp.x = point.x+i;
                        pointTemp.y = point.y;
                        pointQueue.push(pointTemp);
                        pointTemp.y = point.y+m;
                        pointQueue.push(pointTemp);
                    }
                    for (int i=1;i<m;i++) {
                        pointTemp.x = point.x;
                        pointTemp.y = point.y+i;
                        pointQueue.push(pointTemp);
                        pointTemp.x = point.x+n;
                        pointQueue.push(pointTemp);
                    }
                    //输出笔画为"一竖"
                    strokeTemp.x = point.x;
                    strokeTemp.y = point.y;
                    strokeTemp.index = 1;
                    strokeTemp.scale = n;
                    return(strokeTemp);
                }else//否则为竖弯钩或者竖弯
                {
                    ROS_INFO("5\n");
                    if(imageBuffer[point.y+m+1][point.x+n-1] == 1)
                    {
                        ROS_INFO("6\n");
                        //将识别过的点存入点队列、由于特殊性，遍历与之连通的所有点存入点队列
                        int start_x1 = point.x;
                        int start_x2 = point.x+n;
                        int end_x1 = 0,end_x2 = 0;
                        int layerNumMin = 0,layerNumMax = 0;
                        for(int layerNum1=0,layerNum2=0;layerNum1>=0||layerNum2>=0;)
                        {
                            //水平遍历连通的点
                            int neighbourNum1 = 0, neighbourNum2 = 0;
                            for (;neighbourNum1>=0 || neighbourNum2>=0 ;) {
                                if(layerNum1>=0 && neighbourNum1 >=0 && imageBuffer[point.y+layerNum1][start_x1-neighbourNum1]==1){
                                    neighbourNum1++;
                                    end_x1 = start_x1-neighbourNum1;
                                    //将识别过的点存入点队列
                                    pointTemp.x = start_x1-neighbourNum1;
                                    pointTemp.y = point.y+layerNum1;
                                    pointQueue.push(pointTemp);
                                }else {
                                    neighbourNum1 = -1;
                                }

                                if(layerNum2>=0 && neighbourNum2 >=0 && imageBuffer[point.y+layerNum2][start_x2-neighbourNum2]==1){
                                    neighbourNum2++;
                                    end_x2 = start_x2-neighbourNum2;
                                    //将识别过的点存入点队列
                                    pointTemp.x = start_x2-neighbourNum2;
                                    pointTemp.y = point.y+layerNum2;
                                    pointQueue.push(pointTemp);
                                }else {
                                    neighbourNum2 = -1;
                                }
                            }
                            //左右起始点一层一层下移
                            int count=0;
                            for (count=0;imageBuffer[point.y+layerNum1][start_x1-count] ==  0 && layerNum1>=0;) {
                                if(imageBuffer[point.y+layerNum1][start_x1-count] ==  0 && layerNum1>=0)
                                    count++;
                                else if (count>25 && layerNum1>=0){//大于25则层数已遍历完
                                    layerNumMin = layerNum1;
                                    layerNum1 =-1;
                                }
                            }
                            layerNum1=layerNum1==-1?-1:layerNum1+1;
                            start_x1 = start_x1-count;

                            for (count=0;imageBuffer[point.y+layerNum2][start_x2-count] ==  0 && layerNum2>=0;) {
                                if(imageBuffer[point.y+layerNum2][start_x2-count] ==  0 && layerNum2>=0)
                                    count++;
                                else if (count>25 && layerNum2>=0){//大于25则层数已遍历完
                                    layerNumMax = layerNum2;
                                    layerNum2 =-1;
                                }
                            }
                            layerNum2=layerNum2==-1?-1:layerNum2+1;
                            start_x2 = start_x2-count;
                        }
                        for (int i=end_x1;i<=end_x2;i++) {
                            for (int j = layerNumMin;j<=layerNumMax;j++) {
                                //将识别过的点存入点队列
                                pointTemp.x = i;
                                pointTemp.y = j;
                                pointQueue.push(pointTemp);
                            }
                        }

                        strokeTemp.x = point.x;
                        strokeTemp.y = point.y;
                        strokeTemp.index = 2;
                        strokeTemp.scale = n;
                        return(strokeTemp);
                    }else if(imageBuffer[point.y+m+1][point.x+n+1] == 1)
                    {
                        ROS_INFO("7\n");
                        //将识别过的点存入点队列、由于特殊性，遍历与之连通的所有点存入点队列
                        int start_x1 = point.x;
                        int start_x2 = point.x+n;
                        int end_x1 = 0,end_x2 = 0;
                        int layerNumMin = 0,layerNumMax = 0;
                        for(int layerNum1=0,layerNum2=0;layerNum1>=0||layerNum2>=0;)
                        {
                            //水平遍历连通的点
                            int neighbourNum1 = 0, neighbourNum2 = 0;
                            for (;neighbourNum1>=0 || neighbourNum2>=0 ;) {
                                if(layerNum1>=0 && neighbourNum1 >=0 && imageBuffer[point.y+layerNum1][start_x1+neighbourNum1]==1){
                                    neighbourNum1++;
                                    end_x1 = start_x1+neighbourNum1;
                                    //将识别过的点存入点队列
                                    pointTemp.x = start_x1+neighbourNum1;
                                    pointTemp.y = point.y+layerNum1;
                                    pointQueue.push(pointTemp);
                                }else {                                    
                                    neighbourNum1 = -1;
                                }

                                if(layerNum2>=0 && neighbourNum2 >=0 && imageBuffer[point.y+layerNum2][start_x2+neighbourNum2]==1){
                                    neighbourNum2++;
                                    end_x2 = start_x2+neighbourNum2;
                                    //将识别过的点存入点队列
                                    pointTemp.x = start_x2+neighbourNum2;
                                    pointTemp.y = point.y+layerNum2;
                                    pointQueue.push(pointTemp);
                                }else {
                                    neighbourNum2 = -1;
                                }
                            }
                            //左右起始点一层一层下移
                            int count=0;
                            for (count=0;imageBuffer[point.y+layerNum1][start_x1+count] ==  0 && layerNum1>=0;) {
                                if(imageBuffer[point.y+layerNum1][start_x1+count] ==  0 && layerNum1>=0)
                                    count++;
                                else if (count>25 && layerNum1>=0){//大于25则层数已遍历完
                                    layerNumMax = layerNum1;
                                    layerNum1 =-1;
                                }
                            }
                            layerNum1=layerNum1==-1?-1:layerNum1+1;
                            start_x1 = start_x1+count;

                            for (count=0;imageBuffer[point.y+layerNum2][start_x2+count] ==  0 && layerNum2>=0;) {
                                if(imageBuffer[point.y+layerNum2][start_x2+count] ==  0 && layerNum2>=0)
                                    count++;
                                else if (count>25 && layerNum2>=0){//大于25则层数已遍历完
                                    layerNumMin = layerNum2;
                                    layerNum2 =-1;
                                }
                            }
                            layerNum2=layerNum2==-1?-1:layerNum2+1;
                            start_x2 = start_x2+count;
                        }
                        for (int i=end_x1;i<=end_x2;i++) {
                            for (int j = layerNumMin;j<=layerNumMax;j++) {
                                //将识别过的点存入点队列
                                pointTemp.x = i;
                                pointTemp.y = j;
                                pointQueue.push(pointTemp);
                            }
                        }

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
                //ROS_INFO("x");
                strokeQueue.push(findStroke(image));
                //ROS_INFO("y");
                removeRecognizedPoints(image);
                //ROS_INFO("z");
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
