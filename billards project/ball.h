#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTimer>
#include <QWidget>

class QGraphicsScene;

class BallView : public QGraphicsView
{
	Q_OBJECT
public:
	BallView(QGraphicsScene*, QWidget*x=  NULL);
	int pressX,pressY;
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
};

class Vector
{
public:
	float x,y;
	Vector(float,float);
	Vector* add(Vector*);
	Vector* sub(Vector*);
	Vector* mul(float);
	float dot(Vector*);
    Vector* normalize();
	float length();
	float lengthsquared();
	float angle();	
    //added
    Vector* VelocityVector(float, float);
};
class Rectangle : public QGraphicsItem
{
public:
    Rectangle(int x, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    float mass;
    float radius;
    //Vector* velocity;
    Vector* position;
    bool contains(float,float);

private:
    QColor color;
};
class Hole : public QGraphicsItem
{
public:
    Hole(int x, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
   // float mass;
    float radius;
    //Vector* velocity;
    Vector* position;
    bool contains(float,float);

private:
    QColor color;
};
class Ball : public QGraphicsItem
{
public:
    Ball(int x,int y, int z); //changed
    QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    float mass;
	float radius;
    float colorvalue;
    Vector* velocity;
	Vector* position;
    void move();
	bool contains(float,float);

private:
	QColor color;
};

class TimerHandler : public QWidget
{
Q_OBJECT
public:
	TimerHandler(int);
protected slots:
	void onTimer();
};

void handleCollision(Ball*,Ball*);
void handleHoleCollision(Ball*,Hole*);
