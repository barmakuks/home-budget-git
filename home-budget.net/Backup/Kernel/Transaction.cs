using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
    public class Transaction
    {
        public Transaction() 
        {
            Account = new Account();
            Amount = new Amount();
        }
        public Account Account { get; set; }
        public Amount Amount { get; set; }
    }
}
