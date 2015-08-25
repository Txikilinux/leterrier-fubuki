/** Réimplémentation de la classe QWidget afin de lui donner une image de fond
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/LeTerrierExercice
  * @author 2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  *
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

#ifndef ABULEDUPIXMAPWIDGETV1_H
#define ABULEDUPIXMAPWIDGETV1_H

#include <QWidget>
#include <QFile>
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>

class AbulEduPixmapWidgetV1 : public QWidget
{
public:
    ///
    /// \brief Constructeur sans chemin d'image. Appelé par les constructions par promotion dans les interfaces graphiques.
    /// \param parent
    ///
    explicit AbulEduPixmapWidgetV1(QWidget *parent = 0);

    ///
    /// \brief Constructeur permettant de construire directement un widget avec son image de fond.
    /// \param picturePath
    /// \param parent
    ///
    explicit AbulEduPixmapWidgetV1(QString picturePath, QColor background = QColor(0,0,0,0),QWidget *parent = 0);

    ///
    /// \brief Donne le chemin d'une image de fond au widget
    /// \param picturePath est le chemin de l'image. Elle sera retaillée avec l'option KeepAspectRatioByExpanding, c'est à dire non déformée mais coupée à droite ou en bas si le ratio du widget
    /// est différent de celui de l'image
    /// \return true si l'image existe
    ///
    bool abePixmapWidgetSetPixmap(const QString& picturePath, QColor background = QColor(0,0,0,0));

    inline QSizeF abePixmapWidgetGetPixmapSize() const {return m_pixmapSize;}

    Qt::Alignment abePixmapWidgetGetAlignment() const;
    void abePixmapWidgetSetAlignment(const Qt::Alignment &alignment);

protected:
    ///
    /// \brief Réimplémentation de paintEvent qui dessine l'image de fond
    /// \param e l'événement
    ///
    void paintEvent(QPaintEvent *e);

private:
    ///
    /// \brief m_picturePath stocke le chemin de l'image de fond du widget
    ///
    QString m_picturePath;

    ///
    /// \brief m_backgroundColor la couleur de remplissage autour de l'image
    ///
    QColor m_backgroundColor;

    ///
    /// \brief m_pixmapRect la place (position et taille) occupée par l'image dans le widget
    ///
    QSizeF m_pixmapSize;

    Qt::Alignment m_alignment;
};

#endif // ABULEDUPIXMAPWIDGETV1_H
