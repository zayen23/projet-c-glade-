#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

typedef struct {
    char nompark[100];
    int idpark;
    int nbrdispopark;
    float prixpark;
    int capacite;
    char etatpark[50];
    char typepark[50];
} Parking;

typedef struct {
    char nomagent[100];
    char idagent[100];
    char nomparking[100];
} Agent;

int Agent_exists(Agent a, const char *filename);
int ajouter(Parking p, char *parking);
void setup_treeview_columns(GtkTreeView *treeview);
void populate_treeview(GtkTreeView *treeview, const char *filename);

void setup_treeview3_columns(GtkTreeView *treeview);
void populate_treeview3(GtkTreeView *treeview3, const char *filename);
void on_btn_aff_clicked(GtkButton *button, gpointer user_data);

void on_btn_ajouterpark_clicked(GtkButton *button, gpointer user_data);

void on_btn_supp_clicked(GtkWidget *widget, gpointer data);

void remove_parking_from_file(const gchar *nompark, int idpark);

void on_btn_search_park_clicked(GtkButton *button, gpointer user_data);

void on_btn_modif_clicked(GtkButton *button, gpointer user_data);


void
on_button_confmodpark_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton1_group_changed          (GtkRadioButton  *radiobutton,
                                        gpointer         user_data);

void
on_radiobutton2_group_changed          (GtkRadioButton  *radiobutton,
                                        gpointer         user_data);

void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkbuttonconf_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data);



void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data);


///////////////////////////////////////////





void
on_RHbuttonG__rerCompteCitoyen_clicked (GtkButton *button,
                                        gpointer   user_data);

void
on_RHbuttonG__rerReservationCitoyen_clicked
                                        (GtkButton *button,
                                         gpointer   user_data);

void
on_RHbuttonAvis_ReclamationCitoyen_clicked
                                        (GtkButton *button,
                                         gpointer   user_data);

void
on_RHbuttonEnregistrerAjouterReclamation_clicked
                                        (GtkWidget *button,
                                         gpointer   user_data);


void
on_RHbuttonSupprimerReclamation_clicked
                                        (GtkButton *button,
                                         gpointer   user_data);

void
on_RHradiobuttonBasseUrgenceAjouterReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                         gpointer         user_data);

void
on_RHradiobuttonMoyenneUrgenceAjouterReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                         gpointer         user_data);

void
on_RHradiobuttonHauteUrgenceAjouterReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                         gpointer         user_data);




void
on_RHcheckbutton1_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHcheckbutton2_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHcheckbutton3_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHcheckbutton4_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHcheckbutton5_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHbuttonEnregistrementAvis_clicked  (GtkButton       *button,
                                        gpointer         user_data);



void
on_RHbuttonEnregistrementAvis_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_RHbuttonAfficherReclamation_clicked (GtkButton       *button,
                                        gpointer         user_data);




void
on_RHtreeviewAfficherReclamation_row_activated
                                        (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_RHbuttonChercherIdAfficherReclamation_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_RHbuttonChercherReclamationSuprrimer_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_RHcheckbuttonReclamationSupprimer_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHbuttonChercherIdModifierReclamation_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_RHradiobuttonBasseModifierReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHradiobuttonMoyenneModifierReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_RHbuttonEnregistrerModifierReclamation_clicked
                                        (GtkWidget       *button,
                                        gpointer         user_data);

void
on_RHradiobuttonHauteModifierReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);



void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_RHbuttonRetourWindowPrincipal_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);



#endif /* CALLBACKS_H */
