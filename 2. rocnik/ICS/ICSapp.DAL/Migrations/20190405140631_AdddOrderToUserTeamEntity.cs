using Microsoft.EntityFrameworkCore.Migrations;

namespace ICSapp.DAL.Migrations
{
    public partial class AdddOrderToUserTeamEntity : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<byte>(
                name: "Order",
                table: "UserTeam",
                nullable: false,
                defaultValue: (byte)0);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "Order",
                table: "UserTeam");
        }
    }
}
