/*--------------------------------------------------------------------
 *
 *	Copyright (c) 1991-2022 by the GMT Team (https://www.generic-mapping-tools.org/team.html)
 *	See LICENSE.TXT file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation; version 3 or any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	Contact info: www.generic-mapping-tools.org
 *--------------------------------------------------------------------*/
/*
 * Author:	Paul Wessel
 * Date:	2-May-2013
 * Version:	6 API
 *
 * Brief synopsis: gmt write lets us (read from memory and) write any of the 6 GMT resources.
 *
 */

#include "gmt_dev.h"

#define THIS_MODULE_CLASSIC_NAME	"gmtwrite"
#define THIS_MODULE_MODERN_NAME	"gmtwrite"
#define THIS_MODULE_LIB		"core"
#define THIS_MODULE_PURPOSE	"Write GMT objects from external API"
#define THIS_MODULE_KEYS	"-T-,<?{,>?}"
#define THIS_MODULE_NEEDS	""
#define THIS_MODULE_OPTIONS "->RV"

/* Control structure for gmtwrite */

struct GMTWRITE_CTRL {
	struct GMTWRITE_IO {	/* Need two args with filenames */
		bool active[2];
		char *file[2];
	} IO;
	struct GMTWRITE_T {	/* -T sets data type */
		bool active;
		enum GMT_enum_family mode;
	} T;
};

static void *New_Ctrl (struct GMT_CTRL *GMT) {	/* Allocate and initialize a new control structure */
	struct GMTWRITE_CTRL *C;

	C = gmt_M_memory (GMT, NULL, 1, struct GMTWRITE_CTRL);

	/* Initialize values whose defaults are not 0/false/NULL */

	return (C);
}

static void Free_Ctrl (struct GMT_CTRL *GMT, struct GMTWRITE_CTRL *C) {	/* Deallocate control structure */
	if (!C) return;
	gmt_M_str_free (C->IO.file[GMT_IN]);
	gmt_M_str_free (C->IO.file[GMT_OUT]);
	gmt_M_free (GMT, C);
}

static int usage (struct GMTAPI_CTRL *API, int level) {
	const char *name = gmt_show_name_and_purpose (API, THIS_MODULE_LIB, THIS_MODULE_CLASSIC_NAME, THIS_MODULE_PURPOSE);
	if (level == GMT_MODULE_PURPOSE) return (GMT_NOERROR);
	GMT_Message (API, GMT_TIME_NONE, "usage: %s <infile> <outfile> -Tc|d|g|i|p|u [%s] [%s]\n", name, GMT_Rx_OPT, GMT_V_OPT);

	if (level == GMT_SYNOPSIS) return (GMT_MODULE_SYNOPSIS);

	GMT_Message (API, GMT_TIME_NONE, "\t   Specify input and output file names\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-T Specify data type.  Choose among:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   c : CPT\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   d : Dataset\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   g : Grid\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   i : Image\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   p : PostScript\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   u : Cube\n");
	GMT_Message (API, GMT_TIME_NONE, "\n  OPTIONAL ARGUMENTS:\n");
	GMT_Option (API, "R,V,.");

	return (GMT_MODULE_USAGE);
}

static int parse (struct GMT_CTRL *GMT, struct GMTWRITE_CTRL *Ctrl, struct GMT_OPTION *options) {

	/* This parses the options provided to grdwrite and sets parameters in CTRL.
	 * Any GMT common options will override values set previously by other commands.
	 * It also replaces any file names specified as input or output with the data ID
	 * returned when registering these sources/destinations with the API.
	 */

	unsigned int n_errors = 0, n_files = 0;
	struct GMT_OPTION *opt = NULL;
	struct GMTAPI_CTRL *API = GMT->parent;

	for (opt = options; opt; opt = opt->next) {	/* Process all the options given */

		switch (opt->option) {
			/* Processes program-specific parameters */

			case GMT_OPT_INFILE:	/* File args */
				if (Ctrl->IO.active[GMT_OUT]) {	/* User gave output as ->outfile and it was found on the command line earlier */
					n_errors += gmt_M_repeated_module_option (API, Ctrl->IO.active[GMT_IN]);
					Ctrl->IO.active[GMT_IN] = true;
					Ctrl->IO.file[GMT_IN] = strdup (opt->arg);
				}
				else if (n_files < 2) {	/* Encountered input file(s); the 2nd would be output */
					n_errors += gmt_M_repeated_module_option (API, Ctrl->IO.active[n_files]);
					Ctrl->IO.active[n_files] = true;
					Ctrl->IO.file[n_files] = strdup (opt->arg);
				}
				n_files++;
				break;
			case GMT_OPT_OUTFILE:	/* Got specific output argument */
				n_errors += gmt_M_repeated_module_option (API, Ctrl->IO.active[GMT_OUT]);
				Ctrl->IO.active[GMT_OUT] = true;
				Ctrl->IO.file[GMT_OUT] = strdup (opt->arg);
				n_files++;
				break;
			case 'T':	/* Type */
				n_errors += gmt_M_repeated_module_option (API, Ctrl->T.active);
				Ctrl->T.active = true;
				switch (opt->arg[0]) {
					case 't':
						if (gmt_M_compat_check (GMT, 5))	/* There is no longer a T type but we will honor T as D from GMT5 */
							Ctrl->T.mode = GMT_IS_DATASET;
						else {
							GMT_Report (API, GMT_MSG_ERROR, "Unrecognized data type %c.  Choose from c, d, g, i, p, and u\n", opt->arg[0]);
							n_errors++;
						}
						break;
					case 'd': Ctrl->T.mode = GMT_IS_DATASET; break;
					case 'g': Ctrl->T.mode = GMT_IS_GRID;	 break;
					case 'c': Ctrl->T.mode = GMT_IS_PALETTE;	 break;
					case 'i': Ctrl->T.mode = GMT_IS_IMAGE;	 break;
					case 'p': Ctrl->T.mode = GMT_IS_POSTSCRIPT;	 break;
					case 'u': Ctrl->T.mode = GMT_IS_CUBE;	 break;
					default:
						GMT_Report (API, GMT_MSG_ERROR, "Unrecognized data type %c.  Choose from c, d, g, i, p, and u\n", opt->arg[0]);
						n_errors++;
						break;
				}
				break;

			default:	/* Report bad options */
				n_errors += gmt_default_option_error (GMT, opt);
				break;
		}
	}

	n_errors += gmt_M_check_condition (GMT, !(Ctrl->IO.active[GMT_IN] && Ctrl->IO.active[GMT_OUT]), "Must specify both input and output filenames\n");
	n_errors += gmt_M_check_condition (GMT, Ctrl->IO.active[GMT_IN] && (!Ctrl->IO.file[GMT_IN] || !Ctrl->IO.file[GMT_IN][0]), "Must specify input filename\n");
	n_errors += gmt_M_check_condition (GMT, Ctrl->IO.active[GMT_OUT] && (!Ctrl->IO.file[GMT_OUT] || !Ctrl->IO.file[GMT_OUT][0]), "Must specify output filename\n");
	n_errors += gmt_M_check_condition (GMT, n_files != 2, "Must specify only two filenames (input and output)\n");
	n_errors += gmt_M_check_condition (GMT, !Ctrl->T.active, "Option -T: Must specify a valid datatype\n");

	return (n_errors ? GMT_PARSE_ERROR : GMT_NOERROR);
}

#define bailout(code) {gmt_M_free_options (mode); return (code);}
#define Return(code) {Free_Ctrl (GMT, Ctrl); gmt_end_module (GMT, GMT_cpy); bailout (code);}

EXTERN_MSC int GMT_gmtwrite (void *V_API, int mode, void *args) {
	int error = 0;
	struct GMTWRITE_CTRL *Ctrl = NULL;
	struct GMT_CTRL *GMT = NULL, *GMT_cpy = NULL;
	struct GMT_OPTION *options = NULL;
	struct GMTAPI_CTRL *API = gmt_get_api_ptr (V_API);	/* Cast from void to GMTAPI_CTRL pointer */

	/*----------------------- Standard module initialization and parsing ----------------------*/

	if (API == NULL) return (GMT_NOT_A_SESSION);
	if (mode == GMT_MODULE_PURPOSE) return (usage (API, GMT_MODULE_PURPOSE));	/* Return the purpose of program */
	options = GMT_Create_Options (API, mode, args);	if (API->error) return (API->error);	/* Set or get option list */

	if ((error = gmt_report_usage (API, options, 0, usage)) != GMT_NOERROR) bailout (error);	/* Give usage if requested */

	/* Parse the command-line arguments */

	if ((GMT = gmt_init_module (API, THIS_MODULE_LIB, THIS_MODULE_CLASSIC_NAME, THIS_MODULE_KEYS, THIS_MODULE_NEEDS, NULL, &options, &GMT_cpy)) == NULL) bailout (API->error); /* Save current state */
	if (GMT_Parse_Common (API, THIS_MODULE_OPTIONS, options)) Return (API->error);
	Ctrl = New_Ctrl (GMT);	/* Allocate and initialize a new control structure */
	if ((error = parse (GMT, Ctrl, options)) != 0) Return (error);

	/*---------------------------- This is the gmtwrite main code ----------------------------*/

	error = gmt_copy (API, Ctrl->T.mode, GMT_OUT, Ctrl->IO.file[GMT_IN], Ctrl->IO.file[GMT_OUT]);
	Return (error);
}
