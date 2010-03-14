#include <gtk/gtkbutton.h>
#include <gtk/gtkprivate.h>
#include <math.h>
#include <unistd.h>

#include "launcher.h"
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>
enum {
	PROP_0,
	PROP_IMAGE_PATH,
	PROP_LABEL_TEXT,
	PROP_ACTION
};

#define LAUNCHER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), LAUNCHER_TYPE, launcherPrivate))

G_DEFINE_TYPE (Launcher, launcher, GTK_TYPE_BUTTON);

typedef struct _launcherPrivate launcherPrivate;

struct _launcherPrivate {
	gboolean focused;
	cairo_surface_t *image;
};

gboolean launcher_expose_event(GtkWidget *, GdkEventExpose *);
void launcher_get_property(GObject *, guint, GValue *, GParamSpec *);
void launcher_set_property(GObject *, guint, const GValue *, GParamSpec *);
void launcher_set_image_path(Launcher *, const gchar *);
void launcher_set_label_text(Launcher *, const gchar *);
void launcher_set_action(Launcher *, const gchar *);
void launcher_clicked(GtkButton *);

cairo_surface_t * scale_icon(cairo_surface_t *, gdouble, gdouble);
cairo_surface_t * scale_svg(const gchar *, gdouble, gdouble);

void launcher_class_init(LauncherClass *class) {
	GObjectClass *obj_class = G_OBJECT_CLASS(class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);
	GtkButtonClass *button_class = GTK_BUTTON_CLASS(class);	
	
	obj_class->set_property = launcher_set_property;
	
	obj_class->get_property = launcher_get_property;
	
	
	widget_class->expose_event = launcher_expose_event;
		
	g_object_class_install_property (obj_class, PROP_IMAGE_PATH,
										g_param_spec_string("image_path",
															"Image Path",
															"Path to the image to render on the launcher",
															NULL,
															G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
	
	g_object_class_install_property (obj_class, PROP_LABEL_TEXT,
										g_param_spec_string("label_text",
															"Label Text",
															"Text to render under the launcher image",
															NULL,
															G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
	
	g_object_class_install_property (obj_class, PROP_ACTION,
										g_param_spec_string("action",
															"Action",
															"Command line action to execute",
															NULL,
															G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
	
	g_type_class_add_private(obj_class, sizeof(launcherPrivate));
	
}

void launcher_init(Launcher *launcher) {
	launcherPrivate *priv = LAUNCHER_GET_PRIVATE(launcher);
	
	GTK_WIDGET_SET_FLAGS (launcher, GTK_CAN_FOCUS |  GTK_RECEIVES_DEFAULT);
	GTK_WIDGET_SET_FLAGS (launcher, GTK_NO_WINDOW);
	
	priv->focused = FALSE;
	
}

gboolean launcher_expose_event(GtkWidget *widget, GdkEventExpose *event) {
	Launcher *l = LAUNCHER(widget);
	launcherPrivate *priv = LAUNCHER_GET_PRIVATE(widget);
	cairo_text_extents_t extents;
	cairo_t *cr;
	
	cr = gdk_cairo_create(widget->window);

	cairo_select_font_face(cr,"Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	
	cairo_set_font_size(cr,widget->allocation.height*0.2);

	cairo_text_extents(cr, l->label_text, &extents);

	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

	cairo_move_to(cr,(widget->allocation.x+widget->allocation.width*0.5)-(extents.width/2),widget->allocation.y+widget->allocation.height*0.9);
	
	cairo_show_text(cr,l->label_text);

	cairo_stroke(cr);

	cairo_set_source_rgb(cr,1,1,1);
	
	cairo_rectangle (cr, widget->allocation.x, widget->allocation.y, 
							widget->allocation.width, widget->allocation.height*0.6);

	cairo_clip (cr);
	
	cairo_fill(cr);
	
	if(priv->image!=NULL) {
		cairo_set_source_surface(cr, scale_icon(priv->image,widget->allocation.width*0.6, widget->allocation.height*0.6), widget->allocation.x+(widget->allocation.width*0.2), widget->allocation.y);
	}
	
	cairo_paint(cr);

	cairo_destroy(cr);
	
	if(GTK_WIDGET_HAS_FOCUS(widget)) {
		gtk_paint_focus(widget->style, widget->window, 
								GTK_STATE_SELECTED,
								NULL, NULL, NULL,
								widget->allocation.x,
								widget->allocation.y,
								widget->allocation.width,
								widget->allocation.height);
	}
	
	return FALSE;
}

void launcher_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	Launcher *l;
	
	l = LAUNCHER(object);
	
	switch(prop_id) {
		case PROP_IMAGE_PATH:
			launcher_set_image_path(l, g_value_get_string(value));
			break;
		case PROP_LABEL_TEXT:
			launcher_set_label_text(l,g_value_get_string(value));
			break;
		case PROP_ACTION:
			launcher_set_action(l,g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

void launcher_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	Launcher *l;
	
	l = LAUNCHER(object);
	
	switch (prop_id) {
		case PROP_IMAGE_PATH:
			g_value_set_string(value, l->image_path);
			break;
		case PROP_LABEL_TEXT:
			g_value_set_string(value, l->label_text);
			break;
		case PROP_ACTION:
			g_value_set_string(value, l->action);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

void launcher_set_image_path(Launcher *launcher, const gchar *image_path) {
	
	g_return_if_fail(LAUNCHER_IS_LAUNCHER(launcher));
	launcherPrivate *priv = LAUNCHER_GET_PRIVATE(launcher);
	GError *error = NULL;
	cairo_surface_t *surface=NULL;
	gchar *mime=NULL;
	
	GFile *ico = g_file_new_for_path(image_path);
	GFileInfo *info = g_file_query_info(ico, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, &error);
	
	if(error==NULL) {
		mime = g_file_info_get_attribute_as_string(info,"standard::content-type");
	}
	
	if(mime!=NULL) {
		if(g_strcmp0(mime,"image/png")==0) {
			surface = cairo_image_surface_create_from_png(image_path);
		}
		
		if(g_strcmp0(mime,"image/svg+xml")==0) {
			surface = scale_svg(image_path, 256.0, 256.0);
		}
		
		if(surface!=NULL && cairo_surface_status(surface)==CAIRO_STATUS_SUCCESS)	{
			priv->image = surface;
		} else {
			priv->image = NULL;
		}
	}
	
	launcher->image_path = g_strdup(image_path);
}

void launcher_set_label_text(Launcher *launcher, const gchar *label_text) {
	
	g_return_if_fail(LAUNCHER_IS_LAUNCHER(launcher));
	
	launcher->label_text = g_strdup(label_text);
}

void launcher_set_action(Launcher *launcher, const gchar *action) {
	
	g_return_if_fail(LAUNCHER_IS_LAUNCHER(launcher));
	
	launcher->action = g_strdup(action);
}

cairo_surface_t * scale_icon(cairo_surface_t *oldSurface, gdouble newW, gdouble newH) {
	cairo_surface_t *newSurface = cairo_surface_create_similar(oldSurface, CAIRO_CONTENT_COLOR_ALPHA, newW, newH);

	cairo_t *ct = cairo_create(newSurface);
	
	cairo_scale(ct, (gdouble)newW / cairo_image_surface_get_width(oldSurface) , (gdouble)newH / cairo_image_surface_get_height(oldSurface));
	
	cairo_set_source_surface(ct, oldSurface, 0, 0);
	
	cairo_pattern_set_extend (cairo_get_source(ct), CAIRO_EXTEND_REFLECT); 

	cairo_set_operator (ct, CAIRO_OPERATOR_SOURCE);

	cairo_paint (ct);
   
    cairo_destroy (ct);
	
	return newSurface;
}

cairo_surface_t * scale_svg(const gchar *svg_path, gdouble width, gdouble height) {
	rsvg_init();
	GError *rsvg_error=NULL;
	cairo_surface_t *target_surface;
	cairo_t *cr;
	gdouble scalew, scaleh, stride;
	int dw, dh;
	RsvgHandle *rsvg_handle;
	RsvgDimensionData dimension_data;
	
	rsvg_handle = rsvg_handle_new_from_file(svg_path, &rsvg_error);
	if(rsvg_error==NULL) {
		rsvg_handle_get_dimensions(rsvg_handle, &dimension_data);
		dw = dimension_data.width;
		dh = dimension_data.height;
		scalew = width / dw;
		scaleh = height / dh;
		stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
		void *image = g_malloc0(stride * height);
		target_surface = cairo_image_surface_create_for_data(image,
														CAIRO_FORMAT_ARGB32, 
														width, height, stride);
		cairo_t *cr = cairo_create(target_surface);
		cairo_scale(cr,scalew,scaleh);
						
		if(rsvg_handle_render_cairo(rsvg_handle, cr)) {
			rsvg_term();
			return target_surface;
		}
			
	}
	rsvg_term();
	return NULL;
}

GtkWidget *launcher_new(const gchar *text, const gchar *image, const gchar *action) {
	return g_object_new(LAUNCHER_TYPE, "label_text", text, "image_path", image, "action", action, NULL);
}

const gchar * launcher_get_action(Launcher *l) {
	return l->action;
}

const gchar * launcher_get_name(Launcher *l) {
	return l->label_text;
}

GAppInfo * launcher_get_app_launcher(Launcher *l) {
	GError *error;
	GAppInfo *app_info = g_app_info_create_from_commandline(l->action,l->label_text,G_APP_INFO_CREATE_NONE, &error);
	return app_info;
}
