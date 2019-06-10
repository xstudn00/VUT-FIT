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
    public class CommentRepository : ICommentRepository
    {
        private readonly IDbContextFactory dbContextFactory;

        public CommentRepository(IDbContextFactory dbContextFactory)
        {
            this.dbContextFactory = dbContextFactory;
        }

        public CommentModel GetById(Guid commentId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {
                var entity = dbContext.Comments.FirstOrDefault(t => t.Id == commentId);

                if (entity == null)
                {
                    return null;
                }

                return Mapper.Mapper.MapCommentEntityToCommentModel(entity);
            }
        }

        public List<CommentModel> GetCommentsByPost(Guid postId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {

                var postEntity = dbContext.Posts
                    .Include(x => x.Contributions).ThenInclude(x => x.Author)
                    .First(t => t.Id == postId);

                List<CommentModel> commentList = new List<CommentModel>();
                foreach (Comment comment in postEntity.Contributions)
                {
                    commentList.Add(Mapper.Mapper.MapCommentEntityToCommentModel(comment));
                }
                return commentList;
            }
        }

        public void DeleteComment(Guid commentId)
        {
            using (var dbContext = dbContextFactory.CreateDbContext())
            {

                var entity = dbContext.Comments.FirstOrDefault(x => x.Id == commentId);
                dbContext.Comments.Remove(entity);
                dbContext.SaveChanges();
            }
        }
    }
}
