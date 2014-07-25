using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reports
{
    public class DocTypeInfo
    {
        public bool IsExpanded = false;
        public int Id { get; set; }
        public string Name { get; set; }
        public int ParentId = 0;
        public DocTypeInfo(int id, string name, int parent_id) 
        {
            Id = id; Name = name; ParentId = parent_id;
        }
    }
}
