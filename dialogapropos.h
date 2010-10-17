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

#ifndef DIALOGAPROPOS_H
#define DIALOGAPROPOS_H

#include <QDialog>
    /** Classe gérant l'affichage de l'aide pour les logiciels du Terrier
            */
namespace Ui {
    class DialogApropos;
}

class DialogApropos : public QDialog {
    Q_OBJECT
public:
    DialogApropos(QWidget *parent = 0);
    ~DialogApropos();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogApropos *ui;
private slots:
    /** Slot : Affiche l'onglet demandé
      * @param onglet le numéro de l'onglet à afficher
      */
    void changeTab(int onglet);
};

#endif // DIALOGAPROPOS_H
