using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Windows.Automation.Peers;
using System.Windows.Automation.Provider;

namespace WpfHomeBudget
{
    /// <summary>
    /// Interaction logic for DocumentDialog.xaml
    /// </summary>
    public partial class DocumentInOutDialog : Window
    {
        private Kernel.DocumentType.DocTypeSign _sign;
        private int _docs_created = 0;
        private Kernel.Document _current_doc = null;
        TreeViewBuilder _tree_builder = null;

        private DocumentInOutDialog()
        {
            InitializeComponent();
        }
        public DocumentInOutDialog(Kernel.DocumentType.DocTypeSign sign): this()
        {
            CreateDocTypeTree(sign);
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
        /// <summary>
        /// Начинает редактирование и создание новых документов
        /// </summary>
        /// <param name="document">Певый документ для редактирования</param>
        /// <returns>Количество созданных документов</returns>
        internal int EditDocument(Kernel.Document document, Window windowOwner) 
        {
            SetData(document);            
            this.Owner = windowOwner;
            ShowDialog();
            return _docs_created;            
        }

        /// <summary>
        /// Инициирует дерево типов документов
        /// </summary>
        /// <param name="sign">Тип дерева</param>
        private void CreateDocTypeTree(Kernel.DocumentType.DocTypeSign sign)
        {
            _sign = sign;
            _tree_builder = new TreeViewBuilder(treeDocTypes);
            treeDocTypes.Items.Clear();            
            foreach (Kernel.DocumentType doc_type in Kernel.Globals.DocTypes.Values) 
            {
                if (doc_type.Sign == sign) 
                {
                    _tree_builder.AddItem(doc_type);
                }
            }
            treeDocTypes.Items.SortDescriptions.Add(new System.ComponentModel.SortDescription("Content", System.ComponentModel.ListSortDirection.Ascending));
        }

        /// <summary>
        /// Считывает джанные с формы в документ
        /// </summary>
        /// <param name="document">Ссылка на документ</param>
        private void GetData(ref Kernel.Document document)
        {
            document.DocDate = txtDate.SelectedDate.Value;
            if (treeDocTypes.SelectedItem is TreeViewItem)
                if ((treeDocTypes.SelectedItem as TreeViewItem).DataContext is Kernel.DocumentType)
                    document.DocType = (treeDocTypes.SelectedItem as TreeViewItem).DataContext as Kernel.DocumentType;
            document.Note = txtNote.Text;
            if(txtShop.IsVisible)
                document.Shop = txtShop.Text;
            Kernel.Transaction tran = new Kernel.Transaction();
            tran = new Kernel.Transaction();
            tran.Account = cbxAccount.SelectedValue as Kernel.Account;
            tran.Amount = new Kernel.Amount(cbxCurrency.SelectedItem as Kernel.Currency, Convert.ToInt32(Convert.ToDouble(txtSumma.Text) * 100));

            switch(_sign)
            {
                case Kernel.DocumentType.DocTypeSign.Cost:
                    document.Credit = tran;
                    break;
                case Kernel.DocumentType.DocTypeSign.Income:
                    document.Debit = tran;
                    break;
            }
            
        }
        /// <summary>
        /// Заполняет форму данными из документа
        /// </summary>
        /// <param name="document">Исходный документ</param>
        private void SetData(Kernel.Document document)
        {
            _current_doc = document;
            cbxAccount.ItemsSource = Kernel.Globals.ActiveAccounts.Values;
            cbxCurrency.ItemsSource = Kernel.Globals.Currency.Values;
            txtDate.SelectedDate = document.DocDate;
            txtNote.Text = document.Note;
            Kernel.Transaction transaction = null;
            switch (_sign) 
            {
                case Kernel.DocumentType.DocTypeSign.Income:
                    transaction = document.Debit;
                    txtShop.Visibility = Visibility.Hidden;
                    lblShop.Visibility = Visibility.Hidden;                    
                    break;
                case Kernel.DocumentType.DocTypeSign.Cost:
                    transaction = document.Credit;
                    txtShop.Text = document.Shop;
                    txtShop.ItemsSource = db.Database.GetShopsList();                    
                    break;
            }
            if (transaction == null)
                return;
            if (transaction.Amount.Summa != 0)
                txtSumma.Text = transaction.Amount.ToMoney();
            else
                txtSumma.Text = "";
            if (transaction.Account != null && transaction.Account.Id != 0)
            {
                if (!Kernel.Globals.ActiveAccounts.ContainsKey(transaction.Account.Id))
                    cbxAccount.ItemsSource = Kernel.Globals.Accounts.Values;
                cbxAccount.SelectedItem = Kernel.Globals.Accounts[transaction.Account.Id];
                cbxCurrency.SelectedItem = Kernel.Globals.Currency[transaction.Amount.Currency.Code];
            }
            else
                cbxAccount.SelectedIndex = 0;
            txtSumma.Focus();
            if (document.DocType != null) 
            {
                TreeViewItem item = _tree_builder.GetTreeItemById(document.DocType.Id);
                if (item != null)
                {
                    item.IsSelected = true;
                    while (item.Parent is TreeViewItem)
                    {
                        item = item.Parent as TreeViewItem;
                        item.IsExpanded = true;
                    }                    
                }
                Dispatcher.BeginInvoke(new Action(ScrollToSelectedItem));
            }
            else
            {
                if (treeDocTypes.Items.Count > 0)
                {
                    (treeDocTypes.Items[0] as TreeViewItem).IsExpanded = true;
                }

            }    
                
        }

        private void ScrollToSelectedItem()
        {
            //treeDocTypes.UpdateLayout();
            TreeViewItem item = treeDocTypes.SelectedItem as TreeViewItem;
            item.BringIntoView();
        }


        private void cbxAccount_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cbxAccount.SelectedItem is Kernel.Account)
            {
                cbxCurrency.SelectedItem = Kernel.Globals.Currency[(cbxAccount.SelectedItem as Kernel.Account).DefaultCurrency];
            }
            else cbxCurrency.SelectedIndex = 0;
        }

        private void txtSumma_GotFocus(object sender, RoutedEventArgs e)
        {
            txtSumma.SelectAll();
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            if (CheckDocumentType() && CheckSumma())
            {
                SaveCurrentDocument();
                DialogResult = true;
                Close();
            }
        }
        /// <summary>
        /// Считывает данные в текущий документ и сохраняет его в БД
        /// </summary>
        private void SaveCurrentDocument()
        {
            GetData(ref _current_doc);
            db.Database.WriteDocument(_current_doc);
            _docs_created++;
        }

        /// <summary>
        /// Проверяет, выбран ли тип документа
        /// </summary>
        /// <returns></returns>
        private bool CheckDocumentType() 
        {
            // проверяем тип документа
            if (treeDocTypes.SelectedItem is TreeViewItem && (treeDocTypes.SelectedItem as TreeViewItem).DataContext is Kernel.DocumentType)
                return true;
            else
            {
                ToolTip tt = new ToolTip();
                tt.Content = "Укажите тип дохода или расхода";
                tt.PlacementTarget = treeDocTypes;
                tt.Placement = System.Windows.Controls.Primitives.PlacementMode.Top;
                tt.IsOpen = true;
                DispatcherTimer timer = new DispatcherTimer { Interval = new TimeSpan(0, 0, 3), IsEnabled = true };
                timer.Tick += new EventHandler(delegate(object timerSender, EventArgs timerArgs)
                {
                    if (tt != null)
                    {
                        tt.IsOpen = false;
                    }
                    tt = null;
                    timer = null;
                });
            }
            return false;
        }
        /// <summary>
        /// Проверяет введена ли сумма
        /// </summary>
        /// <returns>true, если форма заполнена всеми необходимыми данными</returns>
        private bool CheckSumma()
        {
            double summa;
            //проверяем сумму
            if (!Double.TryParse(txtSumma.Text, out summa))
                return false;
            return true;
        }

        private void btnMore_Click(object sender, RoutedEventArgs e)
        {
            if (!CheckDocumentType())
                return;
            if (CheckSumma())
            {
                SaveCurrentDocument();
                _current_doc = _current_doc.CopyDocument();
                _current_doc.ClearAmount();
                SetData(_current_doc);
            }
            else Close();
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItem item = e.Source as MenuItem;
            if (item.CommandParameter.ToString() == "CREATE")
            {
                CreateSubDocType();
            }
            if (item.CommandParameter.ToString() == "RENAME")
            {
                RenameSelectedDocType();
            }
            if (item.CommandParameter.ToString() == "DELETE")
            {
            }
        }

        private void CreateSubDocType()
        {
            TreeViewItem parent_item = treeDocTypes.SelectedItem as TreeViewItem;
            if (parent_item != null)
            {
                string value = "";
                if (InputDialog.Show("Создание", "Введите имя", ref value))
                {
                    Kernel.DocumentType parent_doc_type = parent_item.DataContext as Kernel.DocumentType;
                    if (parent_doc_type != null)
                    {                        
                        Kernel.DocumentType doc_type = new Kernel.DocumentType();
                        doc_type.Sign = _sign;
                        doc_type.ParentId = parent_doc_type.Id;
                        doc_type.Name = value;
                        db.Database.WriteDocumentType(doc_type);
                        Kernel.Globals.DocTypes.Add(doc_type.Id, doc_type);
                        TreeViewItem new_item = _tree_builder.AddItem(doc_type);
                        parent_item.IsExpanded = true;
                        new_item.IsSelected = true;
                    }
                }
            }
        }

        private void RenameSelectedDocType() 
        {
            TreeViewItem item = treeDocTypes.SelectedItem as TreeViewItem;
            if (item != null) 
            {
                string value = item.Header.ToString();
                if(InputDialog.Show("Переименовать","Введите имя", ref value))
                {                    
                    Kernel.DocumentType doc_type = item.DataContext as Kernel.DocumentType ;
                    if (doc_type != null) 
                    {
                        item.Header = value;
                        doc_type.Name = value;
                        db.Database.WriteDocumentType(doc_type);
                    }
                }                
            }
        }
    }
}
