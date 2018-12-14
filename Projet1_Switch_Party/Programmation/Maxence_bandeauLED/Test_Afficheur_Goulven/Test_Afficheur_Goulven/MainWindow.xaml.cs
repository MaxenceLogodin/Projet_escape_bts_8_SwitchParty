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

namespace Test_Afficheur_Goulven
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
            SerialPort mySerialPort = new SerialPort("COM1", 9600, Parity.None, 8);
            string pre_message = "<L1><PA><FE><MB><WC><FE>";
            string message_afficheur = "Youpi ca marche";
            mySerialPort.Open();
            string calcul = pre_message + message_afficheur;
            string checksum = calculerCheckSum(calcul);

            string trame = "<ID01>" + pre_message + message_afficheur + checksum + " < E>";
            mySerialPort.WriteLine(trame);
            Console.WriteLine();
            mySerialPort.Close();
        }

    }
}
