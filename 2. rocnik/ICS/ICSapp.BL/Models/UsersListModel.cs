using System;
using System.Collections.Generic;
using System.Text;


namespace ICSapp.BL.Models
{
    public class UsersListModel : EntityBaseModel
    {
        public string FirstName { get; set; }

        public string Surname { get; set; }

        public string EmailAdress { get; set; }

        public byte[] Picture { get; set; }
    }
}
