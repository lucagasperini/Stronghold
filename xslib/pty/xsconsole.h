#ifndef XSCONSOLE_H
#define XSCONSOLE_H

#include <QTextStream>
#include <utils/xspassword.h>
#include <xslib_global.h>

#define endl "\n"

class XSLIBSHARED_EXPORT xsConsole
{
public:
    explicit xsConsole();

    void Write(QString text);
    void Read(QString& text);
    void setXY(int x, int y);
    void getXY(int& x, int& y);
    static xsPassword ReadPasswd(bool copyClear = false, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512, int maxhit = 6);
    static QStringList Shell(QString var1, QString var2);
    xsConsole& operator <<(QString text);
    xsConsole& operator >>(QString& text);

private:
    QTextStream* in;
    QTextStream* out;
};

#endif // XSCONSOLE_H
