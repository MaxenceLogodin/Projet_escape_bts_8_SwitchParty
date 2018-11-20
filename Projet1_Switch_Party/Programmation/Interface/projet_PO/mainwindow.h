#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * \file mainwindow.h
 * \brief Fichier de déclaration de la classe MainWindow.
 * \author Sébastien Angibaud
 */


#include <QMainWindow>
#include <QPushButton>
#include <QList>
#include <QLabel>
#include <QTimer>
#include <QPoint>

#include "phare.h"

namespace Ui {
    class MainWindow;
}


/**
 * \class MainWindow
 * \brief Classe décrivant la fenetre principale de l'application.
 * \author Sébastien Angibaud
 */
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        void creerToolbar();
        void ouvrirPhare();
        void creerPhare( int temps_allume, int temps_eteint, int temps_image2, int temps_image3);

    private slots:
        void onOuvrirPhareClick();
        void onDemarrerSimulationClick();
        void onArreterSimulationClick();
        void onFinTimer();


    private:
        /** \brief Le pointeur sur la fenetre .*/
        Ui::MainWindow *ui;

        /** \brief Le phare à visualiser.*/
        Phare * m_phare;

        /** \brief Un pointeur sur le widget affichant le phare.*/
        QLabel * m_image_phare;

        /** \brief Un pointeur sur le chronomètre.*/
        QTimer* m_timer;

        /** \brief Un pointeur sur le bouton de la barre d'outil pour charger un phare.*/
        QPushButton *m_toolbar_bouton_charger_phare;

        /** \brief Un pointeur sur le  bouton de la barre d'outil pour démarrer la simulation.*/
        QPushButton *m_toolbar_bouton_demarrer_simulation;

        /** \brief Un pointeur sur le bouton de la barre d'outil pour arréter la simulation.*/
        QPushButton *m_toolbar_bouton_arreter_simulation;
};

#endif // MAINWINDOW_H
