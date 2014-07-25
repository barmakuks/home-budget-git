using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Windows.Media;

namespace Kernel
{
    public class ColorItem
    {
        public string Name { get; set; }
        public uint Value { get; set; }
        public ColorItem(string name, Color color)
        {
            Name = name;
            Value = ColorToUInt(color);
        }
        public override string ToString()
        {
            return Name;
        }
        public static uint ColorToUInt(Color color)
        {
            return (uint)((color.A << 24) | (color.R << 16) |
                          (color.G << 8) | (color.B << 0));
        }
        public static Color UIntToColor(uint color)
        {
            byte a = (byte)(color >> 24);
            byte r = (byte)(color >> 16);
            byte g = (byte)(color >> 8);
            byte b = (byte)(color >> 0);
            return Color.FromArgb(a, r, g, b);
        }
    }
    /// <summary>
    /// Глобальные справочники и списки
    /// </summary>
    public class Globals
    {
        private static AccountList _accounts = new AccountList();
        private static AccountList _active_accounts = null;
        private static Dictionary<uint,ColorItem> _colors = null;
        private static EventsList _events = new EventsList();

        public static AccountList Accounts 
        {
            get {
                return _accounts;
            }
            set {
                _accounts = value;
                BuildActiveAccounts();
            }
        }
        private static void BuildActiveAccounts()
        {
            _active_accounts = new AccountList();
            foreach (Account account in _accounts.Values) 
            {
                if (account.IsActive)
                    _active_accounts.Add(account.Id, account);
            }
            
        }
        public static AccountList ActiveAccounts 
        {
            get {
                return _active_accounts;
            }
        }
        /// <summary>
        /// глобальный список валют
        /// </summary>
        public static CurrencyList Currency = new CurrencyList();
        /// <summary>
        /// Глобальный список типов документов
        /// </summary>
        public static DocumentTypes DocTypes = new DocumentTypes();
        /// <summary>
        /// Глобальный список напоминаний
        /// </summary>
        public static EventsList Events 
        {
            get {
                return _events;
            }
        }
        public static ColorItem[] Colors         
        {
            get {
                if (_colors == null)
                    _colors = GetKnownColors();
                return _colors.Values.ToArray();
            }
        }
        public static ColorItem GetColor(uint color) 
        {
            if (_colors == null)
                _colors = GetKnownColors();
            if (_colors.ContainsKey(color))
                return _colors[color];
            else
                return _colors[0xFF000000];
        }

        private static Dictionary<uint, ColorItem> GetKnownColors()
        {
            Dictionary<uint, ColorItem> lst = new Dictionary<uint,ColorItem>();
            Type ColorType = typeof(System.Windows.Media.Colors);
            PropertyInfo[] arrPiColors = ColorType.GetProperties(BindingFlags.Public | BindingFlags.Static);
            
            foreach (PropertyInfo pi in arrPiColors) 
            {
                Color color = ((Color)pi.GetValue(null, null));
                if (!lst.ContainsKey(ColorItem.ColorToUInt(color)))
                    lst.Add(ColorItem.ColorToUInt(color), new ColorItem(pi.Name, color));
            }                
            return lst;
        }

    }
}
