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

// TODO: make pub after validating necessity
mod HTMLparser;
mod HTMLtree;
mod SAX;
mod SAX2;
mod buf;
mod c14n;
mod catalog;
pub mod chvalid;
mod debugXML;
pub mod dict;
mod encoding;
pub mod entities;
mod error;
mod globals;
pub mod hash;
mod legacy;
pub mod list;
mod nanoftp;
mod nanohttp;
pub mod parser;
pub mod parserInternals;
pub mod pattern;
mod relaxng;
mod schematron;
mod testdso;
pub mod threads;
pub mod tree;
pub mod uri;
pub mod valid;
mod xinclude;
mod xlink;
mod xmlIO;
mod xmlcatalog;
mod xmlmemory;
mod xmlmodule;
mod xmlreader;
pub mod xmlregexp;
mod xmlsave;
pub mod xmlschemas;
pub mod xmlschemastypes;
pub mod xmlstring;
pub mod xmlunicode;
mod xmlwriter;
mod xpath;
mod xpointer;
mod xzlib;
