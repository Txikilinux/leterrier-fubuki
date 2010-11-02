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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPushButton>
#include "abuleduexercicev0.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public AbulEduExerciceV0 {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    /**
      niveau :
        facile (4 nombres sont donnés)
        moyen (3)
        difficile (2)
        très difficile (1)
        diabolique (0)
        infernal (0 et le choix des nombres n'est pas fixé)
        RMQ : ça fait peur mais c'est plus facile que le niveau précédent
        (6 équations à 9 inconnues => infinité (théorique) de solutions)
    */
    int niveau;
    int alea;
    /**
      plus grand nombre utilisé (9 par défaut)
    */
    int borneSup;
    /**
      vrai si un nombre a été placé par l'utilisateur
      (utilisé dans le niveau infernal)
    */
    bool placement;

    /**
      fontes utilisées
    */
    QFont fontBIG; // 35
    QFont fontMEDIUM; // 25
    QFont fontMINUS; //10

    QList<int> casesInitial;
    QList<int> cases;
    QList<int> ligSomme;
    QList<int> colSomme;
    QList <QPushButton *> nomBtnCase;
    QList <QPushButton *> nomBtnNbre;

    int actuelBtnNbre;  // indice du btnNbre qui a été sélectionné par un clic
    int actuelBtnCase; // indice du btnCase qui a été sélectionné par un clic

    void initFubuki();
    void printFubuki();
    int indexInCasesInitial(int inCasesDonnees);
    void restaurerBtnNbre(int i);
    void setInformation();

signals:
    /** Demande le positionnement de l'onglet de l'aide
      * @param onglet : le numéro de l'onglet
      */
    void setindex(int onglet);

private slots:
    void on_btnNouveau_clicked();
    void on_btnInformation_clicked();
    void on_btnCorrige_clicked();
    void on_cBoxSuite_activated(int index);
    void on_cBoxNiveau_activated(int index);
    void on_btnVerifier_clicked();
    void on_btnCase0_clicked();
    void on_btnCase1_clicked();
    void on_btnCase2_clicked();
    void on_btnCase3_clicked();
    void on_btnCase4_clicked();
    void on_btnCase5_clicked();
    void on_btnCase6_clicked();
    void on_btnCase7_clicked();
    void on_btnCase8_clicked();
    void _btnCase(int i);

    void on_btnNbre0_clicked();
    void on_btnNbre1_clicked();
    void on_btnNbre2_clicked();
    void on_btnNbre3_clicked();
    void on_btnNbre4_clicked();
    void on_btnNbre5_clicked();
    void on_btnNbre6_clicked();
    void on_btnNbre7_clicked();
    void on_btnNbre8_clicked();
    void _btnNbre(int i);

    /** Slot demandant l'affichage de l'aide sur l'onglet Aide
      */
    void montreAide();
    /** Slot demandant l'affichage de l'aide sur l'onglet Contacts
      */
    void montreContacts();
    /** Slot demandant l'affichage de l'aide sur l'onglet A Propos
      */
    void montreAPropos();
    /** Slot demandant l'affichage de l'aide sur l'onglet Traduire
      */
    void montreTraduire();
};

#endif // MAINWINDOW_H
