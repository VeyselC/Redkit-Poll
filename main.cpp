#include "mainwindow.h"
#include <QApplication>
#include"wrightvaluedialog.h"
#include <QLabel>
#include <QtGui>
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QApplication::setGraphicsSystem("raster");
#endif
	QApplication a(argc, argv);
	MainWindow w;
//	a = new QApplication(argc, argv);
//	QPalette pal = a.palette();
//	pal.setColor(QPalette::Window, QColor(150,150,150,255));
//	a.setPalette(pal);

//	QPixmap image("red.png");
//	  QLabel imageLabel;
//	  imageLabel.setPixmap(image);
//	  imageLabel.show();

	w.show();
	return a.exec();
}
