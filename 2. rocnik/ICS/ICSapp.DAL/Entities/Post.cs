using System;
using System.Collections.Generic;

namespace ICSapp.DAL.Entities
{
    public class Post : EntityBase
    {
        /// <summary>
        /// The title of Post. About what is the text below.
        /// </summary>
        public string Title { get; set; }
        /// <summary>
        /// When was post created.
        /// </summary>
        public DateTime Time { get; set; }
        /// <summary>
        /// Content of post.
        /// </summary>
        public string MessageContent { get; set; }
        /// <summary>
        /// Author created the post.
        /// </summary>
        public virtual User Author { get; set; }
        /// <summary>
        /// Comments under post.
        /// </summary>
        public virtual ICollection<Comment> Contributions { get; set; }

        private sealed class PostEqualityComparer : IEqualityComparer<Post>
        {
            public bool Equals(Post x, Post y)
            {
                if (ReferenceEquals(x, y))
                {
                    return true;
                }

                if (ReferenceEquals(x, null))
                {
                    return false;
                }

                if (ReferenceEquals(y, null))
                {
                    return false;
                }

                if (x.GetType() != y.GetType())
                {
                    return false;
                }
                return string.Equals(x.MessageContent, y.MessageContent) &&
                    DateTime.Equals(x.Time, y.Time) &&
                    Equals(x.Contributions, y.Contributions) &&
                    x.Id.Equals(y.Id);
            }

            public int GetHashCode(Post obj)
            {
                unchecked
                {
                    var hashCode = (obj.MessageContent != null ? obj.MessageContent.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ obj.Id.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<Post> DescriptionNameIdComparer { get; } = new PostEqualityComparer();
    }
}