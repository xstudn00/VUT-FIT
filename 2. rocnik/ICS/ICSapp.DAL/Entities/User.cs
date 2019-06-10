using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace ICSapp.DAL.Entities
{
    public class User : EntityBase
    {
        /// <summary>
        /// EmailAdress primary key for logging in
        /// </summary>
        public string EmailAdress { get; set; }
        /// <summary>
        /// User's password
        /// </summary>
        public string Password { get; set; }
        /// <summary>
        /// User's first name 
        /// </summary>
        public string FirstName { get; set; }
        /// <summary>
        /// User's last name
        /// </summary>
        public string Surname { get; set; }
        /// <summary>
        /// User's descripiton of himself/herself/otherself
        /// </summary>
        public string Description { get; set; }
        /// <summary>
        /// Image converted into array of bytes.
        /// </summary>
        public byte[] Picture { get; set; }
        /// <summary>
        /// Info about being online/offline.
        /// </summary>
        public bool Activity { get; set; }
        /// <summary>
        /// Date and time of the last logout
        /// </summary>
        public DateTime LastActivity { get; set; }
        /// <summary>
        /// In which teams the member is.
        /// </summary>
        public virtual ICollection<UserTeam> TeamLinks { get; set; }
        /// <summary>
        /// User's comments.
        /// </summary>
        public virtual ICollection<Comment> Contributions { get; set; }

        private sealed class UserEqualityComparer : IEqualityComparer<User>
        {
            public bool Equals(User x, User y)
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
                var b = string.Equals(x.EmailAdress, y.EmailAdress) &&
                       string.Equals(x.Password, y.Password) &&
                       string.Equals(x.FirstName, y.FirstName) &&
                       string.Equals(x.Surname, y.Surname) &&
                       string.Equals(x.Description, y.Description) &&
                       x.Picture.SequenceEqual(y.Picture) &&
                       DateTime.Equals(x.LastActivity, y.LastActivity) &&
                       x.Activity.Equals(y.Activity) &&
                       x.Id.Equals(y.Id);
                if (x.Contributions != null && y.Contributions != null)
                {
                    b = b && Int32.Equals(x.Contributions.Count, y.Contributions.Count);
                    foreach (var item in x.Contributions)
                    {
                        b = b && y.Contributions.Any(i => i.Id == item.Id);
                    }
                }
                if (x.TeamLinks != null && y.TeamLinks != null)
                {
                    b = b && Int32.Equals(x.TeamLinks.Count, y.TeamLinks.Count);
                    foreach (var item in x.TeamLinks)
                    {
                        b = b && y.TeamLinks.Any(i => i.Id == item.Id);
                    }
                }
                return b;
            }

            public int GetHashCode(User obj)
            {
                unchecked
                {
                    var hashCode = (obj.EmailAdress != null ? obj.EmailAdress.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.Password != null ? obj.Password.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.FirstName != null ? obj.FirstName.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.Surname != null ? obj.Surname.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.Description != null ? obj.Description.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.Picture != null ? obj.Picture.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ obj.Activity.GetHashCode();
                    hashCode = (hashCode * 397) ^ (obj.LastActivity != null ? obj.LastActivity.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.TeamLinks != null ? obj.TeamLinks.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ (obj.Contributions != null ? obj.Contributions.GetHashCode() : 0);
                    hashCode = (hashCode * 397) ^ obj.Id.GetHashCode();
                    return hashCode;
                }
            }
        }

        public static IEqualityComparer<User> UserComparer { get; } = new UserEqualityComparer();

    }
}

