using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Repositories;

namespace ICSapp.BL.Tests
{
    public class PostRepositoryTestsFixture
    {
        private readonly IPostRepository repository;
        private readonly ITeamRepository teamRepository;
        private readonly IUserRepository userRepository;
        private readonly ICommentRepository commentRepository;

        public PostRepositoryTestsFixture()
        {
            repository = new PostRepository(new InMemoryDbContextFactory());
            teamRepository = new TeamRepository(new InMemoryDbContextFactory());
            userRepository = new UserRepository(new InMemoryDbContextFactory());
            commentRepository = new CommentRepository(new InMemoryDbContextFactory());
        }

        public IPostRepository Repository => repository;
        public ICommentRepository CommentRepository => commentRepository;
        public ITeamRepository TeamRepository => teamRepository;
        public IUserRepository UserRepository => userRepository;
    }
}
