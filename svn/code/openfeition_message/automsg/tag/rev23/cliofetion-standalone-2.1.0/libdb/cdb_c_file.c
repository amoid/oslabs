#include <stdio.h>
#include <string.h>

#include <cdb_type.h>
#include <cdb_def.h>
#include <cdb_err.h>
#include <cdb_c_file.h>

cdb_doc_state_t doc_state;

int32
cdb_doc_state_init(void)
{
    cdb_memset(&doc_state, 0x0, sizeof(cdb_doc_state_t));
    return CDB_E_NONE;
}

int32 
cdb_doc_line_is_comment(int8 *s)
{
    if(s[0] == CDB_DOC_STATE_COMMENT_TOKEN){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32 
cdb_doc_line_is_file_start(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_START, cdb_strlen(CDB_DOC_STATE_START))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32
cdb_doc_line_is_file_end(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_END, cdb_strlen(CDB_DOC_STATE_END))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32
cdb_doc_line_is_file_name(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_FILE_NAME_START, 
		cdb_strlen(CDB_DOC_STATE_FILE_NAME_START))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}


int32 
cdb_doc_line_is_file_size(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_FILE_SIZE_START, 
		cdb_strlen(CDB_DOC_STATE_FILE_SIZE_START))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32
cdb_doc_line_is_file_offset(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_FILE_OFFSET_START, 
		cdb_strlen(CDB_DOC_STATE_FILE_OFFSET_START))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32
cdb_doc_line_is_file_block(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_FILE_BLOCK_SIZE_START, 
		cdb_strlen(CDB_DOC_STATE_FILE_BLOCK_SIZE_START))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32
cdb_doc_line_is_finished_state(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_FINISHED_STATE_START, 
		cdb_strlen(CDB_DOC_STATE_FINISHED_STATE_START))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32
cdb_doc_line_is_finished_yes(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_FINISHED_SATE_YES, 
		cdb_strlen(CDB_DOC_STATE_FINISHED_SATE_YES))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}

int32
cdb_doc_line_is_finished_no(int8 *s)
{
    if(!cdb_strncmp(s, CDB_DOC_STATE_FINISHED_SATE_NO, 
		cdb_strlen(CDB_DOC_STATE_FINISHED_SATE_NO))){
        return CDB_TRUE;
    }else{
        return CDB_FALSE;
    }
}
int32
cdb_doc_state_file_parser(FILE *fp, cdb_doc_state_t *state)
{
    int8 tmp_string[CDB_DOC_MAX_STR_LEN] = {0x0};
    int rv = CDB_E_NONE;

    if(NULL == fp || state == NULL){
        return CDB_E_ERROR;
    }

    while(!cdb_feof(fp)){
        cdb_fgets(tmp_string, sizeof(tmp_string), fp);

        if(cdb_doc_line_is_comment(tmp_string)){
            continue;
        }else if(cdb_doc_line_is_file_start(tmp_string)){
            continue;
        }else if(cdb_doc_line_is_file_end(tmp_string)){
            rv = CDB_E_NONE;
            break;
        }else if(cdb_doc_line_is_file_name(tmp_string)){
            cdb_strcpy(state->file_name,
				1 + tmp_string+cdb_strlen(CDB_DOC_STATE_FILE_NAME_START));
            {
                int32 i = 0;
                while(i < CDB_DOC_STATE_FILE_NAME_SIZE){
                    if(state->file_name[i] == '"'){
                        state->file_name[i] = 0x0;
                        break;
                    }else{
                        i++;
                    }
                }
            }
            continue;
        }else if(cdb_doc_line_is_file_size(tmp_string)){
            state->file_size = 
				cdb_atol(tmp_string+cdb_strlen(CDB_DOC_STATE_FILE_SIZE_START));
            continue;
        }else if(cdb_doc_line_is_file_offset(tmp_string)){
            state->last_offset = 
				cdb_atol(tmp_string+cdb_strlen(CDB_DOC_STATE_FILE_OFFSET_START));
            continue;
        }else if(cdb_doc_line_is_file_block(tmp_string)){
            state->block_size = 
				cdb_atol(tmp_string+cdb_strlen(CDB_DOC_STATE_FILE_BLOCK_SIZE_START));
            continue;
        }else if(cdb_doc_line_is_finished_state(tmp_string)){
            if(cdb_doc_line_is_finished_no(tmp_string)){
				cdb_strcpy(state->finished_state,tmp_string);
				continue;
			}else{
				cdb_strcpy(state->finished_state,tmp_string);
				break;
			}
        }else{
            rv = CDB_E_ERROR;
            cdb_printf("line is invalid:%s\n",tmp_string);
            break;
        }
    }

    return rv;
}

int32
cdb_doc_c_file_update(int8 *file_name, cdb_doc_state_t *state)
{
	FILE *fp;
	int8 buff[2048];
	int32 curr_p;
	int8 tmp_buff[256] = {0x0};
	
	if(NULL == file_name || state == NULL){
		return CDB_E_NULL;
	}

	fp  = cdb_fopen(file_name, "r+");

	if(NULL == fp){
		return CDB_E_FILE;
	}

	cdb_fputs("#this file is the file state\n",fp);
	cdb_fputs("@start\n",fp);
	cdb_fputs("finished=no\n",fp);
	cdb_sprintf(tmp_buff,"file.name=\"%s\"\n",state->file_name);
	cdb_fputs(tmp_buff,fp);
	cdb_sprintf(tmp_buff,"file.size=%d\n",state->file_size);
	cdb_fputs(tmp_buff,fp);
	cdb_sprintf(tmp_buff,"file.offset=%d\n",state->last_offset);
	cdb_fputs(tmp_buff,fp);
	cdb_sprintf(tmp_buff,"file.block=%d\n",state->block_size);
	cdb_fputs(tmp_buff,fp);
	cdb_fputs("@end\n",fp);
	#if 0
	curr_p = cdb_ftell(fp);
	while(!cdb_feof(fp)){
		cdb_fgets(buff, sizeof(buff), fp);
		if(cdb_doc_line_is_file_offset(buff)){
			cdb_fseek(fp, curr_p, SEEK_SET);
			cdb_strcpy(buff,CDB_DOC_STATE_FILE_OFFSET_START);
			cdb_sprintf(tmp_buff,"%d #\n",state->last_offset);
			cdb_strcat(buff,tmp_buff);
			cdb_fputs(buff,fp);
		}else if(cdb_doc_line_is_finished_state(buff)){
			if(cdb_strncmp(buff,state->finished_state,
					cdb_strlen(state->finished_state - 1))){
				cdb_fseek(fp, curr_p, SEEK_SET);
				cdb_fputs(buff,fp);
			}
		}else if(cdb_doc_line_is_file_end(buff)){
			break;
		}
		curr_p = cdb_ftell(fp);
	}
	#endif
	cdb_fclose(fp);

	return CDB_E_NONE;
}

int32
cdb_doc_record_load(int8 *file_name, cdb_doc_state_t *state)
{
	int32 rv;
	FILE *fp;
	
	if(NULL == file_name || NULL == state){
		return CDB_E_NULL;
	}

	fp = cdb_fopen(file_name, "r+");

	if(NULL == fp){
		return CDB_E_FILE;
	}

	rv = cdb_doc_state_file_parser(fp, state);

	cdb_fclose(fp);

	return rv;
}
