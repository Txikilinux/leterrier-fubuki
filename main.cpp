/**
  * Fubuki (le terrier d'AbulEdu)
  *
  * @warning aucun traitement d'erreur n'est pour l'instant implémenté
  * @see https://redmine.ryxeo.com/projects/
  * @author 2009-2010 Andre Connes <andre dot connes at wanadoo dot fr>
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#include <QtGui/QApplication>
#include <QtCore>
#include "mainwindow.h"
#include "version.h"
#include "abuleduapplicationv1.h"

int main(int argc, char *argv[])
{
    AbulEduApplicationV1 a(argc, argv,VER_INTERNALNAME_STR, VER_PRODUCTVERSION_STR, VER_COMPANYDOMAIN_STR, VER_COMPANYNAME_STR);
    a.setAbeApplicationLongName(QObject::trUtf8(VER_FILEDESCRIPTION_STR));

    MainWindow w;
#ifdef __ABULEDUTABLETTEV1__MODE__
    QApplication::setStyle("plastique");
    w.showFullScreen();
#else
    w.showMaximized();
#endif
    return a.exec();
}
