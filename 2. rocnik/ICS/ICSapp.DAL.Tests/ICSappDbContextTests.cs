using System;
using System.Collections.Generic;
using System.Text;
using Xunit;
using ICSapp.DAL.Entities;
using System.Linq;
using Microsoft.EntityFrameworkCore;

namespace ICSapp.DAL.Tests
{
    public class ICSappDbContextTests : IClassFixture<ICSappDbContextTestsClassSetupFixture>
    {
        public ICSappDbContextTests(ICSappDbContextTestsClassSetupFixture testContext)
        {
            _testContext = testContext;
        }

        private readonly ICSappDbContextTestsClassSetupFixture _testContext;
        
        [Fact]
        public void AddUserTest()
        {
            
            var user = new User
            {
                Id = new Guid("02222222-2222-2222-2222-222222222222"),
                EmailAdress = "DavidValecky@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "170cm blonďák, tetování až na patách.",
                Activity = false,
                LastActivity = DateTime.Now,
                Picture = new byte[1]
            };
            
            _testContext.ICSappDbContextSUT.Users.Add(user);
            _testContext.ICSappDbContextSUT.SaveChanges();

            
            using (var dbx = _testContext.ICSappDbContext())
            {
                var retrievedUser = dbx.Users.First(entity => entity.Id == user.Id);
                Assert.Equal(user, retrievedUser, User.UserComparer);
            }
        }

        [Fact]
        public void AddUserWithContributionsTest()
        {
            
            var user = new User
            {
                Id = new Guid("12222222-2222-2222-2222-222222222222"),
                EmailAdress = "Sharks@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "170cm blonďák, tetování až na patách.",
                Activity = false,
                LastActivity = DateTime.Now,
                Picture = new byte[1],
                Contributions = new List<Comment>
                {
                    new Comment
                    {
                        Time = DateTime.Now,
                        MessageContent = "Nic se mi nechce.",
                    },
                    new Comment
                    {
                        Time = DateTime.Now,
                        MessageContent = "Nemám tu být.",
                    }
                },
            };

            _testContext.ICSappDbContextSUT.Users.Add(user);
            _testContext.ICSappDbContextSUT.SaveChanges();

            
            using (var dbx = _testContext.ICSappDbContext())
            {
                var retrievedUser = dbx.Users
                    .Include(entity => entity.Contributions)
                    .First(entity => entity.Id == user.Id);
                Assert.Equal(user, retrievedUser, User.UserComparer);
            }
            
        }
        [Fact]
        public void AddUserWithTeamsTest()
        {
            var team = new Team
            {
                Id = new Guid("02111111-1111-1111-1111-111111111111"),
                TeamName = "ICSmasters"
            };
            var user = new User
            {
                Id = new Guid("12223222-2222-2222-2222-222222222222"),
                EmailAdress = "Sharks@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "170cm blonďák, tetování až na patách.",
                Activity = false,
                LastActivity = DateTime.Now,
                Picture = new byte[1],
                TeamLinks = new List<UserTeam>
                {
                    new UserTeam
                    {
                        Team = team,
                        Order = 0
                    },                  
                }
            };

            _testContext.ICSappDbContextSUT.Teams.Add(team);
            _testContext.ICSappDbContextSUT.Users.Add(user);
            _testContext.ICSappDbContextSUT.SaveChanges();

            
            using (var dbx = _testContext.ICSappDbContext())
            {
                var retrievedUser = dbx.Users
                    .Include(entity => entity.TeamLinks)
                    .First(entity => entity.Id == user.Id);
                Assert.Equal(user, retrievedUser, User.UserComparer);                
            }
        }

        [Fact]
        public void AddTeamWithUsersTest()
        {
            var userTest = new User
            {
                Id = new Guid("22222422-2222-2222-2222-222222222222"),
                EmailAdress = "testuser@seznam.cz",
                Password = "lavinaa1234",
                FirstName = "Jindráš",
                Surname = "Otočpádlo",
                Description = "Test",
                Activity = false,
                LastActivity = DateTime.Now,
                Picture = new byte[1]
            };
            var userTest2 = new User
            {
                Id = new Guid("32122222-2222-2222-2222-222222222222"),
                EmailAdress = "testuser2@seznam.cz",
                Password = "lavinaa1234",
                FirstName = "Lukáš",
                Surname = "Otočpádlo",
                Description = "Tests",
                Activity = false,
                LastActivity = DateTime.Now,
                Picture = new byte[1]
            };
            var team = new Team
            {
                Id = new Guid("42222222-2222-2222-2222-222222222222"),
                TeamName = "IDSnicmoc",
                UserLinks = new List<UserTeam>
                {
                    new UserTeam
                    {
                        User = userTest,
                        Order = 0
                    },
                    new UserTeam
                    {
                        User = userTest2,
                        Order = 1
                    },
                }
            };

            _testContext.ICSappDbContextSUT.Teams.Add(team);
            _testContext.ICSappDbContextSUT.SaveChanges();


            using (var dbx = _testContext.ICSappDbContext())
            {
                var retrievedTeam = dbx.Teams
                    .Include(entity => entity.UserLinks)
                    .First(entity => entity.Id == team.Id);
                Assert.Equal(team, retrievedTeam, Team.TeamComparer);
            }
        }

        [Fact]
        public void AddTeamWithAllTest()
        {
            var userTest = new User
            {
                Id = new Guid("62222222-2222-2222-2222-222222222222"),
                EmailAdress = "testuser@seznam.cz",
                Password = "lavinaa1234",
                FirstName = "Jindráš",
                Surname = "Otočpádlo",
                Description = "Test",
                Activity = false,
                LastActivity = DateTime.Now,
                Picture = new byte[1]
            };
            var userTest2 = new User
            {
                Id = new Guid("72222222-2222-2222-2222-222222222222"),
                EmailAdress = "testuser2@seznam.cz",
                Password = "lavinaa1234",
                FirstName = "Lukáš",
                Surname = "Otočpádlo",
                Description = "Tests",
                Activity = false,
                LastActivity = DateTime.Now,
                Picture = new byte[1]
            };
            var posts = new List<Post>
            {
                new Post
                {
                    Time = DateTime.Now,
                    MessageContent = "Nic se mi nechce.",
                    Author = userTest,
                },
                new Post
                {
                    Time = DateTime.Now,
                    MessageContent = "Nemám tu být.",
                    Author = userTest2
                }
            };
            var team = new Team
            {
                Id = new Guid("82222222-2222-2222-2222-222222222222"),
                TeamName = "IDSnicmoc",
                UserLinks = new List<UserTeam>
                {
                    new UserTeam
                    {
                        User = userTest,
                        Order = 0
                    },
                    new UserTeam
                    {
                        User = userTest2,
                        Order = 1
                    },
                },
                Posts = posts,
            };
            
            _testContext.ICSappDbContextSUT.Teams.Add(team);
            _testContext.ICSappDbContextSUT.SaveChanges();


            using (var dbx = _testContext.ICSappDbContext())
            {
                var retrievedTeam = dbx.Teams
                .Include(entity => entity.UserLinks)
                .Include(entity => entity.Posts)
                .First(entity => entity.Id == team.Id);
                Assert.Equal(team, retrievedTeam, Team.TeamComparer);
            }
        }
    }
}
