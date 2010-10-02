/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Oct 2 16:15:17 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuitter;
    QAction *action_propos;
    QAction *actionAide_en_local;
    QWidget *centralWidget;
    QPushButton *btnCase0;
    QPushButton *btnCase1;
    QPushButton *btnCase2;
    QPushButton *btnCase3;
    QPushButton *btnCase4;
    QPushButton *btnCase5;
    QPushButton *btnCase6;
    QPushButton *btnCase7;
    QPushButton *btnCase8;
    QLabel *lblH0;
    QLabel *lblH1;
    QLabel *lblH2;
    QLabel *lblV0;
    QLabel *lblV1;
    QLabel *lblV2;
    QPushButton *btnNbre0;
    QPushButton *btnNbre1;
    QPushButton *btnNbre2;
    QPushButton *btnNbre3;
    QPushButton *btnNbre4;
    QPushButton *btnNbre5;
    QPushButton *btnNbre6;
    QPushButton *btnNbre7;
    QPushButton *btnNbre8;
    QPushButton *btnVerifier;
    QComboBox *cBoxNiveau;
    QPushButton *btnAide;
    QLabel *lblRegle;
    QComboBox *cBoxSuite;
    QPushButton *btnCorrige;
    QPushButton *btnNouveau;
    QMenuBar *menuBar;
    QMenu *menuFichier;
    QMenu *menuAide;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        actionQuitter = new QAction(MainWindow);
        actionQuitter->setObjectName(QString::fromUtf8("actionQuitter"));
        action_propos = new QAction(MainWindow);
        action_propos->setObjectName(QString::fromUtf8("action_propos"));
        actionAide_en_local = new QAction(MainWindow);
        actionAide_en_local->setObjectName(QString::fromUtf8("actionAide_en_local"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        btnCase0 = new QPushButton(centralWidget);
        btnCase0->setObjectName(QString::fromUtf8("btnCase0"));
        btnCase0->setGeometry(QRect(30, 10, 60, 60));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(26);
        font.setBold(true);
        font.setWeight(75);
        btnCase0->setFont(font);
        btnCase1 = new QPushButton(centralWidget);
        btnCase1->setObjectName(QString::fromUtf8("btnCase1"));
        btnCase1->setGeometry(QRect(110, 10, 60, 60));
        btnCase1->setFont(font);
        btnCase2 = new QPushButton(centralWidget);
        btnCase2->setObjectName(QString::fromUtf8("btnCase2"));
        btnCase2->setGeometry(QRect(190, 10, 60, 60));
        btnCase2->setFont(font);
        btnCase3 = new QPushButton(centralWidget);
        btnCase3->setObjectName(QString::fromUtf8("btnCase3"));
        btnCase3->setGeometry(QRect(30, 90, 60, 60));
        btnCase3->setFont(font);
        btnCase4 = new QPushButton(centralWidget);
        btnCase4->setObjectName(QString::fromUtf8("btnCase4"));
        btnCase4->setGeometry(QRect(110, 90, 60, 60));
        btnCase4->setFont(font);
        btnCase5 = new QPushButton(centralWidget);
        btnCase5->setObjectName(QString::fromUtf8("btnCase5"));
        btnCase5->setGeometry(QRect(190, 90, 60, 60));
        btnCase5->setFont(font);
        btnCase6 = new QPushButton(centralWidget);
        btnCase6->setObjectName(QString::fromUtf8("btnCase6"));
        btnCase6->setGeometry(QRect(30, 160, 60, 60));
        btnCase6->setFont(font);
        btnCase7 = new QPushButton(centralWidget);
        btnCase7->setObjectName(QString::fromUtf8("btnCase7"));
        btnCase7->setGeometry(QRect(110, 160, 60, 60));
        btnCase7->setFont(font);
        btnCase8 = new QPushButton(centralWidget);
        btnCase8->setObjectName(QString::fromUtf8("btnCase8"));
        btnCase8->setGeometry(QRect(190, 160, 60, 60));
        btnCase8->setFont(font);
        lblH0 = new QLabel(centralWidget);
        lblH0->setObjectName(QString::fromUtf8("lblH0"));
        lblH0->setGeometry(QRect(270, 10, 61, 61));
        lblH0->setFont(font);
        lblH0->setFrameShape(QFrame::StyledPanel);
        lblH1 = new QLabel(centralWidget);
        lblH1->setObjectName(QString::fromUtf8("lblH1"));
        lblH1->setGeometry(QRect(270, 90, 61, 61));
        lblH1->setFont(font);
        lblH1->setFrameShape(QFrame::StyledPanel);
        lblH2 = new QLabel(centralWidget);
        lblH2->setObjectName(QString::fromUtf8("lblH2"));
        lblH2->setGeometry(QRect(270, 160, 61, 61));
        lblH2->setFont(font);
        lblH2->setFrameShape(QFrame::StyledPanel);
        lblV0 = new QLabel(centralWidget);
        lblV0->setObjectName(QString::fromUtf8("lblV0"));
        lblV0->setGeometry(QRect(30, 230, 61, 61));
        lblV0->setFont(font);
        lblV0->setFrameShape(QFrame::StyledPanel);
        lblV1 = new QLabel(centralWidget);
        lblV1->setObjectName(QString::fromUtf8("lblV1"));
        lblV1->setGeometry(QRect(110, 230, 61, 61));
        lblV1->setFont(font);
        lblV1->setFrameShape(QFrame::StyledPanel);
        lblV2 = new QLabel(centralWidget);
        lblV2->setObjectName(QString::fromUtf8("lblV2"));
        lblV2->setGeometry(QRect(190, 230, 61, 61));
        lblV2->setFont(font);
        lblV2->setFrameShape(QFrame::StyledPanel);
        btnNbre0 = new QPushButton(centralWidget);
        btnNbre0->setObjectName(QString::fromUtf8("btnNbre0"));
        btnNbre0->setGeometry(QRect(390, 110, 40, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(15);
        font1.setBold(true);
        font1.setWeight(75);
        btnNbre0->setFont(font1);
        btnNbre1 = new QPushButton(centralWidget);
        btnNbre1->setObjectName(QString::fromUtf8("btnNbre1"));
        btnNbre1->setGeometry(QRect(440, 110, 40, 40));
        btnNbre1->setFont(font1);
        btnNbre2 = new QPushButton(centralWidget);
        btnNbre2->setObjectName(QString::fromUtf8("btnNbre2"));
        btnNbre2->setGeometry(QRect(490, 110, 40, 40));
        btnNbre2->setFont(font1);
        btnNbre3 = new QPushButton(centralWidget);
        btnNbre3->setObjectName(QString::fromUtf8("btnNbre3"));
        btnNbre3->setGeometry(QRect(390, 60, 40, 40));
        btnNbre3->setFont(font1);
        btnNbre4 = new QPushButton(centralWidget);
        btnNbre4->setObjectName(QString::fromUtf8("btnNbre4"));
        btnNbre4->setGeometry(QRect(440, 60, 40, 40));
        btnNbre4->setFont(font1);
        btnNbre5 = new QPushButton(centralWidget);
        btnNbre5->setObjectName(QString::fromUtf8("btnNbre5"));
        btnNbre5->setGeometry(QRect(490, 60, 40, 40));
        btnNbre5->setFont(font1);
        btnNbre6 = new QPushButton(centralWidget);
        btnNbre6->setObjectName(QString::fromUtf8("btnNbre6"));
        btnNbre6->setGeometry(QRect(390, 10, 40, 40));
        btnNbre6->setFont(font1);
        btnNbre7 = new QPushButton(centralWidget);
        btnNbre7->setObjectName(QString::fromUtf8("btnNbre7"));
        btnNbre7->setGeometry(QRect(440, 10, 40, 40));
        btnNbre7->setFont(font1);
        btnNbre8 = new QPushButton(centralWidget);
        btnNbre8->setObjectName(QString::fromUtf8("btnNbre8"));
        btnNbre8->setGeometry(QRect(490, 10, 40, 40));
        btnNbre8->setFont(font1);
        btnVerifier = new QPushButton(centralWidget);
        btnVerifier->setObjectName(QString::fromUtf8("btnVerifier"));
        btnVerifier->setGeometry(QRect(400, 260, 191, 31));
        cBoxNiveau = new QComboBox(centralWidget);
        cBoxNiveau->setObjectName(QString::fromUtf8("cBoxNiveau"));
        cBoxNiveau->setGeometry(QRect(271, 310, 117, 31));
        btnAide = new QPushButton(centralWidget);
        btnAide->setObjectName(QString::fromUtf8("btnAide"));
        btnAide->setGeometry(QRect(270, 260, 121, 27));
        lblRegle = new QLabel(centralWidget);
        lblRegle->setObjectName(QString::fromUtf8("lblRegle"));
        lblRegle->setGeometry(QRect(350, 160, 231, 91));
        cBoxSuite = new QComboBox(centralWidget);
        cBoxSuite->setObjectName(QString::fromUtf8("cBoxSuite"));
        cBoxSuite->setGeometry(QRect(30, 310, 231, 31));
        btnCorrige = new QPushButton(centralWidget);
        btnCorrige->setObjectName(QString::fromUtf8("btnCorrige"));
        btnCorrige->setGeometry(QRect(400, 310, 93, 27));
        btnNouveau = new QPushButton(centralWidget);
        btnNouveau->setObjectName(QString::fromUtf8("btnNouveau"));
        btnNouveau->setGeometry(QRect(500, 310, 93, 27));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menuFichier = new QMenu(menuBar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        menuAide = new QMenu(menuBar);
        menuAide->setObjectName(QString::fromUtf8("menuAide"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFichier->menuAction());
        menuBar->addAction(menuAide->menuAction());
        menuFichier->addAction(actionQuitter);
        menuAide->addAction(action_propos);
        menuAide->addAction(actionAide_en_local);

        retranslateUi(MainWindow);
        QObject::connect(actionQuitter, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Fubuki du Terrier", 0, QApplication::UnicodeUTF8));
        actionQuitter->setText(QApplication::translate("MainWindow", "Quitter", 0, QApplication::UnicodeUTF8));
        action_propos->setText(QApplication::translate("MainWindow", "\303\200 propos", 0, QApplication::UnicodeUTF8));
        actionAide_en_local->setText(QApplication::translate("MainWindow", "Aide en local", 0, QApplication::UnicodeUTF8));
        btnCase0->setText(QString());
        btnCase1->setText(QString());
        btnCase2->setText(QString());
        btnCase3->setText(QString());
        btnCase4->setText(QString());
        btnCase5->setText(QString());
        btnCase6->setText(QString());
        btnCase7->setText(QString());
        btnCase8->setText(QString());
        lblH0->setText(QString());
        lblH1->setText(QString());
        lblH2->setText(QString());
        lblV0->setText(QString());
        lblV1->setText(QString());
        lblV2->setText(QString());
        btnNbre0->setText(QString());
        btnNbre1->setText(QString());
        btnNbre2->setText(QString());
        btnNbre3->setText(QString());
        btnNbre4->setText(QString());
        btnNbre5->setText(QString());
        btnNbre6->setText(QString());
        btnNbre7->setText(QString());
        btnNbre8->setText(QString());
        btnVerifier->setText(QApplication::translate("MainWindow", "V\303\251rifier", 0, QApplication::UnicodeUTF8));
        cBoxNiveau->clear();
        cBoxNiveau->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Facile", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Moyen", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Difficile", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Tr\303\250s difficile", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Diabolique", 0, QApplication::UnicodeUTF8)
        );
        btnAide->setText(QApplication::translate("MainWindow", "Aide", 0, QApplication::UnicodeUTF8));
        lblRegle->setText(QApplication::translate("MainWindow", "Compl\303\250te la grille de gauche avec\n"
"les nombres de la grille de droite\n"
"de sorte que les sommes :\n"
"    - horizontales\n"
"    - verticales\n"
"soient justes.", 0, QApplication::UnicodeUTF8));
        cBoxSuite->clear();
        cBoxSuite->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Nombres de 1 \303\240 9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Nombres al\303\251atoires de 0 \303\240 34", 0, QApplication::UnicodeUTF8)
        );
        btnCorrige->setText(QApplication::translate("MainWindow", "Corrig\303\251", 0, QApplication::UnicodeUTF8));
        btnNouveau->setText(QApplication::translate("MainWindow", "Nouveau", 0, QApplication::UnicodeUTF8));
        menuFichier->setTitle(QApplication::translate("MainWindow", "Fichier", 0, QApplication::UnicodeUTF8));
        menuAide->setTitle(QApplication::translate("MainWindow", "Aide", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
