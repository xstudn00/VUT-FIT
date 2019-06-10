using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace ICSapp.DAL.Migrations
{
    public partial class ChangeUserTeamAttributesNamesTypes : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_Team_TeamId",
                table: "UserTeam");

            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_User_UserId",
                table: "UserTeam");

            migrationBuilder.DropPrimaryKey(
                name: "PK_UserTeam",
                table: "UserTeam");

            migrationBuilder.DropIndex(
                name: "IX_UserTeam_UserId",
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

            migrationBuilder.AddPrimaryKey(
                name: "PK_UserTeam",
                table: "UserTeam",
                columns: new[] { "UserId", "TeamId" });

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

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_Team_TeamId",
                table: "UserTeam");

            migrationBuilder.DropForeignKey(
                name: "FK_UserTeam_User_UserId",
                table: "UserTeam");

            migrationBuilder.DropPrimaryKey(
                name: "PK_UserTeam",
                table: "UserTeam");

            migrationBuilder.AlterColumn<Guid>(
                name: "TeamId",
                table: "UserTeam",
                nullable: true,
                oldClrType: typeof(Guid));

            migrationBuilder.AlterColumn<Guid>(
                name: "UserId",
                table: "UserTeam",
                nullable: true,
                oldClrType: typeof(Guid));

            migrationBuilder.AddPrimaryKey(
                name: "PK_UserTeam",
                table: "UserTeam",
                column: "Id");

            migrationBuilder.CreateIndex(
                name: "IX_UserTeam_UserId",
                table: "UserTeam",
                column: "UserId");

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
    }
}
