#include "optionWindow.h"
#include <gdk/gdk.h>

#define OPTION_WINDOW_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), OPTION_WINDOW_TYPE, optionWindowPrivate))

G_DEFINE_TYPE (OptionWindow, optionWindow, GTK_TYPE_WINDOW);

typedef struct _optionWindowPrivate optionWindowPrivate;

struct _optionWindowPrivate {
	GdkColor *background;
};

void optionWindow_class_init(OptionWindowClass *klass) {
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	GtkWindowClass *window_class = GTK_WINDOW_CLASS(klass);
	
	g_type_class_add_private(object_class, sizeof(optionWindowPrivate));
}

void optionWindow_init(OptionWindow *optionWindow) {
	gtk_window_set_type_hint(GTK_WINDOW(optionWindow), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	gtk_window_set_decorated(GTK_WINDOW(optionWindow),FALSE);
}

void option_window_set_background(OptionWindow *op, GdkColor *bg) {
	g_return_if_fail(OPTION_WINDOW_IS_OPTION_WINDOW(op));
	
	optionWindowPrivate *priv = OPTION_WINDOW_GET_PRIVATE(op);
	
	priv->background = bg;
}


GtkWidget * option_window_new() {
	return g_object_new(OPTION_WINDOW_TYPE,NULL);
}
