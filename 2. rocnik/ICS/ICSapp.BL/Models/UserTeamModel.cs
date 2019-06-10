using System;
using System.Collections.Generic;
using System.Text;

namespace ICSapp.BL.Models
{
    public class UserTeamModel : EntityBaseModel
    {
        public Guid? TeamId { get; set; }
        public TeamModel Team { get; set; }
        public Guid UserId { get; set; }
        public UserModel User { get; set; }
    }
}
