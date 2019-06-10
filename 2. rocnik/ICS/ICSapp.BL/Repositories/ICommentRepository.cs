using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Models;

namespace ICSapp.BL.Repositories
{
    public interface ICommentRepository
    {
        CommentModel GetById(Guid commentId);
        List<CommentModel> GetCommentsByPost(Guid postId);
        void DeleteComment(Guid commentId);
    }
}