#include "uilogin.h"
#include <QMessageBox>
#include <QComboBox>
#include <QPushButton>

uiLogin::uiLogin(bool firstRun, QDialog *parent) :
    QDialog(parent)
{
    if(firstRun) setupFirstUi();
    else setupUi();

    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

uiLogin::~uiLogin()
{
    //delete ui;
}

void uiLogin::setupUi()
{
    if (objectName().isEmpty())
        setObjectName(QSL("this"));
    resize(400, 300);
    setWindowIcon(QIcon(QSL(":/xs/shield.png")));
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QSL("gridLayout"));

    label_3 = new QLabel(this);
    label_3->setObjectName(QSL("label_3"));

    gridLayout->addWidget(label_3, 0, 0, 1, 1);

    comboBox = new QComboBox(this);
    comboBox->setObjectName(QSL("comboBox"));

    gridLayout->addWidget(comboBox, 0, 1, 1, 1);

    label_2 = new QLabel(this);
    label_2->setObjectName(QSL("label_2"));

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QSL("lineEdit"));
    lineEdit->setEchoMode(QLineEdit::Password);

    gridLayout->addWidget(lineEdit, 1, 1, 1, 1);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QSL("buttonBox"));
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setText(QSL("Login"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(QSL("Exit"));
    buttonBox->setCenterButtons(true);

    gridLayout->addWidget(buttonBox, 3, 0, 1, 2);
    label_3->setText(QSL("Insert your\nusername"));
    label_2->setText(QSL("Insert your\npassword"));
}

void uiLogin::setupFirstUi()
{
    if (objectName().isEmpty())
        setObjectName(QSL("this"));
    resize(400, 300);
    setWindowIcon(QIcon(QSL(":/xs/shield.png")));
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QSL("gridLayout"));
    label_3 = new QLabel(this);
    label_3->setObjectName(QSL("label_3"));

    gridLayout->addWidget(label_3, 0, 0, 1, 1);

    lineEdit_3 = new QLineEdit(this);
    lineEdit_3->setObjectName(QSL("lineEdit_3"));

    gridLayout->addWidget(lineEdit_3, 0, 1, 1, 1);

    label_2 = new QLabel(this);
    label_2->setObjectName(QSL("label_2"));

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QSL("lineEdit"));
    lineEdit->setEchoMode(QLineEdit::Password);

    gridLayout->addWidget(lineEdit, 1, 1, 1, 1);

    label = new QLabel(this);
    label->setObjectName(QSL("label"));

    gridLayout->addWidget(label, 2, 0, 1, 1);

    lineEdit_2 = new QLineEdit(this);
    lineEdit_2->setObjectName(QSL("lineEdit_2"));
    lineEdit_2->setEchoMode(QLineEdit::Password);

    gridLayout->addWidget(lineEdit_2, 2, 1, 1, 1);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QSL("buttonBox"));
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setText(QSL("Create user"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(QSL("Exit"));
    buttonBox->setCenterButtons(true);

    gridLayout->addWidget(buttonBox, 3, 0, 1, 2);
    label_3->setText(QSL("Insert your\nusername"));
    label_2->setText(QSL("Insert your\npassword"));
    label->setText(QSL("Repeat your\npassword"));
}
