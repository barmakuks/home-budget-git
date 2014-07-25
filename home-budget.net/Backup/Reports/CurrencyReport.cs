using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reports
{
    /// <summary>
    /// Одна строчка в отчете за один день по одной валюте
    /// </summary>
    public class CurrencyReport
    {
        public string Currency { get; set; }
        public int Money { get; set; }
        public CurrencyReport(string currency) 
        {
            Currency = currency;
        }
        public override string ToString()
        {
            if (Money != 0)
            {
                return Kernel.Amount.ToMoney(Money) + " " + Currency;
            }
            else return String.Empty;            
        }
        public string MoneyString 
        {
            get{
                if (Money != 0)
                {
                    return Kernel.Amount.ToMoney(Money);
                }
                else return String.Empty;
            }
        }
    }
}
