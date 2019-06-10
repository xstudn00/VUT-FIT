using System;
using System.Collections.Generic;
using System.Linq;
using ICSapp.DAL.Entities;
using ICSapp.BL.Models;
using ICSapp.BL.Mapper;
using ICSapp.BL.Factories;
using Microsoft.EntityFrameworkCore;

namespace ICSapp.BL.Repositories
{
    public class TeamRepository : ITeamRepository
    {
        private readonly IDbContextFactory dbContextFactory;

        public TeamRepository(IDbContextFactory dbContextFactory)
        {
            this.dbContextFactory = dbContextFactory;
        }

        public List<UsersListModel> GetUsersByTeam(Guid teamId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                try
                {
                    var users = dbContext.Users
                    .Where(s => s.TeamLinks.Any(c => c.TeamId == teamId))
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .Select(Mapper.Mapper.MapUserEntityToUsersListModel)
                    .ToList();
                    return users;
                }
                catch
                {
                    return null;
                }
                
            }
        }

        public TeamModel GetById(Guid teamId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                
                var entity = dbContext.Teams
                    .Include(x => x.UserLinks).ThenInclude(x => x.User)
                    .Include(x => x.UserLinks).ThenInclude(x => x.Team)
                    .Include(x => x.Posts).ThenInclude(x => x.Author)
                    .FirstOrDefault(t => t.Id == teamId);

                if (entity == null)
                {
                    return null;
                }

                return Mapper.Mapper.MapTeamEntityToTeamModel(entity);
                
            }
        }

        public TeamModel AddTeamMate(Guid teamId, Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var teamEntity = dbContext.Teams
                    .Include(x => x.UserLinks).ThenInclude(x => x.User)
                    .Include(x => x.UserLinks).ThenInclude(x => x.Team)
                    .Include(x => x.Posts).ThenInclude(x => x.Author)
                    .FirstOrDefault(x => x.Id == teamId);

                if (teamEntity.UserLinks.Where(x => x.UserId == userId).Any() == false)
                {
                    var userEntity = dbContext.Users
                        .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                        .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                        .FirstOrDefault(x => x.Id == userId);

                    var userTeamLink = new UserTeam { UserId = userEntity.Id, TeamId = teamEntity.Id, User = userEntity, Team = teamEntity };

                    teamEntity.UserLinks.Add(userTeamLink);
                    userEntity.TeamLinks.Add(userTeamLink);

                    dbContext.Teams.Update(teamEntity);
                    dbContext.Users.Update(userEntity);
                    dbContext.SaveChanges();
                }

                return Mapper.Mapper.MapTeamEntityToTeamModel(teamEntity);
            }
        }

        public void RemoveTeam(Guid teamId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var teamEntity = dbContext.Teams
                    .Include(x => x.UserLinks).ThenInclude(x => x.User)
                    .Include(x => x.UserLinks).ThenInclude(x => x.Team)
                    .Include(x => x.Posts).ThenInclude(x => x.Author)
                    .First(t => t.Id == teamId);

                dbContext.Teams.Remove(teamEntity);
                dbContext.SaveChanges();                
            }
        }

        public TeamModel RemoveTeamMate(Guid teamId, Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {

                var teamEntity = dbContext.Teams
                    .Include(x => x.UserLinks).ThenInclude(x => x.User)
                    .Include(x => x.UserLinks).ThenInclude(x => x.Team)
                    .Include(x => x.Posts).ThenInclude(x => x.Author)
                    .First(t => t.Id == teamId);

                var userLink = teamEntity.UserLinks
                    .First(t => t.UserId == userId);

                teamEntity.UserLinks.Remove(userLink);
                dbContext.Teams.Update(teamEntity);

                var userEntity = dbContext.Users
                    .Include(x => x.TeamLinks).ThenInclude(x => x.User)
                    .Include(x => x.TeamLinks).ThenInclude(x => x.Team)
                    .First(t => t.Id == userId);

                var teamLink = userEntity.TeamLinks.First(t => t.TeamId == teamId);
                userEntity.TeamLinks.Remove(teamLink);
                dbContext.Users.Update(userEntity);

                dbContext.SaveChanges();
                //if empty -> delete team
                return Mapper.Mapper.MapTeamEntityToTeamModel(teamEntity);
                }
        }

        public PostModel CreatePost(PostModel postModel, Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var userEntity = dbContext.Users
                    .First(t => t.Id == userId);

                var postEntity = new Post
                {
                    Title = postModel.Title,
                    Time = postModel.Time,
                    MessageContent = postModel.MessageContent,
                    Author = userEntity,
                    Contributions = new List<Comment>()
                };

                dbContext.Posts.Add(postEntity);
                dbContext.SaveChanges();
                return Mapper.Mapper.MapPostEntityToPostModel(postEntity);
            }
        }

        public TeamModel AddPost(Guid teamId, Guid postId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var teamEntity = dbContext.Teams
                    .Include(x => x.UserLinks).ThenInclude(x => x.User)
                    .Include(x => x.UserLinks).ThenInclude(x => x.Team)
                    .Include(x => x.Posts).ThenInclude(x => x.Author)
                    .First(t => t.Id == teamId);

                var postEntity = dbContext.Posts
                    .Include(x => x.Contributions).ThenInclude(x => x.Author)
                    .Include(x => x.Author)
                    .First(t => t.Id == postId);

                if (teamEntity.Posts == null)
                {
                    teamEntity.Posts = new List<Post>();
                }

                teamEntity.Posts.Add(postEntity);

                dbContext.Teams.Update(teamEntity);
                dbContext.SaveChanges();

                return Mapper.Mapper.MapTeamEntityToTeamModel(teamEntity);
            }
        }
    }
}