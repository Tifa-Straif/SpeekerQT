#include "mvscene.h"
#include "mvform.h"

MvForm::MvForm(QWidget *parent):QMainWindow(parent)
{
	setupUi(this);
        setWindowTitle(trUtf8("DataBase Connection"));
	scene = new MvScene;
	graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	graphicsView->setScene(scene);        
}
