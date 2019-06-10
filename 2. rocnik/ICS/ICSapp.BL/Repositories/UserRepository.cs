using ICSapp.BL.Factories;
using ICSapp.BL.Models;
using ICSapp.DAL.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.EntityFrameworkCore;

namespace ICSapp.BL.Repositories
{
    public class UserRepository : IUserRepository
    {
        private readonly IDbContextFactory dbContextFactory;

        public UserRepository(IDbContextFactory dbContextFactory)
        {
            this.dbContextFactory = dbContextFactory;
        }

        public IEnumerable<UsersListModel> GetAll()
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                return dbContext.Users
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .Select(e => Mapper.Mapper.MapUserEntityToUsersListModel(e));
            }
        }

        public UserModel GetById(Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .FirstOrDefault(t => t.Id == userId);

                if (entity == null)
                {
                    return null;
                }

                return Mapper.Mapper.MapUserEntityToUserModel(entity);
            }
        }

        public List<TeamModel> GetTeamsByUser(Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                return dbContext.Teams
                    .Where(s => s.UserLinks.Any(c => c.UserId == userId))
                    .Include(x => x.UserLinks).ThenInclude(x => x.User)
                    .Include(x => x.UserLinks).ThenInclude(x => x.Team)
                    .Include(x => x.Posts).ThenInclude(x => x.Author)
                    .Select(s => Mapper.Mapper.MapTeamEntityToTeamModel(s))
                    .ToList();
            }
        }

        public UserModel GetByEmailAdress(String emailAdress)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {               
                var entity = dbContext.Users
                .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                .FirstOrDefault(t => t.EmailAdress == emailAdress);

                if (entity == null)
                {
                    return null;
                }

                return Mapper.Mapper.MapUserEntityToUserModel(entity);                        
            }
        }

        public UserModel CreateUser(UserModel userModel)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = new User
                {
                    EmailAdress = userModel.EmailAdress,
                    Password = userModel.Password,
                    FirstName = userModel.FirstName,
                    Surname = userModel.Surname,
                    Description = userModel.Description,
                    Picture = userModel.Picture,
                    Activity = userModel.Activity,
                    LastActivity = userModel.LastActivity,
                    TeamLinks = new List<UserTeam>()
                };

                dbContext.Users.Add(entity);
                dbContext.SaveChanges();
                return Mapper.Mapper.MapUserEntityToUserModel(entity);
            }
        }

        public void UpdateUser(UserModel userModel)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .First(t => t.Id == userModel.Id);

                entity.FirstName = userModel.FirstName;
                entity.Surname = userModel.Surname;
                entity.Password = userModel.Password;
                entity.Description = userModel.Description;
                entity.Picture = userModel.Picture;

                dbContext.Users.Update(entity);
                dbContext.SaveChanges();
            }
        }

        public void RemoveUser(Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .First(t => t.Id == userId);

                dbContext.Remove(entity);
                dbContext.SaveChanges();
            }
        }

        public TeamModel CreateTeam(TeamModel teamModel)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = new Team
                {
                    TeamName = teamModel.TeamName,
                    UserLinks = new List<UserTeam>(),
                    Posts = new List<Post>()
                };

                dbContext.Teams.Add(entity);
                dbContext.SaveChanges();
                
                return Mapper.Mapper.MapTeamEntityToTeamModel(entity);
            }
        }

        public bool LogIn(string emailAdress, string logInPassword)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .FirstOrDefault(x => x.EmailAdress == emailAdress);

                if (entity.Password != null && BCrypt.Net.BCrypt.Verify(logInPassword, entity.Password))
                {
                    entity.Activity = true;
                    dbContext.Users.Update(entity);
                    dbContext.SaveChanges();
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        public void LogOut(Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Users
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .First(t => t.Id == userId);

                entity.Activity = false;
                entity.LastActivity = DateTime.Now;
                dbContext.Users.Update(entity);
                dbContext.SaveChanges();
            }
        }
    }
}