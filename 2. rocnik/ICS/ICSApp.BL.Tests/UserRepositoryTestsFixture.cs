using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Repositories;

namespace ICSapp.BL.Tests
{
    public class UserRepositoryTestsFixture
    {
        private readonly IUserRepository repository;
        private readonly ITeamRepository teamRepository;

        public UserRepositoryTestsFixture()
        {
            repository = new UserRepository(new InMemoryDbContextFactory());
            teamRepository = new TeamRepository(new InMemoryDbContextFactory());
        }

        public IUserRepository Repository => repository;
        public ITeamRepository TeamRepository => teamRepository;

    }
}
