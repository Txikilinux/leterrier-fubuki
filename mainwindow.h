#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPushButton>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
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
    */
    int niveau;
    int alea;

    /**
      fontes utilisées
    */
    QFont fontBIG; // 30
    QFont fontMEDIUM; // 20
    QFont fontMINUS; //12

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
    void setAide();

signals:
    /** Demande le positionnement de l'onglet de l'aide
      * @param onglet : le numéro de l'onglet
      */
    void setindex(int onglet);

private slots:
    void on_btnNouveau_clicked();
    void on_btnAide_clicked();
    void on_actionAide_en_local_triggered();
    void on_action_propos_triggered();
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
