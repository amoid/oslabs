#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void
parseStory (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;

	cur = cur->xmlChildrenNode;

	while(cur != NULL){
		if(!xmlStrcmp(cur->name, ((const xmlChar *)"keyword"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("keyword: %s\n",key);
			xmlFree(key);
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
		if(!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo")){
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
