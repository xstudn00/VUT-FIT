using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Text;

namespace ICSapp.DAL.Tests
{
    public class ICSappDbContextTestsClassSetupFixture : IDisposable
    {
        public AppDbContext ICSappDbContextSUT { get; set; }

        public ICSappDbContextTestsClassSetupFixture()
        {
            this.ICSappDbContextSUT = ICSappDbContext();
        }

        public AppDbContext ICSappDbContext()
        {
            return new AppDbContext(CreateDbContextOptions());
        }

        private DbContextOptions<AppDbContext> CreateDbContextOptions()
        {
            var contextOptionsBuilder = new DbContextOptionsBuilder<AppDbContext>();
            contextOptionsBuilder.UseInMemoryDatabase("ICSAppDb");
            return contextOptionsBuilder.Options;
        }

        public void Dispose()
        {
            ICSappDbContextSUT?.Dispose();
        }
    }
}