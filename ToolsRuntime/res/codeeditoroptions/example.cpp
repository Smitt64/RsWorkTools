// Адрес субъекта
typedef struct
{
    db_int32 partyid;              // Идентификатор субъекта
    db_int16 type;                 // Номер вида адреса
    char     adress[512];          // Адрес
    char     country[4];           // Русский код страны
    char     postindex[16];        // Индекс
    char     region[251];          // Регион
    char     codeprovince[51];     // Наименование кода района
    char     province[251];        // Район
    char     codedistrict[51];     // Наименование кода города
    char     district[251];        // Город
    char     codeplace[51];        // Наименование кода населенного пункта
    char     place[251];           // Населенный пункт
    char     codestreet[51];       // Наименование кода улицы
    char     street[251];          // Улица
    char     house[51];            // Номер дома
    char     numcorps[51];         // Номер корпуса
    char     flat[101];            // Номер квартиры
    char     territory[3];         // Код территории
    db_int16 branch;               // Подразделение
    db_int32 numsession;           // Номер сессии
    char     kladr[24];            // Код КЛАДР
    char     coderegion[51];       // Наименование кода региона
    char     okato[12];            // Код ОКАТО
    char     regionnum[5];         // Числовой код региона
    char     codeplan[51];         // Наименование кода планировочной структуры
    char     plan[251];            // Планировочная структура
    char     building[51];         // Строение
    char     oktmo[12];            // Код ОКТМО
    char     fias[24];             // Код ФИАС
    char     fiasguid[37];         // GUID
    char     housetype[21];        // Основной тип дома
    char     housetype1[21];       // Дополнительный тип дома 1
    char     housetype2[21];       // Дополнительный тип дома 2
    char     gfiasunrz[34];        // Уникальный номер реестровой записи - УНРЗ
    char     GFiasApartType[11];   // Помещение_тип
    char     GFiasApartAdd[51];    // Помещение_доп.поле
    char     GFiasParkingType[11]; // Машиноместо_тип
    char     GFiasParkingNum[51];  // Машиноместо_номер
    char     GFiasDetailNum[6];    // Уровень детализации адреса - номер
    char     GFiasDetailName[101]; // Уровень детализации адреса - наименование
    char     GFiasSteadType[11];   // Земельный участок тип
    char     GFiasStead[251];      // Земельный участок номер
    db_int64 gfiasobjectid;        // Глобальный уникальный идентификатор объекта
    bdate    begindate;            // Дата начала действия
    bdate    enddate;              // Дата окончания действия
    db_int32 id;                   // Ид. записи
    db_int32 parentid;             // Ид. записи родительской записи
} ADRESS;

typedef union
{
    struct
    {
        db_baseint32 partyid;            
        db_baseint16 type;               
    } _partyid_type;

    struct
    {
        db_baseint16 type;               
        db_baseint32 partyid;            
    } _type_partyid;

    struct
    {
        db_baseint32 numsession;         
    } _numsession;

    struct
    {
        db_baseint32 id;                 
    } _id;

    struct
    {
        db_baseint32 parentid;           
    } _parentid;

    struct
    {
        db_baseint16 type;               
        bdate    begindate;          
    } _type_begindate;
} ADRESS_KEYS;

typedef enum
{
    ADRESS_PARTYID_TYPE,
    ADRESS_TYPE_PARTYID,
    ADRESS_NUMSESSION,
    ADRESS_ID,
    ADRESS_PARENTID,
    ADRESS_TYPE_BEGINDATE
} ADRESS_KEYNUM;

// Адрес субъекта
BTRVFILE *FileADRESS = NULL; 

// Адрес субъекта
int iOpenADRESS(int OpenMode)
{
    char FileName[_MAX_PATH];
    getFileName(FileName, "adress.dbt");
    return bfOpen(&FileADRESS, FileName, OpenMode, sizeof(ADRESS), sizeof(ADRESS_KEYS), 0, NULL, NULL, NULL, NULL);
}

void InitADRESS(ADRESS *RecBuf)
{
    memset(RecBuf, 0, sizeof(ADRESS));

    RecBuf->begindate = BDATE_ZERO;
    RecBuf->enddate = BDATE_ZERO;
}

static int KF_ADRESS_0(BTRVFILE *bf)
{
    ADRESS_KEYS *KB = (ADRESS_KEYS *)bf->KeyBuf;
    ADRESS_KEYS *TV = (ADRESS_KEYS *)bf->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)bf->KeyBotVal;

    return !(
        TV->_partyid_type.partyid <= KB->_partyid_type.partyid && 
        KB->_partyid_type.partyid <= BV->_partyid_type.partyid && 
        TV->_partyid_type.type <= KB->_partyid_type.type && 
        KB->_partyid_type.type <= BV->_partyid_type.type);
}

static int KF_ADRESS_1(BTRVFILE *bf)
{
    ADRESS_KEYS *KB = (ADRESS_KEYS *)bf->KeyBuf;
    ADRESS_KEYS *TV = (ADRESS_KEYS *)bf->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)bf->KeyBotVal;

    return !(
        TV->_type_partyid.type <= KB->_type_partyid.type && 
        KB->_type_partyid.type <= BV->_type_partyid.type && 
        TV->_type_partyid.partyid <= KB->_type_partyid.partyid && 
        KB->_type_partyid.partyid <= BV->_type_partyid.partyid);
}

static int KF_ADRESS_2(BTRVFILE *bf)
{
    ADRESS_KEYS *KB = (ADRESS_KEYS *)bf->KeyBuf;
    ADRESS_KEYS *TV = (ADRESS_KEYS *)bf->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)bf->KeyBotVal;

    return !(
        TV->_numsession.numsession <= KB->_numsession.numsession && 
        KB->_numsession.numsession <= BV->_numsession.numsession);
}

static int KF_ADRESS_3(BTRVFILE *bf)
{
    ADRESS_KEYS *KB = (ADRESS_KEYS *)bf->KeyBuf;
    ADRESS_KEYS *TV = (ADRESS_KEYS *)bf->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)bf->KeyBotVal;

    return !(
        TV->_id.id <= KB->_id.id && 
        KB->_id.id <= BV->_id.id);
}

static int KF_ADRESS_4(BTRVFILE *bf)
{
    ADRESS_KEYS *KB = (ADRESS_KEYS *)bf->KeyBuf;
    ADRESS_KEYS *TV = (ADRESS_KEYS *)bf->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)bf->KeyBotVal;

    return !(
        TV->_parentid.parentid <= KB->_parentid.parentid && 
        KB->_parentid.parentid <= BV->_parentid.parentid);
}

static int KF_ADRESS_5(BTRVFILE *bf)
{
    ADRESS_KEYS *KB = (ADRESS_KEYS *)bf->KeyBuf;
    ADRESS_KEYS *TV = (ADRESS_KEYS *)bf->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)bf->KeyBotVal;

    return !(
        TV->_type_begindate.type <= KB->_type_begindate.type && 
        KB->_type_begindate.type <= BV->_type_begindate.type && 
        TV->_type_begindate.begindate >= KB->_type_begindate.begindate && 
        KB->_type_begindate.begindate >= BV->_type_begindate.begindate);
}

void SKF_ADRESS_PARTYIDTYPE(int32 partyid, int16 type)
{
    ADRESS_KEYS *TV = (ADRESS_KEYS *)FileADRESS->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)FileADRESS->KeyBotVal;

    FileADRESS->KeyNum = ADRESS_PARTYID_TYPE;
    FileADRESS->KeyFltr = KF_ADRESS_0;

    if (partyid != 0)
	{
        TV->_partyid_type.partyid = partyid;
        BV->_partyid_type.partyid = partyid;
    }
    else
    {
        TV->_partyid_type.partyid = DB_INT32_MIN;
        BV->_partyid_type.partyid = DB_INT32_MAX;
    }

    if (type != 0)
	{
        TV->_partyid_type.type = type;
        BV->_partyid_type.type = type;
    }
    else
    {
        TV->_partyid_type.type = DB_INT16_MIN;
        BV->_partyid_type.type = DB_INT16_MAX;
    }
}

void SKF_ADRESS_TYPEPARTYID(int16 type, int32 partyid)
{
    ADRESS_KEYS *TV = (ADRESS_KEYS *)FileADRESS->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)FileADRESS->KeyBotVal;

    FileADRESS->KeyNum = ADRESS_TYPE_PARTYID;
    FileADRESS->KeyFltr = KF_ADRESS_1;

    if (type != 0)
	{
        TV->_type_partyid.type = type;
        BV->_type_partyid.type = type;
    }
    else
    {
        TV->_type_partyid.type = DB_INT16_MIN;
        BV->_type_partyid.type = DB_INT16_MAX;
    }

    if (partyid != 0)
	{
        TV->_type_partyid.partyid = partyid;
        BV->_type_partyid.partyid = partyid;
    }
    else
    {
        TV->_type_partyid.partyid = DB_INT32_MIN;
        BV->_type_partyid.partyid = DB_INT32_MAX;
    }
}

void SKF_ADRESS_NUMSESSION(int32 numsession)
{
    ADRESS_KEYS *TV = (ADRESS_KEYS *)FileADRESS->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)FileADRESS->KeyBotVal;

    FileADRESS->KeyNum = ADRESS_NUMSESSION;
    FileADRESS->KeyFltr = KF_ADRESS_2;

    if (numsession != 0)
	{
        TV->_numsession.numsession = numsession;
        BV->_numsession.numsession = numsession;
    }
    else
    {
        TV->_numsession.numsession = DB_INT32_MIN;
        BV->_numsession.numsession = DB_INT32_MAX;
    }
}

void SKF_ADRESS(int32 id)
{
    ADRESS_KEYS *TV = (ADRESS_KEYS *)FileADRESS->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)FileADRESS->KeyBotVal;

    FileADRESS->KeyNum = ADRESS_ID;
    FileADRESS->KeyFltr = KF_ADRESS_3;

    if (id != 0)
	{
        TV->_id.id = id;
        BV->_id.id = id;
    }
    else
    {
        TV->_id.id = DB_INT32_MIN;
        BV->_id.id = DB_INT32_MAX;
    }
}

void SKF_ADRESS_PARENTID(int32 parentid)
{
    ADRESS_KEYS *TV = (ADRESS_KEYS *)FileADRESS->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)FileADRESS->KeyBotVal;

    FileADRESS->KeyNum = ADRESS_PARENTID;
    FileADRESS->KeyFltr = KF_ADRESS_4;

    if (parentid != 0)
	{
        TV->_parentid.parentid = parentid;
        BV->_parentid.parentid = parentid;
    }
    else
    {
        TV->_parentid.parentid = DB_INT32_MIN;
        BV->_parentid.parentid = DB_INT32_MAX;
    }
}

void SKF_ADRESS_TYPEBEGINDATE(int16 type, bdate begindate)
{
    ADRESS_KEYS *TV = (ADRESS_KEYS *)FileADRESS->KeyTopVal;
    ADRESS_KEYS *BV = (ADRESS_KEYS *)FileADRESS->KeyBotVal;

    FileADRESS->KeyNum = ADRESS_TYPE_BEGINDATE;
    FileADRESS->KeyFltr = KF_ADRESS_5;

    if (type != 0)
	{
        TV->_type_begindate.type = type;
        BV->_type_begindate.type = type;
    }
    else
    {
        TV->_type_begindate.type = DB_INT16_MIN;
        BV->_type_begindate.type = DB_INT16_MAX;
    }

    if (begindate != BDATE_ZERO)
	{
        TV->_type_begindate.begindate = begindate;
        BV->_type_begindate.begindate = begindate;
    }
    else
    {
        TV->_type_begindate.begindate = BDATE_MAX;
        BV->_type_begindate.begindate = BDATE_MIN;
    }
}

int FindADRESS(int32 id, ADRESS *buff)
{
    ADRESS_KEYS KB;
    memset(&KB, 0, sizeof(ADRESS_KEYS));

    KB._id.id = id;

    return CB_FindRecord(&FileADRESS, iOpenADRESS, ADRESS_ID, &KB, buff, NULL);
}

// Адрес субъекта
extern BTRVFILE *FileADRESS;
extern int iOpenADRESS(int OpenMode);
extern void InitADRESS(ADRESS *RecBuf);
extern void SKF_ADRESS_PARTYIDTYPE(int32 partyid, int16 type);
extern void SKF_ADRESS_TYPEPARTYID(int16 type, int32 partyid);
extern void SKF_ADRESS_NUMSESSION(int32 numsession);
extern void SKF_ADRESS(int32 id);
extern void SKF_ADRESS_PARENTID(int32 parentid);
extern void SKF_ADRESS_TYPEBEGINDATE(int16 type, bdate begindate);
extern int FindADRESS(int32 id, ADRESS *buff);
