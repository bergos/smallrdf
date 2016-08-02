#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <RDF.h>
#include <RDFNTriplesParser.h>
#include <RDFNTriplesSerializer.h>

class Led {
 public:
  Led(int pin);

  void begin();
  bool isOn();
  void on();
  void off();

 protected:
  int pin;
};

Led::Led(int pin) : pin(pin) {
}

void Led::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

bool Led::isOn() {
  return digitalRead(pin) == LOW;
}

void Led::on() {
  digitalWrite(pin, LOW);
}

void Led::off() {
  digitalWrite(pin, HIGH);
}

RDFString rdfType("http://www.w3.org/1999/02/22-rdf-syntax-ns#");
RDFString dhLed("http://ns.bergnet.org/dark-horse#Led");
RDFString dhStatus("http://ns.bergnet.org/dark-horse#status");
RDFString dhOn("http://ns.bergnet.org/dark-horse#On");
RDFString dhOff("http://ns.bergnet.org/dark-horse#Off");

AsyncWebServer server(80);

void setup () {
  Serial.begin(115200);

  Led led0(0);

  led0.begin();

  server.on("/led/on", HTTP_GET, [&led0](AsyncWebServerRequest* request) {
    Serial.println("GET /led/on");
    
    led0.on();

    request->send(201);
  });

  server.on("/led/off", HTTP_GET, [&led0](AsyncWebServerRequest* request) {
    Serial.println("GET /led/off");

    led0.off();

    request->send(201);
  });

  server.on("/led", HTTP_GET, [&led0](AsyncWebServerRequest* request) {
    Serial.println("GET /led");

    RDFDocument document;

    String iri = String("http://") + String(WiFi.localIP().toString()) + String("/led");
    const RDFString* iriStr = document.string(iri.c_str());
    const RDFNamedNode* subject = document.namedNode(iriStr);

    const RDFQuad* typeQuad = document.triple(subject, document.namedNode(&rdfType), document.namedNode(&dhLed));

    RDFString* status = led0.isOn() ? &dhOn : &dhOff;
    const RDFQuad* statusQuad = document.triple(subject, document.namedNode(&dhStatus), document.namedNode(status));

    const uint8_t* ntriples = NTriplesSerializer::serialize_static(&document);

    request->send(200, "application/n-triples", (const char*)ntriples);

    delete ntriples;
  });

  /*server.on("/led", HTTP_PUT, [&led0](AsyncWebServerRequest* request) {
    Serial.println("PUT /led");

    RDFDocument document;

    AsyncWebParameter* body = request->getParam("body", true);

    const RDFString* bodyStr = document.string(body->value().c_str());
    NTriplesParser::parse_static(bodyStr, &document);

    RDFQuad* status = document.find(0, document.namedNode(&dhStatus));

    if (status) {
      if (status->object->value->equals(&dhOn)) {
        led0.on();
      } else if (status->object->value->equals(&dhOff)) {
        led0.off();
      }
    }

    request->send(201);
  });*/

  server.begin();
}

void loop () {
}
