#ifndef __LIBXML2_XMLLINT_VARIADIC
#define __LIBXML2_XMLLINT_VARIADIC

// from xmllint.c:
void XMLCDECL LIBXML_ATTR_FORMAT(1,2) endTimer(char *format, ...);
void XMLCDECL LIBXML_ATTR_FORMAT(2,3) xmlHTMLError(void *ctx, const char *msg, ...);
void XMLCDECL LIBXML_ATTR_FORMAT(2,3) xmlHTMLWarning(void *ctx, const char *msg, ...);
void XMLCDECL LIBXML_ATTR_FORMAT(2,3) xmlHTMLValidityError(void *ctx, const char *msg, ...);
void XMLCDECL LIBXML_ATTR_FORMAT(2,3) xmlHTMLValidityWarning(void *ctx, const char *msg, ...);
void XMLCDECL LIBXML_ATTR_FORMAT(2,3) warningDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...);
void XMLCDECL LIBXML_ATTR_FORMAT(2,3) errorDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...);
void XMLCDECL LIBXML_ATTR_FORMAT(2,3) fatalErrorDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...);

#endif
