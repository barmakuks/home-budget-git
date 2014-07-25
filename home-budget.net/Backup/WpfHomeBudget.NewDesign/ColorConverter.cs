using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Data;
using System.Windows.Media;
using System.Globalization;

namespace WpfHomeBudget
{
    [ValueConversion(typeof(uint), typeof(Brush))]
    public class AccountColorConverter : IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return new SolidColorBrush(Kernel.ColorItem.UIntToColor((uint)value));
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return Colors.Red;
        }

    }

    [ValueConversion(typeof(int), typeof(Brush))]
    public class SummaColorConverter : IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((int)value >= 0)
                return new SolidColorBrush(Colors.Black);
            else
                return new SolidColorBrush(Colors.Red);
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return 0;
        }

    }

    [ValueConversion(typeof(Kernel.DocumentType.DocTypeSign), typeof(Brush))]
    public class DocTypeColorConverter : IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            switch ((Kernel.DocumentType.DocTypeSign)value) 
            {
                case Kernel.DocumentType.DocTypeSign.Income:
                    return new SolidColorBrush(Colors.Blue);
                case Kernel.DocumentType.DocTypeSign.Movement:
                    return new SolidColorBrush(Colors.Green);
                default:
                    return new SolidColorBrush(Colors.Black);
            }
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return Kernel.DocumentType.DocTypeSign.Income;
        }

    }

}
