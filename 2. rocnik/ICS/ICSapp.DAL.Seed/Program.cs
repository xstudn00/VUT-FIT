using System;
using System.Collections.Generic;
using ICSapp.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSapp.DAL.Seed
{
    class Program
    {
        static void Main(string[] args)
        {
            using (var dbContext = CreateDbContext())
            {
                Console.WriteLine("Clearing Data into DB");
                ClearDatabase(dbContext);
                Console.WriteLine("Seeding Data into DB");
                SeedData(dbContext);
            }
        }

        private static void SeedData(AppDbContext dbContext)
        {
            var user = new User
            {
                Id = new Guid("02222222-2222-2222-2222-222222222222"),
                EmailAdress = "DavidValecky@centrum.cz",
                Password = "rabat587",
                FirstName = "David",
                Surname = "Valecký",
                Description = "Jsem hodný švarný kluk, který se do toho nebojí praštit.",
                Activity = false,
                LastActivity = DateTime.Now,      
            };
            var comment = new Comment
            {
                Time = DateTime.Now,
                MessageContent = "Ja tak rád trsám trsám.",
                Author = user,
            };
            dbContext.Comments.Add(comment);
            comment = new Comment
            {
                Time = DateTime.Now,
                MessageContent = "Nic se mi nechce.",
                Author = user,
            };
            dbContext.Comments.Add(comment);
            var user2 = new User
            {
                Id = new Guid("12222222-2222-2222-2222-222222222222"),
                EmailAdress = "Ondrasss@seznam.cz",
                Password = "krutor1234",
                FirstName = "Ondra",
                Surname = "Studnicka",
                Description = "Mám velký nohy a nebojím se je použít.",
                Activity = false,
                LastActivity = DateTime.Now
            };
            comment = new Comment
            {
                Time = DateTime.Now,
                MessageContent = "Jdu už spát.",
                Author = user2,
            };
            dbContext.Comments.Add(comment);
            var user3 = new User
            {
                Id = new Guid("22222222-2222-2222-2222-222222222222"),
                EmailAdress = "Dejfiiik@gmail.com",
                Password = "walk545man",
                FirstName = "David",
                Surname = "Gajdoš",
                Description = "Rád sportuju, cvičím jogu, žiji zdravě a používám lubrikant.",
                Activity = false,
                LastActivity = DateTime.Now
            };
            var user4 = new User
            {
                Id = new Guid("32222222-2222-2222-2222-222222222222"),
                EmailAdress = "Machaamp@seznam.cz",
                Password = "pokemoni666",
                FirstName = "Martin",
                Surname = "Macháček",
                Description = "Ne, nejsem pokémon, jen se mi tak říká.",
                Activity = false,
                LastActivity = DateTime.Now
            };
            var user5 = new User
            {
                Id = new Guid("42222222-2222-2222-2222-222222222222"),
                EmailAdress = "VencoslavT@seznam.cz",
                Password = "Messias10",
                FirstName = "Václav",
                Surname = "Trampeška",
                Description = "Když zrovna nedávám předměty na první dobrou, nedávám předměty.",
                Activity = false,
                LastActivity = DateTime.Now
            };            

            var team = new Team
            {
                Id = new Guid("01111111-1111-1111-1111-111111111111"),
                TeamName = "Programátorské grémium"
            };
            team.UserLinks = new List<UserTeam>
            {
                new UserTeam
                {
                    User = user,
                    Team = team,
                    Order = 0
                },
                new UserTeam
                {
                    User = user2,
                    Team = team,
                    Order = 1
                },
                new UserTeam
                {
                    User = user3,
                    Team = team,
                    Order = 2
                },
                new UserTeam
                {
                    User = user4,
                    Team = team,
                    Order = 3
                },
                new UserTeam
                {
                    User = user5,
                    Team = team,
                    Order = 4
                },


            };
            dbContext.Teams.Add(team);

            var team2 = new Team
            {
                Id = new Guid("11111111-1111-1111-1111-111111111111"),
                TeamName = "Traktoři!"
            };
            dbContext.Teams.Add(team);

            var user6 = new User
            {
                Id = new Guid("52222222-2222-2222-2222-222222222222"),
                EmailAdress = "testuser@seznam.cz",
                Password = "lavinaa1234",
                FirstName = "Jindráš",
                Surname = "Otočpádlo",
                Description = "Test",
                Activity = false,
                LastActivity = DateTime.Now,
                TeamLinks = new List<UserTeam>
                {
                    new UserTeam
                    {
                        Team = team,
                        Order = 5
                    },
                    new UserTeam
                    {
                        Team = team2,
                        Order = 0
                    },
                }
            };
            dbContext.Users.Add(user6);

            team = new Team
            {
                Id = new Guid("21111111-1111-1111-1111-111111111111"),
                TeamName = "Hen taj tot kaj",
                Posts = new List<Post>
                {
                    new Post
                    {
                        Time = DateTime.Now,
                        MessageContent = "Nic se mi nechce.",
                        Author = user,
                    },
                    new Post
                    {
                        Time = DateTime.Now,
                        MessageContent = "Nemám tu být.",
                        Author = user6
                    }
                },
            };
            dbContext.Teams.Add(team);

            dbContext.SaveChanges();
        }
        private static void ClearDatabase(AppDbContext dbContext)
        {
            if (dbContext.Comments != null)
                dbContext.RemoveRange(dbContext.Comments);
            if (dbContext.Posts != null)
                dbContext.RemoveRange(dbContext.Posts);
            if (dbContext.Users!=null)
                dbContext.RemoveRange(dbContext.Users);
            if(dbContext.Teams!=null)
                dbContext.RemoveRange(dbContext.Teams);
            
            dbContext.SaveChanges();
        }

        private static AppDbContext CreateDbContext()
        {
            var optionsBuilder = new DbContextOptionsBuilder<AppDbContext>();
            optionsBuilder.UseSqlServer(@"Data Source=(localdb)\MSSQLLocalDB;Initial Catalog=ICSAppDb;Integrated Security=True;Connect Timeout=30;Encrypt=False;TrustServerCertificate=False;ApplicationIntent=ReadWrite;MultiSubnetFailover=False");
            return new AppDbContext(optionsBuilder.Options);
        }
    }
}
