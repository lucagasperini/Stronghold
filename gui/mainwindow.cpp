#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        sum = new SUM(LOGINFILE);

        if (!sum->isFirstRun()) {
                winLogin = new uiLogin(false);
                winLogin->comboBox->addItems(sum->getUsers());
                connect(winLogin, SIGNAL(accepted()), this, SLOT(login()));
        } else {
                winLogin = new uiLogin(true);
                connect(winLogin, SIGNAL(accepted()), this, SLOT(addFirstUser()));
        }

        while (!bLogin)
                if (!winLogin->exec())
                        exit(0);

        pem = new PEM(sum->user);
        /*   Plugin* s;
           QPluginLoader pluginLoader("example.so");
                   QObject *plugin = pluginLoader.instance();
                   if (plugin)
                   {
                       s = qobject_cast<Plugin*>(plugin);
                       QMessageBox::warning(nullptr, "plugin", QString::number(s->use()));
                   }
           */
        setupUi();

        if (pem->tableList().empty())
                addTable();
        else
                refreshTables();
        tables->treeWidget->setCurrentItem(tables->treeWidget->topLevelItem(0)); //TODO: set last item used if exist?

}

void MainWindow::addFirstUser()
{
        if (winLogin->lineEdit_2->text() != winLogin->lineEdit->text()) {
                QMessageBox::warning(winLogin, "Password mismatch", "Password mismatch, please retry!");
                return;
        }
        bLogin = sum->addRoot(winLogin->lineEdit_3->text(), winLogin->lineEdit->text(), "~/default.db");
}

void MainWindow::login()
{
        bLogin = sum->login(winLogin->comboBox->currentText(), winLogin->lineEdit->text());
}

int MainWindow::commit()
{
        if (!bChanges)
                return 0;
        if (QMessageBox::warning(nullptr, "Commit?", "Red cells will be save with new value!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save) {
                pem->commit();
                bChanges = false;
                databaseTable(pem->tableActive());
                return OK;
        } else {
                bChanges = false;
                pem->commitClear();
                return 0;
        }
}

void MainWindow::switchMode()
{
        if (bAdmin) {
                buttons->button("buttonCommit")->setDisabled(false);
                buttons->button("buttonAddRecord")->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table-row.svg")));
                buttons->button("buttonDeleteRecord")->setIcon(xsUi::getFromIcons(QSL(":/icons/delete-table-row.svg")));
                buttons->button("buttonSwitch")->setIcon(xsUi::getFromIcons(QSL(":/icons/system-switch-user.svg")));
                databaseTable(pem->tableActive());
        } else {
                buttons->button("buttonCommit")->setDisabled(true);
                buttons->button("buttonAddRecord")->setIcon(xsUi::getFromIcons(QSL(":/icons/list-add-user.svg")));
                buttons->button("buttonDeleteRecord")->setIcon(xsUi::getFromIcons(QSL(":/icons/list-remove-user.svg")));
                buttons->button("buttonSwitch")->setIcon(xsUi::getFromIcons(QSL(":/icons/table.svg")));
                adminTable();
        }
}

void MainWindow::databaseTable(const QString &tablename)
{
        commit();
        pem->tableUse(tablename);
        bAdmin = false;

        table->clear();
        table->setHorizontalHeaderLabels(pem->tableField());

        QList<QStandardItem*> out;

        for (int i = 0; i < pem->db->getRecordCount(); i++) {
                QStringList in = pem->get(i);
                for (int x = 0; x < in.count(); x++)
                        out.append(new QStandardItem(in.at(x)));

                table->appendRow(out);
                out.clear();
        }
}


void MainWindow::loadTable(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
        if (bAdmin)
                adminTable();
        else
                databaseTable(current->text(0));
}

void MainWindow::deleteTable()
{
        if (QMessageBox::warning(nullptr, "Commit?", "Deleting a table, all changes will be commited!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save) {
                QTreeWidgetItem* in = tables->treeWidget->currentItem();
                pem->tableDelete(in->text(0));
                pem->commit();
                delete in;
        }
}

void MainWindow::addTable()
{
        winCreate *create = new winCreate();
        connect(create, SIGNAL(getTableName(QString, QStringList)), this, SLOT(createTable(QString, QStringList)));
        if (create->exec() == QDialog::Accepted)
                refreshTables();
}

void MainWindow::adminTable()
{
        bAdmin = true;

        commit();
        table->clear();

        table->setHorizontalHeaderLabels(sum->getFields());

        QList<QStandardItem*> out;

        for (int i = 0; i < sum->db->getRecordCount(); i++) {
                QStringList in = sum->get(i);
                for (int x = 0; x < in.count(); x++)
                        out.append(new QStandardItem(in.at(x)));

                table->appendRow(out);
                out.clear();
        }
}

qlonglong MainWindow::getID(qlonglong index)
{
        return table->index(index, 0).data().toInt();
}

void MainWindow::addRecord()
{
        bChanges = true;
        QList<QStandardItem*> out;
        out.append(new QStandardItem(QString::number(getID(table->rowCount() - 1) + 1)));
        out.at(0)->setData(QBrush(QColor("#da4453")), Qt::BackgroundColorRole);
        if (bAdmin) {
                sum->add();
                for (int i = 1; i < sum->db->getFieldCount(); i++)
                        out.append(new QStandardItem(""));
        } else {
                pem->add();
                for (int i = 1; i < pem->db->getFieldCount(); i++)
                        out.append(new QStandardItem(""));
        }

        table->appendRow(out);
}

void MainWindow::removeRecord()
{
        qlonglong in = tableView->currentIndex().row();
        bChanges = true;
        if (bAdmin)
                sum->remove(getID(in));
        else
                pem->remove(getID(in));
        table->removeRow(in);
}

void MainWindow::updateRecord(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
        int row = topLeft.row(), column = bottomRight.column();
        bChanges = true;
        table->item(row, column)->setBackground(QBrush(QColor("#da4453")));
        if (bAdmin)
                sum->update(column, getID(row), table->item(row, column)->text());
        else
                pem->update(column, table->item(row, column)->text(), getID(row));
}

void MainWindow::importTable()
{
        QString file;
        file = QFileDialog::getOpenFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
        if (!file.isEmpty())
                pem->importTable(QFileInfo(file).completeBaseName(), file); //TODO: NEXT LINE!!!
        //comboTable->addItem(QFileInfo(file).completeBaseName());
}

void MainWindow::exportTable()
{
        QString file;
        file = QFileDialog::getSaveFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
        if (!file.isEmpty())
                pem->exportTable(file);
}

void MainWindow::exportDatabase()
{
        QString file = QFileDialog::getExistingDirectory(nullptr, "Select directory...", QDir::homePath());
        QDir dir(file);
        if (!file.isEmpty())
                pem->exportDatabase(dir);
}

void MainWindow::generate()
{
        winGenerate* generate = new winGenerate;
        generate->show();
}

void MainWindow::createTable(const QString &tablename, const QStringList &fieldname)
{
        if (QMessageBox::warning(nullptr, "Commit?", "Creating a new table, all changes will be commited!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save) {
                QTreeWidgetItem *buffer = new QTreeWidgetItem;
                buffer->setText(0, tablename);
                pem->tableCreate(tablename, fieldname);
                tables->treeWidget->addTopLevelItem(buffer);
                pem->commit();
        }
}

void MainWindow::refreshTables()
{
        disconnect(tables->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(loadTable(QTreeWidgetItem*, QTreeWidgetItem*)));
        tables->treeWidget->clear();
        QStringList in = pem->tableList();
        for (int i = 0; i < in.count(); i++)
                (new QTreeWidgetItem(tables->treeWidget))->setText(0, in.at(i));
        connect(tables->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(loadTable(QTreeWidgetItem*, QTreeWidgetItem*)));
}

void MainWindow::aboutQT()
{
        QMessageBox::aboutQt(0, "About...");
}

void MainWindow::aboutXSoftware()
{
        xsAbout* about = new xsAbout;
        about->show();
}

void MainWindow::setupUi()
{
        if (objectName().isEmpty())
                setObjectName(QSL("MainWindow"));
        resize(1000, 800);
        setMinimumSize(QSize(400, 400));

        setWindowIcon(QIcon(QSL(":/xs/shield.png")));
        actionImport_Table = new QAction(this);
        actionImport_Table->setObjectName(QSL("actionImport_Table"));
        actionImport_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/document-import.svg")));
        actionImport_Database = new QAction(this);
        actionImport_Database->setObjectName(QSL("actionImport_Database"));
        actionExport_Table = new QAction(this);
        actionExport_Table->setObjectName(QSL("actionExport_Table"));

        actionExport_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/document-export.svg")));
        actionExport_Database = new QAction(this);
        actionExport_Database->setObjectName(QSL("actionExport_Database"));

        actionExport_Database->setIcon(xsUi::getFromIcons(QSL(":/icons/network-server-database.svg")));
        actionGenerator = new QAction(this);
        actionGenerator->setObjectName(QSL("actionGenerator"));
        actionAbout_XSoftware = new QAction(this);
        actionAbout_XSoftware->setObjectName(QSL("actionAbout_XSoftware"));

        actionAbout_XSoftware->setIcon(QIcon(QSL(":/xs/men.png")));
        actionAbout_QT = new QAction(this);
        actionAbout_QT->setObjectName(QSL("actionAbout_QT"));

        actionAbout_QT->setIcon(xsUi::getFromIcons(QSL(":/icons/qtcreator.svg")));
        actionAdminMode = new QAction(this);
        actionAdminMode->setObjectName(QSL("actionAdminMode"));
        actionDatabaseMode = new QAction(this);
        actionDatabaseMode->setObjectName(QSL("actionDatabaseMode"));
        actionAdd_Row = new QAction(this);
        actionAdd_Row->setObjectName(QSL("actionAdd_Row"));

        actionAdd_Row->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table-row.svg")));
        actionDelete_Row = new QAction(this);
        actionDelete_Row->setObjectName(QSL("actionDelete_Row"));

        actionDelete_Row->setIcon(xsUi::getFromIcons(QSL(":/icons/delete-table-row.svg")));
        actionSave_Changes = new QAction(this);
        actionSave_Changes->setObjectName(QSL("actionSave_Changes"));

        actionSave_Changes->setIcon(xsUi::getFromIcons(QSL(":/icons/document-save.svg")));
        actionPassword_Generator = new QAction(this);
        actionPassword_Generator->setObjectName(QSL("actionPassword_Generator"));

        actionPassword_Generator->setIcon(xsUi::getFromIcons(QSL(":/icons/password-generate.svg")));
        actionSwitch_Mode = new QAction(this);
        actionSwitch_Mode->setObjectName(QSL("actionSwitch_Mode"));

        actionSwitch_Mode->setIcon(xsUi::getFromIcons(QSL(":/icons/system-switch-user.svg")));
        actionAdd_Table = new QAction(this);
        actionAdd_Table->setObjectName(QSL("actionAdd_Table"));

        actionAdd_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table.svg")));
        actionDelete_Table = new QAction(this);
        actionDelete_Table->setObjectName(QSL("actionDelete_Table"));

        actionDelete_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/delete_table.svg")));
        centralWidget = new QWidget(this);
        centralWidget->setObjectName(QSL("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QSL("gridLayout"));

        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QSL("tableWidget"));
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        gridLayout->addWidget(tableView, 0, 2, 1, 1);

        table = new QStandardItemModel;
        tableView->setModel(table);

        buttons = new xsButtons(centralWidget);
        buttons->setObjectName(QSL("buttons"));
        buttons->setMinimumSize(QSize(100, 0));
        buttons->buttonAdd(QSL("buttonSwitch"), QSL(":/icons/system-switch-user.svg"));
        buttons->buttonAdd(QSL("buttonAddRecord"), QSL(":/icons/insert-table-row.svg"));
        buttons->buttonAdd(QSL("buttonDeleteRecord"), QSL(":/icons/delete-table-row.svg"));
        buttons->buttonAdd(QSL("buttonGenPassword"), QSL(":/icons/password-generate.svg"));
        buttons->buttonAdd(QSL("buttonCommit"), QSL(":/icons/document-save.svg"));

        gridLayout->addWidget(buttons, 0, 3, 1, 1);

        tables = new uiTableList(centralWidget);
        tables->setObjectName(QSL("tables"));
        tables->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(tables, 0, 0, 1, 1);

        setCentralWidget(centralWidget);
        statusBar = new QStatusBar(this);
        statusBar->setObjectName(QSL("statusBar"));
        setStatusBar(statusBar);
        menuBar = new QMenuBar(this);
        menuBar->setObjectName(QSL("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QSL("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QSL("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QSL("menuEdit"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QSL("menuTools"));
        setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionImport_Table);
        menuFile->addAction(actionExport_Table);
        menuFile->addAction(actionExport_Database);
        menuHelp->addAction(actionAbout_XSoftware);
        menuHelp->addAction(actionAbout_QT);
        menuEdit->addAction(actionAdd_Row);
        menuEdit->addAction(actionDelete_Row);
        menuEdit->addAction(actionSave_Changes);
        menuEdit->addSeparator();
        menuEdit->addAction(actionAdd_Table);
        menuEdit->addAction(actionDelete_Table);
        menuTools->addAction(actionPassword_Generator);
        menuTools->addAction(actionSwitch_Mode);

        connect(actionImport_Table, SIGNAL(triggered()), this, SLOT(importTable()));
        connect(actionExport_Database, SIGNAL(triggered()), this, SLOT(exportDatabase()));
        connect(actionExport_Table, SIGNAL(triggered()), this, SLOT(exportTable()));
        connect(actionAdd_Row, SIGNAL(triggered(bool)), this, SLOT(addRecord()));
        connect(actionDelete_Row, SIGNAL(triggered(bool)), this, SLOT(removeRecord()));
        connect(actionSave_Changes, SIGNAL(triggered(bool)), this, SLOT(commit()));
        connect(actionAdd_Table, SIGNAL(triggered(bool)), this, SLOT(addTable()));
        connect(actionDelete_Table, SIGNAL(triggered(bool)), this, SLOT(deleteTable()));
        connect(actionPassword_Generator, SIGNAL(triggered(bool)), this, SLOT(generate()));
        connect(actionSwitch_Mode, SIGNAL(triggered(bool)), this, SLOT(switchMode()));
        connect(actionAbout_QT, SIGNAL(triggered(bool)), this, SLOT(aboutQT()));
        connect(actionAbout_XSoftware, SIGNAL(triggered(bool)), this, SLOT(aboutXSoftware()));

        connect(buttons->button("buttonGenPassword"), SIGNAL(clicked(bool)), this, SLOT(generate()));
        connect(buttons->button("buttonAddRecord"), SIGNAL(clicked(bool)), this, SLOT(addRecord()));
        connect(buttons->button("buttonDeleteRecord"), SIGNAL(clicked(bool)), this, SLOT(removeRecord()));
        connect(buttons->button("buttonCommit"), SIGNAL(clicked(bool)), this, SLOT(commit()));
        connect(buttons->button("buttonSwitch"), SIGNAL(clicked(bool)), this, SLOT(switchMode()));
        //connect(tables->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(loadTable(QTreeWidgetItem*,QTreeWidgetItem*)));
        connect(tables->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addTable()));
        connect(tables->pushButtonDelete, SIGNAL(clicked(bool)),  this, SLOT(deleteTable()));
        connect(table, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(updateRecord(QModelIndex, QModelIndex)));


        retranslateUi();
}

void MainWindow::retranslateUi()
{
        setWindowTitle(QApplication::translate("MainWindow", "Stronghold", Q_NULLPTR));
        actionImport_Table->setText(QApplication::translate("MainWindow", "&Import Table", Q_NULLPTR));
        actionImport_Database->setText(QApplication::translate("MainWindow", "Import &Database", Q_NULLPTR));
        actionExport_Table->setText(QApplication::translate("MainWindow", "&Export Table", Q_NULLPTR));
        actionExport_Database->setText(QApplication::translate("MainWindow", "E&xport Database", Q_NULLPTR));
        actionGenerator->setText(QApplication::translate("MainWindow", "&Generator", Q_NULLPTR));
        actionAbout_XSoftware->setText(QApplication::translate("MainWindow", "&About XSoftware", Q_NULLPTR));
        actionAbout_QT->setText(QApplication::translate("MainWindow", "About &Qt Framework", Q_NULLPTR));
        actionAdminMode->setText(QApplication::translate("MainWindow", "&Switch Admin Mode", Q_NULLPTR));
        actionDatabaseMode->setText(QApplication::translate("MainWindow", "Switch &Database Mode", Q_NULLPTR));
        actionAdd_Row->setText(QApplication::translate("MainWindow", "Add Row", Q_NULLPTR));
        actionDelete_Row->setText(QApplication::translate("MainWindow", "Delete Row", Q_NULLPTR));
        actionSave_Changes->setText(QApplication::translate("MainWindow", "Save Changes", Q_NULLPTR));
        actionPassword_Generator->setText(QApplication::translate("MainWindow", "Password Generator", Q_NULLPTR));
        actionSwitch_Mode->setText(QApplication::translate("MainWindow", "Switch Mode", Q_NULLPTR));
        actionAdd_Table->setText(QApplication::translate("MainWindow", "Add Table", Q_NULLPTR));
        actionDelete_Table->setText(QApplication::translate("MainWindow", "Delete Table", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "Fi&le", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", Q_NULLPTR));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", Q_NULLPTR));
}
