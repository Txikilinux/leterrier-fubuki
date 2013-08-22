/**
  * Fubuki (le terrier d'AbulEdu)
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
#include "abuleduaproposv0.h"
#include "abuleduexercicev0.h"

bool isIn(int i, QList<int> s);

MainWindow::MainWindow(QWidget *parent) :
    AbulEduExerciceV0(parent),
    ui(new Ui::MainWindow)
{

    //Langue
    QString locale = QLocale::system().name().section('_', 0, 0);

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

    ui->setupUi(this);
    creeMenuLangue();

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

    //Une astuce pour eviter de faire 7 boutons * 3 lignes pour activer les icones
    QList<AbulEduFlatBoutonV1 *> btns = ui->frmIcones->findChildren<AbulEduFlatBoutonV1 *>();
    for(int i = 0; i < btns.count(); i++)
    {
        QString composant = btns.at(i)->whatsThis();
        btns.at(i)->setIconeNormale(QString(":/data/images/%1").arg(composant));

#ifdef __ABULEDUTABLETTEV1__MODE__
        btns.at(i)->setIconePressed(QString(":/data/images/%1Hover").arg(composant));
#else
        btns.at(i)->setIconeSurvol(QString(":/data/images/%1Hover").arg(composant));
#endif
        btns.at(i)->setIconeDisabled(QString(":/data/images/%1Disabled").arg(composant));
        btns.at(i)->setTexteAlignement(Qt::AlignLeft);
    }

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
    foreach(AbulEduFlatBoutonV1* enfant,ui->frmIcones->findChildren<AbulEduFlatBoutonV1 *>())
    {
        enfant->setCouleurFondPressed(QColor(255,255,255,50));
        enfant->setCouleurTexteSurvol(Qt::red);
        enfant->setCouleurTexteNormale(Qt::white);
        enfant->setStyleSheet(enfant->styleSheet().replace("border-image","text-align: bottom;background-image"));
        enfant->setStyleSheet(enfant->styleSheet().replace("image-position: center","background-position: center top"));
    }

    /* Positionnement en dur puisque la hauteur de fenêtre "utile" est fixe */
    ui->frmNiveau->move(ui->frmIcones->x()-ui->frmNiveau->width()+8,ui->frmIcones->y()+21);
    ui->frmChoixNombres->move(ui->frmIcones->x()-ui->frmChoixNombres->width()+8,ui->frmIcones->y()+128);
    ui->frmNiveau->setVisible(false);
    ui->frmChoixNombres->setVisible(false);


    setWindowFlags(Qt::CustomizeWindowHint);
    ui->frmButtons->move(9,0);
    ui->frmButtons->setVisible(false);

    ui->btnLanguages->setIconeNormale(":/data/flags/fr");
    ui->frmChoixLangues->move(790,0);
    ui->frmChoixLangues->setVisible(false);
//    ui->btnEs->setVisible(false);
    ui->btnIt->setVisible(false);
    ui->btnDe->setVisible(false);
    ui->btnOc->setVisible(false);
    foreach(AbulEduFlatBoutonV1* btn, ui->frmChoixLangues->findChildren<AbulEduFlatBoutonV1*>())
    {
        if(!btn->whatsThis().isEmpty())
        {
            connect(btn, SIGNAL(clicked()),SLOT(slotChangeLangue()),Qt::UniqueConnection);
        }
    }

#ifdef __ABULEDUTABLETTEV1__MODE__
    ui->btnMinimized->setVisible(false);
    ui->btnFullScreen->setVisible(false);
#else
    ui->btnMinimized->setCouleurFondSurvol(QColor(252,152,41));
    ui->btnMinimized->setCouleurFondPressed(QColor(252,152,41));
    ui->btnMinimized->setCouleurFondNormale(QColor(203,106,89));
    ui->btnMinimized->setAllMargins(8,4,8,12);
    ui->btnMinimized->setBorderRadius(4);
    ui->btnFullScreen->setCouleurFondSurvol(QColor(252,152,41));
    ui->btnFullScreen->setCouleurFondPressed(QColor(252,152,41));
    ui->btnFullScreen->setCouleurFondNormale(QColor(203,106,89));
    ui->btnFullScreen->setAllMargins(8,12,8,4);
    ui->btnFullScreen->setBorderRadius(4);
#endif

    QDesktopWidget *widget = QApplication::desktop();
    int desktop_width = widget->width();
    int desktop_height = widget->height();
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
    if(m_isFirstFubuki)
    {
        initFubuki();
    }
    m_isFirstFubuki = false;
    foreach(AbulEduFlatBoutonV1* enfant,ui->frmButtons->findChildren<AbulEduFlatBoutonV1 *>())
    {
        enfant->setCouleurTexteSurvol(Qt::red);
        enfant->setStyleSheet(enfant->styleSheet().replace("border-image","text-align: bottom;background-image"));
        enfant->setStyleSheet(enfant->styleSheet().replace("image-position: center","background-position: center top"));
    }
    ui->btnFeuille->setStyleSheet("QPushButton > *{color:red;}QPushButton{border: none; color:rgba(0,0,0,255);background-repeat: no-repeat;background-color:transparent;}");
}

#ifndef __ABULEDUTABLETTEV1__MODE__
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isWindowMoving) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && ui->lblTitre->rect().contains(event->pos())) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
        m_isWindowMoving = true;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isWindowMoving = false;
}
#endif

void MainWindow::initFubuki()
{
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
    if (alea == 0 && niveau < 5)
        casesInitial << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    else {
        if (niveau >= 5) {
            borneSup = 15 + rand() %20;
//            ui->cBoxSuite->setDisabled(true);
            ui->btnNombres->setDisabled(true);
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
    if (niveau <= 4) {
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

    ui->btnNouveau->setDisabled(true);

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
    if (niveau > 4) {
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
        AbulEduMessageBoxV1* inputBox = new AbulEduMessageBoxV1(trUtf8("Nombre maximum"),trUtf8("Choisis un nombre entre 1 et 34"));
        inputBox->setWink();
        inputBox->abeMessageBoxShowInput(true);
        inputBox->show();
        /* Pour pouvoir vérifier que mon nombre est bien compris entre 1 et 34 il me faut un validateur
           Ou alors il faudra que je mette cet AbulEduMessageBox dans un méthode retournant un booléen, dans laquelle je testerai le retour
        inputBox->setValidatorForInput(QRegExp)*/
        connect(inputBox, SIGNAL(signalAbeMessageBoxInputOK(QString)),SLOT(slotMainWindowSetBorneSup(QString)),Qt::UniqueConnection);
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
            nomBtnCase[i]->setStyleSheet("background:transparent;color : #d40000");
            nomBtnCase[i]->setProperty("text", QString::number(casesInitial[actuelBtnNbre]));
            actuelBtnNbre = -1;
        }
    } else if (actuelBtnNbre >= 0) {
        // déposer actuelBtnNbre sur cette case
        nomBtnCase[i]->setStyleSheet("background:transparent;color : #d40000");
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
    ui->frmButtons->setVisible(false);
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
    if (actuelBtnNbre >= 0 && niveau <= 4) {
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
    ui->frmButtons->setVisible(false);
}

void MainWindow::restaurerBtnNbre(int i) {
    nomBtnNbre[i]->setStyleSheet("background:transparent;color :#d40000");
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
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Vérification refusée"),trUtf8("Commence à remplir la grille !"));
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
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Félicitations"),trUtf8("Tu peux choisir une nouvelle grille..."));
        msg->setWink();
        msg->show();
        setAbeLineLog("Complète la grille","", -1, 0 , abeEvaluation(), "", "", "", trUtf8("Vérification"));
        pushAbulEduLogs();
        ui->btnVerifier->setDisabled(true);
        ui->btnAbandonner->setDisabled(true);

        ui->btnNouveau->setDisabled(false);

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
            AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Une ou des erreurs ..."),trUtf8("Par exemple le nombre : %1 \n\nJe te prie de corriger !").arg(nomBtnCase[iFaute]->text()));
            msg->show();
            nErreurs++;
            setAbeLineLog("Complète la grille","", -1, 0 , "d", "", "", "", trUtf8("Vérification"));
        } else {
            AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Pas d'erreur !"),trUtf8("Continue à complèter la grille..."));
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
                    AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Une ou des erreurs ..."),trUtf8("Par exemple la ligne %1\n\nJe te prie de corriger !").arg(i+1));
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
                    AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Une ou des erreurs ..."),trUtf8("Par exemple la colonne %1\n\nJe te prie de corriger !").arg(i+1));
                    msg->show();
                    setAbeLineLog("Complète la grille","", -1, 0 , "d", "", "", "", trUtf8("Vérification"));
                    return;
            }
        }
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Pas d'erreur !"),trUtf8("Continue à complèter la grille..."));
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
    for (int i = 0; i < 9; i++) {
        if (nomBtnCase[i]->text().toInt() != cases[i]) {
            nomBtnCase[i]->setStyleSheet("background:transparent;color : #d40000");
            nErreurs++;
        } else {
            nomBtnCase[i]->setStyleSheet("background:transparent;color : #20C020");
        }
        nomBtnCase[i]->setProperty("text", QString::number(cases[i]));
        nomBtnCase[i]->setDisabled(true);
    }
    ui->btnVerifier->setDisabled(true);
    ui->btnAbandonner->setDisabled(true);

    ui->btnNouveau->setDisabled(false);

    if (nErreurs > 0)
    {
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Pas trouvé ?"),trUtf8("Voici un corrigé ! \n\nTu peux choisir une nouvelle grille..."));
        msg->show();
    }
    else
    {
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Félicitations"),trUtf8("Tu peux choisir une nouvelle grille..."));
        msg->setWink();
        msg->show();
    }
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
    AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Allez, je t'aide..."),trUtf8("Je propose le nombre ... %1").arg(QString::number(cases[inconnus[iBtn]])));
    msg->setWink();
    msg->show();
    nomBtnCase[inconnus[iBtn]]->setStyleSheet("background:transparent;color : #C02020");
    //nomBtnCase[inconnus[iBtn]]->setFont(fontBIG);
    nomBtnCase[inconnus[iBtn]]->setProperty("text", QString::number(cases[inconnus[iBtn]]));
    nomBtnCase[inconnus[iBtn]]->setDisabled(true);
    // rechercher la case correspondant au nombre donné
    //qDebug() << "rechercher la case " << cases[inconnus[iBtn]];
    int k = indexInCasesInitial(cases[inconnus[iBtn]]);
    nomBtnNbre[k]->setStyleSheet("background:transparent;color : #C02020");
    nomBtnNbre[k]->setFont(fontMINUS);
    nomBtnNbre[k]->setDisabled(true);

    nAides++;
    setAbeLineLog("Complète la grille","", -1, 0 , "", "", "", "", trUtf8("Information"));

    setInformation();
}

void MainWindow::on_btnNouveau_clicked()
{
    initFubuki();
    on_btnFeuille_clicked();
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

void MainWindow::slotChangeLangue()
{
    QString lang = static_cast<AbulEduFlatBoutonV1*>(sender())->whatsThis();
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

void MainWindow::creeMenuLangue()
{
    /** @todo à revoir sans le menu */
    QString locale = QLocale::system().name().section('_', 0, 0);

    QAction* actionLangueEn = new QAction(trUtf8("&Anglais"),this);
    actionLangueEn->setCheckable(true);
    actionLangueEn->setObjectName("en");
    connect(actionLangueEn, SIGNAL(triggered()), this, SLOT(slotChangeLangue()));
    m_languesDisponibles << actionLangueEn;

    QAction* actionLangueFr = new QAction(trUtf8("&Français"),this);
    actionLangueFr->setCheckable(true);
    actionLangueFr->setObjectName("fr");
    connect(actionLangueFr, SIGNAL(triggered()), this, SLOT(slotChangeLangue()));
    m_languesDisponibles << actionLangueFr;

    foreach(QAction* langue,m_languesDisponibles)
    {
        if (langue->objectName() == locale)
        {
            langue->setChecked(true);
        }
        else
        {
            langue->setChecked(false);
        }
    }
}

void MainWindow::on_btnNiveaux_clicked()
{
    ui->frmNiveau->setVisible(true);
    ui->frmNiveau->raise();
    ui->btnNiveaux->setStyleSheet(ui->btnNiveaux->styleSheet().replace("background-color:rgba(0,0,0,0);","border-radius:5px;background-color:#ffffff;"));
    if (ui->frmButtons->isVisible())
    {
        ui->frmButtons->setVisible(false);
    }
    on_btnNombresFermer_clicked();
    on_btnLangueAnnuler_clicked();
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
    if (ui->frmButtons->isVisible())
    {
        ui->frmButtons->setVisible(false);
    }
    on_btnLangueAnnuler_clicked();
    on_btnNiveauAnnuler_clicked();
}

void MainWindow::on_btnNiveauFacile_clicked()
{
    niveau = 0;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauMoyen_clicked()
{
    niveau = 1;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauDifficile_clicked()
{
    niveau = 2;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauTresDifficile_clicked()
{
    niveau = 3;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauDiabolique_clicked()
{
    niveau = 4;
    ui->lblLevel->setPixmap(QPixmap(":/data/images/belt"+QString::number(niveau)));
    initFubuki();
}

void MainWindow::on_btnNiveauInfernal_clicked()
{
    niveau = 5;
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
    AbulEduMessageBoxV1* inputBox = new AbulEduMessageBoxV1(trUtf8("Nombre maximum"),trUtf8("Choisis un nombre entre 1 et 34"));
    inputBox->abeMessageBoxShowInput(true);
    inputBox->setWink();
    inputBox->show();
    /* Pour pouvoir vérifier que mon nombre est bien compris entre 1 et 34 il me faut un validateur
       Ou alors il faudra que je mette cet AbulEduMessageBox dans un méthode retournant un booléen, dans laquelle je testerai le retour
    inputBox->setValidatorForInput(QRegExp)*/
    connect(inputBox, SIGNAL(signalAbeMessageBoxInputOK(QString)),SLOT(slotMainWindowSetBorneSup(QString)),Qt::UniqueConnection);

    ui->frmChoixNombres->setVisible(false);
    ui->btnNombres->setStyleSheet(ui->btnNombres->styleSheet().replace("border-radius:5px;background-color:#ffffff;","background-color:rgba(0,0,0,0);"));
}

void MainWindow::on_btnNombresFermer_clicked()
{
    ui->frmChoixNombres->setVisible(false);
    ui->btnNombres->setStyleSheet(ui->btnNombres->styleSheet().replace("border-radius:5px;background-color:#ffffff;","background-color:rgba(0,0,0,0);"));
}

void MainWindow::on_btnSortie_clicked()
{
    close();
}

void MainWindow::on_btnFeuille_clicked()
{
    if (ui->frmButtons->isVisible())
    {
        ui->frmButtons->setVisible(false);
    }
    else
    {
        ui->frmButtons->setVisible(true);
        ui->frmButtons->raise();
    }
    on_btnNombresFermer_clicked();
    on_btnNiveauAnnuler_clicked();
    on_btnLangueAnnuler_clicked();
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->slideInWidget(ui->pagePrincipale);
}

void MainWindow::on_btnAideFeuille_clicked()
{
    /* En attendant d'avoir avancé sur une nouvelle boite à propos, je vais ici appeler l'autre bouton aide, celui de la télécommande
    ui->stackedWidget->slideInWidget(ui->pageApropos); */
    on_btnInformation_clicked();
    on_btnFeuille_clicked();
}

void MainWindow::on_btnMinimized_clicked()
{
    showMinimized();
}

void MainWindow::on_btnFullScreen_clicked()
{
    if(isFullScreen())
    {
        showNormal();
        ui->centralWidget->move(0,0);
        ui->widgetContainer->move(0,0);
        ui->btnFullScreen->setIconeNormale(":/data/images/showMaximized");
    }
    else
    {
        QDesktopWidget *widget = QApplication::desktop();
        int desktop_width = widget->width();
        int desktop_height = widget->height();
//        this->move((desktop_width-this->width())/2, (desktop_height-this->height())/2);
        ui->centralWidget->move((desktop_width-ui->centralWidget->width())/2, (desktop_height-ui->centralWidget->height())/2);
        ui->widgetContainer->move((desktop_width-ui->widgetContainer->width())/2, (desktop_height-ui->widgetContainer->height())/2);
        showFullScreen();
        ui->btnFullScreen->setIconeNormale(":/data/images/showNormal");
    }
}

void MainWindow::slotMainWindowSetBorneSup(QString nombreLu)
{
    bool ok;// = false;
    borneSup = nombreLu.toInt(&ok);
    if (!ok || borneSup > 34){
        alea = 0;
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Problème"),trUtf8("Tu n'as pas donné un nombre compris entre 1 et 34, on reste avec les nombres de 1 à 9..."));
        msg->show();
    }
    initFubuki();
}

void MainWindow::on_btnDebut_clicked()
{
    ui->btnVerifier->setDisabled(false);
    ui->btnAbandonner->setDisabled(false);
    ui->btnNouveau->setDisabled(true);
    restoreCases();
    restoreNbres();
}

void MainWindow::on_btnLanguages_clicked()
{
    ui->frmChoixLangues->setVisible(true);
    if (ui->frmButtons->isVisible())
    {
        ui->frmButtons->setVisible(false);
    }
    on_btnNombresFermer_clicked();
    on_btnNiveauAnnuler_clicked();
}

void MainWindow::on_btnFr_clicked()
{
    ui->btnLanguages->setIconeNormale(":/data/flags/fr");
    ui->frmChoixLangues->setVisible(false);
}

void MainWindow::on_btnEn_clicked()
{
    ui->btnLanguages->setIconeNormale(":/data/flags/en");
    ui->frmChoixLangues->setVisible(false);
}

void MainWindow::on_btnEs_clicked()
{
    ui->btnLanguages->setIconeNormale(":/data/flags/es");
    ui->frmChoixLangues->setVisible(false);
}

void MainWindow::on_btnIt_clicked()
{
    ui->btnLanguages->setIconeNormale(":/data/flags/it");
    ui->frmChoixLangues->setVisible(false);
}

void MainWindow::on_btnDe_clicked()
{
    ui->btnLanguages->setIconeNormale(":/data/flags/de");
    ui->frmChoixLangues->setVisible(false);
}

void MainWindow::on_btnOc_clicked()
{
    ui->btnLanguages->setIconeNormale(":/data/flags/oc");
    ui->frmChoixLangues->setVisible(false);
}

void MainWindow::on_btnLangueAnnuler_clicked()
{
    ui->frmChoixLangues->setVisible(false);
}
