#include <gtk/gtk.h>

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);

    //variables declaration
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget* fixed = gtk_fixed_new();
    GtkWidget* button1 = gtk_button_new_with_label("button1");
    GtkWidget* button2 = gtk_button_new_with_label("button2");
    GtkWidget* img = gtk_image_new_from_file("img/test1.jpg");

    //window properties
    gtk_window_set_title(GTK_WINDOW(window), "OCR Sudoku");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 1024);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    //widget properties
    gtk_widget_set_size_request(button1, 1200, 40);
    gtk_widget_set_size_request(img, 200, 200);

    //fixed properties
    gtk_container_add(GTK_CONTAINER(window), fixed);
    gtk_fixed_put(GTK_FIXED(fixed), button1, 0, 50);
    gtk_fixed_put(GTK_FIXED(fixed), button2, 100, 100);
    gtk_fixed_put(GTK_FIXED(fixed), img, 300, 300);


    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}



