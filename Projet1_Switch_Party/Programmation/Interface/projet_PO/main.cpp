/**
 * \file main.cpp
 * \brief Fichier principal de l'application.
 * \author lucas Minaud
 */

#include "mainwindow.h"

#include <QApplication>

/** --------------------------------------------------------------------------------------
    \brief Fonction principale de l'application.
    \param argc Le nombre de paramètre de l'application.
    \param argv Le tableau des paramètres.
    \return Un entier indiquant le code erreur.
*/
int main(int argc, char *argv[])
{
    QFile File("stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    QApplication a(argc, argv);
    a.setStyleSheet(StyleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}
