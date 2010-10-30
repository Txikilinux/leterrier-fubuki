  /**
  * Fubuki (le terrier d'AbulEdu)
  *
  * @warning aucun traitement d'erreur n'est pour l'instant implémenté
  * @see https://redmine.ryxeo.com/projects/
  * @author 2009-2010 Andre Connes <andre dot connes at wanadoo dot fr>
  * @see The GNU Public License (GPL)
  */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

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
