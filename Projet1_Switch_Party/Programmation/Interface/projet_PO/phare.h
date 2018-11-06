#ifndef PHARE_H
#define PHARE_H

/**
 * \file phare.h
 * \brief Fichier de déclaration de la classe Phare.
 * \author Sébastien Angibaud
 */

#include <QLabel>

/**
 \brief Classe décrivant un phare.
 \author Sébastien Angibaud
*/
class Phare
{
    public:
        Phare(QLabel *image, int duree_allume, int duree_eteint );

        void executerUneSeconde();
        void demarrer();

    private:
        void allumer();
        void eteindre();

    private:
        /** \brief Pointeur sur l'image représentant le phare.*/
        QLabel * m_image;

        /** \brief La durée durant laquelle le phare reste allumé, en secondes.*/
        int m_duree_allume;

        /** \brief La durée durant laquelle le phare reste eteint, en secondes.*/
        int m_duree_eteint;

        /** \brief La temps restant pour l'état actuel.*/
        int m_temps_restant;

        /** \brief Indique si le phare est actuellement allumé.*/
        bool m_est_allume;
};

#endif // PHARE_H
