#include <QCoreApplication>
#include <time.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int result = 1;

    while (result == 1) {
        result = system("ping 10.16.2.81");
        if(result == 1)
        {
            printf(" \n \n les branchements ne sont pas bons \n \n");
            sleep(5);

        }

    }

        printf(" \n \n Felicitation vous avez bien tout reconnecte \n Pour vous recompenser de vos effort voici le code a envoyer : 7 \n \n");







    return a.exec();
}
