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
    public partial class AccountsListWindow : Window
    {
        public AccountsListWindow()
        {
            InitializeComponent();
        }
        private void btnAdd_Click(object sender, RoutedEventArgs e)
        {
            Kernel.Account account = new Kernel.Account();
            if (AccountDialog.Edit(account)) 
            {
                db.Database.WriteAccount(account);                
                UpdateAccountList();                
            }            
        }

        private void UpdateAccountList()
        {
            Kernel.Globals.Accounts = db.Database.GetAccountList();
            itemsAccounts.ItemsSource = Kernel.Globals.Accounts.Values;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            UpdateAccountList();
        }

        private void btnEdit_Click(object sender, RoutedEventArgs e)
        {
            if (itemsAccounts.SelectedItem is Kernel.Account) 
            {
                Kernel.Account account = itemsAccounts.SelectedItem as Kernel.Account;
                if (AccountDialog.Edit(account))
                {
                    db.Database.WriteAccount(account);
                    itemsAccounts.Items.Refresh();
                }
            }
        }

        private void itemsAccounts_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            btnEdit_Click(sender, e);
        }

        private void btnUp_Click(object sender, RoutedEventArgs e)
        {
            int selection = itemsAccounts.SelectedIndex;
            if (itemsAccounts.SelectedItem is Kernel.Account) 
            {
                if (db.Database.AccountMoveUp(itemsAccounts.SelectedItem as Kernel.Account)) 
                {
                    UpdateAccountList();
                    itemsAccounts.SelectedIndex = selection - 1;
                }                
            }
        }

        private void btnDown_Click(object sender, RoutedEventArgs e)
        {
            int selection = itemsAccounts.SelectedIndex;
            if (itemsAccounts.SelectedItem is Kernel.Account)
            {
                if (db.Database.AccountMoveDown(itemsAccounts.SelectedItem as Kernel.Account)) 
                {
                    UpdateAccountList();
                    itemsAccounts.SelectedIndex = selection + 1;
                }                
            }
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            int selection = itemsAccounts.SelectedIndex;
            Kernel.Account acc = itemsAccounts.SelectedItem as Kernel.Account;
            if (acc != null)
            {
                if (db.Database.AccountCanDelete(acc))
                {
                    if (MessageDialog.Ask("Удаление", "Удалить указанный счет?"))
                    {
                        if (db.Database.AccountDelete(itemsAccounts.SelectedItem as Kernel.Account))
                        {
                            UpdateAccountList();
                            itemsAccounts.SelectedIndex = (selection < itemsAccounts.Items.Count) ? selection : selection - 1;
                        }
                    }
                }
                else 
                {
                    MessageDialog.ShowMessage("Удаление невозможно", "Невозможно удалить указанный счет.");
                }
            }
        }

    }
}
