
#include "FormMain.h"

#include <QtGui>
#include <QtSql>
#include "Chart.h"
#include "DialogInput.h"

#ifdef DEBUG
#include <QDebug>
#endif

FormMain::FormMain( QWidget * parent )
	: QMainWindow( parent )
{
#ifdef DEBUG
	connect( this, SIGNAL( yell( const QString & ) ), SLOT( yellDebug( const QString & ) ) );
	emit yell( "is debugging!!!" );
#endif

	setWindowTitle( tr("Folder chart") );

	createWidgets();

	//createToolBar();

	QSqlDatabase sqlite3 = QSqlDatabase::addDatabase("QSQLITE");	// Sqlite3 database

	sqlite3.setDatabaseName( DB_PATH );

	if ( sqlite3.open() ) {
		QSqlQuery q( sqlite3 );

		// speed up sqlite3 queries

		if ( ! q.exec( "PRAGMA synchronous=OFF" ) )
			emit yell( q.lastError().text() );

		if ( ! q.exec( "PRAGMA journal_mode=OFF" ) )
			emit yell( q.lastError().text() );

		if ( ! q.exec( "PRAGMA cache_size=2000000" ) )		// 2 MB
			emit yell( q.lastError().text() );

	} else {
		emit yell( tr("Can't open %1").arg( DB_PATH ) );

	}
}

#ifdef DEBUG
void
FormMain::yellDebug( const QString & message )
{
	qDebug() << "yell:" << message;
#ifdef Q_OS_WIN32
	QMessageBox::critical( this, tr("Critical"), message );
#endif
}
#endif

void
FormMain::createWidgets()
{
	tree = new QTreeWidget( this );
	tree->setHeaderLabel("");

	editInfo = new QTextEdit( this );

	connect( tree, SIGNAL( currentItemChanged( QTreeWidgetItem *, QTreeWidgetItem * ) ),
			SLOT( folderChanged( QTreeWidgetItem *, QTreeWidgetItem * ) ) );

	//listExc = new QListWidget( this );

	chart = new Chart( this );

	QSplitter * splitterHor = new QSplitter( Qt::Horizontal, this );
			  //* splitterVer = new QSplitter( Qt::Vertical, this );

	//splitterVer->addWidget( listExc );
	//splitterVer->addWidget( chart );

	splitterHor->addWidget( tree );
	splitterHor->addWidget( editInfo );
	//splitterHor->addWidget( splitterVer );
	splitterHor->addWidget( chart );

	setCentralWidget( splitterHor );
}

/*
void
FormMain::createToolBar()
{
	QToolBar * toolBar = addToolBar("");

	toolBar->addAction( QIcon(":/blue.png"), tr("set current path"),
			this, SLOT( selectCurrentPath() ) );

	toolBar->addAction( QIcon(":/red.png"), tr("add exclude folder"),
			this, SLOT( selectExcludeFolder() ) );
}
*/

void
FormMain::setCurrentPath( const QString & newPath, const QListWidget & exc )
{
	if ( newPath.isNull() )
		return;

	if ( currentPath == newPath )
		return;

	currentPath = newPath;

	tree->clear();

	dbClear();

	qint64 commonSize = 0;


	qApp->setOverrideCursor( Qt::WaitCursor );

	QTreeWidgetItem * rootItem = processPath( currentPath, commonSize, exc );

	qApp->restoreOverrideCursor();

	if ( rootItem )
		tree->addTopLevelItem( rootItem );

	statusBar()->showMessage( "Finished", 3000 );	// 3 seconds
}

/*
void
FormMain::addExcludeFolder( QString path )
{
	QDir dir( path );

	if ( ! dir.exists() )
		return;

	QListWidgetItem * item = new QListWidgetItem( dir.dirName() );
	item->setIcon( QIcon(":/red.png") );
	item->setData( Qt::ToolTipRole, dir.canonicalPath() );

	listExc->addItem( item );

	// renew tree

	QString cp = currentPath;

	currentPath.clear();

	setCurrentPath( cp );
}
*/

/*
void
FormMain::selectCurrentPath()
{
	QString dir = QFileDialog::getExistingDirectory( this, tr("Select current directory"),
			currentPath );

	if ( ! dir.isEmpty() )
		setCurrentPath( dir );
}
*/

/*
void
FormMain::selectExcludeFolder()
{
	QString dir = QFileDialog::getExistingDirectory( this, tr("Select exclude directory"),
			currentPath );

	if ( ! dir.isEmpty() )
		addExcludeFolder( dir );
}
*/

QTreeWidgetItem *
FormMain::processPath( const QString & path, qint64 & dirSize,
		const QListWidget & exc, int parent_id )
{
	statusBar()->showMessage( path );

	qApp->processEvents();

	QDir dir( path );

	if ( ! dir.exists() )
		return 0;

	const int dbDirId = dbSaveFolder( dir, parent_id );

	if ( dbDirId == -1 )
		return 0;

	QTreeWidgetItem * item = new QTreeWidgetItem();
	item->setIcon( 0, QIcon(":/blue.png" ) );
	item->setText( 0, dir.dirName() );
	item->setData( 0, Qt::UserRole, dbDirId );

	qint64 dir_size = 0;

	// folders
	QFileInfoList list = dir.entryInfoList( QDir::AllDirs | QDir::NoDotAndDotDot , QDir::Name );

	const int dirCount = list.size();

	for ( int i = 0; i < list.size(); ++i ) {

		/*
		if ( exclude( list[ i ] ) )
			continue;
			*/
		bool e = false;

		for ( int j = 0; j < exc.count(); ++j )
			if ( list[ i ].absoluteFilePath() == exc.item( j )->text() ) {
				e = true;
				break;
			}

		if ( e )
			continue;

		item->addChild( processPath( list[ i ].absoluteFilePath(), dir_size, exc, dbDirId ) );
	}

	// files
	list = dir.entryInfoList( QDir::Files | QDir::Hidden | QDir::System, QDir::Type );

#ifdef DEBUG
	qDebug() << "files count: " << dir.dirName() << list.size();
#endif

	for ( int i = 0; i < list.size(); ++ i ) {
		dbSaveFile( list[ i ], dbDirId );

		dir_size += list[ i ].size();
	}

	// percents
	QHash< QString, int > counts;

	for ( int i = 0; i < list.size(); ++i ) {
		const QString suffix = list[ i ].suffix();

		if ( counts.contains( suffix ) )
			counts[ suffix ] += 1;
		else
			counts[ suffix ] = 1;
	}

	dbSavePercents( counts, dbDirId, dirCount + list.size() );

	dbSaveFolderSize( dbDirId, dir_size );

	dirSize += dir_size;

	return item;
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
FormMain::dbSaveFolder( const QDir & dir, int parent_id ) const
{
	QSqlQuery q;

	q.prepare("INSERT INTO folders ("
			"parent_id, "
			"name, "
			"path, "
			"size "
		") VALUES ("
			":pid, "
			":nam, "
			":pat, "
			":siz )");

	q.bindValue(":pid", parent_id );
	q.bindValue(":nam", dir.dirName() );
	q.bindValue(":pat", dir.absolutePath() );
	q.bindValue(":siz", 0 );		// void dbSaveFolderSize( int folder_id, qint64 size ) const

	if ( q.exec() ) {
		q.prepare("SELECT last_insert_rowid()");

		if ( q.exec() &&
				q.first() ) {
			return q.value( 0 ).toInt();

		} else {
			emit yell( q.lastError().text() );
			return -1;
		}

	} else {
		emit yell( q.lastError().text() );
		return -1;
	}
}

int
FormMain::dbSaveFile( const QFileInfo & fileInfo, int folder_id ) const
{
	QSqlQuery q;

	q.prepare("INSERT INTO files ("
			"folders_id, "
			"name, "
			"size "
		") VALUES ("
			":fid, "
			":nam, "
			":siz )");

	q.bindValue(":fid", folder_id );
	q.bindValue(":nam", fileInfo.fileName() );
	q.bindValue(":siz", fileInfo.size() );

	if ( q.exec() ) {
		q.prepare("SELECT last_insert_rowid()");

		if ( q.exec() &&
				q.first() ) {
			return q.value( 0 ).toInt();

		} else {
			emit yell( q.lastError().text() );
			return -1;
		}

	} else {
		emit yell( q.lastError().text() );
		return -1;
	}
}

void
FormMain::dbSaveFolderSize( int folder_id, qint64 size ) const
{
	QSqlQuery q;

	q.prepare("UPDATE folders SET "
			"size = :siz "
		"WHERE "
			"id = :id ");

	q.bindValue(":siz", size );
	q.bindValue(":id", folder_id );

	if ( ! q.exec() )
		emit yell( q.lastError().text() );
}

void
FormMain::dbSavePercents( const QHash< QString, int > & counts, int folder_id, int count ) const
{
	QSqlQuery q;

	QHash< QString, int >::const_iterator ci = counts.constBegin();

	while ( ci != counts.constEnd() ) {

		q.prepare("INSERT INTO percents ("
				"folders_id, "
				"type, "
				"quantity, "
				"percent "
			") VALUES ("
				":fid, "
				":typ, "
				":qua, "
				":per )");

		q.bindValue(":fid", folder_id );
		q.bindValue(":typ", ci.key() );
		q.bindValue(":qua", ci.value() );
		q.bindValue(":per", ci.value() / (qreal) count * 100. );

		if ( ! q.exec() ) {
			emit yell( q.lastError().text() );
			break;
		}

		++ci;
	}
}

void
FormMain::folderChanged( QTreeWidgetItem * current, QTreeWidgetItem * )
{
	editInfo->clear();

	if ( ! current )
		return;

	const int folder_id = current->data( 0, Qt::UserRole ).toInt();

	QString text("Folder: ");

	QSqlQuery q;

	// self
	q.prepare("SELECT "
			"name, "
			"path, "
			"size "
		"FROM "
			"folders "
		"WHERE "
			"id = :id ");

	q.bindValue(":id", folder_id );

	if ( q.exec() && q.first() ) {

		text += q.value( 0 ).toString() + "<BR>" +
			"Path: " + q.value( 1 ).toString() + "<BR>" +
			"Size: " + q.value( 2 ).toString() + "<BR>";

	} else {
		emit yell( q.lastError().text() );
		return;
	}
	// count of folders
	q.prepare("SELECT count( 1 ) "
			"FROM "
				"folders "
			"WHERE "
				"parent_id = :id");

	q.bindValue(":id", folder_id );

	int folderCount = 0;

	if ( q.exec() && q.first() )
		folderCount = q.value( 0 ).toInt();
	else
		emit yell( q.lastError().text() );

	// percent of folders
	q.prepare("SELECT "
			"100 - SUM( percent ) "
		"FROM "
			"percents "
		"WHERE "
			"folders_id = :id ");

	q.bindValue(":id", folder_id );

	if ( q.exec() && q.first() && ( folderCount !=0 ) )
		text += tr("folders: %1 (%2%)<BR>")
			.arg( folderCount )
			.arg( q.value( 0 ).toDouble(), 0, 'f', 1 );

	else
		emit yell( q.lastError().text() );

	// count of files
	q.prepare("SELECT "
			"type, "
			"quantity, "
			"percent "
		"FROM "
			"percents "
		"WHERE "
			"folders_id = :id ");

	q.bindValue(":id", folder_id );

	if ( q.exec() )
		while ( q.next() )
			text += tr("%1: %2 (%3%)<BR>")
				.arg( q.value( 0 ).toString() )
				.arg( q.value( 1 ).toString() )
				.arg( q.value( 2 ).toDouble(), 0, 'f', 1 );
	else
		emit yell( q.lastError().text() );

	text += QString( 50, '-' ) + "<BR>";		// horizontal line -------

	// folders
	text += "<BR><B>folders:</B><BR>";

	q.prepare("SELECT "
			"name, "
			"size "
		"FROM "
			"folders "
		"WHERE "
			"parent_id = :id "
		"ORDER BY "
			"size DESC");

	q.bindValue(":id", folder_id );

	if ( q.exec() ) {
		while ( q.next() )
			text += q.value( 0 ).toString() +
				" (" + q.value( 1 ).toString() + ")<BR>";

	} else {
		emit yell( q.lastError().text() );
		return;
	}

	// files
	text += "<BR><B>files:</B><BR>";

	q.prepare("SELECT "
			"name, "
			"size "
		"FROM "
			"files "
		"WHERE "
			"folders_id = :id "
		"ORDER BY "
			"size DESC");

	q.bindValue(":id", folder_id );

	if ( q.exec() ) {
		while ( q.next() )
			text += q.value( 0 ).toString() +
				" (" + q.value( 1 ).toString() + ")<BR>";

	} else {
		emit yell( q.lastError().text() );
		return;
	}

	// chart
	q.prepare("SELECT "
			"type, "
			"percent "
		"FROM "
			"percents "
		"WHERE "
			"folders_id = :id");

	q.bindValue(":id", folder_id );

	if ( q.exec() ) {

		chart->clear();

		while ( q.next() )
			chart->addPiece( q.value( 1 ).toReal(), q.value( 0 ).toString() );

	} else {
		emit yell( q.lastError().text() );
		return;
	}

	editInfo->setHtml( text );
}

/*
bool
FormMain::exclude( const QFileInfo & fileInfo ) const
{
	const QString dirName = fileInfo.absoluteFilePath();

	for ( int i = 0; i < listExc->count(); ++i )
		if ( listExc->item( i )->data( Qt::ToolTipRole ).toString() == dirName )
			return true;

	return false;
}
*/



