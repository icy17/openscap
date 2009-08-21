/**
 * @file oval_results.h 
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


#ifndef OVAL_RESULTS_H_
#define OVAL_RESULTS_H_

#include "oval_definitions.h"
#include "oval_system_characteristics.h"
#include <stdbool.h>

typedef enum {
	OVAL_RESULT_TRUE           = 1,
	OVAL_RESULT_FALSE          = 2,
	OVAL_RESULT_UNKNOWN        = 3,
	OVAL_RESULT_ERROR          = 4,
	OVAL_RESULT_NOT_EVALUATED  = 5,
	OVAL_RESULT_NOT_APPLICABLE = 6
} oval_result_enum;

typedef enum {
	OVAL_DIRECTIVE_UNKNOWN                = 0,
	OVAL_DIRECTIVE_TRUE                   = 1,
	OVAL_DIRECTIVE_FALSE                  = 2,
	OVAL_DIRECTIVE_UNKNOWN_CONTENT        = 3,
	OVAL_DIRECTIVE_ERROR_CONTENT          = 4,
	OVAL_DIRECTIVE_NOT_EVALUATED_CONTENT  = 5,
	OVAL_DIRECTIVE_NOT_APPLICABLE_CONTENT = 6
} oval_result_directive_enum;

typedef enum {
	OVAL_DIRECTIVE_CONTENT_UNKNOWN = 0,
	OVAL_DIRECTIVE_CONTENT_THIN    = 1,
	OVAL_DIRECTIVE_CONTENT_FULL    = 2
} oval_result_directive_content_enum;

struct oval_result_item;
struct oval_iterator_result_item;

struct oval_result_test;
struct oval_iterator_result_test;

struct oval_result_criteria_node;
struct oval_iterator_result_criteria_node;

struct oval_result_directives;

struct oval_result;
struct oval_iterator_result;

int oval_iterator_result_item_has_more(struct oval_iterator_result_item *);
struct oval_result_item *oval_iterator_result_item_next(struct
							oval_iterator_result_item
							*);

struct oval_sysdata *oval_result_item_sysdata(struct oval_result_item *);
oval_result_enum oval_result_item_result(struct oval_result_item *);
char *oval_result_item_message(struct oval_result_item *);

int oval_iterator_result_test_has_more(struct oval_iterator_result_test *);
struct oval_result_test *oval_iterator_result_test_next(struct
							oval_iterator_result_test
							*);

struct oval_test *oval_result_test_test(struct oval_result_test *);
oval_check_enum oval_result_test_check(struct oval_result_test *);
oval_result_enum oval_result_test_result(struct oval_result_test *);
char *oval_result_test_message(struct oval_result_test *);
struct oval_iterator_result_item *oval_result_test_items(struct oval_result_test
							 *);

int oval_iterator_result_criteria_node_has_more(struct
						oval_iterator_result_criteria_node
						*);
struct oval_result_criteria_node *oval_iterator_result_criteria_node_next(struct
									  oval_iterator_result_criteria_node
									  *);

oval_criteria_node_type_enum oval_result_criteria_node_type(struct
							    oval_result_criteria_node
							    *);
oval_result_enum oval_result_criteria_node_result(struct
						  oval_result_criteria_node *);
oval_operator_enum oval_result_criteria_node_operator(struct oval_result_criteria_node *);	//type==NODETYPE_CRITERIA
struct oval_iterator_criteria_node *oval_result_criteria_node_subnodes(struct oval_result_criteria_node *);	//type==NODETYPE_CRITERIA
struct oval_result_test *oval_result_criteria_node_test(struct oval_result_criteria_node *);	//type==NODETYPE_CRITERION
struct oval_result *oval_result_criteria_node_extends(struct oval_result_criteria_node *);	//type==NODETYPE_EXTENDDEF

bool oval_result_directive_reported
	(struct oval_result_directives *, oval_result_directive_enum);
oval_result_directive_content_enum oval_result_directive_content
	(struct oval_result_directives *, oval_result_directive_enum);

void set_oval_result_directive_reported
	(struct oval_result_directives *, oval_result_directive_enum, bool);
void set_oval_result_directive_content
	(struct oval_result_directives *, oval_result_directive_enum, oval_result_directive_content_enum);

int oval_iterator_result_has_more(struct oval_iterator_result *);
struct oval_result *oval_iterator_result_next(struct oval_iterator_result *);

struct oval_definition *oval_result_definition(struct oval_result *);
oval_result_enum oval_result_result(struct oval_result *);
char *oval_result_message(struct oval_result *);
struct oval_result_directives *oval_result_directives(struct oval_result *);
struct oval_result_criteria_node *oval_result_criteria(struct oval_result *);

#endif				/* OVAL_RESULTS_H_ */
