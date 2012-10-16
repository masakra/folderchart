
#include <QApplication>

#include "FormMain.h"

int
main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	FormMain fm;

	fm.show();

	if ( argc > 1 )
		fm.setCurrentPath( argv[ 1 ] );
	else
		fm.setCurrentPath();

	fm.selectCurrentPath();

	app.exec();
}

