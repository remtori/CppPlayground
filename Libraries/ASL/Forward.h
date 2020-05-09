#pragma once

namespace ASL {

class ByteBuffer;

template<typename>
class Function;

template<typename K, typename V, typename KeyTraits>
class HashMap;

template<typename T, typename TraitsForT>
class HashTable;

template<typename T>
class NonnullOwnPtr;

template<typename T>
class OwnPtr;

template<typename T>
class NonnullRefPtr;

template<typename T>
class RefPtr;

template<typename T>
class Vector;

template<typename T>
class Optional;

template<typename T>
class SinglyLinkedList;

class SharedString;
class StringImpl;
class String;
class StringBuilder;
class StringView;

class JsonValue;
class JsonObject;
class JsonArray;

} // namespace ASL
