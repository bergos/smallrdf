/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#ifndef RDFNTRIPLESPARSER_H_
#define RDFNTRIPLESPARSER_H_

#include "RDF.h"

class NTriplesParser {
 public:
  RDFDocument* document;

  explicit NTriplesParser(const RDFString* input, RDFDocument* document = 0);

  RDFDocument* parse();
  static RDFDocument* parse_static(const RDFString* input,
                                   RDFDocument* document = 0);

  const RDFQuad* parseQuad();
  static const RDFQuad* parseQuad_static(const RDFString* input,
                                         RDFDocument* document);

 protected:
  const uint8_t* _buf;
  const uint8_t* _cur;
  const uint8_t* _end;

  const bool hasNext();
  const uint8_t getNext(const bool keep = false);
  const bool readWhiteSpace();
  const RDFTerm* readSubject();
  const RDFTerm* readPredicate();
  const RDFTerm* readObject();
  const bool isLiteral();
  const RDFLiteral* readLiteral();
  const RDFString* readLangtag();
  const bool isIRIRef();
  const RDFString* readIRIRef();
  const bool isStringLiteralQuote();
  const RDFString* readStringLiteralQuote();
  const bool isBlankNodeLabel();
  const RDFString* readBlankNodeLabel();
};

#endif  // RDFNTRIPLESPARSER_H_
