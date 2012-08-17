/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *          
 ***************************************************************************/
 
#ifndef _CDB_ERR_H_
#define _CDB_ERR_H_

typedef enum {
	CDB_E_NONE = 0,
	CDB_E_FILE = 1,
	CDB_E_NULL = 2,
	CDB_E_PARM = 3,
	CDB_E_ERROR = -1
}cdb_err_code_t;

typedef enum {
	CDB_FALSE = 0,
	CDB_TRUE	= 1
}cdb_bool_t;

#endif /*_CDB_ERR_H_*/
 