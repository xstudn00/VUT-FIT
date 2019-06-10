using ICSapp.App.Commands;
using ICSapp.App.Views;
using ICSapp.BL.Models;
using ICSapp.BL.Repositories;
using ICSapp.BL.Services;
using ICSapp.BL.Messages;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using System.Windows.Navigation;
using System.Windows.Controls;
using ICSapp.App.Views.Windows;

namespace ICSapp.App.ViewModels
{
    public class UserViewModel : BaseViewModel
    {
        private readonly IUserRepository repository;
        private readonly ITeamRepository teamRepository;

        public UserModel User { get; set; }
        public IEnumerable<UsersListModel> UsersList { get; set; }
        public List<TeamModel> TeamsList { get; set; }
        public TeamModel Team { get; set; }
        public ICommand OpenSettingsCommand { get; set; }
        public ICommand EditUserInformationCommand { get; set; }
        public ICommand OpenTeamsCommand { get; set; }
        public ICommand CreateTeamCommand { get; set; }
        public ICommand LogOutCommand { get; set; }

        public string CreateTeamInfo { get; set; }
        public string EditName { get; set; }
        public string EditSurname { get; set; }
        public string EditDescription { get; set; }
        public string EditPassword { get; set; }
        public string EditText { get; set; }

        private readonly IMediator mediator;
        
        public UserViewModel(IUserRepository repository, ITeamRepository teamRepository, IMediator mediator)
        {
            this.repository = repository;
            this.teamRepository = teamRepository;
            this.mediator = mediator;

            User = new UserModel();
            Team = new TeamModel();

            OpenSettingsCommand = new RelayCommand(OpenSettings);
            EditUserInformationCommand = new RelayCommand(EditUserInformation);
            OpenTeamsCommand = new RelayCommand(OpenTeams);
            CreateTeamCommand = new RelayCommand(CreateNewTeam);
            LogOutCommand = new RelayCommand(OpenLogIn);


            mediator.Register<ActiveUserMessage>(UserLoggedIn);
        }

        private void EditUserInformation()
        {
            EditText = "";
            if (string.IsNullOrEmpty(EditName) ||
                string.IsNullOrEmpty(EditSurname) ||
                string.IsNullOrEmpty(EditPassword)){
                EditText = "Fill boxes";
            }
            else
            {
                User.FirstName = EditName;
                User.Surname = EditSurname;
                User.Description = EditDescription;
                User.Password = BCrypt.Net.BCrypt.HashPassword(EditPassword);

                UpdateUser();
                mediator.Send(new ActiveUserMessage { Id = User.Id });
                EditText = "Information updated successfully";
            }
        }

        private void CreateNewTeam(object obj)
        {
            if (string.IsNullOrEmpty(this.Team.TeamName))
            {
                CreateTeamInfo = "Field must not be empty";
            }
            else
            {
                CreateTeam();
                AddPersonToTeam();
                CreateTeamInfo = "Team successfully created";
                mediator.Send(new ActiveUserMessage { Id = User.Id });
            }
        }

        private void UserLoggedIn(ActiveUserMessage obj)
        {
            LoadTeamsByUserId(obj.Id);
            LoadUserById(obj.Id);
        }
        public void LoadTeamsByUserId(Guid id)
        {
            TeamsList = repository.GetTeamsByUser(id);
        }
        public void LoadAllUsers()
        {
            UsersList = repository.GetAll().ToList();
        }
        public void LoadUserById(Guid id)
        {
            User = repository.GetById(id);
        }

        public void LoadUserByEmail(String EmailAddress)
        {
            User = repository.GetByEmailAdress(EmailAddress);
        }
        public void UpdateUser()
        {
            repository.UpdateUser(User);
        }
        public void AddPersonToTeam()
        {
            Team = teamRepository.AddTeamMate(Team.Id, User.Id);
        }
        public void RemoveUser()
        {
            repository.RemoveUser(User.Id);
        }
        public void CreateTeam()
        {
            Team = repository.CreateTeam(Team);
        }
        public void LogOut(Guid Id)
        {
            repository.LogOut(Id);
        }

        private void OpenSettings(object obj)
        {
            SettingsWindow SettingsWindow = new SettingsWindow();
            SettingsWindow.Show();

            mediator.Send(new ActiveUserMessage { Id = User.Id });
        }
        private void OpenTeams(object obj)
        {
            TeamsWindow TeamsWindow = new TeamsWindow();
            TeamsWindow.Show();

            mediator.Send(new ActiveUserMessage { Id = User.Id });
        }

        private void OpenLogIn()
        {

            LogOut(this.User.Id);

            Start Start = new Start();


            Start.Show();

            List<Window> windows = new List<Window>();

            foreach (Window f in Application.Current.Windows)
                windows.Add(f);

            foreach (Window f in windows)
            {
                if (f.Name != "Starting")
                    f.Close();
            }            
        }
    }
}
