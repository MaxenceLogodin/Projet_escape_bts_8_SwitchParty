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
Phare::Phare(QLabel *image, int duree_allume, int duree_eteint, int duree_image2, int duree_image3)
{
    std::cout << "Creation d'un phare ( allume " << duree_allume << "s et eteint " << duree_eteint << "s)" << duree_image2 << std::endl;
    m_image = image;

    m_duree_allume = duree_allume;
    m_duree_1 = duree_eteint;
    m_duree_2 = duree_image2;
    m_duree_3 = duree_image3;

    demarrer();
}

/** --------------------------------------------------------------------------------------
    \brief Effectue une seconde.
*/
void Phare::executerUneSeconde()
{
    std::string n;
    m_temps_restant = m_temps_restant - 1;
    std::cout << m_temps_restant << std::endl;

    if ( m_temps_restant == 0 )
    {
        if ( m_est_noir == 1 ){
            defile();
            m_est_noir = m_est_noir + 1;
        }
        else if( m_est_noir == 2){
            defile2();
            m_est_noir = m_est_noir + 1;
        }
        else if( m_est_noir == 3){
            defile3();
            m_est_noir = m_est_noir + 1;
        }
        else if( m_est_noir == 4){
            defile4();
            }
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
    m_est_noir = true;

    QPixmap pixmap = QPixmap( "images/texte0.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}

/** --------------------------------------------------------------------------------------
    \brief Eteint le phare.
*/
void Phare::defile()
{
    std::cout << "image1" << std::endl;

    m_temps_restant = m_duree_1;
    m_est_noir = 1;


    QPixmap pixmap = QPixmap( "images/texte1.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}
void Phare::defile2()
{
    std::cout << "image2" << std::endl;

    m_temps_restant = m_duree_2;
    m_est_noir = 2;

    QPixmap pixmap = QPixmap( "images/texte4.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}
void Phare::defile3()
{
    std::cout << "image3" << std::endl;

    m_temps_restant = m_duree_3;
    m_est_noir = 3;

    QPixmap pixmap = QPixmap( "images/texte5.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}
void Phare::defile4()
{
    std::cout << "image4" << std::endl;

    m_est_noir = 4;

    QPixmap pixmap = QPixmap( "images/texte6.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}
void Phare::defile5()
{
    std::cout << "image5" << std::endl;

    m_est_noir = 5;

    QPixmap pixmap = QPixmap( "images/texte2.png" );
    m_image->setPixmap( pixmap.scaled(m_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    m_image->show();
}
