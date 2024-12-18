#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "greeting.h"


#define MAX_AGENTS 100
#define MAX_PARKINGS 100



int generate_unique_id1(char *parking_file) {
    FILE *file = fopen(parking_file, "r");
    int max_id = 0;
    char line[256];
    
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            Parking temp;
            sscanf(line, "%99[^;];%d;%d;%d;%e;%99[^;];%99[^\n]",
                   temp.nompark, &temp.idpark,&temp.capacite ,&temp.nbrdispopark, &temp.prixpark,
                   temp.etatpark, temp.typepark);
            if (temp.idpark > max_id) {
                max_id = temp.idpark;
            }
        }
        fclose(file);
    }
    return max_id + 1; // Generate the next unique ID
}

int ajouter(Parking p, char *parking_file) {
    FILE *file = fopen(parking_file, "a");
    if (file != NULL) {
        fprintf(file, "%s;%d;%d;%d;%.2f;%s;%s\n",
                p.nompark, p.idpark,p.capacite, p.nbrdispopark, p.prixpark,p.etatpark, p.typepark);
        fclose(file);
        return 1;
    }
    return 0;
}


int n,y;

void setup_treeview_columns(GtkTreeView *treeview) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes("Nom Parking", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Parking", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("CAPACITE", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Nombre Dispo", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Prix", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Etat", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(treeview, column);
}

void populate_treeview(GtkTreeView *treeview, const char *filename) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *file = fopen(filename, "r");
    char line[256];

    store = gtk_list_store_new(7, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT,G_TYPE_INT ,G_TYPE_FLOAT, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(store));

    while (fgets(line, sizeof(line), file)) {
        Parking p;
        sscanf(line, "%99[^;];%d;%d;%d;%e;%99[^;];%99[^\n]",
               p.nompark, &p.idpark, &p.capacite ,&p.nbrdispopark, &p.prixpark,p.etatpark, p.typepark);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, p.nompark,
                           1, p.idpark,
                           2, p.capacite,
                           3, p.nbrdispopark,
                           4, p.prixpark,
                           5, p.etatpark,
                           6, p.typepark,
                           -1);
    }

    fclose(file);
}

void on_btn_aff_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview1");

    setup_treeview_columns(GTK_TREE_VIEW(treeview));
    populate_treeview(GTK_TREE_VIEW(treeview), "parking.txt");
}

void on_btn_ajouterpark_clicked(GtkButton *button, gpointer user_data) {
    Parking p;
    GtkWidget *entry_nompark,*entry_capacite , *entry_nbrdispopark, *entry_prixpark;
    GtkWidget *combobox_etatpark, *combobox_typepark;
    GtkWidget *message, *label_message;
    char fichier[] = "parking.txt";
    char msg[] = "Parking ajouté avec succès"; // Success message
    FILE *file;
    char line[256];
    int id_exists = 0;

    entry_nompark = lookup_widget(GTK_WIDGET(button), "entry_nompark");
    entry_capacite = lookup_widget(GTK_WIDGET(button), "entry_capacite");
    entry_nbrdispopark = lookup_widget(GTK_WIDGET(button), "entry_nbrdispopark");
    entry_prixpark = lookup_widget(GTK_WIDGET(button), "entry_prixpark");
    combobox_etatpark = lookup_widget(GTK_WIDGET(button), "combobox_etatpark");
    combobox_typepark = lookup_widget(GTK_WIDGET(button), "combobox_typepark");
    message = lookup_widget(GTK_WIDGET(button), "labelmsg");
    label_message = lookup_widget(GTK_WIDGET(button), "label_message");

    strcpy(p.nompark, gtk_entry_get_text(GTK_ENTRY(entry_nompark)));
    p.capacite = atoi(gtk_entry_get_text(GTK_ENTRY(entry_capacite)));
    p.nbrdispopark = atoi(gtk_entry_get_text(GTK_ENTRY(entry_nbrdispopark)));
    p.prixpark = atoi(gtk_entry_get_text(GTK_ENTRY(entry_prixpark)));

    // Automatically generate the unique ID
    p.idpark = generate_unique_id1(fichier);

    if (combobox_etatpark != NULL) {
        gchar *etat = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_etatpark));
        if (etat != NULL) {
            strcpy(p.etatpark, etat);
            g_free(etat);
        } else {
            strcpy(p.etatpark, "N/A");
        }
    }

    if (combobox_typepark != NULL) {
        gchar *type = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_typepark));
        if (type != NULL) {
            strcpy(p.typepark, type);
            g_free(type);
        } else {
            strcpy(p.typepark, "N/A");
        }
    }

    // Add parking to file
    if (y==1){
    if (ajouter(p, fichier)) {
        gtk_label_set_text(GTK_LABEL(label_message), msg); // Set success message to label_message
        gtk_label_set_text(GTK_LABEL(message), "");

        gtk_entry_set_text(GTK_ENTRY(entry_nompark), "");
        gtk_entry_set_text(GTK_ENTRY(entry_capacite), "");
        gtk_entry_set_text(GTK_ENTRY(entry_nbrdispopark), "");
        gtk_entry_set_text(GTK_ENTRY(entry_prixpark), "");
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_etatpark), -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_typepark), -1);
    } else {
        gtk_label_set_text(GTK_LABEL(message), "Erreur lors de l'enregistrement");
    }}
}

void on_btn_supp_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *treeview = lookup_widget(widget, "treeview1");
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkListStore *store;
    gchar *nompark;
    int idpark;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &nompark, 1, &idpark, -1);

        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Are you sure you want to delete the parking '%s' with ID %d?",
                                                   nompark, idpark);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES) {
            store = GTK_LIST_STORE(model);
            gtk_list_store_remove(store, &iter);

            remove_parking_from_file(nompark, idpark);
        }

        g_free(nompark);
    }
}

void remove_parking_from_file(const gchar *nompark, int idpark) {
    FILE *file = fopen("parking.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char line[256];
    Parking p;

    if (file == NULL || temp_file == NULL) {
        return;
    }

    // Read the original file line by line
    while (fgets(line, sizeof(line), file)) {
         sscanf(line, "%99[^;];%d;%d;%d;%e;%99[^;];%99[^\n]",
                   p.nompark, &p.idpark,&p.capacite, &p.nbrdispopark, &p.prixpark,
                   p.etatpark, p.typepark);

        // Skip the line that matches the parking to delete
        if (strcmp(nompark, p.nompark) != 0 || idpark != p.idpark) {
            fprintf(temp_file, "%s;%d;%d;%d;%.2f;%s;%s\n",
                p.nompark, p.idpark, p.capacite ,p.nbrdispopark,   p.prixpark,p.etatpark, p.typepark);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Remove the original file and rename the temp file
    remove("parking.txt");
    rename("temp.txt", "parking.txt");
}

void on_btn_search_park_clicked(GtkButton *button, gpointer user_data) {
    const char *search_term;
    GtkWidget *entry_search_park, *dialog, *label;
    FILE *file = fopen("parking.txt", "r");
    char line[256];
    Parking p;
    int found = 0; // Flag to check if we find the parking ID

    // Get the ID entered in the entry widget
    entry_search_park = lookup_widget(GTK_WIDGET(button), "entry_search_park");
    if (entry_search_park == NULL) {
        g_warning("Widget 'entry_search_park' not found.");
        return;
    }
    
    search_term = gtk_entry_get_text(GTK_ENTRY(entry_search_park));
    
    // Open the parking file for reading
    if (file == NULL) {
        g_warning("Failed to open 'parking.txt'.");
        return;
    }

    // Search for the parking information based on the entered ID
    while (fgets(line, sizeof(line), file)) {
         sscanf(line, "%99[^;];%d;%d;%d;%e;%99[^;];%99[^\n]",
                   p.nompark, &p.idpark,&p.capacite,&p.nbrdispopark, &p.prixpark,p.etatpark, p.typepark);

        if (p.idpark == atoi(search_term)) {
            // If parking is found, display the information
            found = 1;
            break;
        }
    }
    fclose(file);

    // Create a dialog to show the search result
    if (found) {
        dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_INFO,
                                        GTK_BUTTONS_OK,
                                        "Parking found:\ \n \nName:%s \nID:%d \nnbrdispopark:%d \nPrix:%.2f \nEtat:%s \nType:%s ",
                                        p.nompark, p.idpark, p.nbrdispopark, p.prixpark, p.etatpark, p.typepark);


        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Parking with ID %s not found.",
                                        search_term);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}
void on_btn_modif_clicked(GtkButton *button, gpointer user_data)
{
      GtkWidget *entry_search_park = lookup_widget(GTK_WIDGET(button), "entry_search_park");
    GtkWidget *entry_nompark = lookup_widget(GTK_WIDGET(button), "entry_nompark");
    GtkWidget *entry_idpark = lookup_widget(GTK_WIDGET(button), "entry_idpark");
    GtkWidget *entry_capacite = lookup_widget(GTK_WIDGET(button), "entry_capacite");
    GtkWidget *entry_nbrdispopark = lookup_widget(GTK_WIDGET(button), "entry_nbrdispopark");
    GtkWidget *entry_prixpark = lookup_widget(GTK_WIDGET(button), "entry_prixpark");
    GtkWidget *notebook = lookup_widget(GTK_WIDGET(button), "notebook1");

    const char *id_search = gtk_entry_get_text(GTK_ENTRY(entry_search_park));
    FILE *file = fopen("parking.txt", "r");
    if (!file) return;

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char nom[100], id[10], capacite[20] ,dispo[10], prix[10], type[50], etat[50];
        sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
               nom, id,capacite,dispo, prix, type, etat);

        if (strcmp(id, id_search) == 0) {
            gtk_entry_set_text(GTK_ENTRY(entry_nompark), nom);
            gtk_entry_set_text(GTK_ENTRY(entry_idpark), id);
            gtk_entry_set_text(GTK_ENTRY(entry_capacite), capacite);
            gtk_entry_set_text(GTK_ENTRY(entry_nbrdispopark), dispo);
            gtk_entry_set_text(GTK_ENTRY(entry_prixpark), prix);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        // Optionally, display a message to indicate no matching parking was found.
    }

    // Navigate to the first tab of notebook1
    if (notebook != NULL) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
    }
} 

void
on_button_confmodpark_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{


    GtkWidget *entry_search_park = lookup_widget(GTK_WIDGET(button), "entry_search_park");
    GtkWidget *entry_nompark = lookup_widget(GTK_WIDGET(button), "entry_nompark");
    GtkWidget *entry_idpark = lookup_widget(GTK_WIDGET(button), "entry_idpark");
    GtkWidget *entry_capacite = lookup_widget(GTK_WIDGET(button), "entry_capacite");
    GtkWidget *entry_nbrdispopark = lookup_widget(GTK_WIDGET(button), "entry_nbrdispopark");
    GtkWidget *entry_prixpark = lookup_widget(GTK_WIDGET(button), "entry_prixpark");
    GtkWidget *combobox_typepark = lookup_widget(GTK_WIDGET(button), "combobox_typepark");
    GtkWidget *combobox_etatpark = lookup_widget(GTK_WIDGET(button), "combobox_etatpark");
    
    

    const char *id_search = gtk_entry_get_text(GTK_ENTRY(entry_search_park));
    const char *new_nom = gtk_entry_get_text(GTK_ENTRY(entry_nompark));
    const char *new_id = gtk_entry_get_text(GTK_ENTRY(entry_idpark));
    const char *new_cap = gtk_entry_get_text(GTK_ENTRY(entry_capacite));
    const char *new_dispo = gtk_entry_get_text(GTK_ENTRY(entry_nbrdispopark));
    const char *new_prix = gtk_entry_get_text(GTK_ENTRY(entry_prixpark));

    GtkComboBox *combo_box_type = GTK_COMBO_BOX(combobox_typepark);
    GtkComboBox *combo_box_etat = GTK_COMBO_BOX(combobox_etatpark);

    // Get the active index of the combo boxes
    gint active_type = gtk_combo_box_get_active(combo_box_type);
    gint active_etat = gtk_combo_box_get_active(combo_box_etat);

    // If the combo box has valid items, retrieve the active text
    const char *new_type = (active_type != -1) ? gtk_combo_box_get_active_text(combo_box_type) : NULL;
    const char *new_etat = (active_etat != -1) ? gtk_combo_box_get_active_text(combo_box_etat) : NULL;
    FILE *file = fopen("parking.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) return;

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char nom[100], id[10], capacite[100] ,dispo[100], prix[10], type[50], etat[50];
        sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
               nom, id,capacite,dispo, prix, type, etat);

        if (strcmp(id, id_search) == 0) {
            fprintf(temp, "%s;%s;%s;%s;%s;%s;%s\n",
                    new_nom, new_id, new_cap ,new_dispo, new_prix, new_type, new_etat);
            found = 1;
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);
    if (found) {
        remove("parking.txt");
        rename("temp.txt", "parking.txt");
    } else {
        remove("temp.txt");
        // Optionally, notify the user that no matching parking was found.
    }
    // Clear the input fields after successful modification
  
     
       

    gtk_entry_set_text(GTK_ENTRY(entry_nompark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_idpark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_capacite), "");
    gtk_entry_set_text(GTK_ENTRY(entry_nbrdispopark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_prixpark), "");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_etatpark), -1);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_typepark), -1);

}

void
on_checkbuttonconf_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
{y=1;}
}

void setup_treeview3_columns(GtkTreeView *treeview3) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Remove any existing columns
    gtk_tree_view_remove_column(treeview3, gtk_tree_view_get_column(treeview3, 0));
    gtk_tree_view_remove_column(treeview3, gtk_tree_view_get_column(treeview3, 1));
    gtk_tree_view_remove_column(treeview3, gtk_tree_view_get_column(treeview3, 2));

    // Renderer for text
    renderer = gtk_cell_renderer_text_new();

    // Column for Nom Agent
    column = gtk_tree_view_column_new_with_attributes("Nom Agent", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(treeview3, column);

    // Column for ID Agent
    column = gtk_tree_view_column_new_with_attributes("ID Agent", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(treeview3, column);

    // Column for Nom Parking
    column = gtk_tree_view_column_new_with_attributes("Nom Parking", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(treeview3, column);
}





  
void populate_treeview3(GtkTreeView *treeview3, const char *filename) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *file = fopen(filename, "r");
    char line[256];

    if (file == NULL) {
        g_print("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    // Create a new GtkListStore with 3 columns: STRING, STRING, STRING
    store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    // Read each line from the file
    while (fgets(line, sizeof(line), file)) {
        Agent a;
        // Parse the line into the Agent structure
        sscanf(line, "%99[^;];%99[^;];%99[^\n]",
               a.nomagent, a.idagent, a.nomparking);

        // Append data to the list store
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, a.nomagent,
                           1, a.idagent,
                           2, a.nomparking,
                           -1);
    }

    fclose(file);

    // Set the list store as the model for the treeview
    gtk_tree_view_set_model(treeview3, GTK_TREE_MODEL(store));

    // Unref the list store to reduce memory leaks
    g_object_unref(store);
}


void on_button2_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview3 = lookup_widget(GTK_WIDGET(button), "treeview3");

    // Set up columns
    setup_treeview3_columns(GTK_TREE_VIEW(treeview3));

    // Populate the treeview with data
    populate_treeview3(GTK_TREE_VIEW(treeview3), "agent.txt");
}


void on_button1_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_nom_agent = lookup_widget(GTK_WIDGET(button), "entry_nom_agent");
    GtkWidget *entry_id_agent = lookup_widget(GTK_WIDGET(button), "entry_id_agent");  
    GtkWidget *entry_nom_parking = lookup_widget(GTK_WIDGET(button), "entry_nom_parking");
    

    FILE *file = fopen("agent.txt", "a");
    if (file == NULL) {
        g_print("Erreur: Impossible d'ouvrir le fichier agent.txt\n");
        return;
    }

    // Créer un objet Agent pour stocker les données saisies
    Agent a;

    // Lire les entrées de l'utilisateur
    const gchar *nomagent_input = gtk_entry_get_text(GTK_ENTRY(entry_nom_agent));
    const gchar *idagent_input = gtk_entry_get_text(GTK_ENTRY(entry_id_agent));    
    const gchar *nomparking_input = gtk_entry_get_text(GTK_ENTRY(entry_nom_parking));
    
    // Copier les données dans l'objet Agent
    strncpy(a.nomagent, nomagent_input, sizeof(a.nomagent) - 1);
    a.nomagent[sizeof(a.nomagent) - 1] = '\0';
    strncpy(a.idagent, idagent_input, sizeof(a.idagent) - 1);
    a.nomparking[sizeof(a.nomparking) - 1] = '\0';
    strncpy(a.nomparking, nomparking_input, sizeof(a.nomparking) - 1);
    a.nomparking[sizeof(a.nomparking) - 1] = '\0';
    

    // Ajouter les données dans le fichier
    fprintf(file, "%s;%s;%s\n", a.nomagent, a.idagent, a.nomparking);

    // Fermer le fichier
    fclose(file);

     // Effacer les champs d'entrée
     gtk_entry_set_text(GTK_ENTRY(entry_nom_agent), "");
     gtk_entry_set_text(GTK_ENTRY(entry_id_agent), "");    
     gtk_entry_set_text(GTK_ENTRY(entry_nom_parking), "");
   
  
}


/////////////////////////////////////////////////////////




int urgenceLevel,urgenceLevelModif, z, c,g,l,p;		// Changed from Urgence

GtkWidget *global_checkbutton_sujet = NULL; // Checkbutton pour afficher le sujet
char global_id_unique[50] = "";            // ID unique de la réclamation trouvée
char global_sujet[100] = "";               // Sujet de la réclamation trouvée
int checkbutton_state = 0;                 // État du CheckButton (0 = décoché, 1 = coché)


void
on_RHbuttonG__rerCompteCitoyen_clicked (GtkButton * button,
					gpointer user_data)
{
  // Implementation for this callback (if any)
}

void
on_RHbuttonG__rerReservationCitoyen_clicked (GtkButton * button,
					     gpointer user_data)
{

 GtkWidget *Parking, *windowParky;
windowParky=lookup_widget(button,"RHwindowParky");
gtk_widget_destroy(windowParky);
Parking=create_Parking();
gtk_widget_show (Parking);
}

void
on_RHbuttonAvis_ReclamationCitoyen_clicked (GtkButton * button,
					    gpointer user_data)
{
GtkWidget *windowparky, *windowRec;
windowparky=lookup_widget(button,"RHwindowParky");
gtk_widget_destroy(windowparky);
windowRec=create_RHwindowAvis_Reclamation ();
gtk_widget_show (windowRec);
}

void
on_RHbuttonEnregistrerAjouterReclamation_clicked (GtkWidget * button,
						  gpointer user_data)
{ 
      GtkWidget *parent_window = GTK_WIDGET(user_data);
    const char fichier[] = "reclamation.txt";
    Reclamation r;
    GtkWidget *Id, *comboNom, *comboSujet, *descr, *Cin, *message, *dialog;
    GtkWidget *windowajout, *windowavis;
    windowajout = create_RHwindowAvis_Reclamation();

    char msg[100] = "Votre réclamation a été bien ajoutée ! Merci pour votre confiance.";
    char error_msg[512] = ""; // Message d'erreur combiné
    gboolean valid = TRUE;    // Flag pour vérifier la validité des champs

    Id = lookup_widget(button, "RHentryIdClientAjouterReclamation");
    comboNom = lookup_widget(button, "RHcomboboxNomParkingAjouterReclamation");
    comboSujet = lookup_widget(button, "RHcomboboxSujetAjouterReclamation");
    descr = lookup_widget(button, "RHentryDescriptionAjouterReclamation");
    Cin = lookup_widget(button, "RHentryCINAjouterReclamation");
    message = lookup_widget(button, "RHlabelMsgEnregistrementAjouterReclamation");

    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(Id));
    const gchar *cin_text = gtk_entry_get_text(GTK_ENTRY(Cin));
    const gchar *descr_text = gtk_entry_get_text(GTK_ENTRY(descr));

    // Validation de l'ID (doit contenir uniquement des chiffres)
    for (int i = 0; id_text[i] != '\0'; i++) {
        if (!g_ascii_isdigit(id_text[i])) {
            valid = FALSE;
            strcat(error_msg, "- L'ID doit contenir uniquement des chiffres.\n");
            break;
        }
    }

    // Validation du CIN (exactement 8 chiffres)
    if (strlen(cin_text) != 8) {
        valid = FALSE;
        strcat(error_msg, "- Le CIN doit contenir exactement 8 chiffres.\n");
    } else {
        for (int i = 0; cin_text[i] != '\0'; i++) {
            if (!g_ascii_isdigit(cin_text[i])) {
                valid = FALSE;
                strcat(error_msg, "- Le CIN ne doit contenir que des chiffres.\n");
                break;
            }
        }
    }

    // Validation des autres champs
    if (strlen(descr_text) == 0) {
        valid = FALSE;
        strcat(error_msg, "- La description doit être remplie.\n");
    }
    if (gtk_combo_box_get_active(GTK_COMBO_BOX(comboNom)) == -1) {
        valid = FALSE;
        strcat(error_msg, "- Veuillez sélectionner un parking.\n");
    }
    if (gtk_combo_box_get_active(GTK_COMBO_BOX(comboSujet)) == -1) {
        valid = FALSE;
        strcat(error_msg, "- Veuillez sélectionner un sujet.\n");
    }

    // Afficher une boîte de dialogue unique si des erreurs sont détectées
    if (!valid) {
        dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_WARNING,
                                        GTK_BUTTONS_OK,
                                        "Les erreurs suivantes ont été détectées :\n%s",
                                        error_msg);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Si tout est valide, remplir la structure
    strcpy(r.ID, id_text);
    r.CIN = atoi(cin_text);
    strcpy(r.Description_du_probleme, descr_text);

    gchar *selected_nom = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboNom));
    gchar *selected_sujet = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboSujet));

    if (selected_nom != NULL) {
        strcpy(r.Nom_du_parking, selected_nom);
        g_free(selected_nom);
    }

    if (selected_sujet != NULL) {
        strcpy(r.Sujet, selected_sujet);
        g_free(selected_sujet);
    }

    // Ajouter le niveau d'urgence en fonction de `urgenceLevel`
    if (urgenceLevel == 1) {
        strcpy(r.Urgence, "Basse");
    } else if (urgenceLevel == 2) {
        strcpy(r.Urgence, "Moyenne");
    } else if (urgenceLevel == 3) {
        strcpy(r.Urgence, "Haute");
    } else {
        // Gérer le cas où aucun niveau d'urgence n'est sélectionné
        dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "Veuillez sélectionner un niveau d'urgence.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Appeler la fonction d'ajout et afficher un message de confirmation ou d'erreur
    if (Ajouter(fichier, r, parent_window)) {
        gtk_label_set_text(GTK_LABEL(message), msg);
        gtk_entry_set_text(GTK_ENTRY(Id), "");
        gtk_entry_set_text(GTK_ENTRY(descr), "");
        gtk_entry_set_text(GTK_ENTRY(Cin), "");
        gtk_combo_box_set_active(GTK_COMBO_BOX(comboNom), -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(comboSujet), -1);
    } else {
        gtk_label_set_text(GTK_LABEL(message), "Erreur lors de l'ajout de la réclamation.");
    }
}



      void on_RHbuttonSupprimerReclamation_clicked (GtkButton * button,
						    gpointer user_data)
      {

 GtkWidget *message= lookup_widget(button,"RHlabelMessageConfirmationSupprimerReclamation");
    // Vérifier si le CheckButton est coché
    if (checkbutton_state == 0) {
        g_warning("Veuillez cocher le sujet avant de supprimer.");
        return;
    }

    // Créer une boîte de dialogue de confirmation
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Confirmer la suppression",
        NULL,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_Oui", GTK_RESPONSE_YES,
        "_Non", GTK_RESPONSE_NO,
        NULL);
    
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Êtes-vous sûr de vouloir supprimer cette réclamation ?");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(dialog);

    // Attendre la réponse de l'utilisateur
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Si l'utilisateur confirme la suppression
    if (response == GTK_RESPONSE_YES) {
        // Ouvrir les fichiers pour lire et écrire
        FILE *file = fopen("reclamation.txt", "r");
        FILE *temp_file = fopen("temp.txt", "w");
        if (!file || !temp_file) {
            g_warning("Erreur : Impossible d'ouvrir les fichiers.");
            if (file) fclose(file);
            if (temp_file) fclose(temp_file);
            return;
        }

        char line[512];
        Reclamation r;
        int deleted = 0;

        // Lire et écrire toutes les lignes sauf celle à supprimer
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%49[^;];%99[^;];%99[^;];%199[^;];%d;%49[^;];%19[^;];%49[^\n]",
                   r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, &r.CIN, r.Urgence, r.date, r.ID_Unique);

            if (strcmp(r.ID_Unique, global_id_unique) != 0) {
                fputs(line, temp_file); // Copier dans le fichier temporaire
            } else {
                deleted = 1; // Marquer comme supprimé
            }
        }

        fclose(file);
        fclose(temp_file);

        if (deleted) {
            gtk_label_set_text (GTK_LABEL (message), "Reclamation supprimé avec succées!");
            remove("reclamation.txt");
            rename("temp.txt", "reclamation.txt");
            g_print("Réclamation supprimée avec succès.");
        } else {
            remove("temp.txt");
            g_warning("Aucune réclamation n'a été supprimée.");
        }

        // Réinitialiser le CheckButton
        gtk_widget_set_sensitive(global_checkbutton_sujet, FALSE);
        gtk_button_set_label(GTK_BUTTON(global_checkbutton_sujet), ""); // Réinitialiser le label
        checkbutton_state = 0; // Réinitialiser l'état

        // Réinitialiser le champ ID
        GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "RHentryIdReclamationSupprimer");
        gtk_entry_set_text(GTK_ENTRY(entry_id), "");

        // Réinitialiser les SpinButtons
        GtkWidget *spin_jour = lookup_widget(GTK_WIDGET(button), "RHspinbuttonJourSupprimerReclamation");
        GtkWidget *spin_mois = lookup_widget(GTK_WIDGET(button), "RHspinbuttonMoisSupprimerReclamation");
        GtkWidget *spin_annee = lookup_widget(GTK_WIDGET(button), "RHspinbuttonAnneeSupprimerReclamation");

        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_jour), 31);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_mois), 12);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_annee), 2024); // Vous pouvez ajuster l'année par défaut selon votre besoin
    } else {
        // L'utilisateur a annulé la suppression
        g_print("Suppression annulée.");
    }
     }





      void
	on_RHradiobuttonBasseUrgenceAjouterReclamation_toggled
	(GtkToggleButton * togglebutton, gpointer user_data)
      {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton)))
	  {
	    urgenceLevel = 1;
	  }
      }

      void
	on_RHradiobuttonMoyenneUrgenceAjouterReclamation_toggled
	(GtkToggleButton * togglebutton, gpointer user_data)
      {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton)))
	  {
	    urgenceLevel = 2;
	  }
      }

      void
	on_RHradiobuttonHauteUrgenceAjouterReclamation_toggled
	(GtkToggleButton * togglebutton, gpointer user_data)
      {
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton)))
	  {
	    urgenceLevel = 3;
	  }
      }

      int av[] = { 0, 0, 0, 0, 0 };
      void
	on_RHcheckbutton1_toggled (GtkToggleButton * togglebutton,
				   gpointer user_data)
      {
	if (gtk_toggle_button_get_active (togglebutton))
	  {
	    av[0] = 1;
	  }
	else
	    {av[0] = 0;}

      }


      void
	on_RHcheckbutton2_toggled (GtkToggleButton * togglebutton,
				   gpointer user_data)
      {
	if (gtk_toggle_button_get_active (togglebutton))
	  {
	    av[1] = 1;
	  }
	else
	    av[1] = 0;
      }


      void
	on_RHcheckbutton3_toggled (GtkToggleButton * togglebutton,
				   gpointer user_data)
      {
	if (gtk_toggle_button_get_active (togglebutton))
	  {
	    av[2] = 1;
	  }
	else
	    av[2] = 0;
      }


      void
	on_RHcheckbutton4_toggled (GtkToggleButton * togglebutton,
				   gpointer user_data)
      {
	if (gtk_toggle_button_get_active (togglebutton))
	  {
	    av[3] = 1;
	  }
	else
	    av[3] = 0;
      }


      void
	on_RHcheckbutton5_toggled (GtkToggleButton * togglebutton,
				   gpointer user_data)
      {
	if (gtk_toggle_button_get_active (togglebutton))
	  {
	    av[4] = 1;
	  }
	else
	  av[4] = 0;
      }


      void
	on_RHbuttonEnregistrementAvis_clicked (GtkButton * button,
					       gpointer user_data)
      { 
    const char fich[] = "avis.txt";
    
    Reclamation a;
    GtkWidget *window1, *id, *combonom, *message, *dialog;
    char msg[] = "Votre avis a été enregistré avec succès!";
    
    window1 = create_RHwindowAvis_Reclamation();
    id = lookup_widget(button, "RHentryIdClientAvisReclamation");
    combonom = lookup_widget(button, "RHcomboboxNomParkingAvisReclamation");
    message = lookup_widget(button, "RHlabelMsgEnregistrementAvis");

    // Obtenir le texte du champ d'entrée pour l'ID
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(id));
    
    // Vérifier si l'ID contient uniquement des chiffres
    gboolean is_numeric = TRUE;
    for (int i = 0; id_text[i] != '\0'; i++) {
        if (!g_ascii_isdigit(id_text[i])) { // Vérifie si chaque caractère est un chiffre
            is_numeric = FALSE;
            break;
        }
    }

    // Vérifier que tous les champs sont remplis
    if (strlen(id_text) == 0 || combonom == NULL || gtk_combo_box_get_active(combonom) == -1 || !is_numeric) {
        // Créer et afficher une boîte de dialogue informant l'utilisateur de remplir tous les champs
        dialog = gtk_message_dialog_new(GTK_WINDOW(window1),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_WARNING,
                                        GTK_BUTTONS_OK,
                                        "Veuillez remplir tous les champs.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);  // Détruire la boîte de dialogue après utilisation

        // Arrêter le traitement si les champs ne sont pas remplis
        return;
    }

    // Si l'ID est valide, continuer
    strcpy(a.ID, id_text);

    // Vérifier l'avis sélectionné
    if (av[0] == 1)
        strcpy(a.Avis, "Très Insatisfait");
    else if (av[1] == 1)
        strcpy(a.Avis, "Insatisfait");
    else if (av[2] == 1)
        strcpy(a.Avis, "Neutre");
    else if (av[3] == 1)
        strcpy(a.Avis, "Satisfait");
    else if (av[4] == 1)
        strcpy(a.Avis, "Très Satisfait");

    // Vérifier le nom du parking sélectionné
    if (combonom != NULL) {
        gchar *selected_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combonom));

        if (selected_text != NULL) {
            g_print("Nom sélectionné : %s\n", selected_text);
            strcpy(a.Nom_du_parking, selected_text);
            g_free(selected_text);
        }
    }

    // Enregistrer l'avis
    if (avis_rec(fich, a)) {
        gtk_label_set_text(GTK_LABEL(message), msg);
    } else {
        gtk_label_set_text(GTK_LABEL(message), "Erreur lors de l'ajout de l'avis.");
    }

    // Réinitialiser les champs
    gtk_combo_box_set_active(GTK_COMBO_BOX(combonom), -1);
    gtk_entry_set_text(GTK_ENTRY(id), "");
}


void
on_RHbuttonAfficherReclamation_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
          const char *search_term;
    GtkWidget *entry_search_rec, *treeview;
    FILE *file;
    char line[256];
    Reclamation r;
    int found = 0; // Indicateur pour vérifier si une réclamation a été trouvée
    GtkListStore *list_store;
    GtkTreeIter iter;

    // Récupérer l'entrée de l'utilisateur
    entry_search_rec = lookup_widget(GTK_WIDGET(button), "RHentryIdClientAfficherReclamation");
    if (entry_search_rec == NULL) {
        g_warning("Widget 'entry_search_rec' not found.");
        return;
    }
    search_term = gtk_entry_get_text(GTK_ENTRY(entry_search_rec));
    
    // Vérifier si l'utilisateur a saisi un ID
    if (strlen(search_term) == 0) {
        g_warning("No ID entered.");
        return;
    }

    // Ouvrir le fichier pour lire les réclamations
    file = fopen("reclamation.txt", "r");
    if (file == NULL) {
        g_warning("Failed to open 'reclamation.txt'.");
        return;
    }

    // Récupérer le TreeView où afficher les données
    treeview = lookup_widget(GTK_WIDGET(button), "RHtreeviewAfficherReclamation");
    if (treeview == NULL) {
        g_warning("TreeView 'RHtreeviewAfficherReclamation' not found.");
        fclose(file);
        return;
    }

    // Créer un GtkListStore pour stocker les données filtrées
    list_store = gtk_list_store_new(8, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    // Parcourir le fichier pour trouver les réclamations correspondant à l'ID
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%99[^;];%99[^;];%99[^;];%99[^;];%d;%99[^;];%99[^;];%99[^\n]", 
               r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, &r.CIN, r.Urgence, r.date, r.ID_Unique);

        // Vérifier si l'ID correspond à l'ID recherché
        if (strcmp(r.ID, search_term) == 0) {
            // Ajouter les données correspondantes au GtkListStore
            gtk_list_store_append(list_store, &iter);
            gtk_list_store_set(list_store, &iter, 
                               0, r.ID,
                               1, r.Nom_du_parking,
                               2, r.Sujet,
                               3, r.Description_du_probleme,
                               4, r.CIN,
                               5, r.Urgence,
                               6, r.date,
                               7, r.ID_Unique,
                               -1);
            found = 1; // Réclamation trouvée
        }
    }
    // Fermer le fichier après la lecture
    fclose(file);

    // Si aucune réclamation n'est trouvée, afficher un avertissement
    if (!found) {
        g_warning("No reclamation found with ID '%s'.", search_term);
        return;
    }

 // Associer le GtkListStore au GtkTreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));

    // Configurer les colonnes du TreeView si ce n'est pas déjà fait
    setup_treeview_columns1(GTK_TREE_VIEW(treeview));

    // Libérer la mémoire du GtkListStore (référence détenue par le TreeView)
    g_object_unref(list_store);

}


void
on_RHtreeviewAfficherReclamation_row_activated
                                        (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{/*
 GtkTreeIter iter;
gchar *id;
gchar *nom;
gchar *sujet;
gchar *description;
gchar *cin;
gchar *urgence;
gchar *date
Reclamation r;

GtkTreeModel *model = gtk tree_view_get_mode(RHtreeviewAfficherReclamation);

if (gtk_tree_model_get_iter(model,&iter, path)) {

gtk_tree_model_get (GTK_LIST_STORE(model), &iter, 0, &id, 1, &nom, 2, &sujet, 3, &description, 4, &cin, 5, &urgence, 6, &date, -1);

strcpy(r.ID, id);
strcpy(r.Nom_du_parking, nom);
strcpy(r.Sujet, sujet);
strcpy(r.Description_du_probleme, description);
strcpy(r.CIN, cin);
strcpy(r.Urgence, urgence);
strcpy(r.date, date);*/

}


void
on_RHbuttonChercherIdAfficherReclamation_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

   const char *search_term;
    GtkWidget *entry_search_rec, *dialog, *label;
    FILE *file = fopen("reclamation.txt", "r");
    char line[256];
    Reclamation r;
    int found = 0; // Flag to check if we find the parking ID

    // Get the ID entered in the entry widget
    entry_search_rec = lookup_widget(GTK_WIDGET(button), "RHentryIdClientAfficherReclamation");
    if (entry_search_rec == NULL) {
        g_warning("Widget 'entry_search_rec' not found.");
        return;
    }
    
    search_term = gtk_entry_get_text(GTK_ENTRY(entry_search_rec));
    
    // Open the parking file for reading

    if (file == NULL) {
        g_warning("Failed to open 'reclamation.txt'.");
        return;
    }

    // Search for the parking information based on the entered ID
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%99[^;];%99[^;];%99[^;];%99[^;];%d;%99[^;];%99[^;];%99[^\n]",r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, &r.CIN, r.Urgence, r.date,r.ID_Unique);

        if (strcmp(r.ID,search_term) ==0) {
	   
            // Create the message dialog to show the parking info
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
                                            "Reclamation Found:\n\n"
                                            "ID: %s\nNom_Park: %s\nSujet: %s\nDescr_Prob: %s\nCIN: %d\nUrgence: %s\ndate: %s\nID_Unique:%s\n",  r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, r.CIN, r.Urgence, r.date,r.ID_Unique);

            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            found = 1;
            break;
        }
}



    if (!found) {
        // Create the message dialog if parking is not found
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, 
                                        "Reclamation with ID %s not found.", search_term);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    fclose(file);
}

void
on_RHbuttonChercherReclamationSuprrimer_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{  
    // Récupération des widgets de l'interface
    GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "RHentryIdReclamationSupprimer");
    GtkWidget *spin_jour = lookup_widget(GTK_WIDGET(button), "RHspinbuttonJourSupprimerReclamation");
    GtkWidget *spin_mois = lookup_widget(GTK_WIDGET(button), "RHspinbuttonMoisSupprimerReclamation");
    GtkWidget *spin_annee = lookup_widget(GTK_WIDGET(button), "RHspinbuttonAnneeSupprimerReclamation");

    // Récupération des valeurs de l'ID unique et de la date
    const char *id_unique = gtk_entry_get_text(GTK_ENTRY(entry_id));
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));

    char date_recherchee[11];
    snprintf(date_recherchee, sizeof(date_recherchee), "%02d-%02d-%04d", jour, mois, annee);

    // Ouverture du fichier de réclamations
    FILE *file = fopen("reclamation.txt", "r");
    if (!file) {
        g_warning("Erreur : Impossible d'ouvrir le fichier 'reclamation.txt'.");
        return;
    }

    char line[512];
    Reclamation r;
    int found = 0;
    char resultats[1024] = "";

    // Recherche dans le fichier
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^;];%99[^;];%99[^;];%199[^;];%d;%49[^;];%19[^;];%49[^\n]",
               r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, &r.CIN, r.Urgence, r.date, r.ID_Unique);

        if (strcmp(r.ID_Unique, id_unique) == 0 && strcmp(r.date, date_recherchee) == 0) {
            snprintf(resultats, sizeof(resultats),
                     "ID : %s\nNom du parking : %s\nSujet : %s\nDescription : %s\nCIN : %d\nUrgence : %s\nDate : %s\nID Unique : %s\n",
                     r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, r.CIN, r.Urgence, r.date, r.ID_Unique);

            strcpy(global_id_unique, r.ID_Unique); // Sauvegarde de l'ID unique trouvé
            strcpy(global_sujet, r.Sujet);         // Sauvegarde du sujet trouvé
            found = 1;
            break;
        }
    }
    fclose(file);

    // Affichage des résultats de la recherche dans une boîte de dialogue
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Résultats de la Recherche",
        NULL,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new(found ? resultats : "Aucun résultat trouvé pour cette recherche.");
    gtk_container_add(GTK_CONTAINER(content_area), label);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Si la réclamation a été trouvée, on affiche le sujet dans le checkbutton
    if (found) {
        // Trouver le CheckButton et mettre à jour son label avec le sujet de la réclamation
        global_checkbutton_sujet = lookup_widget(GTK_WIDGET(button), "RHcheckbuttonReclamationSupprimer");
        gtk_button_set_label(GTK_BUTTON(global_checkbutton_sujet), r.Sujet); // Met à jour le label du CheckButton
        gtk_widget_set_sensitive(global_checkbutton_sujet, TRUE); // Active le CheckButton
    }
}


void
on_RHcheckbuttonReclamationSupprimer_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
   if (gtk_toggle_button_get_active(togglebutton)) {
        checkbutton_state = 1; // Le CheckButton est coché
    } else {
        checkbutton_state = 0; // Le CheckButton est décoché
    }

}


void
on_RHbuttonChercherIdModifierReclamation_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

    // Récupération des widgets de l'interface
    GtkWidget *entry_search_rec = lookup_widget(GTK_WIDGET(button), "RHentryIdReclamationModifierReclamation");
    GtkWidget *label_id = lookup_widget(GTK_WIDGET(button), "RHlabelIdModifierReclamation");
    GtkWidget *Descr = lookup_widget(GTK_WIDGET(button), "RHentryDescriptionProbModifierReclamation");
    GtkWidget *cin = lookup_widget(GTK_WIDGET(button), "RHentryCinModifierReclamation");
    GtkWidget *parent_window = gtk_widget_get_toplevel(GTK_WIDGET(button));

    // Récupération de l'ID Unique à rechercher
    const char *id_search = gtk_entry_get_text(GTK_ENTRY(entry_search_rec));

    // Ouverture du fichier contenant les réclamations
    FILE *file = fopen("reclamation.txt", "r");
    if (!file) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur : Impossible d'ouvrir le fichier reclamation.txt.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char line[256];
    int found = 0;

    // Déclaration de la structure Reclamation
    Reclamation r;

    // Lecture ligne par ligne du fichier
    while (fgets(line, sizeof(line), file)) {
        // Extraction des données de la ligne lue et remplissage de la structure
        sscanf(line, "%99[^;];%99[^;];%99[^;];%99[^;];%d;%99[^;];%99[^;];%99[^\n]",
               r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, &r.CIN, r.Urgence, r.date, r.ID_Unique);

        // Comparaison de l'ID Unique avec l'ID recherché
        if (strcmp(r.ID_Unique, id_search) == 0) {
            // Si l'ID Unique est trouvé, remplir les champs d'entrée avec les données correspondantes
            gtk_label_set_text(GTK_LABEL(label_id), r.ID);
            gtk_entry_set_text(GTK_ENTRY(Descr), r.Description_du_probleme);

            // Conversion de l'entier CIN en chaîne de caractères avant de l'afficher
            char cin_str[20]; // Buffer pour le CIN sous forme de chaîne
            snprintf(cin_str, sizeof(cin_str), "%d", r.CIN); // Conversion de l'entier en chaîne
            gtk_entry_set_text(GTK_ENTRY(cin), cin_str); // Affichage dans l'entry

            found = 1;
            break;
        }
    }

    // Fermeture du fichier
    fclose(file);

    // Si aucune réclamation n'a été trouvée, afficher une boîte de dialogue
    if (!found) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "Aucune réclamation trouvée avec l'ID '%s'.\nVeuillez entrer un ID de réclamation valide.",
                                                   id_search);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


void
on_RHradiobuttonBasseModifierReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton)))
	  {
	    urgenceLevelModif = 1;
	  }
}


void
on_RHradiobuttonMoyenneModifierReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton)))
	  {
	    urgenceLevelModif = 2;
	  }
}


void
on_RHbuttonEnregistrerModifierReclamation_clicked
                                        (GtkWidget       *button,
                                        gpointer         user_data)
{
    Reclamation r;
    int modified = 0;  // Flag pour indiquer si une modification a été effectuée

    static char initial_cin[10]; // CIN initial affiché lors du chargement
    GtkWidget *label_idclient = lookup_widget(GTK_WIDGET(button), "RHlabelIdModifierReclamation");
    GtkWidget *entry_search_rec = lookup_widget(GTK_WIDGET(button), "RHentryIdReclamationModifierReclamation");
    GtkWidget *combobox_parking = lookup_widget(GTK_WIDGET(button), "RHcomboboxNomParkingModifierReclamation");
    GtkWidget *combobox_sujet = lookup_widget(GTK_WIDGET(button), "RHcomboboxSujetModifierReclamation");
    GtkWidget *entry_description = lookup_widget(GTK_WIDGET(button), "RHentryDescriptionProbModifierReclamation");
    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "RHentryCinModifierReclamation");
    GtkWidget *message = lookup_widget(GTK_WIDGET(button), "RHlabelMsgModificationReclamation");

    // Récupérer les valeurs des widgets
    const char *idclient = gtk_label_get_text(GTK_LABEL(label_idclient));
    const char *id_unique = gtk_entry_get_text(GTK_ENTRY(entry_search_rec));
    const char *description = gtk_entry_get_text(GTK_ENTRY(entry_description));
    const char *cin_str = gtk_entry_get_text(GTK_ENTRY(entry_cin));

    // Validation des champs obligatoires avec boîtes de dialogue
    if (strlen(idclient) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "L'ID client est obligatoire.\nVeuillez le remplir.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    if (strlen(id_unique) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "L'ID unique est obligatoire.\nVeuillez le remplir.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    if (strlen(description) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "La description du problème est obligatoire.\nVeuillez la remplir.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Récupérer les valeurs des combobox
    gchar *selected_parking = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_parking));
    gchar *selected_sujet = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_sujet));

    if (selected_parking == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un parking.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(selected_sujet); // Nettoyage mémoire
        return;
    }

    if (selected_sujet == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un sujet.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(selected_parking); // Nettoyage mémoire
        return;
    }

    // Si c'est la première fois que le bouton est cliqué, sauvegarder la CIN initiale
    if (strlen(initial_cin) == 0) {
        strncpy(initial_cin, cin_str, sizeof(initial_cin) - 1);
    }

    // Vérifier si la CIN a été modifiée
    if (strcmp(initial_cin, cin_str) != 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_OK,
                                                   "La CIN ne peut pas être modifiée.\nVeuillez la laisser telle quelle.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return; // Bloquer l'enregistrement
    }

    // Vérifier le niveau d'urgence
    if (urgenceLevelModif == 1) {
        strncpy(r.Urgence, "Basse", sizeof(r.Urgence) - 1);
    } else if (urgenceLevelModif == 2) {
        strncpy(r.Urgence, "Moyenne", sizeof(r.Urgence) - 1);
    } else if (urgenceLevelModif == 3) {
        strncpy(r.Urgence, "Haute", sizeof(r.Urgence) - 1);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez sélectionner un niveau d'urgence.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(selected_parking);
        g_free(selected_sujet);
        return;
    }

    // Conversion CIN
    int cin = atoi(cin_str);

    // Récupérer la date actuelle
    time_t now = time(NULL);
struct tm *local_time = localtime(&now);

// Modifier le format de la date pour qu'il soit jours-mois-annee
snprintf(r.date, sizeof(r.date), "%02d-%02d-%04d",
         local_time->tm_mday,
         local_time->tm_mon + 1,
         local_time->tm_year + 1900);

    // Remplir la structure `r`
    snprintf(r.ID, sizeof(r.ID), "%s", idclient);
    snprintf(r.ID_Unique, sizeof(r.ID_Unique), "%s", id_unique);
    snprintf(r.Nom_du_parking, sizeof(r.Nom_du_parking), "%s", selected_parking);
    snprintf(r.Sujet, sizeof(r.Sujet), "%s", selected_sujet);
    snprintf(r.Description_du_probleme, sizeof(r.Description_du_probleme), "%s", description);
    r.CIN = cin;

    // Confirmation
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(button)),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Voulez-vous vraiment enregistrer les modifications ?");
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_NO) {
        g_free(selected_parking);
        g_free(selected_sujet);
        return;
    }

    // Modifier dans le fichier
    FILE *file = fopen("reclamation.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char line[512];

    while (fgets(line, sizeof(line), file)) {
        Reclamation temp;
        sscanf(line, "%49[^;];%99[^;];%99[^;];%199[^;];%d;%49[^;];%19[^;];%49[^\n]",
               temp.ID, temp.Nom_du_parking, temp.Sujet, temp.Description_du_probleme, &temp.CIN, temp.Urgence, temp.date, temp.ID_Unique);

        if (strcmp(temp.ID_Unique, id_unique) == 0) {
            // Mettre à jour l'urgence avec la nouvelle valeur
            fprintf(temp_file, "%s;%s;%s;%s;%d;%s;%s;%s\n",
                    r.ID, r.Nom_du_parking, r.Sujet, r.Description_du_probleme, r.CIN, r.Urgence, r.date, r.ID_Unique);
            modified = 1;
        } else {
            fputs(line, temp_file);
        }
    }
    fclose(file);
    fclose(temp_file);

    if (modified) {
        remove("reclamation.txt");
        rename("temp.txt", "reclamation.txt");
        gtk_label_set_text(GTK_LABEL(message), "Réclamation modifiée avec succès.");
    }

    // Réinitialiser les champs
    gtk_label_set_text(GTK_LABEL(label_idclient), "");
    gtk_entry_set_text(GTK_ENTRY(entry_search_rec), "");
    gtk_entry_set_text(GTK_ENTRY(entry_description), "");
    gtk_entry_set_text(GTK_ENTRY(entry_cin), "");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_parking), -1);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_sujet), -1);
    initial_cin[0] = '\0';
    g_free(selected_parking);
    g_free(selected_sujet);
}

void
on_RHradiobuttonHauteModifierReclamation_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton)))
	  {
	    urgenceLevelModif = 3;
	  }
}






void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *windowparky, *windowRec;
windowparky=lookup_widget(button,"Parking");
gtk_widget_destroy(windowparky);
windowRec=create_RHwindowParky ();
gtk_widget_show (windowRec);
}

void
on_RHbuttonRetourWindowPrincipal_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{

GtkWidget *windowparky, *windowRec;
windowparky=lookup_widget(button,"RHwindowAvis_Reclamation");
gtk_widget_destroy(windowparky);
windowRec=create_RHwindowParky ();
gtk_widget_show (windowRec);
}

