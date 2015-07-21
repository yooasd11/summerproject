// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: InGamePacket.proto

#ifndef PROTOBUF_InGamePacket_2eproto__INCLUDED
#define PROTOBUF_InGamePacket_2eproto__INCLUDED

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

namespace InGamePacket {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_InGamePacket_2eproto();
void protobuf_AssignDesc_InGamePacket_2eproto();
void protobuf_ShutdownFile_InGamePacket_2eproto();

class C_Move;
class S_Move;

// ===================================================================

class C_Move : public ::google::protobuf::Message {
 public:
  C_Move();
  virtual ~C_Move();

  C_Move(const C_Move& from);

  inline C_Move& operator=(const C_Move& from) {
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
  static const C_Move& default_instance();

  void Swap(C_Move* other);

  // implements Message ----------------------------------------------

  inline C_Move* New() const { return New(NULL); }

  C_Move* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const C_Move& from);
  void MergeFrom(const C_Move& from);
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
  void InternalSwap(C_Move* other);
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

  // required uint32 direction = 1;
  bool has_direction() const;
  void clear_direction();
  static const int kDirectionFieldNumber = 1;
  ::google::protobuf::uint32 direction() const;
  void set_direction(::google::protobuf::uint32 value);

  // required uint32 duration = 2;
  bool has_duration() const;
  void clear_duration();
  static const int kDurationFieldNumber = 2;
  ::google::protobuf::uint32 duration() const;
  void set_duration(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:InGamePacket.C_Move)
 private:
  inline void set_has_direction();
  inline void clear_has_direction();
  inline void set_has_duration();
  inline void clear_has_duration();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 direction_;
  ::google::protobuf::uint32 duration_;
  friend void  protobuf_AddDesc_InGamePacket_2eproto();
  friend void protobuf_AssignDesc_InGamePacket_2eproto();
  friend void protobuf_ShutdownFile_InGamePacket_2eproto();

  void InitAsDefaultInstance();
  static C_Move* default_instance_;
};
// -------------------------------------------------------------------

class S_Move : public ::google::protobuf::Message {
 public:
  S_Move();
  virtual ~S_Move();

  S_Move(const S_Move& from);

  inline S_Move& operator=(const S_Move& from) {
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
  static const S_Move& default_instance();

  void Swap(S_Move* other);

  // implements Message ----------------------------------------------

  inline S_Move* New() const { return New(NULL); }

  S_Move* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const S_Move& from);
  void MergeFrom(const S_Move& from);
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
  void InternalSwap(S_Move* other);
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

  // required uint32 uid = 1;
  bool has_uid() const;
  void clear_uid();
  static const int kUidFieldNumber = 1;
  ::google::protobuf::uint32 uid() const;
  void set_uid(::google::protobuf::uint32 value);

  // required uint32 x = 2;
  bool has_x() const;
  void clear_x();
  static const int kXFieldNumber = 2;
  ::google::protobuf::uint32 x() const;
  void set_x(::google::protobuf::uint32 value);

  // required uint32 y = 3;
  bool has_y() const;
  void clear_y();
  static const int kYFieldNumber = 3;
  ::google::protobuf::uint32 y() const;
  void set_y(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:InGamePacket.S_Move)
 private:
  inline void set_has_uid();
  inline void clear_has_uid();
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 uid_;
  ::google::protobuf::uint32 x_;
  ::google::protobuf::uint32 y_;
  friend void  protobuf_AddDesc_InGamePacket_2eproto();
  friend void protobuf_AssignDesc_InGamePacket_2eproto();
  friend void protobuf_ShutdownFile_InGamePacket_2eproto();

  void InitAsDefaultInstance();
  static S_Move* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// C_Move

// required uint32 direction = 1;
inline bool C_Move::has_direction() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void C_Move::set_has_direction() {
  _has_bits_[0] |= 0x00000001u;
}
inline void C_Move::clear_has_direction() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void C_Move::clear_direction() {
  direction_ = 0u;
  clear_has_direction();
}
inline ::google::protobuf::uint32 C_Move::direction() const {
  // @@protoc_insertion_point(field_get:InGamePacket.C_Move.direction)
  return direction_;
}
inline void C_Move::set_direction(::google::protobuf::uint32 value) {
  set_has_direction();
  direction_ = value;
  // @@protoc_insertion_point(field_set:InGamePacket.C_Move.direction)
}

// required uint32 duration = 2;
inline bool C_Move::has_duration() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void C_Move::set_has_duration() {
  _has_bits_[0] |= 0x00000002u;
}
inline void C_Move::clear_has_duration() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void C_Move::clear_duration() {
  duration_ = 0u;
  clear_has_duration();
}
inline ::google::protobuf::uint32 C_Move::duration() const {
  // @@protoc_insertion_point(field_get:InGamePacket.C_Move.duration)
  return duration_;
}
inline void C_Move::set_duration(::google::protobuf::uint32 value) {
  set_has_duration();
  duration_ = value;
  // @@protoc_insertion_point(field_set:InGamePacket.C_Move.duration)
}

// -------------------------------------------------------------------

// S_Move

// required uint32 uid = 1;
inline bool S_Move::has_uid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void S_Move::set_has_uid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void S_Move::clear_has_uid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void S_Move::clear_uid() {
  uid_ = 0u;
  clear_has_uid();
}
inline ::google::protobuf::uint32 S_Move::uid() const {
  // @@protoc_insertion_point(field_get:InGamePacket.S_Move.uid)
  return uid_;
}
inline void S_Move::set_uid(::google::protobuf::uint32 value) {
  set_has_uid();
  uid_ = value;
  // @@protoc_insertion_point(field_set:InGamePacket.S_Move.uid)
}

// required uint32 x = 2;
inline bool S_Move::has_x() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void S_Move::set_has_x() {
  _has_bits_[0] |= 0x00000002u;
}
inline void S_Move::clear_has_x() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void S_Move::clear_x() {
  x_ = 0u;
  clear_has_x();
}
inline ::google::protobuf::uint32 S_Move::x() const {
  // @@protoc_insertion_point(field_get:InGamePacket.S_Move.x)
  return x_;
}
inline void S_Move::set_x(::google::protobuf::uint32 value) {
  set_has_x();
  x_ = value;
  // @@protoc_insertion_point(field_set:InGamePacket.S_Move.x)
}

// required uint32 y = 3;
inline bool S_Move::has_y() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void S_Move::set_has_y() {
  _has_bits_[0] |= 0x00000004u;
}
inline void S_Move::clear_has_y() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void S_Move::clear_y() {
  y_ = 0u;
  clear_has_y();
}
inline ::google::protobuf::uint32 S_Move::y() const {
  // @@protoc_insertion_point(field_get:InGamePacket.S_Move.y)
  return y_;
}
inline void S_Move::set_y(::google::protobuf::uint32 value) {
  set_has_y();
  y_ = value;
  // @@protoc_insertion_point(field_set:InGamePacket.S_Move.y)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace InGamePacket

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_InGamePacket_2eproto__INCLUDED
