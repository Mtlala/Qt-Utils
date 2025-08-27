# Qt-Utils

**QtUtils** is a lightweight utility library inspired by **Boost**, but implemented in a **Qt-style**.  
It provides additional data structures and algorithms designed to integrate seamlessly with Qt’s ecosystem (`QVector`, `QHash`, `QSharedPointer`, `QJsonObject`, etc.).

---

## Features

- **Qt-style API**: Follows Qt naming conventions, iterator patterns, and integrates well with Qt containers.  
- **Cross-platform**: Based on Qt, works on Windows / Linux / macOS.  
- **Extensible**: Core data structures are implemented; more will be added over time.  

---

## Implemented Data Structures

### QLinkedHash
- A hybrid of **hash table** and **linked list**.  
- Maintains **insertion order** while providing efficient hash lookup.  
- Similar to `QHash`, but supports ordered iteration.

### QCircularBuffer
- A fixed-capacity circular array (ring buffer).  
- Efficient FIFO queue operations.  
- Iterator support, works like a Qt container.

---

## Planned Structures

- `List`: Doubly-linked list.  
- `Tree`: Balanced trees (AVL / Red-Black / B-Tree / Interval tree).  
- `Graph`: Graph structure with common algorithms.  
- `Utils`: General-purpose algorithms and iterator adapters.  

---

## Examples

#### QLinkedHash
```cpp
#include "list/QLinkedHash.hpp"

QLinkedHash<QString, int> linkedHash;
linkedHash.append("key1", 100);
linkedHash.append("key2", 200);

for (auto it = linkedHash.begin(); it != linkedHash.end(); ++it) {
    qDebug() << *it;
}
```


#### QCircularBuffer
```cpp
#include "list/QCircularBuffer.hpp"

QCircularBuffer<int> buffer(5);
buffer.push_back(1);
buffer.push_back(2);
buffer.push_back(3);

for (size_t i = 0; i < buffer.size(); ++i) {
    qDebug() << buffer[i];
}
```
