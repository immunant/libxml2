#![feature(libc)]
#![feature(i128_type)]
#![feature(const_ptr_null)]
#![feature(offset_to)]
#![feature(const_ptr_null_mut)]
#![feature(extern_types)]
#![feature(asm)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(mutable_transmutes)]
#![allow(unused_mut)]

// TODO: make pub after validating necessity
mod HTMLparser;
mod HTMLtree;
mod SAX;
mod SAX2;
mod buf;
mod c14n;
mod catalog;
mod chvalid;
mod debugXML;
mod dict;
mod encoding;
mod entities;
mod error;
mod globals;
mod hash;
mod legacy;
mod list;
mod nanoftp;
mod nanohttp;
mod parser;
mod parserInternals;
mod pattern;
mod relaxng;
mod schematron;
mod testdso;
mod threads;
mod tree;
mod uri;
mod valid;
mod xinclude;
mod xlink;
mod xmlIO;
mod xmlcatalog;
mod xmllint;
mod xmlmemory;
mod xmlmodule;
mod xmlreader;
mod xmlregexp;
mod xmlsave;
mod xmlschemas;
mod xmlschemastypes;
mod xmlstring;
mod xmlunicode;
mod xmlwriter;
mod xpath;
mod xpointer;
mod xzlib;
