using System;
using System.Collections.Generic;
using System.Text;
using ICSapp.BL.Models;

namespace ICSapp.BL.Messages
{
    public class ActiveTeamMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}
