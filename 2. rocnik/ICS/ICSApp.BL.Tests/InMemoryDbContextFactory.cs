using System;
using Xunit;
using ICSapp.BL.Factories;
using ICSapp.DAL;
using Microsoft.EntityFrameworkCore;

namespace ICSapp.BL.Tests
{
    class InMemoryDbContextFactory : IDbContextFactory
    {
        public AppDbContext CreateDbContext()
        {
            var optionsBuilder = new DbContextOptionsBuilder<AppDbContext>();
            optionsBuilder.UseInMemoryDatabase("ICSappDB");
            return new AppDbContext(optionsBuilder.Options);
        }
    }
}
