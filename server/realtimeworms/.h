// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MovePacket.proto

#ifndef PROTOBUF_MovePacket_2eproto__INCLUDED
#define PROTOBUF_MovePacket_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace MovePacket {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_MovePacket_2eproto();
void protobuf_AssignDesc_MovePacket_2eproto();
void protobuf_ShutdownFile_MovePacket_2eproto();

class c_move;
class s_move;

// ===================================================================

class c_move : public ::google::protobuf::Message {
 public:
  c_move();
  virtual ~c_move();

  c_move(const c_move& from);

  inline c_move& operator=(const c_move& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const c_move& default_instance();

  void Swap(c_move* other);

  // implements Message ----------------------------------------------

  inline c_move* New() const { return New(NULL); }

  c_move* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const c_move& from);
  void MergeFrom(const c_move& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(c_move* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 direction = 1;
  bool has_direction() const;
  void clear_direction();
  static const int kDirectionFieldNumber = 1;
  ::google::protobuf::int32 direction() const;
  void set_direction(::google::protobuf::int32 value);

  // required int32 time = 2;
  bool has_time() const;
  void clear_time();
  static const int kTimeFieldNumber = 2;
  ::google::protobuf::int32 time() const;
  void set_time(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:MovePacket.c_move)
 private:
  inline void set_has_direction();
  inline void clear_has_direction();
  inline void set_has_time();
  inline void clear_has_time();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 direction_;
  ::google::protobuf::int32 time_;
  friend void  protobuf_AddDesc_MovePacket_2eproto();
  friend void protobuf_AssignDesc_MovePacket_2eproto();
  friend void protobuf_ShutdownFile_MovePacket_2eproto();

  void InitAsDefaultInstance();
  static c_move* default_instance_;
};
// -------------------------------------------------------------------

class s_move : public ::google::protobuf::Message {
 public:
  s_move();
  virtual ~s_move();

  s_move(const s_move& from);

  inline s_move& operator=(const s_move& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const s_move& default_instance();

  void Swap(s_move* other);

  // implements Message ----------------------------------------------

  inline s_move* New() const { return New(NULL); }

  s_move* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const s_move& from);
  void MergeFrom(const s_move& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(s_move* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 Id = 1;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 1;
  ::google::protobuf::int32 id() const;
  void set_id(::google::protobuf::int32 value);

  // required int32 x = 2;
  bool has_x() const;
  void clear_x();
  static const int kXFieldNumber = 2;
  ::google::protobuf::int32 x() const;
  void set_x(::google::protobuf::int32 value);

  // required int32 y = 3;
  bool has_y() const;
  void clear_y();
  static const int kYFieldNumber = 3;
  ::google::protobuf::int32 y() const;
  void set_y(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:MovePacket.s_move)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 x_;
  ::google::protobuf::int32 y_;
  friend void  protobuf_AddDesc_MovePacket_2eproto();
  friend void protobuf_AssignDesc_MovePacket_2eproto();
  friend void protobuf_ShutdownFile_MovePacket_2eproto();

  void InitAsDefaultInstance();
  static s_move* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// c_move

// required int32 direction = 1;
inline bool c_move::has_direction() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void c_move::set_has_direction() {
  _has_bits_[0] |= 0x00000001u;
}
inline void c_move::clear_has_direction() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void c_move::clear_direction() {
  direction_ = 0;
  clear_has_direction();
}
inline ::google::protobuf::int32 c_move::direction() const {
  // @@protoc_insertion_point(field_get:MovePacket.c_move.direction)
  return direction_;
}
inline void c_move::set_direction(::google::protobuf::int32 value) {
  set_has_direction();
  direction_ = value;
  // @@protoc_insertion_point(field_set:MovePacket.c_move.direction)
}

// required int32 time = 2;
inline bool c_move::has_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void c_move::set_has_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void c_move::clear_has_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void c_move::clear_time() {
  time_ = 0;
  clear_has_time();
}
inline ::google::protobuf::int32 c_move::time() const {
  // @@protoc_insertion_point(field_get:MovePacket.c_move.time)
  return time_;
}
inline void c_move::set_time(::google::protobuf::int32 value) {
  set_has_time();
  time_ = value;
  // @@protoc_insertion_point(field_set:MovePacket.c_move.time)
}

// -------------------------------------------------------------------

// s_move

// required int32 Id = 1;
inline bool s_move::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void s_move::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void s_move::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void s_move::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 s_move::id() const {
  // @@protoc_insertion_point(field_get:MovePacket.s_move.Id)
  return id_;
}
inline void s_move::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:MovePacket.s_move.Id)
}

// required int32 x = 2;
inline bool s_move::has_x() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void s_move::set_has_x() {
  _has_bits_[0] |= 0x00000002u;
}
inline void s_move::clear_has_x() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void s_move::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline ::google::protobuf::int32 s_move::x() const {
  // @@protoc_insertion_point(field_get:MovePacket.s_move.x)
  return x_;
}
inline void s_move::set_x(::google::protobuf::int32 value) {
  set_has_x();
  x_ = value;
  // @@protoc_insertion_point(field_set:MovePacket.s_move.x)
}

// required int32 y = 3;
inline bool s_move::has_y() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void s_move::set_has_y() {
  _has_bits_[0] |= 0x00000004u;
}
inline void s_move::clear_has_y() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void s_move::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline ::google::protobuf::int32 s_move::y() const {
  // @@protoc_insertion_point(field_get:MovePacket.s_move.y)
  return y_;
}
inline void s_move::set_y(::google::protobuf::int32 value) {
  set_has_y();
  y_ = value;
  // @@protoc_insertion_point(field_set:MovePacket.s_move.y)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace MovePacket

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_MovePacket_2eproto__INCLUDED