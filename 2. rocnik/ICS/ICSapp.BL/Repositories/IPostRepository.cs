using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Models;

namespace ICSapp.BL.Repositories
{
    public interface IPostRepository
    {
        PostModel GetById(Guid postId);
        List<PostModel> GetPostsByTeam(Guid teamId);
        void DeletePost(Guid postId);
        CommentModel CreateComment(CommentModel commentModel, Guid userId);
        PostModel AddComment(Guid postId, Guid commentId);
    }
}