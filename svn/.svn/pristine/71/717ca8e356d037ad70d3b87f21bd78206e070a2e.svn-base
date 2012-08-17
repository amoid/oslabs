#include <libxml/parser.h>
#include <libxml/xpath.h>

xmlDocPtr
getDoc(char *docName)
{
	xmlDocPtr doc;

	doc = xmlParserFile(docName);

	if(NULL == doc){
		printf("Document not parse successfully\n");
		return NULL;
	}

	return doc;
}

xmlXPathObjectPtr
getNodeSet(xmlDocPtr doc, xmlChar *xpath)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if(NULL == context){
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}

	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if(NULL == result){
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}

	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}

	return result;
}	

int 
main(int argc, char *argv[])
{
	char *docName;
	xmlDocPtr doc;
	xmlChar *xpath = (xmlChar *)"//keyword";
	xmlNodeSetPtr nodeset;
	xmlXpathObjectPtr result;
	int i;
	xmlChar *keyword;

	if(argc <= 1){
		printf("Usage: %s docname\n",argv[0]);
		return 0;
	}

	docName = argv[1];
	doc = getDoc(docName);
	result = getNodeSet(doc, xpath);

	if(result){
		nodeset =  result->nodesetval;
		for(i=0; i < nodeset->nodeNr; i++){
			keyword = xmlNodeListGetString(doc, nodeset->nodeTb[i]);
			printf("keyword: %s\n",keyword);
			xmlFree(keyword);
		}
		xmlXPathFreeObject(result);
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 1;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void 
parseStory(xmlDoctPtr doc, xmlNodePtr cur, char *keyword)
{
	xmlNewTextChild(cur, NULL, "keyword", keyword);

	return ;
}

xmlDocPtr
parseDoc(char *docName, char *keyword)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParserFile(docName);

	if(NULL == doc){
		printf("Document not parsed successfully\n");
		return ;
	}

	cur = xmlDocGetRootElement(doc);

	if(NULL == cur){
		printf("empty document\n");
		return NULL;
	}

	if(xmlStrcmp(cur->name, (const xmlChar *)"story")){
		printf("Document of the wrong type, root node\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	cur = cur->xmlChildrenNode;
	while(cur != NULL){
		if(!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo")){
			parseStory(doc, cur, keyword);
		}
	cur = cur->next;	
	}
	return doc;
}

int 
main(int argc, char *argv[]){
	char *docName;
	char *keyword;
	xmlDocPtr doc;

	if(argc <= 2){
		printf("Usage: %s docname, keyword\n",argv[0]);
		return 0;
	}

	docName = argv[1];
	keyword = argv[2];
	doc = parseDoc(docName, keyword);
	if(doc != NULL){
		xmlSaveFormatFile(docName, doc, 0);
		xmlFreeDoc(doc);
	}
	return 1;
}

