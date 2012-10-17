// TextToPDFArchive.c : Defines the entry point for the console application.
//

#include "stdafx.h"

jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);

    longjmp(env, 1);
}

HPDF_Page SetupNewPage(HPDF_Doc pdf, HPDF_Font font)
{
    HPDF_REAL height;
    HPDF_REAL width;
    HPDF_Page page;
    page = HPDF_AddPage (pdf);

    height = HPDF_Page_GetHeight (page);
    width = HPDF_Page_GetWidth (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 7);
    HPDF_Page_MoveTextPos (page, 20, height - 20);
    HPDF_Page_SetTextLeading (page, 12);
    
    return page;
}

void addColorOutputIntent(HPDF_Doc pdf)
{ 
    HPDF_OutputIntent icc;
    HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
    unsigned int size = SizeofResource(NULL, resource);
   
    HGLOBAL resource_data = LoadResource(NULL, resource);
    void* icc_data = LockResource(resource_data);

    icc = HPDF_DictStream_New (pdf->mmgr, pdf->xref);
    HPDF_Dict_AddNumber (icc, "N", 1); // 1 is DeviceGray
    HPDF_Dict_AddName (icc, "Alternate", "DeviceGray");
    HPDF_Stream_Write (icc->stream, (HPDF_BYTE*)icc_data, size);
    HPDF_PDFA_AppendOutputIntents(pdf, "DeviceGray", icc);
}

int main (int argc, char **argv)
{
    const int MAX_LINE_LENGTH = 1024;
    HPDF_Doc  pdf;
    char out_fname[1024];
    char in_fname[1024];
    HPDF_Page page;
    HPDF_Font def_font;
    FILE* in_file = NULL;
    char text_line[1024];
    char* font_path;
    const char* font_name;
    HPDF_STATUS status;
    HPDF_Date hpdfDate;
    struct tm *current_time = NULL;                   
    time_t t_val = 0;
    int length;
    int lines = 0;
    
    time(&t_val);
    current_time = gmtime(&t_val);

    if (argc < 3) 
    {
        printf("Please provide source and destination files.");
        return 1;
    }

    strcpy (in_fname, argv[1]);
    strcpy (out_fname, argv[2]);

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }

    // Metadata
    HPDF_SetInfoAttr(pdf, HPDF_INFO_CREATOR, "TextToPDFArchive");
    HPDF_SetInfoAttr(pdf, HPDF_INFO_PRODUCER, "TextToPDFArchive");

    hpdfDate.year        = current_time->tm_year + 1900;
    hpdfDate.month       = current_time->tm_mon + 1;
    hpdfDate.day         = current_time->tm_mday;
    hpdfDate.hour        = current_time->tm_hour;
    hpdfDate.minutes     = current_time->tm_min;
    hpdfDate.seconds     = current_time->tm_sec;
    hpdfDate.ind         = '+';
    hpdfDate.off_hour    = 0;
    hpdfDate.off_minutes = 0;
    HPDF_SetInfoDateAttr(pdf, HPDF_INFO_CREATION_DATE, hpdfDate);
    HPDF_SetInfoDateAttr(pdf, HPDF_INFO_MOD_DATE, hpdfDate);

    // Embed the default font
    font_path = getenv("SystemRoot");
    font_path = strcat(font_path, "\\Fonts\\cour.ttf");
    font_name = HPDF_LoadTTFontFromFile(pdf, font_path, HPDF_TRUE);
    def_font = HPDF_GetFont(pdf, font_name, "WinAnsiEncoding");
    
    HPDF_SetCompressionMode(pdf, HPDF_COMP_NONE);
    page = SetupNewPage(pdf, def_font);

    in_file = fopen(in_fname, "rt");
    if(in_file != NULL)
    {
       while(feof(in_file) == 0)
       {
          fgets(text_line, MAX_LINE_LENGTH, in_file);

          length = strlen(text_line);
          text_line[length - 1] = '\0'; // Eat line feeds

          if (lines > 115 || (int)text_line[0] == 12) // Encountered page feed characters
          {
            text_line[0] = ' ';
            HPDF_Page_EndText(page);
            page = SetupNewPage(pdf, def_font);
            lines = 0;
          }

          if (feof(in_file) == 0) 
          {
            HPDF_Page_ShowTextNextLine(page, text_line);
            lines++;
          }
       }
       fclose(in_file);
    }
    else
    {
       printf("Could not open input file.");
       HPDF_Free(pdf);
       return 1;
    }
    HPDF_Page_EndText(page);

    status = HPDF_PDFA_SetPDFAConformance(pdf, HPDF_PDFA_1B);
    if (status != HPDF_OK)
    {
      printf("Could not convert to PDF/A.");
      return 1;
    }

    addColorOutputIntent(pdf);

    HPDF_SaveToFile(pdf, out_fname);
    HPDF_Free(pdf);

    return 0;
}