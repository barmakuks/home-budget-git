;-- --- -- Globals -- --- -- SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
-- SET FOREIGN_KEY_CHECKS=0;
-- --- -- Table 'accounts' -- -- --- 
-- DROP TABLE IF EXISTS accounts;
CREATE TABLE accounts (id INTEGER NOT NULL AUTO_INCREMENT,
                        name VARCHAR(50) NOT NULL,
                        description VARCHAR(255) DEFAULT NULL,
                        default_currency_code INTEGER NOT NULL,
                        sort_order INTEGER DEFAULT 0,
                        bank VARCHAR(255) DEFAULT NULL,
                        is_active INTEGER NOT NULL DEFAULT 1,
                        foreground_color INTEGER NOT NULL DEFAULT 0,
                        PRIMARY KEY (id) );
-- --- -- Table currency_list -- -- --- 
CREATE TABLE currency_list (code INTEGER NOT NULL,
                        iso_code VARCHAR(5) NOT NULL,
                        short_name VARCHAR(10) NOT NULL,
                        symbol CHAR(1) DEFAULT NULL,
                        place INTEGER NOT NULL,
                        PRIMARY KEY (code) );
 -- --- -- Table doc_types -- -- --- 
CREATE TABLE doc_types (id INTEGER NOT NULL AUTO_INCREMENT,
                        parent_id INTEGER DEFAULT NULL,
                        operation_sign INTEGER DEFAULT NULL,
                        name VARCHAR(50) DEFAULT NULL,
                        PRIMARY KEY (id) );
 -- --- -- Table documents -- -- --- 
CREATE TABLE documents (id INTEGER NOT NULL AUTO_INCREMENT,
                        doc_date CHAR(8) NOT NULL,
                        doc_type_id INTEGER NOT NULL,
                        account_from_id INTEGER DEFAULT NULL,
                        account_from_cur INTEGER DEFAULT NULL,
                        amount_from INTEGER DEFAULT NULL,
                        account_to_id INTEGER DEFAULT NULL,
                        account_to_cur INTEGER DEFAULT NULL,
                        amount_to INTEGER DEFAULT NULL,
                        note VARCHAR(100) DEFAULT NULL,
                        shop VARCHAR(50) DEFAULT NULL, PRIMARY KEY (id) );
 -- --- -- Table events -- -- --- 
CREATE TABLE events ( id INTEGER NOT NULL AUTO_INCREMENT,
                        date CHAR(8) NOT NULL,
                        periodicity_type INTEGER NOT NULL,
                        periodicity_value INTEGER DEFAULT NULL,
                        last_date CHAR(8) DEFAULT NULL,
                        message VARCHAR(255) DEFAULT NULL,
                        duration INTEGER DEFAULT NULL,
                        is_active INTEGER NOT NULL DEFAULT 1,
                        PRIMARY KEY (id) );
 -- --- -- Table params -- -- --- 
CREATE TABLE params ( key VARCHAR NOT NULL,
                        value VARCHAR DEFAULT NULL,
                        PRIMARY KEY (key));
 -- --- -- Foreign Keys -- --- 
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Maestro','',980,4,'Банк Базис',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('VISA. UAH','',980,5,'ProCreditBank',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('VISA. EUR','',978,6,'ProCreditBank',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Visa Classic','',980,19,'ProCreditBank',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Депозит','',980,9,'Piraeusbank',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Депозит','',980,16,'ProCreditBank',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Депозит','',980,15,'Raiffeisen Bank Aval',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Депозит','',980,17,'UkrEximBank',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Депозит','',978,10,'Дельта банк',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Депозит','',840,11,'Приватбанк',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Депозит','',980,18,'УкрСибБанк',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Друзья','',980,20,'',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Копилка','',980,3,'',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Лимаренки','',980,21,'',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Наличные','',980,1,'',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Проценты по вкладу','',980,22,'ProCreditBank',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Родители','',980,13,'',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Родители Юли','',980,14,'',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Юлина контора','',980,12,'',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Яков','',840,23,'',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Счет 2600 для ЧП','Счет для предпринимателя',980,24,'УкрСибБанк',0);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('Питание','',980,2,'',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('vitalii.misiura','счет misiura.vitalii.gmail.com на moneybookers.com',978,7,'Moneybookers',1);
INSERT INTO accounts (name,description, default_currency_code, sort_order, bank, is_active) VALUES('VISA Classic','',978,8,'VAB-Bank',1);
INSERT INTO currency_list VALUES(643,'RUB','р.','р',1);
INSERT INTO currency_list VALUES(840,'USD','дол.','$',0);
INSERT INTO currency_list VALUES(978,'EUR','евро','€',0);
INSERT INTO currency_list VALUES(980,'ГРН','грн.','',1);
INSERT INTO doc_types VALUES(1,0,-1,'Все расходы');
INSERT INTO doc_types VALUES(2,1,-1,'Питание');
INSERT INTO doc_types VALUES(3,1,-1,'Транспорт');
INSERT INTO doc_types VALUES(4,1,-1,'Одежда и другие вещи');
INSERT INTO doc_types VALUES(5,2,-1,'Питание на работе');
INSERT INTO doc_types VALUES(6,2,-1,'Продукты питания');
INSERT INTO doc_types VALUES(7,1,-1,'Развлечения и подарки');
INSERT INTO doc_types VALUES(8,7,-1,'Подарки');
INSERT INTO doc_types VALUES(9,5,-1,'Питание на работе Виталик');
INSERT INTO doc_types VALUES(10,3,-1,'Метро');
INSERT INTO doc_types VALUES(11,3,-1,'Трамвай');
INSERT INTO doc_types VALUES(12,3,-1,'Автобус');
INSERT INTO doc_types VALUES(13,3,-1,'Такси');
INSERT INTO doc_types VALUES(14,5,-1,'Питание на работе Юля');
INSERT INTO doc_types VALUES(15,83,-1,'Масло сливочное');
INSERT INTO doc_types VALUES(16,85,-1,'Растительное масло');
INSERT INTO doc_types VALUES(17,1,-1,'Пресса');
INSERT INTO doc_types VALUES(18,17,-1,'Газета( програмка)');
INSERT INTO doc_types VALUES(19,1,-1,'Гигиена и здоровье');
INSERT INTO doc_types VALUES(20,6,-1,'Яйца');
INSERT INTO doc_types VALUES(21,81,-1,'Изюм');
INSERT INTO doc_types VALUES(22,83,-1,'Молоко');
INSERT INTO doc_types VALUES(23,1,-1,'Прочие расходы');
INSERT INTO doc_types VALUES(24,53,-1,'Чай');
INSERT INTO doc_types VALUES(25,88,-1,'Мука и макаронные изделия');
INSERT INTO doc_types VALUES(26,82,-1,'Вино');
INSERT INTO doc_types VALUES(27,86,-1,'Готовые закуски');
INSERT INTO doc_types VALUES(28,6,-1,'Сладости');
INSERT INTO doc_types VALUES(29,1,-1,'Хозяйственные принадлежности');
INSERT INTO doc_types VALUES(30,88,-1,'Хлебобулочные изделия');
INSERT INTO doc_types VALUES(31,6,-1,'Колбаса');
INSERT INTO doc_types VALUES(32,1,-1,'Квартира');
INSERT INTO doc_types VALUES(33,32,-1,'Телефон');
INSERT INTO doc_types VALUES(34,3,-1,'Маршрутка');
INSERT INTO doc_types VALUES(35,29,-1,'Канцелярские принадлежности');
INSERT INTO doc_types VALUES(36,19,-1,'Лекарства');
INSERT INTO doc_types VALUES(37,3,-1,'Троллейбус');
INSERT INTO doc_types VALUES(38,81,-1,'Цитрусовые');
INSERT INTO doc_types VALUES(39,83,-1,'Сырковая масса и творог');
INSERT INTO doc_types VALUES(40,17,-1,'Книги');
INSERT INTO doc_types VALUES(41,83,-1,'Сыр');
INSERT INTO doc_types VALUES(42,83,-1,'Сметана');
INSERT INTO doc_types VALUES(43,84,-1,'Капуста');
INSERT INTO doc_types VALUES(44,28,-1,'Печенье, торты и пирожные');
INSERT INTO doc_types VALUES(45,85,-1,'Пряности и приправы');
INSERT INTO doc_types VALUES(46,28,-1,'Конфеты и шоколад');
INSERT INTO doc_types VALUES(47,29,-1,'Туалет и ванная');
INSERT INTO doc_types VALUES(48,58,-1,'Говядина и телятина');
INSERT INTO doc_types VALUES(49,1,-1,'Потери');
INSERT INTO doc_types VALUES(50,81,-1,'Яблоки');
INSERT INTO doc_types VALUES(51,58,-1,'Рыба');
INSERT INTO doc_types VALUES(52,8,-1,'Цветы');
INSERT INTO doc_types VALUES(53,6,-1,'Напитки');
INSERT INTO doc_types VALUES(54,82,-1,'Коньяк');
INSERT INTO doc_types VALUES(55,84,-1,'Лук');
INSERT INTO doc_types VALUES(56,84,-1,'Фасоль');
INSERT INTO doc_types VALUES(57,81,-1,'Бананы');
INSERT INTO doc_types VALUES(58,6,-1,'Мясные продукты');
INSERT INTO doc_types VALUES(59,3,-1,'Поезд');
INSERT INTO doc_types VALUES(60,85,-1,'Соусы');
INSERT INTO doc_types VALUES(61,58,-1,'Копчености и нарезка');
INSERT INTO doc_types VALUES(62,81,-1,'Маслины');
INSERT INTO doc_types VALUES(63,53,-1,'Сок');
INSERT INTO doc_types VALUES(64,86,-1,'Салаты');
INSERT INTO doc_types VALUES(65,84,-1,'Редис');
INSERT INTO doc_types VALUES(66,84,-1,'Огрурцы');
INSERT INTO doc_types VALUES(67,83,-1,'Мороженное');
INSERT INTO doc_types VALUES(68,28,-1,'Сироп');
INSERT INTO doc_types VALUES(69,84,-1,'Кабачки');
INSERT INTO doc_types VALUES(70,84,-1,'Картофель');
INSERT INTO doc_types VALUES(71,84,-1,'Помидоры');
INSERT INTO doc_types VALUES(72,81,-1,'Клубника');
INSERT INTO doc_types VALUES(73,81,-1,'Вишня и черешня');
INSERT INTO doc_types VALUES(74,53,-1,'Квас');
INSERT INTO doc_types VALUES(75,82,-1,'Водка');
INSERT INTO doc_types VALUES(76,53,-1,'Кофе');
INSERT INTO doc_types VALUES(77,84,-1,'Грибы');
INSERT INTO doc_types VALUES(78,53,-1,'Вода');
INSERT INTO doc_types VALUES(79,81,-1,'Дыня');
INSERT INTO doc_types VALUES(80,4,-1,'Одежда Юля');
INSERT INTO doc_types VALUES(81,6,-1,'Ягоды и фрукты');
INSERT INTO doc_types VALUES(82,6,-1,'Спиртные напитки');
INSERT INTO doc_types VALUES(83,6,-1,'Молочные продукты');
INSERT INTO doc_types VALUES(84,6,-1,'Овощи и грибы');
INSERT INTO doc_types VALUES(85,6,-1,'Пряности, приправы, соусы');
INSERT INTO doc_types VALUES(86,6,-1,'Салаты, закуски');
INSERT INTO doc_types VALUES(87,6,-1,'Крупы');
INSERT INTO doc_types VALUES(88,6,-1,'Мучные изделия');
INSERT INTO doc_types VALUES(89,58,-1,'Свинина');
INSERT INTO doc_types VALUES(90,23,-1,'Печать фотографий');
INSERT INTO doc_types VALUES(91,84,-1,'Морковь');
INSERT INTO doc_types VALUES(92,53,-1,'Пиво');
INSERT INTO doc_types VALUES(93,86,-1,'Консервация');
INSERT INTO doc_types VALUES(94,101,-1,'Комп. диск');
INSERT INTO doc_types VALUES(95,29,-1,'Средства для уборки');
INSERT INTO doc_types VALUES(96,32,-1,'Кабельное TV');
INSERT INTO doc_types VALUES(97,83,-1,'Кефир');
INSERT INTO doc_types VALUES(98,1,-1,'Сотовый телефон');
INSERT INTO doc_types VALUES(99,7,-1,'Копилка');
INSERT INTO doc_types VALUES(100,83,-1,'Йогурт и десерт');
INSERT INTO doc_types VALUES(101,1,-1,'Компьютер');
INSERT INTO doc_types VALUES(102,81,-1,'Персик');
INSERT INTO doc_types VALUES(103,29,-1,'Инструменты и детали');
INSERT INTO doc_types VALUES(104,58,-1,'Курица');
INSERT INTO doc_types VALUES(105,58,-1,'Субпродукты');
INSERT INTO doc_types VALUES(106,81,-1,'Ананас');
INSERT INTO doc_types VALUES(107,29,-1,'Стирка');
INSERT INTO doc_types VALUES(108,86,-1,'Консервы, паштеты, икра и пр.');
INSERT INTO doc_types VALUES(109,28,-1,'Сахар');
INSERT INTO doc_types VALUES(110,81,-1,'Виноград');
INSERT INTO doc_types VALUES(111,84,-1,'Чеснок');
INSERT INTO doc_types VALUES(112,88,-1,'Вареники и пельмени');
INSERT INTO doc_types VALUES(113,29,-1,'Постельное белье');
INSERT INTO doc_types VALUES(114,7,-1,'Отпуск');
INSERT INTO doc_types VALUES(115,23,-1,'Налог');
INSERT INTO doc_types VALUES(116,17,-1,'Другая печатная продукция');
INSERT INTO doc_types VALUES(117,81,-1,'Авокадо');
INSERT INTO doc_types VALUES(118,101,-1,'Internet');
INSERT INTO doc_types VALUES(119,0,1,'Доходы');
INSERT INTO doc_types VALUES(120,119,1,'Доходы Виталика');
INSERT INTO doc_types VALUES(121,119,1,'Доходы Юли');
INSERT INTO doc_types VALUES(122,119,1,'Общие доходы');
INSERT INTO doc_types VALUES(123,0,0,'Дали в долг');
INSERT INTO doc_types VALUES(124,0,0,'Взяли в долг');
INSERT INTO doc_types VALUES(125,0,0,'Мы вернули долг');
INSERT INTO doc_types VALUES(126,0,0,'Нам вернули долг');
INSERT INTO doc_types VALUES(127,0,0,'Обмен валюты');
INSERT INTO doc_types VALUES(128,0,0,'Снятие наличных');
INSERT INTO doc_types VALUES(129,120,1,'ПО BEZMA');
INSERT INTO doc_types VALUES(130,120,1,'ПО Salling');
INSERT INTO doc_types VALUES(131,120,1,'Сопровождение Metalica');
INSERT INTO doc_types VALUES(132,119,1,'Остатки с прошлого года');
INSERT INTO doc_types VALUES(133,19,-1,'Больница и лечение');
INSERT INTO doc_types VALUES(134,19,-1,'Банные принадлежности');
INSERT INTO doc_types VALUES(135,19,-1,'Личная гигиена');
INSERT INTO doc_types VALUES(136,134,-1,'Мыло');
INSERT INTO doc_types VALUES(137,134,-1,'Шампунь');
INSERT INTO doc_types VALUES(138,135,-1,'Прокладки');
INSERT INTO doc_types VALUES(139,135,-1,'Депилятор');
INSERT INTO doc_types VALUES(140,135,-1,'Дезодорант');
INSERT INTO doc_types VALUES(141,134,-1,'Ополаскиватель для волос');
INSERT INTO doc_types VALUES(142,135,-1,'Бритвенные принадлежности');
INSERT INTO doc_types VALUES(143,135,-1,'Зубная паста');
INSERT INTO doc_types VALUES(144,135,-1,'Зубная щетка');
INSERT INTO doc_types VALUES(145,4,-1,'Одежда Виталик');
INSERT INTO doc_types VALUES(146,145,-1,'Верхняя одежда');
INSERT INTO doc_types VALUES(147,145,-1,'Обувь');
INSERT INTO doc_types VALUES(148,145,-1,'Белье и носки');
INSERT INTO doc_types VALUES(149,145,-1,'Летняя одежда');
INSERT INTO doc_types VALUES(150,145,-1,'Аксессуары');
INSERT INTO doc_types VALUES(151,80,-1,'Аксессуары');
INSERT INTO doc_types VALUES(152,80,-1,'Обувь');
INSERT INTO doc_types VALUES(153,80,-1,'Белье');
INSERT INTO doc_types VALUES(154,80,-1,'Летняя одежда');
INSERT INTO doc_types VALUES(155,80,-1,'Верхняя одежда');
INSERT INTO doc_types VALUES(156,135,-1,'Краска для волос');
INSERT INTO doc_types VALUES(157,4,-1,'Ремонт одежды');
INSERT INTO doc_types VALUES(158,32,-1,'Квартплата');
INSERT INTO doc_types VALUES(159,32,-1,'Ремонт');
INSERT INTO doc_types VALUES(160,32,-1,'Бытовая техника');
INSERT INTO doc_types VALUES(161,32,-1,'Мебель');
INSERT INTO doc_types VALUES(162,19,-1,'Спорт');
INSERT INTO doc_types VALUES(163,19,-1,'Сауна');
INSERT INTO doc_types VALUES(164,162,-1,'Бильярд');
INSERT INTO doc_types VALUES(165,162,-1,'Футбол');
INSERT INTO doc_types VALUES(166,135,-1,'Косметика');
INSERT INTO doc_types VALUES(167,19,-1,'Парикмахерская');
INSERT INTO doc_types VALUES(168,4,-1,'Химчистка');
INSERT INTO doc_types VALUES(169,29,-1,'Кухня');
INSERT INTO doc_types VALUES(170,29,-1,'Бытовая химия');
INSERT INTO doc_types VALUES(171,107,-1,'Стиральный порошок');
INSERT INTO doc_types VALUES(172,107,-1,'Ополаскиватель');
INSERT INTO doc_types VALUES(173,107,-1,'Отбеливатель');
INSERT INTO doc_types VALUES(174,29,-1,'Средства для чистки вещей и об');
INSERT INTO doc_types VALUES(175,169,-1,'Посуда');
INSERT INTO doc_types VALUES(176,169,-1,'Кухонные принадлежности');
INSERT INTO doc_types VALUES(177,7,-1,'Досуг');
INSERT INTO doc_types VALUES(178,177,-1,'Кафе');
INSERT INTO doc_types VALUES(179,177,-1,'Кино и театр');
INSERT INTO doc_types VALUES(180,177,-1,'Встреча с друзьями');
INSERT INTO doc_types VALUES(181,7,-1,'Праздники');
INSERT INTO doc_types VALUES(182,7,-1,'Праздники на работе');
INSERT INTO doc_types VALUES(183,182,-1,'Праздники на работе у Юли');
INSERT INTO doc_types VALUES(184,8,-1,'Алеша');
INSERT INTO doc_types VALUES(185,8,-1,'Дима');
INSERT INTO doc_types VALUES(186,8,-1,'Леся');
INSERT INTO doc_types VALUES(187,8,-1,'Игорь');
INSERT INTO doc_types VALUES(188,8,-1,'Мама Юли');
INSERT INTO doc_types VALUES(189,8,-1,'Папа Юли');
INSERT INTO doc_types VALUES(190,8,-1,'Анастасия Тимченко');
INSERT INTO doc_types VALUES(191,8,-1,'Мама Виталика');
INSERT INTO doc_types VALUES(192,8,-1,'Папа Виталика');
INSERT INTO doc_types VALUES(193,8,-1,'Жуковы');
INSERT INTO doc_types VALUES(194,8,-1,'Домничи');
INSERT INTO doc_types VALUES(195,8,-1,'Скворцовы');
INSERT INTO doc_types VALUES(196,8,-1,'Виталик');
INSERT INTO doc_types VALUES(197,8,-1,'Юля');
INSERT INTO doc_types VALUES(198,31,-1,'Колбаса варенная');
INSERT INTO doc_types VALUES(199,31,-1,'Колбаса сырокопченная');
INSERT INTO doc_types VALUES(200,84,-1,'Зелень');
INSERT INTO doc_types VALUES(201,82,-1,'Экзотические напитки');
INSERT INTO doc_types VALUES(202,86,-1,'Пицца');
INSERT INTO doc_types VALUES(203,81,-1,'Экзотические фрукты');
INSERT INTO doc_types VALUES(204,8,-1,'на новый год');
INSERT INTO doc_types VALUES(205,23,-1,'кульки');
INSERT INTO doc_types VALUES(206,85,-1,'Майонез');
INSERT INTO doc_types VALUES(207,19,-1,'Жевательная резинка');
INSERT INTO doc_types VALUES(208,86,-1,'к пиву');
INSERT INTO doc_types VALUES(209,23,-1,'почта');
INSERT INTO doc_types VALUES(210,32,-1,'кредит');
INSERT INTO doc_types VALUES(211,1,-1,'командировка');
INSERT INTO doc_types VALUES(212,0,0,'Перемещение');
INSERT INTO doc_types VALUES(213,162,-1,'рыбалка');
INSERT INTO doc_types VALUES(214,121,1,'премия Юля');
INSERT INTO doc_types VALUES(215,119,1,'Проценты по вкладам');
INSERT INTO doc_types VALUES(216,101,-1,'Фотоаппарат');
INSERT INTO doc_types VALUES(217,162,-1,'Каток');
INSERT INTO doc_types VALUES(218,23,-1,'Обслуживание в банке');
INSERT INTO doc_types VALUES(220,120,1,'ПО Competition manager');
INSERT INTO doc_types VALUES(221,121,1,'зарплата Юли');
INSERT INTO doc_types VALUES(222,120,1,'Другое ПО');
INSERT INTO doc_types VALUES(223,83,-1,'Сливки');
INSERT INTO doc_types VALUES(224,29,-1,'для цветов');

