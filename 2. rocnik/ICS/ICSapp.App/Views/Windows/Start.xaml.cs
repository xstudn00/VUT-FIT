using ICSapp.App.ViewModels;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ICSapp.App
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Start : Window
    {
        public Start()
        {
            InitializeComponent();
            FrameContent.Source = new Uri("/Views/Pages/LoginPage.xaml", UriKind.Relative);
        }
    }
}
