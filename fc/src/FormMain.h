
#ifndef FORMMAIN_H
#define FORMMAIN_H

#define DB_PATH "data.db"

/** \class MainWindow
 *
 * \brief Main widget
 */

#include <QMainWindow>


class QDir;
class QFileInfo;
class QListWidget;
class QTextEdit;
class QTreeWidget;
class QTreeWidgetItem;
class Chart;

class FormMain : public QMainWindow
{
	Q_OBJECT

	private:
		void createWidgets();

		void createToolBar();

		QTreeWidget * tree;

		QTextEdit * editInfo;

		QListWidget * listExc;

		Chart * chart;

		QString currentPath;

		QTreeWidgetItem * processPath( const QString & path, qint64 & dirSize, int parent_id = -1 );

		void dbClear() const;

		int dbSaveFolder( const QDir & dir, int parent_id ) const;

		int dbSaveFile( const QFileInfo & fileInfo, int folder_id ) const;

		void dbSavePercents( const QHash< QString, int > & counts, int folder_id, int count ) const;

		void dbSaveFolderSize( int folder_id, qint64 size ) const;

		bool exclude( const QFileInfo & fileInfo ) const;

	public:
		FormMain( QWidget * parent = 0 );

	public Q_SLOTS:
		void selectCurrentPath();

		void setCurrentPath( QString newPath = QString() );

		void selectExcludeFolder();

		void addExcludeFolder( QString path );

		void folderChanged( QTreeWidgetItem * current, QTreeWidgetItem * previous );
#ifdef DEBUG
		void yellDebug( const QString & message );
#endif

	Q_SIGNALS:
		void yell( const QString & message ) const;
};

#endif

