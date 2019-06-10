using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.DAL.Entities;
using ICSapp.BL.Models;
using System.Linq;

namespace ICSapp.BL.Mapper
{
    internal static class Mapper
    {       
        public static UsersListModel MapUserEntityToUsersListModel(User entity)
        {
            return new UsersListModel
            {
                Id = entity.Id,
                FirstName = entity.FirstName,
                Surname = entity.Surname,
                EmailAdress = entity.EmailAdress,
                Picture = entity.Picture
            };
        }

        public static UserModel MapUserEntityToUserModel(User entity)
        {
            List<UserTeamModel> teamLink;
            if ( entity.TeamLinks != null && entity.TeamLinks.Count > 0)
            {
                teamLink = entity.TeamLinks.Select(x => MapUserTeamEntityToUserTeamModel(x)).ToList();
            }
            else
            {
                teamLink = new List<UserTeamModel>();
            }
                       
            return new UserModel
            {
                Id = entity.Id,
                EmailAdress = entity.EmailAdress,
                Password = entity.Password,
                FirstName = entity.FirstName,
                Surname = entity.Surname,
                Description = entity.Description,
                Picture = entity.Picture,
                Activity = entity.Activity,
                LastActivity = entity.LastActivity,
                Teams = teamLink
            };
        }

        public static UserModel MapUserEntityToUserModelBase(User entity)
        {
            return new UserModel
            {
                Id = entity.Id,
                EmailAdress = entity.EmailAdress,
                Password = entity.Password,
                FirstName = entity.FirstName,
                Surname = entity.Surname,
                Description = entity.Description,
                Picture = entity.Picture,
                Activity = entity.Activity,
                LastActivity = entity.LastActivity
            };
        }

        public static User MapUserModelToUserEntity(UserModel model)
        {
            List<UserTeam> teamLink;

            if (model.Teams != null && model.Teams.Count > 0)
            {
                teamLink = model.Teams.Select(x => MapUserTeamModelToUserTeamEntity(x)).ToList();
            }
            else
            {
                teamLink = new List<UserTeam>();
            }           


            var user = new User
            {
                Id = model.Id,
                EmailAdress = model.EmailAdress,
                Password = model.Password,
                FirstName = model.FirstName,
                Surname = model.Surname,
                Description = model.Description,
                Picture = model.Picture,
                Activity = model.Activity,
                LastActivity = model.LastActivity,
                TeamLinks = teamLink
            };
            return user;
        }

        public static User MapUserModelToUserEntityBase(UserModel model)
        {
            return new User
            {
                Id = model.Id,
                EmailAdress = model.EmailAdress,
                Password = model.Password,
                FirstName = model.FirstName,
                Surname = model.Surname,
                Description = model.Description,
                Picture = model.Picture,
                Activity = model.Activity,
                LastActivity = model.LastActivity
            };
        }

        public static TeamModel MapTeamEntityToTeamModel(Team entity)
        {
            List<UserTeamModel> members;
            if (entity.UserLinks != null && entity.UserLinks.Count > 0)
            {
                members = entity.UserLinks.Select(x => MapUserTeamEntityToUserTeamModel(x)).ToList();
            }
            else
            {
                members = new List<UserTeamModel>();
            }
            
            List<PostModel> posts;
            if (entity.Posts != null && entity.Posts.Count > 0)
            {
                posts = entity.Posts.Select(x => MapPostEntityToPostModel(x)).ToList();
            }
            else
            {
                posts = new List<PostModel>();
            }
            
            return new TeamModel
            {
                Id = entity.Id,
                TeamName = entity.TeamName,
                Members = members,
                Posts = posts
            };
        }

        public static TeamModel MapTeamEntityToTeamModelBase(Team entity)
        {
            return new TeamModel
            {
                Id = entity.Id,
                TeamName = entity.TeamName
            };
        }

        public static Team MapTeamModelToTeamEntity(TeamModel model)
        {
            List<UserTeam> members;
            if (model.Members != null && model.Members.Count > 0)
            {
                members = model.Members.Select(x => MapUserTeamModelToUserTeamEntity(x)).ToList();
            }
            else
            {
                members = new List<UserTeam>();
            }
            
            List<Post> posts;
            if (model.Posts != null && model.Posts.Count > 0)
            {
                posts = model.Posts.Select(x => MapPostModelToPostEntity(x)).ToList();
            }
            else
            {
                posts = new List<Post>();
            }

            return new Team
            {
                Id = model.Id,
                TeamName = model.TeamName,
                UserLinks = members,
                Posts = posts
            };
        }

        public static Team MapTeamModelToTeamEntityBase(TeamModel model)
        {
            return new Team
            {
                Id = model.Id,
                TeamName = model.TeamName
            };
        }

        public static UserTeamModel MapUserTeamEntityToUserTeamModel(UserTeam entity)
        {
            if (entity == null)
                return null;

            var teamId = entity.TeamId;
            var team = MapTeamEntityToTeamModelBase(entity.Team);
            var userId = entity.UserId;
            var user = MapUserEntityToUserModelBase(entity.User);
            return new UserTeamModel
            {
                TeamId = teamId,
                Team = team,
                UserId = userId,
                User = user
            };
        }

        public static UserTeam MapUserTeamModelToUserTeamEntity(UserTeamModel model)
        {
            return new UserTeam
            {
                TeamId = model.TeamId,
                Team = MapTeamModelToTeamEntityBase(model.Team),
                UserId = model.UserId,
                User = MapUserModelToUserEntityBase(model.User)
            };
        }

        public static PostModel MapPostEntityToPostModel(Post entity)
        {
            if (entity == null)
                return null;

            List<CommentModel> comments;
            if (entity.Contributions != null && entity.Contributions.Count > 0)
            {
                comments = entity.Contributions.Select(x => MapCommentEntityToCommentModel(x)).ToList();
            }
            else
            {
                comments = new List<CommentModel>();
            }

            return new PostModel
            {
                Id = entity.Id,
                Title = entity.Title,
                Time = entity.Time,
                MessageContent = entity.MessageContent,
                Author = MapUserEntityToUserModelBase(entity.Author),
                Contributions = comments
            };
        }

        public static Post MapPostModelToPostEntity(PostModel model)
        {
            List<Comment> comments;
            if (model.Contributions != null && model.Contributions.Count > 0)
            {
                comments = model.Contributions.Select(x => MapCommentModelToCommentEntity(x)).ToList();
            }
            else
            {
                comments = new List<Comment>();
            }

            return new Post
            {
                Id = model.Id,
                Title = model.Title,
                Time = model.Time,
                MessageContent = model.MessageContent,
                Author = MapUserModelToUserEntityBase(model.Author),
                Contributions = comments
            };
        }
        
        public static CommentModel MapCommentEntityToCommentModel(Comment entity)
        {
            if (entity == null)
                return null;
            return new CommentModel
            {
                Id = entity.Id,
                Time = entity.Time,
                MessageContent = entity.MessageContent,
                Author = MapUserEntityToUserModelBase(entity.Author) 
            };
        }

        public static Comment MapCommentModelToCommentEntity(CommentModel model)
        {
            return new Comment
            {
                Id = model.Id,
                Time = model.Time,
                MessageContent = model.MessageContent,
                Author = MapUserModelToUserEntityBase(model.Author)
            };
        }
    }
}