// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef RSLIBDYNAMICFUNCS_H
#define RSLIBDYNAMICFUNCS_H

typedef void *(*LibRslTArrayCreate)(int,int);
typedef void *(*LibRslTArrayPut)(void*,long, void*);
typedef void *(*LibRslIsTArray)(void *obj);
typedef long (*LibRslTArraySize)(void *obj);
typedef void *(*LibRslTArrayGet)(void *obj, long id);
typedef bool (*LibSetParm)(int,unsigned char,void*);

extern LibRslTArrayCreate _LibRslTArrayCreate;
extern LibRslTArrayPut _LibRslTArrayPut;
extern LibRslIsTArray _LibRslIsTArray;
extern LibRslTArraySize _LibRslTArraySize;
extern LibRslTArrayGet _LibRslTArrayGet;
extern LibSetParm _LibSetParm;

extern void LoadFunctions();

#endif // RSLIBDYNAMICFUNCS_H
