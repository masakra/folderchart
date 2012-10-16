
#include "FormMain.h"

#include <QtGui>
#include <QtSql>

#ifdef DEBUG
#include <QDebug>
#endif

FormMain::FormMain( QWidget * parent )
	: QMainWindow( parent )
{
#ifdef DEBUG
	connect( this, SIGNAL( yell( const QString & ) ), SLOT( yellDebug( const QString & ) ) );
	emit yell( "is begugging!!!" );
#endif

	createWidgets();

	QSqlDatabase sqlite3 = QSqlDatabase::addDatabase("QSQLITE");	// Sqlite3 database

	sqlite3.setDatabaseName( DB_PATH );

	if ( ! sqlite3.open() )
		emit yell( tr("Can't open %1").arg( DB_PATH ) );
}

#ifdef DEBUG
void
FormMain::yellDebug( const QString & message )
{
	qDebug() << "yell:" << message;
}
#endif

void
FormMain::createWidgets()
{
	tree = new QTreeWidget( this );

	list = new QListWidget( this );

	chart = new QWidget( this ); // TODO replace with Chart


	QSplitter * splitterHor = new QSplitter( Qt::Horizontal, this ),
			  * splitterVer = new QSplitter( Qt::Vertical, this );

	splitterVer->addWidget( list );
	splitterVer->addWidget( chart );

	splitterHor->addWidget( tree );
	splitterHor->addWidget( splitterVer );

	setCentralWidget( splitterHor );
}

void
FormMain::setCurrentPath( QString newPath )
{
	if ( newPath.isNull() )
		newPath = QDir::currentPath();

	if ( currentPath == newPath )
		return;

	currentPath = newPath;

	dbClear();

	processPath( currentPath );
}

void
FormMain::selectCurrentPath()
{
	QString dir = QFileDialog::getExistingDirectory( this, tr("Select directory"),
			currentPath );

	if ( ! dir.isEmpty() )
		setCurrentPath( dir );
}

void
FormMain::processPath( const QString & path )
{
	qDebug() << "processPath" << path;

	QDir dir( path );

	if ( ! dir.exists() )
		return;

	QFileInfoList list = dir.entryInfoList( QDir::AllDirs | QDir::NoDotAndDotDot , QDir::Name );

	for ( int i = 0; i < list.size(); ++i ) {
		qDebug() << i << list[ i ].fileName();

		processPath( list[ i ].absoluteFilePath() );
	}
}

void
FormMain::dbClear() const
{
	QSqlDatabase sqlite3 = QSqlDatabase::database();

	QSqlQuery q;

	QStringList tables = sqlite3.tables();

	for ( int i = 0; i < tables.size(); ++i ) {

		q.prepare( "DELETE FROM " + tables[ i ] );

		if ( ! q.exec() ) {
			emit yell( q.lastError().text() );
			break;
		}


	}

}

int
FormMain::dbSaveFolder( const QString & folder, int parent_id )
{
}
