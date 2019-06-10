using System;
using System.Collections.Generic;

namespace ICSapp.DAL.Entities
{
    public class Comment : EntityBase
    {
        /// <summary>
        /// When was comment created.
        /// </summary>
        public DateTime Time { get; set; }
        /// <summary>
        /// Content of comment.
        /// </summary>
        public string MessageContent { get; set; }
        /// <summary>
        /// Author created the comment.
        /// </summary>
        public virtual User Author { get; set; }

        private sealed class CommentEqualityComparer : IEqualityComparer<Comment>
        {
            public bool Equals(Comment x, Comment y)
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
                    Equals(x.Author, y.Author) &&
                    x.Id.Equals(y.Id);
            }

            public int GetHashCode(Comment obj)
            {
                unchecked
                {
                    var hashCode = (obj.MessageContent != null ? obj.MessageContent.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ obj.Id.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<Comment> CommentComparer { get; } = new CommentEqualityComparer();
    }
}