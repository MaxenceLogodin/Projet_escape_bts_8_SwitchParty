using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.IO.Ports;

namespace WpfApp1
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public void lireMessage()
        {
            String Message = tbMessage.Text;
        }

        private void btEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            Initialiser_Port();
        }
        private string calculerCheckSum(string message)
        {
            return calculerCheckSum(Encoding.Default.GetBytes(message));
        }


        private string calculerCheckSum(byte[] messageBytes)
        {
            /* calcul du checksum */

            byte checksum = 0;

            int i = 0;

            for (i = 0; i < messageBytes.Length; i++)
            {
                checksum ^= messageBytes[i];
            }

            // conversion du checksum en chaine Hexa.

            string checksumHex = checksum.ToString("X2");

            /* FIN - calcul du checksum */

            return checksumHex;
        }



        public void Initialiser_Port()
        {
            /*SerialPort mySerialPort = new SerialPort("COM1", 9600, Parity.None, 8);
            string pre_message = "<L1><PA><FE><MB><WC><FE>";
            string message_afficheur = "Salut";

            string calcul = pre_message + message_afficheur;
            string checksum = calculerCheckSum(calcul);

            mySerialPort.Open();

            string trame = "<ID01>" + pre_message + message_afficheur + checksum + "<E>";
            mySerialPort.Write(trame);
            Console.WriteLine();
            mySerialPort.Close();
            */
            do
            {
                for (int i = 1; i <= 24; i++)
                {
                    SerialPort mySerialPort = new SerialPort("COM1", 9600, Parity.None, 8);
                    string pre_message = "<L1><PA><FE><MA><WB><FE>";

                    string message_afficheur = i.ToString();

                    string couleur_message;

                    if (i == 1 || i == 4 || i == 7 || i == 8 || i == 9 || i == 10 || i == 12 || i == 15 || i == 17 || i == 18 || i == 20 || i == 21 || i == 22 || i == 24)
                    {
                        couleur_message = "<CB>"; //rouge
                    }
                    else
                    {
                        couleur_message = "<CE>"; //vert
                    }


                    string calcul = pre_message + couleur_message + message_afficheur;
                    string checksum = calculerCheckSum(calcul);

                    mySerialPort.Open();

                    System.Threading.Thread.Sleep(3800);

                    string trame = "<ID01>" + pre_message + couleur_message + message_afficheur + checksum + "<E>";
                    mySerialPort.Write(trame);
                    Console.WriteLine();
                    mySerialPort.Close();

                }
            }
            while (true);
        }

    }
}
