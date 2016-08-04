/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#include <gtest/gtest.h>

#include "RDFNTriplesSerializer.h"

TEST(NTriplesSerializer, NamedNode) {
  RDFDataset dataset;
  RDFString subjectStr("http://example.org/subject");
  RDFString predicateStr("http://example.org/predicate");
  RDFString objectStr("http://example.org/object");
  RDFNamedNode subject(&subjectStr);
  RDFNamedNode predicate(&predicateStr);
  RDFNamedNode object(&objectStr);
  RDFQuad quad(&subject, &predicate, &object);
  dataset.quads.add(&quad);

  const char* expected =
      "<http://example.org/subject> <http://example.org/predicate> <http://example.org/object> .\n";
  const char* actual = (const char*) NTriplesSerializer::serialize_static(
      &dataset);

  ASSERT_STREQ(expected, actual);

  delete[] actual;
}

TEST(NTriplesSerializer, Literal) {
  RDFDataset dataset;
  RDFString subjectStr("http://example.org/subject");
  RDFString predicateStr("http://example.org/predicate");
  RDFString objectStr("object");
  RDFNamedNode subject(&subjectStr);
  RDFNamedNode predicate(&predicateStr);
  RDFLiteral object(&objectStr);
  RDFQuad quad(&subject, &predicate, &object);
  dataset.quads.add(&quad);

  const char* expected =
      "<http://example.org/subject> <http://example.org/predicate> \"object\" .\n";
  const char* actual = (const char*) NTriplesSerializer::serialize_static(
      &dataset);

  ASSERT_STREQ(expected, actual);

  delete[] actual;
}
