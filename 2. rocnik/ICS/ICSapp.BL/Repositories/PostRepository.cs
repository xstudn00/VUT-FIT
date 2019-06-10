using System;
using System.Collections.Generic;
using System.Linq;
using ICSapp.DAL.Entities;
using ICSapp.BL.Models;
using ICSapp.BL.Mapper;
using ICSapp.BL.Factories;
using ICSapp.BL.Repositories;
using Microsoft.EntityFrameworkCore;

namespace ICSapp.BL.Repositories
{
    public class PostRepository : IPostRepository
    {
        private readonly IDbContextFactory dbContextFactory;

        public PostRepository(IDbContextFactory dbContextFactory)
        {
            this.dbContextFactory = dbContextFactory;
        }

        public PostModel GetById(Guid postId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Posts
                    .Include(x => x.Author)
                    .Include(x => x.Contributions).ThenInclude(x => x.Author)
                    .FirstOrDefault(t => t.Id == postId);

                if (entity == null)
                {
                    return null;
                }

                return Mapper.Mapper.MapPostEntityToPostModel(entity);
            }
        }

        public List<PostModel> GetPostsByTeam(Guid teamId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {                                            
                var teamEntity = dbContext.Teams
                    .Include(e => e.Posts).ThenInclude(e => e.Author)
                    .Include(e => e.Posts).ThenInclude(e => e.Contributions).ThenInclude(e => e.Author)
                    .First(x => x.Id == teamId);

                List<PostModel> PostList = new List<PostModel>();
                foreach (Post Post in teamEntity.Posts)
                {
                    PostList.Add(Mapper.Mapper.MapPostEntityToPostModel(Post));
                }
                return PostList;
            }
        }

        public void DeletePost(Guid postId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Posts
                    .Include(x => x.Contributions)
                    .First(x => x.Id == postId);

                dbContext.Posts.Remove(entity);
                dbContext.SaveChanges();
            }
        }
        
        public CommentModel CreateComment(CommentModel commentModel, Guid userId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var userEntity = dbContext.Users
                    .First(t => t.Id == userId);

                var entity = new Comment
                {
                    Time = commentModel.Time,
                    MessageContent = commentModel.MessageContent,
                    Author = userEntity
                };
                
                dbContext.Comments.Add(entity);
                dbContext.SaveChanges();
                return Mapper.Mapper.MapCommentEntityToCommentModel(entity);
            }
        }

        public PostModel AddComment(Guid postId, Guid commentId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var commentEntity = dbContext.Comments
                    .Include(x => x.Author)
                    .FirstOrDefault(x => x.Id == commentId);

                var postEntity = dbContext.Posts
                    .Include(x => x.Author)
                    .Include(x => x.Contributions).ThenInclude(x => x.Author)
                    .First(x => x.Id == postId);
                
                postEntity.Contributions.Add(commentEntity);
                dbContext.Update(postEntity);              
                
                dbContext.SaveChanges();
                return Mapper.Mapper.MapPostEntityToPostModel(postEntity);
            }
        }
    }
}
