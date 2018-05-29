#include <libxml/xmlexports.h>
#include <libxml/xmlversion.h>
#include <libxml/xmlerror.h>
#include <string.h>

// Structure declarations
// from xmllint.c:
extern int xmllint_callbacks;
extern char xmllint_buffer[50000];
typedef enum {
    XMLLINT_RETURN_OK = 0,	/* No error */
    XMLLINT_ERR_UNCLASS = 1,	/* Unclassified */
    XMLLINT_ERR_DTD = 2,	/* Error in DTD */
    XMLLINT_ERR_VALID = 3,	/* Validation error */
    XMLLINT_ERR_RDFILE = 4,	/* CtxtReadFile error */
    XMLLINT_ERR_SCHEMACOMP = 5,	/* Schema compilation */
    XMLLINT_ERR_OUT = 6,	/* Error writing output */
    XMLLINT_ERR_SCHEMAPAT = 7,	/* Error in schema pattern */
    XMLLINT_ERR_RDREGIS = 8,	/* Error in Reader registration */
    XMLLINT_ERR_MEM = 9,	/* Out of memory error */
    XMLLINT_ERR_XPATH = 10	/* XPath evaluation error */
} xmllintReturnCode;
extern xmllintReturnCode progresult;
extern int xmllint_noout;

// Extern functions
// N/A

// from xmllint.c
#if defined(HAVE_GETTIMEOFDAY)
void XMLCDECL LIBXML_ATTR_FORMAT(1,2)
endTimer(const char *fmt, ...)
{
    long msec;
    va_list ap;

    gettimeofday(&end, NULL);
    msec = end.tv_sec - begin.tv_sec;
    msec *= 1000;
    msec += (end.tv_usec - begin.tv_usec) / 1000;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fprintf(stderr, " took %ld ms\n", msec);
}
#elif defined(HAVE_TIME_H)
void XMLCDECL LIBXML_ATTR_FORMAT(1,2)
endTimer(const char *fmt, ...)
{
    long msec;
    va_list ap;

    end = clock();
    msec = ((end - begin) * 1000) / CLOCKS_PER_SEC;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, " took %ld ms\n", msec);
}
#else
void XMLCDECL LIBXML_ATTR_FORMAT(1,2)
endTimer(char *format, ...)
{
    /*
     * We cannot do anything because we don't have a timing function
     */
#ifdef HAVE_STDARG_H
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, " was not timed\n");
#else
    /* We don't have gettimeofday, time or stdarg.h, what crazy world is
     * this ?!
     */
#endif
}
#endif

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
xmlHTMLError(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input;
    va_list args;
    int len;

    xmllint_buffer[0] = 0;
    input = ctxt->input;
    if ((input != NULL) && (input->filename == NULL) && (ctxt->inputNr > 1)) {
        input = ctxt->inputTab[ctxt->inputNr - 2];
    }

    xmlHTMLPrintFileInfo(input);

    xmlGenericError(xmlGenericErrorContext, "<b>error</b>: ");
    va_start(args, msg);
    len = strlen(xmllint_buffer);
    vsnprintf(&xmllint_buffer[len],  sizeof(xmllint_buffer) - len, msg, args);
    va_end(args);
    xmlHTMLEncodeSend();
    xmlGenericError(xmlGenericErrorContext, "</p>\n");

    xmlHTMLPrintFileContext(input);
    xmlHTMLEncodeSend();
}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
xmlHTMLWarning(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input;
    va_list args;
    int len;

    xmllint_buffer[0] = 0;
    input = ctxt->input;
    if ((input != NULL) && (input->filename == NULL) && (ctxt->inputNr > 1)) {
        input = ctxt->inputTab[ctxt->inputNr - 2];
    }


    xmlHTMLPrintFileInfo(input);

    xmlGenericError(xmlGenericErrorContext, "<b>warning</b>: ");
    va_start(args, msg);
    len = strlen(xmllint_buffer);
    vsnprintf(&xmllint_buffer[len],  sizeof(xmllint_buffer) - len, msg, args);
    va_end(args);
    xmlHTMLEncodeSend();
    xmlGenericError(xmlGenericErrorContext, "</p>\n");

    xmlHTMLPrintFileContext(input);
    xmlHTMLEncodeSend();
}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
xmlHTMLValidityError(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input;
    va_list args;
    int len;

    xmllint_buffer[0] = 0;
    input = ctxt->input;
    if ((input->filename == NULL) && (ctxt->inputNr > 1))
        input = ctxt->inputTab[ctxt->inputNr - 2];

    xmlHTMLPrintFileInfo(input);

    xmlGenericError(xmlGenericErrorContext, "<b>validity error</b>: ");
    len = strlen(xmllint_buffer);
    va_start(args, msg);
    vsnprintf(&xmllint_buffer[len],  sizeof(xmllint_buffer) - len, msg, args);
    va_end(args);
    xmlHTMLEncodeSend();
    xmlGenericError(xmlGenericErrorContext, "</p>\n");

    xmlHTMLPrintFileContext(input);
    xmlHTMLEncodeSend();
    progresult = XMLLINT_ERR_VALID;
}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
xmlHTMLValidityWarning(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input;
    va_list args;
    int len;

    xmllint_buffer[0] = 0;
    input = ctxt->input;
    if ((input->filename == NULL) && (ctxt->inputNr > 1))
        input = ctxt->inputTab[ctxt->inputNr - 2];

    xmlHTMLPrintFileInfo(input);

    xmlGenericError(xmlGenericErrorContext, "<b>validity warning</b>: ");
    va_start(args, msg);
    len = strlen(xmllint_buffer);
    vsnprintf(&xmllint_buffer[len],  sizeof(xmllint_buffer) - len, msg, args);
    va_end(args);
    xmlHTMLEncodeSend();
    xmlGenericError(xmlGenericErrorContext, "</p>\n");

    xmlHTMLPrintFileContext(input);
    xmlHTMLEncodeSend();
}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
warningDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...)
{
    va_list args;

    xmllint_callbacks++;
    if (xmllint_noout)
	return;
    va_start(args, msg);
    fprintf(stdout, "SAX.warning: ");
    vfprintf(stdout, msg, args);
    va_end(args);
}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
errorDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...)
{
    va_list args;

    xmllint_callbacks++;
    if (xmllint_noout)
	return;
    va_start(args, msg);
    fprintf(stdout, "SAX.error: ");
    vfprintf(stdout, msg, args);
    va_end(args);
}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
fatalErrorDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...)
{
    va_list args;

    xmllint_callbacks++;
    if (xmllint_noout)
	return;
    va_start(args, msg);
    fprintf(stdout, "SAX.fatalError: ");
    vfprintf(stdout, msg, args);
    va_end(args);
}
