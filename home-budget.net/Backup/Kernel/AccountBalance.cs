using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
    public class AccountBalance
    {
        private List<Amount> _balance = new List<Amount>();
        
        public Account Account { get; set; }
        public List<Amount> Balance {
            get {
                return _balance;
            }
        }
    }
}
