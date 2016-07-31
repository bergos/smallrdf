/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#ifndef RDFNTRIPLESPARSER_H_
#define RDFNTRIPLESPARSER_H_

#include "RDF.h"

class NTriplesParser {
 public:
  RDFDocument* document;

  NTriplesParser(const RDFString* input, RDFDocument* document = 0);

  RDFDocument* parse();
  static RDFDocument* parse_static(const RDFString* input);

  RDFQuad* parseQuad();
  static RDFQuad* parseQuad_static(RDFString* input,
                                         RDFDocument* document);

 protected:
  const uint8_t* _buf;
  const uint8_t* _cur;
  const uint8_t* _end;

  bool hasNext();
  uint8_t getNext(const bool keep = false);
  bool readWhiteSpace();
  RDFTerm* readSubject();
  RDFTerm* readPredicate();
  RDFTerm* readObject();
  bool isLiteral();
  RDFLiteral* readLiteral();
  RDFString* readLangtag();
  bool isIRIRef();
  RDFString* readIRIRef();
  bool isStringLiteralQuote();
  RDFString* readStringLiteralQuote();
  bool isBlankNodeLabel();
  RDFString* readBlankNodeLabel();
};

#endif  // RDFNTRIPLESPARSER_H_
