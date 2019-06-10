using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Models;

namespace ICSapp.BL.Repositories
{
    public interface ITeamRepository
    {
        List<UsersListModel> GetUsersByTeam(Guid teamId);
        TeamModel GetById(Guid teamId);
        TeamModel AddTeamMate(Guid teamId, Guid userId);
        void RemoveTeam(Guid teamId);
        TeamModel RemoveTeamMate(Guid teamId, Guid userId);
        PostModel CreatePost(PostModel postModel, Guid userId);
        TeamModel AddPost(Guid teamId, Guid postId);
    }
}