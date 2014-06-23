#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

void print_usage() {
    printf("Usage: imclip [-o outfile [-t type]|-i infile]\n");
}

char *strdup(const char *s) {
    char *dup = malloc(strlen(s) + 1);
    if(dup != NULL) {
        strcpy(dup, s);
    }

    return dup;
}

int clipboard_to_file(char* filename, char* type) {
    GtkClipboard *clipboard;
    GdkPixbuf *pixbuf;

    char *ptr_to_free_filename_dup = NULL;
    char* image_type;
    int error = 0;

    if(type == NULL) {
        char *tok;
        char *filename_to_split = strdup(filename);
        ptr_to_free_filename_dup = filename_to_split;

        tok = strtok(filename_to_split, ".");
        image_type = strtok(NULL, ".");
        if(image_type == NULL) {
            printf("Invalid filename: %s. If type is not provided with the -t flag, please provide a valid filename with a valid image-extension.\n", filename);
            error = 1;
        } else if(strcasecmp(image_type, "jpg") == 0) {
            image_type = "jpeg";
        }

    } else {
        image_type = type;
    }

    if(error == 0) {
        clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        pixbuf = gtk_clipboard_wait_for_image(clipboard);
        
        GError *gerror = NULL;
        gdk_pixbuf_save(pixbuf, filename, image_type, &gerror, NULL);

        if(gerror != NULL) {
            printf("Unable to save clipboard image to %s: %s\n", filename, gerror->message);
            error = 1;

            g_error_free(gerror);
        }

        if(ptr_to_free_filename_dup) {
            free(ptr_to_free_filename_dup);
        }

        g_object_unref(pixbuf);
    }

    return error;
}

int file_to_clipboard(char* filename) {
    GtkClipboard *clipboard;
    GdkPixbuf *pixbuf;

    int error = 0;

    GError *gerror = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &gerror);
    if(gerror != NULL) {
        printf("Unable to load image data from file %s: %s\n", filename, gerror->message);
        error = 1;

        g_error_free(gerror);
    } else {
        clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        gtk_clipboard_set_image(clipboard, pixbuf);
        gtk_clipboard_store(clipboard);
        g_object_unref(pixbuf);

    }

    return error;
}

static bool in_flag, out_flag, type_flag;

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"in", required_argument, NULL, 'i'},
        {"out", required_argument, NULL, 'o'},
        {"type", required_argument, NULL, 't'},
        {NULL, 0, NULL, 0} 
    };

    int opt;
    int error = 0;
    char *filename = NULL;
    char *type = NULL;
    int option_index = 0;

    gtk_init(&argc, &argv);
   
    while((opt = getopt_long(argc, argv, "i:o:t:", long_options, &option_index)) != -1) {
        switch(opt) {
            case 'i': filename = strdup(optarg);
                      in_flag = true;
                      break;

            case 'o': filename = strdup(optarg);
                      out_flag = true;
                      break;

            case 't': type = strdup(optarg);
                      type_flag = true;
                      break;

            default : print_usage();
                      exit(1);
        }
    }

    if(in_flag) {
        error = file_to_clipboard(filename);
    } else if(out_flag) {
        error = clipboard_to_file(filename, type);
        free(filename);

        if(type_flag) {
            free(type);
        }
    } else {
        print_usage();
        error = 1;
    }

    return error;
}
