using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
    public class Amount
    {
        public Currency Currency { get; set; }
        public int Summa { get; set; }
        public string AsMoney { get { return ShortString(); } }
        public Amount(Currency currency, int amount) 
        {
            Summa = amount;
            Currency = currency;
        }
        public Amount() 
        {
            Summa = 0;
            Currency = null;
        }

        private string ShortString()
        {
            if (Currency.SymbolPlace == Currency.Place.Before)
                return (Currency.Symbol + " " + ToMoney()).Trim();
            else
                return (ToMoney() + " " + Currency.Symbol).Trim();
        }
        public static string ToMoney(int summa)
        {
            int abs = Math.Abs(summa);
            string sign = summa < 0 ? "-" : "";
            string res = "0.00";
            if (abs < 10)
                res = "0.0" + abs.ToString();
            else
                if (abs < 100)
                    res = "0." + abs.ToString();
                else 
                {
                    res = (abs / 100).ToString() + ".";
                    int kop = abs % 100;
                    if(kop < 10)
                        res = res + "0" + kop.ToString();
                    else
                        res = res + kop.ToString();
                }
            return sign + res;
        }
        public string ToMoney() 
        {
            return ToMoney(Summa);
        }
        public string LongString()
        {            
            return String.Format("{0} {1}", ToMoney(), Currency.ShortName);
        }
        public string ToString(bool shortFormat)
        {
            if (shortFormat)
                return ShortString();
            else
                return LongString();
        }
        public override string ToString()
        {
            return ToString(false);
        }

        public static string ToMoney(double p)
        {
            return ToMoney(Convert.ToInt32(Math.Round(p * 100.0)));
        }
        public static int Parse(string value, int def_value) 
        {
            double dbl = 0.0;
            if (Double.TryParse(value, out dbl)) 
            {
                return Convert.ToInt32(Math.Round(dbl * 100.0));
            }
            return def_value;
        }
    }
}
