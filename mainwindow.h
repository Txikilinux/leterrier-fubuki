/**
  * Fubuki (le Terrier d'AbulÉdu)
  *
  * @warning aucun traitement d'erreur n'est pour l'instant implémenté
  * @see https://redmine.ryxeo.com/projects/leterrier-fubuki
  * @author 2009-2010 André Connes <andre.connes@wanadoo.fr>
  * @author 2011 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2011-2013 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2012-2013 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2012-2013 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
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
#include <QApplication>
#include "abuleduaproposv0.h"
#include "abuleduexercicev0.h"
#include "abuledumessageboxv1.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public AbulEduExerciceV0 {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    QString getMainWindowLang();
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *);
#ifndef __ABULEDUTABLETTEV1__MODE__
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
#endif

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

    bool m_isFirstFubuki;
    int niveau;
    /**
        plage de 1 à 9, ou choix entre 15 et 35
      */
    int alea;
    /**
      plus grand nombre utilisé (9 par défaut) - voir alea précédent
    */
    int borneSup;
    /**
      nombre d'erreurs
      */
    int nErreurs;
    /**
      nombre de fois que l'aide est demandée
      */
    int nAides;
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
    /** Liste des boutons contenant la grille */
    QList <QPushButton *> nomBtnCase;
    /** Liste des boutons contenant la pioche */
    QList <QPushButton *> nomBtnNbre;

    QList<QPushButton*> caseCopy;
    QList<QPushButton> btnCopy;

    int actuelBtnNbre;  // indice du btnNbre qui a été sélectionné par un clic
    int actuelBtnCase; // indice du btnCase qui a été sélectionné par un clic

    void initFubuki();
    void printFubuki();
    int indexInCasesInitial(int inCasesDonnees);
    void restaurerBtnNbre(int i);
    void setInformation();
    QString abeEvaluation(); // renvoie a,b,c,d ou z en fonction du niveau, #erreurs, etc.

    /** Appelé au clic sur le bouton Début, cette méthode remet les cases dans l'état du début de l'exercice */
    void restoreCases();
    /** Appelé au clic sur le bouton Début, cette méthode remet les nombres dans l'état du début de l'exercice */
    void restoreNbres();

    QTranslator qtTranslator;
    QTranslator myappTranslator;
    QString m_lang;
    QList<QAction*> m_languesDisponibles;

    AbulEduAproposV0 *m_monAide;

    /** Position de la souris pour gerer le deplacement de la fenetre */
    QPoint m_dragPosition;
    bool   m_isWindowMoving;

signals:
    /** Demande le positionnement de l'onglet de l'aide
      * @param onglet : le numéro de l'onglet
      */
    void setindex(int onglet);
    void toto();

private slots:
    void on_abeMenuFeuilleBtnNew_clicked();
    void on_btnInformation_clicked();
    void on_btnAbandonner_clicked();
//    void on_cBoxSuite_activated(int index);
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

    void slotChangeLangue();
    void creeMenuLangue();
    void on_btnNiveaux_clicked();
    void on_btnNiveauAnnuler_clicked();
    void on_btnNiveauFacile_clicked();
    void on_btnNiveauMoyen_clicked();
    void on_btnNiveauDifficile_clicked();
    void on_btnNiveauTresDifficile_clicked();
    void on_btnNiveauDiabolique_clicked();
    void on_btnNiveauInfernal_clicked();
    void on_btnNombres_clicked();
    void on_btnNombresZeroNeuf_clicked();
    void on_btnNombresAuChoix_clicked();
    void on_btnNombresFermer_clicked();
    void on_abeMenuFeuilleBtnQuit_clicked();
//    void on_btnFeuille_clicked();
    void on_pushButton_clicked();
    void on_abeMenuFeuilleBtnHelp_clicked();
//    void on_btnMinimized_clicked();
//    void on_btnFullScreen_clicked();
    void slotMainWindowSetBorneSup(QString nombreLu);
    void slotMainWindowSetInCase(QString nombreLu);
    void on_btnDebut_clicked();
//    void on_btnLanguages_clicked();
//    void on_btnFr_clicked();
//    void on_btnEn_clicked();
//    void on_btnEs_clicked();
//    void on_btnIt_clicked();
//    void on_btnDe_clicked();
//    void on_btnOc_clicked();
//    void on_btnNl_clicked();
//    void on_btnLangueAnnuler_clicked();
};

#endif // MAINWINDOW_H
