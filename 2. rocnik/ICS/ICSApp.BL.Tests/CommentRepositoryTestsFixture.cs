using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Repositories;

namespace ICSapp.BL.Tests
{
    public class CommentRepositoryTestsFixture
    {
        private readonly ICommentRepository repository;
        private readonly IPostRepository postRepository;
        private readonly ITeamRepository teamRepository;
        private readonly IUserRepository userRepository;

        public CommentRepositoryTestsFixture()
        {
            repository = new CommentRepository(new InMemoryDbContextFactory());
            teamRepository = new TeamRepository(new InMemoryDbContextFactory());
            userRepository = new UserRepository(new InMemoryDbContextFactory());
            postRepository = new PostRepository(new InMemoryDbContextFactory());
        }

        public ICommentRepository Repository => repository;
        public IPostRepository PostRepository => postRepository;
        public ITeamRepository TeamRepository => teamRepository;
        public IUserRepository UserRepository => userRepository;
    }
}
