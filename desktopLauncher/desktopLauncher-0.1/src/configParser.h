#ifndef __config_parser__

#define __config_parser__

#include <glib.h>

void config_parser_start_element(GMarkupParseContext *, const gchar *, const gchar **, 
										const gchar **, gpointer, GError **);

void config_parser_end_element(GMarkupParseContext *, const gchar *,gpointer, GError **);

void config_parser_text(GMarkupParseContext *, const gchar *, gsize, gpointer, GError **);

GList * config_parser_get_launchers();

gchar * config_parser_get_fg();

gchar * config_parser_get_bg();


typedef struct _launcherEntry launcherEntry;

struct _launcherEntry {
	const gchar *name;
	const gchar *action;
	const gchar *icon;
};

static GMarkupParser configParser = {
	config_parser_start_element,
	config_parser_end_element,
	config_parser_text,
	NULL,
	NULL
};

#endif
