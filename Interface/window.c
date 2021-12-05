#include <gtk/gtk.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
#include "../manipulateImage/sobel.h"
#include "../manipulateImage/big_line_detection.h"
#include"../Cutting/cut.h"
#include "../neuralNetwork/initNetwork.h"

enum {
  COLUMN_STRING,
  COLUMN_INT,
  COLUMN_BOOLEAN,
  N_COLUMNS
};

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
GtkWidget *TextView2;
GtkWidget *saveText;
GtkWidget *editText;
GtkTextBuffer *TextBuffer;
GtkTextBuffer *TextBuffer2;
GtkWidget *solveButton;
GtkWidget *saveLabel;
GtkWidget *saveFile;
GtkWidget *noGridLabel;
GtkWidget *combo1;
GtkWidget *entry1;
char tmp[1024];
neuralNetwork *nn;
int isFailed;


void computeImage(char *filename);
int main (int argc, char *argv[])
{
	nn = loadNetwork();
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
	TextView2 = GTK_WIDGET(gtk_builder_get_object(builder, "TextView2"));
	saveText = GTK_WIDGET(gtk_builder_get_object(builder, "saveText"));
	editText = GTK_WIDGET(gtk_builder_get_object(builder, "editText"));
	solveButton = GTK_WIDGET(gtk_builder_get_object(builder, "solveButton"));
	saveLabel = GTK_WIDGET(gtk_builder_get_object(builder, "saveLabel"));
	saveFile = GTK_WIDGET(gtk_builder_get_object(builder, "saveFile"));
	combo1 = GTK_WIDGET(gtk_builder_get_object(builder, "combo1"));
	entry1 = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
	noGridLabel = GTK_WIDGET(gtk_builder_get_object(builder, "noGridLabel"));



	TextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TextView));
	g_signal_connect(TextBuffer, "changed", G_CALLBACK(on_changed_text), NULL);
	TextBuffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TextView2));
    
	strcpy(tmp, "... ... ...\n... ... ...\n... ... ...\n\n... ... ...\n... ... ...\n... ... ...\n\n... ... ...\n... ... ...\n... ... ...");

	gtk_text_buffer_set_text(TextBuffer, (const gchar *) tmp, (gint) -1);
	gtk_widget_hide(saveText);
	gtk_widget_hide(noGridLabel);
	gtk_widget_hide(saveLabel);

	
    // Runs the main loop.
    gtk_widget_show(window);
    image1 = NULL;
	image3 = NULL;
    gtk_main();
	freeNetwork(nn);

    // Exits.
    return 0;
}

void on_saveFile_clicked(GtkButton *b)
{
	FILE *saveFile = fopen("sudokuResult.txt", "w");
	if(saveFile != NULL)
	{
		GtkTextIter begin, end;
		gchar *text;
		gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(TextBuffer2), &begin, (gint) 0);
		gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(TextBuffer2), &end, (gint) -1);
		text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(TextBuffer2), &begin, &end, TRUE);
	    fprintf(saveFile,"%s",text);
		fclose(saveFile);
		int error = system("mv sudokuResult.txt ~/Downloads/sudokuResult.txt");
		if (error)
			return;
	}
    
	gtk_widget_show(saveLabel);
}

void on_solveButton_clicked(GtkWidget *b)
{
	gtk_widget_hide(saveLabel);
	GtkTextIter begin, end;
	gchar *text;
	gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(TextBuffer), &begin, (gint) 0);
	gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(TextBuffer), &end, (gint) -1);
	text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(TextBuffer), &begin, &end, TRUE);
	char *solvedSudoku = solve(text);
	gtk_text_buffer_set_text(TextBuffer2, (const gchar *) solvedSudoku, (gint) -1);
}

void on_chooser_file_activated(GtkFileChooserButton *b)
{
	printf("ok");
}

void computeImage(char *filename)
{
	SDL_Surface *BaseImg = IMG_Load(filename);
	isFailed = 0;
	SDL_Surface *grayImg = Grayscale(BaseImg);
	SDL_Surface *thresholdImg = Threshold(grayImg);
	SDL_Surface *blurImg = GaussianBlur(thresholdImg);
	SDL_Surface *sobelImg = sobel(blurImg);
	int *res = Detection(sobelImg, &isFailed);
	if (!isFailed)
	{
		char **names = CutGrid(thresholdImg, res[1], res[2], res[1]+res[0]-1, res[2]+res[0]-1);
		
		int k = 0;
		
		for(size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				matrix *inputs = matAlloc(1, 900);
				convertImageToMat(names[j*9+i], inputs);
				printf("%ld\n", j*9+i);
				
				matrix *resN = matAlloc(1, 10);
				forwardPropagation(nn, inputs, resN);
				printf("%ld\n", j*9+i);

				int number = getPrediction(resN);
				if (tmp[k] == '\n' || tmp[k] == ' ')
					k++;
				if (tmp[k] == '\n')
					k++;
				
				tmp[k] = number==0 ? '.' : number+48;
				k++;
				printf("%ld\n", j*9+i);
				
				freeMat(inputs);
				freeMat(resN);
				free(names[j*9+i]);
			}
		}
		gtk_text_buffer_set_text(TextBuffer, (const gchar *) tmp, (gint) -1);
		
		free(names);
		free(res);
	}
	else
	{
		strcpy(tmp, "... ... ...\n... ... ...\n... ... ...\n\n... ... ...\n... ... ...\n... ... ...\n\n... ... ...\n... ... ...\n... ... ...");
		gtk_text_buffer_set_text(TextBuffer, (const gchar *) tmp, (gint) -1);
	}	
}

void on_chooser_file_set(GtkWidget *b)
{
	gtk_widget_hide(saveLabel);
	gtk_widget_hide(noGridLabel);
	gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(b));
	int len = strlen(filename);
	if ((filename[len-1] == 'g' && filename[len-2] == 'p' && filename[len-3] == 'j') ||
		(filename[len-1] == 'g' && filename[len-2] == 'n' && filename[len-3] == 'p') || 
		(filename[len-1] == 'p' && filename[len-2] == 'm' && filename[len-3] == 'b') ||
		(filename[len-1] == 'g' && filename[len-2] == 'e' && filename[len-3] == 'p' && filename[len-4] == 'j'))
	{
		computeImage(filename);
		int error;
		char* error1;
		char cmd[2048];
		FILE *f1;
		int j, h, v;
		int hor = 0;
		int ver = 200;

		if (image1)
		{
			gtk_container_remove(GTK_CONTAINER(stkfxd1), image1);
		}
		//gtk_widget_hide(image2);
		
		sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", filename);
		f1 = popen(cmd, "r");
		

		strcpy(cmd,"");
		error1 = fgets(cmd, 512, f1);
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


		if (h < 100 || v < 100)
		{
			printf("**** questionable image: %s\n", filename);
			return;
		}


		int width = 300;
		int height = 300;

		sprintf(cmd, "convert \"%s\" -resize %dx%d tmp.jpg", filename, width, height);
		error = system(cmd);
		strcpy(filename, "tmp.jpg");

		sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", filename);
		f1 = popen(cmd, "r");

		strcpy(cmd,"");
		error1 = fgets(cmd, 512, f1);
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

		
		error = system("rm tmp.jpg");
		if (error || error1)
		{
			return;
		}
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
	printf("#######\n%s\n#######\n", text);
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

void on_entry1_changed(GtkEntry *e)
{
	const gchar* mode = gtk_entry_get_text(e);
	gchar* filename = "results/grayscaleIMG.jpg";
	int error;
	char *error1;
	if (strcmp(mode, "GrayScale") == 0)
	{
		filename = "results/grayscaleIMG.jpg";
	}
	else if (strcmp(mode, "Threshold") == 0)
	{
		filename = "results/thresholdIMG.jpg";
	}
	else if (strcmp(mode, "Sobel") == 0)
	{
		filename = "results/sobelIMG.jpg";
	}
	else if (isFailed)
	{
		if (image3 != NULL)
		{
			gtk_container_remove(GTK_CONTAINER(stkfxd1), image3);
		}
			
		gtk_widget_show(noGridLabel);
		return;
	}
	else
	{
		filename = "results/bigLines.jpg";
	}

	char cmd[2048];
	FILE *f1;
	int j, h, v;
	int hor = 400;
	int ver = 200;
	if (image3 != NULL)
	{
		gtk_container_remove(GTK_CONTAINER(stkfxd1), image3);
	}
	//gtk_widget_hide(image2);
	
	sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", filename);
	f1 = popen(cmd, "r");

	strcpy(cmd,"");
	error1 = fgets(cmd, 512, f1);
	
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

	if (h < 100 || v < 100)
	{
		printf("**** questionable image: %s\n", filename);
		return;
	}

	int width = 300;
	int height = 300;

	sprintf(cmd, "convert \"%s\" -resize %dx%d tmp1.jpg", filename, width, height);
	error = system(cmd);
	//strcpy(filename, "tmp1.jpg");


	sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", "tmp1.jpg");
	f1 = popen(cmd, "r");

	strcpy(cmd,"");
	error1 = fgets(cmd, 512, f1);
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
	image3 = gtk_image_new_from_file("tmp1.jpg");
	gtk_container_add(GTK_CONTAINER(stkfxd1), image3);
	gtk_widget_show(image3);

	gtk_fixed_move(GTK_FIXED(stkfxd1), image3, hor, ver);

	
	error = system("rm tmp1.jpg");
	if (error || error1)
		return;

}