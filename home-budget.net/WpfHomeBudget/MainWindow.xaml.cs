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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        bool _load_finished = false;
        public MainWindow()
        {
            StartWindow.StaticWindow = new StartWindow();
            StartWindow.StaticWindow.Show();
            InitializeComponent();
        }

        private void BtnClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
        private void BtnMaximize_Click(object sender, RoutedEventArgs e)
        {
            this.WindowState = (this.WindowState == WindowState.Maximized) ? WindowState.Normal : WindowState.Maximized;
        }
        private void BtnMinimize_Click(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Minimized;
        }
        private void pnlCaption_MouseDown(object sender, MouseButtonEventArgs e)
        {
            //if (e.ClickCount >= 2) 
            {
                BtnMaximize_Click(sender, RoutedEventArgs.Empty as RoutedEventArgs);
            }
        }

        #region Move window routine
        Vector div = new Vector();
        private void lblCaption_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {            
            div = Point.Subtract(new Point(this.Left, this.Top), this.PointToScreen(Mouse.GetPosition(this)));
            Mouse.Capture(lblCaption);
            //div.Negate();
        }
        private void lblCaption_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (div.X != 0 && div.Y != 0)
            {
                Point loc = Point.Add(this.PointToScreen(Mouse.GetPosition(this)), div);
                this.Left = loc.X;
                this.Top = loc.Y;
            }
        }
        private void lblCaption_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            div.X = 0;
            div.Y = 0;
            Mouse.Capture(null);
        }
        #endregion

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            pnlFilter.Height = pnlFilter.Height == 200 ? 30 : 200;
        }
        private void SetPeriod()
        {
            DateTime today = DateTime.Today;
            int dayOfWeek = Convert.ToInt32(today.DayOfWeek) - 1;
            switch (cbxPeriod.SelectedIndex)
            {
                case 0: //Сегодня 
                    dateMin.SelectedDate = today;
                    dateMax.SelectedDate = today;
                    break;
                case 1: //Вчера 
                    dateMin.SelectedDate = today.AddDays(-1);
                    dateMax.SelectedDate = today.AddDays(-1);
                    break;
                case 2: //Эта неделя                     
                    dateMin.SelectedDate = today.AddDays(-1 * dayOfWeek);
                    dateMax.SelectedDate = today;
                    break;
                case 3: //Прошлая неделя 
                    dateMin.SelectedDate = today.AddDays(-1 * dayOfWeek - 7);
                    dateMax.SelectedDate = today.AddDays(-1 * dayOfWeek - 1);
                    break;
                case 4: //Этот месяц 
                    dateMin.SelectedDate = new DateTime(today.Year, today.Month, 1);
                    dateMax.SelectedDate = DateTime.Today;
                    break;
                case 5: //Прошлый месяц 
                    dateMin.SelectedDate = (today.AddMonths(-1)).AddDays(-today.Day + 1);
                    dateMax.SelectedDate = today.AddDays(-today.Day);
                    break;
                case 6: //Этот квартал 
                    dateMin.SelectedDate = today.AddMonths(-(today.Month - 1) % 3).AddDays(-today.Day + 1);
                    dateMax.SelectedDate = dateMin.SelectedDate.Value.AddMonths(3).AddDays(-1);
                    break;
                case 7: //Прошлый квартал 
                    dateMin.SelectedDate = today.AddMonths(-(today.Month - 1) % 3 - 3).AddDays(-today.Day + 1);
                    dateMax.SelectedDate = dateMin.SelectedDate.Value.AddMonths(3).AddDays(-1);
                    break;
                case 8: //Этот год 
                    dateMin.SelectedDate = new DateTime(today.Year, 1, 1);
                    dateMax.SelectedDate = new DateTime(today.Year, 12, 31); ;
                    break;
                case 9: //Прошлый год 
                    dateMin.SelectedDate = new DateTime(today.Year - 1, 1, 1);
                    dateMax.SelectedDate = new DateTime(today.Year - 1, 12, 31); ;
                    break;
                case 10: //Весь период 
                    dateMin.SelectedDate = DateTime.MinValue;
                    dateMax.SelectedDate = DateTime.MaxValue;
                    break;
                default:
                    dateMin.SelectedDate = DateTime.Today;
                    dateMax.SelectedDate = DateTime.Today;
                    break;
            }
            dateMin.DisplayDate = dateMin.SelectedDate.Value;
            dateMax.DisplayDate = dateMax.SelectedDate.Value;
        }
        private void cbxPeriod_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SetPeriod();
            if (_load_finished) 
            {
                UpdateDocumentsGrid();
                UpdateReport();
            }
        }
        private void date_SelectedDatesChanged(object sender, SelectionChangedEventArgs e)
        {
            if (dateMin.SelectedDate != null)
                lblMinDate.Content = dateMin.SelectedDate.Value.ToString("dd.MM.yyyy");
            if (dateMax.SelectedDate != null)
                lblMaxDate.Content = dateMax.SelectedDate.Value.ToString("dd.MM.yyyy");
        }
        private void btnShowPeriod_Click(object sender, RoutedEventArgs e)
        {
            if (pnlFilter.Height < 50)
            {
                pnlFilter.Height = 220;
                Uri uri = new Uri(@"pack://application:,,,/Resources/Up.png");
                BitmapImage bitmap = new BitmapImage(uri);
                Image img = new Image();
                imgButton.Source = bitmap;
                dateMin.Visibility = Visibility.Visible;
                dateMax.Visibility = Visibility.Visible;
            }
            else 
            {
                pnlFilter.Height = 48;
                Uri uri = new Uri("pack://application:,,,/Resources/Down.png");
                BitmapImage bitmap = new BitmapImage(uri);
                Image img = new Image();
                imgButton.Source = bitmap;
                dateMin.Visibility = Visibility.Hidden;
                dateMax.Visibility = Visibility.Hidden;
                UpdateDocumentsGrid();
            }
            
        }
        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (tcMain.SelectedItem == tpgDocs)
            {
                tpgDocs.Background = tcMain.Background;
                tpgReports.Background = new SolidColorBrush(Color.FromArgb(0xFF, 0x30, 0x30, 0x30));
            }
            else 
            {
                tpgDocs.Background = new SolidColorBrush(Color.FromArgb(0xFF, 0x30, 0x30, 0x30));
                tpgReports.Background = tcMain.Background;
            }
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            _load_finished = false;
            cbxPeriod.SelectedIndex = 0;
            SetAccountFilter();
            SetCurrencyFilter();
            UpdateAll();
            _load_finished = true;
            StartWindow.StaticWindow.Close();            
        }
        private void dateMin_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            btnShowPeriod_Click(sender, new RoutedEventArgs());
        }
        private void dateAccount_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            UpdateAccountBalance();
        }
        private void UpdateAccountBalance() 
        {
            List<Kernel.AccountBalance> balance = new List<Kernel.AccountBalance>(db.Database.GetAccountBalance(dateAccount.SelectedDate.Value));
            balance.Add(db.Database.GetTotalBalance(balance));
            itemsAccountBalance.ItemsSource = balance;
            lblBalanceCaption.Content = "Баланс счетов на " + dateAccount.SelectedDate.Value.ToString("dd.MM.yyyy");
        }
        private void UpdateDocumentsGrid() 
        {
            grdDocuments.AutoGenerateColumns = false;
            Kernel.Account account = null;
            Kernel.Currency currency = null;
            if(cbxAccount.SelectedIndex > 0)
                account = cbxAccount.SelectedItem as Kernel.Account;
            if (cbxCurrency.SelectedIndex > 0)
                currency = cbxCurrency.SelectedItem as Kernel.Currency;
            grdDocuments.ItemsSource = db.Database.GetDocuments(dateMin.SelectedDate.Value, dateMax.SelectedDate.Value, account, currency);
            if (grdDocuments.Items.Count > 1) 
            {
                grdDocuments.SelectedIndex = 0;
                grdDocuments.UpdateLayout();
                if (grdDocuments.IsVisible)
                    grdDocuments.ScrollIntoView(grdDocuments.SelectedItem);
            }
            /*if (grdDocuments.SelectedItem != null)
                */
        }
        private void SetAccountFilter() 
        {
            cbxAccount.Items.Clear();
            cbxAccount.Items.Add("<Все счета>");
            foreach (Kernel.Account account in db.Database.GetAccountList().Values)
            {
                cbxAccount.Items.Add(account);
            }
            dateAccount.SelectedDate = DateTime.Now;
            cbxAccount.SelectedIndex = 0;
        }
        private void SetCurrencyFilter() 
        {
            cbxCurrency.Items.Clear();
            cbxCurrency.Items.Add("<Все>");
            foreach (Kernel.Currency cur in db.Database.GetCurrencyList().Values)
            {
                cbxCurrency.Items.Add(cur);
            }            
            cbxCurrency.SelectedIndex = 0;
        }
        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            UpdateAll();
        }
        private void btnAccounts_Click(object sender, RoutedEventArgs e)
        {
            AccountsListWindow acc = new AccountsListWindow();
            acc.ShowDialog();
            SetAccountFilter();
            UpdateAccountBalance();            
            UpdateDocumentsGrid();
        }

        private void btnCurrency_Click(object sender, RoutedEventArgs e)
        {
            CurrencyListWindow cur = new CurrencyListWindow();
            cur.ShowDialog();
            SetCurrencyFilter();
            UpdateAccountBalance();
            UpdateDocumentsGrid();
        }

        private void btnDebit_Click(object sender, RoutedEventArgs e)
        {
            if (DocumentEditor.Create(Kernel.DocumentType.DocTypeSign.Income) > 0)
                UpdateAll();
        }

        private void btnEdit_Click(object sender, RoutedEventArgs e)
        {
            Kernel.Document doc = null;
            if(grdDocuments.SelectedItems.Count == 1)
            {
                if (grdDocuments.SelectedItems[0] is Kernel.Document)
                    doc = grdDocuments.SelectedItems[0] as Kernel.Document;
            }
            if (doc != null && DocumentEditor.Edit(doc) > 0)
                UpdateAll();
        }

        private void UpdateAll()
        {
            int selected = grdDocuments.SelectedIndex;
            UpdateDocumentsGrid();
            if(grdDocuments.Items.Count > selected)
                grdDocuments.SelectedIndex = selected;
            UpdateAccountBalance();
            UpdateReport();
        }

        private void UpdateReport()
        {
            grdReport.AutoGenerateColumns = true;
            Kernel.Account account = null;
            Kernel.Currency currency = null;
            if (cbxAccount.SelectedIndex > 0)
                account = cbxAccount.SelectedItem as Kernel.Account;
            if (cbxCurrency.SelectedIndex > 0)
                currency = cbxCurrency.SelectedItem as Kernel.Currency;
            Kernel.ReportItem[] report = db.Database.GetReport(dateMin.SelectedDate.Value, dateMax.SelectedDate.Value);
            Kernel.ReportBuilder.CreateReport(report, Kernel.ReportBuilder.Periodicity.Daily);            

        }

        private void grdDocuments_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            btnEdit_Click(sender, e);
        }

        private void btnCredit_Click(object sender, RoutedEventArgs e)
        {
            if (DocumentEditor.Create(Kernel.DocumentType.DocTypeSign.Cost) > 0)
                UpdateAll();
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            Kernel.Document doc = null;
            if (grdDocuments.SelectedItems.Count == 1)
            {
                if (grdDocuments.SelectedItems[0] is Kernel.Document)
                    doc = grdDocuments.SelectedItems[0] as Kernel.Document;
            }
            if (doc != null && MessageDialog.Ask("Удаление документа","Удалить текущий документ?") && db.Database.DeleteDocument(doc))
                UpdateAll();
        }

        private void btnMovement_Click(object sender, RoutedEventArgs e)
        {
            if (DocumentEditor.Create(Kernel.DocumentType.DocTypeSign.Movement) > 0)
                UpdateAll();
        }

        private void cbxAccount_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {    
            if(_load_finished)
                UpdateDocumentsGrid();
        }

        private void grdDocuments_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            Dictionary<string, MenuItem> items = new Dictionary<string, MenuItem>();
            ContextMenu menu = new ContextMenu();
            for (int i = 0; i < grdDocuments.Items.Count;i++)
            {
                Kernel.Document doc = null;
                if (grdDocuments.Items[i] is Kernel.Document)
                {
                    doc = grdDocuments.Items[i] as Kernel.Document;
                    if (!String.IsNullOrEmpty(doc.Shop) && !items.ContainsKey(doc.Shop))
                    {
                        MenuItem item = new MenuItem();
                        item.Header = doc.Shop;
                        item.IsCheckable = true;
                        item.Click += new RoutedEventHandler(item_Click);
                        items.Add(doc.Shop, item);

                        menu.Items.Add(item);
                    }
                }
            }
            if (menu.Items.Count > 0)
                menu.IsOpen = true;
        }

        void item_Click(object sender, RoutedEventArgs e)
        {
            if (e.Source is MenuItem) 
            {
                _load_finished = false;
                try 
                {
                    string caption = (e.Source as MenuItem).Header.ToString();
                    grdDocuments.SelectedItems.Clear();
                    for (int i = 0; i < grdDocuments.Items.Count; i++)
                    {
                        Kernel.Document doc = null;
                        if (grdDocuments.Items[i] is Kernel.Document)
                        {
                            doc = grdDocuments.Items[i] as Kernel.Document;
                            if (!String.IsNullOrEmpty(doc.Shop) && doc.Shop == caption)
                            {
                                grdDocuments.SelectedItems.Add(grdDocuments.Items[i]);
                            }
                        }
                    }
                }
                finally
                {
                    _load_finished = true;
                }
                grdDocuments_SelectionChanged(sender, null);
            }
        }

        private void grdDocuments_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (!_load_finished)
                return;
            if(grdDocuments.SelectedItems.Count > 1)
            {
                Dictionary<int, int> summa_out = new Dictionary<int, int>();
                for (int i = 0; i < grdDocuments.SelectedItems.Count; i++) 
                {
                    Kernel.Document doc = grdDocuments.SelectedItems[i] as Kernel.Document;
                    if (doc != null) 
                    {
                        if (doc.Credit != null && doc.Debit == null)
                        {
                            if (summa_out.ContainsKey(doc.Credit.Amount.Currency.Code))
                                summa_out[doc.Credit.Amount.Currency.Code] += doc.Credit.Amount.Summa;
                            else
                                summa_out.Add(doc.Credit.Amount.Currency.Code, doc.Credit.Amount.Summa);
                        }
                    }
                }
                pnlSummary.Children.Clear();                
                foreach(int currency in summa_out.Keys) 
                {
                    TextBlock lbl = new TextBlock();
                    lbl.Margin = new Thickness(0,3,0,0);
                    lbl.Width = 100; 
                    lbl.TextAlignment = TextAlignment.Right;
                    lbl.Text = Kernel.Amount.ToMoney(summa_out[currency]) + " " + Kernel.Globals.Currency[currency];
                    pnlSummary.Children.Add(lbl);                    
                }
                pnlSummary.Visibility = Visibility.Visible;

            }
            else pnlSummary.Visibility = Visibility.Collapsed;
        }


    }
}
