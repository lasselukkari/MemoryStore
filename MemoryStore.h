#ifndef MEMORYSTORE_H_
#define MEMORYSTORE_H_

#include <aJSON.h>

class MemoryStore {

public:
  MemoryStore();
  aJsonObject* init(aJsonObject * root);
  aJsonObject* init(Stream * stream);
  aJsonObject* query();
  void streamTo(Stream * stream);
  void remove();
  
  aJsonObject* save(aJsonObject* model);
  aJsonObject* save(Stream * stream);
  aJsonObject* find(int id);
  void streamTo(Stream * stream, int id);
  void remove(int id);

private:
  aJsonObject* _root;
  int _nextId;
  
};

#endif
