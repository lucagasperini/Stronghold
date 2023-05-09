#ifndef WINGENERATE_H
#define WINGENERATE_H

#include <QDialog>
#include "stronghold_global.h"

namespace Ui {
class winGenerate;
}

class STRONGHOLDSHARED_EXPORT winGenerate : public QDialog
{
    Q_OBJECT

public:
    explicit winGenerate(QWidget *parent = 0);
    ~winGenerate();

private slots:
    void on_pushGenerate_clicked();

    void on_pushButton_clicked();

private:
    Ui::winGenerate *ui;
};

#endif // WINGENERATE_H
