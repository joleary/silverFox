#ifndef __phone_book_parser__

#define __phone_book_parser__

#include <glib.h>

void phone_book_parser_start_element(GMarkupParseContext *, const gchar *, const gchar **, 
										const gchar **, gpointer, GError **);

void phone_book_parser_end_element(GMarkupParseContext *, const gchar *,gpointer, GError **);

void phone_book_parser_text(GMarkupParseContext *, const gchar *, gsize, gpointer, GError **);

GList * phone_book_parser_get_parsed_contacts_list();

typedef struct _phoneBookEntry phoneBookEntry;

struct _phoneBookEntry {
	const gchar *name;
	const gchar *number;
	const gchar *pictureURI;
};

static GMarkupParser phoneBookParser = {
	phone_book_parser_start_element,
	phone_book_parser_end_element,
	phone_book_parser_text,
	NULL,
	NULL
};

#endif
