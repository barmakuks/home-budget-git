using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Communal
{
    public class CommunalBalance
    {
        public CommunalType Type { get; set; }
        public int Balance { get; set; }
        public string BalanceString {
            get {
                return ToMoney(Balance);
            }
        }
        public int BalanceAlt { get; set; }
        public string BalanceAltString
        {
            get
            {
                return ToMoney(BalanceAlt);
            }
        }

        private static string ToMoney(int summa)
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
                    if (kop < 10)
                        res = res + "0" + kop.ToString();
                    else
                        res = res + kop.ToString();
                }
            return sign + res;
        }

    }
}
