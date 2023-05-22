#include "interface.h"
#include "utils.h"
#include "loader.h"
#include "algo/algopti.h"
#include "basic_display/basic_display.h"

typedef struct GUI
{
	GtkWindow* interface;
	GtkStack* window_pages;
	GtkBox* load;
	GtkBox* solution;
	GtkButton* QuitButton;
	GtkButton* StartButton;
	gchar* filename;
	GtkImage* image_holder;
	GtkBox* help_menu;
	GtkButton* help_button;
	GtkButton* refresh_button;
	//GtkButton* menu_button;
	GtkButton* resolve_button;
	GdkScreen *screen;
	gchar* result_file;
	GtkSwitch* switch_button;
	//GtkAdjustment* binadj;
	//GtkAdjustment* conadj;
	//GtkAdjustment* rotadj;
	GtkAdjustment* mass_u;
	GtkAdjustment* rpp_min;
	GtkAdjustment* rpp_max;
	GtkAdjustment* deltav;
    GtkButton* MenuButton;
	GtkButton* PreviousButton;
	GtkButton* NextButton;
	GtkButton* LoadButton;
	GtkButton* SaveButton;
	GtkScale* scale_mass_u;
	GtkScale* scale_rpp_min;
	GtkScale* scale_rpp_max;
	GtkScale* scale_deltav;

	GtkTextBuffer* textbuffer_rocketinfo;
	GtkTextView* rocketinfo;
	GtkScrolledWindow* scrolledresult;

	Datas* result_data;
	int current_stage;

	int activate;
	GtkButton* menuback;
}GUI;

int activate = 0;

void on_quit_clicked()
{
	gtk_main_quit();
}

void going_back_menu(GtkButton* b, gpointer user)
{
	GUI* gui = user;
    gtk_stack_set_visible_child(GTK_STACK(gui->window_pages), GTK_WIDGET(gui->load));
	//gtk_widget_show(GTK_WIDGET(gui->load));
	//gtk_widget_hide(GTK_WIDGET(gui->help_menu));
	//gtk_widget_hide(GTK_WIDGET(gui->solution));
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


/*void binarise_scale_update(GtkAdjustment* k, gpointer user)*/
/*{*/
	/*GUI* gui = user;*/
	/*if (activate == 1)*/
	/*{*/
		/*process("output.bmp",gtk_adjustment_get_value(gui->conadj),gtk_adjustment_get_value(gui->binadj),1);*/
	/*}*/
	/*else*/
	/*{*/
		/*process("output.bmp",gtk_adjustment_get_value(gui->conadj),gtk_adjustment_get_value(gui->binadj),0);*/
	/*}	*/
	/*//gui->filename = "../processing/output.bmp";*/
	/*display_image(gui->image_holder, "output_1.bmp");*/
/*}*/

/*void contrast_scale_update(GtkAdjustment* i, gpointer user)*/
/*{*/
	/*GUI* gui = user;*/
	/*if (activate == 1)*/
	/*{*/
		/*process("output.bmp",gtk_adjustment_get_value(gui->conadj),gtk_adjustment_get_value(gui->binadj),1);*/
	/*}*/
	/*else*/
	/*{*/
		/*process("output.bmp",gtk_adjustment_get_value(gui->conadj),gtk_adjustment_get_value(gui->binadj),0);*/
	/*}	*/
	/*//gui->filename = "../processing/output.bmp";*/
	/*display_image(gui->image_holder, "output_1.bmp");*/
/*}*/


/*void rot_display(GtkAdjustment* j, gpointer user)
{
	GUI* gui = user;
	rotate(gtk_adjustment_get_value(gui->rotadj), gui->filename);
	display_image(gui->image_holder, "output.bmp");

}*/

size_t my_strlen(char str[])
{
	size_t i = 0;
	for (; str[i] != 0;i ++); 
	return i;
}

char *str_concat(char str1[], char str2[])
{
	char *res = calloc(my_strlen(str1) + my_strlen(str2)+1, sizeof(char));
	for (size_t i = 0; i < my_strlen(str1); i ++) {
		res[i] = str1[i];
	}
	for (size_t i = 0; i < my_strlen(str2); i ++) {
		res[i + my_strlen(str1)] = str2[i];
	}
	return res;
}

void resolve_clicked(GtkButton* b, gpointer user, gchar* filename)
{
	//GUI* gui = user;
	//display_result(gui->image_holder, );
}/*
void open_files_explorer(GtkButton* b, gpointer user)
{
	GUI* gui = user;
	GtkWidget *p_dialog = NULL;
	p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"document-cancel", GTK_RESPONSE_CANCEL,
			"document-open", GTK_RESPONSE_ACCEPT,
			NULL);

	gchar *file_name = NULL;
	if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
	{
		file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
	}
	gtk_widget_destroy (p_dialog);
	gui->filename = file_name;
	gtk_widget_show(GTK_WIDGET(gui->solution));
	gtk_widget_hide(GTK_WIDGET(gui->load));
	display_image(gui->image_holder, file_name);
}

char * convertNumberIntoArray(unsigned int number) {
    unsigned int length = (int)(log10((float)number)) + 1;
    char * arr = (char *) malloc(length * sizeof(char)), * curr = arr;
    do {
        *curr++ = number % 10;
        number /= 10;
    } while (number != 0);
    return arr;
}
*/

void refresh(GtkButton* b, gpointer user)
{
	GUI *gui = user;
	display_image(gui->image_holder, gui->filename);
	
}
/*
void otsu_switch()
{
	if(activate == 0)
	{
		activate = 1;
	}
	else
	{
		activate = 0;
	}
}
*/

void solution_show(GtkButton *b, gpointer user)
{
    GUI* gui = user;
    gtk_stack_set_visible_child(GTK_STACK(gui->window_pages), GTK_WIDGET(gui->solution));
	//gtk_widget_show(GTK_WIDGET(gui->solution));
	//gtk_widget_hide(GTK_WIDGET(gui->load));
	//gtk_widget_hide(GTK_WIDGET(gui->help_menu));
}

void open_help_menu(GtkButton* b, gpointer user)
{
	GUI* gui = user;
    gtk_stack_set_visible_child(GTK_STACK(gui->window_pages), GTK_WIDGET(gui->help_menu));
	//gtk_widget_show(GTK_WIDGET(gui->help_menu));
	//gtk_widget_hide(GTK_WIDGET(gui->load));
	//gtk_widget_hide(GTK_WIDGET(gui->solution));
}

void on_menu_clicked(GtkButton* b, gpointer user)
{
	GUI* gui = user;
    gtk_stack_set_visible_child(GTK_STACK(gui->window_pages), GTK_WIDGET(gui->load));
	//gtk_widget_hide(GTK_WIDGET(gui->help_menu));
	//gtk_widget_show(GTK_WIDGET(gui->load));
	//gtk_widget_hide(GTK_WIDGET(gui->solution));
}

void start_button_clicked(GtkButton* b, gpointer user_data)
{
    // Cast the user data pointer to the GUI struct type
    GUI* gui = (GUI*) user_data;

    // Show the solution page and hide the load page
    gtk_widget_hide(GTK_WIDGET(gui->load));
    gtk_widget_show(GTK_WIDGET(gui->solution));
}

Stage* get_stage_from_data(Datas* d, int n)
{
	Stage* s = d->best_rocket->first_stage;
	return s;
}

void on_create_clicked(GtkButton* b, gpointer user_data)
{
    GUI* gui = (GUI*) user_data;
	
	// Utilise l'objet GtkScale
	gdouble tmp_value = gtk_adjustment_get_value (gui->rpp_min);
	double twrmin = (double) tmp_value;
	
	tmp_value = gtk_adjustment_get_value (gui->rpp_max);
	double twrmax = (double) tmp_value;

	tmp_value = gtk_adjustment_get_value (gui->deltav);
	double deltav = (double) tmp_value;

	tmp_value = gtk_adjustment_get_value(gui->mass_u);
	double mass_u = (double) tmp_value;

    gui->result_data = create_datas();
    load_parts(gui->result_data);

	gui->result_data->mass_payload = mass_u;
    gui->result_data->deltaV_min = deltav;
    gui->result_data->TWR_min = twrmin;
    gui->result_data->TWR_max = twrmax;

    int r = linear_algo(gui->result_data);

	gui->current_stage = 0;
	gtk_widget_show(GTK_WIDGET(gui->PreviousButton));
	gtk_widget_show(GTK_WIDGET(gui->NextButton));
	gtk_widget_show(GTK_WIDGET(gui->SaveButton));
	gtk_widget_show(GTK_WIDGET(gui->rocketinfo));

	GString* result = g_string_new("====== DATAS ======\n");
	g_string_append_printf(result, "DeltaV_min: %f\n", gui->result_data->deltaV_min);
	g_string_append_printf(result, "TWR_min: %f\n", gui->result_data->TWR_min);
	g_string_append_printf(result, "TWR_max: %f\n", gui->result_data->TWR_max);
	g_string_append_printf(result, "nbr_engines: %ln\n", gui->result_data->nbr_engines);
	g_string_append_printf(result, "mass_payload: %f\n", gui->result_data->mass_payload);
	
	Rocket* best_rocket = gui->result_data->best_rocket;
	int count = 0;
	for (Stage* i = best_rocket->first_stage; i != NULL; i = i->next)
	{
		g_string_append_printf(result, "\n========= STAGE %d =========\n", count);
		g_string_append_printf(result, "total_mass: %f\n", i->mass_full+i->mass_dry);
		g_string_append_printf(result, "DeltaV    : %f\n", i->DeltaV);
		g_string_append_printf(result, "cost      : %f\n", i->cost);
		g_string_append_printf(result, "ISP_vac   : %f\n", i->ISP_vac);
		g_string_append_printf(result, "ISP_atm   : %f\n", i->ISP_atm);
		g_string_append_printf(result, "TWR_min   : %f\n", i->TWR_min);
		g_string_append_printf(result, "TWR_max   : %f\n", i->TWR_max);

		g_string_append_printf(result, "\n------ Tanks ------\n");
		for (Part* j = i->first_tank; j != NULL; j = j->next)
		{
			g_string_append_printf(result, "- %s\n", j->name);
		}

		g_string_append_printf(result, "\n------ Engines ------\n");


		for (Part* j = i->engine; j != NULL; j = j->next)
		{
			g_string_append_printf(result, "- %s\n", j->name);

		}

		g_string_append_printf(result, "\n------ Decouplers ------\n");
		for (Part* j = i->decoupler; j != NULL; j = j->next)
		{
			g_string_append_printf(result, "- %s\n", j->name);
		}
		count++;
	}

	gtk_text_buffer_set_text(gui->textbuffer_rocketinfo, result->str, -1);
	pretty_print(gui->result_data);
	
    /*basic_display(gui->result_data->best_rocket);*/
}

void on_previous_clicked(GtkButton* b, gpointer user_data)
{
	printf("oui\n");
    GUI* gui = (GUI*) user_data;

	gtk_text_buffer_set_text(gui->textbuffer_rocketinfo, "Nouveau texte du GtkTextView", -1);
}

void on_next_clicked(GtkButton* b, gpointer user_data)
{
	return;
}

void on_load_clicked(GtkButton* b, gpointer user_data)
{
	return;
}

void on_save_clicked(GtkButton* b, gpointer user_data)
{
	return;
}

int start_interface()
{
	gtk_init(0, NULL);

	GtkBuilder* builder = gtk_builder_new_from_file("./interface/interface.glade");

	// Load CSS
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "./interface/interface.css", NULL);

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
	//GtkImage* image_holder = GTK_IMAGE(gtk_builder_get_object(builder, "image_holder"));
	GtkBox* help_menu = GTK_BOX(gtk_builder_get_object(builder, "help_menu"));
	GtkButton* help_button = GTK_BUTTON(gtk_builder_get_object(builder, "HelpButton"));
	GtkButton* QuitButton1 = GTK_BUTTON(gtk_builder_get_object(builder, "QuitButton1"));
	GtkButton* refresh_button = GTK_BUTTON(gtk_builder_get_object(builder, "RefreshButton"));
	GtkButton* menu_button = GTK_BUTTON(gtk_builder_get_object(builder, "MenuButton"));
    GtkButton* MenuButton = GTK_BUTTON(gtk_builder_get_object(builder, "MenuButton"));
    GtkButton* MenuButton1 = GTK_BUTTON(gtk_builder_get_object(builder, "MenuButton1"));
	GtkButton* resolve_button = GTK_BUTTON(gtk_builder_get_object(builder, "ResolveButton"));
	//GtkSwitch* switch_button = GTK_SWITCH(gtk_builder_get_object(builder, "switch_button"));
	//GtkAdjustment* binadj = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "binadj"));
	//GtkAdjustment* conadj = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "conadj"));
	//GtkAdjustment* rotadj = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "rotadj"));
	GtkButton* menuback = GTK_BUTTON(gtk_builder_get_object(builder, "menuback"));

	GtkAdjustment* rpp_min = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "rpp_min"));
	GtkAdjustment* rpp_max = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "rpp_max"));
	GtkAdjustment* deltav = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "deltav"));
	GtkAdjustment* mass_u = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "mass_u"));
	GtkScale* scale_mass_u = GTK_SCALE(gtk_builder_get_object(builder, "id_mass_u"));
	GtkScale* scale_rpp_min = GTK_SCALE(gtk_builder_get_object(builder, "id_rpp_min"));
	GtkScale* scale_rpp_max = GTK_SCALE(gtk_builder_get_object(builder, "id_rpp_max"));
	GtkScale* scale_deltav = GTK_SCALE(gtk_builder_get_object(builder, "id_deltav"));

	GtkButton* PreviousButton = GTK_BUTTON(gtk_builder_get_object(builder, "PreviousButton"));
	GtkButton* NextButton = GTK_BUTTON(gtk_builder_get_object(builder, "NextButton"));
	GtkButton* LoadButton = GTK_BUTTON(gtk_builder_get_object(builder, "LoadButton"));
	GtkButton* SaveButton = GTK_BUTTON(gtk_builder_get_object(builder, "SaveButton"));
	GtkScrolledWindow* scrolledresult = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledresult"));
	GtkTextView* rocketinfo = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textview_rocketinfo"));
	GtkTextBuffer* textbuffer_rocketinfo = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "textbuffer_rocketinfo"));

	GUI gui={
		.interface = interface,
		.window_pages = window_pages,
		.load = load,
		.solution = solution,
		.QuitButton = QuitButton,
		.StartButton = StartButton,
		//.filename = NULL,
		//.image_holder = image_holder,
		.help_menu = help_menu,
		.help_button = help_button,
		.MenuButton = menu_button,
		.screen = screen,
		.scrolledresult = scrolledresult,
		.PreviousButton = PreviousButton,
		.NextButton = NextButton,
		.LoadButton = LoadButton,
		.SaveButton = SaveButton,

		.rocketinfo = rocketinfo,
		.textbuffer_rocketinfo = textbuffer_rocketinfo,
		.activate = 0,
		.mass_u = mass_u,
		.rpp_min = rpp_min,
		.rpp_max = rpp_max,
		.deltav = deltav,
		.scale_mass_u = scale_mass_u,
		.scale_rpp_min = scale_rpp_min,
		.scale_rpp_max = scale_rpp_max,
		.scale_deltav = scale_deltav,
		.menuback = menuback,
	};

	//CONNECTION
	g_signal_connect(interface, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(QuitButton, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(QuitButton1, "clicked", G_CALLBACK(on_quit_clicked),NULL);
	g_signal_connect(QuitButton2, "clicked", G_CALLBACK(on_quit_clicked),NULL);

	g_signal_connect(StartButton, "clicked", G_CALLBACK(solution_show), &gui);
	g_signal_connect(help_button, "clicked", G_CALLBACK(open_help_menu), &gui);

	g_signal_connect(MenuButton, "clicked", G_CALLBACK(going_back_menu), &gui);
	g_signal_connect(MenuButton1, "clicked", G_CALLBACK(going_back_menu), &gui);

	// ResolveButton
	g_signal_connect(resolve_button, "clicked", G_CALLBACK(on_create_clicked), &gui);

	g_signal_connect(PreviousButton, "clicked", G_CALLBACK(on_previous_clicked), &gui);
	g_signal_connect(NextButton, "clicked", G_CALLBACK(on_next_clicked), &gui);
	g_signal_connect(SaveButton, "clicked", G_CALLBACK(on_save_clicked), &gui);
	g_signal_connect(LoadButton, "clicked", G_CALLBACK(on_load_clicked), &gui);

	/*g_signal_connect(refresh_button, "clicked", G_CALLBACK(refresh), &gui);*/
	/*g_signal_connect(menu_button, "clicked", G_CALLBACK(on_menu_clicked), &gui);*/
    /*g_signal_connect(MenuButton, "clicked", G_CALLBACK(solution_show), &gui);*/
    //g_signal_connect(gui->StartButton, "clicked", G_CALLBACK(start_button_clicked), (void *) gui);
    //g_signal_connect(gui->StartButton, "clicked", G_CALLBACK(start_button_clicked), *gui);
    /*g_signal_connect(StartButton, "clicked", G_CALLBACK(solution_show), &gui);*/

	//g_signal_connect(resolve_button, "clicked", G_CALLBACK(resolve_clicked), &gui);
	//g_signal_connect(switch_button, "state-set", G_CALLBACK(otsu_switch), NULL);
	//g_signal_connect(switch_button, "activate", G_CALLBACK(otsu_switch), NULL);
	//g_signal_connect(binadj, "value-changed", G_CALLBACK(binarise_scale_update), &gui);
	//g_signal_connect(conadj, "value-changed", G_CALLBACK(contrast_scale_update), &gui);
//	g_signal_connect(rotadj, "value-changed", G_CALLBACK(rot_display), &gui);
	//g_signal_connect(resolve_button, "clicked", G_CALLBACK(display_result), &gui);
	g_signal_connect(menuback, "clicked", G_CALLBACK(going_back_menu), &gui);

	gtk_builder_connect_signals(builder, NULL);

	gtk_widget_show(GTK_WIDGET(interface));
	gtk_main();

	return EXIT_SUCCESS;
}

