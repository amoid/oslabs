/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *
 ***************************************************************************/
#ifndef _CDB_DBG_H_
#define _CDB_DBG_H_

#include <cdb_cfg.h>
#include <cdb_def.h>

#ifdef CDB_DBG_ENABLE
#define CDB_CDBG_LOG_TRACE() cdb_printf("%s,%s,%d\n",__FILE__,__FUNCTION__,\
	__LINE__);

#define CDB_CDBG_PRINTF(x)	cdb_printf x
#else
#define CDB_CDBG_LOG_TRACE() 
#define CDB_CDBG_PRINTF(x)	
#endif

#endif /*_CDB_DBG_H_*/
 
