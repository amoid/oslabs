/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *
 ***************************************************************************/
 
#include <cdb_type.h>
#include <cdb_err.h>
#include <cdb_def.h>
#include <cdb_dbg.h>
#include <cdb_c_file.h>
#include <cdb_u_file.h>
#include <cdb_d_file.h>

FILE *
cdb_d_file_open(int8 *file_name)
{
	FILE *fp;
	
	if(NULL == file_name){
		return NULL;
	}

	fp = cdb_fopen(file_name, "rb");
	if(NULL != fp){
		return fp;
	}else{
		return NULL;
	}
}

int32
cdb_d_file_block_read(FILE *fp, int32 block_size, int32 offset, int8 *buff, 
	int32 *data_len)
{
	int rv = CDB_E_NONE;
	uint32 left_bytes = 0;
	uint32 read_len = 0;
	int8 *left_buff;
	int32 i; 
	
	if(NULL == fp || buff == NULL || data_len == NULL){
		CDB_CDBG_LOG_TRACE();
		return CDB_E_NULL;
	}

	/*first make size eauql block_size*/
	left_bytes = block_size;
	left_buff = buff;

	CDB_CDBG_PRINTF(("block_size = %d offset = %d\n",block_size, offset));
	cdb_fseek(fp, offset, SEEK_SET);

	while(!cdb_feof(fp) && left_bytes > 0){
		cdb_fgets(left_buff, left_bytes, fp);
		CDB_CDBG_LOG_TRACE();
		CDB_CDBG_PRINTF(("%s\n",left_buff));
		read_len = cdb_strlen(left_buff)+1;
		CDB_CDBG_PRINTF(("readlen = %d\n",read_len));
		CDB_CDBG_PRINTF(("left_bytes = %d\n",left_bytes));
		if(read_len >= left_bytes){
			left_bytes = left_bytes - read_len;
			rv = CDB_E_NONE;
			CDB_CDBG_LOG_TRACE();
			break;
		}else{
			left_bytes = left_bytes - read_len;
			left_buff = left_buff + read_len;
		}
	}

	rv = CDB_E_NONE;

	*data_len = block_size - left_bytes;
	CDB_CDBG_LOG_TRACE();
	return rv;
}

int32
cdb_d_file_close(FILE *fp)
{
	cdb_fclose(fp);
}

int32 
cdb_d_file_msg_get(int8 *file_name, int32 block_size, int32 offset, int8 *buff, 
	int32 *data_len)
{
	int32 rv;
	FILE *fp;
	
	if(NULL == file_name || NULL == buff || data_len == NULL){
		return CDB_E_NULL;
	}	

	fp = cdb_d_file_open(file_name);

	if(NULL == fp){
		return CDB_E_NULL;
	}

	rv = cdb_d_file_block_read(fp, block_size, offset, buff, data_len);

	cdb_d_file_close(fp);

	return rv;
}
