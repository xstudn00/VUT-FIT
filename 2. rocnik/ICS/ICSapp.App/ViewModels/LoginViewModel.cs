using ICSapp.App.Commands;
using ICSapp.BL.Messages;
using ICSapp.BL.Models;
using ICSapp.BL.Repositories;
using ICSapp.BL.Services;
using System;
using System.Windows;
using System.Windows.Input;

namespace ICSapp.App.ViewModels
{
    public class LoginViewModel : BaseViewModel
    {

        private readonly IUserRepository repository;
        public UserModel User { get; set; }
        public ICommand RegisterUserCommand { get; set; }
        public ICommand LoginUserCommand { get; set; }
        public ICommand OpenWindowCommand { get; set; }
        private readonly IMediator mediator;
        public RelayCommand<Window> CloseWindowCommand { get; set; }
        public string LoginText { get; set; }

        MainWindow window = new MainWindow();

        public bool login;

        public LoginViewModel(IUserRepository repository, IMediator mediator)
        {
            this.repository = repository;
            this.mediator = mediator;
            User = new UserModel();

            RegisterUserCommand = new RelayCommand(RegisterUser);
            LoginUserCommand = new RelayCommand(TryLogin);
            OpenWindowCommand = new RelayCommand<string>(OpenWindow);
            CloseWindowCommand = new RelayCommand<Window>(CloseWindow);
        }

        public bool DoesUserExist()
        {
            UserModel ExistingUser = repository.GetByEmailAdress(this.User.EmailAdress);
            if (ExistingUser == null) return false;
            return true;
        }
        private void RegisterUser()
        {
            User.Password = BCrypt.Net.BCrypt.HashPassword(User.Password);
            User.Description = "";
            User.Picture = null;

            if (ValidateForm())
            {
                CreateUser(this.User);
                LoginText = "Account successfully created";
            }
        }

        private bool ValidateForm()
        {
            if (string.IsNullOrEmpty(this.User.Password) ||
                string.IsNullOrEmpty(this.User.EmailAdress) ||
                string.IsNullOrEmpty(this.User.FirstName) ||
                string.IsNullOrEmpty(this.User.Surname))
            {
                LoginText = "Fields must not be empty";
                return false;
            }
            if (DoesUserExist())
            {
                LoginText = "This E-mail is already taken";
                return false;
            }
            return true;
        }

        private void TryLogin()
        {
            
            if (string.IsNullOrEmpty(this.User.Password) ||
                string.IsNullOrEmpty(this.User.EmailAdress))
            {
                this.LoginText = "Fields must not be empty";
            }
            else if (DoesUserExist())
            {
                LogIn();
                if (this.login == false)
                {
                    this.LoginText = "Wrong password";
                }
                else
                {
                    LoadUserByEmail(this.User.EmailAdress);
                    this.OpenWindowCommand.Execute("MainWindow");
                    mediator.Send(new ActiveUserMessage { Id = User.Id });
                    this.CloseWindowCommand.Execute(GetWindow("Starting"));
                }
            }
            else
            {
                this.LoginText = "Account doesnt exist";
            }
        }

        public Window GetWindow(string Name)
        {
            Window windowObject = null;
            foreach (Window window in System.Windows.Application.Current.Windows)
            {
                if (window.Name == Name) windowObject = window;
            }
            return windowObject;
        }
        private void OpenWindow(string WName)
        {
            window.Show();
        }

        private void CloseWindow(Window w)
        {
            if (w!= null) w.Close();
        }

        public void LoadUserByEmail(String EmailAddress)
        {
            User = repository.GetByEmailAdress(EmailAddress);
        }

        public void CreateUser(UserModel User)
        {
            repository.CreateUser(User);
        }
        private void LogIn()
        {
            login = repository.LogIn(User.EmailAdress, User.Password);
        }


    }
}
