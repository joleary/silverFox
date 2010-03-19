#include <gtk/gtk.h>
#include <gio/gio.h>
#include "phoneBookParser.h"
#include "phoneBookEntry.h"

GtkWidget *phoneBookWindow;
GtkWidget *phoneBookDialog;
GtkWidget *box;
GtkWidget *scroll;

gchar *dataLocation;

GList *current_contacts=NULL;

void phoneBookExit(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}

/* Return TRUE if the xml file with
 * data is found in the home directory of the
 * running user.
 */
gboolean checkForData() {
	GFile *data = g_file_new_for_path(dataLocation);
	GError *error=NULL;
	g_file_create(data, G_FILE_CREATE_PRIVATE, NULL, &error);
	if(error->code!=G_IO_ERROR_EXISTS) {
		fprintf(stdout,"Data does not exist\n");
		fflush(stdout);
		return FALSE;
	}
	return TRUE;
}

/* Populate the list with phone numbers, names
 * and (optional) image locations
 */
gboolean populateData() {
	GError *error=NULL;
	
	GFile *data = g_file_new_for_path(dataLocation);
	
	GFileIOStream *stream = g_file_open_readwrite(data, NULL, &error);
	
	GInputStream *is = g_io_stream_get_input_stream((GIOStream *)stream);

	GMarkupParseContext *context = g_markup_parse_context_new(&phoneBookParser,G_MARKUP_TREAT_CDATA_AS_TEXT, NULL, NULL);
	
	gchar *buffer = g_malloc0(256);
	
	int read = g_input_stream_read(is, buffer,256,NULL,&error);
	
	while(read > 0) {
		GError *parse_error = NULL;
		g_markup_parse_context_parse(context, buffer, read, &parse_error);
		buffer = g_malloc0(256);
		error = NULL;
		read = g_input_stream_read(is, buffer,256,NULL,&error);
	}
	
	g_markup_parse_context_end_parse(context,NULL);

	current_contacts = phone_book_parser_get_parsed_contacts_list();

	g_markup_parse_context_free(context);

	error=NULL;
	
	g_io_stream_close((GIOStream *)stream,NULL,&error);
}

gboolean updateData() {
	GError *error=NULL;
	GFile *data = g_file_new_for_path(dataLocation);
	GFileOutputStream *stream = g_file_create(data, G_FILE_CREATE_PRIVATE,NULL, &error);
	
	
}

gboolean refreshData() {
	
}

setupEnvironment() {
	const gchar *homeDir = g_getenv("HOME");
	gchar *fileName = g_strconcat(homeDir,"/.phoneBookNumbers.xml\0",NULL);
	dataLocation = g_strdup(fileName);
	fprintf(stdout,"location: %s\n",dataLocation);
	fflush(stdout);
}

GtkWidget *add_contact_to_ui(phoneBookEntry *pbe) {
	GtkWidget *entry = phone_book_entry_new();
	phone_book_entry_set_name(PHONE_BOOK_ENTRY(entry), pbe->name);
	phone_book_entry_set_number(PHONE_BOOK_ENTRY(entry), pbe->number);
	//GtkWidget *text = gtk_label_new(pbe->name);
	return entry;
}

int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv);
	
	setupEnvironment();
	
	checkForData();
	
	populateData();
	
	phoneBookWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	scroll = gtk_scrolled_window_new(NULL,NULL);

	box = gtk_vbox_new(TRUE,10);

	while(current_contacts) {
		gtk_box_pack_end_defaults(GTK_BOX(box), add_contact_to_ui(current_contacts->data));
		current_contacts = current_contacts->next;
	}

	//gtk_container_add(GTK_CONTAINER(scroll),box);

	gtk_container_add(GTK_CONTAINER(phoneBookWindow),box);

	gtk_window_resize(GTK_WINDOW(phoneBookWindow),300,400);

	g_signal_connect(phoneBookWindow,"destroy",G_CALLBACK(phoneBookExit),NULL);

	gtk_widget_show_all(phoneBookWindow);

	gtk_main();

	return(0);
}
