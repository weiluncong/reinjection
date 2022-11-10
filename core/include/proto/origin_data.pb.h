// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: origin_data.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_origin_5fdata_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_origin_5fdata_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3020000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3020001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_origin_5fdata_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_origin_5fdata_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_origin_5fdata_2eproto;
namespace OriginDataTrans {
class OriginCanData;
struct OriginCanDataDefaultTypeInternal;
extern OriginCanDataDefaultTypeInternal _OriginCanData_default_instance_;
class OriginUdpData;
struct OriginUdpDataDefaultTypeInternal;
extern OriginUdpDataDefaultTypeInternal _OriginUdpData_default_instance_;
}  // namespace OriginDataTrans
PROTOBUF_NAMESPACE_OPEN
template<> ::OriginDataTrans::OriginCanData* Arena::CreateMaybeMessage<::OriginDataTrans::OriginCanData>(Arena*);
template<> ::OriginDataTrans::OriginUdpData* Arena::CreateMaybeMessage<::OriginDataTrans::OriginUdpData>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace OriginDataTrans {

// ===================================================================

class OriginCanData final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:OriginDataTrans.OriginCanData) */ {
 public:
  inline OriginCanData() : OriginCanData(nullptr) {}
  ~OriginCanData() override;
  explicit PROTOBUF_CONSTEXPR OriginCanData(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  OriginCanData(const OriginCanData& from);
  OriginCanData(OriginCanData&& from) noexcept
    : OriginCanData() {
    *this = ::std::move(from);
  }

  inline OriginCanData& operator=(const OriginCanData& from) {
    CopyFrom(from);
    return *this;
  }
  inline OriginCanData& operator=(OriginCanData&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const OriginCanData& default_instance() {
    return *internal_default_instance();
  }
  static inline const OriginCanData* internal_default_instance() {
    return reinterpret_cast<const OriginCanData*>(
               &_OriginCanData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(OriginCanData& a, OriginCanData& b) {
    a.Swap(&b);
  }
  inline void Swap(OriginCanData* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(OriginCanData* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  OriginCanData* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<OriginCanData>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const OriginCanData& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const OriginCanData& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(OriginCanData* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "OriginDataTrans.OriginCanData";
  }
  protected:
  explicit OriginCanData(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kCanDataFieldNumber = 4,
    kIdFieldNumber = 1,
    kRawTimestampFieldNumber = 2,
    kTimeStampFieldNumber = 3,
    kDataLenFieldNumber = 5,
    kBusIdFieldNumber = 6,
  };
  // bytes can_data = 4;
  void clear_can_data();
  const std::string& can_data() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_can_data(ArgT0&& arg0, ArgT... args);
  std::string* mutable_can_data();
  PROTOBUF_NODISCARD std::string* release_can_data();
  void set_allocated_can_data(std::string* can_data);
  private:
  const std::string& _internal_can_data() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_can_data(const std::string& value);
  std::string* _internal_mutable_can_data();
  public:

  // int32 id = 1;
  void clear_id();
  int32_t id() const;
  void set_id(int32_t value);
  private:
  int32_t _internal_id() const;
  void _internal_set_id(int32_t value);
  public:

  // int32 raw_timestamp = 2;
  void clear_raw_timestamp();
  int32_t raw_timestamp() const;
  void set_raw_timestamp(int32_t value);
  private:
  int32_t _internal_raw_timestamp() const;
  void _internal_set_raw_timestamp(int32_t value);
  public:

  // double time_stamp = 3;
  void clear_time_stamp();
  double time_stamp() const;
  void set_time_stamp(double value);
  private:
  double _internal_time_stamp() const;
  void _internal_set_time_stamp(double value);
  public:

  // int32 data_len = 5;
  void clear_data_len();
  int32_t data_len() const;
  void set_data_len(int32_t value);
  private:
  int32_t _internal_data_len() const;
  void _internal_set_data_len(int32_t value);
  public:

  // int32 bus_id = 6;
  void clear_bus_id();
  int32_t bus_id() const;
  void set_bus_id(int32_t value);
  private:
  int32_t _internal_bus_id() const;
  void _internal_set_bus_id(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:OriginDataTrans.OriginCanData)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr can_data_;
  int32_t id_;
  int32_t raw_timestamp_;
  double time_stamp_;
  int32_t data_len_;
  int32_t bus_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_origin_5fdata_2eproto;
};
// -------------------------------------------------------------------

class OriginUdpData final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:OriginDataTrans.OriginUdpData) */ {
 public:
  inline OriginUdpData() : OriginUdpData(nullptr) {}
  ~OriginUdpData() override;
  explicit PROTOBUF_CONSTEXPR OriginUdpData(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  OriginUdpData(const OriginUdpData& from);
  OriginUdpData(OriginUdpData&& from) noexcept
    : OriginUdpData() {
    *this = ::std::move(from);
  }

  inline OriginUdpData& operator=(const OriginUdpData& from) {
    CopyFrom(from);
    return *this;
  }
  inline OriginUdpData& operator=(OriginUdpData&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const OriginUdpData& default_instance() {
    return *internal_default_instance();
  }
  static inline const OriginUdpData* internal_default_instance() {
    return reinterpret_cast<const OriginUdpData*>(
               &_OriginUdpData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(OriginUdpData& a, OriginUdpData& b) {
    a.Swap(&b);
  }
  inline void Swap(OriginUdpData* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(OriginUdpData* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  OriginUdpData* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<OriginUdpData>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const OriginUdpData& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const OriginUdpData& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(OriginUdpData* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "OriginDataTrans.OriginUdpData";
  }
  protected:
  explicit OriginUdpData(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kUdpDataFieldNumber = 3,
    kTimeStampFieldNumber = 2,
    kSensorTypeFieldNumber = 1,
  };
  // bytes udp_data = 3;
  void clear_udp_data();
  const std::string& udp_data() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_udp_data(ArgT0&& arg0, ArgT... args);
  std::string* mutable_udp_data();
  PROTOBUF_NODISCARD std::string* release_udp_data();
  void set_allocated_udp_data(std::string* udp_data);
  private:
  const std::string& _internal_udp_data() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_udp_data(const std::string& value);
  std::string* _internal_mutable_udp_data();
  public:

  // double time_stamp = 2;
  void clear_time_stamp();
  double time_stamp() const;
  void set_time_stamp(double value);
  private:
  double _internal_time_stamp() const;
  void _internal_set_time_stamp(double value);
  public:

  // int32 sensor_type = 1;
  void clear_sensor_type();
  int32_t sensor_type() const;
  void set_sensor_type(int32_t value);
  private:
  int32_t _internal_sensor_type() const;
  void _internal_set_sensor_type(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:OriginDataTrans.OriginUdpData)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr udp_data_;
  double time_stamp_;
  int32_t sensor_type_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_origin_5fdata_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// OriginCanData

// int32 id = 1;
inline void OriginCanData::clear_id() {
  id_ = 0;
}
inline int32_t OriginCanData::_internal_id() const {
  return id_;
}
inline int32_t OriginCanData::id() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginCanData.id)
  return _internal_id();
}
inline void OriginCanData::_internal_set_id(int32_t value) {
  
  id_ = value;
}
inline void OriginCanData::set_id(int32_t value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginCanData.id)
}

// int32 raw_timestamp = 2;
inline void OriginCanData::clear_raw_timestamp() {
  raw_timestamp_ = 0;
}
inline int32_t OriginCanData::_internal_raw_timestamp() const {
  return raw_timestamp_;
}
inline int32_t OriginCanData::raw_timestamp() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginCanData.raw_timestamp)
  return _internal_raw_timestamp();
}
inline void OriginCanData::_internal_set_raw_timestamp(int32_t value) {
  
  raw_timestamp_ = value;
}
inline void OriginCanData::set_raw_timestamp(int32_t value) {
  _internal_set_raw_timestamp(value);
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginCanData.raw_timestamp)
}

// double time_stamp = 3;
inline void OriginCanData::clear_time_stamp() {
  time_stamp_ = 0;
}
inline double OriginCanData::_internal_time_stamp() const {
  return time_stamp_;
}
inline double OriginCanData::time_stamp() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginCanData.time_stamp)
  return _internal_time_stamp();
}
inline void OriginCanData::_internal_set_time_stamp(double value) {
  
  time_stamp_ = value;
}
inline void OriginCanData::set_time_stamp(double value) {
  _internal_set_time_stamp(value);
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginCanData.time_stamp)
}

// bytes can_data = 4;
inline void OriginCanData::clear_can_data() {
  can_data_.ClearToEmpty();
}
inline const std::string& OriginCanData::can_data() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginCanData.can_data)
  return _internal_can_data();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void OriginCanData::set_can_data(ArgT0&& arg0, ArgT... args) {
 
 can_data_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginCanData.can_data)
}
inline std::string* OriginCanData::mutable_can_data() {
  std::string* _s = _internal_mutable_can_data();
  // @@protoc_insertion_point(field_mutable:OriginDataTrans.OriginCanData.can_data)
  return _s;
}
inline const std::string& OriginCanData::_internal_can_data() const {
  return can_data_.Get();
}
inline void OriginCanData::_internal_set_can_data(const std::string& value) {
  
  can_data_.Set(value, GetArenaForAllocation());
}
inline std::string* OriginCanData::_internal_mutable_can_data() {
  
  return can_data_.Mutable(GetArenaForAllocation());
}
inline std::string* OriginCanData::release_can_data() {
  // @@protoc_insertion_point(field_release:OriginDataTrans.OriginCanData.can_data)
  return can_data_.Release();
}
inline void OriginCanData::set_allocated_can_data(std::string* can_data) {
  if (can_data != nullptr) {
    
  } else {
    
  }
  can_data_.SetAllocated(can_data, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (can_data_.IsDefault()) {
    can_data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:OriginDataTrans.OriginCanData.can_data)
}

// int32 data_len = 5;
inline void OriginCanData::clear_data_len() {
  data_len_ = 0;
}
inline int32_t OriginCanData::_internal_data_len() const {
  return data_len_;
}
inline int32_t OriginCanData::data_len() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginCanData.data_len)
  return _internal_data_len();
}
inline void OriginCanData::_internal_set_data_len(int32_t value) {
  
  data_len_ = value;
}
inline void OriginCanData::set_data_len(int32_t value) {
  _internal_set_data_len(value);
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginCanData.data_len)
}

// int32 bus_id = 6;
inline void OriginCanData::clear_bus_id() {
  bus_id_ = 0;
}
inline int32_t OriginCanData::_internal_bus_id() const {
  return bus_id_;
}
inline int32_t OriginCanData::bus_id() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginCanData.bus_id)
  return _internal_bus_id();
}
inline void OriginCanData::_internal_set_bus_id(int32_t value) {
  
  bus_id_ = value;
}
inline void OriginCanData::set_bus_id(int32_t value) {
  _internal_set_bus_id(value);
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginCanData.bus_id)
}

// -------------------------------------------------------------------

// OriginUdpData

// int32 sensor_type = 1;
inline void OriginUdpData::clear_sensor_type() {
  sensor_type_ = 0;
}
inline int32_t OriginUdpData::_internal_sensor_type() const {
  return sensor_type_;
}
inline int32_t OriginUdpData::sensor_type() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginUdpData.sensor_type)
  return _internal_sensor_type();
}
inline void OriginUdpData::_internal_set_sensor_type(int32_t value) {
  
  sensor_type_ = value;
}
inline void OriginUdpData::set_sensor_type(int32_t value) {
  _internal_set_sensor_type(value);
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginUdpData.sensor_type)
}

// double time_stamp = 2;
inline void OriginUdpData::clear_time_stamp() {
  time_stamp_ = 0;
}
inline double OriginUdpData::_internal_time_stamp() const {
  return time_stamp_;
}
inline double OriginUdpData::time_stamp() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginUdpData.time_stamp)
  return _internal_time_stamp();
}
inline void OriginUdpData::_internal_set_time_stamp(double value) {
  
  time_stamp_ = value;
}
inline void OriginUdpData::set_time_stamp(double value) {
  _internal_set_time_stamp(value);
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginUdpData.time_stamp)
}

// bytes udp_data = 3;
inline void OriginUdpData::clear_udp_data() {
  udp_data_.ClearToEmpty();
}
inline const std::string& OriginUdpData::udp_data() const {
  // @@protoc_insertion_point(field_get:OriginDataTrans.OriginUdpData.udp_data)
  return _internal_udp_data();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void OriginUdpData::set_udp_data(ArgT0&& arg0, ArgT... args) {
 
 udp_data_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:OriginDataTrans.OriginUdpData.udp_data)
}
inline std::string* OriginUdpData::mutable_udp_data() {
  std::string* _s = _internal_mutable_udp_data();
  // @@protoc_insertion_point(field_mutable:OriginDataTrans.OriginUdpData.udp_data)
  return _s;
}
inline const std::string& OriginUdpData::_internal_udp_data() const {
  return udp_data_.Get();
}
inline void OriginUdpData::_internal_set_udp_data(const std::string& value) {
  
  udp_data_.Set(value, GetArenaForAllocation());
}
inline std::string* OriginUdpData::_internal_mutable_udp_data() {
  
  return udp_data_.Mutable(GetArenaForAllocation());
}
inline std::string* OriginUdpData::release_udp_data() {
  // @@protoc_insertion_point(field_release:OriginDataTrans.OriginUdpData.udp_data)
  return udp_data_.Release();
}
inline void OriginUdpData::set_allocated_udp_data(std::string* udp_data) {
  if (udp_data != nullptr) {
    
  } else {
    
  }
  udp_data_.SetAllocated(udp_data, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (udp_data_.IsDefault()) {
    udp_data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:OriginDataTrans.OriginUdpData.udp_data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace OriginDataTrans

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_origin_5fdata_2eproto
