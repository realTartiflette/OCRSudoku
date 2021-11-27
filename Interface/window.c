#include <gtk/gtk.h>
#include <stdio.h>
#include "signal.h"
#include "ctype.h"
#include <stdlib.h>
#include <string.h>
#include "window.h"
#include "../solverSudoku/solverSudoku.h"
#include "../manipulateImage/openImage.h"
#include "../manipulateImage/grayscale.h"
#include "../manipulateImage/blur.h"
#include "../manipulateImage/threshold.h"
#include "../manipulateImage/edgeDetector.h"

// Variables
GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *stkfxd1;
GtkBuilder *builder;
GtkWidget *image1;
GtkWidget *image2;
GtkWidget *image3;
GtkWidget *image4;
GtkWidget *chooser;
GtkWidget *ViewPort;
GtkWidget *TextView;
GtkWidget *saveText;
GtkWidget *editText;
GtkTextBuffer *TextBuffer;
GtkWidget *solveButton;
GtkWidget *saveLabel;
GtkWidget *saveFile;
char tmp[1024];



int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    builder = gtk_builder_new_from_file("Interface/window.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    //signals
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
    
    //
    gtk_builder_connect_signals(builder, NULL);
    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    image2 = GTK_WIDGET(gtk_builder_get_object(builder, "image2"));
	image4 = GTK_WIDGET(gtk_builder_get_object(builder, "image4"));
    chooser = GTK_WIDGET(gtk_builder_get_object(builder, "chooser"));
	stkfxd1 = GTK_WIDGET(gtk_builder_get_object(builder, "stkfxd1"));		
	ViewPort = GTK_WIDGET(gtk_builder_get_object(builder, "ViewPort"));
	TextView = GTK_WIDGET(gtk_builder_get_object(builder, "TextView"));
	saveText = GTK_WIDGET(gtk_builder_get_object(builder, "saveText"));
	editText = GTK_WIDGET(gtk_builder_get_object(builder, "editText"));
	solveButton = GTK_WIDGET(gtk_builder_get_object(builder, "solveButton"));
	saveLabel = GTK_WIDGET(gtk_builder_get_object(builder, "saveLabel"));
	saveFile = GTK_WIDGET(gtk_builder_get_object(builder, "saveFile"));



	TextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TextView));
	g_signal_connect(TextBuffer, "changed", G_CALLBACK(on_changed_text), NULL);
    
	strcpy(tmp, "... ... ...\n... ... ...\n... ... ...\n\n... ... ...\n... ... ...\n... ... ...\n\n... ... ...\n... ... ...\n... ... ...");

	gtk_text_buffer_set_text(TextBuffer, (const gchar *) tmp, (gint) -1);
	gtk_widget_hide(saveText);
	gtk_widget_hide(saveLabel);

    // Runs the main loop.
    gtk_widget_show(window);
    image1 = NULL;
	image3 = NULL;
    gtk_main();

    // Exits.
    return 0;
}

void on_saveFile_clicked(GtkButton *b)
{
	system("cp solverSudoku/solver.c ~/Downloads/sudoku.c");
	gtk_widget_show(saveLabel);
	
}

/*void on_button3_clicked(GtkButton *b)
{
	char filename[] = "test4.jpg";
	int hor = 10;
	int ver = 100;
	if (image1)
		gtk_container_remove(GTK_CONTAINER(fixed1), image1);
	gtk_widget_hide(image2);
	image1 = gtk_image_new_from_file(filename);
	gtk_widget_show(image1);
	gtk_container_add(GTK_CONTAINER(fixed1), image1);
	gtk_fixed_move(GTK_FIXED(fixed1), image1, hor, ver);
}*/
void on_solveButton_clicked(GtkWidget *b)
{
	gtk_widget_hide(saveLabel);
	printf("pomme\n");
	solve("../solverSudoku/sudoku");
	printf("work\n");
}

void on_chooser_file_activated(GtkFileChooserButton *b)
{
	printf("ok");
}

void on_chooser_file_set(GtkWidget *b)
{
	gtk_widget_hide(saveLabel);
	gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(b));
	int len = strlen(filename);
	if (filename[len-1] == 'g' && filename[len-2] == 'p' && filename[len-3] == 'j')
	{
		char* name = Grayscale(filename);
		name = Threshold(name, 1);
		name = detectLine(name);


		char cmd[2048];
		FILE *f1;
		char cmd1[2048];
		FILE *f2;
		int j, h, v;
		int hor = 0;
		int ver = 200;
		int j1, h1, v1;
		int hor1 = 400;
		int ver1 = 200;
		gchar *filename1 = "results/linesDetectedIMG.jpg";
		if (image1 && image3)
		{
			gtk_container_remove(GTK_CONTAINER(stkfxd1), image1);
			gtk_container_remove(GTK_CONTAINER(stkfxd1), image3);
		}
		//gtk_widget_hide(image2);
		
		sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", filename);
		f1 = popen(cmd, "r");
		sprintf(cmd1, "identify -format %%wx%%h \"%s\"\n", filename1);
		f2 = popen(cmd1, "r");

		strcpy(cmd,"");
		fgets(cmd, 512, f1);
		strcpy(cmd1,"");
		fgets(cmd1, 512, f2);
		fclose(f1);
		fclose(f2);

		h = v = 1;
		h1 = v1 = 1;

		if (strlen(cmd))
		{
			for (j = 0; j < strlen(cmd)-1; j++)
			{
				if (cmd[j] == 'x')
					break;
			}
			if (cmd[j] == 'x')
			{
				cmd[j] = 0;
				sscanf(cmd, "%d", &h);
				sscanf(&cmd[j+1],"%d", &v);
			}
		}

		if (strlen(cmd1))
		{
			for (j1 = 0; j1 < strlen(cmd1)-1; j1++)
			{
				if (cmd1[j1] == 'x')
					break;
			}
			if (cmd1[j1] == 'x')
			{
				cmd1[j1] = 0;
				sscanf(cmd1, "%d", &h1);
				sscanf(&cmd1[j1+1],"%d", &v1);
			}
		}

		if (h < 100 || v < 100)
		{
			printf("**** questionable image: %s\n", filename);
			return;
		}
		if (h1 < 100 || v1 < 100)
		{
			printf("**** questionable image: %s\n", filename1);
			return;
		}


		int width = 300;
		int height = 300;

		sprintf(cmd, "convert \"%s\" -resize %dx%d tmp.jpg", filename, width, height);
		system(cmd);
		strcpy(filename, "tmp.jpg");

		sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", filename);
		f1 = popen(cmd, "r");

		strcpy(cmd,"");
		fgets(cmd, 512, f1);
		fclose(f1);

		h = v = 1;
		if (strlen(cmd))
		{
			for (j = 0; j < strlen(cmd)-1; j++)
			{
				if (cmd[j] == 'x')
					break;
			}
			if (cmd[j] == 'x')
			{
				cmd[j] = 0;
				sscanf(cmd, "%d", &h);
				sscanf(&cmd[j+1],"%d", &v);
			}
		}

		image1 = gtk_image_new_from_file(filename);
		gtk_container_add(GTK_CONTAINER(stkfxd1), image1);
		gtk_widget_show(image1);

		gtk_fixed_move(GTK_FIXED(stkfxd1), image1, hor, ver);

		sprintf(cmd, "convert \"%s\" -resize %dx%d tmp.jpg", filename1, width, height);
		system(cmd);
		strcpy(filename, "tmp.jpg");

		sprintf(cmd1, "identify -format %%wx%%h \"%s\"\n", filename);
		f2 = popen(cmd, "r");

		strcpy(cmd,"");
		fgets(cmd, 512, f2);
		fclose(f2);

		
		h1 = v1 = 1;

		
		if (strlen(cmd1))
		{
			for (j1 = 0; j1 < strlen(cmd1)-1; j1++)
			{
				if (cmd1[j1] == 'x')
					break;
			}
			if (cmd1[j1] == 'x')
			{
				cmd1[j1] = 0;
				sscanf(cmd1, "%d", &h1);
				sscanf(&cmd1[j1+1],"%d", &v1);
			}
		}

		

		image3 = gtk_image_new_from_file(filename);
		gtk_container_add(GTK_CONTAINER(stkfxd1), image3);
		gtk_widget_show(image3);

		
		gtk_fixed_move(GTK_FIXED(stkfxd1), image3, hor1, ver1);
		system("rm tmp.jpg");
		//system("rm tmp1.jpg");
	}

}

void on_saveText_clicked(GtkButton *b)
{
	GtkTextIter begin, end;
	gchar *text;

	gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(TextBuffer), &begin, (gint) 0);
	gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(TextBuffer), &end, (gint) -1);

	text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(TextBuffer), &begin, &end, TRUE);
	printf(".......\n%s\n.......\n", text);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(TextView), FALSE);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(TextView), FALSE);
	gtk_widget_hide(saveText);

}

void on_editText_clicked(GtkButton *b)
{
	gtk_widget_hide(saveLabel);
	printf("*** on edit text\n");
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(TextView), TRUE);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(TextView), TRUE);
}

void on_changed_text(GtkTextBuffer *t)
{
	printf("*** text changed\n");
	gtk_widget_show(saveText);
}