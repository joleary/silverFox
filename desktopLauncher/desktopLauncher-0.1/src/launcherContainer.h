#ifndef __LAUNCHER_CONTAINER__
#define __LAUNCHER_CONTAINER__

#include <gtk/gtklayout.h>

G_BEGIN_DECLS


#define LAUNCHER_CONTAINER_TYPE						(launcher_container_get_type ())
#define LAUNCHER_CONTAINER(obj)						(G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_CONTAINER_TYPE, LauncherContainer))
#define LAUNCHER_CONTAINER_CLASS(obj)					(G_TYPE_CHECK_CLASS_CAST ((obj), LAUNCHER_CONTAINER,  LauncherContainerClass))
#define LAUNCHER_CONTAINER_IS_LAUNCHER_CONTAINER(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_CONTAINER_TYPE))
#define LAUNCHER_CONTAINER_IS_LAUNCHER_CONTAINER_CLASS(obj)		(G_TYPE_CHECK_CLASS_TYPE ((obj), LAUNCHER_CONTAINER_TYPE))
#define LAUNCHER_CONTAINER_GET_CLASS					(G_TYPE_INSTANCE_GET_CLASS ((obj), LAUNCHER_CONTAINER_TYPE, LauncherContainerClass))


typedef struct _LauncherContainer LauncherContainer;
typedef struct _LauncherContainerClass LauncherContainerClass;

struct _LauncherContainer {
	GtkLayout parent;

	/* <private> */
	
};

struct _LauncherContainerClass {
	GtkLayoutClass parent_class;
};

GtkWidget *launcher_container_new(void);


G_END_DECLS

#endif
