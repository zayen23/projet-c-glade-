#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"

int main(int argc, char *argv[]) {
    GtkWidget *Parking,*RHwindowAvis_Reclamation, *RHwindowParky;

#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

    gtk_set_locale();
    gtk_init(&argc, &argv);

    add_pixmap_directory(PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

    /*
     * The following code was added by Glade to create one of each component
     * (except popup menus), just so that you see something after building
     * the project. Delete any components that you don't want shown initially.
     */
 /*   Parking = create_Parking();
    gtk_widget_show(Parking);*/
 RHwindowParky = create_RHwindowParky ();
  gtk_widget_show (RHwindowParky);
/*  RHwindowAvis_Reclamation = create_RHwindowAvis_Reclamation ();
  gtk_widget_show (RHwindowAvis_Reclamation);*/

    gtk_main();
    return 0;
}
