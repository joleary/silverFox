#ifndef __CIRCLE_BOX_H__
#define __CIRCLE_BOX_H__


#include <gtk/gtklayout.h>


G_BEGIN_DECLS


#define CIRCLE_BOX_TYPE (circle_box_get_type ())
#define CIRCLE_BOX(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), CIRCLE_BOX_TYPE, CircleBox))
#define CIRCLE_BOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CIRCLE_BOX_TYPE, CircleBoxClass))
#define CIRCLE_BOX_IS_CIRCLE_BOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CIRCLE_BOX_TYPE))
#define CIRCLE_BOX_IS__CIRCLE_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CIRCLE_BOX_TYPE))
#define CIRCLE_BOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CIRCLE_BOX_TYPE, CircleBoxClass))

typedef struct _CircleBox	CircleBox;
typedef struct _CircleBoxClass	CircleBoxClass;
typedef struct _CircleBoxChild	CircleBoxChild;

struct _CircleBox
{
  GtkContainer GSEAL (container);
  GList *GSEAL (children);
  GtkWidget *GSEAL (focus_child);
  /*< public >*/
  GdkWindow *GSEAL (bin_window);
  
};

struct _CircleBoxClass
{
  GtkContainerClass parent_class;
};

struct _CircleBoxChild
{
  GtkWidget *widget;
};

GtkWidget * circle_box_new_with_defaults(void);

GtkWidget * circle_box_new(GdkColor *, GdkColor *);

void circle_box_set_background(CircleBox *, const GdkColor *);

void circle_box_set_foreground(CircleBox *, const GdkColor *);

GList * circle_box_get_all_children(CircleBox *);

G_END_DECLS

#endif
