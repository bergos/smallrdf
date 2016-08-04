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

RDFDocument* NTriplesParser::parse_static(const RDFString* input,
                                          RDFDocument* document) {
  NTriplesParser parser(input, document);

  return parser.parse();
}

const RDFQuad* NTriplesParser::parseQuad() {
  readWhiteSpace();

  const RDFTerm* subject = readSubject();

  readWhiteSpace();

  const RDFTerm* predicate = readPredicate();

  readWhiteSpace();

  const RDFTerm* object = readObject();

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

const RDFQuad* NTriplesParser::parseQuad_static(const RDFString* input,
                                                RDFDocument* document) {
  NTriplesParser parser(input, document);

  return parser.parseQuad();
}

const bool NTriplesParser::hasNext() {
  return _cur < _end;
}

const uint8_t NTriplesParser::getNext(const bool keep) {
  if (keep) {
    return *_cur;
  } else {
    return *_cur++;
  }
}

const bool NTriplesParser::readWhiteSpace() {
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

const RDFTerm* NTriplesParser::readSubject() {
  if (isIRIRef()) {
    return document->namedNode(readIRIRef());
  } else if (isBlankNodeLabel()) {
    return document->blankNode(readBlankNodeLabel());
  } else {
    return 0;
  }
}

const RDFTerm* NTriplesParser::readPredicate() {
  if (isIRIRef()) {
    return document->namedNode(readIRIRef());
  } else {
    return 0;
  }
}

const RDFTerm* NTriplesParser::readObject() {
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

const bool NTriplesParser::isLiteral() {
  return isStringLiteralQuote();
}

const RDFLiteral* NTriplesParser::readLiteral() {
  const RDFString* value = readStringLiteralQuote();
  const RDFString* language = readLangtag();

  // TODO(@bergos) check for ^^
  if (getNext(true) == '^') {
    getNext();
    getNext();
  }

  const RDFString* datatype = readIRIRef();

  return document->literal(value, language, datatype);
}

const RDFString* NTriplesParser::readLangtag() {
  if (getNext(true) != '@') {
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

const bool NTriplesParser::isIRIRef() {
  return getNext(true) == '<';
}

const RDFString* NTriplesParser::readIRIRef() {
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

const bool NTriplesParser::isStringLiteralQuote() {
  return getNext(true) == '"';
}

const RDFString* NTriplesParser::readStringLiteralQuote() {
  const uint8_t* buf = ++_cur;
  size_t length = 0;

  while (hasNext() && getNext() != '"') {
    length++;
  }

  return document->string(buf, length);
}

const bool NTriplesParser::isBlankNodeLabel() {
  // TODO(@bergos) check for _:
  return getNext(true) == '_';
}

const RDFString* NTriplesParser::readBlankNodeLabel() {
  _cur += 2;

  const uint8_t* buf = _cur;
  size_t length = 0;

  while (hasNext() && !readWhiteSpace() && getNext(true) != '.') {
    getNext();
    length++;
  }

  return document->string(buf, length);
}
