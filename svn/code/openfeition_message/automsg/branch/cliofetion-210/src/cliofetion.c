/***************************************************************************
 *   Copyright (C) 2010 by lwp                                             *
 *   levin108@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#include <openfetion.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cdb_type.h>
#include <cdb_err.h>
#include <cdb_def.h>
#include <cdb_dbg.h>
#include <cdb_u_file.h>
#include <cdb_c_file.h>
#include <cdb_d_file.h>
 
#define BUFLEN 1024

#define CLI_FETION_DBG	0

extern int32 c_group_count;
extern cdb_user_group_t c_group[CDB_GROUP_MAX_COUNT];
extern cdb_doc_state_t doc_state;
 
int   password_inputed = 0;
int   mobileno_inputed = 0;
int   user_cfg_file_in = 0;
int   record_d_file_in = 0;
User *user;
pthread_t th;
 
static void usage(char *argv[]);
 
int fx_login(const char *mobileno, const char *password)
{
	Config           *config;
	FetionConnection *tcp;
	FetionSip        *sip;
	char             *res;
	char             *nonce;
	char             *key;
	char             *aeskey;
	char             *response;
	int               local_group_count;
	int               local_buddy_count;
	int               group_count;
	int               buddy_count;
	int               ret;
 
	/* construct a user object */
 	user = fetion_user_new(mobileno, password);
	/* construct a config object */
	config = fetion_config_new();
	/* attach config to user */
	fetion_user_set_config(user, config);
 
	/* start ssi authencation,result string needs to be sal_freed after use */
	res = ssi_auth_action(user);
	/* parse the ssi authencation result,if success,user's sipuri and userid
	 * are stored in user object,orelse user->loginStatus was marked failed */
	parse_ssi_auth_response(res, user);
	sal_free(res);
 
	/* whether needs to input a confirm code,or login failed
	 * for other reason like password error */
	if(USER_AUTH_NEED_CONFIRM(user) || USER_AUTH_ERROR(user)) {
		debug_error("authencation failed");
		return 1;
	}
 
	/* initialize configuration for current user */
	if(fetion_user_init_config(user) == -1) {
		debug_error("initialize configuration");
		return 1;
	}
 
	if(fetion_config_download_configuration(user) == -1) {
		debug_error("download configuration");
		return 1;
	}
 
	/* set user's login state to be hidden */
	fetion_user_set_st(user, P_HIDDEN);
 
	/* load user information and contact list information from local host */
	fetion_user_load(user);
	fetion_contact_load(user, &local_group_count, &local_buddy_count);
 
	/* construct a tcp object and connect to the sipc proxy server */
	tcp = tcp_connection_new();
	if((ret = tcp_connection_connect(tcp, config->sipcProxyIP, config->sipcProxyPort)) == -1) {
		debug_error("connect sipc server %s:%d\n", config->sipcProxyIP, config->sipcProxyPort);
		return 1;
	}
 
	/* construct a sip object with the tcp object and attach it to user object */
	sip = fetion_sip_new(tcp, user->sId);
	fetion_user_set_sip(user, sip);
 
	/* register to sipc server */
	if(!(res = sipc_reg_action(user))) {
		debug_error("register to sipc server");
		return 1;
	}
 
	parse_sipc_reg_response(res, &nonce, &key);
	sal_free(res);
	aeskey = generate_aes_key();
 
	response = generate_response(nonce, user->userId, user->password, key, aeskey);
	sal_free(nonce);
	sal_free(key);
	sal_free(aeskey);
 
	/* sipc authencation,you can printf res to see what you received */
	if(!(res = sipc_aut_action(user, response))) {
		debug_error("sipc authencation");
		return 1;
	}
 
	if(parse_sipc_auth_response(res, user, &group_count, &buddy_count) == -1) {
		debug_error("authencation failed");
		return 1;
	}
 
	sal_free(res);
	sal_free(response);
 
	if(USER_AUTH_ERROR(user) || USER_AUTH_NEED_CONFIRM(user)) {
		debug_error("login failed");
		return 1;
	}
 
	/* save the user information and contact list information back to the local database */
	fetion_user_save(user);
	fetion_contact_save(user);
 
	/* these... fuck the fetion protocol */
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	char buf[1024];
	if(setsockopt(user->sip->tcp->socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
		debug_error("settimeout");
		return 1;
	}
	tcp_connection_recv(user->sip->tcp, buf, sizeof(buf));
 
	return 0;
}
 
int send_message(const char *mobileno, const char *receiveno, const char *message)
{
	Conversation *conv;
	Contact      *contact;
	Contact      *contact_cur;
	Contact      *target_contact = NULL;
	int           daycount;
	int           monthcount;
 
	/* send this message to yourself */
	if(*receiveno == '\0' || strcmp(receiveno, mobileno) == 0) {
		/* construct a conversation object with the sipuri to set NULL
		 * to send a message to yourself  */
		conv = fetion_conversation_new(user, NULL, NULL);
		if(fetion_conversation_send_sms_to_myself_with_reply(conv, message) == -1) {
			debug_error("send message \"%s\" to %s", message, user->mobileno);
			return 1;
		}
	}else{
		/* get the contact detail information by mobile number,
		 * note that the result doesn't contain sipuri */
		contact = fetion_contact_get_contact_info_by_no(user, receiveno, MOBILE_NO);
		if(!contact) {
			debug_error("get contact information of %s", receiveno);
			return 1;
		}
 
		/* find the sipuri of the target user */
		foreach_contactlist(user->contactList, contact_cur) {
			if(strcmp(contact_cur->userId, contact->userId) == 0) {
				target_contact = contact_cur;
				break;
			}
		}
 
		if(!target_contact) {
			debug_error("sorry,maybe %s isn't in your contact list");
			return 1;
		}
 
		/* do what the function name says */
		conv = fetion_conversation_new(user, target_contact->sipuri, NULL);
		if(fetion_conversation_send_sms_to_phone_with_reply(conv, message, &daycount, &monthcount) == -1) {
			debug_error("send sms to %s", receiveno);
			return 1;
		}else{
			debug_info("successfully send sms to %s\nyou have sent %d messages today, %d messages this monthcount",
					receiveno, daycount, monthcount);
			return 0;
		}
	}
	return 0;
}
 
int main(int argc, char *argv[])
{
	int ch;
	char mobileno[BUFLEN];
	char password[BUFLEN];
	char userfile[BUFLEN];
	char recordfile[BUFLEN];
	int data_len;
	int try_count = 0;
	int i;
	int j;
 
	memset(mobileno, 0, sizeof(mobileno));
	memset(password, 0, sizeof(password));
	memset(userfile, 0, sizeof(userfile));
	memset(recordfile, 0, sizeof(recordfile));
 
	while((ch = getopt(argc, argv, "f:p:u:r:")) != -1) {
		switch(ch) {
			case 'f':
				mobileno_inputed = 1;
				strncpy(mobileno, optarg, sizeof(mobileno) - 1);	
				break;
			case 'p':
				password_inputed = 1;
				strncpy(password, optarg, sizeof(password) - 1);
				break;
			case 'u':
				user_cfg_file_in = 1;
				strncpy(userfile, optarg, sizeof(userfile) - 1);
				break;
			case 'r':
				record_d_file_in = 1;
				strncpy(recordfile, optarg, sizeof(recordfile) - 1);
				break;
			default:
				break;
		}
	}
 
	if(!mobileno_inputed 
		|| !password_inputed 
		|| !user_cfg_file_in 
		|| !record_d_file_in) {
		usage(argv);
		return 1;
	}
 
	if(fx_login(mobileno, password)){
		debug_error("login phone %s pwd %s failed.\n",mobileno, password);
		return 1;
	}

	try_count = 0;
	while(1){
		try_count++;

		if(try_count > 10)break;
		sleep(1);
	}
	
	/*init the user group now*/
	if(cdb_group_init()){
		debug_error("cdb_group_init failed.\n");
		return 1;
	}

	if(cdb_user_group_member_load(userfile, c_group)){
		debug_error("cdb_user_group_member_load failed %s.\n",userfile);
		return 1;
	}

	if(cdb_doc_state_init()){
		debug_error("cdb_doc_state_init failed.\n");
		return 1;
	}

	if(cdb_doc_record_load(recordfile, &doc_state)){
		debug_error("cdb_doc_record_load failed.\n");
		return 1;
	}

	if (cdb_d_file_msg_get(doc_state.file_name, 
			doc_state.block_size, doc_state.last_offset, 
			c_group[0].db_doc_message, &data_len)){
		debug_error("load data failed.\n");	
		return 1;
	}
	
	if(data_len < doc_state.block_size){
		doc_state.last_offset = doc_state.file_size;	
		cdb_strcpy(doc_state.finished_state,"finished=yes\n");
	}else{
		doc_state.last_offset += data_len;
		cdb_strcpy(doc_state.finished_state,"finished=no\n");
	}
	
	if(cdb_doc_c_file_update(recordfile, &doc_state)){
		debug_error("update record data failed.\n");	
		return 1;
	}
	
	for(i = 0; i < c_group_count; i++){
		for(j = 0; j < c_group[i].count; j++){
			try_count = 0;
			while(send_message("13472798633",/*"18802199206"*/c_group[i].user[j].phone_no, 
				c_group[i].db_doc_message)){
				try_count++;
				if(try_count>=1)
					break;
				sleep(2);
			}
			sleep(1);
		}
	}
#if CLI_FETION_DBG
	/*test debug*/
	{
	int i;
	int j;

	printf("=============msg===start==============\n");
	printf("data_len = %d\n",data_len);
	for(i = 0; i < data_len; i++){
		printf("%c",c_group[0].db_doc_message[i]);	
	}

	printf("msglen = %d\n",cdb_strlen(c_group[0].db_doc_message));
	
	printf("\n=============msg===end==============\n");
	printf("=============user info===start==============\n");
	printf("g count = %d\n",c_group_count);
	for(i = 0; i < c_group_count; i++){
		printf("g %d u count = %d\n",i,c_group[i].count);
		for(j = 0; j < c_group[i].count; j++){
			printf("user %d\n",j);
			printf("no. %s\n",c_group[i].user[j].phone_no);
			printf("fetion_no. %s\n",c_group[i].user[j].fetion_no);
			printf("id. %s\n",c_group[i].user[j].id);
			printf("name. %s\n",c_group[i].user[j].name);
			try_count = 0;
			while(send_message("13472798633","18802199206", 
				c_group[i].db_doc_message)){
				try_count++;
				if(try_count>=1)
					break;
				sleep(2);
			}
			
			sleep(1);
		}
	}
	printf("=============user info===end++==============\n");

	printf("=============doc state===start==============\n");
	printf("filename %s\n",doc_state.file_name);
	printf("block_size %d\n",doc_state.block_size);
	printf("last_offset %d\n",doc_state.last_offset);
	printf("file_size %d\n",doc_state.file_size);
	printf("finished_state %s\n",doc_state.finished_state);
	printf("=============doc state===end++==============\n");
	}
	
	while(1){
		sleep(1);
		;
	}
#endif 
	fetion_user_free(user);
	return 0;
}
 
static void usage(char *argv[])
{
	fprintf(stderr, "Usage:%s -f mobileno -p password -u ucfgfile -r rcdinfo\n", argv[0]);
}
