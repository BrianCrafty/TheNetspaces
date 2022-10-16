#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

typedef struct{
	gint intColors;
	GimpDrawable* drawable;
	char* strFilename;
	gboolean blnDoQuantisation;
	gboolean blnDoZeroAlphaOnWhite;
	gboolean blnDoSaveAsFacebrick;
}QuantisationSettings;
static void query();
static void run(const gchar* strName,gint intNparams,const GimpParam* param,gint* intNreturnValues,GimpParam** returnValues);
static void zeroAlphaChannelOnWhite(GimpDrawable** drawable);
static void init_mem(guchar*** row,guchar** outrow,gint intBytesAmount);
static void zeroAlphaChannelOnWhite_row(guchar** row,guchar* outrow,gint intX1,gint intY1,gint intWidth,gint intHeight,gint intChannels,gint i);
static gboolean quantisation_dialog(GimpDrawable* drawable);
static void updatePreview(GtkFileChooser* fileChooser,gpointer data);
static gboolean facebrickFileChooser(GimpDrawable* drawable);
static gboolean exportLaFacjataFacebrick(GtkWidget* widget,QuantisationSettings* quantisationSettings);
static void whatsDone();
static QuantisationSettings quantisationSettings={90/*intColors*/,NULL/*drawable*/,NULL/*strFilename*/,TRUE/*blnDoQuantisation*/,TRUE/*blnDoZeroAlphaOnWhite*/,TRUE/*blnDoSaveAsFacebrick*/};
static char strLaFacjataPath[]="/home/user/soft/LaFacjata/";

GimpPlugInInfo PLUG_IN_INFO={NULL,NULL,query,run};

MAIN()

static void query(){
	static GimpParamDef args[] ={{
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
			"plug-in-lafacjataexportfacebrick",
			"LaFacjata export facebrick",
			"LaFacjata export facebrick",
			"Brian Cobrickowsky",
			"Have fun",
			"2004",
			"_LaFacjata export facebrick...",
			"RGB*,GRAY*",
			GIMP_PLUGIN,
			G_N_ELEMENTS(args),0,
			args,NULL);
		gimp_plugin_menu_register("plug-in-lafacjataexportfacebrick","<Image>/Filters/Misc");
}
static void run(const gchar* strName,gint intNparams,const GimpParam* param,gint* intNreturnValues,GimpParam** returnValues){
static GimpParam  values[1];
	GimpPDBStatusType status=GIMP_PDB_SUCCESS;
	GimpRunMode runMode;
	GimpDrawable* drawable;
	/*Mandatory output values*/
	*intNreturnValues=1;
	*returnValues=values;
	values[0].type=GIMP_PDB_STATUS;
	values[0].data.d_status=status;
	runMode=param[0].data.d_int32;
	drawable=gimp_drawable_get(param[2].data.d_drawable);
	switch(runMode){
		case GIMP_RUN_INTERACTIVE:
		gimp_get_data("plug-in-lafacjataexportfacebrick",&quantisationSettings);
		if(!quantisation_dialog(drawable))
			return;
		break;
		case GIMP_RUN_NONINTERACTIVE:
			if(intNparams!=4)
				status=GIMP_PDB_CALLING_ERROR;
			if(status==GIMP_PDB_SUCCESS)
				quantisationSettings.intColors= param[3].data.d_int32;
		break;
		case GIMP_RUN_WITH_LAST_VALS:
			/*Get options last values if needed*/
			gimp_get_data("plug-in-lafacjataexportfacebrick",&quantisationSettings);
		break;
		default:
		break;
	}
	if(TRUE==quantisationSettings.blnDoQuantisation){
		printf("Colors quantisation will be implemented in the future versions.");
		//g_message("Colors quantisation will be implemented in the future versions.");
	}
	if(TRUE==quantisationSettings.blnDoZeroAlphaOnWhite)
		zeroAlphaChannelOnWhite(&drawable);
	if(runMode==GIMP_RUN_INTERACTIVE){
		gimp_set_data("plug-in-lafacjataexportfacebrick",&quantisationSettings,sizeof(QuantisationSettings));
		if(TRUE==quantisationSettings.blnDoSaveAsFacebrick)
			facebrickFileChooser(drawable);
	}
	quantisationSettings.drawable=drawable;
	whatsDone();
	gimp_displays_flush();
	gimp_drawable_detach(drawable);
	return;
}
static void zeroAlphaChannelOnWhite(GimpDrawable** drawable){
	gint i,ii,intChannels;
	gint intX1,intY1,intX2,intY2;
	GimpPixelRgn rgn_in,rgn_out;
	guchar** row;
	guchar* outrow;
	gint intWidth,intHeight;
	gimp_progress_init("Zero alpha channel on white...");
	gimp_drawable_mask_bounds((*drawable)->drawable_id,&intX1,&intY1,&intX2,&intY2);
	intWidth=intX2-intX1;
	intHeight=intY2-intY1;
	intChannels=gimp_drawable_bpp((*drawable)->drawable_id);
	g_message("intChannels=%d",intChannels);
	if(intChannels<4){
		guint32 intImage2=gimp_drawable_get_image((*drawable)->drawable_id);
		guint32 intPreviousActiveLayer=gimp_image_get_active_layer(intImage2);
		gimp_edit_copy((*drawable)->drawable_id);
		gimp_edit_paste((*drawable)->drawable_id,TRUE);
		guint32 intActiveLayer=gimp_image_get_active_layer(intImage2);
		gimp_item_set_name(intActiveLayer,"LaFacjata/facebrick.png");
		gimp_image_remove_layer(intImage2,intPreviousActiveLayer);
		gimp_displays_flush();
		intActiveLayer=gimp_image_get_active_layer(intImage2);
		gint32 intActiveDrawable=gimp_image_get_active_drawable(intImage2);
		*drawable=gimp_drawable_get(intActiveDrawable);
		intChannels=gimp_drawable_bpp((*drawable)->drawable_id);
		g_message("Converted to 4 channels. After conversion: intChannels=%d",intChannels);
	}
	quantisationSettings.drawable=*drawable;
	gimp_tile_cache_ntiles(2*((*drawable)->width/gimp_tile_width()+1));
	/*Initialises two PixelRgns, one to read original data,
	and the other to write output data. That second one will
	be merged at the end by the call to
	gimp_drawable_merge_shadow()*/
	gimp_pixel_rgn_init(&rgn_in,(*drawable),intX1,intY1,intWidth,intHeight,FALSE,FALSE);
	gimp_pixel_rgn_init(&rgn_out,(*drawable),intX1,intY1,intWidth,intHeight,TRUE,TRUE);
	init_mem(&row,&outrow,intWidth*intChannels);
	for(i=0;i<intHeight;i++)
	{
		gimp_pixel_rgn_get_row(&rgn_in,row[0],intX1,i+intY1,intWidth);
		zeroAlphaChannelOnWhite_row(row,outrow,intX1, intY1,intWidth,intHeight,intChannels,i);
		gimp_pixel_rgn_set_row(&rgn_out,outrow,intX1,i+intY1,intWidth);
		if(i%10==0)
			gimp_progress_update((gdouble)i/(gdouble)intHeight);
	}
	g_free(row[0]);

	g_free(row);
	g_free(outrow);
	/*Update the modified region*/
	gimp_drawable_flush((*drawable));
	gimp_drawable_merge_shadow((*drawable)->drawable_id,TRUE);
	gimp_drawable_update((*drawable)->drawable_id,intX1,intY1,intWidth,intHeight);
}
static void init_mem(guchar*** row,guchar** outrow,gint intBytesAmount){
	gint i;
	*row=g_new(guchar*,1);
	/*usually pixel crunching on many input rows*/
	(*row)[0]=g_new(guchar,intBytesAmount);
	*outrow=g_new(guchar,intBytesAmount);
}
static void zeroAlphaChannelOnWhite_row(guchar** row,guchar* outrow,gint intX1,gint intY1,gint intWidth,gint intHeight,gint intChannels,gint i){
	gint j;
	for(j=0;j<intWidth;j++)
	{
		gint k,ii,jj;
		gint left=(j-quantisationSettings.intColors),right=(j+quantisationSettings.intColors);
		for(k=0;k<intChannels;k++)
		{
			if(3==k%4){

				if((240<row[0][intChannels*j+k-3])&&(240<row[0][intChannels*j+k-2])&&(240<row[0][intChannels*j+k-1])){
					outrow[intChannels*j+k]=0;
					outrow[intChannels*j+k-1]=row[0][intChannels*j+k-1];
					outrow[intChannels*j+k-2]=row[0][intChannels*j+k-2];
					outrow[intChannels*j+k-3]=row[0][intChannels*j+k-3];
				}else{
					outrow[intChannels*j+k]=row[0][intChannels*j+k];
					outrow[intChannels*j+k-1]=row[0][intChannels*j+k-1];
					outrow[intChannels*j+k-2]=row[0][intChannels*j+k-2];
					outrow[intChannels*j+k-3]=row[0][intChannels*j+k-3];
				}
			}
		}
	}
}
static void whatsDone(){
	GimpDrawable* drawable=quantisationSettings.drawable;
	gint32 intImage;
	intImage=gimp_drawable_get_image(drawable->drawable_id);
	gint32 intWidth;
	gint32 intHeight;
	intWidth=gimp_image_width(intImage);
	intHeight=gimp_image_height(intImage);
	gint32 intChannels=gimp_drawable_bpp(drawable->drawable_id);
	char* strWhatsDone=(char*)malloc(123*sizeof(char));
	if(TRUE==quantisationSettings.blnDoQuantisation)
		strncpy(strWhatsDone+0,"TRUE; ",123);
	if(FALSE==quantisationSettings.blnDoQuantisation)
		strncpy(strWhatsDone+0,"FALSE;",123);
	if(TRUE==quantisationSettings.blnDoZeroAlphaOnWhite)
		strncpy(strWhatsDone+6,"TRUE; ",123);
	if(FALSE==quantisationSettings.blnDoZeroAlphaOnWhite)
		strncpy(strWhatsDone+6,"FALSE;",123);
	if(TRUE==quantisationSettings.blnDoSaveAsFacebrick)
		strncpy(strWhatsDone+12,"TRUE; ",123);
	if(FALSE==quantisationSettings.blnDoSaveAsFacebrick)
		strncpy(strWhatsDone+12,"FALSE;",123);
	g_message("%s: LaFacjata face brick exported; intWidth=%d, intHeight=%d, intChannels=%d",strWhatsDone,intWidth,intHeight,intChannels);
	free(strWhatsDone);
}
static gboolean exportLaFacjataFacebrick(GtkWidget* widget,QuantisationSettings* quantisationSettings){
	if(NULL==quantisationSettings->strFilename)return FALSE;
	GimpDrawable* drawable=quantisationSettings->drawable;
	char* strFilename=quantisationSettings->strFilename;
	gint32 intImage;
	//g_message("Export LaFacjata facebrick: for opaque images use e.x. png format.");
	intImage=gimp_drawable_get_image(drawable->drawable_id);
	gimp_file_save(0,intImage,drawable->drawable_id,strFilename,strFilename);
	g_free(quantisationSettings->strFilename);
	return TRUE;
}
static void updatePreview(GtkFileChooser* fileChooser,gpointer data)
{
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
	GtkFileChooserAction action=GTK_FILE_CHOOSER_ACTION_SAVE;
	gint intResult;

	GtkWidget* parentWindow;
	GtkWidget* preview;
	gimp_ui_init("lafacjataexportfacebrick",FALSE);
	parentWindow=gimp_dialog_new("LaFacjata insert facebrick", "lafacjataexportfacebrick",NULL,0,gimp_standard_help_func,"plug-in-lafacjataexportfacebrick",GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);

	dialog=gtk_file_chooser_dialog_new("Save File",GTK_WINDOW(parentWindow),action,("_Cancel"),GTK_RESPONSE_CANCEL,("_Save"),GTK_RESPONSE_ACCEPT,NULL);

	gboolean blnResult=gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),strLaFacjataPath);
	gboolean blnPreview;
	preview=gtk_image_new();
	g_signal_connect(dialog,"update-preview",G_CALLBACK(updatePreview),preview);
	gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(dialog),preview);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),TRUE);
	intResult=gtk_dialog_run(GTK_DIALOG(dialog));
	g_signal_connect(dialog,"destroy",G_CALLBACK(exportLaFacjataFacebrick),&quantisationSettings);
	if(intResult==GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_hide_all(dialog);
		char* strFilename;
		GtkFileChooser* chooser=GTK_FILE_CHOOSER(dialog);
		strFilename=gtk_file_chooser_get_filename(chooser);
		quantisationSettings.drawable=drawable;
		quantisationSettings.strFilename=strFilename;
		gtk_widget_destroy(dialog);
		//exportLaFacjataFacebrick(&quantisationSettings);
		return TRUE;
	}else{
		quantisationSettings.drawable=NULL;
		quantisationSettings.strFilename=NULL;
		gtk_widget_destroy(dialog);
		return FALSE;
	}
}
void colorsQuantisationWillBeImplementedInTheFuture(GtkToggleButton *togglebutton,gpointer user_data){
	g_message("Colors quantisation will be implemented in the future versions.");
	gtk_toggle_button_set_active(togglebutton,FALSE);
}
static gboolean quantisation_dialog(GimpDrawable* drawable){
	GtkWidget* dialog;
	GtkWidget* quantisationVbox;
	GtkWidget* quantisationHbox;
	GtkWidget* frameQuantisation;
	GtkWidget* chkDoQuantisation;
	GtkWidget* colorsLabel;
	GtkWidget* alignment;
	GtkWidget* spinbutton;
	GtkObject* spinbutton_adj;
	GtkWidget* frameQuantisationLabel;
	GtkWidget* chkDoZeroAlphaOnWhite;
	GtkWidget* chkDoSaveAsFacebrick;
	gboolean run;
	gimp_ui_init("lafacjataexportfacebrick",FALSE);
	dialog=gimp_dialog_new("LaFacjata export facebrick","lafacjataexportfacebrick",NULL,0,gimp_standard_help_func,"plug-in-lafacjataexportfacebrick",GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	quantisationVbox=gtk_vbox_new(FALSE,6);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),quantisationVbox);
	chkDoQuantisation=gtk_check_button_new_with_mnemonic("Do _quantisation");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkDoQuantisation),FALSE);
	g_signal_connect(chkDoQuantisation,"toggled",G_CALLBACK(colorsQuantisationWillBeImplementedInTheFuture),&quantisationSettings);
	gtk_box_pack_start(GTK_BOX(quantisationVbox),chkDoQuantisation,TRUE,TRUE,0);
	frameQuantisation=gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(quantisationVbox),frameQuantisation,TRUE,TRUE,0);
	gtk_container_set_border_width(GTK_CONTAINER(frameQuantisation),6);
	alignment=gtk_alignment_new(0.5,0.5,1,1);
	gtk_container_add(GTK_CONTAINER(frameQuantisation),alignment);
	gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),6,6,6,6);
	quantisationHbox=gtk_hbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(alignment),quantisationHbox);
	colorsLabel=gtk_label_new_with_mnemonic("_Colors:");
	gtk_box_pack_start(GTK_BOX(quantisationHbox),colorsLabel,FALSE,FALSE,6);
	gtk_label_set_justify(GTK_LABEL(colorsLabel),GTK_JUSTIFY_RIGHT);
	spinbutton_adj=gtk_adjustment_new(90,1,255*255*255,1,5,5);
	spinbutton=gtk_spin_button_new(GTK_ADJUSTMENT(spinbutton_adj),1,0);
	gtk_box_pack_start(GTK_BOX(quantisationHbox),spinbutton,FALSE,FALSE,6);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spinbutton),TRUE);
	frameQuantisationLabel=gtk_label_new("<b>Colors quantisation to</b>");
	gtk_frame_set_label_widget(GTK_FRAME(frameQuantisation),frameQuantisationLabel);
	gtk_label_set_use_markup(GTK_LABEL(frameQuantisationLabel),TRUE);
	g_signal_connect(spinbutton_adj,"value_changed",G_CALLBACK(gimp_int_adjustment_update),&quantisationSettings.intColors);
	chkDoZeroAlphaOnWhite=gtk_check_button_new_with_mnemonic("Do _zero alpha on white");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkDoZeroAlphaOnWhite),TRUE);
	gtk_box_pack_start(GTK_BOX(quantisationVbox),chkDoZeroAlphaOnWhite,TRUE,TRUE,0);
	chkDoSaveAsFacebrick=gtk_check_button_new_with_mnemonic("Do _save as facebrick");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkDoSaveAsFacebrick),TRUE);
	gtk_box_pack_start(GTK_BOX(quantisationVbox),chkDoSaveAsFacebrick,TRUE,TRUE,0);
	gtk_widget_show_all(dialog);
	run=(gimp_dialog_run(GIMP_DIALOG(dialog))==GTK_RESPONSE_OK);
	quantisationSettings.blnDoQuantisation=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkDoQuantisation));
	quantisationSettings.blnDoZeroAlphaOnWhite=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkDoZeroAlphaOnWhite));
	quantisationSettings.blnDoSaveAsFacebrick=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkDoSaveAsFacebrick));
	gtk_widget_destroy(dialog);
	return run;
}
