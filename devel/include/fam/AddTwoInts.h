// Generated by gencpp from file fam/AddTwoInts.msg
// DO NOT EDIT!


#ifndef FAM_MESSAGE_ADDTWOINTS_H
#define FAM_MESSAGE_ADDTWOINTS_H

#include <ros/service_traits.h>


#include <fam/AddTwoIntsRequest.h>
#include <fam/AddTwoIntsResponse.h>


namespace fam
{

struct AddTwoInts
{

typedef AddTwoIntsRequest Request;
typedef AddTwoIntsResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct AddTwoInts
} // namespace fam


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::fam::AddTwoInts > {
  static const char* value()
  {
    return "6a2e34150c00229791cc89ff309fff21";
  }

  static const char* value(const ::fam::AddTwoInts&) { return value(); }
};

template<>
struct DataType< ::fam::AddTwoInts > {
  static const char* value()
  {
    return "fam/AddTwoInts";
  }

  static const char* value(const ::fam::AddTwoInts&) { return value(); }
};


// service_traits::MD5Sum< ::fam::AddTwoIntsRequest> should match 
// service_traits::MD5Sum< ::fam::AddTwoInts > 
template<>
struct MD5Sum< ::fam::AddTwoIntsRequest>
{
  static const char* value()
  {
    return MD5Sum< ::fam::AddTwoInts >::value();
  }
  static const char* value(const ::fam::AddTwoIntsRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::fam::AddTwoIntsRequest> should match 
// service_traits::DataType< ::fam::AddTwoInts > 
template<>
struct DataType< ::fam::AddTwoIntsRequest>
{
  static const char* value()
  {
    return DataType< ::fam::AddTwoInts >::value();
  }
  static const char* value(const ::fam::AddTwoIntsRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::fam::AddTwoIntsResponse> should match 
// service_traits::MD5Sum< ::fam::AddTwoInts > 
template<>
struct MD5Sum< ::fam::AddTwoIntsResponse>
{
  static const char* value()
  {
    return MD5Sum< ::fam::AddTwoInts >::value();
  }
  static const char* value(const ::fam::AddTwoIntsResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::fam::AddTwoIntsResponse> should match 
// service_traits::DataType< ::fam::AddTwoInts > 
template<>
struct DataType< ::fam::AddTwoIntsResponse>
{
  static const char* value()
  {
    return DataType< ::fam::AddTwoInts >::value();
  }
  static const char* value(const ::fam::AddTwoIntsResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // FAM_MESSAGE_ADDTWOINTS_H