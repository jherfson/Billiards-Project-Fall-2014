//VECTOR METHODS GO HERE
//YOU SHOULD ADD ON THE add, normalize, AND OTHER METHODS AS YOU NEED THEM

#include "ball.h"
#include <math.h>

Vector::Vector(float xx, float yy)
{
	x=xx; y=yy;
}
Vector* Vector::add(Vector* v)
{
    Vector* vnew=new Vector(x,y);
    vnew->x+=v->x;
    vnew->y+=v->y;
    return vnew;
}

Vector* Vector::sub(Vector* v)
{
    Vector* vnew=new Vector(x,y);
	vnew->x-=v->x;
	vnew->y-=v->y;
	return vnew;
}
Vector* Vector::mul(float multvalue)
{
   Vector* vnew=new Vector(x,y);
    vnew->x =x*multvalue;
    vnew->y = y*multvalue;
    return vnew;
}

float Vector::length()
{
   float theLength = sqrt(x*x+y*y);
   return theLength;
}
Vector* Vector::normalize()
{
    Vector* nnew=new Vector(x,y);
    nnew->x=x/length();
    nnew->y=y/length();
    return nnew;
}
/*float angle()
{

}*/

float Vector::dot(Vector* dotting)
{
    float dotResult = x*dotting->x + y*dotting->y;
    return dotResult;

}

float Vector::lengthsquared()
{
	return (x*x+y*y);
}
Vector* Vector::VelocityVector(float, float)
{

    Vector* nnew=new Vector(x,y);
    nnew->x=x;
    nnew->y=y;
    return nnew;

}





//TODO:  YOU NEED TO ADD THE REST OF THE VECTOR METHODS...
