#include "phoneBookEntry.h"

#include <gtk/gtklabel.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtkeventbox.h>

#define PHONE_BOOK_ENTRY_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), PHONE_BOOK_ENTRY_TYPE, phoneBookEntryPrivate))

G_DEFINE_TYPE (PhoneBookEntry, phoneBookEntry, GTK_TYPE_FRAME);

typedef struct _phoneBookEntryPrivate phoneBookEntryPrivate;

struct _phoneBookEntryPrivate {
	GtkWidget * name;
	GtkWidget * number;
};

void phoneBookEntry_class_init(PhoneBookEntryClass *klass) {
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	GtkFrameClass *frame_class = GTK_FRAME_CLASS(klass);
	
	g_type_class_add_private(object_class, sizeof(phoneBookEntryPrivate));
}

void focus_me(GtkWidget *widget, GdkEvent *event) {
	fprintf(stdout,"focused\n");
	fflush(stdout);
}

void phoneBookEntry_init(PhoneBookEntry *pbe) {
	phoneBookEntryPrivate *priv = PHONE_BOOK_ENTRY_GET_PRIVATE(pbe);
	GtkWidget *event_box = gtk_event_box_new();
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(event_box), TRUE);
	gtk_widget_add_events(event_box, GDK_FOCUS_CHANGE_MASK);
	g_signal_connect(G_OBJECT(event_box), "focus", G_CALLBACK(focus_me), NULL);
	GtkWidget *vbox = gtk_vbox_new(FALSE, 2);
	priv->name = gtk_label_new("");
	priv->number = gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(vbox), priv->name, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), priv->number, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(event_box), vbox);
	gtk_container_add(GTK_CONTAINER(pbe), event_box);
}

void phone_book_entry_set_name(PhoneBookEntry *pbe, const gchar *name) {
	g_return_if_fail(PHONE_BOOK_ENTRY_IS_PHONE_BOOK_ENTRY(pbe));
	phoneBookEntryPrivate *priv = PHONE_BOOK_ENTRY_GET_PRIVATE(pbe);
	gtk_label_set_text(GTK_LABEL(priv->name), name);
}

void phone_book_entry_set_number(PhoneBookEntry *pbe, const gchar *number) {
	g_return_if_fail(PHONE_BOOK_ENTRY_IS_PHONE_BOOK_ENTRY(pbe));
	phoneBookEntryPrivate *priv = PHONE_BOOK_ENTRY_GET_PRIVATE(pbe);
	gtk_label_set_text(GTK_LABEL(priv->number), number);
}

GtkWidget * phone_book_entry_new() {
	return g_object_new(PHONE_BOOK_ENTRY_TYPE, NULL);
}
