namespace ICSapp.BL.Factories
{
    public interface IDbContextFactory
    {
        AppDbContext CreateDbContext();
    }
}