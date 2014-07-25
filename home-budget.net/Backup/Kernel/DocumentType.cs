using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Kernel
{
    public class DocumentType
    {
        public enum DocTypeSign { Cost = -1, Movement = 0, Income = 1}
        public int Id { get; set; }
        public DocTypeSign Sign { get; set; }
        public string Name { get; set; }
        public int ParentId { get; set; }
        public override string ToString()
        {
            return Name;
        }
    }
    public class DocumentTypes : Dictionary<int, DocumentType> 
    {
    }
}
