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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfHomeBudget
{
    /// <summary>
    /// Interaction logic for DocumentGrid.xaml
    /// </summary>
    public partial class DocumentGrid : UserControl
    {
        public DocumentGrid()
        {
            InitializeComponent();
        }

        public event MouseButtonEventHandler MouseDouble;
        public event MouseButtonEventHandler MouseRightButton;
        public event SelectionChangedEventHandler SelectionChanged;

        private void grdDocuments_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (MouseDouble != null)
                MouseDouble(sender, e);
        }

        private void grdDocuments_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (MouseRightButton != null)
                MouseRightButton(sender, e);
        }

        private void grdDocuments_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (SelectionChanged != null)
                SelectionChanged(sender, e);
        }
    }
}
