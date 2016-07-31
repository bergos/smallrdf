/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#include "RDFNTriplesParser.h"

NTriplesParser::NTriplesParser(const RDFString* input, RDFDocument* document)
    : document(document),
      _buf(input->buffer()),
      _cur(input->buffer()),
      _end(input->buffer() + input->length()) {
  if (document == 0) {
    this->document = new RDFDocument();
  }
}

RDFDocument* NTriplesParser::parse() {
  while (hasNext()) {
    parseQuad();
  }

  return document;
}

RDFDocument* NTriplesParser::parse_static(const RDFString* input) {
  NTriplesParser parser(input);

  return parser.parse();
}

RDFQuad* NTriplesParser::parseQuad() {
  readWhiteSpace();

  RDFTerm* subject = readSubject();

  readWhiteSpace();

  RDFTerm* predicate = readPredicate();

  readWhiteSpace();

  RDFTerm* object = readObject();

  readWhiteSpace();

  if (getNext(true) == '.') {
    getNext();
  }

  if (subject && predicate && object) {
    return document->triple(subject, predicate, object);
  } else {
    return 0;
  }
}

RDFQuad* NTriplesParser::parseQuad_static(RDFString* input,
                                                RDFDocument* document) {
  NTriplesParser parser(input, document);

  return parser.parseQuad();
}

bool NTriplesParser::hasNext() {
  return _cur < _end;
}

uint8_t NTriplesParser::getNext(const bool keep) {
  if (keep) {
    return *_cur;
  } else {
    return *_cur++;
  }
}

bool NTriplesParser::readWhiteSpace() {
  bool read = false;
  uint8_t next = getNext(true);

  while (hasNext()
      && (next == ' ' || next == '\t' || next == '\n' || next == '\r')) {
    read = true;

    getNext();

    next = getNext(true);
  }

  return read;
}

RDFTerm* NTriplesParser::readSubject() {
  if (isIRIRef()) {
    return document->namedNode(readIRIRef());
  } else {
    return 0;
  }
}

RDFTerm* NTriplesParser::readPredicate() {
  if (isIRIRef()) {
    return document->namedNode(readIRIRef());
  } else {
    return 0;
  }
}

RDFTerm* NTriplesParser::readObject() {
  if (isIRIRef()) {
    return document->namedNode(readIRIRef());
  } else if (isLiteral()) {
    return readLiteral();
  } else if (isBlankNodeLabel()) {
    return document->blankNode(readBlankNodeLabel());
  } else {
    return 0;
  }
}

bool NTriplesParser::isLiteral() {
  return isStringLiteralQuote();
}

RDFLiteral* NTriplesParser::readLiteral() {
  RDFString* value = readStringLiteralQuote();
  RDFString* language = readLangtag();

  // TODO(@bergos) check for ^^
  if (getNext(true) == '^') {
    getNext();
    getNext();
  }

  RDFString* datatype = readIRIRef();

  return document->literal(value, language, datatype);
}

RDFString* NTriplesParser::readLangtag() {
  if (getNext(true) == '@') {
    return 0;
  }

  const uint8_t* buf = ++_cur;
  size_t length = 0;

  while (hasNext() && !readWhiteSpace()) {
    getNext();
    length++;
  }

  return document->string(buf, length);
}

bool NTriplesParser::isIRIRef() {
  return getNext(true) == '<';
}

RDFString* NTriplesParser::readIRIRef() {
  if (!isIRIRef()) {
    return 0;
  }

  const uint8_t* buf = ++_cur;
  size_t length = 0;

  while (hasNext() && getNext() != '>') {
    length++;
  }

  return document->string(buf, length);
}

bool NTriplesParser::isStringLiteralQuote() {
  return getNext(true) == '"';
}

RDFString* NTriplesParser::readStringLiteralQuote() {
  const uint8_t* buf = ++_cur;
  size_t length = 0;

  while (hasNext() && getNext() != '"') {
    length++;
  }

  return document->string(buf, length);
}

bool NTriplesParser::isBlankNodeLabel() {
  // TODO(@bergos) check for _:
  return getNext(true) == '_';
}

RDFString* NTriplesParser::readBlankNodeLabel() {
  _cur += 2;

  const uint8_t* buf = _cur;
  size_t length = 0;

  while (hasNext() && !readWhiteSpace() && getNext(true) != '.') {
    getNext();
    length++;
  }

  return document->string(buf, length);
}
