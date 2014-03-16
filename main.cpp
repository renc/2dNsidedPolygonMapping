#include <QtWidgets\QtWidgets>
#include <QtGui\QtGui>
#include "Window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
	MainWindow window;
							  
    window.show();

    return app.exec();
}
