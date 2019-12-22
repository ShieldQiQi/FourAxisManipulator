/****************************************************************************
 * Project:Give the solution of the Four Axis Manipulator
 * Author:Qi
 * Created Time:2019/12/18
 * *****************************/

#ifndef SOLUTION_H
#define SOLUTION_H

#include <cmath>

class inverseSolutionKiller
{
public:

    inverseSolutionKiller(double a_1=0.016,double a_2=0.103,double a_3=0.097,double h_1=0.18,double h_2=0.09,double x_1=0.018);
    ~inverseSolutionKiller();

    float angleArray[6];

    bool getThetaArray(double x,double y);

private:

    //related D-H and some other parameters
    double a1;
    double a2;
    double a3;
    double h1;
    double h2;
    double x1;

};

bool inverseSolutionKiller::getThetaArray(double x,double y)
{
    angleArray[0] = atan(y/x);

    double q = x/cos(angleArray[0]) - a1 - x1;

    angleArray[2] = acos((0.09*0.09+q*q-a2*a2-a3*a3)/(2*a2*a3));

    double s2 = (-q-0.09*(a3*cos(angleArray[2])+a2)/a3/sin(angleArray[2]))/
            (a3*sin(angleArray[2])+(a3*cos(angleArray[2])+a2)*(a3*cos(angleArray[2])+a2)/(a3*sin(angleArray[2])));
    double c2 = (-0.09+q*(a3*cos(angleArray[2])+a2)/a3/sin(angleArray[2]))/
            (a3*sin(angleArray[2])+(a3*cos(angleArray[2])+a2)*(a3*cos(angleArray[2])+a2)/(a3*sin(angleArray[2])));

    angleArray[1] = atan(s2/c2);

    angleArray[3] = - (angleArray[1]+angleArray[2]);

    angleArray[4] = angleArray[5] = 0;

    return 1;
}

inverseSolutionKiller::inverseSolutionKiller(double a_1,double a_2,double a_3,double h_1,double h_2,double x_1)
{
    a1 = a_1;
    a2 = a_2;
    a3 = a_3;
    h1 = h_1;
    h2 = h_2;
    x1 = x_1;
}

inverseSolutionKiller::~inverseSolutionKiller()
{

}

#endif /*SOLUTION_H*/
