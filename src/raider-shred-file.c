#include <gtk/gtk.h>
#include "raider-file-row.h"
#include "raider-shred-file.h"

void launch (GtkWidget *widget, gpointer data)
{
    g_thread_pool_push(data, widget, NULL);
}

/*
 * THE BIG FUNCTION
 *
 * This is called when the user clicks on the Shred button in the options pane.
 */
void shred_file(GtkWidget *widget, gpointer data)
{
    RaiderWindow *window = RAIDER_WINDOW(data);

    /* Clear the subtitle. */
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(window->header_bar), NULL);

    GThreadPool *pool = g_thread_pool_new(shredding_thread, data, 10, FALSE, NULL);
    gtk_container_forall(GTK_CONTAINER(window->list_box), launch, pool);
}

void increment_number_of_subprocesses_finished(GPid pid, gint status, gpointer data)
{
    RaiderWindow *window = RAIDER_WINDOW(data);
    static int how_many_done = 0;

    how_many_done++;
    if (how_many_done == window->loaded_file_count)
    {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                            GTK_DIALOG_DESTROY_WITH_PARENT,
                            GTK_MESSAGE_INFO,
                            GTK_BUTTONS_OK,
                            "Shredding finished");

        gtk_window_set_title(GTK_WINDOW(dialog), "Message");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    g_spawn_close_pid(pid);
}
