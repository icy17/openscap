/**
 * @file oval_resultDirectives.c
 * \brief Open Vulnerability and Assessment Language
 *
 * See more details at http://oval.mitre.org/
 */

/*
 * Copyright 2008 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *      "David Niemoller" <David.Niemoller@g2-inc.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "oval_results_impl.h"
#include "oval_collection_impl.h"


struct _oval_result_directive{
	bool                               reported;
	oval_result_directive_content_enum content;
};

#define NUMBER_OF_DIRECTIVES 7

typedef struct oval_result_directives {
	struct _oval_result_directive directive[NUMBER_OF_DIRECTIVES];
} oval_result_directives_t;

struct oval_result_directives *oval_result_directives_new()
{
	oval_result_directives_t *directives = (oval_result_directives_t *)
		malloc(sizeof(oval_result_directives_t));
	int i;for(i=0;i<NUMBER_OF_DIRECTIVES;i++){
		directives->directive[i].reported = false;
		directives->directive[i].content  = OVAL_DIRECTIVE_CONTENT_UNKNOWN;
	}
	return directives;
}
void oval_result_directives_free(struct oval_result_directives *directives)
{
	free(directives);
}

bool oval_result_directive_reported
	(struct oval_result_directives *directives, oval_result_directive_enum type)
{
	return directives->directive[type].reported;
}
oval_result_directive_content_enum oval_result_directive_content
	(struct oval_result_directives *directives, oval_result_directive_enum type)
{
	return directives->directive[type].content;
}
void set_oval_result_directive_reported
	(struct oval_result_directives *directives, oval_result_directive_enum type, bool reported)
{
	directives->directive[type].reported = reported;
}
void set_oval_result_directive_content
	(struct oval_result_directives *directives, oval_result_directive_enum type, oval_result_directive_content_enum content)
{
	directives->directive[type].content = content;
}

//typedef int (*oval_xml_tag_parser) (xmlTextReaderPtr, struct oval_parser_context *, void *);
int _oval_result_directives_parse_tag
	(xmlTextReaderPtr reader, struct oval_parser_context *context, void *client)
{
	struct oval_result_directives *directives = (struct oval_result_directives *)client;
	oval_result_directive_content_enum type = OVAL_DIRECTIVE_UNKNOWN;
	char *tag_names[NUMBER_OF_DIRECTIVES] =
	{
		NULL
		,"definition_true"
		,"definition_false"
		,"definition_unknown"
		,"definition_error"
		,"definition_not_evaluated"
		,"definition_not_applicable"
	};
	int i, retcode = 1;
	xmlChar *name = xmlTextReaderLocalName(reader);
	for(i=1;i<NUMBER_OF_DIRECTIVES && type==OVAL_DIRECTIVE_UNKNOWN;i++){
		if(strcmp(tag_names[i],name)==0){
			type = i;
		}
	}
	if(type){
		{//reported
			char* boolstr = xmlTextReaderGetAttribute(reader, "reported");
			bool reported = strcmp(boolstr,"1")==0 || strcmp(boolstr,"true")==0;
			free(boolstr);
			set_oval_result_directive_reported(directives, type, reported);
		}
		{//content
			xmlChar *contentstr =  xmlTextReaderGetAttribute(reader, "content");
			oval_result_directive_content_enum content = OVAL_DIRECTIVE_CONTENT_UNKNOWN;
			if(contentstr){
				char *content_names[3] = {NULL,"thin", "full"};
				for(i=1;i<3 && content==OVAL_DIRECTIVE_CONTENT_UNKNOWN;i++){
					if(strcmp(content_names[i],contentstr)==0){
						content = i;
					}
				}
				if(content){
					set_oval_result_directive_content(directives, type, content);
				}else{
					char message[200];
					sprintf(message, "_oval_result_directives_parse_tag: cannot resolve @content=\"%s\"",contentstr);
					oval_parser_log_warn(context, message);
					retcode = 0;
				}
				free(contentstr);
			}else{
				content = OVAL_DIRECTIVE_CONTENT_FULL;
			}
		}
	}else{
		char message[200];
		sprintf(message, "_oval_result_directives_parse_tag: cannot resolve <%s>",name);
		oval_parser_log_warn(context, message);
		retcode = 0;
	}
	free(name);
	return retcode;
}

int oval_result_directives_parse_tag
	(xmlTextReaderPtr reader, struct oval_parser_context *context, struct oval_result_directives *directives)
{
	return oval_parser_parse_tag(reader, context, &_oval_result_directives_parse_tag, directives);
}

