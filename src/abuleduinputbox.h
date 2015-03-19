#ifndef ABULEDUINPUTBOX_H
#define ABULEDUINPUTBOX_H

#include<QLineEdit>
#include "abuledumessageboxv1.h"

class AbulEduInputBox : public AbulEduMessageBoxV1
{
public:
    AbulEduInputBox(const QString &titre, const QString &texte, bool doYouWantToClose = true, QWidget *parent = 0);

private:
    QLineEdit* m_leInput;
    void on_btnOK_clicked();

signals:
//    void signalAbeInputBoxOK(QString);
};

#endif // ABULEDUINPUTBOX_H
