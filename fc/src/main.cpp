
/** \mainpage
 *
 * ForlderChart
 *
 * Builds tree of selected folder. Shows by folders and files with sizes.
 * Statistics by file types ( extentions ) with histogramm.
 *
 * Usage:
 * > fc						// Builds current directory tree
 * > fc /home/user/			// Builds /home/user tree 
 *
 * \date 18.10.2012 (14:05 MSK)
 */
#include <QApplication>

#include "FormMain.h"
#include "DialogInput.h"

int
main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	app.setWindowIcon( QIcon(":/blue.png") );

	FormMain fm;

	DialogInput d;

	if ( d.exec() ) {
		fm.show();
		fm.setCurrentPath( d.path(), d.exc() );

	} else {
		return -1;
	}
	/*

	fm.show();

	if ( argc > 1 )
		fm.setCurrentPath( argv[ 1 ] );
	else
		fm.setCurrentPath();
		*/

	return app.exec();
}

