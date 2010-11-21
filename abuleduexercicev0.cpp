/**
  * Classe de base en version zéro pour les exercices AbulEdu permettant de structurer
  * nos logiciels et respecter ainsi un certain nombre de choses dont l'interface publique
  * à laquelle le plugin peut se connecter pour échanger des données (logs pour le moment)
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Normalisation_des_%C3%A9changes_entre_l%27application_et_le_plugin
  * @author 2009-2010 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  *
  * @warning: NE MODIFIEZ SURTOUT PAS CES FICHIERS, SI VOUS LE FAITES NE SOYEZ PAS ÉTONNÉS
  *           QUE L'APPLICATION NE S'INTERFACE PLUS CORRECTEMENT AVEC LE PLUGIN !!!!!
  *
  */
/*
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

#include "abuleduexercicev0.h"

/** constructeur ... */
AbulEduExerciceV0::AbulEduExerciceV0(QWidget *parent) :
    QMainWindow(parent), sharedMemory("abeUpdateRequest")

{  
    //Initialisation des différentes variables
    m_exerciceName     = "";
    m_level            = "";
    m_nbTotalQuestions = 0;
    m_skill            = "";
    m_localDebug       = 0;
    m_arrayLogs.clear();
    m_downloadFilter.clear();
    int already_requested = 0;

    // ---------------------------------- memoire partagee et requete reseau update
    if (sharedMemory.isAttached())
        detach();
    //creation du segment de memoire partagee
    if(sharedMemory.create(sizeof(already_requested))) {
        if(m_localDebug)
            qDebug() << "abuleduexercicesv0.cpp creation de la zone memoire partagee, taille" << sharedMemory.size();

        // ----------------------------------
        //On cherche pour voir s'il y a une mise a jour de l'application de disponible
        //On lance notre requete reseau et on gere la suite
        QString appCodeName(qApp->applicationName()); //idee en cas de blablabla % codename .midRef(qApp->applicationName().lastIndexOf("%")+2).toString());
        QString os("");
    #if defined(Q_OS_MAC)
        os="osx";
    #elif defined(Q_OS_WIN32)
        os="windows";
    #elif defined(Q_OS_LINUX)
        os="linux";
    #endif
        QUrl urlUpdates("http://updates.ryxeo.com/application/" + appCodeName + "/version/" + qApp->applicationVersion() + "/os/" + os + "/format/xml");
        QNetworkAccessManager* nam = new QNetworkAccessManager(this);
        connect(nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(onlineUpdateRequestSlot(QNetworkReply*)));
        QNetworkReply* reply = nam->get(QNetworkRequest(urlUpdates));
        // ----------------------------------

        // on écrit le flag comme quoi la requete reseau est deja partie ... merci de ne pas relancer cette requete :)
        sharedMemory.lock();
        already_requested = 1;
        if(m_localDebug)
            qDebug() << "abuleduexercicesv0.cpp ecriture dans la memoire partagee: requete reseau lancee";
        memcpy(sharedMemory.data(), &already_requested, sizeof(already_requested));
        sharedMemory.unlock();
    }
    else {
        if(m_localDebug)
            qDebug() << "abuleduexercicesv0.cpp zone memoire partagee deja existante, la requete reseau a deja ete lancee";
        // si le segment existe deja on l'utilise
        if(sharedMemory.error() == QSharedMemory::AlreadyExists) {
            if(sharedMemory.attach()) {
                // lecture des donnees
                sharedMemory.lock();
                already_requested = *(bool *)sharedMemory.constData();
                sharedMemory.unlock();
            }
            else {
                if(m_localDebug)
                    qDebug() << "abuleduexercicesv0.cpp erreur sur sharedMemory :: " << sharedMemory.errorString();
            }
        }
        else {
            if(m_localDebug)
                qDebug() << "abuleduexercicesv0.cpp erreur sur sharedMemory :: " << sharedMemory.errorString();
        }
    }
    // ---------------------------------- fin de gestion memoire partagee et requete reseau update

    // ---------------------------------- chargement plugin
    //Ensuite on charge les plugins qui se trouvent dans le sous dossier plugins de l'application
    QDir pluginsDir(qApp->applicationDirPath());
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#if defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    if(pluginsDir.cd("plugins")) {
        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
            if(m_localDebug)
                qDebug() << "Chargement du plugin " << fileName;
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            if(m_localDebug)
                qDebug() << "Chargement du plugin (2)" << plugin;
            if (plugin && m_localDebug) {
                qDebug() << "Chargement du plugin " << fileName << " réussi";
            }
        }
    }
    // ---------------------------------- Fin du chargement plugin

}

AbulEduExerciceV0::~AbulEduExerciceV0()
{
    //On detache la memoire partagee
    detach();
}

void AbulEduExerciceV0::setAbeLineLog(QString question, QString answer,
                                      int score, int nbPrintedQuestions, QString evaluation,
                                      QString expected, QString answerTime, QString answerIntermediate, QString answerProposed)
{
    //Construction de la structure de données
    QHash <QString, QString> line;
    line.insert("exerciceName", m_exerciceName);
    line.insert("level", m_level);
    line.insert("score", QString::number(score));
    line.insert("nbTotalQuestions", QString::number(m_nbTotalQuestions));
    line.insert("nbPrintedQuestions", QString::number(nbPrintedQuestions));
    line.insert("question", question);
    line.insert("answer", answer);
    line.insert("evaluation", evaluation);
    if(expected != "")
        line.insert("expected", expected);
    if(m_skill != "")
        line.insert("skill", m_skill);
    if(answerTime != "")
        line.insert("answerTime", answerTime);
    if(answerIntermediate != "")
        line.insert("answerIntermediate", answerIntermediate);
    if(answerProposed != "")
        line.insert("answerProposed", answerProposed);

    m_arrayLogs.insert(m_arrayLogs.count(),line);
}

void AbulEduExerciceV0::resetAbeLogs()
{
    m_arrayLogs.clear();
}

QHash<int, QHash<QString, QString> >  AbulEduExerciceV0::getPluginLogs(QString login, QString group)
{
    if(m_localDebug)
        qDebug() << "abuleduexercicesv0.cpp getPluginLogs " << m_arrayLogs;
    return m_arrayLogs;
}

void AbulEduExerciceV0::pushAbulEduLogs()
{
    QEvent* logEvent = new QEvent(AbulEduLogsPush);
    QApplication::postEvent(this, logEvent);
}


void AbulEduExerciceV0::setAbeExerciceName(QString s)
{
    m_exerciceName = s;
}

void AbulEduExerciceV0::setAbeLevel(QString s)
{
    m_level = s;
}

void AbulEduExerciceV0::setAbeNbTotalQuestions(int n)
{
    m_nbTotalQuestions = n;
}

void AbulEduExerciceV0::setAbeSkill(QString s)
{
    m_skill = s;
}

QString AbulEduExerciceV0::getAbeExerciceName()
{
    return m_exerciceName;
}

QString AbulEduExerciceV0::getAbeLevel()
{
    return m_level;
}

int AbulEduExerciceV0::getAbeNbTotalQuestions()
{
    return m_nbTotalQuestions;
}

QString AbulEduExerciceV0::getAbeSkill()
{
    return m_skill;
}

QHash<QString, QString> AbulEduExerciceV0::downloadPluginLogsFilter()
{
    return m_downloadFilter;
}

void AbulEduExerciceV0::setPluginLogs(QHash<int, QHash<QString, QString> > h)
{
    m_arrayLogs.clear();
    m_arrayLogs = h;
}

void AbulEduExerciceV0::downloadAbulEduLogs()
{
    QEvent* downloadEvent = new QEvent(AbulEduLogsDownload);
    QApplication::postEvent(this, downloadEvent);
}

void AbulEduExerciceV0::setAbeDownloadLogsFilter(QDate dateBegin, QDate dateEnd, QString login, QString group, QString evaluation, QString skill)
{
    m_downloadFilter.insert("login",login);
    m_downloadFilter.insert("group",group);
    m_downloadFilter.insert("dateBegin",dateBegin.toString("yyyy-MM-dd"));
    m_downloadFilter.insert("dateEnd",dateEnd.toString("yyyy-MM-dd"));
    m_downloadFilter.insert("evaluation",evaluation);
    m_downloadFilter.insert("skill",skill);
}

void AbulEduExerciceV0::setAbeDebugLevel(int i)
{
    m_localDebug = i;
}

void AbulEduExerciceV0::onlineUpdateRequestSlot(QNetworkReply *reply)
{
    // Pas d'erreur ?
    if (reply->error() == QNetworkReply::NoError)
    {
        QString titre("");
        QString desc("");
        QString link("");
        QString lang("");
        QDomDocument *xml = new QDomDocument();
        xml->setContent(reply->readAll());
        if(m_localDebug)
            qDebug() << "abuleduexercicesv0.cpp download XML update " << xml->toString();

        QDomElement xmlElem = xml->documentElement();
        QDomNode node = xmlElem.firstChild();
        while(!node.isNull())
        {
            QDomElement element = node.toElement(); // On essaie de convertir le node en element
            if(!element.isNull())
            {
                if(element.tagName()=="application") // Le node est bien un élément.
                {
                    QDomNode node=element.firstChild(); //On parcourt le document à partir du tag "application"
                    while(!node.isNull())
                    {
                        QDomElement elem=node.toElement(); //On cherche le titre du "Channel"
                        if(elem.tagName()=="title")
                        {
                            titre = elem.text();
                        }
                        else if((elem.tagName()=="link"))
                        {
                            link = elem.text();
                        }
                        else if((elem.tagName()=="description"))
                        {
                            desc = elem.text();
                        }
                        else if((elem.tagName()=="language"))
                        {
                            lang = elem.text();
                        }
                        node=node.nextSibling();
                    }
                }
                node = node.nextSibling();
            }
        }

        //On affiche une boite d'information a l'utilisateur ...
        if(titre.length())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(titre);
            msgBox.setText(desc);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }
}

void AbulEduExerciceV0::detach()
{
    if (!sharedMemory.detach())
        qDebug() << "Unable to detach from shared memory.";
}
