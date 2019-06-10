using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Repositories;

namespace ICSapp.BL.Tests
{
    public class TeamRepositoryTestsFixture
    {
        private readonly ITeamRepository repository;
        private readonly IUserRepository userRepository;
        private readonly IPostRepository postRepository;

        public TeamRepositoryTestsFixture()
        {
            repository = new TeamRepository(new InMemoryDbContextFactory());
            userRepository = new UserRepository(new InMemoryDbContextFactory());
            postRepository = new PostRepository(new InMemoryDbContextFactory());
        }

        public ITeamRepository Repository => repository;
        public IUserRepository UserRepository => userRepository;
        public IPostRepository PostRepository => postRepository;

    }
}
