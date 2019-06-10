using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Models;
using ICSapp.DAL.Entities;
using Xunit;

namespace ICSapp.BL.Tests
{
    public class UserRepositoryTests : IClassFixture<UserRepositoryTestsFixture>
    {
        private readonly UserRepositoryTestsFixture fixture;

        public UserRepositoryTests(UserRepositoryTestsFixture fixture)
        {
            this.fixture = fixture;
        }

        [Fact]
        public void Create_Test()
        {
            var userModel = new UserModel
            {
                EmailAdress = "DavidValecky@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "170cm blonďák, tetování až na patách.",
                Picture = new byte[10],
                Activity = false,
                LastActivity = DateTime.Now
            };

            var returnedModel = fixture.Repository.CreateUser(userModel);

            Assert.NotNull(returnedModel);

            fixture.Repository.RemoveUser(returnedModel.Id);
        }

        [Fact]
        public void GetByEmailAddress_Test()
        {
            var userModel = new UserModel
            {
                EmailAdress = "DavidValecky@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "170cm blonďák, tetování až na patách.",
                Activity = false,
                LastActivity = DateTime.Now
            };

            var returnedModel1 = fixture.Repository.CreateUser(userModel);
            var returnedModel2 = fixture.Repository.GetByEmailAdress("DavidValecky@centrum.cz");

            Assert.Equal(returnedModel1.Id, returnedModel2.Id);

            fixture.Repository.RemoveUser(returnedModel2.Id);
        }

        [Fact]
        public void GetByID_Test()
        {
            var userModel = new UserModel
            {
                EmailAdress = "DavidValecky@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "170cm blonďák, tetování až na patách.",
                Activity = false,
                LastActivity = DateTime.Now
            };

            var returnedModel1 = fixture.Repository.CreateUser(userModel);
            var returnedModel2 = fixture.Repository.GetById(returnedModel1.Id);

            Assert.NotNull(returnedModel2);

            fixture.Repository.RemoveUser(returnedModel2.Id);
        }

        [Fact]
        public void UpdateUser_Test()
        {
            var userModel = new UserModel
            {
                EmailAdress = "DavidValecky@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "170cm blonďák, tetování až na patách.",
                Activity = false,
                LastActivity = DateTime.Now
            };

            var returnedModel = fixture.Repository.CreateUser(userModel);

            string newDescription = "Novy popis uzivatele";
            returnedModel.Description = newDescription;

            fixture.Repository.UpdateUser(returnedModel);
            var returnedModel2 = fixture.Repository.GetById(returnedModel.Id);

            Assert.Equal("Novy popis uzivatele", returnedModel2.Description);

            fixture.Repository.RemoveUser(returnedModel2.Id);
        }

        [Fact]
        public void CreateTeam_Test()
        {
            var teamModel = new TeamModel
            {
                TeamName = "Holubičky"
            };
            
            var returnedModel2 = fixture.Repository.CreateTeam(teamModel);

            var returnedTeam = fixture.TeamRepository.GetById(returnedModel2.Id);

            Assert.NotNull(returnedModel2);
            
            fixture.TeamRepository.RemoveTeam(returnedModel2.Id);
        }
    }
}
