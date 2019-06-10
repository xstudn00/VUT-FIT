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
    class PostViewModel
    {
        private readonly IPostRepository repository = new PostRepository(new DbContextFactory());

        public PostModel Post { get; set; } = new PostModel();
        public CommentModel Comment { get; set; } = new CommentModel();
        public List<PostModel> PostsList { get; set; } = new List<PostModel>();

        public void LoadPostById(Guid id)
        {
            Post = repository.GetById(id);
        }
        public void LoadPostsByTeam(Guid id)
        {
            PostsList = repository.GetPostsByTeam(id);
        }
        public void DeletePost()
        {
            repository.DeletePost(Post.Id);
        }
        public void CreateComment()
        {
            Post = repository.AddComment(Post.Id, Comment.Id);
        }
    }
}
