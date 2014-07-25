using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Kernel
{
    public class Document
    {
        public Color ForegroundColor
        {
            get
            {
                return Color.Red;
            }
        }

        public int Id { get; set; }
        public string DayOfWeek { get { return DocDate.ToString("ddd"); } }
        public string DateString { get { return DocDate.ToString("dd.MM.yyyy"); } }
        public DateTime DocDate { get; set; }
        public DocumentType DocType { get; set; }
        /// <summary>
        /// Дебит - то, что принадлежит нам, то есть это наш приход
        /// </summary>
        public Transaction Debit { get; set; }
        /// <summary>
        /// Кредит - то, что принадлежит не нам, то есть это наш расход
        /// </summary>
        public Transaction Credit { get; set; }
        public string Shop { get; set; }
        public string Note { get; set; }
        /// <summary>
        /// Возвращает копию документа, не копирует ID
        /// </summary>
        /// <returns>Новый документ</returns>
        public Document CopyDocument() 
        {
            Document doc = new Document();
            if(Debit != null)
            {
                doc.Debit = new Transaction();
                doc.Debit.Account = Debit.Account;
                doc.Debit.Amount = Debit.Amount;
            }
            if(Credit != null)
            {
                doc.Credit= new Transaction();
                doc.Credit.Account = Credit.Account;
                doc.Credit.Amount = Credit.Amount;
            }
            doc.Shop = Shop;
            doc.DocDate = DocDate;
            doc.DocType = DocType;
            return doc;
        }
        /// <summary>
        /// Создание нового пустого документа
        /// </summary>
        /// <param name="sign">Тип создаваемого документа</param>
        /// <returns>Созданный документ</returns>
        public static Document CreateEmptyDocument(DocumentType.DocTypeSign sign) 
        {
            Document doc = new Document();
            doc.DocDate = DateTime.Now;
            doc.Debit = new Kernel.Transaction();
            doc.Credit = new Kernel.Transaction();
            switch (sign)
            {
                case DocumentType.DocTypeSign.Income:
                    doc.Credit = null; // нам не нужен кредит при приходе
                    break;
                case DocumentType.DocTypeSign.Cost:
                    doc.Debit = null; // нам не нужен дебит при расходе
                    break;
            }
            return doc;
        }

        /// <summary>
        /// Обнуляет сумму в документе
        /// </summary>
        public void ClearAmount()
        {
            if (Debit != null && Debit.Amount != null)
                Debit.Amount.Summa = 0;
            if (Credit != null && Credit.Amount != null)
                Credit.Amount.Summa = 0;
        }
    }
}
