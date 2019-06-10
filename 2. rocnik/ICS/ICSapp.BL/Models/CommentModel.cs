using System;

namespace ICSapp.BL.Models
{
    public class CommentModel : EntityBaseModel
    {
        public DateTime Time { get; set; }

        public string MessageContent { get; set; }

        public virtual UserModel Author { get; set; }
    }
}
