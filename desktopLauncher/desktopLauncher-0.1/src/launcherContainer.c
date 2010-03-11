#include <gtk/gtk.h>
#include <math.h>
#include <unistd.h>

#include "launcherContainer.h"

/* overrride the default mechanism for retrieving data private to a GtkWidget instance
 * to use data for the launcher implementation
 */
#define LAUNCHER_CONTAINER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), LAUNCHER_CONTAINER_TYPE, launcherContainerPrivate))

/* type definition for casting to the custom widget from GtkWidget
 */
G_DEFINE_TYPE (LauncherContainer, launcher_container, GTK_TYPE_LAYOUT);


/* data that will be private to a widget instance
 */
typedef struct _launcherContainerPrivate launcherContainerPrivate;

struct _launcherContainerPrivate {
	GList *children;
	int numChilds;
};

/* PROTOTYPES */
static gboolean launcher_container_expose_event(GtkWidget *, GdkEventExpose *);

/* this is called when an instance reference to the calls is created
 * it sets up the object types and private data to the widget
 * and sets the correct methods to override default event implementations
 */
static void launcher_container_class_init(LauncherContainerClass *class) {
	GObjectClass *obj_class;
	GtkWidgetClass *widget_class;
	
	obj_class = G_OBJECT_CLASS(class);
	widget_class = GTK_WIDGET_CLASS(class);
	
	widget_class->expose_event = launcher_container_expose_event;
	
	//widget_class->resize_event = launcher_container_resize_event;
	
	g_type_class_add_private(obj_class, sizeof(launcherContainerPrivate));
	
}


static void launcher_container_init(LauncherContainer *launcherContainer) {
	
	gtk_widget_add_events(GTK_WIDGET(launcherContainer), GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
	
}


static void draw(GtkWidget *destinationWidget, cairo_t *cr) {
	launcherContainerPrivate *priv = LAUNCHER_CONTAINER_GET_PRIVATE(destinationWidget);
	GList *launchers = priv->children;
	
	gdouble x, y;
	x = destinationWidget->allocation.x + destinationWidget->allocation.width / 2;
	y = destinationWidget->allocation.y + destinationWidget->allocation.height / 2;

	gdouble radius;
	radius = MIN(x, y) - 5;

	gdouble icon_scale = destinationWidget->allocation.height / ((gdouble)priv->numChilds / 2.0);
	
	cairo_arc (cr, x, y, radius, 0, 2 * M_PI);
	
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_stroke (cr);
}

/* this is the event when the widget is exposed
 * for rendering, it calls the draw method to render the widget
 */
gboolean launcher_container_expose_event(GtkWidget *widget, GdkEventExpose *event) {

	launcherContainerPrivate *priv = LAUNCHER_CONTAINER_GET_PRIVATE(widget);
	
	cairo_t *cr;
	
	cr = gdk_cairo_create(GTK_LAYOUT(widget)->bin_window);
	
	cairo_rectangle (cr, event->area.x, event->area.y, event->area.width, event->area.height);
	
	cairo_clip (cr);

	draw (widget, cr);

	cairo_destroy(cr);
	
	return FALSE;
}

/* create a new launcher widget
 */
GtkWidget * launcher_container_new(void) {
	return g_object_new(LAUNCHER_CONTAINER_TYPE, NULL);
}
