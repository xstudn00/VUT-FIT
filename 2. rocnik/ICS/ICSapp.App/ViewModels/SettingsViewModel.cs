using ICSapp.BL.Messages;
using ICSapp.BL.Models;
using ICSapp.BL.Repositories;
using ICSapp.BL.Services;
using System;

namespace ICSapp.App.ViewModels
{
    public class SettingsViewModel : BaseViewModel
    {
        private readonly IUserRepository repository;
        public UserModel User { get; set; }
        private readonly IMediator mediator;

        public SettingsViewModel(IUserRepository repository, IMediator mediator)
        {
            this.repository = repository;
            this.mediator = mediator;

            this.User = new UserModel();
            mediator.Register<UserLoginMessage>(UserLoggedIn);
        }

        private void UserLoggedIn(UserLoginMessage obj)
        {
            LoadUserById(obj.Id);
        }

        public void LoadUserById(Guid id)
        {
            User = repository.GetById(id);
        }
    }
}
