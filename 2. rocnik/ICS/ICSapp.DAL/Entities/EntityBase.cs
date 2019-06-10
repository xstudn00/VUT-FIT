using System;

namespace ICSapp.DAL.Entities
{
    /// <summary>
    /// Base entity with primary key.
    /// </summary>
    public class EntityBase : IEntity
    {
        public Guid Id { get; set; }
    }
}
