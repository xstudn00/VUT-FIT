using System;
using System.Collections.Generic;

namespace ICSapp.BL.Models
{
    public class PostModel : EntityBaseModel
    {
        public string Title { get; set; }
        public DateTime Time { get; set; }

        public string MessageContent { get; set; }

        public virtual UserModel Author { get; set; }

        public virtual ICollection<CommentModel> Contributions { get; set; }
    }
}
