#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

static void query();
static void run(const gchar* strName,gint intNparams,const GimpParam* param,gint* intNreturnValues,GimpParam** returnValues);
static void updatePreview(GtkFileChooser *fileChooser,gpointer data);
static gboolean facebrickFileChooser(GimpDrawable *drawable);
static void insertLaFacjataFacebrick(GimpDrawable *drawable,char* strFilename);
static char strLaFacjataPath[]="/home/user/soft/LaFacjata/";
GimpPlugInInfo PLUG_IN_INFO={NULL,NULL,query,run};

MAIN()

static void query(){
	static GimpParamDef args[]={{
				GIMP_PDB_INT32,
				"run-mode",
				"Run mode"
			},
			{
				GIMP_PDB_IMAGE,
				"image",
				"Input image"
			},
			{
				GIMP_PDB_DRAWABLE,
				"drawable",
				"Input drawable"
			}
		};
	gimp_install_procedure(
		"plug-in-lafacjatainsertfacebrick",
		"LaFacjata insert facebrick",
		"LaFacjata insert facebrick",
		"Brian Cobrickowsky",
		"Have fun",
		"2004",
		"_LaFacjata insert facebrick...",
		"RGB*,GRAY*",
		GIMP_PLUGIN,
		G_N_ELEMENTS(args),0,
		args,NULL);
	gimp_plugin_menu_register("plug-in-lafacjatainsertfacebrick","<Image>/Filters/Misc");
}
static void run(const gchar* strName,gint intNparams,const GimpParam* param,gint* intNreturnValues,GimpParam** returnValues){
	static GimpParam values[1];
	GimpPDBStatusType status=GIMP_PDB_SUCCESS;
	GimpRunMode runMode;
	GimpDrawable *drawable;
	/*Obligatory output values*/
	*intNreturnValues=1;
	*returnValues=values;
	values[0].type=GIMP_PDB_STATUS;
	values[0].data.d_status=status;
	runMode=param[0].data.d_int32;
	drawable=gimp_drawable_get(param[2].data.d_drawable);
	switch(runMode)
	{
		case GIMP_RUN_INTERACTIVE:
		if(!facebrickFileChooser(drawable))
			return;
		break;
		case GIMP_RUN_NONINTERACTIVE:
		break;
		case GIMP_RUN_WITH_LAST_VALS:
		break;
		default:
		break;
	}
	gimp_displays_flush();
	gimp_drawable_detach(drawable);
	return;
}
static void insertLaFacjataFacebrick(GimpDrawable* drawable,char* strFilename){
	gint32 intInputDrawable;
	gint32 intImage;
	intImage=gimp_file_load(1,strFilename,strFilename);
	gint32 intWidth;
	gint32 intHeight;
	intWidth=gimp_image_width(intImage);
	intHeight=gimp_image_height(intImage);
	intInputDrawable=gimp_image_get_active_drawable(intImage);
	gint32 intChannels=gimp_drawable_bpp(intInputDrawable);
	g_message("LaFacjata facebrick inserted;intWidth=%d,intHeight=%d,intChannels=%d",intWidth,intHeight,intChannels);
	gimp_edit_copy(intInputDrawable);
	gimp_edit_paste(drawable->drawable_id,TRUE);
	guint32 intImage2=gimp_drawable_get_image(drawable->drawable_id);
	guint32 intActiveLayer=gimp_image_get_active_layer(intImage2);
	size_t intFilenameLength=strlen(strFilename);
	char* strBrickname=(char*)malloc(intFilenameLength*sizeof(char));
	strncpy(strBrickname,strFilename+16,intFilenameLength);
	gimp_item_set_name(intActiveLayer,strBrickname);
	free(strBrickname);
}
static void updatePreview(GtkFileChooser* fileChooser,gpointer data){
	GtkWidget* preview;
	char* strFilename;
	GdkPixbuf* pixbuf;
	gboolean blnHavePreview;
	preview=GTK_WIDGET(data);
	strFilename=gtk_file_chooser_get_preview_filename(fileChooser);
	pixbuf=gdk_pixbuf_new_from_file_at_size(strFilename,128,128,NULL);
	blnHavePreview=(pixbuf!=NULL);
	g_free(strFilename);
	gtk_image_set_from_pixbuf(GTK_IMAGE(preview),pixbuf);
	if(pixbuf)
		g_object_unref(pixbuf);
	gtk_file_chooser_set_preview_widget_active(fileChooser,blnHavePreview);
}
static gboolean facebrickFileChooser(GimpDrawable* drawable){
	GtkWidget* dialog;
	GtkFileChooserAction action=GTK_FILE_CHOOSER_ACTION_OPEN;
	gint intResult;
	GtkWidget* parentWindow;
	GtkWidget* preview;
	gimp_ui_init("LaFacjataInsertFacebrick",FALSE);
	parentWindow=gimp_dialog_new("LaFacjata insert facebrick","lafacjatainsertfacebrick",NULL,0,gimp_standard_help_func,"plug-in-lafacjatainsertfacebrick",GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	dialog=gtk_file_chooser_dialog_new("Open File",GTK_WINDOW(parentWindow),action,("_Cancel"),GTK_RESPONSE_CANCEL,("_Open"),GTK_RESPONSE_ACCEPT,NULL);
	gboolean blnResult=gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),strLaFacjataPath);
	gboolean blnPreview;
	preview=gtk_image_new();
	g_signal_connect(dialog,"update-preview",G_CALLBACK(updatePreview),preview);
	gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(dialog),preview);
	intResult=gtk_dialog_run(GTK_DIALOG(dialog));
	if(intResult==GTK_RESPONSE_ACCEPT)
	{
		char *strFilename;
		GtkFileChooser *chooser=GTK_FILE_CHOOSER(dialog);
		strFilename=gtk_file_chooser_get_filename(chooser);
		insertLaFacjataFacebrick(drawable,strFilename);
		g_free(strFilename);
		gtk_widget_destroy(dialog);
		return TRUE;
	}else{
		gtk_widget_destroy(dialog);
		return FALSE;
	}
}
