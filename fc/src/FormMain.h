
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

		/** \fn void createToolBar()
		 *
		 * \brief Creates tool bar
		 *
		 * For demonstration only. Without this call slots selectCurrentPath()
		 * and selectExcludeFolder().
		 *
		 * \sa void selectCurrentPath(), void selectExcludeFolder()
		 */
		void createToolBar();

		QTreeWidget * tree;

		QTextEdit * editInfo;

		//QListWidget * listExc;

		Chart * chart;

		/** \fn QTreeWidgetItem * processPath( const QString & path, qint64 & dirSize,
		 * const QListWidget & exc, int parent_id = -1 )
		 *
		 * \brief Creates tree item for folder
		 *
		 * Creates tree item for folder by \a path. \a dirSize is a reference for
		 * parent folder size variable. \a parent_id is a ID in sqlite3 table 'folders'.
		 * Calls recursively!
		 *
		 * \return Tree item
		 */
		QTreeWidgetItem * processPath( const QString & path, qint64 & dirSize,
				const QListWidget & exc, int parent_id = -1 );

		/** \fn void dbClear() const
		 *
		 * \brief Clear sqlite3 database
		 *
		 * Deletes all data from tables 'folders', 'files' and 'percents'.
		 */
		void dbClear() const;

		/** \fn int dbSaveFolder( const QDir & dir, int parent_id ) const
		 *
		 * \brief Make a database record for directory
		 *
		 * Saves all information for \a dir into table 'folders' but size unavailable at the
		 * moment. Size information saves later by void dbSaveFolderSize( int folder_id,
		 * qint64 size ) const.
		 *
		 * \sa void dbSaveFolderSize( int folder_id, qint64 size ) const
		 *
		 * \return Folder ID in table 'folders'
		 */
		int dbSaveFolder( const QDir & dir, int parent_id ) const;

		/** \fn int dbSaveFile( const QFileInfo & fileInfo, int folder_id ) const
		 *
		 * \brief Saves all information for file
		 *
		 * Saves all information for file described by \a fileInfo.
		 *
		 * \return File ID in table 'files'
		 */
		int dbSaveFile( const QFileInfo & fileInfo, int folder_id ) const;

		/** \fn void dbSavePercents( const QHash< QString, int > & counts, int folder_id, int count ) const
		 *
		 * \brief Saves percents for file types
		 *
		 * Saves percents for file types into table 'percents'.
		 */
		void dbSavePercents( const QHash< QString, int > & counts, int folder_id, int count ) const;

		/** \fn void dbSaveFolderSize( int folder_id, qint64 size ) const
		 *
		 * \brief Update record for folder with size information
		 *
		 * Update record for folder \a folder_id with size information.
		 */
		void dbSaveFolderSize( int folder_id, qint64 size ) const;

		void dbRestoreFromDb();

		void dbProcessFolderId( int parent_id, QTreeWidgetItem * parent = 0 );

	public:
		FormMain( QWidget * parent = 0 );

	public Q_SLOTS:
		void selectFolders();

		void setFolders( const QListWidget & cur, const QListWidget & exc );

		void clearAll();

		void folderChanged( QTreeWidgetItem * current, QTreeWidgetItem * previous );
#ifdef DEBUG
		void yellDebug( const QString & message );
#endif

	Q_SIGNALS:
		/** \fn void yell( const QString & message ) const
		 *
		 * \brief Emits errors and other messages
		 */
		void yell( const QString & message ) const;
};

#endif

