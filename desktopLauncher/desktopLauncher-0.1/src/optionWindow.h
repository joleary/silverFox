#ifndef __OPTION_WINDOW__
#define __OPTION_WINDOW__

#include <gtk/gtkwindow.h>

G_BEGIN_DECLS


#define OPTION_WINDOW_TYPE						(optionWindow_get_type ())
#define OPTION_WINDOW(obj)						(G_TYPE_CHECK_INSTANCE_CAST ((obj), OPTION_WINDOW_TYPE, OptionWindow))
#define OPTION_WINDOW_CLASS(obj)					(G_TYPE_CHECK_CLASS_CAST ((obj), OPTION_WINDOW,  OptionWindowClass))
#define OPTION_WINDOW_IS_OPTION_WINDOW(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), OPTION_WINDOW_TYPE))
#define OPTION_WINDOW_IS_OPTION_WINDOW_CLASS(obj)		(G_TYPE_CHECK_CLASS_TYPE ((obj), OPTION_WINDOW_TYPE))
#define OPTION_WINDOW_GET_CLASS					(G_TYPE_INSTANCE_GET_CLASS ((obj), OPTION_WINDOW_TYPE, OptionWindowClass))


typedef struct _OptionWindow OptionWindow;
typedef struct _OptionWindowClass OptionWindowClass;

struct _OptionWindow {
	GtkWindow parent;

	/* private */
	
};

struct _OptionWindowClass {
	GtkWindowClass parent_class;
};

GtkWidget * option_window_new();

G_END_DECLS

#endif
