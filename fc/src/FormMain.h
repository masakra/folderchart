
#ifndef FORMMAIN_H
#define FORMMAIN_H

#define DB_PATH "data.db"

/** \class MainWindow
 *
 * \brief Main widget
 */

#include <QMainWindow>

class QListWidget;
class QTreeWidget;
class QTreeWidgetItem;

class FormMain : public QMainWindow
{
	Q_OBJECT

	private:
		void createWidgets();

		QListWidget * list;

		QTreeWidget * tree;

		QWidget * chart;

		QString currentPath;

		QTreeWidgetItem * processPath( const QString & path,
				int parent_id = -1 );

		void dbClear() const;

		int dbSaveFolder( const QDir & dir, int parent_id );

	public:
		FormMain( QWidget * parent = 0 );

		void selectCurrentPath();

	public Q_SLOTS:
		void setCurrentPath( QString newPath = QString() );
#ifdef DEBUG
		void yellDebug( const QString & message );
#endif

	Q_SIGNALS:
		void yell( const QString & message ) const;
};

#endif

