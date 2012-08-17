/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *          
 ***************************************************************************/

#ifndef _CDB_U_FILE_H_
#define _CDB_U_FILE_H_

#define CDB_GROUP_MAX_COUNT		10
#define CDB_USER_MAX_COUNT		4096
#define CDB_DOC_MSG_LENGTH		350
#define CDB_USER_PHONE_NO_LEN	16
#define CDB_USER_NAME_LEN		8
#define CDB_USER_FETION_NO_LEN	16
#define CDB_USER_ID_LEN			16

#define CDB_LINE_MAX_LEN			80

#define CDB_CFG_COMMENT_TOKEN		'#'
#define CDB_CFG_START_STR 			"@group start"
#define CDB_CFG_END_STR				"@group end"
#define CDB_CFG_GROUP_START_STR  	"group.%d.start"
#define CDB_CFG_GROUP_END_STR		"group.%d.end"
#define CDB_CFG_GROUP_MEM_START_STR 	"mem.%d"
#define CDB_CFG_GROUP_MEM_END_STR    '#'
#define CDB_CFG_GROUP_MEM_PHNO       "no."
#define CDB_CFG_SPACE_TOKEN          ' '
#define CDB_CFG_GROUP_H_START		"group."
#define CDB_CFG_GROUP_M_H_START		"mem."

typedef struct {
	int8 phone_no[CDB_USER_PHONE_NO_LEN];
	int8 fetion_no[CDB_USER_FETION_NO_LEN];
	int8 id[CDB_USER_ID_LEN];
	int8 name[CDB_USER_NAME_LEN];
}cdb_user_info_t;

typedef struct {
	int32 count;
	cdb_user_info_t user[CDB_USER_MAX_COUNT];
	int8 db_doc_message[CDB_DOC_MSG_LENGTH+2];
}cdb_user_group_t; 

int32 
cdb_group_init(void);

int32
cdb_line_is_comments(int8 *s);

int32 
cdb_line_is_group_cfg_start(int8 *s);

int32
cdb_line_is_group_cfg_end(int8 *s);

int32
cdb_line_is_one_group_head(int8 *s);

int32
cdb_line_is_one_g_mem_head(int8 *s);

int32
cdb_line_is_one_group_start(int8 *s, int32 group_no);

int32
cdb_line_is_one_group_end(int8 *s,int32 group_no);

int32
cdb_line_is_one_mem_start(int8 *s, int32 mem_no);

int32
cdb_line_is_one_mem_end(int8 *s);

int32 
cdb_group_config_group_count_update(void);

int32
cdb_group_config_group_mem_update(cdb_user_group_t *db, int32 mem_id, 
	cdb_user_info_t *info);

int32
cdb_group_config_file_parser_org(FILE *fp, cdb_user_group_t *db);

int32
cdb_group_config_file_parser(FILE *fp, cdb_user_group_t *db);

int32 
cdb_user_group_member_load(const int8 *file_name, cdb_user_group_t *db);

int32
cdb_user_group_doc_load(int32 group_no, int8 *doc_name, 
	int8 *record_name, int8 *msg, int32 *msg_len);

#endif /*_CDB_U_FILE_H_*/

