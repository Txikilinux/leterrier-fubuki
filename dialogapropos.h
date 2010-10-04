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
