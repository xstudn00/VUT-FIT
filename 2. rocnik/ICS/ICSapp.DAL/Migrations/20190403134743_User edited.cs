using Microsoft.EntityFrameworkCore.Migrations;

namespace ICSapp.DAL.Migrations
{
    public partial class Useredited : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.RenameColumn(
                name: "Descrption",
                table: "User",
                newName: "Description");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.RenameColumn(
                name: "Description",
                table: "User",
                newName: "Descrption");
        }
    }
}
