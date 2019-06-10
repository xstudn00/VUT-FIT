using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace ICSapp.DAL.Migrations
{
    public partial class MessageRemoved_ChangedUserSurname : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Message_User_AuthorId",
                table: "Message");

            migrationBuilder.DropForeignKey(
                name: "FK_Message_Message_PostId",
                table: "Message");

            migrationBuilder.DropForeignKey(
                name: "FK_Message_Message_SuperiorPostId",
                table: "Message");

            migrationBuilder.DropForeignKey(
                name: "FK_Message_User_Post_AuthorId",
                table: "Message");

            migrationBuilder.DropForeignKey(
                name: "FK_Message_Team_TeamId",
                table: "Message");

            migrationBuilder.DropPrimaryKey(
                name: "PK_Message",
                table: "Message");

            migrationBuilder.DropIndex(
                name: "IX_Message_PostId",
                table: "Message");

            migrationBuilder.DropIndex(
                name: "IX_Message_SuperiorPostId",
                table: "Message");

            migrationBuilder.DropIndex(
                name: "IX_Message_Post_AuthorId",
                table: "Message");

            migrationBuilder.DropColumn(
                name: "PostId",
                table: "Message");

            migrationBuilder.DropColumn(
                name: "SuperiorPostId",
                table: "Message");

            migrationBuilder.DropColumn(
                name: "Discriminator",
                table: "Message");

            migrationBuilder.DropColumn(
                name: "Post_AuthorId",
                table: "Message");

            migrationBuilder.RenameTable(
                name: "Message",
                newName: "Post");

            migrationBuilder.RenameColumn(
                name: "SureName",
                table: "User",
                newName: "Surname");

            migrationBuilder.RenameIndex(
                name: "IX_Message_TeamId",
                table: "Post",
                newName: "IX_Post_TeamId");

            migrationBuilder.RenameIndex(
                name: "IX_Message_AuthorId",
                table: "Post",
                newName: "IX_Post_AuthorId");

            migrationBuilder.AddPrimaryKey(
                name: "PK_Post",
                table: "Post",
                column: "Id");

            migrationBuilder.CreateTable(
                name: "Comment",
                columns: table => new
                {
                    Id = table.Column<Guid>(nullable: false),
                    Time = table.Column<DateTime>(nullable: false),
                    MessageContent = table.Column<string>(nullable: true),
                    AuthorId = table.Column<Guid>(nullable: true),
                    PostId = table.Column<Guid>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Comment", x => x.Id);
                    table.ForeignKey(
                        name: "FK_Comment_User_AuthorId",
                        column: x => x.AuthorId,
                        principalTable: "User",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Restrict);
                    table.ForeignKey(
                        name: "FK_Comment_Post_PostId",
                        column: x => x.PostId,
                        principalTable: "Post",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Restrict);
                });

            migrationBuilder.CreateIndex(
                name: "IX_Comment_AuthorId",
                table: "Comment",
                column: "AuthorId");

            migrationBuilder.CreateIndex(
                name: "IX_Comment_PostId",
                table: "Comment",
                column: "PostId");

            migrationBuilder.AddForeignKey(
                name: "FK_Post_User_AuthorId",
                table: "Post",
                column: "AuthorId",
                principalTable: "User",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Post_Team_TeamId",
                table: "Post",
                column: "TeamId",
                principalTable: "Team",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Post_User_AuthorId",
                table: "Post");

            migrationBuilder.DropForeignKey(
                name: "FK_Post_Team_TeamId",
                table: "Post");

            migrationBuilder.DropTable(
                name: "Comment");

            migrationBuilder.DropPrimaryKey(
                name: "PK_Post",
                table: "Post");

            migrationBuilder.RenameTable(
                name: "Post",
                newName: "Message");

            migrationBuilder.RenameColumn(
                name: "Surname",
                table: "User",
                newName: "SureName");

            migrationBuilder.RenameIndex(
                name: "IX_Post_TeamId",
                table: "Message",
                newName: "IX_Message_TeamId");

            migrationBuilder.RenameIndex(
                name: "IX_Post_AuthorId",
                table: "Message",
                newName: "IX_Message_AuthorId");

            migrationBuilder.AddColumn<Guid>(
                name: "PostId",
                table: "Message",
                nullable: true);

            migrationBuilder.AddColumn<Guid>(
                name: "SuperiorPostId",
                table: "Message",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "Discriminator",
                table: "Message",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<Guid>(
                name: "Post_AuthorId",
                table: "Message",
                nullable: true);

            migrationBuilder.AddPrimaryKey(
                name: "PK_Message",
                table: "Message",
                column: "Id");

            migrationBuilder.CreateIndex(
                name: "IX_Message_PostId",
                table: "Message",
                column: "PostId");

            migrationBuilder.CreateIndex(
                name: "IX_Message_SuperiorPostId",
                table: "Message",
                column: "SuperiorPostId");

            migrationBuilder.CreateIndex(
                name: "IX_Message_Post_AuthorId",
                table: "Message",
                column: "Post_AuthorId");

            migrationBuilder.AddForeignKey(
                name: "FK_Message_User_AuthorId",
                table: "Message",
                column: "AuthorId",
                principalTable: "User",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Message_Message_PostId",
                table: "Message",
                column: "PostId",
                principalTable: "Message",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Message_Message_SuperiorPostId",
                table: "Message",
                column: "SuperiorPostId",
                principalTable: "Message",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Message_User_Post_AuthorId",
                table: "Message",
                column: "Post_AuthorId",
                principalTable: "User",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Message_Team_TeamId",
                table: "Message",
                column: "TeamId",
                principalTable: "Team",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }
    }
}
