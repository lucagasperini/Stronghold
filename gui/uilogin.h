#ifndef UILOGIN_H
#define UILOGIN_H

#include <QWidget>
#include "stronghold_global.h"
#include <stronghold/sum.h>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

class STRONGHOLDSHARED_EXPORT uiLogin : public QDialog
{
    Q_OBJECT

public:
    explicit uiLogin(bool firstRun, QDialog *parent = 0);
    ~uiLogin();

    void setupUi();
    void setupFirstUi();

    QGridLayout *gridLayout;
    QComboBox *comboBox;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit_3;
    QLabel *label_3;
};

#endif // UILOGIN_H
