#include <QApplication>

#include "App.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	App w;  
	w.show(); 

	return a.exec();
}