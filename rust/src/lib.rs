#![feature(libc)]
#![feature(i128_type)]
#![feature(const_ptr_null)]
#![feature(offset_to)]
#![feature(const_ptr_null_mut)]
#![feature(extern_types)]
#![feature(asm)]
#![feature(used)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(mutable_transmutes)]
#![allow(unused_mut)]
// The following allows are just here because lots
// of spam is being outputted. They are not required
#![allow(unused_unsafe)]
#![allow(unused_parens)]
#![allow(unused_assignments)]
#![allow(unused_variables)]
#![allow(unused_attributes)]

pub mod HTMLparser;
pub mod HTMLtree;
pub mod SAX;
pub mod SAX2;
pub mod buf;
pub mod c14n;
pub mod catalog;
pub mod chvalid;
pub mod debugXML;
pub mod dict;
pub mod encoding;
pub mod entities;
pub mod error;
pub mod globals;
pub mod hash;
pub mod legacy;
pub mod list;
pub mod nanoftp;
pub mod nanohttp;
pub mod parser;
pub mod parserInternals;
pub mod pattern;
pub mod relaxng;
pub mod schematron;
pub mod threads;
pub mod tree;
pub mod uri;
pub mod valid;
pub mod xinclude;
mod xlink;
pub mod xmlIO;
mod xmlcatalog;
pub mod xmlmemory;
pub mod xmlmodule;
pub mod xmlreader;
pub mod xmlregexp;
pub mod xmlsave;
pub mod xmlschemas;
pub mod xmlschemastypes;
pub mod xmlstring;
pub mod xmlunicode;
pub mod xmlwriter;
pub mod xpath;
pub mod xpointer;
pub mod xzlib;
