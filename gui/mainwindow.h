#ifndef MainWindow_H
#define MainWindow_H

#include <stronghold/pem.h>
#include <stronghold/sum.h>
#include "uilogin.h"
#include "wincreate.h"
#include "wingenerate.h"
#include "xsabout.h"
#include "stronghold_global.h"
#include "uitablelist.h"
#include "xsbuttons.h"
#include <plugin/xsplugin.h>

#include <QMainWindow>
#include <QTreeWidget>
#include <QTableWidget>
#include <QItemSelectionModel>
#include <QStandardItemModel>


class STRONGHOLDSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void adminTable();
    void databaseTable(const QString &tablename);
    qlonglong getID(qlonglong index);

    void setupUi();
    void retranslateUi();

    QStandardItemModel* table = nullptr;
    bool bLogin = false;
    bool bChanges = false;
    bool bAdmin = false;
    SUM* sum;
    PEM* pem;

    QAction *actionImport_Table;
    QAction *actionImport_Database;
    QAction *actionExport_Table;
    QAction *actionExport_Database;
    QAction *actionGenerator;
    QAction *actionAbout_XSoftware;
    QAction *actionAbout_QT;
    QAction *actionAdminMode;
    QAction *actionDatabaseMode;
    QAction *actionAdd_Row;
    QAction *actionDelete_Row;
    QAction *actionSave_Changes;
    QAction *actionPassword_Generator;
    QAction *actionSwitch_Mode;
    QAction *actionAdd_Table;
    QAction *actionDelete_Table;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTableView *tableView;
    xsButtons *buttons;
    uiTableList *tables;
    uiLogin *winLogin;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QMenu *menuTools;

signals:

public slots:
    void addFirstUser();
    void login();
    int commit();
    void switchMode();
    void loadTable(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void addRecord();
    void removeRecord();
    void updateRecord(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void importTable();
    void exportTable();
    void exportDatabase();
    void generate();
    void createTable(const QString &tablename, const QStringList &fieldname);
    void deleteTable();
    void addTable();
    void refreshTables();
    void aboutQT();
    void aboutXSoftware();
};

#endif // MainWindow_H
