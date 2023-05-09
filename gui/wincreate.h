#ifndef WINCREATE_H
#define WINCREATE_H

#include <QDialog>
#include "stronghold_global.h"
#include <QStandardItemModel>
#include <stronghold/pem.h>

namespace Ui {
class winCreate;
}

class STRONGHOLDSHARED_EXPORT winCreate : public QDialog
{
    Q_OBJECT
public:
    explicit winCreate(QWidget *parent = 0);
    ~winCreate();

    QString name;

signals:
    void getTableName(const QString &tablename, const QStringList &fieldname);
private slots:
    void on_pushAdd_clicked();

    void on_pushRemove_clicked();

    void on_buttonBox_accepted();

private:
    int index = 0;
    Ui::winCreate *ui;
    QStandardItemModel* list;
};

#endif // WINCREATE_H
