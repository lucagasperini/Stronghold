#ifndef UITABLELIST_H
#define UITABLELIST_H

#include <QApplication>
#include <QButtonGroup>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTreeWidget>
#include <QWidget>
#include <QDockWidget>

#include "wincreate.h"
#include "stronghold_global.h"

class STRONGHOLDSHARED_EXPORT uiTableList : public QWidget
{
    Q_OBJECT

private slots:
public:
    explicit uiTableList(QWidget *parent = 0);
    ~uiTableList();

    QGridLayout *gridLayout;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonDelete;
    QTreeWidget *treeWidget;

    winCreate* create;

    void setupUi();
};

#endif // UITABLELIST_H
