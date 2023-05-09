#ifndef XSBUTTONS_H
#define XSBUTTONS_H

#include <QWidget>
#include <QDockWidget>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>

#include "stronghold_global.h"



class STRONGHOLDSHARED_EXPORT xsButtons : public QWidget
{
    Q_OBJECT

public:
    explicit xsButtons(QWidget *parent = 0);
    ~xsButtons();

    QPushButton* buttonAdd(const QString &_name, const QString &_icon, const QSize &_size = QSize(44,44), bool _flat = true);
    QPushButton* button(const QString &_name);
    void buttonDelete(const QString &_name);

private:
    QVBoxLayout *verticalLayout;
    QList<QPushButton*> lButton;

    void setupUi();
};

#endif // XSBUTTONS_H
