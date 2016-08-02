/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#include "RDFNTriplesSerializer.h"

#include <string.h>

NTriplesSerializer::NTriplesSerializer()
    : _length(0),
      _buf(0),
      _cur(0),
      _end(0) {
}

const uint8_t* NTriplesSerializer::serialize(RDFDataset* dataset) {
  _length = datasetSize(dataset);
  _buf = _cur = new uint8_t[_length];
  _end = _cur + _length;

  serializeDataset(dataset);

  return _buf;
}

const uint8_t* NTriplesSerializer::serialize_static(RDFDataset* dataset) {
  NTriplesSerializer serializer;

  return serializer.serialize(dataset);
}

void NTriplesSerializer::write(const char chr) {
  _cur[0] = chr;
  _cur++;
}

void NTriplesSerializer::write(const RDFString* str) {
  memcpy(_cur, str->buffer(), str->length());
  _cur += str->length();
}

size_t NTriplesSerializer::datasetSize(const RDFDataset* dataset) {
  size_t size = 1;

  for (int i = 0; i < dataset->quads.length; i++) {
    size += quadSize(dataset->quads.get(i));
  }

  return size;
}

void NTriplesSerializer::serializeDataset(const RDFDataset* dataset) {
  for (int i = 0; i < dataset->quads.length; i++) {
    serializeQuad(dataset->quads.get(i));
  }

  write(static_cast<char>(0));
}

size_t NTriplesSerializer::quadSize(const RDFQuad* quad) {
  return termSize(quad->subject) + termSize(quad->predicate)
      + termSize(quad->object) + termSize(quad->graph) + 5;
}

void NTriplesSerializer::serializeQuad(const RDFQuad* quad) {
  serializeTerm(quad->subject);
  write(' ');
  serializeTerm(quad->predicate);
  write(' ');
  serializeTerm(quad->object);
  write(' ');

  if (quad->graph) {
    serializeTerm(quad->graph);
    write(' ');
  }

  write('.');
  write('\n');
}

size_t NTriplesSerializer::termSize(const RDFTerm* term) {
  if (term == 0) {
    return 0;
  }

  switch (term->termType) {
    case RDF_NAMED_NODE:
      return term->value->length() + 2;
    case RDF_LITERAL:
      return literalSize(reinterpret_cast<const RDFLiteral*>(term));
    case RDF_BLANK_NODE:
      return term->value->length() + 2;
    default:
      return 0;
  }
}

void NTriplesSerializer::serializeTerm(const RDFTerm* term) {
  switch (term->termType) {
    case RDF_NAMED_NODE:
      serializeIri(term->value);
      return;
    case RDF_LITERAL:
      serializeLiteral(reinterpret_cast<const RDFLiteral*>(term));
      return;
    case RDF_BLANK_NODE:
      serializeBlankNode(reinterpret_cast<const RDFBlankNode*>(term));
      return;
    default:
      return;
  }
}

size_t NTriplesSerializer::literalSize(const RDFLiteral* literal) {
  size_t size = literal->value->length() + 2;

  if (literal->language) {
    size += literal->language->length() + 1;
  } else if (literal->datatype) {
    size += literal->datatype->length() + 2;
  }

  return size;
}

void NTriplesSerializer::serializeLiteral(const RDFLiteral* literal) {
  write('"');
  write(literal->value);
  write('"');

  if (literal->language) {
    write('@');
    write(literal->language);
  } else if (literal->datatype) {
    write('^');
    write('^');
    serializeIri(literal->datatype);
  }
}

void NTriplesSerializer::serializeBlankNode(const RDFBlankNode* blankNode) {
  write('_');
  write(':');
  write(blankNode->value);
}

void NTriplesSerializer::serializeIri(const RDFString* iri) {
  write('<');
  write(iri);
  write('>');
}
