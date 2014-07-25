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
    /// Interaction logic for Accounts.xaml
    /// </summary>
    public partial class CurrencyListWindow : Window
    {
        public CurrencyListWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {            
            WindowState = WindowState.Minimized;
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            if (WindowState == WindowState.Maximized)
                WindowState = WindowState.Normal;
            else
                WindowState = WindowState.Maximized;
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void lblCaption_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Button_Click_1(sender, e);
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

        private void btnAdd_Click(object sender, RoutedEventArgs e)
        {
            Kernel.Currency new_cur = new Kernel.Currency();
            if (CurrencyDialog.Edit(new_cur))
            {
                if (!Kernel.Globals.Currency.ContainsKey(new_cur.Code)) 
                {
                    db.Database.WriteCurrency(new_cur);
                    Kernel.Globals.Currency.Add(new_cur.Code, new_cur);
                    UpdateAccountList();
                }
            }
        }

        private void UpdateAccountList()
        {
            listCurrency.ItemsSource = db.Database.GetCurrencyList().Values;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            UpdateAccountList();
        }

        private void btnEdit_Click(object sender, RoutedEventArgs e)
        {
            if (listCurrency.SelectedItem is Kernel.Currency) 
            {
                if (CurrencyDialog.Edit(listCurrency.SelectedItem as Kernel.Currency)) 
                {
                    db.Database.WriteCurrency(listCurrency.SelectedItem as Kernel.Currency);
                    listCurrency.Items.Refresh();                    
                }
            }
        }

        private void listCurrency_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            btnEdit_Click(sender, e);
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            if (listCurrency.SelectedItem is Kernel.Currency)
            {
                db.Database.DeleteCurrency((listCurrency.SelectedItem as Kernel.Currency).Code);
                UpdateAccountList();
            }
        }

    }
}
