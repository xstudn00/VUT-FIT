using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ICSapp.BL.Models;
using Xunit;

namespace ICSapp.BL.Tests
{
    public class PostRepositoryTests : IClassFixture<PostRepositoryTestsFixture>
    {
        private readonly PostRepositoryTestsFixture fixture;

        public PostRepositoryTests(PostRepositoryTestsFixture fixture)
        {
            this.fixture = fixture;
        }

        [Fact]
        public void AddPost_Test()
        {
            var userModel = new UserModel
            {
                EmailAdress = "DavidValecky2@centrum.cz",
                Password = "rabat587",
                FirstName = "Otakar",
                Surname = "Březina",
                Description = "170cm blonďák, tetování až na patách.",
                Activity = false,
                LastActivity = DateTime.Now
            };
            
            var postModel = new PostModel
            {
                Title = "Test",
                Time = DateTime.Now,
                MessageContent = "obsah"
            };
            
            var teamModel = new TeamModel
            {
                TeamName = "Gargy team"
            };

            var returnedUserModel = fixture.UserRepository.CreateUser(userModel);
            var returnedTeamModel = fixture.UserRepository.CreateTeam(teamModel);
            fixture.TeamRepository.AddTeamMate(returnedTeamModel.Id, returnedUserModel.Id);

            var returnedPostModel = fixture.TeamRepository.CreatePost(postModel, returnedUserModel.Id);
            fixture.TeamRepository.AddPost(returnedTeamModel.Id, returnedPostModel.Id);
            
            var returnedPostModel2 = fixture.TeamRepository.CreatePost(postModel, returnedUserModel.Id);
            fixture.TeamRepository.AddPost(returnedTeamModel.Id, returnedPostModel2.Id);

            var postsByTeam = fixture.Repository.GetPostsByTeam(returnedTeamModel.Id);

            Assert.Equal(2, postsByTeam.Count());

            fixture.Repository.DeletePost(returnedPostModel.Id);
            fixture.Repository.DeletePost(returnedPostModel2.Id);
            fixture.TeamRepository.RemoveTeam(returnedTeamModel.Id);
            fixture.UserRepository.RemoveUser(returnedUserModel.Id);
        }
    }
}
