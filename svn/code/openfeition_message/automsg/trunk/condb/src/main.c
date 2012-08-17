/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *
 ***************************************************************************/
 
#include "../include/cdb_type.h"
#include "../include/cdb_err.h"
#include "../include/cdb_def.h"
#include "../include/cdb_dbg.h"
#include "../include/cdb_u_file.h"
#include "../include/cdb_d_file.h"
#include "../include/cdb_c_file.h"

extern cdb_doc_state_t doc_state;
extern int32 c_group_count;
extern cdb_user_group_t c_group[CDB_GROUP_MAX_COUNT];

int32
dbg_test(void){return CDB_E_NONE;}

int32 
main(int argc, int8 *argv[])
{
    FILE *fp,*dfp;
    int32 rv = CDB_E_NONE;
	int msg_len = 0;
   
    cdb_doc_state_init();
	cdb_group_init();
    
    fp = cdb_fopen("../db/db.source/fileinfo.dat","r+");
    
    rv = cdb_doc_state_file_parser(fp, &doc_state);

	if(CDB_E_NONE != rv){
		return rv;
	}

	cdb_fclose(fp);

	rv = cdb_user_group_member_load("/mnt/lindows/work/trunk/condb/db/dbcfg/groupinfo.txt",
		c_group);

	if(CDB_E_NONE != rv){
		return rv;
	}

	dfp = cdb_d_file_open(doc_state.file_name);

	if(dfp == NULL){
		CDB_CDBG_LOG_TRACE();
		return CDB_E_FILE;
	}

	CDB_CDBG_LOG_TRACE();
	rv = cdb_d_file_block_read(dfp, doc_state.block_size, doc_state.last_offset,
		c_group[0].db_doc_message, &msg_len);
	
	CDB_CDBG_PRINTF(("msg:%s\n",c_group[0].db_doc_message));
	CDB_CDBG_PRINTF(("msg_len= %d\n",msg_len));

	CDB_CDBG_LOG_TRACE();
	if(rv != CDB_E_NONE){
		CDB_CDBG_LOG_TRACE();
		return rv;
	}
	CDB_CDBG_LOG_TRACE();
	if(msg_len == doc_state.block_size){
		doc_state.last_offset = doc_state.last_offset + msg_len;
		cdb_strcpy(doc_state.finished_state,"finished=yes");
		CDB_CDBG_LOG_TRACE();
	}else{
		doc_state.last_offset = 0;
		cdb_strcpy(doc_state.finished_state,"finished=no");
		CDB_CDBG_LOG_TRACE();
	}

	CDB_CDBG_LOG_TRACE();
	rv = cdb_doc_c_file_update("../db/db.source/fileinfo.dat", &doc_state);
	CDB_CDBG_LOG_TRACE();
	dbg_test();
	
    while(1){;}

    return 0;
}

