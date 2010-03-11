#include "circleBox.h"
#include <math.h>

typedef struct _CircleBoxPrivate CircleBoxPrivate;

enum {
	PROP_0,
	PROP_BACKGROUND,
	PROP_FOREGROUND
};

struct _CircleBoxPrivate {
	gint numIcons;
	GdkColor *background;
	GdkColor *foreground;
};

G_DEFINE_TYPE(CircleBox, circle_box, GTK_TYPE_CONTAINER);

#define CIRCLE_BOX_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CIRCLE_BOX_TYPE, CircleBoxPrivate))

// padding for the circle
#define PADDING 64
// opacity percentage
#define OPACITY 50

// prototypes
void circle_box_size_request(GtkWidget *, GtkRequisition *);
void circle_box_size_allocate(GtkWidget *, GtkAllocation *);
void circle_box_add(GtkContainer *, GtkWidget *);
void circle_box_remove(GtkContainer *, GtkWidget *);
void circle_box_for_all(GtkContainer *, gboolean, GtkCallback, gpointer);
void circle_box_realize(GtkWidget *);
void circle_box_unrealize(GtkWidget *);
gboolean circle_box_expose_event(GtkWidget *, GdkEventExpose *);
void circle_box_map(GtkWidget *);
void circle_box_unmap(GtkWidget *);
void _circle_box_redraw(GtkWidget *);
GList * circle_box_get_all_children(CircleBox *);
gboolean circle_box_focus(GtkWidget *, GtkDirectionType);
gboolean circle_box_focus_move(CircleBox *, GList *, GtkDirectionType);
void circle_box_set_focus_child(CircleBox *, GtkWidget *);
void circle_box_propagate_expose(CircleBox *, GtkWidget *, GdkEventExpose *);
void circle_box_resize_children(CircleBox *, GtkAllocation *);
void circle_box_set_property(GObject *, guint, const GValue *, GParamSpec *);
void circle_box_get_property(GObject *, guint, GValue *, GParamSpec *);
const GdkColor * circle_box_get_foreground(CircleBox *);
const GdkColor * circle_box_get_background(CircleBox *);
void circle_box_set_foreground(CircleBox *, const GdkColor *);
void circle_box_set_background(CircleBox *, const GdkColor *);

GtkContainerClass *parent_class = NULL;

void circle_box_class_init(CircleBoxClass *class) {
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
	GtkContainerClass *container_class = GTK_CONTAINER_CLASS (class);
	
	parent_class = g_type_class_peek_parent(class);
	
	widget_class->size_request = circle_box_size_request;
	
	widget_class->size_allocate = circle_box_size_allocate;
	
	widget_class->realize = circle_box_realize;
	
	widget_class->unrealize = circle_box_unrealize;

	widget_class->expose_event = circle_box_expose_event;
	
	widget_class->map = circle_box_map;
	
	widget_class->unmap = circle_box_unmap;
	
	widget_class->focus = circle_box_focus;
	
	container_class->forall = circle_box_for_all;
	
	container_class->add = circle_box_add;
	
	container_class->remove = circle_box_remove;
	
	object_class->set_property = circle_box_set_property;
	
	object_class->get_property = circle_box_get_property;
	
	g_object_class_install_property (object_class, PROP_BACKGROUND,
										g_param_spec_boxed("backgroundColor",
															"background-color",
															"Background color.",
															GDK_TYPE_COLOR,
															G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
	
	g_object_class_install_property (object_class, PROP_FOREGROUND,
										g_param_spec_boxed("foregroundColor",
															"foreground-color",
															"Foreground color.",
															GDK_TYPE_COLOR,
															G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
	
	g_type_class_add_private(object_class, sizeof(CircleBoxPrivate));
}

void circle_box_init(CircleBox *box) {
	box->focus_child = NULL;
	box->children = NULL;
	box->bin_window = NULL;
}

void circle_box_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	CircleBox *box;
	
	box = CIRCLE_BOX(object);
	
	switch(prop_id) {
		case PROP_BACKGROUND:
			circle_box_set_background(box, g_value_get_boxed(value));
			break;
		case PROP_FOREGROUND:
			circle_box_set_foreground(box,g_value_get_boxed(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

void circle_box_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	CircleBox *box;
	
	box = CIRCLE_BOX(object);
	
	switch (prop_id) {
		case PROP_BACKGROUND:
			g_value_set_boxed(value, circle_box_get_background(box));
			break;
		case PROP_FOREGROUND:
			g_value_set_boxed(value, circle_box_get_foreground(box));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

void circle_box_set_background(CircleBox *box, const GdkColor *bg) {
	CircleBoxPrivate *priv = CIRCLE_BOX_GET_PRIVATE(box);
	priv->background = gdk_color_copy(bg);
}

void circle_box_set_foreground(CircleBox *box, const GdkColor *fg) {
	CircleBoxPrivate *priv = CIRCLE_BOX_GET_PRIVATE(box);
	priv->foreground = gdk_color_copy(fg);
}

const GdkColor * circle_box_get_background(CircleBox *box) {
	CircleBoxPrivate *priv = CIRCLE_BOX_GET_PRIVATE(box);
	return gdk_color_copy(priv->background);
}

const GdkColor * circle_box_get_foreground(CircleBox *box) {
	CircleBoxPrivate *priv = CIRCLE_BOX_GET_PRIVATE(box);
	return gdk_color_copy(priv->foreground);
}

void circle_box_size_request(GtkWidget *widget, GtkRequisition *requisition) {
	CircleBox *box = CIRCLE_BOX(widget);
	CircleBoxPrivate *private = CIRCLE_BOX_GET_PRIVATE(box);
	CircleBoxChild *child;
	guint numChildren=0;
	GList *children;
	
	children = box->children;
	
	numChildren = g_list_length(children);
	
	gdouble radius = numChildren * 24;
	
	requisition->height = radius;
	requisition->width = radius;
}

void circle_box_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
		
	widget->allocation = *allocation;
	
	CircleBox *box = CIRCLE_BOX(widget);
	CircleBoxPrivate *private = CIRCLE_BOX_GET_PRIVATE(box);
	CircleBoxChild *child;
	
	if(GTK_WIDGET_REALIZED(widget)) {
      gdk_window_move_resize (widget->window, allocation->x, allocation->y, allocation->width, allocation->height);
      gdk_window_resize (box->bin_window, allocation->width, allocation->height);
    }
	
	circle_box_resize_children(box,allocation);

	gtk_widget_queue_draw(widget);
}

void circle_box_resize_children(CircleBox *box, GtkAllocation *allocation) {	
	GtkAllocation child_allocation;
	GList *children;
	guint numChilds=0;
	float devisions = 2 * M_PI;
	int x = 0,y = 0;
	int radius = 0.0;
	int d = 0.0;
	
	children = circle_box_get_all_children(box);
	numChilds = g_list_length(children);

	// hack to stop devide by zero :/
	if(numChilds==0) { numChilds = 1; }

	devisions = devisions / numChilds;
		
	x = (allocation->width /2);
	y = (allocation->height /2);
	
	radius = MIN(x-PADDING,y-PADDING);
	
	d = (radius)-((radius)/numChilds);
	
	child_allocation.height = (radius - d)*2;
	child_allocation.width = (radius - d)*2;

	int i=0;
	while(children) {
		int childX=0, childY=0;
		childX = x + (d-child_allocation.width/2) * cos(devisions * i);
		childY = y + (d-child_allocation.width/2) * sin(devisions * i);
		
		child_allocation.x = childX - (radius - d);
		child_allocation.y = childY - (radius - d);
		
		gtk_widget_size_allocate(children->data,&child_allocation);
		
		gtk_widget_queue_draw(children->data);
		
		i++;
		
		children = children->next;
	}
}

void circle_box_add(GtkContainer *container, GtkWidget *widget) {
	CircleBoxPrivate *private = CIRCLE_BOX_GET_PRIVATE(container);
	CircleBox *box = CIRCLE_BOX(container);
	CircleBoxChild *child;
	
	child = g_new(CircleBoxChild, 1);
	
	child->widget = widget;
	
	box->children = g_list_append(box->children, child);
	
	gtk_widget_freeze_child_notify(widget);
	
	if(GTK_WIDGET_REALIZED (widget)) {
		gtk_widget_set_parent_window(child->widget, box->bin_window);
	}
	
	gtk_widget_set_parent(widget, GTK_WIDGET(box));
	
	gtk_widget_thaw_child_notify(widget);
	fprintf(stdout,"end of add\n");
	fflush(stdout);
}

void circle_box_remove(GtkContainer *container, GtkWidget *widget) {
	CircleBox *box = CIRCLE_BOX(container);
	GList *children;
	CircleBoxChild *child;
	children = box->children;
	while(children) {
		child = children->data;
		if(child->widget == widget) {
			break;
		}
		children = children->next;
	}
	if(children) {
		gtk_widget_unparent(widget);
		box->children = g_list_remove(box->children, children);
		g_list_free_1 (children);
		g_free (child);
	}
}

void circle_box_for_all(GtkContainer *container, gboolean include_internals,
							GtkCallback callback, gpointer callback_data) {
	CircleBox *box = CIRCLE_BOX(container);
	GList *children;
	children = circle_box_get_all_children(box);
	while(children) {
		(* callback) (children->data, callback_data);
		children = children->next;
	}
}

void circle_box_realize(GtkWidget *widget) {
	CircleBox *box = CIRCLE_BOX(widget);
	gint attributes_mask;
	GdkWindowAttr attributes;
	GList *children;
	
	GTK_WIDGET_SET_FLAGS(box, GTK_REALIZED);
	
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = widget->allocation.x;
	attributes.y = widget->allocation.y;
	attributes.height = widget->allocation.height;
	attributes.width = widget->allocation.width;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual(widget);
	attributes.colormap = gtk_widget_get_colormap(widget);
	attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK;
	
	attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
	
	widget->window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
	
	gdk_window_set_back_pixmap(widget->window, NULL, FALSE);
	gdk_window_set_user_data(widget->window, widget);
	
	attributes.x = 0;
	attributes.y = 0;
	attributes.width = widget->allocation.width;
	attributes.height = widget->allocation.height;
	attributes.event_mask = (GDK_EXPOSURE_MASK | 
							GDK_SCROLL_MASK | 
							GDK_POINTER_MOTION_MASK |
							GDK_BUTTON_PRESS_MASK |
							GDK_BUTTON_RELEASE_MASK |
							GDK_KEY_PRESS_MASK |
							GDK_KEY_RELEASE_MASK) |
							gtk_widget_get_events (widget);
	
	box->bin_window = gdk_window_new(widget->window, &attributes, attributes_mask);
	
	gdk_window_set_user_data(box->bin_window, widget);
	
	widget->style = gtk_style_attach(widget->style, widget->window);
	gdk_window_set_background(box->bin_window, &widget->style->base[widget->state]);
	
	gdk_window_show(box->bin_window);

	children = circle_box_get_all_children(box);
	while(children) {
		gtk_widget_set_parent_window(children->data, box->bin_window);
		children = children->next;
	}
}

void circle_box_unrealize(GtkWidget *widget) {
	CircleBox *box = CIRCLE_BOX(widget);
	
	gdk_window_set_user_data (box->bin_window, NULL);
	
	gdk_window_destroy(box->bin_window);
	
	box->bin_window = NULL;
	
	GTK_WIDGET_CLASS (circle_box_parent_class)->unrealize (widget);
}

gboolean circle_box_expose_event(GtkWidget *widget, GdkEventExpose *event) {
	CircleBox *box = CIRCLE_BOX(widget);
	
	cairo_t *cr;	
	
	if(event->window != box->bin_window) {
		return FALSE;
	}
	
	int x,y;
	
	x = widget->allocation.width / 2;
	y = widget->allocation.height / 2;

	int radius = MIN(x-PADDING,y-PADDING);

	cr = gdk_cairo_create (box->bin_window);

	gdk_cairo_set_source_color(cr, circle_box_get_background(box));

	cairo_rectangle(cr, 0, 0, widget->allocation.width, widget->allocation.height);

	cairo_fill(cr);

	cairo_set_line_width (cr, 2.);
	
	gdk_cairo_set_source_color(cr, circle_box_get_foreground(box));
	
	cairo_arc(cr, x, y, radius,0,2*M_PI);
	
	cairo_fill_preserve(cr);
	
	cairo_set_source_rgb(cr,0,0,0);
	
	cairo_stroke(cr);
	
	cairo_destroy(cr);
	
	GTK_WIDGET_CLASS (circle_box_parent_class)->expose_event (widget, event);
	
	return TRUE;
}

void circle_box_map(GtkWidget *widget) {
	CircleBox *box = CIRCLE_BOX(widget);
	GList *children=NULL;
	
	GTK_WIDGET_SET_FLAGS (widget, GTK_MAPPED);
	
	children = circle_box_get_all_children(box);
	while(children) {
		if(GTK_WIDGET_VISIBLE(children->data)) {
			if(!GTK_WIDGET_MAPPED(children->data)) {
				gtk_widget_map(children->data);
			}
		}
		children = children->next;
	}
	
	gdk_window_show(box->bin_window);
	gdk_window_show(widget->window);
}

void circle_box_unmap(GtkWidget *widget) {
	CircleBox *box = CIRCLE_BOX(widget);
	GList *children=NULL;
	
	GTK_WIDGET_UNSET_FLAGS(widget, GTK_MAPPED);
	
	children = circle_box_get_all_children(box);
	while(children) {
		if(GTK_WIDGET_VISIBLE(children->data)) {
			if(GTK_WIDGET_MAPPED(children->data)) {
				gtk_widget_unmap(children->data);
			}
		}
		children = children->next;
	}
	
	gdk_window_hide(box->bin_window);
	gdk_window_hide(widget->window);
	
}

GList * circle_box_get_all_children(CircleBox *box) {
	g_return_val_if_fail(CIRCLE_BOX_IS_CIRCLE_BOX(box), NULL);
	
	GList *f = NULL;
	GList *children = NULL;
	CircleBoxChild *child;
	
	children = box->children;
	while(children) {
		child = children->data;
		children = children->next;
		if(child->widget) {
			f=g_list_append(f, child->widget);
		}
	}
	
	return f;
}

gboolean circle_box_focus(GtkWidget *widget, GtkDirectionType direction) {
	GList *children=NULL;
	CircleBox *box = CIRCLE_BOX(widget);
	gboolean return_val;
	
	g_return_val_if_fail(CIRCLE_BOX_IS_CIRCLE_BOX(widget),FALSE);
	
	children = circle_box_get_all_children(box);
	
	return_val = FALSE;
	
	if(direction==GTK_DIR_TAB_FORWARD) {
		return_val = circle_box_focus_move(box, children, direction);
	}
	
	if(direction==GTK_DIR_TAB_BACKWARD) {
		children = g_list_reverse(children);
		return_val = circle_box_focus_move(box, children, direction);
	}
	
	g_list_free(children);
	if(!return_val) {
		box->focus_child=NULL;
	}
	return return_val;
}

gboolean circle_box_focus_move(CircleBox *box, GList *children, GtkDirectionType direction) {
	GtkWidget *focus_child;
	GtkWidget *child;
	focus_child = box->focus_child;
	while(children) {
		child = children->data;
		children = children->next;
		if(!child) {
			continue;
		}
		if(focus_child) {
			if(focus_child == child) {
				focus_child = NULL;
				if(gtk_widget_child_focus(child, direction)) {
					circle_box_set_focus_child(box, child);
					return TRUE;
				}
			}
		} else if(GTK_WIDGET_DRAWABLE(child) && gtk_widget_is_ancestor(child, GTK_WIDGET(box))) {
			if(gtk_widget_child_focus(child,direction)) {
				circle_box_set_focus_child(box, child);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void circle_box_set_focus_child(CircleBox *box, GtkWidget *child) {
	g_return_if_fail(CIRCLE_BOX_IS_CIRCLE_BOX(box));
	g_return_if_fail(child==NULL || GTK_IS_WIDGET(child));
	
	if(child != box->focus_child && child!=NULL) {
		if(box->focus_child) {
			g_object_unref(box->focus_child);
		}
		box->focus_child = child;
		if(box->focus_child) {
			g_object_ref(box->focus_child);
		}
	}
	
}

GtkWidget * circle_box_get_focus_child(GtkContainer *container) {
	CircleBox *box = CIRCLE_BOX(container);
	return box->focus_child;
}

GtkWidget * circle_box_new(GdkColor *background, GdkColor *foreground) {
	return g_object_new(CIRCLE_BOX_TYPE, "backgroundColor", background, "foregroundColor", foreground, NULL);
}

GtkWidget * circle_box_new_with_defaults(void) {
	GdkColor fg, bg;
	gdk_color_parse("#f2f",&fg);
	gdk_color_parse("#f9f",&bg);
	return circle_box_new(&bg,&fg);
}
