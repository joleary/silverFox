#include "phoneBookParser.h"
#include <stdio.h>

GString *parsedText;
phoneBookEntry *pbEntry;
GList *contact_list;

void phone_book_parser_start_element(GMarkupParseContext *context, const gchar *element_name,
                          const gchar **attribute_names, const gchar **attribute_values,
                          gpointer user_data, GError **error) {
	parsedText = g_string_new("");
	if(strcmp(element_name,"phoneBook")==0) {
		contact_list = NULL;
		return;
	}
	if(strcmp(element_name,"contact")==0) {
		pbEntry = g_malloc0(sizeof(phoneBookEntry *));
		return;
	}

}

void phone_book_parser_end_element(GMarkupParseContext *context, const gchar *element_name, 
						gpointer user_data, GError **error) {
	if(g_strcmp0(element_name,"contact")==0) {
		contact_list = g_list_append(contact_list,pbEntry);
		return;
	}
	if(g_strcmp0(element_name,"name")==0) {
		pbEntry->name = g_strdup(parsedText->str);
		return;
	}
	if(g_strcmp0(element_name,"number")==0) {
		pbEntry->number = g_strdup(parsedText->str);
		return;
	}
	if(g_strcmp0(element_name,"picture")==0) {
		pbEntry->pictureURI = g_strdup(parsedText->str);
		return;
	}
}

void phone_book_parser_text(GMarkupParseContext *context, const gchar *text, gsize text_len,  
									gpointer user_data, GError **error) {
	g_string_append(parsedText, text);
}

GList * phone_book_parser_get_parsed_contacts_list() {
	return g_list_copy(contact_list);
}
