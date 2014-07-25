using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
    public class ReportItem
    {
        private Dictionary<int, Amount> _amounts = new Dictionary<int, Amount>();

        public DateTime DocDate { get; set; }
        public DocumentType DocType { get; set; }
        public Dictionary<int, Amount> Amounts { get { return _amounts; } }
    }
}
