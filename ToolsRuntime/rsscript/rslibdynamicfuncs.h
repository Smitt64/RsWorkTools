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
