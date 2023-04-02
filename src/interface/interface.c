#include "interface.h"

typedef struct GUI
{
    GtkWindow* interface;
    GtkStack* window_pages;
    GtkBox* load;
    GtkBox* construct;
    GtkBox* solution;
    GtkButton* QuitButton;
    GtkButton* StartButton;
    gchar* filename;
    GtkImage* image_holder;
    GtkBox* help_menu;
    GtkButton* help_button;
    GtkButton* refresh_button;
    GtkButton* menu_button;
    GtkButton* resolve_button;
    GdkScreen *screen;
    gchar* result_file;
    GtkSwitch* switch_button;
    GtkAdjustment* deltav;
    GtkAdjustment* rpp;
    GtkAdjustment* masse_u;
    GtkScale* binarise_scale;
    GtkScale* contrast_scale;
    GtkScale* rotation_scale;
    int activate;
    GtkButton* menuback;
}GUI;

int activate = 0;

void exit_window()
{
    gtk_main_quit();
}

void menu(GtkButton *b, gpointer user)
{
    GUI *gui = user;
    gtk_widget_hide(GTK_WIDGET(gui->help_menu));
    gtk_widget_show(GTK_WIDGET(gui->load));
    gtk_widget_hide(GTK_WIDGET(gui->solution));
}

void help(GtkButton *b, gpointer user)
{
    GUI *gui = user;
    gtk_widget_show(GTK_WIDGET(gui->help_menu));
    gtk_widget_hide(GTK_WIDGET(gui->load));
    gtk_widget_hide(GTK_WIDGET(gui->solution));
}

void display_image(GtkImage* image_holder, gchar* file_name)
{
	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(file_name, NULL);
	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(image_holder, pixbuf);
	//gtk_image_set_from_file(image_holder, file_name);
}


void display_result(GtkImage* image_holder, gchar* result_file)
{
	display_image(image_holder, result_file);
}

void refresh(GtkButton* b, gpointer user)
{
	GUI *gui = user;
	display_image(gui->image_holder, gui->filename);

}

int main(int argc, char *argv[])
{
	gtk_init(&argc ,&argv);

	GtkBuilder* builder = gtk_builder_new_from_file("interface.glade");

	// Load CSS
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "./interface.css", NULL);

	// Inject CSS
	GdkScreen *screen = gdk_screen_get_default();
	gtk_style_context_add_provider_for_screen(screen,
			GTK_STYLE_PROVIDER(cssProvider),
			GTK_STYLE_PROVIDER_PRIORITY_USER);

	GtkWindow* interface = GTK_WINDOW(gtk_builder_get_object(builder, "interface"));
	GtkStack* window_pages = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
	GtkBox* load = GTK_BOX(gtk_builder_get_object(builder, "load"));
	GtkBox* solution = GTK_BOX(gtk_builder_get_object(builder, "solution"));
	GtkButton* QuitButton = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton"));
	GtkButton* QuitButton2 = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton2"));
	GtkButton* StartButton = GTK_BUTTON(gtk_builder_get_object(builder, "StartButton"));
	GtkImage* image_holder = GTK_IMAGE(gtk_builder_get_object(builder, "image_holder"));
	GtkBox* help_menu = GTK_BOX(gtk_builder_get_object(builder, "help_menu"));
	GtkButton* help_button = GTK_BUTTON(gtk_builder_get_object(builder, "HelpButton"));
	GtkButton* QuitButton1 = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton1"));
	GtkButton* refresh_button = GTK_BUTTON(gtk_builder_get_object(builder, "RefreshButton"));
	GtkButton* menu_button = GTK_BUTTON(gtk_builder_get_object(builder, "MenuButton"));
	//GtkButton* resolve_button = GTK_BUTTON(gtk_builder_get_object(builder, "ResolveButton"));
	GtkSwitch* switch_button = GTK_SWITCH(gtk_builder_get_object(builder, "switch_button"));
	GtkAdjustment* deltav = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "deltav"));
	GtkAdjustment* masse_u = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "conadj"));
	GtkAdjustment* rpp = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "masse_u"));
	GtkButton* menuback = GTK_BUTTON(gtk_builder_get_object(builder, "rpp"));
	GtkScale* binarise_scale = GTK_SCALE(gtk_builder_get_object(builder, "binarise_scale"));
	GtkScale* contrast_scale = GTK_SCALE(gtk_builder_get_object(builder, "contrast_scale"));

	GUI gui={
		.interface = interface,
		.window_pages = window_pages,
		.load = load,
		.solution = solution,
		.QuitButton = QuitButton,
		.StartButton = StartButton,
		.filename = NULL,
		.image_holder = image_holder,
		.help_menu = help_menu,
		.help_button = help_button,
		.menu_button = menu_button,
		.screen = screen,
		.switch_button = switch_button,
		.activate = 0,
		.deltav = deltav,
		.masse_u = masse_u,
		.rpp = rpp,
		.binarise_scale = binarise_scale,
		.contrast_scale = contrast_scale,
		.menuback = menuback,
	};

	//CONNECTION
	g_signal_connect(interface, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(QuitButton, "clicked", G_CALLBACK(exit_window),NULL);
	g_signal_connect(QuitButton1, "clicked", G_CALLBACK(exit_window),NULL);
	g_signal_connect(QuitButton2, "clicked", G_CALLBACK(exit_window),NULL);
	//g_signal_connect(StartButton, "clicked", G_CALLBACK(open_files_explorer), &gui);
	g_signal_connect(help_button, "clicked", G_CALLBACK(help), &gui);
	g_signal_connect(refresh_button, "clicked", G_CALLBACK(refresh), &gui);
	g_signal_connect(menu_button, "clicked", G_CALLBACK(menu), &gui);
	//g_signal_connect(resolve_button, "clicked", G_CALLBACK(resolve_clicked), &gui);
	//g_signal_connect(switch_button, "state-set", G_CALLBACK(otsu_switch), NULL);
	//g_signal_connect(switch_button, "activate", G_CALLBACK(otsu_switch), NULL);
	//g_signal_connect(binadj, "value-changed", G_CALLBACK(binarise_scale_update), &gui);
	//g_signal_connect(conadj, "value-changed", G_CALLBACK(contrast_scale_update), &gui);
//	g_signal_connect(rotadj, "value-changed", G_CALLBACK(rot_display), &gui);
	//g_signal_connect(resolve_button, "clicked", G_CALLBACK(display_result), &gui);
	g_signal_connect(menuback, "clicked", G_CALLBACK(menu), &gui);
	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(GTK_WIDGET(interface));
	gtk_main();

	return EXIT_SUCCESS;
}

