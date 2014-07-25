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
using Microsoft.Windows.Controls;
using Microsoft.Windows.Controls.Primitives;
using System.Windows.Markup;
using System.IO;
using System.Xml;

namespace WpfHomeBudget
{
    
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        bool _load_finished = false;
        Reports.ReportBuilder _report_builder = null;
        Reports.Report _report = null;

        public MainWindow()
        {
            StartWindow.StaticWindow = new StartWindow();
            StartWindow.StaticWindow.Show();
            InitializeComponent();
#if COMMUNAL
            pnlCommunal.Visibility = Visibility.Visible;
            tpgCommunal.Visibility = Visibility.Visible;
#else
            pnlCommunal.Visibility = Visibility.Collapsed;
            tpgCommunal.Visibility = Visibility.Collapsed;
            rowCommunal.Height = new GridLength(0);
#endif
            LoadLocation();
            this.AddHandler(CloseableTabItem.CloseTabEvent, new RoutedEventHandler(this.CloseTab));
        }

        private void LoadLocation()
        {            
            Left = Convert.ToInt32(db.Database.GetValue("LEFT", "0"));
            Top = Convert.ToInt32(db.Database.GetValue("TOP", "0"));
            Width = Convert.ToInt32(db.Database.GetValue("WIDTH", "900"));
            Height = Convert.ToInt32(db.Database.GetValue("HEIGHT", "700"));
            int index = 0;
            foreach (DataGridColumn column in dgDocuments.grdDocuments.Columns)
            {
                string name = String.Format("DOC_COL_{0}", index++);
                string width = db.Database.GetValue(name, "0");
                if (width != "0" && width != "Auto")
                    column.Width = Convert.ToDouble(width);
                else
                    column.Width = new DataGridLength(100,  DataGridLengthUnitType.Auto);
            }
        }

        private void SaveLocation()
        {
            db.Database.SetValue("LEFT", Left);
            db.Database.SetValue("TOP", Top);
            db.Database.SetValue("WIDTH", Width);
            db.Database.SetValue("HEIGHT", Height);
            int index = 0;
            foreach(DataGridColumn column in dgDocuments.grdDocuments.Columns)
            {
                string name = String.Format("DOC_COL_{0}", index++);
                db.Database.SetValue(name, column.Width.ToString());                
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            pnlFilter.Height = pnlFilter.Height == 200 ? 30 : 200;
        }
        private void SetPeriod()
        {
            DateTime today = DateTime.Today;
            int dayOfWeek = Convert.ToInt32(today.DayOfWeek) - 1;
            if (dayOfWeek < 0)
                dayOfWeek = 6;
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
                    DateTime min_date;
                    DateTime max_date;
                    db.Database.GetMaxDataPeriod(out min_date, out max_date);
                    dateMin.SelectedDate = min_date;
                    dateMax.SelectedDate = (max_date < DateTime.Today) ? DateTime.Today : max_date;
                    break;
                default:
                    dateMin.SelectedDate = DateTime.Today;
                    dateMax.SelectedDate = DateTime.Today;
                    break;
            }
            dateMin.DisplayDate = dateMin.SelectedDate.Value;
            dateMax.DisplayDate = dateMax.SelectedDate.Value;
            SetPiriodicity();
        }
        private void cbxPeriod_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SetPeriod();
            if (_load_finished) 
            {
                UpdateAll();                
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
            if (pnlCalendar.Visibility == Visibility.Collapsed)
            {
                pnlCalendar.Visibility = Visibility.Visible;
                Uri uri = new Uri(@"pack://application:,,,/Resources/Up.png");
                BitmapImage bitmap = new BitmapImage(uri);
                Image img = new Image();
                imgButton.Source = bitmap;
                pnlCalendar.UpdateLayout();
            }
            else 
            {
                pnlCalendar.Visibility = Visibility.Collapsed;
                Uri uri = new Uri("pack://application:,,,/Resources/Down.png");
                BitmapImage bitmap = new BitmapImage(uri);
                Image img = new Image();
                imgButton.Source = bitmap;
                pnlCalendar.UpdateLayout();
                UpdateDocumentsGrid();
                SetPiriodicity();
            }
            
        }
        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {            
            if (tcMain.SelectedItem == tpgDocs)
            {
                pnlAccount.Visibility = Visibility.Visible;
                pnlPeriodicity.Visibility = Visibility.Collapsed;
                pnlCurrency.Visibility = Visibility.Visible;
            }
            if (tcMain.SelectedItem == tpgReports)
            {
                pnlAccount.Visibility = Visibility.Collapsed;
                pnlPeriodicity.Visibility = Visibility.Visible;
                pnlCurrency.Visibility = Visibility.Visible;
            }
            if (tcMain.SelectedItem == tpgEvents) 
            {
                pnlAccount.Visibility = Visibility.Collapsed;
                pnlPeriodicity.Visibility = Visibility.Collapsed;
                pnlCurrency.Visibility = Visibility.Collapsed;
            }
            if (tcMain.SelectedItem == tpgCommunal) 
            {
                pnlAccount.Visibility = Visibility.Collapsed;
                pnlPeriodicity.Visibility = Visibility.Collapsed;
                pnlCurrency.Visibility = Visibility.Collapsed;
            }
        }

        private void SetPiriodicity()
        {
            bool old_value = _load_finished;
            try
            {

                _load_finished = false;
                int days = Math.Abs((dateMax.SelectedDate.Value - dateMin.SelectedDate.Value).Days);
                if (days < 7)
                {
                    cbxPeriodicity.Items.Clear();
                    cbxPeriodicity.Items.Add(Reports.Report.Period.Dayly);
                }
                else
                    if (days < 30)
                    {
                        cbxPeriodicity.Items.Clear();
                        cbxPeriodicity.Items.Add(Reports.Report.Period.Dayly);
                        cbxPeriodicity.Items.Add(Reports.Report.Period.Weekly);
                    }
                    else
                        if (days < 93)
                        {
                            cbxPeriodicity.Items.Clear();
                            cbxPeriodicity.Items.Add(Reports.Report.Period.Dayly);
                            cbxPeriodicity.Items.Add(Reports.Report.Period.Weekly);
                            cbxPeriodicity.Items.Add(Reports.Report.Period.Monthly);
                        }
                        else
                            if (days < 185)
                            {
                                cbxPeriodicity.Items.Clear();
                                cbxPeriodicity.Items.Add(Reports.Report.Period.Weekly);
                                cbxPeriodicity.Items.Add(Reports.Report.Period.Monthly);
                                cbxPeriodicity.Items.Add(Reports.Report.Period.Quarterly);
                            }
                            else
                                if (days < 366)
                                {
                                    cbxPeriodicity.Items.Clear();
                                    cbxPeriodicity.Items.Add(Reports.Report.Period.Weekly);
                                    cbxPeriodicity.Items.Add(Reports.Report.Period.Monthly);
                                    cbxPeriodicity.Items.Add(Reports.Report.Period.Quarterly);
                                }
                                else
                                {
                                    cbxPeriodicity.Items.Clear();
                                    cbxPeriodicity.Items.Add(Reports.Report.Period.Monthly);
                                    cbxPeriodicity.Items.Add(Reports.Report.Period.Quarterly);
                                    cbxPeriodicity.Items.Add(Reports.Report.Period.Yearly);
                                }
                cbxPeriodicity.SelectedIndex = cbxPeriodicity.Items.Count - 1;
            }
            finally 
            {
                _load_finished = old_value;
            }
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            _load_finished = false;
            cbxPeriod.SelectedIndex = 0;
            SetAccountFilter();
            SetCurrencyFilter();            
            _load_finished = true;
            UpdateAccountBalance();
            UpdateDocumentsGrid();
            UpdateEventBoard();
            UpdateTariffsGrid();
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
#if COMMUNAL
            Communal.CommunalBalance[] com_balance = db.Database.GetCommunalBalance(dateAccount.SelectedDate.Value);
            itemsCommunalBalance.ItemsSource = com_balance;
#endif
        }
        private void UpdateDocumentsGrid() 
        {
            dgDocuments.grdDocuments.AutoGenerateColumns = false;
            Kernel.Account account = null;
            Kernel.Currency currency = null;
            if(cbxAccount.SelectedIndex > 0)
                account = cbxAccount.SelectedItem as Kernel.Account;
            if (cbxCurrency.SelectedIndex > 0)
                currency = cbxCurrency.SelectedItem as Kernel.Currency;
            dgDocuments.grdDocuments.ItemsSource = db.Database.GetDocuments(dateMin.SelectedDate.Value, dateMax.SelectedDate.Value, account, currency);
            if (dgDocuments.grdDocuments.Items.Count > 1) 
            {
                dgDocuments.grdDocuments.SelectedIndex = 0;
                dgDocuments.grdDocuments.UpdateLayout();
                if (dgDocuments.grdDocuments.IsVisible)
                    dgDocuments.grdDocuments.ScrollIntoView(dgDocuments.grdDocuments.SelectedItem);
            }
            /*if (grdDocuments.SelectedItem != null)
                */
        }
        private void UpdateTariffsGrid() 
        {
#if COMMUNAL
            grdCommunalActualTariffs.AutoGenerateColumns = false;
            grdCommunalActualTariffs.ItemsSource = Communal.Globals.ActualTariffs.GetTariffsOnDate(DateTime.Today).Values;
            grdCommunalAllTariffs.AutoGenerateColumns = false;
            grdCommunalAllTariffs.ItemsSource = Communal.Globals.ActualTariffs;
            grdCommunal.ItemsSource = db.Database.GetCommunalDocuments(dateMin.SelectedDate.Value, dateMax.SelectedDate.Value);
#endif
        }
        private void UpdateEventBoard() 
        {            
            grdEvents.ItemsSource = Kernel.Globals.Events;
            pnlEvents.Children.Clear();
            foreach (Kernel.Event msg in Kernel.Globals.Events.GetDateEvents(DateTime.Today)) 
            {
                pnlEvents.Children.Add(EventControl.Create(msg));
            }
            pnlEvents.Visibility = Visibility.Visible;
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

        private void AddParentsToReport(Reports.Report report, int parent_id)
        {
            if (parent_id > 0 && !report.ContainsId(parent_id)) 
            {                
                int new_parent_id = 0;
                if (Kernel.Globals.DocTypes.ContainsKey(parent_id))
                {
                    new_parent_id = Kernel.Globals.DocTypes[parent_id].ParentId;
                    report.AddRow(parent_id, Kernel.Globals.DocTypes[parent_id].ParentId, Kernel.Globals.DocTypes[parent_id].Name);
                    if (Kernel.Globals.DocTypes[parent_id].ParentId > 0)
                        AddParentsToReport(report, Kernel.Globals.DocTypes[parent_id].ParentId);
                }
            }
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
            if (DocumentEditor.Create(Kernel.DocumentType.DocTypeSign.Income, this) > 0)
                UpdateAll();
        }

        private void btnEdit_Click(object sender, RoutedEventArgs e)
        {
            if (dgDocuments.grdDocuments.IsVisible)
            {
                Kernel.Document doc = null;
                if (dgDocuments.grdDocuments.SelectedItems.Count == 1)
                {
                    if (dgDocuments.grdDocuments.SelectedItem is Kernel.Document)
                        doc = dgDocuments.grdDocuments.SelectedItem as Kernel.Document;
                }
                if (doc != null && DocumentEditor.Edit(doc, this) > 0)
                    UpdateAll();
            }
                
        }

        private void UpdateAll()
        {
            if (!_load_finished)
                return;
            if (tpgDocs.IsSelected) 
            {
                int selected = dgDocuments.grdDocuments.SelectedIndex;
                UpdateDocumentsGrid();
                if (dgDocuments.grdDocuments.Items.Count > selected)
                    dgDocuments.grdDocuments.SelectedIndex = selected;
                if (dgDocuments.grdDocuments.SelectedItem != null && dgDocuments.grdDocuments.IsVisible)
                    Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                        new Action(
                            delegate()
                            {
                                dgDocuments.grdDocuments.UpdateLayout();
                                dgDocuments.grdDocuments.ScrollIntoView(dgDocuments.grdDocuments.SelectedItem);
                            }));
                UpdateAccountBalance();
            }
            
            if(tpgReports.IsSelected)
            {
                UpdateReport();
            }
            if (tpgEvents.IsSelected) 
            {
                UpdateEventBoard();
            }
#if COMMUNAL
            if (tpgCommunal.IsSelected) 
            {
                UpdateTariffsGrid();
            }
#endif
        }

        private void UpdateReport()
        {
            if (!_load_finished)
                return;
            DateTime min_date = dateMin.SelectedDate.Value;
            DateTime max_date = dateMax.SelectedDate.Value;
            if (min_date > max_date)
            {
                DateTime date = min_date;
                min_date = max_date;
                max_date = date;
            }

            Kernel.Account account = null;
            Kernel.Currency currency = null;
            if (cbxAccount.SelectedIndex > 0)
                account = cbxAccount.SelectedItem as Kernel.Account;
            if (cbxCurrency.SelectedIndex > 0)
                currency = cbxCurrency.SelectedItem as Kernel.Currency;

            Kernel.ReportItem[] report_items = db.Database.GetReport(min_date, max_date, currency);

            Reports.Report.Period period = (Reports.Report.Period)cbxPeriodicity.SelectedItem;

            _report = new Reports.Report(min_date, max_date, period);
            foreach (Kernel.ReportItem item in report_items)
            {
                AddParentsToReport(_report, item.DocType.ParentId);
                foreach (Kernel.Amount amount in item.Amounts.Values)
                {
                    _report.AddBalance(item.DocType.Id, item.DocType.ParentId, item.DocType.Name, item.DocDate, amount.Currency.ShortName, amount.Summa);
                }
            }
            Style rightColumnStyle = (Style)FindResource("RightCellStyle");
            _report_builder = new Reports.ReportBuilder(_report, grdReport, rightColumnStyle);
            _report_builder.FillData();
        }

        private void grdDocuments_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // Проверка на то, что нажато заголовок или ячейка
            DependencyObject dep = (DependencyObject)e.OriginalSource;

            // iteratively traverse the visual tree
            while ((dep != null) &&
                    !(dep is DataGridCell) &&
                    !(dep is DataGridColumnHeader))
            {
                dep = VisualTreeHelper.GetParent(dep);
            }

            if (dep == null)
                return;

            if (dep is DataGridColumnHeader)
            {
                DataGridColumnHeader columnHeader = dep as DataGridColumnHeader;
                // do something
            }

            if (dep is DataGridCell)
            {
                DataGridCell cell = dep as DataGridCell;
                btnEdit_Click(sender, e);
            }            

        }

        private void btnCredit_Click(object sender, RoutedEventArgs e)
        {
            if (DocumentEditor.Create(Kernel.DocumentType.DocTypeSign.Cost, this) > 0)
                UpdateAll();
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            if (!dgDocuments.grdDocuments.IsVisible)
                return;
            Kernel.Document doc = null;
            if (dgDocuments.grdDocuments.SelectedItems.Count == 1)
            {
                if (dgDocuments.grdDocuments.SelectedItems[0] is Kernel.Document)
                    doc = dgDocuments.grdDocuments.SelectedItems[0] as Kernel.Document;
            }
            if (doc != null && MessageDialog.Ask("Удаление документа","Удалить текущий документ?") && db.Database.DeleteDocument(doc))
                UpdateAll();
        }

        private void btnMovement_Click(object sender, RoutedEventArgs e)
        {
            if (DocumentEditor.Create(Kernel.DocumentType.DocTypeSign.Movement, this) > 0)
                UpdateAll();
        }

        private void cbxAccount_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (_load_finished) 
            {
                UpdateAll();
            }                
        }

        private void grdDocuments_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            Dictionary<string, MenuItem> items = new Dictionary<string, MenuItem>();
            ContextMenu menu = new ContextMenu();
            for (int i = 0; i < dgDocuments.grdDocuments.Items.Count; i++)
            {
                Kernel.Document doc = null;
                if (dgDocuments.grdDocuments.Items[i] is Kernel.Document)
                {
                    doc = dgDocuments.grdDocuments.Items[i] as Kernel.Document;
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
                    dgDocuments.grdDocuments.SelectedItems.Clear();
                    for (int i = 0; i < dgDocuments.grdDocuments.Items.Count; i++)
                    {
                        Kernel.Document doc = null;
                        if (dgDocuments.grdDocuments.Items[i] is Kernel.Document)
                        {
                            doc = dgDocuments.grdDocuments.Items[i] as Kernel.Document;
                            if (!String.IsNullOrEmpty(doc.Shop) && doc.Shop == caption)
                            {
                                dgDocuments.grdDocuments.SelectedItems.Add(dgDocuments.grdDocuments.Items[i]);
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
            if (dgDocuments.grdDocuments.SelectedItems.Count > 1)
            {
                Dictionary<int, int> summa_out = new Dictionary<int, int>();
                for (int i = 0; i < dgDocuments.grdDocuments.SelectedItems.Count; i++) 
                {
                    Kernel.Document doc = dgDocuments.grdDocuments.SelectedItems[i] as Kernel.Document;
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

        private void grdReport_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // Проверка на то, что нажато заголовок или ячейка
            DependencyObject dep = (DependencyObject)e.OriginalSource;

            // iteratively traverse the visual tree
            while ((dep != null) &&
                    !(dep is DataGridCell) &&
                    !(dep is DataGridColumnHeader))
            {
                dep = VisualTreeHelper.GetParent(dep);
            }

            if (dep == null)
                return;

            if (dep is DataGridColumnHeader)
            {
                DataGridColumnHeader columnHeader = dep as DataGridColumnHeader;
                // do something
            }

            if (dep is DataGridCell)
            {
                DataGridCell cell = dep as DataGridCell;
                switch (cell.Column.DisplayIndex) 
                {
                    case 0:
                        _report_builder.Expand(grdReport.SelectedItem as Reports.ReportRow);
                        break;
                    case 1:
                        CreateNewTab(grdReport.SelectedItem as Reports.ReportRow, dateMin.SelectedDate.Value, dateMax.SelectedDate.Value);
                        e.Handled = true;
                        break;
                    default :
                        int col_index = cell.Column.DisplayIndex;
                        Reports.ColumnDateHeader date_col = _report_builder.Report.Columns[col_index - 2] as Reports.ColumnDateHeader;
                        DateTime minDate = dateMin.SelectedDate.Value;
                        DateTime maxDate = dateMin.SelectedDate.Value;
                        if (date_col != null) 
                        {
                            minDate = date_col.MinDate;
                            maxDate = date_col.MaxDate;
                        }
                        CreateNewTab(grdReport.SelectedItem as Reports.ReportRow, minDate, maxDate);
                        e.Handled = true;
                        break;
                }
                    
            }            

        }

        private void CreateNewTab(Reports.ReportRow row, DateTime minDate, DateTime maxDate)
        {
            CloseableTabItem item = new CloseableTabItem();

            DocumentGrid newGrid = new DocumentGrid();

            item.Content = newGrid;
            item.VerticalContentAlignment = VerticalAlignment.Stretch;
            item.HorizontalContentAlignment = HorizontalAlignment.Stretch;

            Kernel.Currency currency = null;
            if (cbxCurrency.SelectedIndex > 0)
                currency = cbxCurrency.SelectedItem as Kernel.Currency;

            newGrid.grdDocuments.ItemsSource = db.Database.GetDocuments(minDate, maxDate, null, currency, row.DocType.Id);
            item.Header = String.Format("{0} {1}-{2}", row.DocTypeName, minDate.ToString("dd.MM.yyyy"), maxDate.ToString("dd.MM.yyyy"));
            tcMain.Items.Add(item);
            tcMain.SelectedItem = item;
            tcMain.UpdateLayout();
        }
        private void CloseTab(object source, RoutedEventArgs args)
        {
            TabItem tabItem = args.Source as TabItem;
            if (tabItem != null)
            {
                TabControl tabControl = tabItem.Parent as TabControl;
                if (tabControl != null)
                    tabControl.Items.Remove(tabItem);
            }
        }

        private void grdEvents_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            btnEditEvent_Click(sender, e);
        }

        private void btnEvent_Click(object sender, RoutedEventArgs e)
        {
            Kernel.Event evnt = new Kernel.Event();

            if (EventDialog.Edit(evnt)) 
            {
                Kernel.Globals.Events.Add(evnt);
                tpgEvents.IsSelected = true;
                UpdateEventBoard();
                grdEvents.Items.Refresh();
            }

        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            SaveLocation();
        }

        private void btnEditEvent_Click(object sender, RoutedEventArgs e)
        {
            if (tpgEvents.IsVisible)
            {
                Kernel.Event evnt = null;
                if (grdEvents.SelectedItems.Count == 1 && grdEvents.SelectedItem is Kernel.Event)
                    evnt = grdEvents.SelectedItem as Kernel.Event;
                if (evnt != null && EventDialog.Edit(evnt))
                {
                    UpdateEventBoard();
                    grdEvents.Items.Refresh();
                    grdEvents.UpdateLayout();

                }
            }
        }

        private void btnDeleteEvent_Click(object sender, RoutedEventArgs e)
        {
            if (!grdEvents.IsVisible)
                return;
            Kernel.Event evnt = null;
            if (grdEvents.SelectedItems.Count == 1)
            {
                if (grdEvents.SelectedItems[0] is Kernel.Event)
                    evnt = grdEvents.SelectedItems[0] as Kernel.Event;
            }
            if (evnt != null && MessageDialog.Ask("Удаление напоминания", "Удалить текущее напоминание?") && db.Database.DeleteEvent(evnt))
            {
                UpdateEventBoard();
                grdEvents.Items.Refresh();
                grdEvents.UpdateLayout();
            }
        }

        private void btnAddCommunal_Click(object sender, RoutedEventArgs e)
        {
            Communal.Document doc = new Communal.Document();
            doc.Sign = 1;
            if(CommunalDialog.Edit(doc, this))
            {
                UpdateTariffsGrid();
            }
        }

        private void btnPayCommunal_Click(object sender, RoutedEventArgs e)
        {
            Communal.Document doc = new Communal.Document();
            doc.Sign = -1;
            if (CommunalDialog.Edit(doc, this))
            {
                UpdateTariffsGrid();
            }
        }

        private void btnEditCommunal_Click(object sender, RoutedEventArgs e)
        {
            Communal.Document doc = grdCommunal.SelectedItem as Communal.Document;
            if (doc != null) 
            {
                if (CommunalDialog.Edit(doc, this))
                {
                    grdCommunal.Items.Refresh();
                }
            }
        }

        private void btnDeleteCommunal_Click(object sender, RoutedEventArgs e)
        {
#if COMMUNAL
            Communal.Document doc = grdCommunal.SelectedItem as Communal.Document;
            if (doc != null)
            {
                if(MessageDialog.Ask("Удаление", "Удалить текущий документ?"))
                {
                    db.Database.DeleteDocument(doc);
                    UpdateTariffsGrid();
                }
            }
#endif
        }

        private void btnCommunalTypes_Click(object sender, RoutedEventArgs e)
        {
            CommunalTypesList lst = new CommunalTypesList();
            lst.Owner = this;
            lst.ShowDialog();
            if (lst.WasChanged) 
            {
                UpdateTariffsGrid();
            }
        }

        private void btnAddTariff_Click(object sender, RoutedEventArgs e)
        {
            Communal.Tariff tariff = new Communal.Tariff();
            if (CommunalTariffDialog.Edit(tariff, this)) 
            {
                UpdateTariffsGrid();
            }
        }

        private void btnEditTariff_Click(object sender, RoutedEventArgs e)
        {
            Communal.Tariff tariff = grdCommunalAllTariffs.SelectedItem as Communal.Tariff;
            if (tariff == null)
                return;
            if (CommunalTariffDialog.Edit(tariff, this))
            {
                UpdateTariffsGrid();
            }

        }

        private void btnDeleteTariff_Click(object sender, RoutedEventArgs e)
        {

        }

        private void grdCommunalAllTariffs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            btnEditTariff_Click(sender, e);
        }



    }
}
