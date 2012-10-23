
/** \mainpage
 *
 * ForlderChart
 *
 * Builds tree of selected folder. Shows by folders and files with sizes.
 * Statistics by file types ( extentions ) with histogramm.
 *
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

	fm.show();

	return app.exec();
}

