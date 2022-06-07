#ifndef _DPRINT_H_
#define _DPRINT_H_

#define DPRINTF(msg) /*printf(__FILE__"(%d)" __FUNCTION__"(): ", __LINE__), printf msg*/
//#define DFPRINTF(msg) do{FILE *dbgfp = fopen("debug.log","a+");fprintf(dbgfp,__FILE__"(%d)" __FUNCTION__"(): ", __LINE__), fprintf msg,fclose(dbgfp);}while(0)

#endif//_DPRINT_H_
