  /*

   Loquîî is a chat program who can help you to speak by writing to a friend or with someone else
    Copyright (C) 2012  Guillaume Villena Dylan Delaporte

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/

#include <QtGui/QApplication>
#include "fenoptions.h"
#include "apropos.h"
#include "fenprincipal.h"
#include "fenmacumba.h"
#include "fendem.h"
#include <QSplashScreen>
#include <QPixmap>
#include <QFtp>
#include <QFile>
#include <QIcon>
#include <QSettings>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("windows");

    QTranslator translator;

    app.installTranslator(&translator);

    QSettings options("pox", "loquii");

    if (options.value("choixdelalangue", "fr") == "fr")
    {
        std::cout << "langue francaise" << std::endl;
        QString locale = QLocale::system().name().section('_', 0, 0);
        QTranslator translator_bouton;
        translator_bouton.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&translator_bouton);
    }
    else if (options.value("choixdelalangue", "en") == "en")
    {
        std::cout << "langue anglaise" << std::endl;
        translator.load("loquii_en");
    }
    else if (options.value("choixdelalangue", "es") == "es")
    {
        std::cout << "langue espagnole" << std::endl;
        translator.load("loquii_es");
    }
    else
    {

    }

    FenPrincipal w;
    w.show();

    return app.exec();
}
