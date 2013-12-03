#include "abuleduinputbox.h"

AbulEduInputBox::AbulEduInputBox(QString titre, QString texte,bool doYouWantToClose, QWidget *parent)
    : AbulEduMessageBoxV1(titre, texte,doYouWantToClose,parent)

{
    abeSetModeEnum(AbulEduMessageBoxV1::abeAcceptCancelButton);
    /* Création de la frame à ajouter au Ui d'AbulEduMessageBoxV1 */
    QFrame* frmAbeInput = new QFrame(this);
    QHBoxLayout* frmAbeInputLayout = new QHBoxLayout(frmAbeInput);
    frmAbeInputLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_leInput = new QLineEdit(frmAbeInput);
    m_leInput->setAlignment(Qt::AlignCenter);
    m_leInput->setStyleSheet("border : 1px solid;");
    frmAbeInputLayout->addWidget(m_leInput);
    frmAbeInputLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    QGridLayout* gl = (QGridLayout*) abeMessageBoxGetMainFrame()->layout();
    gl->addWidget(frmAbeInput,1,0);
}

void AbulEduInputBox::on_btnOK_clicked()
{
    emit signalAbeInputBoxOK(m_leInput->text().simplified());
    close();
}
