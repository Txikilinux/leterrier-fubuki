/**
  * Fubuki (le terrier d'AbulEdu)
  *
  * @warning aucun traitement d'erreur n'est pour l'instant implémenté
  * @see https://redmine.ryxeo.com/projects/
  * @author 2009-2010 Andre Connes <andre dot connes at wanadoo dot fr>
  * @author 2012 Éric Seigne <eric.seigne@ryxeo.com>
  *
  *
  * @see The GNU Public License (GPL)
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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QMessageBox>
#include <QDialog>
#include <QInputDialog>
#include <QFontDatabase>
#include "abuleduaproposv0.h"
#include "abuleduexercicev0.h"

bool isIn(int i, QList<int> s);

MainWindow::MainWindow(QWidget *parent) :
    AbulEduExerciceV0(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fontBIG.setPointSize(30);
    fontMEDIUM.setPointSize(18);
    fontMINUS.setPointSize(10);

    QRegExp nomCasesRegExp("btnCase");
    nomBtnCase = ui->centralWidget->findChildren <QPushButton *> (nomCasesRegExp);
    //for (int i = 0; i < nomBtnCase.length(); i++) { // DEBUG
    //    qDebug() << nomBtnCase[i]->objectName();
    //}

    //Note ES. 2012 02 24: ca ne marche pas sur mon ordinateur, la liste findChildren n'est pas retournée dans l'ordre, de ce fait
    //j'ai btn0 qui porte le nombre 7 et ainsi de suite ... donc je passe par une liste temporaire pour savoir combien de bouton
    //existent et ensuite la boucle permet d'affecter les bon nombres...
    QRegExp nomNombresRegExp("btnNbre");
    QList <QPushButton *> nomBtnNbreTemp = ui->centralWidget->findChildren <QPushButton *> (nomNombresRegExp);
    for(int i = 0; i < nomBtnNbreTemp.size(); i++) {
        QRegExp nomNombresRegExp("btnNbre" + QString::number(i));
        nomBtnNbre << ui->centralWidget->findChildren <QPushButton *> (nomNombresRegExp);
    }

    niveau = 0; // défaut
    alea = 0; // défaut
    borneSup = 9; // défaut
    nErreurs = 0;
    nAides = 0;
    initFubuki();

    actuelBtnNbre = -1; // pas de nombre sélectionné
    actuelBtnCase = -1; // pas de nombre sur cette case sélectionnée

    AbulEduAproposV0 *monAide=new AbulEduAproposV0(this);

    QFontDatabase::addApplicationFont(":/data/sonic_comic.ttf");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::initFubuki()
{
    if (alea == 0)
        setAbeExerciceName(trUtf8("de 1 à 9"));
    else
        setAbeExerciceName(trUtf8("entre 15 et %1").arg(QString::number(borneSup)));
    setAbeLevel(trUtf8("niveau %1").arg(QString::number(niveau)));
    setAbeNbTotalQuestions(1);
    setAbeSkill(trUtf8("anticiper 6 totaux de 3 nombres"));

    // effacer l'affichage
    ui->tedAffiche->clear();
    ui->cBoxSuite->setDisabled(false);
    // pas de nombre placé par l'utilisateur
    placement = false;
    // les nombres à manipuler
    casesInitial.clear();
    if (alea == 0 && niveau < 5)
        casesInitial << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    else {
        if (niveau >= 5) {
            borneSup = 15 + rand() %20;
            ui->cBoxSuite->setDisabled(true);
        }
        casesInitial << borneSup;
        while( casesInitial.length() < 9) {
            int r = rand() % borneSup+1;
            if (! isIn(r, casesInitial)) casesInitial << r;
        }
        qSort(casesInitial.begin(),casesInitial.end() );
    }
    // remplir les boutons à placer (par défaut : tous)
    for (int i = 0; i < 9; i++) {
        nomBtnNbre[i]->setStyleSheet("color :#d40000; font-weight: bold;");
        nomBtnNbre[i]->setFont(fontMEDIUM);
        nomBtnNbre[i]->setDisabled(false);
        nomBtnNbre[i]->setProperty("text", QString::number(casesInitial[i]));
        qDebug() << "On affecte le texte " << casesInitial[i] << " a la case " << i;
    }
    cases = casesInitial;
    // permuter cette liste
    for (int i = 0; i < 9; i++) {
        int j = rand() % 9;
        int a = cases[i];
        cases[i] = cases[j];
        cases[j] = a;
    }
    // sommer les lignes et afficher
    ligSomme.clear();
    ligSomme << cases[0]+cases[1]+cases[2] << cases[3]+cases[4]+cases[5] << cases[6]+cases[7]+cases[8];
    ui->lblH0->setText(QString::number(ligSomme[0]));
    ui->lblH1->setText(QString::number(ligSomme[1]));
    ui->lblH2->setText(QString::number(ligSomme[2]));
    // sommer les colonnes et afficher
    colSomme.clear();
    colSomme << cases[0]+cases[3]+cases[6] << cases[1]+cases[4]+cases[7] << cases[2]+cases[5]+cases[8];
    ui->lblV0->setText(QString::number(colSomme[0]));
    ui->lblV1->setText(QString::number(colSomme[1]));
    ui->lblV2->setText(QString::number(colSomme[2]));

    // vider les cases
    for (int i = 0; i < 9; i++) {
        nomBtnCase[i]->setStyleSheet("color :black");
        nomBtnCase[i]->setFont(fontBIG);
        nomBtnCase[i]->setDisabled(false);
        nomBtnCase[i]->setProperty("text", "");
    }
    // choisir les cases données en fonction du niveau et afficher
    QList<int> casesDonnees;
    if(niveau == 0) {
        int k = rand() % 5;
        switch (k) {
            case 0 : casesDonnees << 0 << 2 << 6 << 8; break;
            case 1 : casesDonnees << 0 << 2 << 4 << 6; break;
            case 2 : casesDonnees << 2 << 4 << 6 << 8; break;
            case 3 : casesDonnees << 0 << 2 << 4 << 8; break;
            case 4 : casesDonnees << 0 << 4 << 6 << 8; break;
        }
    } else if (niveau == 1) {
        int k = rand() % 6;
        switch (k) {
            case 0 : casesDonnees << 0 << 4 << 8; break;
            case 1 : casesDonnees << 0 << 5 << 7; break;
            case 2 : casesDonnees << 1 << 3 << 8; break;
            case 3 : casesDonnees << 1 << 5 << 6; break;
            case 4 : casesDonnees << 2 << 3 << 7; break;
            case 5 : casesDonnees << 2 << 4 << 6; break;
            }
    } else {
        int maxDonnees = (4 - niveau < 0)? 0 : 4 - niveau; // max de 4-niveau et 0
        while (casesDonnees.length() < maxDonnees) {
            int r = rand() % 9;
            if (! isIn(r, casesDonnees)) casesDonnees << r;
        }
        //qDebug() << casesDonnees;
    }
    for (int i = 0; i < casesDonnees.length(); i++) {
        nomBtnCase[casesDonnees[i]]->setStyleSheet("color :#C02020");
        nomBtnCase[casesDonnees[i]]->setDisabled(true);
        nomBtnCase[casesDonnees[i]]->setProperty("text", QString::number(cases[casesDonnees[i]]));
    }

    // qDebug() << "casesInitial cases casesDonnees " << casesInitial << cases << casesDonnees;

    // réduire la taille des nombres déjà placés de btnNombres
    if (niveau <= 4) {
        for (int i = 0; i < 9; i++) {
            if (isIn(i, casesDonnees)) {
                int k = indexInCasesInitial(cases[i]);
                nomBtnNbre[k]->setStyleSheet("color :#C02020");
                nomBtnNbre[k]->setFont(fontMINUS);
                nomBtnNbre[k]->setDisabled(true);
            }
        }
    } else {
        // mais tout effacer si niveau > 4
        for (int i = 0; i < 9; i++) {
            nomBtnNbre[i]->setDisabled(true);
            nomBtnNbre[i]->setProperty("text", "");
        }
    }

    // gerer l'aide
    setInformation();
    nErreurs = 0;
    nAides = 0;
    ui->btnVerifier->setDisabled(false);
    ui->btnAbandonner->setDisabled(false);
    ui->btnNouveau->setDisabled(true);
    ui->btnQuitter->setDisabled(true);

} // fin initFubuki

bool isIn(int i, QList<int> s) {
    for (int j = 0; j < s.length(); j++) {
        if (i == s[j]) return true;
    }
    return false;
}

QString MainWindow::abeEvaluation() {
    if (((niveau == 0 || niveau == 1) && (2*nAides + nErreurs == 0)) ||
        ((niveau == 2 || niveau == 3) && (2*nAides + nErreurs <  3)) ||
        ((niveau == 4 || niveau == 5) && (2*nAides + nErreurs <  5)))
       return "a";
    else if (((niveau == 0 || niveau == 1) && (2*nAides + nErreurs == 2)) ||
            ((niveau == 2 || niveau == 3) && (2*nAides + nErreurs <  5)) ||
            ((niveau == 4 || niveau == 5) && (2*nAides + nErreurs <  7)))
       return "b";
    else if (((niveau == 0 || niveau == 1) && (2*nAides + nErreurs == 3)) ||
            ((niveau == 2 || niveau == 3) && (2*nAides + nErreurs <  6)) ||
            ((niveau == 4 || niveau == 5) && (2*nAides + nErreurs <  8)))
       return "c";
    return "d";
}

int MainWindow::indexInCasesInitial(int inCasesDonnees) {
    for (int i = 0; i < 9; i++) {
        if (casesInitial[i] == inCasesDonnees) return i;
    }
    return -1;
}

void MainWindow::on_btnCase0_clicked() { _btnCase(0); }
void MainWindow::on_btnCase1_clicked() { _btnCase(1); }
void MainWindow::on_btnCase2_clicked() { _btnCase(2); }
void MainWindow::on_btnCase3_clicked() { _btnCase(3); }
void MainWindow::on_btnCase4_clicked() { _btnCase(4); }
void MainWindow::on_btnCase5_clicked() { _btnCase(5); }
void MainWindow::on_btnCase6_clicked() { _btnCase(6); }
void MainWindow::on_btnCase7_clicked() { _btnCase(7); }
void MainWindow::on_btnCase8_clicked() { _btnCase(8); }

void MainWindow::_btnCase(int i) {
    ui->tedAffiche->setText(trUtf8(""));
    if (niveau > 4) {
        if (nomBtnCase[i]->text() != "") {
            //le btnCase n'est pas vide, vider !
            nomBtnCase[i]->setText("");
            return;
        }
        // valuer le btnCase
        bool ok = false;
        while (!ok)
            // astuce (ben oui) puisque actuelBtnNbre désigne normalement un indice pas une valeur
            actuelBtnNbre = QInputDialog::getInteger(this, trUtf8("Ton choix"), trUtf8("Nombre entier entre 1 et 34"), 16, 1, 34, 1, &ok);
    }
    int k = indexInCasesInitial(nomBtnCase[i]->text().toInt());
    if (k >= 0) {
        // bouton déjà occupé : restaurer le btnNbre
        if (niveau <= 4) restaurerBtnNbre(k);
        // effacer le bouton
        nomBtnCase[i]->setText("");
        actuelBtnCase = -1; 
        if (actuelBtnNbre >= 0 && niveau < 5) {
            // déposer actuelBtnNbre sur cette case
            nomBtnCase[i]->setStyleSheet("color : #d40000");
            nomBtnCase[i]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
            actuelBtnNbre = -1;
        }
    } else if (actuelBtnNbre >= 0) {
        // déposer actuelBtnNbre sur cette case
        nomBtnCase[i]->setStyleSheet("color : #d40000");
        if (niveau > 4)
            nomBtnCase[i]->setProperty("text", QString::number(actuelBtnNbre));
        else
            nomBtnCase[i]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
        actuelBtnNbre = -1;
        actuelBtnCase = -1;
    } else {
        // conserver ce bouton pour déposer
        actuelBtnCase = i;
    }
    setInformation();
} //fin _btnCase(int i)

void MainWindow::on_btnNbre0_clicked() { _btnNbre(0); }
void MainWindow::on_btnNbre1_clicked() { _btnNbre(1); }
void MainWindow::on_btnNbre2_clicked() { _btnNbre(2); }
void MainWindow::on_btnNbre3_clicked() { _btnNbre(3); }
void MainWindow::on_btnNbre4_clicked() { _btnNbre(4); }
void MainWindow::on_btnNbre5_clicked() { _btnNbre(5); }
void MainWindow::on_btnNbre6_clicked() { _btnNbre(6); }
void MainWindow::on_btnNbre7_clicked() { _btnNbre(7); }
void MainWindow::on_btnNbre8_clicked() { _btnNbre(8); }

void MainWindow::_btnNbre(int i) {
    qDebug() << "MainWindow::_btnNbre(int i) on a clique sur " << i << casesInitial[i];
    ui->tedAffiche->setText(trUtf8(""));
    if (actuelBtnNbre >= 0 && niveau <= 4) {
        restaurerBtnNbre(actuelBtnNbre);
    }
    nomBtnNbre[i]->setStyleSheet("color :#d40000");
    nomBtnNbre[i]->setFont(fontMINUS);
    nomBtnNbre[i]->setDisabled(true);
    actuelBtnNbre = i;
    if (actuelBtnCase > 0) {
        // déposer le nombe de ce btnNbre sur le actuelBtnCase
        nomBtnCase[actuelBtnCase]->setStyleSheet("color : #d40000");
        nomBtnCase[actuelBtnCase]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
        actuelBtnCase = -1;
        actuelBtnNbre = -1;
    }
    setInformation();
}

void MainWindow::restaurerBtnNbre(int i) {
    nomBtnNbre[i]->setStyleSheet("color :#d40000");
    nomBtnNbre[i]->setFont(fontMEDIUM);
    nomBtnNbre[i]->setDisabled(false);
    setInformation();
}

void MainWindow::on_btnVerifier_clicked()
{   
    // vérifier que la grille est suffisamment remplie sinon return
    QList<int> connus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() != "") connus << i;
    }
    if (connus.length() <= (4 - niveau) || (connus.length() == 0 && niveau > 4)) {
        ui->tedAffiche->setText(trUtf8("Vérification réfusée.\n\nCommence à remplir la grille de gauche !"));
        return;
    }

    // vérifier la globalité de la grille ; si ok, return
    bool ok = true;
    /*
     la vérification est faite en utilisant les cases remplies, pas en comparant avec la solution connue
     en effet, il peut y avoir plusieurs solutions (en particulier niveau infernal)
    */
        ok = ok && nomBtnCase[0]->text().toInt()+nomBtnCase[1]->text().toInt()+nomBtnCase[2]->text().toInt() == ui->lblH0->text().toInt();
        ok = ok && nomBtnCase[3]->text().toInt()+nomBtnCase[4]->text().toInt()+nomBtnCase[5]->text().toInt() == ui->lblH1->text().toInt();
        ok = ok && nomBtnCase[6]->text().toInt()+nomBtnCase[7]->text().toInt()+nomBtnCase[8]->text().toInt() == ui->lblH2->text().toInt();
        ok = ok && nomBtnCase[0]->text().toInt()+nomBtnCase[3]->text().toInt()+nomBtnCase[6]->text().toInt() == ui->lblV0->text().toInt();
        ok = ok && nomBtnCase[1]->text().toInt()+nomBtnCase[4]->text().toInt()+nomBtnCase[7]->text().toInt() == ui->lblV1->text().toInt();
        ok = ok && nomBtnCase[2]->text().toInt()+nomBtnCase[5]->text().toInt()+nomBtnCase[8]->text().toInt() == ui->lblV2->text().toInt();
    if (ok)
    {
        ui->tedAffiche->setText(trUtf8("Bravo, tout est parfait !\n\nTu peux choisir une nouvelle grille..."));
        setAbeLineLog("Complète la grille","", -1, 0 , abeEvaluation(), "", "", "", trUtf8("Vérification"));
        pushAbulEduLogs();
        ui->btnVerifier->setDisabled(true);
        ui->btnAbandonner->setDisabled(true);
        ui->btnNouveau->setDisabled(false);
        ui->btnQuitter->setDisabled(false);
        return;
    }

    // grille incomplète ; vérification en fonction du niveau

    if (niveau < 5)
    {
    // vérification terme à terme si niveau < infernal
        int iFaute = -1;
        for (int i = 0; i < 9; i++) {
            if (nomBtnCase[i]->text().toInt() != cases[i] && nomBtnCase[i]->text() != "") {
                iFaute = i;
                break;
            }
        }
        if (iFaute != -1) {
            ui->tedAffiche->setText(trUtf8("Une ou des erreurs ...\nPar exemple le nombre : %1 \n\nJe te prie de corriger !").arg(nomBtnCase[iFaute]->text()));
            nErreurs++;
            setAbeLineLog("Complète la grille","", -1, 0 , "d", "", "", "", trUtf8("Vérification"));
        } else {
            ui->tedAffiche->setText(trUtf8("Pas d'erreur !\n\nComplète la grille..."));
            setAbeLineLog("Complète la grille","", -1, 0 , "a", "", "", "", trUtf8("Vérification"));
        }
    } else {
    // vérification ligne /colonne si pleine
        for (int i = 0; i < 3 ; i++ ) { //ligne i
            int nVides = 0, somme = 0;
            for (int j = 0; j < 3; j++) {
                nVides += (nomBtnCase[3*i+j]->text() == "" ? 1 : 0);
                somme += nomBtnCase[3*i+j]->text().toInt();
            }
            if (nVides == 0 && somme != ligSomme[i]) {
                    ui->tedAffiche->setText(trUtf8("Une ou des erreurs ...\nPar exemple la ligne %1\n\nJe te prie de corriger !").arg(i+1));
                    nErreurs++;
                    setAbeLineLog("Complète la grille","", -1, 0 , "d", "", "", "", trUtf8("Vérification"));
                    return;
            }
        }
        for (int i = 0; i < 3 ; i++ ) { //colonne i
            int nVides = 0, somme = 0;
            for (int j = 0; j < 3; j++) {
                nVides += (nomBtnCase[i+3*j]->text() == "" ? 1 : 0);
                somme += nomBtnCase[i+3*j]->text().toInt();
            }
            if (nVides == 0 && somme != colSomme[i]) {
                    ui->tedAffiche->setText(trUtf8("Une ou des erreurs ...\nPar exemple la colonne %1\n\nJe te prie de corriger !").arg(i+1));
                    setAbeLineLog("Complète la grille","", -1, 0 , "d", "", "", "", trUtf8("Vérification"));
                    return;
            }
        }
        ui->tedAffiche->setText(trUtf8("Pas d'erreur !\n\nComplète la grille..."));
        setAbeLineLog("Complète la grille","", -1, 0 , "a", "", "", "", trUtf8("Vérification"));
    }
} // fin on_btnVerifier_clicked

void MainWindow::on_cBoxNiveau_activated(int index)
{
    niveau = index;

    //Test de changement d'arrière plan
    ui->centralWidget->setStyleSheet("QWidget#centralWidget {\
                                                             background-image: url(:/data/images/background);\
                                                             background-repeat: repeat-no;\
                                                             background-position: top right;\
                                                             font-family: Sonic Comics;\
                                                         }");
//    if(index < 2) {
//    }
//    else {
//        ui->centralWidget->setStyleSheet("QWidget#centralWidget {\
//                                                 background-image: url(:/data/images/background-nuit);\
//                                                 background-repeat: repeat-no;\
//                                                 background-position: top right;\
//                                                 font-family: Sonic Comics;\
//                                         }");
//    }
    initFubuki();
}

void MainWindow::on_cBoxSuite_activated(int index)
{
    alea = index;
    if (index > 0) {
        bool ok = false;
        int n = QInputDialog::getInteger(this, trUtf8("A toi de choisir"), trUtf8("Nombre entier\n  plus grand que 15\n  plus petit que 35"), 25, 16, 34, 1, &ok);
        if (ok)
            borneSup = n;
        else {
            ui->cBoxSuite->setCurrentIndex(0);
            alea = 0;
        }
    }
    initFubuki();
}

void MainWindow::on_btnAbandonner_clicked()
{
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text().toInt() != cases[i]) {
            nomBtnCase[i]->setStyleSheet("color : #d40000");
            nErreurs++;
        } else {
            nomBtnCase[i]->setStyleSheet("color : #20C020");
        }
        nomBtnCase[i]->setProperty("text", QString::number(cases[i]));
        nomBtnCase[i]->setDisabled(true);
    }
    ui->btnVerifier->setDisabled(true);
    ui->btnAbandonner->setDisabled(true);
    ui->btnNouveau->setDisabled(false);
    ui->btnQuitter->setDisabled(false);
    if (nErreurs > 0)
        ui->tedAffiche->setText(trUtf8("Abandon. Voici un corrigé !\n\nTu peux choisir une nouvelle grille..."));
    else
        ui->tedAffiche->setText(trUtf8("Bravo, tout est parfait !\n\nTu peux choisir une nouvelle grille..."));
    setAbeLineLog("Complète la grille","", -1, 0 , abeEvaluation(), "", "", "", trUtf8("Abandon"));
    pushAbulEduLogs();
}

void MainWindow::on_btnInformation_clicked()
{
    QList<int> inconnus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() == "") inconnus << i;       
    }
    // tirer l'un de ces nombres au hasard
    int iBtn = rand() % inconnus.length();
    //qDebug() << "Je prends le " << iBtn << "ieme bouton inconnu : " << inconnus[iBtn];
    //qDebug() << "AIDE : Je propose la btnCase " << inconnus[iBtn] << " de valeur " << cases[inconnus[iBtn]] << cases;
    ui->tedAffiche->setText(trUtf8("Je propose le nombre ... %1").arg(QString::number(cases[inconnus[iBtn]])));
    nomBtnCase[inconnus[iBtn]]->setStyleSheet("color : #C02020");
    //nomBtnCase[inconnus[iBtn]]->setFont(fontBIG);
    nomBtnCase[inconnus[iBtn]]->setProperty("text", QString::number(cases[inconnus[iBtn]]));
    nomBtnCase[inconnus[iBtn]]->setDisabled(true);
    // rechercher la case correspondant au nombre donné
    //qDebug() << "rechercher la case " << cases[inconnus[iBtn]];
    int k = indexInCasesInitial(cases[inconnus[iBtn]]);
    nomBtnNbre[k]->setStyleSheet("color : #C02020");
    nomBtnNbre[k]->setFont(fontMINUS);
    nomBtnNbre[k]->setDisabled(true);

    nAides++;
    setAbeLineLog("Complète la grille","", -1, 0 , "", "", "", "", trUtf8("Information"));

    setInformation();
}

void MainWindow::on_btnNouveau_clicked()
{
    initFubuki();
}

void MainWindow::setInformation() {
    if (niveau >4) {
        ui->btnInformation->setDisabled(true);
        return;
    }
    QList<int> inconnus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() == "") inconnus << i;
    }
    if (inconnus.length() < 5)
        ui->btnInformation->setDisabled(true);
    else
        ui->btnInformation->setDisabled(false);
}
