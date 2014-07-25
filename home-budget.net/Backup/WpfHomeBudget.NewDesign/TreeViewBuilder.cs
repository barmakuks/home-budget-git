using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;

namespace WpfHomeBudget
{
    class TreeViewBuilder
    {
        private Dictionary<int, TreeViewItem> _added_items = new Dictionary<int, TreeViewItem>();
        private TreeView _tree;
        public TreeViewBuilder(TreeView tree) 
        {
            _tree = tree;
        }
        public TreeViewItem AddItem(Kernel.DocumentType doc_type) 
        {
            if (_added_items.ContainsKey(doc_type.Id))
                return _added_items[doc_type.Id];
            TreeViewItem item = new TreeViewItem();
            item.DataContext = doc_type;
            item.Header = doc_type.Name;
            if (doc_type.ParentId == 0)
            {
                _tree.Items.Add(item);
                _added_items.Add(doc_type.Id, item);
            }
            else
            {
                TreeViewItem parent_item = null;
                if (_added_items.ContainsKey(doc_type.ParentId))
                    parent_item = _added_items[doc_type.ParentId];
                else
                {
                    Kernel.DocumentType parent = Kernel.Globals.DocTypes[doc_type.ParentId];
                    parent_item = AddItem(parent);
                }
                parent_item.Items.Add(item);                
                _added_items.Add(doc_type.Id, item);
            }
            return item;
        }
        public TreeViewItem GetTreeItemById(int doc_type_id) 
        {
            if (_added_items.ContainsKey(doc_type_id))
                return _added_items[doc_type_id];
            else return null;
        }
    }
}
