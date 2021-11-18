#include <stdio.h>
#include <gtk/gtk.h>
 
struct windows {
	GtkWidget *window[2];
	int current;
};
 
static void hide_wind(GtkButton *dummy, gpointer wins);
static void open_dialog(GtkWidget* button, gpointer window);
 
gboolean destroy (GtkWidget *window);
 
int main (int argc, char *argv[]){
    GtkWidget *window, *button, *grid, *button2;
	struct windows windows;
	char *title, *caption;
	int i, height, width;
 
    gtk_init (&argc, &argv);
	for (i = 0; i < 2; i++) {
		windows.current = i;
		windows.window[i] = window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        grid = gtk_grid_new ();
		gtk_container_add (GTK_CONTAINER (window), grid);
        if (i == 0)
        {
            title = "import page";
            width = 300;
            height = 300;
            caption = "Import image";
        }
        else
        {
            title = "Solver page";
            width = 1280;
            height = 1040;
            caption = "Back to import page";
        }

		gtk_window_set_title (GTK_WINDOW (window), title);
		gtk_container_set_border_width (GTK_CONTAINER (window), 10);
		gtk_widget_set_size_request (window, width, height);
		gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
        
		button = gtk_button_new_with_label (caption);
		g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK(hide_wind), &windows);
        //g_signal_connect (G_OBJECT (button), "open-location", G_CALLBACK(open-location), &windows);
		
        gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
        

        if (i == 0)
        {
            gtk_grid_attach(GTK_GRID(grid), button, 2, 2, 10, 10);
            button2 = gtk_button_new_with_label("Click me");
            g_signal_connect(button, "clicked", G_CALLBACK(open_dialog), window);
            gtk_grid_attach(GTK_GRID(grid), button, 10, 10, 10, 10);
        }
        else
        {
            gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 3, 3);
        }
	}
	windows.current = 0;
    gtk_widget_show_all (windows.window[0]);
    gtk_main ();
    return 0;
}
 
static void hide_wind(GtkButton *dummy, gpointer wins) {
	struct windows *windows = wins;
 
    (void)dummy;
	gtk_widget_hide(windows->window[windows->current]);
	windows->current = !windows->current;
	gtk_widget_show_all(windows->window[windows->current]);
}
 
gboolean destroy (GtkWidget *widget){
    gtk_widget_destroy (widget);
    return TRUE;
}

/*static void dialog_result(GtkWidget *dialog, gint resp, gpointer data)
{
    if (resp == GTK_RESPONSE_OK)

    else
        gtk_widget_destroy(dialog);
}*/

static void open_dialog(GtkWidget* button, gpointer window)
{
    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new("Choose a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_show_all(dialog);
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK)
        g_print("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    else
        g_print("You pressed the cancel\n");
    gtk_widget_destroy(dialog);
}



/*#include <gtk/gtk.h>

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);

    //variables declaration
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget* grid = gtk_grid_new();
    GtkWidget* button1 = gtk_button_new_with_label("button1");
    GtkWidget* button2 = gtk_button_new_with_label("button2");
    //GtkWidget* img = gtk_image_new_from_file("img/test1.jpg");

    //window properties
    gtk_window_set_title(GTK_WINDOW(window), "OCR Sudoku");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 1024);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    //gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    //grid properties
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_attach(GTK_GRID(grid), img, 0, 0, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), button2, 2, 2, 40, 40);
    


    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}*/