#include "BaseGraphItem.h"
//-------------------------------------------------------------------------
CClickRect::CClickRect(void)
{
    m_path.addEllipse(-RADIUS, -RADIUS, DIAMETER, DIAMETER);
}
CClickRect::~CClickRect(void)
{

}
void CClickRect::buildGraph(const QPoint &topLeft, const QPoint &bottomRight)
{
	QPoint offset = bottomRight - topLeft;

	if(m_bottomRight - m_topLeft == offset)
	{
		m_topLeft = topLeft;
		m_bottomRight = bottomRight;
		return;
	}

	m_topLeft = topLeft;
	m_bottomRight = bottomRight;	
	m_pointsPosition[0] = QPoint(0, 0);
	m_pointsPosition[1] = QPoint(offset.x()/2, 0);
	m_pointsPosition[2] = QPoint(offset.x(), 0);
	m_pointsPosition[3] = QPoint(offset.x(), offset.y()/2);
	m_pointsPosition[4] = QPoint(offset.x(), offset.y());
	m_pointsPosition[5] = QPoint(offset.x()/2, offset.y());
	m_pointsPosition[6] = QPoint(0, offset.y());
	m_pointsPosition[7] = QPoint(0, offset.y()/2);
	m_pointsPosition[8] = QPoint(offset.x()/2, -3 * RADIUS);
}
void CClickRect::paintSelf(QPainter &painter)
{
	painter.save();
	painter.setBrush(QBrush(Qt::white));
	painter.setPen(QPen(Qt::black, 1));

	QPoint centerPoin = m_bottomRight - m_topLeft;
 	painter.translate(centerPoin);
 	painter.rotate(m_angle);
 	painter.translate(-centerPoin);

 	painter.drawLine(m_pointsPosition[1], m_pointsPosition[8]);
	for (int i = 0; i < 9; ++i)
	{
		painter.translate(m_pointsPosition[i]);
		painter.drawPath(m_path);
		painter.translate(-m_pointsPosition[i]);
	}
	
	painter.restore();	
}
Qt::CursorShape CClickRect::getCursorShape(const QPoint &pos)
{
	QPointF rr = m_path.currentPosition();
	bool isTrue = m_path.contains(pos - m_pointsPosition[8]);
	if (m_path.contains(pos - m_pointsPosition[0]) 
		|| m_path.contains(pos - m_pointsPosition[4]))
	{
		return Qt::SizeFDiagCursor;
	}
	if (m_path.contains(pos - m_pointsPosition[1]) 
		|| m_path.contains(pos - m_pointsPosition[5]))
	{
		return Qt::SizeVerCursor;
	}
	if (m_path.contains(pos - m_pointsPosition[2]) 
		|| m_path.contains(pos - m_pointsPosition[6]))
	{
		return Qt::SizeBDiagCursor;
	}
	if (m_path.contains(pos - m_pointsPosition[3]) 
		|| m_path.contains(pos - m_pointsPosition[7]))
	{
		return Qt::SizeHorCursor;
	}
	if (m_path.contains(pos - m_pointsPosition[8]))
	{
		return Qt::PointingHandCursor;
	}
	return Qt::ArrowCursor;
}
//-------------------------------------------------------------------------
CBaseGraphItem::CBaseGraphItem(void)
	:m_isClick(false),m_clickRect(NULL)
	, m_topLeft(QPoint(0,0))
	, m_bottomRight(QPoint(0,0))
	, m_angle(0)
{

}
CBaseGraphItem::~CBaseGraphItem(void)
{
	if (m_clickRect)
	{
		delete m_clickRect;
		m_clickRect = NULL;
	}
}
void CBaseGraphItem::paint(QPainter &painter)
{
	paintSelf(painter);

   if (m_isClick == true)
   {
       if (m_clickRect == NULL)
       {
		   m_clickRect = new CClickRect;
       }
	   m_clickRect->buildGraph(m_topLeft, m_bottomRight);
	   m_clickRect->setRoteAngle(m_angle);
	   painter.translate(m_topLeft);
	   m_clickRect->paintSelf(painter);
	   painter.translate(-m_topLeft);
   }  
}
Qt::CursorShape CBaseGraphItem::getCursorShape(const QPoint &pos)
{
	if (isContains(pos))
	{
		return Qt::SizeAllCursor;
	}
	if (m_isClick && m_clickRect)
	{
		return m_clickRect->getCursorShape(pos - m_topLeft);
	}

	return Qt::ArrowCursor;
}
//-------------------------------------------------------------------------
CGraphItems::~CGraphItems(void)
{
	foreach(CBaseGraphItem* item, m_graphList)
	{
		delete item;
	}
}
void CGraphItems::paintSelf(QPainter &painter)
{
	painter.save();
	//旋转
	QPoint centerPoin = m_bottomRight - m_topLeft;
	painter.translate(centerPoin);
	painter.rotate(m_angle);
	painter.translate(-centerPoin);
	//painter.setTransform(m_transform);

	//坐标原点移动，用相对坐标绘制里面的图形
	painter.translate(m_topLeft);

	foreach(CBaseGraphItem* item, m_graphList)
	{
		item->paintSelf(painter);
	}

	painter.translate(-m_topLeft);

	painter.restore();
}
void CGraphItems::addToList(QList<CBaseGraphItem*> &graphList)
{
	foreach(CBaseGraphItem* item, m_graphList)
	{
		graphList.append(item);
		item->moveTo(m_topLeft);
		item->setRoteAngle(item->getRoteAngle() + m_angle);
		//还没实现
		//item->setTransform(item->getTransform() * m_transform);
	}
	m_graphList.clear();
}
bool CGraphItems::isContains(const QPoint &pos)
{
	QPoint offset  = pos - m_topLeft;

	foreach(CBaseGraphItem* item, m_graphList)
	{

		if (item->isContains(offset * m_transform))
		{
			return true;
		}
	}
	
	return false;
}
void CGraphItems::moveTo(const QPoint &offSet)
{
	m_topLeft = m_topLeft + offSet;
	m_bottomRight = m_bottomRight + offSet;

}
CGraphItems* CGraphItems::getClone()
{
	CGraphItems* newItem = new CGraphItems;
	newItem->m_angle = m_angle;
	newItem->m_topLeft = m_topLeft;
	newItem->m_bottomRight = m_bottomRight;
	newItem->m_isClick = m_isClick;

	foreach(CBaseGraphItem* item, m_graphList)
	{
		newItem->m_graphList.append(item->getClone());
	}

	return newItem;
}

void CGraphItems::addGraphItem(CBaseGraphItem* item)
{
	if(item == NULL) return;

	if (m_graphList.isEmpty())
	{
		m_topLeft = item->getTopLeft();
		m_bottomRight = item->getBottomRight();

		item->moveTo(-m_topLeft);
	}
	else
	{
		QPoint backupTopLeft = m_topLeft;
		QPoint backupBottomRight = m_bottomRight;

		//计算两个类组合时的左上角和右下角两个点
		QPoint topleft = item->getTopLeft();
		QPoint bottomright = item->getBottomRight();

		if(m_topLeft.x() > topleft.x()) 
			m_topLeft.setX(topleft.x());

		if(m_topLeft.y() > topleft.y()) 
			m_topLeft.setY(topleft.y());

		if(m_bottomRight.x() < bottomright.x())
			m_bottomRight.setX(bottomright.x());

		if(m_bottomRight.y() < bottomright.y()) 
			m_bottomRight.setY(bottomright.y());

		if(m_topLeft != backupTopLeft)
		{
			QPoint offset  = backupTopLeft - m_topLeft;
			foreach(CBaseGraphItem* item, m_graphList)
			{
				item->moveTo(offset);
			}
		}
		
		QPoint offset = item->getTopLeft() - m_topLeft;
		item->moveTo(-item->getTopLeft() + offset);
	}
	m_graphList.append(item);
}

void CGraphItems::setPen(const QPen &pen)
{
	foreach(CBaseGraphItem* item, m_graphList)
	{
		item->setPen(pen);
	}
}
void CGraphItems::setBrush(const QBrush &brush)
{
	foreach(CBaseGraphItem* item, m_graphList)
	{
		item->setBrush(brush);
	}
}

void CGraphItems::setTopLeft(const QPoint &topLeft)
{
	QPoint offset = m_topLeft - topLeft;
	m_topLeft = topLeft;

	foreach(CBaseGraphItem* item, m_graphList)
	{
		item->setBottomRight(item->getBottomRight() + offset);
	}
}
void CGraphItems::setBottomRight(const QPoint &bottomRight)
{
	QPoint offset = bottomRight - m_bottomRight;
	m_bottomRight = bottomRight;

	foreach(CBaseGraphItem* item, m_graphList)
	{
		item->setBottomRight(item->getBottomRight() + offset);
	}
}


//-------------------------------------------------------------------------
void CGraphItem::paintSelf(QPainter &painter)
{
	painter.save();

	//旋转
	QPoint centerPoint = m_topLeft + (m_bottomRight - m_topLeft)/2;
	painter.translate(centerPoint);
	painter.rotate(m_angle);
	painter.translate(-centerPoint);
//	painter.setTransform(m_transform);

	painter.translate(m_topLeft);
	painter.setPen(m_pen);
	painter.setBrush(m_brush);
	painter.drawPath(m_path);
	painter.translate(-m_topLeft);

	painter.restore();

}
bool CGraphItem::isContains(const QPoint &pos)
{
	return m_path.contains(pos - m_topLeft);
}
void CGraphItem::moveTo(const QPoint &offSet)
{
	m_topLeft = m_topLeft + offSet;
	m_bottomRight = m_bottomRight + offSet;
}
CGraphItem* CGraphItem::getClone()
{
	CGraphItem* item = getInstance();
	item->m_isClick = this->m_isClick;
	item->m_topLeft = this->m_topLeft;
	item->m_bottomRight = this->m_bottomRight;
	item->m_pen = this->m_pen;
	item->m_brush = this->m_brush;
	item->m_path = this->m_path;
	item->m_angle = this->m_angle;
	return item;

}
void CGraphItem::addToList(QList<CBaseGraphItem*> &graphList)
{
	graphList.append(getClone());
}
void CGraphItem::setTopLeft(const QPoint &topLeft)
{
	if (topLeft != m_topLeft)
		buildGraph(topLeft, m_bottomRight);
}
void CGraphItem::setBottomRight(const QPoint &bottomRight)
{
	if(bottomRight != m_bottomRight)
		buildGraph(m_topLeft, bottomRight);
}
//-------------------------------------------------------------------------
void CRect::buildGraph(const QPoint &topLeft, const QPoint &bottomRight)
{
	QPoint offset  = bottomRight - topLeft;

	if (offset != (m_bottomRight - m_topLeft))
	{
		m_path = QPainterPath();
		m_path.addRect(QRect(QPoint(0,0), offset));
	}

	m_topLeft = topLeft;
	m_bottomRight = bottomRight;
}
//-------------------------------------------------------------------------
void CTriangle::buildGraph(const QPoint &topLeft, const QPoint &bottomRight)
{
	QPoint offset  = bottomRight - topLeft;

	if (offset != (m_bottomRight - m_topLeft))
	{
		m_path = QPainterPath();
		m_path.moveTo(offset.x(), offset.y());
		m_path.lineTo(0, offset.y());
		m_path.lineTo(offset.x()/2, 0);
		m_path.lineTo(offset.x(), offset.y());
	}

	m_topLeft = topLeft;
	m_bottomRight = bottomRight;
}
//-------------------------------------------------------------------------
void CEllipse::buildGraph(const QPoint &topLeft, const QPoint &bottomRight)
{
	QPoint offset  = bottomRight - topLeft;

	if (offset != (m_bottomRight - m_topLeft))
	{
		m_path = QPainterPath();
		offset = offset/2;
		this->m_path.addEllipse(offset, abs(offset.x()), abs(offset.y()));
	}

	m_topLeft = topLeft;
	m_bottomRight = bottomRight;
}
//-------------------------------------------------------------------------