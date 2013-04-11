#ifndef DRAWING_H
#define DRAWING_H

#include <QtGui/QMainWindow>
#include "BaseGraphItem.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPoint;
QT_END_NAMESPACE

enum
	SHAPE{Triangle, Rect, Ellipse};

class Drawing : public QMainWindow
{
	Q_OBJECT

public:
	Drawing(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Drawing();

protected:
	void paintEvent(QPaintEvent *event);

	//�����Ӧ�¼�
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	//������Ӧ�¼�
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);


private slots:
	void selectRect(){ this->m_shape = Rect; m_isDraw = true;};
	void selectTriangle(){this->m_shape = Triangle; m_isDraw = true;};
	void selectEllipse(){this->m_shape = Ellipse; m_isDraw = true;};
	void groupGraphs();
	void separateGraphs();

private:
	QMenu* m_insertMenu;
	QMenu* m_groupMenu;

	QAction* m_insertRect;
	QAction* m_insertTriangle;
	QAction* m_insertEllipse;
	QAction* m_group;
	QAction* m_separate;

	QList<CBaseGraphItem*> m_graphList;
	QList<CBaseGraphItem*> m_backUpList;
	SHAPE m_shape;
	bool m_isDraw;
	bool m_moreChoose;
	QPoint m_startPoint;

	CBaseGraphItem* m_zoomGraph;  //����ʱ���浱ǰͼ�Σ���Ϊ���ŵı�׼
	CGraphItem* m_newGraph; 

	void createActions();
	void createMenus();
	void initGraphs();
	CGraphItem* newGraphItem(const SHAPE &shape);
	int itemAt(const QPoint &pos);
	void moveItemTo(CBaseGraphItem* item, const QPoint &pos);
	Qt::CursorShape getCursorShape(const QPoint &pos);
	//����ͼ��
	void drawGraph(const QPoint &pos);
	//����ͼ��
	void zoomGraph(const Qt::CursorShape &cursorShape, const QPoint &pos);
	void resizeClickGraph(const QPoint &offsetX, const QPoint &offsetY);

	//��תͼ��
	qreal getRoteAngle(const QPoint &pos);

	void setGraphListUnClick();
};

#endif // DRAWING_H
