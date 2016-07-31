/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#ifndef RDF_H_
#define RDF_H_

#include "RDFList.h"

#include <stddef.h>
#include <stdint.h>

class RDFString {
 public:
  RDFString();
  RDFString(const char* buf);
  RDFString(const uint8_t* buf, const size_t length);
  virtual ~RDFString();

  const uint8_t* buffer() const;
  const size_t length() const;
  const bool equals(const RDFString other) const;
  const bool equals(const RDFString* other) const;
  const char* c_str();

 protected:
  const uint8_t* _buf;
  const size_t _length;
  bool _trailingZero;
  char* _c_str;
};

typedef enum {
  RDF_NAMED_NODE,
  RDF_BLANK_NODE,
  RDF_LITERAL,
  RDF_VARIABLE
} RDFTermType;

class RDFTerm {
 public:
  RDFTermType termType;
  RDFString* value;

  RDFTerm(RDFTermType termType, RDFString* value);
  virtual ~RDFTerm();

  virtual bool equals(const RDFTerm* other) const;
};

class RDFNamedNode : public RDFTerm {
 public:
  RDFNamedNode(RDFString* value);
};

class RDFLiteral : public RDFTerm {
 public:
  RDFString* language;
  RDFString* datatype;

  RDFLiteral(RDFString* value, RDFString* language = 0,
             RDFString* datatype = 0);

  virtual bool equals(const RDFTerm* other) const;
};

class RDFBlankNode : public RDFTerm {
 public:
  RDFBlankNode(RDFString* value);
};

class RDFQuad {
 public:
  RDFTerm* subject;
  RDFTerm* predicate;
  RDFTerm* object;
  RDFTerm* graph;

  RDFQuad(RDFTerm* subject, RDFTerm* predicate,
          RDFTerm* object, RDFTerm* graph = 0);

  const bool match(const RDFTerm* subject, const RDFTerm* predicate = 0,
                   const RDFTerm* object = 0, const RDFTerm* graph = 0);
};

class RDFDataset {
 public:
  RDFList<RDFQuad*> quads;

  virtual ~RDFDataset();

  RDFQuad* find(const RDFTerm* subject, const RDFTerm* predicate = 0,
                const RDFTerm* object = 0, const RDFTerm* graph = 0);
  RDFDataset* match(const RDFTerm* subject, const RDFTerm* predicate = 0,
                    const RDFTerm* object = 0, const RDFTerm* graph = 0);

 protected:
  RDFList<RDFDataset*> _datasets;
};

class RDFDocument : public RDFDataset {
 public:
  virtual ~RDFDocument();

  RDFString* string(const char* buf);
  RDFString* string(const uint8_t* buf, const size_t length);
  RDFNamedNode* namedNode(RDFString* value);
  RDFLiteral* literal(RDFString* value, RDFString* language,
                      RDFString* datatype);
  RDFBlankNode* blankNode(RDFString* value);
  RDFQuad* triple(RDFTerm* subject, RDFTerm* predicate,
                  RDFTerm* object);
  RDFQuad* quad(RDFTerm* subject, RDFTerm* predicate,
                RDFTerm* object, RDFTerm* graph);

 protected:
  RDFList<RDFString*> _strings;
  RDFList<RDFTerm*> _terms;

  RDFString* findString(const RDFString* newStr) const;
  RDFTerm* findTerm(const RDFTerm* newTerm) const;
};

#endif  // RDF_H_
