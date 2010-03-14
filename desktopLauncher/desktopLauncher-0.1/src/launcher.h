
#ifndef __LAUNCHER__
#define __LAUNCHER__

#include <gtk/gtkbutton.h>

G_BEGIN_DECLS


#define LAUNCHER_TYPE						(launcher_get_type ())
#define LAUNCHER(obj)						(G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_TYPE, Launcher))
#define LAUNCHER_CLASS(obj)					(G_TYPE_CHECK_CLASS_CAST ((obj), LAUNCHER,  LauncherClass))
#define LAUNCHER_IS_LAUNCHER(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_TYPE))
#define LAUNCHER_IS_LAUNCHER_CLASS(obj)		(G_TYPE_CHECK_CLASS_TYPE ((obj), LAUNCHER_TYPE))
#define LAUNCHER_GET_CLASS					(G_TYPE_INSTANCE_GET_CLASS ((obj), LAUNCHER_TYPE, LauncherClass))


typedef struct _Launcher Launcher;
typedef struct _LauncherClass LauncherClass;

struct _Launcher {
	GtkButton parent;

	GdkWindow *GSEAL (event_window);

	gchar *GSEAL (label_text);

	gchar *GSEAL (image_path);

	gchar *GSEAL (action);

	/* private */
	
	
	
};

struct _LauncherClass {
	GtkButtonClass parent_class;
};

GtkWidget *launcher_new(const gchar *, const gchar *, const gchar *);

const gchar * launcher_get_action(Launcher *);

const gchar * launcher_get_name(Launcher *);

GAppInfo * launcher_get_app_launcher(Launcher *);
G_END_DECLS

#endif
