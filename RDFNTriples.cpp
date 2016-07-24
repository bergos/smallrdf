#include "RDF.h"
#include "RDFNTriples.h"

#include <stdio.h>

NTriplesParser::NTriplesParser () : _document(), _buf(NULL), _cur(NULL), _end(NULL){}

RDFDocument* NTriplesParser::parse (const uint8_t* buf, size_t length) {
  RDFDocument* document = _document = new RDFDocument();
  _buf = _cur = buf;
  _end = _buf + length;

  while(hasNext()) {
    readWhiteSpace();

    if (hasNext()) {
      readTriple();
    }
  }

  _document = NULL;
  _buf = _cur = _end = NULL;

  return document;
}

bool NTriplesParser::hasNext () {
  return _cur < _end;
}

const uint8_t NTriplesParser::getNext (bool keep) {
  if (keep) {
    return *_cur;
  } else {
	return *_cur++;
  }
}

bool NTriplesParser::readWhiteSpace () {
  bool read = false;
  uint8_t next = getNext(true);

  while(hasNext() && (next == ' ' || next == '\t' || next == '\n' || next == '\r')) {
    read = true;

    getNext();

	next = getNext(true);
  }

  return read;
}

RDFQuad* NTriplesParser::readTriple () {
  RDFTerm* subject = readSubject();

  readWhiteSpace();

  RDFTerm* predicate = readPredicate();

  readWhiteSpace();
  
  RDFTerm* object = readObject();

  readWhiteSpace();

  if (getNext(true) == '.') {
    getNext();
  }

  return _document->triple(subject, predicate, object);
}

RDFTerm* NTriplesParser::readSubject () {
  if (isIRIREF()) {
	return _document->namedNode(readIRIREF());
  } else {
	return NULL;
  }
}

RDFTerm* NTriplesParser::readPredicate () {
  if (isIRIREF()) {
	return _document->namedNode(readIRIREF());
  } else {
	return NULL;
  }
}

RDFTerm* NTriplesParser::readObject () {
  if (isIRIREF()) {
	return _document->namedNode(readIRIREF());
  } else if (isSTRING_LITERAL_QUOTE()) {
    return _document->literal(readSTRING_LITERAL_QUOTE());
  } else if (isBLANK_NODE_LABEL()) {
    return _document->blankNode(readBLANK_NODE_LABEL());
  } else {
	return NULL;
  }
}

bool NTriplesParser::isIRIREF () {	
  return getNext(true) == '<';
}

RDFString NTriplesParser::readIRIREF () {
  const uint8_t* buf = ++_cur;
  size_t length = 0;

  while (hasNext() && getNext() != '>') {
    length++;
  }

  return RDFString(buf, length);
}

bool NTriplesParser::isSTRING_LITERAL_QUOTE() {
  return getNext(true) == '"';
}

RDFString NTriplesParser::readSTRING_LITERAL_QUOTE() {
  const uint8_t* buf = ++_cur;
  size_t length = 0;

  while (hasNext() && getNext() != '"') {
    length++;
  }

  return RDFString(buf, length);
}

bool NTriplesParser::isBLANK_NODE_LABEL() {
  return getNext(true) == '_'; // TODO: support for more getahead
}

RDFString NTriplesParser::readBLANK_NODE_LABEL() {
  _cur += 2;

  const uint8_t* buf = _cur;
  size_t length = 0;

  while (hasNext() && !readWhiteSpace() && getNext(true) != '.') {
    getNext();
    length++;
  }

  return RDFString(buf, length);
}

NTriplesSerializer::NTriplesSerializer() : _buf(NULL), _cur(NULL), _end(NULL), _length(0) {}

const uint8_t* NTriplesSerializer::serialize(RDFDocument* document) {
  _length = documentSize(document);
  _buf = _cur = new uint8_t[_length];
  _end = _cur + _length;

  serializeDocument(document);

  return _buf;
}

void NTriplesSerializer::write(char character) {
	//TODO check eof
  _cur[0] = character;
  _cur++;
}

void NTriplesSerializer::write(RDFString* string) {
  rdf_memcpy(_cur, string->buf, string->length);
  _cur += string->length;
}

size_t NTriplesSerializer::termSize(RDFTerm* term) {
  switch(term->termType) {
    case RDF_NAMED_NODE:
      return term->value.length + 2;
    case RDF_LITERAL:
      return term->value.length + 2;
    case RDF_BLANK_NODE:
      return term->value.length + 2;
    default:
      return 0;
  }
}

size_t NTriplesSerializer::quadSize(RDFQuad* quad) {
  return termSize(quad->subject) + termSize(quad->predicate) + termSize(quad->object) + termSize(quad->graph) + 5;
}

size_t NTriplesSerializer::documentSize(RDFDocument* document) {
  size_t size = 1;

  for (int i=0; i<document->quads.length; i++) {
    size += quadSize(document->quads.get(i));
  }

  return size;
}


void NTriplesSerializer::serializeTerm(RDFTerm* term) {
  switch (term->termType) {
    case RDF_NAMED_NODE:
      write('<');
      write(&term->value);
      write('>');
      return;
    case RDF_LITERAL:
      write('"');
      write(&term->value);
      write('"');
      return;
    case RDF_BLANK_NODE:
      write('_');
      write(':');
      write(&term->value);
      return;
    default:
      return;
  }
}

void NTriplesSerializer::serializeQuad(RDFQuad* quad) {
  serializeTerm(quad->subject);
  write(' ');
  serializeTerm(quad->predicate);
  write(' ');
  serializeTerm(quad->object);
  write(' ');
  write('.');
  write('\n');
}

void NTriplesSerializer::serializeDocument(RDFDocument* document) {
  for (int i=0; i<document->quads.length; i++) {
    serializeQuad(document->quads.get(i));
  }

  write((char)0);
}
