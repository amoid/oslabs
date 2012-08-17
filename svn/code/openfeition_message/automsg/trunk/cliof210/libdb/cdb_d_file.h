/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *          
 ***************************************************************************/

#ifndef _CDB_D_FILE_H_
#define _CDB_D_FILE_H_

FILE *
cdb_d_file_open(int8 *file_name);

int32
cdb_d_file_block_read(FILE *fp, int32 block_size, int32 offset, int8 *buff, 
	int32 *data_len);

int32
cdb_d_file_close(FILE *fp);

int32 
cdb_d_file_msg_get(int8 *file_name, int32 block_size, int32 offset, int8 *buff, 
	int32 *data_len);

#endif 
