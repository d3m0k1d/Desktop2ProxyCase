// parse_xml.c
#include "parse_xml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parse_open_ports(const char *filename) {
    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (!doc) return NULL;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root) { xmlFreeDoc(doc); return NULL; }

    size_t cap = 4096;
    char *out = malloc(cap);
    if (!out) { xmlFreeDoc(doc); return NULL; }
    out[0] = '\0';

    for (xmlNodePtr host = root->children; host; host = host->next) {
        if (host->type==XML_ELEMENT_NODE && !xmlStrcmp(host->name,(const xmlChar*)"host")) {
            xmlChar *addr = NULL;
            for (xmlNodePtr n=host->children; n; n=n->next) {
                if (!xmlStrcmp(n->name,(const xmlChar*)"address")) {
                    addr = xmlGetProp(n,(const xmlChar*)"addr");
                    break;
                }
            }
            const char *ip = addr?(char*)addr:"unknown";
            size_t need = strlen(out)+6+strlen(ip)+2;
            if (need>cap) { cap=need*2; out=realloc(out,cap); }
            strcat(out,"Host: "); strcat(out,ip); strcat(out,"\n");
            if (addr) xmlFree(addr);

            for (xmlNodePtr n=host->children; n; n=n->next) {
                if (!xmlStrcmp(n->name,(const xmlChar*)"ports")) {
                    for (xmlNodePtr p=n->children; p; p=p->next) {
                        if (!xmlStrcmp(p->name,(const xmlChar*)"port")) {
                            xmlChar *protocol = xmlGetProp(p,(const xmlChar*)"protocol");
                            xmlChar *portid   = xmlGetProp(p,(const xmlChar*)"portid");
                            int is_open = 0;
                            xmlChar *svc_name = NULL;

                            for (xmlNodePtr ch=p->children; ch; ch=ch->next) {
                                if (!xmlStrcmp(ch->name,(const xmlChar*)"state")) {
                                    xmlChar *st = xmlGetProp(ch,(const xmlChar*)"state");
                                    if (st && !xmlStrcmp(st,(const xmlChar*)"open")) is_open = 1;
                                    if (st) xmlFree(st);
                                }
                                if (!xmlStrcmp(ch->name,(const xmlChar*)"service")) {
                                    svc_name = xmlGetProp(ch,(const xmlChar*)"name");
                                }
                            }
                            if (is_open) {
                                const char *pp = protocol?(char*)protocol:"?";
                                const char *pnum = portid?(char*)portid:"?";
                                const char *sv = svc_name?(char*)svc_name:"?";
                                need = strlen(out)+3+strlen(pp)+1+strlen(pnum)+1+strlen(sv)+2;
                                if (need>cap) { cap=need*2; out=realloc(out,cap); }
                                strcat(out,"  ");
                                strcat(out,pp);
                                strcat(out,"/");
                                strcat(out,pnum);
                                if (sv[0]) { strcat(out," "); strcat(out,sv); }
                                strcat(out,"\n");
                            }

                            if (protocol) xmlFree(protocol);
                            if (portid)   xmlFree(portid);
                            if (svc_name) xmlFree(svc_name);
                        }
                    }
                }
            }
        }
    }
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return out;
}
