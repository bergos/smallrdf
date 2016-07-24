#ifndef __RDF_NTRIPLES_H__
#define __RDF_NTRIPLES_H__

#include "RDF.h"

class NTriplesParser {
  public:
    NTriplesParser();
    RDFDocument* parse (const uint8_t* buf, size_t length);
	bool hasNext();
  protected:
	RDFDocument* _document;
    const uint8_t* _buf;
	const uint8_t* _cur;
	const uint8_t* _end;

	const uint8_t getNext(bool keep=false);
	bool readWhiteSpace();
    RDFQuad* readTriple();
	RDFTerm* readSubject();
	RDFTerm* readPredicate();
	RDFTerm* readObject();
	bool isIRIREF();
	RDFString readIRIREF();
	bool isSTRING_LITERAL_QUOTE();
	RDFString readSTRING_LITERAL_QUOTE();
	bool isBLANK_NODE_LABEL();
	RDFString readBLANK_NODE_LABEL();
};

class NTriplesSerializer {
  public:
	NTriplesSerializer();
	const uint8_t* serialize(RDFDocument* document);

  protected:
	size_t _length;
	uint8_t* _buf;
	uint8_t* _cur;
	uint8_t* _end;

	void write(char character);
	void write(RDFString* string);
	size_t termSize(RDFTerm* term);
	size_t quadSize(RDFQuad* quad);
	size_t documentSize(RDFDocument* document);
	void serializeTerm(RDFTerm* term);
	void serializeQuad(RDFQuad* quad);
	void serializeDocument(RDFDocument* document);
};

#endif //__RDF_NTRIPLES_H__
