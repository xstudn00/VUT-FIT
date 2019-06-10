using ICSapp.App.ViewModels;
using ICSapp.BL.Factories;
using ICSapp.BL.Repositories;
using ICSapp.BL.Services;
using Microsoft.Extensions.Configuration;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ICSapp.App
{
    public class ViewModelLocator
    {
        private readonly ICommentRepository commentRepository;
        private readonly IPostRepository postRepository;
        private readonly ITeamRepository teamRepository;
        private readonly IUserRepository userRepository;
        private readonly IMediator mediator;

        public ViewModelLocator()
        {
            var dbContextFactory = new DbContextFactory();

            mediator = new Mediator();
            commentRepository = new CommentRepository(dbContextFactory);
            postRepository = new PostRepository(dbContextFactory);
            teamRepository = new TeamRepository(dbContextFactory);
            userRepository = new UserRepository(dbContextFactory); 
        }

        public UserViewModel UserViewModel => new UserViewModel(userRepository, teamRepository, mediator);
        public LoginViewModel LoginViewModel => new LoginViewModel(userRepository, mediator);
        public TeamViewModel TeamViewModel => new TeamViewModel(userRepository, teamRepository, postRepository, mediator);
        public TeamsListViewModel TeamsListViewModel => new TeamsListViewModel(userRepository, postRepository, teamRepository, commentRepository, mediator);
    }
}
