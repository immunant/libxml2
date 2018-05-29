#include <libxml/xmlexports.h>
#include <libxml/xmlversion.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlschemas.h>
#include <stdlib.h>
#include <string.h>

// #include "libxml2.h"
#include "variadic.h"

// Structure declarations


// Extern functions
// from error.c:
void xmlReportError(xmlErrorPtr err, xmlParserCtxtPtr ctxt, const char *str, xmlGenericErrorFunc channel, void *data);

// from error.c:
void XMLCDECL
xmlGenericErrorDefaultFunc(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...) {
    va_list args;

    if (xmlGenericErrorContext == NULL)
	    xmlGenericErrorContext = (void *) stderr;

    va_start(args, msg);
    vfprintf((FILE *)xmlGenericErrorContext, msg, args);
    va_end(args);
}

void XMLCDECL
__xmlRaiseError(xmlStructuredErrorFunc schannel,
              xmlGenericErrorFunc channel, void *data, void *ctx,
              void *nod, int domain, int code, xmlErrorLevel level,
              const char *file, int line, const char *str1,
              const char *str2, const char *str3, int int1, int col,
	      const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = NULL;
    xmlNodePtr node = (xmlNodePtr) nod;
    char *str = NULL;
    xmlParserInputPtr input = NULL;
    xmlErrorPtr to = &xmlLastError;
    xmlNodePtr baseptr = NULL;

    if (code == XML_ERR_OK)
        return;
    if ((xmlGetWarningsDefaultValue == 0) && (level == XML_ERR_WARNING))
        return;
    if ((domain == XML_FROM_PARSER) || (domain == XML_FROM_HTML) ||
        (domain == XML_FROM_DTD) || (domain == XML_FROM_NAMESPACE) ||
	    (domain == XML_FROM_IO) || (domain == XML_FROM_VALID)) {
        ctxt = (xmlParserCtxtPtr) ctx;
        if ((schannel == NULL) && (ctxt != NULL) && (ctxt->sax != NULL) &&
            (ctxt->sax->initialized == XML_SAX2_MAGIC) &&
            (ctxt->sax->serror != NULL)) {
            schannel = ctxt->sax->serror;
            data = ctxt->userData;
        }
    }
    /*
     * Check if structured error handler set
     */
    if (schannel == NULL) {
	schannel = xmlStructuredError;
	/*
	 * if user has defined handler, change data ptr to user's choice
	 */
	if (schannel != NULL)
	    data = xmlStructuredErrorContext;
    }
    /*
     * Formatting the message
     */
    if (msg == NULL) {
        str = (char *) xmlStrdup(BAD_CAST "No error message provided");
    } else {
        XML_GET_VAR_STR(msg, str);
    }

    /*
     * specific processing if a parser context is provided
     */
    if (ctxt != NULL) {
        if (file == NULL) {
            input = ctxt->input;
            if ((input != NULL) && (input->filename == NULL) &&
                (ctxt->inputNr > 1)) {
                input = ctxt->inputTab[ctxt->inputNr - 2];
            }
            if (input != NULL) {
                file = input->filename;
                line = input->line;
                col = input->col;
            }
        }
        to = &ctxt->lastError;
    } else if ((node != NULL) && (file == NULL)) {
	int i;

	if ((node->doc != NULL) && (node->doc->URL != NULL)) {
	    baseptr = node;
/*	    file = (const char *) node->doc->URL; */
	}
	for (i = 0;
	     ((i < 10) && (node != NULL) && (node->type != XML_ELEMENT_NODE));
	     i++)
	     node = node->parent;
        if ((baseptr == NULL) && (node != NULL) &&
	    (node->doc != NULL) && (node->doc->URL != NULL))
	    baseptr = node;

	if ((node != NULL) && (node->type == XML_ELEMENT_NODE))
	    line = node->line;
	if ((line == 0) || (line == 65535))
	    line = xmlGetLineNo(node);
    }

    /*
     * Save the information about the error
     */
    xmlResetError(to);
    to->domain = domain;
    to->code = code;
    to->message = str;
    to->level = level;
    if (file != NULL)
        to->file = (char *) xmlStrdup((const xmlChar *) file);
    else if (baseptr != NULL) {
#ifdef LIBXML_XINCLUDE_ENABLED
	/*
	 * We check if the error is within an XInclude section and,
	 * if so, attempt to print out the href of the XInclude instead
	 * of the usual "base" (doc->URL) for the node (bug 152623).
	 */
        xmlNodePtr prev = baseptr;
	int inclcount = 0;
	while (prev != NULL) {
	    if (prev->prev == NULL)
	        prev = prev->parent;
	    else {
	        prev = prev->prev;
		if (prev->type == XML_XINCLUDE_START) {
		    if (--inclcount < 0)
		        break;
		} else if (prev->type == XML_XINCLUDE_END)
		    inclcount++;
	    }
	}
	if (prev != NULL) {
	    if (prev->type == XML_XINCLUDE_START) {
            prev->type = XML_ELEMENT_NODE;
            to->file = (char *) xmlGetProp(prev, BAD_CAST "href");
            prev->type = XML_XINCLUDE_START;
	    } else {
		    to->file = (char *) xmlGetProp(prev, BAD_CAST "href");
	    }
	} else
#endif
	    to->file = (char *) xmlStrdup(baseptr->doc->URL);
        if ((to->file == NULL) && (node != NULL) && (node->doc != NULL)) {
            to->file = (char *) xmlStrdup(node->doc->URL);
        }
    }
    to->line = line;
    if (str1 != NULL)
        to->str1 = (char *) xmlStrdup((const xmlChar *) str1);
    if (str2 != NULL)
        to->str2 = (char *) xmlStrdup((const xmlChar *) str2);
    if (str3 != NULL)
        to->str3 = (char *) xmlStrdup((const xmlChar *) str3);
    to->int1 = int1;
    to->int2 = col;
    to->node = node;
    to->ctxt = ctx;

    if (to != &xmlLastError)
        xmlCopyError(to,&xmlLastError);

    if (schannel != NULL) {
	    schannel(data, to);
	return;
    }

    /*
     * Find the callback channel if channel param is NULL
     */
    if ((ctxt != NULL) && (channel == NULL) &&
        (xmlStructuredError == NULL) && (ctxt->sax != NULL)) {
        if (level == XML_ERR_WARNING)
	        channel = ctxt->sax->warning;
        else
	        channel = ctxt->sax->error;
	    data = ctxt->userData;
    } else if (channel == NULL) {
	    channel = xmlGenericError;
        if (ctxt != NULL) {
            data = ctxt;
        } else {
            data = xmlGenericErrorContext;
        }
    }
    if (channel == NULL)
        return;

    if ((channel == xmlParserError) ||
        (channel == xmlParserWarning) ||
	(channel == xmlParserValidityError) ||
	(channel == xmlParserValidityWarning))
	    xmlReportError(to, ctxt, str, NULL, NULL);
    else if ((channel == (xmlGenericErrorFunc) fprintf) ||
             (channel == xmlGenericErrorDefaultFunc))
	    xmlReportError(to, ctxt, str, channel, data);
    else
	    channel(data, "%s", str);
}

void XMLCDECL
xmlParserError(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input = NULL;
    xmlParserInputPtr cur = NULL;
    char * str;

    if (ctxt != NULL) {
        input = ctxt->input;
        if ((input != NULL) && (input->filename == NULL) &&
            (ctxt->inputNr > 1)) {
            cur = input;
            input = ctxt->inputTab[ctxt->inputNr - 2];
        }
        xmlParserPrintFileInfo(input);
    }

    xmlGenericError(xmlGenericErrorContext, "error: ");
    XML_GET_VAR_STR(msg, str);
    xmlGenericError(xmlGenericErrorContext, "%s", str);
    if (str != NULL)
	    xmlFree(str);

    if (ctxt != NULL) {
        xmlParserPrintFileContext(input);
        if (cur != NULL) {
            xmlParserPrintFileInfo(cur);
            xmlGenericError(xmlGenericErrorContext, "\n");
            xmlParserPrintFileContext(cur);
        }
    }
}

void XMLCDECL
xmlParserWarning(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input = NULL;
    xmlParserInputPtr cur = NULL;
    char * str;

    if (ctxt != NULL) {
        input = ctxt->input;
        if ((input != NULL) && (input->filename == NULL) &&
            (ctxt->inputNr > 1)) {
            cur = input;
            input = ctxt->inputTab[ctxt->inputNr - 2];
        }
        xmlParserPrintFileInfo(input);
    }

    xmlGenericError(xmlGenericErrorContext, "warning: ");
    XML_GET_VAR_STR(msg, str);
    xmlGenericError(xmlGenericErrorContext, "%s", str);
    if (str != NULL)
	    xmlFree(str);

    if (ctxt != NULL) {
        xmlParserPrintFileContext(input);
        if (cur != NULL) {
            xmlParserPrintFileInfo(cur);
            xmlGenericError(xmlGenericErrorContext, "\n");
            xmlParserPrintFileContext(cur);
        }
    }
}

void XMLCDECL
xmlParserValidityError(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input = NULL;
    char * str;
    int len = xmlStrlen((const xmlChar *) msg);
    static int had_info = 0;

    if ((len > 1) && (msg[len - 2] != ':')) {
        if (ctxt != NULL) {
            input = ctxt->input;
            if ((input->filename == NULL) && (ctxt->inputNr > 1))
            input = ctxt->inputTab[ctxt->inputNr - 2];

            if (had_info == 0) {
            xmlParserPrintFileInfo(input);
            }
        }
        xmlGenericError(xmlGenericErrorContext, "validity error: ");
        had_info = 0;
    } else {
	    had_info = 1;
    }

    XML_GET_VAR_STR(msg, str);
    xmlGenericError(xmlGenericErrorContext, "%s", str);
    if (str != NULL)
	    xmlFree(str);

    if ((ctxt != NULL) && (input != NULL)) {
	    xmlParserPrintFileContext(input);
    }
}

void XMLCDECL
xmlParserValidityWarning(void *ctx, const char *msg, ...)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlParserInputPtr input = NULL;
    char * str;
    int len = xmlStrlen((const xmlChar *) msg);

    if ((ctxt != NULL) && (len != 0) && (msg[len - 1] != ':')) {
        input = ctxt->input;
        if ((input->filename == NULL) && (ctxt->inputNr > 1))
            input = ctxt->inputTab[ctxt->inputNr - 2];

        xmlParserPrintFileInfo(input);
    }

    xmlGenericError(xmlGenericErrorContext, "validity warning: ");
    XML_GET_VAR_STR(msg, str);
    xmlGenericError(xmlGenericErrorContext, "%s", str);
    if (str != NULL)
	    xmlFree(str);

    if (ctxt != NULL) {
	    xmlParserPrintFileContext(input);
    }
}

// from valid.c:
void XMLCDECL xmlNoValidityErr(void *ctx ATTRIBUTE_UNUSED,
                               const char *msg ATTRIBUTE_UNUSED, ...) {
    return;
}

// from xmlreader.c:
void XMLCDECL
xmlTextReaderValidityErrorRelay(void *ctx, const char *msg, ...)
{
    xmlTextReaderPtr reader = (xmlTextReaderPtr) ctx;

    char *str;

    va_list ap;

    va_start(ap, msg);
    str = xmlTextReaderBuildMessage(msg, ap);
    if (!reader->errorFunc) {
        xmlTextReaderValidityError(ctx, "%s", str);
    } else {
        reader->errorFunc(reader->errorFuncArg, str,
                          XML_PARSER_SEVERITY_VALIDITY_ERROR,
                          NULL /* locator */ );
    }
    if (str != NULL)
        xmlFree(str);
    va_end(ap);
}

void XMLCDECL
xmlTextReaderValidityWarningRelay(void *ctx, const char *msg, ...)
{
    xmlTextReaderPtr reader = (xmlTextReaderPtr) ctx;

    char *str;

    va_list ap;

    va_start(ap, msg);
    str = xmlTextReaderBuildMessage(msg, ap);
    if (!reader->errorFunc) {
        xmlTextReaderValidityWarning(ctx, "%s", str);
    } else {
        reader->errorFunc(reader->errorFuncArg, str,
                          XML_PARSER_SEVERITY_VALIDITY_WARNING,
                          NULL /* locator */ );
    }
    if (str != NULL)
        xmlFree(str);
    va_end(ap);
}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
xmlTextReaderError(void *ctxt, const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    xmlTextReaderGenericError(ctxt,
                              XML_PARSER_SEVERITY_ERROR,
                              xmlTextReaderBuildMessage(msg, ap));
    va_end(ap);

}

void XMLCDECL LIBXML_ATTR_FORMAT(2,3)
xmlTextReaderWarning(void *ctxt, const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    xmlTextReaderGenericError(ctxt,
                              XML_PARSER_SEVERITY_WARNING,
                              xmlTextReaderBuildMessage(msg, ap));
    va_end(ap);
}

void XMLCDECL
xmlTextReaderValidityError(void *ctxt, const char *msg, ...)
{
    va_list ap;

    int len = xmlStrlen((const xmlChar *) msg);

    if ((len > 1) && (msg[len - 2] != ':')) {
        /*
         * some callbacks only report locator information:
         * skip them (mimicking behaviour in error.c)
         */
        va_start(ap, msg);
        xmlTextReaderGenericError(ctxt,
                                  XML_PARSER_SEVERITY_VALIDITY_ERROR,
                                  xmlTextReaderBuildMessage(msg, ap));
        va_end(ap);
    }
}

void XMLCDECL
xmlTextReaderValidityWarning(void *ctxt, const char *msg, ...)
{
    va_list ap;

    int len = xmlStrlen((const xmlChar *) msg);

    if ((len != 0) && (msg[len - 1] != ':')) {
        /*
         * some callbacks only report locator information:
         * skip them (mimicking behaviour in error.c)
         */
        va_start(ap, msg);
        xmlTextReaderGenericError(ctxt,
                                  XML_PARSER_SEVERITY_VALIDITY_WARNING,
                                  xmlTextReaderBuildMessage(msg, ap));
        va_end(ap);
    }
}

// from xmlchemas.c:
#define TODO								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);
void XMLCDECL
warningSplit(void *ctx, const char *msg ATTRIBUTE_UNUSED, ...) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->warning != NULL)) {
	TODO
    }
}
void XMLCDECL
errorSplit(void *ctx, const char *msg ATTRIBUTE_UNUSED, ...) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->error != NULL)) {
	TODO
    }
}
void XMLCDECL
fatalErrorSplit(void *ctx, const char *msg ATTRIBUTE_UNUSED, ...) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->fatalError != NULL)) {
	TODO
    }
}

// from xmlstring.c:
int XMLCDECL
xmlStrPrintf(xmlChar *buf, int len, const char *msg, ...) {
    va_list args;
    int ret;

    if((buf == NULL) || (msg == NULL)) {
        return(-1);
    }

    va_start(args, msg);
    ret = vsnprintf((char *) buf, len, (const char *) msg, args);
    va_end(args);
    buf[len - 1] = 0; /* be safe ! */

    return(ret);
}

// from xmlwriter.c:
int XMLCDECL
xmlTextWriterWriteFormatComment(xmlTextWriterPtr writer,
                                const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatComment(writer, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatRaw(xmlTextWriterPtr writer, const char *format,
                            ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatRaw(writer, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatString(xmlTextWriterPtr writer, const char *format,
                               ...)
{
    int rc;
    va_list ap;

    if ((writer == NULL) || (format == NULL))
        return -1;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatString(writer, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatAttribute(xmlTextWriterPtr writer,
                                  const xmlChar * name, const char *format,
                                  ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatAttribute(writer, name, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatAttributeNS(xmlTextWriterPtr writer,
                                    const xmlChar * prefix,
                                    const xmlChar * name,
                                    const xmlChar * namespaceURI,
                                    const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatAttributeNS(writer, prefix, name,
                                              namespaceURI, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatElement(xmlTextWriterPtr writer,
                                const xmlChar * name, const char *format,
                                ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatElement(writer, name, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatElementNS(xmlTextWriterPtr writer,
                                  const xmlChar * prefix,
                                  const xmlChar * name,
                                  const xmlChar * namespaceURI,
                                  const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatElementNS(writer, prefix, name,
                                            namespaceURI, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatPI(xmlTextWriterPtr writer, const xmlChar * target,
                           const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatPI(writer, target, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatCDATA(xmlTextWriterPtr writer, const char *format,
                              ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatCDATA(writer, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatDTD(xmlTextWriterPtr writer,
                            const xmlChar * name,
                            const xmlChar * pubid,
                            const xmlChar * sysid, const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatDTD(writer, name, pubid, sysid, format,
                                      ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatDTDElement(xmlTextWriterPtr writer,
                                   const xmlChar * name,
                                   const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatDTDElement(writer, name, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatDTDAttlist(xmlTextWriterPtr writer,
                                   const xmlChar * name,
                                   const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatDTDAttlist(writer, name, format, ap);

    va_end(ap);
    return rc;
}
int XMLCDECL
xmlTextWriterWriteFormatDTDInternalEntity(xmlTextWriterPtr writer,
                                          int pe,
                                          const xmlChar * name,
                                          const char *format, ...)
{
    int rc;
    va_list ap;

    va_start(ap, format);

    rc = xmlTextWriterWriteVFormatDTDInternalEntity(writer, pe, name,
                                                    format, ap);

    va_end(ap);
    return rc;
}
