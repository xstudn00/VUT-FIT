using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSapp.BL.Factories;
using ICSapp.BL.Models;
using ICSapp.BL.Repositories;

namespace ICSapp.App.ViewModels
{
    class CommentViewModel
    {
        private readonly ICommentRepository repository = new CommentRepository(new DbContextFactory());
        public CommentModel Comment { get; set; } = new CommentModel();
        public List<CommentModel> CommentsList { get; set; } = new List<CommentModel>();
        public PostModel Post { get; set; } = new PostModel();

        public void LoadCommentById(Guid id)
        {
            Comment = repository.GetById(id);
        }

        public void LoadCommentsByPostId(Guid id)
        {
            CommentsList = repository.GetCommentsByPost(id);
        }

        public void DeleteComment()
        {
            repository.DeleteComment(Comment.Id);
        }
    }
}
