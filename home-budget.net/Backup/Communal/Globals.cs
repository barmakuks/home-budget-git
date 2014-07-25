using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Communal
{
    public class Globals
    {
        private static Tariffs _tariffs = null;
        private static CommunalTypesList _communal_types = null;

        /// <summary>
        /// Перечень действующих тарифов
        /// </summary>
        public static Tariffs ActualTariffs 
        {
            get {
                return _tariffs;
            }
            set {
                _tariffs = value;
            }
        }
        /// <summary>
        /// Перечень видов коммунальных услуг
        /// </summary>
        public static CommunalTypesList CommunalTypes 
        {
            get {
                return _communal_types;
            }
            set {
                _communal_types = value;
            }
        }
    }
}
