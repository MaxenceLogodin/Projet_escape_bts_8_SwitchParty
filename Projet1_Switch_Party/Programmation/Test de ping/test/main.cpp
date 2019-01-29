#include <QCoreApplication>
#include <time.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int result = 1;

    while (result == 1) {
        result = system("ping 192.168.1.1");
        if(result == 1)
        {
            printf(" \n \n LES BRANCHEMENTS NE SONT PAS BONS. PEUT-ETRE DEVRIEZ-VOUS REVOIR LA CONFIGURATION DES BRANCHEMENTS ? \n \n");
            sleep(5);

        }

    }

        printf(" \n \n TOUT REFONCTIONNE ! MERCI POUR TON AIDE, JE VAIS ENFIN POUVOIR CONTINUER MON TRAVAIL D'ASSISTANT SOCIAL POUR ORDINATEUR.\n\n POUR TE RECOMPENSER TOI ET TES AMIS (SI TU EN AS) DE TES EFFORTS, VOICI LE CHIFFRE CLE A ENVOYER POUR ACCEDER A DE FABULEUX TRESORS : 8 \n \n");







    return a.exec();
}
