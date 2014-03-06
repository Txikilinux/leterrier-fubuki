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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QMessageBox>
#include <QDialog>
#include <QInputDialog>
#include <QDesktopWidget>
#include <QFontDatabase>
#include "abuleduexercicev0.h"

bool isIn(int i, QList<int> s);

MainWindow::MainWindow(QWidget *parent) :
    AbulEduExerciceV0(parent),
    ui(new Ui::MainWindow)
{
    //Langue
    QString locale = QLocale::system().name().section('_', 0, 0);
    ui->setupUi(this);
    ui->frmIcones->raise();

    //Un 1er qtranslator pour prendre les traductions QT Systeme
    //c'est d'ailleur grace a ca qu'on est en RTL
    qtTranslator.load("qt_" + locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);

    //Et un second qtranslator pour les traductions specifiques du logiciel
//    if(! myappTranslator.load("leterrier-fubuki_"+locale, "lang")) {
//        myappTranslator.load("leterrier-fubuki_en","lang");
//    }

    myappTranslator.load("leterrier-fubuki_" + locale, "lang");
    qApp->installTranslator(&myappTranslator);


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
    m_isFirstFubuki = true;
    niveau = 0; // défaut
    alea = 0; // défaut
    borneSup = 9; // défaut
    nErreurs = 0;
    nAides = 0;
    initFubuki();

    actuelBtnNbre = -1; // pas de nombre sélectionné
    actuelBtnCase = -1; // pas de nombre sur cette case sélectionnée

//    m_monAide=new AbulEduAproposV0(this);

    QFontDatabase::addApplicationFont(":/data/sonic_comic.ttf");

#ifdef __ABULEDUTABLETTEV1__MODE__
    /// 15/01/2012 Icham -> mode tablette, pas de tooltips (pas de survol en mode tactile, et puis ça faisait des trucs bizarres parfois)
    /// 15/01/2013 iCHAM -> icones survol = icones normales
    // on cherche tous les enfants, et on leur met une chaine vide en tooltips (= desactivation)
    foreach (QWidget *obj, findChildren<QWidget*>()) {
        obj->setToolTip("");
//        if(dynamic_cast<AbulEduFlatBoutonV1*>(obj)){
//            dynamic_cast<AbulEduFlatBoutonV1*>(obj)->setIconeSurvol(dynamic_cast<AbulEduFlatBoutonV1*>(obj)->getIconeNormale());
//        }
    }
#endif

    /* Positionnement en dur puisque la hauteur de fenêtre "utile" est fixe */
    ui->frmNiveau->move(ui->frmIcones->x()-ui->frmNiveau->width()+8,ui->frmIcones->y()+21);
    ui->frmChoixNombres->move(ui->frmIcones->x()-ui->frmChoixNombres->width()+8,ui->frmIcones->y()+128);
    ui->frmNiveau->setVisible(false);
    ui->frmChoixNombres->setVisible(false);

    /** @todo s'assurer que c'est la bonne façon de faire */
    connect(ui->frmMenuFeuille, SIGNAL(signalAbeMenuFeuilleChangeLanguage(QString)),this,SLOT(slotChangeLangue(QString)),Qt::UniqueConnection);
    connect(ui->pageAbout, SIGNAL(signalAbeAproposBtnCloseClicked()), this, SLOT(slotMainWindowShowMainPage()),Qt::UniqueConnection);

    setWindowFlags(Qt::CustomizeWindowHint);

    /* Centrage fenetre avec gestion multi-ecrans */
    QDesktopWidget *widget = QApplication::desktop();
    /* C'est ici qu'on récupère le uméro d'écran de l'appli ->screenNumber(this) */
    int desktop_width  = widget->screen(widget->screenNumber(this))->width();
    int desktop_height = widget->screen(widget->screenNumber(this))->height();
    this->move((desktop_width-this->width())/2, (desktop_height-this->height())/2);

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
        initFubuki();
        break;
    default:
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    /* Utilité à vérifier depuis les derniers changements */
    if(m_isFirstFubuki)
    {
        initFubuki();
    }
    m_isFirstFubuki = false;
}

void MainWindow::initFubuki()
{
    foreach(AbulEduMessageBoxV1* mbox,ui->pagePrincipale->findChildren<AbulEduMessageBoxV1*>()){
        mbox->close();
    }
    if(ui->frmNiveau->isVisible())
    {
        ui->frmNiveau->setVisible(false);
        ui->btnNiveaux->setStyleSheet(ui->btnNiveaux->styleSheet().replace("border-radius:5px;background-color:#ffffff;","background-color:rgba(0,0,0,0);"));
    }
    if (alea == 0)
        setAbeExerciceName(trUtf8("de 1 à 9"));
    else
        setAbeExerciceName(trUtf8("entre 15 et %1").arg(QString::number(borneSup)));
    setAbeLevel(trUtf8("niveau %1").arg(QString::number(niveau)));
    setAbeNbTotalQuestions(1);
    setAbeSkill(trUtf8("anticiper 6 totaux de 3 nombres"));

//    ui->cBoxSuite->setDisabled(false);
    ui->btnNombres->setDisabled(false);
    // pas de nombre placé par l'utilisateur
    placement = false;
    // les nombres à manipuler
    casesInitial.clear();
    if (alea == 0 && niveau < 4)
    {
        casesInitial << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
        ui->lblIndicationNiveauNoir->setVisible(false);
        ui->frmBtnNombres->setVisible(true);
    }
    else {
        if (niveau >= 4) {
            borneSup = 15 + rand() %20;
//            ui->cBoxSuite->setDisabled(true);
            ui->btnNombres->setDisabled(true);
            ui->frmBtnNombres->setVisible(false);
            ui->lblIndicationNiveauNoir->setVisible(true);

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
        nomBtnNbre[i]->setStyleSheet("background:transparent;color :#d40000; font-weight: bold;");
        nomBtnNbre[i]->setFont(fontMEDIUM);
        nomBtnNbre[i]->setDisabled(false);
        nomBtnNbre[i]->setProperty("text", QString::number(casesInitial[i]));
//        qDebug() << "On affecte le texte " << casesInitial[i] << " a la case " << i;
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
        nomBtnCase[i]->setStyleSheet("background:transparent;color :black");
        nomBtnCase[i]->setFont(fontBIG);
        nomBtnCase[i]->setDisabled(false);
        nomBtnCase[i]->setProperty("text", "");
        nomBtnCase[i]->setProperty("used",false);
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
        nomBtnCase[casesDonnees[i]]->setStyleSheet("background:transparent;color :#C02020");
        nomBtnCase[casesDonnees[i]]->setDisabled(true);
        nomBtnCase[casesDonnees[i]]->setProperty("text", QString::number(cases[casesDonnees[i]]));
        nomBtnCase[casesDonnees[i]]->setProperty("used",true);
    }

    // qDebug() << "casesInitial cases casesDonnees " << casesInitial << cases << casesDonnees;

    // réduire la taille des nombres déjà placés de btnNombres
    if (niveau <= 3) {
        for (int i = 0; i < 9; i++) {
            if (isIn(i, casesDonnees)) {
                int k = indexInCasesInitial(cases[i]);
                nomBtnNbre[k]->setStyleSheet("background:transparent;color :#C02020");
                nomBtnNbre[k]->setFont(fontMINUS);
                nomBtnNbre[k]->setDisabled(true);
                nomBtnNbre[k]->setProperty("used",true);
            }
            else
            {
                int k = indexInCasesInitial(cases[i]);
                nomBtnNbre[k]->setProperty("used",false);
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

//    ui->btnNouveau->setDisabled(true);

    /* Creation d'une copie des cases */


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
        ((niveau == 4) && (2*nAides + nErreurs <  5)))
       return "a";
    else if (((niveau == 0 || niveau == 1) && (2*nAides + nErreurs == 2)) ||
            ((niveau == 2 || niveau == 3) && (2*nAides + nErreurs <  5)) ||
            ((niveau == 4) && (2*nAides + nErreurs <  7)))
       return "b";
    else if (((niveau == 0 || niveau == 1) && (2*nAides + nErreurs == 3)) ||
            ((niveau == 2 || niveau == 3) && (2*nAides + nErreurs <  6)) ||
            ((niveau == 4) && (2*nAides + nErreurs <  8)))
       return "c";
    return "d";
}

void MainWindow::restoreCases()
{
    foreach(QPushButton* btn,nomBtnCase)
    {
        if(!btn->property("used").toBool())
        {
            btn->setStyleSheet("background:transparent;color :black");
            btn->setFont(fontBIG);
            btn->setDisabled(false);
            btn->setFlat(true);
            btn->setProperty("text", "");
            btn->setText("");
            btn->setProperty("used",false);
        }
        else
        {
            btn->setStyleSheet("background:transparent;color :#C02020");
            btn->setDisabled(true);
            btn->setProperty("text", btn->property("text").toString());
            btn->setProperty("used",true);
        }
    }
}

void MainWindow::restoreNbres()
{
    foreach(QPushButton* btn,nomBtnNbre)
    {
        if(!btn->property("used").toBool())
        {
            btn->setStyleSheet("background:transparent;color :#d40000; font-weight: bold;");
            btn->setFont(fontMEDIUM);
            btn->setDisabled(false);
            btn->setProperty("text", btn->property("text").toString());
            btn->setProperty("used",false);
        }
        else
        {
            btn->setStyleSheet("background:transparent;color :#C02020");
            btn->setDisabled(true);
            btn->setProperty("text", btn->property("text").toString());
            btn->setProperty("used",true);
        }
    }
}

QPushButton *MainWindow::findCaseWhereIs(int x)
{
    QPushButton* retour;
    bool found = false;
    int z = 0;
    while(z < nomBtnCase.size() && !found){
        if(nomBtnCase.at(z)->text().toInt() == x){
            found = true;
            retour = nomBtnCase.at(z);
            qDebug()<<x<<" trouvé dans la case "<<z;
        }
        z++;
    }
    return retour;
}

QPushButton *MainWindow::findNbreWhereIs(int x)
{
    QPushButton* retour;
    bool found = false;
    int z = 0;
    while(z < nomBtnNbre.size() && !found){
        if(nomBtnNbre.at(z)->text().toInt() == x){
            found = true;
            retour = nomBtnNbre.at(z);
        }
        z++;
    }
    return retour;
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
    if (niveau > 3) {
        if (nomBtnCase[i]->text() != "") {
            //le btnCase n'est pas vide, vider !
            nomBtnCase[i]->setText("");
            return;
        }
        // valuer le btnCase
//        bool ok = false;
//        while (!ok)
//            // astuce (ben oui) puisque actuelBtnNbre désigne normalement un indice pas une valeur
//            actuelBtnNbre = QInputDialog::getInteger(this, trUtf8("Ton choix"), trUtf8("Nombre entier entre 1 et 34"), 16, 1, 34, 1, &ok);
        AbulEduInputBox* inputBox = new AbulEduInputBox(trUtf8("Choisis un nombre"),trUtf8("Choisis un nombre entre 1 et 34"));
        inputBox->setWink();
        inputBox->show();
        actuelBtnCase = i;
        /* Pour pouvoir vérifier que mon nombre est bien compris entre 1 et 34 il me faut un validateur
           Ou alors il faudra que je mette cet AbulEduMessageBox dans un méthode retournant un booléen, dans laquelle je testerai le retour
        inputBox->setValidatorForInput(QRegExp)*/
        connect(inputBox, SIGNAL(signalAbeInputBoxOK(QString)),SLOT(slotMainWindowSetInCase(QString)),Qt::UniqueConnection);
    }
    int k = indexInCasesInitial(nomBtnCase[i]->text().toInt());
    if (k >= 0) {
        // bouton déjà occupé : restaurer le btnNbre
        if (niveau <= 3) restaurerBtnNbre(k);
        // effacer le bouton
        nomBtnCase[i]->setText("");
        actuelBtnCase = -1;
        if (actuelBtnNbre >= 0 && niveau < 4) {
            // déposer actuelBtnNbre sur cette case
            nomBtnCase[i]->setStyleSheet("background:transparent;color : #d40000");
            nomBtnCase[i]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
            actuelBtnNbre = -1;
        }
    } else if (actuelBtnNbre >= 0) {
        // déposer actuelBtnNbre sur cette case
        nomBtnCase[i]->setStyleSheet("background:transparent;color : #d40000");
        if (niveau > 3)
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
//    ui->frmButtons->setVisible(false);
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
    if (actuelBtnNbre >= 0 && niveau <= 3) {
        restaurerBtnNbre(actuelBtnNbre);
    }
    nomBtnNbre[i]->setStyleSheet("background:transparent;color :#d40000");
    nomBtnNbre[i]->setFont(fontMINUS);
    nomBtnNbre[i]->setDisabled(true);
    actuelBtnNbre = i;
    if (actuelBtnCase > 0) {
        // déposer le nombe de ce btnNbre sur le actuelBtnCase
        nomBtnCase[actuelBtnCase]->setStyleSheet("background:transparent;color : #d40000");
        nomBtnCase[actuelBtnCase]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
        actuelBtnCase = -1;
        actuelBtnNbre = -1;
    }
    setInformation();
}

void MainWindow::restaurerBtnNbre(int i) {
    nomBtnNbre[i]->setStyleSheet("background:transparent;color :#d40000");
    nomBtnNbre[i]->setFont(fontMEDIUM);
    nomBtnNbre[i]->setDisabled(false);
    setInformation();
}

void MainWindow::on_btnVerifier_clicked()
{
    foreach(AbulEduMessageBoxV1* mbox,ui->pagePrincipale->findChildren<AbulEduMessageBoxV1*>()){
        mbox->close();
    }
    // vérifier que la grille est suffisamment remplie sinon return
    QList<int> connus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() != "") connus << i;
    }
    if (connus.length() <= (4 - niveau) || (connus.length() == 0 && niveau > 3)) {
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Vérification refusée"),trUtf8("Commence à remplir la grille !"),true,ui->pagePrincipale);
        msg->show();
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
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Félicitations"),trUtf8("Tu peux choisir une nouvelle grille en cliquant sur la feuille de cerisier ou en changeant de niveau..."),true,ui->pagePrincipale);
        msg->setWink();
        msg->show();
        setAbeLineLog("Complète la grille","", -1, 0 , abeEvaluation(), "", "", "", trUtf8("Vérification"));
        pushAbulEduLogs();
        ui->btnVerifier->setDisabled(true);
        ui->btnAbandonner->setDisabled(true);
        return;
    }

    // grille incomplète ; vérification en fonction du niveau

    if (niveau < 4)
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
            AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Une ou des erreurs ..."),trUtf8("Par exemple le nombre : %1 \n\nJe te prie de corriger !").arg(nomBtnCase[iFaute]->text()),true,ui->pagePrincipale);
            msg->show();
            nErreurs++;
            setAbeLineLog("Complète la grille","", -1, 0 , "d", "", "", "", trUtf8("Vérification"));
        } else {
            AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Pas d'erreur !"),trUtf8("Continue à complèter la grille..."),true,ui->pagePrincipale);
            msg->setWink();
            msg->show();
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
                    AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Une ou des erreurs ..."),trUtf8("Par exemple la ligne %1\n\nJe te prie de corriger !").arg(i+1),true,ui->pagePrincipale);
                    msg->show();
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
                    AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Une ou des erreurs ..."),trUtf8("Par exemple la colonne %1\n\nJe te prie de corriger !").arg(i+1),true,ui->pagePrincipale);
                    msg->show();
                    setAbeLineLog("Complète la grille","", -1, 0 , "d", "", "", "", trUtf8("Vérification"));
                    return;
            }
        }
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Pas d'erreur !"),trUtf8("Continue à complèter la grille..."),true,ui->pagePrincipale);
        msg->setWink();
        msg->show();
        setAbeLineLog("Complète la grille","", -1, 0 , "a", "", "", "", trUtf8("Vérification"));
    }
} // fin on_btnVerifier_clicked

void MainWindow::on_cBoxNiveau_activated(int index)
{
    /* A supprimer */
}

//void MainWindow::on_cBoxSuite_activated(int index)
//{
//    alea = index;
//    if (index > 0) {
//        bool ok = false;
//        int n = QInputDialog::getInteger(this, trUtf8("A toi de choisir"), trUtf8("Nombre entier\n  plus grand que 15\n  plus petit que 35"), 25, 16, 34, 1, &ok);
//        if (ok)
//            borneSup = n;
//        else {
////            ui->cBoxSuite->setCurrentIndex(0);
//            alea = 0;
//        }
//    }
//    initFubuki();
//}

void MainWindow::on_btnAbandonner_clicked()
{
    foreach(AbulEduMessageBoxV1* mbox,ui->pagePrincipale->findChildren<AbulEduMessageBoxV1*>()){
        mbox->close();
    }
    for (int i = 0; i < 9; i++) {
        qDebug()<<"case "<<i<<" : "<<nomBtnCase[i]->text().toInt()<<" -> "<<cases[i];
        if (nomBtnCase[i]->text().toInt() != cases[i]) {
            nomBtnCase[i]->setStyleSheet("background:transparent;color : #006cc0");
            nErreurs++;
        } else {
//            nomBtnCase[i]->setStyleSheet("background:transparent;color : #ec11da");
        }
        nomBtnCase[i]->setProperty("text", QString::number(cases[i]));
        nomBtnCase[i]->setDisabled(true);
    }
    ui->btnVerifier->setDisabled(true);
    ui->btnAbandonner->setDisabled(true);

//    ui->btnNouveau->setDisabled(false);

    if (nErreurs > 0)
    {
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Pas trouvé ?"),trUtf8("Voici un corrigé ! \n\nTu peux choisir une nouvelle grille en cliquant sur la feuille de cerisier ou en changeant de niveau..."),true,ui->pagePrincipale);
        msg->move(20,150);
        msg->resize(400,100);
        msg->show();
    }
    else
    {
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Félicitations"),trUtf8("Tu peux choisir une nouvelle grille en cliquant sur la feuille de cerisier ou en changeant de niveau..."),true,ui->pagePrincipale);
        msg->move(20,150);
        msg->resize(400,100);
        msg->setWink();
        msg->show();
    }
    setAbeLineLog("Complète la grille","", -1, 0 , abeEvaluation(), "", "", "", trUtf8("Abandon"));
    pushAbulEduLogs();
}

void MainWindow::on_btnInformation_clicked()
{
    foreach(AbulEduMessageBoxV1* mbox,ui->pagePrincipale->findChildren<AbulEduMessageBoxV1*>()){
        mbox->close();
    }

    qDebug()<<"cases";
    foreach(int t,cases){
        qDebug()<<t;
    }
    QList<QPushButton*> faux;
    for (int i = 0; i < 9; i++) {
        if (!nomBtnCase[i]->text().isEmpty() && nomBtnCase[i]->text().toInt() != cases[i]) {
            faux << nomBtnCase[i];
        }
    }
    QList<QPushButton*> inconnus = faux;
    for (int i = 0; i < 9; i++) {
        qDebug()<<i<<" -> "<<nomBtnCase[i]->text();
        if (nomBtnCase[i]->text() == "") inconnus << nomBtnCase[i];
    }
    qDebug()<<"Inconnus";
    qDebug()<<inconnus;
    qDebug()<<"faux";
    qDebug()<<faux;
    // tirer l'un de ces nombres au hasard
    if(inconnus.size() > 0)
    {
        QString alertMsg;
        /* je tire un rang parmi les cases inconnues (vides ou fausses) */
        int iBtn = rand() % inconnus.length();
        /* je récupère le numéro de bouton à ce rang */
        int ind = inconnus.at(iBtn)->objectName().remove("btnCase").toInt();
        /* je trouve le nombre qui doit se trouver à ce bouton */
        int nbDonne = cases.at(ind);

        qDebug()<<iBtn<<" donc "<<nbDonne;
        if(faux.contains(inconnus[iBtn])){
            alertMsg = trUtf8("Je te corrige le nombre ... %1").arg(QString::number(nbDonne));
            qDebug() << "c'est un faux "<<alertMsg ;
            inconnus[iBtn]->setText(QString::number(nbDonne));
            inconnus.at(iBtn)->setStyleSheet("background:transparent;color : rgb(134,45,176);");
            findCaseWhereIs(nbDonne)->setText("");
        }
        else {
            alertMsg = trUtf8("Je propose le nombre ... %1").arg(QString::number(nbDonne));
            inconnus.at(iBtn)->setText(QString::number(nbDonne));
            inconnus.at(iBtn)->setStyleSheet("background:transparent;color : rgb(0,108,192);");
            qDebug() << "C'est un inconnu "<<alertMsg ;
            QPushButton* inPioche = findNbreWhereIs(nbDonne);
            inPioche->setStyleSheet("background:transparent;color : #C02020");
            inPioche->setFont(fontMINUS);
            inPioche->setDisabled(true);
        }
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Allez, je t'aide..."),alertMsg,true,ui->pagePrincipale);
        msg->move(20,150);
        msg->resize(400,100);
        msg->setWink();
        msg->show();

        nAides++;
        setAbeLineLog("Complète la grille","", -1, 0 , "", "", "", "", trUtf8("Information"));

        setInformation();
    }
    else
    {
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Allez, je t'aide..."),trUtf8("Choisis une nouvelle grille par le menu Feuille ou change de niveau."),true,ui->pagePrincipale);
        msg->setWink();
        msg->show();
    }
}

void MainWindow::on_abeMenuFeuilleBtnNew_clicked()
{
    initFubuki();
//    on_btnFeuille_clicked();
}

void MainWindow::setInformation() {
    if (niveau >3) {
        ui->btnInformation->setDisabled(true);
        return;
    }
    QList<int> inconnus;
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text() == "") inconnus << i;
    }
//    if (inconnus.length() < 5)
//        ui->btnInformation->setDisabled(true);
//    else
//        ui->btnInformation->setDisabled(false);
}

void MainWindow::slotChangeLangue(QString lang)
{
    qApp->removeTranslator(&qtTranslator);
    qApp->removeTranslator(&myappTranslator);

    //Un 1er qtranslator pour prendre les traductions QT Systeme
    //c'est d'ailleur grace a ca qu'on est en RTL
    qtTranslator.load("qt_" + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);

    //foreach (QWidget *widget, QApplication::allWidgets()) widget->setLayoutDirection(Qt::RightToLeft);
    //Et un second qtranslator pour les traductions specifiques du
    //logiciel
    myappTranslator.load("leterrier-fubuki_" + lang, "lang");
    qApp->installTranslator(&myappTranslator);
    ui->retranslateUi(this);
}

void MainWindow::on_btnNiveaux_clicked()
{
    ui->frmNiveau->setVisible(true);
    ui->frmNiveau->raise();
    ui->btnNiveaux->setStyleSheet(ui->btnNiveaux->styleSheet().replace("background-color:rgba(0,0,0,0);","border-radius:5px;background-color:#ffffff;"));
    on_btnNombresFermer_clicked();
}

void MainWindow::on_btnNiveauAnnuler_clicked()
{
    ui->frmNiveau->setVisible(false);
    ui->btnNiveaux->setStyleSheet(ui->btnNiveaux->styleSheet().replace("border-radius:5px;background-color:#ffffff;","background-color:rgba(0,0,0,0);"));
}


void MainWindow::on_btnNombres_clicked()
{
    ui->frmChoixNombres->setVisible(true);
    ui->frmChoixNombres->raise();
    ui->btnNombres->setStyleSheet(ui->btnNombres->styleSheet().replace("background-color:rgba(0,0,0,0);","border-radius:5px;background-color:#ffffff;"));
    on_btnNiveauAnnuler_clicked();
}

void MainWindow::on_btnNiveauTresFacile_clicked()
{
    niveau = 0;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauFacile_clicked()
{
    niveau = 1;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauMoyen_clicked()
{
    niveau = 2;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauDifficile_clicked()
{
    niveau = 3;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauTresDifficile_clicked()
{
    niveau = 4;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNombresZeroNeuf_clicked()
{
    alea = 0;
    initFubuki();
    ui->frmChoixNombres->setVisible(false);
    ui->btnNombres->setStyleSheet(ui->btnNombres->styleSheet().replace("border-radius:5px;background-color:#ffffff;","background-color:rgba(0,0,0,0);"));
}

void MainWindow::on_btnNombresAuChoix_clicked()
{
    alea = 1;
    AbulEduInputBox* inputBox = new AbulEduInputBox(trUtf8("Nombre maximum"),trUtf8("Choisis un nombre entre 1 et 34"),true,ui->pagePrincipale);
    inputBox->setWink();
    inputBox->show();
    /* Pour pouvoir vérifier que mon nombre est bien compris entre 1 et 34 il me faut un validateur
       Ou alors il faudra que je mette cet AbulEduMessageBox dans un méthode retournant un booléen, dans laquelle je testerai le retour
    inputBox->setValidatorForInput(QRegExp)*/
    connect(inputBox, SIGNAL(signalAbeInputBoxOK(QString)),SLOT(slotMainWindowSetBorneSup(QString)),Qt::UniqueConnection);

    ui->frmChoixNombres->setVisible(false);
    ui->btnNombres->setStyleSheet(ui->btnNombres->styleSheet().replace("border-radius:5px;background-color:#ffffff;","background-color:rgba(0,0,0,0);"));
}

void MainWindow::on_btnNombresFermer_clicked()
{
    ui->frmChoixNombres->setVisible(false);
    ui->btnNombres->setStyleSheet(ui->btnNombres->styleSheet().replace("border-radius:5px;background-color:#ffffff;","background-color:rgba(0,0,0,0);"));
}

void MainWindow::on_abeMenuFeuilleBtnQuit_clicked()
{
    close();
}

void MainWindow::on_abeMenuFeuilleBtnHelp_clicked()
{
    ui->stackedWidget->abeStackedWidgetSlideInWidget(ui->pageAbout);
}

void MainWindow::slotMainWindowSetBorneSup(QString nombreLu)
{
    bool ok;// = false;
    borneSup = nombreLu.toInt(&ok);
    if (!ok || borneSup > 34){
        alea = 0;
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Problème"),trUtf8("Tu n'as pas donné un nombre compris entre 1 et 34, on reste avec les nombres de 1 à 9..."),true,ui->pagePrincipale);
        msg->show();
    }
    initFubuki();
}

void MainWindow::slotMainWindowSetInCase(QString nombreLu)
{
    bool ok;
    int conversion = nombreLu.toInt(&ok);
    if (!ok || conversion > 34 || conversion < 1){
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Problème"),trUtf8("Tu n'as pas donné un nombre compris entre 1 et 34..."),true,ui->pagePrincipale);
        msg->show();
    }
    else
    {
        nomBtnCase[actuelBtnCase]->setText(nombreLu);
        nomBtnCase[actuelBtnCase]->setStyleSheet("background:transparent;color : #d40000");
    }
}

void MainWindow::on_btnDebut_clicked()
{
    foreach(AbulEduMessageBoxV1* mbox,ui->pagePrincipale->findChildren<AbulEduMessageBoxV1*>()){
        mbox->close();
    }
    ui->btnVerifier->setDisabled(false);
    ui->btnAbandonner->setDisabled(false);
    restoreCases();
    restoreNbres();
}

void MainWindow::slotMainWindowShowMainPage()
{
    ui->stackedWidget->abeStackedWidgetSlideInWidget(ui->pagePrincipale);
}
