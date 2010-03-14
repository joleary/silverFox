#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xatom.h>

#include "launcher.h"
#include "circleBox.h"
#include "configParser.h"
#include "optionWindow.h"

GtkWidget *rootWindow;

GtkWidget *oWindow;
GtkWidget *oButton;

GdkScreen *gdkScreen;

GtkWidget *lContainer;

GtkWidget *box, *box2;

GList *launchers = NULL;

gchar *dataLocation;

GdkColor fg;
GdkColor bg;
	
gboolean launch_application(GAppInfo *appinfo) {

	GdkAppLaunchContext *context;
	gboolean retval;
	GError *error;
	
	g_return_val_if_fail(G_IS_APP_INFO(appinfo),FALSE);
	
	context  = gdk_app_launch_context_new();
	gdk_app_launch_context_set_screen(context,gdkScreen);
	gdk_app_launch_context_set_timestamp(context,gtk_get_current_event_time());

	retval = g_app_info_launch_uris(appinfo, NULL, (GAppLaunchContext *) context, &error);

	g_object_unref(context);
	
	return retval;
}

void quitting(GtkWidget *widget, GdkEvent *event) {
	gtk_main_quit();
}

void clicked(GtkWidget *widget, GdkEvent * event) {
	g_return_if_fail(LAUNCHER_IS_LAUNCHER(widget));
	GAppInfo *appinfo = launcher_get_app_launcher(LAUNCHER(widget));
	launch_application(appinfo);
}

/* alter the window to be a desktop type window
 */
void set_desktop_hint(GdkWindow *window) {
	GdkAtom atom;

	atom = gdk_atom_intern ("_NET_WM_WINDOW_TYPE_DESKTOP", FALSE);

	gdk_property_change (window,
			     gdk_atom_intern ("_NET_WM_WINDOW_TYPE", FALSE),
			     gdk_x11_xatom_to_atom (XA_ATOM), 32,
			     GDK_PROP_MODE_REPLACE, (guchar *) &atom, 1);
	
}

/* force the root window uid 
 */
void set_as_root(GtkWidget *widget, GdkWindow *window) {

	Window window_xid;

	GdkWindow *root_window;

	root_window = gdk_screen_get_root_window (gtk_window_get_screen(GTK_WINDOW(widget)));

	window_xid = GDK_WINDOW_XWINDOW (window);

	gdk_property_change (root_window,
			     gdk_atom_intern ("DESKTOPLAUNCHER_DESKTOP_WINDOW_ID", FALSE),
			     gdk_x11_xatom_to_atom (XA_WINDOW), 32,
			     GDK_PROP_MODE_REPLACE, (guchar *) &window_xid, 1);
	
}

static void setup(GtkWidget *widget) {
	
	set_desktop_hint(widget->window);
	
	set_as_root(widget, widget->window);
	
}

void parse_config() {
	GError *error=NULL;
	
	GFile *data = g_file_new_for_path(dataLocation);
	
	GFileIOStream *stream = g_file_open_readwrite(data, NULL, &error);
	
	GInputStream *is = g_io_stream_get_input_stream((GIOStream *)stream);

	GMarkupParseContext *context = g_markup_parse_context_new(&configParser, 
													G_MARKUP_TREAT_CDATA_AS_TEXT, 
													NULL, 
													NULL);
	
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

	launchers = config_parser_get_launchers();

	gchar *fgColor;
	gchar *bgColor;

	fgColor = config_parser_get_fg();
	
	bgColor = config_parser_get_bg();
	
	if(!gdk_color_parse(fgColor, &fg)) {
		gdk_color_parse("#fff", &fg);
	}
	if(!gdk_color_parse(bgColor, &bg)) {
		gdk_color_parse("#000",&bg);
	}

	g_markup_parse_context_free(context);

	error=NULL;
	
	g_io_stream_close((GIOStream *)stream,NULL,&error);
}

void apply_config() {

	while(launchers) {
		launcherEntry *l = launchers->data;

		GtkWidget *newLauncher = launcher_new(l->name, l->icon, l->action);
		
		g_signal_connect(G_OBJECT(newLauncher),"clicked",G_CALLBACK(clicked),NULL);
		
		gtk_container_add(GTK_CONTAINER(lContainer), newLauncher);
		
		launchers = launchers->next;
	}
	
	circle_box_set_background(CIRCLE_BOX(lContainer),&bg);
	circle_box_set_foreground(CIRCLE_BOX(lContainer),&fg);
	
}

void toggleOptions(GtkWidget *widget, GdkEvent *event) {
	if(!gtk_widget_get_visible(oWindow)) {
		gtk_widget_show_all(oWindow);
	} else {
		gtk_widget_hide_all(oWindow);
	}
	
	GdkColor bgC;
		
		if(gdk_color_parse("#33f", &bgC)) {
				gtk_widget_modify_bg(oWindow,GTK_STATE_NORMAL,&bgC);
		}
}


int main(int argc, char *argv[]) {
	
	gtk_init(&argc, &argv);
	
	dataLocation = g_strdup("config.xml");
	
	parse_config();
	
	GdkWindow *gdkRootWindow;

	gdkRootWindow = gdk_get_default_root_window();

	box = gtk_vbox_new(FALSE,0);

	box2 = gtk_hbox_new(FALSE,0);
	
	lContainer = circle_box_new_with_defaults();
			
	rootWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_type_hint(GTK_WINDOW(rootWindow), GDK_WINDOW_TYPE_HINT_DESKTOP);

	oWindow = option_window_new();
	
	oButton = gtk_button_new_with_mnemonic("&Add");

	gdkScreen = gtk_window_get_screen(GTK_WINDOW(rootWindow));

	//gtk_window_resize(GTK_WINDOW(rootWindow),gdk_screen_get_width(gdkScreen),gdk_screen_get_height(gdkScreen));
	gtk_window_fullscreen(GTK_WINDOW(rootWindow));
	gtk_window_resize(GTK_WINDOW(rootWindow),400,400);
	
	gtk_window_set_position(GTK_WINDOW(rootWindow),GTK_WIN_POS_CENTER_ALWAYS);


	gtk_box_pack_start_defaults(GTK_BOX(box), lContainer);

	//gtk_box_pack_end(GTK_BOX(box), box2, FALSE, FALSE, 0);

	//gtk_box_pack_end(GTK_BOX(box2), oButton, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(rootWindow), box);
	
	g_signal_connect(rootWindow, "destroy", G_CALLBACK(quitting), NULL);

	g_signal_connect(oButton, "clicked", G_CALLBACK(toggleOptions), NULL);

	//setup(rootWindow);

	apply_config();

	gtk_widget_show_all(rootWindow);

	gtk_main();

	return 0;
}
