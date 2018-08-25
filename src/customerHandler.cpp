/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "customerHandler.hpp"

#include "callDTO.hpp"


// Constansts +
/*! Thrift protocol definition */
const std::string THRIFT_PROTO("thrift-cpp");
// Constansts -


// Functions +
/*!
 * Convert \ref AddressDTO to \ref Address
 *
 * \param address the result ; \ref Address
 * \param dto the \ref AddressDTO to convert
 */
void
fromDTO(Address& address, const AddressDTO& dto) {
  if(!dto.lines.empty()) {
    for(auto iter = dto.lines.cbegin() ; iter != dto.lines.cend() ; ++iter) {
      address.lines.push_back(*iter);
    }
  }
  address.zipCode = dto.zipCode;
  address.city = dto.city;
  address.country = dto.country;
}

/*!
 * Convert \ref PhoneDTO to \ref Phone
 *
 * \param phone the result ; \ref Phone
 * \param dto the \ref PhoneDTO to convert
 */
void
fromDTO(Phone& phone, const PhoneDTO& dto) {
  phone.type = static_cast<PhoneType>(static_cast<int>(dto.type));
  phone.number = dto.number;
}

/*!
 * Convert \ref PhoneDTO list to \ref Phone list
 *
 * \param phones the result ; \ref Phone list
 * \param dto the \ref PhoneDTO list to convert
 */
void
fromDTO(std::vector<Phone>& phones, const std::vector<PhoneDTO>& dto) {
  if(dto.empty()) {
    return;
  }
  for(auto iter = dto.cbegin() ; iter != dto.cend() ; ++iter) {
    Phone phone;
    fromDTO(phone, *iter);
    phones.push_back(phone);
  }
}

/*!
 * Convert \ref CustomerDTO to \ref Customer
 *
 * \param customer the result ; \ref Customer
 * \param dto the \ref CustomerDTO to convert
 */
void
fromDTO(Customer& customer, const CustomerDTO& dto) {
  customer.id = dto.id.toString();
  customer.firstName = dto.firstName;
  customer.lastName = dto.lastName;
  customer.birthDate = dto.birthDate; // \todo convert to int64_t
  customer.email = dto.email;
  fromDTO(customer.address, dto.address);
  fromDTO(customer.phones, dto.phones);
}

/*!
 * Convert \ref CustomerDTO list to \ref Customer list
 *
 * \param customer the result ; \ref Customer list
 * \param dto the \ref CustomerDTO list to convert
 */
void
fromDTO(std::vector<Customer>& customers, const std::vector<CustomerDTO>& dto) {
  if(dto.empty()) {
    return;
  }
  for(auto iter = dto.cbegin() ; iter != dto.cend() ; ++iter) {
    Customer cust;
    fromDTO(cust, *iter);
    customers.push_back(cust);
  }
}

/*!
 * Convert \ref Address to \ref AddressDTO
 *
 * \param address the result ; \ref AddressDTO
 * \param thrift the \ref Address to convert
 */
void
fromThrift(AddressDTO& address, const Address& thrift) {
  if(!thrift.lines.empty()) {
    for(auto iter = thrift.lines.cbegin() ; iter != thrift.lines.cend() ; ++iter) {
      address.lines.push_back(*iter);
    }
  }
  address.zipCode = thrift.zipCode;
  address.city = thrift.city;
  address.country = thrift.country;
}

/*!
 * Convert \ref Phone to \ref PhoneDTO
 *
 * \param phone the result ; \ref Phone
 * \param thrift the \ref Phone to convert
 */
void
fromThrift(PhoneDTO& phone, const Phone& thrift) {
  phone.type = static_cast<PhoneTypeDTO>(static_cast<int>(thrift.type));
  phone.number = thrift.number;
}

/*!
 * Convert \ref Phone list to \ref PhoneDTO list
 *
 * \param phones the result ; \ref PhoneDTO list
 * \param thrift the \ref Phone list to convert
 */
void
fromThrift(std::vector<PhoneDTO>& phones, const std::vector<Phone>& thrift) {
  if(thrift.empty()) {
    return;
  }
  for(auto iter = thrift.cbegin() ; iter != thrift.cend() ; ++iter) {
    PhoneDTO phone;
    fromThrift(phone, *iter);
    phones.push_back(phone);
  }
}

/*!
 * Convert \ref Customer to \ref CustomerDTO
 *
 * \param customer the result ; \ref CustomerDTO
 * \param thrift the \ref Customer to convert
 */
void
fromThrift(CustomerDTO& customer, const Customer& thrift) {
  if(thrift.__isset.id && thrift.id != "") {
    customer.id = thrift.id;
  }
  customer.firstName = thrift.firstName;
  customer.lastName = thrift.lastName;
  std::chrono::seconds secEpoch(thrift.birthDate / 1000); // Timestamp are sore in ms
  std::chrono::time_point<std::chrono::system_clock> timePoint(secEpoch);
  customer.birthDate = anch::date::Date(std::chrono::system_clock::to_time_t(timePoint));
  customer.email = thrift.email;
  fromThrift(customer.address, thrift.address);
  fromThrift(customer.phones, thrift.phones);
}
// Functions -

// Constructors +
CustomerHandler::CustomerHandler(CustomerDAO& dao): _dao(dao) {
  // Nothing to do
}
// Constructors -

// Destructor +
CustomerHandler::~CustomerHandler() {
  // Nothing to do
}
// Destructor -

// Methods +
void
CustomerHandler::listCustomers(std::vector<Customer>& response, const ListAllRequest& request) {
  static const std::string method("list");
  int32_t reqSeq = -1;
  if(request.__isset.header) {
    reqSeq = request.header.requestSeq;
  }
  CallDTO call = initializeCall(reqSeq, THRIFT_PROTO, method);
  try {
    std::vector<CustomerDTO> customers;
    _dao.listCustomers(customers);
    fromDTO(response, customers);
  } catch(const std::exception& e) {
    CustomerException error;
    error.code = ErrorCode::SERVER;
    error.message = e.what();
    throw error;
  } catch(...) {
    CustomerException error;
    error.code = ErrorCode::SERVER;
    error.message = "Unexpected error";
    throw error;
  }
  call.ok = true;
  registerCall(call);
}

void
CustomerHandler::get(Customer& response, const GetRequest& request) {
  static const std::string method("get");
  int32_t reqSeq = -1;
  if(request.__isset.header) {
    reqSeq = request.header.requestSeq;
  }
  CallDTO call = initializeCall(reqSeq, THRIFT_PROTO, method);
  if(!request.__isset.id) {
    registerCall(call);
    CustomerException error;
    error.code = ErrorCode::PARAMETER;
    error.message = "Customer identifier is not set";
    throw error;
  }
  try {
    CustomerDTO customer;
    _dao.getDetails(request.id, customer);
    fromDTO(response, customer);
  } catch(const std::bad_cast& e) {
    CustomerException error;
    error.code = ErrorCode::PARAMETER;
    error.message = std::string("Invalid UUID: ") + request.id;
    throw error;
  } catch(const std::exception& e) {
    CustomerException error;
    error.code = ErrorCode::SERVER;
    error.message = e.what();
    throw error;
  } catch(...) {
    CustomerException error;
    error.code = ErrorCode::SERVER;
    error.message = "Unexpected error";
    throw error;
  }
  call.ok = true;
  registerCall(call);
}

void
CustomerHandler::create(std::string& response, const CreateRequest& request) {
  static const std::string method("create");
  int32_t reqSeq = -1;
  if(request.__isset.header) {
    reqSeq = request.header.requestSeq;
  }
  CallDTO call = initializeCall(reqSeq, THRIFT_PROTO, method);
  const Customer& customer = request.customer;
  // Customer structure checks +
  if(!request.__isset.customer) {
    registerCall(call);
    CustomerException error;
    error.code = ErrorCode::PARAMETER;
    error.message = "Customer is not set";
    throw error;
  }
  if(!customer.__isset.firstName || !customer.__isset.lastName) {
    registerCall(call);
    CustomerException error;
    error.code = ErrorCode::PARAMETER;
    std::ostringstream oss;
    oss << "Customer first_name, last_name and brith_date has to be set: " << customer;
    error.message = oss.str();
    throw error;
  }
  // Customer structure checks -
  // Address structure checks +
  if(customer.__isset.address) {
    const Address& addr = customer.address;
    if(!addr.__isset.lines || !addr.__isset.zipCode || !addr.__isset.city || !addr.__isset.country) {
      registerCall(call);
      CustomerException error;
      error.code = ErrorCode::PARAMETER;
      std::ostringstream oss;
      oss << "Address lines[0], zipCode, city and country has to be set: " << customer;
      error.message = oss.str();
      throw error;
    }
  }
  // Address structure checks -
  CustomerDTO cust;
  fromThrift(cust, customer);
  try {
    response = _dao.create(cust);
  } catch(const std::exception& e) {
    CustomerException error;
    error.code = ErrorCode::SERVER;
    error.message = e.what();
    throw error;
  } catch(...) {
    CustomerException error;
    error.code = ErrorCode::SERVER;
    error.message = "Unexpected error";
    throw error;
  }
  call.ok = true;
  registerCall(call);
}

void
CustomerHandler::deleteAll() {
  _dao.deleteAll();
}
// Methods -
