using ICSapp.DAL;
using Microsoft.EntityFrameworkCore;

namespace ICSapp.BL.Factories
{
    public class DbContextFactory : IDbContextFactory
    {
        public AppDbContext CreateDbContext()
        {
            var optionsBuilder = new DbContextOptionsBuilder<AppDbContext>();
            optionsBuilder.UseSqlServer(@"Data Source=(localdb)\MSSQLLocalDB;Initial Catalog=ICSAppDb;Integrated Security=True;Connect Timeout=30;Encrypt=False;TrustServerCertificate=False;ApplicationIntent=ReadWrite;MultiSubnetFailover=False");
            return new AppDbContext(optionsBuilder.Options);
        }

    }
}
