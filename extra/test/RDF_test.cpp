/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#include <gtest/gtest.h>

#include "RDF.h"

TEST(RDFString, length) {
  ASSERT_EQ(0, RDFString().length());
  ASSERT_EQ(4, RDFString("test").length());
  ASSERT_EQ(3, RDFString((const uint8_t*)"test", 3).length());
}

TEST(RDFString, buffer) {
  const char* test = "test";

  ASSERT_EQ(0, RDFString().buffer());
  ASSERT_EQ((const uint8_t*)test, RDFString(test).buffer());
  ASSERT_EQ((const uint8_t*)test,
            RDFString((const uint8_t*)test, 4).buffer());
}

TEST(RDFString, equals) {
  RDFString test("test");
  RDFString tset("tset");
  RDFString testDifferentLength("testDifferentLength");
  RDFString empty;
  RDFString equals("test");
  RDFString equalsEmpty;

  ASSERT_EQ(false, test.equals(&tset));
  ASSERT_EQ(false, test.equals(&testDifferentLength));
  ASSERT_EQ(false, test.equals(&empty));
  ASSERT_EQ(true, test.equals(&equals));
  ASSERT_EQ(true, empty.equals(&equalsEmpty));
}

TEST(RDFString, c_str) {
  RDFString test("test");
  RDFString tes((const uint8_t*) "test", 3);

  ASSERT_STREQ("test", test.c_str());
  ASSERT_STREQ("tes", tes.c_str());
}

TEST(RDFTerm, termType) {
  RDFString value;

  ASSERT_EQ(RDF_NAMED_NODE, RDFTerm(RDF_NAMED_NODE, &value).termType);
}

TEST(RDFTerm, value) {
  RDFString str("test");

  ASSERT_EQ(&str, RDFTerm(RDF_NAMED_NODE, &str).value);
}

TEST(RDFTerm, equals) {
  RDFString test("test");
  RDFString tset("tset");

  RDFTerm termTest(RDF_NAMED_NODE, &test);
  RDFTerm termTset(RDF_NAMED_NODE, &tset);

  ASSERT_TRUE(termTest.equals(&termTest));
  ASSERT_FALSE(termTest.equals(&termTset));
}

TEST(RDFNamedNode, termType) {
  RDFString value;

  ASSERT_EQ(RDF_NAMED_NODE, RDFNamedNode(&value).termType);
}

TEST(RDFNamedNode, value) {
  RDFString str("http://example.org/");

  ASSERT_EQ(&str, RDFNamedNode(&str).value);
}

TEST(RDFLiteral, termType) {
  RDFString value;

  ASSERT_EQ(RDF_LITERAL, RDFLiteral(&value).termType);
}

TEST(RDFLiteral, value) {
  RDFString value("test");

  ASSERT_EQ(&value, RDFLiteral(&value).value);
}

TEST(RDFLiteral, language) {
  RDFString value;
  RDFString language("en");

  ASSERT_EQ(&language, RDFLiteral(&value, &language).language);
}

TEST(RDFLiteral, datatype) {
  RDFString value;
  RDFString datatype("http://example.org/");

  ASSERT_EQ(&datatype, RDFLiteral(&value, 0, &datatype).datatype);
}

TEST(RDFBlankNode, termType) {
  RDFString value;

  ASSERT_EQ(RDF_BLANK_NODE, RDFBlankNode(&value).termType);
}

TEST(RDFBlankNode, value) {
  RDFString value("test");

  ASSERT_EQ(&value, RDFBlankNode(&value).value);
}

TEST(RDFQuad, subject) {
  RDFString iri("http://example.org/");
  RDFNamedNode term(&iri);

  ASSERT_EQ(&term, RDFQuad(&term, 0, 0, 0).subject);
}

TEST(RDFQuad, predicate) {
  RDFString iri("http://example.org/");
  RDFNamedNode term(&iri);

  ASSERT_EQ(&term, RDFQuad(0, &term, 0, 0).predicate);
}

TEST(RDFQuad, object) {
  RDFString iri("http://example.org/");
  RDFNamedNode term(&iri);

  ASSERT_EQ(&term, RDFQuad(0, 0, &term, 0).object);
}

TEST(RDFQuad, graph) {
  RDFString iri("http://example.org/");
  RDFNamedNode term(&iri);

  ASSERT_EQ(&term, RDFQuad(0, 0, 0, &term).graph);
}

TEST(RDFDataset, find) {
  RDFDataset dataset;
  RDFString subjectStr1("http://example.org/subject1");
  RDFString subjectStr2("http://example.org/subject2");
  RDFString predicateStr("http://example.org/predicate");
  RDFString objectStr("http://example.org/object");
  RDFNamedNode subject1(&subjectStr1);
  RDFNamedNode subject2(&subjectStr2);
  RDFNamedNode predicate(&predicateStr);
  RDFNamedNode object(&objectStr);
  RDFQuad quad1(&subject1, &predicate, &object);
  RDFQuad quad2(&subject2, &predicate, &object);
  dataset.quads.add(&quad1);
  dataset.quads.add(&quad2);

  ASSERT_TRUE(dataset.find(&subject1) != 0);
  ASSERT_TRUE(dataset.find(&subject2) != 0);
  ASSERT_TRUE(dataset.find(&object) == 0);
}

TEST(RDFDocument, string) {
  RDFDocument doc;

  const RDFString* str1 = doc.string("test");
  const RDFString* str2 = doc.string("test");
  const RDFString* str3 = doc.string((const uint8_t*) "test", 4);

  ASSERT_STREQ(str1->c_str(), "test");
  ASSERT_EQ(str1, str2);
  ASSERT_EQ(str1, str3);
}
