#include <arduino.h>
#include <MemoryStore.h>
#include <aJSON.h>

MemoryStore::MemoryStore() {
  _nextId = 1;
  _root = aJson.createArray();
}

aJsonObject* MemoryStore::init(aJsonObject* root) {
  int max =  0;

  if (root && root->child){
    aJsonObject * model = root->child;

    while (model ) {
      int id = aJson.getObjectItem(model, "_id")->valueint;
      if(id > max){
        max = id;  
      }
      model = model->next;
    }

    _root = root;
    _nextId = max + 1;
  }

  return _root;
}

aJsonObject* MemoryStore::init(Stream * stream) {
  aJsonStream aJsonStream(stream);
  return init(aJson.parse(&aJsonStream));
}

aJsonObject*  MemoryStore::query() {
  return _root;
}

void MemoryStore::streamTo(Stream * stream){
  aJsonStream aJsonStream(stream);
  aJson.print(_root, &aJsonStream);
}

void MemoryStore::remove() {
   aJson.deleteItem(_root);
   _root = aJson.createArray();
}

aJsonObject* MemoryStore::save(aJsonObject* model) {
  aJsonObject* idItem = aJson.getObjectItem(model, "_id");

  if (idItem == NULL) {
    aJson.addNumberToObject(model, "_id", _nextId++);
    aJson.addItemToArray(_root, model);
  } else {
    int id = aJson.getObjectItem(model, "_id")->valueint;
    int modelIndex = 0;

    aJsonObject * oldModel = _root->child;

    while (oldModel && aJson.getObjectItem(oldModel, "_id")->valueint != id) {
      oldModel = oldModel->next;
      modelIndex++;
    }

    if (oldModel != model) {
      aJson.replaceItemInArray(_root, modelIndex, model);
    }
  }
  
  return model;
}

aJsonObject* MemoryStore::save(Stream * stream){
  aJsonStream aJsonStream(stream);
  return this->save(aJson.parse(&aJsonStream));
}

aJsonObject* MemoryStore::find(int id) {

  aJsonObject * model = _root->child;
  while (model && aJson.getObjectItem(model, "_id")->valueint != id) {
    model = model->next;
  }
  return model;
}

void MemoryStore::streamTo(Stream * stream, int id) {
  aJsonStream aJsonStream(stream);
  aJson.print(this->find(id), &aJsonStream);
}

void MemoryStore::remove(int id) {
  int modelIndex = 0;
  aJsonObject * model = _root->child;

  while (model) {

    if (aJson.getObjectItem(model, "_id")->valueint == id) {
       aJson.deleteItemFromArray(_root, modelIndex);
       break;
    }
    
    modelIndex++;
    model = model->next;
  }
}
