/**
 * \file mainwindow.cpp
 * \brief Fichier d'implémentation de la classe MainWindow.
 * \author Sébastien Angibaud
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>

/** --------------------------------------------------------------------------------------
    \brief Constructeur de la classe MainWindow.
    \param parent Un pointeur sur le QWidget parent.
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_phare = NULL;

    setWindowTitle("Simulation de phare");

    // instanciation et initialisation du timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onFinTimer()));

    // création de la barre d'outils
    creerToolbar();

    // création du widget permettant l'affichage du phare
    m_image_phare = new QLabel(this);
    m_image_phare->resize(QSize(160,320));
    m_image_phare->move( QPoint(120,150) );

    // taille par défaut de la fenêtre non redimensionnable
    setFixedSize(QSize(400,600));
}

/** --------------------------------------------------------------------------------------
    \brief Destructeur de la classe MainWindow.
*/
MainWindow::~MainWindow()
{
    delete ui;
}


/** --------------------------------------------------------------------------------------
 \brief Création de la barre d'outils.
*/
void MainWindow::creerToolbar()
{
    QStyle* style = QApplication::style();

    m_toolbar_bouton_charger_phare= new QPushButton();
    ui->mainToolBar->addWidget(m_toolbar_bouton_charger_phare);

    m_toolbar_bouton_demarrer_simulation= new QPushButton();
    ui->mainToolBar->addWidget(m_toolbar_bouton_demarrer_simulation);

    m_toolbar_bouton_arreter_simulation= new QPushButton();
    ui->mainToolBar->addWidget(m_toolbar_bouton_arreter_simulation);

    m_toolbar_bouton_charger_phare->setIcon(style->standardIcon( QStyle::SP_DialogOpenButton ));
    m_toolbar_bouton_charger_phare->setText("Charger un phare");
    connect( m_toolbar_bouton_charger_phare, SIGNAL(released()), this, SLOT(onOuvrirPhareClick()));

    m_toolbar_bouton_demarrer_simulation->setIcon(style->standardIcon( QStyle::SP_MediaPlay ));
    m_toolbar_bouton_demarrer_simulation->setText("Démarrer la simulation");
    connect( m_toolbar_bouton_demarrer_simulation, SIGNAL(released()), this, SLOT(onDemarrerSimulationClick()));

    m_toolbar_bouton_arreter_simulation->setIcon(style->standardIcon( QStyle::SP_MediaStop ));
    m_toolbar_bouton_arreter_simulation->setText("Arréter la simulation");
    connect( m_toolbar_bouton_arreter_simulation, SIGNAL(released()), this, SLOT(onArreterSimulationClick()));

    m_toolbar_bouton_demarrer_simulation->setEnabled(false);
    m_toolbar_bouton_arreter_simulation->setEnabled(false);
}

/** --------------------------------------------------------------------------------------
    \brief Méthode appelée lors d'une demande d'ouverture d'un phare.
*/
void MainWindow::onOuvrirPhareClick()
{
    ouvrirPhare();
    m_toolbar_bouton_demarrer_simulation->setEnabled(true);
    m_toolbar_bouton_arreter_simulation->setEnabled(false);
}

/** --------------------------------------------------------------------------------------
    \brief Méthode appelée lors d'une demande de démarrage de la simulation.
*/
void MainWindow::onDemarrerSimulationClick()
{
    m_phare->demarrer();
    m_timer->start(1000);
    m_toolbar_bouton_charger_phare->setEnabled(false);
    m_toolbar_bouton_demarrer_simulation->setEnabled(false);
    m_toolbar_bouton_arreter_simulation->setEnabled(true);
}

/** --------------------------------------------------------------------------------------
    \brief Méthode appelée lors d'une demande d'arrêt de la simulation.
*/
void MainWindow::onArreterSimulationClick()
{
    m_timer->stop();

    m_toolbar_bouton_demarrer_simulation->setEnabled(true);
    m_toolbar_bouton_arreter_simulation->setEnabled(false);
    m_toolbar_bouton_charger_phare->setEnabled(true);
}

/** --------------------------------------------------------------------------------------
    \brief Méthode appelée lorsque le timer est terminé.
*/
void MainWindow::onFinTimer()
{
    m_phare->executerUneSeconde();
    update();
    m_timer->start(1000);
}

/** --------------------------------------------------------------------------------------
    \brief Ouvre un fichier pour charger un phare.
*/
void MainWindow::ouvrirPhare()
{
    QString nom_fichier =
            QFileDialog::getOpenFileName( this, tr("Ouvrir un phare"),
                                          "mes_phares", tr("phare (*.pha);;"));

    if (nom_fichier.isEmpty())
        return;

    QFile file(nom_fichier);

    if (! file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Impossible d'ouvrir le fichier."), file.errorString());
    }
    else
    {
        QTextStream flux(&file);

        QString ligne;
        while(! flux.atEnd())
        {
            ligne = flux.readLine();

            QStringList list = ligne.split(" ");
            QString commande = list.at(0);
            list.removeFirst();

            if ( commande == "PHARE" && list.size() == 2 )
            {
                // on controle si les temps sont bien des entiers
                bool conversion_ok;
                int temps_allume = list.at(0).toInt(& conversion_ok);
                int temps_eteint;

                if ( conversion_ok )
                    temps_eteint = list.at(1).toInt(& conversion_ok);
                if ( conversion_ok )
                    creerPhare( temps_allume, temps_eteint);
            }
        }
    }
}

/** --------------------------------------------------------------------------------------
    \brief Crée le phare.
    \param temps_allume La durée durant laquelle le phare reste allumé, en secondes.
    \param temps_eteint La durée durant laquelle le phare reste eteint, en secondes.
*/
void MainWindow::creerPhare( int temps_allume, int temps_eteint )
{
    // On détruit l'ancien phare s'il existe
    if ( m_phare != NULL )
        delete m_phare;

    // création du phare
    m_phare = new Phare( m_image_phare, temps_allume, temps_eteint );
}
