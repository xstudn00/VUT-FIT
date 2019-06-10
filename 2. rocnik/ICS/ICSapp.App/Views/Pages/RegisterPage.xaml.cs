using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace ICSapp.App.Views.Pages
{
    /// <summary>
    /// Interakční logika pro RegisterPage.xaml
    /// </summary>
    public partial class RegisterPage : Page
    {
        public RegisterPage()
        {
            InitializeComponent();
        }

        private void Cancel(object sender, RoutedEventArgs e)
        {
            NavigationService ns = NavigationService.GetNavigationService(this);
            ns.Navigate(new Uri("/Views/Pages/LoginPage.xaml", UriKind.Relative));
        }
    }
}
