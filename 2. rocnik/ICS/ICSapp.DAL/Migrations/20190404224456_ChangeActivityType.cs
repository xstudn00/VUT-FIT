using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace ICSapp.DAL.Migrations
{
    public partial class ChangeActivityType : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_Team_TeamId",
                table: "UserTeam");

            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_User_UserId",
                table: "UserTeam");

            migrationBuilder.AlterColumn<Guid>(
                name: "UserId",
                table: "UserTeam",
                nullable: true,
                oldClrType: typeof(Guid));

            migrationBuilder.AlterColumn<Guid>(
                name: "TeamId",
                table: "UserTeam",
                nullable: true,
                oldClrType: typeof(Guid));

            migrationBuilder.AlterColumn<bool>(
                name: "Activity",
                table: "User",
                nullable: false,
                oldClrType: typeof(int));

            migrationBuilder.AddForeignKey(
                name: "FK_UserTeam_Team_TeamId",
                table: "UserTeam",
                column: "TeamId",
                principalTable: "Team",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_UserTeam_User_UserId",
                table: "UserTeam",
                column: "UserId",
                principalTable: "User",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_Team_TeamId",
                table: "UserTeam");

            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_User_UserId",
                table: "UserTeam");

            migrationBuilder.AlterColumn<Guid>(
                name: "UserId",
                table: "UserTeam",
                nullable: false,
                oldClrType: typeof(Guid),
                oldNullable: true);

            migrationBuilder.AlterColumn<Guid>(
                name: "TeamId",
                table: "UserTeam",
                nullable: false,
                oldClrType: typeof(Guid),
                oldNullable: true);

            migrationBuilder.AlterColumn<int>(
                name: "Activity",
                table: "User",
                nullable: false,
                oldClrType: typeof(bool));

            migrationBuilder.AddForeignKey(
                name: "FK_UserTeam_Team_TeamId",
                table: "UserTeam",
                column: "TeamId",
                principalTable: "Team",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_UserTeam_User_UserId",
                table: "UserTeam",
                column: "UserId",
                principalTable: "User",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);
        }
    }
}
