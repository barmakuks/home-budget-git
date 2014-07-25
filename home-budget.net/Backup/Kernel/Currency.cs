using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
    public class Currency
    {
        public enum Place{Before, After}
        public int Code { get; set; }
        public string ISO { get; set; }
        public string Symbol { get; set; }
        public string ShortName { get; set; }
        public Place SymbolPlace { get; set; }
        public int SymbolPlaceFlag 
        {
            get {
                return SymbolPlace == Place.After ? 1 : 0;
            }
            set {
                SymbolPlace = (value == 0) ? Place.Before : Place.After;
            }
        }

        public int Parse(string amount) 
        {
            return 0;
        }
        public override string ToString()
        {
            return ISO;
        }
    }
    public class CurrencyList : Dictionary<int, Currency> 
    {
    }
}
