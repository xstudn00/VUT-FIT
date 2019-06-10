using System;
using System.Collections.Generic;
using System.Text;

namespace ICSapp.BL.Models
{
    public class UserModel : EntityBaseModel
    {
        public string EmailAdress { get; set; }

        public string Password { get; set; }

        public string FirstName { get; set; }

        public string Surname { get; set; }

        public string Description { get; set; }

        public byte[] Picture { get; set; }

        public bool Activity { get; set; }

        public DateTime LastActivity { get; set; }

        public virtual ICollection<UserTeamModel> Teams { get; set; }

        public virtual ICollection<CommentModel> Contributions { get; set; }
    }
}
