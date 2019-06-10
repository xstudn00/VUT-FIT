using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Models;

namespace ICSapp.BL.Repositories
{
    public interface IUserRepository
    {
        IEnumerable<UsersListModel> GetAll();
        UserModel GetById(Guid userId);
        List<TeamModel> GetTeamsByUser(Guid userId);
        UserModel GetByEmailAdress(String emailAdress);
        UserModel CreateUser(UserModel userModel);
        void UpdateUser(UserModel userModel);
        void RemoveUser(Guid userId);
        TeamModel CreateTeam(TeamModel teamModel);
        bool LogIn(string emailAdress, string logInPassword);
        void LogOut(Guid userId);
    }
}