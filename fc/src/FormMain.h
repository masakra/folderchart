
#ifndef FORMMAIN_H
#define FORMMAIN_H

#define DB_PATH "data.db"

/** \class FormMain
 *
 * \brief Main window
 */

/** \def NO_PARENT
 *
 * \brief Value for `parent_id' column in table `folders'
 *
 * This value defines folders without a parent.
 */
#define NO_PARENT -1

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
		 */
		void createToolBar();

		QTreeWidget * tree;

		QTextEdit * editInfo;

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
		 * \return Item of tree
		 */
		QTreeWidgetItem * processPath( const QString & path, qint64 & dirSize,
				const QListWidget & exc, int parent_id = NO_PARENT );

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

		/** \fn void dbRestoreFromDb()
		 *
		 * \brief Restores tree state from sqlite database
		 */
		void dbRestoreFromDb();

		/** \fn void dbGetFolder( int parent_id, QTreeWidgetItem * parent = 0 )
		 *
		 * \brief Restores content of folder by \a parent_id
		 */
		void dbGetFolder( int parent_id, QTreeWidgetItem * parent = 0 );

		QString prettyPrint( qint64 value ) const;

	public:
		FormMain( QWidget * parent = 0 );

	public Q_SLOTS:
		/** \fn void selectFolders()
		 *
		 * \brief Invokes folder and exclusion selection dialog
		 */
		void selectFolders();

		/** \fn void setFolders( const QListWidget & cur, const QListWidget & exc )
		 *
		 * \brief Appends root items into tree
		 */
		void setFolders( const QListWidget & cur, const QListWidget & exc );

		/** \fn void clearAll()
		 *
		 * \brief Clears database and removes all entries from tree
		 */
		void clearAll();

		void folderChanged( QTreeWidgetItem * current, QTreeWidgetItem * previous );

		void treeContextMenu( const QPoint & pos );

		/** \fn void openInFileManager()
		 *
		 * \brief Open file manager with folder selected in tree
		 *
		 * For Windows file manager is explorer.exe otherwise it is xdg-open.
		 *
		 */
		void openInFileManager();

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

