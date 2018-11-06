/**
 * \file phare.cpp
 * \brief Fichier d'implémentation de la classe Phare.
 * \author Sébastien Angibaud
 */

#include "phare.h"

#include <iostream>

/** --------------------------------------------------------------------------------------
    \brief Constructeur.
    \param image Un pointeur sur le widget affichant la lampe du phare.
    \param duree_allume La durée durant laquelle le phare reste allumé, en secondes.
    \param duree_eteint La durée durant laquelle le phare reste eteint, en secondes.
*/
Phare::Phare(QLabel *image, int duree_allume, int duree_eteint )
{
    std::cout << "Creation d'un phare ( allume " << duree_allume << "s et eteint " << duree_eteint << "s)" << std::endl;
    m_image = image;

    m_duree_allume = duree_allume;
    m_duree_eteint = duree_eteint;

    demarrer();
}

/** --------------------------------------------------------------------------------------
    \brief Effectue une seconde.
*/
void Phare::executerUneSeconde()
{
    m_temps_restant = m_temps_restant - 1;
    std::cout << m_temps_restant << std::endl;

    if ( m_temps_restant == 0 )
    {
        if ( m_est_allume )
            eteindre();
        else
            allumer();
    }
}

/** --------------------------------------------------------------------------------------
    \brief Initialise le phare pour démarrer la simulation.
*/
void Phare::demarrer()
{
    allumer();
}

/** --------------------------------------------------------------------------------------
    \brief Allume le phare.
*/
void Phare::allumer()
{
    std::cout << "Allumer" << std::endl;

    m_temps_restant = m_duree_allume;
    m_est_allume = true;

    QPixmap pixmap = QPixmap( "images/phare_allume.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}

/** --------------------------------------------------------------------------------------
    \brief Eteint le phare.
*/
void Phare::eteindre()
{
    std::cout << "Eteindre" << std::endl;

    m_temps_restant = m_duree_eteint;
    m_est_allume = false;

    QPixmap pixmap = QPixmap( "images/phare_eteint.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}
