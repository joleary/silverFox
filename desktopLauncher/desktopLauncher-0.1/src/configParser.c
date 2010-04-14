#include "configParser.h"
#include <stdio.h>

GString *parsedText;
launcherEntry *lEntry;
GList *launcher_list;

gchar *fgColor;

gchar *bgColor;

void config_parser_start_element(GMarkupParseContext *context, const gchar *element_name,
                          const gchar **attribute_names, const gchar **attribute_values,
                          gpointer user_data, GError **error) {
	
	parsedText = g_string_new("");
	
	if(g_strcmp0(element_name,"desktopLauncher")==0) {
		g_list_free(launcher_list);
		
		const gchar **names = attribute_names;
		const gchar **values = attribute_values;
		
		while(*names) {
			if(g_strcmp0("fg",*names)==0) { fgColor = g_strdup(*values); }
			if(g_strcmp0("bg",*names)==0) { bgColor = g_strdup(*values); }
			names++;
			values++;
		}
		
		return;
	}
	
	if(g_strcmp0(element_name,"launcher")==0) {
		lEntry = g_malloc0(sizeof(launcherEntry *));
		const gchar **names = attribute_names;
		const gchar **values = attribute_values;
		
		while(*names) {
			if(g_strcmp0("name",*names)==0) { lEntry->name = g_strdup(*values); }
			if(g_strcmp0("action",*names)==0) { lEntry->action = g_strdup(*values); }
			if(g_strcmp0("icon",*names)==0) { 
				GString *iconPath = g_string_new(*values);
				if(iconPath->str[0]!='/') {
					g_string_prepend(iconPath,((GString *)user_data)->str);
				}
				lEntry->icon = iconPath->str;
				fprintf(stdout,"%s\n", lEntry->icon);
				fflush(stdout);
			}
			names++;
			values++;
		}
		
		launcher_list = g_list_append(launcher_list, lEntry);
		
		return;
	}

}

void config_parser_end_element(GMarkupParseContext *context, const gchar *element_name, 
						gpointer user_data, GError **error) {
}

void config_parser_text(GMarkupParseContext *context, const gchar *text, gsize text_len,  
									gpointer user_data, GError **error) {
	g_string_append(parsedText, text);
}

GList * config_parser_get_launchers() {
	return g_list_copy(launcher_list);
}

gchar * config_parser_get_bg() {
	return g_strdup(bgColor);
}

gchar * config_parser_get_fg() {
	return g_strdup(fgColor);
}
