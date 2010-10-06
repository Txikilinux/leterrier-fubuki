#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QLocale>
#include <QMessageBox>
#include <QDialog>
#include "dialogapropos.h"

bool isIn(int i, QList<int> s);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fontBIG.setPointSize(25);
    fontMEDIUM.setPointSize(15);
    fontMINUS.setPointSize(10);

    QRegExp nomCasesRegExp("btnCase");
    nomBtnCase = ui->centralWidget->findChildren <QPushButton *> (nomCasesRegExp);
    //for (int i = 0; i < nomBtnCase.length(); i++) { // DEBUG
    //    qDebug() << nomBtnCase[i]->objectName();
    //}
    QRegExp nomNombresRegExp("btnNbre");
    nomBtnNbre = ui->centralWidget->findChildren <QPushButton *> (nomNombresRegExp);
    //for (int i = 0; i < nomBtnNbre.length(); i++) { // DEBUG
    //    qDebug() << nomBtnNbre[i]->objectName();
    //}

    niveau = 0; // défaut
    alea = 0; // défaut

    initFubuki();

    actuelBtnNbre = -1; // pas de nombre sélectionné
    actuelBtnCase = -1; // pas de nombre sur cette case sélectionnée

    // Connexions fenêtre APropos
    connect(ui->actionAideRapide,SIGNAL(triggered()),this,SLOT(montreAide()));  //
    connect(ui->actionContacts,SIGNAL(triggered()),this,SLOT(montreContacts()));//
    connect(ui->actionAPropos,SIGNAL(triggered()),this,SLOT(montreAPropos()));  //
    connect(ui->actionTraduire,SIGNAL(triggered()),this,SLOT(montreTraduire()));  //
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
    // les nombres à manipuler
    casesInitial.clear();
    if (alea == 0)
        casesInitial << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    else {
        while( casesInitial.length() < 9) {
            int r = rand() % 35;
            if (! isIn(r, casesInitial)) casesInitial << r;
        }
    }
    // remplir les boutons à placer (par défaut : tous)
    for (int i = 0; i < 9; i++) {
        nomBtnNbre[i]->setStyleSheet("color :#2020C0");
        nomBtnNbre[i]->setFont(fontMEDIUM);
        nomBtnNbre[i]->setDisabled(false);
        nomBtnNbre[i]->setProperty("text", QString::number(casesInitial[i]));
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
        int maxDonnees = 4 - niveau;
        while (casesDonnees.length() < maxDonnees) {
            int r = rand() % 9;
            if (! isIn(r, casesDonnees)) casesDonnees << r;
        }
        qDebug() << casesDonnees;
    }
    for (int i = 0; i < casesDonnees.length(); i++) {
        nomBtnCase[casesDonnees[i]]->setStyleSheet("color :#C02020");
        nomBtnCase[casesDonnees[i]]->setDisabled(true);
        nomBtnCase[casesDonnees[i]]->setProperty("text", QString::number(cases[casesDonnees[i]]));
    }

    // qDebug() << "casesInitial cases casesDonnees " << casesInitial << cases << casesDonnees;

    // réduire la taille des nombres déjà placés de btnNombres
    for (int i = 0; i < 9; i++) {
        if (isIn(i, casesDonnees)) {
            int k = indexInCasesInitial(cases[i]);
            nomBtnNbre[k]->setStyleSheet("color :#C02020");
            nomBtnNbre[k]->setFont(fontMINUS);
            nomBtnNbre[k]->setDisabled(true);
        }
    }

    // rappeler la règle
    ui->lblRegle->setText(trUtf8("Complète la grille de gauche avec\nles nombres de la grille de droite\nde sorte que les sommes :\n   - horizontales\n    - verticales\nsoient justes."));
    // gerer l'aide
    setAide();
} // fin initFubuki

bool isIn(int i, QList<int> s) {
    for (int j = 0; j < s.length(); j++) {
        if (i == s[j]) return true;
    }
    return false;
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
    int k = indexInCasesInitial(nomBtnCase[i]->text().toInt());
    if (k >= 0) {
        // bouton déjà occupé : restaurer le btnNbre
        restaurerBtnNbre(k);
        // effacer le bouton
        nomBtnCase[i]->setText("");
        actuelBtnCase = -1; 
        if (actuelBtnNbre >= 0) {
            // déposer actuelBtnNbre sur cette case
            nomBtnCase[i]->setStyleSheet("color : #2020C0");
            nomBtnCase[i]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
            actuelBtnNbre = -1;
        }
    } else if (actuelBtnNbre >= 0) {
        // déposer actuelBtnNbre sur cette case
        nomBtnCase[i]->setStyleSheet("color : #2020C0");
        nomBtnCase[i]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
        actuelBtnNbre = -1;
        actuelBtnCase = -1;
    } else {
        // conserver ce bouton pour déposer
        actuelBtnCase = i;
    }
    setAide();
}

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
    if (actuelBtnNbre >= 0) {
        restaurerBtnNbre(actuelBtnNbre);
    }
    nomBtnNbre[i]->setStyleSheet("color :#2020C0");
    nomBtnNbre[i]->setFont(fontMINUS);
    nomBtnNbre[i]->setDisabled(true);
    actuelBtnNbre = i;
    if (actuelBtnCase > 0) {
        // déposer le nombe de ce btnNbre sur le actuelBtnCase
        nomBtnCase[actuelBtnCase]->setStyleSheet("color : #2020C0");
        nomBtnCase[actuelBtnCase]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
        actuelBtnCase = -1;
        actuelBtnNbre = -1;
    }
    setAide();
}

void MainWindow::restaurerBtnNbre(int i) {
    nomBtnNbre[i]->setStyleSheet("color :#2020C0");
    nomBtnNbre[i]->setFont(fontMEDIUM);
    nomBtnNbre[i]->setDisabled(false);
    setAide();
}

void MainWindow::on_btnVerifier_clicked()
{
    QList<int> connus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() != "") connus << i;
    }
    if (connus.length() <= (4 - niveau)) {
        QMessageBox::critical(this, trUtf8("Vérification"), trUtf8("Vérification réfusée.\n\nCommence à remplir la grille de gauche !"));
        return;
    }    
    bool ok = true;
        ok = ok && nomBtnCase[0]->text().toInt()+nomBtnCase[1]->text().toInt()+nomBtnCase[2]->text().toInt() == ui->lblH0->text().toInt();
        ok = ok && nomBtnCase[3]->text().toInt()+nomBtnCase[4]->text().toInt()+nomBtnCase[5]->text().toInt() == ui->lblH1->text().toInt();
        ok = ok && nomBtnCase[6]->text().toInt()+nomBtnCase[7]->text().toInt()+nomBtnCase[8]->text().toInt() == ui->lblH2->text().toInt();
        ok = ok && nomBtnCase[0]->text().toInt()+nomBtnCase[3]->text().toInt()+nomBtnCase[6]->text().toInt() == ui->lblV0->text().toInt();
        ok = ok && nomBtnCase[1]->text().toInt()+nomBtnCase[4]->text().toInt()+nomBtnCase[7]->text().toInt() == ui->lblV1->text().toInt();
        ok = ok && nomBtnCase[2]->text().toInt()+nomBtnCase[5]->text().toInt()+nomBtnCase[8]->text().toInt() == ui->lblV2->text().toInt();
    if (ok)
    {
        QMessageBox::information(this, trUtf8("Vérification"), trUtf8("Bravo, tout est parfait !\n\nNouvelle grille..."));
        initFubuki();
    }
    else
    {
        int iFaute = -1;
        for (int i = 0; i < 9; i++) {
            if (nomBtnCase[i]->text().toInt() != cases[i] && nomBtnCase[i]->text() != "") {
                iFaute = i;
                break;
            }
        }
        if (iFaute != -1) {
            QMessageBox::critical(this, trUtf8("Vérification"), trUtf8("Une ou des erreurs ...\n\nPar exemple le nombre : %1 \n\nJe te prie de corriger !").arg(nomBtnCase[iFaute]->text()));
        } else {
            QMessageBox::information(this, trUtf8("Vérification"), trUtf8("Pas d'erreur !\n\nComplète la grille..."));
        }
    }
}

void MainWindow::on_cBoxNiveau_activated(int index)
{
    niveau = index;
    initFubuki();
}

void MainWindow::on_cBoxSuite_activated(int index)
{
    alea = index;
    initFubuki();
}

void MainWindow::on_btnCorrige_clicked()
{
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text().toInt() != cases[i]) {
            nomBtnCase[i]->setStyleSheet("color : red");
        } else {
            nomBtnCase[i]->setStyleSheet("color : #20C020");
        }
        nomBtnCase[i]->setProperty("text", QString::number(cases[i]));
        nomBtnCase[i]->setDisabled(true);
    }
}

void MainWindow::on_action_propos_triggered()
{
    QMessageBox::information(this, trUtf8("Fubuki : A propos"), trUtf8("AbulEdu-Le terrier\n\nGPL\n\nVersion du :\n4 octobre 2010"));
}

void MainWindow::on_actionAide_en_local_triggered()
{

    QMessageBox::critical(this, trUtf8("Fubuki"), trUtf8("Aide en local non implantée"));
}

void MainWindow::on_btnAide_clicked()
{
    QList<int> inconnus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() == "") inconnus << i;       
    }
    // tirer l'un de ces nombres au hasard
    int iBtn = rand() % inconnus.length();
    //qDebug() << "Je prends le " << iBtn << "ieme bouton inconnu : " << inconnus[iBtn];
    //qDebug() << "AIDE : Je propose la btnCase " << inconnus[iBtn] << " de valeur " << cases[inconnus[iBtn]] << cases;
    QMessageBox::information(this, trUtf8("Aide proposée"), trUtf8("Je propose le nombre ...\n\n   %1").arg(QString::number(cases[inconnus[iBtn]])));
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

    setAide();
}

void MainWindow::on_btnNouveau_clicked()
{
    initFubuki();
}

void MainWindow::setAide() {
    QList<int> inconnus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() == "") inconnus << i;
    }
    if (inconnus.length() < 5)
        ui->btnAide->setDisabled(true);
    else
        ui->btnAide->setDisabled(false);
}

//--------------------------------------------------------------------------------------
//
//------------------------------Gestion fenêtre A Propos--------------------------------
//
//--------------------------------------------------------------------------------------


/**
  *les fonctions montreAide(), montreContacts(), montreTraduire(), montreAPropos()
  *créent la fenêtre d'APropos en mode modal et positionnent le bon onglet
  *en émettant le signal setIndex(int)
  */
void MainWindow::montreAide()                                                   //
{
    QDialog * fenetreAPropos=new DialogApropos(this);
    connect(this, SIGNAL(setindex(int)),fenetreAPropos,SLOT(changeTab(int)));
    emit setindex(0);
    fenetreAPropos->exec();  // exec : pour avoir une fenêtre modale
}

void MainWindow::montreContacts()
{
    QDialog * fenetreAPropos=new DialogApropos(this);
    connect(this, SIGNAL(setindex(int)),fenetreAPropos,SLOT(changeTab(int)));
    emit setindex(1);
    fenetreAPropos->exec();
}

void MainWindow::montreTraduire()
{
    QDialog * fenetreAPropos=new DialogApropos(this);
    connect(this, SIGNAL(setindex(int)),fenetreAPropos,SLOT(changeTab(int)));
    emit setindex(2);
    fenetreAPropos->exec();
}

void MainWindow::montreAPropos()
{
    QDialog * fenetreAPropos=new DialogApropos(this);
    connect(this, SIGNAL(setindex(int)),fenetreAPropos,SLOT(changeTab(int)));
    emit setindex(3);
    fenetreAPropos->exec();
}
