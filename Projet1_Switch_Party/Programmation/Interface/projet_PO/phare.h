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
        Phare(QLabel *image, int duree_allume, int duree_eteint, int duree_image2, int duree_image3, int duree_image4);

        void executerUneSeconde();
        void demarrer();

    private:
        void allumer();
        void defile();
        void defile2();
        void defile3();
        void defile4();

    private:
        /** \brief Pointeur sur l'image représentant le phare.*/
        QLabel * m_image;

        /** \brief La durée durant laquelle l'image reste noir, en secondes.*/
        int m_duree_allume;

        /** \brief La durée durant laquelle le texte reste sur chaque image pour que l'utilisatuer puisse lire, en secondes.*/
        int m_duree_1;
        int m_duree_2;
        int m_duree_3;
        int m_duree_4;

        /** \brief La temps restant pour l'état actuel.*/
        int m_temps_restant;

        /** \brief Indique si le phare est actuellement allumé.*/
        int m_est_noir = 1;
};

#endif // PHARE_H
