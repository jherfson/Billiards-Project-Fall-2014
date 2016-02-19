//CSC432 FALL 2014, Michael Black
//2D BALL MOVEMENT DEMO
//YOU CAN USE THIS AS A BASIS FOR YOUR PROJECT 1
//
//THIS HAS BALLS MOVING AROUND IN 2D AND BOUNCING OFF WALLS.  COLLISIONS ARE NOT YET DETECTED.
//
//A MOUSE LISTENER IS INCLUDED.  ON DOUBLECLICK, A NEW BALL APPEARS.
//
//When you see the words "TODO", you should add code there

//necessary libraries
#include <iostream>
#include <QtWidgets>
#include <math.h>

//your class definitions
#include "ball.h"

//constants.  these numbers can be adjusted
const int MAXBALLS=1000;	//more than MAXBALLS balls will cause a segmentation fault
//const int BOUND=500;
const int WIDTH=400;		//size of the screen
const int HEIGHT=800;
const int MARGIN=50;		//balls are created at least this far from the edge
const int RADIUS=10;		//all balls start with this radius
const float RESTITUTION=1.0;	//energy lost in a collision: from 0.0 (inelastic: all is lost) to 1.0 (elastic: nothing is lost)
const float GRAVITY=0.5;	//not currently used, but you can add GRAVITY * DT to a ball's y velocity in move() to create a gravity effect
const float DT=0.2;		//smaller = slower simulation = more accurate
const int TIMEUNIT=1000/66;	//timer update function runs 66 times per second
const float FrictionConstant = .05*9.8;
//globals
int BallCount=16;		//starting with 10 balls.  you can change this.
int HoleCount=6;
bool clicked=false;

Ball* balls[MAXBALLS];		//array of ball pointers

Hole* holes[10];            //array of 10 hole pointers
Rectangle* rectangle[10];
QGraphicsScene* thescene;	//the canvas holding the ball graphical objects
BallView* view;			//the window

//ball constructor
//makes a ball at a random position, with a random velocity, and a random mass


Rectangle::Rectangle(int x, int y){
    setPos(0,0);
    color.setGreen(255);
}



Ball::Ball(int x, int y, int z) //changed
{
    //balls[0]->position->x=0;
    //balls[0]->position->y=0;
    setPos(x,y);
   // balls[0]->position=new Vector(x(),y());
   //setPos(qrand()%(WIDTH-MARGIN*2)+MARGIN,qrand()%(HEIGHT-MARGIN*2)+MARGIN);   //proper pos
     position=new Vector(x,y);
    //position=new Vector(x(),y());    //proper poss
    //mass=(qrand()%10000)/1000.0;     /random mass
    mass = .375;
    color.setBlue(255);
	//the color is random.  use a darker shade to indicate greater mass.
    //color.setBlue(0); color.setRed(0); color.setGreen(0);
    //switch(qrand()%3)
    //{
        //case 0:color.setBlue(255-(int)(25*mass)); break;
        //case 1:color.setRed(255-(int)(25*mass)); break;
        //case 2:color.setGreen(255-(int)(25*mass)); break;
    //}

	//choosing an initial velocity with random direction but magnitude 1
	//all balls start out going the same speed
    //float speed=qrand()%5+15;
    float speed = 0;   //no initial velocity
	float angle=(qrand()%360)*2*3.14/360.0;
    velocity=new Vector(speed*cos(angle),speed*sin(angle));
    //velocity=0;
	//all balls have the same radius
   /* for(int i=0; i<23;i++)
    *
    {
        if(i<16)
        {
            radius=RADIUS;
        }
        else
        {
            radius=RADIUS+3;
        }*/
    radius=RADIUS;
    colorvalue=z;
    if (z==1){
        color.setBlue(255);
    }
    if (z==2){
        color.setRed(255);
    }
    if (z==3){
        color.setBlue(255);
        color.setRed(255);
        color.setGreen(255);

    }
}

Hole::Hole(int x, int y)
{
    setPos(x,y);
    position=new Vector(x,y);
    color.setBlue(1);
    radius = RADIUS+3;
    //mass = 10;
    //float speed = 0;   //no initial velocity
    //float angle=(qrand()%360)*2*3.14/360.0;
    //velocity=new Vector(speed*cos(angle),speed*sin(angle));
}

//where to update screen relative to a ball's center position
//you shouldn't need to change this
QRectF Ball::boundingRect() const
{
	return QRectF(-radius,-radius,radius*2,radius*2);
}
//called by qt system to paint the ball when it moves.  you shouldn't need to change this

QRectF Hole::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}
QRectF Rectangle::boundingRect() const
{
    return QRectF(0,0,WIDTH,HEIGHT);
}
void Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(color);
    painter->drawRect(0,0,400,800);
}




void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //painter->setBrush(color);//new
    //painter->drawRect(0,0,400,800);//new
	painter->setBrush(color);
	painter->drawEllipse(-radius,-radius,radius*2,radius*2);
    //draw text or draw string
    //drawLine
}

void Hole::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //painter->setBrush(color);//new
    //painter->drawRect(0,0,400,800);//new
    painter->setBrush(color);
    painter->drawEllipse(-radius,-radius,radius*2,radius*2);
    //draw text or draw string
    //drawLine
}
//called when a collision happens.  your code will go in here
void handleCollision(Ball* ball1, Ball* ball2)
{
    //std::cout << "Collision happened" << std::endl;
    Vector* collide = ball1->position->sub(ball2->position);
    float distance = sqrt(collide->lengthsquared());
    float adjust = (ball1->radius + ball2->radius - distance)/distance;
    Vector* adjuster = collide->mul(adjust);
    Vector* adjuster1 = adjuster->mul((1/ball1->mass)/(1/ball1->mass + 1/ball2->mass));
    Vector* adjuster2 = adjuster->mul((1/ball2->mass)/(1/ball1->mass + 1/ball2->mass));
    ball1->position = ball1->position->add(adjuster1);
    ball2->position = ball2->position->sub(adjuster2);
    //Vector* XcollisionNorm = normalize(collide);
    Vector* XcollisionNorm = collide->normalize();
    Vector* VelocityDiff = ball1->velocity->sub(ball2->velocity);
    float VelocityAdjust = VelocityDiff->dot(XcollisionNorm);
    float VelocityAdjust1 = (1+RESTITUTION)*VelocityAdjust*((1/ball1->mass)/(1/ball1->mass + 1/ball2->mass));
    float VelocityAdjust2 = (1+RESTITUTION)*VelocityAdjust*((1/ball2->mass)/(1/ball1->mass + 1/ball2->mass));
    ball1->velocity = ball1->velocity->sub(XcollisionNorm->mul(VelocityAdjust1));
    ball2->velocity = ball2->velocity->add(XcollisionNorm->mul(VelocityAdjust2));
    delete collide, adjuster, adjuster1, adjuster2, XcollisionNorm, VelocityDiff, VelocityAdjust,  VelocityAdjust1,  VelocityAdjust2;

	//TODO: INSERT YOUR 2D COLLISION HANDLING CODE HERE...
    //Declare variables so u can delete them... ball1/2position and ball1velocity/ball2velocity etc
}
void handleHoleCollision(Ball* ball1, Hole* hole1)
{

    if (ball1->colorvalue!=3){
     ball1->position->x=2000;
     ball1->mass=0;
     ball1->radius=0;
     ball1->velocity->x=0;
     ball1->velocity->y=0;
    }
    else{
        ball1->position->x=200;
        ball1->position->y=550;
        ball1->velocity->x=0;
        ball1->velocity->y=0;
    }


}

//move a ball one timestep foreward
void Ball::move()
{
    float angle = (float)atan2(velocity->y,velocity->x);
    float adjustX = velocity->x-FrictionConstant*cos(angle)*DT;  //original friction attempt
    float adjustY = velocity->y-FrictionConstant*sin(angle)*DT;  // original friction attempt
    float totalVelocity=sqrt(velocity->x*velocity->x+velocity->y*velocity->y);
    if((adjustX<0 && velocity->x>0) || (adjustX>0 && velocity->x<0))
        adjustX=0;
    if((adjustY<0 && velocity->y>0) || (adjustY>0 && velocity->y<0))
        adjustY=0;
    if(totalVelocity>0)
    {
        velocity->x=adjustX;
        velocity->y=adjustY;
    }
    //float totalVelocityAfter=sqrt(velocity->x*velocity->x+velocity->y*velocity->y);
    //if(totalVelocityAfter>totalVelocity)
    //{
        //std::cout << "bad" << std::endl;
        //exit(0);
    //}
       //use if statements to differentiate from negative and positive velocity
    position->x+=(velocity->x*DT);
    position->y+=(velocity->y*DT);


	//TODO: if you want gravity or friction or another movement dimension, put it here
}


//sets up timer.  don't change this.
TimerHandler::TimerHandler(int t)
{
	QTimer* timer=new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	timer->start(t);
}

//called automatically 66 times per second (you can adjust this by changing TIMESTEP at the top of the program)
void TimerHandler::onTimer()
{
	int i,j;
	//move each ball
	for(i=0; i<BallCount; i++)
    {
		balls[i]->move();
}
	//detect collision between balls
	for(i=0; i<BallCount; i++)
	{
		for(j=i+1; j<BallCount; j++)
		{
			//TODO:  check if a collision happened between balls i and j
            //if (abs(balls[i]->position->sub(balls[j]->position)->lengthsquared())<=(balls[j]->radius+balls[i]->radius)*(balls[j]->radius+balls[i]->radius))
         if (abs(balls[i]->position->sub(balls[j]->position)->lengthsquared())<=(balls[j]->radius+balls[i]->radius)*(balls[j]->radius+balls[i]->radius))
            {
                handleCollision(balls[i],balls[j]);
                //if distsquared(balli,ballj)<=radii^2
			//	handleCollision(balls[j],balls[i]);

			//if you used vectors, collect the garbage here with delete
		}
	}
}

    //detect collision between balls and holes
    for(i=0; i<BallCount; i++)
    {
        for(j=0; j<6; j++)
        {
            //TODO:  check if a collision happened between balls i and j
            //if (abs(balls[i]->position->sub(balls[j]->position)->lengthsquared())<=(balls[j]->radius+balls[i]->radius)*(balls[j]->radius+balls[i]->radius))
         if (abs(balls[i]->position->sub(holes[j]->position)->lengthsquared())<=(holes[j]->radius+balls[i]->radius)*(holes[j]->radius+balls[i]->radius))
            {
             std::cout << "Hole ball Collision" << std::endl;
                handleHoleCollision(balls[i],holes[j]);
                //if distsquared(balli,ballj)<=radii^2
            //	handleCollision(balls[j],balls[i]);

            //if you used vectors, collect the garbage here with delete
        }
    }
}

	//detect collision with the boundary walls
	//TODO:  you may want to add restitution to these collisions as well
	for(i=0; i<BallCount; i++)
	{
		//Bottom
        if(balls[i]->position->y + balls[i]->radius >= HEIGHT && balls[i]->position->x<1000)
		{
            balls[i]->position->y = HEIGHT-balls[i]->radius;
			if(balls[i]->velocity->y > 0)
                balls[i]->velocity->y = RESTITUTION*(-balls[i]->velocity->y);
		}
		//Right
        if(balls[i]->position->x + balls[i]->radius >= WIDTH && balls[i]->position->x<1000)
		{
            balls[i]->position->x = WIDTH-balls[i]->radius;
			if(balls[i]->velocity->x > 0)
                balls[i]->velocity->x = RESTITUTION*(-balls[i]->velocity->x);
		}
		//Top
        if(balls[i]->position->y - balls[i]->radius <=0 && balls[i]->position->x<1000)
		{
			balls[i]->position->y = balls[i]->radius;
			if(balls[i]->velocity->y < 0)
                balls[i]->velocity->y =RESTITUTION*(-balls[i]->velocity->y);
		}
		//Left
        if(balls[i]->position->x - balls[i]->radius <=0 && balls[i]->position->x<1000)
		{
			balls[i]->position->x = balls[i]->radius;
			if(balls[i]->velocity->x < 0)
                balls[i]->velocity->x = RESTITUTION*(-balls[i]->velocity->x);
		}
	}

	//redraw the balls on the screen
	for(i=0; i<BallCount; i++)

		balls[i]->setPos(balls[i]->position->x,balls[i]->position->y);
}

//constructor for the window.  you can leave it empty if you want.
BallView::BallView(QGraphicsScene *scene, QWidget* parent):QGraphicsView(scene, parent)
{
}

//this is called when the mouse is pressed.  use it if you want.

void BallView::mousePressEvent(QMouseEvent *event)
{
    clicked=false;
    if(event->button() == Qt::LeftButton)
    {

        float positionX=event->x();
        float positionY=event->y();
        if (balls[15]->position->x<positionX && balls[15]->position->x+(2*balls[15]->radius)>positionX && balls[15]->position->y<positionY && balls[15]->position->y+(2*balls[15]->radius)>positionY)

        {

            std::cout << "You clicked a ball" << std::endl;
            clicked=true;
            //balls[15]->velocity->x=0;
            //balls[15]->velocity->y=-60;

        }

        /*if (balls[15]->position->x>1000){
            balls[15]->position->x=200;
            balls[15]->position->y=550;
            balls[15]->velocity->x=0;
            balls[15]->velocity->y=0;
        }*/
        //balls[BallCount]=new Ball(150,150,3);
        //balls[BallCount]->position->x=event->x()-RADIUS;
        //balls[BallCount]->position->y=event->y()-RADIUS;
        //thescene->addItem(balls[BallCount]);
        //balls[BallCount]->setPos(balls[BallCount]->position->x,balls[BallCount]->position->y);
        //BallCount++;

        //if (balls[15]->position->x<1000){

        //}
    }
}

//this is called when the mouse is released.  use it if you want.
void BallView::mouseReleaseEvent(QMouseEvent *event)
{
	//TODO: on mouse released...
    if (clicked==true){
    float positionX=event->x();
    float positionY=event->y();
    balls[15]->velocity->x=balls[15]->position->x-positionX;
    balls[15]->velocity->y=balls[15]->position->y-positionY;
    }
}

//this is called when the mouse is doubleclicked.
//currently I have it just creating a new ball at that point
//you can remove that and change this to whatever you like
void BallView::mouseDoubleClickEvent(QMouseEvent *event)
{
	//TODO: modify this if you want to...

    /*if(event->button() == Qt::LeftButton)
	{
        balls[BallCount]=new Ball();
		balls[BallCount]->position->x=event->x()-RADIUS;
		balls[BallCount]->position->y=event->y()-RADIUS;
		balls[BallCount]->velocity->x=balls[BallCount]->velocity->y=0;
		thescene->addItem(balls[BallCount]);
		balls[BallCount]->setPos(balls[BallCount]->position->x,balls[BallCount]->position->y);
        BallCount++;*/
	}
//}

//the program starts here
int main(int argc, char **argv)
{
	//this will keep the program running even after main ends
	QApplication app(argc,argv);
	//seed random number generator to current time
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	//make a scene
	thescene=new QGraphicsScene();
    thescene->setSceneRect(0,0,WIDTH,HEIGHT);
    //Rectangle = new Rectangle;
   // thescene->addRect(rectangle);
	//make the balls and add them to the scene
    rectangle[0]=new Rectangle(0,0);
    balls[0] = new Ball(160,150,1);
    balls[1] = new Ball(180,150,2);
    balls[2] = new Ball(200,150,1);
    balls[3] = new Ball(220,150,2);
    balls[4] = new Ball(240,150,1);
    balls[5] = new Ball(170,170,2);
    balls[6] = new Ball(190,170,1);
    balls[7] = new Ball(210,170,2);
    balls[8] = new Ball(230,170,1);
    balls[9] = new Ball(180,190,2);
    balls[10] = new Ball(200,190,1);
    balls[11] = new Ball(220,190,2);
    balls[12] = new Ball(190,210,1);
    balls[13] = new Ball(210,210,2);
    balls[14] = new Ball(200,230,1);
    balls[15] = new Ball(200,550,3);   //que ball
    holes[0] = new Hole(3,3);   //hole top left
    holes[1] = new Hole(3,380);   //hole middle left
    holes[2] = new Hole(3,797);   //hole bottom left
    holes[3] = new Hole(397,3);   //hole top right
    holes[4] = new Hole(397,380);   //hole middle right
    holes[5] = new Hole(397,797);   //hole bottom right
    thescene->addItem(rectangle[0]);
    thescene->addItem(balls[0]);
    thescene->addItem(balls[1]);
    thescene->addItem(balls[2]);
    thescene->addItem(balls[3]);
    thescene->addItem(balls[4]);
    thescene->addItem(balls[5]);
    thescene->addItem(balls[6]);
    thescene->addItem(balls[7]);
    thescene->addItem(balls[8]);
    thescene->addItem(balls[9]);
    thescene->addItem(balls[10]);
    thescene->addItem(balls[11]);
    thescene->addItem(balls[12]);
    thescene->addItem(balls[13]);
    thescene->addItem(balls[14]);
    thescene->addItem(balls[15]);
    thescene->addItem(holes[0]);//holes
    thescene->addItem(holes[1]);
    thescene->addItem(holes[2]);
    thescene->addItem(holes[3]);
    thescene->addItem(holes[4]);
    thescene->addItem(holes[5]);








    //RIGHT CODE
  /*  for(int i=0; i<BallCount; i++)
	{
        //ball[0]=new Ball(xPos, yPos)
        //rectangle = new Rectangle;
		balls[i] = new Ball;
        //thescene->addRect(rectangle);
		thescene->addItem(balls[i]);

	}
*/
	//TODO:
	//if you want different balls, with different radii or behavior, such as pistons,
	//create them here

	//make the window
	view=new BallView(thescene);
	view->setWindowTitle("Ball");
    view->resize(WIDTH+50,HEIGHT+50);
	view->show();
	view->setMouseTracking(true);

	//start the timer going.  onTimer will now start running periodically
	TimerHandler timer(TIMEUNIT);


	//make everything appear and end main
	return app.exec();
}
