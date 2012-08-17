#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void
parseUsers (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *name;
	xmlChar *phoneNo;
	xmlChar *fetionNo;
	xmlChar *sipNo;

	cur = cur->xmlChildrenNode;

	while(cur != NULL){
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"name"))){
			name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("name: %s\n",name);
			xmlFree(name);
		}
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"phoneNo"))){
			phoneNo = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("phoneNo: %s\n",phoneNo);
			xmlFree(phoneNo);
		}
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"fetionNo"))){
			fetionNo = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("fetionNo: %s\n",fetionNo);
			xmlFree(fetionNo);
		}
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"sipNo"))){
			sipNo = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("sipNo: %s\n",sipNo);
			xmlFree(sipNo);
		}
		cur = cur->next;
	}

	return;
}

void
parseUsers (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *user;
	
	cur = cur->xmlChildrenNode;

	while(cur != NULL){
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"name"))){
			name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("name: %s\n",name);
			xmlFree(name);
		}
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"phoneNo"))){
			phoneNo = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("phoneNo: %s\n",phoneNo);
			xmlFree(phoneNo);
		}
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"fetionNo"))){
			fetionNo = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("fetionNo: %s\n",fetionNo);
			xmlFree(fetionNo);
		}
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"sipNo"))){
			sipNo = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("sipNo: %s\n",sipNo);
			xmlFree(sipNo);
		}
		cur = cur->next;
	}

	return;
}

static void
parseDoc(char *docName)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docName);

	if(NULL == doc){
		printf("input a NULL file name\n");
		return ;
	}

	cur = xmlDocGetRootElement(doc);

	if(NULL == cur){
		printf("empty document\n");
		xmlFreeDoc(doc);
		return ;
	}

	cur = cur->xmlChildrenNode;
	while(cur != NULL){
		if(!xmlStrcmp(cur->name, (const xmlChar *)"user")){
			parseStory(doc, cur);
		}
		cur = cur->next;
	}
	xmlFreeDoc(doc);

	return;
}

int
main(int argc, char *argv[])
{
	char *docName;

	if(argc <= 1){
		printf("usage: %s docname\n",argv[0]);
		return 0;
	}

	docName = argv[1];

	parseDoc(docName);

	return 1;
}
