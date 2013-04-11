#include "drawing.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	Drawing w;
	w.show();
	return a.exec();
}
