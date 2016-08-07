/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#ifndef RDF_H_
#define RDF_H_

#include "RDFList.h"

#include <stddef.h>
#include <stdint.h>

#if defined(ARDUINO)
#include <Arduino.h>
#endif

class RDFString {
 public:
  RDFString();
  explicit RDFString(const char* buf);
  RDFString(const uint8_t* buf, const size_t length);
  virtual ~RDFString();

#if defined(ARDUINO)
  explicit RDFString(String str, bool copy = false);
#endif

  const uint8_t* buffer() const;
  const size_t length() const;
  const bool equals(const RDFString other) const;
  const bool equals(const RDFString* other) const;
  const char* c_str() const;

 protected:
  const uint8_t* _buf;
  const size_t _length;
  const bool _trailingZero;
  const char* _c_str;
};

typedef enum {
  RDF_NAMED_NODE,
  RDF_BLANK_NODE,
  RDF_LITERAL,
  RDF_VARIABLE
} RDFTermType;

class RDFTerm {
 public:
  const RDFTermType termType;
  const RDFString* value;

  RDFTerm(const RDFTermType termType, const RDFString* value);
  virtual ~RDFTerm();

  virtual bool equals(const RDFTerm* other) const;
};

class RDFNamedNode : public RDFTerm {
 public:
  explicit RDFNamedNode(const RDFString* value);
};

class RDFLiteral : public RDFTerm {
 public:
  const RDFString* language;
  const RDFString* datatype;

  RDFLiteral(const RDFString* value, const RDFString* language = 0,
             const RDFString* datatype = 0);

  virtual bool equals(const RDFTerm* other) const;
};

class RDFBlankNode : public RDFTerm {
 public:
  explicit RDFBlankNode(const RDFString* value);
};

class RDFQuad {
 public:
  const RDFTerm* subject;
  const RDFTerm* predicate;
  const RDFTerm* object;
  const RDFTerm* graph;

  RDFQuad(const RDFTerm* subject, const RDFTerm* predicate,
          const RDFTerm* object, const RDFTerm* graph = 0);

  const bool match(const RDFTerm* subject, const RDFTerm* predicate = 0,
                   const RDFTerm* object = 0, const RDFTerm* graph = 0) const;
};

class RDFDataset {
 public:
  RDFList<const RDFQuad*> quads;

  virtual ~RDFDataset();

  const RDFQuad* find(const RDFTerm* subject, const RDFTerm* predicate = 0,
                const RDFTerm* object = 0, const RDFTerm* graph = 0);
  RDFDataset* match(const RDFTerm* subject, const RDFTerm* predicate = 0,
                    const RDFTerm* object = 0, const RDFTerm* graph = 0);

 protected:
  RDFList<RDFDataset*> _datasets;
};

class RDFDocument : public RDFDataset {
 public:
  virtual ~RDFDocument();

  const RDFString* string(const char* buf);
  const RDFString* string(const uint8_t* buf, const size_t length);
  const RDFNamedNode* namedNode(const RDFString* value);
  const RDFLiteral* literal(const RDFString* value, const RDFString* language =
                                0,
                            const RDFString* datatype = 0);
  const RDFBlankNode* blankNode(const RDFString* value);
  const RDFQuad* triple(const RDFTerm* subject, const RDFTerm* predicate,
                      const RDFTerm* object, const RDFTerm* graph = 0);
  RDFDataset* dataset();

#if defined(ARDUINO)
  const RDFString* string(String str, bool copy = false);
#endif

 protected:
  RDFList<RDFString*> _strings;
  RDFList<RDFTerm*> _terms;

  const RDFString* findString(const RDFString* newStr) const;
  const RDFTerm* findTerm(const RDFTerm* newTerm) const;
};

#endif  // RDF_H_
