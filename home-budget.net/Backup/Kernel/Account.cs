using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Kernel
{
    public class Account
    {
        public int Id { get; set; }
        public bool IsActive { get; set; }
        public string Bank { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        //public int GroupId { get; set; }        
        public int DefaultCurrency { get; set; }
        public string DefaultCurrencyName {
            get {
                if (Globals.Currency.ContainsKey(DefaultCurrency))
                    return Globals.Currency[DefaultCurrency].ISO;
                else return "";
            }
        }
        public int SortOrder { get; set; }        

        public override string ToString()
        {
            if (String.IsNullOrEmpty(Bank))
                return Name;
            else
                return Bank + " - " + Name;
        }
        public Account() 
        {
            Id = 0;
            IsActive = true;
            ForegroundColor = 0xFF000000;
        }

        public uint ForegroundColor { get; set; }
    }
    public class AccountList : Dictionary<int, Account> 
    {
    }
}
