#ifndef __phone_book_entry__
#define __phone_book_entry__

#include <gtk/gtkframe.h>

G_BEGIN_DECLS

#define PHONE_BOOK_ENTRY_TYPE (phoneBookEntry_get_type())
#define PHONE_BOOK_ENTRY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PHONE_BOOK_ENTRY_TYPE, PhoneBookEntry))
#define PHONE_BOOK_ENTRY_CLASS(obj) (G_TYPE_CHECK_CLASS_CAST ((obj), PHONE_BOOK_ENTRY, PhoneBookEntryClass))
#define PHONE_BOOK_ENTRY_IS_PHONE_BOOK_ENTRY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PHONE_BOOK_ENTRY_TYPE))
#define PHONE_BOOK_ENTRY_IS_PHONE_BOOK_ENTRY_CLASS(obj) (G_TYPE_CHECK_CLASS_TYPE ((obj), PHONE_BOOK_ENTRY_CLASS))
#define PHONE_BOOK_ENTRY_GET_CLASS (G_TYPE_INSTANCE_GET_CLASS ((obj), PHONE_BOOK_ENTRY_TYPE, PhoneBookEntryClass))

typedef struct _PhoneBookEntry PhoneBookEntry;
typedef struct _PhoneBookEntryClass PhoneBookEntryClass;

struct _PhoneBookEntry {
	GtkFrame parent;
};

struct _PhoneBookEntryClass {
	GtkFrameClass parent_class;
};


GtkWidget * phone_book_entry_new();

void phone_book_entry_set_name(PhoneBookEntry *, const gchar *);

void phone_book_entry_set_number(PhoneBookEntry *, const gchar *);

G_END_DECLS

#endif
