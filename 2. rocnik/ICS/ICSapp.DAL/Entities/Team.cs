using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ICSapp.DAL.Entities
{
    public class Team : EntityBase
    {
        /// <summary>
        /// Name of team
        /// </summary>
        public string TeamName { get; set; }
        /// <summary>
        /// Members of the team
        /// </summary>
        public ICollection<UserTeam> UserLinks { get; set; }
        /// <summary>
        /// Posts (messages) in the team.
        /// </summary>
        public virtual ICollection<Post> Posts { get; set; }

        private sealed class TeamComparerEqualityComparer : IEqualityComparer<Team>
        {
            public bool Equals(Team x, Team y)
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
                var b= string.Equals(x.TeamName, y.TeamName) &&
                       x.Id.Equals(y.Id);
                if (x.Posts != null && y.Posts != null)
                {
                    b = b && Int32.Equals(x.Posts.Count, y.Posts.Count);
                    foreach (var item in x.Posts)
                    {
                        b = b && y.Posts.Any(i => i.Id == item.Id);
                    }
                }
                if (x.UserLinks != null && y.UserLinks != null)
                {
                    b = b && Int32.Equals(x.UserLinks.Count, y.UserLinks.Count);
                    foreach (var item in x.UserLinks)
                    {
                        b = b && y.UserLinks.Any(i => i.Id == item.Id);
                    }
                }
                return b;
            }

            public int GetHashCode(Team obj)
            {
                unchecked
                {
                    var hashCode = (obj.TeamName != null ? obj.TeamName.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.UserLinks != null ? obj.UserLinks.GetHashCode() : 0);
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<Team> TeamComparer { get; } = new TeamComparerEqualityComparer();
    }
}
