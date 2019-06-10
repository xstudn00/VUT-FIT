using ICSapp.DAL.Entities;
using Microsoft.EntityFrameworkCore;
using System;

public class AppDbContext : DbContext
{
    public AppDbContext()
    {

    }
    public AppDbContext(DbContextOptions<AppDbContext> contextOptions)
        : base(contextOptions)
    {
    }
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<UserTeam>()
           .HasKey(x => new { x.UserId, x.TeamId });

        modelBuilder.Entity<UserTeam>()
            .HasOne(pt => pt.User)
            .WithMany(p => p.TeamLinks)
            .HasForeignKey(pt => pt.UserId);

        modelBuilder.Entity<UserTeam>()
            .HasOne(pt => pt.Team)
            .WithMany(t => t.UserLinks)
            .HasForeignKey(pt => pt.TeamId);
    }
    public DbSet<User> Users { get; set; }
    public DbSet<Team> Teams { get; set; }
    public DbSet<Comment> Comments { get; set; }
    public DbSet<Post> Posts { get; set; }
}
