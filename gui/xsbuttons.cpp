#include "xsbuttons.h"

xsButtons::xsButtons(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

}

xsButtons::~xsButtons()
{
    lButton.clear();
}

QPushButton* xsButtons::buttonAdd(const QString &_name, const QString &_icon, const QSize &_size, bool _flat)
{
    QPushButton *newButton = new QPushButton(this);
    newButton->setObjectName(_name);

    newButton->setIcon(xsUi::getFromIcons(_icon));
    newButton->setIconSize(_size);
    newButton->setFlat(_flat);

    verticalLayout->addWidget(newButton);

    lButton.append(newButton); //TODO: manage same _name in this list
    return newButton;
}

QPushButton* xsButtons::button(const QString &_name)
{
    for(int i = 0; i < lButton.count(); i++)
        if(lButton.at(i)->objectName() == _name)
            return lButton.at(i);

    return nullptr;
}

void xsButtons::buttonDelete(const QString &_name)
{
    delete button(_name);
}

void xsButtons::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QSL("uiActions"));
    this->resize(100, 640);
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QSL("verticalLayout"));
}
