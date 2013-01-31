#include <gtk/gtk.h>

static void
save_params (GtkWidget * widget, GtkWidget * kid1)
{
  gint *kid1_txt;
  kid1_txt = (gint *) gtk_entry_get_text (GTK_ENTRY (kid1));
  g_print ("Saving params...\n");
  g_print ((const gchar *) kid1_txt);
  g_print ("\n");
}

static gboolean
delete_event (GtkWidget * widget, GdkEvent * event, gpointer data)
{
  gtk_main_quit ();
  return FALSE;
}

int
main (int argc, char **argv)
{
  GtkBuilder *builder;
  GtkWidget *window;
  GtkWidget *quit_button, *save_params_button;
  GtkWidget *kid1, *kid2;
  GError *error = NULL;

  gtk_init (&argc, &argv);

  builder = gtk_builder_new ();
  if (!gtk_builder_add_from_file (builder, "syn++-gui-gtk2.glade", &error))
    {
      g_warning ("%s", error->message);
      g_free (error);
      return (1);
    }

  /* connect widgets */
  window = GTK_WIDGET (gtk_builder_get_object (builder, "syn++-gui_window"));
  quit_button = GTK_WIDGET (gtk_builder_get_object (builder, "quit_button"));
  save_params_button =
    GTK_WIDGET (gtk_builder_get_object (builder, "save_params_button"));
  kid1 = GTK_WIDGET (gtk_builder_get_object (builder, "s01_kurucz_id_entry"));
  kid2 = GTK_WIDGET (gtk_builder_get_object (builder, "s02_kurucz_id_entry"));

  gtk_builder_connect_signals (builder, NULL);
  /* destroy builder after we've connected all the widgets */
  g_object_unref (G_OBJECT (builder));

  g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
  g_signal_connect (quit_button, "clicked", G_CALLBACK (delete_event),
		    window);
  g_signal_connect (save_params_button, "clicked", G_CALLBACK (save_params),
		    kid1);
  g_signal_connect_swapped (quit_button, "clicked",
			    G_CALLBACK (gtk_widget_destroy), window);

  gtk_widget_show (window);

  gtk_main ();

  return (0);
}
