/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#ifndef RDFNTRIPLESSERIALIZER_H_
#define RDFNTRIPLESSERIALIZER_H_

#include "RDF.h"

class NTriplesSerializer {
 public:
  NTriplesSerializer();
  const uint8_t* serialize(RDFDataset* dataset);

  static const uint8_t* serialize_static(RDFDataset* dataset);

 protected:
  size_t _length;
  uint8_t* _buf;
  uint8_t* _cur;
  uint8_t* _end;

  void write(const char chr);
  void write(const RDFString* str);

  size_t datasetSize(const RDFDataset* dataset);
  void serializeDataset(const RDFDataset* dataset);
  size_t quadSize(const RDFQuad* quad);
  void serializeQuad(const RDFQuad* quad);
  size_t termSize(const RDFTerm* term);
  void serializeTerm(const RDFTerm* term);
  size_t literalSize(const RDFLiteral* literal);
  void serializeLiteral(const RDFLiteral* literal);
  void serializeBlankNode(const RDFBlankNode* blankNode);
  void serializeIri(const RDFString* iri);
};

#endif  // RDFNTRIPLESSERIALIZER_H_
