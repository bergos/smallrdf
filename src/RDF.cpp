/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#include "RDF.h"

#include <string.h>

RDFString::RDFString()
    : _buf(0),
      _length(0),
      _trailingZero(true),
      _c_str(0) {
}

RDFString::RDFString(const char* buf)
    : _buf((const uint8_t*) buf),
      _length(strlen(buf)),
      _trailingZero(true),
      _c_str(0) {
}

RDFString::RDFString(const uint8_t* buf, size_t length)
    : _buf(buf),
      _length(length),
      _trailingZero(false),
      _c_str(0) {
}

RDFString::~RDFString() {
  if (_c_str) {
    delete[] _c_str;
  }
}

const uint8_t* RDFString::buffer() const {
  return _buf;
}

const size_t RDFString::length() const {
  return _length;
}

const bool RDFString::equals(const RDFString other) const {
  return this->equals(&other);
}

const bool RDFString::equals(const RDFString* other) const {
  if (this == other) {
    return true;
  }

  if (length() != other->length()) {
    return false;
  }

  if (buffer() == 0 || other->buffer() == 0) {
    return buffer() == other->buffer();
  }

  return memcmp(buffer(), other->buffer(), length()) == 0;
}

const char* RDFString::c_str() const {
  if (_trailingZero) {
    return (const char*) _buf;
  } else {
    if (!_c_str) {
      char** writable_c_str = const_cast<char**>(&this->_c_str);

      *writable_c_str = new char[_length + 1];
      memcpy(*writable_c_str, _buf, _length);
      *(*writable_c_str + _length) = 0;
    }

    return _c_str;
  }
}

#if defined(ARDUINO)

RDFString::RDFString(String str, bool copy)
: _buf((const uint8_t*)str.c_str()),
_length(str.length()),
_trailingZero(true),
_c_str(0) {
  if (copy) {
    char** writable_c_str = const_cast<char**>(&this->_c_str);

    *writable_c_str = new char[_length + 1];
    memcpy(*writable_c_str, _buf, _length + 1);

    *const_cast<uint8_t**>(&this->_buf) =
        reinterpret_cast<uint8_t*>(*writable_c_str);
  }
}

#endif

RDFTerm::RDFTerm(const RDFTermType termType, const RDFString* value)
    : termType(termType),
      value(value) {
}

RDFTerm::~RDFTerm() {
}

bool RDFTerm::equals(const RDFTerm* other) const {
  if (this == other) {
    return true;
  }

  return termType == other->termType && value->equals(other->value);
}

RDFNamedNode::RDFNamedNode(const RDFString* value)
    : RDFTerm(RDF_NAMED_NODE, value) {
}

RDFLiteral::RDFLiteral(const RDFString* value, const RDFString* language,
                       const RDFString* datatype)
    : RDFTerm(RDF_LITERAL, value),
      language(language),
      datatype(datatype) {
}

bool RDFLiteral::equals(const RDFTerm* other) const {
  if (this == other) {
    return true;
  }

  if (other->termType != RDF_LITERAL) {
    return false;
  }

  const RDFLiteral* otherLiteral = reinterpret_cast<const RDFLiteral*>(other);

  return value->equals(otherLiteral->value)
      && language->equals(otherLiteral->language)
      && datatype->equals(otherLiteral->datatype);
}

RDFBlankNode::RDFBlankNode(const RDFString* value)
    : RDFTerm(RDF_BLANK_NODE, value) {
}

RDFQuad::RDFQuad(const RDFTerm* subject, const RDFTerm* predicate,
                 const RDFTerm* object, const RDFTerm* graph)
    : subject(subject),
      predicate(predicate),
      object(object),
      graph(graph) {
}

const bool RDFQuad::match(const RDFTerm* subject, const RDFTerm* predicate,
                          const RDFTerm* object, const RDFTerm* graph) const {
  if (subject && !this->subject->equals(subject)) {
    return false;
  }

  if (predicate && !this->predicate->equals(predicate)) {
    return false;
  }

  if (object && !this->object->equals(object)) {
    return false;
  }

  if (graph && (!this->graph || !this->graph->equals(graph))) {
    return false;
  }

  return true;
}

RDFDataset::~RDFDataset() {
  for (int i = 0; i < _datasets.length; i++) {
    delete _datasets.get(i);
  }
}

const RDFQuad* RDFDataset::find(const RDFTerm* subject,
                                const RDFTerm* predicate, const RDFTerm* object,
                                const RDFTerm* graph) {
  for (int i = 0; i < quads.length; i++) {
    const RDFQuad* quad = quads.get(i);

    if (quad->match(subject, predicate, object, graph)) {
      return quad;
    }
  }

  return 0;
}

RDFDataset* RDFDataset::match(const RDFTerm* subject, const RDFTerm* predicate,
                              const RDFTerm* object, const RDFTerm* graph) {
  RDFDataset* matches = _datasets.add(new RDFDataset());

  for (int i = 0; i < quads.length; i++) {
    const RDFQuad* quad = quads.get(i);

    if (quad->match(subject, predicate, object, graph)) {
      matches->quads.add(quad);
    }
  }

  return matches;
}

RDFDocument::~RDFDocument() {
  for (int i = 0; i < quads.length; i++) {
    delete quads.get(i);
  }

  for (int i = 0; i < _terms.length; i++) {
    delete _terms.get(i);
  }

  for (int i = 0; i < _strings.length; i++) {
    delete _strings.get(i);
  }
}

const RDFString* RDFDocument::string(const char* buf) {
  const RDFString cur(buf);

#if defined(ARDUINO)
  const RDFString* found = findString(&cur);

  if (found != 0) {
    return found;
  }
#endif

  return reinterpret_cast<RDFString*>(_strings.add(new RDFString(buf)));
}

const RDFString* RDFDocument::string(const uint8_t* buf, const size_t length) {
  const RDFString cur(buf, length);

#if defined(ARDUINO)
  const RDFString* found = findString(&cur);

  if (found != 0) {
    return found;
  }
#endif

  return reinterpret_cast<RDFString*>(_strings.add(new RDFString(buf, length)));
}

const RDFNamedNode* RDFDocument::namedNode(const RDFString* value) {
  const RDFNamedNode cur(value);

#if defined(ARDUINO)
  const RDFTerm* found = findTerm(&cur);

  if (found != 0) {
    return reinterpret_cast<const RDFNamedNode*>(found);
  }
#endif

  return reinterpret_cast<const RDFNamedNode*>(_terms.add(
      new RDFNamedNode(value)));
}

const RDFLiteral* RDFDocument::literal(const RDFString* value,
                                       const RDFString* language,
                                       const RDFString* datatype) {
  const RDFLiteral cur(value, language, datatype);

#if defined(ARDUINO)
  const RDFTerm* found = findTerm(&cur);

  if (found != 0) {
    return reinterpret_cast<const RDFLiteral*>(found);
  }
#endif

  return reinterpret_cast<const RDFLiteral*>(_terms.add(
      new RDFLiteral(value, language, datatype)));
}

const RDFBlankNode* RDFDocument::blankNode(const RDFString* value) {
  const RDFBlankNode cur(value);

#if defined(ARDUINO)
  const RDFTerm* found = findTerm(&cur);

  if (found != 0) {
    return reinterpret_cast<const RDFBlankNode*>(found);
  }
#endif

  return reinterpret_cast<const RDFBlankNode*>(_terms.add(
      new RDFBlankNode(value)));
}

const RDFQuad* RDFDocument::triple(const RDFTerm* subject,
                                   const RDFTerm* predicate,
                                   const RDFTerm* object,
                                   const RDFTerm* graph) {
  return quads.add(new RDFQuad(subject, predicate, object, graph));
}

RDFDataset* RDFDocument::dataset() {
  return _datasets.add(new RDFDataset());
}

const RDFString* RDFDocument::findString(const RDFString* newStr) const {
  for (int i = 0; i < _strings.length; i++) {
    RDFString* cur = _strings.get(i);

    if (cur->equals(newStr)) {
      return cur;
    }
  }

  return 0;
}

const RDFTerm* RDFDocument::findTerm(const RDFTerm* newTerm) const {
  for (int i = 0; i < _terms.length; i++) {
    RDFTerm* cur = _terms.get(i);

    if (cur->equals(newTerm)) {
      return cur;
    }
  }

  return 0;
}

#if defined(ARDUINO)

const RDFString* RDFDocument::string(String str, bool copy) {
  const RDFString cur(str, copy);
  const RDFString* found = findString(&cur);

  if (found != 0) {
    return found;
  }

  return reinterpret_cast<RDFString*>(_strings.add(new RDFString(str, copy)));
}

#endif
