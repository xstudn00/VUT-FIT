using System;

namespace ICSapp.BL.Messages
{
    public class ActiveUserMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}
