using ICSapp.BL.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;

namespace ICSapp.BL.Tests
{
    public class CommentRepositoryTests : IClassFixture<CommentRepositoryTestsFixture>
    {
        private readonly CommentRepositoryTestsFixture fixture;

        public CommentRepositoryTests(CommentRepositoryTestsFixture fixture)
        {
            this.fixture = fixture;
        }

        [Fact]
        public void AddComment_Test()
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

            var commentModel = new CommentModel
            {
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

            var returnedCommentModel = fixture.PostRepository.CreateComment(commentModel, returnedUserModel.Id);
            fixture.PostRepository.AddComment(returnedPostModel.Id, returnedCommentModel.Id);

            var commentsByPost = fixture.Repository.GetCommentsByPost(returnedPostModel.Id);

            Assert.Single(commentsByPost);

            fixture.PostRepository.DeletePost(returnedPostModel.Id);
            fixture.Repository.DeleteComment(returnedCommentModel.Id);
            fixture.TeamRepository.RemoveTeam(returnedTeamModel.Id);
            fixture.UserRepository.RemoveUser(returnedUserModel.Id);
        }
    }
}
