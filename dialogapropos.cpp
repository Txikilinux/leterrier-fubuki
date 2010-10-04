#include "dialogapropos.h"
#include "ui_dialogapropos.h"
#include <QDebug>

/**
  *On construit (instancie) une nouvelle fenêtre d'aide,de contacts et d'Apropos
  *Ici, il n'y a rien à faire !
  *Il y a juste un slot : changeTab(int) appelé depuis la fenêtre principale
  *Il permet de positionner la fenêtre sur le bon onglet
  */

DialogApropos::DialogApropos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogApropos)

{
    ui->setupUi(this);
    //QUrl aide= QUrl::fromLocalFile("data/aide_calculette/Aide_Calculette.html");
    //ui->textBrowser->setSource(aide);
}

DialogApropos::~DialogApropos()
{
    delete ui;
}

void DialogApropos::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogApropos::changeTab(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}
