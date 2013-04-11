#pragma once
#include <qt>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QTransform>


const int RADIUS = 5;
const int DIAMETER = 10;
class CClickRect
{
public:
	CClickRect(void);
	~CClickRect(void);
	void buildGraph(const QPoint &topLeft, const QPoint &bottomRight);
	void paintSelf(QPainter &painter);
	//getter
	Qt::CursorShape getCursorShape(const QPoint &pos);
	qreal getRoteAngle() const {return m_angle;}

	//setter
	void setRoteAngle(const qreal &angle) 
		{m_angle = angle;};
	void setTransform(const QTransform &transform)
		{m_transform = transform;}
private:
	QPainterPath m_path;
	QPoint m_pointsPosition[9];
	QPoint m_topLeft;
	QPoint m_bottomRight;
	
	qreal m_angle;
	//还没有使用坐标矩阵实现旋转 
	QTransform m_transform;
};

class CBaseGraphItem
{
public:
	
	virtual ~CBaseGraphItem(void);
	virtual void paintSelf(QPainter &painter) =0;
	virtual void addToList(QList<CBaseGraphItem*> &graphList) =0;
	virtual bool isContains(const QPoint &pos) =0;
	virtual void moveTo(const QPoint &offSet) =0;
	virtual CBaseGraphItem* getClone() = 0;

	//实现自己的绘制及一个选中框
	void paint(QPainter &painter);	 

	//getter
	Qt::CursorShape getCursorShape(const QPoint &pos);
	QPoint getTopLeft() const
		{ return m_topLeft;}
	QPoint getBottomRight() const
		{ return m_bottomRight;}
	bool getClick()const
		{return m_isClick;}
	qreal getRoteAngle() const 
		{return m_angle;}
	QTransform getTransform()const 
		{return m_transform;}

	//setter
	virtual void setPen(const QPen &pen) =0;
	virtual void setBrush(const QBrush &brush) =0;
	virtual void setTopLeft(const QPoint &topLeft) =0;
	virtual void setBottomRight(const QPoint &bottomRight) =0;
	void setClick(const bool &isClick = false) 
		{ m_isClick = isClick;};
	void setRoteAngle(const qreal &angle) 
		{m_angle = angle;};
	void setTransform(const QTransform &transform)
		{m_transform = transform;}

protected:
	CBaseGraphItem(void);

	QPoint m_topLeft;
	QPoint m_bottomRight;
	//一个是否被选中的标志
	bool m_isClick;
	qreal m_angle;
	CClickRect* m_clickRect;
	QTransform m_transform;
};

//可以存放多个图形
class CGraphItems :public CBaseGraphItem
{
public:
	CGraphItems(void){};
	~CGraphItems(void);

	void paintSelf(QPainter &painter);
	void addToList(QList<CBaseGraphItem*> &graphList);
	bool isContains(const QPoint &pos);
	void moveTo(const QPoint &offSet);
	CGraphItems* getClone();

	//专有方法
	void addGraphItem(CBaseGraphItem* item);

	//getter 
	Qt::CursorShape getCursorShape(const QPoint &pos);
	//setter
	void setPen(const QPen &pen);
	void setBrush(const QBrush &brush);
	void setTopLeft(const QPoint &topLeft);
	void setBottomRight(const QPoint &bottomRight);

private:
	QList<CBaseGraphItem*> m_graphList;
};


//单个图形的基类
class CGraphItem : public CBaseGraphItem
{
public:
	CGraphItem(void){};
	~CGraphItem(void){};

	void paintSelf(QPainter &painter);
	bool isContains(const QPoint &pos);
	void moveTo(const QPoint &offSet);
	Qt::CursorShape getCursorShape(const QPoint &pos); 
	CGraphItem* getClone();
	void addToList(QList<CBaseGraphItem*> &graphList);

	//专有方法
	virtual void buildGraph(const QPoint &topLeft, const QPoint &bottomRight) =0;

	//getter
	QPen getPen() const 
		{return m_pen;}
	QBrush getBrush() const 
		{return m_brush;}
	QPainterPath getPath() const 
		{return m_path;}

	//setter
	void setPen(const QPen &pen)
		{m_pen = pen;};
	void setBrush(const QBrush &brush)
		{m_brush = brush;};
	void setTopLeft(const QPoint &topLeft);
	void setBottomRight(const QPoint &bottomRight);

protected:
	virtual CGraphItem* getInstance() =0;

	QPainterPath m_path;
	QPen m_pen;
	QBrush m_brush;
};

class CRect :public CGraphItem
{
public:
	void buildGraph(const QPoint &topLeft, const QPoint &bottomRight);
private:
	CRect* getInstance()
		{return (new CRect);};
};

class CTriangle :public CGraphItem
{
public:
	void buildGraph(const QPoint &topLeft, const QPoint &bottomRight);
private:
	CTriangle* getInstance()
		{return (new CTriangle);};
};

class CEllipse :public CGraphItem
{
public:
	void buildGraph(const QPoint &topLeft, const QPoint &bottomRight);
private:
	CEllipse* getInstance()
		{return (new CEllipse);};
};