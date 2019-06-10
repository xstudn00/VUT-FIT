using System;
using System.Collections.Generic;
using System.Text;

namespace ICSapp.BL.Models
{
    public class TeamModel : EntityBaseModel
    {
        public string TeamName { get; set; }

        public virtual ICollection<UserTeamModel> Members { get; set; }

        public virtual ICollection<PostModel> Posts { get; set; }
    }
}
