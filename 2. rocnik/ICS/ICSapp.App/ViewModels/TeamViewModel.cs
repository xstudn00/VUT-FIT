using ICSapp.App.Commands;
using ICSapp.App.Views.Windows;
using ICSapp.BL.Factories;
using ICSapp.BL.Messages;
using ICSapp.BL.Models;
using ICSapp.BL.Repositories;
using ICSapp.BL.Services;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;

namespace ICSapp.App.ViewModels
{
    public class TeamViewModel : BaseViewModel
    {
        private readonly ITeamRepository teamRepository;
        private readonly IUserRepository userRepository;
        private readonly IPostRepository postRepository;

        public List<UsersListModel> UsersList { get; set; }
        public TeamModel Team { get; set; }
        public UserModel SearchUser { get; set; }
        public List<PostModel> PostsList { get; set; }

        public ICommand OpenAddUserWindowCommand { get; set; }
        public ICommand AddUserToTeamCommand { get; set; }
        public ICommand DeleteUserFromTeamCommand { get; set; }
        
        private readonly IMediator mediator;

        public string AddUserText { get; set; }
        public string AddUserEmail { get; set; }

        public TeamViewModel(IUserRepository userRepository, ITeamRepository teamRepository, IPostRepository postRepository, IMediator mediator)
        {
            this.teamRepository = teamRepository;
            this.userRepository = userRepository;
            this.postRepository = postRepository;

            Team = new TeamModel();
            SearchUser = new UserModel();

            this.mediator = mediator;
            
            OpenAddUserWindowCommand = new RelayCommand(OpenAddUserWindow);
            AddUserToTeamCommand = new RelayCommand(TryAddUserToTeam);
            DeleteUserFromTeamCommand = new RelayCommand(DeleteUserFromTeam);

            mediator.Register<ActiveTeamMessage>(GetTeam);
        }

        private void OpenAddUserWindow(object obj)
        {
            AddUserWindow AddUserWindow = new AddUserWindow();
            AddUserWindow.Show();

            mediator.Send(new ActiveTeamMessage { Id = Team.Id });
        }

        private void GetTeam(ActiveTeamMessage obj)
        {
            ActivateTeam(obj.Id);
        }
        private void DeleteUserFromTeam(object obj)
        {
            string email = (string)obj;
            LoadUserByEmail(email);

            if (VerifyDelete())
            {
                if(Team.Members.Count == 1)
                {
                    MessageBoxResult messageBox = System.Windows.MessageBox.Show("Last member must delete the team", "Last member", System.Windows.MessageBoxButton.OK);
                }
                else
                {
                    RemovePersonFromTeam();
                }
            }
            mediator.Send(new ActiveTeamMessage { Id = Team.Id });
        }

        private void TryAddUserToTeam(object obj)
        {
            if (string.IsNullOrEmpty(AddUserEmail))
            {
                AddUserText = "Field must not be empty";
            }
            else
            {
                LoadUserByEmail(AddUserEmail);
                if (SearchUser != null)
                {
                    AddPersonToTeam();
                    if (Team == null)
                    {
                        AddUserText = "This user is already in this team";
                    }
                    else
                    {
                        AddUserText = "User successfully added to the team";
                        mediator.Send(new ActiveTeamMessage { Id = Team.Id });
                    }     
                }
                else
                {
                    AddUserText = "This user doesnt exist";
                }
            } 
        }
        public void LoadTeamById(Guid id)
        {
            Team = teamRepository.GetById(id);
        }
        public void AddPersonToTeam()
        {
            Team = teamRepository.AddTeamMate(Team.Id, SearchUser.Id);
        }
        public void LoadUsersByTeamId(Guid id)
        {
            UsersList = teamRepository.GetUsersByTeam(id);
        }

        public void RemovePersonFromTeam()
        {
            Team = teamRepository.RemoveTeamMate(Team.Id, SearchUser.Id); 
        }
        public void LoadUserByEmail(String EmailAddress)
        {
            SearchUser = userRepository.GetByEmailAdress(EmailAddress);
        }
        private void ActivateTeam(object obj)
        {
            Guid t = (Guid)obj;
            LoadTeamById(t);
            LoadUsersByTeamId(t);
        }
        private bool VerifyDelete()
        {
            MessageBoxResult messageBox = System.Windows.MessageBox.Show("Are you sure you want to delete this user from the team?", "Delete", System.Windows.MessageBoxButton.YesNo);
            if (messageBox == MessageBoxResult.Yes)
            {
                return true;
            }
            return false;
        }
    }
}
