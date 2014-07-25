﻿using System;
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
    /// Interaction logic for SimpleDialog.xaml
    /// </summary>
    public partial class MessageDialog : Window
    {
        public MessageDialog()
        {
            InitializeComponent();
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

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
        /// <summary>
        /// Выводит вопрос, на который ответ ДА или НЕТ
        /// </summary>
        /// <param name="caption"></param>
        /// <param name="text"></param>
        /// <returns>True, если нажата кнопка ДА</returns>
        public static bool Ask(string caption, string text) 
        {
            MessageDialog dlg = new MessageDialog();
            dlg.lblCaption.Content = caption;
            dlg.lblText.Content = text;
            dlg.btnOk.Visibility = Visibility.Collapsed;
            dlg.btnCancel.Visibility = Visibility.Collapsed;
            dlg.btnYes.IsDefault = true;
            dlg.btnNo.IsCancel = true;
            return dlg.ShowDialog().Value;
        }
        /// <summary>
        /// Информационное сообщение с одной кнопкой ОК
        /// </summary>
        /// <param name="caption"></param>
        /// <param name="text"></param>
        /// <returns></returns>
        public static bool ShowMessage(string caption, string text) 
        {
            MessageDialog dlg = new MessageDialog();
            dlg.lblCaption.Content = caption;
            dlg.lblText.Content = text;
            dlg.btnYes.Visibility = Visibility.Collapsed;
            dlg.btnNo.Visibility = Visibility.Collapsed;
            dlg.btnCancel.Visibility = Visibility.Collapsed;
            dlg.btnOk.IsDefault = true;
            dlg.btnOk.IsCancel = true;
            return dlg.ShowDialog().Value;
        }
        /// <summary>
        /// Выводит подтверждение с кнопками ОЛ и ОТМЕНА
        /// </summary>
        /// <param name="caption"></param>
        /// <param name="text"></param>
        /// <returns>true, если нажата кнопка ОК</returns>
        public static bool Confirm(string caption, string text) 
        {
            MessageDialog dlg = new MessageDialog();
            dlg.lblCaption.Content = caption;
            dlg.lblText.Content = text;
            dlg.btnYes.Visibility = Visibility.Collapsed;
            dlg.btnNo.Visibility = Visibility.Collapsed;
            dlg.btnOk.IsDefault = true;
            dlg.btnCancel.IsCancel = true;
            return dlg.ShowDialog().Value;
        }

        private void btnYes_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void btnNo_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }
    }
}
