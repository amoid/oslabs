/***************************************************************************
 *   Copyright (C) 2010 by amoid                                           *
 *   amoid@163.com                                                         *
 *                                                                         *
 ***************************************************************************/
 
#include <cdb_type.h>
#include <cdb_err.h>
#include <cdb_def.h>
#include <cdb_dbg.h>
#include <cdb_u_file.h>

int32 c_group_count = 0;

cdb_user_group_t c_group[CDB_GROUP_MAX_COUNT];

int32 
cdb_group_init(void)
{
	c_group_count = 0;
	cdb_memset(c_group, 0x0, sizeof(cdb_user_group_t)*CDB_GROUP_MAX_COUNT);
	return CDB_E_NONE;
}

int32
cdb_line_is_comments(int8 *s)
{
	if(s[0] == CDB_CFG_COMMENT_TOKEN){
		CDB_CDBG_LOG_TRACE();
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32 
cdb_line_is_group_cfg_start(int8 *s)
{
	if(!cdb_strncmp(CDB_CFG_START_STR, s, cdb_strlen(CDB_CFG_START_STR))){
		CDB_CDBG_LOG_TRACE();
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32
cdb_line_is_group_cfg_end(int8 *s)
{
	if(!cdb_strncmp(CDB_CFG_END_STR, s, cdb_strlen(CDB_CFG_END_STR))){
		CDB_CDBG_LOG_TRACE();	
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32
cdb_line_is_one_group_head(int8 *s)
{
	if(!cdb_strncmp(CDB_CFG_GROUP_H_START, s, cdb_strlen(CDB_CFG_GROUP_H_START))){
		CDB_CDBG_LOG_TRACE();	
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32
cdb_line_is_one_g_mem_head(int8 *s)
{
	if(!cdb_strncmp(CDB_CFG_GROUP_M_H_START, s, cdb_strlen(CDB_CFG_GROUP_M_H_START))){
		CDB_CDBG_LOG_TRACE();	
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32
cdb_line_is_one_group_start(int8 *s, int32 group_no)
{
	int8 tmp_string[CDB_LINE_MAX_LEN] = {0X0};

	cdb_sprintf(tmp_string, CDB_CFG_GROUP_START_STR, group_no);

	CDB_CDBG_PRINTF(("s = %s\nt = %s\n",s,tmp_string));

	if(!cdb_strncmp(tmp_string, s, cdb_strlen(tmp_string))){
		CDB_CDBG_LOG_TRACE();
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32
cdb_line_is_one_group_end(int8 *s,int32 group_no)
{
	int8 tmp_string[CDB_LINE_MAX_LEN] = {0X0};

	cdb_sprintf(tmp_string, CDB_CFG_GROUP_END_STR, group_no);

	if(!cdb_strncmp(tmp_string, s, cdb_strlen(tmp_string))){
		CDB_CDBG_LOG_TRACE();
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32
cdb_line_is_one_mem_start(int8 *s, int32 mem_no)
{
	int8 tmp_string[CDB_LINE_MAX_LEN] = {0X0};

	cdb_sprintf(tmp_string, CDB_CFG_GROUP_MEM_START_STR, mem_no);

	CDB_CDBG_PRINTF(("s = %s\nt = %s\n",s,tmp_string));

	if(!cdb_strncmp(tmp_string, s, cdb_strlen(tmp_string))){
		CDB_CDBG_LOG_TRACE();
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32
cdb_line_is_one_mem_end(int8 *s)
{
	if(s[0] == CDB_CFG_COMMENT_TOKEN){
		CDB_CDBG_LOG_TRACE();
		return CDB_TRUE;
	}else{
		CDB_CDBG_LOG_TRACE();
		return CDB_FALSE;
	}
}

int32 
cdb_group_config_group_count_update(void)
{
	CDB_CDBG_LOG_TRACE();
	c_group_count++;
}

int32
cdb_group_config_group_mem_update(cdb_user_group_t *db, int32 mem_id, 
	cdb_user_info_t *info)
{
	if(NULL == db || NULL == info){
		CDB_CDBG_LOG_TRACE();	
		return CDB_E_NULL;
	}

	db->count++;
	cdb_strcpy(db->user[mem_id].fetion_no, info->fetion_no);
	cdb_strcpy(db->user[mem_id].id, info->id);
	cdb_strcpy(db->user[mem_id].phone_no, info->phone_no);
	cdb_strcpy(db->user[mem_id].name, info->name);

	CDB_CDBG_LOG_TRACE();
	return CDB_E_NONE;
}

int32
cdb_group_config_file_parser_org(FILE *fp, cdb_user_group_t *db)
{
	int32 rv = CDB_E_NONE;
	int8 tmp_string[2048] = {0x0};
	int32 group_id = 0;
	int32 last_group_id = 0;
	int32 mem_id = 0;
	int32 last_mem_id = 0;
	cdb_user_info_t user_info;
	int32 index;
	
	if(fp == NULL || db == NULL){
		CDB_CDBG_LOG_TRACE();
		return CDB_E_NULL;
	}

	cdb_memset(&user_info, 0x0, sizeof(cdb_user_info_t));
	
	while(!cdb_feof(fp)){
		cdb_fgets(tmp_string, sizeof(tmp_string), fp);
		if(cdb_line_is_comments(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			continue;
		}
		
		if(cdb_line_is_group_cfg_start(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			continue;
		}

		if(cdb_line_is_group_cfg_end(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			break;
		}
		
		for(group_id = last_group_id; group_id < CDB_GROUP_MAX_COUNT; group_id++){
			if(cdb_line_is_one_group_start(tmp_string, group_id)){
				last_group_id = group_id;
				last_mem_id = 0;
				cdb_group_config_group_count_update();
				CDB_CDBG_LOG_TRACE();
				break;
			}else if(cdb_line_is_one_group_end(tmp_string, group_id)){
				last_group_id = group_id+1;
				last_mem_id = 0;
				CDB_CDBG_LOG_TRACE();
				break;
			}else {
				continue;
			}		
		}

		if(cdb_line_is_one_group_start(tmp_string, group_id) 
			|| cdb_line_is_one_group_end(tmp_string, group_id)){
			CDB_CDBG_LOG_TRACE();
			continue;
		}

		for (mem_id = last_mem_id; mem_id < CDB_USER_MAX_COUNT;mem_id++) {
			CDB_CDBG_LOG_TRACE();
			if(cdb_line_is_one_mem_start(tmp_string, mem_id)){
				CDB_CDBG_LOG_TRACE();
				for(index = 0; index < cdb_strlen(tmp_string); index++){
					CDB_CDBG_LOG_TRACE();
					if(!cdb_strncmp(&tmp_string[index], CDB_CFG_GROUP_MEM_PHNO, 
						cdb_strlen(CDB_CFG_GROUP_MEM_PHNO))){
						CDB_CDBG_LOG_TRACE();
						cdb_strncpy(user_info.phone_no,
							&tmp_string[index+cdb_strlen(CDB_CFG_GROUP_MEM_PHNO)],
							CDB_USER_PHONE_NO_LEN);
						for(index = 0; index < CDB_USER_PHONE_NO_LEN; index++){
							CDB_CDBG_LOG_TRACE();
							if(user_info.phone_no[index] ==  ' '){
								CDB_CDBG_LOG_TRACE();
								user_info.phone_no[index] = 0x0;
								break;
							}
						}
						CDB_CDBG_LOG_TRACE();
						break;
					}
				}	
				cdb_group_config_group_mem_update(&db[last_group_id],mem_id,&user_info);
				last_mem_id = mem_id + 1;
				CDB_CDBG_LOG_TRACE();
				break;
			}
		}
	}

	return rv;
}

int32
cdb_group_config_file_parser(FILE *fp, cdb_user_group_t *db)
{
	int32 rv = CDB_E_NONE;
	int8 tmp_string[2048] = {0x0};
	int32 group_id = 0;
	int32 last_group_id = 0;
	int32 mem_id = 0;
	int32 last_mem_id = 0;
	cdb_user_info_t user_info;
	int32 index;
	
	if(fp == NULL || db == NULL){
		CDB_CDBG_LOG_TRACE();
		return CDB_E_NULL;
	}

	cdb_memset(&user_info, 0x0, sizeof(cdb_user_info_t));
	
	while(!cdb_feof(fp)){
		cdb_fgets(tmp_string, sizeof(tmp_string), fp);
		CDB_CDBG_PRINTF(("aaa = %s\n",tmp_string));
		if(cdb_line_is_comments(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			continue;
		}else if(cdb_line_is_group_cfg_start(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			continue;
		}else if(cdb_line_is_group_cfg_end(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			break;
		}else if (cdb_line_is_one_group_head(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			for(group_id = last_group_id; group_id < CDB_GROUP_MAX_COUNT; group_id++){
				if(cdb_line_is_one_group_start(tmp_string, group_id)){
					last_group_id = group_id;
					last_mem_id = 0;
					cdb_group_config_group_count_update();
					CDB_CDBG_LOG_TRACE();
					break;
				}else if(cdb_line_is_one_group_end(tmp_string, group_id)){
					last_group_id = group_id+1;
					last_mem_id = 0;
					CDB_CDBG_LOG_TRACE();
					break;
				}else {
					continue;
				}		
			}
		}else if(cdb_line_is_one_g_mem_head(tmp_string)){
			CDB_CDBG_LOG_TRACE();
			for (mem_id = 0; mem_id < CDB_USER_MAX_COUNT;mem_id++) {
				CDB_CDBG_LOG_TRACE();
				if(cdb_line_is_one_mem_start(tmp_string, mem_id)){
					CDB_CDBG_LOG_TRACE();
					
					for(index = 0; index < cdb_strlen(tmp_string); index++){
						CDB_CDBG_LOG_TRACE();
						if(!cdb_strncmp(&tmp_string[index], CDB_CFG_GROUP_MEM_PHNO, 
							cdb_strlen(CDB_CFG_GROUP_MEM_PHNO))){
							CDB_CDBG_LOG_TRACE();
							cdb_strncpy(user_info.phone_no,
								&tmp_string[index+cdb_strlen(CDB_CFG_GROUP_MEM_PHNO)],
								CDB_USER_PHONE_NO_LEN);
							for(index = 0; index < CDB_USER_PHONE_NO_LEN; index++){
								CDB_CDBG_LOG_TRACE();
								if(user_info.phone_no[index] ==  ' '){
									CDB_CDBG_LOG_TRACE();
									user_info.phone_no[index] = 0x0;
									break;
								}
							}
							CDB_CDBG_LOG_TRACE();
							break;
						}else {
							CDB_CDBG_PRINTF(("s= %s\n",&tmp_string[index]));
							CDB_CDBG_PRINTF(("no = %s\n",CDB_CFG_GROUP_MEM_PHNO));
							CDB_CDBG_LOG_TRACE();
						}
					}
					CDB_CDBG_LOG_TRACE();
					CDB_CDBG_PRINTF(("user.no = %s %d %d\n",user_info.phone_no,
						last_group_id,last_mem_id));
					cdb_group_config_group_mem_update(&db[last_group_id],mem_id,
						&user_info);
					last_mem_id = mem_id + 1;
					
					CDB_CDBG_LOG_TRACE();
					break;
				}
			}
			continue;
		}
	}	
}

int32 
cdb_user_group_member_load(const int8 *file_name, cdb_user_group_t *db)
{
	FILE *fp;

	fp = cdb_fopen(file_name, "r+");
	if (NULL == fp){
		CDB_CDBG_LOG_TRACE();
		return CDB_E_FILE;
	}
	CDB_CDBG_LOG_TRACE();
	cdb_group_config_file_parser(fp, db);
	cdb_fclose(fp);
	CDB_CDBG_LOG_TRACE();
	return CDB_E_NONE;
}

int32
cdb_user_group_doc_load(int32 group_no, int8 *doc_name, 
	int8 *record_name, int8 *msg, int32 *msg_len)
{

}

