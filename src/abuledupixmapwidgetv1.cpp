#include "abuledupixmapwidgetv1.h"

AbulEduPixmapWidgetV1::AbulEduPixmapWidgetV1(QWidget *parent) :
    QWidget(parent),
    m_picturePath(QString()),
    m_backgroundColor(QColor(0,0,0,0)),
    m_pixmapSize(QSizeF()),
    m_alignment(Qt::AlignHCenter)
{
}

AbulEduPixmapWidgetV1::AbulEduPixmapWidgetV1(QString picturePath, QColor background, QWidget *parent) :
    QWidget(parent),
    m_picturePath(picturePath),
    m_backgroundColor(background),
    m_pixmapSize(QSizeF()),
    m_alignment(Qt::AlignHCenter)
{

}

bool AbulEduPixmapWidgetV1::abePixmapWidgetSetPixmap(const QString &picturePath,QColor backgroundColor)
{
    m_backgroundColor = backgroundColor;
    if(!picturePath.isNull()){
        if(QFile(picturePath).exists()){
            m_picturePath = picturePath;
//            repaint();
        }
        else {
            return false;
        }
    }
    return true;
}

void AbulEduPixmapWidgetV1::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    if(!m_picturePath.isNull()){
        QPainter painter(this);
        painter.setBrush(QBrush(m_backgroundColor));
        float pixmapRatio = QPixmap(m_picturePath).width() / (float) QPixmap(m_picturePath).height();
        float widgetRatio = width() / (float) height();
        QPixmap scaledPixmap;
        bool force = false;
        if (pixmapRatio  > widgetRatio && force){
//            qDebug()<<"pour le widget "<<objectName();
//            qDebug()<<"j'étire jusqu'à une largeur max";
            scaledPixmap = QPixmap(m_picturePath).scaledToWidth(width(),Qt::SmoothTransformation);
        }
        else{
            scaledPixmap = QPixmap(m_picturePath).scaledToHeight(height(),Qt::SmoothTransformation);
//            qDebug()<<"pour le widget "<<objectName();
//            qDebug()<<"j'étire jusqu'à une hauteur max";
        }
        int leftGap = 0;
        if(m_alignment == Qt::AlignHCenter){
            leftGap = (width()-scaledPixmap.width())/2;
        }
        painter.drawPixmap(QPoint(leftGap,0),scaledPixmap);
        QRect decalages(1,1,2,2);
        QRect r(geometry().x() + decalages.x(),geometry().y() + decalages.y(),geometry().width() - decalages.width(),geometry().height() - decalages.height());
        painter.drawRect(r);
        m_pixmapSize = QSizeF(scaledPixmap.width(),scaledPixmap.height());
    }
    QWidget::paintEvent(e);
}

Qt::Alignment AbulEduPixmapWidgetV1::abePixmapWidgetGetAlignment() const
{
    return m_alignment;
}

void AbulEduPixmapWidgetV1::abePixmapWidgetSetAlignment(const Qt::Alignment &alignment)
{
    m_alignment = alignment;
}

