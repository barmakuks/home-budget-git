using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.SQLite;


namespace db
{
    public class Database
    {
        private static string ConnectionString = @"Data Source=budget.sqlite;New=False;Version=3";
        private const string DATE_FORMAT = "yyyyMMdd";
        static Database() 
        {
            UpdateGlobals();
        }
        public static void UpdateGlobals()
        {
            Kernel.Globals.Currency = GetCurrencyList();
            Kernel.Globals.Accounts = GetAccountList();
            Kernel.Globals.DocTypes = GetDocumentTypeList();
            Kernel.Globals.Events.Clear();
            Kernel.Globals.Events.AddRange(GetEvents());
#if COMMUNAL
            Communal.Globals.CommunalTypes = GetCommunalTypeList();
            Communal.Globals.ActualTariffs = GetCommunalTariffs();
#endif
        }

        /// <summary>
        /// Задает строку подключения к БД
        /// </summary>
        /// <param name="connectionString">Строка подключения к БД</param>
        public static void Connect(string connectionString)
        {
            ConnectionString = connectionString;
        }

        #region Функции для работы с документами
        /// <summary>
        /// Возвращает список документов за период
        /// </summary>
        /// <param name="minDate">Начало периода</param>
        /// <param name="maxDate">Конец периода</param>
        /// <returns></returns>
        public static IEnumerable<Kernel.Document> GetDocuments(DateTime minDate, DateTime maxDate)
        {
            return GetDocuments(minDate, maxDate, null, null, -1);
        }
        /// <summary>
        /// Возвращает список документов за период с движениями по указанному счету
        /// </summary>
        /// <param name="minDate">Начало периода</param>
        /// <param name="maxDate">Конец периода</param>
        /// <param name="account">Счет. Если null, то все документы</param>
        /// <param name="currency">Валюта. Если null, то все документы</param>
        /// <returns></returns>
        public static IEnumerable<Kernel.Document> GetDocuments(DateTime minDate, DateTime maxDate, Kernel.Account account, Kernel.Currency currency) 
        {
            return GetDocuments(minDate, maxDate, account, currency, -1);
        }
        /// <summary>
        /// Возвращает список документов указанного типа за период с движениями по указанному счету
        /// </summary>
        /// <param name="minDate">Начало периода</param>
        /// <param name="maxDate">Конец периода</param>
        /// <param name="account">Счет. Если null, то все документы</param>
        /// <param name="currency">Валюта. Если null, то все документы</param>
        /// <param name="doc_type">Тип документа. Если null, то все документы</param>
        /// <returns></returns>
        public static IEnumerable<Kernel.Document> GetDocuments(DateTime minDate, DateTime maxDate, Kernel.Account account, Kernel.Currency currency, int doc_type_id)
        {
            string sql = String.Format(@"SELECT * from Documents WHERE doc_date BETWEEN '{0}' AND '{1}' ", minDate.ToString(DATE_FORMAT), maxDate.ToString(DATE_FORMAT));
            if (account != null)
                sql = sql + String.Format(" and (account_from_id = {0} or account_to_id = {0}) ", account.Id);
            if (currency != null)
                sql = sql + String.Format(" and (account_from_cur = {0} or account_to_cur = {0}) ", currency.Code);
            if (doc_type_id > -1) 
            {
                string doc_types = "";
                string separator = "";
                foreach(int id in GetDocTypeChildId(doc_type_id))
                {
                    doc_types = doc_types + separator + id.ToString();
                    separator = " ,";
                }
                sql = sql + String.Format(" and doc_type_id in ({0}) ", doc_types);
            }
                
            sql = sql + "ORDER BY doc_date DESC, id";

            List<Kernel.Document> docs = new List<Kernel.Document>();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Kernel.Document doc = new Kernel.Document();
                    doc.Id = Convert.ToInt32(reader["Id"]);
                    string date = reader["doc_date"].ToString();
                    doc.DocDate = DateTime.ParseExact(date, DATE_FORMAT, null);
                    doc.Note = Convert.ToString(reader["Note"]);
                    doc.Shop = Convert.ToString(reader["Shop"]);

                    if (!reader.IsDBNull(reader.GetOrdinal("account_from_id")) && Convert.ToInt32(reader["account_from_id"]) != 0)
                    {
                        doc.Credit = new Kernel.Transaction();
                        doc.Credit.Account = Kernel.Globals.Accounts[Convert.ToInt32(reader["account_from_id"])];
                        doc.Credit.Amount = new Kernel.Amount(Kernel.Globals.Currency[Convert.ToInt32(reader["account_from_cur"])], Convert.ToInt32(reader["amount_from"]));
                    }
                    if (!reader.IsDBNull(reader.GetOrdinal("account_to_id")) && Convert.ToInt32(reader["account_to_id"]) != 0)
                    {
                        doc.Debit = new Kernel.Transaction();
                        int id = Convert.ToInt32(reader["account_to_id"]);
                        doc.Debit.Account = Kernel.Globals.Accounts[id];
                        doc.Debit.Amount = new Kernel.Amount(Kernel.Globals.Currency[Convert.ToInt32(reader["account_to_cur"])], Convert.ToInt32(reader["amount_to"]));
                    }
                    doc.DocType = Kernel.Globals.DocTypes[Convert.ToInt32(reader["doc_type_id"])];
                    docs.Add(doc);
                }
            }
            catch (Exception )
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return docs;
        }

        /// <summary>
        /// Возвращает список магазинов, когда-либо указанных в документах 
        /// </summary>
        /// <returns></returns>
        public static IEnumerable GetShopsList()
        {
            string sql = @"SELECT distinct shop FROM documents WHERE shop IS NOT NULL and shop <> ''";
            List<string> shops = new List<string>();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    shops.Add(Convert.ToString(reader["shop"]));
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return shops;

        }

        /// <summary>
        /// Сохраняет документ в БД
        /// </summary>
        /// <param name="doc">Документ для сохранения</param>
        public static bool WriteDocument(Kernel.Document doc)
        {
            string sql = "";
            if (doc.Id <= 0)
            {
                sql = String.Format(@"INSERT INTO documents 
                (doc_date, doc_type_id, account_from_id, account_from_cur, amount_from, account_to_id, account_to_cur, amount_to, note, shop) 
                VALUES ('{0}',{1},{2},{3},{4},{5},{6},{7},'{8}','{9}')"
                    ,doc.DocDate.ToString(DATE_FORMAT), doc.DocType.Id,
                    doc.Credit == null ? "NULL" : doc.Credit.Account.Id.ToString(),
                    doc.Credit == null ? "NULL" : doc.Credit.Amount.Currency.Code.ToString(),
                    doc.Credit == null ? "NULL" : doc.Credit.Amount.Summa.ToString(),
                    doc.Debit == null ? "NULL" : doc.Debit.Account.Id.ToString(),
                    doc.Debit == null ? "NULL" : doc.Debit.Amount.Currency.Code.ToString(),
                    doc.Debit == null ? "NULL" : doc.Debit.Amount.Summa.ToString(),
                    doc.Note, doc.Shop);
            }
            else
            {
                sql = String.Format(@"UPDATE documents SET
                doc_date= '{0}', doc_type_id = {1}, account_from_id = {2}, account_from_cur = {3}, amount_from = {4},
                account_to_id = {5}, account_to_cur = {6}, amount_to = {7}, note = '{8}', shop = '{9}' 
                WHERE Id = {10}"
                    , doc.DocDate.ToString(DATE_FORMAT), doc.DocType.Id,
                    doc.Credit == null ? "NULL" : doc.Credit.Account.Id.ToString(),
                    doc.Credit == null ? "NULL" : doc.Credit.Amount.Currency.Code.ToString(),
                    doc.Credit == null ? "NULL" : doc.Credit.Amount.Summa.ToString(),
                    doc.Debit == null ? "NULL" : doc.Debit.Account.Id.ToString(),
                    doc.Debit == null ? "NULL" : doc.Debit.Amount.Currency.Code.ToString(),
                    doc.Debit == null ? "NULL" : doc.Debit.Amount.Summa.ToString(),
                    doc.Note, doc.Shop, doc.Id);
            }

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }            
        }
        /// <summary>
        /// Удаляет документ из БД
        /// </summary>
        /// <param name="doc_id">Id документа</param>
        /// <returns></returns>
        private static bool DeleteDocument(int doc_id)
        {
            string sql = String.Format(@"DELETE FROM documents WHERE id = {0}", doc_id);

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        /// <summary>
        /// Удаляет указанный документ из БД
        /// </summary>
        /// <param name="doc">Докумнт на удаление</param>
        /// <returns></returns>
        public static bool DeleteDocument(Kernel.Document doc) 
        {
            return DeleteDocument(doc.Id);
        }

        #endregion
        
        #region Фукнции расчета баланса
        /// <summary>
        /// Баланс счетов на определенную дату
        /// </summary>
        /// <param name="date">дата, на вечер которой рассчитывается баланс</param>
        /// <returns>Баланс счетов</returns>
        public static IEnumerable<Kernel.AccountBalance> GetAccountBalance(DateTime date) 
        {
            return GetAccountBalance(date, null);
        }
        /// <summary>
        /// Баланс счетов на определенную дату
        /// </summary>
        /// <param name="date">дата, на вечер которой рассчитывается баланс</param>
        /// <param name="account">счет, по которому рассчитывается баланс</param>
        /// <returns>Баланс счетов</returns>
        public static IEnumerable<Kernel.AccountBalance> GetAccountBalance(DateTime date, Kernel.Account account)
        {
            UpdateGlobals();
            string sql = @"SELECT acc.id as account_id, account_cur, SUM(amount) as total_amount, sort_order
            FROM accounts_view acc LEFT OUTER JOIN money_moving_view mm ON mm.account_id = acc.id
            WHERE doc_date <= '$DATE' $ACCOUNT
            GROUP BY acc.id, account_cur , sort_order HAVING total_amount <> 0
            ORDER BY sort_order, account_id, account_cur DESC";

            sql = sql.Replace("$DATE", date.ToString(DATE_FORMAT));
            if (account != null)
                sql = sql.Replace("$ACCOUNT", "and acc.account_id=" + account.Id.ToString());
            else
                sql = sql.Replace("$ACCOUNT", "");

            List<Kernel.AccountBalance> balance = new List<Kernel.AccountBalance>();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);            
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                Kernel.AccountBalance line = null;
                while (reader.Read())
                {
                    int id = Convert.ToInt32(reader["account_id"]);
                    if (line == null || id != line.Account.Id)
                    {
                        line = new Kernel.AccountBalance();
                        line.Account = Kernel.Globals.Accounts[Convert.ToInt32(reader["account_id"])];
                        balance.Add(line);
                    }
                    Kernel.Amount bal = new Kernel.Amount(Kernel.Globals.Currency[Convert.ToInt32(reader["account_cur"])], Convert.ToInt32(reader["total_amount"]));
                    line.Balance.Add(bal);                    
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return balance;
        }
        /// <summary>
        /// Возвращает общий баланс по всем счета
        /// </summary>
        /// <param name="balance">Список балансов счетов</param>
        /// <returns>Общий баланс</returns>
        public static Kernel.AccountBalance GetTotalBalance(IEnumerable<Kernel.AccountBalance> balance) 
        {
            Kernel.AccountBalance total = new Kernel.AccountBalance();
            total.Account = new Kernel.Account();
            total.Account.Name = "Всего";
            //пара КодВалюты-Сумма
            Dictionary<int, Kernel.Amount> total_cur = new Dictionary<int, Kernel.Amount>();
            foreach (Kernel.AccountBalance acc_balance in balance) 
            {
                foreach (Kernel.Amount amount in acc_balance.Balance)
                {
                    if(!total_cur.ContainsKey(amount.Currency.Code))
                        total_cur.Add(amount.Currency.Code, new Kernel.Amount(amount.Currency, 0));
                    total_cur[amount.Currency.Code].Summa += amount.Summa;
                }
            }
            foreach (Kernel.Amount amount in total_cur.Values)
            {
                total.Balance.Add(amount);
            }            
            return total;
        }
        #endregion

        #region Функции для работы с типами документов
        /// <summary>
        /// Возвращает список всех типов документов
        /// </summary>
        /// <returns></returns>
        public static Kernel.DocumentTypes GetDocumentTypeList() 
        {
            string sql = @"SELECT * FROM doc_types ORDER BY Parent_Id, Name";
            Kernel.DocumentTypes doc_types = new Kernel.DocumentTypes();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Kernel.DocumentType doc_type = new Kernel.DocumentType();
                    doc_type.Id = Convert.ToInt32(reader["Id"]);
                    doc_type.Name = Convert.ToString(reader["Name"]);
                    if(reader.IsDBNull(reader.GetOrdinal("parent_id")))
                        doc_type.ParentId = 0;
                    else
                        doc_type.ParentId = Convert.ToInt32(reader["parent_id"]);
                    switch(Convert.ToInt32(reader["operation_sign"]))
                    {
                        case -1: doc_type.Sign = Kernel.DocumentType.DocTypeSign.Cost; break;
                        case 0: doc_type.Sign = Kernel.DocumentType.DocTypeSign.Movement; break;
                        case 1: doc_type.Sign = Kernel.DocumentType.DocTypeSign.Income; break;
                        default: doc_type.Sign = Kernel.DocumentType.DocTypeSign.Income; break;
                    }
                    doc_types.Add(doc_type.Id, doc_type);
                }
            }
            catch (Exception )
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return doc_types;
        }

        /// <summary>
        /// Сохраняет Тип Документа в БД
        /// </summary>
        /// <param name="doc_type">Сохраняемый тип документа</param>
        /// <returns></returns>
        public static bool WriteDocumentType(Kernel.DocumentType doc_type) 
        {
            string sql = "";// String.Format(@"SELECT * FROM accounts WHERE Id = {0}", account.Id);
            if (doc_type.Id <= 0)
            {
                int sign = 0;
                switch (doc_type.Sign) 
                {
                    case Kernel.DocumentType.DocTypeSign.Cost:
                        sign = -1;
                        break;
                    case Kernel.DocumentType.DocTypeSign.Income:
                        sign = 1;
                        break;
                    default:
                        sign = 0;
                        break;
                }
                sql = String.Format("INSERT INTO doc_types (parent_id, operation_sign, name) VALUES ({0},{1},'{2}')",
                    doc_type.ParentId, sign, doc_type.Name);
            }
            else 
            {
                sql = String.Format("UPDATE doc_types SET name='{0}' WHERE Id = {1}",
                    doc_type.Name, doc_type.Id);
            }
            
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                if (doc_type.Id <= 0) 
                {
                    doc_type.Id = GetLastId("doc_types");
                }
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }

        }

        #endregion

        #region Функции для работы с валютой
        /// <summary>
        /// Возвращает список валют
        /// </summary>
        /// <returns></returns>
        public static Kernel.CurrencyList GetCurrencyList()
        {
            string sql = @"SELECT * FROM currency_list ORDER BY Code DESC";
            Kernel.CurrencyList currency = new Kernel.CurrencyList();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Kernel.Currency cur = new Kernel.Currency();
                    cur.Code = Convert.ToInt32(reader["Code"]);
                    cur.ISO = Convert.ToString(reader["ISO_Code"]);
                    cur.ShortName = Convert.ToString(reader["short_name"]);
                    object obj = reader["symbol"];
                    cur.Symbol = Convert.ToString(reader["symbol"]);
                    cur.SymbolPlace = (Convert.ToBoolean(reader["symbol_place"])) ? Kernel.Currency.Place.After : Kernel.Currency.Place.Before;
                    currency.Add(cur.Code, cur);
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return currency;
        }
        /// <summary>
        /// Сохраняет валюту в БД
        /// </summary>
        /// <param name="currency">Валюта</param>
        /// <returns>true, если операция прошла успешно</returns>
        public static bool WriteCurrency(Kernel.Currency currency) 
        {
            string sql = String.Format(@"SELECT * FROM currency_list WHERE CODE = {0}", currency.Code);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                if (reader.Read())
                {
                    sql = String.Format(@"UPDATE currency_list SET iso_code='{1}', short_name = '{2}', symbol = '{3}', symbol_place = {4} WHERE code = {0}", currency.Code, currency.ISO, currency.ShortName, currency.Symbol, currency.SymbolPlaceFlag);
                }
                else 
                {
                    sql = String.Format(@"INSERT INTO currency_list (code, iso_code, short_name, symbol, symbol_place) VALUES ({0}, '{1}', '{2}','{3}',{4})", currency.Code, currency.ISO, currency.ShortName, currency.Symbol, currency.SymbolPlaceFlag);
                }
                reader.Close();
                command.CommandText = sql;
                connection.Close();
                connection.Open();
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        /// <summary>
        /// Удаляет валюту из БД
        /// </summary>
        /// <param name="currency">Код валюты</param>
        /// <returns>true, если операция прошла успешно</returns>
        public static bool DeleteCurrency(int code)
        {
            return true;
        }
        #endregion

        #region Функции для работы со счетами
        /// <summary>
        /// Возвращает список всех счетов
        /// </summary>
        /// <returns></returns>
        public static Kernel.AccountList GetAccountList()
        {
            string sql = @"SELECT * FROM accounts_view ORDER BY sort_order";
            Kernel.AccountList accounts = new Kernel.AccountList();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Kernel.Account account = new Kernel.Account();
                    account.Id = Convert.ToInt32(reader["id"]);
                    account.IsActive = Convert.ToBoolean(reader["is_active"]);
                    account.Name = Convert.ToString(reader["name"]);
                    account.Description = Convert.ToString(reader["description"]);
                    account.SortOrder = Convert.ToInt32(reader["sort_order"]);                    
                    account.DefaultCurrency = Convert.ToInt32(reader["default_currency_code"]);
                    account.Bank = Convert.ToString(reader["bank"]);
                    account.ForegroundColor = Convert.ToUInt32(reader["foreground_color"]);
                    accounts.Add(account.Id, account);
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return accounts;
        }
        public static string[] GetBanksList() 
        {
            string sql = @"SELECT DISTINCT Bank FROM accounts WHERE bank IS NOT NULL and bank <>'' ORDER BY Bank";
            List<string> banks = new List<string>();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    banks.Add(Convert.ToString(reader["bank"]));
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return banks.ToArray();
        }
        /// <summary>
        /// Сохраняет счет в БД
        /// </summary>
        /// <param name="account">Счет</param>
        /// <returns></returns>
        public static bool WriteAccount(Kernel.Account account) 
        {
            string sql = "";// String.Format(@"SELECT * FROM accounts WHERE Id = {0}", account.Id);
            if (account.Id <= 0)
            {
                sql = String.Format("INSERT INTO accounts (name, description, default_currency_code, bank, is_active, foreground_color) VALUES ('{0}','{1}',{2},'{3}', {4}, {5})",
                    account.Name, account.Description, account.DefaultCurrency, account.Bank, account.IsActive ? 1 : 0, account.ForegroundColor);
            }
            else 
            {
                sql = String.Format("UPDATE accounts SET name='{0}', description='{1}', default_currency_code={2}, sort_order={3}, bank = '{5}', is_active = {6}, foreground_color = {7} WHERE Id = {4}",
                    account.Name, account.Description, account.DefaultCurrency, account.SortOrder, account.Id, account.Bank, account.IsActive ? 1 : 0, account.ForegroundColor);
            }
            
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        /// <summary>
        /// Перемещает счет в списке на одну позицию вверх
        /// </summary>
        /// <param name="account">Счет</param>
        /// <returns></returns>
        public static bool AccountMoveUp(Kernel.Account account) 
        {
            return AccountMove(account, true);
        }
        /// <summary>
        /// Перемещает счет в списке на одну позицию вниз
        /// </summary>
        /// <param name="account">Счет</param>
        /// <returns></returns>
        public static bool AccountMoveDown(Kernel.Account account) 
        {
            return AccountMove(account, false);
        }
        /// <summary>
        /// Выполняет перемещение счета по списку вверх или вниз
        /// </summary>
        /// <param name="account">Счет</param>
        /// <param name="move_up">true для перемещения вверх по списку, false - вниз</param>
        /// <returns></returns>
        private static bool AccountMove(Kernel.Account account,bool move_up) 
        {
            if ((move_up && account.SortOrder > 1) || (!move_up && account.SortOrder < Kernel.Globals.Accounts.Count))
            {
                int delta = move_up ? -1 : 1;
                foreach (Kernel.Account acc in Kernel.Globals.Accounts.Values)
                    {
                        if(acc.SortOrder == account.SortOrder + delta)
                        {
                            acc.SortOrder = account.SortOrder;
                            WriteAccount(acc);
                        }                            
                    }
                    account.SortOrder += delta;
                    WriteAccount(account);
                    Kernel.Globals.Accounts = GetAccountList();
                    return true;
            }
            return false;
        }
        /// <summary>
        /// Проверяет возможность удаление счета, т.е. отсутствие ссылки на него в документах
        /// </summary>
        /// <param name="account">Счет</param>
        /// <returns>true, если документ можно удалить из БД</returns>
        public static bool AccountCanDelete(Kernel.Account account) 
        {
            string sql = String.Format(@"SELECT * FROM documents WHERE account_from_id = {0} or account_to_id = {0} LIMIT 1", account.Id);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                if(reader.Read())
                {
                    return false;
                }
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        /// <summary>
        /// Удаляет счет из БД
        /// </summary>
        /// <param name="account">Счет</param>
        /// <returns>true, в случае успешного удаления</returns>
        public static bool AccountDelete(Kernel.Account account) 
        {
            if (AccountCanDelete(account))
            {
                string sql = String.Format(@"DELETE FROM accounts WHERE id = {0}", account.Id);
                SQLiteConnection connection = new SQLiteConnection(ConnectionString);
                try
                {
                    SQLiteCommand command = new SQLiteCommand(sql, connection);
                    command.CommandType = System.Data.CommandType.Text;
                    connection.Open();
                    command.ExecuteNonQuery();
                    return true;
                }
                catch (Exception)
                {
                    return false;
                }
                finally
                {
                    connection.Close();
                }
            }
            else return false;
        }

        #endregion

        #region Функции для работы с отчетами
        /// <summary>
        /// Формирует отчет по расходам и доходам в заданный промежуток времени
        /// </summary>
        /// <param name="minDate"></param>
        /// <param name="maxDate"></param>
        /// <returns></returns>
        public static Kernel.ReportItem[] GetReport(DateTime minDate, DateTime maxDate, Kernel.Currency currency) 
        {
            List<Kernel.ReportItem> report = new List<Kernel.ReportItem>();
            string sql = String.Format(@"SELECT * FROM report WHERE doc_date between '{0}' and '{1}' ",
                            minDate.ToString(DATE_FORMAT), maxDate.ToString(DATE_FORMAT));
            if (currency != null)
                sql = sql + String.Format(" and cur_code={0} ", currency.Code);
            sql = sql + " ORDER BY sign, cur_code DESC, doc_date";
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Kernel.ReportItem item = null;
                    int doc_type_id = Convert.ToInt32(reader["doc_type_id"]);
                    item = new Kernel.ReportItem();
                    item.DocType = Kernel.Globals.DocTypes[doc_type_id];
                    item.DocDate = DateTime.ParseExact(Convert.ToString(reader["doc_date"]),DATE_FORMAT, null);
                    
                    report.Add(item);
                    int cur_code = Convert.ToInt32(reader["cur_code"]);
                    if(!item.Amounts.ContainsKey(cur_code))
                        item.Amounts.Add(cur_code, new Kernel.Amount(Kernel.Globals.Currency[cur_code], 0));
                    item.Amounts[cur_code].Summa += Convert.ToInt32(reader["amount"]);
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }

            return report.ToArray();
        }
        /// <summary>
        /// Возвращает минимальную и максимальную дату, присутствующую в документах
        /// </summary>
        /// <param name="minDate"></param>
        /// <param name="maxDate"></param>
        public static void GetMaxDataPeriod(out DateTime minDate, out DateTime maxDate) 
        {
            minDate = DateTime.Today;
            maxDate = DateTime.Today;
            string sql = "SELECT MIN(min_date) as min_date, MAX(max_date) as max_date FROM min_max_dates";
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                if (reader.Read())
                {
                    minDate = DateTime.ParseExact(Convert.ToString(reader["min_date"]), DATE_FORMAT, null);
                    maxDate = DateTime.ParseExact(Convert.ToString(reader["max_date"]), DATE_FORMAT, null);
                }
            }
            catch (Exception)
            {
            }
            finally
            {
                connection.Close();
            }

        }

        /// <summary>
        /// Возвращает список ID всех дочерних типов документов
        /// </summary>
        /// <param name="doc_type_id">ID типа документа</param>
        /// <returns></returns>
        public static int[] GetDocTypeChildId(int doc_type_id) 
        {
            HashSet<int> doc_types = new HashSet<int>();
            
            string sql = String.Format(@"SELECT id FROM doc_types WHERE parent_id = {0}", doc_type_id);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    int id = Convert.ToInt32(reader["id"]);
                    doc_types.Add(id);
                }
                connection.Close();
                
                HashSet<int> childs = new HashSet<int>();
                foreach (int id in doc_types) 
                {                    
                    foreach(int child_id in GetDocTypeChildId(id))
                    {
                        childs.Add(child_id);
                    }
                }
                foreach (int id in childs) 
                {
                    doc_types.Add(id);
                }                
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                if(connection.State == ConnectionState.Open)
                    connection.Close();
            }
            doc_types.Add(doc_type_id);
            return doc_types.ToArray();
        }
        #endregion

        #region Функции для работы с напоминаниями

        /// <summary>
        /// Возвращает список напоминаний, для промежутка времени 
        /// </summary>
        /// <param name="minDate">Начало временного промежутка</param>
        /// <param name="maxDate">Конец временного промежутка</param>
        /// <returns></returns>
        public static IEnumerable<Kernel.Event> GetEvents()
        {
            // выбираем все сообщения, которые есть в системе
            string sql = @"SELECT * from Events ORDER BY is_active DESC, date";

            List<Kernel.Event> messages = new List<Kernel.Event>();
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Kernel.Event msg = new Kernel.Event();
                    msg.Id = Convert.ToInt32(reader["Id"]);
                    string date = reader["date"].ToString();
                    msg.Date = DateTime.ParseExact(date, DATE_FORMAT, null);
                    date = reader["last_date"].ToString();
                    msg.LastDate = DateTime.ParseExact(date, DATE_FORMAT, null);
                    msg.Message = reader["message"].ToString();
                    msg.ActionId = Convert.ToInt32(reader["action_id"]);
                    msg.IsActive = Convert.ToInt32(reader["is_active"]) == 1;
                    msg.Periodicity.ExactValue = Convert.ToInt32(reader["periodicity_value"]);
                    msg.Periodicity.Period = (Kernel.EventPeriodicity.Periodicity)(Convert.ToInt32(reader["periodicity_type"]));
                    msg.Duration = Convert.ToInt32(reader["duration"]);
                    // вычисляем ближайшую дату напоминания в промежутке между minDate и maxDate
                    msg.CalculateNextDate(DateTime.Today);
                    messages.Add(msg);
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return messages;
        }
        public static bool DeleteEvent(Kernel.Event evnt) 
        {
            string sql_delete_message = String.Format("DELETE FROM events  WHERE Id = {0}", evnt.Id);
            string sql_delete_action = String.Format("DELETE FROM event_actions WHERE Id = {0}", evnt.ActionId);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql_delete_message, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                command = new SQLiteCommand(sql_delete_action, connection);
                command.CommandType = System.Data.CommandType.Text;
                command.ExecuteNonQuery();
                Kernel.Globals.Events.Remove(evnt);
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        public static bool WriteEvent(Kernel.Event evnt) 
        {
            string sql_format = "";// String.Format(@"SELECT * FROM accounts WHERE Id = {0}", account.Id);
            if (evnt.Id <= 0)
            {
                sql_format = "INSERT INTO events (date, periodicity_type, periodicity_value, last_date, message, action_id, duration, is_active) VALUES ('{0}',{1},{2},'{3}', '{4}', {5}, {6}, {7})";
            }
            else
            {
                sql_format = "UPDATE events SET date = '{0}', periodicity_type= {1}, periodicity_value = {2}, last_date = '{3}', message = '{4}', action_id = {5}, duration = {6}, is_active= {7} WHERE Id = {8}";
            }
            string sql = String.Format(sql_format,
                evnt.Date.ToString(DATE_FORMAT), // msg_date, 
                Convert.ToInt32(evnt.Periodicity.Period), // periodicity_type, 
                evnt.Periodicity.ExactValue,// periodicity_value, 
                evnt.LastDate.ToString(DATE_FORMAT), // last_msg_date, 
                evnt.Message, // message_text, 
                evnt.ActionId, // linked_document_id, 
                evnt.Duration, // msg_duration
                evnt.IsActive ? "1" : "0",
                evnt.Id // Id
                );

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        #endregion

        #region Функции для доступа к локальным параметрам приложения
        public static string GetValue(string key) 
        {
            string sql = String.Format(@"SELECT value from params WHERE key = '{0}'", key.ToUpper());
            
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                if (reader.Read())
                {
                    return reader["value"].ToString();
                }
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
            return null;

        }
        public static string GetValue(string key, string if_null_value)
        {
            string value = GetValue(key);
            if (String.IsNullOrEmpty(value))
                return if_null_value;
            else
                return value;
        }
        public static void SetValue(string key, string value) 
        {
            string sql = String.Format(@"INSERT INTO params (key, value) VALUES ('{0}', '{1}')", key.ToUpper(), value);
            if(GetValue(key) != null)
                sql = String.Format(@"UPDATE params SET value = '{1}' WHERE key = '{0}'", key.ToUpper(), value);

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
            }
            catch (Exception)
            {
            }
            finally
            {
                connection.Close();
            }
        }
        public static void SetValue(string key, double value)
        {
            SetValue(key, value.ToString());
        }
        public static void SetValue(string key, int value)
        {
            SetValue(key, value.ToString());
        }
        #endregion

#if COMMUNAL
        #region Функции для работы с коммунальными платежами

        #region Виды коммунальных услуг
        public static Communal.CommunalTypesList GetCommunalTypeList() 
        {
            string sql = String.Format(@"SELECT * from communal_types order by name");

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                Communal.CommunalTypesList communal_types = new Communal.CommunalTypesList();
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Communal.CommunalType type = new Communal.CommunalType();
                    type.Id = Convert.ToInt32(reader["id"]);
                    type.Name = Convert.ToString(reader["name"]);
                    type.DefaultValue = Convert.ToDouble(reader["default_value"]);
                    type.Measure = Communal.MeasureTypes.GetType(reader["measure_type"]);
                    communal_types.Add(type.Id, type);
                }
                return communal_types;
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
        }
        public static bool WriteCommunalType(Communal.CommunalType communal_type) 
        {
            string sql_format = "INSERT INTO communal_types (name, measure_type, default_value) VALUES('{0}',{1},{2})";
            if (communal_type.Id > 0)
                sql_format = "UPDATE communal_types SET name = '{0}', measure_type = {1}, default_value = {2} WHERE Id = {3}";
            string sql = String.Format(sql_format, communal_type.Name, Convert.ToInt32(communal_type.Measure), communal_type.DefaultValue, communal_type.Id);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                if (communal_type.Id <= 0)
                    communal_type.Id = GetLastId("communal_types");
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        public static bool DeleteCommunalType(Communal.CommunalType communal_type) 
        {
            throw new NotImplementedException();
        }
        #endregion

        #region Тарифы
        /// <summary>
        /// Возвращает перечень тарифов, актаульных на указанную дату
        /// </summary>
        /// <param name="date"></param>
        /// <returns></returns>
        public static Communal.Tariffs GetCommunalTariffs() 
        {
            string sql = String.Format(@"SELECT * from communal_tariffs ORDER BY date DESC");

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                Communal.Tariffs tariffs = new Communal.Tariffs();
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Communal.Tariff tariff = new Communal.Tariff();
                    tariff.Id = Convert.ToInt32(reader["id"]);
                    tariff.Date = DateTime.ParseExact(reader["date"].ToString(), DATE_FORMAT, null);
                    tariff.Type = Communal.Globals.CommunalTypes[Convert.ToInt32(reader["communal_type_id"])];
                    tariff.Value = Convert.ToDouble(reader["value"]);
                    tariffs.Add(tariff);
                }
                return tariffs;
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
        }


        /// <summary>
        /// Сохраняет строку изменения тарифов
        /// </summary>
        /// <param name="tariff"></param>
        public static void WriteTariff(Communal.Tariff tariff) 
        {
            string sql_format = "INSERT INTO communal_tariffs (communal_type_id,date,value) VALUES({0},'{1}',{2})";
            if (tariff.Id > 0)
                sql_format = "UPDATE communal_tariffs SET communal_type_id = {0}, date = '{1}', value = {2} WHERE Id = {3}";
            string sql = String.Format(sql_format, tariff.Type.Id, tariff.Date.ToString(DATE_FORMAT), tariff.Value, tariff.Id);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                if (tariff.Id <= 0)
                    tariff.Id = GetLastId("communal_tariffs");                
            }
            catch (Exception)
            {
                
            }
            finally
            {
                connection.Close();
            }            
        }
        /// <summary>
        /// Удаляет изменение тарифов
        /// </summary>
        /// <param name="tariff"></param>
        public static void DeleteTariff(Communal.Tariff tariff) 
        {
            string sql = String.Format("DELETE FROM communal_tariffs WHERE Id = {0}", tariff.Id);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                Communal.Globals.ActualTariffs = GetCommunalTariffs();
            }
            catch (Exception)
            {

            }
            finally
            {
                connection.Close();
            }
        }
        #endregion

        #region Коммунальные платежи и начисления
        public static Communal.CommunalBalance[] GetCommunalBalance(DateTime date) 
        {
            string sql = String.Format(@"SELECT Id, name, SUM(amount * sign) as Amount, SUM(amount_alt*sign) as amount_alt from communal_balance_view WHERE date IS NULL or date <= '{0}' GROUP BY id, name ORDER BY name" , date.ToString(DATE_FORMAT));

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                List<Communal.CommunalBalance> balance_list = new List<Communal.CommunalBalance>();
                
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Communal.CommunalBalance balance = new Communal.CommunalBalance();
                    balance.Balance = ToInt(reader["amount"]);
                    balance.BalanceAlt = ToInt(reader["amount_alt"]);
                    balance.Type = Communal.Globals.CommunalTypes[ToInt(reader["id"])];
                    balance_list.Add(balance);
                }
                return balance_list.ToArray();
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }            
        }
        public static int GetLastCounterValue(int communal_type_id, DateTime period)
        {
            string sql = String.Format(@"SELECT MAX(counter_after) as counter_after from communal_documents WHERE period <= '{0}' and communal_type_id = {1}", period.ToString(DATE_FORMAT), communal_type_id);

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                List<Communal.CommunalBalance> balance_list = new List<Communal.CommunalBalance>();

                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                if (reader.Read())
                {
                    return ToInt(reader["counter_after"]);
                }
                return 0;
            }
            catch (Exception)
            {
                return 0;
            }
            finally
            {
                connection.Close();
            }
        }
        public static bool WriteDocument(Communal.Document document)
        {
            string sql_format = @"INSERT INTO communal_documents (date, communal_type_id, period_begin, counter_before, counter_after, counter, amount, amount_alt, sign, tariff, period_end) VALUES('{0}',{1},'{2}',{3},{4},{5},{6},{7},{8},{9},'{10}')";
            if (document.Id > 0)
                sql_format = "UPDATE communal_documents SET date = '{0}', communal_type_id = {1}, period_begin = '{2}', counter_before = {3}, counter_after = {4}, counter = {5}, amount = {6}, amount_alt = {7}, sign = {8}, tariff = {9}, period_end= '10' WHERE Id = {11}";
            string sql = String.Format(sql_format, document.Date.ToString(DATE_FORMAT), 
                            document.Type.Id, document.PeriodBegin.ToString(DATE_FORMAT), document.CounterBefore, document.CounterAfter, document.Counter,
                            document.Amount, document.AmountAlt, document.Sign,document.Tariff, document.PeriodEnd, document.Id);
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                if (document.Id <= 0)
                    document.Id = GetLastId("communal_documents");
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }
        public static IEnumerable GetCommunalDocuments(DateTime dateMin, DateTime dateMax)
        {
            string sql = String.Format(@"SELECT * from communal_documents WHERE date between '{0}' and '{1}' ORDER BY date DESC", dateMin.ToString(DATE_FORMAT), dateMax.ToString(DATE_FORMAT));

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                List<Communal.Document> documents = new List<Communal.Document>();

                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    Communal.Document doc = new Communal.Document();
                    doc.Amount = ToInt(reader["amount"]);
                    doc.AmountAlt = ToInt(reader["amount_alt"]);
                    doc.Counter = ToInt(reader["counter"]);
                    doc.CounterAfter = ToInt(reader["counter_after"]);
                    doc.CounterBefore = ToInt(reader["counter_before"]);
                    doc.Date = DateTime.ParseExact(reader["date"].ToString(), DATE_FORMAT, null);
                    doc.Id = ToInt(reader["id"]);
                    doc.PeriodBegin = DateTime.ParseExact(reader["period_begin"].ToString(), DATE_FORMAT, null);
                    doc.PeriodEnd = DateTime.ParseExact(reader["period_end"].ToString(), DATE_FORMAT, null);
                    doc.Sign = ToInt(reader["sign"]);
                    doc.Tariff = Convert.ToDouble(reader["tariff"]);
                    doc.Type = Communal.Globals.CommunalTypes[ToInt(reader["communal_type_id"])];
                    documents.Add(doc);
                }
                return documents;
            }
            catch (Exception)
            {
                return null;
            }
            finally
            {
                connection.Close();
            }
        }
        public static bool DeleteDocument(Communal.Document doc)
        {
            string sql = String.Format(@"DELETE FROM communal_documents WHERE id = {0}", doc.Id);

            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                command.ExecuteNonQuery();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                connection.Close();
            }
        }

        #endregion
        #endregion
#endif
        #region Вспомогательные функции 
        private static int ToInt(object obj) 
        {
            return ToInt(obj, 0);
        }
        private static int ToInt(object obj, int if_null) 
        {
            if (obj is DBNull || obj == null)
                return if_null;
            else return Convert.ToInt32(obj);
        }
        /// <summary>
        /// Возвращает значение максимального Id из таблицы
        /// </summary>
        /// <param name="table_name">Имя таблицы</param>
        /// <returns></returns>
        private static int GetLastId(string table_name)
        {
            string sql = @"SELECT MAX(id) as last_id FROM " + table_name;
            SQLiteConnection connection = new SQLiteConnection(ConnectionString);
            try
            {
                SQLiteCommand command = new SQLiteCommand(sql, connection);
                command.CommandType = System.Data.CommandType.Text;
                connection.Open();
                SQLiteDataReader reader = command.ExecuteReader();
                if (reader.Read())
                {
                    return Convert.ToInt32(reader["last_id"]);
                }
            }
            catch (Exception)
            {
                return -1;
            }
            finally
            {
                connection.Close();
            }
            return -1;
        }
        #endregion

    }
}
