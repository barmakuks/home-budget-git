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

namespace WpfHomeBudget
{
    /// <summary>
    /// Interaction logic for DocumentDialog.xaml
    /// </summary>
    public partial class DocumentMoveDialog : Window
    {
        private Kernel.Document _current_doc = null;

        private DocumentMoveDialog ()
        {
            InitializeComponent();
        }
        public DocumentMoveDialog(Kernel.DocumentType.DocTypeSign sign)
            : this()
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
        internal int EditDocument(Kernel.Document document) 
        {
            SetData(document);
            ShowDialog();
            return 1; 
        }

        /// <summary>
        /// Инициирует дерево типов документов
        /// </summary>
        /// <param name="sign">Тип дерева</param>
        private void CreateDocTypeTree(Kernel.DocumentType.DocTypeSign sign)
        {
            cbxDocType.Items.Clear();            
            foreach (Kernel.DocumentType doc_type in Kernel.Globals.DocTypes.Values) 
            {
                if (doc_type.Sign == sign) 
                {
                    cbxDocType.Items.Add(doc_type);
                }
            }            
        }

        /// <summary>
        /// Считывает джанные с формы в документ
        /// </summary>
        /// <param name="document">Ссылка на документ</param>
        private void GetData(ref Kernel.Document document)
        {
            document.DocDate = txtDate.SelectedDate.Value;
            if (cbxDocType.SelectedItem is Kernel.DocumentType)
                document.DocType = cbxDocType.SelectedItem as Kernel.DocumentType;
            document.Note = txtNote.Text;
            document.Shop = "";
            document.Credit = new Kernel.Transaction();
            document.Credit.Account = cbxAccountFrom.SelectedValue as Kernel.Account;
            document.Credit.Amount = new Kernel.Amount(cbxCurrencyFrom.SelectedItem as Kernel.Currency, Convert.ToInt32(Convert.ToDouble(txtSummaFrom.Text) * 100));
            document.Debit = new Kernel.Transaction();
            document.Debit.Account = cbxAccountTo.SelectedValue as Kernel.Account;
            document.Debit.Amount = new Kernel.Amount(cbxCurrencyTo.SelectedItem as Kernel.Currency, Convert.ToInt32(Convert.ToDouble(txtSummaTo.Text) * 100));
        }
        /// <summary>
        /// Заполняет форму данными из документа
        /// </summary>
        /// <param name="document">Исходный документ</param>
        private void SetData(Kernel.Document document)
        {
            _current_doc = document;
            cbxAccountFrom.ItemsSource = Kernel.Globals.Accounts.Values;
            cbxAccountTo.ItemsSource = Kernel.Globals.Accounts.Values;
            cbxCurrencyFrom.ItemsSource = Kernel.Globals.Currency.Values;
            cbxCurrencyTo.ItemsSource = Kernel.Globals.Currency.Values;
            txtDate.SelectedDate = document.DocDate;
            txtNote.Text = document.Note;

            if (document.Debit.Amount.Summa != 0) txtSummaTo.Text = document.Debit.Amount.ToMoney();
            else txtSummaTo.Text = "";
            if (document.Debit.Account != null && document.Debit.Account.Id > 0)
            {
                cbxAccountTo.SelectedItem = Kernel.Globals.Accounts[document.Debit.Account.Id];
                cbxCurrencyTo.SelectedItem = Kernel.Globals.Currency[document.Debit.Amount.Currency.Code];
            }
            else cbxAccountTo.SelectedIndex = 0;
            txtSummaFrom.Focus();

            if (document.Credit.Amount.Summa != 0) txtSummaFrom.Text = document.Credit.Amount.ToMoney();
            else txtSummaFrom.Text = "";
            if (document.Credit.Account != null && document.Credit.Account.Id > 0)
            {
                cbxAccountFrom.SelectedItem = Kernel.Globals.Accounts[document.Credit.Account.Id];
                cbxCurrencyFrom.SelectedItem = Kernel.Globals.Currency[document.Credit.Amount.Currency.Code];
            }
            else cbxAccountFrom.SelectedIndex = 0;

            if (document.DocType != null) 
            {
                foreach (Kernel.DocumentType doc_type in cbxDocType.Items) 
                {
                    if(doc_type.Id == document.DocType.Id)
                        cbxDocType.SelectedItem = doc_type;
                }                
            }                
        }

        private void cbxAccountFrom_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cbxAccountFrom.SelectedItem is Kernel.Account)
            {
                cbxCurrencyFrom.SelectedItem = Kernel.Globals.Currency[(cbxAccountFrom.SelectedItem as Kernel.Account).DefaultCurrency];
            }
            else cbxCurrencyFrom.SelectedIndex = 0;
        }
        private void cbxAccountTo_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cbxAccountTo.SelectedItem is Kernel.Account)
            {
                cbxCurrencyTo.SelectedItem = Kernel.Globals.Currency[(cbxAccountTo.SelectedItem as Kernel.Account).DefaultCurrency];
            }
            else cbxCurrencyTo.SelectedIndex = 0;
        }


        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            if (CheckData())
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
        }
        /// <summary>
        /// Проверяет полноту введенной информации
        /// </summary>
        /// <returns>true, если форма заполнена всеми необходимыми данными</returns>
        private bool CheckData()
        {
            double summa;
            //проверяем сумму
            if (!Double.TryParse(txtSummaFrom.Text, out summa))
                return false;
            if (!Double.TryParse(txtSummaTo.Text, out summa))
                return false;
            // проверяем тип документа
            if (cbxDocType.SelectedItem is Kernel.DocumentType)
                return true;
            return false;
        }

        private void txtSummaFrom_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (cbxCurrencyFrom.SelectedIndex == cbxCurrencyTo.SelectedIndex) 
            {
                txtSummaTo.Text = txtSummaFrom.Text;
            }
        }

    }
}
