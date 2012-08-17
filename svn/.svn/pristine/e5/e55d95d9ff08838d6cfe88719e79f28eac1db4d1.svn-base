/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *          
 ***************************************************************************/

#ifndef _CDB_C_FILE_H_
#define _CDB_C_FILE_H_

#define CDB_DOC_FILE_NAME                           "amoid.txt"
#define CDB_DOC_MAX_STR_LEN                         2048

#define CDB_DOC_STATE_FILE_NAME_SIZE                256
#define CDB_DOC_STATE_COMMENT_TOKEN                 '#'
#define CDB_DOC_STATE_START                         "@start"
#define CDB_DOC_STATE_END                           "@end"
#define CDB_DOC_STATE_FILE_NAME_START               "file.name="
#define CDB_DOC_STATE_FILE_SIZE_START               "file.size="
#define CDB_DOC_STATE_FILE_OFFSET_START             "file.offset="
#define CDB_DOC_STATE_FILE_BLOCK_SIZE_START         "file.block="
#define CDB_DOC_STATE_FINISHED_STATE_START			"finished="
#define CDB_DOC_STATE_FINISHED_SATE_YES				"finished=yes"
#define CDB_DOC_STATE_FINISHED_SATE_NO				"finished=no"

typedef struct {
    int8 file_name[CDB_DOC_STATE_FILE_NAME_SIZE];
    int32 file_size;
    int32 last_offset;
    int32 block_size;
	int8  finished_state[CDB_DOC_STATE_FILE_NAME_SIZE];
} cdb_doc_state_t;

int32
cdb_doc_state_init(void);

int32 
cdb_doc_line_is_comment(int8 *s);

int32 
cdb_doc_line_is_file_start(int8 *s);

int32
cdb_doc_line_is_file_end(int8 *s);

int32
cdb_doc_line_is_file_name(int8 *s);

int32 
cdb_doc_line_is_file_size(int8 *s);

int32
cdb_doc_line_is_file_offset(int8 *s);

int32
cdb_doc_line_is_file_block(int8 *s);

int32
cdb_doc_state_file_parser(FILE *fp, cdb_doc_state_t *state);

int32
cdb_doc_c_file_update(int8 *file_name, cdb_doc_state_t *state);

#endif

