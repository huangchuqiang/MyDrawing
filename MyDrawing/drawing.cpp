#include <QtGui>
#include "drawing.h"
#include<QtCore/qmath.h>


#define NewGraphItem(item) \
	new C##item; 

#define GetValue(str)\
	new C##str




Drawing::Drawing(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	resize(600, 600);	
	setWindowTitle(tr("MyDrawing Demo"));
	setMouseTracking(true);

	m_isDraw = false;
	m_moreChoose = false;
	m_newGraph = NULL;
	m_zoomGraph = NULL;

	createActions();
	createMenus();
	initGraphs();
}

Drawing::~Drawing()
{
   foreach(CBaseGraphItem* item, m_graphList)
	   delete item;
}

void Drawing::createMenus()
{
	m_insertMenu = menuBar()->addMenu(tr("insert graphics(插入图形)"));
	m_insertMenu->addAction(m_insertRect);
	m_insertMenu->addAction(m_insertEllipse);
	m_insertMenu->addAction(m_insertTriangle);

	m_groupMenu = menuBar()->addMenu(tr("group graphics(组合图形)"));
	m_groupMenu->addAction(m_group);
	m_groupMenu->addAction(m_separate);
}
void Drawing::createActions()
{
	m_insertRect = new QAction(tr("rectangular(矩形)"), this);
	connect(m_insertRect, SIGNAL(triggered()), this, SLOT(selectRect()));

	m_insertEllipse = new QAction(tr("ellipse(椭圆)"), this);
	connect(m_insertEllipse, SIGNAL(triggered()), this, SLOT(selectEllipse()));

	m_insertTriangle = new QAction(tr("trangle(三角形)"), this);
	connect(m_insertTriangle, SIGNAL(triggered()), this, SLOT(selectTriangle()));

	m_group = new QAction(tr("group(组合)"), this);
	connect(m_group, SIGNAL(triggered()), this, SLOT(groupGraphs()));

	m_separate = new QAction(tr("detach(分离)"), this);
	connect(m_separate, SIGNAL(triggered()), this, SLOT(separateGraphs()));
}
CGraphItem* Drawing::newGraphItem(const SHAPE &shape)
{
/*
	CGraphItem* newGraph = NULL;
	switch (shape)
	{
	case SHAPE::Rect:
		newGraph = new CRect;
		break;
	case SHAPE::Triangle:
		newGraph = new CTriangle;
		break;
	case SHAPE::Ellipse:
		newGraph = new CEllipse;
		break;
	default:
		newGraph = new CRect;
	}
	*/
	QString t = "Rect";
	CGraphItem* newGraph = GetValue(Rect);
	newGraph->setPen(QPen(Qt::black, 2));
	newGraph->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
	return newGraph;
}

void Drawing::initGraphs()
{
	CGraphItems* items = new CGraphItems;

	CBaseGraphItem* item = new CRect();
	m_graphList.append(item);
	item->setTopLeft(QPoint(0, 30));
	item->setBottomRight(QPoint(200,200));
	item->setBrush(QBrush(Qt::darkYellow, Qt::DiagCrossPattern));
	item->setPen(QPen(Qt::black, 2));
	item->setClick(true);
	items->addGraphItem(item->getClone());

	item = new CTriangle();
	m_graphList.append(item);
	item->setTopLeft(QPoint(30, 230));
	item->setBottomRight(QPoint(200,400));
	item->setBrush(QBrush(Qt::red, Qt::DiagCrossPattern));
	item->setPen(QPen(Qt::green, 2));
	item->setClick(true);
	items->addGraphItem(item->getClone());

	item = new CEllipse();
	m_graphList.append(item);
	item->setTopLeft(QPoint(230, 30));
	item->setBottomRight(QPoint(400,200));
	item->setBrush(QBrush(Qt::darkYellow, Qt::DiagCrossPattern));
	item->setPen(QPen(Qt::black, 2));
	item->setClick(true);
	items->addGraphItem(item->getClone());
	items->moveTo(QPoint(100,100));
	items->setClick(true);
	m_graphList.append(items);	
}

int Drawing::itemAt(const QPoint &pos)
{
	for (int i = m_graphList.size() - 1; i >= 0; --i)
	{
		CBaseGraphItem* item = m_graphList[i];
		if (item->isContains(pos))
			return i;
	}
	return -1;
}
void Drawing::moveItemTo(CBaseGraphItem* item, const QPoint &pos)
{
	QPoint offset = pos - m_startPoint;
	bool bIsBackUp = m_backUpList.empty();
	
	foreach(CBaseGraphItem* item, m_graphList)
	{
		if (item->getClick())
		{
			if(bIsBackUp)
			{
				CBaseGraphItem* newItem = item->getClone();
				newItem->setClick(false);
				m_backUpList.append(newItem);
			}
			item->moveTo(offset);
		}
	}
	m_startPoint = pos;
	update();
}
void Drawing::setGraphListUnClick()
{
	foreach(CBaseGraphItem* item, m_graphList)
	{
		item->setClick(false);
	}
}

void Drawing::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(Qt::black, 1));
	painter.setBrush(QBrush(Qt::yellow));

	for(QList<CBaseGraphItem*>::iterator iter = m_backUpList.begin()
		; iter != m_backUpList.end(); ++iter)
		(*iter)->paint(painter);

	for(QList<CBaseGraphItem*>::iterator iter = m_graphList.begin()
		; iter != m_graphList.end(); ++iter)
		(*iter)->paint(painter);
}
void Drawing::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		Qt::CursorShape cursorShape = this->cursor().shape();
		m_startPoint = event->pos();
		if (cursorShape == Qt::SizeVerCursor || cursorShape == Qt::SizeHorCursor
			|| cursorShape == Qt::SizeBDiagCursor || cursorShape == Qt::SizeFDiagCursor
			|| cursorShape == Qt::PointingHandCursor)
		{
			//setCursor(Qt::CrossCursor);
		}
		else
		{
			if(m_isDraw == false)
			{
				int index = itemAt(event->pos());
				if (index != -1)
				{
					if (m_moreChoose)
					{
						m_graphList[index]->setClick(!m_graphList[index]->getClick());
					}
					else
					{
						if (m_graphList[index]->getClick() == false)
						{
							setGraphListUnClick();
							m_graphList[index]->setClick(true);
						}
					}
					m_graphList.move(index, m_graphList.size() -1);
				}
				else
				{
					setGraphListUnClick();
				}
			}
			update();
		}
	}
}
void Drawing::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		if (m_isDraw)
		{	
			//画图的代码
			drawGraph(event->pos());
		}
		else
		{
			Qt::CursorShape cursorShape = this->cursor().shape();
			if (cursorShape == Qt::SizeVerCursor || cursorShape == Qt::SizeHorCursor
				|| cursorShape == Qt::SizeBDiagCursor || cursorShape == Qt::SizeFDiagCursor)
			{
				//缩放的代码
				zoomGraph(cursorShape, event->pos());
			}
			else
			{
				if (cursorShape == Qt::SizeAllCursor)
				{
					//移动的代码
					int index = itemAt(event->pos());
					if (index != -1)
					{
						moveItemTo(m_graphList[index], event->pos());
					}
				}
				else
				{
					if (cursorShape == Qt::PointingHandCursor)
					{
						//旋转代码

						qreal angle = getRoteAngle(event->pos());

						foreach(CBaseGraphItem* item, m_graphList)
						{
							if(item->getClick())
								item->setRoteAngle(item->getRoteAngle() + angle);
						}

						m_startPoint = event->pos();
					}
				}

			}
		}
	}
	else
		setCursor(getCursorShape(event->pos()));

	update();
}
void Drawing::mouseReleaseEvent(QMouseEvent *event)
{
	if(m_isDraw)
	{ 
		m_isDraw = false;
		m_newGraph = NULL;
	}

	foreach(CBaseGraphItem* item, m_backUpList)
		delete item;

	m_backUpList.clear();
	update();
}
void Drawing::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (QMessageBox::question(NULL
		, tr("删除提示")
		, tr("是否删除该图形")
		, QMessageBox::Yes | QMessageBox::No
		, QMessageBox::Yes) == QMessageBox::Yes)
	{
		for(QList<CBaseGraphItem*>::iterator iter = m_graphList.begin()
			; iter != m_graphList.end();)
		{
			if ((*iter)->getClick())
			{
				delete (*iter);
				iter = m_graphList.erase(iter);
			}
			else
				++iter;
		}
		update();
	}
}
void Drawing::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Control)
	{
		m_moreChoose = true;
	}
}
void Drawing::keyReleaseEvent(QKeyEvent *event)
{
	m_moreChoose = false;
}
void Drawing::groupGraphs()
{
	CGraphItems* graphItems = new CGraphItems;
	int graphCount =0;

	for(QList<CBaseGraphItem*>::iterator iter = m_graphList.begin()
		; iter != m_graphList.end();)
	{
		if((*iter)->getClick())
		{
			graphItems->addGraphItem(*iter);
			++graphCount;
			iter = m_graphList.erase(iter);
		}
		else
			++iter;
	}
	if (graphCount == 1)
	{
		graphItems->addToList(m_graphList);
		delete graphItems;

	}
	else
	{
		m_graphList.append(graphItems);
		graphItems->setClick(true);
	}
	update();
}
void Drawing::separateGraphs()
{
	QList<CBaseGraphItem*> addToList; 

	for(QList<CBaseGraphItem*>::iterator iter = m_graphList.begin()
		; iter != m_graphList.end();)
	{
		if ((*iter)->getClick())
		{
			(*iter)->addToList(addToList);
			delete (*iter);
			iter = m_graphList.erase(iter);
		}
		else
			++iter;
	}
	m_graphList.append(addToList);
	update();
}
void Drawing::drawGraph(const QPoint &pos)
{
	if(m_newGraph == NULL)
	{
		m_newGraph = newGraphItem(m_shape);
		m_newGraph->buildGraph(m_startPoint,m_startPoint);
		m_newGraph->setClick(true);
		setGraphListUnClick();
		m_graphList.append(m_newGraph);
	}
	else
	{
		m_newGraph->setBottomRight(pos);
	}
	update();
}
Qt::CursorShape Drawing::getCursorShape(const QPoint &pos)
{
	Qt::CursorShape cursorShape = Qt::ArrowCursor;
	foreach(CBaseGraphItem* item, m_graphList)
	{
		Qt::CursorShape returnShape = item->getCursorShape(pos); 
		if(returnShape == Qt::SizeVerCursor || returnShape == Qt::SizeHorCursor
			|| returnShape == Qt::SizeBDiagCursor || returnShape == Qt::SizeFDiagCursor
			|| returnShape == Qt::PointingHandCursor)
		{
			m_zoomGraph = item;
			return returnShape;
		}
		if (returnShape == Qt::SizeAllCursor)
			cursorShape = Qt::SizeAllCursor;
	}
	return cursorShape;
}
void Drawing::zoomGraph(const Qt::CursorShape &cursorShape, const QPoint &pos)
{
	QPoint topleft = m_zoomGraph->getTopLeft();
	QPoint bottomright = m_zoomGraph->getBottomRight();
	QPoint offset = pos - m_startPoint;
	switch(cursorShape)
	{
	case Qt::SizeVerCursor: //vertically
			if((m_startPoint.y() > topleft.y() - RADIUS) 
				&& (m_startPoint.y() < topleft.y() + RADIUS))
			{
				resizeClickGraph(QPoint(0, offset.y()), QPoint(0, 0));
			}
			else
			{
				resizeClickGraph(QPoint(0, 0), QPoint(0, offset.y()));
			}
			break;
	case  Qt::SizeHorCursor://horizontally
			if((m_startPoint.x() > topleft.x() - RADIUS) 
				&& (m_startPoint.x() < topleft.x() + RADIUS))
			{
				resizeClickGraph(QPoint(offset.x(), 0), QPoint(0, 0));
			}
			else
			{
				resizeClickGraph(QPoint(0, 0), QPoint(offset.x(), 0));
			}
			break;
	case Qt::SizeBDiagCursor://top right bottom left
			if((m_startPoint.x() > bottomright.x() - RADIUS) 
				&& (m_startPoint.x() < bottomright.x() + RADIUS)
				&&((m_startPoint.y() > topleft.y() - RADIUS)
				&&(m_startPoint.y() < topleft.y() + RADIUS)))
			{
				resizeClickGraph(QPoint(0, offset.y()), QPoint(offset.x(), 0));
			}
			else
			{
				resizeClickGraph(QPoint(offset.x(), 0), QPoint(0, offset.y()));
			}
			break;
	case Qt::SizeFDiagCursor://top left bottom right
			if((m_startPoint.x() > topleft.x() - RADIUS) 
				&& (m_startPoint.x() < topleft.x() + RADIUS)
				&&((m_startPoint.y() > topleft.y() - RADIUS)
				&&(m_startPoint.y() < topleft.y() + RADIUS)))
			{
				resizeClickGraph(offset, QPoint(0, 0));
			}
			else
			{
				resizeClickGraph(QPoint(0, 0), offset);
			}
			break;
	}
	m_startPoint = pos;
	update();
}
void Drawing::resizeClickGraph(const QPoint &offsetX, const QPoint &offsetY)
{
	foreach(CBaseGraphItem* item, m_graphList)
	{
		if (item->getClick())
		{
			QPoint topleft = item->getTopLeft();
			QPoint bottomright = item->getBottomRight();
			item->setTopLeft(topleft + offsetX);
			item->setBottomRight(bottomright + offsetY);
		}
	}
}
qreal Drawing::getRoteAngle(const QPoint &pos)
{
	QPoint topLeft = m_zoomGraph->getTopLeft();
	QPoint bottomRight = m_zoomGraph->getBottomRight();
	QPoint centerPoint = topLeft + (bottomRight - topLeft)/2;

	//通过判断一个点与直线的关系，来判断旋转的方向。
	bool direction = false;
	QPoint offset = m_startPoint - centerPoint;

	if(offset.x() == 0)
		return -1;

	float k = offset.y()/offset.x();
	float m = m_startPoint.y() - m_startPoint.x() * k;
	qreal value = pos.x() * k - pos.y() + m;

	if(value == 0)
		return  -1;

	if(value > 0)
		direction = true;
	else
		direction = false;

	//用余弦定理求旋转角的余弦
	double aa = qPow(centerPoint.x() - m_startPoint.x(), 2)
		+ qPow(centerPoint.y() - m_startPoint.y(), 2);
	double bb = qPow(centerPoint.x() - pos.x(), 2) 
		+ qPow(centerPoint.y() - pos.y(), 2);
	double cc = qPow(pos.x() - m_startPoint.x(), 2) 
		+ qPow(pos.y() - m_startPoint.y(), 2);
	double cosValue = (aa + bb - cc)/ (2 * sqrt(aa) * sqrt(bb));
	qreal angle = acos(cosValue);

	angle = angle * 3;
	if (direction == false)
		angle = - angle;

	return angle;
}