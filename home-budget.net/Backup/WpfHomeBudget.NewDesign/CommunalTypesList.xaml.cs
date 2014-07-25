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
    /// Interaction logic for CommunalTypesList.xaml
    /// </summary>
    public partial class CommunalTypesList : Window
    {
        public bool WasChanged = false;
        public CommunalTypesList()
        {
            InitializeComponent();
        }

        private void UpdateList()
        {
            listCommunalTypes.ItemsSource = Communal.Globals.CommunalTypes.Values;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            UpdateList();
        }

        private void btnEdit_Click(object sender, RoutedEventArgs e)
        {
#if COMMUNAL
            if (listCommunalTypes.SelectedItem is Communal.CommunalType) 
            {
                Communal.CommunalType ctype = listCommunalTypes.SelectedItem as Communal.CommunalType;
                if (CommunalTypeDialog.Edit(ctype, this))
                {
                    db.Database.WriteCommunalType(ctype);
                    db.Database.UpdateGlobals();
                    UpdateList();
                    WasChanged = true;
                }
            }
#endif
        }

        private void btnAdd_Click(object sender, RoutedEventArgs e)
        {
#if COMMUNAL
            Communal.CommunalType ctype = new Communal.CommunalType();
            if (CommunalTypeDialog.Edit(ctype, this)) 
            {
                db.Database.WriteCommunalType(ctype);
                db.Database.UpdateGlobals();
                UpdateList();
                WasChanged = true;
            }
#endif
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {

        }

        private void listCommunalTypes_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            btnEdit_Click(sender, e);
        }
    }
}
