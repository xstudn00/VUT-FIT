using System;

namespace ICSapp.DAL.Entities
{
    public class UserTeam : EntityBase
    {
        public Guid? TeamId { get; set; }
        public Guid UserId { get; set; }
        public byte Order { get; set; }
        public User User { get; set; }
        public Team Team { get; set; }
    }

}